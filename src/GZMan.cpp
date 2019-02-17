/***************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  GZ Manager
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


#include "GZMan.h"
#include "GZ.h"
#include "ODPoint.h"
#include "PointMan.h"
#include "ocpn_draw_pi.h"
#include "ODUtils.h"

#ifdef __WXOSX__
#include <math.h>
#else
#include "math.h"
#endif

wxString GZMan::FindPointInGZ( double lat, double lon, int type, int state )
{
    wxGZListNode *GZ_node = g_pGZList->GetFirst();
    GZ *pGZ = NULL;
    bool bInPoly = false;
    ODPoint *pop;
    int k = 0;
    wxString l_GUID = wxEmptyString;
    
    while( GZ_node ) {
        pGZ = GZ_node->GetData();

        bool l_bOK = false;
        switch (state) {
            case ID_PATH_STATE_ANY:
                l_bOK = true;
                break;
            case ID_PATH_STATE_ACTIVE:
                if(pGZ->IsActive()) l_bOK = true;
                else l_bOK = false;
                break;
            case ID_PATH_STATE_INACTIVE:
                if(!pGZ->IsActive()) l_bOK = true;
                else l_bOK = false;
                break;
        }
        
        if(l_bOK) {
            switch (type) {
                case ID_BOUNDARY_ANY:
                    l_bOK = true;
                    break;
                case ID_BOUNDARY_EXCLUSION:
                    if(pGZ->m_bExclusionGZ) l_bOK = true;
                    else l_bOK = false;
                    break;
                case ID_BOUNDARY_INCLUSION:
                    if(pGZ->m_bInclusionGZ) l_bOK = true;
                    else l_bOK = false;
                    break;
                case ID_BOUNDARY_NEITHER:
                    if(pGZ->m_bExclusionGZ || pGZ->m_bInclusionGZ) l_bOK = false;
                    else l_bOK = true;
                    break;
            }
        }
        if(l_bOK) {
            k++;
            int i, j;
            j = pGZ->m_pODPointList->GetCount();
            double *polyX;
            double *polyY;
            polyX = new double[j];
            polyY = new double[j];
            
            wxODPointListNode *OCPNpoint_node = ( pGZ->m_pODPointList )->GetFirst();
            wxODPointListNode *OCPNpoint_last_node = ( pGZ->m_pODPointList )->GetLast();
            i = 0;
            while( OCPNpoint_node ) {
                pop = OCPNpoint_node->GetData();
                polyX[i] = pop->m_lon;
                polyY[i] = pop->m_lat;
                i++;
                OCPNpoint_node = OCPNpoint_node->GetNext();           // next OD point
                if(OCPNpoint_node == OCPNpoint_last_node) break;
            }
            bInPoly = pointInPolygon(i, polyX, polyY, lon, lat);
            delete [] polyX;
            delete [] polyY;
            if(bInPoly) {
                l_GUID = pGZ->m_GUID;
                break;
            }
        }
        GZ_node = GZ_node->GetNext();                         // next GZ
    }
    
    if(bInPoly) return l_GUID;
    else return wxT("");
}

bool GZMan::FindPointInGZ( GZ *pGZ, double lat, double lon, int type, int state )
{
    bool bInPoly = false;
    wxPoint *points;
    wxPoint l_l1p1;
    wxPoint l_l1p2;
    wxPoint l_l2p1;
    wxPoint l_l2p2;
    wxPoint l_pCentre;
    wxPoint l_xy;
    
    bool l_bOK = false;
    switch (state) {
        case ID_PATH_STATE_ANY:
            l_bOK = true;
            break;
        case ID_PATH_STATE_ACTIVE:
            if(pGZ->IsActive()) l_bOK = true;
            else l_bOK = false;
            break;
        case ID_PATH_STATE_INACTIVE:
            if(!pGZ->IsActive()) l_bOK = true;
            else l_bOK = false;
            break;
    }
    
    if(l_bOK) {
        switch (type) {
            case ID_BOUNDARY_ANY:
                l_bOK = true;
                break;
            case ID_BOUNDARY_EXCLUSION:
                if(pGZ->m_bExclusionGZ) l_bOK = true;
                else l_bOK = false;
                break;
            case ID_BOUNDARY_INCLUSION:
                if(pGZ->m_bInclusionGZ) l_bOK = true;
                else l_bOK = false;
                break;
            case ID_BOUNDARY_NEITHER:
                if(pGZ->m_bExclusionGZ || pGZ->m_bInclusionGZ) l_bOK = false;
                else l_bOK = true;
                break;
        }
    }
    if(!l_bOK) return false;
    
    pGZ->GetLatLonPoints( g_VP, &l_pCentre, &l_l1p1, &l_l1p2, &l_l2p1, &l_l2p2 );
    
    int numpoints = ArcSectorPoints( *&points, l_pCentre.x, l_pCentre.y, l_l1p1.x, l_l1p1.y, l_l1p2.x, l_l1p2.y, l_l2p2.x, l_l2p2.y, l_l2p1.x, l_l2p1.y, true);
    double *polyX;
    double *polyY;
    polyX = new double[numpoints];
    polyY = new double[numpoints];
    for(int i = 0; i < numpoints; i++) {
        polyX[i] = points[i].x;
        polyY[i] = points[i].y;
    }
    GetCanvasPixLL( &g_VP, &l_xy, lat, lon );
    bInPoly = pointInPolygon(numpoints, polyX, polyY, l_xy.x, l_xy.y  );

    delete [] polyX;
    delete [] polyY;
    delete [] points;

    return bInPoly;
}

bool GZMan::FindPointInGZ( wxString l_GUID, double lat, double lon, int type, int state )
{
    bool bInPoly = false;
    bool bGZFound = false;
    int i;
    double *polyX = NULL;
    double *polyY = NULL;
    GZ *pGZ = NULL;
    
    wxGZListNode *pGZ_node = g_pGZList->GetFirst();
    while( pGZ_node ) {
        pGZ = (GZ *)pGZ_node->GetData();
        if(pGZ->m_GUID == l_GUID) {
            bool l_bOK = false;
            switch (state) {
                case ID_PATH_STATE_ANY:
                    l_bOK = true;
                    break;
                case ID_PATH_STATE_ACTIVE:
                    if(pGZ->IsActive()) l_bOK = true;
                    else l_bOK = false;
                    break;
                case ID_PATH_STATE_INACTIVE:
                    if(!pGZ->IsActive()) l_bOK = true;
                    else l_bOK = false;
                    break;
            }
            
            if(l_bOK) {
                switch (type) {
                    case ID_BOUNDARY_ANY:
                        l_bOK = true;
                        break;
                    case ID_BOUNDARY_EXCLUSION:
                        if(pGZ->m_bExclusionGZ) l_bOK = true;
                        else l_bOK = false;
                        break;
                    case ID_BOUNDARY_INCLUSION:
                        if(pGZ->m_bInclusionGZ) l_bOK = true;
                        else l_bOK = false;
                        break;
                    case ID_BOUNDARY_NEITHER:
                        if(pGZ->m_bExclusionGZ || pGZ->m_bInclusionGZ) l_bOK = false;
                        else l_bOK = true;
                        break;
                }
            }
            if(l_bOK)
                bGZFound = true;
            break;
        }
        pGZ = (GZ *)pGZ_node->GetNext();
    }
    if(pGZ && bGZFound) {
        polyX = new double[pGZ->m_pODPointList->GetCount()];
        polyY = new double[pGZ->m_pODPointList->GetCount()];
        wxODPointListNode *OCPNpoint_node = ( pGZ->m_pODPointList )->GetFirst();
        wxODPointListNode *OCPNpoint_last_node = ( pGZ->m_pODPointList )->GetLast();
        i = 0;
        while( OCPNpoint_node ) {
            ODPoint *pop = OCPNpoint_node->GetData();
            polyX[i] = pop->m_lon;
            polyY[i] = pop->m_lat;
            i++;
            OCPNpoint_node = OCPNpoint_node->GetNext();           // next OD point
            if(OCPNpoint_node == OCPNpoint_last_node) break;
        }
        bInPoly = pointInPolygon(i, polyX, polyY, lon, lat);
    } 
    
    if(polyX) delete [] polyX;
    if(polyY) delete [] polyY;
    
    return bInPoly;
}

wxString GZMan::FindLineCrossingGZ( double StartLon, double StartLat, double EndLon, double EndLat, double *CrossingLon, double *CrossingLat, double *CrossingDist, int type, int state )
{
    wxGZListNode *GZ_node = g_pGZList->GetFirst();
    GZ *pGZ = NULL;
    ODPoint *popFirst;
    ODPoint *popSecond;
    wxString l_GUID = wxEmptyString;
    bool l_bCrosses;
    
    GZCrossingList.clear();
    
    while( GZ_node ) {
        bool    l_bNext = false;
        pGZ = GZ_node->GetData();
        switch (state) {
            case ID_PATH_STATE_ANY:
                l_bNext = false;
                break;
            case ID_PATH_STATE_ACTIVE:
                if(pGZ->IsActive()) l_bNext = false;
                else l_bNext = true;
                break;
            case ID_PATH_STATE_INACTIVE:
                if(!pGZ->IsActive()) l_bNext = false;
                else l_bNext = true;
                break;
        }
        
        if(!l_bNext) {
            switch (type) {
                case ID_BOUNDARY_ANY:
                    l_bNext = false;
                    break;
                case ID_BOUNDARY_EXCLUSION:
                    if(!pGZ->m_bExclusionGZ) l_bNext = true;
                    break;
                case ID_BOUNDARY_INCLUSION:
                    if(!pGZ->m_bInclusionGZ) l_bNext = true;
                    break;
                case ID_BOUNDARY_NEITHER:
                    if(pGZ->m_bExclusionGZ || pGZ->m_bInclusionGZ) l_bNext = true;
                    break;
            }
        }
        
        if(!l_bNext) {
            wxODPointListNode *OCPNpoint_node = ( pGZ->m_pODPointList )->GetFirst();
            wxODPointListNode *OCPNpoint_next_node = OCPNpoint_node->GetNext();

            popFirst = OCPNpoint_node->GetData();
            while( OCPNpoint_next_node ) {
                double l_dCrossingLon;
                double l_dCrossingLat;
                popSecond = OCPNpoint_next_node->GetData();
                l_bCrosses = GetLineIntersection(StartLon, StartLat, EndLon, EndLat, popFirst->m_lon, popFirst->m_lat, popSecond->m_lon, popSecond->m_lat, &l_dCrossingLon, &l_dCrossingLat);
                if(l_bCrosses) {
                    double brg;
                    double len;
                    GZCROSSING l_GZCrossing;
                    l_GZCrossing.GUID = pGZ->m_GUID;
                    l_GZCrossing.Lon = l_dCrossingLon;
                    l_GZCrossing.Lat = l_dCrossingLat;
                    DistanceBearingMercator_Plugin( StartLat, StartLon, l_dCrossingLat, l_dCrossingLon, &brg, &len );
                    l_GZCrossing.Len = len;
                    GZCrossingList.push_back(l_GZCrossing);
                }
                popFirst = popSecond;
                OCPNpoint_next_node = OCPNpoint_next_node->GetNext();
            }
        }
        GZ_node = GZ_node->GetNext();                         // next GZ
    }
    // if list of crossings <> 0 then find one closest to start point
    if(!GZCrossingList.empty()) {
        std::list<GZCROSSING>::iterator it = GZCrossingList.begin();
        wxString l_sGUID = it->GUID;
        *CrossingDist = it->Len;
        *CrossingLon = it->Lon;
        *CrossingLat = it->Lat;
        ++it;
        while( it != GZCrossingList.end() ) {
            if( *CrossingDist > it->Len ) {
                *CrossingDist = it->Len;
                *CrossingLon = it->Lon;
                *CrossingLat = it->Lat;
                l_sGUID = it->GUID;
            }
            ++it;
        }
        return l_sGUID;
    }
    return _T("");
}

