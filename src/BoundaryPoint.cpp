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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers


#include "BoundaryPoint.h"
#include "georef.h"
#include "ODdc.h"
#include "ocpn_draw_pi.h"

#include "GL/gl.h"
#include <wx/graphics.h>

extern PlugIn_ViewPort  *g_pVP;
extern ocpn_draw_pi     *g_ocpn_draw_pi;
extern bool         g_bExclusionBoundaryPoint;
extern bool         g_bInclusionBoundaryPoint;
extern int          g_iInclusionBoundaryPointSize;
extern unsigned int g_uiBoundaryPointFillTransparency;
extern int         g_iBoundaryPointRangeRingLineWidth;
extern int         g_iBoundaryPointRangeRingLineStyle;


BoundaryPoint::BoundaryPoint(double lat, double lon, const wxString& icon_ident, const wxString& name, const wxString& pGUID, bool bAddToList)
: ODPoint( lat, lon, icon_ident, name, pGUID, bAddToList )
{
    m_sTypeString = wxT("Boundary Point");
    m_uiBoundaryPointFillTransparency = g_uiBoundaryPointFillTransparency;
    m_bExclusionBoundaryPoint = g_bExclusionBoundaryPoint;
    m_bInclusionBoundaryPoint = g_bInclusionBoundaryPoint;
    m_iInclusionBoundaryPointSize = g_iInclusionBoundaryPointSize;
    m_iRangeRingWidth = g_iBoundaryPointRangeRingLineWidth;
    m_iRangeRingStyle = g_iBoundaryPointRangeRingLineStyle;
}

BoundaryPoint::BoundaryPoint(BoundaryPoint* orig) : ODPoint( orig )
{
    m_sTypeString = orig->m_sTypeString;
}

BoundaryPoint::BoundaryPoint() : ODPoint()
{
    m_sTypeString = wxT("Boundary Point");
    m_bInclusionBoundaryPoint = g_bInclusionBoundaryPoint;
    m_iInclusionBoundaryPointSize = g_iInclusionBoundaryPointSize;
    m_iRangeRingWidth = g_iBoundaryPointRangeRingLineWidth;
    m_iRangeRingStyle = g_iBoundaryPointRangeRingLineStyle;
}

void BoundaryPoint::Draw(ODDC& dc, wxPoint* rpn )
{
    wxPoint r;
    GetCanvasPixLL( g_pVP, &r,  m_lat, m_lon);
    
    double factor = 1.00;
    if( m_iODPointRangeRingsStepUnits == 1 )          // nautical miles
        factor = 1 / 1.852;
    
    factor *= m_fODPointRangeRingsStep;
    
    double tlat, tlon;
    wxPoint r1;
    ll_gc_ll( m_lat, m_lon, 0, factor, &tlat, &tlon );
    GetCanvasPixLL( g_pVP, &r1,  tlat, tlon);
    
    double lpp = sqrt( pow( (double) (r.x - r1.x), 2) +
    pow( (double) (r.y - r1.y), 2 ) );
    int pix_radius = (int) lpp * m_iODPointRangeRingsNumber;
    
    // fill boundary with hatching
    if ( m_bIsVisible && (m_bExclusionBoundaryPoint || m_bInclusionBoundaryPoint) && pix_radius > 10 && m_iODPointRangeRingsNumber && m_bShowODPointRangeRings ) {
        wxColour tCol;
        tCol.Set(m_wxcODPointRangeRingsColour.Red(), m_wxcODPointRangeRingsColour.Green(), m_wxcODPointRangeRingsColour.Blue(), m_uiBoundaryPointFillTransparency);
        if(m_bExclusionBoundaryPoint && !m_bInclusionBoundaryPoint) {
            wxPen savePen = dc.GetPen();
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxBRUSHSTYLE_CROSSDIAG_HATCH ) );
            dc.DrawDisk( r.x, r.y, 0, pix_radius );
            dc.SetPen( savePen );
        } else if(!m_bExclusionBoundaryPoint && m_bInclusionBoundaryPoint){
            wxPen savePen = dc.GetPen();
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxBRUSHSTYLE_CROSSDIAG_HATCH ) );
            dc.DrawDisk( r.x, r.y, pix_radius, pix_radius + m_iInclusionBoundaryPointSize );
            dc.SetPen( savePen );
        }
    }

    ODPoint::Draw( dc, rpn );
}

void BoundaryPoint::DrawGL(PlugIn_ViewPort& pivp)
{
#ifdef ocpnUSE_GL
    ODDC dc;
    
    wxPoint r;
    GetCanvasPixLL( g_pVP, &r,  m_lat, m_lon);
    
    double factor = 1.00;
    if( m_iODPointRangeRingsStepUnits == 1 )          // nautical miles
        factor = 1 / 1.852;
    
    factor *= m_fODPointRangeRingsStep;
    
    double tlat, tlon;
    wxPoint r1;
    ll_gc_ll( m_lat, m_lon, 0, factor, &tlat, &tlon );
    GetCanvasPixLL( g_pVP, &r1,  tlat, tlon);
    
    double lpp = sqrt( pow( (double) (r.x - r1.x), 2) +
    pow( (double) (r.y - r1.y), 2 ) );
    int pix_radius = (int) lpp * m_iODPointRangeRingsNumber;
    
    // Each byte represents a single pixel for Alpha. This provides a cross hatch in a 16x16 pixel square
    GLubyte slope_cross_hatch[] = {
        0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
        0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
        0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00,
        0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF
    };        
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture( GL_TEXTURE_2D, textureID );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, 16, 16, 0, GL_ALPHA, GL_UNSIGNED_BYTE, slope_cross_hatch );
    dc.SetTextureSize( 16, 16 );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // fill boundary with hatching
    if ( m_bIsVisible && (m_bExclusionBoundaryPoint || m_bInclusionBoundaryPoint) && pix_radius > 10 && m_iODPointRangeRingsNumber && m_bShowODPointRangeRings ) {
        wxPen savePen = dc.GetPen();
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_wxcODPointRangeRingsColour, wxPENSTYLE_SOLID ) );
        if(m_bExclusionBoundaryPoint && ! m_bInclusionBoundaryPoint) {
            dc.DrawDisk( r.x, r.y , 0, pix_radius);
        }
        else {
            dc.DrawDisk( r.x, r.y , pix_radius, pix_radius + m_iInclusionBoundaryPointSize);
        }
        dc.SetPen( savePen );
    }
    
    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );
    glDeleteTextures(1, &textureID);
    
    ODPoint::DrawGL( pivp );
#endif
}
