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
#include "clipper.hpp"

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

using namespace std;
using namespace ClipperLib;

WX_DEFINE_LIST ( BoundaryList );

extern wxColour     g_colourActiveBoundaryLineColour;
extern wxColour     g_colourInActiveBoundaryLineColour;
extern wxColour     g_colourActiveBoundaryFillColour;
extern wxColour     g_colourInActiveBoundaryFillColour;
extern bool         g_bExclusionBoundary;
extern bool         g_bInclusionBoundary;
extern ocpn_draw_pi *g_ocpn_draw_pi;
extern unsigned int g_uiFillTransparency;
extern int          g_iInclusionBoundarySize;

Boundary::Boundary() : ODPath()
{
    m_sTypeString = _T("Boundary");

    m_wxcActiveLineColour = g_colourActiveBoundaryLineColour;
    m_wxcActiveFillColour = g_colourActiveBoundaryFillColour;
    m_wxcInActiveLineColour = g_colourInActiveBoundaryLineColour;
    m_wxcInActiveFillColour = g_colourInActiveBoundaryFillColour;
    m_uiFillTransparency = g_uiFillTransparency;
    m_bExclusionBoundary = g_bExclusionBoundary;
    m_bInclusionBoundary = g_bInclusionBoundary;
    m_iInclusionBoundarySize = g_iInclusionBoundarySize;
    SetActiveColours();
    
}

Boundary::~Boundary()
{
    //dtor
}

void Boundary::Draw( ODDC& dc, PlugIn_ViewPort &VP )
{
    ODPath::Draw( dc, VP );
    
    if ( m_bVisible ) {
        if(m_pODPointList->GetCount() < 3 ) return; // Nothing to fill at this point
        if( m_bExclusionBoundary && !m_bInclusionBoundary ) {
            // fill boundary with hatching
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
        } else if( !m_bExclusionBoundary && m_bInclusionBoundary ) {
            // surround boundary with hatching
            // Use ClipperLib to manage Pollygon 
            Path poly;
            for( size_t i = 0; i < m_pODPointList->GetCount(); i++ ) {
                poly << IntPoint( m_bpts[i].x, m_bpts[i].y );
            }
            ClipperOffset co;
            Paths ExpandedBoundaries;
            co.AddPath( poly, jtSquare, etClosedPolygon );
            co.Execute( ExpandedBoundaries, +15.0 );
            
            wxPoint *l_InclusionBoundary = new wxPoint[ ExpandedBoundaries[0].size() + 1 ];
            for( size_t i = 0; i < ExpandedBoundaries[0].size(); i++ )
            {
                l_InclusionBoundary[i].x = ExpandedBoundaries[0][i].X;
                l_InclusionBoundary[i].y = ExpandedBoundaries[0][i].Y;
            }
            // need to add first point to end to ensure the polygon is closed
            l_InclusionBoundary[ ExpandedBoundaries[0].size()].x = ExpandedBoundaries[0][0].X;
            l_InclusionBoundary[ ExpandedBoundaries[0].size()].y = ExpandedBoundaries[0][0].Y;

            int *l_iPolygonPointCount = new int[2];
            l_iPolygonPointCount[0] = m_pODPointList->GetCount();
            l_iPolygonPointCount[1] = ExpandedBoundaries[0].size() + 1;
            
            // Create one array containing the original polygon and the expanded polygon to allow filling
            wxPoint *l_AllPoints = new wxPoint[ l_iPolygonPointCount[0] + l_iPolygonPointCount[1] ];
            for( size_t i = 0; i < l_iPolygonPointCount[0]; i++ ) {
                l_AllPoints[i] = m_bpts[i];
            }
            for( size_t i = 0; i < l_iPolygonPointCount[1]; i++ ) {
                l_AllPoints[ i + l_iPolygonPointCount[0] ] += l_InclusionBoundary[i];
            }
            
            dc.SetPen(*wxTRANSPARENT_PEN);
            wxColour tCol;
            tCol.Set(m_fillcol.Red(), m_fillcol.Green(), m_fillcol.Blue(), m_uiFillTransparency);
            dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxCROSSDIAG_HATCH ) );
            dc.GetDC()->DrawPolyPolygon( 2, l_iPolygonPointCount, l_AllPoints, 0, 0, wxODDEVEN_RULE );
            ExpandedBoundaries.clear();
        }
    }

}

void Boundary::DrawGL( PlugIn_ViewPort &piVP )
{
#ifdef ocpnUSE_GL
    //Path::DrawGL( piVP );
    if ( !m_bVisible ) return;

    ODDC dc;
    
    if( m_bExclusionBoundary && !m_bInclusionBoundary ) {
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
    }
    
    ODPath::DrawGL( piVP );
    
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
    
    ODPath::DeletePoint( op, bRenamePoints );
}

void Boundary::SetActiveColours( void )
{
    wxString fillcolour;
    
    ODPath::SetActiveColours();
    
    if( m_bVisible && m_bPathIsActive ) m_fillcol = m_wxcActiveFillColour;
    else m_fillcol = m_wxcInActiveFillColour;
        
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