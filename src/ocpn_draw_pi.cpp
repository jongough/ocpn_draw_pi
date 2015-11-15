/******************************************************************************
 * $Id: ocpn_draw_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN General Drawing Plugin
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
 *   $EMAIL$                                                               *
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
 ***************************************************************************
 */
//#define _2_9_x_ // uncomment this to compile for 2.9.x

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "wx/wxprec.h"

#include "ocpn_draw_pi.h"
#include "Boundary.h"
#include "BoundaryMan.h"
#include "BoundaryPoint.h"
#include "BoundaryProp.h"
#include "EBL.h"
#include "EBLProp.h"
#include "ODPath.h"
#include "PathMan.h"
#include "pathmanagerdialog.h"
#include "PointMan.h"
#include "ODConfig.h"
#include "ODdc.h"
#include "ODEventHandler.h"
#include "ODPropertiesDialogImpl.h"
#include "ODicons.h"
#include "ODPoint.h"
#include "ODSelect.h"
#include "ODPointPropertiesImpl.h"
#include "ODToolbarImpl.h"
#include "ODUtils.h"
#include "version.h"
#include "SelectItem.h"
#include "TextPoint.h"

#include "chcanv.h"
#include "Layer.h"
#include "OCPNPlatform.h"
#include "pluginmanager.h"
#include "geodesic.h"
#include "FontMgr.h"
#include "IDX_entry.h"
#include "wx/stdpaths.h"
#include <wx/timer.h>
#include <wx/event.h>
#include <wx/sysopt.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/filefn.h>
#include <wx/msgdlg.h>
#include <wx/listbook.h>
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

#if !defined(NAN)
static const long long lNaN = 0xfff8000000000000;
#define NAN (*(double*)&lNaN)
#endif

ocpn_draw_pi            *g_ocpn_draw_pi;
PathList                *g_pPathList;
PointMan                *g_pODPointMan;
bool                    g_bODIsNewLayer;
int                     g_ODLayerIdx;
int                     g_path_line_width;
wxString                g_OD_default_wp_icon;
double                  g_dLat;
double                  g_dLon;
int                     g_cursor_x;
int                     g_cursor_y;
ODSelect                *g_pODSelect;
ODConfig                *g_pODConfig;
wxString                *g_SData_Locn;
void                    *g_ppimgr;
PathMan                 *g_pPathMan;
BoundaryMan             *g_pBoundaryMan;
wxString                g_default_ODPoint_icon;
ODPathPropertiesDialogImpl   *g_pODPathPropDialog;
ODPathPropertiesDialogImpl   *g_pPathPropDialog;
BoundaryProp            *g_pBoundaryPropDialog;
EBLProp                 *g_pEBLPropDialog;
PathManagerDialog       *g_pPathManagerDialog;
ODPointPropertiesImpl   *g_pODPointPropDialog;
ODPropertiesDialogImpl  *g_pOCPNDrawPropDialog;
PlugInManager           *g_OD_pi_manager;
BoundaryList            *g_pBoundaryList;
EBLList                 *g_pEBLList;
ODPointList             *g_pODPointList;
ChartCanvas             *ocpncc1;
ODPath                  *g_PathToEdit;
ODRolloverWin           *g_pPathRolloverWin;
SelectItem              *g_pRolloverPathSeg;

wxColour    g_colourActiveBoundaryLineColour;
wxColour    g_colourInActiveBoundaryLineColour;
wxColour    g_colourActiveBoundaryFillColour;
wxColour    g_colourInActiveBoundaryFillColour;
bool        g_bExclusionBoundary;
bool        g_bInclusionBoundary;
unsigned int g_uiFillTransparency;
int         g_BoundaryLineWidth; 
int         g_BoundaryLineStyle;
int         g_iInclusionBoundarySize;
wxString    g_sEBLEndIconName;
wxString    g_sEBLStartIconName;
wxColour    g_colourEBLLineColour;
bool        g_bEBLFixedEndPosition;
int         g_EBLPersistenceType;
bool        g_bEBLShowArrow;
bool        g_bEBLVRM;
int         g_EBLLineWidth; 
int         g_EBLLineStyle;
wxColour    g_colourActivePathLineColour;
wxColour    g_colourInActivePathLineColour;
wxColour    g_colourActivePathFillColour;
wxColour    g_colourInActivePathFillColour;
int         g_PathLineWidth; 
int         g_PathLineStyle;

wxString    *g_PrivateDataDir;

wxString    *g_pHome_Locn;
wxString    *g_pData;

ODEventHandler   *g_ODEventHandler;

bool            g_bODPointShowName;
bool            g_bODPointShowRangeRings;
int             g_iODPointRangeRingsNumber;
float           g_fODPointRangeRingsStep;
int             g_iODPointRangeRingsStepUnits;
wxColour        g_colourODPointRangeRingsColour;
wxString        g_sODPointIconName;
wxColour        g_colourDefaultTextColour;
wxFont          g_DisplayTextFont;
int             g_DisplayTextFontPointSize;
wxColour        g_colourDefaultTextBackgroundColour;
int             g_iTextBackgroundTransparency;
int             g_iTextPosition;
int             g_iTextTopOffsetX;
int             g_iTextTopOffsetY;
int             g_iTextBottomOffsetX;
int             g_iTextBottomOffsetY;
int             g_iTextBottomNameExtraOffsetY;
int             g_iTextCentreOffsetX;
int             g_iTextCentreOffsetY;
int             g_iTextRightOffsetX;
int             g_iTextRightOffsetY;
int             g_iTextLeftOffsetX;
int             g_iTextLeftOffsetY;

PlugIn_ViewPort *g_pivp;
ODDC          *g_pDC;
bool            g_bShowMag;
double          g_dVar;
double          g_UserVar;
double          g_n_arrival_circle_radius;
wxRect          g_blink_rect;
bool            g_btouch;

int             g_LayerIdx;
bool            g_bShowLayers;
wxString        g_VisibleLayers;
wxString        g_InvisibleLayers;
LayerList       *pLayerList;
int             g_navobjbackups;
int             g_EdgePanSensitivity;
int             g_InitialEdgePanSensitivity;

ODPoint       *pAnchorWatchPoint1;
ODPoint       *pAnchorWatchPoint2;

IDX_entry       *gpIDX;

wxString        g_locale;
int             g_click_stop;
bool            g_bConfirmObjectDelete;

ODToolbarImpl   *g_pODToolbar;
int             g_iToolbarPosX;
int             g_iToolbarPosY;
int             g_iDisplayToolbar;

double          g_dScale;
int             g_iNSScale;

ODPlugIn_Position_Fix_Ex  g_pfFix;

wxImage ICursorLeft;
wxImage ICursorRight;
wxImage ICursorUp;
wxImage ICursorDown;
wxImage ICursorPencil;
wxImage ICursorCross;

wxBitmap *_img_ocpn_draw_pi;
wxBitmap *_img_ocpn_draw_gray_pi;
wxBitmap *_img_ocpn_draw;
wxBitmap *_img_ocpn_draw_boundary;
wxBitmap *_img_ocpn_draw_boundary_gray;
wxBitmap *_img_ocpn_draw_point;
wxBitmap *_img_ocpn_draw_point_gray;
wxBitmap *_img_ocpn_draw_textpoint;
wxBitmap *_img_ocpn_draw_textpoint_gray;
wxBitmap *_img_ocpn_draw_ebl;
wxBitmap *_img_ocpn_draw_ebl_gray;
wxBitmap *_img_ocpn_draw_dr;
wxBitmap *_img_ocpn_draw_dr_gray;
const wxBitmap *_img_Bullet_green;
const wxBitmap *_img_Bullet_red;
const wxBitmap *_img_Bullet_yellow;

// Needed for ocpndc.cpp to compile. Normally would be in glChartCanvas.cpp
float g_GLMinSymbolLineWidth;

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
    g_OD_pi_manager = (PlugInManager *) ppimgr;
    g_ocpn_draw_pi = this;
    m_pSelectedPath = NULL;
    
    g_pData = GetpPrivateApplicationDataLocation();
    appendOSDirSlash( g_pData );
    g_pData->Append(_T("plugins"));
    appendOSDirSlash( g_pData );
    if ( !wxDir::Exists(*g_pData))
        wxMkdir( *g_pData );
    g_pData->Append(_T("ocpn_draw_pi"));
    appendOSDirSlash( g_pData );
    if ( !wxDir::Exists(*g_pData))
        wxMkdir( *g_pData );
    g_PrivateDataDir = new wxString;
    g_PrivateDataDir->Append(*g_pData);
    g_pData->append( wxS("data") );
    appendOSDirSlash( g_pData );
    if ( !wxDir::Exists(*g_pData))
        wxMkdir( *g_pData );
    
    initialize_images();
}

ocpn_draw_pi::~ocpn_draw_pi()
{
    
}

int ocpn_draw_pi::Init(void)
{
    m_bBoundaryEditing = false;
    m_bPathEditing = false;
    m_bODPointEditing = false;
    m_bTextPointEditing = false;
    m_bEBLEditing = false;
    m_bEBLMoveOrigin = false;
    nConfig_State = 0;
    m_pMouseBoundary = NULL;
    m_pEBLBoatPoint = NULL;
    m_bDrawingBoundary = NULL;
    m_pFoundODPoint = NULL;
    m_pSelectedPath = NULL;
    m_pSelectedBoundary = NULL;
    m_pMouseBoundary = NULL;
    m_pSelectedEBL = NULL;
    m_pMouseEBL = NULL;
    g_dVar = NAN;
    nBoundary_State = 0;
    nPoint_State = 0;
    nTextPoint_State = 0;
    nPath_State = 0;
    nEBL_State = 0;
    m_chart_scale = 0.;
    g_pfFix.valid = false;
    
    // Drawing modes from toolbar
    m_Mode = 0;
    m_numModes = ID_LAST_MODE - 1;
    
    // Not sure what this is
    AddLocaleCatalog( wxS("opencpn-ocpn_draw_pi") );
    
    lastODPointInPath = wxS("-1");
    eventsEnabled = true;
    
    // Get a pointer to the opencpn display canvas, to use as a parent for windows created
    m_parent_window = GetOCPNCanvasWindow();
    
    m_pODConfig = GetOCPNConfigObject();
    g_pODConfig = new ODConfig( wxString( wxS("") ), wxString( wxS("") ), wxS(" ") );
    g_pODConfig->m_pODNavObjectChangesSet = new ODNavObjectChanges( g_pODConfig->m_sODNavObjSetChangesFile );
    //    g_pODConfig->m_pODNavObjectChangesSet = new ODNavObjectChanges( wxS("/home/jon/.opencpn/odnavobj.xml.changes") );
    wxString sChangesFile = g_pODConfig->m_sODNavObjSetChangesFile;
    //    g_pODConfig->m_pODNavObjectChangesSet = new ODNavObjectChanges( sChangesFile );
    
    g_pODSelect = new ODSelect();
    
    LoadConfig();
    if(m_bLOGShowIcon) {
        m_config_button_id  = InsertPlugInTool(_("OCPN Draw Manager"), _img_ocpn_draw_pi, _img_ocpn_draw_gray_pi, wxITEM_NORMAL,
                                               _("OCPN Draw Manager"), wxS(""), NULL,
                                               OCPN_DRAW_POSITION, 0, this);
        m_draw_button_id  = InsertPlugInTool(_("OCPN Draw Boundary"), _img_ocpn_draw_boundary, _img_ocpn_draw_boundary_gray, wxITEM_CHECK,
                                             _("OCPN Draw"), wxS(""), NULL,
                                             OCPN_DRAW_POSITION, 0, this);
    }
    
    // Set tool to show in tool bar
    switch (m_Mode)
    {
        case ID_MODE_BOUNDARY:
            // Boundary
            SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_boundary_gray, _img_ocpn_draw_boundary);
            break;
            
        case ID_MODE_POINT:
            // Point
            SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_point_gray, _img_ocpn_draw_point);
            break;
            
        case ID_MODE_TEXT_POINT:
            // Text Point
            SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_textpoint, _img_ocpn_draw_textpoint_gray);
            break;
            
        case ID_MODE_EBL:
            // EBL
            SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_ebl, _img_ocpn_draw_ebl_gray);
            break;
            
        default:
            // Boundary
            m_Mode = ID_MODE_BOUNDARY;
            SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_boundary_gray, _img_ocpn_draw_boundary);
            break;
    }
    
    // Create floating toolbar for drawing
    g_pODToolbar = new ODToolbarImpl( m_parent_window, ID_TOOLBARPANEL, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
    wxPoint wxpToolbarPos;
    wxpToolbarPos.x = g_iToolbarPosX;
    wxpToolbarPos.y = g_iToolbarPosY;
    g_pODToolbar->SetPosition( wxpToolbarPos );
    g_pODToolbar->Fit();
    g_pODToolbar->SetInitialSize();
    //g_pODToolbar->SetMaxSize(g_pODToolbar->GetSize());
    if( g_iToolbarPosX == 0 && g_iToolbarPosY == 0 ) g_pODToolbar->CenterOnParent();
    if( g_iDisplayToolbar == ID_DISPLAY_ALWAYS ) g_pODToolbar->Show();
    g_pODToolbar->SetToolbarTool( m_Mode );
    
    // TODO fix up undo
    //    undo = new Undo();
    // Create the Context Menu Items
    
    //    In order to avoid an ASSERT on msw debug builds,
    //    we need to create a dummy menu to act as a surrogate parent of the created MenuItems
    //    The Items will be re-parented when added to the real context meenu
    wxMenu dummy_menu;
    
    // Now initialize UI Style.
    //g_ODStyleManager = new ocpnStyle::StyleManager();
    //g_ODStyleManager = (ocpnStyle::StyleManager *)GetStyleManager_PlugIn();
    
    ocpncc1 = (ChartCanvas *)m_parent_window;
    
    // Create an OCPN Draw event handler
    g_ODEventHandler = new ODEventHandler( g_ocpn_draw_pi );
    
    g_pPathRolloverWin = new ODRolloverWin( m_parent_window );
    g_pRolloverPathSeg = NULL;
    
    m_RolloverPopupTimer.SetOwner( ocpncc1, ODROPOPUP_TIMER );
    m_rollover_popup_timer_msec = 20;
    m_parent_window->Connect( m_RolloverPopupTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler( ODEventHandler::OnRolloverPopupTimerEvent ) );
    
    //    m_pCurrentCursor = ocpncc1->pCursorArrow;
    m_pCurrentCursor = NULL;
    
    //build_cursors(); // build cursors to use on chart
    m_pTextCursorCross = new wxCursor( wxCURSOR_CHAR );
    
    if( g_pODPointMan == NULL ) g_pODPointMan = new PointMan();
    g_pODPointMan->SetColorScheme( global_color_scheme );
    
    g_pPathMan = new PathMan();
    g_pPathMan->SetColorScheme( global_color_scheme );
    g_pBoundaryMan = new BoundaryMan();
    
    g_pODPathPropDialog = NULL;
    g_pBoundaryPropDialog = NULL;
    g_pEBLPropDialog = NULL;
    
    g_pODConfig->LoadNavObjects();
    
    
    return (
    WANTS_OVERLAY_CALLBACK  |
    WANTS_CURSOR_LATLON       |
    WANTS_TOOLBAR_CALLBACK    |
    INSTALLS_TOOLBAR_TOOL     |
    WANTS_CONFIG              |
    INSTALLS_TOOLBOX_PAGE     |
    INSTALLS_CONTEXTMENU_ITEMS  |
    WANTS_NMEA_EVENTS         |
    USES_AUI_MANAGER                |
    WANTS_PREFERENCES         |
    WANTS_ONPAINT_VIEWPORT    |
    WANTS_PLUGIN_MESSAGING    |
    WANTS_OPENGL_OVERLAY_CALLBACK |
    WANTS_LATE_INIT           |
    WANTS_MOUSE_EVENTS        |
    WANTS_VECTOR_CHART_OBJECT_INFO  |
    INSTALLS_PLUGIN_CHART_GL  |
    WANTS_KEYBOARD_EVENTS
    );
}

void ocpn_draw_pi::LateInit(void)
{
    SendPluginMessage(wxS("OCPN_DRAW_PI_READY_FOR_REQUESTS"), wxS("TRUE"));
    
    return;
}

bool ocpn_draw_pi::DeInit(void)
{
    ocpncc1->Disconnect( m_RolloverPopupTimer.GetId(), wxTimerEventHandler( ODEventHandler::OnRolloverPopupTimerEvent ) );
    if( g_ODEventHandler ) delete g_ODEventHandler;
    g_ODEventHandler = NULL;
    if( g_pPathRolloverWin ) delete g_pPathRolloverWin;
    g_pPathRolloverWin = NULL;
    
    if( m_config_button_id ) RemovePlugInTool(m_config_button_id);
    m_config_button_id = 0;
    
    if( m_draw_button_id ) RemovePlugInTool(m_draw_button_id);
    m_draw_button_id = 0;
    if( g_pODConfig ) {
        g_pODConfig->UpdateNavObj();
        SaveConfig();
    }
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

void ocpn_draw_pi::SetColorScheme(PI_ColorScheme cs)
{
    global_color_scheme = cs;
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
void ocpn_draw_pi::SendVectorChartObjectInfo(wxString &chart, wxString &feature, wxString &objname, double lat, double lon, double scale, int nativescale) 
{
    g_dScale = scale;
    g_iNSScale = nativescale;
}

void ocpn_draw_pi::OnContextMenuItemCallback(int id)
{
    switch ( id ) {
        case ID_PATH_MENU_PROPERTIES: {
            //        ShowPathPropertiesDialog( wxT("Path Properties"), m_pSelectedPath );
            break;
        }
    }
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
    //dlgShow = false;
    if( NULL == g_pOCPNDrawPropDialog )
        g_pOCPNDrawPropDialog = new ODPropertiesDialogImpl( parent );
    
    g_pOCPNDrawPropDialog->SetDialogSize();
    g_pOCPNDrawPropDialog->UpdateProperties();
    
    g_pOCPNDrawPropDialog->ShowModal();
    
    delete g_pOCPNDrawPropDialog;
    g_pOCPNDrawPropDialog = NULL;
    
}

void ocpn_draw_pi::SetPositionFixEx( PlugIn_Position_Fix_Ex &pfix )
{
    double incLat, incLon;
    
    if(pfix.FixTime && pfix.nSats)
        m_LastFixTime = wxDateTime::Now();

    incLat = g_pfFix.Lat - pfix.Lat;
    incLon = g_pfFix.Lon - pfix.Lon;
    
    g_pfFix.Lat = pfix.Lat;
    g_pfFix.Lon = pfix.Lon;
    g_pfFix.Cog = pfix.Cog;
    g_pfFix.Sog = pfix.Sog;
    g_pfFix.Var = pfix.Var;
    g_pfFix.Hdm = pfix.Hdm;
    g_pfFix.Hdt = pfix.Hdt;
    g_pfFix.FixTime = pfix.FixTime;
    g_pfFix.nSats = pfix.nSats;
    if(!g_pfFix.valid) {
        g_pfFix.valid = true;
        return;
    }
    
    if(incLat || incLon) {
        wxEBLListNode *node = g_pEBLList->GetFirst();
        for(size_t i = 0; i < g_pEBLList->GetCount(); i++) {
            EBL *ebl = (EBL *)node->GetData();
            if(ebl->m_bCentreOnBoat)  {
                if(!ebl->m_bFixedEndPosition)
                    ebl->MoveEndPoint( incLat, incLon );
                ebl->CentreOnBoat();
            }
            node = node->GetNext();
        }
    }
}

void ocpn_draw_pi::OnToolbarToolDownCallback(int id)
{
    m_iCallerId = id;
    if( m_Mode == ID_NONE ) m_Mode = 0;
    
    if ( id == m_config_button_id ) {
        if( 0 == nConfig_State ){
            // show the Draw dialog
            nConfig_State = 1;
            //SetToolbarItemState( m_config_button_id, true );
            if( NULL == g_pPathManagerDialog )         // There is one global instance of the Dialog
                g_pPathManagerDialog = new PathManagerDialog( ocpncc1 );
            
            DimeWindow( g_pPathManagerDialog );
            g_pPathManagerDialog->UpdatePathListCtrl();
            g_pPathManagerDialog->UpdateODPointsListCtrl();
            g_pPathManagerDialog->Show();
            
            
            //    Required if RMDialog is not STAY_ON_TOP
            #ifdef __WXOSX__
            g_pPathManagerDialog->Centre();
            g_pPathManagerDialog->Raise();
            #endif
            nConfig_State = 0;
            //SetToolbarItemState( m_config_button_id, false );
            
        } else {
            nConfig_State = 0;
            //SetToolbarItemState( m_config_button_id, false );
        }
    }
    else if ( id == m_draw_button_id ) {
        switch (m_Mode)
        {
            case ID_MODE_BOUNDARY:
                if( 0 == nBoundary_State ){
                    nBoundary_State = 1;
                    m_pCurrentCursor = ocpncc1->pCursorPencil;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, true );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    FinishBoundary();
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                }
                break;
                
            case ID_MODE_POINT:
                if( 0 == nPoint_State ){
                    nPoint_State = 1;
                    m_pCurrentCursor = ocpncc1->pCursorCross;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, true );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                }
                break;
                
            case ID_MODE_TEXT_POINT:
                if( 0 == nTextPoint_State ){
                    nTextPoint_State = 1;
                    m_pCurrentCursor = m_pTextCursorCross;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, true );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                }
                break;
            
            case ID_MODE_EBL:
                if( 0 == nEBL_State ){
                    nEBL_State = 1;
                    m_pCurrentCursor = ocpncc1->pCursorCross;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, true );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                    RequestRefresh( m_parent_window );
                }
                break;
                
            case ID_MODE_DR:
                if( 0 == nDR_State ){
                    nDR_State = 1;
                    // Need to show DR dialog box here
                    nDR_State = 0;
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    nDR_State = 0;
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                    RequestRefresh( m_parent_window );
                }
                break;
            default:
                m_Mode = ID_MODE_BOUNDARY;
                break;
        }
    }
}
void ocpn_draw_pi::OnToolbarToolUpCallback(int id)
{
    return;
}
void ocpn_draw_pi::SaveConfig()
{
    wxFileConfig *pConf = m_pODConfig;
    
    if(pConf)
    {
        pConf->SetPath( wxS( "/PlugIns/libocpn_draw_pi.so" ) );
        pConf->Write( wxS( "DefaultActivePathLineColour" ), g_colourActivePathLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultInActivePathLineColour" ), g_colourInActivePathLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultActiveBoundaryLineColour" ), g_colourActiveBoundaryLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultInActiveBoundaryLineColour" ), g_colourInActiveBoundaryLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultActiveBoundaryFillColour" ), g_colourActiveBoundaryFillColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        int l_BoundaryType;
        if(g_bExclusionBoundary && !g_bInclusionBoundary) l_BoundaryType = ID_BOUNDARY_EXCLUSION;
        else if(!g_bExclusionBoundary && g_bInclusionBoundary) l_BoundaryType = ID_BOUNDARY_INCLUSION;
        else if(!g_bExclusionBoundary && !g_bInclusionBoundary) l_BoundaryType = ID_BOUNDARY_NONE;
        else l_BoundaryType = ID_BOUNDARY_EXCLUSION;
        pConf->Write( wxS( "DefaultBoundaryType" ), l_BoundaryType );
        long l_longFillTransparency = g_uiFillTransparency;
        pConf->Write( wxS( "DefaultBoundaryFillTransparency" ), l_longFillTransparency );
        pConf->Write( wxS( "DefaultInclusionBoundarySize" ), g_iInclusionBoundarySize );
        pConf->Write( wxS( "DefaultInActiveBoundaryFillColour" ), g_colourInActiveBoundaryFillColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultEBLStartIcon" ), g_sEBLStartIconName );
        pConf->Write( wxS( "DefaultEBLEndIcon" ), g_sEBLEndIconName );
        pConf->Write( wxS( "DefaultEBLLineColour" ), g_colourEBLLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultBoundaryLineWidth" ), g_BoundaryLineWidth );
        pConf->Write( wxS( "DefaultBoundaryLineStyle" ), g_BoundaryLineStyle );
        pConf->Write( wxS( "DefaultEBLLineWidth" ), g_EBLLineWidth );
        pConf->Write( wxS( "DefaultEBLLineStyle" ), g_EBLLineStyle );
        pConf->Write( wxS( "DefaultEBLShowArrow" ), g_bEBLShowArrow );
        pConf->Write( wxS( "DefaultEBLVRM" ), g_bEBLVRM );
        pConf->Write( wxS( "DefaultEBLPersistenceType" ), g_EBLPersistenceType );
        pConf->Write( wxS( "DefaultEBLFixedEndPosition" ), g_bEBLFixedEndPosition );
        pConf->Write( wxS( "DefaultPathLineWidth" ), g_PathLineWidth );
        pConf->Write( wxS( "DefaultPathLineStyle" ), g_PathLineStyle );
        pConf->Write( wxS( "ShowLOGIcon" ), m_bLOGShowIcon );
        pConf->Write( wxS( "PathLineWidth" ), g_path_line_width );
        pConf->Write( wxS( "DefaultODPointIcon" ), g_sODPointIconName );
        pConf->Write( wxS( "ODPointRangeRingsNumber" ), g_iODPointRangeRingsNumber );
        pConf->Write( wxS( "ODPointRangeRingsStep" ), g_fODPointRangeRingsStep );
        pConf->Write( wxS( "ODPointRangeRingsStepUnits" ), g_iODPointRangeRingsStepUnits );
        pConf->Write( wxS( "ODPointRangeRingsColour" ), g_colourODPointRangeRingsColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "ShowMag" ), g_bShowMag );
        pConf->Write( wxS( "UserMagVariation" ), wxString::Format( _T("%.2f"), g_UserVar ) );
        pConf->Write( wxS( "KeepODNavobjBackups" ), g_navobjbackups );
        pConf->Write( wxS( "CurrentDrawMode" ), m_Mode );
        pConf->Write( wxS( "ConfirmObjectDelete" ), g_bConfirmObjectDelete );
        pConf->Write( wxS( "InitialEdgePanSensitivity" ), g_InitialEdgePanSensitivity );
        pConf->Write( wxS( "EdgePanSensitivity" ), g_EdgePanSensitivity );
        pConf->Write( wxS( "ToolBarPosX" ), g_iToolbarPosX );
        pConf->Write( wxS( "ToolBarPosY" ), g_iToolbarPosY );
        pConf->Write( wxS( "DisplayToolbar"), g_iDisplayToolbar );
        pConf->Write( wxS( "DefaultTextColour" ), g_colourDefaultTextColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultTextBackgroundColour" ), g_colourDefaultTextBackgroundColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultTextBackgroundTransparency" ), g_iTextBackgroundTransparency );
        pConf->Write( wxS( "DefaultTextPosition" ), g_iTextPosition );
        pConf->Write( wxS( "DefaultTextTopOffsetX" ), g_iTextTopOffsetX );
        pConf->Write( wxS( "DefaultTextTopOffsetY" ), g_iTextTopOffsetY );
        pConf->Write( wxS( "DefaultTextBottomffsetX" ), g_iTextBottomOffsetX );
        pConf->Write( wxS( "DefaultTextBottomOffsetY" ), g_iTextBottomOffsetY );
        pConf->Write( wxS( "DefaultTextBottomNameExtraOffsetY"), g_iTextBottomNameExtraOffsetY );
        pConf->Write( wxS( "DefaultTextCenterOffsetX" ), g_iTextCentreOffsetX );
        pConf->Write( wxS( "DefaultTextCentreOffsetY" ), g_iTextCentreOffsetY );
        pConf->Write( wxS( "DefaultTextRightOffsetX" ), g_iTextRightOffsetX );
        pConf->Write( wxS( "DefaultTextRightOffsetY" ), g_iTextRightOffsetY );
        pConf->Write( wxS( "DefaultTextLeftOffsetX" ), g_iTextLeftOffsetX );
        pConf->Write( wxS( "DefaultTextLeftOffsetY" ), g_iTextLeftOffsetY );
        pConf->Write( wxS( "DefaultTextPointPointSize" ), g_DisplayTextFont.GetPointSize() );
        pConf->Write( wxS( "DefaultTextPointFontFamily" ), (int)g_DisplayTextFont.GetFamily() );
        pConf->Write( wxS( "DefaultTextPointFontStyle" ), (int)g_DisplayTextFont.GetStyle() );
        pConf->Write( wxS( "DefaultTextPointFontWeight" ), (int)g_DisplayTextFont.GetWeight() );
        pConf->Write( wxS( "DefaultTextPointFontUnderline" ), g_DisplayTextFont.GetUnderlined() );
        pConf->Write( wxS( "DefaultTextPointFontStrikethrough" ), g_DisplayTextFont.GetStrikethrough() );
        pConf->Write( wxS( "DefaultTextPointFaceName" ), g_DisplayTextFont.GetFaceName() );
        pConf->Write( wxS( "DefaultTextPointFontEncoding" ), (int)g_DisplayTextFont.GetEncoding() );
        
    }
}
void ocpn_draw_pi::LoadConfig()
{
    wxFileConfig *pConf = (wxFileConfig *)m_pODConfig;
    
    if(pConf)
    {
        wxString val;
        pConf->SetPath( wxS( "/PlugIns/libocpn_draw_pi.so" ) );
        wxString  l_wxsColour;
        pConf->Read( wxS( "DefaultActivePathLineColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourActivePathLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActivePathLineColour" ), &l_wxsColour, wxS( "LIGHT_GREY" ) );
        g_colourInActivePathLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultActiveBoundaryLineColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourActiveBoundaryLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActiveBoundaryLineColour" ), &l_wxsColour, wxS( "LIGHT_GREY" ) );
        g_colourInActiveBoundaryLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultActiveBoundaryFillColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourActiveBoundaryFillColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActiveBoundaryFillColour" ), &l_wxsColour, wxS( "LIGHT_GREY" ) );
        g_colourInActiveBoundaryFillColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActiveBoundaryFillColour" ), &l_wxsColour, wxS( "LIGHT_GREY" ) );
        int l_BoundaryType;
        pConf->Read( wxS( "DefaultBoundaryType" ), &l_BoundaryType, ID_BOUNDARY_EXCLUSION );
        switch (l_BoundaryType) {
            case ID_BOUNDARY_EXCLUSION:
                g_bExclusionBoundary = true;
                g_bInclusionBoundary = false;
                break;
            case ID_BOUNDARY_INCLUSION:
                g_bExclusionBoundary = false;
                g_bInclusionBoundary = true;
                break;
            case ID_BOUNDARY_NONE:
                g_bExclusionBoundary = false;
                g_bInclusionBoundary = false;
                break;
            default:
                g_bExclusionBoundary = true;
                g_bInclusionBoundary = false;
                break;
        }
        pConf->Read( wxS( "DefaultEBLLineColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourEBLLineColour.Set( l_wxsColour );
        long l_longFillTransparency;
        pConf->Read( wxS( "DefaultBoundaryFillTransparency" ), &l_longFillTransparency, 175 );
        g_uiFillTransparency = l_longFillTransparency;
        pConf->Read( wxS( "DefaultInclusionBoundarySize" ), &g_iInclusionBoundarySize, 15 );
        pConf->Read( wxS( "DefaultBoundaryLineWidth" ), &g_BoundaryLineWidth, 2  );
        pConf->Read( wxS( "DefaultBoundaryLineStyle" ), &g_BoundaryLineStyle, wxSOLID );
        pConf->Read( wxS( "DefaultEBLEndIcon" ), &g_sEBLEndIconName, wxS("Circle") );
        pConf->Read( wxS( "DefaultEBLStartIcon" ), &g_sEBLStartIconName, wxS("Circle") );
        pConf->Read( wxS( "DefaultEBLLineWidth" ), &g_EBLLineWidth, 2  );
        pConf->Read( wxS( "DefaultEBLLineStyle" ), &g_EBLLineStyle, wxSOLID );
        pConf->Read( wxS( "DefaultEBLShowArrow" ), &g_bEBLShowArrow, false );
        pConf->Read( wxS( "DefaultEBLVRM" ), &g_bEBLVRM, false );
        pConf->Read( wxS( "DefaultEBLPersistenceType" ),  &g_EBLPersistenceType, 0 );
        pConf->Read( wxS( "DefaultEBLFixedEndPosition" ),  &g_bEBLFixedEndPosition, 0 );
        pConf->Read( wxS( "DefaulPathLineWidth" ), &g_PathLineWidth, 2  );
        pConf->Read( wxS( "DefaultPathLineStyle" ), &g_PathLineStyle, 100 );
        pConf->Read( wxS( "ShowLOGIcon" ),  &m_bLOGShowIcon, 1 );
        pConf->Read( wxS( "PathLineWidth" ), &g_path_line_width, 2 );
        pConf->Read( wxS( "DefaultODPointIcon" ), &g_sODPointIconName, wxS("triangle") );
        pConf->Read( wxS( "ODPointRangeRingsNumber" ), &g_iODPointRangeRingsNumber, 0 );
        pConf->Read( wxS( "ODPointRangeRingsStep" ), &val, wxS("1.0") );
        g_fODPointRangeRingsStep = atof( val.mb_str() );
        pConf->Read( wxS( "ODPointRangeRingsStepUnits" ), &g_iODPointRangeRingsStepUnits, 0 );
        wxString  l_wxsODPointRangeRingsColour;
        g_colourODPointRangeRingsColour = wxColour( *wxRED );
        pConf->Read( wxS( "ODPointRangeRingsColour" ), &l_wxsODPointRangeRingsColour, wxS( "RED" ) );
        g_colourODPointRangeRingsColour.Set( l_wxsODPointRangeRingsColour );
        pConf->Read( wxS( "ShowMag" ), &g_bShowMag, 0 );
        g_UserVar = 0.0;
        wxString umv;
        pConf->Read( wxS( "UserMagVariation" ), &umv );
        if(umv.Len())
            umv.ToDouble( &g_UserVar );
        pConf->Read( wxS( "KeepODNavobjBackups" ), &g_navobjbackups, 0 );
        pConf->Read( wxS( "CurrentDrawMode" ), &m_Mode, 0 );
        pConf->Read( wxS( "ConfirmObjectDelete" ), &g_bConfirmObjectDelete, 0 );
        
        pConf->Read( wxS( "InitialEdgePanSensitivity" ), &g_InitialEdgePanSensitivity, 2);
        pConf->Read( wxS( "EdgePanSensitivity" ), &g_EdgePanSensitivity, 5);
        
        pConf->Read( wxS( "ToolBarPosX" ), &g_iToolbarPosX, 0);
        pConf->Read( wxS( "ToolBarPosY" ), &g_iToolbarPosY, 0);
        pConf->Read( wxS( "DisplayToolbar" ), &g_iDisplayToolbar, 1 );
        wxString  l_wxsDefautlTextColour;
        g_colourDefaultTextColour = wxColour( *wxBLACK );
        pConf->Read( wxS( "DefaultTextColour" ), &l_wxsDefautlTextColour, wxS( "BLACK" ) );
        g_colourDefaultTextBackgroundColour.Set( l_wxsDefautlTextColour );
        wxString  l_wxsDefautlTextBackgroundColour;
        g_colourDefaultTextBackgroundColour = wxColour( *wxYELLOW );
        pConf->Read( wxS( "DefaultTextBackgroundColour" ), &l_wxsDefautlTextBackgroundColour, wxS( "YELLOW" ) );
        g_colourDefaultTextBackgroundColour.Set( l_wxsDefautlTextBackgroundColour );
        pConf->Read( wxS( "DefaultTextBackgroundTransparency" ), &g_iTextBackgroundTransparency, 100 );
        pConf->Read( wxS( "DefaultTextPosition" ), &g_iTextPosition, ID_TEXT_BOTTOM );
        pConf->Read( wxS( "DefaultTextTopOffsetX" ), &g_iTextTopOffsetX, -10 );
        pConf->Read( wxS( "DefaultTextTopOffsetY" ), &g_iTextTopOffsetY, -5 );
        pConf->Read( wxS( "DefaultTextBottomOffsetX" ), &g_iTextBottomOffsetX, -10 );
        pConf->Read( wxS( "DefaultTextBottomOffsetY" ), &g_iTextBottomOffsetY, 10 );
        pConf->Read( wxS( "DefaultTextBottomNameExtraOffsetY"), &g_iTextBottomNameExtraOffsetY, 10 );
        pConf->Read( wxS( "DefaultTextCentreOffsetX" ), &g_iTextCentreOffsetX, 0 );
        pConf->Read( wxS( "DefaultTextCentreOffsetY" ), &g_iTextCentreOffsetY, 0 );
        pConf->Read( wxS( "DefaultTextRightOffsetX" ), &g_iTextRightOffsetX, 10 );
        pConf->Read( wxS( "DefaultTextRightOffsetY" ), &g_iTextRightOffsetY, -5 );
        pConf->Read( wxS( "DefaultTextLeftOffsetX" ), &g_iTextLeftOffsetX, -25 );
        pConf->Read( wxS( "DefaultTextLeftOffsetY" ), &g_iTextLeftOffsetY, -5 );
        int l_fontInfo;
        bool l_bfontInfo;
        wxString l_wxsfontInfo;
        wxFont *l_pDisplayTextFont = OCPNGetFont( wxS("Marks"), 0 );
        pConf->Read( wxS( "DefaultTextPointPointSize" ), &l_fontInfo, (int)l_pDisplayTextFont->GetPointSize() );
        g_DisplayTextFont.SetPointSize( l_fontInfo );
        pConf->Read( wxS( "DefaultTextPointFontFamily" ), &l_fontInfo, (int)l_pDisplayTextFont->GetFamily() );
        g_DisplayTextFont.SetFamily( l_fontInfo );
        pConf->Read( wxS( "DefaultTextPointFontStyle" ), &l_fontInfo, (int)l_pDisplayTextFont->GetStyle() );
        g_DisplayTextFont.SetStyle( l_fontInfo );
        pConf->Read( wxS( "DefaultTextPointFontWeight" ), &l_fontInfo, (int)l_pDisplayTextFont->GetWeight() );
        g_DisplayTextFont.SetWeight( l_fontInfo );
        pConf->Read( wxS( "DefaultTextPointFontUnderline" ), &l_bfontInfo, false );
        g_DisplayTextFont.SetUnderlined( l_bfontInfo );
        pConf->Read( wxS( "DefaultTextPointFontStrikethrough" ), &l_bfontInfo, false );
        g_DisplayTextFont.SetStrikethrough( l_bfontInfo );
        wxString l_wxsFaceName;
        pConf->Read( wxS( "DefaultTextPointFaceName" ), &l_wxsFaceName, l_pDisplayTextFont->GetFaceName() );
        g_DisplayTextFont.SetFaceName( l_wxsFaceName );
        pConf->Read( wxS( "DefaultTextPointFontEncoding" ), &l_fontInfo, (int)l_pDisplayTextFont->GetEncoding() );
        g_DisplayTextFont.SetEncoding( (wxFontEncoding)l_fontInfo );
    }
    
    g_pODPointList = new ODPointList;
    g_pBoundaryList = new BoundaryList;
    g_pEBLList = new EBLList;
    g_pPathList = new PathList;
    //    Layers
    pLayerList = new LayerList;
    
}

void ocpn_draw_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    wxJSONValue     root;
    wxJSONReader    reader;
    wxJSONValue     jMsg;
    wxJSONWriter    writer;
    wxString        MsgString;
    
    wxString    sLogMessage;
    wxString    l_sType;
    wxString    l_sMsg;
    double      l_dLat;
    double      l_dLon;
    wxString    l_GUID;
    bool        bFail = false;
    
    if(message_id == wxS("OCPN_DRAW_PI")) {
        // now read the JSON text and store it in the 'root' structure
        // check for errors before retreiving values...
        int numErrors = reader.Parse( message_body, &root );
        if ( numErrors > 0 )  {
            const wxArrayString& errors = reader.GetErrors();
            for(size_t i = 0; i < errors.GetCount(); i++)
            {
                if(i == 0) {
                    sLogMessage.clear();
                    sLogMessage.Append(wxT("ocpn_draw_pi: Error parsing JSON message - "));
                    sLogMessage.Append( message_id );
                }
                else sLogMessage.Append(wxT("\n"));
                sLogMessage.append( errors.Item( i ) );
                wxLogMessage( sLogMessage );
            }
            return;
        }
        
        if(!root.HasMember( wxS("Source"))) {
            // Originator
            wxLogMessage( wxS("No Source found in message") );
            bFail = true;
        }
        
        if(!root.HasMember( wxS("Msg"))) {
            // Message identifier
            wxLogMessage( wxS("No Msg found in message") );
            bFail = true;
        }
        
        if(!root.HasMember( wxS("Type"))) {
            // Message type, orig or resp
            wxLogMessage( wxS("No Type found in message") );
            bFail = true;
        }
        
        if(!root.HasMember( wxS("MsgId"))) {
            // Unique (?) Msg number
            wxLogMessage( wxS("No MsgId found in message") );
            bFail = true;
        }

        if(!bFail && root[wxS("Msg")].AsString() == wxS("Version")) {
            jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
            jMsg[wxT("Msg")] = root[wxT("Msg")];
            jMsg[wxT("Type")] = wxT("Response");
            jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
            jMsg[wxS("Major")] = PLUGIN_VERSION_MAJOR;
            jMsg[wxS("Minor")] = PLUGIN_VERSION_MINOR;
            jMsg[wxS("Patch")] = PLUGIN_VERSION_PATCH;
            jMsg[wxS("Date")] = PLUGIN_VERSION_DATE;
            writer.Write( jMsg, MsgString );
            SendPluginMessage( root[wxS("Source")].AsString(), MsgString );
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("FindPointInAnyBoundary")) {
            if(!root.HasMember( wxS("lat"))) {
                wxLogMessage( wxS("No Latitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("lon"))) {
                wxLogMessage( wxS("No Longitude found in message") );
                bFail = true;
            }
            
            if(!bFail) {
                l_dLat = root[wxS("lat")].AsDouble();
                l_dLon = root[wxS("lon")].AsDouble();
                
                l_sType = root[wxS("Type")].AsString();
                l_sMsg = root[wxT("Msg")].AsString();
                
                if(l_sType == wxS("Request")) {
                    if(l_sMsg == wxT("FindPointInAnyBoundary")) {
                        wxString l_sGUID = GetBoundaryWithPointInBoundary( l_dLat, l_dLon );
                        if(l_sGUID.length() > 0)
                            jMsg[wxS("Found")] = true;
                        else 
                            jMsg[wxS("Found")] = false;
                        jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                        jMsg[wxT("Msg")] = root[wxT("Msg")];
                        jMsg[wxT("Type")] = wxT("Response");
                        jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                        jMsg[wxS("GUID")] = l_sGUID;
                        jMsg[wxS("lat")] = l_dLat;
                        jMsg[wxS("lon")] = l_dLon;
                        writer.Write( jMsg, MsgString );
                        SendPluginMessage( root[wxS("Source")].AsString(), MsgString );
                        return;
                    }
                }
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("FindPointInBoundary")) {
            if(!root.HasMember( wxS("GUID"))) {
                wxLogMessage( wxS("No GUID found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("lat"))) {
                wxLogMessage( wxS("No Latitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("lon"))) {
                wxLogMessage( wxS("No Longitude found in message") );
                bFail = true;
            }
            
            if(!bFail) {
                wxString l_sGUID = root[wxS("GUID")].AsString();
                l_dLat = root[wxS("lat")].AsDouble();
                l_dLon = root[wxS("lon")].AsDouble();
                
                l_sType = root[wxS("Type")].AsString();
                l_sMsg = root[wxT("Msg")].AsString();
                
                if(l_sType == wxS("Request")) {
                    Boundary *l_boundary;
                    if(l_sMsg == wxS("FindPointInBoundary")) {
                        l_dLat = root[wxS("lat")].AsDouble();
                        l_dLon = root[wxS("lon")].AsDouble();
                        
                        l_boundary = (Boundary *)g_pBoundaryMan->FindPathByGUID( root[wxS("GUID")].AsString() );
                        if(!l_boundary) {
                            wxString l_msg;
                            l_msg.append( wxS("Boundary, with GUID: ") );
                            l_msg.append(root[wxS("GUID")].AsString());
                            l_msg.append( wxS(", not found") );
                            wxLogMessage( l_msg );
                            jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                            jMsg[wxT("Msg")] = root[wxT("Msg")];
                            jMsg[wxT("Type")] = wxT("Response");
                            jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                            jMsg[wxS("Found")] = false;
                            jMsg[wxS("lat")] = l_dLat;
                            jMsg[wxS("lon")] = l_dLon;
                            jMsg[wxS("GUID")] = root[wxS("GUID")];
                            writer.Write( jMsg, MsgString );
                            SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                            return;
                        }
                        
                        bool l_bFound = FindPointInBoundary( l_boundary, l_dLat, l_dLon );
                        jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                        jMsg[wxT("Msg")] = root[wxT("Msg")];
                        jMsg[wxT("Type")] = wxT("Response");
                        jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                        jMsg[wxS("Found")] = l_bFound;
                        jMsg[wxS("lat")] = l_dLat;
                        jMsg[wxS("lon")] = l_dLon;
                        jMsg[wxS("GUID")] = root[wxS("GUID")];
                        if( l_boundary->m_bExclusionBoundary && !l_boundary->m_bInclusionBoundary)
                            jMsg[wxS("BoundaryType")] = wxT("Exclusion");
                        else if( !l_boundary->m_bExclusionBoundary && l_boundary->m_bInclusionBoundary)
                            jMsg[wxS("BoundaryType")] = wxT("Inclusion");
                        else if( !l_boundary->m_bExclusionBoundary && !l_boundary->m_bInclusionBoundary)
                            jMsg[wxS("BoundaryType")] = wxT("Neither");
                        else
                            jMsg[wxS("BoundaryType")] = wxT("Unknown");
                        writer.Write( jMsg, MsgString );
                        SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                        return;
                    }
                }
            }
        }
        
    } else if(message_id == _T("WMM_VARIATION_BOAT")) {

    // construct the JSON root object
        wxJSONValue  root;
    // construct a JSON parser
        wxJSONReader reader;

    // now read the JSON text and store it in the 'root' structure
    // check for errors before retreiving values...
        int numErrors = reader.Parse( message_body, &root );
        if ( numErrors > 0 )  {
//              const wxArrayString& errors = reader.GetErrors();
            return;
        }

        // get the DECL value from the JSON message
        wxString decl = root[_T("Decl")].AsString();
        double decl_val;
        decl.ToDouble(&decl_val);

        g_dVar = decl_val;
    }
    
    return;
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
                    m_Mode = ID_MODE_BOUNDARY;
                    SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_boundary, _img_ocpn_draw_boundary_gray);
                    m_iCallerId = m_draw_button_id;
                    m_pCurrentCursor = ocpncc1->pCursorPencil;
                    bret = TRUE;
                } else bret = FALSE;
                if(g_iDisplayToolbar == ID_DISPLAY_WHILST_DRAWING ) {
                    g_pODToolbar->SetPosition( wxPoint(g_iToolbarPosX, g_iToolbarPosY) );
                    g_pODToolbar->Show();
                }
                break;
                
            case 27: // Generic Break
                if( nBoundary_State > 0 ){
                    nBoundary_State = 0;
                    FinishBoundary();
                    m_pCurrentCursor = NULL;
                    SetToolbarItemState( m_draw_button_id, false );
                    RequestRefresh( m_parent_window );
                    bret = TRUE;
                } else if( nPoint_State > 0 ){
                    nPoint_State = 0;
                    m_pCurrentCursor = NULL;
                    SetToolbarItemState( m_draw_button_id, false );
                    RequestRefresh( m_parent_window );
                    bret = TRUE;
                } else if( nTextPoint_State > 0 ){
                    nTextPoint_State = 0;
                    m_pCurrentCursor = NULL;
                    SetToolbarItemState( m_draw_button_id, false );
                    RequestRefresh( m_parent_window );
                    bret = TRUE;
                } else if( nEBL_State > 0 ){
                    nEBL_State = 0;
                    m_pCurrentCursor = NULL;
                    SetToolbarItemState( m_draw_button_id, false );
                    RequestRefresh( m_parent_window );
                    bret = TRUE;
                } else if( m_bODPointEditing ) {
                    m_bODPointEditing = false;
                    m_pCurrentCursor = NULL;
                    m_pFoundODPoint->m_bIsBeingEdited = false;
                    RequestRefresh( m_parent_window );
                } else bret = FALSE;
                m_iCallerId = 0;
                g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                break;
        }
    }
    SetCursor_PlugIn( m_pCurrentCursor );
    return bret;
}

bool ocpn_draw_pi::MouseEventHook( wxMouseEvent &event )
{
    bool bret = FALSE;
    bool bRefresh = FALSE;
    
    g_cursor_x = event.GetX();
    g_cursor_y = event.GetY();
    m_cursorPoint.x = g_cursor_x;
    m_cursorPoint.y = g_cursor_y;
    
    if( g_pPathRolloverWin && g_pPathRolloverWin->IsActive() )
        m_RolloverPopupTimer.Start( 10, wxTIMER_ONE_SHOT );               // faster response while the rollover is turned on
    else
        m_RolloverPopupTimer.Start( m_rollover_popup_timer_msec, wxTIMER_ONE_SHOT );
        
        
    if( nBoundary_State == 1 || nPoint_State >= 1 || nPath_State == 1 || nTextPoint_State == 1 
        || m_bPathEditing || m_bODPointEditing || m_bTextPointEditing || nEBL_State > 0 || m_bEBLMoveOrigin ) {
        CheckEdgePan_PlugIn( g_cursor_x, g_cursor_y, event.Dragging(), g_InitialEdgePanSensitivity, 2 );
        bRefresh = TRUE;
    }
    
    if( nBoundary_State >= 2 ) {
        r_rband.x = g_cursor_x;
        r_rband.y = g_cursor_y;
        m_bDrawingBoundary = true;
        
        CheckEdgePan_PlugIn( g_cursor_x, g_cursor_y, event.Dragging(), g_EdgePanSensitivity, 2 );
        bRefresh = TRUE;
    }
    
    if ( event.LeftDClick() ) {
        FindSelectedObject();
        if( m_pSelectedPath ) {
            if( NULL == g_pPathManagerDialog )         // There is one global instance of the Dialog
                g_pPathManagerDialog = new PathManagerDialog( ocpncc1 );
            
            DimeWindow( g_pPathManagerDialog );
            g_pPathManagerDialog->ShowPathPropertiesDialog( m_pSelectedPath );
            m_pSelectedPath = NULL;
            bret = true;
        } else if( m_pFoundODPoint ) {
            if( NULL == g_pODPointPropDialog )
                g_pODPointPropDialog = new ODPointPropertiesImpl( ocpncc1 );
            
            DimeWindow( g_pODPointPropDialog );
            g_pODPointPropDialog->SetODPoint( m_pFoundODPoint );
            g_pODPointPropDialog->SetDialogSize();
            g_pODPointPropDialog->UpdateProperties();
            
            if( !g_pODPointPropDialog->IsShown() )
                g_pODPointPropDialog->Show();
            m_pFoundODPoint = NULL;
            bret = true;
        }
    }
        
    if ( event.LeftDown() ) {
        if( m_iCallerId == m_draw_button_id ) {
            if (nBoundary_State > 0 ) {   
                bret = CreateBoundaryLeftClick( event );
            } else if ( nPoint_State > 0) {
                bret = CreatePointLeftClick( event );
            } else if ( nTextPoint_State > 0 ) {
                bret = CreateTextPointLeftClick( event );
            } else if ( nEBL_State > 0 ) {
                bret = CreateEBLLeftClick( event );
            }
        } else if( m_bPathEditing ) {
            m_pCurrentCursor = ocpncc1->pCursorCross;
            if( !m_pFoundODPoint ) {
            m_PathMove_cursor_start_lat = m_cursor_lat;
            m_PathMove_cursor_start_lon = m_cursor_lon;
            } 
            bRefresh = TRUE;
        } else if ( m_bODPointEditing ) {
            m_pCurrentCursor = ocpncc1->pCursorCross;
            bret = TRUE;
        } else if ( m_bTextPointEditing ) {
            m_pCurrentCursor = m_pTextCursorCross;
            bret = TRUE;
        } 
    }
    
    if( event.LeftUp() ) {
        if (m_iCallerId == m_draw_button_id && (nBoundary_State > 0 || nPoint_State > 0 || nTextPoint_State > 0 || nEBL_State > 0 ) ) {
            bret = true;
        }
        if (m_bEBLMoveOrigin) {
            m_bEBLMoveOrigin = false;
            m_pSelectedEBL = (EBL *)m_pSelectedPath;
            m_pSelectedEBL->CentreOnLatLon( m_cursor_lat, m_cursor_lon );
            m_bPathEditing = FALSE;
            m_bODPointEditing = FALSE;
            m_pCurrentCursor = NULL;
            m_pSelectedPath->m_bIsBeingEdited = FALSE;
            
            // TODO reimplement undo
            //undo->AfterUndoableAction( m_pRoutePointEditTarget );
            m_pSelectedPath = NULL;
            m_pFoundODPoint = NULL;
            bRefresh = TRUE;
            bret = TRUE;
        } else if( m_bPathEditing || ( m_bODPointEditing && m_pSelectedPath )) {
            m_bPathEditing = FALSE;
            m_bODPointEditing = FALSE;
            m_pCurrentCursor = NULL;
            m_pSelectedPath->m_bIsBeingEdited = FALSE;
            if( m_pFoundODPoint ) {
                //g_pODSelect->UpdateSelectablePathSegments( m_pFoundODPoint );
                //m_pFoundODPoint->m_iBlink--;
                m_pFoundODPoint->m_bIsBeingEdited = false;
            }
            g_pODSelect->DeleteAllSelectablePathSegments( m_pSelectedPath );
            g_pODSelect->DeleteAllSelectableODPoints( m_pSelectedPath );
            g_pODSelect->AddAllSelectablePathSegments( m_pSelectedPath );
            g_pODSelect->AddAllSelectableODPoints( m_pSelectedPath );
            
            m_pSelectedPath->FinalizeForRendering();
            m_pSelectedPath->UpdateSegmentDistances();
            bool prev_bskip = g_pODConfig->m_bSkipChangeSetUpdate;
            g_pODConfig->m_bSkipChangeSetUpdate = false;
            g_pODConfig->UpdatePath( m_pSelectedPath );
            g_pODConfig->m_bSkipChangeSetUpdate = prev_bskip;
            
            if( m_pSelectedPath->m_pODPointList ) {
                for( unsigned int ip = 0; ip < m_pSelectedPath->m_pODPointList->GetCount(); ip++ ) {
                    ODPath *pp = (ODPath *) m_pSelectedPath->m_pODPointList->Item( ip );
                    if( g_pPathMan->IsPathValid(pp) ) {
                        pp->FinalizeForRendering();
                        pp->UpdateSegmentDistances();
                        pp->m_bIsBeingEdited = false;
                        
                        g_pODConfig->UpdatePath( pp );
                        
                        pp->SetHiLite( 0 );
                    }
                }
                bRefresh = TRUE;
            }
            
            //    Update the PathProperties Dialog, if currently shown
            if( ( NULL != g_pODPathPropDialog ) && ( g_pODPathPropDialog->IsShown() ) ) {
                if( m_pSelectedPath->m_pODPointList ) {
                    for( unsigned int ip = 0; ip < m_pSelectedPath->m_pODPointList->GetCount(); ip++ ) {
                        ODPath *pp = (ODPath *) m_pSelectedPath->m_pODPointList->Item( ip );
                        if( g_pPathMan->IsPathValid(pp) ) {
                            g_pODPathPropDialog->SetPathAndUpdate( pp, true );
                        }
                    }
                }
            }
            
            //m_pSelectedPath->m_bPtIsSelected = false;
            
            // TODO reimplement undo
            //undo->AfterUndoableAction( m_pRoutePointEditTarget );
            m_pSelectedPath = NULL;
            m_pFoundODPoint = NULL;
            bRefresh = TRUE;
            bret = TRUE;
        } else if( m_pFoundODPoint && m_bODPointEditing ) {
            m_bODPointEditing = FALSE;
            m_pFoundODPoint->m_bIsBeingEdited = FALSE;
            m_pFoundODPoint->m_bPtIsSelected = false;
            m_pCurrentCursor = NULL;
            g_pODSelect->DeleteSelectableODPoint( m_pFoundODPoint );
            g_pODSelect->AddSelectableODPoint( m_cursor_lat, m_cursor_lon, m_pFoundODPoint );
            bool prev_bskip = g_pODConfig->m_bSkipChangeSetUpdate;
            g_pODConfig->m_bSkipChangeSetUpdate = false;
            g_pODConfig->UpdateODPoint( m_pFoundODPoint );
            g_pODConfig->m_bSkipChangeSetUpdate = prev_bskip;
            
            m_pSelectedPath = NULL;
            m_pFoundODPoint = NULL;
            
            bret = TRUE;
        } else if( m_pFoundODPoint ) {
            m_pFoundODPoint->m_bPtIsSelected = false;
            bret = TRUE;
        } else if ( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
            // This is to handle the double click to bring up the dialog box so that the screen does not jump on the extra clicks.
            bret = TRUE;
        }
        
        
    }
        
    if( event.Dragging() ) {
        if( event.LeftIsDown() ) {
            if ( nBoundary_State > 0 || nPoint_State > 0 ) bret = true;
            else if( m_pFoundODPoint ) {
                if( m_bPathEditing )
                {
                    m_pCurrentCursor = ocpncc1->pCursorCross;
                    m_pFoundODPoint->m_lat = m_cursor_lat;
                    m_pFoundODPoint->m_lon = m_cursor_lon;
                    g_pODSelect->UpdateSelectablePathSegments( m_pFoundODPoint );
                    m_pSelectedPath->FinalizeForRendering();
                    m_pSelectedPath->UpdateSegmentDistances();
                    m_pSelectedPath->SetHiLite( 0 );
                    
                    //    Update the PathProperties Dialog, if currently shown
                    if( ( NULL != g_pODPathPropDialog ) && ( g_pODPathPropDialog->IsShown() ) ) g_pODPathPropDialog->UpdateProperties( m_pSelectedPath );
                    if( g_pODPointPropDialog && m_pFoundODPoint == g_pODPointPropDialog->GetODPoint() ) g_pODPointPropDialog->UpdateProperties( TRUE );
                    
                    bRefresh = TRUE;
                    bret = FALSE;
                    event.SetEventType(wxEVT_MOVING); // stop dragging canvas on event flow through
                } else if ( m_bODPointEditing ) {
                    
                    m_pFoundODPoint->m_lat = m_cursor_lat;
                    m_pFoundODPoint->m_lon = m_cursor_lon;
                    if(m_pSelectedPath && m_pSelectedPath->m_sTypeString == wxT("EBL")) {
                        EBL *m_pSelectedEBL = (EBL *)m_pSelectedPath;
                        m_pSelectedEBL->ResizeVRM( );
                    }
                    
                    if ( g_pODPointPropDialog && m_pFoundODPoint == g_pODPointPropDialog->GetODPoint() ) g_pODPointPropDialog->UpdateProperties( TRUE );
                    
                    m_pCurrentCursor = ocpncc1->pCursorCross;
                    bRefresh = TRUE;
                    bret = FALSE;
                    event.SetEventType(wxEVT_MOVING); // stop dragging canvas on event flow through
                }
            } else if(m_bPathEditing) {
                // Do move of whole path
                double l_move_lat = m_PathMove_cursor_start_lat - m_cursor_lat;
                double l_move_lon = m_PathMove_cursor_start_lon - m_cursor_lon;
                m_pSelectedPath->MoveAllPoints( l_move_lat, l_move_lon );
                m_PathMove_cursor_start_lat = m_cursor_lat;
                m_PathMove_cursor_start_lon = m_cursor_lon;
                bRefresh = TRUE;
                bret = FALSE;
                event.SetEventType(wxEVT_MOVING); // stop dragging canvas on event flow through
            } else if(m_bEBLEditing) {
                m_pFoundODPoint->m_lat = m_cursor_lat;
                m_pFoundODPoint->m_lon = m_cursor_lon;
                
                if ( g_pODPointPropDialog && m_pFoundODPoint == g_pODPointPropDialog->GetODPoint() ) g_pODPointPropDialog->UpdateProperties( TRUE );
                
                m_pCurrentCursor = ocpncc1->pCursorCross;
                bRefresh = TRUE;
                bret = FALSE;
                event.SetEventType(wxEVT_MOVING); // stop dragging canvas on event flow through
            }
            
        }
        
    }
    if ( event.RightDown() ) {
        if(m_bODPointEditing) {
            m_bODPointEditing = false;
            m_pCurrentCursor = NULL;
            m_pFoundODPoint->m_bIsBeingEdited = false;
            RequestRefresh( m_parent_window );
            bret = TRUE;
        }
        if ( nBoundary_State == 1 || nPoint_State == 1 || nTextPoint_State == 1 || nEBL_State == 1 ) {
            m_Mode++;
            SetToolbarTool();
            g_pODToolbar->SetToolbarTool( m_Mode );
            bret = TRUE;
        } else if ( nBoundary_State > 1 ) {
            m_iCallerId = 0;
            nBoundary_State = 0;
            FinishBoundary();
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nPoint_State > 1) {
            m_iCallerId = 0;
            nPoint_State = 0;
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nTextPoint_State > 1) {
            m_iCallerId = 0;
            nTextPoint_State = 0;
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nEBL_State > 1 ) {
            m_iCallerId = 0;
            nEBL_State = 0;
            m_bEBLMoveOrigin = false;
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( m_bEBLMoveOrigin ) {
            m_bEBLMoveOrigin = false;
            m_pCurrentCursor = NULL;
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nBoundary_State == 0 ) {
            FindSelectedObject();
            
            if( 0 != m_seltype ) {
                if(m_pSelectedPath) {
                    m_pSelectedBoundary = NULL;
                    m_pSelectedEBL = NULL;
                    if(m_pSelectedPath->m_sTypeString == wxT("Boundary"))
                        m_pSelectedBoundary = (Boundary *)m_pSelectedPath;
                    else if(m_pSelectedPath->m_sTypeString == wxT("EBL"))
                        m_pSelectedEBL = (EBL *)m_pSelectedPath;
                }
                g_ODEventHandler->SetCanvas( ocpncc1 );
                g_ODEventHandler->SetPath( m_pSelectedPath );
                g_ODEventHandler->SetPoint( m_pFoundODPoint );
                g_ODEventHandler->SetLatLon( m_cursor_lat, m_cursor_lon );
                g_ODEventHandler->PopupMenu( g_cursor_x, g_cursor_y, m_seltype );
                
                //RequestRefresh( m_parent_window );
                bRefresh = TRUE;
                bret = TRUE;
            } else bret = FALSE;
            
            //end           
        } 
    }
    
    //      Check to see if there is a path under the cursor
    //      If so, start the rollover timer which creates the popup
    bool b_start_rollover = false;
    if(!b_start_rollover && !m_bPathEditing){
        SelectableItemList SelList = g_pODSelect->FindSelectionList( m_cursor_lat, m_cursor_lon, SELTYPE_PATHSEGMENT );
        wxSelectableItemListNode *node = SelList.GetFirst();
        while( node ) {
            SelectItem *pFindSel = node->GetData();
            
            ODPath *pp= (ODPath *) pFindSel->m_pData3;        //candidate
            
            if( pp && pp->IsVisible() ){
                b_start_rollover = true;
                break;
            }
            node = node->GetNext();
        }       // while
    }
    
    if( b_start_rollover )
        m_RolloverPopupTimer.Start( m_rollover_popup_timer_msec, wxTIMER_ONE_SHOT );
    
    SetCursor_PlugIn( m_pCurrentCursor );
    
    if( bRefresh ) RequestRefresh( m_parent_window );
    return bret;
}

void ocpn_draw_pi::FindSelectedObject()
{
    double slat, slon;
    slat = m_cursor_lat;
    slon = m_cursor_lon;
    m_seltype = 0;
    
    SelectItem *pFindPP;
    SelectItem *pFindPathSeg;
    pFindPP = g_pODSelect->FindSelection( slat, slon, SELTYPE_OCPNPOINT );
    pFindPathSeg = g_pODSelect->FindSelection( slat, slon, SELTYPE_PATHSEGMENT );
    // start           
    m_pFoundODPoint = NULL;
    m_pSelectedPath = NULL;
    if( pFindPP ) {
        ODPoint *pFirstVizPoint = NULL;
        ODPoint *pFoundActiveODPoint = NULL;
        ODPoint *pFoundVizODPoint = NULL;
        ODPath *pSelectedActivePath = NULL;
        ODPath *pSelectedVizPath = NULL;
        
        //There is at least one OCPNpoint, so get the whole list
        SelectableItemList SelList = g_pODSelect->FindSelectionList( slat, slon, SELTYPE_OCPNPOINT );
        wxSelectableItemListNode *node = SelList.GetFirst();
        while( node ) {
            SelectItem *pFindSel = node->GetData();
            
            ODPoint *pop = (ODPoint *) pFindSel->m_pData1;        //candidate
            if( pop->m_sTypeString == wxT("EBL Point") && pop->m_MarkName == _("Boat") ) continue;
            
            //    Get an array of all paths using this point
            wxArrayPtrVoid *ppath_array = g_pPathMan->GetPathArrayContaining( pop );
            
            // Use path array (if any) to determine actual visibility for this point
            bool bop_viz = false;
            if( ppath_array ) {
                for( unsigned int ip = 0; ip < ppath_array->GetCount(); ip++ ) {
                    ODPath *pp = (ODPath *) ppath_array->Item( ip );
                    if( pp->IsVisible() ) {
                        bop_viz = true;
                        break;
                    }
                }
                if( !bop_viz )                          // is not visible as part of path
                    bop_viz = pop->IsVisible();         //  so treat as isolated point
                    
            } else
                bop_viz = pop->IsVisible();               // isolated point
                
                if( ( NULL == pFirstVizPoint ) && bop_viz ) pFirstVizPoint = pop;
                
                // Use path array to choose the appropriate path
                // Give preference to any active path, otherwise select the first visible path in the array for this point
                if( ppath_array ) {
                    for( unsigned int ip = 0; ip < ppath_array->GetCount(); ip++ ) {
                        ODPath *pp = (ODPath *) ppath_array->Item( ip );
                        if( pp->m_bPathIsActive ) {
                            pSelectedActivePath = pp;
                            pFoundActiveODPoint = pop;
                            break;
                        }
                    }
                    
                    if( NULL == pSelectedVizPath ) {
                        for( unsigned int ip = 0; ip < ppath_array->GetCount(); ip++ ) {
                            ODPath *pp = (ODPath *) ppath_array->Item( ip );
                            if( pp->IsVisible() ) {
                                pSelectedVizPath = pp;
                                pFoundVizODPoint = pop;
                                break;
                            }
                        }
                    }
                    
                    delete ppath_array;
                }
                
                node = node->GetNext();
        }
        
        //      Now choose the "best" selections
        if( pFoundActiveODPoint ) {
            m_pFoundODPoint = pFoundActiveODPoint;
            m_pSelectedPath = pSelectedActivePath;
        } else if( pFoundVizODPoint ) {
            m_pFoundODPoint = pFoundVizODPoint;
            m_pSelectedPath = pSelectedVizPath;
        } else
            // default is first visible point in list
            m_pFoundODPoint = pFirstVizPoint;
        
        if ( m_pSelectedPath ) {
            if ( m_pSelectedPath->IsVisible() )
                m_seltype |= SELTYPE_OCPNPOINT;
        } else if( m_pFoundODPoint ) m_seltype |= SELTYPE_OCPNPOINT;
        
        if( m_pFoundODPoint ) m_pFoundODPoint->m_bPtIsSelected = true;
    }
    
    if( pFindPathSeg )                  // there is at least one select item
    {
        SelectableItemList SelList = g_pODSelect->FindSelectionList( slat, slon,
                                                                     SELTYPE_PATHSEGMENT );
        
        if( NULL == m_pSelectedPath )  // the case where a segment only is selected
        {
            //  Choose the first visible path containing segment in the list
            wxSelectableItemListNode *node = SelList.GetFirst();
            while( node ) {
                SelectItem *pFindSel = node->GetData();
                
                ODPath *pp = (ODPath *) pFindSel->m_pData3;
                if( pp->IsVisible() ) {
                    m_pSelectedPath = pp;
                    break;
                }
                node = node->GetNext();
            }
        }
        
        if( m_pSelectedPath ) {
            if( NULL == m_pFoundODPoint ) m_pFoundODPoint =
                (ODPoint *) pFindPathSeg->m_pData1;
            m_pFoundODPointSecond = (ODPoint *) pFindPathSeg->m_pData2;
            
            m_pSelectedPath->m_bPathIsSelected = !(m_seltype & SELTYPE_OCPNPOINT);
            //                    if( m_pSelectedPath->m_bPathIsSelected ) {
            //                        m_pSelectedPath->m_iBlink++;
            //m_pSelectedPath->Draw( *g_pDC, *g_pivp );
            //                    }
            m_seltype |= SELTYPE_PATHSEGMENT;
        }
        
    }
    
}

void ocpn_draw_pi::SetCursorLatLon(double lat, double lon)
{
    g_dLat = lat;
    g_dLon = lon;
    
    m_cursor_lat = lat;
    m_cursor_lon = lon;
    if( g_ODEventHandler ) g_ODEventHandler->SetLatLon( lat, lon );
}

wxString ocpn_draw_pi::FormatDistanceAdaptive( double distance ) 
{
    wxString result;
    wxString *sUnit = new wxString( getUsrDistanceUnit_Plugin( -1 ) );
    double usrDistance = toUsrDistance_Plugin( distance, -1 );
    if( usrDistance < 0.1 &&  ( sUnit->IsSameAs( wxS("km") ) || sUnit->IsSameAs( wxS("mi") ) || sUnit->IsSameAs( wxS("NMi") ) ) ) {
        if ( sUnit->IsSameAs(wxS("mi")) ) sUnit->assign(wxS("ft"));
        else sUnit->assign(wxS("M"));
        usrDistance = toUsrDistance_Plugin( distance, -1 );
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

bool ocpn_draw_pi::RenderOverlay(wxMemoryDC *pmdc, PlugIn_ViewPort *pivp)
{
    m_vp = pivp;
    g_pivp = pivp;
    m_chart_scale = pivp->chart_scale;
    m_view_scale = pivp->view_scale_ppm;
    
    ODDC ocpnmdc( *pmdc );
    
    RenderPathLegs( ocpnmdc );
    return TRUE;
}

bool ocpn_draw_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *pivp)
{
    m_vp = pivp;
    g_pivp = pivp;
    m_chart_scale = pivp->chart_scale;
    m_view_scale = pivp->view_scale_ppm;
    
    g_pDC = new ODDC( dc );
    LLBBox llbb;
    llbb.SetMin( pivp->lon_min, pivp->lat_min );
    llbb.SetMax( pivp->lon_max, pivp->lat_max );
    
    DrawAllPathsInBBox( *g_pDC, llbb );
    DrawAllODPointsInBBox( *g_pDC, llbb );
    RenderPathLegs( *g_pDC );
    
    return TRUE;
}

bool ocpn_draw_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *pivp)
{
    m_pcontext = pcontext;
    m_vp = pivp;
    g_pivp = pivp;
    m_chart_scale = pivp->chart_scale;
    m_view_scale = pivp->view_scale_ppm;
    
    g_pDC = new ODDC();
    LLBBox llbb;
    llbb.SetMin( pivp->lon_min, pivp->lat_min );
    llbb.SetMax( pivp->lon_max, pivp->lat_max );
    
    //    DrawAllODPointsInBBox( *g_pDC, llbb );
    RenderPathLegs( *g_pDC );
    
    if (m_pMouseBoundary) m_pMouseBoundary->DrawGL( *pivp );
    
    DrawAllPathsAndODPoints( *pivp );

    if( g_pPathRolloverWin && g_pPathRolloverWin->IsActive() && g_pPathRolloverWin->GetBitmap() != NULL ) {
        g_pDC->DrawBitmap( *(g_pPathRolloverWin->GetBitmap()),
                       g_pPathRolloverWin->GetPosition().x,
                       g_pPathRolloverWin->GetPosition().y, false );
    }
    
    return TRUE;
}

void ocpn_draw_pi::RenderPathLegs( ODDC &dc ) 
{
    ODDC tdc( dc );
    
    if( nBoundary_State >= 2) {
        
        Boundary* boundary = 0;
        boundary = m_pMouseBoundary;
        state = nBoundary_State;
        
        if(!boundary)
            return;
        
        double rhumbBearing, rhumbDist, gcBearing, gcBearing2, gcDist;
        DistanceBearingMercator_Plugin( m_cursor_lat, m_cursor_lon, m_prev_rlat, m_prev_rlon, &rhumbBearing, &rhumbDist );
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
        boundary->DrawPointWhich( tdc, boundary->m_lastMousePointIndex, &lastPoint );
        if( boundary->m_NextLegGreatCircle ) {
            for( int i=1; i<=milesDiff; i++ ) {
                double p = (double)i * (1.0/(double)milesDiff);
                double pLat, pLon;
                Geodesic::GreatCircleTravel( m_prev_rlon, m_prev_rlat, gcDist*p, brg, &pLon, &pLat, &gcBearing2 );
                GetCanvasPixLL( m_vp, &destPoint, m_cursor_lat, m_cursor_lon);
                //destPoint = VPoint.GetPixFromLL( pLat, pLon );
                boundary->DrawSegment( tdc, &lastPoint, &destPoint, *m_vp, false );
                wxPoint rpn;
                boundary->GetPoint( 1 )->Draw( tdc, &rpn );
                boundary->DrawSegment( tdc, &rpn , &destPoint, *m_vp, false );
                lastPoint = destPoint;
            }
        }
        else {
            boundary->DrawSegment( tdc, &lastPoint, &r_rband, *m_vp, false );
            if ( nBoundary_State >= 2) { 
                wxPoint rpn;
                boundary->GetPoint( 1 )->Draw( tdc, &rpn );
                boundary->DrawSegment( tdc, &rpn , &r_rband, *m_vp, false );
            }
        }
        
        wxString info = CreateExtraPathLegInfo(tdc, boundary, brg, dist, m_cursorPoint);
        RenderExtraPathLegInfo( tdc, r_rband, info );
    } else if( nEBL_State > 0 || m_bEBLMoveOrigin ) {
        EBL *ebl = new EBL();
        double brg, dist;
        wxPoint tpoint;
        if(m_bEBLMoveOrigin) {
            ODPoint *tp = (ODPoint *) m_pSelectedEBL->m_pODPointList->GetLast()->GetData();
            GetCanvasPixLL( g_pivp, &tpoint, tp->m_lat, tp->m_lon );
            DistanceBearingMercator_Plugin( m_cursor_lat, m_cursor_lon, tp->m_lat, tp->m_lon, &brg, &dist );
            ebl->DrawSegment( tdc, &tpoint, &m_cursorPoint, *m_vp, false );
        } else {
            GetCanvasPixLL( g_pivp, &tpoint, g_pfFix.Lat, g_pfFix.Lon );
            DistanceBearingMercator_Plugin( m_cursor_lat, m_cursor_lon, g_pfFix.Lat, g_pfFix.Lon, &brg, &dist );
            ebl->DrawSegment( tdc, &tpoint, &m_cursorPoint, *m_vp, false );
        }
        wxString info = CreateExtraPathLegInfo(tdc, ebl, brg, dist, m_cursorPoint);
        if(info.length() > 0)
            RenderExtraPathLegInfo( tdc, m_cursorPoint, info );
        delete ebl;
    }
        
        
}

wxString ocpn_draw_pi::CreateExtraPathLegInfo(ODDC &dc, ODPath *path, double brg, double dist, wxPoint ref_point)
{
    wxString pathInfo;
    if( g_bShowMag )
        pathInfo << wxString::Format( wxString("%03d°(M)  ", wxConvUTF8 ), (int)GetTrueOrMag( brg ) );
    else
        pathInfo << wxString::Format( wxString("%03d°  ", wxConvUTF8 ), (int)GetTrueOrMag( brg ) );
    
    pathInfo << wxS(" ") << FormatDistanceAdaptive( dist );
    
    wxFont *dFont;
    dFont = OCPNGetFont( wxS("RouteLegInfoRollover"), 0 );
    dc.SetFont( *dFont );
    
    int w, h;
    int xp, yp;
    int hilite_offset = 3;
    #ifdef __WXMAC__
    wxScreenDC sdc;
    sdc.GetTextExtent( pathInfo, &w, &h, NULL, NULL, dFont );
    #else
    dc.GetTextExtent( pathInfo, &w, &h );
    #endif
    xp = ref_point.x - w;
    yp = ref_point.y;
    yp += hilite_offset;
    
    wxColour tColour;
    GetGlobalColor( wxS("YELO1"), &tColour );
    AlphaBlending( dc, xp, yp, w, h, 0.0, tColour, 172 );
    
    GetGlobalColor( wxS("UBLCK"), &tColour );
    dc.SetTextForeground( tColour );
    dc.SetPen( wxPen( tColour ) );
    dc.DrawText( pathInfo, xp, yp );
    
    wxString s0;
    if(path->m_sTypeString == wxT("Boundary")) {
        if ( nBoundary_State >= 2 ) {
            if( !path->m_bIsInLayer )
                s0.Append( _("Boundary: ") );
            else
                s0.Append( _("Layer Boundary: ") );
        }
        s0 += FormatDistanceAdaptive( path->m_path_length + dist );
    } 
    
    return s0;
}

void ocpn_draw_pi::RenderExtraPathLegInfo( ODDC &dc, wxPoint ref_point, wxString s )
{
    wxFont *dFont = OCPNGetFont( wxS("RouteLegInfoRollover"), 0 );
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
    
    wxColour tColour;
    GetGlobalColor( wxS("YELO1"), &tColour );
    AlphaBlending( dc, xp, yp, w, h, 0.0, tColour, 172 );
    
    GetGlobalColor( wxS("UBLCK"), &tColour );
    dc.SetPen( wxPen( tColour ) );
    dc.DrawText( s, xp, yp );
}

void ocpn_draw_pi::SetCurrentViewPort(PlugIn_ViewPort &vp)
{
    m_vp = &vp;
    g_pivp = &vp;
}

void ocpn_draw_pi::FinishBoundary( void )
{
    nBoundary_State = 0;
    m_prev_pMousePoint = NULL;
    
    //SetToolbarItemState( m_draw_button_id, false );
    m_bDrawingBoundary = false;
    
    if ( m_pMouseBoundary && m_pMouseBoundary->GetnPoints() > 1 && m_pMouseBoundary->m_pLastAddedPoint != m_pMouseBoundary->m_pFirstAddedPoint ) {
        g_pODSelect->AddSelectablePathSegment(m_prev_rlat, m_prev_rlon, m_dStartLat, m_dStartLon, m_pMouseBoundary->m_pLastAddedPoint, m_pMouseBoundary->m_pFirstAddedPoint, m_pMouseBoundary );
        m_pMouseBoundary->AddPoint( m_pMouseBoundary->m_pFirstAddedPoint );
        m_pMouseBoundary->m_lastMousePointIndex = m_pMouseBoundary->GetnPoints();
    }
    
    if( m_pMouseBoundary ) {
        if( m_pMouseBoundary->GetnPoints() > 1 ) {
            g_pODConfig->AddNewPath( m_pMouseBoundary, -1 );    // use auto next num
        } else {
            g_pPathMan->DeletePath( m_pMouseBoundary );
            m_pMouseBoundary = NULL;
        }
        
        if( m_pMouseBoundary ){
            m_pMouseBoundary->RebuildGUIDList(); // ensure the GUID list is intact and good
            m_pMouseBoundary->SetHiLite(0);
            m_pMouseBoundary->m_bIsBeingCreated = false;
        }
        
        
        if( g_pODPathPropDialog && ( g_pODPathPropDialog->IsShown() ) ) {
            g_pODPathPropDialog->SetPathAndUpdate( m_pMouseBoundary, true );
        }
        
        if( g_pPathManagerDialog && g_pPathManagerDialog->IsShown() )
            g_pPathManagerDialog->UpdatePathListCtrl();
    }
    
    m_pMouseBoundary = NULL;
    
    m_pSelectedPath = NULL;
    m_pSelectedBoundary = NULL;
    m_pSelectedEBL = NULL;
    m_pFoundODPointSecond = NULL;
    
    // TODO fix up undo
    //undo->InvalidateUndo();
    RequestRefresh( m_parent_window );
}

void ocpn_draw_pi::DrawAllPathsInBBox(ODDC &dc,  LLBBox& BltBBox)
{
    Boundary *active_boundary = NULL;
    
//    wxBoundaryListNode *pnode = (wxBoundaryListNode *)g_pPathList->GetFirst();
    wxBoundaryListNode *pnode = g_pBoundaryList->GetFirst();
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
                    pBoundaryDraw->Draw( dc, *m_vp );
            } else if( pBoundaryDraw->CrossesIDL() ) {
                wxPoint2DDouble xlate( -360., 0. );
                wxBoundingBox test_box1 = pBoundaryDraw->GetBBox();
                test_box1.Translate( xlate );
                if( b_run ) test_box1.Expand( m_lon, m_lat );
                
                if( !BltBBox.IntersectOut( test_box1 ) ) // Boundary is not wholly outside window
                {
                    b_drawn = true;
                    if( ( pBoundaryDraw != active_boundary ) ) pBoundaryDraw->Draw( dc, *m_vp );
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
                        if( ( pBoundaryDraw != active_boundary ) ) pBoundaryDraw->Draw( dc, *m_vp );
                    }
                } else if( !b_drawn && ( BltBBox.GetMinX() < 180. ) && ( BltBBox.GetMaxX() > 180. ) ) {
                    wxPoint2DDouble xlate( 360., 0. );
                    wxBoundingBox test_box3 = pBoundaryDraw->GetBBox();
                    test_box3.Translate( xlate );
                    if( !BltBBox.IntersectOut( test_box3 ) ) // Boundary is not wholly outside window
                    {
                        b_drawn = true;
                        if( ( pBoundaryDraw != active_boundary ) ) pBoundaryDraw->Draw( dc, *m_vp );
                    }
                }
            }
        }
        
        pnode = pnode->GetNext();
    }
    
    //  Draw any active or selected route, boundary or track last, so that is is always on top
    if( active_boundary ) active_boundary->Draw( dc, *m_vp );
    
    wxEBLListNode *pEBLNode = g_pEBLList->GetFirst();
    while( pEBLNode ) {
        bool b_drawn = false;
        EBL *pEBLDraw = pEBLNode->GetData();
        if( pEBLDraw ) {
            
            wxBoundingBox test_box = pEBLDraw->GetBBox();
            
            if( !BltBBox.IntersectOut( test_box ) ) // Boundary is not wholly outside window
            {
                b_drawn = true;
                pEBLDraw->Draw( dc, *m_vp );
            } else if( pEBLDraw->CrossesIDL() ) {
                wxPoint2DDouble xlate( -360., 0. );
                wxBoundingBox test_box1 = pEBLDraw->GetBBox();
                test_box1.Translate( xlate );
                
                if( !BltBBox.IntersectOut( test_box1 ) ) // Boundary is not wholly outside window
                {
                    b_drawn = true;
                    pEBLDraw->Draw( dc, *m_vp );
                }
            }
            
            //      Need to quick check for the case where VP crosses IDL
            if( !b_drawn ) {
                if( ( BltBBox.GetMinX() < -180. ) && ( BltBBox.GetMaxX() > -180. ) ) {
                    wxPoint2DDouble xlate( -360., 0. );
                    wxBoundingBox test_box2 = pEBLDraw->GetBBox();
                    test_box2.Translate( xlate );
                    if( !BltBBox.IntersectOut( test_box2 ) ) // Boundary is not wholly outside window
                    {
                        b_drawn = true;
                        pEBLDraw->Draw( dc, *m_vp );
                    }
                } else if( !b_drawn && ( BltBBox.GetMinX() < 180. ) && ( BltBBox.GetMaxX() > 180. ) ) {
                    wxPoint2DDouble xlate( 360., 0. );
                    wxBoundingBox test_box3 = pEBLDraw->GetBBox();
                    test_box3.Translate( xlate );
                    if( !BltBBox.IntersectOut( test_box3 ) ) // Boundary is not wholly outside window
                    {
                        b_drawn = true;
                        pEBLDraw->Draw( dc, *m_vp );
                    }
                }
            }
            if(pEBLDraw == m_pSelectedEBL && m_bODPointEditing) {
                double brg, dist;
                wxPoint destPoint;
                ODPoint *pStartPoint = m_pSelectedEBL->m_pODPointList->GetFirst()->GetData();
                ODPoint *pEndPoint = m_pSelectedEBL->m_pODPointList->GetLast()->GetData();
                DistanceBearingMercator_Plugin( pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &dist );
                GetCanvasPixLL( m_vp, &destPoint, pEndPoint->m_lat, pEndPoint->m_lon);
                wxString info = CreateExtraPathLegInfo(dc, m_pSelectedEBL, brg, dist, destPoint);
                if(info.length() > 0)
                    RenderExtraPathLegInfo( dc, destPoint, info );
            }
        }
        
        pEBLNode = pEBLNode->GetNext();
    }
    
}

void ocpn_draw_pi::DrawAllODPointsInBBox( ODDC& dc, LLBBox& BltBBox )
{
    //        wxBoundingBox bbx;
    if(!g_pODPointMan)
        return;
    
    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();
    
    while( node ) {
        ODPoint *pOP = node->GetData();
        if(node->GetData()->m_sTypeString == "Boundary Point")
            pOP = (BoundaryPoint *)node->GetData();
        if( pOP ) {
            if( pOP->m_bIsInRoute || pOP->m_bIsInPath ) {
                node = node->GetNext();
                continue;
            } else {
                if( BltBBox.PointInBox( pOP->m_lon, pOP->m_lat, 0 ) ) pOP->Draw( dc, NULL );
            }
        }
        
        node = node->GetNext();
    }
}

bool ocpn_draw_pi::CreatePointLeftClick( wxMouseEvent &event )
{
    double rlat, rlon;
    
    rlat = m_cursor_lat;
    rlon = m_cursor_lon;
    
    //    Check to see if there is a nearby point which may be reused
//    ODPoint *pMousePoint = NULL;
    BoundaryPoint *pMousePoint = NULL;
    
    //    Calculate meaningful SelectRadius
    int nearby_sel_rad_pix = 8;
    //        double nearby_radius_meters = nearby_sel_rad_pix / m_true_scale_ppm;
    double nearby_radius_meters = nearby_sel_rad_pix / 1;
    
//    ODPoint *pNearbyPoint = g_pODPointMan->GetNearbyODPoint( rlat, rlon, nearby_radius_meters );
    BoundaryPoint *pNearbyPoint = (BoundaryPoint *)g_pODPointMan->GetNearbyODPoint( rlat, rlon, nearby_radius_meters );

    if( pNearbyPoint && ( pNearbyPoint != (BoundaryPoint *)m_prev_pMousePoint )
        && !pNearbyPoint->m_bIsInTrack && !pNearbyPoint->m_bIsInLayer )
    {
        int dlg_return;
        #ifndef __WXOSX__
        dlg_return = OCPNMessageBox_PlugIn( m_parent_window, _("Use nearby Point?"),
                                            _("OpenCPN Point Create"),
                                            (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
        #else
        dlg_return = wxID_YES;
        #endif
        if( dlg_return == wxID_YES ) {
            pMousePoint = pNearbyPoint;
            
            // Using existing OCPNpoint, so nothing to delete for undo.
            //if( nBoundary_State > 1 )
            // TODO fix up undo
            //undo->BeforeUndoableAction( Undo_AppendWaypoint, pMousePoint, Undo_HasParent, NULL );
            
            // check all other boundaries and routes to see if this point appears in any other route
            // If it appears in NO other route, then it should e considered an isolated mark
            if( !g_pPathMan->FindPathContainingODPoint( pMousePoint ) ) pMousePoint->m_bKeepXPath = true;
        }
    }
    
    if( NULL == pMousePoint ) {                 // need a new point
//        pMousePoint = new ODPoint( rlat, rlon, g_sODPointIconName, wxS(""), wxT("") );
        pMousePoint = new BoundaryPoint( rlat, rlon, g_sODPointIconName, wxS(""), wxT("") );
        pMousePoint->SetNameShown( false );
        pMousePoint->SetTypeString( wxS("Boundary Point") );
        pMousePoint->m_bIsolatedMark = TRUE;
        
        g_pODConfig->AddNewODPoint( pMousePoint, -1 );    // use auto next num
        g_pODSelect->AddSelectableODPoint( rlat, rlon, pMousePoint );
        
    }
    
    nPoint_State++;
    
    RequestRefresh( m_parent_window );
    
    return TRUE;
}

bool ocpn_draw_pi::CreateTextPointLeftClick( wxMouseEvent &event )
{
    double rlat, rlon;
    
    rlat = m_cursor_lat;
    rlon = m_cursor_lon;
    
    //    Check to see if there is a nearby point which may be reused
    TextPoint *pMousePoint = NULL;
    
    //    Calculate meaningful SelectRadius
    int nearby_sel_rad_pix = 8;
    //        double nearby_radius_meters = nearby_sel_rad_pix / m_true_scale_ppm;
    double nearby_radius_meters = nearby_sel_rad_pix / 1;
    
    TextPoint *pNearbyPoint = (TextPoint *)g_pODPointMan->GetNearbyODPoint( rlat, rlon,
                                                                            nearby_radius_meters );
    if( pNearbyPoint && ( pNearbyPoint != m_prev_pMousePoint )
        && !pNearbyPoint->m_bIsInTrack && !pNearbyPoint->m_bIsInLayer )
    {
        int dlg_return;
        #ifndef __WXOSX__
        dlg_return = OCPNMessageBox_PlugIn( m_parent_window, _("Use nearby Pointoint?"),
                                            _("OpenCPN Point Create"),
                                            (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
        #else
        dlg_return = wxID_YES;
        #endif
        if( dlg_return == wxID_YES ) {
            pMousePoint = pNearbyPoint;
            
            // Using existing OCPNpoint, so nothing to delete for undo.
            //if( nBoundary_State > 1 )
            // TODO fix up undo
            //undo->BeforeUndoableAction( Undo_AppendWaypoint, pMousePoint, Undo_HasParent, NULL );
            
            // check all other boundaries and routes to see if this point appears in any other route
            // If it appears in NO other route, then it should e considered an isolated mark
            if( !g_pPathMan->FindPathContainingODPoint( pMousePoint ) ) pMousePoint->m_bKeepXPath = true;
        }
    }
    
    if( NULL == pMousePoint ) {                 // need a new point
        pMousePoint = new TextPoint( rlat, rlon, g_sODPointIconName, wxS(""), wxT("") );
        pMousePoint->SetNameShown( false );
        pMousePoint->m_bIsolatedMark = TRUE;
        
        g_pODConfig->AddNewODPoint( pMousePoint, -1 );    // use auto next num
        g_pODSelect->AddSelectableODPoint( rlat, rlon, pMousePoint );
        
    }
    
    nTextPoint_State++;
    
    RequestRefresh( m_parent_window );
    
    return TRUE;
}

bool ocpn_draw_pi::CreateBoundaryLeftClick( wxMouseEvent &event )
{
    //Process boundary creation
    double rlat, rlon;
    
    rlat = m_cursor_lat;
    rlon = m_cursor_lon;
    
    m_bBoundaryEditing = true;
    
    if( nBoundary_State == 1 ) {
        m_pMouseBoundary = new Boundary();
        g_pBoundaryList->Append( m_pMouseBoundary );
        g_pPathList->Append( m_pMouseBoundary);
        m_pMouseBoundary->m_width = g_BoundaryLineWidth;
        m_pMouseBoundary->m_style = g_BoundaryLineStyle;
        r_rband.x = g_cursor_x;
        r_rband.y = g_cursor_y;
        m_dStartLat = m_cursor_lat;
        m_dStartLon = m_cursor_lon;
    }
    
    //    Check to see if there is a nearby point which may be reused
    BoundaryPoint *pMousePoint = NULL;
    
    //    Calculate meaningful SelectRadius
    int nearby_sel_rad_pix = 8;
    //        double nearby_radius_meters = nearby_sel_rad_pix / m_true_scale_ppm;
    double nearby_radius_meters = nearby_sel_rad_pix / 1;
    
    BoundaryPoint *pNearbyPoint = (BoundaryPoint *)g_pODPointMan->GetNearbyODPoint( rlat, rlon,
                                                             nearby_radius_meters );
    if( pNearbyPoint && ( pNearbyPoint != m_prev_pMousePoint )
        && !pNearbyPoint->m_bIsInTrack && !pNearbyPoint->m_bIsInLayer )
    {
        int dlg_return;
        #ifndef __WXOSX__
        dlg_return = OCPNMessageBox_PlugIn( m_parent_window, _("Use nearby waypoint?"),
                                            _("OpenCPN Boundary Create"),
                                            (long) wxYES_NO | wxCANCEL | wxYES_DEFAULT );
        #else
        dlg_return = wxID_YES;
        #endif
        if( dlg_return == wxID_YES ) {
            pMousePoint = pNearbyPoint;
            
            // Using existing OCPNpoint, so nothing to delete for undo.
            //if( nBoundary_State > 1 )
            // TODO fix up undo
            //undo->BeforeUndoableAction( Undo_AppendWaypoint, pMousePoint, Undo_HasParent, NULL );
            
            // check all other boundaries and routes to see if this point appears in any other route
            // If it appears in NO other route, then it should e considered an isolated mark
            if( !g_pPathMan->FindPathContainingODPoint( pMousePoint ) ) pMousePoint->m_bKeepXPath = true;
        }
    }
    
    if( NULL == pMousePoint ) {                 // need a new point
        pMousePoint = new BoundaryPoint( rlat, rlon, g_sODPointIconName, wxS(""), wxT("") );
        pMousePoint->SetNameShown( false );
        pMousePoint->SetTypeString( wxS("Boundary Point") );
        
        g_pODConfig->AddNewODPoint( pMousePoint, -1 );    // use auto next num
        g_pODSelect->AddSelectableODPoint( rlat, rlon, pMousePoint );
        
        //if( nBoundary_State > 1 )
        // TODO fix up undo
        //undo->BeforeUndoableAction( Undo_AppendWaypoint, pMousePoint, Undo_IsOrphanded, NULL );
    }
    
    if(m_pMouseBoundary){
        if( nBoundary_State == 1 ) {
            // First point in the Boundary.
            m_pMouseBoundary->AddPoint( pMousePoint );
        } else {
            if( m_pMouseBoundary->m_NextLegGreatCircle ) {
                double rhumbBearing, rhumbDist, gcBearing, gcDist;
                DistanceBearingMercator_Plugin( rlat, rlon, m_prev_rlat, m_prev_rlon, &rhumbBearing, &rhumbDist );
                Geodesic::GreatCircleDistBear( m_prev_rlon, m_prev_rlat, rlon, rlat, &gcDist, &gcBearing, NULL );
                double gcDistNM = gcDist / 1852.0;
                
                // Empirically found expression to get reasonable route segments.
                int segmentCount = (3.0 + (rhumbDist - gcDistNM)) / pow(rhumbDist-gcDistNM-1, 0.5 );
                
                wxString msg;
                msg << _("For this leg the Great Circle boundary is ")
                << FormatDistanceAdaptive( rhumbDist - gcDistNM ) << _(" shorter than rhumbline.\n\n")
                << _("Would you like include the Great Circle boundary points for this leg?");
                
                m_disable_edge_pan = true;  // This helps on OS X if MessageBox does not fully capture mouse
                
                int answer = OCPNMessageBox_PlugIn( m_parent_window, msg, _("OpenCPN Boundary Create"), wxYES_NO | wxNO_DEFAULT );
                
                m_disable_edge_pan = false;
                
                if( answer == wxID_YES ) {
                    BoundaryPoint* gcPoint;
                    BoundaryPoint* prevGcPoint = (BoundaryPoint *)m_prev_pMousePoint;
                    wxRealPoint gcCoord;
                    
                    for( int i = 1; i <= segmentCount; i++ ) {
                        double fraction = (double) i * ( 1.0 / (double) segmentCount );
                        Geodesic::GreatCircleTravel( m_prev_rlon, m_prev_rlat, gcDist * fraction,
                                                     gcBearing, &gcCoord.x, &gcCoord.y, NULL );
                        
                        if( i < segmentCount ) {
                            gcPoint = new BoundaryPoint( gcCoord.y, gcCoord.x, wxS("xmblue"), wxS(""),
                                                   wxT("") );
                            gcPoint->SetNameShown( false );
                            gcPoint->SetTypeString( wxS("Boundary Point") );
                            g_pODConfig->AddNewODPoint( gcPoint, -1 );
                            g_pODSelect->AddSelectableODPoint( gcCoord.y, gcCoord.x, gcPoint );
                        } else {
                            gcPoint = pMousePoint; // Last point, previously exsisting!
                        }
                        
                        m_pMouseBoundary->AddPoint( gcPoint );
                        g_pODSelect->AddSelectablePathSegment( prevGcPoint->m_lat, prevGcPoint->m_lon,
                                                               gcPoint->m_lat, gcPoint->m_lon, prevGcPoint, gcPoint, m_pMouseBoundary );
                        prevGcPoint = gcPoint;
                    }
                    
                    // TODO fix up undo
                    //undo->CancelUndoableAction( true );
                    
                } else {
                    m_pMouseBoundary->AddPoint( pMousePoint );
                    g_pODSelect->AddSelectablePathSegment( m_prev_rlat, m_prev_rlon,
                                                           rlat, rlon, m_prev_pMousePoint, pMousePoint, m_pMouseBoundary );
                    //TODO fix up undo
                    //undo->AfterUndoableAction( m_pMouseBoundary );
                }
            } else {
                // Ordinary rhumblinesegment.
                m_pMouseBoundary->AddPoint( pMousePoint );
                g_pODSelect->AddSelectablePathSegment( m_prev_rlat, m_prev_rlon,
                                                       rlat, rlon, m_prev_pMousePoint, pMousePoint, m_pMouseBoundary );
                // TODO fix up undo
                //undo->AfterUndoableAction( m_pMouseBoundary );
            }
        }
    }
    
    m_prev_rlat = rlat;
    m_prev_rlon = rlon;
    m_prev_pMousePoint = pMousePoint;
    if(m_pMouseBoundary)
        m_pMouseBoundary->m_lastMousePointIndex = m_pMouseBoundary->GetnPoints();
    
    nBoundary_State++;
    RequestRefresh( m_parent_window );
    
    return TRUE;
} 

bool ocpn_draw_pi::CreateEBLLeftClick( wxMouseEvent &event )
{
    ODPoint *pMousePoint = NULL;
    double rlat, rlon;
    
    rlat = m_cursor_lat;
    rlon = m_cursor_lon;
    
    m_pMouseEBL = new EBL();
    g_pEBLList->Append( m_pMouseEBL );
    g_pPathList->Append( m_pMouseEBL );
    m_pMouseEBL->m_PathNameString << g_pEBLList->GetCount();
    m_dStartLat = g_pfFix.Lat;
    m_dStartLon = g_pfFix.Lon;

    ODPoint *beginPoint = new ODPoint( g_pfFix.Lat, g_pfFix.Lon, wxEmptyString, wxS("Boat"), wxT("") );
    beginPoint->SetNameShown( false );
    beginPoint->SetTypeString( wxT("EBL Point"));
    beginPoint->m_bIsolatedMark = false;
    m_pMouseEBL->AddPoint( beginPoint, false );
    
    pMousePoint = new ODPoint( rlat, rlon, g_sEBLEndIconName, wxS("End"), wxT("") );
    
    pMousePoint->SetNameShown( false );
    pMousePoint->SetTypeString( wxS("EBL Point") );
    pMousePoint->m_bIsolatedMark = FALSE;
    m_pMouseEBL->AddPoint( pMousePoint );
    m_pMouseEBL->m_bCentreOnBoat = true;

    if(m_pMouseEBL->m_PersistenceType == ID_EBL_PERSISTENT || m_pMouseEBL->m_PersistenceType == ID_EBL_PERSISTENT_CRASH)
        g_pODConfig->AddNewPath( m_pMouseEBL, -1 );    // don't save over restart
    g_pODSelect->AddSelectableODPoint( rlat, rlon, pMousePoint );
    g_pODSelect->AddSelectablePathSegment( g_pfFix.Lat, g_pfFix.Lon, rlat, rlon, beginPoint, pMousePoint, m_pMouseEBL );
    
    m_pMouseEBL->RebuildGUIDList();
    
    nEBL_State++;
    
    RequestRefresh( m_parent_window );
    
    return TRUE;
}

void ocpn_draw_pi::OnTimer(wxTimerEvent& ev)
{
    ProcessTimerEvent( ev );
}

void ocpn_draw_pi::ProcessTimerEvent(wxTimerEvent& ev)
{
    //if(popUp())
    //    plogbook_pi->m_plogbook_window->logbook->appendRow(true, true);
}

void ocpn_draw_pi::appendOSDirSlash(wxString* pString)
{
    wxChar sep = wxFileName::GetPathSeparator();
    
    if (pString->Last() != sep)
        pString->Append(sep);
}

void ocpn_draw_pi::DrawAllPathsAndODPoints( PlugIn_ViewPort &pivp )
{
    for(wxPathListNode *node = g_pPathList->GetFirst(); node; node = node->GetNext() ) {
        ODPath *pPathDraw = node->GetData();
        if( !pPathDraw )
            continue;
        
        /* defer rendering active routes until later */ 
        //if( pPathDraw->IsActive() || pPathDraw->IsSelected() )
        //            continue;
        
        /* this routine is called very often, so rather than using the
        *       wxBoundingBox::Intersect routine, do the comparisons directly
        *       to reduce the number of floating point comparisons */
        
        //        const wxBoundingBox &vp_box = vp.GetBBox(), &test_box = pPathDraw->GetBBox();
        const wxBoundingBox &test_box = pPathDraw->GetBBox();
        double test_miny = test_box.GetMinY(), test_maxy = test_box.GetMaxY();
        
        if(test_maxy < pivp.lat_min)
            continue;
        
        if(test_miny > pivp.lat_max)
            continue;
        
        double test_minx = test_box.GetMinX(), test_maxx = test_box.GetMaxX();
        
        // is not wholly outside viewport
        if(test_maxx >= pivp.lon_min && test_minx <= pivp.lon_max) {
            pPathDraw->DrawGL( pivp );
        } else if( pivp.lat_max > 180. ) {
            if(test_minx + 360 <= pivp.lon_max && test_maxx + 360 >= pivp.lon_min)
                pPathDraw->DrawGL( pivp );
        } else if( pPathDraw->CrossesIDL() || pivp.lon_min < -180. ) {
            if(test_maxx - 360 >= pivp.lon_min && test_minx - 360 <= pivp.lon_max)
                pPathDraw->DrawGL( pivp );
        }
        if(pPathDraw == m_pSelectedEBL && m_bODPointEditing) {
            ODDC dc;
            double brg, dist;
            wxPoint destPoint;
            ODPoint *pStartPoint = m_pSelectedEBL->m_pODPointList->GetFirst()->GetData();
            ODPoint *pEndPoint = m_pSelectedEBL->m_pODPointList->GetLast()->GetData();
            DistanceBearingMercator_Plugin( pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &dist );
            GetCanvasPixLL( m_vp, &destPoint, pEndPoint->m_lat, pEndPoint->m_lon);
            wxString info = CreateExtraPathLegInfo(dc, m_pSelectedEBL, brg, dist, destPoint);
            if(info.length() > 0)
                RenderExtraPathLegInfo( dc, destPoint, info );
        }
        
    }
        
    /* ODPoints not drawn as part of routes */
    ViewPort vp = (ViewPort &)pivp;
    if( pivp.bValid && g_pODPointList ) {
        for(wxODPointListNode *pnode = g_pODPointMan->GetODPointList()->GetFirst(); pnode; pnode = pnode->GetNext() ) {
            ODPoint *pOP = pnode->GetData();
            if( ( pOP->m_lon >= pivp.lon_min && pOP->m_lon <= pivp.lon_max ) && ( pOP->m_lat >= pivp.lat_min && pOP->m_lat <= pivp.lat_max ) )
                pOP->DrawGL( pivp );
        }
    }
        
}

/* render a rectangle at a given color and transparency */
void ocpn_draw_pi::AlphaBlending( ODDC &dc, int x, int y, int size_x, int size_y, float radius, wxColour color,
                                  unsigned char transparency )
{
    wxDC *pdc = dc.GetDC();
    if( pdc ) {
        //    Get wxImage of area of interest
        wxBitmap obm( size_x, size_y );
        wxMemoryDC mdc1;
        mdc1.SelectObject( obm );
        mdc1.Blit( 0, 0, size_x, size_y, pdc, x, y );
        mdc1.SelectObject( wxNullBitmap );
        wxImage oim = obm.ConvertToImage();
        
        //    Create destination image
        wxBitmap olbm( size_x, size_y );
        wxMemoryDC oldc( olbm );
        if(!oldc.IsOk())
            return;
        
        oldc.SetBackground( *wxBLACK_BRUSH );
        oldc.SetBrush( *wxWHITE_BRUSH );
        oldc.Clear();
        
        if( radius > 0.0 )
            oldc.DrawRoundedRectangle( 0, 0, size_x, size_y, radius );
        
        wxImage dest = olbm.ConvertToImage();
        unsigned char *dest_data = (unsigned char *) malloc(
        size_x * size_y * 3 * sizeof(unsigned char) );
        unsigned char *bg = oim.GetData();
        unsigned char *box = dest.GetData();
        unsigned char *d = dest_data;
        
        //  Sometimes, on Windows, the destination image is corrupt...
        if(NULL == box)
            return;
        
        float alpha = 1.0 - (float)transparency / 255.0;
        int sb = size_x * size_y;
        for( int i = 0; i < sb; i++ ) {
            float a = alpha;
            if( *box == 0 && radius > 0.0 ) a = 1.0;
            int r = ( ( *bg++ ) * a ) + (1.0-a) * color.Red();
            *d++ = r; box++;
            int g = ( ( *bg++ ) * a ) + (1.0-a) * color.Green();
            *d++ = g; box++;
            int b = ( ( *bg++ ) * a ) + (1.0-a) * color.Blue();
            *d++ = b; box++;
        }
        
        dest.SetData( dest_data );
        
        //    Convert destination to bitmap and draw it
        wxBitmap dbm( dest );
        dc.DrawBitmap( dbm, x, y, false );
        
        // on MSW, the dc Bounding box is not updated on DrawBitmap() method.
        // Do it explicitely here for all platforms.
        dc.CalcBoundingBox( x, y );
        dc.CalcBoundingBox( x + size_x, y + size_y );
    } else {
        #ifdef ocpnUSE_GL
        /* opengl version */
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
        if(radius > 1.0f){
            wxColour c(color.Red(), color.Green(), color.Blue(), transparency);
            dc.SetBrush(wxBrush(c));
            dc.DrawRoundedRectangle( x, y, size_x, size_y, radius );
        }
        else {
            glColor4ub( color.Red(), color.Green(), color.Blue(), transparency );
            glBegin( GL_QUADS );
            glVertex2i( x, y );
            glVertex2i( x + size_x, y );
            glVertex2i( x + size_x, y + size_y );
            glVertex2i( x, y + size_y );
            glEnd();
        }
        glDisable( GL_BLEND );
        #endif
    }
}

double ocpn_draw_pi::GetTrueOrMag(double a)
{
    if( g_bShowMag ){
        if(!wxIsNaN(g_dVar)){
            if((a - g_dVar) >360.)
                return (a - g_dVar - 360.);
            else
                return ((a - g_dVar) >= 0.) ? (a - g_dVar) : (a - g_dVar + 360.);
        }
        else{
            if((a - g_UserVar) >360.)
                return (a - g_UserVar - 360.);
            else
                return ((a - g_UserVar) >= 0.) ? (a - g_UserVar) : (a - g_UserVar + 360.);
        }
    }
    else
        return a;
}

void ocpn_draw_pi::DimeControl( wxWindow* ctrl )
{
    #ifdef __WXQT__
    return; // this is seriously broken on wxqt
    #endif
    
    if( NULL == ctrl ) return;
    
    wxColour col, window_back_color, gridline, uitext, udkrd, ctrl_back_color, text_color;
    GetGlobalColor( wxS("DILG0"), &col );
    GetGlobalColor( wxS("DILG1"), &window_back_color );
    GetGlobalColor( wxS("DILG1"), &ctrl_back_color );
    GetGlobalColor( wxS("DILG3"), &text_color );
    GetGlobalColor( wxS("UITX1"), &uitext );
    GetGlobalColor( wxS("UDKRD"), &udkrd );
    GetGlobalColor( wxS("GREY2"), &gridline );
    
    DimeControl( ctrl, col, window_back_color, ctrl_back_color, text_color, uitext, udkrd, gridline );
}

void ocpn_draw_pi::DimeControl( wxWindow* ctrl, wxColour col, wxColour window_back_color, wxColour ctrl_back_color,
                                wxColour text_color, wxColour uitext, wxColour udkrd, wxColour gridline )
{
    
    ColorScheme cs = ocpncc1->GetColorScheme();
    
    static int depth = 0; // recursion count
    if ( depth == 0 ) {   // only for the window root, not for every child
        
        // If the color scheme is DAY or RGB, use the default platform native colour for backgrounds
        if( cs == GLOBAL_COLOR_SCHEME_DAY || cs == GLOBAL_COLOR_SCHEME_RGB ) {
            #ifdef __WXOSX__
            window_back_color = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWFRAME);
            #else
            window_back_color = wxNullColour;
            #endif
            
            col = wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX);
        }
        
        ctrl->SetBackgroundColour( window_back_color );
    }
    
    wxWindowList kids = ctrl->GetChildren();
    for( unsigned int i = 0; i < kids.GetCount(); i++ ) {
        wxWindowListNode *node = kids.Item( i );
        wxWindow *win = node->GetData();
        
        if( win->IsKindOf( CLASSINFO(wxListBox) ) )
            ( (wxListBox*) win )->SetBackgroundColour( col );
        
        else if( win->IsKindOf( CLASSINFO(wxListCtrl) ) )
            ( (wxListCtrl*) win )->SetBackgroundColour( col );
        
        else if( win->IsKindOf( CLASSINFO(wxTextCtrl) ) )
            ( (wxTextCtrl*) win )->SetBackgroundColour( col );
        
        else if( win->IsKindOf( CLASSINFO(wxStaticText) ) )
            ( (wxStaticText*) win )->SetForegroundColour( uitext );
        
        #ifndef __WXOSX__
        // on OS X most controls can't be styled, and trying to do so only creates weird coloured boxes around them
        
        else if( win->IsKindOf( CLASSINFO(wxBitmapComboBox) ) )
            ( (wxBitmapComboBox*) win )->SetBackgroundColour( col );
        
        else if( win->IsKindOf( CLASSINFO(wxChoice) ) )
            ( (wxChoice*) win )->SetBackgroundColour( col );
        
        else if( win->IsKindOf( CLASSINFO(wxComboBox) ) )
            ( (wxComboBox*) win )->SetBackgroundColour( col );
        
        else if( win->IsKindOf( CLASSINFO(wxRadioButton) ) )
            ( (wxRadioButton*) win )->SetBackgroundColour( window_back_color );
        
        else if( win->IsKindOf( CLASSINFO(wxScrolledWindow) ) ) {
            if( cs != GLOBAL_COLOR_SCHEME_DAY && cs != GLOBAL_COLOR_SCHEME_RGB )
                ( (wxScrolledWindow*) win )->SetBackgroundColour( window_back_color );
        }
        #endif
        
        else if( win->IsKindOf( CLASSINFO(wxGenericDirCtrl) ) )
            ( (wxGenericDirCtrl*) win )->SetBackgroundColour( window_back_color );
        
        else if( win->IsKindOf( CLASSINFO(wxListbook) ) )
            ( (wxListbook*) win )->SetBackgroundColour( window_back_color );
        
        else if( win->IsKindOf( CLASSINFO(wxTreeCtrl) ) )
            ( (wxTreeCtrl*) win )->SetBackgroundColour( col );
        
        else if( win->IsKindOf( CLASSINFO(wxNotebook) ) ) {
            ( (wxNotebook*) win )->SetBackgroundColour( window_back_color );
            ( (wxNotebook*) win )->SetForegroundColour( text_color );
        }
        
        else if( win->IsKindOf( CLASSINFO(wxButton) ) ) {
            ( (wxButton*) win )->SetBackgroundColour( window_back_color );
        }
        
        else if( win->IsKindOf( CLASSINFO(wxToggleButton) ) ) {
            ( (wxToggleButton*) win )->SetBackgroundColour( window_back_color );
        }
        
        //        else if( win->IsKindOf( CLASSINFO(wxPanel) ) ) {
        ////                  ((wxPanel*)win)->SetBackgroundColour(col1);
        //            if( cs != GLOBAL_COLOR_SCHEME_DAY && cs != GLOBAL_COLOR_SCHEME_RGB )
        //                ( (wxPanel*) win )->SetBackgroundColour( ctrl_back_color );
        //            else
        //                ( (wxPanel*) win )->SetBackgroundColour( wxNullColour );
        //        }
        
        else if( win->IsKindOf( CLASSINFO(wxHtmlWindow) ) ) {
            if( cs != GLOBAL_COLOR_SCHEME_DAY && cs != GLOBAL_COLOR_SCHEME_RGB )
                ( (wxPanel*) win )->SetBackgroundColour( ctrl_back_color );
            else
                ( (wxPanel*) win )->SetBackgroundColour( wxNullColour );
        }
        
        else if( win->IsKindOf( CLASSINFO(wxGrid) ) ) {
            ( (wxGrid*) win )->SetDefaultCellBackgroundColour( window_back_color );
            ( (wxGrid*) win )->SetDefaultCellTextColour( uitext );
            ( (wxGrid*) win )->SetLabelBackgroundColour( col );
            ( (wxGrid*) win )->SetLabelTextColour( uitext );
            #if !wxCHECK_VERSION(3,0,0)
            ( (wxGrid*) win )->SetDividerPen( wxPen( col ) );
            #endif            
            ( (wxGrid*) win )->SetGridLineColour( gridline );
        }
        
        else {
            ;
        }
        
        if( win->GetChildren().GetCount() > 0 ) {
            depth++;
            wxWindow * w = win;
            DimeControl( w, col, window_back_color, ctrl_back_color, text_color, uitext, udkrd, gridline );
            depth--;
        }
    }
}

void ocpn_draw_pi::SetToolbarTool( void )
{
    if ( nBoundary_State == 1 || nPoint_State == 1 || nTextPoint_State == 1 || nEBL_State == 1 ) {
        if (m_Mode > m_numModes ) m_Mode = 0;
        switch (m_Mode)
        {
            case ID_MODE_BOUNDARY:
                // Boundary
                m_pCurrentCursor = ocpncc1->pCursorPencil;
                SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_boundary_gray, _img_ocpn_draw_boundary);
                SetToolbarItemState( m_draw_button_id, true );
                nBoundary_State = 1;
                nPoint_State = 0;
                nTextPoint_State = 0;
                nEBL_State = 0;
                break;
                
            case ID_MODE_POINT:
                // Point
                m_pCurrentCursor = ocpncc1->pCursorCross;
                SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_point_gray, _img_ocpn_draw_point);
                SetToolbarItemState( m_draw_button_id, true );
                nBoundary_State = 0;
                nPoint_State = 1;
                nTextPoint_State = 0;
                nEBL_State = 0;
                break;
                
            case ID_MODE_TEXT_POINT:
                // Text Point
                m_pCurrentCursor = m_pTextCursorCross;
                SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_textpoint, _img_ocpn_draw_textpoint_gray);
                SetToolbarItemState( m_draw_button_id, true );
                nPoint_State = 0;
                nBoundary_State = 0;
                nTextPoint_State = 1;
                nEBL_State = 0;
                break;
                
            case ID_MODE_EBL:
                // EBL
                m_pCurrentCursor = ocpncc1->pCursorCross;
                SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_ebl, _img_ocpn_draw_ebl_gray);
                SetToolbarItemState( m_draw_button_id, true );
                nPoint_State = 0;
                nBoundary_State = 0;
                nTextPoint_State = 0;
                nEBL_State = 1;
                RequestRefresh( m_parent_window );
                break;
                
            default:
                // Boundary
                m_Mode = ID_MODE_BOUNDARY;
                m_pCurrentCursor = ocpncc1->pCursorPencil;
                SetToolbarToolBitmaps(m_draw_button_id, _img_ocpn_draw_boundary_gray, _img_ocpn_draw_boundary);
                g_pODToolbar->SetToolbarTool( ID_BOUNDARY );
                SetToolbarItemState( m_draw_button_id, true );
                nBoundary_State = 1;
                nPoint_State = 0;
                nTextPoint_State = 0;
                nEBL_State = 0;
                break;
        }
    }
    
}

wxString GetBoundaryWithPointInBoundary( double lat, double lon )
{
    return g_pBoundaryMan->FindPointInBoundary( lat, lon );
}


bool FindPointInBoundary( double lat, double lon )
{
    if(g_pBoundaryMan->FindPointInBoundary( lat, lon ).length() > 0 ) return true;
    else return false;
}

bool FindPointInBoundary( Boundary *pBoundary, double lat, double lon )
{
    return g_pBoundaryMan->FindPointInBoundary( pBoundary, lat, lon );
}

