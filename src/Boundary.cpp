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
#include "BoundaryPoint.h"
#include "ODdc.h"
#include "ocpn_draw_pi.h"
//#include "cutil.h"
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
    m_bODPointsVisible = g_bBoundaryODPointsVisible;
    CreateColourSchemes();
    SetColourScheme();
    SetActiveColours();
    
}

Boundary::~Boundary()
{
    //dtor
}

void Boundary::CreateColourSchemes(void)
{
    ODPath::CreateColourSchemes();
    m_wxcActiveFillColourRGB = m_wxcActiveFillColour;
    m_wxcInActiveFillColourRGB = m_wxcInActiveFillColour;
    m_wxcActiveFillColourDay = m_wxcActiveFillColour;
    m_wxcInActiveFillColourDay = m_wxcInActiveFillColour;
    m_wxcActiveFillColourDusk.Set( m_wxcActiveFillColour.Red()/2, m_wxcActiveFillColour.Green()/2, m_wxcActiveFillColour.Blue()/2, m_wxcActiveFillColour.Alpha());
    m_wxcInActiveFillColourDusk.Set( m_wxcInActiveFillColour.Red()/2, m_wxcInActiveFillColour.Green()/2, m_wxcInActiveFillColour.Blue()/2, m_wxcInActiveFillColour.Alpha());
    m_wxcActiveFillColourNight.Set( m_wxcActiveFillColour.Red()/4, m_wxcActiveFillColour.Green()/4, m_wxcActiveFillColour.Blue()/4, m_wxcActiveFillColour.Alpha());
    m_wxcInActiveFillColourNight.Set( m_wxcInActiveFillColour.Red()/4, m_wxcInActiveFillColour.Green()/4, m_wxcInActiveFillColour.Blue()/4, m_wxcInActiveFillColour.Alpha());
}

void Boundary::SetColourScheme(PI_ColorScheme cs)
{
    ODPath::SetColourScheme(cs);
    switch (cs) {
        case PI_GLOBAL_COLOR_SCHEME_RGB:
            m_wxcSchemeActiveFillColour = m_wxcActiveFillColourRGB;
            m_wxcSchemeInActiveFillColour = m_wxcInActiveFillColourRGB;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DAY:
            m_wxcSchemeActiveFillColour = m_wxcActiveFillColourDay;
            m_wxcSchemeInActiveFillColour = m_wxcInActiveFillColourDay;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DUSK:
            m_wxcSchemeActiveFillColour = m_wxcActiveFillColourDusk;
            m_wxcSchemeInActiveFillColour = m_wxcInActiveFillColourDusk;
            break;
        case PI_GLOBAL_COLOR_SCHEME_NIGHT:
            m_wxcSchemeActiveFillColour = m_wxcActiveFillColourNight;
            m_wxcSchemeInActiveFillColour = m_wxcInActiveFillColourNight;
            break;
        default:
            m_wxcSchemeActiveFillColour = m_wxcActiveFillColourDay;
            m_wxcSchemeInActiveFillColour = m_wxcInActiveFillColourDay;
            break;
    }

}

void Boundary::Draw( ODDC& dc, PlugIn_ViewPort &piVP )
{
    if ( m_bVisible && m_pODPointList->GetCount() > 2) {
        int l_iBoundaryPointCount = 0;
        m_bpts = new wxPoint[ m_pODPointList->GetCount() ];
        wxPoint r;
        for(wxODPointListNode *node = m_pODPointList->GetFirst(); node; node = node->GetNext()) {
            ODPoint *pOp = node->GetData();
            GetCanvasPixLL( &piVP, &r, pOp->m_lat, pOp->m_lon );
            m_bpts[ l_iBoundaryPointCount++ ] = r;
        }
        
        if( m_bExclusionBoundary && !m_bInclusionBoundary ) {
            // fill boundary with hatching
            wxGraphicsContext *wxGC = NULL;
            wxMemoryDC *pmdc = wxDynamicCast(dc.GetDC(), wxMemoryDC);
            if( pmdc ) wxGC = wxGraphicsContext::Create( *pmdc );
            else {
                wxClientDC *pcdc = wxDynamicCast(dc.GetDC(), wxClientDC);
                if( pcdc ) wxGC = wxGraphicsContext::Create( *pcdc );
            }
            assert(wxGC);

            wxGC->SetPen(*wxTRANSPARENT_PEN);
            wxColour tCol;
            tCol.Set(m_fillcol.Red(), m_fillcol.Green(), m_fillcol.Blue(), m_uiFillTransparency);
            wxGC->SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxBRUSHSTYLE_CROSSDIAG_HATCH ) );
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
        } else if( !m_bExclusionBoundary && m_bInclusionBoundary && m_pODPointList->GetCount() > 3 ) {
            // surround boundary with hatching if there is more than 10 pixels different between points
            int l_imaxpointdiffX = 0;
            int l_imaxpointdiffY = 0;
            for( size_t i = 1; i < m_pODPointList->GetCount(); i++ ) {
                int l_ipointdiffX = abs(m_bpts[0].x - m_bpts[i].x);
                int l_ipointdiffY = abs(m_bpts[0].y - m_bpts[i].y);
                if(l_ipointdiffX > l_imaxpointdiffX) l_imaxpointdiffX = l_ipointdiffX;
                if(l_ipointdiffY > l_imaxpointdiffY) l_imaxpointdiffY = l_ipointdiffY;
            }
            if(l_imaxpointdiffX < 10 && l_imaxpointdiffY < 10 ) return;

            // Use ClipperLib to manage Polygon 
            // If needed simplify polygons to make shading stay outside
            Paths poly(1);
            for( size_t i = 0; i < m_pODPointList->GetCount(); i++ ) {
                poly[0] << IntPoint( m_bpts[i].x, m_bpts[i].y );
            }
            Paths polys;
            SimplifyPolygons( poly, polys );
            ClipperOffset co;
            Paths ExpandedBoundaries;
            co.AddPaths( polys, jtSquare, etClosedPolygon );
            co.Execute( ExpandedBoundaries, m_iInclusionBoundarySize );
            
            wxPoint *l_InclusionBoundary = new wxPoint[ ExpandedBoundaries[0].size() + 1 ];
            for( size_t i = 0; i < ExpandedBoundaries[0].size(); i++ )
            {
                l_InclusionBoundary[i].x = ExpandedBoundaries[0][i].X;
                l_InclusionBoundary[i].y = ExpandedBoundaries[0][i].Y;
            }
            // need to add first point to end to ensure the polygon is closed
            l_InclusionBoundary[ ExpandedBoundaries[0].size()].x = ExpandedBoundaries[0][0].X;
            l_InclusionBoundary[ ExpandedBoundaries[0].size()].y = ExpandedBoundaries[0][0].Y;

            int l_iPolygonPointCount[2];
            l_iPolygonPointCount[0] = m_pODPointList->GetCount();
            l_iPolygonPointCount[1] = ExpandedBoundaries[0].size() + 1;
            
            wxGraphicsContext *wxGC = NULL;
            wxMemoryDC *pmdc = wxDynamicCast(dc.GetDC(), wxMemoryDC);
            if( pmdc ) wxGC = wxGraphicsContext::Create( *pmdc );
            else {
                wxClientDC *pcdc = wxDynamicCast(dc.GetDC(), wxClientDC);
                if( pcdc ) wxGC = wxGraphicsContext::Create( *pcdc );
            }
            assert(wxGC);
            
            wxGC->SetPen(*wxTRANSPARENT_PEN);
            wxColour tCol;
            tCol.Set(m_fillcol.Red(), m_fillcol.Green(), m_fillcol.Blue(), m_uiFillTransparency);
            wxGC->SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxBRUSHSTYLE_CROSSDIAG_HATCH ) );
            wxGraphicsPath path = wxGC->CreatePath();
            path.MoveToPoint(m_bpts[0].x, m_bpts[0].y);
            for( int i = 0; i < l_iPolygonPointCount[0]; i++ ) {
                path.AddLineToPoint(m_bpts[i].x, m_bpts[i].y);
            }
            path.MoveToPoint(l_InclusionBoundary[0].x, l_InclusionBoundary[0].y);
            for( int i = 1; i < l_iPolygonPointCount[1]; i++ ) {
                path.AddLineToPoint(l_InclusionBoundary[i].x, l_InclusionBoundary[i].y);
            }
            path.CloseSubpath();
            wxGC->StrokePath(path);
            wxGC->FillPath( path );
            delete wxGC;
            ExpandedBoundaries.clear();
            polys.clear();
            poly.clear();
            co.Clear();
            delete [] l_InclusionBoundary;
        }
        wxDELETEA( m_bpts );
    }

    
    ODPath::Draw( dc, piVP );
}

void Boundary::DrawGL( PlugIn_ViewPort &piVP )
{
#ifdef ocpnUSE_GL
    if ( !m_bVisible ) return;
    
    ODDC dc;
    
    if(m_pODPointList->GetCount() > 2 ) {
        if( m_bExclusionBoundary || m_bInclusionBoundary ) {
            wxPoint *l_AllPoints = NULL;
            int     l_iAllPointsSizes[2];
            int l_iBoundaryPointCount = 0;
            m_bpts = new wxPoint[ m_pODPointList->GetCount() ];
            wxPoint r;
            for(wxODPointListNode *node = m_pODPointList->GetFirst(); node; node = node->GetNext()) {
                ODPoint *pOp = node->GetData();
                GetCanvasPixLL( &piVP, &r, pOp->m_lat, pOp->m_lon );
                m_bpts[ l_iBoundaryPointCount++ ] = r;
            }
            
            if( !m_bExclusionBoundary && m_bInclusionBoundary && m_pODPointList->GetCount() > 3 ) {
                // surround boundary with hatching if there is more than 10 pixels different between points
                int l_imaxpointdiffX = 0;
                int l_imaxpointdiffY = 0;
                for( size_t i = 1; i < m_pODPointList->GetCount(); i++ ) {
                    int l_ipointdiffX = abs(m_bpts[0].x - m_bpts[i].x);
                    int l_ipointdiffY = abs(m_bpts[0].y - m_bpts[i].y);
                    if(l_ipointdiffX > l_imaxpointdiffX) l_imaxpointdiffX = l_ipointdiffX;
                    if(l_ipointdiffY > l_imaxpointdiffY) l_imaxpointdiffY = l_ipointdiffY;
                }

                if(l_imaxpointdiffX < 10 && l_imaxpointdiffY < 10 ) return;
                // Use ClipperLib to manage Polygon 
                // If needed simplify polygons to make shading stay outside
                Paths poly(1);
                for( int i = 0; i < l_iBoundaryPointCount; i++ ) {
                    poly[0] << IntPoint( m_bpts[i].x, m_bpts[i].y );
                }
                Paths simplePolys;
                SimplifyPolygons( poly, simplePolys );
                ClipperOffset co;
                Paths ExpandedBoundaries;
                co.AddPaths( simplePolys, jtSquare, etClosedPolygon );
                co.Execute( ExpandedBoundaries, m_iInclusionBoundarySize );
                
                int l_iInclusionBoundarySize = ExpandedBoundaries[0].size();
                wxPoint *l_InclusionBoundary = new wxPoint[ l_iInclusionBoundarySize + 1 ];
                for( int i = 0; i < l_iInclusionBoundarySize; i++ )
                {
                    l_InclusionBoundary[i].x = ExpandedBoundaries[0][i].X;
                    l_InclusionBoundary[i].y = ExpandedBoundaries[0][i].Y;
                }
                // need to add first point to end to ensure the polygon is closed
                l_InclusionBoundary[ l_iInclusionBoundarySize ].x = ExpandedBoundaries[0][0].X;
                l_InclusionBoundary[ l_iInclusionBoundarySize ].y = ExpandedBoundaries[0][0].Y;

                // Create one array containing the original polygon joined to the expanded polygon to allow filling
                l_iAllPointsSizes[0] = l_iBoundaryPointCount;
                l_iAllPointsSizes[1] = l_iInclusionBoundarySize;
                l_AllPoints = new wxPoint[ l_iBoundaryPointCount + l_iInclusionBoundarySize + 1 ];
                for( int i = 0; i < l_iBoundaryPointCount; i++ ) {
                    l_AllPoints[i] = m_bpts[ i ];
                }
                for( int i = 0; i < l_iInclusionBoundarySize; i++ ) {
                    l_AllPoints[ i + l_iBoundaryPointCount ] = l_InclusionBoundary[i];
                }
                delete [] l_InclusionBoundary;
                ExpandedBoundaries.clear();
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
            dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxBRUSHSTYLE_SOLID ) );
            if( m_bExclusionBoundary ) {
                if(m_bIsBeingCreated) dc.DrawPolygonTessellated( m_pODPointList->GetCount(), m_bpts, 0, 0);
                else dc.DrawPolygonTessellated( m_pODPointList->GetCount() - 1, m_bpts, 0, 0);
            } else if( m_bInclusionBoundary && m_pODPointList->GetCount() > 3 ) {
                dc.DrawPolygonsTessellated( 2, l_iAllPointsSizes, l_AllPoints, 0, 0);
                delete [] l_AllPoints;
            }

            glDisable( GL_BLEND );
            glDisable( GL_TEXTURE_2D );
            glDeleteTextures(1, &textureID);

            wxDELETEA( m_bpts );
            
        } 
        
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
    
    if( m_bVisible && m_bPathIsActive ) m_fillcol = m_wxcSchemeActiveFillColour;
    else m_fillcol = m_wxcSchemeInActiveFillColour;
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

ODPoint *Boundary::InsertPointAfter( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    int nOP = m_pODPointList->IndexOf( pOP );
    if( nOP >= m_nPoints - 1 )
        return NULL;
    nOP++;
    
    BoundaryPoint *newpoint = new BoundaryPoint( lat, lon, g_sODPointIconName, GetNewMarkSequenced(), wxT("") );
    newpoint->m_bIsInPath = true;
    newpoint->m_bDynamicName = true;
    newpoint->SetNameShown( false );
    newpoint->SetTypeString( wxT("Boundary Point") );
    
    m_pODPointList->Insert( nOP, newpoint );
    
    ODPointGUIDList.Insert( pOP->m_GUID, nOP );
    
    m_nPoints++;
    
    if( bRenamePoints ) RenameODPoints();
    
    FinalizeForRendering();
    UpdateSegmentDistances();
    
    return ( newpoint );
}

void Boundary::SetColours( ODPath * pPath )
{
    Boundary *pBoundary = dynamic_cast<Boundary *>(pPath);
    assert(pBoundary != 0);
    ODPath::SetColours(pBoundary);
    m_wxcActiveFillColour = pBoundary->m_wxcActiveFillColour;
    m_wxcInActiveFillColour = pBoundary->m_wxcInActiveFillColour;
}

void Boundary::RemovePointFromPath(ODPoint* point, ODPath* path)
{
    if((ODPoint *)m_pODPointList->GetFirst()->GetData() == point) {
        m_pODPointList->DeleteObject( point );
        m_pODPointList->Append( (ODPoint *)m_pODPointList->GetFirst()->GetData() );
    }
    
    ODPath::RemovePointFromPath(point, path);
}
