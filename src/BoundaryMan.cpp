/***************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Boundary Manager
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


#include "BoundaryMan.h"
#include "ocpn_draw_pi.h"
#include "Boundary.h"
#include "BoundaryPoint.h"
#include "PointMan.h"
#include "ODUtils.h"

#ifdef __WXOSX__
#include <math.h>
#else
#include "math.h"
#endif

wxString BoundaryMan::FindPointInBoundary( double lat, double lon, int type, int state )
{
    wxBoundaryListNode *boundary_node = g_pBoundaryList->GetFirst();
    Boundary *pboundary = NULL;
    bool bInPoly = false;
    ODPoint *pop;
    int k = 0;
    wxString l_GUID = wxEmptyString;
    
    while( boundary_node ) {
        bool    l_bNext = false;
        pboundary = boundary_node->GetData();
        switch (state) {
            case ID_PATH_STATE_ANY:
                l_bNext = false;
                break;
            case ID_PATH_STATE_ACTIVE:
                if(pboundary->IsActive()) l_bNext = false;
                else l_bNext = true;
                break;
            case ID_PATH_STATE_INACTIVE:
                if(!pboundary->IsActive()) l_bNext = false;
                else l_bNext = true;
                break;
        }

        if(!l_bNext) {
            switch (type) {
                case ID_BOUNDARY_ANY:
                    l_bNext = false;
                    break;
                case ID_BOUNDARY_EXCLUSION:
                    if(!pboundary->m_bExclusionBoundary) l_bNext = true;
                    break;
                case ID_BOUNDARY_INCLUSION:
                    if(!pboundary->m_bInclusionBoundary) l_bNext = true;
                    break;
                case ID_BOUNDARY_NEITHER:
                    if(pboundary->m_bExclusionBoundary || pboundary->m_bInclusionBoundary) l_bNext = true;
                    break;
            }
        }
        
        if(!l_bNext) {
            k++;
            int i, j;
            j = pboundary->m_pODPointList->GetCount();
            double *polyX;
            double *polyY;
            polyX = new double[j];
            polyY = new double[j];
            
            wxODPointListNode *OCPNpoint_node = ( pboundary->m_pODPointList )->GetFirst();
            wxODPointListNode *OCPNpoint_last_node = ( pboundary->m_pODPointList )->GetLast();
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
                l_GUID = pboundary->m_GUID;
                break;
            }
        }
        boundary_node = boundary_node->GetNext();                         // next boundary
    }
    
    if(bInPoly) return l_GUID;
    else return wxT("");
}

bool BoundaryMan::FindPointInBoundary( Boundary *pBoundary, double lat, double lon, int type, int state )
{
    bool bInPoly = false;
    int i, j;
    j = pBoundary->m_pODPointList->GetCount();
    double *polyX;
    double *polyY;
    polyX = new double[j];
    polyY = new double[j];
    
    bool l_bOK = false;
    switch (state) {
        case ID_PATH_STATE_ANY:
            l_bOK = true;
            break;
        case ID_PATH_STATE_ACTIVE:
            if(pBoundary->IsActive()) l_bOK = true;
            else l_bOK = false;
            break;
        case ID_PATH_STATE_INACTIVE:
            if(!pBoundary->IsActive()) l_bOK = true;
            else l_bOK = false;
            break;
    }
    
    if(l_bOK) {
        switch (type) {
            case ID_BOUNDARY_ANY:
                l_bOK = true;
                break;
            case ID_BOUNDARY_EXCLUSION:
                if(pBoundary->m_bExclusionBoundary) l_bOK = true;
                else l_bOK = false;
                break;
            case ID_BOUNDARY_INCLUSION:
                if(pBoundary->m_bInclusionBoundary) l_bOK = true;
                else l_bOK = false;
                break;
            case ID_BOUNDARY_NEITHER:
                if(pBoundary->m_bExclusionBoundary || pBoundary->m_bInclusionBoundary) l_bOK = false;
                else l_bOK = true;
                break;
        }
    }
    if(l_bOK) {
        wxODPointListNode *OCPNpoint_node = ( pBoundary->m_pODPointList )->GetFirst();
        wxODPointListNode *OCPNpoint_last_node = ( pBoundary->m_pODPointList )->GetLast();
        if(pBoundary->m_pODPointList->GetCount() > 3) {
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
    }
    delete [] polyX;
    delete [] polyY;

    return bInPoly;
}

bool BoundaryMan::FindPointInBoundary( wxString l_GUID, double lat, double lon, int type, int state )
{
    bool bInPoly = false;
    bool bBoundaryFound = false;
    int i;
    double *polyX = NULL;
    double *polyY = NULL;
    Boundary *l_pBoundary = NULL;
    
    wxBoundaryListNode *pboundary_node = g_pBoundaryList->GetFirst();
    while( pboundary_node ) {
        l_pBoundary = (Boundary *)pboundary_node->GetData();
        if(l_pBoundary->m_GUID == l_GUID) {
            bBoundaryFound = true;
            break;
        }
        l_pBoundary = (Boundary *)pboundary_node->GetNext();
    }
    if(l_pBoundary && bBoundaryFound) {
        bool l_bOK = false;
        switch (state) {
            case ID_PATH_STATE_ANY:
                l_bOK = true;
                break;
            case ID_PATH_STATE_ACTIVE:
                if(l_pBoundary->IsActive()) l_bOK = true;
                else l_bOK = false;
                break;
            case ID_PATH_STATE_INACTIVE:
                if(!l_pBoundary->IsActive()) l_bOK = true;
                else l_bOK = false;
                break;
        }
        
        if(l_bOK) {
            switch (type) {
                case ID_BOUNDARY_ANY:
                    l_bOK = true;
                    break;
                case ID_BOUNDARY_EXCLUSION:
                    if(l_pBoundary->m_bExclusionBoundary) l_bOK = true;
                    else l_bOK = false;
                    break;
                case ID_BOUNDARY_INCLUSION:
                    if(l_pBoundary->m_bInclusionBoundary) l_bOK = true;
                    else l_bOK = false;
                    break;
                case ID_BOUNDARY_NEITHER:
                    if(l_pBoundary->m_bExclusionBoundary || l_pBoundary->m_bInclusionBoundary) l_bOK = false;
                    else l_bOK = true;
                    break;
            }
        }
        if(!l_bOK) return false;

        polyX = new double[l_pBoundary->m_pODPointList->GetCount()];
        polyY = new double[l_pBoundary->m_pODPointList->GetCount()];
        wxODPointListNode *OCPNpoint_node = ( l_pBoundary->m_pODPointList )->GetFirst();
        wxODPointListNode *OCPNpoint_last_node = ( l_pBoundary->m_pODPointList )->GetLast();
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

wxString BoundaryMan::FindPointInBoundaryPoint( double lat, double lon, int type, int state )
{
    bool bInPoly = false;
    ODPoint *pop;
    wxString l_GUID = wxEmptyString;
    
    wxODPointListNode *ODpoint_node = g_pODPointMan->GetODPointList()->GetFirst();
    BoundaryPoint *l_pBoundaryPoint = NULL;
    while( ODpoint_node ) {
        bool l_bNext = false;
        pop = (ODPoint *) ODpoint_node->GetData();
        if(pop->m_sTypeString == wxT("Boundary Point")) {
            l_pBoundaryPoint = (BoundaryPoint *) ODpoint_node->GetData();
            switch (state) {
                case ID_POINT_STATE_ANY:
                    l_bNext = false;
                    break;
                case ID_POINT_STATE_ACTIVE:
                    if(l_pBoundaryPoint->IsActive()) l_bNext = false;
                    else l_bNext = true;
                    break;
                case ID_POINT_STATE_INACTIVE:
                    if(!l_pBoundaryPoint->IsActive()) l_bNext = false;
                    else l_bNext = true;
                    break;
            }
            if(!l_bNext) {
                switch (type) {
                    case ID_BOUNDARY_ANY:
                        l_bNext = false;
                        break;
                    case ID_BOUNDARY_EXCLUSION:
                        if(!l_pBoundaryPoint->m_bExclusionBoundaryPoint) l_bNext = true;
                        break;
                    case ID_BOUNDARY_INCLUSION:
                        if(!l_pBoundaryPoint->m_bInclusionBoundaryPoint) l_bNext = true;
                        break;
                    case ID_BOUNDARY_NEITHER:
                        if(!l_pBoundaryPoint->m_bExclusionBoundaryPoint && !l_pBoundaryPoint->m_bInclusionBoundaryPoint) l_bNext = true;
                        break;
                }
            }
            if(l_pBoundaryPoint->m_bShowODPointRangeRings && l_pBoundaryPoint->m_iODPointRangeRingsNumber > 0 && !l_bNext) {
                double l_dRangeRingSize = l_pBoundaryPoint->m_iODPointRangeRingsNumber * l_pBoundaryPoint->m_fODPointRangeRingsStep;
                double brg;
                double factor = 1.00;
                if( l_pBoundaryPoint->m_iODPointRangeRingsStepUnits == 1 )          // convert kilometer to nautical miles
                    factor = 1 / 1.852;

                l_dRangeRingSize = factor * l_dRangeRingSize;
                double l_dPointDistance; // l_dPointDistance is in nautical mile

                DistanceBearingMercator_Plugin( l_pBoundaryPoint->m_lat, l_pBoundaryPoint->m_lon, lat, lon, &brg, &l_dPointDistance );
                //l_dPointDistance = sqrt(((pboundarypoint->m_lat - lat) * (pboundarypoint->m_lat - lat)) + ((pboundarypoint->m_lon - lon) * (pboundarypoint->m_lon - lon)));
                if( l_dRangeRingSize > l_dPointDistance ) {
                    l_GUID = l_pBoundaryPoint->m_GUID;
                    bInPoly = true;
                    break;
                }
            }
        }
        ODpoint_node = ODpoint_node->GetNext();
    }
    
    if(bInPoly) return l_GUID;
    else return wxT("");
}

bool BoundaryMan::FindPointInBoundaryPoint( BoundaryPoint *pBoundaryPoint, double lat, double lon, int type, int state )
{
    bool bInPoly = false;
    bool l_bOK = false;
    
    switch (state) {
        case ID_POINT_STATE_ANY:
            l_bOK = true;
            break;
        case ID_POINT_STATE_ACTIVE:
            if(pBoundaryPoint->IsActive()) l_bOK = true;
            else l_bOK = false;
            break;
        case ID_POINT_STATE_INACTIVE:
            if(!pBoundaryPoint->IsActive()) l_bOK = true;
            else l_bOK = false;
            break;
    }
    
    if(l_bOK) {
        switch (type) {
            case ID_BOUNDARY_ANY:
                l_bOK = true;
                break;
            case ID_BOUNDARY_EXCLUSION:
                if(!pBoundaryPoint->m_bExclusionBoundaryPoint) l_bOK = true;
                else l_bOK = false;
                break;
            case ID_BOUNDARY_INCLUSION:
                if(pBoundaryPoint->m_bInclusionBoundaryPoint) l_bOK = true;
                else l_bOK = false;
                break;
            case ID_BOUNDARY_NEITHER:
                if(pBoundaryPoint->m_bExclusionBoundaryPoint || pBoundaryPoint->m_bInclusionBoundaryPoint) l_bOK = false;
                else l_bOK = true;
                break;
        }
    }
    
    if(!l_bOK) return false;
    
    if(pBoundaryPoint->m_iODPointRangeRingsNumber > 0) {
        double l_dRangeRingSize = pBoundaryPoint->m_iODPointRangeRingsNumber * pBoundaryPoint->m_fODPointRangeRingsStep;
        double brg;
        double l_dPointDistance;
        DistanceBearingMercator_Plugin( pBoundaryPoint->m_lat, pBoundaryPoint->m_lon, lat, lon, &brg, &l_dPointDistance );
        if( l_dRangeRingSize > l_dPointDistance ) {
            bInPoly = true;
        }
    }
    
    return bInPoly;
}

bool BoundaryMan::FindPointInBoundaryPoint( wxString l_GUID, double lat, double lon, int type, int state )
{
    bool bInPoly = false;
    
    ODPoint *pPoint = g_pODPointMan->FindODPointByGUID( l_GUID );
    if(pPoint->m_sTypeString == wxT("Boundary Point")) {
        BoundaryPoint *pBoundaryPoint = (BoundaryPoint *) pPoint;
        bool l_bOK = false;
        
        switch (state) {
            case ID_POINT_STATE_ANY:
                l_bOK = true;
                break;
            case ID_POINT_STATE_ACTIVE:
                if(pBoundaryPoint->IsActive()) l_bOK = true;
                else l_bOK = false;
                break;
            case ID_POINT_STATE_INACTIVE:
                if(!pBoundaryPoint->IsActive()) l_bOK = true;
                else l_bOK = false;
                break;
        }
        
        if(l_bOK) {
            switch (type) {
                case ID_BOUNDARY_ANY:
                    l_bOK = true;
                    break;
                case ID_BOUNDARY_EXCLUSION:
                    if(!pBoundaryPoint->m_bExclusionBoundaryPoint) l_bOK = true;
                    else l_bOK = false;
                    break;
                case ID_BOUNDARY_INCLUSION:
                    if(pBoundaryPoint->m_bInclusionBoundaryPoint) l_bOK = true;
                    else l_bOK = false;
                    break;
                case ID_BOUNDARY_NEITHER:
                    if(pBoundaryPoint->m_bExclusionBoundaryPoint || pBoundaryPoint->m_bInclusionBoundaryPoint) l_bOK = false;
                    else l_bOK = true;
                    break;
            }
        }
        if(!l_bOK) return false;
        
        if(pBoundaryPoint->m_iODPointRangeRingsNumber > 0) {
            double l_dRangeRingSize = pBoundaryPoint->m_iODPointRangeRingsNumber * pBoundaryPoint->m_fODPointRangeRingsStep;
            double brg;
            double l_dPointDistance;
            DistanceBearingMercator_Plugin( pBoundaryPoint->m_lat, pBoundaryPoint->m_lon, lat, lon, &brg, &l_dPointDistance );
            if( l_dRangeRingSize > l_dPointDistance )
                bInPoly = true;
        }
    }
        
    return bInPoly;
}

Boundary *BoundaryMan::FindLineCrossingBoundaryPtr(double StartLon, double StartLat, double EndLon, double EndLat, double *CrossingLon, double *CrossingLat, double *CrossingDist, int type, int state, bool FindFirst )
{
    wxBoundaryListNode *boundary_node = g_pBoundaryList->GetFirst();
    Boundary *pboundary = NULL;
    ODPoint *popFirst;
    ODPoint *popSecond;
    wxString l_GUID = wxEmptyString;
    bool l_bCrosses;
    LLBBox RBBox;
    RBBox.SetFromSegment(StartLat, StartLon, EndLat, EndLon);

    std::list<BOUNDARYCROSSING> BoundaryCrossingList;
    
    while( boundary_node ) {
        bool    l_bNext = false;
        pboundary = boundary_node->GetData();
        switch (state) {
            case ID_PATH_STATE_ANY:
                l_bNext = false;
                break;
            case ID_PATH_STATE_ACTIVE:
                if(pboundary->IsActive()) l_bNext = false;
                else l_bNext = true;
                break;
            case ID_PATH_STATE_INACTIVE:
                if(!pboundary->IsActive()) l_bNext = false;
                else l_bNext = true;
                break;
        }
        
        if(!l_bNext) {
            switch (type) {
                case ID_BOUNDARY_ANY:
                    l_bNext = false;
                    break;
                case ID_BOUNDARY_EXCLUSION:
                    if(!pboundary->m_bExclusionBoundary) l_bNext = true;
                    break;
                case ID_BOUNDARY_INCLUSION:
                    if(!pboundary->m_bInclusionBoundary) l_bNext = true;
                    break;
                case ID_BOUNDARY_NEITHER:
                    if(pboundary->m_bExclusionBoundary || pboundary->m_bInclusionBoundary) l_bNext = true;
                    break;
            }
        }
        
        if(!l_bNext && !RBBox.IntersectOut(pboundary->GetBBox())) {
            wxODPointListNode *OCPNpoint_node = ( pboundary->m_pODPointList )->GetFirst();
            wxODPointListNode *OCPNpoint_next_node = OCPNpoint_node->GetNext();

            popFirst = OCPNpoint_node->GetData();
            while( OCPNpoint_next_node ) {
                double l_dCrossingLon;
                double l_dCrossingLat;
                popSecond = OCPNpoint_next_node->GetData();
                l_bCrosses = GetLineIntersection(StartLon, StartLat, EndLon, EndLat, popFirst->m_lon, popFirst->m_lat, popSecond->m_lon, popSecond->m_lat, &l_dCrossingLon, &l_dCrossingLat);
                if(l_bCrosses) {
                    if(!FindFirst) {
                        double brg;
                        double len;
                        BOUNDARYCROSSING l_BoundaryCrossing;
                        l_BoundaryCrossing.ptr = pboundary;
                        l_BoundaryCrossing.GUID = pboundary->m_GUID;
                        l_BoundaryCrossing.Lon = l_dCrossingLon;
                        l_BoundaryCrossing.Lat = l_dCrossingLat;
                        DistanceBearingMercator_Plugin( StartLat, StartLon, l_dCrossingLat, l_dCrossingLon, &brg, &len );
                        l_BoundaryCrossing.Len = len;
                        BoundaryCrossingList.push_back(l_BoundaryCrossing);
                    } else {
                        return pboundary;
                    }
                }
                popFirst = popSecond;
                OCPNpoint_next_node = OCPNpoint_next_node->GetNext();
            }
        }
        boundary_node = boundary_node->GetNext();                         // next boundary
    }
    
    // if list of crossings <> 0 then find one closest to start point
    if(!BoundaryCrossingList.empty()) {
        std::list<BOUNDARYCROSSING>::iterator it = BoundaryCrossingList.begin();
        Boundary *pret = it->ptr;

        wxString l_sGUID = it->GUID;
        *CrossingDist = it->Len;
        *CrossingLon = it->Lon;
        *CrossingLat = it->Lat;
        ++it;
        while( it != BoundaryCrossingList.end() ) {
            if( *CrossingDist > it->Len ) {
                *CrossingDist = it->Len;
                *CrossingLon = it->Lon;
                *CrossingLat = it->Lat;
                l_sGUID = it->GUID;
                pret = it->ptr;
            }
            ++it;
        }
        return pret;
    }
    return 0;
}

wxString BoundaryMan::FindLineCrossingBoundary(double StartLon, double StartLat, double EndLon, double EndLat, double *CrossingLon, double *CrossingLat, double *CrossingDist, int type, int state, bool FindFirst )
{
    Boundary *pboundary;
    pboundary = FindLineCrossingBoundaryPtr(StartLon, StartLat, EndLon, EndLat, CrossingLon, CrossingLat, CrossingDist, type, state, FindFirst );
    if ( pboundary != 0)
        return  pboundary->m_GUID;
    return _T("");
}
