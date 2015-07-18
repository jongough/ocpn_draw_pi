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
#include "Boundary.h"
#include "ocpn_draw_pi.h"

#include "math.h"

extern PathList         *g_pPathList;
extern BoundaryList     *g_pBoundaryList;

wxString BoundaryMan::FindPointInBoundary( double lat, double lon )
{
    wxPathListNode *boundary_node = g_pPathList->GetFirst();
    Boundary *pboundary = NULL;
    bool bInPoly;
    ODPoint *pop;
    int k = 0;
    
    while( boundary_node ) {
        pboundary = (Boundary *)boundary_node->GetData();
        
        k++;
        int i, j;
        j = pboundary->m_pODPointList->GetCount();
        float *polyX;
        float *polyY;
        polyX = new float[j];
        polyY = new float[j];
        
        wxODPointListNode *OCPNpoint_node = ( pboundary->m_pODPointList )->GetFirst();
        wxODPointListNode *OCPNpoint_last_node = ( pboundary->m_pODPointList )->GetLast();
        i = 0;
        while( OCPNpoint_node ) {
            pop = OCPNpoint_node->GetData();
            polyX[i] = (float)pop->m_lon;
            polyY[i] = (float)pop->m_lat;
            i++;
            OCPNpoint_node = OCPNpoint_node->GetNext();           // next OD point
            if(OCPNpoint_node == OCPNpoint_last_node) break;
        }
        bInPoly = pointInPolygon(i, polyX, polyY, lon, lat);
        if(bInPoly) break;
        boundary_node = boundary_node->GetNext();                         // next boundary
        delete [] polyX;
        delete [] polyY;
    }
    
    if(bInPoly) return pboundary->m_GUID;
    else return wxT("");
}

bool BoundaryMan::FindPointInBoundary( Boundary *pBoundary, double lat, double lon )
{
    bool bInPoly;
    int i, j;
    j = pBoundary->m_pODPointList->GetCount();
    float *polyX;
    float *polyY;
    polyX = new float[j];
    polyY = new float[j];
    
    wxODPointListNode *OCPNpoint_node = ( pBoundary->m_pODPointList )->GetFirst();
    wxODPointListNode *OCPNpoint_last_node = ( pBoundary->m_pODPointList )->GetLast();
    i = 0;
    while( OCPNpoint_node ) {
        ODPoint *pop = OCPNpoint_node->GetData();
        polyX[i] = (float)pop->m_lon;
        polyY[i] = (float)pop->m_lat;
        i++;
        OCPNpoint_node = OCPNpoint_node->GetNext();           // next OD point
        if(OCPNpoint_node == OCPNpoint_last_node) break;
    }
    bInPoly = pointInPolygon(i, polyX, polyY, lon, lat);

    delete [] polyX;
    delete [] polyY;
    
    return bInPoly;
}

//  Parameters for this function:
//
//  int    polyCorners  =  how many corners the polygon has
//  float  polyX[]      =  horizontal coordinates of corners
//  float  polyY[]      =  vertical coordinates of corners
//  float  x, y         =  point to be tested
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.

bool BoundaryMan::pointInPolygon(int polyCorners, float *polyX, float *polyY, float x, float y) {
    
    int   i, j=polyCorners-1 ;
    bool  oddNodes=false      ;
    
    for (i=0; i<polyCorners; i++) {
        if (((polyY[i]< y && polyY[j]>=y)
            ||   (polyY[j]< y && polyY[i]>=y))
            &&  (polyX[i]<=x || polyX[j]<=x)) {
            oddNodes^=(polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x); }
            j=i; 
        
    }
            
    return oddNodes; 
        
}