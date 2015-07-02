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
#include <wx/dc.h>

extern ocpn_draw_pi     *g_ocpn_draw_pi;
extern PlugIn_ViewPort  *g_pivp;
extern PointMan     *g_pODPointMan;
extern ODText       *g_ODText;
extern int          g_iTextPosition;
extern int          g_iTextTopOffsetX;
extern int          g_iTextTopOffsetY;
extern int          g_iTextBottomOffsetX;
extern int          g_iTextBottomOffsetY;
extern int          g_iTextRightOffsetX;
extern int          g_iTextRightOffsetY;
extern int          g_iTextLeftOffsetX;
extern int          g_iTextLeftOffsetY;

//extern ChartCanvas  *ocpncc1;

TextPoint::TextPoint() : ODPoint()
{
    m_sTypeString = wxT("Text Point");
    switch ( g_iTextPosition )
    {
        case ID_TEXT_TOP:
            m_TextLocationOffsetX = g_iTextTopOffsetX;
            m_TextLocationOffsetY = g_iTextTopOffsetY;
            break;
        case ID_TEXT_BOTTOM:
            m_TextLocationOffsetX = g_iTextBottomOffsetX;
            m_TextLocationOffsetY = g_iTextBottomOffsetY;
            break;
        case ID_TEXT_RIGHT:
            m_TextLocationOffsetX = g_iTextRightOffsetX;
            m_TextLocationOffsetY = g_iTextRightOffsetY;
            break;
        case ID_TEXT_LEFT:
            m_TextLocationOffsetX = g_iTextLeftOffsetX;
            m_TextLocationOffsetY = g_iTextLeftOffsetY;
            break;
    }
    m_iTextPosition = g_iTextPosition;
    m_iWrapLen = 250;
    m_pDescriptionFont = NULL;
#ifdef ocpnUSE_GL
    m_iDescriptionTextTexture = 0;
#endif
    
    //m_pstText = new wxStaticText( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""));
}

TextPoint::TextPoint(const TextPoint& other)
{
    m_sTypeString = wxT("Text Point");
    m_TextLocationOffsetX = other.m_TextLocationOffsetX;
    m_TextLocationOffsetY = other.m_TextLocationOffsetY;
    m_iTextPosition = other.m_iTextPosition;
    m_iWrapLen = 250;
    m_pDescriptionFont = other.m_pDescriptionFont;
#ifdef ocpnUSE_GL
    m_iDescriptionTextTexture = other.m_iDescriptionTextTexture;
#endif
    
    //m_pstText = new wxStaticText( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""));
}

TextPoint::TextPoint( double lat, double lon, const wxString& icon_ident, const wxString& name,
                  const wxString &pGUID, bool bAddToList )
: ODPoint( lat, lon, icon_ident, name, pGUID, bAddToList )
{
    m_sTypeString = wxT("Text Point");
    switch ( g_iTextPosition )
    {
        case ID_TEXT_TOP:
            m_TextLocationOffsetX = g_iTextTopOffsetX;
            m_TextLocationOffsetY = g_iTextTopOffsetY;
            break;
        case ID_TEXT_BOTTOM:
            m_TextLocationOffsetX = g_iTextBottomOffsetX;
            m_TextLocationOffsetY = g_iTextBottomOffsetY;
            break;
        case ID_TEXT_RIGHT:
            m_TextLocationOffsetX = g_iTextRightOffsetX;
            m_TextLocationOffsetY = g_iTextRightOffsetY;
            break;
        case ID_TEXT_LEFT:
            m_TextLocationOffsetX = g_iTextLeftOffsetX;
            m_TextLocationOffsetY = g_iTextLeftOffsetY;
            break;
    }
    m_iTextPosition = g_iTextPosition;
    m_iWrapLen = 250;
    m_pDescriptionFont = NULL;
#ifdef ocpnUSE_GL
    m_iDescriptionTextTexture = 0;
#endif
    
    m_pstText = NULL;
    wxSize tSize;
    tSize.x = 250;
    tSize.y = 75;
//    m_pstText = new wxTextCtrl( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize ,wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE );
//    m_pstText = new wxTextCtrl( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT("text"), wxDefaultPosition, wxDefaultSize ,wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE );
    //m_pstText = new wxTextCtrl();
    //m_pstText->SetBackgroundStyle( wxBG_STYLE_TRANSPARENT );
    //m_pstText->Create( g_ODText, wxID_ANY, wxT("test"), wxPoint( 0, 0 ), tSize, wxTE_MULTILINE );
    
    //m_pstText = new wxStaticText( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT("test"), wxDefaultPosition, tSize, wxALIGN_LEFT );
    wxColour wxCol;
    GetGlobalColor( wxT("YELO1"), &wxCol );
    //m_pstText->SetBackgroundColour( wxCol );
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

    m_pDescriptionFont = OCPNGetFont( wxS("Marks"), 0 );
    m_FontColor = FontMgr::Get().GetFontColor( wxS( "Marks" ) );
    wxColour wxCol;
    GetGlobalColor( wxT("YELO1"), &wxCol );
    dc.SetFont( *m_pDescriptionFont );
    dc.SetTextForeground( wxCol );
    dc.SetBackground( wxBrush( wxCol ) );

    if( m_TextPointText.Len() > 0 ) {
        CalculateTextExtents();
        switch ( m_iTextPosition )
        {
            case ID_TEXT_TOP:
                m_TextLocationOffsetX = g_iTextTopOffsetX;
                m_TextLocationOffsetY = g_iTextTopOffsetY - ( m_TextExtents.y / 2 );
                break;
            case ID_TEXT_BOTTOM:
                m_TextLocationOffsetX = g_iTextBottomOffsetX;
                m_TextLocationOffsetY = g_iTextBottomOffsetY;
                break;
            case ID_TEXT_RIGHT:
                m_TextLocationOffsetX = g_iTextRightOffsetX;
                m_TextLocationOffsetY = g_iTextRightOffsetY;
                break;
            case ID_TEXT_LEFT:
                m_TextLocationOffsetX = g_iTextLeftOffsetX - ( m_TextExtents.x / 2 );
                m_TextLocationOffsetY = g_iTextLeftOffsetY;
                break;
        }
        int sx2 = m_pbmIcon->GetWidth() / 2;
        int sy2 = m_pbmIcon->GetHeight() / 2;
        
        //    Calculate the mark drawing extents
        wxPoint r;
        GetCanvasPixLL( g_pivp, &r,  m_lat, m_lon);    
        wxRect r1( r.x - sx2, r.y - sy2, sx2 * 2, sy2 * 2 );           // the bitmap extents
        if( m_pDescriptionFont ) {
            wxRect r2( r.x + m_TextLocationOffsetX, r.y + m_TextLocationOffsetY, m_TextExtents.x,
                    m_TextExtents.y );
            r1.Union( r2 );
        }
    
        r.x = r.x + m_TextLocationOffsetX;
        r.y = r.y + m_TextLocationOffsetY;
        
        if( m_pDescriptionFont ) {
            dc.SetFont( *m_pDescriptionFont );
            dc.SetTextForeground( m_FontColor );
            
            dc.DrawText( m_TextPointText, r.x + m_TextLocationOffsetX, r.y + m_TextLocationOffsetY );
        }
        
    /*    m_pstText->SetPosition( r );
        m_pstText->SetTransparent( wxIMAGE_ALPHA_OPAQUE );
        m_pstText->SetBackgroundColour( wxCol );
        m_pstText->ClearBackground();
        //m_pstText->SetForegroundColour( wxCol );
        
        m_pstText->Wrap( m_iWrapLen );
        
    //    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    //SetFont(wxFont(32, wxFONTFAMILY_SWISS,  wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        //dc.DrawText();
        if( m_MarkDescription.Len() == 0 )
            m_pstText->Show( false );
        else
            m_pstText->Show( true );

        wxBitmap tbm(m_TextExtents.x, m_TextExtents.y); /* render text on dc */
    /*    wxMemoryDC wxmdc;
    //    wxmdc.SetDeviceLocalOrigin( r.x, r.y );
        wxmdc.SelectObject( tbm );               
    //    wxBrush wxbrush( *wxWHITE );
    //    wxbrush.SetStyle( wxTRANSPARENT );
        wxmdc.SetBackgroundMode( wxTRANSPARENT );
        wxmdc.Clear();
        wxmdc.SetFont( *m_pDescriptionFont );
        wxmdc.SetTextForeground( *wxBLACK );
        wxmdc.DrawText( m_MarkDescription, 0, 0 );
        wxRasterOperationMode rom = wxCOPY;
        wxDC *pdc = dc.GetDC();
        DEBUGST("r.x: ");
        DEBUGCONT(r.x);
        DEBUGCONT(", r.y: ");
        DEBUGEND(r.y);
        pdc->Blit( r.x, r.y, m_TextExtents.x, m_TextExtents.y, &wxmdc, 0, 0, rom , true );
        wxmdc.delete;
    */    
    }
    ODPoint::Draw( dc, rpn );
}

void TextPoint::DrawGL( PlugIn_ViewPort &pivp )
{
    if( !m_bIsVisible )
        return;
    
    m_pDescriptionFont = OCPNGetFont( wxS("Marks"), 0 );
    m_FontColor = FontMgr::Get().GetFontColor( wxS( "Marks" ) );
    CalculateTextExtents();

    //    Substitue icon?
    wxBitmap *pbm;
    if( m_bIsActive ) pbm = g_pODPointMan->GetIconBitmap(
        _T ( "activepoint" ) );
    else
        pbm = m_pbmIcon;
    if( m_MarkDescription.Len() > 0 ) {
        int sx2 = pbm->GetWidth() / 2;
        int sy2 = pbm->GetHeight() / 2;
        
        //    Calculate the mark drawing extents
        wxPoint r;
        GetCanvasPixLL( &pivp, &r,  m_lat, m_lon);    
        wxRect r1( r.x - sx2, r.y - sy2, sx2 * 2, sy2 * 2 );           // the bitmap extents
        if( m_pDescriptionFont ) {
            wxRect r2( r.x + m_TextLocationOffsetX, r.y + m_TextLocationOffsetY, m_TextExtents.x,
                    m_TextExtents.y );
            r1.Union( r2 );
        }
        
        r.x = r.x + m_TextLocationOffsetX;
        r.y = r.y + m_TextLocationOffsetY;
        
        //m_pstText->SetPosition( r );
        
        m_pDescriptionFont = OCPNGetFont( wxS("Marks"), 0 );
        m_FontColor = FontMgr::Get().GetFontColor( wxS( "Marks" ) );
        CalculateTextExtents();
        if( !m_iDescriptionTextTexture && m_TextExtents.x != 0 && m_TextExtents.y != 0 ) {
            wxBitmap tbm(m_TextExtents.x, m_TextExtents.y); /* render text on dc */
            wxMemoryDC dc;
            dc.SelectObject( tbm );               
            dc.SetBackground( wxBrush( *wxBLACK ) );
            dc.Clear();
            dc.SetFont( *m_pDescriptionFont );
            dc.SetTextForeground( *wxWHITE );
            dc.DrawText( m_MarkDescription, 0, 0);
            dc.SelectObject( wxNullBitmap );
            
            /* make alpha texture for text */
            wxImage image = tbm.ConvertToImage();
            unsigned char *d = image.GetData();
            unsigned char *e = new unsigned char[m_TextExtents.x * m_TextExtents.y];
            if(d && e){
                for( int p = 0; p < m_TextExtents.x*m_TextExtents.y; p++)
                    e[p] = d[3*p + 0];
            }
            /* create texture for rendered text */
            glGenTextures(1, &m_iDescriptionTextTexture);
            glBindTexture(GL_TEXTURE_2D, m_iDescriptionTextTexture);
            
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            
            m_iDescriptionTextTextureWidth = NextPow2(m_TextExtents.x);
            m_iDescriptionTextTextureHeight = NextPow2(m_TextExtents.y);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_iDescriptionTextTextureWidth, m_iDescriptionTextTextureHeight,
                        0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_TextExtents.x, m_TextExtents.y,
                            GL_ALPHA, GL_UNSIGNED_BYTE, e);
            delete [] e;
            DEBUG("Creating iTexture");
        }
        
        if(m_iDescriptionTextTexture) {
            /* draw texture with text */
            glBindTexture(GL_TEXTURE_2D, m_iDescriptionTextTexture);
            
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glColor3ub(m_FontColor.Red(), m_FontColor.Green(), m_FontColor.Blue());
            
            int x = r.x + m_TextLocationOffsetX, y = r.y + m_TextLocationOffsetY;
            float u = (float)m_TextExtents.x/m_iDescriptionTextTextureWidth, v = (float)m_TextExtents.y/m_iDescriptionTextTextureHeight;
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(x, y);
            glTexCoord2f(u, 0); glVertex2f(x+m_TextExtents.x, y);
            glTexCoord2f(u, v); glVertex2f(x+m_TextExtents.x, y+m_TextExtents.y);
            glTexCoord2f(0, v); glVertex2f(x, y+m_TextExtents.y);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
    }
    }
    ODPoint::DrawGL( pivp );
}

void TextPoint::CalculateTextExtents( void )
{
    if( m_pDescriptionFont ) {
        wxScreenDC dc;
        
        dc.SetFont( *m_pDescriptionFont );
        m_TextExtents = dc.GetMultiLineTextExtent( m_TextPointText );
    } else
        m_TextExtents = wxSize( 0, 0 );
    
}

/*void TextPoint::SetMarkDescription( wxString sMarkDescription )
{
    ODPoint::SetMarkDescription( sMarkDescription );
    return;
    bool bMarkup;
    bMarkup = m_pstText->SetLabelMarkup( sMarkDescription );
    if (!bMarkup)
        m_pstText->SetLabel( sMarkDescription );
    ShowText();
    m_iDescriptionTextTexture = 0;
}
*/
void TextPoint::SetPointText( wxString sTextPointText )
{
    m_TextPointText = ( sTextPointText );
    return;
    bool bMarkup;
    bMarkup = m_pstText->SetLabelMarkup( sTextPointText );
    if (!bMarkup)
        m_pstText->SetLabel( sTextPointText );
    ShowText();
    m_iDescriptionTextTexture = 0;
}

void TextPoint::ShowText( void )
{
    return;
    if( m_MarkDescription.Len() == 0 )
        m_pstText->Show( false );
  //  else
//        m_pstText->Show( true );
}

void TextPoint::HideText( void )
{
    m_pstText->Show( false );
}