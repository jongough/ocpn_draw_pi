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

#include <wx/listimpl.cpp>
WX_DEFINE_LIST ( BoundaryList );

extern wxString    g_ActiveBoundaryLineColour;
extern wxString    g_InActiveBoundaryLineColour;
extern wxString    g_ActiveBoundaryFillColour;
extern wxString    g_InActiveBoundaryFillColour;

Boundary::Boundary() : Path()
{
    //ctor
    m_itest = 1;
    m_sTypeString = _T("Boundary");

    m_ActiveLineColour = g_ActiveBoundaryLineColour;
    m_ActiveFillColour = g_ActiveBoundaryFillColour;
    m_InActiveLineColour = g_InActiveBoundaryLineColour;
    m_InActiveFillColour = g_InActiveBoundaryFillColour;
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
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_fillcol, wxCROSSDIAG_HATCH ) );
        dc.DrawPolygon( m_pODPointList->GetCount(), m_bpts, 0, 0);
    }

}

void Boundary::DrawGL( PlugIn_ViewPort &piVP )
{
#ifdef ocpnUSE_GL
    Path::DrawGL( piVP );
    
    ocpnDC dc;
    
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
    if ( m_bVisible ) {
        //dc.SetPen(*wxTRANSPARENT_PEN); 
        dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_fillcol, wxBDIAGONAL_HATCH ) );
        if ( m_pODPointList->GetCount() > 2 )
            dc.DrawPolygonTessellated( m_pODPointList->GetCount(), m_bpts, 0, 0);
    }
    glEnd();
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
    
    if( m_bVisible && m_bPathIsActive ) {
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
    
}
