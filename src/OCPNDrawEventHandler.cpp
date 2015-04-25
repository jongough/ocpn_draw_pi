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

#include "OCPNDrawEventHandler.h"
#include "ocpn_draw_pi.h"
#include "ocpn_plugin.h"
#include "OCPNSelect.h"
#include "PathMan.h"
#include "PathProp.h"
#include "ODPointPropertiesImpl.h"
#include "ODRolloverWin.h"
#include "ODUtils.h"
#include "chcanv.h"
#include "PointMan.h"
#include <wx/window.h>

extern ocpn_draw_pi    *g_ocpn_draw_pi;
extern PathManagerDialog *pPathManagerDialog;
extern OCPNSelect      *pOCPNSelect;
extern OCPNDrawConfig  *pOCPNDrawConfig;
extern PlugIn_ViewPort *g_pivp;
extern ChartCanvas     *ocpncc1;
extern PathProp       *pPathPropDialog;
extern PathMan          *g_pPathMan;
extern ODPointPropertiesImpl    *g_pODPointPropDialog;
extern Path             *g_PathToEdit;
extern PointMan         *pOCPNPointMan;
extern bool             g_bShowMag;
extern bool             g_bConfirmObjectDelete;
extern ODRolloverWin     *g_pPathRolloverWin;
extern SelectItem      *g_pRolloverPathSeg;
extern int              g_cursor_x;
extern int              g_cursor_y;

// Event Handler implementation 

//BEGIN_EVENT_TABLE ( OCPNDrawEventHandler, wxEvtHandler ) 
//    EVT_TIMER ( ROLLOVER_TIMER, OCPNDrawEventHandler::OnTimerEvent ) 
    //EVT_TIMER ( ROPOPUP_TIMER, OCPNDrawEventHandler::OnRolloverPopupTimerEvent )
    //    EVT_TIMER ( HEAD_DOG_TIMER, OCPNDrawEventHandler::OnTimerEvent ) 
//    EVT_MENU ( ID_PATH_MENU_PROPERTIES, OCPNDrawEventHandler::PopupMenuHandler )
//END_EVENT_TABLE()


OCPNDrawEventHandler::OCPNDrawEventHandler(ocpn_draw_pi *parent)
{
    //ctor
    m_parent = parent;
}


OCPNDrawEventHandler::OCPNDrawEventHandler(ChartCanvas *parent,
                          Path *selectedPath,
                          OCPNPoint *selectedOCPNPoint)
{
    m_parentcanvas = parent;
    m_pSelectedPath = selectedPath;
    m_pFoundOCPNPoint = selectedOCPNPoint;
}

void OCPNDrawEventHandler::SetPath( Path *path )
{
    m_pSelectedPath = path;
}

void OCPNDrawEventHandler::SetPoint( OCPNPoint* point )
{
    m_pFoundOCPNPoint = point;
}

void OCPNDrawEventHandler::SetCanvas( ChartCanvas* canvas )
{
    m_parentcanvas = canvas;
}

void OCPNDrawEventHandler::SetLatLon( double lat, double lon )
{
    m_cursor_lat = lat;
    m_cursor_lon = lon;
}


OCPNDrawEventHandler::~OCPNDrawEventHandler()
{
    //dtor
}

void OCPNDrawEventHandler::OnTimerEvent(wxTimerEvent& event) 
{ 
    m_parent->ProcessTimerEvent( event ); 
} 

void OCPNDrawEventHandler::OnRolloverPopupTimerEvent( wxTimerEvent& event )
{
    #ifdef __OCPN__ANDROID__
    return;
    #endif
    
    bool b_need_refresh = false;
    
    bool showRollover = false;
    
    if( NULL == g_pRolloverPathSeg ) {
        //    Get a list of all selectable sgements, and search for the first visible segment as the rollover target.
        
        SelectableItemList SelList = pOCPNSelect->FindSelectionList( g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon,
                                                                     SELTYPE_PATHSEGMENT );
        
        int iTest = SelList.GetCount();
        wxSelectableItemListNode *node = SelList.GetFirst();
        while( node ) {
            SelectItem *pFindSel = node->GetData();
            
            Path *pp = (Path *) pFindSel->m_pData3;        //candidate
            
            if( pp && pp->IsVisible() ) {
                g_pRolloverPathSeg = pFindSel;
                showRollover = true;
                
                if( NULL == g_pPathRolloverWin ) {
                    g_pPathRolloverWin = new ODRolloverWin( g_ocpn_draw_pi->m_parent_window );
                    g_pPathRolloverWin->IsActive( false );
                }
                
                if( !g_pPathRolloverWin->IsActive() ) {
                    wxString s;
                    OCPNPoint *segShow_point_a = (OCPNPoint *) g_pRolloverPathSeg->m_pData1;
                    OCPNPoint *segShow_point_b = (OCPNPoint *) g_pRolloverPathSeg->m_pData2;
                    
                    double brg, dist;
                    DistanceBearingMercator( segShow_point_b->m_lat, segShow_point_b->m_lon,
                                             segShow_point_a->m_lat, segShow_point_a->m_lon, &brg, &dist );
                    
                    if( !pp->m_bIsInLayer ) {
                        wxString wxsText;
                        wxsText.append( pp->m_sTypeString );
                        wxsText.append( wxT(": ") );
                        s.append( wxsText );
                    }
                    else {
                        wxString wxsText;
                        wxsText.append( wxT("Layer ") );
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
                    
                    if( g_bShowMag )
                        s << wxString::Format( wxString("%03d°(M)  ", wxConvUTF8 ), (int)g_ocpn_draw_pi->GetTrueOrMag( brg ) );
                    else
                        s << wxString::Format( wxString("%03d°  ", wxConvUTF8 ), (int)g_ocpn_draw_pi->GetTrueOrMag( brg ) );
                    
                    s << g_ocpn_draw_pi->FormatDistanceAdaptive( dist );
                    
                    // Compute and display cumulative distance from route start point to current
                    // leg end point.
                    
                    if( segShow_point_a != pp->pOCPNPointList->GetFirst()->GetData() ) {
                        wxOCPNPointListNode *node = (pp->pOCPNPointList)->GetFirst()->GetNext();
                        OCPNPoint *pop;
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
                    
                    g_pPathRolloverWin->SetString( s );
                    
                    wxSize win_size = ocpncc1->GetSize();
                    //if( console && console->IsShown() ) win_size.x -= console->().x;
                    wxPoint point;
                    GetCanvasPixLL( g_pivp, &point, g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon );
                    PlugInNormalizeViewport( g_pivp );
                    g_pPathRolloverWin->SetBestPosition( g_cursor_x, g_cursor_y, 16, 16, PATH_ROLLOVER, win_size );
                    g_pPathRolloverWin->SetBitmap( PATH_ROLLOVER );
                    g_pPathRolloverWin->IsActive( true );
                    b_need_refresh = true;
                    showRollover = true;
                    break;
                }
            } else
                node = node->GetNext();
        }
    } else {
        //    Is the cursor still in select radius?
        if( !pOCPNSelect->IsSelectableSegmentSelected( g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon, g_pRolloverPathSeg ) ) 
            showRollover = false;
        else
            showRollover = true;
    }
    
    //    If currently creating a Path, do not show this rollover window
    if( g_ocpn_draw_pi->nPath_State )
        showRollover = false;
    
    if( g_pPathRolloverWin && g_pPathRolloverWin->IsActive() && !showRollover ) {
        g_pPathRolloverWin->IsActive( false );
        g_pRolloverPathSeg = NULL;
        g_pPathRolloverWin->Destroy();
        g_pPathRolloverWin = NULL;
        b_need_refresh = true;
    } else if( g_pPathRolloverWin && showRollover ) {
        g_pPathRolloverWin->IsActive( true );
        g_pPathRolloverWin->Show();
        b_need_refresh = true;
    }
    
    if( b_need_refresh )
        RequestRefresh( g_ocpn_draw_pi->m_parent_window );
}

void OCPNDrawEventHandler::PopupMenuHandler(wxCommandEvent& event ) 
{
    //m_parent->PopupMenuHandler( event ); 
    OCPNPoint *pLast;
    int dlg_return;
    
    wxPoint r;
    
    //ocpncc1->GetCanvasPixPoint( popx, popy, zlat, zlon );
    wxPoint wxp;
    wxp.x = popx;
    wxp.y = popy;
    
    switch( event.GetId() )
    {            
        case ID_PATH_MENU_PROPERTIES:
            if( NULL == pPathManagerDialog )         // There is one global instance of the Dialog
                pPathManagerDialog = new PathManagerDialog( ocpncc1 );
            
            pPathManagerDialog->ShowPathPropertiesDialog( m_pSelectedPath );
            break;
        case ID_PATH_MENU_MOVE_POINT:
            m_pSelectedPath->m_bIsBeingEdited = TRUE;
            g_PathToEdit = m_pSelectedPath;
            g_ocpn_draw_pi->m_bPathEditing = TRUE;
            break;
        case ID_PATH_MENU_INSERT:
            // Insert new OCPN Point
            m_pSelectedPath->InsertPointAfter( m_pFoundOCPNPoint, m_cursor_lat, m_cursor_lon );
            
            pOCPNSelect->DeleteAllSelectableOCPNPoints( m_pSelectedPath );
            pOCPNSelect->DeleteAllSelectablePathSegments( m_pSelectedPath );
            
            pOCPNSelect->AddAllSelectablePathSegments( m_pSelectedPath );
            pOCPNSelect->AddAllSelectableOCPNPoints( m_pSelectedPath );
            
            m_pSelectedPath->RebuildGUIDList();          // ensure the GUID list is intact and good
            pOCPNDrawConfig->UpdatePath( m_pSelectedPath );
            
            if( pPathPropDialog && ( pPathPropDialog->IsShown() ) ) {
                pPathPropDialog->SetPathAndUpdate( m_pSelectedPath, true );
            }
            
            break;
        case ID_PATH_MENU_DELETE: {
            dlg_return = wxID_YES;
            if( g_bConfirmObjectDelete ) {
                wxString sTypeLong = wxT("Are you sure you want to delete this ");
                sTypeLong.append( m_pSelectedPath->m_sTypeString );
                sTypeLong.append( wxT("?") );
                wxString sTypeShort = wxT("OpenCPN ");
                sTypeShort.append( m_pSelectedPath->m_sTypeString );
                sTypeShort.append( wxT(" Delete") );
                dlg_return = OCPNMessageBox_PlugIn( m_parentcanvas,  sTypeLong, sTypeShort, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
            }
            
            if( dlg_return == wxID_YES ) {
                if( g_pPathMan->GetpActivePath() == m_pSelectedPath ) g_pPathMan->DeactivatePath( m_pSelectedPath );
                
                if( !g_pPathMan->DeletePath( m_pSelectedPath ) )
                    break;
                if( pPathPropDialog && ( pPathPropDialog->IsShown()) && (m_pSelectedPath == pPathPropDialog->GetPath()) ) {
                    pPathPropDialog->Hide();
                }
                
                if( pPathManagerDialog && pPathManagerDialog->IsShown() )
                    pPathManagerDialog->UpdatePathListCtrl();
                
                if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
                    g_pODPointPropDialog->ValidateMark();
                    g_pODPointPropDialog->UpdateProperties();
                }
                
                // TODO implement UNDO
                //m_parent->undo->InvalidateUndo();
                RequestRefresh( m_parentcanvas );
                
            }
            break;
        }
        case ID_PATH_MENU_DEACTIVATE: {
            wxString msg_id( _T("OCPN_PATH_DEACTIVATED") );
            wxString msg;
            msg.append( wxS("Name: ") );
            msg.append( m_pSelectedPath->m_PathNameString.c_str() );
            msg.append( wxS(", GUID: ") );
            msg.append( m_pSelectedPath->m_GUID );
            SendPluginMessage( msg_id, msg );
            
            g_pPathMan->DeactivatePath( m_pSelectedPath );
            break;
        }
        case ID_PATH_MENU_ACTIVATE: {
            wxString msg_id( wxS("OCPN_PATH_ACTIVATED") );
            wxString msg;
            msg.append( wxS("Name: ") );
            msg.append( m_pSelectedPath->m_PathNameString.c_str() );
            msg.append( wxS(", GUID: ") );
            msg.append( m_pSelectedPath->m_GUID );
            SendPluginMessage( msg_id, msg );
            
            g_pPathMan->ActivatePath( m_pSelectedPath );
            break;
        }
        case ID_OCPNPOINT_MENU_PROPERTIES:
            if( NULL == pPathManagerDialog )         // There is one global instance of the Dialog
                pPathManagerDialog = new PathManagerDialog( ocpncc1 );
            
            pPathManagerDialog->OCPNPointShowPropertiesDialog( m_pFoundOCPNPoint, ocpncc1 );
            break;
        case ID_PATH_MENU_ACTPOINT:
        case ID_PATH_MENU_ACTNXTPOINT:
            break;
        case ID_PATH_MENU_REMPOINT: {
            wxString sMessage( wxS("Are you sure you want to remove this ") );
            wxString sCaption( wxS("OCPN Draw Remove ") );
            wxString sType( wxS("") );
            if (!m_pFoundOCPNPoint || m_pFoundOCPNPoint->GetTypeString().IsNull() || m_pFoundOCPNPoint->GetTypeString().IsEmpty() )
                sType.append( wxS("OCPN Point") );
            else
                sType.append( m_pFoundOCPNPoint->GetTypeString() );
            sMessage.append( sType );
            sMessage.append( wxS("?") );
            sCaption.append( sType );
            
            dlg_return = OCPNMessageBox_PlugIn( m_parentcanvas, sMessage, sCaption, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
            
            if( dlg_return == wxID_YES ) {
                m_pSelectedPath->RemovePointFromPath( m_pFoundOCPNPoint, m_pSelectedPath );
                m_pFoundOCPNPoint->SetTypeString( wxT("OCPN Point") );
            }
            
            pOCPNSelect->DeleteAllSelectableOCPNPoints( m_pSelectedPath );
            pOCPNSelect->DeleteAllSelectablePathSegments( m_pSelectedPath );
            
            pOCPNSelect->AddAllSelectablePathSegments( m_pSelectedPath );
            pOCPNSelect->AddAllSelectableOCPNPoints( m_pSelectedPath );

            g_ocpn_draw_pi->m_bPathEditing = FALSE;
            g_ocpn_draw_pi->m_bOCPNPointEditing = FALSE;
            
            break;
        }
        case ID_OCPNPOINT_MENU_EDIT:
            m_pFoundOCPNPoint->m_bIsBeingEdited = TRUE;
            g_ocpn_draw_pi->m_bOCPNPointEditing = TRUE;
            break;
        case ID_OCPNPOINT_MENU_COPY:
            break;
        case ID_PATH_MENU_DELPOINT: {
            wxString sMessage( wxS("Are you sure you want to delete this ") );
            wxString sCaption( wxS("OCPN Draw Delete ") );
            wxString sType( wxS("") );
            if (!m_pFoundOCPNPoint || m_pFoundOCPNPoint->GetTypeString().IsNull() || m_pFoundOCPNPoint->GetTypeString().IsEmpty() )
                sType.append( wxS("OCPN Point") );
            else
                sType.append( m_pFoundOCPNPoint->GetTypeString() );
            sMessage.append( sType );
            sMessage.append( wxS("?") );
            sCaption.append( sType );
            
            dlg_return = OCPNMessageBox_PlugIn( m_parentcanvas, sMessage, sCaption, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
            
            if( dlg_return == wxID_YES ) {
                m_pSelectedPath->DeletePoint( m_pFoundOCPNPoint );
                if( pPathPropDialog && pPathPropDialog->IsShown() ) pPathPropDialog->SetPathAndUpdate( m_pSelectedPath, true );
            }
            g_ocpn_draw_pi->m_bPathEditing = FALSE;
            g_ocpn_draw_pi->m_bOCPNPointEditing = FALSE;
            break;
        }
        case ID_OCPNPOINT_MENU_DELPOINT: {
            wxString sMessage( wxS("Are you sure you want to delete this ") );
            wxString sCaption( wxS("OCPN Draw Delete ") );
            wxString sType( wxS("") );
            if (!m_pFoundOCPNPoint || m_pFoundOCPNPoint->GetTypeString().IsNull() || m_pFoundOCPNPoint->GetTypeString().IsEmpty() )
                sType.append( wxS("OCPN Point") );
            else
                sType.append( m_pFoundOCPNPoint->GetTypeString() );
            sMessage.append( sType );
            sMessage.append( wxS("?") );
            sCaption.append( sType );
            
            dlg_return = OCPNMessageBox_PlugIn( m_parentcanvas, sMessage, sCaption, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
            
            if( dlg_return == wxID_YES ) {
                // If the OCPNPoint belongs to an invisible path, we come here instead of to ID_PATH_MENU_DELPOINT
                //  Check it, and if so then remove the point from its routes
                wxArrayPtrVoid *ppath_array = g_pPathMan->GetPathArrayContaining( m_pFoundOCPNPoint );
                if( ppath_array ) {
                    pOCPNPointMan->DestroyOCPNPoint( m_pFoundOCPNPoint );
                }
                else {
                    pOCPNDrawConfig->DeleteOCPNPoint( m_pFoundOCPNPoint );
                    pOCPNSelect->DeleteSelectablePoint( m_pFoundOCPNPoint, SELTYPE_OCPNPOINT );
                    if( NULL != pOCPNPointMan )
                        pOCPNPointMan->RemoveOCPNPoint( m_pFoundOCPNPoint );
                }
                
                if( pPathManagerDialog && pPathManagerDialog->IsShown() )
                    pPathManagerDialog->UpdateOCPNPointsListCtrl();
            }
            g_ocpn_draw_pi->m_bPathEditing = FALSE;
            g_ocpn_draw_pi->m_bOCPNPointEditing = FALSE;
            break;
        }
            
    }
    
} 

void OCPNDrawEventHandler::PopupMenu( int x, int y, int seltype )
{
    wxMenu* contextMenu = new wxMenu;
    wxMenu* menuOCPNPoint = new wxMenu( wxS("OCPNPoint") );
    wxMenu* menuPath = new wxMenu( wxS("Path") );
    
    wxMenu *subMenuChart = new wxMenu;
    
    wxMenu *menuFocus = contextMenu;    // This is the one that will be shown
    
    popx = x;
    popy = y;
    
    //  This is the default context menu
    menuFocus = contextMenu;
    
    if( seltype & SELTYPE_PATHSEGMENT ) {
        bool blay = false;
        if( m_pSelectedPath && m_pSelectedPath->m_bIsInLayer )
            blay = true;
        
        if( blay ) {
            delete menuPath;
            menuPath = new wxMenu( _("Layer Path") );
            MenuAppend( menuPath, ID_PATH_MENU_PROPERTIES, _( "Properties..." ) );
        }
        else {
            MenuAppend( menuPath, ID_PATH_MENU_PROPERTIES, _( "Properties..." ) );
            wxString sType;
            sType.append( wxS("Move ") );
            sType.append(m_pSelectedPath->m_sTypeString);
            sType.append( wxS(" Point") );
            MenuAppend( menuPath, ID_PATH_MENU_MOVE_POINT, sType );
            sType.clear();
            sType.append( wxS("Insert ") );
            sType.append(m_pSelectedPath->m_sTypeString);
            sType.append( wxT(" Point") );
            MenuAppend( menuPath, ID_PATH_MENU_INSERT, sType );
            MenuAppend( menuPath, ID_PATH_MENU_DELETE, _( "Delete..." ) );
            if ( m_pSelectedPath->m_bPathIsActive ) MenuAppend( menuPath, ID_PATH_MENU_DEACTIVATE, _( "Deactivate") );
            else  MenuAppend( menuPath, ID_PATH_MENU_ACTIVATE, _( "Activate" ) );
        }
        
        //      Set this menu as the "focused context menu"
        menuFocus = menuPath;
    }
    
    if( seltype & SELTYPE_OCPNPOINT ) {
        bool blay = false;
        if( m_pFoundOCPNPoint && m_pFoundOCPNPoint->m_bIsInLayer )
            blay = true;
        
        if( blay ){
            wxString sType;
            sType.append( wxT("Layer ") );
            sType.append( m_pFoundOCPNPoint->m_sTypeString );
            menuOCPNPoint->SetTitle( sType );
            MenuAppend( menuOCPNPoint, ID_OCPNPOINT_MENU_PROPERTIES, _( "Properties..." ) );
            
            //if( m_pSelectedPath && m_pSelectedPath->IsActive() )
            //    MenuAppend( menuOCPNPoint, ID_PATH_MENU_ACTPOINT, _( "Activate" ) );
        }
        else {
            wxString sType;
            sType.append( m_pFoundOCPNPoint->m_sTypeString );
            menuOCPNPoint->SetTitle( sType );
            MenuAppend( menuOCPNPoint, ID_OCPNPOINT_MENU_PROPERTIES, _( "Properties..." ) );
            sType.clear();
            sType.append( wxS("Move ") );
            sType.append(m_pFoundOCPNPoint->m_sTypeString);
            MenuAppend( menuOCPNPoint, ID_OCPNPOINT_MENU_EDIT, sType );

//            if( m_pSelectedPath && m_pSelectedPath->IsActive() ) {
//                if(m_pSelectedPath->m_pPathActivePoint != m_pFoundOCPNPoint )
//                    MenuAppend( menuOCPNPoint, ID_PATH_MENU_ACTPOINT, _( "Activate" ) );
//            }
            
//            if( m_pSelectedPath && m_pSelectedPath->IsActive() ) {
//                if(m_pSelectedPath->m_pPathActivePoint == m_pFoundOCPNPoint ) {
//                    int indexActive = m_pSelectedPath->GetIndexOf( m_pSelectedPath->m_pPathActivePoint );
//                    if( ( indexActive + 1 ) <= m_pSelectedPath->GetnPoints() )
//                        MenuAppend( menuOCPNPoint, ID_PATH_MENU_ACTNXTPOINT, _( "Activate Next OCPNPoint" ) );
//                }
//            }
            if( m_pSelectedPath && m_pSelectedPath->GetnPoints() > 2 )
                MenuAppend( menuOCPNPoint, ID_PATH_MENU_REMPOINT, _( "Remove Point from Path" ) );
            
            if( m_pSelectedPath )
                MenuAppend( menuOCPNPoint, ID_PATH_MENU_DELPOINT,  _( "Delete" ) );
            else
                MenuAppend( menuOCPNPoint, ID_OCPNPOINT_MENU_DELPOINT,  _( "Delete" ) );
            
        }
        //      Set this menu as the "focused context menu"
        menuFocus = menuOCPNPoint;
    }
    
    //        Invoke the correct focused drop-down menu
    m_parentcanvas->Connect( wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( OCPNDrawEventHandler::PopupMenuHandler ), NULL, this );
    m_parentcanvas->PopupMenu( menuFocus, x, y );
    m_parentcanvas->Disconnect( wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( OCPNDrawEventHandler::PopupMenuHandler ), NULL, this );
    
    // Cleanup
    if( ( m_pSelectedPath ) ) {
        m_pSelectedPath->m_bPathIsSelected = false;
    }
    
    m_pSelectedPath = NULL;
    
    if( m_pFoundOCPNPoint ) {
        m_pFoundOCPNPoint->m_bPtIsSelected = false;
    }
    m_pFoundOCPNPoint = NULL;
    
    //m_pFoundOCPNPointSecond = NULL;
    menuFocus = NULL;
    delete contextMenu;
    delete menuPath;
    delete menuOCPNPoint;
    
}

