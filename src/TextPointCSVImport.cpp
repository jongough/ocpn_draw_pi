/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  OCPN Draw Text Point CSV Import support
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

#include "TextPointCSVImport.h"
#include "ocpn_draw_pi.h"

#include <wx/tokenzr.h>

TextPointCSVImport::TextPointCSVImport()
{
    //ctor
    m_sName.Clear();
    m_dLat = 0;
    m_dLon = 0;
}

TextPointCSVImport::~TextPointCSVImport()
{
    //dtor
}

TextPointCSVImport::TextPointCSVImport(wxStringTokenizer TextPointCSV)
{
    //ctor
    size_t l_count = TextPointCSV.CountTokens();
    m_sName.Clear();
    m_dLat = 0;
    m_dLon = 0;
    if(l_count >= 3) {
        m_sName = TextPointCSV.GetNextToken();
        m_sName = m_sName.SubString(1, m_sName.Length()-2);
        TextPointCSV.GetNextToken().ToDouble(&m_dLat);
        TextPointCSV.GetNextToken().ToDouble(&m_dLon);
    } 
    if(l_count >=4) {
        wxString l_TextLocation = TextPointCSV.GetNextToken();
        if(l_TextLocation == _T("'t'") || l_TextLocation == _T("'T'")) m_iTextPosition = ID_TEXT_TOP;
        else if(l_TextLocation == _T("'ct'") || l_TextLocation == _T("'CT'")) m_iTextPosition = ID_TEXT_CENTRE_TOP;
        else if(l_TextLocation == _T("'b'") || l_TextLocation == _T("'B'")) m_iTextPosition = ID_TEXT_BOTTOM;
        else if(l_TextLocation == _T("'cb'") || l_TextLocation == _T("'CB'")) m_iTextPosition = ID_TEXT_CENTRE_BOTTOM;
        else if(l_TextLocation == _T("'c'") || l_TextLocation == _T("'C'")) m_iTextPosition = ID_TEXT_CENTRE;
        else if(l_TextLocation == _T("'r'") || l_TextLocation == _T("'R'")) m_iTextPosition = ID_TEXT_RIGHT;
        else if(l_TextLocation == _T("'l'") || l_TextLocation == _T("'L'")) m_iTextPosition = ID_TEXT_LEFT;
        else m_iTextPosition = ID_TEXT_CENTRE_BOTTOM;
    }
    if(l_count >=5) {
        wxString l_Show = TextPointCSV.GetNextToken();
        if(l_Show == _T("'a'") || l_Show == _T("'A'")) m_iDisplayTextWhen = ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ALWAYS;
        else if(l_Show == _T("'r'") || l_Show == _T("'R'")) m_iDisplayTextWhen = ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ON_ROLLOVER;
        else if(l_Show == _T("'n'") || l_Show == _T("'N'")) m_iDisplayTextWhen = ID_TEXTPOINT_DISPLAY_TEXT_SHOW_NEVER;
        else m_iDisplayTextWhen = ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ALWAYS;
    }
    if(l_count >= 6) {
        wxString vis = TextPointCSV.GetNextToken();
        if(vis == _T("'F'") || vis == _T("'f'"))
            m_bVisible = false;
        else
            m_bVisible = true;
    } 
    if(l_count >= 7) {
        m_TextPointText = TextPointCSV.GetNextToken();
        m_TextPointText = m_TextPointText.SubString(1, m_TextPointText.Length()-2);
        m_TextPointText.Replace("\\n", "\n");
    }
}
