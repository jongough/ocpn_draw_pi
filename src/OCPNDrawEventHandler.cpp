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
//#include "OCPNDrawCanvasMenuHandler.h"
#include "OCPNSelect.h"
#include "PathMan.h"
#include "PathProp.h"
#include "ODPointPropertiesImpl.h"
#include "ODUtils.h"
#include "chcanv.h"

extern ocpn_draw_pi    *g_ocpn_draw_pi;
extern PathManagerDialog *pPathManagerDialog;
extern OCPNSelect      *pOCPNSelect;
extern OCPNDrawConfig  *pOCPNDrawConfig;
extern PlugIn_ViewPort *g_pivp;
extern ChartCanvas     *ocpncc1;
extern PathProp       *pPathPropDialog;
extern PathMan          *g_pPathMan;
extern ODPointPropertiesImpl    *g_pODPointPropDialog;

extern bool             g_bConfirmObjectDelete;
// Event Handler implementation 
/*
BEGIN_EVENT_TABLE ( OCPNDrawEventHandler, wxEvtHandler ) 
    EVT_TIMER ( ROLLOVER_TIMER, OCPNDrawEventHandler::OnTimerEvent ) 
    EVT_TIMER ( HEAD_DOG_TIMER, OCPNDrawEventHandler::OnTimerEvent ) 
    EVT_MENU ( ID_PATH_MENU_PROPERTIES, OCPNDrawEventHandler::PopupMenuHandler )
END_EVENT_TABLE()
*/

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

void OCPNDrawEventHandler::PopupMenuHandler(wxCommandEvent& event ) 
{
    //m_parent->PopupMenuHandler( event ); 
    OCPNPoint *pLast;
    
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
        case ID_PATH_MENU_DELETE:
            int dlg_return;
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
            
        case ID_PATH_MENU_DEACTIVATE:
            break;
        case ID_PATH_MENU_ACTIVATE:
            break;
        case ID_OCPNPOINT_MENU_PROPERTIES:
            if( NULL == pPathManagerDialog )         // There is one global instance of the Dialog
                pPathManagerDialog = new PathManagerDialog( ocpncc1 );
            
            pPathManagerDialog->OCPNPointShowPropertiesDialog( m_pFoundOCPNPoint, ocpncc1 );
            break;
        case ID_PATH_MENU_ACTPOINT:
        case ID_PATH_MENU_ACTNXTPOINT:
        case ID_PATH_MENU_REMPOINT:
        case ID_OCPNPOINT_MENU_COPY:
        case ID_PATH_MENU_DELPOINT:
            break;
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
            delete menuOCPNPoint;
            menuOCPNPoint = new wxMenu( _("Layer OCPNPoint") );
            MenuAppend( menuOCPNPoint, ID_OCPNPOINT_MENU_PROPERTIES, _( "Properties..." ) );
            
            if( m_pSelectedPath && m_pSelectedPath->IsActive() )
                MenuAppend( menuOCPNPoint, ID_PATH_MENU_ACTPOINT, _( "Activate" ) );
        }
        else {
            MenuAppend( menuOCPNPoint, ID_OCPNPOINT_MENU_PROPERTIES, _( "Properties..." ) );
            if( m_pSelectedPath && m_pSelectedPath->IsActive() ) {
                if(m_pSelectedPath->m_pPathActivePoint != m_pFoundOCPNPoint )
                    MenuAppend( menuOCPNPoint, ID_PATH_MENU_ACTPOINT, _( "Activate" ) );
            }
            
            if( m_pSelectedPath && m_pSelectedPath->IsActive() ) {
                if(m_pSelectedPath->m_pPathActivePoint == m_pFoundOCPNPoint ) {
                    int indexActive = m_pSelectedPath->GetIndexOf( m_pSelectedPath->m_pPathActivePoint );
                    if( ( indexActive + 1 ) <= m_pSelectedPath->GetnPoints() )
                        MenuAppend( menuOCPNPoint, ID_PATH_MENU_ACTNXTPOINT, _( "Activate Next OCPNPoint" ) );
                }
            }
            if( m_pSelectedPath && m_pSelectedPath->GetnPoints() > 2 )
                MenuAppend( menuOCPNPoint, ID_PATH_MENU_REMPOINT, _( "Remove from Path" ) );
            
            MenuAppend( menuOCPNPoint, ID_OCPNPOINT_MENU_COPY, _( "Copy as KML" ) );
            
            if ( m_pFoundOCPNPoint->GetIconName() != wxS("mob") ) {
                if ( m_pSelectedPath )
                    MenuAppend( menuOCPNPoint, ID_PATH_MENU_DELPOINT,  _( "Delete" ) );
            }
            
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

