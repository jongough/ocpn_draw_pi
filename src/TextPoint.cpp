/***************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Text points 
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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "TextPoint.h"
#include "ocpn_draw_pi.h"
#include "FontMgr.h"
#include "ocpndc.h"

#include <wx/stattext.h>

extern ocpn_draw_pi     *g_ocpn_draw_pi;

//extern ChartCanvas  *ocpncc1;

TextPoint::TextPoint() : ODPoint()
{
    m_pstText = new wxTextCtrl( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""));
}

TextPoint::TextPoint(const TextPoint& other)
{
    m_pstText = new wxTextCtrl( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""));
}

TextPoint::TextPoint( double lat, double lon, const wxString& icon_ident, const wxString& name,
                  const wxString &pGUID, bool bAddToList )
: ODPoint( lat, lon, icon_ident, name, pGUID, bAddToList )
{
    wxPoint destPoint;
    GetCanvasPixLL( g_ocpn_draw_pi->m_vp, &destPoint, m_lat, m_lon);
    wxSize tSize;
    tSize.x = 200;
    tSize.y = 50;
    m_pstText = new wxTextCtrl( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""), destPoint, tSize ,wxTE_MULTILINE | wxTE_READONLY );
//    m_pstText = new wxTextCtrl( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""), destPoint);
    m_pstText->SetForegroundColour(*wxBLACK);
    //m_pstText->SetBackgroundColour(wxTRANSPARENT);
    m_pstText->SetEditable( FALSE );
//    wxTextAttr  *pStyle;
//    pStyle->AddFlag(wxTE_MULTILINE);
    //m_pstText->SetDefaultStyle( *pStyle );
}
TextPoint::~TextPoint()
{
    if( m_pstText ) delete m_pstText;

}

void TextPoint::Draw( ocpnDC& dc, wxPoint *rpn )
{
    ODPoint::Draw( dc, rpn );
    
    if( !m_bIsVisible )
        return;
    
    dc.SetTextForeground( *wxBLACK );
    //dc.SetBackground( *wxWHITE );
    wxPoint destPoint;
    GetCanvasPixLL( g_ocpn_draw_pi->m_vp, &destPoint, m_lat, m_lon);
    //if( m_pstText ) delete m_pstText;
//    m_pstText = new wxStaticText( g_ocpn_draw_pi->m_parent_window, wxID_ANY, m_MarkDescription, destPoint );
//    wxFont *pFont = OCPNGetFont(wxT("Marks"), 0);
//    m_pstText->SetFont( *pFont );
    m_pstText->SetPosition( destPoint );
    m_pstText->Clear();
    m_pstText->AppendText( m_MarkDescription );
    //dc.DrawText( m_MarkDescription, destPoint.x, destPoint.y);
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    //SetFont(wxFont(32, wxFONTFAMILY_SWISS,  wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    
    //dc.DrawText();
}
