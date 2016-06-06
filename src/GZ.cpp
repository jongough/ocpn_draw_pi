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

#include "GZ.h"
#include "GZProp.h"
#include "ODdc.h"
#include "ocpn_draw_pi.h"
#include "ODSelect.h"
#include "PathMan.h"
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

WX_DEFINE_LIST ( GZList );

extern wxColour     g_colourActiveGZLineColour;
extern wxColour     g_colourInActiveGZLineColour;
extern wxColour     g_colourActiveGZFillColour;
extern wxColour     g_colourInActiveGZFillColour;
extern bool         g_bExclusionGZ;
extern bool         g_bInclusionGZ;
extern ocpn_draw_pi *g_ocpn_draw_pi;
extern unsigned int g_uiGZFillTransparency;
extern int          g_iInclusionGZSize;
extern bool         g_bGZRotateWithBoat;
extern int          g_iGZMaintainWith;
extern ODPlugIn_Position_Fix_Ex  g_pfFix;
extern ODSelect     *g_pODSelect;
extern PathMan      *g_pPathMan;
extern ODConfig     *g_pODConfig;
extern GZProp       *g_pGZPropDialog;


GZ::GZ() : ODPath()
{
    m_sTypeString = _T("GuardZone");

    m_wxcActiveLineColour = g_colourActiveGZLineColour;
    m_wxcActiveFillColour = g_colourActiveGZFillColour;
    m_wxcInActiveLineColour = g_colourInActiveGZLineColour;
    m_wxcInActiveFillColour = g_colourInActiveGZFillColour;
    m_uiGZFillTransparency = g_uiGZFillTransparency;
    m_dFirstDistance = 0;
    m_dFirstLineDirection = 0;
    m_dSecondDistance = 0;
    m_dSecondLineDirection = 0;
    m_bRotateWithBoat = g_bGZRotateWithBoat;
    m_iMaintainWith = g_iGZMaintainWith;
    m_bSetTransparent = false;
    SetActiveColours();
    
}

GZ::~GZ()
{
    //dtor
}

void GZ::Draw( ODDC& dc, PlugIn_ViewPort &piVP )
{
    if(m_dSecondDistance == 0) return;
    
    m_bSetTransparent = true;
    ODPath::Draw( dc, piVP );
    m_bSetTransparent = false;
    SetActiveColours();
    
    double l_dLat;
    double l_dLon;
    wxPoint l_l1p1;
    wxPoint l_l1p2;
    wxPoint l_l2p1;
    wxPoint l_l2p2;
    //GZ *gz = new GZ();
    // get x, y of first point on first line
    PositionBearingDistanceMercator_Plugin( m_dCentreLat, m_dCentreLon, m_dFirstLineDirection, m_dFirstDistance, &l_dLat, &l_dLon);
    GetCanvasPixLL( &piVP, &l_l1p1, l_dLat, l_dLon );
    // get x, y of second point on first line
    PositionBearingDistanceMercator_Plugin( m_dCentreLat, m_dCentreLon, m_dFirstLineDirection, m_dSecondDistance, &l_dLat, &l_dLon);
    GetCanvasPixLL( &piVP, &l_l1p2, l_dLat, l_dLon );
    // get x, y of second point on second line
    PositionBearingDistanceMercator_Plugin( m_dCentreLat, m_dCentreLon, m_dSecondLineDirection, m_dSecondDistance, &l_dLat, &l_dLon);
    GetCanvasPixLL( &piVP, &l_l2p2, l_dLat, l_dLon );
    // get x, y of first point on second line
    PositionBearingDistanceMercator_Plugin( m_dCentreLat, m_dCentreLon, m_dSecondLineDirection, m_dFirstDistance, &l_dLat, &l_dLon);
    GetCanvasPixLL( &piVP, &l_l2p1, l_dLat, l_dLon);
    
    wxPoint l_pCentre;
    GetCanvasPixLL( &piVP, &l_pCentre, m_dCentreLat, m_dCentreLon );
    
    wxColour tCol;
    tCol.Set(m_fillcol.Red(), m_fillcol.Green(), m_fillcol.Blue(), m_uiGZFillTransparency);
    wxPen savePen = dc.GetPen();
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxBRUSHSTYLE_CROSSDIAG_HATCH ) );

    DrawArcSegment( dc, &l_pCentre, &l_l1p1, &l_l1p2, &l_l2p2, &l_l2p1, piVP, false );
    
}

void GZ::DrawGL( PlugIn_ViewPort &piVP )
{
#ifdef ocpnUSE_GL
    if(m_dSecondDistance == 0) return;
    if ( !m_bVisible ) return;
    
    double l_dLat;
    double l_dLon;
    wxPoint l_l1p1;
    wxPoint l_l1p2;
    wxPoint l_l2p1;
    wxPoint l_l2p2;
    //GZ *gz = new GZ();
    // get x, y of first point on first line
    PositionBearingDistanceMercator_Plugin( m_dCentreLat, m_dCentreLon, m_dFirstLineDirection, m_dFirstDistance, &l_dLat, &l_dLon);
    GetCanvasPixLL( &piVP, &l_l1p1, l_dLat, l_dLon );
    // get x, y of second point on first line
    PositionBearingDistanceMercator_Plugin( m_dCentreLat, m_dCentreLon, m_dFirstLineDirection, m_dSecondDistance, &l_dLat, &l_dLon);
    GetCanvasPixLL( &piVP, &l_l1p2, l_dLat, l_dLon );
    // get x, y of second point on second line
    PositionBearingDistanceMercator_Plugin( m_dCentreLat, m_dCentreLon, m_dSecondLineDirection, m_dSecondDistance, &l_dLat, &l_dLon);
    GetCanvasPixLL( &piVP, &l_l2p2, l_dLat, l_dLon );
    // get x, y of first point on second line
    PositionBearingDistanceMercator_Plugin( m_dCentreLat, m_dCentreLon, m_dSecondLineDirection, m_dFirstDistance, &l_dLat, &l_dLon);
    GetCanvasPixLL( &piVP, &l_l2p1, l_dLat, l_dLon);
    
    wxPoint l_pCentre;
    GetCanvasPixLL( &piVP, &l_pCentre, m_dCentreLat, m_dCentreLon );
    
    ODDC dc;
    
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
    tCol.Set(m_fillcol.Red(), m_fillcol.Green(), m_fillcol.Blue(), m_uiGZFillTransparency);
    dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( tCol, wxPENSTYLE_SOLID ) );
    
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_fillcol, wxPENSTYLE_SOLID ) );
    dc.DrawSector( l_pCentre.x, l_pCentre.y, l_l1p1.x, l_l1p1.y, l_l1p2.x, l_l1p2.y, l_l2p2.x, l_l2p2.y, l_l2p1.x, l_l2p1.y );

    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );
    glDeleteTextures(1, &textureID);
    
    
#endif
    
}

void GZ::SetActiveColours( void )
{
    wxString fillcolour;
    
    if(m_bSetTransparent) 
        m_col = wxTransparentColour;
    else {
        ODPath::SetActiveColours();
        
        if( m_bVisible && m_bPathIsActive ) m_fillcol = m_wxcActiveFillColour;
        else m_fillcol = m_wxcInActiveFillColour;
    }
    
}

void GZ::MoveAllPoints( double inc_lat, double inc_lon )
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

void GZ::CentreOnBoat( void )
{
    ODPoint *pFirstPoint = m_pODPointList->GetFirst()->GetData();
    ODPoint *pLastPoint = m_pODPointList->GetLast()->GetData();
    m_dCentreLat = g_pfFix.Lat;
    m_dCentreLon = g_pfFix.Lon;
    m_dBoatHeading = g_pfFix.Hdt;
    m_dBoatCOG = g_pfFix.Cog;
    
    if(m_bRotateWithBoat) {
        MaintainWith();
    } else {
        PositionBearingDistanceMercator_Plugin(m_dCentreLat, m_dCentreLon, m_dFirstLineDirection, m_dFirstDistance, &pFirstPoint->m_lat, &pFirstPoint->m_lon);
        PositionBearingDistanceMercator_Plugin(m_dCentreLat, m_dCentreLon, m_dSecondLineDirection, m_dSecondDistance, &pLastPoint->m_lat, &pLastPoint->m_lon);
    } 
    
    UpdateGZ();
    bool l_bRequestRefresh = true;

    if(g_pGZPropDialog && g_pGZPropDialog->IsShown())
        l_bRequestRefresh = g_pGZPropDialog->UpdateProperties();
    
    m_bSaveUpdates = false;
    if(l_bRequestRefresh)
        RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    
    return;
}

void GZ::UpdateGZ( void )
{
    g_pODSelect->DeleteAllSelectablePathSegments( this );
    g_pODSelect->DeleteAllSelectableODPoints( this );
    g_pODSelect->AddAllSelectablePathSegments( this );
    g_pODSelect->AddAllSelectableODPoints( this );
    
    FinalizeForRendering();

    bool prev_bskip = g_pODConfig->m_bSkipChangeSetUpdate;
    g_pODConfig->m_bSkipChangeSetUpdate = false;
    g_pODConfig->UpdatePath( this ); 
    g_pODConfig->m_bSkipChangeSetUpdate = prev_bskip;
    
    for( unsigned int ip = 0; ip < m_pODPointList->GetCount(); ip++ ) {
        ODPath *pp = (ODPath *) m_pODPointList->Item( ip );
        if( g_pPathMan->IsPathValid(pp) ) {
            pp->FinalizeForRendering();
            pp->m_bIsBeingEdited = false;
            
            g_pODConfig->UpdatePath( pp );
            
            pp->SetHiLite( 0 );
        }
    }
    
    //    Update the PathProperties Dialog, if currently shown
    if( ( NULL != g_pGZPropDialog ) && ( g_pGZPropDialog->IsShown() ) ) {
        if( m_pODPointList ) {
            for( unsigned int ip = 0; ip < m_pODPointList->GetCount(); ip++ ) {
                ODPath *pp = (ODPath *) m_pODPointList->Item( ip );
                if( g_pPathMan->IsPathValid(pp) ) {
                    g_pGZPropDialog->SetPathAndUpdate( pp, true );
                }
            }
        }
    }
}

void GZ::MaintainWith( void )
{
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    m_dCentreLat = g_pfFix.Lat;
    m_dCentreLon = g_pfFix.Lon;
    
    switch(m_iMaintainWith) {
        case ID_MAINTAIN_WITH_HEADING:
            if(!isnan(g_pfFix.Hdt)) {
                PositionBearingDistanceMercator_Plugin(m_dCentreLat, m_dCentreLon, g_pfFix.Hdt + m_dFirstLineDirection, m_dFirstDistance, &pStartPoint->m_lat, &pStartPoint->m_lon);
                PositionBearingDistanceMercator_Plugin(m_dCentreLat, m_dCentreLon, g_pfFix.Hdt + m_dSecondLineDirection, m_dSecondDistance, &pEndPoint->m_lat, &pEndPoint->m_lon);
            } else {
                PositionBearingDistanceMercator_Plugin(m_dCentreLat, m_dCentreLon, m_dFirstLineDirection, m_dFirstDistance, &pStartPoint->m_lat, &pStartPoint->m_lon);
                PositionBearingDistanceMercator_Plugin(m_dCentreLat, m_dCentreLon, m_dSecondLineDirection, m_dSecondDistance, &pEndPoint->m_lat, &pEndPoint->m_lon);
            }
            break;
        case ID_MAINTAIN_WITH_COG:
            if(!isnan(g_pfFix.Cog)) {
                PositionBearingDistanceMercator_Plugin(m_dCentreLat, m_dCentreLon, g_pfFix.Cog + m_dFirstLineDirection, m_dFirstDistance, &pStartPoint->m_lat, &pStartPoint->m_lon);
                PositionBearingDistanceMercator_Plugin(m_dCentreLat, m_dCentreLon, g_pfFix.Cog + m_dSecondLineDirection, m_dSecondDistance, &pEndPoint->m_lat, &pEndPoint->m_lon);
            } else {
                PositionBearingDistanceMercator_Plugin(m_dCentreLat, m_dCentreLon, m_dFirstLineDirection, m_dFirstDistance, &pStartPoint->m_lat, &pStartPoint->m_lon);
                PositionBearingDistanceMercator_Plugin(m_dCentreLat, m_dCentreLon, m_dSecondLineDirection, m_dSecondDistance, &pEndPoint->m_lat, &pEndPoint->m_lon);
            }
            break;
    }
}
