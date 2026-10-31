/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Path Manager
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 **************************************************************************/

#include "PathMan.h"

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "wx/image.h"
#include "wx/tokenzr.h"
#include <wx/progdlg.h>

//#include "dychart.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <wx/listimpl.cpp>

#include "ocpn_draw_pi.h"
#include "ODSelect.h"
#include "ODConfig.h"
#include "Boundary.h"
#include "EBL.h"
#include "DR.h"
#include "GZ.h"
#include "PIL.h"

#include "georef.h"
//#include "cutil.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/apptrait.h>

//    List definitions for Point Manager Icons
WX_DECLARE_LIST(wxBitmap, markicon_bitmap_list_type);
WX_DECLARE_LIST(wxString, markicon_key_list_type);
WX_DECLARE_LIST(wxString, markicon_description_list_type);

//    List implementation for Point Manager Icons
#include <wx/listimpl.cpp>
WX_DEFINE_LIST(markicon_bitmap_list_type);
WX_DEFINE_LIST(markicon_key_list_type);
WX_DEFINE_LIST(markicon_description_list_type);

//--------------------------------------------------------------------------------
//      Pathman   "Path Manager"
//--------------------------------------------------------------------------------

PathMan::PathMan()
{
    pActivePath = NULL;
    pActivePoint = NULL;
    pPathActivatePoint = NULL;
}

PathMan::~PathMan()
{
    if( pPathActivatePoint ) delete pPathActivatePoint;
}

bool PathMan::IsPathValid( ODPath *pPath )
{
    for(std::list<ODPath *>::iterator it = g_PathList.m_pODPathList.begin(); it != g_PathList.m_pODPathList.end(); ++it) {
        if( pPath == *it ) return true;
    }
    return false;
}

bool PathMan::ActivatePath(ODPath *pPathToActivate )
{
    pPathToActivate->m_bPathIsActive = true;
    pPathToActivate->SetActiveColours();

    return true;
}

bool PathMan::DeactivatePath( ODPath *pPathToDeactivate )
{
    pPathToDeactivate->m_bPathIsActive = false;
    pPathToDeactivate->SetActiveColours();

    return true;
}

bool PathMan::DeletePath( ODPath *pPath )
{
    if( pPath ) {

        if( pPath->m_bIsInLayer )
            return false;
            
        g_pODConfig->DeleteConfigPath( pPath );

        //    Remove the path from associated lists
        g_pODSelect->DeleteAllSelectablePathSegments( pPath );
        g_PathList.m_pODPathList.remove( pPath );
        if(pPath->m_sTypeString == wxT("Boundary")) g_BoundaryList.m_pBoundaryList.remove( (Boundary *)pPath );
        if(pPath->m_sTypeString == wxT("EBL")) g_EBLList.m_pEBLList.remove( (EBL *)pPath );
        if(pPath->m_sTypeString == wxT("DR")) g_DRList.m_pDRList.remove( (DR *)pPath );
        if(pPath->m_sTypeString == wxT("Guard Zone")) g_GZList.m_pGZList.remove( (GZ *)pPath );
        if(pPath->m_sTypeString == wxT("PIL")) g_PILList.m_pPILList.remove( (PIL *)pPath );

        // walk the path, tentatively deleting/marking points used only by this path
        std::list<ODPoint *>::iterator it1;
        for(std::list<ODPoint *>::iterator it = pPath->m_ODPointList.m_pODPoint.begin(); it != pPath->m_ODPointList.m_pODPoint.end(); ++it) {
            ODPoint *podp = *it;
            it1 = it;
            ++it1;

            // check all other paths to see if this point appears in any other path
            ODPath *pcontainer_path = FindPathContainingODPoint( podp );

            if( pcontainer_path == NULL && podp->m_bIsInPath ) {
                podp->m_bIsInPath = false;          // Take this point out of this (and only) path
                if( !podp->m_bKeepXPath ) {
//    This does not need to be done with navobj.xml storage, since the ODPoints are stored with the path
//                              g_pODConfig->DeleteODPoint(podp);

                    g_pODSelect->DeleteSelectablePoint( podp, SELTYPE_ODPOINT );

                    // Remove all instances of this point from the list.
                    pPath->m_ODPointList.m_pODPoint.remove(podp);

                    if(podp->m_ODPointName == wxT("Boat") && pPath->m_sTypeString == wxT("EBL") ) g_ocpn_draw_pi->m_pEBLBoatPoint = NULL;
                    delete podp;
                } else {
                    podp->m_bDynamicName = false;
                    podp->m_bIsolatedMark = true;        // This has become an isolated mark
                    podp->m_bKeepXPath = false;         // and is no longer part of a Boundary
                }

            }
            it = it1;
        }

        delete pPath;

    }
    return true;
}

bool PathMan::DoesPathContainSharedPoints( ODPath *pPath )
{
    if( pPath ) {
        // walk the path, looking at each point to see if it is used by another path
        // or is isolated
        for(std::list<ODPoint *>::iterator it = pPath->m_ODPointList.m_pODPoint.begin(); it != pPath->m_ODPointList.m_pODPoint.end(); ++it) {
            ODPoint *podp = *it;

            // check all other paths to see if this point appears in any other path
            wxArrayPtrVoid *pRA = GetPathArrayContaining( podp );
            
             if( pRA ) {
                 for( unsigned int ir = 0; ir < pRA->GetCount(); ir++ ) {
                    ODPath *pb = (ODPath *) pRA->Item( ir );
                    if( pb == pPath)
                        continue;               // self
                    else  {
                        delete pRA;
                        return true;
                    }
                }
                delete pRA;
            }
        }
        
        //      Now walk the path again, looking for isolated type shared ODPoints
        for(std::list<ODPoint *>::iterator it = pPath->m_ODPointList.m_pODPoint.begin(); it != pPath->m_ODPointList.m_pODPoint.end(); ++it) {
            ODPoint *podp = *it;
            if( podp->m_bKeepXPath == true )
                return true;
        }
    }
    
    return false;
}
  
wxArrayPtrVoid *PathMan::GetPathArrayContaining( ODPoint *pODP )
{
    wxArrayPtrVoid *pArray = new wxArrayPtrVoid;

    for(std::list<ODPath *>::iterator it = g_PathList.m_pODPathList.begin(); it != g_PathList.m_pODPathList.end(); ++it) {
        ODPath *ppath = *it;

        for(std::list<ODPoint *>::iterator it1 = ppath->m_ODPointList.m_pODPoint.begin(); it1 != ppath->m_ODPointList.m_pODPoint.end(); ++it1) {
            ODPoint *podp = *it1;
            if( podp == pODP )                // success
            pArray->Add( (void *) ppath );
        }
    }

    if( pArray->GetCount() ) return pArray;

    else {
        delete pArray;
        return NULL;
    }
}

void PathMan::DeleteAllPaths( void )
{
    //    Iterate on the pathList
    if(g_PathList.m_pODPathList.size() > 0) {
        for(std::list<ODPath *>::iterator it = g_PathList.m_pODPathList.begin(); it != g_PathList.m_pODPathList.end(); ++it) {
            if(g_PathList.m_pODPathList.size() > 0) return;
            ODPath *ppath = *it;
            if( ppath->m_bIsInLayer ) {
                continue;
            }

            DeletePath( ppath );
        }
    }


}

ODPath *PathMan::FindPathContainingODPoint( ODPoint *pODP )
{
    for(std::list<ODPath *>::iterator it = g_PathList.m_pODPathList.begin(); it != g_PathList.m_pODPathList.end(); ++it) {
        ODPath *ppath = *it;

        for(std::list<ODPoint *>::iterator it1 = ppath->m_ODPointList.m_pODPoint.begin(); it1 !=ppath->m_ODPointList.m_pODPoint.end(); ++it1) {
            ODPoint *podp = *it1;
            if( podp == pODP )  return ppath;
        }
    }

    return NULL;                              // not found
}

wxArrayString *PathMan::FindAllPathsGUIDS ( wxString GUIDType, bool InLayer )
{
  wxArrayString *GUIDArray = new wxArrayString();
  for(std::list<ODPath *>::iterator it = g_PathList.m_pODPathList.begin(); it != g_PathList.m_pODPathList.end(); ++it) {
    ODPath *ppath = *it;
    if(GUIDType == "" || ppath->m_sTypeString == GUIDType) {
      if(InLayer == ppath->m_bIsInLayer){
        GUIDArray->Add( ppath->m_GUID );
      }
    }
  }
    return GUIDArray;
}

ODPath *PathMan::FindPathByGUID( wxString guid )
{
    for(std::list<ODPath *>::iterator it = g_PathList.m_pODPathList.begin(); it != g_PathList.m_pODPathList.end(); ++it) {
        ODPath *ppath = *it;
        if(ppath->m_GUID == guid) return ppath;
    }
    
    return NULL;                              // not found
}

void PathMan::SetColorScheme( PI_ColorScheme cs )
{
    //    Iterate on the pathList
    for(std::list<ODPath *>::iterator it = g_PathList.m_pODPathList.begin(); it != g_PathList.m_pODPathList.end(); ++it) {
        ODPath *ppath = *it;
        ppath->SetColourScheme(cs);
    }


    // Re-Create the pens and colors

    m_pActiveODPointPen = wxThePenList->FindOrCreatePen( wxColour( 0, 0, 255 ), g_path_line_width, wxPENSTYLE_SOLID );
    m_pODPointPen = wxThePenList->FindOrCreatePen( wxColour( 0, 0, 255 ), g_path_line_width, wxPENSTYLE_SOLID );

//    Or in something like S-52 compliance
    wxColour tColour;
    GetGlobalColor( wxS("UINFB"), &tColour );
    m_pPathPen = wxThePenList->FindOrCreatePen( tColour, g_path_line_width, wxPENSTYLE_SOLID );
    m_pPathBrush = wxTheBrushList->FindOrCreateBrush( tColour, wxBRUSHSTYLE_SOLID );

    GetGlobalColor( wxS("UINFO"), &tColour );
    m_pSelectedPathPen = wxThePenList->FindOrCreatePen( tColour, g_path_line_width, wxPENSTYLE_SOLID );
    m_pSelectedPathBrush = wxTheBrushList->FindOrCreateBrush( tColour, wxBRUSHSTYLE_SOLID );

    GetGlobalColor( wxS("UARTE"), &tColour );
    m_pActivePathPen = wxThePenList->FindOrCreatePen( tColour, g_path_line_width, wxPENSTYLE_SOLID );

    GetGlobalColor( wxS("PLRTE"), &tColour );
    m_pActivePathBrush = wxTheBrushList->FindOrCreateBrush( tColour, wxBRUSHSTYLE_SOLID );
    m_pActiveODPointBrush =  wxTheBrushList->FindOrCreateBrush( tColour, wxBRUSHSTYLE_SOLID);

    m_pActiveODPointPen = wxThePenList->FindOrCreatePen( tColour, g_path_line_width, wxPENSTYLE_SOLID );
    GetGlobalColor( wxS("CHBLK"), &tColour );
    m_pODPointPen = wxThePenList->FindOrCreatePen( tColour, g_path_line_width, wxPENSTYLE_SOLID );
    m_pODPointBrush = wxTheBrushList->FindOrCreateBrush( tColour, wxBRUSHSTYLE_SOLID);

}

