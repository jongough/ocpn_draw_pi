/***************************************************************************
 * Project:  OpenCPN
 * Purpose:  EBL
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

#include "EBL.h"
#include "ODdc.h"
#include "ocpn_draw_pi.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "EBLProp.h"
#include "ODConfig.h"

#include "ocpn_plugin.h"

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
WX_DEFINE_LIST ( EBLList );

extern wxColour    g_colourEBLLineColour;
extern wxString    g_sEBLEndIconName;
extern wxString    g_sEBLStartIconName;
extern bool        g_bEBLFixedEndPosition;
extern int         g_EBLPersistenceType;
extern int         g_EBLLineWidth; 
extern int         g_EBLLineStyle;

extern ocpn_draw_pi *g_ocpn_draw_pi;
extern EBLList      *g_pEBLList;
extern ODSelect     *g_pODSelect;
extern PathMan      *g_pPathMan;
extern ODPlugIn_Position_Fix_Ex  g_pfFix;
extern wxString      g_sODPointIconName;
extern ODConfig     *g_pODConfig;
extern EBLProp      *g_pEBLPropDialog;
extern bool         g_bEBLShowArrow;
extern bool         g_bEBLVRM;
extern bool         g_bEBLRotateWithBoat;
extern int          g_iEBLMaintainWith;

EBL::EBL() : ODPath()
{
    m_sTypeString = _T("EBL");
    m_wxcActiveLineColour = g_colourEBLLineColour;
    m_width = g_EBLLineWidth;
    m_style = g_EBLLineStyle;
    m_bDrawArrow = g_bEBLShowArrow;
    m_bVRM = g_bEBLVRM;
    m_bVRM = g_bEBLVRM;
    m_bCentreOnBoat = true;
    m_bFixedEndPosition = g_bEBLFixedEndPosition;
    m_bSaveUpdates = false;
    SetPersistence( g_EBLPersistenceType );
    SetActiveColours();
    m_bRotateWithBoat = g_bEBLRotateWithBoat;
    m_iMaintainWith = g_iEBLMaintainWith;
    m_dBoatHeading = g_pfFix.Hdt;
    m_dBoatCOG = g_pfFix.Cog;
    m_dEBLAngle = 0.;
    m_dLength = 0.;
}

EBL::~EBL()
{
    //dtor
}

void EBL::AddPoint( ODPoint *pNewPoint, bool b_rename_in_sequence, bool b_deferBoxCalc, bool b_isLoading )
{
    ODPath::AddPoint( pNewPoint, b_rename_in_sequence, b_deferBoxCalc, b_isLoading );
    if(pNewPoint->m_ODPointName == _("Start") || pNewPoint->m_ODPointName == _("Boat")) {
        pNewPoint->SetODPointRangeRingsNumber( 1 );
        pNewPoint->SetODPointRangeRingsStep( 0 );
        pNewPoint->SetODPointRangeRingsColour( GetCurrentColour() );
        if(m_bVRM)
            pNewPoint->m_bShowODPointRangeRings = true;
        else
            pNewPoint->m_bShowODPointRangeRings = false;
    } else if(pNewPoint->m_ODPointName == _("End")) {
        ODPoint *pFirstPoint = m_pODPointList->GetFirst()->GetData();
        pFirstPoint->SetODPointRangeRingsStep( pNewPoint->m_seg_len );
    }
}

void EBL::ResizeVRM( double lat, double lon )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    pEndPoint->m_lat = lat;
    pEndPoint->m_lon = lon;
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    double brg;
    DistanceBearingMercator_Plugin( lat, lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &m_dLength );
    pEndPoint->m_seg_len = m_dLength;

    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
        g_pEBLPropDialog->UpdateProperties();
}

void EBL::ResizeVRM( void )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    double brg;
    DistanceBearingMercator_Plugin( pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &m_dLength );
    pEndPoint->m_seg_len = m_dLength;

    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
        g_pEBLPropDialog->UpdateProperties();
}

void EBL::MoveStartPoint( double lat, double lon )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    pStartPoint->m_lat = lat;
    pStartPoint->m_lon = lon;
    if(m_dLength == 0.) m_dLength = pEndPoint->m_seg_len;
    
    if(m_bRotateWithBoat){
        switch(m_iMaintainWith) {
            case ID_EBL_MAINTAIN_WITH_HEADING:
                if(!isnan(g_pfFix.Hdt))
                    PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, g_pfFix.Hdt + m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                else
                    PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                break;
            case ID_EBL_MAINTAIN_WITH_COG:
                if(!isnan(g_pfFix.Cog)) {
                    PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, g_pfFix.Cog + m_dEBLAngle, pEndPoint->m_seg_len, &pEndPoint->m_lat, &pEndPoint->m_lon);
                }
                else
                    PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                break;
        }
    } else {
        PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
    }
    
    
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        pStartPoint->SetODPointRangeRingsStep( m_dLength / pStartPoint->GetODPointRangeRingsNumber() );
    }
    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
        g_pEBLPropDialog->UpdateProperties();
    
}

void EBL::MoveEndPoint( void )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    double brg;
    double hdg = 0;
    DistanceBearingMercator_Plugin(pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &m_dLength);
    switch(m_iMaintainWith) {
        case ID_EBL_MAINTAIN_WITH_HEADING:
            if(!isnan(g_pfFix.Hdt))
                hdg = g_pfFix.Hdt;
            break;
        case ID_EBL_MAINTAIN_WITH_COG:
            if(!isnan(g_pfFix.Cog))
                hdg = g_pfFix.Cog;
            break;
    }
    if(hdg > brg)
        m_dEBLAngle = brg + 360 - hdg;
    else if(hdg < brg)
        m_dEBLAngle = brg - hdg;
    else m_dEBLAngle = 0;
    
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        pStartPoint->SetODPointRangeRingsStep( m_dLength / pStartPoint->GetODPointRangeRingsNumber() );
    }
    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
        g_pEBLPropDialog->UpdateProperties();
    
}

void EBL::MoveEndPoint( bool bUpdateEBL )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();

    if(m_dLength == 0.) m_dLength = pEndPoint->m_seg_len;
    
    if(m_bRotateWithBoat) {
        switch(m_iMaintainWith) {
            case ID_EBL_MAINTAIN_WITH_HEADING:
                if(!isnan(g_pfFix.Hdt))
                    PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, g_pfFix.Hdt + m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                else
                    PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                break;
            case ID_EBL_MAINTAIN_WITH_COG:
                if(!isnan(g_pfFix.Cog))
                    PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, g_pfFix.Cog + m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                else
                    PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                break;
        }
    } else {
        if(!m_bFixedEndPosition)
            PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
    }
    
    if(m_bVRM)
        pStartPoint->SetODPointRangeRingsStep( m_dLength / pStartPoint->GetODPointRangeRingsNumber() );
    
    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
        g_pEBLPropDialog->UpdateProperties();
    
    if(bUpdateEBL) {
        bool l_bSaveUpdatesState = m_bSaveUpdates;
        m_bSaveUpdates = true;
        UpdateEBL();
        m_bSaveUpdates = l_bSaveUpdatesState;
    } else
        UpdateEBL();
}

void EBL::SetPersistence( int PersistenceType )
{
    m_iPersistenceType = PersistenceType;
    if(PersistenceType == ID_EBL_NOT_PERSISTENT || PersistenceType == ID_EBL_PERSISTENT_CRASH)
        m_bTemporary = true;
    else
        m_bTemporary = false;
}

void EBL::CentreOnBoat( bool bMoveEndPoint )
{
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    pStartPoint->m_lat = g_pfFix.Lat;
    pStartPoint->m_lon = g_pfFix.Lon;

    if(m_dLength == 0.) m_dLength = pEndPoint->m_seg_len;

    if(bMoveEndPoint) {
        if(m_bRotateWithBoat) {
            switch(m_iMaintainWith) {
                case ID_EBL_MAINTAIN_WITH_HEADING:
                    if(!isnan(g_pfFix.Hdt))
                        PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, g_pfFix.Hdt + m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                    else
                        PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                    break;
                case ID_EBL_MAINTAIN_WITH_COG:
                    if(!isnan(g_pfFix.Cog)) {
                        PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, g_pfFix.Cog + m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                    }
                    else
                        PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
                    break;
            }
        } else {
            if(!m_bFixedEndPosition) {
                PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
            } else {
                double brg, dist;
                double hdg = 0.;
                DistanceBearingMercator_Plugin(pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &m_dLength);
                switch(m_iMaintainWith) {
                    case ID_EBL_MAINTAIN_WITH_HEADING:
                        if(!isnan(g_pfFix.Hdt))
                            hdg = g_pfFix.Hdt;
                        break;
                    case ID_EBL_MAINTAIN_WITH_COG:
                        if(!isnan(g_pfFix.Cog))
                            hdg = g_pfFix.Cog;
                        break;
                }
                if(hdg > brg)
                    m_dEBLAngle = brg + 360 - hdg;
                else if(hdg < brg)
                    m_dEBLAngle = hdg - brg;
                else m_dEBLAngle = 0;
            }
        }
    } else {
        DistanceBearingMercator_Plugin(pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &m_dEBLAngle, &m_dLength);
        switch(m_iMaintainWith) {
            case ID_EBL_MAINTAIN_WITH_HEADING:
                if(!isnan(g_pfFix.Hdt))
                    m_dEBLAngle -= g_pfFix.Hdt;
                break;
            case ID_EBL_MAINTAIN_WITH_COG:
                if(!isnan(g_pfFix.Cog))
                    m_dEBLAngle -= g_pfFix.Cog;
                break;
        }
    }
    pStartPoint->m_ODPointName = _("Boat");
    m_bCentreOnBoat = true;
    
    UpdateEBL();
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
        pStartPoint->SetODPointRangeRingsStep( m_dLength / pStartPoint->GetODPointRangeRingsNumber() );
    }
    
    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
        g_pEBLPropDialog->UpdateProperties();

    m_bSaveUpdates = false;
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    
    return;
}

void EBL::CentreOnLatLon( double lat, double lon )
{
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    pStartPoint->m_lat = lat;
    pStartPoint->m_lon = lon;
    DistanceBearingMercator_Plugin(pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &m_dEBLAngle, &m_dLength);
    m_bRotateWithBoat = false;
    m_bCentreOnBoat = false;
    pStartPoint->m_ODPointName = _("Start");
    if(pStartPoint->GetIconName() != wxEmptyString) {
        pStartPoint->SetIconName( g_sEBLStartIconName );
        pStartPoint->ReLoadIcon();
    }
    m_bCentreOnBoat = false;
    m_bSaveUpdates = true;
    
    UpdateEBL();
    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
        g_pEBLPropDialog->UpdateProperties();
    
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    return;
}

void EBL::RemovePoint( ODPoint *op, bool bRenamePoints )
{
    g_pODSelect->DeleteAllSelectableODPoints( this );
    g_pODSelect->DeleteAllSelectablePathSegments( this );
    
    m_pODPointList->DeleteObject( op );
    if( wxNOT_FOUND != ODPointGUIDList.Index( op->m_GUID ) ) ODPointGUIDList.Remove( op->m_GUID );
    
    // check all other routes to see if this point appears in any other route
    ODPath *pcontainer_path = g_pPathMan->FindPathContainingODPoint( op );
    
    if( pcontainer_path == NULL ) {
        op->m_bIsInPath = false;
        op->m_bDynamicName = false;
        op->m_bIsolatedMark = true;
        if(op == g_ocpn_draw_pi->m_pEBLBoatPoint)
            g_ocpn_draw_pi->m_pEBLBoatPoint = NULL;
        this->DeletePoint( op );
        if(op == g_ocpn_draw_pi->m_pEBLBoatPoint) g_ocpn_draw_pi->m_pEBLBoatPoint = NULL;
        RebuildGUIDList();
    } else
        m_nPoints -= 1;
    
    
}

ODPoint *EBL::InsertPointBefore( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    ODPoint *newpoint = ODPath::InsertPointBefore( pOP, lat, lon );
    newpoint->m_IconName = g_sEBLStartIconName;
    newpoint->m_sTypeString = wxT("EBL Point");
    newpoint->m_ODPointName = _("Start");
    FinalizeForRendering();
    return newpoint;
}

ODPoint *EBL::InsertPointAfter( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    ODPoint *newpoint = ODPath::InsertPointAfter( pOP, lat, lon );
    newpoint->m_IconName = g_sEBLStartIconName;
    newpoint->m_sTypeString = wxT("EBL Point");
    newpoint->m_ODPointName = _("Start");
    ReloadPathPointIcons();
    FinalizeForRendering();
    return newpoint;
}

void EBL::InsertPointAfter( ODPoint *pOP, ODPoint *pnOP, bool bRenamePoints )
{
    ODPath::InsertPointAfter( pOP, pnOP );
}

void EBL::UpdateEBL( void )
{
    g_pODSelect->DeleteAllSelectablePathSegments( this );
    g_pODSelect->DeleteAllSelectableODPoints( this );
    g_pODSelect->AddAllSelectablePathSegments( this );
    g_pODSelect->AddAllSelectableODPoints( this );
    
    FinalizeForRendering();
    UpdateSegmentDistances();
    bool prev_bskip = g_pODConfig->m_bSkipChangeSetUpdate;
    g_pODConfig->m_bSkipChangeSetUpdate = false;
    if(m_iPersistenceType == ID_EBL_PERSISTENT || m_iPersistenceType == ID_EBL_PERSISTENT_CRASH)
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
    if( ( NULL != g_pEBLPropDialog ) && ( g_pEBLPropDialog->IsShown() ) ) {
        if( m_pODPointList ) {
            for( unsigned int ip = 0; ip < m_pODPointList->GetCount(); ip++ ) {
                ODPath *pp = (ODPath *) m_pODPointList->Item( ip );
                if( g_pPathMan->IsPathValid(pp) ) {
                    g_pEBLPropDialog->SetPathAndUpdate( pp, true );
                }
            }
        }
    }
}

void EBL::Draw( ODDC& dc, PlugIn_ViewPort &VP )
{
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
        pStartPoint->SetODPointRangeRingsStep( pEndPoint->m_seg_len / pStartPoint->GetODPointRangeRingsNumber() );
    }
    ODPath::Draw( dc, VP );
}
    
void EBL::DrawGL( PlugIn_ViewPort &piVP )
{
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
        pStartPoint->SetODPointRangeRingsStep( pEndPoint->m_seg_len / pStartPoint->GetODPointRangeRingsNumber() );
    }
    ODPath::DrawGL( piVP );
}