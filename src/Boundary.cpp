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

#include "Boundary.h"
#include "ocpndc.h"
#include "ocpn_draw_pi.h"

#ifdef __WXMSW__
#include "GL/gl.h"            // local copy for Windows
#include <GL/glu.h>
#else

#ifndef __OCPN__ANDROID__
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include "qopengl.h"                  // this gives us the qt runtime gles2.h
#include "GL/gl_private.h"
#endif

#endif

#ifdef ocpnUSE_GL
#include <wx/glcanvas.h>
#endif


#include <wx/graphics.h>

#include <wx/listimpl.cpp>
WX_DEFINE_LIST ( BoundaryList );

extern wxColour    g_colourActiveBoundaryLineColour;
extern wxColour    g_colourInActiveBoundaryLineColour;
extern wxColour    g_colourActiveBoundaryFillColour;
extern wxColour    g_colourInActiveBoundaryFillColour;
extern ocpn_draw_pi *g_ocpn_draw_pi;
extern unsigned int g_uiFillTransparency;

Boundary::Boundary() : Path()
{
    m_sTypeString = _T("Boundary");

    m_wxcActiveLineColour = g_colourActiveBoundaryLineColour;
    m_wxcActiveFillColour = g_colourActiveBoundaryFillColour;
    m_wxcInActiveLineColour = g_colourInActiveBoundaryLineColour;
    m_wxcInActiveFillColour = g_colourInActiveBoundaryFillColour;
    m_uiFillTransparency = g_uiFillTransparency;
    SetActiveColours();
    
}

Boundary::~Boundary()
{
    //dtor
}

void Boundary::Draw( ocpnDC& dc, PlugIn_ViewPort &VP )
{
    Path::Draw( dc, VP );
    
    // fill boundary with hatching
    if ( m_bVisible ) {
        wxGraphicsContext *wxGC = NULL;
        wxMemoryDC *pmdc = wxDynamicCast(dc.GetDC(), wxMemoryDC);
        if( pmdc ) wxGC = wxGraphicsContext::Create( *pmdc );
        else {
            wxClientDC *pcdc = wxDynamicCast(dc.GetDC(), wxClientDC);
            if( pcdc ) wxGC = wxGraphicsContext::Create( *pcdc );
        }
        
        wxGC->SetPen(*wxTRANSPARENT_PEN);
        wxColour tCol;
        tCol.Set(m_fillcol.Red(), m_fillcol.Green(), m_fillcol.Blue(), m_uiFillTransparency);
        wxGC->SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxCROSSDIAG_HATCH ) );
        wxGraphicsPath path = wxGC->CreatePath();
        path.MoveToPoint(m_bpts[0].x, m_bpts[0].y);
        for( size_t i = 1; i < m_pODPointList->GetCount(); i++ )
        {
            path.AddLineToPoint(m_bpts[i].x, m_bpts[i].y);
        }
        path.CloseSubpath();
        wxGC->StrokePath(path);
        wxGC->FillPath( path );
        delete wxGC;
        //dc.SetPen(*wxTRANSPARENT_PEN);
        //dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_fillcol, wxCROSSDIAG_HATCH ) );
        //dc.DrawPolygon( m_pODPointList->GetCount(), m_bpts, 0, 0);
    }

}

void Boundary::DrawGL( PlugIn_ViewPort &piVP )
{
#ifdef ocpnUSE_GL
    Path::DrawGL( piVP );
    
    ocpnDC dc;

    glEnable( GL_POLYGON_STIPPLE );
    glEnable( GL_BLEND );
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
    if ( m_bVisible ) {
        dc.SetPen(*wxTRANSPARENT_PEN); 
        wxColour tCol;
        tCol.Set(m_fillcol.Red(), m_fillcol.Green(), m_fillcol.Blue(), m_uiFillTransparency);
        dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxBDIAGONAL_HATCH ) );
        if ( m_pODPointList->GetCount() > 2 ) {
            dc.DrawPolygonTessellated( m_pODPointList->GetCount(), m_bpts, 0, 0);
            
        }
    }
    glEnd();
    glDisable( GL_BLEND );
    glDisable (GL_POLYGON_STIPPLE);
    
#endif
}

void Boundary::DeletePoint( ODPoint *op, bool bRenamePoints )
{
    //    n.b. must delete Selectables  and update config before deleting the point
    if( op->m_bIsInLayer ) return;
    
    if((ODPoint *)m_pODPointList->GetFirst()->GetData() == op) {
        m_pODPointList->DeleteObject( op );
        m_pODPointList->Append( (ODPoint *)m_pODPointList->GetFirst()->GetData() );
    }
    
    Path::DeletePoint( op, bRenamePoints );
}

void Boundary::SetActiveColours( void )
{
    wxString fillcolour;
    
    Path::SetActiveColours();
    
    if( m_bVisible && m_bPathIsActive ) m_fillcol = m_wxcActiveFillColour;
    else m_fillcol = m_wxcInActiveFillColour;
        
/*    if( m_bVisible && m_bPathIsActive ) {
        fillcolour = m_ActiveFillColour;
    }
    else {
        fillcolour = m_InActiveFillColour;
    }
    if( fillcolour.IsNull() ) {
        fillcolour = m_ActiveFillColour;
    }
    
    for( unsigned int i = 0; i < sizeof( ::GpxxColorNames ) / sizeof(wxString); i++ ) {
        if( fillcolour == ::GpxxColorNames[i] ) {
            m_fillcol = ::GpxxColors[i];
            break;
        }
    }
*/    
    
}
