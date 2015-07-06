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
#include "PathProp.h"
#include "ODPointPropertiesImpl.h"
#include "ODRolloverWin.h"
#include "ODUtils.h"
#include "chcanv.h"
#include "PointMan.h"
#include "TextPoint.h"
#include <wx/window.h>

extern ocpn_draw_pi    *g_ocpn_draw_pi;
extern PathManagerDialog *g_pPathManagerDialog;
extern ODSelect         *g_pODSelect;
extern ODConfig         *g_pODConfig;
extern PlugIn_ViewPort *g_pivp;
extern ChartCanvas     *ocpncc1;
extern PathProp         *g_pPathPropDialog;
extern PathMan          *g_pPathMan;

extern ODPointPropertiesImpl    *g_pODPointPropDialog;
extern Path             *g_PathToEdit;
extern PointMan         *g_pODPointMan;
extern bool             g_bShowMag;
extern bool             g_bConfirmObjectDelete;
extern ODRolloverWin    *g_pPathRolloverWin;
extern SelectItem       *g_pRolloverPathSeg;
extern int              g_cursor_x;
extern int              g_cursor_y;

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


ODEventHandler::ODEventHandler(ChartCanvas *parent,
                          Path *selectedPath,
                          ODPoint *selectedODPoint)
{
    m_parentcanvas = parent;
    m_pSelectedPath = selectedPath;
    m_pFoundODPoint = selectedODPoint;
}

ODEventHandler::ODEventHandler(ChartCanvas *parent,
                               Path *selectedPath,
                               TextPoint *selectedTextPoint)
{
    m_parentcanvas = parent;
    m_pSelectedPath = selectedPath;
    m_pFoundODPoint = selectedTextPoint;
}

void ODEventHandler::SetPath( Path *path )
{
    m_pSelectedPath = path;
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
        ( g_pPathPropDialog && g_pPathPropDialog->IsShown() ) )
        RequestRefresh( m_parentcanvas );
}

void ODEventHandler::OnRolloverPopupTimerEvent( wxTimerEvent& event )
{
    #ifdef __OCPN__ANDROID__
    return;
    #endif
    
    bool b_need_refresh = false;
    
    bool showRollover = false;
    
    if( NULL == g_pRolloverPathSeg ) {
        //    Get a list of all selectable sgements, and search for the first visible segment as the rollover target.
        
        SelectableItemList SelList = g_pODSelect->FindSelectionList( g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon,
                                                                     SELTYPE_PATHSEGMENT );
        
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
                    ODPoint *segShow_point_a = (ODPoint *) g_pRolloverPathSeg->m_pData1;
                    ODPoint *segShow_point_b = (ODPoint *) g_pRolloverPathSeg->m_pData2;
                    
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
        if( !g_pODSelect->IsSelectableSegmentSelected( g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon, g_pRolloverPathSeg ) ) 
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

void ODEventHandler::PopupMenuHandler(wxCommandEvent& event ) 
{
    int dlg_return;
    
    wxPoint r;
    
    //ocpncc1->GetCanvasPixPoint( popx, popy, zlat, zlon );
    wxPoint wxp;
    wxp.x = popx;
    wxp.y = popy;
    
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
            g_ocpn_draw_pi->m_pCurrentCursor = ocpncc1->pCursorCross;
            m_pSelectedPath->m_bIsBeingEdited = TRUE;
            g_PathToEdit = m_pSelectedPath;
            g_ocpn_draw_pi->m_bPathEditing = TRUE;
            break;
        case ID_PATH_MENU_INSERT:
            // Insert new OCPN Point
            m_pSelectedPath->InsertPointAfter( m_pFoundODPoint, m_cursor_lat, m_cursor_lon );
            
            g_pODSelect->DeleteAllSelectableODPoints( m_pSelectedPath );
            g_pODSelect->DeleteAllSelectablePathSegments( m_pSelectedPath );
            
            g_pODSelect->AddAllSelectablePathSegments( m_pSelectedPath );
            g_pODSelect->AddAllSelectableODPoints( m_pSelectedPath );
            
            m_pSelectedPath->RebuildGUIDList();          // ensure the GUID list is intact and good
            g_pODConfig->UpdatePath( m_pSelectedPath );
            
            if( g_pPathPropDialog && ( g_pPathPropDialog->IsShown() ) ) {
                g_pPathPropDialog->SetPathAndUpdate( m_pSelectedPath, true );
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
                DeletePath();
/*                if( g_pPathMan->GetpActivePath() == m_pSelectedPath ) g_pPathMan->DeactivatePath( m_pSelectedPath );
                
                if( !g_pPathMan->DeletePath( m_pSelectedPath ) )
                    break;
                if( g_pPathPropDialog && ( g_pPathPropDialog->IsShown()) && (m_pSelectedPath == g_pPathPropDialog->GetPath()) ) {
                    g_pPathPropDialog->Hide();
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
*/                
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
            m_pSelectedPath = NULL;
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
            m_pSelectedPath = NULL;
            break;
        }
        case ID_OCPNPOINT_MENU_PROPERTIES:
            if( NULL == g_pPathManagerDialog )         // There is one global instance of the Dialog
                g_pPathManagerDialog = new PathManagerDialog( ocpncc1 );
            
            g_pPathManagerDialog->ODPointShowPropertiesDialog( m_pFoundODPoint, ocpncc1 );
            m_pFoundODPoint = NULL;
            break;
        case ID_PATH_MENU_ACTPOINT:
        case ID_PATH_MENU_ACTNXTPOINT:
            break;
        case ID_PATH_MENU_REMPOINT: {
            dlg_return = wxID_YES;
            if( g_bConfirmObjectDelete ) {
                wxString sMessage( wxS("Are you sure you want to remove this ") );
                wxString sCaption( wxS("OCPN Draw Remove ") );
                wxString sType( wxS("") );
                if (!m_pFoundODPoint || m_pFoundODPoint->GetTypeString().IsNull() || m_pFoundODPoint->GetTypeString().IsEmpty() )
                    sType.append( wxS("OCPN Point") );
                else
                    sType.append( m_pFoundODPoint->GetTypeString() );
                sMessage.append( sType );
                sMessage.append( wxS("?") );
                sCaption.append( sType );
                
                dlg_return = OCPNMessageBox_PlugIn( m_parentcanvas, sMessage, sCaption, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
            }
            
            if( dlg_return == wxID_YES ) {
                m_pSelectedPath->RemovePointFromPath( m_pFoundODPoint, m_pSelectedPath );
                m_pFoundODPoint->SetTypeString( wxT("OCPN Point") );
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
        case ID_OCPNPOINT_MENU_MOVE:
            m_pFoundODPoint->m_bIsBeingEdited = TRUE;
            g_ocpn_draw_pi->m_bODPointEditing = TRUE;
            break;
        case ID_OCPNPOINT_MENU_COPY:
            break;
        case ID_PATH_MENU_DELPOINT: {
            dlg_return = wxID_YES;
            if( g_bConfirmObjectDelete ) {
                wxString sMessage( wxS("Are you sure you want to delete this ") );
                wxString sCaption( wxS("OCPN Draw Delete ") );
                wxString sType( wxS("") );
                if (!m_pFoundODPoint || m_pFoundODPoint->GetTypeString().IsNull() || m_pFoundODPoint->GetTypeString().IsEmpty() )
                    sType.append( wxS("OCPN Point") );
                else
                    sType.append( m_pFoundODPoint->GetTypeString() );
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
                    if( g_pPathPropDialog && g_pPathPropDialog->IsShown() ) g_pPathPropDialog->SetPathAndUpdate( m_pSelectedPath, true );
                }
            }
            g_ocpn_draw_pi->m_bPathEditing = FALSE;
            g_ocpn_draw_pi->m_bODPointEditing = FALSE;
            m_pFoundODPoint = NULL;
            m_pSelectedPath = NULL;
            break;
        }
        case ID_OCPNPOINT_MENU_DELPOINT: {
            dlg_return = wxID_YES;
            if( g_bConfirmObjectDelete ) {
                wxString sMessage( wxS("Are you sure you want to delete this ") );
                wxString sCaption( wxS("OCPN Draw Delete ") );
                wxString sType( wxS("") );
                if (!m_pFoundODPoint || m_pFoundODPoint->GetTypeString().IsNull() || m_pFoundODPoint->GetTypeString().IsEmpty() )
                    sType.append( wxS("OCPN Point") );
                else
                    sType.append( m_pFoundODPoint->GetTypeString() );
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
                    g_pODConfig->DeleteODPoint( m_pFoundODPoint );
                    g_pODSelect->DeleteSelectablePoint( m_pFoundODPoint, SELTYPE_OCPNPOINT );
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
            
    }
    
} 

void ODEventHandler::PopupMenu( int x, int y, int seltype )
{
    wxMenu* contextMenu = new wxMenu;
    wxMenu* menuODPoint = NULL;
    wxMenu* menuPath = new wxMenu( wxS("Path") );
    
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
        if( m_pFoundODPoint && m_pFoundODPoint->m_bIsInLayer )
            blay = true;
        
        if( blay ){
            wxString sType;
            sType.append( wxT("Layer ") );
            sType.append( m_pFoundODPoint->m_sTypeString );
            menuODPoint = new wxMenu( sType );
            MenuAppend( menuODPoint, ID_OCPNPOINT_MENU_PROPERTIES, _( "Properties..." ) );
            
            //if( m_pSelectedPath && m_pSelectedPath->IsActive() )
            //    MenuAppend( menuODPoint, ID_PATH_MENU_ACTPOINT, _( "Activate" ) );
        }
        else {
            wxString sType;
            sType.append( m_pFoundODPoint->m_sTypeString );
            menuODPoint = new wxMenu( sType );
            MenuAppend( menuODPoint, ID_OCPNPOINT_MENU_PROPERTIES, _( "Properties..." ) );
            sType.clear();
            sType.append( wxS("Move ") );
            sType.append(m_pFoundODPoint->m_sTypeString);
            MenuAppend( menuODPoint, ID_OCPNPOINT_MENU_MOVE, sType );

//            if( m_pSelectedPath && m_pSelectedPath->IsActive() ) {
//                if(m_pSelectedPath->m_pPathActivePoint != m_pFoundODPoint )
//                    MenuAppend( menuODPoint, ID_PATH_MENU_ACTPOINT, _( "Activate" ) );
//            }
            
//            if( m_pSelectedPath && m_pSelectedPath->IsActive() ) {
//                if(m_pSelectedPath->m_pPathActivePoint == m_pFoundODPoint ) {
//                    int indexActive = m_pSelectedPath->GetIndexOf( m_pSelectedPath->m_pPathActivePoint );
//                    if( ( indexActive + 1 ) <= m_pSelectedPath->GetnPoints() )
//                        MenuAppend( menuODPoint, ID_PATH_MENU_ACTNXTPOINT, _( "Activate Next ODPoint" ) );
//                }
//            }
            if( m_pSelectedPath && m_pSelectedPath->GetnPoints() > 2 )
                MenuAppend( menuODPoint, ID_PATH_MENU_REMPOINT, _( "Remove Point from Path" ) );
            
            if( m_pSelectedPath )
                MenuAppend( menuODPoint, ID_PATH_MENU_DELPOINT,  _( "Delete" ) );
            else
                MenuAppend( menuODPoint, ID_OCPNPOINT_MENU_DELPOINT,  _( "Delete" ) );
            
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
    m_parentcanvas->PopupMenu( menuFocus, x, y );
    m_parentcanvas->Disconnect( wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ODEventHandler::PopupMenuHandler ), NULL, this );
    
    // Cleanup
    if( ( m_pSelectedPath ) ) m_pSelectedPath->m_bPathIsSelected = false;
    m_pSelectedPath = NULL;
    
    if( m_pFoundODPoint ) m_pFoundODPoint->m_bPtIsSelected = false;
    m_pFoundODPoint = NULL;
    
    //m_pFoundODPointSecond = NULL;
    menuFocus = NULL;
    delete contextMenu;
    delete menuPath;
    if( menuODPoint ) delete menuODPoint;
    
}

void ODEventHandler::DeletePath( void )
{
    if( g_pPathMan->GetpActivePath() == m_pSelectedPath ) g_pPathMan->DeactivatePath( m_pSelectedPath );
    
    if( !g_pPathMan->DeletePath( m_pSelectedPath ) )
        return;
    if( g_pPathPropDialog && ( g_pPathPropDialog->IsShown()) && (m_pSelectedPath == g_pPathPropDialog->GetPath()) ) {
        g_pPathPropDialog->Hide();
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