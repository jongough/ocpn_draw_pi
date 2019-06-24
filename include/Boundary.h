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

#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "ODPath.h"

class Boundary : public ODPath
{
    public:
        Boundary();
        virtual ~Boundary();
        void Draw( ODDC& dc, PlugIn_ViewPort &VP );
        void DrawGL( PlugIn_ViewPort &piVP );
        void DeletePoint( ODPoint *op, bool bRenamePoints );
        void SetColours(ODPath * );
        void SetActiveColours( void );
        void SetColourScheme( PI_ColorScheme cs = PI_GLOBAL_COLOR_SCHEME_RGB );
        void CreateColourSchemes( void );
        void MoveAllPoints( double inc_lat, double inc_lon );
        ODPoint *InsertPointAfter( ODPoint *pOP, double lat, double lon, bool bRenamePoints );
        void RemovePointFromPath( ODPoint* point, ODPath* path );

        wxColour        m_wxcActiveFillColour;
        wxColour        m_wxcInActiveFillColour;
        unsigned int    m_uiFillTransparency;
        int             m_iInclusionBoundarySize;
        bool            m_bExclusionBoundary;
        bool            m_bInclusionBoundary;
        
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
        
};

WX_DECLARE_LIST(Boundary, BoundaryList); // establish class Boundary as list member

#endif // BOUNDARY_H
