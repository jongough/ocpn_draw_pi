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
    m_bEndPointMoving = true;

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
    PilLineList.push_back(plNewLine);
    return plNewLine.iID;
}

void PIL::DelLine(int iID)
{
    std::list<PILLINE>::iterator it = PilLineList.begin();
    while(it != PilLineList.end()) {
        if(it->iID == iID) break;
        it++;
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
        it++;
    }
}

void PIL::Draw( ODDC& dc, PlugIn_ViewPort &VP )
{
    ODPath::Draw( dc, VP );

    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *l_pCentre = node->GetData();
    wxPoint l_Centreppt;

    GetCanvasPixLL( &VP, &l_Centreppt,  l_pCentre->m_lat, l_pCentre->m_lon);

    std::list<PILLINE>::iterator it = PilLineList.begin();
    while(it != PilLineList.end()) {
        double l_dAngle;
        if(it->dOffset >= 0) l_dAngle = m_dEBLAngle + 90.;
        else l_dAngle = m_dEBLAngle - 90.;
        if(l_dAngle > 360.) l_dAngle -= 360.;
        if(l_dAngle < 0) l_dAngle += 360.;
        double l_dLat, l_dLon;
        PositionBearingDistanceMercator_Plugin(l_pCentre->m_lat, l_pCentre->m_lon, l_dAngle, it->dOffset, &l_dLat, &l_dLon);
        GetCanvasPixLL( &VP, &l_Centreppt,  l_dLat, l_dLon);
        wxPoint l_dPoint1, l_dPoint2;
        double l_len, l_len1, l_len2;
        l_len1 = (VP.pix_width - l_Centreppt.x)/sin(l_dAngle);
        l_len2 = (VP.pix_height - l_Centreppt.y)/cos(l_dAngle);
        if(l_len1 < l_len2) l_len = l_len1;
        else l_len = l_len2;
        l_dPoint1.x = l_Centreppt.x + (l_len * sin(l_dAngle));
        l_dPoint1.y = l_Centreppt.y + (l_len * cos(l_dAngle));
        l_dPoint2.x = l_Centreppt.x - (l_len * sin(l_dAngle));
        l_dPoint2.y = l_Centreppt.y - (l_len * cos(l_dAngle));

        RenderSegment( dc, l_dPoint1.x, l_dPoint1.y, l_dPoint2.x, l_dPoint2.y, VP, m_bDrawArrow, m_hiliteWidth );

        it++;
    }
    if(PilLineList.size() > 0)
        PilLineList.erase(it);

}

void PIL::DrawGL( PlugIn_ViewPort &piVP )
{
    ODPath::DrawGL( piVP );
}

void PIL::DrawSegment( ODDC& dc, wxPoint *rp1, wxPoint *rp2, PlugIn_ViewPort &VP, bool bdraw_arrow )
{
    ODPath::DrawSegment( dc, rp1, rp2, VP, bdraw_arrow );
}