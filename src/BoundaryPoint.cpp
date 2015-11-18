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

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "wx/wxprec.h"


#include "BoundaryPoint.h"
#include "georef.h"
#include "ODdc.h"
#include "ocpn_draw_pi.h"

#include "GL/gl.h"
#include <wx/graphics.h>

extern PlugIn_ViewPort  *g_pivp;
extern ocpn_draw_pi     *g_ocpn_draw_pi;
extern bool         g_bExclusionBoundaryPoint;
extern bool         g_bInclusionBoundaryPoint;
extern int          g_iInclusionBoundaryPointSize;
extern unsigned int g_uiBoundaryPointFillTransparency;

BoundaryPoint::BoundaryPoint(double lat, double lon, const wxString& icon_ident, const wxString& name, const wxString& pGUID, bool bAddToList)
: ODPoint( lat, lon, icon_ident, name, pGUID, bAddToList )
{
    m_sTypeString = wxT("Boundary Point");
    m_uiBoundaryPointFillTransparency = g_uiBoundaryPointFillTransparency;
    m_bExclusionBoundaryPoint = g_bExclusionBoundaryPoint;
    m_bInclusionBoundaryPoint = g_bInclusionBoundaryPoint;
    m_iInclusionBoundaryPointSize = g_iInclusionBoundaryPointSize;
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
}

void BoundaryPoint::Draw(ODDC& dc, wxPoint* rpn)
{
    ODPoint::Draw( dc, rpn );
    
    wxPoint r;
    GetCanvasPixLL( g_pivp, &r,  m_lat, m_lon);
    
    double factor = 1.00;
    if( m_iODPointRangeRingsStepUnits == 1 )          // nautical miles
        factor = 1 / 1.852;
    
    factor *= m_fODPointRangeRingsStep;
    
    double tlat, tlon;
    wxPoint r1;
    ll_gc_ll( m_lat, m_lon, 0, factor, &tlat, &tlon );
    //cc1->GetCanvasPointPix( tlat, tlon, &r1 );
    GetCanvasPixLL( g_pivp, &r1,  tlat, tlon);
    
    double lpp = sqrt( pow( (double) (r.x - r1.x), 2) +
    pow( (double) (r.y - r1.y), 2 ) );
    int pix_radius = (int) lpp * m_iODPointRangeRingsNumber;
    
    // fill boundary with hatching
    if ( m_bIsVisible && (m_bExclusionBoundaryPoint || m_bInclusionBoundaryPoint) && pix_radius > 0 && m_iODPointRangeRingsNumber && m_bShowODPointRangeRings ) {
        wxColour tCol;
        tCol.Set(m_wxcODPointRangeRingsColour.Red(), m_wxcODPointRangeRingsColour.Green(), m_wxcODPointRangeRingsColour.Blue(), m_uiBoundaryPointFillTransparency);
        if(m_bExclusionBoundaryPoint && !m_bInclusionBoundaryPoint) {
            wxPen savePen = dc.GetPen();
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxHATCHSTYLE_CROSSDIAG ) );
            dc.DrawCircle( r, pix_radius );
            dc.SetPen( savePen );
        } else if(!m_bExclusionBoundaryPoint && m_bInclusionBoundaryPoint){
            wxPen savePen = dc.GetPen();
            dc.SetPen(*wxTRANSPARENT_PEN);
            dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxHATCHSTYLE_CROSSDIAG ) );
            dc.DrawDisk( r.x, r.y, pix_radius, pix_radius + m_iInclusionBoundaryPointSize );
            dc.SetPen( savePen );
/*            
            wxGraphicsContext *wxGC = NULL;
            wxMemoryDC *pmdc = wxDynamicCast(dc.GetDC(), wxMemoryDC);
            if( pmdc ) wxGC = wxGraphicsContext::Create( *pmdc );
            else {
                wxClientDC *pcdc = wxDynamicCast(dc.GetDC(), wxClientDC);
                if( pcdc ) wxGC = wxGraphicsContext::Create( *pcdc );
            }
            wxGC->SetPen(*wxTRANSPARENT_PEN);
            wxGC->SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxHATCHSTYLE_CROSSDIAG ) );
            wxGraphicsPath p = wxGC->CreatePath();
            p.AddCircle( r.x, r.y, pix_radius );
            p.AddCircle( r.x, r.y, pix_radius + m_iInclusionBoundaryPointSize );
            wxGC->FillPath(p);
*/            
        }
    }
    
}

void BoundaryPoint::DrawGL(PlugIn_ViewPort& pivp)
{
#ifdef ocpnUSE_GL
    ODPoint::DrawGL( pivp );
    
    ODDC dc;
    
    wxPoint r;
    GetCanvasPixLL( g_pivp, &r,  m_lat, m_lon);
    
    double factor = 1.00;
    if( m_iODPointRangeRingsStepUnits == 1 )          // nautical miles
        factor = 1 / 1.852;
    
    factor *= m_fODPointRangeRingsStep;
    
    double tlat, tlon;
    wxPoint r1;
    ll_gc_ll( m_lat, m_lon, 0, factor, &tlat, &tlon );
    //cc1->GetCanvasPointPix( tlat, tlon, &r1 );
    GetCanvasPixLL( g_pivp, &r1,  tlat, tlon);
    
    double lpp = sqrt( pow( (double) (r.x - r1.x), 2) +
    pow( (double) (r.y - r1.y), 2 ) );
    int pix_radius = (int) lpp * m_iODPointRangeRingsNumber;
    
    glEnable( GL_POLYGON_STIPPLE );
    GLubyte slope_cross_hatch[] = {
        0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
        0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55,
        0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
        0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55,
        0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
        0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55,
        0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
        0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55,
        0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
        0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55,
        0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
        0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55,
        0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
        0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55,
        0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
        0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55,
        0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x55,
        0x22, 0x22, 0x22, 0x22, 0x55, 0x55, 0x55, 0x55
    };    
    glPolygonStipple( slope_cross_hatch );
    glBegin(GL_POLYGON_STIPPLE);
    // fill boundary with hatching
    if ( m_bIsVisible && (m_bExclusionBoundaryPoint || m_bInclusionBoundaryPoint) && pix_radius > 0 && m_iODPointRangeRingsNumber && m_bShowODPointRangeRings ) {
        wxPen savePen = dc.GetPen();
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_wxcODPointRangeRingsColour, wxHATCHSTYLE_CROSSDIAG ) );
        if(m_bExclusionBoundaryPoint && ! m_bInclusionBoundaryPoint)
            dc.DrawCircle( r, pix_radius );
        else {
            dc.DrawDisk( r.x, r.y , pix_radius, pix_radius + m_iInclusionBoundaryPointSize);
        }
        dc.SetPen( savePen );
    }
    glEnd();
    glDisable (GL_POLYGON_STIPPLE);

#endif
}
