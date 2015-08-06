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
#include "ODdc.h"
#include "ocpn_draw_pi.h"
#include "cutil.h"

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

void Boundary::Draw( ODDC& dc, PlugIn_ViewPort &VP )
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
    //Path::DrawGL( piVP );
    if ( !m_bVisible ) return;

    ODDC dc;
    
    int j = 0;
    m_bpts = new wxPoint[ m_pODPointList->GetCount() ];
    wxPoint r;
    for(wxODPointListNode *node = m_pODPointList->GetFirst(); node; node = node->GetNext()) {
        ODPoint *pOp = node->GetData();
        GetCanvasPixLL( &piVP, &r, pOp->m_lat, pOp->m_lon );
        m_bpts[ j++ ] = r;
    }
    
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
    wxColour tCol;
    tCol.Set(m_fillcol.Red(), m_fillcol.Green(), m_fillcol.Blue(), m_uiFillTransparency);
    dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxSOLID ) );
    if ( m_pODPointList->GetCount() > 2 ) {
        if(m_bIsBeingCreated) dc.DrawPolygonTessellated( m_pODPointList->GetCount(), m_bpts, 0, 0);
        else dc.DrawPolygonTessellated( m_pODPointList->GetCount() - 1, m_bpts, 0, 0);
    }
    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );
    glDeleteTextures(1, &textureID);
    
    Path::DrawGL( piVP );
    
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

void Boundary::MoveAllPoints( double inc_lat, double inc_lon )
{
    wxODPointListNode *node = m_pODPointList->GetFirst();
    int nodeCount = m_pODPointList->GetCount();
    int i = 1;
    while(node) {
        if(i == nodeCount) break;
        ODPoint *bp = (ODPoint *)node->GetData();
        bp->m_lat -= inc_lat;
        bp->m_lon -= inc_lon;
        node = node->GetNext();
        i++;
    }
}