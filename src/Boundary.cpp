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

#include "Boundary.h"

#include <wx/listimpl.cpp>
WX_DEFINE_LIST ( BoundaryList );

extern wxString    g_ActiveBoundaryLineColour;
extern wxString    g_InActiveBoundaryLineColour;
extern wxString    g_ActiveBoundaryFillColour;
extern wxString    g_InActiveBoundaryFillColour;

Boundary::Boundary() : Path()
{
    //ctor
    m_itest = 1;
    m_sTypeString = _T("Boundary");

    m_ActiveLineColour = g_ActiveBoundaryLineColour;
    m_ActiveFillColour = g_ActiveBoundaryFillColour;
    m_InActiveLineColour = g_InActiveBoundaryLineColour;
    m_InActiveFillColour = g_InActiveBoundaryFillColour;
    
}

Boundary::~Boundary()
{
    //dtor
}
