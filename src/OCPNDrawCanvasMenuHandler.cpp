/******************************************************************************
 * $Id: ocpn_draw_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OCPN Draw Canvas Menu handler
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *   $EMAIL$   *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#include "OCPNDrawCanvasMenuHandler.h"
#include "chcanv.h"
#include "ocpn_plugin.h"

extern ChartCanvas     *ocpncc1;
extern int              g_click_stop;
extern PlugIn_ViewPort  *g_pivp;


//OCPNDrawCanvasMenuHandler::OCPNDrawCanvasMenuHandler(ChartCanvas *parentCanvas,
//                  Path *selectedPath,
//                  OCPNPoint *selectedOCPNPoint) : CanvasMenuHandler( parentCanvas, (Route *) NULL, (Route *) NULL, (RoutePoint *) NULL, (int) 0, NULL )
OCPNDrawCanvasMenuHandler::OCPNDrawCanvasMenuHandler(ChartCanvas *parentCanvas,
                  Path *selectedPath,
                  OCPNPoint *selectedOCPNPoint)
{
    //ctor
    parent = parentCanvas;
    m_pSelectedPath = selectedPath;
    m_pFoundOCPNPoint = selectedOCPNPoint;
}

OCPNDrawCanvasMenuHandler::~OCPNDrawCanvasMenuHandler()
{
    //dtor
}

void OCPNDrawCanvasMenuHandler::PopupMenuHandler( wxCommandEvent& event )
{
    OCPNPoint *pLast;

    wxPoint r;
    double zlat, zlon;

    //ocpncc1->GetCanvasPixPoint( popx, popy, zlat, zlon );
    wxPoint wxp;
    wxp.x = popx;
    wxp.y = popy;
    GetCanvasPixLL( g_pivp, &wxp, zlat, zlon );

    switch( event.GetId() ) {
/*    case ID_WP_MENU_DELPOINT: {
        if( m_pFoundRoutePoint == pAnchorWatchPoint1 ) {
            pAnchorWatchPoint1 = NULL;
            g_AW1GUID.Clear();
        } else if( m_pFoundRoutePoint == pAnchorWatchPoint2 ) {
            pAnchorWatchPoint2 = NULL;
            g_AW2GUID.Clear();
        }

        if( m_pFoundRoutePoint && !( m_pFoundRoutePoint->m_bIsInLayer )
                && ( m_pFoundRoutePoint->GetIconName() != _T("mob") ) ) {

            // If the WP belongs to an invisible route, we come here instead of to ID_RT_MENU_DELPOINT
            //  Check it, and if so then remove the point from its routes
            wxArrayPtrVoid *proute_array = g_pRouteMan->GetRouteArrayContaining( m_pFoundRoutePoint );
            if( proute_array ) {
                pWayPointMan->DestroyWaypoint( m_pFoundRoutePoint );
             }
            else {
                parent->undo->BeforeUndoableAction( Undo_DeleteWaypoint, m_pFoundRoutePoint, Undo_IsOrphanded, NULL );
                pOCPNDrawConfig->DeleteWayPoint( m_pFoundRoutePoint );
                pOCPNSelect->DeleteSelectablePoint( m_pFoundRoutePoint, SELTYPE_ROUTEPOINT );
                if( NULL != pWayPointMan )
                    pWayPointMan->RemoveRoutePoint( m_pFoundRoutePoint );
                parent->undo->AfterUndoableAction( NULL );
            }

            if( pMarkPropDialog ) {
                pMarkPropDialog->SetRoutePoint( NULL );
                pMarkPropDialog->UpdateProperties();
            }

            if( pRouteManagerDialog && pRouteManagerDialog->IsShown() )
                pRouteManagerDialog->UpdateWptListCtrl();

            parent->InvalidateGL();
        }
        break;
    }
    case ID_WP_MENU_PROPERTIES:
        parent->ShowMarkPropertiesDialog( m_pFoundRoutePoint );
        break;

    case ID_RT_MENU_REVERSE: {
        if( m_pSelectedRoute->m_bIsInLayer ) break;

        int ask_return = OCPNMessageBox( parent, g_pRouteMan->GetRouteReverseMessage(),
                               _("Rename Waypoints?"), wxYES_NO | wxCANCEL );

        if( ask_return != wxID_CANCEL ) {
            pOCPNSelect->DeleteAllSelectableRouteSegments( m_pSelectedRoute );
            m_pSelectedRoute->Reverse( ask_return == wxID_YES );
            pOCPNSelect->AddAllSelectableRouteSegments( m_pSelectedRoute );

            pOCPNDrawConfig->UpdateRoute( m_pSelectedRoute );

            if( pRoutePropDialog && ( pRoutePropDialog->IsShown() ) ) {
                pRoutePropDialog->SetRouteAndUpdate( m_pSelectedRoute );
                pRoutePropDialog->UpdateProperties();
            }
        }
        break;
    }

    case ID_RT_MENU_DELETE: {
        int dlg_return = wxID_YES;
        if( g_bConfirmObjectDelete ) {
            dlg_return = OCPNMessageBox( parent,  _("Are you sure you want to delete this route?"),
                _("OpenCPN Route Delete"), (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
        }

        if( dlg_return == wxID_YES ) {
            if( g_pRouteMan->GetpActiveRoute() == m_pSelectedRoute ) g_pRouteMan->DeactivateRoute();

            if( m_pSelectedRoute->m_bIsInLayer )
                break;

            if( !g_pRouteMan->DeleteRoute( m_pSelectedRoute ) )
                break;
            if( pRoutePropDialog && ( pRoutePropDialog->IsShown()) && (m_pSelectedRoute == pRoutePropDialog->GetRoute()) ) {
                pRoutePropDialog->Hide();
            }

            if( pRouteManagerDialog && pRouteManagerDialog->IsShown() )
                pRouteManagerDialog->UpdateRouteListCtrl();

            if( pMarkPropDialog && pMarkPropDialog->IsShown() ) {
                pMarkPropDialog->ValidateMark();
                pMarkPropDialog->UpdateProperties();
            }

            parent->undo->InvalidateUndo();

            parent->InvalidateGL();
        }
        break;
    }

    case ID_RT_MENU_ACTIVATE: {
        if( g_pRouteMan->GetpActiveRoute() )
            g_pRouteMan->DeactivateRoute();

        //  If this is an auto-created MOB route, always select the second point (the MOB)
        // as the destination.
        RoutePoint *best_point;
        if(m_pSelectedRoute){
            if(wxNOT_FOUND == m_pSelectedRoute->m_RouteNameString.Find(_T("MOB")) ){
                best_point = g_pRouteMan->FindBestActivatePoint( m_pSelectedRoute, gLat,
                                 gLon, gCog, gSog );
            }
            else
                best_point = m_pSelectedRoute->GetPoint( 2 );
        
            g_pRouteMan->ActivateRoute( m_pSelectedRoute, best_point );
            m_pSelectedRoute->m_bRtIsSelected = false;
        }
            

        break;
    }

    case ID_RT_MENU_DEACTIVATE:
        g_pRouteMan->DeactivateRoute();
        m_pSelectedRoute->m_bRtIsSelected = false;

        break;

    case ID_RT_MENU_INSERT:

        if( m_pSelectedRoute->m_bIsInLayer ) break;

        m_pSelectedRoute->InsertPointAfter( m_pFoundRoutePoint, zlat, zlon );

        pOCPNSelect->DeleteAllSelectableRoutePoints( m_pSelectedRoute );
        pOCPNSelect->DeleteAllSelectableRouteSegments( m_pSelectedRoute );

        pOCPNSelect->AddAllSelectableRouteSegments( m_pSelectedRoute );
        pOCPNSelect->AddAllSelectableRoutePoints( m_pSelectedRoute );

        m_pSelectedRoute->RebuildGUIDList();          // ensure the GUID list is intact and good
        pOCPNDrawConfig->UpdateRoute( m_pSelectedRoute );

        if( pRoutePropDialog && ( pRoutePropDialog->IsShown() ) ) {
            pRoutePropDialog->SetRouteAndUpdate( m_pSelectedRoute, true );
        }

        break;

    case ID_RT_MENU_APPEND:

        if( m_pSelectedRoute->m_bIsInLayer ) break;

        parent->m_pMouseRoute = m_pSelectedRoute;
        parent->parent_frame->nRoute_State = m_pSelectedRoute->GetnPoints() + 1;
        parent->m_pMouseRoute->m_lastMousePointIndex = m_pSelectedRoute->GetnPoints();

        pLast = m_pSelectedRoute->GetLastPoint();

        parent->m_prev_rlat = pLast->m_lat;
        parent->m_prev_rlon = pLast->m_lon;
        parent->m_prev_pMousePoint = pLast;

        parent->m_bAppendingRoute = true;

        parent->SetCursor( *parent->pCursorPencil );

        break;

    case ID_RT_MENU_COPY:
        if( m_pSelectedRoute ) Kml::CopyRouteToClipboard( m_pSelectedRoute );
        break;

    case ID_WPT_MENU_COPY:
        if( m_pFoundRoutePoint ) Kml::CopyWaypointToClipboard( m_pFoundRoutePoint );
        break;

    case ID_PASTE_WAYPOINT:
        pupHandler_PasteWaypoint();
        break;

    case ID_PASTE_ROUTE:
        pupHandler_PasteRoute();
        break;

    case ID_PASTE_TRACK:
        pupHandler_PasteTrack();
        break;

    case ID_RT_MENU_DELPOINT:
        if( m_pSelectedRoute ) {
            if( m_pSelectedRoute->m_bIsInLayer ) break;

            pWayPointMan->DestroyWaypoint( m_pFoundRoutePoint );

            if( pRoutePropDialog && ( pRoutePropDialog->IsShown() ) ) {
                //    Selected route may have been deleted as one-point route, so check it
                if( g_pRouteMan->IsRouteValid( m_pSelectedRoute ) ){        
                    pRoutePropDialog->SetRouteAndUpdate( m_pSelectedRoute, true );
                }
                else
                    pRoutePropDialog->Hide();

            }

            if( pRouteManagerDialog && pRouteManagerDialog->IsShown() ) {
                pRouteManagerDialog->UpdateWptListCtrl();
                pRouteManagerDialog->UpdateRouteListCtrl();
            }

            parent->InvalidateGL();
        }

        break;

    case ID_RT_MENU_REMPOINT:
        if( m_pSelectedRoute ) {
            if( m_pSelectedRoute->m_bIsInLayer ) break;
            parent->RemovePointFromRoute( m_pFoundRoutePoint, m_pSelectedRoute );
            parent->InvalidateGL();
        }
        break;

    case ID_RT_MENU_ACTPOINT:
        if( g_pRouteMan->GetpActiveRoute() == m_pSelectedRoute ) {
            g_pRouteMan->ActivateRoutePoint( m_pSelectedRoute, m_pFoundRoutePoint );
            m_pSelectedRoute->m_bRtIsSelected = false;
        }

        break;

    case ID_RT_MENU_DEACTPOINT:
        break;

    case ID_RT_MENU_ACTNXTPOINT:
        if( g_pRouteMan->GetpActiveRoute() == m_pSelectedRoute ) {
            g_pRouteMan->ActivateNextPoint( m_pSelectedRoute, true );
            m_pSelectedRoute->m_bRtIsSelected = false;
        }

        break;

    case ID_RT_MENU_PROPERTIES: {
        parent->ShowRoutePropertiesDialog( _("Route Properties"), m_pSelectedRoute );
        break;
    }

    case ID_RC_MENU_FINISH:
        parent->FinishRoute();
        gFrame->SurfaceToolbar();
        parent->Refresh( false );
        break;

    default: {
        //  Look for PlugIn Context Menu selections
        //  If found, make the callback
        ArrayOfPlugInMenuItems item_array = g_OD_pi_manager->GetPluginContextMenuItemArray();

        for( unsigned int i = 0; i < item_array.GetCount(); i++ ) {
            PlugInMenuItemContainer *pimis = item_array.Item( i );
            {
                if( pimis->id == event.GetId() ) {
                    if( pimis->m_pplugin )
                        pimis->m_pplugin->OnContextMenuItemCallback( pimis->id );
                }
            }
        }

        break;
    }
*/    
    }           // switch

    //  Chart Groups....
//    if( ( event.GetId() >= ID_DEF_MENU_GROUPBASE )
//            && ( event.GetId() <= ID_DEF_MENU_GROUPBASE + (int) g_pGroupArray->GetCount() ) ) {
//        gFrame->SetGroupIndex( event.GetId() - ID_DEF_MENU_GROUPBASE );
//    }

    g_click_stop = 0;    // Context menu was processed, all is well

}
