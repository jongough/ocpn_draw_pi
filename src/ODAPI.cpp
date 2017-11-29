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
#include "PathMan.h"
#include "PointMan.h"

extern PathMan      *g_pPathMan;
extern BoundaryMan  *g_pBoundaryMan;
extern PointMan     *g_pODPointMan;

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

    wxString l_sGUID = g_pBoundaryMan->FindLineCrossingBoundary( pFCBLC->dStartLon, pFCBLC->dStartLat, pFCBLC->dEndLon, pFCBLC->dEndLat, &pFCBLC->dCrossingLon, &pFCBLC->dCrossingLat, &pFCBLC->dCrossingDistance, l_BoundaryType, l_BoundaryState );
    if(l_sGUID.length() > 0) {
        Boundary *l_boundary = (Boundary *)g_pBoundaryMan->FindPathByGUID( l_sGUID );
        pFCBLC->sName = l_boundary->m_PathNameString;
        pFCBLC->sDescription = l_boundary->m_PathDescription;
        pFCBLC->sGUID = l_sGUID;
        pFCBLC->sBoundaryObjectType = wxT("Boundary");
        return true;
    }

    // point state is meaningless for boundary test
    l_BoundaryState = ID_POINT_STATE_ANY;

    l_sGUID = g_pODPointMan->FindLineCrossingBoundary( pFCBLC->dStartLon, pFCBLC->dStartLat, pFCBLC->dEndLon, pFCBLC->dEndLat, l_BoundaryType, l_BoundaryState );
    if(l_sGUID.length() > 0) {
        BoundaryPoint *l_boundary = dynamic_cast<BoundaryPoint *>(g_pODPointMan->FindODPointByGUID( l_sGUID ));
        assert (l_boundary != 0);
        pFCBLC->sName = l_boundary->GetName();
        pFCBLC->sDescription = l_boundary->GetDescription();
        pFCBLC->sGUID = l_sGUID;
        pFCBLC->sBoundaryObjectType = wxT("BoundaryPoint");
        return true;
    }
    return false;
}
