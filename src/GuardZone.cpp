/***************************************************************************
 * Project:  OpenCPN
 * Purpose:  Guard Zone
 * Author:   Jon Gough
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

#include "GuardZone.h"
#include "ODdc.h"
#include "ocpn_draw_pi.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "ODPathPropertiesDialogImpl.h"

#include "ODConfig.h"

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
WX_DEFINE_LIST ( GZList );

extern wxColour    g_colourGZLineColour;
extern wxString    g_sGZIconName;
extern int         g_GZPersistenceType;
extern int         g_GZLineWidth; 
extern int         g_GZLineStyle;

extern ocpn_draw_pi *g_ocpn_draw_pi;
extern GZList      *g_pGZList;
extern ODSelect     *g_pODSelect;
extern PathMan      *g_pPathMan;
extern ODPlugIn_Position_Fix_Ex  g_pfFix;
extern wxString      g_sODPointIconName;
extern ODConfig     *g_pODConfig;
extern ODPathPropertiesDialogImpl      *g_pGZPropDialog;
extern int          g_PathLineWidth; 
extern int          g_PathLineStyle;

GuardZone::GuardZone() : ODPath()
{
    m_sTypeString = _T("GZ");
//    m_wxcActiveLineColour = g_colourGZLineColour;
    m_width = g_PathLineWidth;
    m_style = g_PathLineStyle;
    m_bCentreOnBoat = true;
    m_bSaveUpdates = false;
    SetPersistence( g_GZPersistenceType );
    SetActiveColours();
    m_dCentreLat = g_pfFix.Lat;
    m_dCentreLon = g_pfFix.Lon;
    PositionBearingDistanceMercator_Plugin( m_dCentreLat, m_dCentreLon, -15.0, 1.0, &m_dFirstPointLat, &m_dFirstPointLon );
    PositionBearingDistanceMercator_Plugin( m_dCentreLat, m_dCentreLon, 15.0, 1.5, &m_dSecondPointLat, &m_dSecondPointLon );
    m_dAngleBetweenLines = 30.0;
    m_bVRM = false;
}

GuardZone::~GuardZone()
{
    //dtor
}

void GuardZone::ResizeVRM( double lat, double lon )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    pEndPoint->m_lat = lat;
    pEndPoint->m_lon = lon;
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    double brg, dd;
    DistanceBearingMercator_Plugin( lat, lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &dd );
    pEndPoint->m_seg_len = dd;

    if(g_pGZPropDialog && g_pGZPropDialog->IsShown())
        g_pGZPropDialog->UpdateProperties();
}

void GuardZone::ResizeVRM( void )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    double brg, dd;
    DistanceBearingMercator_Plugin( pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &dd );
    pEndPoint->m_seg_len = dd;

    if(g_pGZPropDialog && g_pGZPropDialog->IsShown())
        g_pGZPropDialog->UpdateProperties();
}

void GuardZone::MoveEndPoint( double inc_lat, double inc_lon )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    pEndPoint->m_lat -= inc_lat;
    pEndPoint->m_lon -= inc_lon;
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        pStartPoint->SetODPointRangeRingsStep( pEndPoint->m_seg_len / pStartPoint->GetODPointRangeRingsNumber() );
    }
    if(g_pGZPropDialog && g_pGZPropDialog->IsShown())
        g_pGZPropDialog->UpdateProperties();
}

void GuardZone::SetPersistence( int PersistenceType )
{
    m_iPersistenceType = PersistenceType;
    if(PersistenceType == ID_GZ_NOT_PERSISTENT || PersistenceType == ID_GZ_PERSISTENT_CRASH)
        m_bTemporary = true;
    else
        m_bTemporary = false;
}

void GuardZone::CentreOnBoat( void )
{
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    pStartPoint->m_lat = g_pfFix.Lat;
    pStartPoint->m_lon = g_pfFix.Lon;
    pStartPoint->m_MarkName = _("Boat");
    m_bCentreOnBoat = true;
    
    UpdateGZ();
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
        pStartPoint->SetODPointRangeRingsStep( pEndPoint->m_seg_len / pStartPoint->GetODPointRangeRingsNumber() );
    }
    
//    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
//        g_pEBLPropDialog->UpdateProperties();

    m_bSaveUpdates = false;
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    
    return;
}

void GuardZone::CentreOnLatLon( double lat, double lon )
{
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    pStartPoint->m_lat = lat;
    pStartPoint->m_lon = lon;
    pStartPoint->m_MarkName = _("Start");
    if(pStartPoint->GetIconName() != wxEmptyString) {
        pStartPoint->SetIconName( g_sGZIconName );
        pStartPoint->ReLoadIcon();
    }
    m_bCentreOnBoat = false;
    m_bSaveUpdates = true;
    
    UpdateGZ();
//    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
//        g_pEBLPropDialog->UpdateProperties();
    
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    return;
}

void GuardZone::UpdateGZ( void )
{
    g_pODSelect->DeleteAllSelectablePathSegments( this );
    g_pODSelect->DeleteAllSelectableODPoints( this );
    g_pODSelect->AddAllSelectablePathSegments( this );
    g_pODSelect->AddAllSelectableODPoints( this );
    
    FinalizeForRendering();
    UpdateSegmentDistances();
    bool prev_bskip = g_pODConfig->m_bSkipChangeSetUpdate;
    g_pODConfig->m_bSkipChangeSetUpdate = false;
    if(m_iPersistenceType == ID_GZ_PERSISTENT || m_iPersistenceType == ID_GZ_PERSISTENT_CRASH)
        g_pODConfig->UpdatePath( this ); 
    g_pODConfig->m_bSkipChangeSetUpdate = prev_bskip;
    
    for( unsigned int ip = 0; ip < m_pODPointList->GetCount(); ip++ ) {
        ODPath *pp = (ODPath *) m_pODPointList->Item( ip );
        if( g_pPathMan->IsPathValid(pp) ) {
            pp->FinalizeForRendering();
            pp->UpdateSegmentDistances();
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

void GuardZone::Draw( ODDC& dc, PlugIn_ViewPort &VP )
{
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
        pStartPoint->SetODPointRangeRingsStep( pEndPoint->m_seg_len / pStartPoint->GetODPointRangeRingsNumber() );
    }

    wxString colour;
    int style = g_PathLineStyle;
    int width = g_PathLineWidth;
    
    //if( m_nPoints == 0 || !m_bVisible ) return;
    
    if( m_style != STYLE_UNDEFINED ) style = m_style;
    if( m_width != STYLE_UNDEFINED ) width = m_width;
    width = 3;
    
    SetActiveColours();
    
    dc.SetPen( *wxThePenList->FindOrCreatePen( m_col, width, style ) );
    dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_col, wxBRUSHSTYLE_TRANSPARENT ) );
    dc.DrawTorusSector( 200.0, 200.0, 100.0, 0.0, 90.0);
    dc.DrawTorusSector( 200.0, 200.0, 150.0, 0.0, 90.0);
    
}
    
void GuardZone::DrawGL( PlugIn_ViewPort &piVP )
{
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
        pStartPoint->SetODPointRangeRingsStep( pEndPoint->m_seg_len / pStartPoint->GetODPointRangeRingsNumber() );
    }
    ODPath::DrawGL( piVP );
}