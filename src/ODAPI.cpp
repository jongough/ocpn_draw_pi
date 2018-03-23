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
#include "ODPath.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "PointMan.h"
#include "TextPoint.h"

extern PathMan      *g_pPathMan;
extern BoundaryMan  *g_pBoundaryMan;
extern PointMan     *g_pODPointMan;
extern ODSelect     *g_pODSelect;
extern wxString      g_sODPointIconName;

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

bool ODAPI::OD_CreateBoundary(CreateBoundary_t* pCB)
{
    bool    l_bValidBoundary = true;
    bool    l_test = true;
    
    // validate boundary information
    if(pCB->type != EXCLUSION_BOUNDARY &&  pCB->type != INCLUSION_BOUNDARY && pCB->type != NEITHER_BOUNDARY) l_bValidBoundary = false;
    if(pCB->BoundaryPointsList.size() <= 1) l_bValidBoundary = false;
    wxString l_rgbLine = pCB->lineColour.SubString(1, pCB->lineColour.Length() - 2);
    wxColour l_Colour;
    l_test = l_Colour.Set(l_rgbLine);
    if(!l_test) l_bValidBoundary = false;
    wxString l_rgbFill = pCB->fillColour.SubString(1, pCB->fillColour.Length() - 2);
    l_test = l_Colour.Set(l_rgbFill);
    if(!l_test) l_bValidBoundary = false;
    
    // Create boundary
    Boundary *l_boundary;
    l_boundary = new Boundary();
    
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
    l_boundary->m_wxcActiveLineColour = pCB->lineColour;
    l_boundary->m_wxcActiveFillColour = pCB->fillColour;
    
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
        l_pBP->SetODPointRangeRingsColour((*it)->ringscolour);
        
        ++it;
    }
    
    l_boundary->AddPoint(l_boundary->m_pODPointList->GetFirst()->GetData());
    l_boundary->m_bIsBeingCreated = false;
    ODNavObjectChanges *l_ODNavObjectChanges = new ODNavObjectChanges();
    l_ODNavObjectChanges->InsertPathA(l_boundary);
    l_boundary = NULL;
    
    delete l_ODNavObjectChanges;
    
    return l_bValidBoundary;
}

bool ODAPI::OD_CreateBoundaryPoint(CreateBoundaryPoint_t* pCBP)
{
    // create boundary point
    bool    l_bValidBoundaryPoint = true;
    BoundaryPoint *l_pBP = new BoundaryPoint(pCBP->lat, pCBP->lon, g_sODPointIconName, pCBP->name, wxEmptyString, false);

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
    g_pODPointMan->AddODPoint(l_pBP);
    l_pBP->m_bIsolatedMark = true;
    g_pODSelect->AddSelectableODPoint(pCBP->lat, pCBP->lon, l_pBP);
    l_pBP->m_bIsInBoundary = false;
    l_pBP->m_bIsInPath = false;
    
    return l_bValidBoundaryPoint;
}

bool ODAPI::OD_CreateTextPoint(CreateTextPoint_t* pCTP)
{
    // create text point
    bool    l_bValidTextPoint = true;
    TextPoint *l_pTP = new TextPoint(pCTP->lat, pCTP->lon, g_sODPointIconName, pCTP->name, wxEmptyString, false);
    
    l_pTP->m_TextPointText = pCTP->TextToDisplay;
    l_pTP->SetMarkDescription(pCTP->description);
    l_pTP->SetVisible(pCTP->visible); 
    l_pTP->SetShowODPointRangeRings(pCTP->ringsvisible);
    l_pTP->SetODPointRangeRingsNumber(pCTP->ringsnumber);
    l_pTP->SetODPointRangeRingsStep(pCTP->ringssteps);
    l_pTP->SetODPointRangeRingsStepUnits(pCTP->ringsunits);
    l_pTP->SetODPointRangeRingsColour(pCTP->ringscolour);
    g_pODPointMan->AddODPoint(l_pTP);
    l_pTP->m_bIsolatedMark = true;
    g_pODSelect->AddSelectableODPoint(pCTP->lat, pCTP->lon, l_pTP);
    
    return l_bValidTextPoint;
}
