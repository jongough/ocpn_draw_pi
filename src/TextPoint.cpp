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
#include "ODdc.h"
#include "ODUtils.h"
#include "PointMan.h"
#include "cutil.h"

#include <wx/stattext.h>
#include <wx/dc.h>

#include "GL/gl.h"

// Fix for VS2010 not having the round function
#if defined (_WIN32)
#if defined (_MSC_VER)
#if _MSC_VER < 1700
int round (double x) {
    int i = (int) x;
    if (x >= 0.0) {
        return ((x-i) >= 0.5) ? (i + 1) : (i);
    } else {
        return (-x+i >= 0.5) ? (i - 1) : (i);
    }
}
#endif
#endif
#endif

TextPoint::TextPoint() : ODPoint()
{
    m_sTypeString = wxT("Text Point");
    m_bShowName = g_bTextPointShowName;
    m_bIsolatedMark = true;
    switch ( g_iTextPosition )
    {
        case ID_TEXT_TOP:
        case ID_TEXT_CENTRE_TOP:
            m_TextLocationOffsetX = g_iTextTopOffsetX;
            m_TextLocationOffsetY = g_iTextTopOffsetY;
            break;
        case ID_TEXT_BOTTOM:
        case ID_TEXT_CENTRE_BOTTOM:
            m_TextLocationOffsetX = g_iTextBottomOffsetX;
            m_TextLocationOffsetY = g_iTextBottomOffsetY;
            break;
        case ID_TEXT_CENTRE:
            m_TextLocationOffsetX = g_iTextCentreOffsetX;
            m_TextLocationOffsetY = g_iTextCentreOffsetY;
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
    m_iBackgroundTransparency = g_iTextBackgroundTransparency;
    m_colourTextColour = g_colourDefaultTextColour;
    m_colourTextBackgroundColour = g_colourDefaultTextBackgroundColour;
    m_iWrapLen = g_iTextMaxWidth;
    m_iTextMaxWidthType = g_iTextMaxWidthType;
    m_DisplayTextFont = g_DisplayTextFont;
    m_bTextChanged = true;
    m_bShowDisplayTextOnRollover = false;
    m_iDisplayTextWhen = g_iTextPointDisplayTextWhen;
#ifdef ocpnUSE_GL
    m_iDisplayTextTexture = 0;
#endif
    
    m_natural_scale = g_ocpn_draw_pi->m_chart_scale;
    m_scale_factor = 0;

    CreateColourSchemes();
    SetColourScheme();
    
    //m_pstText = new wxStaticText( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""));
}

TextPoint::TextPoint(const TextPoint& other) : ODPoint(other)
{
    m_sTypeString = wxT("Text Point");
    m_TextLocationOffsetX = other.m_TextLocationOffsetX;
    m_TextLocationOffsetY = other.m_TextLocationOffsetY;
    m_iTextPosition = other.m_iTextPosition;
    m_iBackgroundTransparency = other.m_iBackgroundTransparency;
    m_colourTextColour = other.m_colourTextColour;
    m_colourTextBackgroundColour = other.m_colourTextBackgroundColour;
    m_iWrapLen = other.m_iWrapLen;
    m_iTextMaxWidthType = other.m_iTextMaxWidthType;
    m_DisplayTextFont = other.m_DisplayTextFont;
    m_bTextChanged = true;
    m_iDisplayTextWhen = other.m_iDisplayTextWhen;
#ifdef ocpnUSE_GL
    m_iDisplayTextTexture = other.m_iDisplayTextTexture;
#endif
    m_natural_scale = other.m_natural_scale;
    m_scale_factor = other.m_scale_factor;
    m_bShowDisplayTextOnRollover = other.m_bShowDisplayTextOnRollover;
    
    CreateColourSchemes();
    SetColourScheme();

    //m_pstText = new wxStaticText( g_ocpn_draw_pi->m_parent_window, wxID_ANY, wxT(""));
}

TextPoint::TextPoint( double lat, double lon, const wxString& icon_ident, const wxString& name,
                  const wxString &pGUID, bool bAddToList )
: ODPoint( lat, lon, icon_ident, name, pGUID, bAddToList )
{
    m_sTypeString = wxT("Text Point");
    m_bIsolatedMark = true;
    switch ( g_iTextPosition )
    {
        case ID_TEXT_TOP:
        case ID_TEXT_CENTRE_TOP:
            m_TextLocationOffsetX = g_iTextTopOffsetX;
            m_TextLocationOffsetY = g_iTextTopOffsetY;
            break;
        case ID_TEXT_BOTTOM:
        case ID_TEXT_CENTRE_BOTTOM:
            m_TextLocationOffsetX = g_iTextBottomOffsetX;
            m_TextLocationOffsetY = g_iTextBottomOffsetY;
            break;
        case ID_TEXT_CENTRE:
            m_TextLocationOffsetX = g_iTextCentreOffsetX;
            m_TextLocationOffsetY = g_iTextCentreOffsetY;
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
    m_iBackgroundTransparency = g_iTextBackgroundTransparency;
    m_colourTextColour = g_colourDefaultTextColour;
    m_colourTextBackgroundColour = g_colourDefaultTextBackgroundColour;
    m_iWrapLen = g_iTextMaxWidth;
    m_iTextMaxWidthType = g_iTextMaxWidthType;
    if(g_DisplayTextFont.IsOk()) m_DisplayTextFont = g_DisplayTextFont;
    m_bTextChanged = true;
    m_bShowDisplayTextOnRollover = false;
    m_iDisplayTextWhen = g_iTextPointDisplayTextWhen;
#ifdef ocpnUSE_GL
    m_iDisplayTextTexture = 0;
#endif
    
    m_pstText = NULL;
    wxSize tSize;
    tSize.x = 250;
    tSize.y = 75;
    
    wxColour wxCol;
    GetGlobalColor( wxT("YELO1"), &wxCol );
    
    m_natural_scale = g_ocpn_draw_pi->m_chart_scale;
    m_scale_factor = 0;

    CreateColourSchemes();
    SetColourScheme();

}
TextPoint::~TextPoint()
{
    if( m_pstText ) delete m_pstText;

}

void TextPoint::CreateColourSchemes(void)
{
    m_colourTextColourRBG = m_colourTextColour;
    m_colourTextBackgroundColourRGB = m_colourTextBackgroundColour;
    m_colourTextColourDay = m_colourTextColour;
    m_colourTextBackgroundColourDay = m_colourTextBackgroundColour;
    m_colourTextColourDusk.Set( m_colourTextColour.Red()/2, m_colourTextColour.Green()/2, m_colourTextColour.Blue()/2, m_colourTextColour.Alpha());
    m_colourTextBackgroundColourDusk.Set( m_colourTextBackgroundColour.Red()/2, m_colourTextBackgroundColour.Green()/2, m_colourTextBackgroundColour.Blue()/2, m_colourTextBackgroundColour.Alpha());
    m_colourTextColourNight.Set( m_colourTextColour.Red()/4, m_colourTextColour.Green()/4, m_colourTextColour.Blue()/4, m_colourTextColour.Alpha());
    m_colourTextBackgroundColourNight.Set( m_colourTextBackgroundColour.Red()/4, m_colourTextBackgroundColour.Green()/4, m_colourTextBackgroundColour.Blue()/4, m_colourTextBackgroundColour.Alpha());

    ODPoint::CreateColourSchemes();
}

void TextPoint::Draw( ODDC& dc, wxPoint *odp)
{
    if( !m_bIsVisible )
        return;
    
    if( m_iDisplayTextWhen == ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ALWAYS || 
        ( m_iDisplayTextWhen == ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ON_ROLLOVER && m_bShowDisplayTextOnRollover)  ) {
        if( m_TextPointText.Len() > 0 ) {
            wxString l_TextPointText;
            if(m_TextPointText.Len() > (size_t)m_iWrapLen)
                if(m_iTextMaxWidthType == 0)
                    l_TextPointText = WrapText(g_parent_window, m_TextPointText, m_iWrapLen);
                else
                    l_TextPointText = WrapString(m_TextPointText, m_iWrapLen);
            else 
                l_TextPointText = m_TextPointText;
            
            CalculateTextExtents(l_TextPointText);

            int teX, teY;
            int scalefactor = round(g_ocpn_draw_pi->m_chart_scale / m_natural_scale);
            
            if(m_natural_scale > (g_ocpn_draw_pi->m_chart_scale / 2) ) {
                teX = m_TextExtents.x;
                teY = m_TextExtents.y;
            } else {
                teX = m_TextExtents.x / scalefactor;
                teY = m_TextExtents.y / scalefactor;
            }
            
            if(teX > 0 && teY > 0 && scalefactor <= 8) {
                switch ( m_iTextPosition )
                {
                    case ID_TEXT_TOP:
                        m_TextLocationOffsetX = g_iTextTopOffsetX;
                        m_TextLocationOffsetY = g_iTextTopOffsetY - teY;
                        break;
                    case ID_TEXT_CENTRE_TOP:
                        m_TextLocationOffsetX = g_iTextTopOffsetX - (teX / 2);
                        m_TextLocationOffsetY = g_iTextTopOffsetY - teY;
                        break;
                    case ID_TEXT_BOTTOM:
                        m_TextLocationOffsetX = g_iTextBottomOffsetX;
                        m_TextLocationOffsetY = g_iTextBottomOffsetY;
                        if(m_bShowName) m_TextLocationOffsetY += g_iTextBottomNameExtraOffsetY;
                        break;
                    case ID_TEXT_CENTRE_BOTTOM:
                        m_TextLocationOffsetX = g_iTextBottomOffsetX - (teX / 2);
                        m_TextLocationOffsetY = g_iTextBottomOffsetY;
                        break;
                    case ID_TEXT_CENTRE:
                        m_TextLocationOffsetX = g_iTextCentreOffsetX - (teX / 2);
                        m_TextLocationOffsetY = g_iTextCentreOffsetY - (teY / 2);
                        break;
                    case ID_TEXT_RIGHT:
                        m_TextLocationOffsetX = g_iTextRightOffsetX;
                        m_TextLocationOffsetY = g_iTextRightOffsetY;
                        break;
                    case ID_TEXT_LEFT:
                        m_TextLocationOffsetX = g_iTextLeftOffsetX - teX;
                        m_TextLocationOffsetY = g_iTextLeftOffsetY;
                        break;
                }
                int sx2 = m_pbmIcon->GetWidth() / 2;
                int sy2 = m_pbmIcon->GetHeight() / 2;
                
                //    Calculate the mark drawing extents
                wxPoint r;
                GetCanvasPixLL( &g_VP, &r,  m_lat, m_lon);    
                wxRect r1( r.x - sx2, r.y - sy2, sx2 * 2, sy2 * 2 );           // the bitmap extents
                if( m_DisplayTextFont.IsOk() ) {
                    // Added to help with display of text (stops end clipping)
                    teX += 20;
                    wxRect r2( r.x + m_TextLocationOffsetX, r.y + m_TextLocationOffsetY, teX,
                            teY );
                    r1.Union( r2 );
            
                    r.x = r.x + m_TextLocationOffsetX;
                    r.y = r.y + m_TextLocationOffsetY;
                
                    dc.SetFont( m_DisplayTextFont );
                    dc.SetTextForeground( m_colourSchemeTextColour );
                    g_ocpn_draw_pi->AlphaBlending( dc, r.x, r.y, r2.width, r2.height, 6.0, m_colourSchemeTextBackgroundColour, m_iBackgroundTransparency );
                    if(m_natural_scale > (g_ocpn_draw_pi->m_chart_scale / 2) )
                        dc.DrawText( l_TextPointText, r.x + 10, r.y );
                    else 
                        dc.DrawText( wxT(" "), r.x + 10, r.y );
                }
            }
        }
    }
    ODPoint::Draw( dc, odp);
}

void TextPoint::DrawGL( PlugIn_ViewPort &pivp )
{
    if( !m_bIsVisible )
        return;
    
    if( m_iDisplayTextWhen == ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ALWAYS || 
        ( m_iDisplayTextWhen == ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ON_ROLLOVER && m_bShowDisplayTextOnRollover)  ) {
        if( m_TextPointText.Len() > 0 ) {
            wxString l_TextPointText;
            if(m_TextPointText.Len() > (size_t)m_iWrapLen)
                if(m_iTextMaxWidthType == 0)
                    l_TextPointText = WrapText(g_parent_window, m_TextPointText, m_iWrapLen);
                else
                    l_TextPointText = WrapString(m_TextPointText, m_iWrapLen);
                else 
                l_TextPointText = m_TextPointText;

            CalculateTextExtents(l_TextPointText);
            
            int teX, teY;
            int scalefactor = round(g_ocpn_draw_pi->m_chart_scale / m_natural_scale);
            
            if(m_natural_scale > (g_ocpn_draw_pi->m_chart_scale / 2) ) {
                teX = m_TextExtents.x;
                teY = m_TextExtents.y;
            } else {
                teX = m_TextExtents.x / scalefactor;
                teY = m_TextExtents.y / scalefactor;
            }
            
            if(teX > 0 && teY > 0 && scalefactor <= 8 ) {
                switch ( m_iTextPosition )
                {
                    case ID_TEXT_TOP:
                        m_TextLocationOffsetX = g_iTextTopOffsetX;
                        m_TextLocationOffsetY = g_iTextTopOffsetY - teY;
                        break;
                    case ID_TEXT_CENTRE_TOP:
                        m_TextLocationOffsetX = g_iTextTopOffsetX - (teX / 2);
                        m_TextLocationOffsetY = g_iTextTopOffsetY - teY;
                        break;
                    case ID_TEXT_BOTTOM:
                        m_TextLocationOffsetX = g_iTextBottomOffsetX;
                        m_TextLocationOffsetY = g_iTextBottomOffsetY;
                        if(m_bShowName) m_TextLocationOffsetY += g_iTextBottomNameExtraOffsetY;
                        break;
                    case ID_TEXT_CENTRE_BOTTOM:
                        m_TextLocationOffsetX = g_iTextBottomOffsetX - (teX / 2);
                        m_TextLocationOffsetY = g_iTextBottomOffsetY;
                        break;
                    case ID_TEXT_CENTRE:
                        m_TextLocationOffsetX = g_iTextCentreOffsetX - (teX / 2);
                        m_TextLocationOffsetY = g_iTextCentreOffsetY - (teY / 2);
                        break;
                    case ID_TEXT_RIGHT:
                        m_TextLocationOffsetX = g_iTextRightOffsetX;
                        m_TextLocationOffsetY = g_iTextRightOffsetY;
                        break;
                    case ID_TEXT_LEFT:
                        m_TextLocationOffsetX = g_iTextLeftOffsetX - teX;
                        m_TextLocationOffsetY = g_iTextLeftOffsetY;
                        break;
                }
                int sx2 = m_pbmIcon->GetWidth() / 2;
                int sy2 = m_pbmIcon->GetHeight() / 2;
                
                //    Calculate the mark drawing extents
                wxPoint r;
                GetCanvasPixLL( &g_VP, &r,  m_lat, m_lon);    
                wxRect r1( r.x - sx2, r.y - sy2, sx2 * 2, sy2 * 2 );           // the bitmap extents
                if( m_DisplayTextFont.IsOk() ) {
                    // Added to help with display of text (stops end clipping)
                    teX += 20;
                    wxRect r2( r.x + m_TextLocationOffsetX, r.y + m_TextLocationOffsetY, teX,
                            teY );
                    r1.Union( r2 );
                    
                    r.x = r.x + m_TextLocationOffsetX;
                    r.y = r.y + m_TextLocationOffsetY;
                    if( scalefactor != m_scale_factor || m_bTextChanged || ( !m_iDisplayTextTexture && teX != 0 && teY != 0 ) ) {
                        m_scale_factor = scalefactor;
                        m_bTextChanged = false;
                        wxBitmap tbm(teX, teY); /* render text on dc */
                        wxMemoryDC dc;
                        dc.SelectObject( tbm );               
                        dc.SetBackground( wxBrush( *wxBLACK ) );
                        dc.Clear();
                        dc.SetFont( m_DisplayTextFont );
                        dc.SetTextForeground(* wxWHITE );
                        if(m_natural_scale > (g_ocpn_draw_pi->m_chart_scale / 2))
                            dc.DrawText( l_TextPointText, 10, 0);
                        //else
                        //    dc.DrawText( wxT(""), 10, 0 );
                        dc.SelectObject( wxNullBitmap );
                        
                        /* make alpha texture for text */
                        wxImage image = tbm.ConvertToImage();
                        unsigned char *d = image.GetData();
                        unsigned char *e = new unsigned char[teX * teY];
                        if(d && e){
                            for( int p = 0; p < teX*teY; p++)
                                e[p] = d[3*p + 0];
                        }
                        /* create texture for rendered text */
                        glGenTextures(1, &m_iDisplayTextTexture);
                        glBindTexture(GL_TEXTURE_2D, m_iDisplayTextTexture);
                        
                        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
                        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                        
                        m_iDisplayTextTextureWidth = NextPow2(teX);
                        m_iDisplayTextTextureHeight = NextPow2(teY);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_iDisplayTextTextureWidth, m_iDisplayTextTextureHeight,
                                    0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);
                        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, teX, teY,
                                        GL_ALPHA, GL_UNSIGNED_BYTE, e);
                        delete [] e;
                    }
                    
                    if(m_iDisplayTextTexture) {
                        // Draw backing box
                        ODDC ocpndc;
                        g_ocpn_draw_pi->AlphaBlending( ocpndc, r.x, r.y, r2.width, r2.height, 6.0, m_colourSchemeTextBackgroundColour, m_iBackgroundTransparency );
                                
                        /* draw texture with text */
                        glBindTexture(GL_TEXTURE_2D, m_iDisplayTextTexture);
                        
                        glEnable(GL_TEXTURE_2D);
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        
                        glColor3ub(m_colourSchemeTextColour.Red(), m_colourSchemeTextColour.Green(), m_colourSchemeTextColour.Blue());
                        
                        int x = r.x, y = r.y;
                        float u = (float)teX/m_iDisplayTextTextureWidth, v = (float)teY/m_iDisplayTextTextureHeight;
                        glBegin(GL_QUADS);
                        glTexCoord2f(0, 0); glVertex2f(x, y);
                        glTexCoord2f(u, 0); glVertex2f(x+teX, y);
                        glTexCoord2f(u, v); glVertex2f(x+teX, y+teY);
                        glTexCoord2f(0, v); glVertex2f(x, y+teY);
                        glEnd();
                        glDisable(GL_BLEND);
                        glDisable(GL_TEXTURE_2D);
                    }

                }
            }
        }
    }
    ODPoint::DrawGL( pivp );
}

void TextPoint::CalculateTextExtents( void )
{
    if( m_DisplayTextFont.IsOk() ) {
        wxScreenDC dc;
        
        dc.SetFont( m_DisplayTextFont );
        m_TextExtents = dc.GetMultiLineTextExtent( m_TextPointText );
    } else
        m_TextExtents = wxSize( 0, 0 );
    
}

void TextPoint::CalculateTextExtents( wxString TextPointText )
{
    if( m_DisplayTextFont.IsOk() ) {
        wxScreenDC dc;
        
        dc.SetFont( m_DisplayTextFont );
        m_TextExtents = dc.GetMultiLineTextExtent( TextPointText );
    } else
        m_TextExtents = wxSize( 0, 0 );
    
}

void TextPoint::SetPointText( wxString sTextPointText )
{
    m_TextPointText = ( sTextPointText );
    return;
}

void TextPoint::SetColourScheme(PI_ColorScheme cs)
{
    switch (cs) {
        case PI_GLOBAL_COLOR_SCHEME_RGB:
            m_colourSchemeTextColour = m_colourTextColourRBG;
            m_colourSchemeTextBackgroundColour = m_colourTextBackgroundColourRGB;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DAY:
            m_colourSchemeTextColour = m_colourTextColourDay;
            m_colourSchemeTextBackgroundColour = m_colourTextBackgroundColourDay;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DUSK:
            m_colourSchemeTextColour = m_colourTextColourDusk;
            m_colourSchemeTextBackgroundColour = m_colourTextBackgroundColourDusk;
            break;
        case PI_GLOBAL_COLOR_SCHEME_NIGHT:
            m_colourSchemeTextColour = m_colourTextColourNight;
            m_colourSchemeTextBackgroundColour = m_colourTextBackgroundColourNight;
            break;
        default:
            m_colourSchemeTextColour = m_colourTextColourDay;
            m_colourSchemeTextBackgroundColour = m_colourTextBackgroundColourDay;
            break;
    }
    ODPoint::SetColourScheme(cs);
}
