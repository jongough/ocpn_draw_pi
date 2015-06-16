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
#include "TextPoint.h"
#include "cutil.h"

#include <wx/stattext.h>

extern ocpn_draw_pi     *g_ocpn_draw_pi;
extern PlugIn_ViewPort  *g_pivp;
extern PointMan     *g_pODPointMan;
extern ODText       *g_ODText;

//extern ChartCanvas  *ocpncc1;

TextPoint::TextPoint() : ODPoint()
{
    m_sTypeString = wxT("Text Point");
    m_DescriptionLocationOffsetX = -10;
    m_DescriptionLocationOffsetY = 8;
    m_iWrapLen = 250;

    m_pstText = new wxStaticText( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""));
}

TextPoint::TextPoint(const TextPoint& other)
{
    m_sTypeString = wxT("Text Point");
    m_DescriptionLocationOffsetX = -10;
    m_DescriptionLocationOffsetY = 8;
    m_iWrapLen = 250;
    
    m_pstText = new wxStaticText( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""));
}

TextPoint::TextPoint( double lat, double lon, const wxString& icon_ident, const wxString& name,
                  const wxString &pGUID, bool bAddToList )
: ODPoint( lat, lon, icon_ident, name, pGUID, bAddToList )
{
    m_sTypeString = wxT("Text Point");
    m_DescriptionLocationOffsetX = -10;
    m_DescriptionLocationOffsetY = 8;

    m_iWrapLen = 250;
    m_pstText = NULL;
    wxSize tSize;
    tSize.x = 250;
    tSize.y = 75;
//    m_pstText = new wxTextCtrl( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize ,wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE );
//    m_pstText = new wxTextCtrl( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT("text"), wxDefaultPosition, wxDefaultSize ,wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE );
    //m_pstText = new wxTextCtrl();
    //m_pstText->SetBackgroundStyle( wxBG_STYLE_TRANSPARENT );
    //m_pstText->Create( g_ODText, wxID_ANY, wxT("test"), wxPoint( 0, 0 ), tSize, wxTE_MULTILINE );
    m_pstText = new wxStaticText( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT("test"), wxDefaultPosition, tSize, wxALIGN_LEFT );
    wxColour wxCol;
    GetGlobalColor( wxT("YELO1"), &wxCol );
    m_pstText->SetBackgroundColour( wxCol );
//    bool btrans;
//    wxString *reason;
//    btrans = m_pstText->IsTransparentBackgroundSupported( reason );
//    btrans = m_pstText->HasTransparentBackground();
//    btrans = m_pstText->CanSetTransparent();
//    btrans = false;
//    btrans = m_pstText->IsTransparentBackgroundSupported();
//    m_pstText->SetTransparent( 0 );
}
TextPoint::~TextPoint()
{
    if( m_pstText ) delete m_pstText;

}

void TextPoint::Draw( ocpnDC& dc, wxPoint *rpn )
{
    
    if( !m_bIsVisible )
        return;
    
    m_pMarkFont = OCPNGetFont( wxS("Marks"), 0 );
    m_FontColor = FontMgr::Get().GetFontColor( wxS( "Marks" ) );
    CalculateDescriptionExtents();
    wxColour wxCol;
    GetGlobalColor( wxT("YELO1"), &wxCol );
    dc.SetFont( *m_pMarkFont );
//    dc.SetTextForeground( m_FontColor );
    dc.SetTextForeground( wxCol );
    dc.SetBackground( wxBrush( wxCol ) );

    
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
    m_pstText->SetTransparent( wxIMAGE_ALPHA_OPAQUE );
    m_pstText->SetBackgroundColour( wxCol );
    m_pstText->ClearBackground();
    //m_pstText->SetForegroundColour( wxCol );
    
    m_pstText->Wrap( m_iWrapLen );
    
//    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
   //SetFont(wxFont(32, wxFONTFAMILY_SWISS,  wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    //dc.DrawText();
    
    ODPoint::Draw( dc, rpn );
}

void TextPoint::DrawGL( PlugIn_ViewPort &pivp )
{
    ODPoint::DrawGL( pivp );
    
    if( !m_bIsVisible )
        return;
    
    m_pMarkFont = OCPNGetFont( wxS("Marks"), 0 );
    m_FontColor = FontMgr::Get().GetFontColor( wxS( "Marks" ) );
    CalculateDescriptionExtents();
    wxBitmap tbm(m_DescriptionExtents.y, m_DescriptionExtents.x); /* render text on dc */
    wxMemoryDC dc;
    dc.SelectObject( tbm );               
    dc.SetBackground( wxBrush( *wxBLACK ) );
    dc.Clear();
    dc.SetFont( *m_pMarkFont );
    dc.SetTextForeground( *wxWHITE );
    dc.DrawText( m_MarkName, 0, 0);
    dc.SelectObject( wxNullBitmap );
    
    /* make alpha texture for text */
    wxImage image = tbm.ConvertToImage();
    unsigned char *d = image.GetData();
    unsigned char *e = new unsigned char[m_DescriptionExtents.x * m_DescriptionExtents.y];
    if(d && e){
        for( int p = 0; p < m_DescriptionExtents.x*m_DescriptionExtents.y; p++)
            e[p] = d[3*p + 0];
    }
    
    /* create texture for rendered text */
    glGenTextures(1, &m_iTextTexture);
    glBindTexture(GL_TEXTURE_2D, m_iTextTexture);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    
    m_iTextTextureWidth = NextPow2(m_DescriptionExtents.x);
    m_iTextTextureHeight = NextPow2(m_DescriptionExtents.y);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_iTextTextureWidth, m_iTextTextureHeight,
                 0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_DescriptionExtents.x, m_DescriptionExtents.y,
                    GL_ALPHA, GL_UNSIGNED_BYTE, e);
    delete [] e;
    
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
    bool bMarkup;
    bMarkup = m_pstText->SetLabelMarkup( sMarkDescription );
    if (!bMarkup)
        m_pstText->SetLabel( sMarkDescription );
    if( m_MarkDescription.Len() == 0 )
        m_pstText->Show( false );
    else
        m_pstText->Show( true );
}