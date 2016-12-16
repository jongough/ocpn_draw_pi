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


#ifndef GZMAN_H
#define GZMAN_H

#include "PathMan.h"
#include "ocpn_draw_pi.h"

class GZ;
class ODPoint;

class GZMan : public PathMan
{
    public:
        wxString    FindPointInGZ( double lat, double lon, int type = ID_BOUNDARY_ANY, int state = ID_PATH_STATE_ANY );
        bool        FindPointInGZ( GZ *pGZ, double lat, double lon, int type = ID_BOUNDARY_ANY, int state = ID_PATH_STATE_ANY );
        bool        FindPointInGZ( wxString l_GUID, double lat, double lon, int type = ID_BOUNDARY_ANY , int state = ID_PATH_STATE_ANY );
        wxString    FindLineCrossingGZ( double StartLat, double StartLon, double Endat, double EndLon, double *CrossingLat, double *CrossingLon, double *Crossingdist, int type, int state );
    private:
        struct GZCROSSING {
            wxString    GUID;
            double      Len;
            double      Lon;
            double      Lat;
        };
        
        std::list<GZCROSSING> GZCrossingList;
};

#endif // GZMAN_H
