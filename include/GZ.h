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

#ifndef GZ_H
#define GZ_H

#include "ODPath.h"
#include "ocpn_plugin.h"

enum {
    ID_LINE_TYPE_ARC = 0,
    ID_LINE_TYPE_STRAIGHT
};

//forward definitions
class ODPoint;
class ODDC;

class GZ : public ODPath
{
    public:
        GZ();
        virtual ~GZ();
        void Draw( ODDC& dc, PlugIn_ViewPort &VP );
        void DrawGL( PlugIn_ViewPort &piVP );
        void SetActiveColours( void );
        void SetColourScheme( PI_ColorScheme cs = PI_GLOBAL_COLOR_SCHEME_RGB );
        void CreateColourSchemes( void );
        void MoveAllPoints( double inc_lat, double inc_lon );
        void UpdateGZ( bool bUpdateSelectablePath = true );
        void UpdateGZSelectablePath( void );
        void MaintainWith( void );
        void UpdateGZ( ODPoint *pGZPoint, bool bUpdateSelectablePath = true );
        void GetLatLonPoints( PlugIn_ViewPort &piVP, wxPoint *l_pCentre, wxPoint *l_l1p1, wxPoint *l_l1p2, wxPoint *l_l2p1, wxPoint *l_l2p2 );
        void SetPersistence( int PersistenceType );
        LLBBox GetBBox( void );
        
        wxColour        m_wxcActiveFillColour;
        wxColour        m_wxcInActiveFillColour;
        int             m_iPersistenceType;
        unsigned int    m_uiFillTransparency;
        double          m_dCentreLat;
        double          m_dCentreLon;
        double          m_dFirstLineDirection;
        double          m_dSecondLineDirection;
        double          m_dFirstDistance;
        double          m_dSecondDistance;
        bool            m_bRotateWithBoat;
        int             m_iMaintainWith;
        int             m_iLineType;
        bool            m_bCentreOnBoat;
        double          m_dBoatHeading;
        double          m_dBoatCOG;
        bool            m_bExclusionGZ;
        bool            m_bInclusionGZ;
        
    protected:
        wxColour    m_fillcol;
        wxColour    m_wxcSchemeActiveFillColour;
        wxColour    m_wxcSchemeInActiveFillColour;
        wxColour    m_wxcActiveFillColourDay;
        wxColour    m_wxcInActiveFillColourDay;
        wxColour    m_wxcActiveFillColourDusk;
        wxColour    m_wxcInActiveFillColourDusk;
        wxColour    m_wxcActiveFillColourNight;
        wxColour    m_wxcInActiveFillColourNight;
        wxColour    m_wxcActiveFillColourRGB;
        wxColour    m_wxcInActiveFillColourRGB;
        
    private:
        bool        m_bSetTransparent;
        
};

WX_DECLARE_LIST(GZ, GZList); // establish class GZ as list member

#endif // GZ_H
