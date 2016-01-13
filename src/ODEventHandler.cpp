/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPN Draw Event Handler Support
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

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ODEventHandler.h"
#include "ocpn_draw_pi.h"
#include "ocpn_plugin.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "ODPathPropertiesDialogImpl.h"
#include "ODPointPropertiesImpl.h"
#include "ODRolloverWin.h"
#include "ODUtils.h"
#include "chcanv.h"
#include "PointMan.h"
#include "Boundary.h"
#include "EBL.h"
#include "DR.h"
#include "ODDRDialogImpl.h"
#include "TextPoint.h"
#include <wx/window.h>
#include <wx/clipbrd.h>

extern ocpn_draw_pi                 *g_ocpn_draw_pi;
extern PathManagerDialog            *g_pPathManagerDialog;
extern ODSelect                     *g_pODSelect;
extern ODConfig                     *g_pODConfig;
extern PlugIn_ViewPort              *g_pivp;
extern ChartCanvas                  *ocpncc1;
extern ODPathPropertiesDialogImpl   *g_pODPathPropDialog;
extern PathMan                      *g_pPathMan;

extern ODPointPropertiesImpl        *g_pODPointPropDialog;
extern ODPath                       *g_PathToEdit;
extern PointMan                     *g_pODPointMan;
extern bool                         g_bShowMag;
extern bool                         g_bConfirmObjectDelete;
extern ODRolloverWin                *g_pODRolloverWin;
extern SelectItem                   *g_pRolloverPathSeg;
extern SelectItem                   *g_pRolloverPoint;
extern int                          g_cursor_x;
extern int                          g_cursor_y;
extern ODPlugIn_Position_Fix_Ex     g_pfFix;
extern ODDRDialogImpl               *g_pODDRDialog;

// Event Handler implementation 

BEGIN_EVENT_TABLE ( ODEventHandler, wxEvtHandler ) 
    //EVT_TIMER( OD_TIMER_1, ODEventHandler::OnODTimer1 )
    EVT_TIMER( -1, ODEventHandler::OnODTimer1 )
END_EVENT_TABLE()


ODEventHandler::ODEventHandler(ocpn_draw_pi *parent)
{
    //ctor
    m_parent = parent;
    m_parentcanvas = (ChartCanvas *)GetOCPNCanvasWindow();
    ODTimer1.SetOwner( this );
    ODTimer1.Start( TIMER_OD_1, wxTIMER_CONTINUOUS );
}


ODEventHandler::ODEventHandler(ChartCanvas *parent, ODPath *selectedPath, ODPoint *selectedODPoint)
{
    m_pBoundary = NULL;
    m_pEBL = NULL;
    m_pDR = NULL;
    m_pFoundTextPoint = NULL;
    g_pRolloverPoint = NULL;
    
    m_parentcanvas = parent;
    if(selectedPath->m_sTypeString == wxT("Boundary")) {
        m_pBoundary = (Boundary *)selectedPath;
        m_pSelectedPath = m_pBoundary;
    } else if(selectedPath->m_sTypeString == wxT("EBL")) {
        m_pEBL = (EBL *)selectedPath;
        m_pSelectedPath = m_pEBL;
    } else if(selectedPath->m_sTypeString == wxT("DR")) {
        m_pDR = (DR *)selectedPath;
        m_pSelectedPath = m_pDR;
    } else
        m_pSelectedPath = selectedPath;

    if(selectedODPoint->m_sTypeString == wxT("Text Point")) {
        m_pFoundTextPoint = (TextPoint *)selectedODPoint;
        m_pFoundODPoint = m_pFoundTextPoint;
    } else
        m_pFoundODPoint = selectedODPoint;
    
}

ODEventHandler::ODEventHandler(ChartCanvas *parent, ODPath *selectedPath, TextPoint *selectedTextPoint)
{
    m_pBoundary = NULL;
    m_pEBL = NULL;
    m_pDR = NULL;
    m_pFoundTextPoint = NULL;
    g_pRolloverPoint = NULL;
    
    m_parentcanvas = parent;
    if(selectedPath->m_sTypeString == wxT("Boundary")) {
        m_pBoundary = (Boundary *)selectedPath;
        m_pSelectedPath = m_pBoundary;
    } else if(selectedPath->m_sTypeString == wxT("EBL")) {
        m_pEBL = (EBL *)selectedPath;
        m_pSelectedPath = m_pEBL;
    } else if(selectedPath->m_sTypeString == wxT("DR")) {
        m_pDR = (DR *)selectedPath;
        m_pSelectedPath = m_pDR;
    } else
        m_pSelectedPath = selectedPath;

    m_pFoundODPoint = selectedTextPoint;
}

void ODEventHandler::SetPath( ODPath *path )
{
    m_pBoundary = NULL;
    m_pEBL = NULL;
    m_pSelectedPath = NULL;
    if(path) {
        if(path->m_sTypeString == wxT("Boundary")) {
            m_pBoundary = (Boundary *)path;
            m_pSelectedPath = m_pBoundary;
        } else if(path->m_sTypeString == wxT("EBL")) {
            m_pEBL = (EBL *)path;
            m_pSelectedPath = m_pEBL;
        } else if(path->m_sTypeString == wxT("DR")) {
            m_pDR = (DR *)path;
            m_pSelectedPath = m_pDR;
        } else
            m_pSelectedPath = path;
    }
}

void ODEventHandler::SetPoint( ODPoint* point )
{
    m_pFoundODPoint = point;
}

void ODEventHandler::SetPoint( TextPoint* point )
{
    m_pFoundODPoint = point;
}

void ODEventHandler::SetCanvas( ChartCanvas* canvas )
{
    m_parentcanvas = canvas;
}

void ODEventHandler::SetLatLon( double lat, double lon )
{
    m_cursor_lat = lat;
    m_cursor_lon = lon;
}


ODEventHandler::~ODEventHandler()
{
    //dtor
    ODTimer1.Stop();
}

void ODEventHandler::OnODTimer1( wxTimerEvent& event )
{
    g_ocpn_draw_pi->nBlinkerTick++; 
    if( ( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) ||
        ( g_pPathManagerDialog && g_pPathManagerDialog->IsShown() ) ||
        ( g_pODPathPropDialog && g_pODPathPropDialog->IsShown() ) )
        RequestRefresh( m_parentcanvas );
}

void ODEventHandler::OnRolloverPopupTimerEvent( wxTimerEvent& event )
{
    #ifdef __OCPN__ANDROID__
    return;
    #endif
    
    bool b_need_refresh = false;
    
    bool showRollover = false;
    
    if( !g_pRolloverPathSeg && !g_pRolloverPoint ) {
        //    Get a list of all selectable sgements, and search for the first visible segment as the rollover target.
        SelectableItemList SelList = g_pODSelect->FindSelectionList( g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon, SELTYPE_PATHSEGMENT );
        
        wxSelectableItemListNode *node = SelList.GetFirst();
        while( node ) {
            SelectItem *pFindSel = node->GetData();
            ODPath *pp = (ODPath *) pFindSel->m_pData3;        //candidate
            
            if( pp && pp->IsVisible() ) {
                g_pRolloverPathSeg = pFindSel;
                
                if( NULL == g_pODRolloverWin ) {
                    g_pODRolloverWin = new ODRolloverWin( g_ocpn_draw_pi->m_parent_window );
                    g_pODRolloverWin->IsActive( false );
                }
                
                if( !g_pODRolloverWin->IsActive() ) {
                    wxString s;
                    ODPoint *segShow_point_a = (ODPoint *) g_pRolloverPathSeg->m_pData1;
                    ODPoint *segShow_point_b = (ODPoint *) g_pRolloverPathSeg->m_pData2;
                    
                    double brgFrom, brgTo, dist;
                    DistanceBearingMercator_Plugin( segShow_point_b->m_lat, segShow_point_b->m_lon,
                                             segShow_point_a->m_lat, segShow_point_a->m_lon, &brgFrom, &dist );
                    DistanceBearingMercator_Plugin( segShow_point_a->m_lat, segShow_point_a->m_lon,
                                             segShow_point_b->m_lat, segShow_point_b->m_lon, &brgTo, &dist );
                    
                    if( !pp->m_bIsInLayer ) {
                        wxString wxsText;
                        wxsText.append( pp->m_sTypeString );
                        wxsText.append( wxT(": ") );
                        s.append( wxsText );
                    }
                    else {
                        wxString wxsText;
                        wxsText.append( _("Layer ") );
                        wxsText.append( pp->m_sTypeString );
                        wxsText.append( wxT(": ") );
                        s.Append( wxsText );
                    }
                    
                    if( pp->m_PathNameString.IsEmpty() ) s.Append( _("(unnamed)") );
                    else
                        s.Append( pp->m_PathNameString );
                    
                    s << _T("\n") << _("Total Length: ") << g_ocpn_draw_pi->FormatDistanceAdaptive( pp->m_path_length)
                    << _T("\n") << _("Leg: from ") << segShow_point_a->GetName()
                    << _(" to ") << segShow_point_b->GetName()
                    << _T("\n");
                    
                    if(pp->m_sTypeString == wxT("EBL")) {
                        s << _("From: ");
                        if( g_bShowMag )
                            s << wxString::Format( wxString("%03d°(M)  ", wxConvUTF8 ), (int)g_ocpn_draw_pi->GetTrueOrMag( brgFrom ) );
                        else
                            s << wxString::Format( wxString("%03d°  ", wxConvUTF8 ), (int)g_ocpn_draw_pi->GetTrueOrMag( brgFrom ) );
                        s << _(" To: ");
                        if( g_bShowMag )
                            s << wxString::Format( wxString("%03d°(M)  ", wxConvUTF8 ), (int)g_ocpn_draw_pi->GetTrueOrMag( brgTo ) );
                        else
                            s << wxString::Format( wxString("%03d°  ", wxConvUTF8 ), (int)g_ocpn_draw_pi->GetTrueOrMag( brgTo ) );
                        s << _T("\n");
                    } else {
                        if( g_bShowMag )
                            s << wxString::Format( wxString("%03d°(M)  ", wxConvUTF8 ), (int)g_ocpn_draw_pi->GetTrueOrMag( brgFrom ) );
                        else
                            s << wxString::Format( wxString("%03d°  ", wxConvUTF8 ), (int)g_ocpn_draw_pi->GetTrueOrMag( brgFrom ) );
                    }
                    
                    s << g_ocpn_draw_pi->FormatDistanceAdaptive( dist );
                    
                    // Compute and display cumulative distance from route start point to current
                    // leg end point.
                    
                    if( segShow_point_a != pp->m_pODPointList->GetFirst()->GetData() ) {
                        wxODPointListNode *node = (pp->m_pODPointList)->GetFirst()->GetNext();
                        ODPoint *pop;
                        float dist_to_endleg = 0;
                        wxString t;
                        
                        while( node ) {
                            pop = node->GetData();
                            dist_to_endleg += pop->m_seg_len;
                            if( pop->IsSame( segShow_point_a ) ) break;
                            node = node->GetNext();
                        }
                        s << _T(" (+") << g_ocpn_draw_pi->FormatDistanceAdaptive( dist_to_endleg ) << _T(")");
                    }
                    
                    g_pODRolloverWin->SetString( s );
                    
                    wxSize win_size = ocpncc1->GetSize();
                    //if( console && console->IsShown() ) win_size.x -= console->().x;
                    wxPoint point;
                    GetCanvasPixLL( g_pivp, &point, g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon );
                    PlugInNormalizeViewport( g_pivp );
                    g_pODRolloverWin->SetBestPosition( g_cursor_x, g_cursor_y, 16, 16, PATH_ROLLOVER, win_size );
                    g_pODRolloverWin->SetBitmap( PATH_ROLLOVER );
                    g_pODRolloverWin->IsActive( true );
                    b_need_refresh = true;
                    showRollover = true;
                    break;
                }
            } else
                node = node->GetNext();
        }
        
        if( !showRollover && !g_pRolloverPathSeg) {
            SelList = g_pODSelect->FindSelectionList( g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon, SELTYPE_ODPOINT );
            node = SelList.GetFirst();
            while( node ) {
                SelectItem *pFindSel = node->GetData();
                ODPoint *pp = (ODPoint *) pFindSel->m_pData1;        //candidate
                if( pp && pp->IsVisible() ) {
                    g_pRolloverPoint = pFindSel;
                    showRollover = true;
                    
                    if( NULL == g_pODRolloverWin ) {
                        g_pODRolloverWin = new ODRolloverWin( g_ocpn_draw_pi->m_parent_window );
                        g_pODRolloverWin->IsActive( false );
                    }
                    
                    TextPoint *tp = NULL;
                    if( pp->m_sTypeString == wxT("Text Point") ) tp = (TextPoint *) pFindSel->m_pData1;

                    if( tp && tp->m_iDisplayTextWhen == ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ON_ROLLOVER ) {
                        tp->m_bShowDisplayTextOnRollover = true;
                        showRollover = true;
                        b_need_refresh = true;
                        break;
                    } else 
                        
                        if( !g_pODRolloverWin->IsActive() ) {
                        wxString s;
                        if( !pp->m_bIsInLayer ) {
                            wxString wxsText;
                            wxsText.append( pp->m_sTypeString );
                            wxsText.append( wxT(": ") );
                            s.append( wxsText );
                        }
                        else {
                            wxString wxsText;
                            wxsText.append( _("Layer") );
                            wxsText.append( wxT(" ") );
                            wxsText.append( pp->m_sTypeString );
                            wxsText.append( wxT(": ") );
                            s.Append( wxsText );
                        }
                        if( pp->m_ODPointName.length() > 0 ) {
                            wxString wxsText;
                            wxsText.append( _("Name") );
                            wxsText.append( wxT(" : ") );
                            wxsText.append( pp->m_ODPointName );
                            s.Append( wxsText );
                        }
                        g_pODRolloverWin->SetString( s );
                        
                        wxSize win_size = ocpncc1->GetSize();
                        //if( console && console->IsShown() ) win_size.x -= console->().x;
                        wxPoint point;
                        GetCanvasPixLL( g_pivp, &point, g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon );
                        PlugInNormalizeViewport( g_pivp );
                        g_pODRolloverWin->SetBestPosition( g_cursor_x, g_cursor_y, 16, 16, POINT_ROLLOVER, win_size );
                        g_pODRolloverWin->SetBitmap( POINT_ROLLOVER );
                        g_pODRolloverWin->IsActive( true );
                        b_need_refresh = true;
                        showRollover = true;
                        break;
                    }
                }
                node = node->GetNext();
            }
        }
    } else {
        //    Is the cursor still in select radius?
        if(g_pRolloverPathSeg) {
            if( !g_pODSelect->IsSelectableSegmentSelected( g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon, g_pRolloverPathSeg ) ) 
                showRollover = false;
            else
                showRollover = true;
        } else if(g_pRolloverPoint) {
            if( !g_pODSelect->IsSelectableSegmentSelected( g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon, g_pRolloverPoint ) ) 
                showRollover = false;
            else
                showRollover = true;
        }
    }
    
    //    If currently creating a Path, do not show this rollover window
    if( g_ocpn_draw_pi->nBoundary_State || g_ocpn_draw_pi->nEBL_State || g_ocpn_draw_pi->nDR_State || g_ocpn_draw_pi->nPoint_State )
        showRollover = false;
    
    if( g_pODRolloverWin && g_pODRolloverWin->IsActive() && !showRollover ) {
        g_pODRolloverWin->IsActive( false );
        g_pRolloverPathSeg = NULL;
        if(g_pRolloverPoint) {
            TextPoint *tp = (TextPoint *) g_pRolloverPoint->m_pData1;
            if( tp && tp->m_sTypeString == wxT("Text Point")) {
                tp->m_bShowDisplayTextOnRollover = false;
                tp = NULL;
            }
        }
        g_pRolloverPoint = NULL;
        g_pODRolloverWin->Destroy();
        g_pODRolloverWin = NULL;
        b_need_refresh = true;
    } else if( g_pODRolloverWin && showRollover ) {
        g_pODRolloverWin->IsActive( true );
        g_pODRolloverWin->Show();
        b_need_refresh = true;
    }
    
    if( b_need_refresh )
        RequestRefresh( g_ocpn_draw_pi->m_parent_window );
}

void ODEventHandler::PopupMenuHandler(wxCommandEvent& event ) 
{
    int dlg_return;
    
    wxPoint r;
    
    switch( event.GetId() )
    {            
        case ID_PATH_MENU_PROPERTIES:
            if( NULL == g_pPathManagerDialog )         // There is one global instance of the Dialog
                g_pPathManagerDialog = new PathManagerDialog( ocpncc1 );
            
            DimeWindow( g_pPathManagerDialog );
            g_pPathManagerDialog->ShowPathPropertiesDialog( m_pSelectedPath );
            m_pSelectedPath = NULL;
            break;
        case ID_PATH_MENU_MOVE_POINT:
            m_pSelectedPath->m_bIsBeingEdited = TRUE;
            g_PathToEdit = m_pSelectedPath;
            g_ocpn_draw_pi->m_bPathEditing = TRUE;
            break;
        case ID_PATH_MENU_MOVE_PATH:
            g_ocpn_draw_pi->m_pFoundODPoint = NULL; // Make sure we dont process a single point
            m_pFoundODPoint = NULL;
            m_pSelectedPath->m_bIsBeingEdited = TRUE;
            g_PathToEdit = m_pSelectedPath;
            g_ocpn_draw_pi->m_bPathEditing = TRUE;
            break;
        case ID_PATH_MENU_INSERT:
            // Insert new OD Point
            m_pSelectedPath->InsertPointAfter( m_pFoundODPoint, m_cursor_lat, m_cursor_lon );
            
            g_pODSelect->DeleteAllSelectableODPoints( m_pSelectedPath );
            g_pODSelect->DeleteAllSelectablePathSegments( m_pSelectedPath );
            
            g_pODSelect->AddAllSelectablePathSegments( m_pSelectedPath );
            g_pODSelect->AddAllSelectableODPoints( m_pSelectedPath );
            
            m_pSelectedPath->RebuildGUIDList();          // ensure the GUID list is intact and good
            g_pODConfig->UpdatePath( m_pSelectedPath );
            
            if( g_pODPathPropDialog && ( g_pODPathPropDialog->IsShown() ) ) {
                g_pODPathPropDialog->SetPathAndUpdate( m_pSelectedPath, true );
            }
            
            break;
        case ID_PATH_MENU_DELETE: {
            dlg_return = wxID_YES;
            if( g_bConfirmObjectDelete ) {
                wxString sTypeLong = _("Are you sure you want to delete this ");
                sTypeLong.append( _(m_pSelectedPath->m_sTypeString) );
                sTypeLong.append( wxT("?") );
                wxString sTypeShort = wxT("OpenCPN ");
                sTypeShort.append( _(m_pSelectedPath->m_sTypeString) );
                sTypeShort.append( _(" Delete") );
                dlg_return = OCPNMessageBox_PlugIn( m_parentcanvas,  sTypeLong, sTypeShort, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
            }
            
            if( dlg_return == wxID_YES ) {
                DeletePath();
            }
            break;
        }
        case ID_PATH_MENU_DEACTIVATE: {
            wxString msg_id( _T("OCPN_PATH_DEACTIVATED") );
            wxString msg;
            msg.append( _("Name: ") );
            msg.append( m_pSelectedPath->m_PathNameString.c_str() );
            msg.append( wxT(", ") );
            msg.append( _("GUID") );
            msg.append( wxT(": ") );
            msg.append( m_pSelectedPath->m_GUID );
            SendPluginMessage( msg_id, msg );
            
            g_pPathMan->DeactivatePath( m_pSelectedPath );
            m_pSelectedPath = NULL;
            break;
        }
        case ID_PATH_MENU_ACTIVATE: {
            wxString msg_id( wxS("OCPN_PATH_ACTIVATED") );
            wxString msg;
            msg.append( _("Name: ") );
            msg.append( m_pSelectedPath->m_PathNameString.c_str() );
            msg.append( wxT(", ") );
            msg.append( _("GUID") );
            msg.append( wxT(": ") );
            msg.append( m_pSelectedPath->m_GUID );
            SendPluginMessage( msg_id, msg );
            
            g_pPathMan->ActivatePath( m_pSelectedPath );
            m_pSelectedPath = NULL;
            break;
        }
        case ID_EBL_MENU_CENTRE_ON_BOAT:
            m_pEBL->CentreOnBoat();
            break;
        case ID_EBL_MENU_CENTRE_ON_BOAT_LATLON:
            m_pEBL->CentreOnLatLon( g_pfFix.Lat, g_pfFix.Lon );
            break;
        case ID_EBL_MENU_PICK_NEW_START:
            g_ocpn_draw_pi->m_bEBLMoveOrigin = true;
            g_ocpn_draw_pi->m_pCurrentCursor = ocpncc1->pCursorCross;
            break;
        case ID_EBL_MENU_VRM_MATCH_EBL_COLOUR: {
            ODPoint *pFirstPoint = m_pEBL->m_pODPointList->GetFirst()->GetData();
            pFirstPoint->SetODPointRangeRingsColour( m_pEBL->GetCurrentColour() );
            break;
        }
        case ID_ODPOINT_MENU_PROPERTIES:
            if( NULL == g_pPathManagerDialog )         // There is one global instance of the Dialog
                g_pPathManagerDialog = new PathManagerDialog( g_ocpn_draw_pi->m_parent_window );
            
            g_pPathManagerDialog->ODPointShowPropertiesDialog( m_pFoundODPoint, g_ocpn_draw_pi->m_parent_window );
            m_pFoundODPoint = NULL;
            break;
        case ID_PATH_MENU_ACTPOINT:
        case ID_PATH_MENU_ACTNXTPOINT:
            break;
        case ID_PATH_MENU_REMPOINT: {
            dlg_return = wxID_YES;
            if( g_bConfirmObjectDelete ) {
                wxString sMessage( _("Are you sure you want to remove this ") );
                wxString sCaption( _("OCPN Draw Remove ") );
                wxString sType( wxS("") );
                if (!m_pFoundODPoint || m_pFoundODPoint->GetTypeString().IsNull() || m_pFoundODPoint->GetTypeString().IsEmpty() )
                    sType.append( _("OD Point") );
                else
                    sType.append( _(m_pFoundODPoint->GetTypeString()) );
                sMessage.append( sType );
                sMessage.append( wxS("?") );
                sCaption.append( sType );
                
                dlg_return = OCPNMessageBox_PlugIn( m_parentcanvas, sMessage, sCaption, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
            }
            
            if( dlg_return == wxID_YES ) {
                m_pSelectedPath->RemovePointFromPath( m_pFoundODPoint, m_pSelectedPath );
                m_pFoundODPoint->SetTypeString( _("OD Point") );
            }
            
            g_pODSelect->DeleteAllSelectableODPoints( m_pSelectedPath );
            g_pODSelect->DeleteAllSelectablePathSegments( m_pSelectedPath );
            
            g_pODSelect->AddAllSelectablePathSegments( m_pSelectedPath );
            g_pODSelect->AddAllSelectableODPoints( m_pSelectedPath );

            g_ocpn_draw_pi->m_bPathEditing = FALSE;
            g_ocpn_draw_pi->m_bODPointEditing = FALSE;
            m_pFoundODPoint = NULL;
            m_pSelectedPath = NULL;
            break;
        }
        case ID_ODPOINT_MENU_MOVE:
            m_pFoundODPoint->m_bIsBeingEdited = TRUE;
            g_ocpn_draw_pi->m_bODPointEditing = TRUE;
            break;
        case ID_ODPOINT_MENU_COPY:
            break;
        case ID_PATH_MENU_DELPOINT: {
            dlg_return = wxID_YES;
            if( g_bConfirmObjectDelete ) {
                wxString sMessage( _("Are you sure you want to delete this ") );
                wxString sCaption( _("OCPN Draw Delete ") );
                wxString sType( wxS("") );
                if (!m_pFoundODPoint || m_pFoundODPoint->GetTypeString().IsNull() || m_pFoundODPoint->GetTypeString().IsEmpty() )
                    sType.append( _("OD Point") );
                else
                    sType.append( _(m_pFoundODPoint->GetTypeString()) );
                sMessage.append( sType );
                sMessage.append( wxS("?") );
                sCaption.append( sType );
                
                dlg_return = OCPNMessageBox_PlugIn( m_parentcanvas, sMessage, sCaption, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
            }
            
            if( dlg_return == wxID_YES ) {
                if(m_pSelectedPath->GetnPoints() <= 3)
                    DeletePath();
                else {
                    m_pSelectedPath->DeletePoint( m_pFoundODPoint );
                    if( g_pODPathPropDialog && g_pODPathPropDialog->IsShown() ) g_pODPathPropDialog->SetPathAndUpdate( m_pSelectedPath, true );
                }
            }
            g_ocpn_draw_pi->m_bPathEditing = FALSE;
            g_ocpn_draw_pi->m_bODPointEditing = FALSE;
            m_pFoundODPoint = NULL;
            m_pSelectedPath = NULL;
            break;
        }
        case ID_ODPOINT_MENU_DELPOINT: {
            dlg_return = wxID_YES;
            if( g_bConfirmObjectDelete ) {
                wxString sMessage( _("Are you sure you want to delete this ") );
                wxString sCaption( _("OCPN Draw Delete ") );
                wxString sType( wxS("") );
                if (!m_pFoundODPoint || m_pFoundODPoint->GetTypeString().IsNull() || m_pFoundODPoint->GetTypeString().IsEmpty() )
                    sType.append( _("OD Point") );
                else
                    sType.append( _(m_pFoundODPoint->GetTypeString()) );
                sMessage.append( sType );
                sMessage.append( wxS("?") );
                sCaption.append( sType );
                
                dlg_return = OCPNMessageBox_PlugIn( m_parentcanvas, sMessage, sCaption, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
            }
            
            if( dlg_return == wxID_YES ) {
                // If the ODPoint belongs to an invisible path, we come here instead of to ID_PATH_MENU_DELPOINT
                //  Check it, and if so then remove the point from its routes
                wxArrayPtrVoid *ppath_array = g_pPathMan->GetPathArrayContaining( m_pFoundODPoint );
                if( ppath_array ) {
                    g_pODPointMan->DestroyODPoint( m_pFoundODPoint );
                }
                else {
                    g_pODSelect->DeleteSelectablePoint( m_pFoundODPoint, SELTYPE_ODPOINT );
                    g_pODConfig->DeleteODPoint( m_pFoundODPoint );
                    if( NULL != g_pODPointMan )
                        g_pODPointMan->RemoveODPoint( m_pFoundODPoint );
                }
                
                if( g_pPathManagerDialog && g_pPathManagerDialog->IsShown() )
                    g_pPathManagerDialog->UpdateODPointsListCtrl();
            }
            g_ocpn_draw_pi->m_bPathEditing = FALSE;
            g_ocpn_draw_pi->m_bODPointEditing = FALSE;
            if(m_pFoundODPoint->m_sTypeString == wxT("Text Point"))
                delete (TextPoint *)m_pFoundODPoint;
            else
                delete m_pFoundODPoint;
            m_pFoundODPoint = NULL;
            break;
        }
        case ID_PATH_MENU_COPY_GUID: {
            if(wxTheClipboard->Open()) {
                wxTheClipboard->SetData( new wxTextDataObject(m_pSelectedPath->m_GUID));
                wxTheClipboard->Close();
            }
            break;
        }
        case ID_ODPOINT_MENU_COPY_GUID: {
            if(wxTheClipboard->Open()) {
                wxTheClipboard->SetData( new wxTextDataObject(m_pFoundODPoint->m_GUID));
                wxTheClipboard->Close();
            }
            break;
        }
        case ID_DR_MENU_UPDATE_INITIAL_CONDITIONS:
            if( NULL == g_pODDRDialog )         // There is one global instance of the Dialog
                g_pODDRDialog = new ODDRDialogImpl( ocpncc1 );
            
            g_pODDRDialog->UpdateDialog( m_pDR );
            DimeWindow( g_pODDRDialog );
            g_pODDRDialog->Show();
            
            
            //    Required if RMDialog is not STAY_ON_TOP
#ifdef __WXOSX__
            g_pODDRDialog->Centre();
            g_pODDRDialog->Raise();
#endif
            
            RequestRefresh( g_ocpn_draw_pi->m_parent_window );
            
            break;
            
    }
    
} 

void ODEventHandler::PopupMenu( int seltype )
{
    wxMenu* contextMenu = new wxMenu;
    wxMenu* menuODPoint = NULL;
    wxMenu* menuPath = NULL;
    
    wxMenu *menuFocus = contextMenu;    // This is the one that will be shown
 
    wxString sString;
    
    //  This is the default context menu
    menuFocus = contextMenu;
    
    if( seltype & SELTYPE_PATHSEGMENT ) {
        bool blay = false;
        if( m_pSelectedPath && m_pSelectedPath->m_bIsInLayer )
            blay = true;
        
        if( blay ) {
            wxString  tName;
            tName.Append( _("Layer ") );
            tName.Append( _(m_pSelectedPath->m_sTypeString) );
            menuPath = new wxMenu( tName );
            MenuAppend( menuPath, ID_PATH_MENU_PROPERTIES, _( "Properties..." ) );
        }
        else {
            menuPath = new wxMenu( _(m_pSelectedPath->m_sTypeString) );
            MenuAppend( menuPath, ID_PATH_MENU_PROPERTIES, _( "Properties..." ) );
            if(m_pSelectedPath->m_sTypeString == wxT("EBL")) {
                if(!m_pEBL->m_bCentreOnBoat) {
                    MenuAppend( menuPath, ID_EBL_MENU_CENTRE_ON_BOAT, _("Centre on moving boat") );
                    MenuAppend( menuPath, ID_EBL_MENU_CENTRE_ON_BOAT_LATLON, _("Centre on boat lat lon") );
                } else
                    MenuAppend( menuPath, ID_EBL_MENU_PICK_NEW_START, _("Pick a new start point") );
                ODPoint *pFirstPoint = m_pEBL->m_pODPointList->GetFirst()->GetData();
                if(m_pEBL->GetCurrentColour() != pFirstPoint->GetODPointRangeRingsColour())
                    MenuAppend( menuPath, ID_EBL_MENU_VRM_MATCH_EBL_COLOUR, _("Match VRM colour to EBL colour"));
            }
            else if(m_pSelectedPath->m_sTypeString == wxT("DR")) {
                MenuAppend( menuPath, ID_DR_MENU_UPDATE_INITIAL_CONDITIONS, _("Update initial conditions") );
            }
            else if(m_pSelectedPath->m_sTypeString != wxT("DR")) {
                sString.clear();
                sString.append( _("Move") );
                sString.append(_T(" "));
                sString.append( _(m_pSelectedPath->m_sTypeString) );
                MenuAppend( menuPath, ID_PATH_MENU_MOVE_PATH, sString );
                sString.clear();
                sString.append( _("Insert") );
                sString.append(_T(" "));
                sString.append( _(m_pSelectedPath->m_sTypeString) );
                sString.append( _(" Point") );
                MenuAppend( menuPath, ID_PATH_MENU_INSERT, sString );
            }
            sString.clear();
            sString.append( _("Delete") );
            sString.append(_T("..."));
            MenuAppend( menuPath, ID_PATH_MENU_DELETE, sString );
            if(m_pSelectedPath->m_sTypeString != wxT("EBL")) {
                if ( m_pSelectedPath->m_bPathIsActive ) MenuAppend( menuPath, ID_PATH_MENU_DEACTIVATE, _( "Deactivate") );
                else  MenuAppend( menuPath, ID_PATH_MENU_ACTIVATE, _( "Activate" ) );
            }
            sString.clear();
            sString.append(_("Copy"));
            sString.append(_T(" "));
            sString.append( _(m_pSelectedPath->m_sTypeString) );
            sString.append(_T(" "));
            sString.append(_("GUID"));
            MenuAppend( menuPath, ID_PATH_MENU_COPY_GUID, sString );
        }
        
        //      Set this menu as the "focused context menu"
        menuFocus = menuPath;
    }
    
    if( seltype & SELTYPE_ODPOINT ) {
        bool blay = false;
        if( m_pFoundODPoint && m_pFoundODPoint->m_bIsInLayer )
            blay = true;
        
        if( blay ){
            sString.clear();
            sString.append( _("Layer ") );
            sString.append( _(m_pFoundODPoint->m_sTypeString) );
            menuODPoint = new wxMenu( sString );
            MenuAppend( menuODPoint, ID_ODPOINT_MENU_PROPERTIES, _( "Properties..." ) );
            
            //if( m_pSelectedPath && m_pSelectedPath->IsActive() )
            //    MenuAppend( menuODPoint, ID_PATH_MENU_ACTPOINT, _( "Activate" ) );
        }
        else {
            sString.clear();
            sString.append( _(m_pFoundODPoint->m_sTypeString) );
            menuODPoint = new wxMenu( sString );
            MenuAppend( menuODPoint, ID_ODPOINT_MENU_PROPERTIES, _( "Properties..." ) );
            sString.clear();
            sString.append( _("Move ") );
            sString.append( _(m_pFoundODPoint->m_sTypeString) );
            MenuAppend( menuODPoint, ID_ODPOINT_MENU_MOVE, sString );

            if( m_pSelectedPath ) {
                if( m_pSelectedPath->m_sTypeString != wxT("DR") ) {
                if( m_pSelectedPath->GetnPoints() > 2 )
                    MenuAppend( menuODPoint, ID_PATH_MENU_REMPOINT, _( "Remove Point from Path" ) );
                
                    MenuAppend( menuODPoint, ID_PATH_MENU_DELPOINT,  _( "Delete" ) );
                }
            } else 
                MenuAppend( menuODPoint, ID_ODPOINT_MENU_DELPOINT,  _( "Delete" ) );
            MenuAppend( menuODPoint, ID_ODPOINT_MENU_COPY_GUID, _("Copy Point GUID") );
            
        }
        //      Set this menu as the "focused context menu"
        menuFocus = menuODPoint;
    }
    
    if( ( m_pSelectedPath ) ) {
        m_pSelectedPath->m_bPathIsSelected = true;
        RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    } else if( m_pFoundODPoint ) {
        m_pFoundODPoint->m_bPtIsSelected = true;
        RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    }
    
    //        Invoke the correct focused drop-down menu
    m_parentcanvas->Connect( wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ODEventHandler::PopupMenuHandler ), NULL, this );
    m_parentcanvas->PopupMenu( menuFocus );
    m_parentcanvas->Disconnect( wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ODEventHandler::PopupMenuHandler ), NULL, this );
    
    // Cleanup
    if( ( m_pSelectedPath ) ) m_pSelectedPath->m_bPathIsSelected = false;
    m_pSelectedPath = NULL;
    
    if( m_pFoundODPoint ) m_pFoundODPoint->m_bPtIsSelected = false;
    m_pFoundODPoint = NULL;
    
    //m_pFoundODPointSecond = NULL;
    menuFocus = NULL;
    delete contextMenu;
    if(menuPath) delete menuPath;
    if( menuODPoint ) delete menuODPoint;
    
}

void ODEventHandler::DeletePath( void )
{
    if( g_pPathMan->GetpActivePath() == m_pSelectedPath ) g_pPathMan->DeactivatePath( m_pSelectedPath );
    
    if( !g_pPathMan->DeletePath( m_pSelectedPath ) )
        return;
    if( g_pODPathPropDialog && ( g_pODPathPropDialog->IsShown()) && (m_pSelectedPath == g_pODPathPropDialog->GetPath()) ) {
        g_pODPathPropDialog->Hide();
    }
    
    if( g_pPathManagerDialog && g_pPathManagerDialog->IsShown() )
        g_pPathManagerDialog->UpdatePathListCtrl();
    
    if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
        g_pODPointPropDialog->ValidateMark();
        g_pODPointPropDialog->UpdateProperties();
    }
    
    // TODO implement UNDO
    //m_parent->undo->InvalidateUndo();
    RequestRefresh( m_parentcanvas );
    m_pSelectedPath = NULL;
    
}