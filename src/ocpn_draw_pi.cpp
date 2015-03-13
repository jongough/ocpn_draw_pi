/******************************************************************************
* $Id: ocpn_draw_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
*
* Project:  OpenCPN
* Purpose:  OpenCPN General Drawing Plugin
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
//#define _2_9_x_ // uncomment this to compile for 2.9.x

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "wx/wxprec.h"

#include "ocpn_draw_pi.h"
#include "Boundary.h"
#include "BoundaryProp.h"
#include "Path.h"
#include "PathMan.h"
#include "PointMan.h"
#include "OCPNDrawConfig.h"
#include "multiplexer.h"
#include "OCPNSelect.h"
#include "pathmanagerdialog.h"
#include "OCPNDrawPointInfoImpl.h"
#include "chcanv.h"
#include "styles.h"
#include "geodesic.h"
#include "FontMgr.h"
#include "wx/stdpaths.h"
#include <wx/timer.h>
#include <wx/event.h>
#include <wx/sysopt.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/filefn.h>
#include <wx/msgdlg.h>
#include <memory>

#include "wx/jsonreader.h"

using namespace std;

#ifndef DECL_EXP
#ifdef __WXMSW__
  #define DECL_EXP     __declspec(dllexport)
#else
  #define DECL_EXP
#endif
#endif


#include "OCPNDrawicons.h"

PathList        *pPathList;
PointMan        *pOCPNPointMan;
bool            g_bIsNewLayer;
int             g_LayerIdx;
int             g_path_line_width;
wxString        g_default_wp_icon;
double          g_dLat;
double          g_dLon;
OCPNSelect      *pSelect;
OCPNDrawConfig  *pConfig;
Multiplexer     *g_pMUX;
float           g_GLMinSymbolLineWidth;
wxString        *g_SData_Locn;
void            *g_ppimgr;
PathMan          *g_pPathMan;
wxString         g_default_OCPNPoint_icon;
PathProp       *pPathPropDialog;
PathManagerDialog *pPathManagerDialog;
OCPNDrawPointInfoImpl *pOCPNPointPropDialog;
PlugInManager       *g_pi_manager;
ocpnStyle::StyleManager* g_StyleManager;
BoundaryList              *pBoundaryList;
OCPNPointList           *pOCPNPointList;
ChartCanvas     *cc1;
wxString    g_ActiveLineColour;
wxString    g_InActiveLineColour;
wxString    g_ActiveFillColour;
wxString    g_InActiveFillColour;
wxString         g_PrivateDataDir;


wxImage ICursorLeft;
wxImage ICursorRight;
wxImage ICursorUp;
wxImage ICursorDown;
wxImage ICursorPencil;
wxImage ICursorCross;


// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return (opencpn_plugin *)new ocpn_draw_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
	delete p;
}



//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------
ocpn_draw_pi::ocpn_draw_pi(void *ppimgr)
      :opencpn_plugin_113(ppimgr)
{
    // Create the PlugIn icons
    g_ppimgr = ppimgr;
    g_pi_manager = (PlugInManager *) ppimgr;
    //g_SData_Locn = new wxString();
    g_SData_Locn = GetpSharedDataLocation();
    initialize_images();
}


ocpn_draw_pi::~ocpn_draw_pi()
{
//    RemovePlugInTool(m_leftclick_config_id);
//    RemovePlugInTool(m_leftclick_boundary_id);
    SaveConfig();
}

int ocpn_draw_pi::Init(void)
{
	dlgShow = false;
	m_bBoundaryEditing = false;
    nBoundary_State = 0;
    nConfig_State = 0;
    m_pMouseBoundary = NULL;
    m_bDrawingBoundary = NULL;


    AddLocaleCatalog( wxS("opencpn-ocpn_draw_pi") );

	lastWaypointInRoute = wxS("-1");
	eventsEnabled = true;

	// Get a pointer to the opencpn display canvas, to use as a parent for windows created
	m_parent_window = GetOCPNCanvasWindow();

	m_pconfig = (OCPNDrawConfig *)GetOCPNConfigObject();
    OCPNDrawConfig *pConfig = new OCPNDrawConfig( wxString( wxS("") ), wxString( wxS("") ), m_pconfig->m_sNavObjSetFile );
    pConfig->m_pOCPNDrawNavObjectChangesSet = new OCPNDrawNavObjectChanges(pConfig->m_sNavObjSetChangesFile);
    
    pSelect = new OCPNSelect();
    
	LoadConfig();
	if(m_bLOGShowIcon) {
            m_leftclick_config_id  = InsertPlugInTool(wxS("OCPN Draw Manager"), _img_ocpn_draw_pi, _img_ocpn_draw_gray_pi, wxITEM_NORMAL,
                  wxS("OCPN Draw Manager"), wxS(""), NULL,
                   OCPN_DRAW_POSITION, 0, this);
            m_leftclick_boundary_id  = InsertPlugInTool(wxS("OCPN Draw Boundary"), _img_ocpn_draw_boundary, _img_ocpn_draw_boundary, wxITEM_NORMAL,
                  wxS("OCPN Draw Boundary"), wxS(""), NULL,
                   OCPN_DRAW_POSITION, 0, this);
	}

    undo = new Undo();
	// Create the Context Menu Items

	//    In order to avoid an ASSERT on msw debug builds,
	//    we need to create a dummy menu to act as a surrogate parent of the created MenuItems
	//    The Items will be re-parented when added to the real context meenu
	wxMenu dummy_menu;

    // Now initialize UI Style.
    g_StyleManager = new ocpnStyle::StyleManager();
    
//    MyFrame *pFrame = g_pi_manager->GetParentFrame();
//    cc1 = pFrame->GetCanvasWindow();    
    cc1 = (ChartCanvas *)m_parent_window;

    if( !g_StyleManager->IsOK() ) {
        wxString msg = wxS("Failed to initialize the user interface. ");
        msg << wxS("OpenCPN cannot start. ");
        msg << wxS("The necessary configuration files were not found. ");
        //msg << wxS("See the log file at ") << glog_file << wxS(" for details.");
        wxMessageDialog w( NULL, msg, wxS("Failed to initialize the user interface. "),
                wxCANCEL | wxICON_ERROR );
        w.ShowModal();
        exit( EXIT_FAILURE );
    }
	
    //build_cursors(); // build cursors to use on chart
    
//    stats = new StatWin( cc1 );
//    stats->SetColorScheme( global_color_scheme );
   
    
    pOCPNPointMan = new PointMan();
    pOCPNPointMan->SetColorScheme( global_color_scheme );
    
    g_pPathMan = new PathMan();
    g_pPathMan->SetColorScheme( global_color_scheme );

	SendPluginMessage(wxS("OCPN_DRAW_READY_FOR_REQUESTS"), wxS("TRUE"));

	return (
            WANTS_OVERLAY_CALLBACK  |
          WANTS_CURSOR_LATLON       |
          WANTS_TOOLBAR_CALLBACK    |
          INSTALLS_TOOLBAR_TOOL     |
          WANTS_CONFIG              |
          INSTALLS_TOOLBOX_PAGE     |
          INSTALLS_CONTEXTMENU_ITEMS  |
          WANTS_PREFERENCES         |
          USES_AUI_MANAGER			    |
          WANTS_ONPAINT_VIEWPORT    |
          WANTS_PLUGIN_MESSAGING    |
          WANTS_LATE_INIT           |
          WANTS_MOUSE_EVENTS        |
          WANTS_VECTOR_CHART_OBJECT_INFO  |
          INSTALLS_PLUGIN_CHART_GL  |
          WANTS_PLUGIN_MESSAGING    |
          WANTS_KEYBOARD_EVENTS
		);

}

void ocpn_draw_pi::LateInit(void)
{
  initialize_images();
	if(m_bLOGShowIcon) {
            m_leftclick_config_id  = InsertPlugInTool(wxS("OCPN Draw Manager"), _img_ocpn_draw_pi, _img_ocpn_draw_gray_pi, wxITEM_NORMAL,
                  wxS("OCPN Draw Manager"), wxS(""), NULL,
                   OCPN_DRAW_POSITION, 0, this);
            m_leftclick_boundary_id  = InsertPlugInTool(wxS("OCPN Draw Boundary"), _img_ocpn_draw_boundary, _img_ocpn_draw_boundary, wxITEM_NORMAL,
                  wxS("OCPN Draw Boundary"), wxS(""), NULL,
                   OCPN_DRAW_POSITION, 0, this);
	}
}

bool ocpn_draw_pi::DeInit(void)
{
	shutdown(false);
	return true;
}

void ocpn_draw_pi::shutdown(bool menu)
{
	SendPluginMessage(wxS("OCPNDRAW_READY_FOR_REQUESTS"), wxS("FALSE"));


}

void ocpn_draw_pi::GetOriginalColors()
{
	mcol = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
	mcol1 = wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVEBORDER  );
	muitext = wxColour(0,0,0);
	mback_color = wxColour(255,255,255);
	mtext_color = wxColour(0,0,0);
}

void ocpn_draw_pi::SetOriginalColors()
{
	col = mcol;
	col1 = mcol1;
	gridline = mgridline;
	uitext = muitext;
	udkrd = mudkrd;
	back_color = mback_color;
	text_color = mtext_color;
}

void ocpn_draw_pi::UpdateAuiStatus(void)
{
}

int ocpn_draw_pi::GetPlugInVersionMajor()
{
	return PLUGIN_VERSION_MAJOR;
}

int ocpn_draw_pi::GetPlugInVersionMinor()
{
	return PLUGIN_VERSION_MINOR;
}

int ocpn_draw_pi::GetAPIVersionMajor()
{
	return API_VERSION_MAJOR;
}

int ocpn_draw_pi::GetAPIVersionMinor()
{
	return API_VERSION_MINOR;
}

wxString ocpn_draw_pi::GetCommonName()
{
	return wxS("ocpn_draw");
}

wxString ocpn_draw_pi::GetShortDescription()
{
	return wxS("General drawing for OpenCPN");
}

wxString ocpn_draw_pi::GetLongDescription()
{
	return wxS("General Drawing for OpenCPN\n\nThanks to the community for their helpful suggestions.");
}


void ocpn_draw_pi::OnContextMenuItemCallback(int id)
{
}

void ocpn_draw_pi::SetDefaults(void)
{
    // If the config somehow says NOT to show the icon, override it so the user gets good feedback
}

wxBitmap *ocpn_draw_pi::GetPlugInBitmap()
{
      return _img_ocpn_draw_pi;
}

int ocpn_draw_pi::GetToolbarToolCount(void)
{
      return 1;
}

void ocpn_draw_pi::ShowPreferencesDialog( wxWindow* parent )
{
	dlgShow = false;
/*
#ifdef __WXMSW__
	optionsDialog = new LogbookOptions(parent, opt, this, -1, wxS("Logbook Preferences"), wxDefaultPosition,  wxSize( 692,660  ),
		wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
#elif defined __WXOSX__
    optionsDialog = new LogbookOptions(parent, opt, this, -1, wxS("Logbook Preferences"), wxDefaultPosition,  wxSize( 710,685 ) ,
		wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
#else
	optionsDialog = new LogbookOptions(parent, opt, this, -1, wxS("Logbook Preferences"), wxDefaultPosition,  wxSize( 740,700 ) ,
		wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
#endif
	optionsDialog->m_checkBoxShowLogbook->SetValue(m_bLOGShowIcon);

     if(optionsDialog->ShowModal() == wxID_OK)
      {
		  optionsDialog->getValues();
            //    Show Icon changed value?
		  if(m_bLOGShowIcon != optionsDialog->m_checkBoxShowLogbook->GetValue())
            {
                  m_bLOGShowIcon= optionsDialog->m_checkBoxShowLogbook->GetValue();

                  if(m_bLOGShowIcon)
                        m_leftclick_config_id  = InsertPlugInTool(wxS(""), _img_logbook_pi, _img_logbook_pi, wxITEM_NORMAL,
                              wxS("ocpn_draw"), wxS(""), NULL, OCPN_DRAW_POSITION,
                              0, this);
                  else
                        RemovePlugInTool(m_leftclick_config_id);
            }
            SaveConfig();
      }
*/
}

void ocpn_draw_pi::OnToolbarToolCallback(int id)
{
    dlgShow = !dlgShow;
    m_iCallerId = id;
      // show the Logbook dialog
    if ( id == m_leftclick_config_id ) {
        if( 0 == nConfig_State ){
            nConfig_State = 1;
            SetToolbarItemState( m_leftclick_config_id, true );
            if( NULL == pPathManagerDialog )         // There is one global instance of the Dialog
            pPathManagerDialog = new PathManagerDialog( cc1 );

            pPathManagerDialog->UpdatePathListCtrl();
            pPathManagerDialog->Show();

            //    Required if RMDialog is not STAY_ON_TOP
#ifdef __WXOSX__
            pPathManagerDialog->Centre();
            pPathManagerDialog->Raise();
#endif
            nConfig_State = 0;
            SetToolbarItemState( m_leftclick_config_id, false );
            
        } else {
            nConfig_State = 0;
            SetToolbarItemState( m_leftclick_config_id, false );
        }
        
        
    }
    else if ( id == m_leftclick_boundary_id ) {
        if( 0 == nBoundary_State ){
            nBoundary_State = 1;
            cc1->SetCursor( *cc1->pCursorPencil );
            SetToolbarItemState( m_leftclick_boundary_id, true );
        } else {
            nBoundary_State = 0;
            FinishBoundary();
            //cc1->SetCursor( *pCursorArrow ); 
            SetToolbarItemState( m_leftclick_boundary_id, false );
            cc1->Refresh(false);
        }
        
    }
/*
	if(NULL == m_plogbook_window)
	{
		if(m_timer == NULL)
		{
			if(timer == NULL)
					timer = new LogbookTimer(this);
			m_timer = new wxTimer(timer,ID_LOGTIMER);
			timer->Connect( wxEVT_TIMER, wxObjectEventFunction( &LogbookTimer::OnTimer ));
		}
        m_plogbook_window = new LogbookDialog(this, m_timer, timer, m_parent_window, wxID_ANY, wxS("Active Logbook"), wxDefaultPosition, wxSize( opt->dlgWidth,opt->dlgHeight ), wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER);
		m_plogbook_window->init();
		m_plogbook_window->CenterOnParent();
		m_plogbook_window->Show();
		dlgShow = true;
	}
	else
	{
		if(m_plogbook_window->IsIconized())
		{
			m_plogbook_window->Iconize(false);
			m_plogbook_window->Show(true);
			dlgShow = true;
		}
		else
		{
			m_plogbook_window->Show(dlgShow);
		}
	}

    if (m_plogbook_window->IsShown())
            SendPluginMessage(wxString(wxS("LOGBOOK_WINDOW_SHOWN")), wxEmptyString);
      else
            SendPluginMessage(wxS("LOGBOOK_WINDOW_HIDDEN"), wxEmptyString);

	if(state == OFF)
	{
		if(!opt->checkStateOfEvents())
			state = ONNOEVENTS;
		else
			state = ONWITHEVENTS;
	}
	else
	{
		state = OFF;
	}
*/
      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
     // SetToolbarItemState( m_leftclick_config_id, dlgShow );
}

void ocpn_draw_pi::SaveConfig()
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(pConf)
    {
        pConf->SetPath ( wxS( "/PlugIns/libocpn_draw_pi.so" ) );
        pConf->Write ( wxS( "DefaultActiveLineColour" ), g_ActiveLineColour );
        pConf->Write ( wxS( "DefaultInActiveLineColour" ), g_InActiveLineColour );
        pConf->Write ( wxS( "DefaultActiveFillColour" ), g_ActiveFillColour );
        pConf->Write ( wxS( "DefaultInActiveFillColour" ), g_InActiveFillColour );
        pConf->Write ( wxS( "ShowLOGIcon" ), m_bLOGShowIcon );
        pConf->Write( wxS( "PathLineWidth" ), g_path_line_width );
        pConf->Write( wxS( "DefaultWPIcon" ), g_default_wp_icon );
    }
}

void ocpn_draw_pi::LoadConfig()
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(pConf)
    {
        pConf->SetPath ( wxS( "/PlugIns/libocpn_draw_pi.so" ) );
        pConf->Read ( wxS( "DefaultActiveLineColour" ), &g_ActiveLineColour, wxS("Red") );
        pConf->Read ( wxS( "DefaultInActiveLineColour" ), &g_InActiveLineColour, wxS("LightGray") );
        pConf->Read ( wxS( "DefaultActiveFillColour" ), &g_ActiveFillColour, wxS("Red") );
        pConf->Read ( wxS( "DefaultInActiveFillColour" ), &g_InActiveFillColour, wxS("LightGray") );
        pConf->Read ( wxS( "ShowLOGIcon" ),  &m_bLOGShowIcon, 1 );
        pConf->Read( wxS( "PathLineWidth" ), &g_path_line_width, 2 );
        pConf->Read( wxS( "DefaultWPIcon" ), &g_default_wp_icon, wxS("triangle") );
    }
    
    pOCPNPointList = new OCPNPointList;
    pBoundaryList = new BoundaryList;
    pPathList = new PathList;
}

void ocpn_draw_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
/*
    if(message_id == wxS("OCPN_RTE_ENDED"))
    {
        if(!opt->waypointArrived) return;

        wxJSONReader reader;
        wxJSONValue  data;
        int numErrors = reader.Parse( message_body, &data );
        if(numErrors != 0) return;

        if(!m_plogbook_window)
            startLogbook();

        RMB rmb;
        rmb.From = lastWaypointInRoute;
        rmb.To = wxS("-1");
        m_plogbook_window->logbook->WP_skipped = false;
        m_plogbook_window->logbook->OCPN_Message = true;

        m_plogbook_window->logbook->checkWayPoint(rmb);
        m_plogbook_window->logbook->OCPN_Message = false;
        lastWaypointInRoute = wxS("-1");
        m_plogbook_window->logbook->lastWayPoint = wxEmptyString;
        m_plogbook_window->logbook->routeIsActive = false;
    }
*/
}

bool ocpn_draw_pi::KeyboardEventHook( wxKeyEvent &event )
{
    bool bret = FALSE;
    if( event.GetKeyCode() < 128 )            //ascii
    {
        int key_char = event.GetKeyCode();
    
        if ( event.ControlDown() )
            key_char -= 64;

        switch( key_char ) {
            case 2:                      // Ctrl B
                if ( event.ShiftDown() ) { // Shift-Ctrl-B
                    nBoundary_State = 1;
                    m_iCallerId = m_leftclick_boundary_id;
                    cc1->SetCursor( *cc1->pCursorPencil );
                    bret = TRUE;
                } else bret = FALSE;
                break;
                
            case 27: // Generic Break
                if( nBoundary_State > 0 ){
                    nBoundary_State = 0;
                    FinishBoundary();
                    cc1->SetCursor( *cc1->pCursorArrow ); 
                    SetToolbarItemState( m_leftclick_boundary_id, false );
                    cc1->Refresh(false);
                    bret = TRUE;
                } else bret = FALSE;
                break;
        }
    }
    return bret;
}

bool ocpn_draw_pi::MouseEventHook( wxMouseEvent &event )
{
    int x, y;
    bool bret = FALSE;

        //    Route Creation Rubber Banding

    if( nBoundary_State >= 2 ) {
        r_rband.x = event.GetX();
        r_rband.y = event.GetY();
        m_bDrawingBoundary = true;

// TODO (jon#1#): Need to get access to 'CheckEdgePan' 
//        cc1->CheckEdgePan( x, y, event.Dragging(), 5, 2 );
        cc1->Refresh( false );
    }

    if ( event.LeftDown() ) {
        if( m_iCallerId == m_leftclick_boundary_id && nBoundary_State > 0 )                  // creating boundary?
        {   
            bret = CreateBoundaryLeftClick( event );
        }
    } 
    
    if( event.LeftUp() ) {
        bool b_startedit_boundary = false;
/*        if( m_bBoundaryEditing ) {            // End of RoutePoint drag
            if( m_pOCPNPointEditTarget ) {
                pSelect->UpdateSelectablePathSegments( m_pOCPNPointEditTarget );
                m_pOCPNPointEditTarget->m_bBlink = false;
                
                if( m_pEditBoundaryArray ) {
                    for( unsigned int ib = 0; ib < m_pEditBoundaryArray->GetCount(); ib++ ) {
                        Boundary *pb = (Boundary *) m_pEditBoundaryArray->Item( ib );
                        if( g_pPathMan->IsBoundaryValid(pb) ) {
                            pb->FinalizeForRendering();
                            pb->UpdateSegmentDistances();
                            pb->m_bIsBeingEdited = false;

                            pConfig->UpdatePath( pb );
                            
                            pb->SetHiLite( 0 );
                        }
                    }
                    Refresh( false );
                }

                //    Update the BoundaryProperties Dialog, if currently shown
                if( ( NULL != pBoundaryPropDialog ) && ( pBoundaryPropDialog->IsShown() ) ) {
                    if( m_pEditBoundaryArray ) {
                        for( unsigned int ib = 0; ib < m_pEditBoundaryArray->GetCount(); ib++ ) {
                            Boundary *pb = (Boundary *) m_pEditBoundaryArray->Item( ib );
                            if( g_pPathMan->IsBoundaryValid(pb) ) {
                                pBoundaryPropDialog->SetBoundaryAndUpdate( pb, true );
                            }
                        }
                    }
                }

                m_pOCPNPointEditTarget->m_bPtIsSelected = false;
                m_pOCPNPointEditTarget->m_bIsBeingEdited = false;
                
                delete m_pEditBoundaryArray;
                m_pEditBoundaryArray = NULL;
                undo->AfterUndoableAction( m_pRoutePointEditTarget );
            }

            InvalidateGL();
            m_bBoundaryEditing = false;
            m_pRoutePointEditTarget = NULL;
            if( !g_FloatingToolbarDialog->IsShown() ) gFrame->SurfaceToolbar();
        }
*/
    }
    
    if ( event.RightDown() ) {
        if ( nBoundary_State > 0 ) {
            nBoundary_State = 0;
            FinishBoundary();
            cc1->SetCursor( *cc1->pCursorArrow ); 
            SetToolbarItemState( m_leftclick_boundary_id, false );
            cc1->Refresh(false);
            bret = TRUE;
        } else if ( nBoundary_State == 0 ) {
           // cc1->CanvasPopupMenu( x, y, SELTYPE_BOUNDARYCREATE ) ;
        }
    }
    return bret;
}

void ocpn_draw_pi::SetCursorLatLon(double lat, double lon)
{
    g_dLat = lat;
    g_dLon = lon;
    
    m_cursor_lat = lat;
    m_cursor_lon = lon;
}

wxString ocpn_draw_pi::FormatDistanceAdaptive( double distance ) 
{
    wxString result;
    wxString *sUnit = new wxString( getUsrDistanceUnit_Plugin( -1 ) );
    double usrDistance = toUsrDistance( distance, -1 );
//    if( usrDistance < 0.1 &&  ( unit == DISTANCE_KM || unit == DISTANCE_MI || unit == DISTANCE_NMI ) ) {
    if( usrDistance < 0.1 &&  ( sUnit->IsSameAs( wxS("km") ) || sUnit->IsSameAs( wxS("mi") ) || sUnit->IsSameAs( wxS("NMi") ) ) ) {
        if ( sUnit->IsSameAs(wxS("mi")) ) sUnit->assign(wxS("ft"));
        else sUnit->assign(wxS("M"));
        usrDistance = toUsrDistance( distance, -1 );
    }
    wxString format;
    if( usrDistance < 5.0 ) {
        format = wxS("%1.2f ");
    } else if( usrDistance < 100.0 ) {
        format = wxS("%2.1f ");
    } else if( usrDistance < 1000.0 ) {
        format = wxS("%3.0f ");
    } else {
        format = wxS("%4.0f ");
    }
    result << wxString::Format(format, usrDistance ) << *sUnit;
    return result;
}
void ocpn_draw_pi::latlong_to_chartpix(double lat, double lon, double &pixx, double &pixy) 
{
    m_lat = lat;
    m_lon = lon;
    m_pixx = pixx;
    m_pixy = pixy;
}

bool ocpn_draw_pi::RenderOverlay(wxMemoryDC *pmdc, PlugIn_ViewPort *vp)
{
    m_vp = vp;
    RenderPathLegs( (ocpnDC &) *pmdc );
	return TRUE;
}

bool ocpn_draw_pi::RenderOverlay(wxDC &mdc, PlugIn_ViewPort *vp)
{
    m_vp = vp;
    ocpnDC *poDC = new ocpnDC( mdc );
    RenderPathLegs( *poDC );
    if (m_pMouseBoundary) m_pMouseBoundary->Draw( *poDC, (ViewPort &)m_vp);
    DrawAllPathsInBBox( *poDC, cc1->GetVP().GetBBox() );
    return TRUE;
}

void ocpn_draw_pi::RenderPathLegs(  ocpnDC &dc ) 
{
    if( nBoundary_State >= 2) {

        Boundary* boundary = 0;
        int state;
        boundary = m_pMouseBoundary;
        state = nBoundary_State;
        
        if(!boundary)
            return;
    
        double rhumbBearing, rhumbDist, gcBearing, gcBearing2, gcDist;
        DistanceBearingMercator( m_cursor_lat, m_cursor_lon, m_prev_rlat, m_prev_rlon, &rhumbBearing, &rhumbDist );
        Geodesic::GreatCircleDistBear( m_prev_rlon, m_prev_rlat, m_cursor_lon, m_cursor_lat, &gcDist, &gcBearing, &gcBearing2);
        double gcDistm = gcDist / 1852.0;

        if( ( m_prev_rlat == m_cursor_lat ) && ( m_prev_rlon == m_cursor_lon ) ) rhumbBearing = 90.;

        wxPoint destPoint, lastPoint;

        double brg = rhumbBearing;
        double dist = rhumbDist;
        boundary->m_NextLegGreatCircle = false;
        int milesDiff = rhumbDist - gcDistm;
        if( milesDiff > 1 ) {
            brg = gcBearing;
            dist = gcDistm;
            boundary->m_NextLegGreatCircle = true;
        }

//        if( !g_btouch) {
            boundary->DrawPointWhich( dc, boundary->m_lastMousePointIndex, &lastPoint );
            if( boundary->m_NextLegGreatCircle ) {
                for( int i=1; i<=milesDiff; i++ ) {
                    double p = (double)i * (1.0/(double)milesDiff);
                    double pLat, pLon;
                    Geodesic::GreatCircleTravel( m_prev_rlon, m_prev_rlat, gcDist*p, brg, &pLon, &pLat, &gcBearing2 );
                    GetCanvasPixLL( m_vp, &destPoint, m_cursor_lat, m_cursor_lon);
                    //destPoint = VPoint.GetPixFromLL( pLat, pLon );
                    boundary->DrawSegment( dc, &lastPoint, &destPoint, (ViewPort &)m_vp, false );
                    wxPoint rpn;
                    boundary->GetPoint( 1 )->Draw( dc, &rpn );
                    boundary->DrawSegment( dc, &rpn , &destPoint, (ViewPort &)m_vp, false );
                    lastPoint = destPoint;
                }
            }
            else {
                boundary->DrawSegment( dc, &lastPoint, &r_rband, (ViewPort &)m_vp, false );
                if ( nBoundary_State >= 2) { 
                    wxPoint rpn;
                    boundary->GetPoint( 1 )->Draw( dc, &rpn );
                    boundary->DrawSegment( dc, &rpn , &r_rband, (ViewPort &)m_vp, false );
                }
            }
//        }

        MyFrame *pFrame = g_pi_manager->GetParentFrame();
        wxString pathInfo;
//        if( g_bShowMag )
//            pathInfo << wxString::Format( wxString("%03d°(M)  ", wxConvUTF8 ), (int)gFrame->GetTrueOrMag( brg ) );
//        else
            pathInfo << wxString::Format( wxString("%03d°  ", wxConvUTF8 ), (int)pFrame->GetTrueOrMag( brg ) );

        pathInfo << wxS(" ") << FormatDistanceAdaptive( dist );

        wxFont *dFont;
        dFont = FontMgr::Get().GetFont( wxS("BoundaryLegInfoRollover") );
        dc.SetFont( *dFont );

        int w, h;
        int xp, yp;
        int hilite_offset = 3;
    #ifdef __WXMAC__
        wxScreenDC sdc;
        sdc.GetTextExtent(routeInfo, &w, &h, NULL, NULL, dFont);
    #else
        dc.GetTextExtent( pathInfo, &w, &h );
    #endif
        xp = r_rband.x - w;
        yp = r_rband.y;
        yp += hilite_offset;

        AlphaBlending( dc, xp, yp, w, h, 0.0, GetGlobalColor( wxS( "YELO1" ) ), 172 );

        dc.SetPen( wxPen( GetGlobalColor( wxS( "UBLCK" ) ) ) );
        dc.DrawText( pathInfo, xp, yp );

        wxString s0;
        if ( nBoundary_State >= 2 ) {
            if( !boundary->m_bIsInLayer )
                s0.Append( wxS("Boundary: ") );
            else
                s0.Append( wxS("Layer Boundary: ") );
        }

        s0 += FormatDistanceAdaptive( boundary->m_path_length + dist );
        
        RenderExtraBoundaryLegInfo( dc, r_rband, s0 );
    }
}

void ocpn_draw_pi::RenderExtraBoundaryLegInfo( ocpnDC &dc, wxPoint ref_point, wxString s )
{
    wxFont *dFont = FontMgr::Get().GetFont( wxS("BoundaryLegInfoRollover") );
    dc.SetFont( *dFont );

    int w, h;
    int xp, yp;
    int hilite_offset = 3;
#ifdef __WXMAC__
    wxScreenDC sdc;
    sdc.GetTextExtent(s, &w, &h, NULL, NULL, dFont);
#else
    dc.GetTextExtent( s, &w, &h );
#endif

    xp = ref_point.x - w;
    yp = ref_point.y + h;
    yp += hilite_offset;

    AlphaBlending( dc, xp, yp, w, h, 0.0, GetGlobalColor( wxS( "YELO1" ) ), 172 );

    dc.SetPen( wxPen( GetGlobalColor( wxS( "UBLCK" ) ) ) );
    dc.DrawText( s, xp, yp );
}

void ocpn_draw_pi::SetCurrentViewPort(PlugIn_ViewPort &vp)
{
    m_vp = &vp;
}

void ocpn_draw_pi::FinishBoundary( void )
{
    nBoundary_State = 0;
    m_prev_pMousePoint = NULL;

    SetToolbarItemState( m_leftclick_boundary_id, false );
//    cc1->SetCursor( *pCursorArrow );
    m_bDrawingBoundary = false;
    
    if ( m_pMouseBoundary && m_pMouseBoundary->GetnPoints() > 1 && m_pMouseBoundary->m_pLastAddedPoint != m_pMouseBoundary->m_pFirstAddedPoint ) {
        pSelect->AddSelectablePathSegment(m_prev_rlat, m_prev_rlon, m_dStartLat, m_dStartLon, m_pMouseBoundary->m_pLastAddedPoint, m_pMouseBoundary->m_pFirstAddedPoint, m_pMouseBoundary );
        m_pMouseBoundary->AddPoint( m_pMouseBoundary->m_pFirstAddedPoint );
        m_pMouseBoundary->m_lastMousePointIndex = m_pMouseBoundary->GetnPoints();
    }

    if( m_pMouseBoundary ) {
        if( m_pMouseBoundary->GetnPoints() > 1 ) {
            pConfig->AddNewPath( m_pMouseBoundary, -1 );    // use auto next num
        } else {
            g_pPathMan->DeletePath( m_pMouseBoundary );
            m_pMouseBoundary = NULL;
        }

        if( m_pMouseBoundary ){
            m_pMouseBoundary->RebuildGUIDList(); // ensure the GUID list is intact and good
            m_pMouseBoundary->SetHiLite(0);
        }
            

        if( pPathPropDialog && ( pPathPropDialog->IsShown() ) ) {
            pPathPropDialog->SetPathAndUpdate( m_pMouseBoundary, true );
        }

        if( pPathManagerDialog && pPathManagerDialog->IsShown() )
            pPathManagerDialog->UpdatePathListCtrl();

    }
    
    m_pMouseBoundary = NULL;

    m_pSelectedBoundary = NULL;
    m_pFoundOCPNPointSecond = NULL;
    
    undo->InvalidateUndo();
    cc1->Refresh(true);
}

void ocpn_draw_pi::DrawAllPathsInBBox(ocpnDC &dc,  LLBBox& BltBBox)
{
    Boundary *active_boundary = NULL;

    wxBoundaryListNode *pnode = (wxBoundaryListNode *)pPathList->GetFirst();
    while( pnode ) {
        bool b_run = false;
        bool b_drawn = false;
        Boundary *pBoundaryDraw = pnode->GetData();
        if( pBoundaryDraw ) {

            wxBoundingBox test_box = pBoundaryDraw->GetBBox();

            if( b_run ) test_box.Expand( m_lon, m_lat );

            if( !BltBBox.IntersectOut( test_box ) ) // Boundary is not wholly outside window
            {
                b_drawn = true;

                if( ( pBoundaryDraw != active_boundary ) )
                    pBoundaryDraw->Draw( dc, cc1->GetVP() );
            } else if( pBoundaryDraw->CrossesIDL() ) {
                wxPoint2DDouble xlate( -360., 0. );
                wxBoundingBox test_box1 = pBoundaryDraw->GetBBox();
                test_box1.Translate( xlate );
                if( b_run ) test_box1.Expand( m_lon, m_lat );

                if( !BltBBox.IntersectOut( test_box1 ) ) // Boundary is not wholly outside window
                {
                    b_drawn = true;
                    if( ( pBoundaryDraw != active_boundary ) ) pBoundaryDraw->Draw( dc, cc1->GetVP() );
                }
            }

            //      Need to quick check for the case where VP crosses IDL
            if( !b_drawn ) {
                if( ( BltBBox.GetMinX() < -180. ) && ( BltBBox.GetMaxX() > -180. ) ) {
                    wxPoint2DDouble xlate( -360., 0. );
                    wxBoundingBox test_box2 = pBoundaryDraw->GetBBox();
                    test_box2.Translate( xlate );
                    if( !BltBBox.IntersectOut( test_box2 ) ) // Boundary is not wholly outside window
                    {
                        b_drawn = true;
                        if( ( pBoundaryDraw != active_boundary ) ) pBoundaryDraw->Draw( dc, cc1->GetVP() );
                    }
                } else if( !b_drawn && ( BltBBox.GetMinX() < 180. ) && ( BltBBox.GetMaxX() > 180. ) ) {
                    wxPoint2DDouble xlate( 360., 0. );
                    wxBoundingBox test_box3 = pBoundaryDraw->GetBBox();
                    test_box3.Translate( xlate );
                    if( !BltBBox.IntersectOut( test_box3 ) ) // Boundary is not wholly outside window
                    {
                        b_drawn = true;
                        if( ( pBoundaryDraw != active_boundary ) ) pBoundaryDraw->Draw( dc, cc1->GetVP() );
                    }
                }
            }
        }

        pnode = pnode->GetNext();
    }

    //  Draw any active or selected route, boundary or track last, so that is is always on top
    if( active_boundary ) active_boundary->Draw( dc, cc1->GetVP() );
}

bool ocpn_draw_pi::CreateBoundaryLeftClick( wxMouseEvent &event )
{
    //Process boundary creation
    double rlat, rlon;
    
    

//            pCursorPencil = new wxCursor ( ICursorPencil );
//            cc1->SetCursor( *pCursorPencil );

    rlat = m_cursor_lat;
    rlon = m_cursor_lon;

    m_bBoundaryEditing = true;

    if( nBoundary_State == 1 ) {
        m_pMouseBoundary = new Boundary();
        pBoundaryList->Append( m_pMouseBoundary );
        pPathList->Append( m_pMouseBoundary);
        r_rband.x = event.GetX();
        r_rband.y = event.GetY();
        m_dStartLat = m_cursor_lat;
        m_dStartLon = m_cursor_lon;
    }

    //    Check to see if there is a nearby point which may be reused
    OCPNPoint *pMousePoint = NULL;

    //    Calculate meaningful SelectRadius
    int nearby_sel_rad_pix = 8;
//        double nearby_radius_meters = nearby_sel_rad_pix / m_true_scale_ppm;
    double nearby_radius_meters = nearby_sel_rad_pix / 1;

    OCPNPoint *pNearbyPoint = pOCPNPointMan->GetNearbyOCPNPoint( rlat, rlon,
                            nearby_radius_meters );
    if( pNearbyPoint && ( pNearbyPoint != m_prev_pMousePoint )
            && !pNearbyPoint->m_bIsInTrack && !pNearbyPoint->m_bIsInLayer )
    {
        int dlg_return;
#ifndef __WXOSX__
        dlg_return = OCPNMessageBox( m_parent_window, wxS("Use nearby waypoint?"),
                                        wxS("OpenCPN Boundary Create"),
                                        (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
#else
        dlg_return = wxID_YES;
#endif
        if( dlg_return == wxID_YES ) {
            pMousePoint = pNearbyPoint;

            // Using existing OCPNpoint, so nothing to delete for undo.
            if( nBoundary_State > 1 )
                undo->BeforeUndoableAction( Undo_AppendWaypoint, pMousePoint, Undo_HasParent, NULL );

            // check all other boundaries and routes to see if this point appears in any other route
            // If it appears in NO other route, then it should e considered an isolated mark
            if( !g_pPathMan->FindPathContainingOCPNPoint( pMousePoint ) ) pMousePoint->m_bKeepXPath =
                    true;
            if( !g_pPathMan->FindPathContainingOCPNPoint( pMousePoint ) ) pMousePoint->m_bKeepXPath =
                    true;
        }
    }

    if( NULL == pMousePoint ) {                 // need a new point
        pMousePoint = new OCPNPoint( rlat, rlon, wxS("diamond"), wxS(""), GPX_EMPTY_STRING );
        pMousePoint->SetNameShown( false );
        pMousePoint->m_sTypeString = wxS("BoundaryPoint");

        pConfig->AddNewOCPNPoint( pMousePoint, -1 );    // use auto next num
        pSelect->AddSelectableOCPNPoint( rlat, rlon, pMousePoint );

        if( nBoundary_State > 1 )
            undo->BeforeUndoableAction( Undo_AppendWaypoint, pMousePoint, Undo_IsOrphanded, NULL );
    }

    if(m_pMouseBoundary){
        if( nBoundary_State == 1 ) {
            // First point in the Boundary.
            m_pMouseBoundary->AddPoint( pMousePoint );
        } else {
            if( m_pMouseBoundary->m_NextLegGreatCircle ) {
                double rhumbBearing, rhumbDist, gcBearing, gcDist;
                DistanceBearingMercator( rlat, rlon, m_prev_rlat, m_prev_rlon, &rhumbBearing, &rhumbDist );
                Geodesic::GreatCircleDistBear( m_prev_rlon, m_prev_rlat, rlon, rlat, &gcDist, &gcBearing, NULL );
                double gcDistNM = gcDist / 1852.0;

                // Empirically found expression to get reasonable route segments.
                int segmentCount = (3.0 + (rhumbDist - gcDistNM)) / pow(rhumbDist-gcDistNM-1, 0.5 );

                wxString msg;
                msg << wxS("For this leg the Great Circle boundary is ")
                    << FormatDistanceAdaptive( rhumbDist - gcDistNM ) << wxS(" shorter than rhumbline.\n\n")
                    << wxS("Would you like include the Great Circle boundary points for this leg?");
                    
                m_disable_edge_pan = true;  // This helps on OS X if MessageBox does not fully capture mouse

                int answer = OCPNMessageBox( m_parent_window, msg, wxS("OpenCPN Boundary Create"), wxYES_NO | wxNO_DEFAULT );

                m_disable_edge_pan = false;
                
                if( answer == wxID_YES ) {
                    OCPNPoint* gcPoint;
                    OCPNPoint* prevGcPoint = m_prev_pMousePoint;
                    wxRealPoint gcCoord;

                    for( int i = 1; i <= segmentCount; i++ ) {
                        double fraction = (double) i * ( 1.0 / (double) segmentCount );
                        Geodesic::GreatCircleTravel( m_prev_rlon, m_prev_rlat, gcDist * fraction,
                                gcBearing, &gcCoord.x, &gcCoord.y, NULL );

                        if( i < segmentCount ) {
                            gcPoint = new OCPNPoint( gcCoord.y, gcCoord.x, wxS("xmblue"), wxS(""),
                                    GPX_EMPTY_STRING );
                            gcPoint->SetNameShown( false );
                            pConfig->AddNewOCPNPoint( gcPoint, -1 );
                            pSelect->AddSelectableOCPNPoint( gcCoord.y, gcCoord.x, gcPoint );
                        } else {
                            gcPoint = pMousePoint; // Last point, previously exsisting!
                        }

                        m_pMouseBoundary->AddPoint( gcPoint );
                        pSelect->AddSelectablePathSegment( prevGcPoint->m_lat, prevGcPoint->m_lon,
                                gcPoint->m_lat, gcPoint->m_lon, prevGcPoint, gcPoint, m_pMouseBoundary );
                        prevGcPoint = gcPoint;
                    }

                    undo->CancelUndoableAction( true );

                } else {
                    m_pMouseBoundary->AddPoint( pMousePoint );
                    pSelect->AddSelectablePathSegment( m_prev_rlat, m_prev_rlon,
                            rlat, rlon, m_prev_pMousePoint, pMousePoint, m_pMouseBoundary );
                    undo->AfterUndoableAction( m_pMouseBoundary );
                }
            } else {
                // Ordinary rhumblinesegment.
                m_pMouseBoundary->AddPoint( pMousePoint );
                pSelect->AddSelectablePathSegment( m_prev_rlat, m_prev_rlon,
                        rlat, rlon, m_prev_pMousePoint, pMousePoint, m_pMouseBoundary );
                undo->AfterUndoableAction( m_pMouseBoundary );
            }
        }
    }

    m_prev_rlat = rlat;
    m_prev_rlon = rlon;
    m_prev_pMousePoint = pMousePoint;
    if(m_pMouseBoundary)
        m_pMouseBoundary->m_lastMousePointIndex = m_pMouseBoundary->GetnPoints();

    nBoundary_State++;
    cc1->InvalidateGL();
    cc1->Refresh( false );
        
    return TRUE;
} 

void ocpn_draw_pi::OnTimer(wxTimerEvent& ev)
{
	ProcessTimerEvent( ev );
}

void ocpn_draw_pi::ProcessTimerEvent(wxTimerEvent& ev)
{
	//if(popUp())
	//	plogbook_pi->m_plogbook_window->logbook->appendRow(true, true);
}

void ocpn_draw_pi::PopupMenuHandler(wxCommandEvent& ev)
{
	//if(plogbook_pi->eventsEnabled || NULL == plogbook_pi->m_plogbook_window) return false;

	MyFrame *pFrame = g_pi_manager->GetParentFrame();
  //wxFrame *frame = (wxFrame*)plogbook_pi->m_parent_window->GetParent();
	if(pFrame->IsIconized())
	{
        if(pFrame->IsIconized())
            pFrame->Iconize(false);

//        plogbook_pi->m_plogbook_window->Iconize(false);
//        plogbook_pi->m_parent_window->SetFocus();
        return;
	}
/*
	if(!plogbook_pi->m_plogbook_window->IsShown() && plogbook_pi->opt->popup)
	{
		plogbook_pi->m_plogbook_window->Show();
        plogbook_pi->SendLogbookMessage(_T("LOGBOOK_WINDOW_SHOWN"), wxEmptyString);
		plogbook_pi->dlgShow = true;
	}

	if(plogbook_pi->opt->popup)
		plogbook_pi->m_plogbook_window->Raise();
*/
	return;
}
