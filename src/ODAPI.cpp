/******************************************************************************
 * $Id: ocpn_draw_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN General Drawing Plugin
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
 *   $EMAIL$                                                               *
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
 ***************************************************************************
 */
//#define _2_9_x_ // uncomment this to compile for 2.9.x

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ODAPI.h"
#include "ocpn_draw_pi.h"
#include "Boundary.h"
#include "BoundaryMan.h"
#include "BoundaryPoint.h"
#include "ODConfig.h"
#include "ODPath.h"
#include "ODPathPropertiesDialogImpl.h"
#include "PathAndPointManagerDialogImpl.h"
#include "ODPointPropertiesImpl.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "PointMan.h"
#include "TextPoint.h"

#include "ocpn_plugin.h"

ODAPI::ODAPI()
{
    // ctor
}

ODAPI::~ODAPI()
{
    // dtor
}

bool ODAPI::OD_FindPathByGUID( wxString sGUID, wxString *sName, wxString *sDescription )
{
    ODPath *l_path;
    l_path = g_pPathMan->FindPathByGUID( sGUID );
    if(l_path) {
        sDescription->append(l_path->m_PathDescription);
        sName->append(l_path->m_PathNameString);
        return true;
    } else
        return false;
    
}

bool ODAPI::OD_FindPointInAnyBoundary( FindPointInAnyBoundary_t *pFPIAB )
{
    int l_BoundaryType;
    int l_BoundaryState;
    
    if(pFPIAB->sBoundaryType == wxT("Exclusion")) l_BoundaryType = ID_BOUNDARY_EXCLUSION;
    else if(pFPIAB->sBoundaryType == wxT("Inclusion")) l_BoundaryType = ID_BOUNDARY_INCLUSION;
    else if(pFPIAB->sBoundaryType == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_ANY;
    else l_BoundaryType = ID_BOUNDARY_ANY;
    
    if(pFPIAB->sBoundaryState == wxT("Active")) l_BoundaryState = ID_PATH_STATE_ACTIVE;
    else if(pFPIAB->sBoundaryState == wxT("Inactive")) l_BoundaryState = ID_PATH_STATE_INACTIVE;
    else if(pFPIAB->sBoundaryState == wxT("Any")) l_BoundaryState = ID_PATH_STATE_ANY;
    else l_BoundaryState = ID_BOUNDARY_ANY;

    wxString l_sGUID = g_pBoundaryMan->FindPointInBoundary( pFPIAB->dLat, pFPIAB->dLon, l_BoundaryType, l_BoundaryState );
    if(l_sGUID.Len() > 0) {
        Boundary *l_boundary = (Boundary *)g_pBoundaryMan->FindPathByGUID( l_sGUID );
        pFPIAB->sName = l_boundary->m_PathNameString;
        pFPIAB->sDescription = l_boundary->m_PathDescription;
        pFPIAB->sBoundaryObjectType = wxT("Boundary");
    }
    else {
        l_sGUID = g_pBoundaryMan->FindPointInBoundaryPoint( pFPIAB->dLat, pFPIAB->dLon, l_BoundaryType );
        if(l_sGUID.Len() > 0) {
            BoundaryPoint *l_boundarypoint = (BoundaryPoint *)g_pODPointMan->FindODPointByGUID( l_sGUID );
            pFPIAB->sName = l_boundarypoint->m_ODPointName;
            pFPIAB->sDescription = l_boundarypoint->m_ODPointDescription;
            pFPIAB->sBoundaryObjectType = wxT("Boundary");
        }
        else
            return false;
    }
    return true;
}

bool ODAPI::OD_FindClosestBoundaryLineCrossing( FindClosestBoundaryLineCrossing_t *pFCBLC )
{
    int l_BoundaryType;
    int l_BoundaryState;
    
    if(pFCBLC->sBoundaryType == wxT("Exclusion")) l_BoundaryType = ID_BOUNDARY_EXCLUSION;
    else if(pFCBLC->sBoundaryType == wxT("Inclusion")) l_BoundaryType = ID_BOUNDARY_INCLUSION;
    else if(pFCBLC->sBoundaryType == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_ANY;
    else l_BoundaryType = ID_BOUNDARY_ANY;
    
    if(pFCBLC->sBoundaryState == wxT("Active")) l_BoundaryState = ID_PATH_STATE_ACTIVE;
    else if(pFCBLC->sBoundaryState == wxT("Inactive")) l_BoundaryState = ID_PATH_STATE_INACTIVE;
    else if(pFCBLC->sBoundaryState == wxT("Any")) l_BoundaryState = ID_PATH_STATE_ANY;
    else l_BoundaryState = ID_BOUNDARY_ANY;

    Boundary *l_boundary = g_pBoundaryMan->FindLineCrossingBoundaryPtr( pFCBLC->dStartLon, pFCBLC->dStartLat, pFCBLC->dEndLon, pFCBLC->dEndLat, &pFCBLC->dCrossingLon, &pFCBLC->dCrossingLat, &pFCBLC->dCrossingDistance, l_BoundaryType, l_BoundaryState );
    if(l_boundary != 0) {
        pFCBLC->sName = l_boundary->m_PathNameString;
        pFCBLC->sDescription = l_boundary->m_PathDescription;
        pFCBLC->sGUID = l_boundary->m_GUID;
        pFCBLC->sBoundaryObjectType = wxT("Boundary");
        return true;
    }

    // point state is meaningless for boundary test
    l_BoundaryState = ID_POINT_STATE_ANY;
    BoundaryPoint *l_op;
    l_op = g_pODPointMan->FindLineCrossingBoundaryPtr( pFCBLC->dStartLon, pFCBLC->dStartLat, pFCBLC->dEndLon, pFCBLC->dEndLat, l_BoundaryType, l_BoundaryState );
    if(l_op != 0) {
        pFCBLC->sName = l_op->GetName();
        pFCBLC->sDescription = l_op->GetDescription();
        pFCBLC->sGUID = l_op->m_GUID;
        pFCBLC->sBoundaryObjectType = wxT("BoundaryPoint");
        return true;
    }
    return false;
}

bool ODAPI::OD_FindFirstBoundaryLineCrossing( FindClosestBoundaryLineCrossing_t *pFCBLC )
{
    int l_BoundaryType;
    int l_BoundaryState;
    
    if(pFCBLC->sBoundaryType == wxT("Exclusion")) l_BoundaryType = ID_BOUNDARY_EXCLUSION;
    else if(pFCBLC->sBoundaryType == wxT("Inclusion")) l_BoundaryType = ID_BOUNDARY_INCLUSION;
    else if(pFCBLC->sBoundaryType == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_ANY;
    else l_BoundaryType = ID_BOUNDARY_ANY;
    
    if(pFCBLC->sBoundaryState == wxT("Active")) l_BoundaryState = ID_PATH_STATE_ACTIVE;
    else if(pFCBLC->sBoundaryState == wxT("Inactive")) l_BoundaryState = ID_PATH_STATE_INACTIVE;
    else if(pFCBLC->sBoundaryState == wxT("Any")) l_BoundaryState = ID_PATH_STATE_ANY;
    else l_BoundaryState = ID_BOUNDARY_ANY;
    
    Boundary *l_boundary = g_pBoundaryMan->FindLineCrossingBoundaryPtr( pFCBLC->dStartLon, pFCBLC->dStartLat, pFCBLC->dEndLon, pFCBLC->dEndLat, &pFCBLC->dCrossingLon, &pFCBLC->dCrossingLat, &pFCBLC->dCrossingDistance, l_BoundaryType, l_BoundaryState, true );
    if(l_boundary != 0) {
        pFCBLC->sName = l_boundary->m_PathNameString;
        pFCBLC->sDescription = l_boundary->m_PathDescription;
        pFCBLC->sGUID = l_boundary->m_GUID;
        pFCBLC->sBoundaryObjectType = wxT("Boundary");
        return true;
    }
    
    // point state is meaningless for boundary test - not true 27/02/2018
    // l_BoundaryState = ID_POINT_STATE_ANY;
    BoundaryPoint *l_op;
    l_op = g_pODPointMan->FindLineCrossingBoundaryPtr( pFCBLC->dStartLon, pFCBLC->dStartLat, pFCBLC->dEndLon, pFCBLC->dEndLat, l_BoundaryType, l_BoundaryState );
    if(l_op != 0) {
        pFCBLC->sName = l_op->GetName();
        pFCBLC->sDescription = l_op->GetDescription();
        pFCBLC->sGUID = l_op->m_GUID;
        pFCBLC->sBoundaryObjectType = wxT("BoundaryPoint");
        return true;
    }
    return false;
}

//bool ODAPI::OD_CreateBoundary(CreateBoundary_t* pCB, wxString *GUID)
//{
//    bool bRet = OD_CreateBoundary(pCB);
//    GUID->Append(*m_psGUID);
//    return bRet;
//}

bool ODAPI::OD_CreateBoundary(CreateBoundary_t* pCB)
{
    if(pCB->ODAPIVersionMajor != ODAPI_VERSION_MAJOR || pCB->ODAPIVersionMinor != ODAPI_VERSION_MINOR) {
        return false;
    }
    
    bool    l_bValidBoundary = true;
    bool    l_test = true;
    
    // validate boundary information
    if(pCB->type != EXCLUSION_BOUNDARY &&  pCB->type != INCLUSION_BOUNDARY && pCB->type != NEITHER_BOUNDARY) l_bValidBoundary = false;
    if(pCB->BoundaryPointsList.size() <= 1) l_bValidBoundary = false;
    
    // Create boundary
    Boundary *l_boundary;
    l_boundary = new Boundary();
    g_pBoundaryList->Append( l_boundary );
    g_pPathList->Append( l_boundary);
    
    l_boundary->m_PathNameString = pCB->name;
    switch (pCB->type) 
    {
        case EXCLUSION_BOUNDARY:
            l_boundary->m_bExclusionBoundary = true;
            l_boundary->m_bInclusionBoundary = false;
            break;
        case INCLUSION_BOUNDARY:
            l_boundary->m_bExclusionBoundary = false;
            l_boundary->m_bInclusionBoundary = true;
            break;
        case NEITHER_BOUNDARY:
            l_boundary->m_bExclusionBoundary = false;
            l_boundary->m_bInclusionBoundary = false;
            break;
    }
    l_boundary->m_bPathIsActive = true;
    l_boundary->SetVisible(pCB->visible);
    l_boundary->m_bODPointsVisible = true;
    if(!pCB->defaultLineColour)
        l_boundary->m_wxcActiveLineColour = pCB->lineColour;
    if(!pCB->defaultFillColour)
        l_boundary->m_wxcActiveFillColour = pCB->fillColour;
    l_boundary->m_bTemporary = pCB->temporary;
    
    std::list<CreateBoundaryPoint_t *>::iterator it = pCB->BoundaryPointsList.begin();
    while( it != pCB->BoundaryPointsList.end() ) {
        BoundaryPoint *l_pBP = new BoundaryPoint((*it)->lat, (*it)->lon, g_sODPointIconName, (*it)->name, wxEmptyString, false);
        l_boundary->AddPoint(l_pBP, false, true, true);
        switch ((*it)->type) 
        {
            case EXCLUSION_BOUNDARY:
                l_pBP->m_bExclusionBoundaryPoint = true;
                l_pBP->m_bInclusionBoundaryPoint = false;
                break;
            case INCLUSION_BOUNDARY:
                l_pBP->m_bExclusionBoundaryPoint = false;
                l_pBP->m_bInclusionBoundaryPoint = true;
                break;
            case NEITHER_BOUNDARY:
                l_pBP->m_bExclusionBoundaryPoint = false;
                l_pBP->m_bInclusionBoundaryPoint = false;
                break;
        }
        l_pBP->SetVisible((*it)->visible); 
        l_pBP->SetShowODPointRangeRings((*it)->ringsvisible);
        l_pBP->SetODPointRangeRingsNumber((*it)->ringsnumber);
        l_pBP->SetODPointRangeRingsStep((*it)->ringssteps);
        l_pBP->SetODPointRangeRingsStepUnits((*it)->ringsunits);
        if(!(*it)->defaultRingColour)
            l_pBP->SetODPointRangeRingsColour((*it)->ringscolour);
        std::list<HyperLinkList_t *>::iterator l_it = (*it)->BoundaryPointHyperLinkList.begin();
        while(l_it != (*it)->BoundaryPointHyperLinkList.end()) {
            l_pBP->AddURL((*l_it)->sLink, (*l_it)->sDescription);
            ++l_it;
        }
        
        ++it;
    }
    
    l_boundary->AddPoint(l_boundary->m_pODPointList->GetFirst()->GetData());
    l_boundary->SetPointVisibility();
    l_boundary->m_bIsBeingCreated = false;
    l_boundary->CreateColourSchemes();
    l_boundary->SetColourScheme();
    l_boundary->SetActiveColours();
    
    g_pODConfig->AddNewPath( l_boundary, -1 );
    g_pODSelect->DeleteAllSelectablePathSegments(l_boundary);
    g_pODSelect->DeleteAllSelectableODPoints(l_boundary);
    g_pODSelect->AddAllSelectablePathSegments(l_boundary);
    g_pODSelect->AddAllSelectableODPoints(l_boundary);
    
    l_boundary->RebuildGUIDList(); // ensure the GUID list is intact and good
    l_boundary->SetHiLite(0);
    if( g_pODPathPropDialog && ( g_pODPathPropDialog->IsShown() ) ) {
        g_pODPathPropDialog->SetPathAndUpdate( l_boundary, true );
    }
    
    if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
        g_pPathAndPointManagerDialog->UpdatePathListCtrl();
    pCB->GUID.Clear();
    pCB->GUID.Append(l_boundary->m_GUID);
    RequestRefresh(g_ocpn_draw_pi->m_parent_window);
    
    return true;
}

bool ODAPI::OD_DeleteBoundary(DeleteBoundary_t* pDB)
{
    if(pDB->ODAPIVersionMajor != ODAPI_VERSION_MAJOR || pDB->ODAPIVersionMinor != ODAPI_VERSION_MINOR) {
        return false;
    }
    
    Boundary *plB = (Boundary *)g_pBoundaryMan->FindPathByGUID(pDB->GUID);
    if(plB) {
        return g_pBoundaryMan->DeletePath(plB);
    } else
        return false;
    
}

bool ODAPI::OD_CreateBoundaryPoint(CreateBoundaryPoint_t* pCBP)
{
    if(pCBP->ODAPIVersionMajor != ODAPI_VERSION_MAJOR || pCBP->ODAPIVersionMinor != ODAPI_VERSION_MINOR) {
        return false;
    }

    bool    l_bValidBoundaryPoint = true;
    BoundaryPoint *l_pBP;
    
    if(pCBP->iconname.IsEmpty()) {
        l_pBP = new BoundaryPoint(pCBP->lat, pCBP->lon, g_sODPointIconName, pCBP->name, wxEmptyString, false);
    } else {
        l_pBP = new BoundaryPoint(pCBP->lat, pCBP->lon, pCBP->iconname, pCBP->name, wxEmptyString, false);
    }

    switch (pCBP->type) 
    {
        case EXCLUSION_BOUNDARY:
            l_pBP->m_bExclusionBoundaryPoint = true;
            l_pBP->m_bInclusionBoundaryPoint = false;
            break;
        case INCLUSION_BOUNDARY:
            l_pBP->m_bExclusionBoundaryPoint = false;
            l_pBP->m_bInclusionBoundaryPoint = true;
            break;
        case NEITHER_BOUNDARY:
            l_pBP->m_bExclusionBoundaryPoint = false;
            l_pBP->m_bInclusionBoundaryPoint = false;
            break;
    }
    l_pBP->SetVisible(pCBP->visible); 
    l_pBP->SetShowODPointRangeRings(pCBP->ringsvisible);
    l_pBP->SetODPointRangeRingsNumber(pCBP->ringsnumber);
    l_pBP->SetODPointRangeRingsStep(pCBP->ringssteps);
    l_pBP->SetODPointRangeRingsStepUnits(pCBP->ringsunits);
    l_pBP->SetODPointRangeRingsColour(pCBP->ringscolour);
    l_pBP->m_bIsolatedMark = true;
    l_pBP->m_bIsInBoundary = false;
    l_pBP->m_bIsInPath = false;
    std::list<HyperLinkList_t *>::iterator l_it = pCBP->BoundaryPointHyperLinkList.begin();
    while(l_it != pCBP->BoundaryPointHyperLinkList.end()) {
        l_pBP->AddURL((*l_it)->sLink, (*l_it)->sDescription);
        ++l_it;
    }
    
    g_pODPointMan->AddODPoint(l_pBP);
    g_pODSelect->AddSelectableODPoint(pCBP->lat, pCBP->lon, l_pBP);
    g_pODConfig->AddNewODPoint( l_pBP, -1 );

    RequestRefresh(g_ocpn_draw_pi->m_parent_window);
    return true;
}

bool ODAPI::OD_DeleteBoundaryPoint(DeleteBoundaryPoint_t* pDBP)
{
    if(pDBP->ODAPIVersionMajor != ODAPI_VERSION_MAJOR || pDBP->ODAPIVersionMinor != ODAPI_VERSION_MINOR) {
        return false;
    }
    
    BoundaryPoint *plBP = (BoundaryPoint *)g_pODPointMan->FindODPointByGUID(pDBP->GUID);
    if(plBP) {
        if(plBP->m_bIsInBoundary) {
            wxArrayPtrVoid *ppath_array = g_pPathMan->GetPathArrayContaining(plBP);
            if( ppath_array ) {
                for( unsigned int ip = 0; ip < ppath_array->GetCount(); ip++ ) {
                    ODPath *pp = (ODPath *) ppath_array->Item( ip );
                    if(pp->GetnPoints() <= 3)
                        g_pPathMan->DeletePath(pp);
                    else {
                        pp->DeletePoint( plBP );
                        if( g_pODPathPropDialog && g_pODPathPropDialog->IsShown() ) g_pODPathPropDialog->SetPathAndUpdate( pp, true );
                    }
                }
                return true;
            } else
                return false;
        } else {
            g_pODPointMan->DestroyODPoint(plBP, false);
            
            g_pODSelect->DeleteSelectablePoint( plBP, SELTYPE_ODPOINT );
            
            if( NULL != g_pODPointMan )
                g_pODPointMan->RemoveODPoint( plBP );
            if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
                g_pPathAndPointManagerDialog->UpdateODPointsListCtrl();
            
            if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
                g_pODPointPropDialog->ValidateMark();
            }
            
            plBP->m_bPtIsSelected = false;
            
            delete (BoundaryPoint *)plBP;
            
            return true;
        }
    }
    else return false;
}

bool ODAPI::OD_CreateTextPoint(CreateTextPoint_t* pCTP)
{
    if(pCTP->ODAPIVersionMajor != ODAPI_VERSION_MAJOR || pCTP->ODAPIVersionMinor != ODAPI_VERSION_MINOR) {
        return false;
    }
    
    bool    l_bValidTextPoint = true;
    TextPoint *l_pTP;
    
    wxString *lGUID;
    if(pCTP->GUID.length() > 0) lGUID = new wxString(pCTP->GUID);
    else lGUID = new wxString(wxEmptyString);
    
    if(pCTP->iconname.IsEmpty()) {
        l_pTP = new TextPoint(pCTP->lat, pCTP->lon, g_sTextPointIconName, pCTP->name, *lGUID, false);
    } else {
        l_pTP = new TextPoint(pCTP->lat, pCTP->lon, pCTP->iconname, pCTP->name, *lGUID, false);
    }
    
    l_pTP->m_ODPointName = pCTP->name;
    l_pTP->m_bShowName = pCTP->ShowName;
    l_pTP->m_TextPointText = pCTP->TextToDisplay;
    l_pTP->SetMarkDescription(pCTP->description);
    l_pTP->SetVisible(pCTP->Visible); 
    l_pTP->m_TextPointText = pCTP->TextToDisplay;
    if(pCTP->TextColour != _T("DEFAULT"))
        l_pTP->m_colourTextColour.Set(pCTP->TextColour);
    if(!pCTP->defaultFont)
        l_pTP->m_DisplayTextFont = pCTP->TextFont;
    if(pCTP->BackgroundColour != _T("DEFAULT"))
        l_pTP->m_colourTextBackgroundColour.Set(pCTP->BackgroundColour);
    if(pCTP->BackgroundTransparancy == TEXTPOINT_DISPLAY_TEXT_SHOW_DEFAULT) 
        l_pTP->m_iBackgroundTransparency = pCTP->BackgroundTransparancy;
    if(pCTP->TextPosition != TEXT_DEFAULT)
        l_pTP->m_iTextPosition = pCTP->TextPosition;
    if(pCTP->TextPointDisplayTextWhen != TEXTPOINT_DISPLAY_TEXT_SHOW_DEFAULT)
        l_pTP->m_iDisplayTextWhen = pCTP->TextPointDisplayTextWhen;
    if(!pCTP->defaultRangeRings) {
        l_pTP->SetShowODPointRangeRings(pCTP->ringsvisible);
        l_pTP->SetODPointRangeRingsNumber(pCTP->ringsnumber);
        l_pTP->SetODPointRangeRingsStep(pCTP->ringssteps);
        l_pTP->SetODPointRangeRingsStepUnits(pCTP->ringsunits);
        if(pCTP->ringscolour != _T("DEFAULT"))
            l_pTP->SetODPointRangeRingsColour(wxColour(pCTP->ringscolour));
    }
    l_pTP->m_bIsolatedMark = true;
    l_pTP->CreateColourSchemes();
    l_pTP->SetColourScheme();
    l_pTP->m_bTemporary = pCTP->temporary;
    std::list<HyperLinkList_t *>::iterator l_it = pCTP->TextPointHyperLinkList.begin();
    while(l_it != pCTP->TextPointHyperLinkList.end()) {
        l_pTP->AddURL((*l_it)->sLink, (*l_it)->sDescription);
        ++l_it;
    }
    
    g_pODPointMan->AddODPoint(l_pTP);
    g_pODSelect->AddSelectableODPoint(pCTP->lat, pCTP->lon, l_pTP);
    if(pCTP->temporary) {
        bool savestate = g_pODConfig->m_bSkipChangeSetUpdate;
        g_pODConfig->m_bSkipChangeSetUpdate = true;
        g_pODConfig->AddNewODPoint( l_pTP, -1 );
        g_pODConfig->m_bSkipChangeSetUpdate = savestate;
    } else
        g_pODConfig->AddNewODPoint( l_pTP, -1 );
    
    RequestRefresh(g_ocpn_draw_pi->m_parent_window);
    return true;
}

bool ODAPI::OD_DeleteTextPoint(DeleteTextPoint_t* pDTP)
{
    if(pDTP->ODAPIVersionMajor != ODAPI_VERSION_MAJOR || pDTP->ODAPIVersionMinor != ODAPI_VERSION_MINOR) {
        return false;
    }
    
    TextPoint *plTP = (TextPoint *)g_pODPointMan->FindODPointByGUID(pDTP->GUID);
    if(plTP) {
        g_pODPointMan->DestroyODPoint(plTP, false);
        
        g_pODSelect->DeleteSelectablePoint( plTP, SELTYPE_ODPOINT );
        
        if( NULL != g_pODPointMan )
            g_pODPointMan->RemoveODPoint( plTP );
        if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
            g_pPathAndPointManagerDialog->UpdateODPointsListCtrl();
        
        if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
            g_pODPointPropDialog->ValidateMark();
        }
        
        plTP->m_bPtIsSelected = false;
        
        delete (TextPoint *)plTP;
        
        return true;
    }
    else return false;
}

bool ODAPI::OD_AddPointIcon(AddPointIcon_t *pAPI)
{
    if(pAPI->ODAPIVersionMajor != ODAPI_VERSION_MAJOR || pAPI->ODAPIVersionMinor != ODAPI_VERSION_MINOR) {
        return false;
    }
    
    g_pODPointMan->ProcessIcon(pAPI->PointIcon, pAPI->PointIconName, pAPI->PointIconDescription);
    return true;
}

bool ODAPI::OD_DeletePointIcon(DeletePointIcon_t *pDPI)
{
    if(pDPI->ODAPIVersionMajor != ODAPI_VERSION_MAJOR || pDPI->ODAPIVersionMinor != ODAPI_VERSION_MINOR) {
        return false;
    }
    
    g_pODPointMan->RemoveIcon(pDPI->PointIconName);
    return true;
}
