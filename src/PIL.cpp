/***************************************************************************
 * Project:  OpenCPN
 * Purpose:  PIL
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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "PIL.h"
#include "ODdc.h"
#include "ocpn_draw_pi.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "ODConfig.h"

#include "ocpn_plugin.h"

#include <wx/listimpl.cpp>
WX_DEFINE_LIST ( PILList );

extern int          g_path_line_width;

extern PlugIn_ViewPort g_VP;

extern wxString    g_sPILEndIconName;
extern wxString    g_sPILStartIconName;
extern wxColour    g_colourPILActiveCentreLineColour;
extern wxColour    g_colourPILInActiveCentreLineColour;
extern wxColour    g_colourPILActiveOffsetLineColour;
extern wxColour    g_colourPILInActiveOffsetLineColour;
extern int         g_PILCentreLineWidth;
extern int         g_PILCentreLineStyle;
extern int         g_PILOffsetLineWidth;
extern int         g_PILOffsetLineStyle;
extern double      g_dPILOffset;
extern int         g_iPILPersistenceType;

extern ocpn_draw_pi *g_ocpn_draw_pi;
extern PILList      *g_pPILList;
extern ODSelect     *g_pODSelect;
extern PathMan      *g_pPathMan;
extern ODPlugIn_Position_Fix_Ex  g_pfFix;
extern wxString      g_sODPointIconName;
extern ODConfig     *g_pODConfig;
//extern EBLProp      *g_pEBLPropDialog;

PIL::PIL() : EBL()
{
    m_sTypeString = _T("PIL");
    m_wxcActiveLineColour = g_colourPILActiveCentreLineColour;
    m_wxcInActiveLineColour = g_colourPILInActiveCentreLineColour;
    m_width = g_PILCentreLineWidth;
    m_style = g_PILCentreLineStyle;
    m_bDrawArrow = false;
    m_bVRM = false;
    m_bCentreOnBoat = true;
    m_bFixedEndPosition = false;
    m_bSaveUpdates = false;
    SetPersistence( g_iPILPersistenceType );
    SetActiveColours();
    m_bRotateWithBoat = false;
    m_iMaintainWith = true;
    if(wxIsNaN(g_pfFix.Hdt)) m_dBoatHeading = 0.;
    else m_dBoatHeading = g_pfFix.Hdt;
    if(wxIsNaN(g_pfFix.Cog)) m_dBoatCOG = 0.;
    else m_dBoatCOG = g_pfFix.Cog;
    m_dEBLAngle = 0.;
    m_dLength = 0.;
    m_bEndPointMoving = false;
    m_width = g_PILCentreLineWidth;
    m_style = g_PILCentreLineStyle;
    m_iWidthOffsetLine = g_PILOffsetLineWidth;
    m_iStyleOffsetLine = g_PILOffsetLineStyle;

    PilLineList.clear();
}

PIL::~PIL()
{
    //dtor
}

int PIL::AddLine(wxString sName, wxString sDescription, double dOffset)
{
    PILLINE plNewLine;
    plNewLine.iID = PilLineList.size() + 1;
    plNewLine.sName = sName;
    plNewLine.sDescription = sDescription;
    plNewLine.dOffset = dOffset;
    plNewLine.wxcActiveColour = g_colourPILActiveOffsetLineColour;
    plNewLine.wxcInActiveColour = g_colourPILInActiveOffsetLineColour;
    plNewLine.dStyle = g_PILOffsetLineStyle;
    plNewLine.dWidth = g_PILOffsetLineWidth;
    PilLineList.push_back(plNewLine);

    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *l_pCentre = node->GetData();
    wxPoint l_Centreppt;
    wxPoint l_dPoint1, l_dPoint2;
    double l_dLat, l_dLon;

    GetCanvasPixLL( &g_VP, &l_Centreppt,  l_pCentre->m_lat, l_pCentre->m_lon);
    double l_dAngle;
    double l_dLat1, l_dLon1, l_dLat2, l_dLon2;
    l_dAngle = m_dEBLAngle + 90.;
    if(l_dAngle > 360.) l_dAngle -= 360.;
    if(l_dAngle < 0) l_dAngle += 360.;

    PositionBearingDistanceMercator_Plugin(l_pCentre->m_lat, l_pCentre->m_lon, l_dAngle, dOffset, &l_dLat, &l_dLon);
    GetCanvasPixLL( &g_VP, &l_Centreppt,  l_dLat, l_dLon );
    CalcOffsetPoints( l_Centreppt, &l_dPoint1, &l_dPoint2);
    GetCanvasLLPix( &g_VP, l_dPoint1, &l_dLat1, &l_dLon1 );
    GetCanvasLLPix( &g_VP, l_dPoint2, &l_dLat2, &l_dLon2 );
    g_pODSelect->AddSelectablePathSegment( l_dLat1, l_dLon1, l_dLat2, l_dLon2, NULL, NULL, this, plNewLine.iID );

    return plNewLine.iID;
}

void PIL::AddLine(PILLINE PilLine)
{
    PilLineList.push_back(PilLine);

    return;
}

void PIL::DelLine(int iID)
{
    std::list<PILLINE>::iterator it = PilLineList.begin();
    while(it != PilLineList.end()) {
        if(it->iID == iID) break;
        ++it;
    }
    PilLineList.erase(it);
}

void PIL::ChangeOffset(int iID, double dOffset)
{
    std::list<PILLINE>::iterator it = PilLineList.begin();
    while(it != PilLineList.end()) {
        if(it->iID == iID) {
            it->dOffset = dOffset;
            return;
        }
        ++it;
    }
}

void PIL::Draw( ODDC& dc, PlugIn_ViewPort &VP )
{
    RenderPIL( dc, VP );
    ODPath::Draw( dc, VP );
}

void PIL::DrawGL( PlugIn_ViewPort &piVP )
{
    ODDC dc;
    RenderPIL( dc, piVP );
    ODPath::DrawGL( piVP );
}

void PIL::DrawSegment( ODDC& dc, wxPoint *rp1, wxPoint *rp2, PlugIn_ViewPort &VP, bool bdraw_arrow )
{
    ODPath::DrawSegment( dc, rp1, rp2, VP, bdraw_arrow );
}

void PIL::RenderPIL( ODDC &dc, PlugIn_ViewPort &VP)
{
    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *l_pCentre = node->GetData();
    wxPoint l_Centreppt;

    GetCanvasPixLL( &VP, &l_Centreppt,  l_pCentre->m_lat, l_pCentre->m_lon);

    std::list<PILLINE>::iterator it = PilLineList.begin();
    while(it != PilLineList.end()) {
        double l_dAngle;
        l_dAngle = m_dEBLAngle + 90.;
        if(l_dAngle > 360.) l_dAngle -= 360.;
        if(l_dAngle < 0) l_dAngle += 360.;

        double l_dLat, l_dLon;
        wxPoint l_Centreppt;
        PositionBearingDistanceMercator_Plugin(l_pCentre->m_lat, l_pCentre->m_lon, l_dAngle, it->dOffset, &l_dLat, &l_dLon);
        GetCanvasPixLL( &VP, &l_Centreppt,  l_dLat, l_dLon );

        wxPoint l_dPoint1, l_dPoint2;
        CalcOffsetPoints( l_Centreppt, &l_dPoint1, &l_dPoint2);

        wxColour l_colour;

        if( m_bVisible && m_bPathIsActive ) {
            if((m_bPathManagerBlink || m_bPathPropertiesBlink) && (g_ocpn_draw_pi->nBlinkerTick & 1))
                l_colour = it->wxcInActiveColour;
            else
                l_colour = it->wxcActiveColour;
        }
        else {
            if((m_bPathManagerBlink || m_bPathPropertiesBlink) && (g_ocpn_draw_pi->nBlinkerTick & 1))
                l_colour= it->wxcActiveColour;
            else
                l_colour = it->wxcInActiveColour;
        }

        dc.SetPen( *wxThePenList->FindOrCreatePen( l_colour, it->dWidth, it->dStyle ) );
        dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( l_colour, wxBRUSHSTYLE_SOLID ) );

        RenderSegment( dc, l_dPoint1.x, l_dPoint1.y, l_dPoint2.x, l_dPoint2.y, VP, m_bDrawArrow, m_hiliteWidth );

        ++it;
    }
}

void PIL::CentreOnBoat( bool bMoveEndPoint )
{
    EBL::CentreOnBoat( bMoveEndPoint );

    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *l_pCentre = node->GetData();

    SetPILLineSelect( l_pCentre->m_lat, l_pCentre->m_lon );
}

void PIL::CentreOnLatLon( double lat, double lon )
{
    ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
    pStartPoint->m_ODPointName = _("Start");
    if(pStartPoint->GetIconName() == wxEmptyString) {
        pStartPoint->SetIconName( g_sPILStartIconName );
        pStartPoint->ReLoadIcon();
    }
    
    EBL::CentreOnLatLon( lat, lon );
    SetPILLineSelect( lat, lon);
    
}

void PIL::UpdatePIL( void )
{
    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *l_pCentre = node->GetData();
    
    SetPILLineSelect( l_pCentre->m_lat, l_pCentre->m_lon );
}

void PIL::CalcOffsetPoints( wxPoint Centreppt, wxPoint *FirstPoint, wxPoint *SecondPoint )
{
    wxPoint l_dPoint1, l_dPoint2;
    double l_len1[4];
    double l_result = -1;
    double l_dSinAngle, l_dCosAngle;
    l_dSinAngle = sin(m_dEBLAngle * PI / 180);
    l_dCosAngle = cos(m_dEBLAngle * PI / 180);

    if(Centreppt.x > g_VP.pix_width)
        l_len1[0] = -1;
    else
        l_len1[0] = (g_VP.pix_width - Centreppt.x) / l_dSinAngle;
    l_len1[1] = -Centreppt.x / l_dSinAngle;
    l_len1[2] = Centreppt.y / l_dCosAngle;
    if(Centreppt.y > g_VP.pix_height)
        l_len1[3] = -1;
    else
        l_len1[3] = (-g_VP.pix_height + Centreppt.y) / l_dCosAngle;

    if(Centreppt.x >= 0 && Centreppt.y >= 0) {
        for (int i = 0; i <= 3; i++) {
            if(l_len1[i] >= 0 && (l_result == -1 || l_len1[i] < l_result))
                l_result = l_len1[i];
        }
    } else {
        for (int i = 0; i <= 3; i++) {
            if(l_len1[i] >= 0 && l_len1[i] > l_result)
                l_result = l_len1[i];
        }
    }

    FirstPoint->x = Centreppt.x + (l_result * l_dSinAngle);
    FirstPoint->y = Centreppt.y - (l_result * l_dCosAngle);

    // get the other half of the line
    l_dSinAngle *= -1;
    l_dCosAngle *= -1;
    l_result = -1;
    if(Centreppt.x > g_VP.pix_width)
        l_len1[0] = -1;
    else
        l_len1[0] = (g_VP.pix_width - Centreppt.x) / l_dSinAngle;
    l_len1[1] = -Centreppt.x / l_dSinAngle;
    l_len1[2] = Centreppt.y / l_dCosAngle;
    if(Centreppt.y > g_VP.pix_height)
        l_len1[3] = -1;
    else
        l_len1[3] = (-g_VP.pix_height + Centreppt.y) / l_dCosAngle;

    if(Centreppt.x >= 0 && Centreppt.y >= 0) {
        for (int i = 0; i <= 3; i++) {
            if(l_len1[i] >= 0 && (l_result == -1 || l_len1[i] < l_result))
                l_result = l_len1[i];
        }
    } else {
        for (int i = 0; i <= 3; i++) {
            if(l_len1[i] >= 0 && l_len1[i] > l_result)
                l_result = l_len1[i];
        }
    }

    SecondPoint->x = Centreppt.x + (l_result * l_dSinAngle);
    SecondPoint->y = Centreppt.y - (l_result * l_dCosAngle);

}

void PIL::MovePILLine(double dLat, double dLon, int iPILId)
{
    double brg, dist;
    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *l_pCentre = node->GetData();

    DistanceBearingMercator_Plugin(l_pCentre->GetLatitude(), l_pCentre->GetLongitude(), dLat, dLon, &brg, &dist);
    dist = -dist * sin((brg - m_dEBLAngle) * PI / 180);

    std::list<PILLINE>::iterator it = PilLineList.begin();
    while(it != PilLineList.end()) {
        if(it->iID == iPILId) {
            it->dOffset = dist;
            return;
        }
        ++it;
    }
}

void PIL::SetPILLineSelect( double lat, double lon)
{
    wxPoint l_Centreppt;
    wxPoint l_dPoint1, l_dPoint2;
    double l_dLat, l_dLon;

    GetCanvasPixLL( &g_VP, &l_Centreppt,  lat, lon);
    std::list<PILLINE>::iterator it = PilLineList.begin();
    while(it != PilLineList.end()) {
        double l_dAngle;
        double l_dLat1, l_dLon1, l_dLat2, l_dLon2;
        l_dAngle = m_dEBLAngle + 90.;
        if(l_dAngle > 360.) l_dAngle -= 360.;
        if(l_dAngle < 0) l_dAngle += 360.;
        
        PositionBearingDistanceMercator_Plugin(lat, lon, l_dAngle, it->dOffset, &l_dLat, &l_dLon);
        GetCanvasPixLL( &g_VP, &l_Centreppt,  l_dLat, l_dLon );
        CalcOffsetPoints( l_Centreppt, &l_dPoint1, &l_dPoint2);
        GetCanvasLLPix( &g_VP, l_dPoint1, &l_dLat1, &l_dLon1 );
        GetCanvasLLPix( &g_VP, l_dPoint2, &l_dLat2, &l_dLon2 );
        g_pODSelect->DeleteSelectablePathSegment(this, it->iID);
        g_pODSelect->AddSelectablePathSegment( l_dLat1, l_dLon1, l_dLat2, l_dLon2, NULL, NULL, this, it->iID );
        
        ++it;
    }
}
