/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  OCPN Draw Nav Objects support
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

#include "BoundaryCSVImport.h"
#include "ocpn_draw_pi.h"

#include <wx/tokenzr.h>

BoundaryCSVImport::BoundaryCSVImport()
{
    //ctor
    m_sName.Clear();
    m_bExclusion = true;
    m_bInclusion = false;
}

BoundaryCSVImport::BoundaryCSVImport(wxStringTokenizer *BoundaryCSV)
{
    //ctor
    size_t l_count = BoundaryCSV->CountTokens();
    wxString l_type;
    wxString vis;
    wxString l_rgb;

    m_sName.Clear();
    m_bExclusion = g_bExclusionBoundary;
    m_bInclusion = g_bInclusionBoundary;
    m_bVisible = true;
    m_LineColour = g_colourActiveBoundaryLineColour;
    m_FillColour = g_colourActiveBoundaryFillColour;
    
    if(l_count >= 3) {
        BoundaryCSV->GetNextToken(); // get rid of comma
        m_sName = BoundaryCSV->GetNextToken();
        BoundaryCSV->GetNextToken(); // get rid of comma
        l_type = BoundaryCSV->GetNextToken();
        if(l_type == _T("Exclusion")) {
            m_bExclusion = true;
            m_bInclusion = false;
        } else if(l_type == _T("Inclusion")) {
            m_bExclusion = false;
            m_bInclusion = true;
        } else if(l_type == _T("Neither")) {
            m_bExclusion = false;
            m_bInclusion = false;
        } else {
            m_bExclusion = true;
            m_bInclusion = false;
        }
    } else {
        m_bExclusion = g_bExclusionBoundary;
        m_bInclusion = g_bInclusionBoundary;
    }
    
    if(l_count >= 7) {
        BoundaryCSV->GetNextToken(); // get rid of comma
        vis = BoundaryCSV->GetNextToken();
        if(vis == _T("F") || vis == _T("f"))
            m_bVisible = false;
        else
            m_bVisible = true;
    }
    
    if(l_count >= 9) {
        BoundaryCSV->GetNextToken(); // get rid of comma
        l_rgb = BoundaryCSV->GetNextToken();
        m_LineColour.Set(l_rgb);
    } else
        m_LineColour = g_colourActiveBoundaryLineColour;
    
    if(l_count >= 11) {
        BoundaryCSV->GetNextToken(); // get rid of comma
        l_rgb = BoundaryCSV->GetNextToken();
        m_FillColour.Set(l_rgb);
    } else
        m_FillColour = g_colourActiveBoundaryFillColour;
    
}

BoundaryCSVImport::~BoundaryCSVImport()
{
    //dtor
}
