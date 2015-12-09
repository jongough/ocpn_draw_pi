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

#ifndef GUARDZONE_H
#define GUARDZONE_H

#include "ODPath.h"

enum {
    ID_GZ_PERSISTENT = 0,
    ID_GZ_PERSISTENT_CRASH,
    ID_GZ_NOT_PERSISTENT,
    
    ID_GZ_PERSISTENT_LAST
};

class GuardZone : public ODPath
{
    public:
        GuardZone();
        virtual ~GuardZone();
        void Draw( ODDC& dc, PlugIn_ViewPort &VP );
        void DrawGL( PlugIn_ViewPort &piVP );
        void ResizeVRM( double lat, double lon );
        void ResizeVRM( void );
        void MoveEndPoint( double inc_lat, double inc_lon );
        
        void SetPersistence( int PersistenceType );
        void CentreOnBoat( void );
        void CentreOnLatLon( double lat, double lon );

        bool        m_bFixedEndPosition;
        int         m_iPersistenceType;
        bool        m_bVRM;
        bool        m_bCentreOnBoat;
        
        double      m_dCentreLat;
        double      m_dCentreLon;
        double      m_dFirstPointLat;
        double      m_dFirstPointLon;
        double      m_dSecondPointLat;
        double      m_dSecondPointLon;
        double      m_dAngleBetweenLines;
        
    protected:
        void        UpdateGZ( void );
        
    private:
        wxColour    m_wxcLineColour;
};

WX_DECLARE_LIST(GuardZone, GZList); // establish class GZ list member

#endif // GZ_H
