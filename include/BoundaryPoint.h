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

#ifndef BOUNDARYPOINT_H
#define BOUNDARYPOINT_H

#include "ODPoint.h"

class BoundaryPoint : public ODPoint
{
public:
    BoundaryPoint(double lat, double lon, const wxString& icon_ident, const wxString& name, const wxString& pGUID = wxT(""), bool bAddToList = true);
    BoundaryPoint(BoundaryPoint* orig);
    BoundaryPoint();
    void Draw(piDC& dc, wxPoint* odp);
    void DrawGL(PlugIn_ViewPort& pivp);
    
    int             m_iInclusionBoundaryPointSize;
    bool            m_bExclusionBoundaryPoint;
    bool            m_bInclusionBoundaryPoint;
    unsigned int    m_uiBoundaryPointFillTransparency;
    bool            m_bIsInBoundary;
    
};

#endif // BOUNDARYPOINT_H
