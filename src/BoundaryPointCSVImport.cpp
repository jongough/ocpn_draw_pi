/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  OCPN Draw Boundary Point CSV Import support
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

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "BoundaryPointCSVImport.h"
#include "ocpn_draw_pi.h"

#include <wx/tokenzr.h>

BoundaryPointCSVImport::BoundaryPointCSVImport()
{
    //ctor
    m_sName.Clear();
    m_dLat = 0;
    m_dLon = 0;
}

BoundaryPointCSVImport::~BoundaryPointCSVImport()
{
    //dtor
}

BoundaryPointCSVImport::BoundaryPointCSVImport(wxStringTokenizer BoundaryPointCSV)
{
    //ctor
    size_t l_count = BoundaryPointCSV.CountTokens();
    m_sName.Clear();
    m_dLat = 0;
    m_dLon = 0;
    if(l_count >= 3) {
        m_sName = BoundaryPointCSV.GetNextToken();
        m_sName = m_sName.SubString(1, m_sName.Length()-2);
        BoundaryPointCSV.GetNextToken().ToDouble(&m_dLat);
        BoundaryPointCSV.GetNextToken().ToDouble(&m_dLon);
    } 
    if(l_count >= 4) {
        wxString l_type = BoundaryPointCSV.GetNextToken();
        if(l_type == _T("'Exclusion'")) {
            m_bExclusion = true;
            m_bInclusion = false;
        } else if(l_type == _T("'Inclusion'")) {
            m_bExclusion = false;
            m_bInclusion = true;
        } else if(l_type == _T("'Neither'")) {
            m_bExclusion = false;
            m_bInclusion = false;
        } else {
            m_bExclusion = true;
            m_bInclusion = false;
        }
    } 
    if(l_count >= 5) {
        wxString vis = BoundaryPointCSV.GetNextToken();
        if(vis == _T("'F'") || vis == _T("'f'"))
            m_bVisible = false;
        else
            m_bVisible = true;
    } else
        m_bVisible = g_bBoundaryODPointsVisible;
    
    if(l_count >= 6) {
        wxString vis = BoundaryPointCSV.GetNextToken();
        if(vis == _T("'F'") || vis == _T("'f'"))
            m_bRangeRingsVisible = false;
        else
            m_bRangeRingsVisible = true;
    } else
        m_bRangeRingsVisible = g_bODPointShowRangeRings;
    
    if(l_count >= 7) {
        BoundaryPointCSV.GetNextToken().ToLong((long int *)&m_iNumRings);
    } else
        m_iNumRings = g_iODPointRangeRingsNumber;
    
    if(l_count >= 8) {
        BoundaryPointCSV.GetNextToken().ToDouble(&m_dStep);
    } else
        m_dStep = g_fODPointRangeRingsStep;
    
    if(l_count >= 9) {
        wxString units = BoundaryPointCSV.GetNextToken();
        if(units == _T("'K'") || units == _T("'k'")) {
            m_iUnits = ID_KILOMETERS;
        } else
            m_iUnits = ID_NAUTICAL_MILES;
    } else
        m_iUnits = g_iODPointRangeRingsStepUnits;
        
    if(l_count >= 10) {
        m_RingColour.Set(BoundaryPointCSV.GetNextToken());
    } else
        m_RingColour = g_colourODPointRangeRingsColour;
}
