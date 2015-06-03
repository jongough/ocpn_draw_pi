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
#include "PointMan.h"

#include <wx/stattext.h>

extern ocpn_draw_pi     *g_ocpn_draw_pi;
extern PlugIn_ViewPort  *g_pivp;
extern PointMan     *g_pODPointMan;

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
    m_DescriptionLocationOffsetX = -10;
    m_DescriptionLocationOffsetY = 8;
    
    wxSize tSize;
    tSize.x = 250;
    tSize.y = 75;
    m_pstText = new wxTextCtrl( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT("Test"), wxDefaultPosition, wxDefaultSize ,wxTE_MULTILINE | wxTE_READONLY );
    m_pstText->SetDefaultStyle(wxTextAttr(*wxBLACK, wxNullColour));
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
    
    m_pMarkFont = OCPNGetFont( wxS("Marks"), 0 );
    m_FontColor = FontMgr::Get().GetFontColor( wxS( "Marks" ) );
    CalculateDescriptionExtents();
    dc.SetFont( *m_pMarkFont );
    dc.SetTextForeground( m_FontColor );

    
    //    Substitue icon?
    wxBitmap *pbm;
    if( m_bIsActive ) pbm = g_pODPointMan->GetIconBitmap(
        _T ( "activepoint" ) );
    else
        pbm = m_pbmIcon;
    
    int sx2 = pbm->GetWidth() / 2;
    int sy2 = pbm->GetHeight() / 2;
    
    //    Calculate the mark drawing extents
    wxPoint r;
    GetCanvasPixLL( g_pivp, &r,  m_lat, m_lon);    
    wxRect r1( r.x - sx2, r.y - sy2, sx2 * 2, sy2 * 2 );           // the bitmap extents
    if( m_pMarkFont ) {
        wxRect r2( r.x + m_DescriptionLocationOffsetX, r.y + m_DescriptionLocationOffsetY, m_DescriptionExtents.x,
                   m_DescriptionExtents.y );
        r1.Union( r2 );
    }
   
   r.x = r.x + m_DescriptionLocationOffsetX;
   r.y = r.y + m_DescriptionLocationOffsetY;
   m_pstText->SetPosition( r );
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    //SetFont(wxFont(32, wxFONTFAMILY_SWISS,  wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    
    //dc.DrawText();
}

void TextPoint::CalculateDescriptionExtents( void )
{
    if( m_pMarkFont ) {
        wxScreenDC dc;
        
        dc.SetFont( *m_pMarkFont );
        m_DescriptionExtents = dc.GetTextExtent( m_MarkDescription );
    } else
        m_DescriptionExtents = wxSize( 0, 0 );
    
}

void TextPoint::SetMarkDescription( wxString sMarkDescription )
{
    ODPoint::SetMarkDescription( sMarkDescription );
    m_pstText->Clear();
    m_pstText->WriteText( sMarkDescription );
}