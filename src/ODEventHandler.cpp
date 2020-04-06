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
#include "PathAndPointManagerDialogImpl.h"
#include "ODPathPropertiesDialogImpl.h"
#include "ODPointPropertiesImpl.h"
#include "ODRolloverWin.h"
#include "ODUtils.h"
#include "PILPropertiesDialogImpl.h"
#include "PointMan.h"
#include "Boundary.h"
#include "EBL.h"
#include "DR.h"
#include "GZ.h"
#include "PIL.h"
#include "ODDRDialogImpl.h"
#include "TextPoint.h"
#include <wx/window.h>
#include <wx/clipbrd.h>

ODEventHandler::ODEventHandler(ocpn_draw_pi *parent)
{
    //ctor
    m_pSelectedPath = NULL;

    m_parent = parent;
    m_parent_window = GetOCPNCanvasWindow();
    g_current_canvas = g_parent_window;
    g_current_canvas_index = 0;
    wxLogMessage("Finished constructor");
}


void ODEventHandler::SetPath( ODPath *path )
{
    m_pBoundary = NULL;
    m_pEBL = NULL;
    m_pDR = NULL;
    m_pGZ = NULL;
    m_pPIL = NULL;
    m_pSelectedPath = NULL;

    if(path == 0)
        return;

    if(path->m_sTypeString == wxT("Boundary")) {
        m_pBoundary = dynamic_cast<Boundary *>(path);
        m_pSelectedPath = m_pBoundary;
    } else if(path->m_sTypeString == wxT("EBL")) {
        m_pEBL = dynamic_cast<EBL *>(path);
        m_pSelectedPath = m_pEBL;
    } else if(path->m_sTypeString == wxT("DR")) {
        m_pDR = dynamic_cast<DR *>(path);
        m_pSelectedPath = m_pDR;
    } else if(path->m_sTypeString == wxT("Guard Zone")) {
        m_pGZ = dynamic_cast<GZ *>(path);
        m_pSelectedPath = m_pGZ;
    } else if(path->m_sTypeString == wxT("PIL")) {
        m_pPIL = dynamic_cast<PIL *>(path);
        m_pSelectedPath = m_pPIL;
    } else
        m_pSelectedPath = path;

    assert(m_pSelectedPath != 0);
}

void ODEventHandler::SetBoundaryList(std::list< Boundary* > pBoundaryList)
{
    m_pBoundaryList = pBoundaryList;
}

void ODEventHandler::SetPoint( ODPoint* point )
{
    m_pFoundODPoint = point;
}

void ODEventHandler::SetPoint( TextPoint* point )
{
    m_pFoundODPoint = point;
}

void ODEventHandler::SetPIL( int iPILL )
{
    m_iFoundPIL = iPILL;
}

void ODEventHandler::SetWindow( wxWindow *window )
{
    m_parent_window = window;
}

void ODEventHandler::SetCanvas( wxWindow *window )
{
    g_current_canvas = window;
}

void ODEventHandler::SetCanvasIndex( int canvasindex )
{
    g_current_canvas_index = canvasindex;
}

void ODEventHandler::SetLatLon( double lat, double lon )
{
    m_cursor_lat = lat;
    m_cursor_lon = lon;
}


ODEventHandler::~ODEventHandler()
{
    //dtor
    wxLogMessage("Finished destructor");
}

void ODEventHandler::OnODTimer( wxTimerEvent& event )
{
    g_ocpn_draw_pi->nBlinkerTick++;
    if(( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) ||
        ( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() ) ||
        ( g_pODPathPropDialog && g_pODPathPropDialog->IsShown() ) ||
        ( m_pSelectedPath && m_pSelectedPath->m_sTypeString == _T("Boundary") ) ) {
        ODERequestRefresh( g_current_timer_canvas_index, TRUE );
        }
}

void ODEventHandler::OnRolloverPopupTimerEvent( wxTimerEvent& event )
{
    #ifdef __OCPN__ANDROID__
    return;
    #endif

#ifndef __WXMSW__
    wxString *l_locale = NULL;
#endif
    
    bool b_need_refresh = false;
    
    bool showRollover = false;
    if( !g_pRolloverPathSeg && !g_pRolloverPoint) {
        //    Get a list of all selectable sgements, and search for the first visible segment as the rollover target.
        SelectableItemList SelList = g_pODSelect->FindSelectionList( g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon, SELTYPE_PATHSEGMENT | SELTYPE_PIL );
        
        wxSelectableItemListNode *node = SelList.GetFirst();
        if(node) {
#ifndef __WXMSW__
			l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)        
            wxSetlocale(LC_NUMERIC, "");
#else
            setlocale(LC_NUMERIC, "");
#endif
#endif
            
        }
        while( node ) {
            SelectItem *pFindSel = node->GetData();
            ODPath *pp = (ODPath *) pFindSel->m_pData3;        //candidate
            
            if( pp && pp->IsVisible() ) {
                g_pRolloverPathSeg = new SelectItem;
                *g_pRolloverPathSeg = *pFindSel;
                
                if(g_pODRolloverWin) {
                    g_pODRolloverWin->Destroy();
                    g_pODRolloverWin = NULL;
                }
                
                if( NULL == g_pODRolloverWin ) {
                    g_pODRolloverWin = new ODRolloverWin( GetCanvasByIndex(g_current_canvas_index) );
                    g_pODRolloverWin->IsActive( false );
                } 
                
                if( !g_pODRolloverWin->IsActive() ) {
                    wxString s;
                    ODPoint *segShow_point_a = (ODPoint *) g_pRolloverPathSeg->m_pData1;
                    ODPoint *segShow_point_b = (ODPoint *) g_pRolloverPathSeg->m_pData2;
                    
                    double brgFrom, brgTo, dist;
                    if(segShow_point_a && segShow_point_b) {
                        DistanceBearingMercator_Plugin( segShow_point_b->m_lat, segShow_point_b->m_lon,
                                                segShow_point_a->m_lat, segShow_point_a->m_lon, &brgFrom, &dist );
                        DistanceBearingMercator_Plugin( segShow_point_a->m_lat, segShow_point_a->m_lon,
                                                segShow_point_b->m_lat, segShow_point_b->m_lon, &brgTo, &dist );
                    }
                    
                    if( !pp->m_bIsInLayer ) {
                        wxString wxsText;
#if wxCHECK_VERSION(3,0,0)
                        wxsText.append( _(pp->m_sTypeString) );
#else
                        wxsText.append( pp->m_sTypeString );
#endif
                        wxsText.append( wxT(": ") );
                        s.append( wxsText );
                    }
                    else {
                        wxString wxsText;
                        wxsText.append( _("Layer ") );
#if wxCHECK_VERSION(3,0,0)
                        wxsText.append( _(pp->m_sTypeString) );
#else
                        wxsText.append( pp->m_sTypeString );
#endif
                        wxsText.append( wxT(": ") );
                        s.Append( wxsText );
                    }
                    
                    if( pp->m_PathNameString.IsEmpty() ) s.Append( _("(unnamed)") );
                    else
                        s.Append( pp->m_PathNameString );
                    s << _T("\n");
                    
                    if(pp->m_sTypeString != wxT("Guard Zone")) {
                        if(pp->m_sTypeString != _T("PIL")) {
                            s << _("Total Length: ") << g_ocpn_draw_pi->FormatDistanceAdaptive( pp->m_path_length)
                            << _T("\n") << _("Leg: from ") << segShow_point_a->GetName()
                            << _(" to ") << segShow_point_b->GetName()
                            << _T("\n");
                        } else {
                            PIL *l_pPIL = (PIL *)pp;
                            std::list<PILLINE>::iterator it = l_pPIL->m_PilLineList.begin();
                            while(it != l_pPIL->m_PilLineList.end()) {
                                if(it->iID == pFindSel->GetUserData()) break;
                                ++it;
                            }
                            if(it != l_pPIL->m_PilLineList.end()) {
                                s << _("Name: ") << it->sName << _T("\n");
                                s << _("ID: ") << it->iID << _T("\n");
                                s << _("Offset: ") << g_ocpn_draw_pi->FormatDistanceAdaptive( it->dOffset ) << _T("\n");
                                brgFrom = l_pPIL->m_dEBLAngle;
                            }
                            s << _("Bearing: ");
                        }
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
                        
                        if(pp->m_sTypeString != _T("PIL")) {
                            s << g_ocpn_draw_pi->FormatDistanceAdaptive( dist );

                            // Compute and display cumulative distance from path start point to current
                            // leg end point.

                            if( segShow_point_a && segShow_point_a != pp->m_pODPointList->GetFirst()->GetData() ) {
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
                        }
                    }
                    g_pODRolloverWin->SetString( s );
                    
                    wxSize win_size = g_parent_window->GetSize();
                    wxPoint point;
                    g_pODRolloverWin->SetParent(GetCanvasByIndex(g_current_canvas_index));
                    g_pODRolloverWin->SetBestPosition( g_cursor_x, g_cursor_y, 16, 16, PATH_ROLLOVER, win_size );
                    g_pODRolloverWin->SetBitmap( PATH_ROLLOVER );
                    g_pODRolloverWin->IsActive( true );
                    b_need_refresh = true;
                    showRollover = true;
                    break;
                }
                else {
                   // XXX endless loop
                   assert(false);
                }
            } else
                node = node->GetNext();
        }
        if( !showRollover && !g_pRolloverPathSeg) {
            SelList = g_pODSelect->FindSelectionList( g_ocpn_draw_pi->m_cursor_lat, g_ocpn_draw_pi->m_cursor_lon, SELTYPE_ODPOINT );
            node = SelList.GetFirst();

            if(node) {
#ifndef __WXMSW__
                l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)        
                wxSetlocale(LC_NUMERIC, "");
#else
                setlocale(LC_NUMERIC, "");
#endif
#endif
            }
            
            while( node ) {
                SelectItem *pFindSel = node->GetData();
                ODPoint *pp = (ODPoint *) pFindSel->m_pData1;        //candidate
                if( pp && pp->IsVisible() ) {
                    g_pRolloverPoint = new SelectItem();
                    *g_pRolloverPoint = *pFindSel;
                    showRollover = true;
                    
                    if(g_pODRolloverWin) {
                        g_pODRolloverWin->Destroy();
                        g_pODRolloverWin = NULL;
                    }
                    
                    if( NULL == g_pODRolloverWin ) {
                        g_pODRolloverWin = new ODRolloverWin( GetCanvasByIndex(g_current_canvas_index) );
                        g_pODRolloverWin->IsActive( false );
                    } 
                    
                    TextPoint *tp = NULL;
                    if( pp->m_sTypeString == wxT("Text Point") ) tp = (TextPoint *) pFindSel->m_pData1;

                    if( tp && tp->m_iDisplayTextWhen == ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ON_ROLLOVER ) {
                        tp->m_bShowDisplayTextOnRollover = true;
                        showRollover = true;
                        b_need_refresh = true;
                        break;
                    } else {
                        if( !g_pODRolloverWin->IsActive() ) {
                            wxString s;
                            if( !pp->m_bIsInLayer ) {
                                wxString wxsText;
#if wxCHECK_VERSION(3,0,0)
                                wxsText.append( _(pp->m_sTypeString) );
#else
                                wxsText.append( pp->m_sTypeString );
#endif
                                wxsText.append( wxT(": ") );
                                s.append( wxsText );
                            }
                            else {
                                wxString wxsText;
                                wxsText.append( _("Layer") );
                                wxsText.append( wxT(" ") );
#if wxCHECK_VERSION(3,0,0)
                                wxsText.append( _(pp->m_sTypeString) );
#else
                                wxsText.append( pp->m_sTypeString );
#endif
                                wxsText.append( wxT(": ") );
                                s.Append( wxsText );
                            }
                            if( pp->m_ODPointName.length() > 0 ) {
                                wxString wxsText;
                                wxsText.append( pp->m_ODPointName );
                                s.Append( wxsText );
                            }
                            g_pODRolloverWin->SetString( s );
                            
                            wxSize win_size = g_parent_window->GetSize();
                            //if( console && console->IsShown() ) win_size.x -= console->().x;
                            int l_OffsetX;
                            int l_OffsetY;
                            if(pp->m_bShowName) {
                                l_OffsetX = 16;
                                l_OffsetY = 28;
                            } else {
                                l_OffsetX = 16;
                                l_OffsetY = 16;
                            }
                            g_pODRolloverWin->SetParent(GetCanvasByIndex(g_current_canvas_index));
                            g_pODRolloverWin->SetBestPosition( g_cursor_x, g_cursor_y, l_OffsetX, l_OffsetY, POINT_ROLLOVER, win_size );
                            g_pODRolloverWin->SetBitmap( POINT_ROLLOVER );
                            g_pODRolloverWin->IsActive( true );
                            b_need_refresh = true;
                            showRollover = true;
                            break;
                        }
                        else {
                            // XXX FIXME may leak g_pRolloverPoint = new SelectItem();
                            // on following loops
                            assert(false);
                        }
                        showRollover = true;
                        b_need_refresh = true;
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
    if(g_ocpn_draw_pi->m_bODPointEditing || g_ocpn_draw_pi->m_bPathEditing)
        showRollover = false;
    
    
    if( g_pODRolloverWin && g_pODRolloverWin->IsActive() && !showRollover ) {
        g_pODRolloverWin->IsActive( false );
        if(g_pRolloverPathSeg) {
            delete g_pRolloverPathSeg;
            g_pRolloverPathSeg = NULL;
        }

        if(g_pRolloverPoint) {
            ODPoint *l_ODPoint = (ODPoint *)g_pRolloverPoint->m_pData1;
            if(l_ODPoint && l_ODPoint->m_GUID != wxEmptyString && l_ODPoint->m_sTypeString == wxT("Text Point")) {
                TextPoint *tp = (TextPoint *) g_pRolloverPoint->m_pData1;
                if( tp ) {
                    tp->m_bShowDisplayTextOnRollover = false;
                    tp = NULL;
                }
            }
            delete g_pRolloverPoint;
            g_pRolloverPoint = NULL;
        }
        
        g_pODRolloverWin->Destroy();
        g_pODRolloverWin = NULL;
        b_need_refresh = true;
    } else if( g_pODRolloverWin && showRollover ) {
        g_pODRolloverWin->IsActive( true );
        g_pODRolloverWin->Show();
        g_pODRolloverWin->Raise();
        b_need_refresh = true;
    }
    
    if( b_need_refresh ) {
        ODERequestRefresh( g_current_canvas_index );
    }

#ifndef __WXMSW__
    if(l_locale) {
#if wxCHECK_VERSION(3,0,0)        
        wxSetlocale(LC_NUMERIC, l_locale->ToAscii());
#else
        setlocale(LC_NUMERIC, l_locale->ToAscii());
#endif
        delete l_locale;
    }
#endif
    
}

void ODEventHandler::PopupMenuHandler(wxCommandEvent& event ) 
{
    int dlg_return;
    bool bFullRefresh = FALSE;
    
    wxPoint r;
    
    g_ocpn_draw_pi->m_iEditMode = event.GetId();
    
    switch( event.GetId() )
    {            
        case ID_PATH_MENU_PROPERTIES:
            if( NULL == g_pPathAndPointManagerDialog )         // There is one global instance of the Dialog
                g_pPathAndPointManagerDialog = new PathAndPointManagerDialogImpl( g_ocpn_draw_pi->m_parent_window );
#ifndef __WXOSX__
            DimeWindow( g_pPathAndPointManagerDialog );
#endif
            g_pPathAndPointManagerDialog->ShowPathPropertiesDialog( m_pSelectedPath );
            m_pSelectedPath = NULL;
            break;
        case ID_PATH_MENU_MOVE_POINT:
            m_pSelectedPath->m_bIsBeingEdited = TRUE;
            g_PathToEdit = m_pSelectedPath;
            g_ocpn_draw_pi->m_bPathEditing = TRUE;
            break;
        case ID_PATH_MENU_MOVE_PATH:
            m_pSelectedPath->m_bIsBeingEdited = TRUE;
            g_PathToEdit = m_pSelectedPath;
            g_ocpn_draw_pi->m_bPathEditing = TRUE;
            break;
        case ID_PATH_MENU_MOVE_PATH_SEGMENT: // Need to make sure we have both points of segment
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
                wxString sTypeLong;
                wxString sTypeShort;
                if(m_pSelectedPath->m_sTypeString == wxT("Boundary")) {
                    sTypeLong = _("Are you sure you want to delete this Boundary?");
                    sTypeShort = _("OCPN Draw Boundary Delete");
                }
                else if(m_pSelectedPath->m_sTypeString == wxT("EBL")) {
                    sTypeLong = _("Are you sure you want to delete this EBL?");
                    sTypeShort = _("OCPN Draw EBL Delete");
                }
                else if(m_pSelectedPath->m_sTypeString == wxT("DR")) {
                    sTypeLong = _("Are you sure you want to delete this DR?");
                    sTypeShort = _("OCPN Draw DR Delete");
                }
                else if(m_pSelectedPath->m_sTypeString == wxT("Guard Zone")) {
                    sTypeLong = _("Are you sure you want to delete this Guard Zone?");
                    sTypeShort = _("OCPN Draw GZ Delete");
                }
                else if(m_pSelectedPath->m_sTypeString == wxT("PIL")) {
                    sTypeLong = _("Are you sure you want to delete this Parallel Index Line Group?");
                    sTypeShort = _("OCPN Draw PIL Delete");
                }
#ifdef __WXOSX__
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window,  sTypeLong, sTypeShort, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT| wxICON_QUESTION );
#else
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window,  sTypeLong, sTypeShort, (long) wxYES_NO | wxYES_DEFAULT );
#endif
            }
            
            if( dlg_return == wxID_YES ) {
                DeletePath();
            }
            break;
        }
        case ID_PIL_MENU_DELETE_INDEX_LINE: {
            dlg_return = wxID_YES;
            if( g_bConfirmObjectDelete ) {
                wxString sTypeLong;
                wxString sTypeShort;
                sTypeLong = _("Are you sure you want to delete this Parallel Index Line?");
                sTypeShort = _("OCPN Draw Parallel Index Line Delete");
#ifdef __WXOSX__
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window,  sTypeLong, sTypeShort, (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT| wxICON_QUESTION );
#else
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window,  sTypeLong, sTypeShort, (long) wxYES_NO | wxYES_DEFAULT );
#endif
            }

            if( dlg_return == wxID_YES ) {
                DeletePIL();
            }
            break;
        }
        case ID_PIL_MENU_INDEX_LINE_PROPERTIES: {
            if( NULL == g_PILIndexLinePropDialog)
                g_PILIndexLinePropDialog = new PILPropertiesDialogImpl( g_ocpn_draw_pi->m_parent_window );
            DimeWindow( g_PILIndexLinePropDialog );
            g_PILIndexLinePropDialog->UpdateProperties( (PIL *)m_pSelectedPath, m_iFoundPIL );
            g_PILIndexLinePropDialog->Show();
            break;
        }
        case ID_PIL_MENU_ADD_INDEX_LINE: {
            m_pPIL->AddLine( _T(""), _T(""), g_dPILOffset );
            break;
        }
        case ID_PIL_MENU_MOVE_INDEX_LINE: {
            g_PathToEdit = m_pSelectedPath;
            g_PILToEdit = m_iFoundPIL;
            g_ocpn_draw_pi->m_bPathEditing = TRUE;
            break;
        }
        case ID_PATH_MENU_DEACTIVATE: {
            g_pPathMan->DeactivatePath( m_pSelectedPath );
            m_pSelectedPath = NULL;
            break;
        }
        case ID_PATH_MENU_ACTIVATE: {
            g_pPathMan->ActivatePath( m_pSelectedPath );
            m_pSelectedPath = NULL;
            break;
        }
        case ID_PATH_MENU_SHOW_ICONS: {
            if(m_pSelectedPath->m_bODPointsVisible)
                m_pSelectedPath->m_bODPointsVisible = false;
            else
                m_pSelectedPath->m_bODPointsVisible = true;
            m_pSelectedPath->SetPointVisibility();
            break;
        }
        case ID_BOUNDARY_LIST_KEEP_MENU:
        case ID_BOUNDARY_LIST_DELETE_MENU: {
            Boundary *l_pBoundary = new Boundary();
            g_pBoundaryList->Append( l_pBoundary );
            g_pPathList->Append( l_pBoundary);
            l_pBoundary->m_width = g_BoundaryLineWidth;
            l_pBoundary->m_style = g_BoundaryLineStyle;

            std::list<Boundary *>::iterator it = m_pBoundaryList.begin();
            LLBBox l_LLBBox = (*it)->GetBBox();
            ++it;
            while( it != m_pBoundaryList.end() ) {
                l_LLBBox.Expand((*it)->GetBBox());
                ++it;
            }

            BoundaryPoint *l_BP1 = new BoundaryPoint(l_LLBBox.GetMaxLat(), l_LLBBox.GetMaxLon(), g_sODPointIconName, wxS(""), wxT(""));
            l_BP1->SetNameShown( false );
            l_BP1->SetTypeString( wxS("Boundary Point") );
            g_pODConfig->AddNewODPoint( l_BP1, -1 );
            g_pODSelect->AddSelectableODPoint( l_LLBBox.GetMaxLat(), l_LLBBox.GetMaxLon(), l_BP1 );
            l_pBoundary->AddPoint( l_BP1 );

            BoundaryPoint *l_BP2 = new BoundaryPoint(l_LLBBox.GetMaxLat(), l_LLBBox.GetMinLon(), g_sODPointIconName, wxS(""), wxT(""));
            l_BP2->SetNameShown( false );
            l_BP2->SetTypeString( wxS("Boundary Point") );
            g_pODConfig->AddNewODPoint( l_BP2, -1 );
            g_pODSelect->AddSelectableODPoint( l_LLBBox.GetMaxLat(), l_LLBBox.GetMinLon(), l_BP2 );
            g_pODSelect->AddSelectablePathSegment( l_LLBBox.GetMaxLat(), l_LLBBox.GetMaxLon(), l_LLBBox.GetMaxLat(), l_LLBBox.GetMinLon(), l_BP1, l_BP2, l_pBoundary );
            l_pBoundary->AddPoint( l_BP2 );

            BoundaryPoint *l_BP3 = new BoundaryPoint(l_LLBBox.GetMinLat(), l_LLBBox.GetMinLon(), g_sODPointIconName, wxS(""), wxT(""));
            l_BP3->SetNameShown( false );
            l_BP3->SetTypeString( wxS("Boundary Point") );
            g_pODConfig->AddNewODPoint( l_BP3, -1 );
            g_pODSelect->AddSelectableODPoint( l_LLBBox.GetMinLat(), l_LLBBox.GetMinLon(), l_BP3 );
            g_pODSelect->AddSelectablePathSegment( l_LLBBox.GetMaxLat(), l_LLBBox.GetMinLon(), l_LLBBox.GetMinLat(), l_LLBBox.GetMinLon(), l_BP2, l_BP3, l_pBoundary );
            l_pBoundary->AddPoint( l_BP3 );

            BoundaryPoint *l_BP4 = new BoundaryPoint(l_LLBBox.GetMinLat(), l_LLBBox.GetMaxLon(), g_sODPointIconName, wxS(""), wxT(""));
            l_BP4->SetNameShown( false );
            l_BP4->SetTypeString( wxS("Boundary Point") );
            g_pODConfig->AddNewODPoint( l_BP4, -1 );
            g_pODSelect->AddSelectableODPoint( l_LLBBox.GetMinLat(), l_LLBBox.GetMaxLon(), l_BP4 );
            g_pODSelect->AddSelectablePathSegment( l_LLBBox.GetMinLat(), l_LLBBox.GetMinLon(), l_LLBBox.GetMinLat(), l_LLBBox.GetMaxLon(), l_BP3, l_BP4, l_pBoundary );
            l_pBoundary->AddPoint( l_BP4 );

            // Add final point to close the boundary
            g_pODSelect->AddSelectablePathSegment( l_LLBBox.GetMinLat(), l_LLBBox.GetMaxLon(), l_LLBBox.GetMaxLat(), l_LLBBox.GetMaxLon(), l_BP4, l_BP1, l_pBoundary );
            l_pBoundary->AddPoint( l_BP1 );

            l_pBoundary->RebuildGUIDList(); // ensure the GUID list is intact and good
            l_pBoundary->SetHiLite(0);
            l_pBoundary->m_bIsBeingCreated = false;

            if( g_pODPathPropDialog && ( g_pODPathPropDialog->IsShown() ) ) {
                g_pODPathPropDialog->SetPathAndUpdate( l_pBoundary, true );
            }

            if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
                g_pPathAndPointManagerDialog->UpdatePathListCtrl();

            it = m_pBoundaryList.begin();
            while( it != m_pBoundaryList.end() ) {
                (*it)->m_bPathPropertiesBlink = false;
                ++it;
            }

            if( event.GetId() == ID_BOUNDARY_LIST_DELETE_MENU) DeletePaths();

            m_pBoundaryList.clear();
            break;
        }
        case ID_EBL_MENU_CENTRE_ON_BOAT:
            m_pEBL->m_bSaveUpdates = true;
            m_pEBL->CentreOnBoat(false);
            bFullRefresh = TRUE;
            break;
        case ID_EBL_MENU_CENTRE_ON_BOAT_LATLON:
            m_pEBL->CentreOnLatLon( g_pfFix.Lat, g_pfFix.Lon );
            bFullRefresh = TRUE;
            break;
        case ID_PIL_MENU_CENTRE_ON_BOAT:
            m_pPIL->m_bSaveUpdates = true;
            m_pPIL->CentreOnBoat(false);
            bFullRefresh = TRUE;
            break;
        case ID_PIL_MENU_CENTRE_ON_BOAT_LATLON:
            m_pPIL->CentreOnLatLon( g_pfFix.Lat, g_pfFix.Lon );
            bFullRefresh = TRUE;
            break;
        case ID_EBL_MENU_PICK_NEW_START:
            g_ocpn_draw_pi->m_bEBLMoveOrigin = true;
            g_ocpn_draw_pi->m_pCurrentCursor = g_ocpn_draw_pi->m_pCursorCross;
            break;
        case ID_EBL_MENU_VRM_MATCH_EBL_COLOUR: {
            ODPoint *pFirstPoint = m_pEBL->m_pODPointList->GetFirst()->GetData();
            pFirstPoint->SetODPointRangeRingsColour( m_pEBL->GetCurrentColour() );
            break;
        }
        case ID_ODPOINT_MENU_PROPERTIES:
            if( NULL == g_pPathAndPointManagerDialog )         // There is one global instance of the Dialog
                g_pPathAndPointManagerDialog = new PathAndPointManagerDialogImpl( g_ocpn_draw_pi->m_parent_window );
            
            g_pPathAndPointManagerDialog->ODPointShowPropertiesDialog( m_pFoundODPoint, g_ocpn_draw_pi->m_parent_window );
            m_pFoundODPoint = NULL;
            break;
        case ID_PATH_MENU_ACTPOINT:
        case ID_PATH_MENU_ACTNXTPOINT:
            break;
        case ID_PATH_MENU_REMPOINT: {
            dlg_return = wxID_YES;
            if( m_pSelectedPath->m_pODPointList->GetCount() < 4 && m_pSelectedPath->m_sTypeString == wxT("Boundary")) {
                wxString sMessage(_("Removing this point will invalidate the Boundary, this is not allowed."));
                wxString sCaption( _("OCPN Draw Remove ") );
                wxString sType( wxS("") );
                if (!m_pFoundODPoint || m_pFoundODPoint->GetTypeString().IsNull() || m_pFoundODPoint->GetTypeString().IsEmpty() )
                    sType.append( _("OD Point") );
                else
#if wxCHECK_VERSION(3,0,0)
                    sType.append( _(m_pFoundODPoint->GetTypeString()) );
#else
                    sType.append( m_pFoundODPoint->GetTypeString() );
#endif                
                sCaption.append( sType );
                
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window, sMessage, sCaption, (long) wxOK );
                break;
            }
            if( g_bConfirmObjectDelete && dlg_return == wxID_YES) {
                wxString sMessage( _("Are you sure you want to remove this ") );
                wxString sCaption( _("OCPN Draw Remove ") );
                wxString sType( wxS("") );
                if (!m_pFoundODPoint || m_pFoundODPoint->GetTypeString().IsNull() || m_pFoundODPoint->GetTypeString().IsEmpty() ) {
                    sMessage = _("Are you sure you want to remove this OD Point?");
                    sCaption = _("OCPN Draw Remove OD Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("Boundary Point")) {
                    sMessage = _("Are you sure you want to remove this Boundary Point?");
                    sCaption = _("OCPN Draw Remove Boundary Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("EBL Point")) {
                    sMessage = _("Are you sure you want to remove this EBL Point?");
                    sCaption = _("OCPN Draw Remove EBL Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("DR Point")) {
                    sMessage = _("Are you sure you want to remove this DR Point?");
                    sCaption = _("OCPN Draw Remove DR Point");
                } else {
                    sMessage = _("Are you sure you want to remove this OD Point?");
                    sCaption = _("OCPN Draw Remove OD Point");
                }
#ifdef __WXOSX__
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window, sMessage, sCaption, (long) wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
#else
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window, sMessage, sCaption, (long) wxYES_NO | wxYES_DEFAULT );
#endif
            }
            
            if( dlg_return == wxID_YES ) {
                m_pSelectedPath->RemovePointFromPath( m_pFoundODPoint, m_pSelectedPath );
                m_pFoundODPoint->SetTypeString( wxT("OD Point") );
                g_pODSelect->DeleteAllSelectableODPoints( m_pSelectedPath );
                g_pODSelect->DeleteAllSelectablePathSegments( m_pSelectedPath );
                
                g_pODSelect->AddAllSelectablePathSegments( m_pSelectedPath );
                g_pODSelect->AddAllSelectableODPoints( m_pSelectedPath );
            }
            

            g_ocpn_draw_pi->m_bPathEditing = FALSE;
            g_ocpn_draw_pi->m_bODPointEditing = FALSE;
            m_pFoundODPoint = NULL;
            m_pSelectedPath = NULL;
            break;
        }
        case ID_ODPOINT_MENU_MOVE:
            m_pFoundODPoint->m_bIsBeingEdited = TRUE;
            g_ocpn_draw_pi->m_bODPointEditing = TRUE;
            if(!m_pFoundODPoint->m_bIsInPath) {
                m_pSelectedPath = NULL;
                g_ocpn_draw_pi->m_pSelectedPath = NULL;
            }
            if(m_pFoundODPoint->m_sTypeString == wxT("EBL Point")) {
                if(m_pFoundODPoint->m_ODPointName == _("Start"))
                    g_ocpn_draw_pi->m_bEBLMoveOrigin = true;
                else {
                    m_pEBL->m_bEndPointMoving = true;
                }
            }
            break;
        case ID_ODPOINT_MENU_COPY:
            break;
        case ID_PATH_MENU_DELPOINT: {
            dlg_return = wxID_YES;
            if( g_bConfirmObjectDelete ) {
                wxString sMessage;
                wxString sCaption;
                if (!m_pFoundODPoint || m_pFoundODPoint->GetTypeString().IsNull() || m_pFoundODPoint->GetTypeString().IsEmpty() ) {
                    sMessage = _("Are you sure you want to delete this OD Point?");
                    sCaption = _("OCPN Draw Delete OD Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("Boundary Point")) {
                    sMessage = _("Are you sure you want to delete this Boundary Point?");
                    sCaption = _("OCPN Draw Delete Boundary Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("DR Point")) {
                    sMessage = _("Are you sure you want to delete this DR Point?");
                    sCaption = _("OCPN Draw Delete DR Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("EBL Point")) {
                    sMessage = _("Are you sure you want to delete this EBL Point?");
                    sCaption = _("OCPN Draw Delete EBL Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("PIL Point")) {
                    sMessage = _("Are you sure you want to delete this PIL Point?");
                    sCaption = _("OCPN Draw Delete PIL Point");
                }
#ifdef __WXOSX__
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window, sMessage, sCaption, (long) wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
#else
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window, sMessage, sCaption, (long) wxYES_NO | wxYES_DEFAULT );
#endif
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
                wxString sMessage;
                wxString sCaption;
                if (!m_pFoundODPoint || m_pFoundODPoint->GetTypeString().IsNull() || m_pFoundODPoint->GetTypeString().IsEmpty() ) {
                    sMessage = _("Are you sure you want to delete this OD Point?");
                    sCaption = _("OCPN Draw Delete OD Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("Boundary Point")) {
                    sMessage = _("Are you sure you want to delete this Boundary Point?");
                    sCaption = _("OCPN Draw Delete Boundary Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("Text Point")) {
                    sMessage = _("Are you sure you want to delete this Text Point?");
                    sCaption = _("OCPN Draw Delete Text Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("DR Point")) {
                    sMessage = _("Are you sure you want to delete this DR Point?");
                    sCaption = _("OCPN Draw Delete DR Point");
                } else if(m_pFoundODPoint->m_sTypeString == wxT("EBL Point")) {
                    sMessage = _("Are you sure you want to delete this EBL Point?");
                    sCaption = _("OCPN Draw Delete EBL Point");
                }
#ifdef __WXOSX__
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window, sMessage, sCaption, (long) wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
#else
                dlg_return = OCPNMessageBox_PlugIn( m_parent_window, sMessage, sCaption, (long) wxYES_NO | wxYES_DEFAULT );
#endif
            }
            
            if( dlg_return == wxID_YES ) {
                // If the ODPoint belongs to an invisible path, we come here instead of to ID_PATH_MENU_DELPOINT
                //  Check it, and if so then remove the point from its paths
                wxArrayPtrVoid *ppath_array = g_pPathMan->GetPathArrayContaining( m_pFoundODPoint );
                if( ppath_array ) {
                    g_pODPointMan->DestroyODPoint( m_pFoundODPoint );
                    delete ppath_array;
                }
                else {
                    g_pODSelect->DeleteSelectablePoint( m_pFoundODPoint, SELTYPE_ODPOINT );
                    g_pODConfig->DeleteODPoint( m_pFoundODPoint );
                    if( NULL != g_pODPointMan )
                        g_pODPointMan->RemoveODPoint( m_pFoundODPoint );
                }
                
                if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
                    g_pPathAndPointManagerDialog->UpdateODPointsListCtrl();

                if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
                    g_pODPointPropDialog->ValidateMark();
                }
                
                m_pFoundODPoint->m_bPtIsSelected = false;

                if(m_pFoundODPoint->m_sTypeString == wxT("Text Point"))
                    delete (TextPoint *)m_pFoundODPoint;
                else
                    delete m_pFoundODPoint;
            } else {
                m_pFoundODPoint->m_bPtIsSelected = false;
            }
            
            g_ocpn_draw_pi->m_bPathEditing = FALSE;
            g_ocpn_draw_pi->m_bODPointEditing = FALSE;
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
        case ID_DR_MENU_UPDATE_INITIAL_CONDITIONS: {
            if( NULL == g_pODDRDialog )         // There is one global instance of the Dialog
                g_pODDRDialog = new ODDRDialogImpl( m_parent_window );
            
            g_pODDRDialog->UpdateDialog( m_pDR );
#ifndef __WXOSX__
            DimeWindow( g_pODDRDialog );
#endif
            g_pODDRDialog->Show();
            
            
            //    Required if RMDialog is not STAY_ON_TOP
#ifdef __WXOSX__
            g_pODDRDialog->Centre();
            g_pODDRDialog->Raise();
#endif
            
            ODERequestRefresh( g_current_canvas_index, bFullRefresh );
            
            break;
        }
        case ID_PATH_MGR_PATH_RIGHT_CLICK_HIDE: {
            g_pPathAndPointManagerDialog->SelectedPathToggleVisibility(false);
            break;
        }
        case ID_PATH_MGR_PATH_RIGHT_CLICK_SHOW: {
            g_pPathAndPointManagerDialog->SelectedPathToggleVisibility(true);
            break;
        }
        case ID_PATH_MGR_ODPOINT_RIGHT_CLICK_HIDE: {
            g_pPathAndPointManagerDialog->SelectedODPointToggleVisibility(false);
            break;
        }
        case ID_PATH_MGR_ODPOINT_RIGHT_CLICK_SHOW: {
            g_pPathAndPointManagerDialog->SelectedODPointToggleVisibility(true);
            break;
        }
        case ID_PATH_MGR_LAYER_RIGHT_CLICK_HIDE: {
            g_pPathAndPointManagerDialog->SelectedLayerToggleVisibility(false);
            break;
        }
        case ID_PATH_MGR_LAYER_RIGHT_CLICK_SHOW: {
            g_pPathAndPointManagerDialog->SelectedLayerToggleVisibility(true);
            break;
        }
    }
    
} 

void ODEventHandler::PopupMenuCloseHandler( wxCommandEvent & event )
{
    if(m_pSelectedPath) {
        m_pSelectedPath->m_bPathPropertiesBlink = false;
    }
}

void ODEventHandler::PopupMenu( int popuptype )
{
    wxMenu* contextMenu = new wxMenu;
    wxMenu* menuODPoint = NULL;
    wxMenu* menuPath = NULL;
    wxMenu* menuBoundaryList = NULL;
    wxMenu* menuPILList = NULL;
    wxMenu* menuPathMgr = NULL;
    
    wxMenu *menuFocus = contextMenu;    // This is the one that will be shown
 
    wxString sString;
    
    g_current_timer_canvas = GetCanvasUnderMouse();
    
    if( popuptype & SELTYPE_PATHSEGMENT ) {
        bool blay = false;
        if( m_pSelectedPath && m_pSelectedPath->m_bIsInLayer )
            blay = true;
        
        if( blay ) {
            wxString  tName;
            tName.Append( _("Layer ") );
#if wxCHECK_VERSION(3,0,0)
            tName.Append( _(m_pSelectedPath->m_sTypeString) );
#else
            tName.Append( m_pSelectedPath->m_sTypeString );
#endif
            if(m_pSelectedPath->m_sTypeString == wxT("PIL")) {
                tName.Append( _T(" ") );
                tName.Append( _("Control Line") );
            }
            menuPath = new wxMenu( tName );
            MenuAppend( menuPath, ID_PATH_MENU_PROPERTIES, _( "Properties..." ) );
        }
        else {
            wxString tName;
#if wxCHECK_VERSION(3,0,0)
            tName.Append( _(m_pSelectedPath->m_sTypeString) );
#else
            tName.Append( m_pSelectedPath->m_sTypeString );
#endif
            if(m_pSelectedPath->m_sTypeString == wxT("PIL")) {
                tName.Append( _T(" ") );
                tName.Append( _("Control Line") );
            }
            menuPath = new wxMenu( tName );

            MenuAppend( menuPath, ID_PATH_MENU_PROPERTIES, _( "Properties..." ) );
            if(m_pSelectedPath->m_sTypeString == wxT("EBL")) {
                if(!m_pEBL->m_bCentreOnBoat) {
                    MenuAppend( menuPath, ID_EBL_MENU_CENTRE_ON_BOAT, _("Centre on moving boat") );
                    MenuAppend( menuPath, ID_EBL_MENU_CENTRE_ON_BOAT_LATLON, _("Move start point to boat lat/lon") );
                }
                MenuAppend( menuPath, ID_EBL_MENU_PICK_NEW_START, _("Pick a new start point") );
                ODPoint *pFirstPoint = m_pEBL->m_pODPointList->GetFirst()->GetData();
                if(m_pEBL->GetCurrentColour() != pFirstPoint->GetODPointRangeRingsColour())
                    MenuAppend( menuPath, ID_EBL_MENU_VRM_MATCH_EBL_COLOUR, _("Match VRM colour to EBL colour"));
            } else if(m_pSelectedPath->m_sTypeString == wxT("PIL")) {
                if(!m_pPIL->m_bCentreOnBoat) {
                    MenuAppend( menuPath, ID_PIL_MENU_CENTRE_ON_BOAT, _("Centre on moving boat") );
                    MenuAppend( menuPath, ID_PIL_MENU_CENTRE_ON_BOAT_LATLON, _("Move start point to boat lat/lon") );
                }
                MenuAppend( menuPath, ID_EBL_MENU_PICK_NEW_START, _("Pick a new start point") );
            }
            else if(m_pSelectedPath->m_sTypeString == wxT("DR")) {
                MenuAppend( menuPath, ID_DR_MENU_UPDATE_INITIAL_CONDITIONS, _("Update initial conditions") );
            }
            else if(m_pSelectedPath->m_sTypeString != wxT("DR") && m_pSelectedPath->m_sTypeString != wxT("Guard Zone")) {
                sString.clear();
                if(m_pSelectedPath->m_sTypeString == wxT("Boundary"))
                    sString.append(_("Move Boundary"));
                else if(m_pSelectedPath->m_sTypeString == wxT("EBL"))
                    sString.append(_("Move EBL"));

                if(!sString.IsEmpty()) MenuAppend( menuPath, ID_PATH_MENU_MOVE_PATH, sString );
                
                sString.clear();
                if(m_pSelectedPath->m_sTypeString == wxT("Boundary")) {
                    sString.append( _("Move Boundary Segment") );
                    MenuAppend( menuPath, ID_PATH_MENU_MOVE_PATH_SEGMENT, sString );
                }

                sString.clear();
                if(m_pSelectedPath->m_sTypeString == wxT("Boundary"))
                    sString.append(_("Insert Boundary Point"));
                else if(m_pSelectedPath->m_sTypeString == wxT("EBL"))
                    sString.append(_("Insert EBL Point"));

                if(sString.Len() > 0)
                    MenuAppend( menuPath, ID_PATH_MENU_INSERT, sString );
            }
            if(m_pSelectedPath->m_sTypeString == wxT("PIL")) {
                sString.clear();
                sString.append( _("Add Index Line"));
                MenuAppend( menuPath, ID_PIL_MENU_ADD_INDEX_LINE, sString );
            }
            if(m_pSelectedPath->m_sTypeString == wxT("Boundary")) {
                if(m_pSelectedPath->m_bODPointsVisible)
                    MenuAppend( menuPath, ID_PATH_MENU_SHOW_ICONS, _("Hide Boundary Point Icons"));
                else
                    MenuAppend( menuPath, ID_PATH_MENU_SHOW_ICONS, _("Show Boundary Point Icons"));
            }
            sString.clear();
            if(g_bConfirmObjectDelete)
                sString.append( _("Delete...") );
            else
                sString.append( _("Delete") );
            MenuAppend( menuPath, ID_PATH_MENU_DELETE, sString );
            if(m_pSelectedPath->m_sTypeString != wxT("EBL")) {
                if ( m_pSelectedPath->m_bPathIsActive ) MenuAppend( menuPath, ID_PATH_MENU_DEACTIVATE, _( "Deactivate") );
                else  MenuAppend( menuPath, ID_PATH_MENU_ACTIVATE, _( "Activate" ) );
            }
            sString.clear();
            if(m_pSelectedPath->m_sTypeString == wxT("Boundary"))
                sString.append(_("Copy Boundary GUID"));
            else if(m_pSelectedPath->m_sTypeString == wxT("EBL"))
                sString.append(_("Copy EBL GUID"));
            else if(m_pSelectedPath->m_sTypeString == wxT("DR"))
                sString.append(_("Copy DR GUID"));
            else if(m_pSelectedPath->m_sTypeString == wxT("Guard Zone"))
                sString.append(_("Copy Guard Zone GUID"));
            if(sString.Len() > 0)
                MenuAppend( menuPath, ID_PATH_MENU_COPY_GUID, sString );
        }
        
        //      Set this menu as the "focused context menu"
        menuFocus = menuPath;
    }
    
    if( popuptype & SELTYPE_ODPOINT ) {
        bool blay = false;
        if( m_pFoundODPoint && m_pFoundODPoint->m_bIsInLayer )
            blay = true;
        
        if( blay ){
            sString.clear();
            if(m_pFoundODPoint->m_sTypeString == wxT("Boundary Point"))
                sString.append(_("Layer Boundary Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("Text Point"))
                sString.append(_("Layer Text Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("EBL Point"))
                sString.append(_("Layer EBL Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("DR Point"))
                sString.append(_("Layer DR Point"));
            menuODPoint = new wxMenu( sString );
            MenuAppend( menuODPoint, ID_ODPOINT_MENU_PROPERTIES, _( "Properties..." ) );
            
            //if( m_pSelectedPath && m_pSelectedPath->IsActive() )
            //    MenuAppend( menuODPoint, ID_PATH_MENU_ACTPOINT, _( "Activate" ) );
        }
        else {
            sString.clear();
            if(m_pFoundODPoint->m_sTypeString == wxT("Boundary Point"))
                sString.append(_("Boundary Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("Text Point"))
                sString.append(_("Text Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("EBL Point"))
                sString.append(_("EBL Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("DR Point"))
                sString.append(_("DR Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("Guard Zone Point"))
                sString.append(_("Guard Zone Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("PIL Point"))
                sString.append(_("PIL Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("OD Point"))
                sString.append(_("OD Point"));
            menuODPoint = new wxMenu( sString );
            MenuAppend( menuODPoint, ID_ODPOINT_MENU_PROPERTIES, _( "Properties..." ) );
            sString.clear();
            if(m_pFoundODPoint->m_sTypeString == wxT("Boundary Point"))
                sString.append(_("Move Boundary Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("Text Point"))
                sString.append(_("Move Text Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("EBL Point"))
                sString.append(_("Move EBL Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("DR Point"))
                sString.append(_("Move DR Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("PIL Point"))
                sString.append(_("Move PIL Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("OD Point"))
                sString.append(_("Move OD Point"));
            else if(m_pFoundODPoint->m_sTypeString == wxT("Guard Zone Point"))
                sString.append(_("Move Guard Zone Point"));
            else sString.append(_("Move OD Point"));
            
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
    
    if( popuptype & SELTYPE_BOUNDARYLIST ) {
        menuBoundaryList = new wxMenu( _("Multiple Boundaries") );
        MenuAppend( menuBoundaryList, ID_BOUNDARY_LIST_KEEP_MENU, _( "Merge and Keep Boundaries" ) );
        MenuAppend( menuBoundaryList, ID_BOUNDARY_LIST_DELETE_MENU, _( "Merge and Delete Boundaries" ) );
        menuFocus = menuBoundaryList;
        m_pSelectedPath = NULL;
        m_pFoundODPoint = NULL;
    }

    if( popuptype & SELTYPE_PIL ) {
        menuPILList = new wxMenu( _("Parallel Index Line") );
        MenuAppend( menuPILList, ID_PIL_MENU_INDEX_LINE_PROPERTIES, _("Properties..."));
        MenuAppend( menuPILList, ID_PIL_MENU_MOVE_INDEX_LINE, _( "Move Line" ) );
        MenuAppend( menuPILList, ID_PIL_MENU_DELETE_INDEX_LINE, _( "Delete Line" ) );
        menuFocus = menuPILList;
    }

    if( popuptype & TYPE_PATHMGR_PATH_DLG ) {
        menuPathMgr = new wxMenu( _("Path Manager Dialog") );
        MenuAppend( menuPathMgr, ID_PATH_MGR_PATH_RIGHT_CLICK_HIDE, _( "Hide Path(s)" ));
        MenuAppend( menuPathMgr, ID_PATH_MGR_PATH_RIGHT_CLICK_SHOW, _( "Show Paths(s)" ));
        menuFocus = menuPathMgr;
    }

    if( popuptype & TYPE_PATHMGR_POINT_DLG ) {
        menuPathMgr = new wxMenu( _("Path Manager Dialog") );
        MenuAppend( menuPathMgr, ID_PATH_MGR_ODPOINT_RIGHT_CLICK_HIDE, _( "Hide Point(s)" ));
        MenuAppend( menuPathMgr, ID_PATH_MGR_ODPOINT_RIGHT_CLICK_SHOW, _( "Show Point(s)" ));
        menuFocus = menuPathMgr;
    }
    
    if( popuptype & TYPE_PATHMGR_LAYER_DLG ) {
        menuPathMgr = new wxMenu( _("Path Manager Dialog") );
        MenuAppend( menuPathMgr, ID_PATH_MGR_LAYER_RIGHT_CLICK_HIDE, _( "Hide Layer(s)" ));
        MenuAppend( menuPathMgr, ID_PATH_MGR_LAYER_RIGHT_CLICK_SHOW, _( "Show Layer(s)" ));
        menuFocus = menuPathMgr;
    }
    
    if( ( m_pSelectedPath ) ) {
        m_pSelectedPath->m_bPathIsSelected = true;
        ODERequestRefresh( g_current_canvas_index );
    } else if( m_pFoundODPoint ) {
        m_pFoundODPoint->m_bPtIsSelected = true;
        ODERequestRefresh( g_current_canvas_index );
    }
    
    //        Invoke the correct focused drop-down menu
    
//    m_parent_window->Connect( wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ODEventHandler::PopupMenuHandler ), NULL, this );
//    m_parent_window->Connect( wxEVT_MENU_CLOSE, wxCommandEventHandler( ODEventHandler::PopupMenuCloseHandler ), NULL, this );
//    m_parent_window->PopupMenu( menuFocus );
//    m_parent_window->Disconnect( wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ODEventHandler::PopupMenuHandler ), NULL, this );
//    m_parent_window->Disconnect( wxEVT_MENU_CLOSE, wxCommandEventHandler( ODEventHandler::PopupMenuCloseHandler ), NULL, this );

    g_current_canvas->Connect( wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ODEventHandler::PopupMenuHandler ), NULL, this );
    g_current_canvas->Connect( wxEVT_MENU_CLOSE, wxCommandEventHandler( ODEventHandler::PopupMenuCloseHandler ), NULL, this );
    g_current_canvas->PopupMenu( menuFocus );
    g_current_canvas->Disconnect( wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ODEventHandler::PopupMenuHandler ), NULL, this );
    g_current_canvas->Disconnect( wxEVT_MENU_CLOSE, wxCommandEventHandler( ODEventHandler::PopupMenuCloseHandler ), NULL, this );
    // Cleanup
    if( ( m_pSelectedPath ) ) m_pSelectedPath->m_bPathIsSelected = false;
    m_pSelectedPath = NULL;
    
    if( m_pFoundODPoint ) m_pFoundODPoint->m_bPtIsSelected = false;
    m_pFoundODPoint = NULL;

    if(!m_pBoundaryList.empty()) {
        std::list<Boundary *>::iterator it = m_pBoundaryList.begin();
        while( it != m_pBoundaryList.end() ) {
            (*it)->m_bPathPropertiesBlink = false;
            ++it;
        }
    }

    
    //m_pFoundODPointSecond = NULL;
    menuFocus = NULL;
    delete contextMenu;
    if(menuPath) delete menuPath;
    if( menuODPoint ) delete menuODPoint;
    if( menuBoundaryList ) delete menuBoundaryList;
    if( menuPILList ) delete menuPILList;
    if( menuPathMgr ) delete menuPathMgr;
    
}

void ODEventHandler::DeletePath( void )
{
    if( g_pPathMan->GetpActivePath() == m_pSelectedPath ) g_pPathMan->DeactivatePath( m_pSelectedPath );
    
    if( !g_pPathMan->DeletePath( m_pSelectedPath ) )
        return;
    if( g_pODPathPropDialog && ( g_pODPathPropDialog->IsShown()) && (m_pSelectedPath == g_pODPathPropDialog->GetPath()) ) {
        g_pODPathPropDialog->Hide();
    }
    
    if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
        g_pPathAndPointManagerDialog->UpdatePathListCtrl();
    
    if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
        g_pODPointPropDialog->ValidateMark();
    }
    
    // TODO implement UNDO
    //m_parent->undo->InvalidateUndo();
    ODERequestRefresh( g_current_canvas_index, TRUE );
    m_pSelectedPath = NULL;
    
}

void ODEventHandler::DeletePaths( void )
{
    std::list<Boundary *>::iterator it = m_pBoundaryList.begin();
    while( it != m_pBoundaryList.end() ) {
        if( !g_pPathMan->DeletePath( (*it) ) )
            return;
        if( g_pODPathPropDialog && ( g_pODPathPropDialog->IsShown()) && ((*it) == g_pODPathPropDialog->GetPath()) ) {
            g_pODPathPropDialog->Hide();
        }

        if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
            g_pPathAndPointManagerDialog->UpdatePathListCtrl();

        if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
            g_pODPointPropDialog->ValidateMark();
            g_pODPointPropDialog->UpdateProperties();
        }
        ++it;
    }

    // TODO implement UNDO
    //m_parent->undo->InvalidateUndo();

    // paths pointers are invalide
    m_pBoundaryList.clear();

    ODERequestRefresh( g_current_canvas_index, TRUE );

}

void ODEventHandler::DeletePIL( void )
{
    PIL *l_pPIL = (PIL *)m_pSelectedPath;

    g_pODSelect->DeleteSelectablePathSegment(l_pPIL, m_iFoundPIL);
    std::list<PILLINE>::iterator it = l_pPIL->m_PilLineList.begin();
    while(it != l_pPIL->m_PilLineList.end()) {
        if(it->iID == m_iFoundPIL) break;
        ++it;
    }

    l_pPIL->m_PilLineList.erase(it);

    if( g_pODPathPropDialog && ( g_pODPathPropDialog->IsShown()) && (m_pSelectedPath == g_pODPathPropDialog->GetPath()) ) {
        g_pODPathPropDialog->Hide();
    }

    if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
        g_pPathAndPointManagerDialog->UpdatePathListCtrl();

    if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
        g_pODPointPropDialog->ValidateMark();
    }

    // TODO implement UNDO
    //m_parent->undo->InvalidateUndo();
    ODERequestRefresh( g_current_canvas_index, TRUE );
    m_pSelectedPath = NULL;

}

void ODEventHandler::ODERequestRefresh( int canvas_index, bool bFullRefresh )
{
    if(!bFullRefresh) {
        if(canvas_index != -1) RequestRefresh(GetCanvasByIndex(canvas_index));
    }
    else {
        for(int i = 0; i < GetCanvasCount(); ++i) {
            RequestRefresh(GetCanvasByIndex(i));
        }
    }
}
