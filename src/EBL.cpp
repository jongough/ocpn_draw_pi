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

EBL::EBL() : ODPath()
{
    m_sTypeString = _T("EBL");
    m_wxcActiveLineColour = g_colourEBLLineColour;
    m_width = g_EBLLineWidth;
    m_style = g_EBLLineStyle;
    m_bDrawArrow = g_bEBLShowArrow;
    m_bVRM = g_bEBLVRM;
    m_bAlwaysShowInfo = g_bEBLAlwaysShowInfo;
    m_bPerpLine = g_bEBLPerpLine;
    m_bCentreOnBoat = true;
    m_bFixedEndPosition = g_bEBLFixedEndPosition;
    m_bSaveUpdates = false;
    SetPersistence( g_iEBLPersistenceType );
    SetActiveColours();
    m_bRotateWithBoat = g_bEBLRotateWithBoat;
    m_iMaintainWith = g_iEBLMaintainWith;
    if(wxIsNaN(g_pfFix.Hdt)) m_dBoatHeading = 0.;
    else m_dBoatHeading = g_pfFix.Hdt;
    if(wxIsNaN(g_pfFix.Cog)) m_dBoatCOG = 0.;
    else m_dBoatCOG = g_pfFix.Cog;
    m_dEBLAngle = 0.;
    m_dLength = 0.;
    m_bEndPointMoving = false;
    m_bAlwaysShowInfo = g_bEBLAlwaysShowInfo;
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

void EBL::Resize( double lat, double lon, bool FixedAngle )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    //pEndPoint->m_lat = lat;
    //pEndPoint->m_lon = lon;
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    double brg;
    DistanceBearingMercator_Plugin( lat, lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &m_dLength );
    if( FixedAngle ) {
        PositionBearingDistanceMercator_Plugin( pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon );
    } else
        pEndPoint->m_seg_len = m_dLength;

    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
        g_pEBLPropDialog->UpdateProperties();
}

void EBL::Resize( void )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();

    DistanceBearingMercator_Plugin( pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &m_dEBLAngle, &m_dLength );
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
        MaintainWith();
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
    DistanceBearingMercator_Plugin(pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &m_dEBLAngle, &m_dLength);
    if(m_bRotateWithBoat) {
        switch(m_iMaintainWith) {
            case ID_MAINTAIN_WITH_HEADING:
                m_dEBLAngle -= g_pfFix.Hdt;
                break;
            case ID_MAINTAIN_WITH_COG:
                m_dEBLAngle -= g_pfFix.Cog;
                break;
        }
    }
    m_dBoatHeading = g_pfFix.Hdt;
    m_dBoatCOG = g_pfFix.Cog;

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
        MaintainWith();
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
    if(PersistenceType == ID_NOT_PERSISTENT || PersistenceType == ID_PERSISTENT_CRASH)
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

    if(bMoveEndPoint && !m_bEndPointMoving) {
        if(m_bRotateWithBoat) {
            MaintainWith();
        } else {
            if(!m_bFixedEndPosition) {
                PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
            } else {
                double brg;
                double hdg = 0.;
                bool   validHdg = false;
                DistanceBearingMercator_Plugin(pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &m_dLength);
                switch(m_iMaintainWith) {
                    case ID_MAINTAIN_WITH_HEADING:
                        if(!wxIsNaN(g_pfFix.Hdt) && g_pfFix.validHdt) {
                            hdg = g_pfFix.Hdt;
                            validHdg = true;
                        }
                        break;
                    case ID_MAINTAIN_WITH_COG:
                        if(!wxIsNaN(g_pfFix.Cog) && g_pfFix.validCog) {
                            hdg = g_pfFix.Cog;
                            validHdg = true;
                        }
                        break;
                }
                if(validHdg) {
                    if(hdg > brg)
                        m_dEBLAngle = brg + 360 - hdg;
                    else if(hdg < brg)
                        m_dEBLAngle = hdg - brg;
                    else m_dEBLAngle = 0;
                } else {
                    m_dEBLAngle = brg;
                }
            }
        }
    } else {
        DistanceBearingMercator_Plugin(pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &m_dEBLAngle, &m_dLength);
        m_dBoatHeading = g_pfFix.Hdt;
        m_dBoatCOG = g_pfFix.Cog;
    }
    pStartPoint->m_ODPointName = _("Boat");
    m_bCentreOnBoat = true;
    
    UpdateEBL();
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        pStartPoint->SetODPointRangeRingsStep( m_dLength / pStartPoint->GetODPointRangeRingsNumber() );
    }
    
    bool l_bRequestRefresh = true;
    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
        l_bRequestRefresh = g_pEBLPropDialog->UpdateProperties();
    
    m_bSaveUpdates = false;
    if(l_bRequestRefresh)
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
    if(pStartPoint->GetIconName() == wxEmptyString) {
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
    
    // check all other paths to see if this point appears in any other path
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
    if(m_iPersistenceType == ID_PERSISTENT || m_iPersistenceType == ID_PERSISTENT_CRASH)
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
        for( unsigned int ip = 0; ip < m_pODPointList->GetCount(); ip++ ) {
            ODPath *pp = (ODPath *) m_pODPointList->Item( ip );
            if( g_pPathMan->IsPathValid(pp) ) {
                g_pEBLPropDialog->SetPathAndUpdate( pp, true );
            }
        }
    }
}

void EBL::Draw( ODDC& dc, PlugIn_ViewPort &VP )
{
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
        double brg, dlength;
        DistanceBearingMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, pEndPoint->m_lat, pEndPoint->m_lon, &brg, &dlength);
        pStartPoint->SetODPointRangeRingsStep( pEndPoint->m_seg_len / pStartPoint->GetODPointRangeRingsNumber() );
    }

    RenderPerpLine( dc, VP );
    ODPath::Draw( dc, VP );
    
    if(m_bAlwaysShowInfo) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
        double brg, dlength;
        DistanceBearingMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, pEndPoint->m_lat, pEndPoint->m_lon, &brg, &dlength);
        wxPoint l_Point;
        GetCanvasPixLL( &VP, &l_Point, pEndPoint->m_lat, pEndPoint->m_lon);
        wxString info = g_ocpn_draw_pi->CreateExtraPathLegInfo(dc, this, brg, dlength, l_Point);
        if(info.length() > 0)
            g_ocpn_draw_pi->RenderExtraPathLegInfo( dc, l_Point, info );
    }
}
    
void EBL::DrawGL( PlugIn_ViewPort &piVP )
{
    ODDC dc;
    RenderPerpLine( dc, piVP );

    ODPath::DrawGL( piVP );
    
    if(m_bAlwaysShowInfo) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
        double brg, dlength;
        DistanceBearingMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, pEndPoint->m_lat, pEndPoint->m_lon, &brg, &dlength);
        wxPoint l_Point;
        GetCanvasPixLL( &piVP, &l_Point, pEndPoint->m_lat, pEndPoint->m_lon);
        wxString info = g_ocpn_draw_pi->CreateExtraPathLegInfo(dc, this, brg, dlength, l_Point);
        if(info.length() > 0)
            g_ocpn_draw_pi->RenderExtraPathLegInfo( dc, l_Point, info );
    }
}

void EBL::MaintainWith( void )
{
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    switch(m_iMaintainWith) {
        case ID_MAINTAIN_WITH_HEADING:
            if(!wxIsNaN(g_pfFix.Hdt))
                PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, g_pfFix.Hdt + m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
            else
                PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
            break;
        case ID_MAINTAIN_WITH_COG:
            if(!wxIsNaN(g_pfFix.Cog))
                PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, g_pfFix.Cog + m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
            else
                PositionBearingDistanceMercator_Plugin(pStartPoint->m_lat, pStartPoint->m_lon, m_dEBLAngle, m_dLength, &pEndPoint->m_lat, &pEndPoint->m_lon);
            break;
    }
}

void EBL::RenderPerpLine( ODDC &dc, PlugIn_ViewPort &VP)
{
    wxString colour;
    wxPenStyle style = wxPENSTYLE_SOLID;
	int width = g_path_line_width;

    if( m_nPoints == 0 || !m_bVisible ) return;

    if( m_style != STYLE_UNDEFINED ) style = m_style;
    if( m_width != STYLE_UNDEFINED ) width = m_width;

    SetActiveColours();

    dc.SetPen( *wxThePenList->FindOrCreatePen( m_col, width, style ) );
    dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_col, wxBRUSHSTYLE_SOLID ) );

    if(m_bPerpLine) {
        double l_dAngle = m_dEBLAngle + 90.;
        if(l_dAngle > 360.) l_dAngle -= 360.;
        l_dAngle = l_dAngle * PI / 180;
        ODPoint *l_pEndPoint = m_pODPointList->GetLast()->GetData();
        wxPoint l_Centreppt;
        GetCanvasPixLL( &VP, &l_Centreppt,  l_pEndPoint->m_lat, l_pEndPoint->m_lon );

        wxPoint l_dPoint1, l_dPoint2;
        double l_len1[4];
        double l_result = -1;
        double l_dSinAngle, l_dCosAngle;
        l_dSinAngle = sin(l_dAngle);
        l_dCosAngle = cos(l_dAngle);

        l_len1[0] = (VP.pix_width - l_Centreppt.x)/l_dSinAngle;
        l_len1[1] = (-VP.pix_height + l_Centreppt.y)/l_dCosAngle;
        l_len1[2] = (-l_Centreppt.x)/l_dSinAngle;
        l_len1[3] = (l_Centreppt.y)/l_dCosAngle;
        for (int i = 0; i <= 3; i++) {
            if(l_len1[i] >= 0 && (l_result == -1 || l_len1[i] < l_result))
                l_result = l_len1[i];
        }

        l_dPoint1.x = l_Centreppt.x + (l_result * l_dSinAngle);
        l_dPoint1.y = l_Centreppt.y - (l_result * l_dCosAngle);

        // get the other half of the line
        l_dSinAngle *= -1;
        l_dCosAngle *= -1;
        l_result = -1;
        l_len1[0] = (VP.pix_width - l_Centreppt.x)/l_dSinAngle;
        l_len1[1] = (-VP.pix_height + l_Centreppt.y)/l_dCosAngle;
        l_len1[2] = (-l_Centreppt.x)/l_dSinAngle;
        l_len1[3] = (l_Centreppt.y)/l_dCosAngle;
        for (int i = 0; i <= 3; i++) {
            if(l_len1[i] >= 0 && (l_result == -1 || l_len1[i] < l_result))
                l_result = l_len1[i];
        }

        l_dPoint2.x = l_Centreppt.x + (l_result * l_dSinAngle);
        l_dPoint2.y = l_Centreppt.y - (l_result * l_dCosAngle);

        RenderSegment( dc, l_dPoint1.x, l_dPoint1.y, l_dPoint2.x, l_dPoint2.y, VP, false, m_hiliteWidth );

    }

}


