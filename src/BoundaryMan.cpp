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
#include <list>

#ifdef __WXOSX__
#include <math.h>
#else
#include "math.h"
#endif

wxString BoundaryMan::FindPointInBoundary( double lat, double lon, int type, int state )
{
    std::list<Boundary *>::iterator l_it;

    Boundary * boundary_node = *l_it;
    Boundary *pboundary = NULL;
    bool bInPoly = false;
    ODPoint *pop;
    int k = 0;
    wxString l_GUID = wxEmptyString;
    for (l_it = g_BoundaryList.m_pBoundaryList.begin(); l_it != g_BoundaryList.m_pBoundaryList.end(); ++l_it) {
        bool    l_bNext = false;
        pboundary = *l_it;
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
            j = pboundary->m_ODPointList.m_pODPoint.size();
            double *polyX;
            double *polyY;
            polyX = new double[j];
            polyY = new double[j];
            
            i = 0;
            for (std::list<ODPoint *>::iterator it = pboundary->m_ODPointList.m_pODPoint.begin(); it != pboundary->m_ODPointList.m_pODPoint.end(); ++it) {
                pop = *it;
                polyX[i] = pop->m_lon;
                polyY[i] = pop->m_lat;
                i++;
            }
            bInPoly = pointInPolygon(i, polyX, polyY, lon, lat);
            delete [] polyX;
            delete [] polyY;
            if(bInPoly) {
                l_GUID = pboundary->m_GUID;
                break;
            }
        }
    }
    
    if(bInPoly) return l_GUID;
    else return wxT("");
}

bool BoundaryMan::FindPointInBoundary( Boundary *pBoundary, double lat, double lon, int type, int state )
{
    bool bInPoly = false;
    int i, j;
    j = pBoundary->m_ODPointList.m_pODPoint.size();
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
        if(pBoundary->m_ODPointList.m_pODPoint.size() > 3) {
            i = 0;
            for (std::list<ODPoint *>::iterator it = pBoundary->m_ODPointList.m_pODPoint.begin(); it != pBoundary->m_ODPointList.m_pODPoint.end(); ++it) {
                ODPoint *pop = *it;
                polyX[i] = pop->m_lon;
                polyY[i] = pop->m_lat;
                i++;
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
    
    //wxBoundaryListNode *pboundary_node = g_BoundaryList->GetFirst();
    for(std::list<Boundary *>::iterator it = g_BoundaryList.m_pBoundaryList.begin(); it != g_BoundaryList.m_pBoundaryList.end(); ++it) {
        l_pBoundary = *it;
        if(l_pBoundary->m_GUID == l_GUID) {
            bBoundaryFound = true;
            break;
        }
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

        polyX = new double[l_pBoundary->m_ODPointList.m_pODPoint.size()];
        polyY = new double[l_pBoundary->m_ODPointList.m_pODPoint.size()];
        i = 0;
        for(std::list<ODPoint *>::iterator it = l_pBoundary->m_ODPointList.m_pODPoint.begin(); it != l_pBoundary->m_ODPointList.m_pODPoint.end(); ++it) {
            ODPoint *pop = *it;
            polyX[i] = pop->m_lon;
            polyY[i] = pop->m_lat;
            i++;
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
    
    BoundaryPoint *l_pBoundaryPoint = NULL;
    for(std::list<ODPoint *>::iterator it = g_pODPointMan->m_ODPointList.m_pODPoint.begin(); it != g_pODPointMan->m_ODPointList.m_pODPoint.end(); ++it) {
        bool l_bNext = false;
        pop = *it;
        if(pop->m_sTypeString == wxT("Boundary Point")) {
            l_pBoundaryPoint = (BoundaryPoint *) *it;
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
    Boundary *pboundary = NULL;
    ODPoint *popFirst;
    ODPoint *popSecond;
    wxString l_GUID = wxEmptyString;
    bool l_bCrosses;
    LLBBox RBBox;
    RBBox.SetFromSegment(StartLat, StartLon, EndLat, EndLon);

    std::list<BOUNDARYCROSSING> BoundaryCrossingList;
    
    for(std::list<Boundary *>::iterator it = g_BoundaryList.m_pBoundaryList.begin(); it != g_BoundaryList.m_pBoundaryList.end(); ++it) {
        bool    l_bNext = false;
        pboundary = *it;
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
            std::list<ODPoint *>::iterator it = pboundary->m_ODPointList.m_pODPoint.begin();
            popFirst = *it;
            for(++it; it != pboundary->m_ODPointList.m_pODPoint.end(); ++it) {
                popSecond = *it;
                double l_dCrossingLon;
                double l_dCrossingLat;
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
            }
        }
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
