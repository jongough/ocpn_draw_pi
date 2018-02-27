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


#ifndef BOUNDARYMAN_H
#define BOUNDARYMAN_H

#include "PathMan.h"
#include "ocpn_draw_pi.h"

class Boundary;
class BoundaryPoint;

class BoundaryMan : public PathMan
{
    public:
        wxString    FindPointInBoundary( double lat, double lon, int type = ID_BOUNDARY_ANY, int state = ID_PATH_STATE_ANY );
        bool        FindPointInBoundary( Boundary *pBoundary, double lat, double lon, int type = ID_BOUNDARY_ANY, int state = ID_PATH_STATE_ANY );
        bool        FindPointInBoundary( wxString l_GUID, double lat, double lon, int type = ID_BOUNDARY_ANY, int state = ID_PATH_STATE_ANY );
        wxString    FindPointInBoundaryPoint( double lat, double lon, int type = ID_BOUNDARY_ANY, int state = ID_PATH_STATE_ANY );
        bool        FindPointInBoundaryPoint( BoundaryPoint *pBoundaryPoint, double lat, double lon, int type = ID_BOUNDARY_ANY, int state = ID_PATH_STATE_ANY );
        bool        FindPointInBoundaryPoint( wxString l_GUID, double lat, double lon, int type = ID_BOUNDARY_ANY, int state = ID_PATH_STATE_ANY );

        Boundary *  FindLineCrossingBoundaryPtr( double StartLat, double StartLon, double EndLat, double EndLon, double *CrossingLat, double *CrossingLon, double *Crossingdist,  
                                                int type = ID_BOUNDARY_ANY, int state = ID_PATH_STATE_ANY, bool FindFirst =  false );

        wxString    FindLineCrossingBoundary( double StartLat, double StartLon, double EndLat, double EndLon, double *CrossingLat, double *CrossingLon, double *Crossingdist,  
                                                int type = ID_BOUNDARY_ANY, int state = ID_PATH_STATE_ANY, bool FindFirst = false );
    private:
        struct BOUNDARYCROSSING {
            Boundary *  ptr;
            wxString    GUID;
            double      Len;
            double      Lon;
            double      Lat;
        };
        
};

#endif // BOUNDARYMAN_H
