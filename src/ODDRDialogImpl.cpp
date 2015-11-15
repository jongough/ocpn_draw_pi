/**************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Dead Reckoning Support
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
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

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "ODDRDialogImpl.h"
#include "DR.h"
#include "ocpn_draw_pi.h"
#include "ODConfig.h"
#include "ODSelect.h"

extern ocpn_draw_pi     *g_ocpn_draw_pi;
extern double   g_dDRSOG;
extern int      g_iDRCOG;
extern double   g_dDRLength;
extern double   g_dDRPointInterval;
extern int      g_iDRLengthType;
extern int      g_iDRIntervalType;
extern int      g_iDRDistanceUnits;
extern int      g_iDRTimeUnits;
extern int      g_iDRPersistenceType;

extern wxString        g_sODPointIconName;

extern ODPlugIn_Position_Fix_Ex g_pfFix;
extern DRList                   *g_pDRList;
extern PathList                 *g_pPathList;
extern ODSelect                 *g_pODSelect;
extern ODConfig                 *g_pODConfig;

ODDRDialogImpl::ODDRDialogImpl( wxWindow* parent ) : ODDRDialogDef( parent )
{
    wxString s;
    s.Printf( _T("%.3f"), g_dDRLength );
    m_textCtrlLength->SetValue( s );
    s.Printf( _T("%.3f"), g_dDRPointInterval );
    m_textCtrlDRPointInterval->SetValue( s );
    m_radioBoxLengthType->SetSelection( g_iDRLengthType );
    m_radioBoxIntervalType->SetSelection( g_iDRIntervalType );
    m_radioBoxDistanceUnits->SetSelection( g_iDRDistanceUnits );
    m_radioBoxTimeUnits->SetSelection( g_iDRTimeUnits );
    
    if(g_pfFix.Sog != g_pfFix.Sog )
        s.Printf( _T("%.3f"), g_dDRSOG );
    else
        s.Printf( _T("%.3f"), g_pfFix.Sog );
    m_textCtrlSOG->SetValue( s );
    if(g_pfFix.Cog != g_pfFix.Cog )
        s.Printf( _T("%i"), g_iDRCOG );
    else
        s.Printf( _T("%.3f"), g_pfFix.Cog );
    m_textCtrlCOG->SetValue( s );
}

void ODDRDialogImpl::OnOK( wxCommandEvent& event )
{
    DR *l_pDR = new(DR);
    g_pDRList->Append( l_pDR );
    g_pPathList->Append( l_pDR );
    l_pDR->m_PathNameString << g_pDRList->GetCount();
    
    ODPoint *beginPoint = new ODPoint( g_pfFix.Lat, g_pfFix.Lon, wxEmptyString, wxS("Start"), wxT("") );
    beginPoint->SetNameShown( false );
    beginPoint->SetTypeString( wxT("DR Point"));
    beginPoint->m_bIsolatedMark = false;
    l_pDR->AddPoint( beginPoint, false );

    m_textCtrlSOG->GetValue().ToDouble( &l_pDR->m_dSoG );
    l_pDR->m_iCoG = wxAtoi( m_textCtrlCOG->GetValue() );
    m_textCtrlLength->GetValue().ToDouble( &l_pDR->m_dDRPathLength );
    m_textCtrlDRPointInterval->GetValue().ToDouble( &l_pDR->m_dDRPointInterval );
    
    l_pDR->m_iLengthType = m_radioBoxLengthType->GetSelection();
    l_pDR->m_iIntervalType = m_radioBoxIntervalType->GetSelection();
    l_pDR->m_iDistanceUnits = m_radioBoxDistanceUnits->GetSelection();
    l_pDR->m_iTimeUnits = m_radioBoxTimeUnits->GetSelection();
    
    switch ( m_radioBoxLengthType->GetSelection() ) {
        case ID_LT_TIME: {
            switch ( m_radioBoxTimeUnits->GetSelection() ) {
                case ID_TU_MINUTES:
                    l_pDR->m_dTotalLengthNM = ( l_pDR->m_dSoG / 60 ) * l_pDR->m_dDRPathLength;
                    break;
                case ID_TU_HOURS:
                    l_pDR->m_dTotalLengthNM = l_pDR->m_dSoG * l_pDR->m_dDRPathLength;
                    break;
                case ID_TU_DAYS:
                    l_pDR->m_dTotalLengthNM = ( l_pDR->m_dSoG * l_pDR->m_dDRPathLength ) * 24;
                    break;
            }
            break;
        }
        case ID_LT_DISTANCE: {
            if( m_radioBoxDistanceUnits->GetSelection() == ID_DU_KILOMETERS )
                l_pDR->m_dTotalLengthNM = l_pDR->m_dDRPathLength / 1.852;
            else
                l_pDR->m_dTotalLengthNM = l_pDR->m_dDRPathLength;
            break;
        }
    }
    
    switch ( m_radioBoxIntervalType->GetSelection() ) {
        case ID_IT_TIME: {
            switch ( m_radioBoxTimeUnits->GetSelection() ) {
                case ID_TU_MINUTES:
                    l_pDR->m_dDRPointIntervalNM = ( l_pDR->m_dSoG / 60 ) * l_pDR->m_dDRPointInterval;
                    break;
                case ID_TU_HOURS:
                    l_pDR->m_dDRPointIntervalNM = l_pDR->m_dSoG * l_pDR->m_dDRPointInterval;
                    break;
                case ID_TU_DAYS:
                    l_pDR->m_dDRPointIntervalNM = ( l_pDR->m_dSoG * l_pDR->m_dDRPointInterval ) * 24;
                    break;
            }
            break;
        }
        case ID_IT_DISTANCE: {
            if( m_radioBoxDistanceUnits->GetSelection() == ID_DU_KILOMETERS )
                l_pDR->m_dDRPointIntervalNM = l_pDR->m_dDRPointInterval / 1.852;
            else
                l_pDR->m_dDRPointIntervalNM = l_pDR->m_dDRPointInterval;
            break;
        }
    }
    
    double l_dStartLat = g_pfFix.Lat;
    double l_dStartLon = g_pfFix.Lon;
    double l_dEndLat;
    double l_dEndLon;
    PositionBearingDistanceMercator_Plugin( l_dStartLat, l_dStartLon, l_pDR->m_iCoG, l_pDR->m_dTotalLengthNM, &l_dEndLat, &l_dEndLon );
    
    int l_iNumWaypoints = floor( l_pDR->m_dTotalLengthNM / l_pDR->m_dDRPointIntervalNM );
    double l_cumLength = l_pDR->m_dDRPointIntervalNM;
    double l_dSaveLat = l_dStartLat;
    double l_dSaveLon = l_dStartLon;
    for( int i = 0; i < l_iNumWaypoints; i++ ) {
        double l_dLat, l_dLon;
        PositionBearingDistanceMercator_Plugin( l_dStartLat, l_dStartLon, l_pDR->m_iCoG, l_cumLength, &l_dLat, &l_dLon );
        ODPoint *l_NewPoint = new ODPoint( l_dLat, l_dLon, g_sODPointIconName, wxT(""), wxT("") );
        l_NewPoint->SetNameShown( false );
        l_NewPoint->SetTypeString( wxS("DR Point") );
        l_NewPoint->m_bIsolatedMark = FALSE;
        l_pDR->AddPoint( l_NewPoint );
        g_pODSelect->AddSelectableODPoint( l_dLat, l_dLon, l_NewPoint );
        g_pODSelect->AddSelectablePathSegment( l_dLat, l_dLon, l_dSaveLat, l_dSaveLon, beginPoint, l_NewPoint, l_pDR );
        l_dSaveLat = l_dLat;
        l_dSaveLon = l_dLon;
        l_cumLength += l_pDR->m_dDRPointIntervalNM;
    }
    
    if( l_dEndLat != l_dSaveLat || l_dEndLon != l_dSaveLon ) {
        ODPoint *l_NewPoint = new ODPoint( l_dEndLat, l_dEndLon, g_sODPointIconName, wxT(""), wxT("") );
        l_NewPoint->SetNameShown( false );
        l_NewPoint->SetTypeString( wxS("DR Point") );
        l_NewPoint->m_bIsolatedMark = FALSE;
        l_pDR->AddPoint( l_NewPoint );
        g_pODSelect->AddSelectableODPoint( l_dEndLat, l_dEndLon, l_NewPoint );
        g_pODSelect->AddSelectablePathSegment( l_dSaveLat, l_dSaveLon, l_dEndLat, l_dEndLon, beginPoint, l_NewPoint, l_pDR );
    }
    if(l_pDR->m_iPersistenceType == ID_DR_PERSISTENT || l_pDR->m_iPersistenceType == ID_DR_PERSISTENT_CRASH)
        g_pODConfig->AddNewPath( l_pDR, -1 );    // don't save over restart

    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    Show( false );
#ifdef __WXOSX__    
    EndModal(wxID_CANCEL);
#endif
}

void ODDRDialogImpl::OnCancel( wxCommandEvent& event )
{
    Show( false );
    #ifdef __WXOSX__    
    EndModal(wxID_CANCEL);
    #endif
    
    event.Skip();
}
