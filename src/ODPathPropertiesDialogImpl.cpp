/**************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Path Properties Support
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

#include "ODPathPropertiesDialogImpl.h"
#include "pathmanagerdialog.h"
#include "Path.h"
#include "Boundary.h"
#include "BoundaryPoint.h"
#include "EBL.h"
#include "ocpn_draw_pi.h"
#include "ODConfig.h"

enum {
    ID_FROM_POINT = 0,
    ID_TO_POINT,
    ID_DISTANCE_FROM_BOAT,
    ID_BEARING_FROM_BOAT,
    ID_LATITUDE,
    ID_LONGITUDE,
    ID_BEARING_FROM_TO,
    ID_DESCRIPTION,
    
    ID_POINTS_LIST_LAST
};

extern bool                 g_bShowMag;
extern ocpn_draw_pi         *g_ocpn_draw_pi;
extern double               g_dLat, g_dLon, gSog, gCog;
extern PathList             *g_pPathList;
extern PathManagerDialog    *g_pPathManagerDialog;
extern ODConfig             *g_pODConfig;
extern wxColour             g_colourActivePathLineColour;
extern wxColour             g_colourInActivePathLineColour;
extern wxColour             g_colourActivePathFillColour;
extern wxColour             g_colourInActivePathFillColour;

ODPathPropertiesDialogImpl::ODPathPropertiesDialogImpl() : ODPathPropertiesDialogDef( g_ocpn_draw_pi->m_parent_window )
{
    m_pPath = NULL;
    SetPointsListHeadings();
    m_staticTextFillColour->Hide();
    m_staticTextFillColour->Enable( false );
    m_colourPickerFillColour->Hide();
    m_colourPickerFillColour->Enable( false );
    m_staticTextFillTransparency->Hide();
    m_staticTextFillTransparency->Enable( false );
    m_sliderFillTransparency->Hide();
    m_sliderFillTransparency->Enable( false );
}

ODPathPropertiesDialogImpl::ODPathPropertiesDialogImpl( wxWindow* parent ) : ODPathPropertiesDialogDef( parent )
{
    m_pPath = NULL;
    SetPointsListHeadings();    
}

ODPathPropertiesDialogImpl::ODPathPropertiesDialogImpl( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos,
                    const wxSize& size, long style ) : ODPathPropertiesDialogDef( parent )
{
    m_nSelected = 0;
    m_pEnroutePoint = NULL;
    m_bStartNow = false;
    m_pPath = NULL;
    
    m_pEnroutePoint = NULL;
    m_bStartNow = false;
    #ifdef __WXOSX__
    style |= wxSTAY_ON_TOP;
    #endif
    
    
    wxFont *qFont = OCPNGetFont(_("Dialog"), 0);
    SetFont( *qFont );
    
    SetPointsListHeadings();
    
    //  Make an estimate of the dialog size, without scrollbars showing
/*    wxSize esize;
    esize.x = GetCharWidth() * 110;
    esize.y = GetCharHeight() * 40;
    SetSize( esize );
*/    
    Centre();
}

void ODPathPropertiesDialogImpl::OnClose( wxCloseEvent& event )
{
    wxCommandEvent nullEvent;
    OnCancel( nullEvent );
}

void ODPathPropertiesDialogImpl::OnOK( wxCommandEvent& event )
{
    //    Look in the path list to be sure the path is still available
    //    (May have been deleted by PathManagerDialog...)
    
    wxPathListNode *node = g_pPathList->GetFirst();
    while( node ) {
        Path *pPath = node->GetData();
        
        if( pPath == m_pPath ) {
            m_pPath->m_iBlink--;
            SaveChanges();              // write changes to globals and update config
            m_pPath->ClearHighlights();
            break;
        }
        node = node->GetNext();
    }
    
    
    m_pEnroutePoint = NULL;
    m_bStartNow = false;
    
    if( g_pPathManagerDialog && g_pPathManagerDialog->IsShown() ) {
        g_pPathManagerDialog->UpdatePathListCtrl();
    }
    
    Hide();
    RequestRefresh( GetOCPNCanvasWindow() );
    
    event.Skip();
}

void ODPathPropertiesDialogImpl::OnCancel( wxCommandEvent& event )
{
    //    Look in the path list to be sure the path is still available
    //    (May have been deleted by PathMangerDialog...)
    
    wxPathListNode *node = g_pPathList->GetFirst();
    while( node ) {
        Path *pPath = node->GetData();
        
        if( pPath == m_pPath ) {
            m_pPath->m_iBlink--;
            m_pPath->ClearHighlights();
            break;
        }
        node = node->GetNext();
    }
    
    
    m_bStartNow = false;
    
    Hide();
    RequestRefresh( GetOCPNCanvasWindow() );
    event.Skip();
}

void ODPathPropertiesDialogImpl::OnRightClick( wxMouseEvent& event )
{
// TODO: Implement OnRightClick
    wxMenu menu;
    
    if( ! m_pPath->m_bIsInLayer ) {
        wxString sPropertiesType( wxS("") );
        if ( m_pPath->m_sTypeString.IsNull() || m_pPath->m_sTypeString.IsEmpty() )
            sPropertiesType.append( wxS("OCPN Draw Point") );
        else {
            sPropertiesType.append( m_pPath->m_sTypeString );
            sPropertiesType.append( wxT(" Point") );
        }
        sPropertiesType.append( wxS(" &Properties...") );
        wxMenuItem* editItem = menu.Append( ID_PATHPROP_MENU_EDIT_WP, sPropertiesType );
        editItem->Enable( m_listCtrlODPoints->GetSelectedItemCount() == 1 );
        
        wxMenuItem* delItem = menu.Append( ID_PATHPROP_MENU_DELETE, wxS("&Remove Selected") );
        delItem->Enable( m_listCtrlODPoints->GetSelectedItemCount() > 0 && m_listCtrlODPoints->GetItemCount() > 2 );
    }
    
    PopupMenu( &menu );
}

void ODPathPropertiesDialogImpl::OnLeftDoubleClick( wxMouseEvent& event )
{
// TODO: Implement OnPathPropertiesDoubleClick
    long item = -1;
    
    item = m_listCtrlODPoints->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    
    if( item == -1 ) return;
    
    ODPoint *op = (ODPoint *) m_listCtrlODPoints->GetItemData( item );
    if( !op ) return;
    
    PathManagerDialog::ODPointShowPropertiesDialog( op, this );
}

void ODPathPropertiesDialogImpl::SetPathAndUpdate( Path *pB, bool only_points )
{
    if( NULL == pB )
        return;
    
    //  Fetch any config file values
    if ( !only_points )
    {
        //      long LMT_Offset = 0;                    // offset in seconds from UTC for given location (-1 hr / 15 deg W)
        m_tz_selection = 1;
        
        if( m_pPath ) {
            m_pPath->m_iBlink--;
            if(m_pPath->m_iBlink < 0 ) 
                m_pPath->m_iBlink = 0;
        }
        m_pPath = pB;
        if(m_pPath->m_sTypeString == wxT("Boundary")) m_pBoundary = (Boundary *)pB;
        if(m_pPath->m_sTypeString == wxT("EBL")) m_pEBL = (EBL *)pB;
        m_pPath->m_iBlink++;
        if(m_pPath->m_iBlink > 2) m_pPath->m_iBlink = 2;
        
        m_textCtrlName->SetValue( m_pPath->m_PathNameString );
        
        m_textCtrlName->SetFocus();
    }
    m_listCtrlODPoints->DeleteAllItems();
    

    InitializeList();

    UpdateProperties( pB );

    if( m_pPath )
        m_listCtrlODPoints->Show();

    Refresh( false );

}

bool ODPathPropertiesDialogImpl::UpdateProperties( Path *pInPath )
{
    Path *pPath;
    Boundary *pBoundary = NULL;
    EBL *pEBL = NULL;
    
    if( NULL == pInPath ) return false;
    ::wxBeginBusyCursor();
    
    if(pInPath->m_sTypeString == wxT("Boundary")) {
        pBoundary = (Boundary *)pInPath;
        pPath = pBoundary;
    } else if(pInPath->m_sTypeString == wxT("EBL")) {
        pEBL = (EBL *)pInPath;
        pPath = pEBL;
    } else
        pPath = pInPath;
        
    m_textCtrlName->SetValue( pPath->m_PathNameString );
    m_textCtrlDesctiption->SetValue( pPath->m_PathDescription);
    m_textCtrlGUID->SetValue( pPath->m_GUID );
    m_checkBoxActive->SetValue( pPath->IsActive() );
    
    double brgFromBoat = 0.;
    double distanceFromBoat = 0.;
    ODPoint *first_point = pPath->GetPoint( 1 );
    if( first_point )
        DistanceBearingMercator_Plugin(  g_dLat, g_dLon, first_point->m_lat, first_point->m_lon, &brgFromBoat, &distanceFromBoat );
    
    //  Total length
    double total_length = pPath->m_path_length;
    
    wxString slen;
    slen.Printf( wxT("%5.2f ") + getUsrDistanceUnit_Plugin(), toUsrDistance_Plugin( total_length ) );
    m_textCtrlTotalLength->SetValue( slen );
    
    wxString time_form;
    wxString tide_form;
    
    //  Iterate on Path Points
    wxODPointListNode *node = pPath->m_pODPointList->GetFirst();
    
    double slat = g_dLat;
    double slon = g_dLon;
    
    wxString nullify = _T("----");
    long item_line_index = 0;
    wxString wxsLastPoint = wxT("Boat");
    while( node ) {
        ODPoint *prp = node->GetData();
        
        //  Leg
        wxString t;
        m_listCtrlODPoints->SetItem( item_line_index, ID_FROM_POINT, wxsLastPoint );
        
        //  ODPoint Name
        m_listCtrlODPoints->SetItem( item_line_index, ID_TO_POINT, prp->GetName() );
        wxsLastPoint = prp->GetName();
        // Store Description
        m_listCtrlODPoints->SetItem( item_line_index, ID_DESCRIPTION, prp->GetDescription() );
        
        //  Distance
        //  Note that Distance/Bearing for Leg 000 is as from current position
        
        double brg, leg_dist;
        
        DistanceBearingMercator_Plugin( prp->m_lat, prp->m_lon, slat, slon, &brg, &leg_dist );
        
        //prp->SetCourse(course); // save the course to the next waypoint for printing.
        // end of calculation
        
        DistanceBearingMercator_Plugin(  g_dLat, g_dLon, prp->m_lat, prp->m_lon, &brgFromBoat, &distanceFromBoat );
        
        t.Printf( _T("%6.2f ") + getUsrDistanceUnit_Plugin(), toUsrDistance_Plugin( distanceFromBoat ) );
        m_listCtrlODPoints->SetItem( item_line_index, ID_DISTANCE_FROM_BOAT, t );
        prp->SetDistance( distanceFromBoat ); // save the course to the next point for printing.
        
        //  Bearing
        if( g_bShowMag )
            t.Printf( _T("%03.0f Deg. M"), g_ocpn_draw_pi->GetTrueOrMag( brgFromBoat ) );
        else
            t.Printf( _T("%03.0f Deg. T"), g_ocpn_draw_pi->GetTrueOrMag( brgFromBoat ) );
        
        m_listCtrlODPoints->SetItem( item_line_index, ID_BEARING_FROM_BOAT, t );
        
        // calculation of bearging from current point to next point.
        double brgFromTo, tmp_leg_dist;
        wxODPointListNode *next_node = node->GetNext();
        ODPoint * _next_prp = (next_node)? next_node->GetData(): NULL;
        if (_next_prp ) {
            DistanceBearingMercator_Plugin( prp->m_lat, prp->m_lon, _next_prp->m_lat, _next_prp->m_lon, &brgFromTo, &tmp_leg_dist );
        }else {
            brgFromTo = 0.0;
            tmp_leg_dist = 0.0;
        }
        
        if (_next_prp){
            if( g_bShowMag )
                t.Printf( _T("%03.0f Deg. M"), g_ocpn_draw_pi->GetTrueOrMag( brgFromTo ) );
            else
                t.Printf( _T("%03.0f Deg. T"), g_ocpn_draw_pi->GetTrueOrMag( brgFromTo ) );
            m_listCtrlODPoints->SetItem( item_line_index, ID_BEARING_FROM_TO, t );
        }
        else
            m_listCtrlODPoints->SetItem( item_line_index, ID_BEARING_FROM_TO, nullify );
        
        //  Lat/Lon
        wxString tlat = toSDMM_PlugIn( 1, prp->m_lat, prp->m_bIsInTrack );  // low precision for routes
        m_listCtrlODPoints->SetItem( item_line_index, ID_LATITUDE, tlat );
        
        wxString tlon = toSDMM_PlugIn( 2, prp->m_lon, prp->m_bIsInTrack );
        m_listCtrlODPoints->SetItem( item_line_index, ID_LONGITUDE, tlon );
        
        //  Save for iterating distance/bearing calculation
        slat = prp->m_lat;
        slon = prp->m_lon;
        
        item_line_index++;
        node = node->GetNext();
        
    }
    
//    if( pPath->m_ActiveLineColour == wxEmptyString ) m_colourPickerLineColour->SetColour( g_colourActivePathLineColour );
    m_colourPickerLineColour->SetColour( pPath->m_wxcActiveLineColour );

    for( unsigned int i = 0; i < sizeof( ::StyleValues ) / sizeof(int); i++ ) {
        if( pPath->m_style == ::StyleValues[i] ) {
            m_choiceLineStyle->Select( i );
            break;
        }
    }
    
    for( unsigned int i = 0; i < sizeof( ::WidthValues ) / sizeof(int); i++ ) {
        if( pPath->m_width == ::WidthValues[i] ) {
            m_choiceLineWidth->Select( i );
            break;
        }
    }
    
    ::wxEndBusyCursor();
    
    return true;
}

bool ODPathPropertiesDialogImpl::UpdateProperties( void )
{
    ::wxBeginBusyCursor();
    
    //  Iterate on Path Points
    wxODPointListNode *node = m_pPath->m_pODPointList->GetFirst();
    
    double slat = g_dLat;
    double slon = g_dLon;
    double brgFromBoat = 0.;
    double distanceFromBoat = 0.;
    
    wxString nullify = _T("----");
    long item_line_index = 0;
    wxString wxsLastPoint = wxT("Boat");
    while( node ) {
        ODPoint *prp = node->GetData();
        
        //  Leg
        wxString t;
        m_listCtrlODPoints->SetItem( item_line_index, ID_FROM_POINT, wxsLastPoint );
        
        //  ODPoint Name
        m_listCtrlODPoints->SetItem( item_line_index, ID_TO_POINT, prp->GetName() );
        wxsLastPoint = prp->GetName();
        // Store Description
        m_listCtrlODPoints->SetItem( item_line_index, ID_DESCRIPTION, prp->GetDescription() );
        
        //  Distance
        //  Note that Distance/Bearing for Leg 000 is as from current position
        
        double brg, leg_dist;
        
        DistanceBearingMercator_Plugin( prp->m_lat, prp->m_lon, slat, slon, &brg, &leg_dist );
        
        //prp->SetCourse(course); // save the course to the next waypoint for printing.
        // end of calculation
        
        DistanceBearingMercator_Plugin(  g_dLat, g_dLon, prp->m_lat, prp->m_lon, &brgFromBoat, &distanceFromBoat );
        
        t.Printf( _T("%6.2f ") + getUsrDistanceUnit_Plugin(), toUsrDistance_Plugin( distanceFromBoat ) );
        m_listCtrlODPoints->SetItem( item_line_index, ID_DISTANCE_FROM_BOAT, t );
        prp->SetDistance( distanceFromBoat ); // save the course to the next point for printing.
        
        //  Bearing
        if( g_bShowMag )
            t.Printf( _T("%03.0f Deg. M"), g_ocpn_draw_pi->GetTrueOrMag( brgFromBoat ) );
        else
            t.Printf( _T("%03.0f Deg. T"), g_ocpn_draw_pi->GetTrueOrMag( brgFromBoat ) );
        
        m_listCtrlODPoints->SetItem( item_line_index, ID_BEARING_FROM_BOAT, t );
        
        // calculation of bearging from current point to next point.
        double brgFromTo, tmp_leg_dist;
        wxODPointListNode *next_node = node->GetNext();
        ODPoint * _next_prp = (next_node)? next_node->GetData(): NULL;
        if (_next_prp ) {
            DistanceBearingMercator_Plugin( prp->m_lat, prp->m_lon, _next_prp->m_lat, _next_prp->m_lon, &brgFromTo, &tmp_leg_dist );
        }else {
            brgFromTo = 0.0;
            tmp_leg_dist = 0.0;
        }
        
        if (_next_prp){
            if( g_bShowMag )
                t.Printf( _T("%03.0f Deg. M"), g_ocpn_draw_pi->GetTrueOrMag( brgFromTo ) );
            else
                t.Printf( _T("%03.0f Deg. T"), g_ocpn_draw_pi->GetTrueOrMag( brgFromTo ) );
            m_listCtrlODPoints->SetItem( item_line_index, ID_BEARING_FROM_TO, t );
        }
        else
            m_listCtrlODPoints->SetItem( item_line_index, ID_BEARING_FROM_TO, nullify );
        
        //  Lat/Lon
        wxString tlat = toSDMM_PlugIn( 1, prp->m_lat, prp->m_bIsInTrack );  // low precision for routes
        m_listCtrlODPoints->SetItem( item_line_index, ID_LATITUDE, tlat );
        
        wxString tlon = toSDMM_PlugIn( 2, prp->m_lon, prp->m_bIsInTrack );
        m_listCtrlODPoints->SetItem( item_line_index, ID_LONGITUDE, tlon );
        
        //  Save for iterating distance/bearing calculation
        slat = prp->m_lat;
        slon = prp->m_lon;
        
        item_line_index++;
        node = node->GetNext();
        
    }
    
    //    if( pPath->m_ActiveLineColour == wxEmptyString ) m_colourPickerLineColour->SetColour( g_colourActivePathLineColour );
    m_colourPickerLineColour->SetColour( m_pPath->m_wxcActiveLineColour );
    
    for( unsigned int i = 0; i < sizeof( ::StyleValues ) / sizeof(int); i++ ) {
        if( m_pPath->m_style == ::StyleValues[i] ) {
            m_choiceLineStyle->Select( i );
            break;
        }
    }
    
    for( unsigned int i = 0; i < sizeof( ::WidthValues ) / sizeof(int); i++ ) {
        if( m_pPath->m_width == ::WidthValues[i] ) {
            m_choiceLineWidth->Select( i );
            break;
        }
    }
    
    ::wxEndBusyCursor();
    
    return true;
}

void ODPathPropertiesDialogImpl::SetDialogTitle(const wxString & title)
{
    SetTitle(title);
}

bool ODPathPropertiesDialogImpl::SaveChanges( void )
{
    
    if( m_pPath && !m_pPath->m_bIsInLayer ) {
        //  Get User input Text Fields
        m_pPath->m_PathNameString = m_textCtrlName->GetValue();
        m_pPath->m_PathDescription = m_textCtrlDesctiption->GetValue();
        m_pPath->m_bPathIsActive = m_checkBoxActive->GetValue();
        
        m_pPath->m_wxcActiveLineColour = m_colourPickerLineColour->GetColour();
        m_pPath->SetActiveColours();
        m_pPath->m_style = ::StyleValues[m_choiceLineStyle->GetSelection()];
        m_pPath->m_width = ::WidthValues[m_choiceLineWidth->GetSelection()];
        
        g_pODConfig->UpdatePath( m_pPath );
        //g_ocpn_draw_pi->SaveConfig();
    }
    
    if( m_pPath->IsActive() )
    {
        wxString msg_id( wxS("OCPN_PATH_ACTIVATED") );
        wxString msg;
        msg.append( wxS("Name: ") );
        msg.append( m_pPath->m_PathNameString.c_str() );
        msg.append( wxS(", GUID: ") );
        msg.append( m_pPath->m_GUID );
        SendPluginMessage( msg_id, msg );
        
    }
    
    return true;
}

void ODPathPropertiesDialogImpl::InitializeList()
{
    if( NULL == m_pPath ) return;
    
    //  Iterate on Route Points, inserting blank fields starting with index 0
    wxODPointListNode *pnode = m_pPath->m_pODPointList->GetFirst();
    int in = 0;
    while( pnode ) {
        m_listCtrlODPoints->InsertItem( in, _T(""), 0 );
        m_listCtrlODPoints->SetItemPtrData( in, (wxUIntPtr)pnode->GetData() );
        in++;
        if( pnode->GetData()->m_seg_etd.IsValid() ) {
            m_listCtrlODPoints->InsertItem( in, _T(""), 0 );
            in++;
        }
        pnode = pnode->GetNext();
    }
}

void ODPathPropertiesDialogImpl::SetPointsListHeadings()
{
    m_listCtrlODPoints->InsertColumn( ID_FROM_POINT, _("From Point"), wxLIST_FORMAT_LEFT );
    m_listCtrlODPoints->InsertColumn( ID_TO_POINT, _("To Point"), wxLIST_FORMAT_LEFT );
    m_listCtrlODPoints->InsertColumn( ID_DISTANCE_FROM_BOAT, _("Distance From Boat"), wxLIST_FORMAT_RIGHT );
    
    if(g_bShowMag)
        m_listCtrlODPoints->InsertColumn( ID_BEARING_FROM_BOAT, _("Bearing (M)"), wxLIST_FORMAT_LEFT );
    else
        m_listCtrlODPoints->InsertColumn( ID_BEARING_FROM_BOAT, _("Bearing"), wxLIST_FORMAT_LEFT );
    
    m_listCtrlODPoints->InsertColumn( ID_LATITUDE, _("Latitude"), wxLIST_FORMAT_LEFT );
    m_listCtrlODPoints->InsertColumn( ID_LONGITUDE, _("Longitude"), wxLIST_FORMAT_LEFT );
    if(g_bShowMag)
        m_listCtrlODPoints->InsertColumn( ID_BEARING_FROM_TO, _("Bearing From-To (M)"), wxLIST_FORMAT_LEFT );
    else
        m_listCtrlODPoints->InsertColumn( ID_BEARING_FROM_TO, _("Bearing From-To"), wxLIST_FORMAT_LEFT );
    m_listCtrlODPoints->InsertColumn( ID_DESCRIPTION, _("Description"), wxLIST_FORMAT_LEFT );
}