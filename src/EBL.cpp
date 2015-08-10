/***************************************************************************
 *
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2013 by David S. Register                               *
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

#include "EBL.h"
#include "ODdc.h"
#include "ocpn_draw_pi.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "EBLProp.h"

#include "ODConfig.h"

#ifdef __WXMSW__
#include "GL/gl.h"            // local copy for Windows
#include <GL/glu.h>
#else

#ifndef __OCPN__ANDROID__
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include "qopengl.h"                  // this gives us the qt runtime gles2.h
#include "GL/gl_private.h"
#endif

#endif

#ifdef ocpnUSE_GL
#include <wx/glcanvas.h>
#endif


#include <wx/graphics.h>

#include <wx/listimpl.cpp>
WX_DEFINE_LIST ( EBLList );

extern wxColour    g_colourEBLLineColour;
extern wxString    g_sEBLEndIconName;
extern wxString    g_sEBLStartIconName;
extern bool        g_bEBLFixedEndPosition;
extern int         g_EBLPersistenceType;
extern int         g_EBLLineWidth; 
extern int         g_EBLLineStyle;

extern ocpn_draw_pi *g_ocpn_draw_pi;
extern EBLList      *g_pEBLList;
extern ODSelect     *g_pODSelect;
extern PathMan      *g_pPathMan;
extern PlugIn_Position_Fix_Ex  g_pfFix;
extern wxString      g_sODPointIconName;
extern ODConfig     *g_pODConfig;
extern EBLProp      *g_pEBLPropDialog;

EBL::EBL() : Path()
{
    m_sTypeString = _T("EBL");
    m_wxcActiveLineColour = g_colourEBLLineColour;
    m_width = g_EBLLineWidth;
    m_style = g_EBLLineStyle;
    m_bFixedEndPosition = g_bEBLFixedEndPosition;
    SetPersistence( g_EBLPersistenceType );
}

EBL::~EBL()
{
    //dtor
}

void EBL::AddPoint( ODPoint *pNewPoint, bool b_rename_in_sequence, bool b_deferBoxCalc, bool b_isLoading )
{
    Path::AddPoint( pNewPoint, b_rename_in_sequence, b_deferBoxCalc, b_isLoading );
    
    if(pNewPoint->GetName() == wxT("Boat")) g_ocpn_draw_pi->m_pEBLBoatPoint = pNewPoint;
}

void EBL::MovePoint( double inc_lat, double inc_lon )
{
    wxODPointListNode *node = m_pODPointList->GetLast();
    ODPoint *bp = (ODPoint *)node->GetData();
    bp->m_lat -= inc_lat;
    bp->m_lon -= inc_lon;
}

void EBL::SetPersistence( int PersistenceType )
{
    m_PersistenceType = PersistenceType;
    if(PersistenceType == ID_EBL_NOT_PERSISTENT || PersistenceType == ID_EBL_PERSISTENT_CRASH)
        m_bTemporary = true;
    else
        m_bTemporary = false;
}

void EBL::CentreOnBoat( void )
{
    ODPoint *pNewPoint;
    if(!g_ocpn_draw_pi->m_pEBLBoatPoint) {
        pNewPoint = new ODPoint( g_pfFix.Lat, g_pfFix.Lon, g_sODPointIconName, wxS("Boat"), wxT("") );
        pNewPoint->SetNameShown( false );
        pNewPoint->SetTypeString( wxT("EBL Point"));
        pNewPoint->SetIconName( wxEmptyString );
        pNewPoint->ReLoadIcon();
        g_ocpn_draw_pi->m_pEBLBoatPoint = pNewPoint;
    } else 
        pNewPoint = g_ocpn_draw_pi->m_pEBLBoatPoint;
    
    InsertPointAfter( (ODPoint *)m_pODPointList->GetFirst()->GetData(), pNewPoint );
    RemovePoint( m_pODPointList->GetFirst()->GetData() );
    
    g_pODSelect->DeleteAllSelectablePathSegments( this );
    g_pODSelect->DeleteAllSelectableODPoints( this );
    g_pODSelect->AddAllSelectablePathSegments( this );
    g_pODSelect->AddAllSelectableODPoints( this );
    
    FinalizeForRendering();
    UpdateSegmentDistances();
    bool prev_bskip = g_pODConfig->m_bSkipChangeSetUpdate;
    g_pODConfig->m_bSkipChangeSetUpdate = false;
    if(m_PersistenceType == ID_EBL_PERSISTENT || m_PersistenceType == ID_EBL_PERSISTENT_CRASH)
        g_pODConfig->UpdatePath( this ); 
    g_pODConfig->m_bSkipChangeSetUpdate = prev_bskip;
    
    for( unsigned int ip = 0; ip < m_pODPointList->GetCount(); ip++ ) {
        Path *pp = (Path *) m_pODPointList->Item( ip );
        if( g_pPathMan->IsPathValid(pp) ) {
            pp->FinalizeForRendering();
            pp->UpdateSegmentDistances();
            pp->m_bIsBeingEdited = false;
            
            g_pODConfig->UpdatePath( pp );
            
            pp->SetHiLite( 0 );
        }
    }
    
    //    Update the PathProperties Dialog, if currently shown
    if( ( NULL != g_pEBLPropDialog ) && ( g_pEBLPropDialog->IsShown() ) ) {
        if( m_pODPointList ) {
            for( unsigned int ip = 0; ip < m_pODPointList->GetCount(); ip++ ) {
                Path *pp = (Path *) m_pODPointList->Item( ip );
                if( g_pPathMan->IsPathValid(pp) ) {
                    g_pEBLPropDialog->SetPathAndUpdate( pp, true );
                }
            }
        }
    }
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    
    return;
}

void EBL::CentreOnLatLon( double lat, double lon )
{
    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *fp = (ODPoint *)node->GetData();
    fp->m_lat = lat;
    fp->m_lon = lon;
    g_pODSelect->DeleteAllSelectableODPoints( this );
    g_pODSelect->DeleteAllSelectablePathSegments( this );
    
    g_pODSelect->AddAllSelectablePathSegments( this );
    g_pODSelect->AddAllSelectableODPoints( this );
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    return;
}

void EBL::RemovePoint( ODPoint *op, bool bRenamePoints )
{
    g_pODSelect->DeleteAllSelectableODPoints( this );
    g_pODSelect->DeleteAllSelectablePathSegments( this );
    
    m_pODPointList->DeleteObject( op );
    if( wxNOT_FOUND != ODPointGUIDList.Index( op->m_GUID ) ) ODPointGUIDList.Remove( op->m_GUID );
    
    // check all other routes to see if this point appears in any other route
    Path *pcontainer_path = g_pPathMan->FindPathContainingODPoint( op );
    
    if( pcontainer_path == NULL ) {
        op->m_bIsInPath = false;
        op->m_bDynamicName = false;
        op->m_bIsolatedMark = true;
        if(op == g_ocpn_draw_pi->m_pEBLBoatPoint)
            g_ocpn_draw_pi->m_pEBLBoatPoint = NULL;
        this->DeletePoint( op );
        if(op == g_ocpn_draw_pi->m_pEBLBoatPoint) g_ocpn_draw_pi->m_pEBLBoatPoint = NULL;
        RebuildGUIDList();
    } else
        m_nPoints -= 1;
    
    
}

ODPoint *EBL::InsertPointBefore( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    ODPoint *newpoint = Path::InsertPointBefore( pOP, lat, lon );
    newpoint->m_IconName = g_sEBLStartIconName;
    newpoint->m_sTypeString = wxT("EBL Point");
    newpoint->m_MarkName = wxT("Start");
    FinalizeForRendering();
    return newpoint;
}

ODPoint *EBL::InsertPointAfter( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    ODPoint *newpoint = Path::InsertPointAfter( pOP, lat, lon );
    newpoint->m_IconName = g_sEBLStartIconName;
    newpoint->m_sTypeString = wxT("EBL Point");
    newpoint->m_MarkName = wxT("Start");
    ReloadPathPointIcons();
    FinalizeForRendering();
    return newpoint;
}

void EBL::InsertPointAfter( ODPoint *pOP, ODPoint *pnOP, bool bRenamePoints )
{
    Path::InsertPointAfter( pOP, pnOP );
}