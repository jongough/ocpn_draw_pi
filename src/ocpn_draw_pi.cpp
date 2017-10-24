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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ocpn_draw_pi.h"
#include "Boundary.h"
#include "BoundaryMan.h"
#include "BoundaryPoint.h"
#include "BoundaryProp.h"
#include "DR.h"
#include "DRProp.h"
#include "EBL.h"
#include "EBLProp.h"
#include "GZ.h"
#include "PIL.h"
#include "GZMan.h"
#include "GZProp.h"
#include "PILProp.h"
#include "ODPath.h"
#include "PathMan.h"
#include "pathmanagerdialog.h"
#include "PointMan.h"
#include "ODAPI.h"
#include "ODConfig.h"
#include "ODdc.h"
#include "ODDRDialogImpl.h"
#include "ODEventHandler.h"
#include "ODPropertiesDialogImpl.h"
#include "ODicons.h"
#include "ODJSON.h"
#include "ODPoint.h"
#include "ODSelect.h"
#include "ODPointPropertiesImpl.h"
#include "ODToolbarImpl.h"
#include "ODUtils.h"
#include "version.h"
#include "SelectItem.h"
#include "TextPoint.h"
#include "PILPropertiesDialogImpl.h"

#include "chcanv.h"
#include "Layer.h"
#include "OCPNPlatform.h"
#include "pluginmanager.h"
#include "geodesic.h"
#include "IDX_entry.h"
#include <wx/stdpaths.h>
#include <wx/timer.h>
#include <wx/event.h>
#include <wx/sysopt.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/filefn.h>
#include <wx/msgdlg.h>
#include <wx/listbook.h>
#include <memory>

#include <wx/jsonreader.h>

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
GZMan                   *g_pGZMan;
wxString                g_default_ODPoint_icon;
ODPathPropertiesDialogImpl   *g_pODPathPropDialog;
ODPathPropertiesDialogImpl   *g_pPathPropDialog;
BoundaryProp            *g_pBoundaryPropDialog;
EBLProp                 *g_pEBLPropDialog;
DRProp                  *g_pDRPropDialog;
GZProp                  *g_pGZPropDialog;
PILProp                 *g_pPILPropDialog;
PathManagerDialog       *g_pPathManagerDialog;
ODPointPropertiesImpl   *g_pODPointPropDialog;
ODPropertiesDialogImpl  *g_pOCPNDrawPropDialog;
PILPropertiesDialogImpl *g_PILIndexLinePropDialog;
ODDRDialogImpl          *g_pODDRDialog;
PlugInManager           *g_OD_pi_manager;
BoundaryList            *g_pBoundaryList;
EBLList                 *g_pEBLList;
DRList                  *g_pDRList;
GZList                  *g_pGZList;
PILList                 *g_pPILList;
ODPointList             *g_pODPointList;
ChartCanvas             *ocpncc1;
ODPath                  *g_PathToEdit;
int                     g_PILToEdit;
ODRolloverWin           *g_pODRolloverWin;
SelectItem              *g_pRolloverPathSeg;
SelectItem              *g_pRolloverPoint;

wxColour    g_colourActiveBoundaryLineColour;
wxColour    g_colourInActiveBoundaryLineColour;
wxColour    g_colourActiveBoundaryFillColour;
wxColour    g_colourInActiveBoundaryFillColour;
bool        g_bExclusionBoundary;
bool        g_bInclusionBoundary;
bool        g_bExclusionBoundaryPoint;
bool        g_bInclusionBoundaryPoint;
bool        g_bBoundaryODPointsVisible;
unsigned int g_uiFillTransparency;
unsigned int g_uiBoundaryPointFillTransparency;
int         g_iBoundaryPointRangeRingLineWidth;
int         g_iBoundaryPointRangeRingLineStyle;
int         g_BoundaryLineWidth; 
int         g_BoundaryLineStyle;
bool        g_bBoundaryPointShowName;
int         g_iInclusionBoundarySize;
int         g_iInclusionBoundaryPointSize;
wxString    g_sEBLEndIconName;
wxString    g_sEBLStartIconName;
wxColour    g_colourEBLLineColour;
bool        g_bEBLFixedEndPosition;
int         g_iEBLPersistenceType;
bool        g_bEBLShowArrow;
bool        g_bEBLVRM;
bool        g_bEBLAlwaysShowInfo;
bool        g_bEBLPerpLine;
int         g_EBLLineWidth; 
int         g_EBLLineStyle;
bool        g_bEBLRotateWithBoat;
int         g_iEBLMaintainWith;

wxString    g_sPILEndIconName;
wxString    g_sPILStartIconName;
wxColour    g_colourPILActiveCentreLineColour;
wxColour    g_colourPILInActiveCentreLineColour;
wxColour    g_colourPILActiveOffsetLine1Colour;
wxColour    g_colourPILInActiveOffsetLine1Colour;
wxColour    g_colourPILActiveOffsetLine2Colour;
wxColour    g_colourPILInActiveOffsetLine2Colour;
int         g_PILCentreLineWidth;
int         g_PILCentreLineStyle;
int         g_PILOffsetLine1Width;
int         g_PILOffsetLine1Style;
int         g_PILOffsetLine2Width;
int         g_PILOffsetLine2Style;
double      g_dPILOffset;
int         g_PILDefaultNumIndexLines;
int         g_iPILPersistenceType;

wxString    g_sDRPointIconName;
wxColour    g_colourDRLineColour;
wxColour    g_colourInActiveDRLineColour;
bool        g_bDRShowArrow;
int         g_DRLineWidth;
int         g_DRLineStyle;
bool        g_bDRPointShowRangeRings;
int         g_iDRPointRangeRingsNumber;
float       g_fDRPointRangeRingsStep;
int         g_iDRPointRangeRingsStepUnits;
wxColour    g_colourDRPointRangeRingsColour;
int         g_iDRPointRangeRingLineWidth;
int         g_iDRPointRangeRingLineStyle;

wxColour     g_colourActiveGZLineColour;
wxColour     g_colourInActiveGZLineColour;
wxColour     g_colourActiveGZFillColour;
wxColour     g_colourInActiveGZFillColour;
int          g_GZLineWidth; 
int          g_GZLineStyle;
unsigned int g_uiGZFillTransparency;
bool         g_bGZRotateWithBoat;
int          g_iGZMaintainWith;
wxString    g_sGZFirstIconName;
wxString    g_sGZSecondIconName;
int         g_iGZPersistenceType;
int         g_iGZMaxNum;

wxColour    g_colourActivePathLineColour;
wxColour    g_colourInActivePathLineColour;
int         g_PathLineWidth; 
int         g_PathLineStyle;

wxString    *g_PrivateDataDir;

wxString    *g_pHome_Locn;
wxString    *g_pData;
wxString    *g_pLayerDir;

ODEventHandler   *g_ODEventHandler;

bool            g_bODPointShowName;
bool            g_bODPointShowRangeRings;
int             g_iODPointRangeRingsNumber;
float           g_fODPointRangeRingsStep;
int             g_iODPointRangeRingsStepUnits;
wxColour        g_colourODPointRangeRingsColour;
wxString        g_sODPointIconName;
wxString        g_sTextPointIconName;
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
int             g_iTextPointDisplayTextWhen;

PlugIn_ViewPort *g_pVP;
PlugIn_ViewPort g_VP;
ODDC            *g_pDC;
bool            g_bShowMag;
bool            g_bAllowLeftDrag;
double          g_dVar;
double          g_UserVar;
double          g_n_arrival_circle_radius;
wxRect          g_blink_rect;

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

wxString        *g_ODlocale;
int             g_iLocaleDepth;

int             g_click_stop;
bool            g_bConfirmObjectDelete;

ODToolbarImpl   *g_pODToolbar;
int             g_iToolbarPosX;
int             g_iToolbarPosY;
int             g_iDisplayToolbar;

double          g_dScale;
int             g_iNSScale;
float           g_ChartScaleFactorExp;

double          g_dDRSOG;
int             g_iDRCOG;
double          g_dDRLength;
double          g_dDRPointInterval;
int             g_iDRLengthType;
int             g_iDRIntervalType;
int             g_iDRDistanceUnits;
int             g_iDRTimeUnits;
int             g_iDRPersistenceType;

ODPlugIn_Position_Fix_Ex  g_pfFix;

ODJSON          *g_pODJSON;
ODAPI           *g_pODAPI;

wxImage ICursorLeft;
wxImage ICursorRight;
wxImage ICursorUp;
wxImage ICursorDown;
wxImage ICursorPencil;
wxImage ICursorCross;

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
    
    wxString *l_pDir = new wxString(*GetpPrivateApplicationDataLocation());
    appendOSDirSlash( l_pDir );
    l_pDir->Append(_T("plugins"));
    appendOSDirSlash( l_pDir );
    if ( !wxDir::Exists(*l_pDir))
        wxMkdir( *l_pDir );
    l_pDir->Append(_T("ocpn_draw_pi"));
    appendOSDirSlash( l_pDir );
    if ( !wxDir::Exists(*l_pDir))
        wxMkdir( *l_pDir );
    g_PrivateDataDir = new wxString;
    g_PrivateDataDir->Append(*l_pDir);
    g_pData = new wxString(*l_pDir);
    g_pData->append( wxS("data") );
    appendOSDirSlash( g_pData );
    if ( !wxDir::Exists(*g_pData))
        wxMkdir( *g_pData );
    g_pLayerDir = new wxString;
    g_pLayerDir->Append(*l_pDir);
    g_pLayerDir->Append( wxT("Layers") );
    appendOSDirSlash( g_pLayerDir );
    
    m_pODicons = new ODicons();
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
    m_pMouseBoundary = NULL;
    m_pEBLBoatPoint = NULL;
    m_bDrawingBoundary = false;
    m_pFoundODPoint = NULL;
    m_pSelectedPath = NULL;
    m_pSelectedBoundary = NULL;
    m_pMouseBoundary = NULL;
    m_pSelectedEBL = NULL;
    m_pSelectedGZ = NULL;
    m_pMouseEBL = NULL;
    m_pMousePIL = NULL;
    m_pSelectedPIL = NULL;
    g_dVar = NAN;
    nBoundary_State = 0;
    nPoint_State = 0;
    nTextPoint_State = 0;
    nPath_State = 0;
    nEBL_State = 0;
    nDR_State = 0;
    nGZ_State = 0;
    nPIL_State = 0;
    bKey_Path_Pressed = false;
    bKey_Boundary_Pressed = false;
    bKey_Point_Pressed = false;
    bKey_TextPoint_Pressed = false;
    bKey_EBL_Pressed = false;
    bKey_DR_Pressed = false;
    bKey_GZ_Pressed = false;
    bKey_PIL_Pressed = false;
    g_iGZMaxNum = 0;
    m_chart_scale = 0.;
    g_pfFix.valid = false;
    g_iLocaleDepth = 0;
    g_ODlocale = NULL;
    
    // Drawing modes from toolbar
    m_Mode = 0;
    m_numModes = ID_MODE_LAST - 1;
    
    // Adds local language support for the plugin to OCPN
    AddLocaleCatalog( PLUGIN_CATALOG_NAME );
    
    lastODPointInPath = wxS("-1");
    eventsEnabled = true;

    m_global_color_scheme = PI_GLOBAL_COLOR_SCHEME_DAY;
    
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

    g_pODJSON = new ODJSON;
    g_pODAPI = new ODAPI;
    g_pODPointList = new ODPointList;
    g_pBoundaryList = new BoundaryList;
    g_pEBLList = new EBLList;
    g_pDRList = new DRList;
    g_pGZList = new GZList;
    g_pPILList = new PILList;
    g_pPathList = new PathList;
    //    Layers
    pLayerList = new LayerList;
    
    if(m_bLOGShowIcon) {
#ifdef ODraw_USE_SVG
        m_config_button_id  = InsertPlugInToolSVG(_("OCPN Draw Manager"), m_pODicons->m_s_ocpn_draw_grey_pi, m_pODicons->m_s_ocpn_draw_pi, wxEmptyString, wxITEM_NORMAL,
                                               _("OCPN Draw Manager"), wxS(""), NULL, OCPN_DRAW_POSITION, 0, this);
        m_draw_button_id  = InsertPlugInToolSVG(_("OCPN Draw Boundary"), m_pODicons->m_s_ocpn_draw_boundary_grey, m_pODicons->m_s_ocpn_draw_boundary, wxEmptyString, wxITEM_CHECK,
                                             _("OCPN Draw"), wxS(""), NULL, OCPN_DRAW_POSITION, 0, this);
#else
        m_config_button_id  = InsertPlugInTool(_("OCPN Draw Manager"), m_pODicons->m_p_bm_ocpn_draw_grey_pi, m_pODicons->m_p_bm_ocpn_draw_pi, wxITEM_NORMAL,
                                               _("OCPN Draw Manager"), wxS(""), NULL, OCPN_DRAW_POSITION, 0, this);
        m_draw_button_id  = InsertPlugInTool(_("OCPN Draw Boundary"), m_pODicons->m_p_bm_ocpn_draw_boundary_grey, m_pODicons->m_p_bm_ocpn_draw_boundary, wxITEM_CHECK,
                                             _("OCPN Draw"), wxS(""), NULL, OCPN_DRAW_POSITION, 0, this);
#endif
    }
    
    // Set tool to show in tool bar
    switch (m_Mode)
    {
        case ID_MODE_BOUNDARY:
            // Boundary
#ifdef ODraw_USE_SVG
            SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_boundary_grey, m_pODicons->m_s_ocpn_draw_boundary, m_pODicons->m_s_ocpn_draw_boundary);
#else            
            SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_boundary_grey, m_pODicons->m_p_bm_ocpn_draw_boundary);
#endif            
            break;
            
        case ID_MODE_POINT:
            // Point
#ifdef ODraw_USE_SVG
            SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_point_grey, m_pODicons->m_s_ocpn_draw_point, m_pODicons->m_s_ocpn_draw_point);
#else            
            SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_point_grey, m_pODicons->m_p_bm_ocpn_draw_point);
#endif            
            break;
            
        case ID_MODE_TEXT_POINT:
            // Text Point
#ifdef ODraw_USE_SVG
            SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_textpoint_grey, m_pODicons->m_s_ocpn_draw_textpoint, m_pODicons->m_s_ocpn_draw_textpoint);
#else            
            SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_textpoint_grey, m_pODicons->m_p_bm_ocpn_draw_textpoint);
#endif            
            break;
            
        case ID_MODE_EBL:
            // EBL
#ifdef ODraw_USE_SVG
            SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_ebl_grey, m_pODicons->m_s_ocpn_draw_ebl, m_pODicons->m_s_ocpn_draw_ebl);
#else            
            SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_ebl_grey, m_pODicons->m_p_bm_ocpn_draw_ebl);
#endif            
            break;
            
        case ID_MODE_DR:
            // DR
#ifdef ODraw_USE_SVG
            SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_dr_grey, m_pODicons->m_s_ocpn_draw_dr, m_pODicons->m_s_ocpn_draw_dr);
#else            
            SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_dr_grey, m_pODicons->m_p_bm_ocpn_draw_dr);
#endif            
            break;
            
        case ID_MODE_GZ:
            // GZ
#ifdef ODraw_USE_SVG
            SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_gz_grey, m_pODicons->m_s_ocpn_draw_gz, m_pODicons->m_s_ocpn_draw_gz);
#else            
            SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_gz_grey, m_pODicons->m_p_bm_ocpn_draw_gz);
#endif            
            break;
            
        case ID_MODE_PIL:
            // PIL
            #ifdef ODraw_USE_SVG
            SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_pil_grey, m_pODicons->m_s_ocpn_draw_pil, m_pODicons->m_s_ocpn_draw_pil);
            #else
            SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_pil_grey, m_pODicons->m_p_bm_ocpn_draw_pil);
            #endif
            break;

        default:
            // Boundary
            m_Mode = ID_MODE_BOUNDARY;
#ifdef ODraw_USE_SVG
            SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_boundary_grey, m_pODicons->m_s_ocpn_draw_boundary, m_pODicons->m_s_ocpn_draw_boundary);
#else            
            SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_boundary_grey, m_pODicons->m_p_bm_ocpn_draw_boundary);
#endif            
            break;
    }
    
    // Create floating toolbar for drawing
#ifdef __WXOSX__
    g_pODToolbar = new ODToolbarImpl( m_parent_window, wxID_ANY, _("Draw Toolbar"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSTAY_ON_TOP );
#else
    g_pODToolbar = new ODToolbarImpl( m_parent_window, wxID_ANY, _("Draw Toolbar"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER );
#endif
    wxPoint wxpToolbarPos;
    wxpToolbarPos.x = g_iToolbarPosX;
    wxpToolbarPos.y = g_iToolbarPosY;
    g_pODToolbar->SetPosition( wxpToolbarPos );
    g_pODToolbar->Fit();
    g_pODToolbar->SetInitialSize();
    //g_pODToolbar->SetMaxSize(g_pODToolbar->GetSize());
    if( g_iToolbarPosX == 0 && g_iToolbarPosY == 0 ) g_pODToolbar->CenterOnParent();
    if( g_iDisplayToolbar == ID_DISPLAY_ALWAYS ) g_pODToolbar->Show();
    
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
    
    g_pODRolloverWin = new ODRolloverWin( m_parent_window );
    g_pRolloverPathSeg = NULL;
    g_pRolloverPoint = NULL;
    
    m_RolloverPopupTimer.SetOwner( ocpncc1, ODROPOPUP_TIMER );
    m_rollover_popup_timer_msec = 20;
    m_parent_window->Connect( m_RolloverPopupTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler( ODEventHandler::OnRolloverPopupTimerEvent ) );
    
    // Get item into font list in options/user interface
    AddPersistentFontKey( wxT("OD_PathLegInfoRollover") );
    AddPersistentFontKey( wxT("OD_PointInfoRollover") );
    wxFont *l_pfont = GetOCPNScaledFont_PlugIn( wxS("OD_PathLegInfoRollover") );
    wxColour l_fontcolout = GetFontColour_PlugIn( wxS("OD_PathLegInfoRollover") );
    l_pfont = GetOCPNScaledFont_PlugIn( wxS("OD_PointInfoRollover") );
    l_fontcolout = GetFontColour_PlugIn( wxS("OD_PointInfoRollover") );
    
    m_pCurrentCursor = NULL;
    
    //build_cursors(); // build cursors to use on chart
    m_pTextCursorCross = new wxCursor( wxCURSOR_CHAR );

    wxImage ICursorPencil = GetIcon_PlugIn(_T("pencil")).ConvertToImage();
    if ( ICursorPencil.Ok() )
    {
        ICursorPencil.SetOption ( wxIMAGE_OPTION_CUR_HOTSPOT_X, 0 );
        ICursorPencil.SetOption ( wxIMAGE_OPTION_CUR_HOTSPOT_Y, 16);
        m_pCursorPencil = new wxCursor ( ICursorPencil );
    }
    else
        m_pCursorPencil = new wxCursor ( wxCURSOR_ARROW );

    wxImage ICursorCross = GetIcon_PlugIn(_T("cross")).ConvertToImage();
    if ( ICursorCross.Ok() )
    {
        ICursorCross.SetOption ( wxIMAGE_OPTION_CUR_HOTSPOT_X, 13 );
        ICursorCross.SetOption ( wxIMAGE_OPTION_CUR_HOTSPOT_Y, 12);
        m_pCursorCross = new wxCursor ( ICursorCross );
    }
    else
        m_pCursorCross = new wxCursor ( wxCURSOR_ARROW );

    
    g_pODPointMan = new PointMan();
    g_pODPointMan->SetColorScheme( m_global_color_scheme );
    
    g_pPathMan = new PathMan();
    g_pPathMan->SetColorScheme( m_global_color_scheme );
    g_pBoundaryMan = new BoundaryMan();
    g_pGZMan = new GZMan();
    
    g_pODPathPropDialog = NULL;
    g_pBoundaryPropDialog = NULL;
    g_pEBLPropDialog = NULL;
    g_pDRPropDialog = NULL;
    g_pPILPropDialog = NULL;
    g_PILIndexLinePropDialog = NULL;
    
    g_pODConfig->LoadNavObjects();
    
    // Import Layer-wise any .gpx files from /Layers directory
    if( wxDir::Exists( *g_pLayerDir ) ) {
        wxString laymsg;
        laymsg.Printf( wxT("Getting .gpx layer files from: %s"), g_pLayerDir->c_str() );
        wxLogMessage( laymsg );
        
        g_pODConfig->LoadLayers(*g_pLayerDir);
    }
    
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
    m_parent_window->Disconnect( m_RolloverPopupTimer.GetId(), wxTimerEventHandler( ODEventHandler::OnRolloverPopupTimerEvent ) );
    if( g_ODEventHandler ) delete g_ODEventHandler;
    g_ODEventHandler = NULL;
    if( g_pODRolloverWin ) g_pODRolloverWin->Destroy();
    g_pODRolloverWin = NULL;
    if( g_pODPointPropDialog ) delete g_pODPointPropDialog;
    g_pODPointPropDialog = NULL;
    if( g_pODPathPropDialog ) delete g_pODPathPropDialog;
    g_pODPathPropDialog = NULL;
    if( g_pODToolbar ) g_pODToolbar->Destroy();
    g_pODToolbar = NULL;
    if( g_pODJSON ) delete g_pODJSON;
    g_pODJSON = NULL;
    if( g_pODAPI ) delete g_pODAPI;
    g_pODAPI = NULL;
    
    while(g_iLocaleDepth) {
        ResetGlobalLocale();
    }
    
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
    SendPluginMessage(wxS("OCPN_DRAW_PI_READY_FOR_REQUESTS"), wxS("FALSE"));
    
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
    m_global_color_scheme = cs;
    m_pODicons->SetColourScheme( cs );
    g_pODToolbar->SetColourScheme( cs );
    g_pODToolbar->UpdateIcons();
    g_pODPointMan->SetColorScheme( m_global_color_scheme );
    g_pPathMan->SetColorScheme( m_global_color_scheme );

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
    return wxS("OCPN Draw");
}
wxString ocpn_draw_pi::GetShortDescription()
{
    return _("General drawing for OpenCPN");
}
wxString ocpn_draw_pi::GetLongDescription()
{
    return _("General Drawing for OpenCPN\n\nThanks to the community for their helpful suggestions.");
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
    return &m_pODicons->m_p_bm_ocpn_draw_pi;
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
    
    g_pOCPNDrawPropDialog->UpdateProperties();
    g_pOCPNDrawPropDialog->SetDialogSize();
    
    g_pOCPNDrawPropDialog->ShowModal();
    
    delete g_pOCPNDrawPropDialog;
    g_pOCPNDrawPropDialog = NULL;
    
}

void ocpn_draw_pi::SetPositionFixEx( PlugIn_Position_Fix_Ex &pfix )
{
    bool    l_bBoatChange = false;
    if(pfix.FixTime && pfix.nSats)
        m_LastFixTime = wxDateTime::Now();

    if(g_pfFix.valid && (g_pfFix.Lat != pfix.Lat || g_pfFix.Lon != pfix.Lon || (g_pfFix.Cog != pfix.Cog && !wxIsNaN(pfix.Cog))  || (g_pfFix.Hdt != pfix.Hdt && !wxIsNaN(pfix.Hdt))))
        l_bBoatChange = true;
    
    g_pfFix.Lat = pfix.Lat;
    g_pfFix.Lon = pfix.Lon;
    if(wxIsNaN(pfix.Cog)) {
        if(g_pfFix.Cog != 0.)
            l_bBoatChange = true;
        g_pfFix.Cog = 0.;
    }
    else g_pfFix.Cog = pfix.Cog;
    if(wxIsNaN(pfix.Sog)) {
        if(g_pfFix.Sog != 0.)
            l_bBoatChange = true;
        g_pfFix.Sog = 0.;
    }
    g_pfFix.Sog = pfix.Sog;
    g_pfFix.Var = pfix.Var;
    if(wxIsNaN(pfix.Hdm)) {
        if(g_pfFix.Hdm != 0.)
            l_bBoatChange = true;
        g_pfFix.Hdm = 0.;
    }
    else g_pfFix.Hdm = pfix.Hdm;
    if(wxIsNaN(pfix.Hdt)) {
        if(g_pfFix.Hdt != 0.)
            l_bBoatChange = true;
        g_pfFix.Hdt = 0.;
    }
    else g_pfFix.Hdt = pfix.Hdt;
    g_pfFix.FixTime = pfix.FixTime;
    g_pfFix.nSats = pfix.nSats;
    if(!g_pfFix.valid) {
        g_pfFix.valid = true;
        l_bBoatChange = true;
    }
    
    if(l_bBoatChange) {
        wxEBLListNode *EBLnode = g_pEBLList->GetFirst();
        for(size_t i = 0; i < g_pEBLList->GetCount(); i++) {
            EBL *ebl = (EBL *)EBLnode->GetData();
            if(ebl->m_bCentreOnBoat)  {
                bool l_bSaveUpdatesState = ebl->m_bSaveUpdates;
                ebl->m_bSaveUpdates = false;
                ebl->CentreOnBoat(true);
                ebl->m_bSaveUpdates = l_bSaveUpdatesState;
            }
            EBLnode = EBLnode->GetNext();
        }

        wxGZListNode *GZnode = g_pGZList->GetFirst();
        for(size_t i = 0; i < g_pGZList->GetCount(); i++) {
            GZ *gz = (GZ *)GZnode->GetData();
            bool l_bSaveUpdatesState = gz->m_bSaveUpdates;
            gz->m_bSaveUpdates = false;
            gz->UpdateGZ();
            gz->m_bSaveUpdates = l_bSaveUpdatesState;

            GZnode = GZnode->GetNext();
        }
        
        wxPILListNode *PILnode = g_pPILList->GetFirst();
        for(size_t i = 0; i < g_pPILList->GetCount(); i++) {
            PIL *pil = (PIL *)PILnode->GetData();
            if(pil->m_bCentreOnBoat)  {
                bool l_bSaveUpdatesState = pil->m_bSaveUpdates;
                pil->m_bSaveUpdates = false;
                pil->CentreOnBoat(true);
                pil->m_bSaveUpdates = l_bSaveUpdatesState;
            }
            PILnode = PILnode->GetNext();
        }

    }
}

void ocpn_draw_pi::OnToolbarToolDownCallback(int id)
{
    m_iCallerId = id;
    if( m_Mode == ID_NONE ) m_Mode = 0;

    if( NULL == g_pPathManagerDialog )         // There is one global instance of the Dialog
        g_pPathManagerDialog = new PathManagerDialog( m_parent_window );

    if ( id == m_config_button_id ) {
        if( !g_pPathManagerDialog->IsShown() ){
            
            DimeWindow( g_pPathManagerDialog );
            g_pPathManagerDialog->UpdatePathListCtrl();
            g_pPathManagerDialog->UpdateODPointsListCtrl();
            g_pPathManagerDialog->Show();
            
            
            //    Required if RMDialog is not STAY_ON_TOP
#ifdef __WXOSX__
            g_pPathManagerDialog->Centre();
            g_pPathManagerDialog->Raise();
#endif
            
        } else {
            if( NULL != g_pPathManagerDialog )
	        g_pPathManagerDialog->Hide();
        }
    }
    else if ( id == m_draw_button_id ) {
        switch (m_Mode)
        {
            case ID_MODE_BOUNDARY:
                if( 0 == nBoundary_State ){
                    nBoundary_State = 1;
                    m_pCurrentCursor = m_pCursorPencil;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, true );
                    g_pODToolbar->SetToolbarTool( m_Mode );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    nDR_State = 0;
                    nGZ_State = 0;
                    nPIL_State = 0;
                    bKey_Boundary_Pressed = false;
                    FinishBoundary();
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->SetToolbarToolEnableAll();
                    g_pODToolbar->SetToolbarTool( ID_NONE );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                }
                break;
                
            case ID_MODE_POINT:
                if( 0 == nPoint_State ){
                    nPoint_State = 1;
                    m_pCurrentCursor = m_pCursorCross;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, true );
                    g_pODToolbar->SetToolbarTool( m_Mode );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    nDR_State = 0;
                    nGZ_State = 0;
                    nPIL_State = 0;
                    bKey_Point_Pressed = false;
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->SetToolbarToolEnableAll();
                    g_pODToolbar->SetToolbarTool( ID_NONE );
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
                    g_pODToolbar->SetToolbarTool( m_Mode );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    nDR_State = 0;
                    nGZ_State = 0;
                    nPIL_State = 0;
                    bKey_TextPoint_Pressed = false;
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->SetToolbarToolEnableAll();
                    g_pODToolbar->SetToolbarTool( ID_NONE );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                }
                break;
            
            case ID_MODE_EBL:
                if( 0 == nEBL_State ){
                    nEBL_State = 1;
                    m_pCurrentCursor = m_pCursorCross;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, true );
                    g_pODToolbar->SetToolbarTool( m_Mode );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    nDR_State = 0;
                    nGZ_State = 0;
                    nPIL_State = 0;
                    bKey_EBL_Pressed = false;
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->SetToolbarToolEnableAll();
                    g_pODToolbar->SetToolbarTool( ID_NONE );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                }
                break;
                
            case ID_MODE_DR:
                if( 0 == nDR_State ){
                    nDR_State = 1;
                    m_pCurrentCursor = m_pCursorCross;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, true );
                    g_pODToolbar->SetToolbarTool( m_Mode );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    nDR_State = 0;
                    nGZ_State = 0;
                    nPIL_State = 0;
                    bKey_DR_Pressed = false;
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->SetToolbarToolEnableAll();
                    g_pODToolbar->SetToolbarTool( ID_NONE );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                }
                break;
                
            case ID_MODE_GZ:
                if( 0 == nGZ_State ){
                    nGZ_State = 1;
                    m_pCurrentCursor = m_pCursorPencil;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, true );
                    g_pODToolbar->SetToolbarTool( m_Mode );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    nDR_State = 0;
                    nGZ_State = 0;
                    nPIL_State = 0;
                    bKey_GZ_Pressed = false;
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->SetToolbarToolEnableAll();
                    g_pODToolbar->SetToolbarTool( ID_NONE );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
                }
                break;

            case ID_MODE_PIL:
                if( 0 == nPIL_State ){
                    nPIL_State = 1;
                    m_pCurrentCursor = m_pCursorCross;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, true );
                    g_pODToolbar->SetToolbarTool( m_Mode );
                    if( g_iDisplayToolbar != ID_DISPLAY_NEVER ) g_pODToolbar->Show();
                } else {
                    m_iCallerId = 0;
                    nBoundary_State = 0;
                    nPoint_State = 0;
                    nTextPoint_State = 0;
                    nEBL_State = 0;
                    nDR_State = 0;
                    nGZ_State = 0;
                    nPIL_State = 0;
                    bKey_PIL_Pressed = false;
                    m_pCurrentCursor = NULL;
                    SetCursor_PlugIn( m_pCurrentCursor );
                    SetToolbarItemState( m_draw_button_id, false );
                    g_pODToolbar->SetToolbarToolEnableAll();
                    g_pODToolbar->SetToolbarTool( ID_NONE );
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
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
    m_pODicons->SetScaleFactor();
    return;
}
void ocpn_draw_pi::SaveConfig()
{
#ifndef __WXMSW__
    wxString *l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)  && !defined(_WXMSW_)       
    wxSetlocale(LC_NUMERIC, "C");
#else
    setlocale(LC_NUMERIC, "C");
#endif
#endif
    
    wxFileConfig *pConf = m_pODConfig;
    
    if(pConf)
    {
        pConf->SetPath( wxS( "/Settings/ocpn_draw_pi" ) );
        pConf->Write( wxS( "DefaultActivePathLineColour" ), g_colourActivePathLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultInActivePathLineColour" ), g_colourInActivePathLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultActiveBoundaryLineColour" ), g_colourActiveBoundaryLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultInActiveBoundaryLineColour" ), g_colourInActiveBoundaryLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultActiveBoundaryFillColour" ), g_colourActiveBoundaryFillColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        int l_BoundaryType;
        if(g_bExclusionBoundary && !g_bInclusionBoundary) l_BoundaryType = ID_BOUNDARY_EXCLUSION;
        else if(!g_bExclusionBoundary && g_bInclusionBoundary) l_BoundaryType = ID_BOUNDARY_INCLUSION;
        else if(!g_bExclusionBoundary && !g_bInclusionBoundary) l_BoundaryType = ID_BOUNDARY_NIETHER;
        else l_BoundaryType = ID_BOUNDARY_EXCLUSION;
        pConf->Write( wxS( "DefaultBoundaryODPointsVisible"), g_bBoundaryODPointsVisible );
        pConf->Write( wxS( "DefaultBoundaryType" ), l_BoundaryType );
        long l_longFillTransparency = g_uiFillTransparency;
        pConf->Write( wxS( "DefaultBoundaryFillTransparency" ), l_longFillTransparency );
        pConf->Write( wxS( "DefaultInclusionBoundarySize" ), g_iInclusionBoundarySize );
        pConf->Write( wxS( "DefaultInActiveBoundaryFillColour" ), g_colourInActiveBoundaryFillColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultGZFirstIcon" ), g_sGZFirstIconName );
        pConf->Write( wxS( "DefaultGZSecondIcon" ), g_sGZSecondIconName );
        pConf->Write( wxS( "DefaultActiveGZLineColour" ), g_colourActiveGZLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultInActiveGZLineColour" ), g_colourInActiveGZLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultActiveGZFillColour" ), g_colourActiveGZFillColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultInActiveGZFillColour" ), g_colourInActiveGZFillColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultGZLineWidth" ), g_GZLineWidth );
        pConf->Write( wxS( "DefaultGZLineStyle" ), g_GZLineStyle );
        long l_longGZFillTransparency = g_uiGZFillTransparency;
        pConf->Write( wxS( "DefaultGZFillTransparency" ), l_longGZFillTransparency );
        pConf->Write( wxS( "DefaultGZRotateWithBoat" ), g_bGZRotateWithBoat );
        pConf->Write( wxS( "DefaultGZMaintainWith" ), g_iGZMaintainWith );
        pConf->Write( wxS( "DefaultGZPersistenceType" ), g_iGZPersistenceType );
        pConf->Write( wxS( "DefaultEBLStartIcon" ), g_sEBLStartIconName );
        pConf->Write( wxS( "DefaultEBLEndIcon" ), g_sEBLEndIconName );
        pConf->Write( wxS( "DefaultEBLLineColour" ), g_colourEBLLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultBoundaryLineWidth" ), g_BoundaryLineWidth );
        pConf->Write( wxS( "DefaultBoundaryLineStyle" ), g_BoundaryLineStyle );
        pConf->Write( wxS( "DefaultEBLLineWidth" ), g_EBLLineWidth );
        pConf->Write( wxS( "DefaultEBLLineStyle" ), g_EBLLineStyle );
        pConf->Write( wxS( "DefaultEBLShowArrow" ), g_bEBLShowArrow );
        pConf->Write( wxS( "DefaultEBLVRM" ), g_bEBLVRM );
        pConf->Write( wxS( "DefaultEBLAlwaysShowInfo" ), g_bEBLAlwaysShowInfo );
        pConf->Write( wxS( "DefaultEBLPerpLine" ), g_bEBLPerpLine );
        pConf->Write( wxS( "DefaultEBLPersistenceType" ), g_iEBLPersistenceType );
        pConf->Write( wxS( "DefaultEBLFixedEndPosition" ), g_bEBLFixedEndPosition );
        pConf->Write( wxS( "DefaultEBLRotateWithBoat" ), g_bEBLRotateWithBoat );
        pConf->Write( wxS( "DefaultEBLMaintainWith" ), g_iEBLMaintainWith );
        pConf->Write( wxS( "DefaultPILStartIcon" ), g_sPILStartIconName );
        pConf->Write( wxS( "DefaultPILEndIcon" ), g_sPILEndIconName );
        pConf->Write( wxS( "DefaultPILActiveCentreLineColour" ), g_colourPILActiveCentreLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultPILInActiveCentreLineColour" ), g_colourPILInActiveCentreLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultPILActiveOffsetLine1Colour" ), g_colourPILActiveOffsetLine1Colour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultPILInActiveOffsetLine1Colour" ), g_colourPILInActiveOffsetLine1Colour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultPILActiveOffsetLine2Colour" ), g_colourPILActiveOffsetLine2Colour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultPILInActiveOffsetLine2Colour" ), g_colourPILInActiveOffsetLine2Colour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultPILCentreLineWidth" ), g_PILCentreLineWidth );
        pConf->Write( wxS( "DefaultPILCentreLineStyle" ), g_PILCentreLineStyle );
        pConf->Write( wxS( "DefaultPILOffsetLine1Width" ), g_PILOffsetLine1Width );
        pConf->Write( wxS( "DefaultPILOffsetLine1Style" ), g_PILOffsetLine1Style );
        pConf->Write( wxS( "DefaultPILOffsetLine2Width" ), g_PILOffsetLine2Width );
        pConf->Write( wxS( "DefaultPILOffsetLine2Style" ), g_PILOffsetLine2Style );
        pConf->Write( wxS( "DefaultPILPersistenceType" ), g_iPILPersistenceType );
        pConf->Write( wxS( "DefaultPILNumIndexLines" ), g_PILDefaultNumIndexLines );
        pConf->Write( wxS( "DefaultPILOffset"), g_dPILOffset );
        pConf->Write( wxS( "DefaultDRPointIcon" ), g_sDRPointIconName );
        pConf->Write( wxS( "DefaultShowDRPointRangeRings"), g_bDRPointShowRangeRings );
        pConf->Write( wxS( "DefaultDRPointRangeRingsNumber" ), g_iDRPointRangeRingsNumber );
        pConf->Write( wxS( "DefaultDRPointRangeRingsStep" ), g_fDRPointRangeRingsStep );
        pConf->Write( wxS( "DefaultDRPointRangeRingsStepUnits" ), g_iDRPointRangeRingsStepUnits );
        pConf->Write( wxS( "DefaultDRPointRangeRingsColour" ), g_colourDRPointRangeRingsColour.GetAsString( wxC2S_CSS_SYNTAX) );
        pConf->Write( wxS( "DefaultDRPointRangeRingLineWidth" ), g_iDRPointRangeRingLineWidth );
        pConf->Write( wxS( "DefaultDRPointRangeRingLineStyle" ), g_iDRPointRangeRingLineStyle );
        pConf->Write( wxS( "DefaultDRLineColour" ), g_colourDRLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultInActiveDRLineColour" ), g_colourInActiveDRLineColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        pConf->Write( wxS( "DefaultDRLineWidth" ), g_DRLineWidth );
        pConf->Write( wxS( "DefaultDRLineStyle" ), g_DRLineStyle );
        pConf->Write( wxS( "DefaultDRSOG" ), g_dDRSOG );
        pConf->Write( wxS( "DefaultDRCOG" ), g_iDRCOG );
        pConf->Write( wxS( "DefaultDRLength" ), g_dDRLength );
        pConf->Write( wxS( "DefaultDRPointInterval" ), g_dDRPointInterval );
        pConf->Write( wxS( "DefaultDRLengthType" ), g_iDRLengthType );
        pConf->Write( wxS( "DefaultDRIntervalType" ), g_iDRIntervalType );
        pConf->Write( wxS( "DefaultDRDistanceUnits" ), g_iDRDistanceUnits );
        pConf->Write( wxS( "DefaultDRTimeUnits" ), g_iDRTimeUnits );
        pConf->Write( wxS( "DefaultDRPersistenceType" ), g_iDRPersistenceType );
        pConf->Write( wxS( "DefaultPathLineWidth" ), g_PathLineWidth );
        pConf->Write( wxS( "DefaultPathLineStyle" ), g_PathLineStyle );
        pConf->Write( wxS( "ShowLOGIcon" ), m_bLOGShowIcon );
        pConf->Write( wxS( "PathLineWidth" ), g_path_line_width );
        pConf->Write( wxS( "DefaultODPointIcon" ), g_sODPointIconName );
        pConf->Write( wxS( "ODPointRangeRingsNumber" ), g_iODPointRangeRingsNumber );
        pConf->Write( wxS( "ODPointRangeRingsStep" ), g_fODPointRangeRingsStep );
        pConf->Write( wxS( "ODPointRangeRingsStepUnits" ), g_iODPointRangeRingsStepUnits );
        pConf->Write( wxS( "ODPointRangeRingsColour" ), g_colourODPointRangeRingsColour.GetAsString( wxC2S_CSS_SYNTAX ) );
        long l_longBoundaryPointFillTransparency = g_uiBoundaryPointFillTransparency;
        pConf->Write( wxS( "DefaultBoundaryPointFillTransparency" ), l_longBoundaryPointFillTransparency );
        pConf->Write( wxS( "DefaultBoundaryPointRangeRingLineWidth" ), g_iBoundaryPointRangeRingLineWidth );
        pConf->Write( wxS( "DefaultBoundaryPointRangeRingLineStyle" ), g_iBoundaryPointRangeRingLineStyle );
        pConf->Write( wxS( "DefaultInclusionBoundaryPointSize" ), g_iInclusionBoundaryPointSize );
        pConf->Write( wxS( "ShowMag" ), g_bShowMag );
        pConf->Write( wxS( "AllowLeftDrag" ), g_bAllowLeftDrag );
        pConf->Write( wxS( "UserMagVariation" ), wxString::Format( _T("%.2f"), g_UserVar ) );
        pConf->Write( wxS( "KeepODNavobjBackups" ), g_navobjbackups );
        pConf->Write( wxS( "CurrentDrawMode" ), m_Mode );
        pConf->Write( wxS( "ConfirmObjectDelete" ), g_bConfirmObjectDelete );
        pConf->Write( wxS( "InitialEdgePanSensitivity" ), g_InitialEdgePanSensitivity );
        pConf->Write( wxS( "EdgePanSensitivity" ), g_EdgePanSensitivity );
        pConf->Write( wxS( "ToolBarPosX" ), g_iToolbarPosX );
        pConf->Write( wxS( "ToolBarPosY" ), g_iToolbarPosY );
        pConf->Write( wxS( "DisplayToolbar"), g_iDisplayToolbar );
        pConf->Write( wxS( "DefaultTextPointIcon" ), g_sTextPointIconName );
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
#if wxCHECK_VERSION(3,0,0)        
        pConf->Write( wxS( "DefaultTextPointFontStrikethrough" ), g_DisplayTextFont.GetStrikethrough() );
#endif
        pConf->Write( wxS( "DefaultTextPointFaceName" ), g_DisplayTextFont.GetFaceName() );
        pConf->Write( wxS( "DefaultTextPointFontEncoding" ), (int)g_DisplayTextFont.GetEncoding() );
        pConf->Write( wxS( "DefaultTextPointDisplayTextWhen"), g_iTextPointDisplayTextWhen );
        
    }
    
#ifndef __WXMSW__
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, l_locale->ToAscii());
#else
    setlocale(LC_NUMERIC, l_locale->ToAscii());
#endif
    delete l_locale;
#endif
}

void ocpn_draw_pi::LoadConfig()
{
#ifndef __WXMSW__
    wxString *l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, "C");
#else
    setlocale(LC_NUMERIC, "C");
#endif
#endif
    
    wxFileConfig *pConf = (wxFileConfig *)m_pODConfig;
    
    if(pConf)
    {
        wxString val;
        pConf->SetPath( wxS( "/Settings/ocpn_draw_pi" ) );
        wxString  l_wxsColour;
        pConf->Read( wxS( "DefaultActivePathLineColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourActivePathLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActivePathLineColour" ), &l_wxsColour, wxS( "LIGHT GREY" ) );
        g_colourInActivePathLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultActiveBoundaryLineColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourActiveBoundaryLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActiveBoundaryLineColour" ), &l_wxsColour, wxS( "LIGHT GREY" ) );
        g_colourInActiveBoundaryLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultActiveBoundaryFillColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourActiveBoundaryFillColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActiveBoundaryFillColour" ), &l_wxsColour, wxS( "LIGHT GREY" ) );
        g_colourInActiveBoundaryFillColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActiveBoundaryFillColour" ), &l_wxsColour, wxS( "LIGHT GREY" ) );
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
            case ID_BOUNDARY_NIETHER:
                g_bExclusionBoundary = false;
                g_bInclusionBoundary = false;
                break;
            default:
                g_bExclusionBoundary = true;
                g_bInclusionBoundary = false;
                break;
        }
        pConf->Read( wxS( "DefaultBoundaryODPointsVisible"), &g_bBoundaryODPointsVisible, 1 );
        pConf->Read( wxS( "DefaultEBLLineColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourEBLLineColour.Set( l_wxsColour );
        long l_longFillTransparency;
        pConf->Read( wxS( "DefaultBoundaryFillTransparency" ), &l_longFillTransparency, 175 );
        g_uiFillTransparency = l_longFillTransparency;
        pConf->Read( wxS( "DefaultBoundaryPointRangeRingLineWidth" ), &g_iBoundaryPointRangeRingLineWidth, 2 );
        pConf->Read( wxS( "DefaultBoundaryPointRangeRingLineStyle" ), &g_iBoundaryPointRangeRingLineStyle, wxPENSTYLE_SOLID );
        pConf->Read( wxS( "DefaultInclusionBoundarySize" ), &g_iInclusionBoundarySize, 15 );
        pConf->Read( wxS( "DefaultBoundaryLineWidth" ), &g_BoundaryLineWidth, 2  );
        pConf->Read( wxS( "DefaultBoundaryLineStyle" ), &g_BoundaryLineStyle, wxPENSTYLE_SOLID );
        
        pConf->Read( wxS( "DefaultGZFirstIcon" ), &g_sGZFirstIconName, wxS("Circle") );
        pConf->Read( wxS( "DefaultGZSecondIcon" ), &g_sGZSecondIconName, wxS("Circle") );
        pConf->Read( wxS( "DefaultActiveGZLineColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourActiveGZLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActiveGZLineColour" ), &l_wxsColour, wxS( "LIGHT GREY" ) );
        g_colourInActiveGZLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultActiveGZFillColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourActiveGZFillColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActiveGZFillColour" ), &l_wxsColour, wxS( "LIGHT GREY" ) );
        g_colourInActiveGZFillColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultGZLineWidth" ), &g_GZLineWidth, 2  );
        pConf->Read( wxS( "DefaultGZLineStyle" ), &g_GZLineStyle, wxPENSTYLE_SOLID );
        long l_longGZFillTransparency;
        pConf->Read( wxS( "DefaultGZFillTransparency" ), &l_longGZFillTransparency, 175 );
        g_uiGZFillTransparency = l_longGZFillTransparency;
        pConf->Read( wxS( "DefaultGZRotateWithBoat" ), &g_bGZRotateWithBoat, false );
        pConf->Read( wxS( "DefaultGZMaintainWith" ), &g_iGZMaintainWith, ID_MAINTAIN_WITH_HEADING );
        pConf->Read( wxS( "DefaultGZPersistenceType" ),  &g_iGZPersistenceType, 0 );
        
        pConf->Read( wxS( "DefaultEBLEndIcon" ), &g_sEBLEndIconName, wxS("Circle") );
        pConf->Read( wxS( "DefaultEBLStartIcon" ), &g_sEBLStartIconName, wxS("Circle") );
        pConf->Read( wxS( "DefaultEBLLineWidth" ), &g_EBLLineWidth, 2  );
        pConf->Read( wxS( "DefaultEBLLineStyle" ), &g_EBLLineStyle, wxPENSTYLE_SOLID );
        pConf->Read( wxS( "DefaultEBLShowArrow" ), &g_bEBLShowArrow, false );
        pConf->Read( wxS( "DefaultEBLVRM" ), &g_bEBLVRM, false );
        pConf->Read( wxS( "DefaultEBLAlwaysShowInfo" ), &g_bEBLAlwaysShowInfo, false );
        pConf->Read( wxS( "DefaultEBLPerpLine" ), &g_bEBLPerpLine, false );
        pConf->Read( wxS( "DefaultEBLPersistenceType" ),  &g_iEBLPersistenceType, 0 );
        pConf->Read( wxS( "DefaultEBLFixedEndPosition" ),  &g_bEBLFixedEndPosition, 0 );
        pConf->Read( wxS( "DefaultEBLRotateWithBoat" ), &g_bEBLRotateWithBoat, false );
        pConf->Read( wxS( "DefaultEBLMaintainWith" ), &g_iEBLMaintainWith, ID_MAINTAIN_WITH_HEADING );

        pConf->Read( wxS( "DefaultPILEndIcon" ), &g_sPILEndIconName, wxS("Circle") );
        pConf->Read( wxS( "DefaultPILStartIcon" ), &g_sPILStartIconName, wxS("Circle") );
        pConf->Read( wxS( "DefaultPILActiveCentreLineColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourPILActiveCentreLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultPILInActiveCentreLineColour" ), &l_wxsColour, wxS( "LIGHT GREY" ) );
        g_colourPILInActiveCentreLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultPILActiveOffsetLine1Colour" ), &l_wxsColour, wxS( "Red" ) );
        g_colourPILActiveOffsetLine1Colour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultPILInActiveOffsetLine1Colour" ), &l_wxsColour, wxS( "LIGHT GREY" ) );
        g_colourPILInActiveOffsetLine1Colour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultPILActiveOffsetLine2Colour" ), &l_wxsColour, wxS( "Red" ) );
        g_colourPILActiveOffsetLine2Colour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultPILInActiveOffsetLine2Colour" ), &l_wxsColour, wxS( "LIGHT GREY" ) );
        g_colourPILInActiveOffsetLine2Colour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultPILCentreLineWidth" ), &g_PILCentreLineWidth, 2 );
        pConf->Read( wxS( "DefaultPILCentreLineStyle" ), &g_PILCentreLineStyle, wxPENSTYLE_SOLID );
        pConf->Read( wxS( "DefaultPILOffsetLine1Width" ), &g_PILOffsetLine1Width, 2 );
        pConf->Read( wxS( "DefaultPILOffsetLine1Style" ), &g_PILOffsetLine1Style, wxPENSTYLE_SOLID );
        pConf->Read( wxS( "DefaultPILOffsetLine2Width" ), &g_PILOffsetLine2Width, 2 );
        pConf->Read( wxS( "DefaultPILOffsetLine2Style" ), &g_PILOffsetLine2Style, wxPENSTYLE_SOLID );
        pConf->Read( wxS( "DefaultPILNumIndexLines" ), &g_PILDefaultNumIndexLines, 0 );
        pConf->Read( wxS( "DefaultPILPersistenceType" ), &g_iPILPersistenceType, 0 );
        pConf->Read( wxS( "DefaultPILOffset"), &g_dPILOffset, 0.5 );
        pConf->Read( wxS( "DefaultDRPointIcon" ), &g_sDRPointIconName, wxS("Circle") );
        pConf->Read( wxS( "DefaultShowDRPointRangeRings"), &g_bDRPointShowRangeRings, false );
        pConf->Read( wxS( "DefaultDRPointRangeRingsNumber" ), &g_iDRPointRangeRingsNumber, 0 );
        pConf->Read( wxS( "DefaultDRPointRangeRingsStep" ), &val, wxS("1.0") );
        g_fDRPointRangeRingsStep = atof( val.mb_str() );
        pConf->Read( wxS( "DefaultDRPointRangeRingsStepUnits" ), &g_iDRPointRangeRingsStepUnits, 0 );
        wxString  l_wxsDRPointRangeRingsColour;
        g_colourDRPointRangeRingsColour = wxColour( *wxRED );
        pConf->Read( wxS( "DefaultDRPointRangeRingsColour" ), &l_wxsDRPointRangeRingsColour, wxS( "RED" ) );
        g_colourDRPointRangeRingsColour.Set( l_wxsDRPointRangeRingsColour );
        pConf->Read( wxS( "DefaultDRPointRangeRingLineWidth" ), &g_iDRPointRangeRingLineWidth, 2 );
        pConf->Read( wxS( "DefaultDRPointRangeRingLineStyle" ), &g_iDRPointRangeRingLineStyle, wxPENSTYLE_SOLID );
        pConf->Read( wxS( "DefaultDRLineColour" ), &l_wxsColour, wxS( "RED" ) );
        g_colourDRLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultInActiveDRLineColour" ), &l_wxsColour, wxS( "LIGHT GREY" ) );
        g_colourInActiveDRLineColour.Set( l_wxsColour );
        pConf->Read( wxS( "DefaultDRLineWidth" ), &g_DRLineWidth, 2  );
        pConf->Read( wxS( "DefaultDRLineStyle" ), &g_DRLineStyle, wxPENSTYLE_SOLID );
        pConf->Read( wxS( "DefaultDRSOG" ), &val, wxS("5.0") );
        g_dDRSOG = atof( val.mb_str() );
        pConf->Read( wxS( "DefaultDRCOG" ), &g_iDRCOG, 0 );
        pConf->Read( wxS( "DefaultDRLength" ), &g_dDRLength, 5 );
        pConf->Read( wxS( "DefaultDRPointInterval" ), &g_dDRPointInterval, 1 );
        pConf->Read( wxS( "DefaultDRLengthType" ), &g_iDRLengthType, 1 );
        pConf->Read( wxS( "DefaultDRIntervalType" ), &g_iDRIntervalType, 1 );
        pConf->Read( wxS( "DefaultDRDistanceUnits" ), &g_iDRDistanceUnits, 1 );
        pConf->Read( wxS( "DefaultDRTimeUnits" ), &g_iDRTimeUnits, 0 );
        pConf->Read( wxS( "DefaultDRPersistenceType" ), &g_iDRPersistenceType, 2 );
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
        long l_longBoundaryPointFillTransparency;
        pConf->Read( wxS( "DefaultBoundaryPointFillTransparency" ), &l_longBoundaryPointFillTransparency, 175 );
        g_uiBoundaryPointFillTransparency = l_longBoundaryPointFillTransparency;
        pConf->Read( wxS( "DefaultInclusionBoundaryPointSize" ), &g_iInclusionBoundaryPointSize, 15 );
        int l_BoundaryPointType;
        pConf->Read( wxS( "DefaultBoundaryPointType" ), &l_BoundaryPointType, ID_BOUNDARY_EXCLUSION );
        switch (l_BoundaryPointType) {
            case ID_BOUNDARY_EXCLUSION:
                g_bExclusionBoundaryPoint = true;
                g_bInclusionBoundaryPoint = false;
                break;
            case ID_BOUNDARY_INCLUSION:
                g_bExclusionBoundaryPoint = false;
                g_bInclusionBoundaryPoint = true;
                break;
            case ID_BOUNDARY_NIETHER:
                g_bExclusionBoundaryPoint = false;
                g_bInclusionBoundaryPoint = false;
                break;
            default:
                g_bExclusionBoundaryPoint = true;
                g_bInclusionBoundaryPoint = false;
                break;
        }
        pConf->Read( wxS( "ShowMag" ), &g_bShowMag, 0 );
        pConf->Read( wxS( "AllowLeftDrag" ), &g_bAllowLeftDrag,0 );
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

        pConf->Read( wxS( "DefaultTextPointIcon" ), &g_sTextPointIconName, wxS("Circle") );
        wxString  l_wxsDefautlTextColour;
        g_colourDefaultTextColour = wxColour( *wxBLACK );
        pConf->Read( wxS( "DefaultTextColour" ), &l_wxsDefautlTextColour, wxS( "BLACK" ) );
        g_colourDefaultTextBackgroundColour.Set( l_wxsDefautlTextColour );
        wxString  l_wxsDefautlTextBackgroundColour;
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
        wxFont *l_pDisplayTextFont = GetOCPNScaledFont_PlugIn( wxS("Marks"), 0 );
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
#if wxCHECK_VERSION(3,0,0)   
        pConf->Read( wxS( "DefaultTextPointFontStrikethrough" ), &l_bfontInfo, false );
        g_DisplayTextFont.SetStrikethrough( l_bfontInfo );
#endif        
        wxString l_wxsFaceName;
        pConf->Read( wxS( "DefaultTextPointFaceName" ), &l_wxsFaceName, l_pDisplayTextFont->GetFaceName() );
        g_DisplayTextFont.SetFaceName( l_wxsFaceName );
        pConf->Read( wxS( "DefaultTextPointFontEncoding" ), &l_fontInfo, (int)l_pDisplayTextFont->GetEncoding() );
        g_DisplayTextFont.SetEncoding( (wxFontEncoding)l_fontInfo );
        pConf->Read( wxS( "DefaultTextPointDisplayTextWhen" ), &g_iTextPointDisplayTextWhen, ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ALWAYS );
    }

#ifndef __WXMSW__
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, l_locale->ToAscii());
#else
    setlocale(LC_NUMERIC, l_locale->ToAscii());
#endif
    delete l_locale;
#endif
}

void ocpn_draw_pi::SetPluginMessage(wxString &message_id, wxString &message_body)
{
    g_pODJSON->ProcessMessage(message_id, message_body);
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

        if((bKey_Boundary_Pressed || bKey_Point_Pressed || bKey_TextPoint_Pressed || bKey_EBL_Pressed || bKey_DR_Pressed || bKey_GZ_Pressed || bKey_PIL_Pressed) && key_char != WXK_ESCAPE) return true;
        
        switch( key_char ) {
            case WXK_CONTROL_B:                      // Ctrl B
                if ( event.ShiftDown() ) { // Shift-Ctrl-B
                    if(event.GetEventType() == wxEVT_KEY_DOWN) {
                        bKey_Boundary_Pressed = true;
                        bKey_Point_Pressed = false;
                        bKey_TextPoint_Pressed = false;
                        bKey_EBL_Pressed = false;
                        bKey_DR_Pressed = false;
                        bKey_GZ_Pressed = false;
                        bKey_PIL_Pressed = false;
                        m_Mode = ID_MODE_BOUNDARY;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        SetToolbarTool();
                    }
                    bret = TRUE;
                } else bret = FALSE;
                break;
            case WXK_CONTROL_P:                      // Ctrl P
                if ( event.ShiftDown() ) { // Shift-Ctrl-P
                    if(event.GetEventType() == wxEVT_KEY_DOWN) {
                        bKey_Boundary_Pressed = false;
                        bKey_Point_Pressed = true;
                        bKey_TextPoint_Pressed = false;
                        bKey_EBL_Pressed = false;
                        bKey_DR_Pressed = false;
                        bKey_GZ_Pressed = false;
                        bKey_PIL_Pressed = false;
                        m_Mode = ID_MODE_POINT;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        SetToolbarTool();
                    }
                    bret = TRUE;
                } else bret = FALSE;
                break;
            case WXK_CONTROL_T:                      // Ctrl T
                if ( event.ShiftDown() ) { // Shift-Ctrl-T
                    if(event.GetEventType() == wxEVT_KEY_DOWN) {
                        bKey_Boundary_Pressed = false;
                        bKey_Point_Pressed = false;
                        bKey_TextPoint_Pressed = true;
                        bKey_EBL_Pressed = false;
                        bKey_DR_Pressed = false;
                        bKey_GZ_Pressed = false;
                        bKey_PIL_Pressed = false;
                        m_Mode = ID_MODE_TEXT_POINT;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        SetToolbarTool();
                    }
                    bret = TRUE;
                } else bret = FALSE;
                break;
            case WXK_CONTROL_E:                      // Ctrl E
                if ( event.ShiftDown() ) { // Shift-Ctrl-E
                    if(event.GetEventType() == wxEVT_KEY_DOWN) {
                        bKey_Boundary_Pressed = false;
                        bKey_Point_Pressed = false;
                        bKey_TextPoint_Pressed = false;
                        bKey_EBL_Pressed = true;
                        bKey_DR_Pressed = false;
                        bKey_GZ_Pressed = false;
                        bKey_PIL_Pressed = false;
                        m_Mode = ID_MODE_EBL;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        SetToolbarTool();
                    }
                    bret = TRUE;
                } else bret = FALSE;
                break;
            case WXK_CONTROL_D:                      // Ctrl D
                if ( event.ShiftDown() ) { // Shift-Ctrl-D
                    if(event.GetEventType() == wxEVT_KEY_DOWN) {
                        bKey_Boundary_Pressed = false;
                        bKey_Point_Pressed = false;
                        bKey_TextPoint_Pressed = false;
                        bKey_EBL_Pressed = false;
                        bKey_DR_Pressed = true;
                        bKey_GZ_Pressed = false;
                        bKey_PIL_Pressed = false;
                        m_Mode = ID_MODE_DR;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        SetToolbarTool();
                    }
                    bret = TRUE;
                } else bret = FALSE;
                break;
            case WXK_CONTROL_G:                      // Ctrl G
                if ( event.ShiftDown() ) { // Shift-Ctrl-G
                    if(event.GetEventType() == wxEVT_KEY_DOWN) {
                        bKey_Boundary_Pressed = false;
                        bKey_Point_Pressed = false;
                        bKey_TextPoint_Pressed = false;
                        bKey_EBL_Pressed = false;
                        bKey_DR_Pressed = false;
                        bKey_GZ_Pressed = true;
                        bKey_PIL_Pressed = false;
                        m_Mode = ID_MODE_GZ;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        SetToolbarTool();
                    }
                    bret = TRUE;
                } else bret = FALSE;
                break;
            case WXK_CONTROL_L:                      // Ctrl L
                if ( event.ShiftDown() ) { // Shift-Ctrl-L
                    if(event.GetEventType() == wxEVT_KEY_DOWN) {
                        bKey_Boundary_Pressed = false;
                        bKey_Point_Pressed = false;
                        bKey_TextPoint_Pressed = false;
                        bKey_EBL_Pressed = false;
                        bKey_DR_Pressed = false;
                        bKey_GZ_Pressed = false;
                        bKey_PIL_Pressed = true;
                        m_Mode = ID_MODE_PIL;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        SetToolbarTool();
                    }
                    bret = TRUE;
                } else bret = FALSE;
                break;

            case WXK_ESCAPE: // Generic Break
                if(event.GetEventType() == wxEVT_KEY_DOWN) {
                        if( nBoundary_State > 0 ){
                        m_Mode = ID_MODE_BOUNDARY;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        bret = TRUE;
                    } else if( nPoint_State > 0 ){
                        m_Mode = ID_MODE_POINT;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        bret = TRUE;
                    } else if( nTextPoint_State > 0 ){
                        m_Mode = ID_MODE_TEXT_POINT;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        bret = TRUE;
                    } else if( nEBL_State > 0 ){
                        m_Mode = ID_MODE_EBL;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        bret = TRUE;
                    } else if( nDR_State > 0 ){
                        m_Mode = ID_MODE_DR;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        bret = TRUE;
                    } else if( nGZ_State > 0 ){
                        m_Mode = ID_MODE_GZ;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        bret = TRUE;
                    } else if( nPIL_State > 0 ){
                        m_Mode = ID_MODE_PIL;
                        g_pODToolbar->m_Mode = m_Mode;
                        OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
                        bret = TRUE;
                    } else if( m_bODPointEditing ) {
                        m_bODPointEditing = false;
                        m_pCurrentCursor = NULL;
                        m_pFoundODPoint->m_bIsBeingEdited = false;
                        RequestRefresh( m_parent_window );
                    } else bret = FALSE;
                    m_iCallerId = 0;
                    bKey_Boundary_Pressed = false;
                    bKey_Point_Pressed = false;
                    bKey_TextPoint_Pressed = false;
                    bKey_EBL_Pressed = false;
                    bKey_DR_Pressed = false;
                    bKey_GZ_Pressed = false;
                    g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
                    if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();

                    if( !m_pBoundaryList.empty() ) {
                        std::list<Boundary *>::iterator it = m_pBoundaryList.begin();
                        while( it != m_pBoundaryList.end() ) {
                            (*it)->m_bPathPropertiesBlink = false;
                            ++it;
                        }

                        m_pBoundaryList.clear();
                    }
                }
                break;
        }
    }
    SetCursor_PlugIn( m_pCurrentCursor );
    if(bret) RequestRefresh(m_parent_window);
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
    
    if( g_pODRolloverWin && g_pODRolloverWin->IsActive() )
        m_RolloverPopupTimer.Start( 10, wxTIMER_ONE_SHOT );               // faster response while the rollover is turned on
    else
        m_RolloverPopupTimer.Start( m_rollover_popup_timer_msec, wxTIMER_ONE_SHOT );
        
        
    if( nBoundary_State == 1 || nPoint_State >= 1 || nPath_State == 1 || nTextPoint_State == 1 || nEBL_State > 0 || nGZ_State > 0 || nPIL_State > 0
        || m_bPathEditing || m_bODPointEditing || m_bTextPointEditing || m_bEBLMoveOrigin) {
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
        if( m_pSelectedPath && m_seltype == SELTYPE_PATHSEGMENT) {
            if( NULL == g_pPathManagerDialog )         // There is one global instance of the Dialog
                g_pPathManagerDialog = new PathManagerDialog( m_parent_window );
            
            DimeWindow( g_pPathManagerDialog );
            g_pPathManagerDialog->ShowPathPropertiesDialog( m_pSelectedPath );
            m_pSelectedPath = NULL;
            bret = true;
        } else if(m_pSelectedPath && m_seltype == SELTYPE_PIL) {
            if( NULL == g_PILIndexLinePropDialog)
                g_PILIndexLinePropDialog = new PILPropertiesDialogImpl( m_parent_window );
            DimeWindow( g_PILIndexLinePropDialog );
            g_PILIndexLinePropDialog->UpdateProperties( (PIL *)m_pSelectedPath, m_iPILId );
            g_PILIndexLinePropDialog->Show();
            bret = true;
        } else if( m_pFoundODPoint ) {
            if( NULL == g_pODPointPropDialog )
                g_pODPointPropDialog = new ODPointPropertiesImpl( m_parent_window );
            
            DimeWindow( g_pODPointPropDialog );
            g_pODPointPropDialog->SetODPoint( m_pFoundODPoint );
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
                g_pODToolbar->SetToolbarToolEnableOnly(ID_MODE_BOUNDARY);
                bret = CreateBoundaryLeftClick( event );
            } else if ( nPoint_State > 0) {
                g_pODToolbar->SetToolbarToolEnableOnly(ID_MODE_POINT);
                bret = CreatePointLeftClick( event );
            } else if ( nTextPoint_State > 0 ) {
                g_pODToolbar->SetToolbarToolEnableOnly(ID_MODE_TEXT_POINT);
                bret = CreateTextPointLeftClick( event );
            } else if ( nEBL_State > 0 ) {
                g_pODToolbar->SetToolbarToolEnableOnly(ID_MODE_EBL);
                bret = CreateEBLLeftClick( event );
            } else if ( nDR_State > 0 ) {
                g_pODToolbar->SetToolbarToolEnableOnly(ID_MODE_DR);
                bret = CreateDRLeftClick( event );
            } else if ( nGZ_State > 0 ) {
                g_pODToolbar->SetToolbarToolEnableOnly(ID_MODE_GZ);
                bret = CreateGZLeftClick( event );
            } else if ( nPIL_State > 0 ) {
                g_pODToolbar->SetToolbarToolEnableOnly(ID_MODE_PIL);
                bret = CreatePILLeftClick( event );
            }
        } else if( m_bPathEditing && ( m_iEditMode == ID_PATH_MENU_MOVE_PATH || m_iEditMode == ID_PATH_MENU_MOVE_PATH_SEGMENT || m_iEditMode == ID_ODPOINT_MENU_MOVE )) {
            m_pCurrentCursor = m_pCursorCross;
            m_PathMove_cursor_start_lat = m_cursor_lat;
            m_PathMove_cursor_start_lon = m_cursor_lon;
            bRefresh = TRUE;
        } else if( m_bPathEditing && m_iEditMode == ID_PIL_MENU_MOVE_INDEX_LINE) {
            m_pCurrentCursor = m_pCursorCross;
            bRefresh = TRUE;
        } else if ( m_bODPointEditing ) {
            m_pCurrentCursor = m_pCursorCross;
            bret = TRUE;
        } else if ( m_bTextPointEditing ) {
            m_pCurrentCursor = m_pTextCursorCross;
            bret = TRUE;
//        } else if( event.ControlDown() ) {
//            FindSelectedObject();
//            
//            if ( 0 != m_seltype ) {
//                if(m_pSelectedPath) {
//                    m_pSelectedBoundary = NULL;
//                    m_pSelectedEBL = NULL;
//                    m_pSelectedDR = NULL;
//                    m_pSelectedGZ = NULL;
//                    if(m_pSelectedPath->m_sTypeString == wxT("Boundary"))
//                        m_pSelectedBoundary = (Boundary *)m_pSelectedPath;
//                    else if(m_pSelectedPath->m_sTypeString == wxT("EBL"))
//                        m_pSelectedEBL = (EBL *)m_pSelectedPath;
//                    else if(m_pSelectedPath->m_sTypeString == wxT("DR"))
//                        m_pSelectedDR = (DR *)m_pSelectedPath;
//                    else if(m_pSelectedPath->m_sTypeString == wxT("Guard Zone"))
//                        m_pSelectedGZ = (GZ *)m_pSelectedPath;
//                }
//                if(m_pSelectedBoundary) {
//                    m_pBoundaryList.push_back( m_pSelectedBoundary );
//                    m_pSelectedBoundary->m_bPathPropertiesBlink = true;
//                }
//                bRefresh = FALSE;
//                bret = FALSE;
//            }
        } else if( g_bAllowLeftDrag && !m_bEBLMoveOrigin) {
            FindSelectedObject();

            if( 0 != m_seltype ) {
                if(m_pSelectedPath) {
                    m_pSelectedBoundary = NULL;
                    m_pSelectedEBL = NULL;
                    m_pSelectedDR = NULL;
                    m_pSelectedGZ = NULL;
                    m_pSelectedPIL = NULL;
                    if(m_pSelectedPath->m_sTypeString == wxT("Boundary")) {
                        m_pSelectedBoundary = (Boundary *)m_pSelectedPath;
                        if( event.ControlDown() ) {
                            m_pBoundaryList.push_back( m_pSelectedBoundary );
                            m_pSelectedBoundary->m_bPathPropertiesBlink = true;
                        }
                    }
                    else if(m_pSelectedPath->m_sTypeString == wxT("EBL"))
                        m_pSelectedEBL = (EBL *)m_pSelectedPath;
                    else if(m_pSelectedPath->m_sTypeString == wxT("DR"))
                        m_pSelectedDR = (DR *)m_pSelectedPath;
                    else if(m_pSelectedPath->m_sTypeString == wxT("Guard Zone"))
                        m_pSelectedGZ = (GZ *)m_pSelectedPath;
                    else if(m_pSelectedPath->m_sTypeString == wxT("PIL"))
                        m_pSelectedPIL = (PIL *)m_pSelectedPath;
                    m_bPathEditing = true;
                    if(m_seltype & SELTYPE_ODPOINT) {
                        if(m_pSelectedBoundary || m_pSelectedDR || m_pSelectedGZ) {
                            m_iEditMode = ID_PATH_MENU_MOVE_POINT;
                            m_pCurrentCursor = m_pCursorCross;
                        } else if(m_pSelectedEBL || m_pSelectedPIL) {
                            m_iEditMode = ID_ODPOINT_MENU_MOVE;
                            m_bODPointEditing = true;
                            m_pFoundODPoint->m_bIsBeingEdited = TRUE;
                            if(m_pSelectedEBL)
                                m_pSelectedEBL->m_bEndPointMoving = true;
                            else if(m_pSelectedPIL)
                                m_pSelectedPIL->m_bEndPointMoving = true;
                        }
                    }
                    else if(m_seltype & SELTYPE_PIL) {
                        m_iEditMode = ID_PIL_MENU_MOVE_INDEX_LINE;
                        m_pCurrentCursor = m_pCursorCross;
                        m_PathMove_cursor_start_lat = m_cursor_lat;
                        m_PathMove_cursor_start_lon = m_cursor_lon;
                    } else {
                        m_iEditMode = ID_PATH_MENU_MOVE_PATH;
                        m_pCurrentCursor = m_pCursorCross;
                        m_PathMove_cursor_start_lat = m_cursor_lat;
                        m_PathMove_cursor_start_lon = m_cursor_lon;
                    }
                } else if(m_pFoundODPoint) {
                    m_iEditMode = ID_ODPOINT_MENU_MOVE;
                    m_pCurrentCursor = m_pCursorCross;
                    m_bODPointEditing = true;
                }
                bRefresh = TRUE;
                bret = FALSE;
            }
        } else if( !m_pBoundaryList.empty() ) {
            std::list<Boundary *>::iterator it = m_pBoundaryList.begin();
            while( it != m_pBoundaryList.end() ) {
                (*it)->m_bPathPropertiesBlink = false;
                ++it;
            }

            m_pBoundaryList.clear();
        }
    }
    
    if( event.LeftUp() ) {
        if (m_iCallerId == m_draw_button_id && (nBoundary_State > 0 || nPoint_State > 0 || nTextPoint_State > 0 || nEBL_State > 0 || nDR_State > 0 || nGZ_State > 0 || nPIL_State > 0) ) {
            bret = true;
        } else {
            if(!m_bPathEditing && !m_bEBLMoveOrigin && !m_bODPointEditing) { // Handle left up moving origin when doing double left click on OD object
                FindSelectedObject();
                if(m_pSelectedPath || m_pFoundODPoint) {
                    m_pSelectedPath = NULL;
                    m_pSelectedBoundary = NULL;
                    m_pSelectedEBL = NULL;
                    m_pSelectedDR = NULL;
                    m_pSelectedGZ = NULL;
                    m_pSelectedPIL = NULL;
                    m_pFoundODPoint = NULL;
                    m_bPathEditing = false;
                    bret = true;
                }
            }
            if (m_bEBLMoveOrigin) {
                m_bEBLMoveOrigin = false;
                if(m_pSelectedPath->m_sTypeString == wxT("EBL")) {
                    m_pSelectedEBL = (EBL *)m_pSelectedPath;
                    m_pSelectedEBL->CentreOnLatLon( m_cursor_lat, m_cursor_lon );
                }
                else {
                    m_pSelectedPIL = (PIL *)m_pSelectedPath;
                    m_pSelectedPIL->CentreOnLatLon( m_cursor_lat, m_cursor_lon );
                }
                m_bPathEditing = FALSE;
                m_bODPointEditing = FALSE;
                m_pCurrentCursor = NULL;
                m_pSelectedPath->m_bIsBeingEdited = FALSE;
                
                // TODO reimplement undo
                //undo->AfterUndoableAction( m_pRoutePointEditTarget );
                m_pSelectedPath = NULL;
                m_pSelectedBoundary = NULL;
                m_pSelectedEBL = NULL;
                m_pSelectedDR = NULL;
                m_pSelectedGZ = NULL;
                m_pSelectedPIL = NULL;
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
                    m_pFoundODPoint->m_bIsBeingEdited = false;
                    m_pFoundODPoint->m_bPtIsSelected = false;
                    if(m_pSelectedEBL)
                        m_pSelectedEBL->MoveEndPoint();
                    if(m_pSelectedPIL)
                        m_pSelectedPIL->MoveEndPoint();
                }
                if(m_pSelectedPath->m_sTypeString == wxT("Guard Zone")) {
                    m_pSelectedGZ->UpdateGZSelectablePath();
                } else if (m_pSelectedPIL) {
                    m_pSelectedPIL->UpdatePIL();
                } else {
                    g_pODSelect->DeleteAllSelectablePathSegments( m_pSelectedPath );
                    g_pODSelect->DeleteAllSelectableODPoints( m_pSelectedPath );
                    g_pODSelect->AddAllSelectablePathSegments( m_pSelectedPath );
                    g_pODSelect->AddAllSelectableODPoints( m_pSelectedPath );
                }
                
                m_pSelectedPath->FinalizeForRendering();
                m_pSelectedPath->UpdateSegmentDistances();
                bool prev_bskip = g_pODConfig->m_bSkipChangeSetUpdate;
                g_pODConfig->m_bSkipChangeSetUpdate = false;
                if(m_pSelectedPath->m_sTypeString == wxT("EBL") || m_pSelectedPath->m_sTypeString == wxT("PIL")) {
                    // Save changes done by user
                    bool l_bSaveUpdatesState = m_pSelectedPath->m_bSaveUpdates;
                    m_pSelectedPath->m_bSaveUpdates = true;
                    g_pODConfig->UpdatePath( m_pSelectedPath );
                    m_pSelectedPath->m_bSaveUpdates = l_bSaveUpdatesState;
                    if(m_pSelectedEBL)
                        m_pSelectedEBL->m_bEndPointMoving = false;
                    if(m_pSelectedPIL)
                        m_pSelectedPIL->m_bEndPointMoving = false;
                } else
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
                m_pSelectedBoundary = NULL;
                m_pSelectedEBL = NULL;
                m_pSelectedDR = NULL;
                m_pSelectedGZ = NULL;
                m_pSelectedPIL = NULL;
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
                m_pSelectedBoundary = NULL;
                m_pSelectedEBL = NULL;
                m_pSelectedDR = NULL;
                m_pSelectedGZ = NULL;
                m_pSelectedPIL = NULL;
                m_pFoundODPoint = NULL;
                
                bret = TRUE;
            } else if( m_pFoundODPoint ) {
                m_pFoundODPoint->m_bPtIsSelected = false;
                bret = false;
            } else if ( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
                // This is to handle the double click to bring up the dialog box so that the screen does not jump on the extra clicks.
                bret = TRUE;
            }
            m_iEditMode = ID_MENU_NOITEM;
        }
    }
        
    if( event.Dragging() ) {
        if( event.LeftIsDown() ) {
            if( !m_pBoundaryList.empty() ) {
                std::list<Boundary *>::iterator it = m_pBoundaryList.begin();
                while( it != m_pBoundaryList.end() ) {
                    (*it)->m_bPathPropertiesBlink = false;
                    ++it;
                }
                m_pBoundaryList.clear();
            }
            if ( nBoundary_State > 0 || nPoint_State > 0 ) bret = true;
            else if( m_pFoundODPoint && ( m_iEditMode == ID_PATH_MENU_MOVE_POINT || m_iEditMode == ID_ODPOINT_MENU_MOVE ) ) {
                if( m_bPathEditing ) {
                    m_pCurrentCursor = m_pCursorCross;
                    if( event.ControlDown() && !event.ShiftDown()) m_pFoundODPoint->m_lat = m_cursor_lat;
                    else if( event.ShiftDown() && !event.ControlDown()) m_pFoundODPoint->m_lon = m_cursor_lon;
                    else {
                        m_pFoundODPoint->m_lat = m_cursor_lat;
                        m_pFoundODPoint->m_lon = m_cursor_lon;
                    }
                    if(m_pSelectedPath->m_sTypeString == wxT("EBL")) {
                        m_pSelectedEBL = (EBL *)m_pSelectedPath;
                        if( event.ControlDown() && event.ShiftDown())
                            m_pSelectedEBL->Resize( m_cursor_lat, m_cursor_lon, true );
                        else
                            m_pSelectedEBL->Resize();
                    } else if(m_pSelectedPath->m_sTypeString == wxT("Guard Zone")) {
                        m_pSelectedGZ = (GZ *)m_pSelectedPath;
                        m_pSelectedGZ->UpdateGZ( m_pFoundODPoint, false );
                    } else if(m_pSelectedPath->m_sTypeString == wxT("PIL")) {
                        m_pSelectedPIL = (PIL *)m_pSelectedPath;
                        m_pSelectedPIL->Resize();
                    }
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
                    if( event.ControlDown() && !event.ShiftDown()) m_pFoundODPoint->m_lat = m_cursor_lat;
                    else if( event.ShiftDown() && !event.ControlDown()) m_pFoundODPoint->m_lon = m_cursor_lon;
                    else {
                        m_pFoundODPoint->m_lat = m_cursor_lat;
                        m_pFoundODPoint->m_lon = m_cursor_lon;
                    }
                    if(m_pSelectedPath) {
                        if(m_pSelectedPath->m_sTypeString == wxT("EBL")) {
                            m_pSelectedEBL = (EBL *)m_pSelectedPath;
                            if(event.ControlDown() && event.ShiftDown())
                                m_pSelectedEBL->Resize( m_cursor_lat, m_cursor_lon, true );
                            else
                                m_pSelectedEBL->Resize( );
                        } else if(m_pSelectedPath->m_sTypeString == wxT("Guard Zone")) {
                            m_pSelectedGZ = (GZ *)m_pSelectedPath;
                            m_pSelectedGZ->UpdateGZ( m_pFoundODPoint, false );
                        } else if(m_pSelectedPath->m_sTypeString == wxT("PIL")) {
                            m_pSelectedPIL = (PIL *)m_pSelectedPath;
                            m_pSelectedPIL->Resize();
                        }
                    }

                    if ( g_pODPointPropDialog && m_pFoundODPoint == g_pODPointPropDialog->GetODPoint() ) g_pODPointPropDialog->UpdateProperties( TRUE );
                    
                    m_pCurrentCursor = m_pCursorCross;
                    bRefresh = TRUE;
                    bret = FALSE;
                    event.SetEventType(wxEVT_MOVING); // stop dragging canvas on event flow through
                }
            } else if(m_bPathEditing && (m_iEditMode == ID_PATH_MENU_MOVE_PATH || m_iEditMode == ID_PATH_MENU_MOVE_PATH_SEGMENT)) {
                // Do move of whole path
                double l_move_lat;
                double l_move_lon;
                if( event.ControlDown()) l_move_lat = m_PathMove_cursor_start_lat - m_cursor_lat;
                else if( event.ShiftDown()) l_move_lon = m_PathMove_cursor_start_lon - m_cursor_lon;
                else {
                    l_move_lat = m_PathMove_cursor_start_lat - m_cursor_lat;
                    l_move_lon = m_PathMove_cursor_start_lon - m_cursor_lon;
                }
                if(m_iEditMode == ID_PATH_MENU_MOVE_PATH)
                    m_pSelectedPath->MoveAllPoints( l_move_lat, l_move_lon );
                else if(m_iEditMode == ID_PATH_MENU_MOVE_PATH_SEGMENT)
                    m_pSelectedPath->MoveSegment( l_move_lat, l_move_lon, m_pFoundODPoint, m_pFoundODPointSecond );
                m_PathMove_cursor_start_lat = m_cursor_lat;
                m_PathMove_cursor_start_lon = m_cursor_lon;
                bRefresh = TRUE;
                bret = FALSE;
                event.SetEventType(wxEVT_MOVING); // stop dragging canvas on event flow through
            } else if(m_bPathEditing && m_iEditMode == ID_PIL_MENU_MOVE_INDEX_LINE) {
                m_pSelectedPIL->MovePILLine(m_cursor_lat, m_cursor_lon, m_iPILId);
                event.SetEventType(wxEVT_MOVING); // stop dragging canvas on event flow through
                bret = FALSE;
            } else if(m_bEBLEditing) {
                if( event.ControlDown()) m_pFoundODPoint->m_lat = m_cursor_lat;
                else if( event.ShiftDown()) m_pFoundODPoint->m_lon = m_cursor_lon;
                else {
                    m_pFoundODPoint->m_lat = m_cursor_lat;
                    m_pFoundODPoint->m_lon = m_cursor_lon;
                }
                
                if ( g_pODPointPropDialog && m_pFoundODPoint == g_pODPointPropDialog->GetODPoint() ) g_pODPointPropDialog->UpdateProperties( TRUE );
                
                m_pCurrentCursor = m_pCursorCross;
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
            bRefresh = TRUE;
            bret = TRUE;
        } else if(m_bPathEditing) {
            m_bPathEditing = false;
            m_pCurrentCursor = NULL;
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nBoundary_State == 1 || nPoint_State == 1 || nTextPoint_State == 1 || nEBL_State == 1 || nDR_State == 1 || nGZ_State == 1 || nPIL_State == 1 ) {
            m_Mode++;
            if(m_Mode >= ID_MODE_BOUNDARY)
                SetToolbarTool();
            g_pODToolbar->SetToolbarTool( m_Mode );
            bret = TRUE;
        } else if ( nBoundary_State > 1 ) {
            m_iCallerId = 0;
            nBoundary_State = 0;
            FinishBoundary();
            bKey_Boundary_Pressed = false;
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->SetToolbarToolEnableAll();
            g_pODToolbar->SetToolbarTool( ID_NONE );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nPoint_State > 1) {
            m_iCallerId = 0;
            nPoint_State = 0;
            bKey_Point_Pressed = false;
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->SetToolbarToolEnableAll();
            g_pODToolbar->SetToolbarTool( ID_NONE );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nTextPoint_State > 1) {
            m_iCallerId = 0;
            nTextPoint_State = 0;
            bKey_TextPoint_Pressed = false;
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->SetToolbarToolEnableAll();
            g_pODToolbar->SetToolbarTool( ID_NONE );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nEBL_State > 1 ) {
            m_iCallerId = 0;
            nEBL_State = 0;
            bKey_EBL_Pressed = false;
            m_bEBLMoveOrigin = false;
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->SetToolbarToolEnableAll();
            g_pODToolbar->SetToolbarTool( ID_NONE );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nDR_State > 1 ) {
            m_iCallerId = 0;
            nDR_State = 0;
            bKey_DR_Pressed = false;
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->SetToolbarToolEnableAll();
            g_pODToolbar->SetToolbarTool( ID_NONE );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nGZ_State > 1 ) {
            m_iCallerId = 0;
            if(!(nGZ_State & 1) && m_pMouseGZ) {
                g_pGZMan->DeletePath(m_pMouseGZ);
                m_pMouseGZ = NULL;
            }
            nGZ_State = 0;
            bKey_GZ_Pressed = false;
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->SetToolbarToolEnableAll();
            g_pODToolbar->SetToolbarTool( ID_NONE );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( nPIL_State > 1 ) {
            m_iCallerId = 0;
            nPIL_State = 0;
            bKey_PIL_Pressed = false;
            m_pCurrentCursor = NULL;
            SetToolbarItemState( m_draw_button_id, false );
            g_pODToolbar->SetToolbarToolEnableAll();
            g_pODToolbar->SetToolbarTool( ID_NONE );
            g_pODToolbar->GetPosition( &g_iToolbarPosX, &g_iToolbarPosY );
            if( g_iDisplayToolbar != ID_DISPLAY_ALWAYS ) g_pODToolbar->Hide();
            bRefresh = TRUE;
            bret = TRUE;
        } else if ( m_bEBLMoveOrigin ) {
            m_bEBLMoveOrigin = false;
            m_pCurrentCursor = NULL;
            bRefresh = TRUE;
            bret = TRUE;
        } else if( m_pBoundaryList.size() > 0 ) {
            g_ODEventHandler->SetCanvas( ocpncc1 );
            g_ODEventHandler->SetBoundaryList( m_pBoundaryList );
            g_ODEventHandler->PopupMenu( SELTYPE_BOUNDARYLIST );
            m_pBoundaryList.clear();
            bRefresh = TRUE;
            bret = true;
        } else if ( nBoundary_State == 0 && nPoint_State == 0 && nTextPoint_State == 0 && nEBL_State == 0 && nDR_State == 0 && nGZ_State == 0 && nPIL_State == 0) {
            FindSelectedObject();
            
            if( 0 != m_seltype ) {
                if(m_pSelectedPath) {
                    m_pSelectedBoundary = NULL;
                    m_pSelectedEBL = NULL;
                    m_pSelectedDR = NULL;
                    m_pSelectedGZ = NULL;
                    m_pSelectedPIL = NULL;
                    if(m_pSelectedPath->m_sTypeString == wxT("Boundary"))
                        m_pSelectedBoundary = (Boundary *)m_pSelectedPath;
                    else if(m_pSelectedPath->m_sTypeString == wxT("EBL"))
                        m_pSelectedEBL = (EBL *)m_pSelectedPath;
                    else if(m_pSelectedPath->m_sTypeString == wxT("DR"))
                        m_pSelectedDR = (DR *)m_pSelectedPath;
                    else if(m_pSelectedPath->m_sTypeString == wxT("Guard Zone"))
                        m_pSelectedGZ = (GZ *)m_pSelectedPath;
                    else if(m_pSelectedPath->m_sTypeString == wxT("PIL"))
                        m_pSelectedPIL = (PIL *)m_pSelectedPath;
                }
                g_ODEventHandler->SetCanvas( ocpncc1 );
                g_ODEventHandler->SetPath( m_pSelectedPath );
                g_ODEventHandler->SetPoint( m_pFoundODPoint );
                g_ODEventHandler->SetPIL( m_iPILId );
                g_ODEventHandler->SetLatLon( m_cursor_lat, m_cursor_lon );
                g_ODEventHandler->PopupMenu( m_seltype );
                
                //RequestRefresh( m_parent_window );
                bRefresh = TRUE;
                bret = true;
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
    m_iPILId = 0;
    
    SelectItem *pFindPP;
    SelectItem *pFindPathSeg;
    SelectItem *pFindPILIndex;
    pFindPP = g_pODSelect->FindSelection( slat, slon, SELTYPE_ODPOINT );
    pFindPathSeg = g_pODSelect->FindSelection( slat, slon, SELTYPE_PATHSEGMENT );
    pFindPILIndex = g_pODSelect->FindSelection( slat, slon, SELTYPE_PIL );
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
        SelectableItemList SelList = g_pODSelect->FindSelectionList( slat, slon, SELTYPE_ODPOINT );
        wxSelectableItemListNode *node = SelList.GetFirst();
        while( node ) {
            SelectItem *pFindSel = node->GetData();
            
            ODPoint *pop = (ODPoint *) pFindSel->m_pData1;        //candidate
            if( pop->m_sTypeString == wxT("EBL Point") && pop->m_ODPointName == _("Boat") ) continue;
            
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
                m_seltype |= SELTYPE_ODPOINT;
            if( m_pSelectedPath->m_sTypeString == wxT("Boundary") ) m_pSelectedBoundary = (Boundary *)m_pSelectedPath;
            else if( m_pSelectedPath->m_sTypeString == wxT("EBL") ) m_pSelectedEBL = (EBL *)m_pSelectedPath;
            else if( m_pSelectedPath->m_sTypeString == wxT("DR") ) m_pSelectedDR = (DR *)m_pSelectedPath;
            else if( m_pSelectedPath->m_sTypeString == wxT("Guard Zone") ) m_pSelectedGZ = (GZ *)m_pSelectedPath;
            else if( m_pSelectedPath->m_sTypeString == wxT("PIL") ) m_pSelectedPIL = (PIL *)m_pSelectedPath;
        } else if( m_pFoundODPoint ) m_seltype |= SELTYPE_ODPOINT;
        
        
        if( m_pFoundODPoint ) m_pFoundODPoint->m_bPtIsSelected = true;
    }
    
    if( pFindPathSeg ) {                 // there is at least one select item
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
            
            m_pSelectedPath->m_bPathIsSelected = !(m_seltype & SELTYPE_ODPOINT);
            m_seltype |= SELTYPE_PATHSEGMENT;
        }
        
    }

    if(pFindPILIndex) {
        m_seltype |= SELTYPE_PIL;
        m_pSelectedPath = (PIL *)pFindPILIndex->m_pData3;
        m_iPILId = pFindPILIndex->GetUserData();  //get Id of particular PIL being selected
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
    delete sUnit;
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
    m_pVP = pivp;
    g_pVP = pivp;
    g_VP = *pivp;
    m_chart_scale = pivp->chart_scale;
    m_view_scale = pivp->view_scale_ppm;
    
    ODDC ocpnmdc( *pmdc );
    
    RenderPathLegs( ocpnmdc );
    return TRUE;
}

bool ocpn_draw_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *pivp)
{
    m_pVP = pivp;
    g_pVP = pivp;
    g_VP = *pivp;
    m_chart_scale = pivp->chart_scale;
    m_view_scale = pivp->view_scale_ppm;
    
    g_pDC = new ODDC( dc );
    LLBBox llbb;
    llbb.Set( pivp->lat_min, pivp->lon_min, pivp->lat_max, pivp->lon_max );
    
    DrawAllPathsInBBox( *g_pDC, llbb );
    DrawAllODPointsInBBox( *g_pDC, llbb );
    RenderPathLegs( *g_pDC );
    
    delete g_pDC;
    return TRUE;
}

bool ocpn_draw_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *pivp)
{
    m_pcontext = pcontext;
    m_pVP = pivp;
    g_pVP = pivp;
    g_VP = *pivp;
    m_chart_scale = pivp->chart_scale;
    m_view_scale = pivp->view_scale_ppm;
    
    g_pDC = new ODDC();
    LLBBox llbb;
    llbb.Set( pivp->lat_min, pivp->lon_min, pivp->lat_max, pivp->lon_max );

    //    DrawAllODPointsInBBox( *g_pDC, llbb );
    RenderPathLegs( *g_pDC );
    
    if (m_pMouseBoundary) m_pMouseBoundary->DrawGL( *pivp );
    
    DrawAllPathsAndODPoints( *pivp );

    if( g_pODRolloverWin && g_pODRolloverWin->IsActive() && g_pODRolloverWin->GetBitmap() != NULL ) {
        g_pDC->DrawBitmap( *(g_pODRolloverWin->GetBitmap()),
                       g_pODRolloverWin->GetPosition().x,
                       g_pODRolloverWin->GetPosition().y, false );
    }
    
    delete g_pDC;
    return TRUE;
}

void ocpn_draw_pi::RenderPathLegs( ODDC &dc ) 
{
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
        
        //        if( !IsTouchInterface_PlugIn) {
        boundary->DrawPointWhich( dc, boundary->m_lastMousePointIndex, &lastPoint );
        if( boundary->m_NextLegGreatCircle ) {
            for( int i=1; i<=milesDiff; i++ ) {
                double p = (double)i * (1.0/(double)milesDiff);
                double pLat, pLon;
                Geodesic::GreatCircleTravel( m_prev_rlon, m_prev_rlat, gcDist*p, brg, &pLon, &pLat, &gcBearing2 );
                GetCanvasPixLL( m_pVP, &destPoint, m_cursor_lat, m_cursor_lon);
                //destPoint = VPoint.GetPixFromLL( pLat, pLon );
                boundary->DrawSegment( dc, &lastPoint, &destPoint, *m_pVP, false );
                wxPoint rpn;
                boundary->GetPoint( 1 )->Draw( dc, &rpn );
                boundary->DrawSegment( dc, &rpn , &destPoint, *m_pVP, false );
                lastPoint = destPoint;
            }
        }
        else {
            boundary->DrawSegment( dc, &lastPoint, &r_rband, *m_pVP, false );
            if ( nBoundary_State >= 2) { 
                wxPoint rpn;
                boundary->GetPoint( 1 )->Draw( dc, &rpn );
                boundary->DrawSegment( dc, &rpn , &r_rband, *m_pVP, false );
            }
        }
        
        wxString info = CreateExtraPathLegInfo(dc, boundary, brg, dist, m_cursorPoint);
        RenderExtraPathLegInfo( dc, r_rband, info );
    } else if( m_pSelectedEBL && (nEBL_State > 0 || m_bEBLMoveOrigin) ) {
        EBL *ebl = new EBL();
        double brg, dist;
        wxPoint boatpoint;
        if(m_bEBLMoveOrigin) {
            ODPoint *tp = (ODPoint *) m_pSelectedEBL->m_pODPointList->GetLast()->GetData();
            GetCanvasPixLL( g_pVP, &boatpoint, tp->m_lat, tp->m_lon );
            DistanceBearingMercator_Plugin( m_cursor_lat, m_cursor_lon, tp->m_lat, tp->m_lon, &brg, &dist );
            ebl->DrawSegment( dc, &boatpoint, &m_cursorPoint, *m_pVP, false );
        } else {
            GetCanvasPixLL( g_pVP, &boatpoint, g_pfFix.Lat, g_pfFix.Lon );
            DistanceBearingMercator_Plugin( m_cursor_lat, m_cursor_lon, g_pfFix.Lat, g_pfFix.Lon, &brg, &dist );
            ebl->DrawSegment( dc, &boatpoint, &m_cursorPoint, *m_pVP, false );
        }
        wxString info = CreateExtraPathLegInfo(dc, ebl, brg, dist, m_cursorPoint);
        if(info.length() > 0)
            RenderExtraPathLegInfo( dc, m_cursorPoint, info );
        delete ebl;
    } else if( m_pSelectedPIL && (nPIL_State > 0 || m_bEBLMoveOrigin) ) {
        EBL *ebl = new EBL();
        double brg, dist;
        wxPoint boatpoint;
        if(m_bEBLMoveOrigin) {
            ODPoint *tp = (ODPoint *) m_pSelectedPIL->m_pODPointList->GetLast()->GetData();
            GetCanvasPixLL( g_pVP, &boatpoint, tp->m_lat, tp->m_lon );
            DistanceBearingMercator_Plugin( m_cursor_lat, m_cursor_lon, tp->m_lat, tp->m_lon, &brg, &dist );
            ebl->DrawSegment( dc, &boatpoint, &m_cursorPoint, *m_pVP, false );
        } else {
            GetCanvasPixLL( g_pVP, &boatpoint, g_pfFix.Lat, g_pfFix.Lon );
            DistanceBearingMercator_Plugin( m_cursor_lat, m_cursor_lon, g_pfFix.Lat, g_pfFix.Lon, &brg, &dist );
            ebl->DrawSegment( dc, &boatpoint, &m_cursorPoint, *m_pVP, false );
        }
        wxString info = CreateExtraPathLegInfo(dc, ebl, brg, dist, m_cursorPoint);
        if(info.length() > 0)
            RenderExtraPathLegInfo( dc, m_cursorPoint, info );
        delete ebl;
    } else if( nGZ_State > 0 ) {
        // draw line from boat to cursor
        EBL *ebl = new EBL();
        double brg, dist;
        wxPoint boatpoint;
        GetCanvasPixLL( g_pVP, &boatpoint, g_pfFix.Lat, g_pfFix.Lon );
        DistanceBearingMercator_Plugin( m_cursor_lat, m_cursor_lon, g_pfFix.Lat, g_pfFix.Lon, &brg, &dist );
        ebl->DrawSegment( dc, &boatpoint, &m_cursorPoint, *m_pVP, false );
        wxString info = CreateExtraPathLegInfo(dc, ebl, brg, dist, m_cursorPoint);
        if(info.length() > 0)
            RenderExtraPathLegInfo( dc, m_cursorPoint, info );
        delete ebl;
        
        // rubber band guard zone
        if( !(nGZ_State & 1) ) {
            // TODO need code here to render GZ as it is drawn
            double l_dLat;
            double l_dLon;
            wxPoint l_l1p1;
            wxPoint l_l1p2;
            wxPoint l_l2p1;
            wxPoint l_l2p2;
            GZ *gz = new GZ();
            // get x, y of first point on first line
            GetCanvasPixLL( g_pVP, &l_l1p1, m_pMouseGZ->m_pODPointList->GetFirst()->GetData()->m_lat, m_pMouseGZ->m_pODPointList->GetFirst()->GetData()->m_lon );
            // get x, y of second point on first line
            PositionBearingDistanceMercator_Plugin( g_pfFix.Lat, g_pfFix.Lon, m_pMouseGZ->m_dFirstLineDirection, dist, &l_dLat, &l_dLon);
            GetCanvasPixLL( g_pVP, &l_l1p2, l_dLat, l_dLon );
            // get x, y of second point on second line
            l_l2p2 = m_cursorPoint;
            // get x, y of first point on second line
            PositionBearingDistanceMercator_Plugin( g_pfFix.Lat, g_pfFix.Lon, brg, m_pMouseGZ->m_dFirstDistance, &l_dLat, &l_dLon);
            GetCanvasPixLL( g_pVP, &l_l2p1, l_dLat, l_dLon);
            
            gz->DrawArcSegment( dc, &boatpoint, &l_l1p1, &l_l1p2, &l_l2p2, &l_l2p1, *m_pVP, false );
            
            delete gz;
        }
    } else if( nPIL_State > 0 || nEBL_State > 0 ) {
        // draw line from boat to cursor
        ODPath *ptPath;
        PIL *pil;
        EBL *ebl;
        if(nPIL_State > 0) {
            pil = new PIL();
            ptPath = pil;
        }
        else {
            ebl = new EBL();
            ptPath = ebl;
        }
        double brg, dist;
        wxPoint boatpoint;
        GetCanvasPixLL( g_pVP, &boatpoint, g_pfFix.Lat, g_pfFix.Lon );
        DistanceBearingMercator_Plugin( m_cursor_lat, m_cursor_lon, g_pfFix.Lat, g_pfFix.Lon, &brg, &dist );
        ptPath->DrawSegment( dc, &boatpoint, &m_cursorPoint, *m_pVP, false );
        wxString info = CreateExtraPathLegInfo(dc, ptPath, brg, dist, m_cursorPoint);
        if(info.length() > 0)
            RenderExtraPathLegInfo( dc, m_cursorPoint, info );
        delete ptPath;    
    }
        
        
}

wxString ocpn_draw_pi::CreateExtraPathLegInfo(ODDC &dc, ODPath *path, double brg, double dist, wxPoint ref_point)
{
    wxString pathInfo;
    if(path->m_sTypeString == wxT("EBL")) {
        int EBLbrgFrom = (int)GetTrueOrMag( brg );
        int EBLbrgTo = EBLbrgFrom - 180;
        if(EBLbrgTo < 0) EBLbrgTo += 360;
        if( g_bShowMag )
            pathInfo << wxString::Format( wxString("From: %03d°(M), To: %03d°(M)\n Dist:", wxConvUTF8 ), EBLbrgFrom, EBLbrgTo  );
        else
            pathInfo << wxString::Format( wxString("From: %03d°, To: %03d°\n Dist:", wxConvUTF8 ), EBLbrgFrom, EBLbrgTo );
    } else {
        if( g_bShowMag )
            pathInfo << wxString::Format( wxString("%03d°(M)  ", wxConvUTF8 ), (int)GetTrueOrMag( brg ) );
        else
            pathInfo << wxString::Format( wxString("%03d°  ", wxConvUTF8 ), (int)GetTrueOrMag( brg ) );
    }
    
    pathInfo << wxS(" ") << FormatDistanceAdaptive( dist );
    
    wxFont *dFont;
    dFont = GetOCPNScaledFont_PlugIn( wxS("OD_PathLegInfoRollover"), 0 );
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
            if( !path->m_bIsInLayer ) {
                s0.Append( _("Boundary") );
            }
            else
                s0.Append( _("Layer Boundary") );
            s0.Append( _T(": ") );
        }
        s0 += FormatDistanceAdaptive( path->m_path_length + dist );
    } 
    
    return s0;
}

void ocpn_draw_pi::RenderExtraPathLegInfo( ODDC &dc, wxPoint ref_point, wxString s )
{
    wxFont *dFont = GetOCPNScaledFont_PlugIn( wxS("OD_PathLegInfoRollover"), 0 );
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
    m_pVP = &vp;
    g_pVP = &vp;
    g_VP = vp;
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
    m_pFoundODPointSecond = NULL;
    
    // TODO fix up undo
    //undo->InvalidateUndo();
    RequestRefresh( m_parent_window );
}

void ocpn_draw_pi::DrawAllPathsInBBox(ODDC &dc,  LLBBox& BltBBox)
{
    wxPathListNode *pnode = g_pPathList->GetFirst();
    while( pnode ) {
        bool b_run = false;
        bool b_drawn = false;
        ODPath *pPath = pnode->GetData();
        ODPath *pPathDraw = NULL;
        Boundary *pBoundaryDraw = NULL;
        EBL *pEBLDraw = NULL;
        DR *pDRDraw = NULL;
        GZ *pGZDraw = NULL;
        PIL *pPILDraw = NULL;
        
        if(pPath->m_sTypeString == wxT("Boundary")){
            pBoundaryDraw = (Boundary *) pPath;
            pPathDraw = pBoundaryDraw;
        } else if(pPath->m_sTypeString == wxT("EBL")) {
            pEBLDraw = (EBL *) pPath;
            pPathDraw = pEBLDraw;
        } else if(pPath->m_sTypeString == wxT("DR")) {
            pDRDraw = (DR *) pPath;
            pPathDraw = pDRDraw;
        } else if(pPath->m_sTypeString == wxT("Guard Zone")) {
            pGZDraw = (GZ *) pPath;
            pPathDraw = pGZDraw;
        } else if(pPath->m_sTypeString == wxT("PIL")) {
            pPILDraw = (PIL *) pPath;
            pPathDraw = pPILDraw;
        }

        if( pPathDraw ) {
            
            LLBBox test_box = pPathDraw->GetBBox();
            LLBBox tb;
            tb.Set(m_lat, m_lon, m_lat, m_lon);

            if( b_run ) test_box.Expand( tb );
            
            if( !BltBBox.IntersectOut( test_box ) ) // Path is not wholly outside window
            {
                b_drawn = true;
                pPathDraw->Draw( dc, *m_pVP );
            }

            if(pPathDraw == m_pSelectedEBL && m_bODPointEditing && !m_pSelectedEBL->m_bAlwaysShowInfo) {
                double brg, dist;
                wxPoint destPoint;
                ODPoint *pStartPoint = m_pSelectedEBL->m_pODPointList->GetFirst()->GetData();
                ODPoint *pEndPoint = m_pSelectedEBL->m_pODPointList->GetLast()->GetData();
                DistanceBearingMercator_Plugin( pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &dist );
                if(m_pFoundODPoint == pEndPoint)
                    GetCanvasPixLL( m_pVP, &destPoint, pEndPoint->m_lat, pEndPoint->m_lon);
                else
                    GetCanvasPixLL( m_pVP, &destPoint, pStartPoint->m_lat, pStartPoint->m_lon);
                wxString info = CreateExtraPathLegInfo(dc, m_pSelectedEBL, brg, dist, destPoint);
                if(info.length() > 0)
                    RenderExtraPathLegInfo( dc, destPoint, info );
            }
            else if (pPathDraw == m_pSelectedPIL  && m_bPathEditing) {
                wxString info;
                if(m_iEditMode == ID_PIL_MENU_MOVE_INDEX_LINE)  {
                    std::list<PILLINE>::iterator it = m_pSelectedPIL->m_PilLineList.begin();
                    while (it != m_pSelectedPIL->m_PilLineList.end()) {
                        if (it->iID == m_iPILId) break;
                        ++it;
                    }
                    if (it != m_pSelectedPIL->m_PilLineList.end()) {
                        info = CreateExtraPathLegInfo(dc, m_pSelectedPIL, m_pSelectedPIL->m_dEBLAngle, it->dOffset, m_cursorPoint);
                    }
                }
                else
                    info = CreateExtraPathLegInfo(dc, m_pSelectedPIL, m_pSelectedPIL->m_dEBLAngle, m_pSelectedPIL->m_dLength, m_cursorPoint);
                if (info.length() > 0)
                        RenderExtraPathLegInfo(dc, m_cursorPoint, info);
            }
        }
        pnode = pnode->GetNext();
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
        if(node->GetData()->m_sTypeString == _T("Boundary Point"))
            pOP = (BoundaryPoint *)node->GetData();
        if( pOP ) {
            if( pOP->m_bIsInRoute || pOP->m_bIsInPath ) {
                node = node->GetNext();
                continue;
            } else {
                if( BltBBox.Contains( pOP->m_lat, pOP->m_lon ) ) pOP->Draw( dc, NULL );
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
    int nearby_sel_rad_pix;
    if(IsTouchInterface_PlugIn())
        nearby_sel_rad_pix = 50;
    else
        nearby_sel_rad_pix = 8;

    double nearby_radius_meters = nearby_sel_rad_pix / m_view_scale;
    
    BoundaryPoint *pNearbyPoint = (BoundaryPoint *)g_pODPointMan->GetNearbyODPoint( rlat, rlon, nearby_radius_meters );

    if( pNearbyPoint && ( pNearbyPoint != (BoundaryPoint *)m_prev_pMousePoint )
        && !pNearbyPoint->m_bIsInTrack && !pNearbyPoint->m_bIsInLayer )
    {
        int dlg_return;
#ifndef __WXOSX__
        dlg_return = OCPNMessageBox_PlugIn( m_parent_window, _("Use nearby Point?"),
                                            _("OCPN Draw Point Create"),
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
    int nearby_sel_rad_pix;
    if(IsTouchInterface_PlugIn())
        nearby_sel_rad_pix = 50;
    else
        nearby_sel_rad_pix = 8;

    double nearby_radius_meters = nearby_sel_rad_pix / m_view_scale;
    
    TextPoint *pNearbyPoint = (TextPoint *)g_pODPointMan->GetNearbyODPoint( rlat, rlon,
                                                                            nearby_radius_meters );
    if( pNearbyPoint && ( pNearbyPoint != m_prev_pMousePoint )
        && !pNearbyPoint->m_bIsInTrack && !pNearbyPoint->m_bIsInLayer )
    {
        int dlg_return;
#ifndef __WXOSX__
        dlg_return = OCPNMessageBox_PlugIn( m_parent_window, _("Use nearby Point?"),
                                            _("OCPN Draw Point Create"),
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
        pMousePoint = new TextPoint( rlat, rlon, g_sTextPointIconName, wxS(""), wxT("") );
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
    int nearby_sel_rad_pix;
    if(IsTouchInterface_PlugIn())
        nearby_sel_rad_pix = 50;
    else
        nearby_sel_rad_pix = 8;

    double nearby_radius_meters = nearby_sel_rad_pix / m_view_scale;
    
    BoundaryPoint *pNearbyPoint = (BoundaryPoint *)g_pODPointMan->GetNearbyODPoint( rlat, rlon,
                                                             nearby_radius_meters );
    if( pNearbyPoint && ( pNearbyPoint != m_prev_pMousePoint )
        && !pNearbyPoint->m_bIsInTrack && !pNearbyPoint->m_bIsInLayer )
    {
        int dlg_return;
#ifndef __WXOSX__
        dlg_return = OCPNMessageBox_PlugIn( m_parent_window, _("Use nearby OD Point?"),
                                            _("OCPN Draw Boundary Create"),
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
                
                int answer = OCPNMessageBox_PlugIn( m_parent_window, msg, _("OCPN Draw Boundary Create"), wxYES_NO | wxNO_DEFAULT );
                
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
            pMousePoint->m_bExclusionBoundaryPoint = m_pMouseBoundary->m_bExclusionBoundary;
            pMousePoint->m_bInclusionBoundaryPoint = m_pMouseBoundary->m_bInclusionBoundary;
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
    m_pMouseEBL->m_PathNameString << _("EBL") << _T(" ") << g_pEBLList->GetCount();
    m_dStartLat = g_pfFix.Lat;
    m_dStartLon = g_pfFix.Lon;

    ODPoint *beginPoint = new ODPoint( g_pfFix.Lat, g_pfFix.Lon, g_sEBLStartIconName, _("Boat"), wxT("") );
    beginPoint->SetNameShown( false );
    beginPoint->SetTypeString( wxT("EBL Point"));
    beginPoint->m_bIsolatedMark = false;
    m_pMouseEBL->AddPoint( beginPoint, false );
    
    pMousePoint = new ODPoint( rlat, rlon, g_sEBLEndIconName, _("End"), wxT("") );
    
    pMousePoint->SetNameShown( false );
    pMousePoint->SetTypeString( wxS("EBL Point") );
    pMousePoint->m_bIsolatedMark = FALSE;
    m_pMouseEBL->AddPoint( pMousePoint );
    m_pMouseEBL->m_bCentreOnBoat = true;
    DistanceBearingMercator_Plugin(rlat, rlon, m_dStartLat, m_dStartLon, &m_pMouseEBL->m_dEBLAngle, &m_pMouseEBL->m_dLength);
    
    if(m_pMouseEBL->m_bRotateWithBoat) {
        switch(m_pMouseEBL->m_iMaintainWith) {
            case ID_MAINTAIN_WITH_HEADING:
                if(!wxIsNaN(g_pfFix.Hdt))
                    m_pMouseEBL->m_dEBLAngle -= g_pfFix.Hdt;
                break;
            case ID_MAINTAIN_WITH_COG:
                if(!wxIsNaN(g_pfFix.Cog))
                    m_pMouseEBL->m_dEBLAngle -= g_pfFix.Cog;
                break;
        }
    }

    
    if(m_pMouseEBL->m_iPersistenceType == ID_PERSISTENT || m_pMouseEBL->m_iPersistenceType == ID_PERSISTENT_CRASH)
        g_pODConfig->AddNewPath( m_pMouseEBL, -1 );    // don't save over restart
    g_pODSelect->AddSelectableODPoint( rlat, rlon, pMousePoint );
    g_pODSelect->AddSelectablePathSegment( g_pfFix.Lat, g_pfFix.Lon, rlat, rlon, beginPoint, pMousePoint, m_pMouseEBL );
    
    m_pMouseEBL->RebuildGUIDList();
    
    nEBL_State++;
    
    RequestRefresh( m_parent_window );
    
    return TRUE;
}

bool ocpn_draw_pi::CreateDRLeftClick( wxMouseEvent &event )
{
    if( NULL == g_pODDRDialog )         // There is one global instance of the Dialog
        g_pODDRDialog = new ODDRDialogImpl( ocpncc1 );
    
    g_pODDRDialog->SetupDialog();
    DimeWindow( g_pODDRDialog );
    g_pODDRDialog->Show();
    
    
    //    Required if RMDialog is not STAY_ON_TOP
#ifdef __WXOSX__
    g_pODDRDialog->Centre();
    g_pODDRDialog->Raise();
#endif
    
    nDR_State++;
    
    RequestRefresh( m_parent_window );
    
    return true;
}

bool ocpn_draw_pi::CreateGZLeftClick( wxMouseEvent &event )
{
    //Process GZ creation
    double rlat, rlon;
    
    rlat = m_cursor_lat;
    rlon = m_cursor_lon;
    
    m_bGZEditing = true;
    
    if( nGZ_State & 1 ) {
        m_pMouseGZ = new GZ();
        g_pGZList->Append( m_pMouseGZ );
        g_pPathList->Append( m_pMouseGZ);
        m_pMouseGZ->m_PathNameString << _("GZ") << _T(" ") << ++g_iGZMaxNum;
        m_pMouseGZ->m_dCentreLat = g_pfFix.Lat;
        m_pMouseGZ->m_dCentreLon = g_pfFix.Lon;
        
        DistanceBearingMercator_Plugin( m_cursor_lat, m_cursor_lon, g_pfFix.Lat, g_pfFix.Lon, &m_pMouseGZ->m_dFirstLineDirection, &m_pMouseGZ->m_dFirstDistance );
        r_rband.x = g_cursor_x;
        r_rband.y = g_cursor_y;
        m_dStartLat = rlat;
        m_dStartLon = rlon;
        if(m_pMouseGZ->m_bRotateWithBoat) {
            switch(m_pMouseGZ->m_iMaintainWith) {
                case ID_MAINTAIN_WITH_HEADING:
                    if(!wxIsNaN(g_pfFix.Hdt))
                        m_pMouseGZ->m_dFirstLineDirection -= g_pfFix.Hdt;
                    break;
                case ID_MAINTAIN_WITH_COG:
                    if(!wxIsNaN(g_pfFix.Cog))
                        m_pMouseGZ->m_dFirstLineDirection -= g_pfFix.Cog;
                    break;
            }
        }
    }
    
    ODPoint *pMousePoint = NULL;
    
    pMousePoint = new ODPoint( rlat, rlon, wxT(""), wxT(""), wxT("") );
    pMousePoint->SetNameShown( false );
    pMousePoint->SetTypeString( wxS("Guard Zone Point") );
    
    g_pODConfig->AddNewODPoint( pMousePoint, -1 );    // use auto next num
    g_pODSelect->AddSelectableODPoint( rlat, rlon, pMousePoint );
    
    //if( nGZ_State > 1 )
    // TODO fix up undo
    //undo->BeforeUndoableAction( Undo_AppendWaypoint, pMousePoint, Undo_IsOrphanded, NULL );
    
    if(m_pMouseGZ){
        m_pMouseGZ->AddPoint( pMousePoint );
        if( !(nGZ_State & 1) ) {
            pMousePoint->SetName(_("Second"));
            pMousePoint->SetIconName( g_sGZSecondIconName );
            DistanceBearingMercator_Plugin( rlat, rlon, g_pfFix.Lat, g_pfFix.Lon, &m_pMouseGZ->m_dSecondLineDirection, &m_pMouseGZ->m_dSecondDistance);
            m_pMouseGZ->UpdateGZSelectablePath();
            if(m_pMouseGZ->m_bRotateWithBoat) {
                switch(m_pMouseGZ->m_iMaintainWith) {
                    case ID_MAINTAIN_WITH_HEADING:
                        if(!wxIsNaN(g_pfFix.Hdt))
                            m_pMouseGZ->m_dSecondLineDirection -= g_pfFix.Hdt;
                        break;
                    case ID_MAINTAIN_WITH_COG:
                        if(!wxIsNaN(g_pfFix.Cog))
                            m_pMouseGZ->m_dSecondLineDirection -= g_pfFix.Cog;
                        break;
                }
            }
            // TODO fix up undo
            //undo->AfterUndoableAction( m_pMouseGZ );
            
            if(m_pMouseGZ->m_iPersistenceType == ID_PERSISTENT || m_pMouseGZ->m_iPersistenceType == ID_PERSISTENT_CRASH) {
                if(m_pMouseGZ->m_iPersistenceType == ID_PERSISTENT_CRASH)
                    m_pMouseGZ->m_bTemporary = true;
                else m_pMouseGZ->m_bTemporary = false;
                g_pODConfig->AddNewPath( m_pMouseGZ, -1 );    // don't save over restart
            }
        } else {
            pMousePoint->SetName(_("First"));
            pMousePoint->SetIconName( g_sGZFirstIconName );
        }
    }
    
    m_prev_rlat = rlat;
    m_prev_rlon = rlon;
    m_prev_pMousePoint = pMousePoint;
    if(m_pMouseGZ) {
        m_pMouseGZ->m_lastMousePointIndex = m_pMouseGZ->GetnPoints();
        m_pMouseGZ->RebuildGUIDList();
    }

    nGZ_State++;

    RequestRefresh( m_parent_window );
    
    return TRUE;
} 

bool ocpn_draw_pi::CreatePILLeftClick( wxMouseEvent &event )
{
    ODPoint *pMousePoint = NULL;
    double rlat, rlon;

    rlat = m_cursor_lat;
    rlon = m_cursor_lon;

    m_pMousePIL = new PIL();
    g_pPILList->Append( m_pMousePIL );
    g_pPathList->Append( m_pMousePIL );
    m_pMousePIL->m_PathNameString << _("PIL") << _T(" ") << g_pPILList->GetCount();
    m_dStartLat = g_pfFix.Lat;
    m_dStartLon = g_pfFix.Lon;

    ODPoint *beginPoint = new ODPoint( g_pfFix.Lat, g_pfFix.Lon, g_sPILStartIconName, _("Boat"), wxT("") );
    beginPoint->SetNameShown( false );
    beginPoint->SetTypeString( wxT("PIL Point"));
    beginPoint->m_bIsolatedMark = false;
    m_pMousePIL->AddPoint( beginPoint, false );

    pMousePoint = new ODPoint( rlat, rlon, g_sPILEndIconName, _("End"), wxT("") );

    pMousePoint->SetNameShown( false );
    pMousePoint->SetTypeString( wxS("PIL Point") );
    pMousePoint->m_bIsolatedMark = FALSE;
    m_pMousePIL->AddPoint( pMousePoint );
    m_pMousePIL->m_bCentreOnBoat = true;
    DistanceBearingMercator_Plugin(rlat, rlon, m_dStartLat, m_dStartLon, &m_pMousePIL->m_dEBLAngle, &m_pMousePIL->m_dLength);

    if(m_pMousePIL->m_bRotateWithBoat) {
        switch(m_pMousePIL->m_iMaintainWith) {
            case ID_MAINTAIN_WITH_HEADING:
                if(!wxIsNaN(g_pfFix.Hdt))
                    m_pMousePIL->m_dEBLAngle -= g_pfFix.Hdt;
                break;
            case ID_MAINTAIN_WITH_COG:
                if(!wxIsNaN(g_pfFix.Cog))
                    m_pMousePIL->m_dEBLAngle -= g_pfFix.Cog;
                break;
        }
    }

    m_pMousePIL->RebuildGUIDList();
    m_pMousePIL->AddLine( _T("Initial"), _T(""), g_dPILOffset);
    if(g_PILDefaultNumIndexLines >=0)
        m_pMousePIL->AddLine( _T("Second"), _T(""), -g_dPILOffset, false );

    if(m_pMousePIL->m_iPersistenceType == ID_PERSISTENT || m_pMousePIL->m_iPersistenceType == ID_PERSISTENT_CRASH)
        g_pODConfig->AddNewPath( m_pMousePIL, -1 );    // don't save over restart
    g_pODSelect->AddSelectableODPoint( rlat, rlon, pMousePoint );
    g_pODSelect->AddSelectablePathSegment( g_pfFix.Lat, g_pfFix.Lon, rlat, rlon, beginPoint, pMousePoint, m_pMousePIL );

    nPIL_State++;

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
        ODPath *pTempPath = node->GetData();
        if( !pTempPath )
            continue;

        ODPath *pPathDraw = NULL;
        Boundary *pBoundaryDraw;
        {
        EBL *pEBLDraw;
        DR *pDRDraw;
        GZ *pGZDraw;
        PIL *pPILDraw;
        
        if(pTempPath->m_sTypeString == wxT("Boundary")) {
            pBoundaryDraw = dynamic_cast<Boundary *>(pTempPath);
            pPathDraw = pBoundaryDraw;
        } else if(pTempPath->m_sTypeString == wxT("EBL")) {
            pEBLDraw = dynamic_cast<EBL *>(pTempPath);
            pPathDraw = pEBLDraw;
        } else if(pTempPath->m_sTypeString == wxT("DR")) {
            pDRDraw = dynamic_cast<DR *>(pTempPath);
            pPathDraw = pDRDraw;
        } else if(pTempPath->m_sTypeString == wxT("Guard Zone")) {
            pGZDraw = dynamic_cast<GZ *>(pTempPath);
            pPathDraw = pGZDraw;
        } else if(pTempPath->m_sTypeString == wxT("PIL")) {
            pPILDraw = dynamic_cast<PIL *>(pTempPath);
            pPathDraw = pPILDraw;
        }
        assert(pPathDraw != 0);
        }
        /* defer rendering active routes until later */ 
        //if( pPathDraw->IsActive() || pPathDraw->IsSelected() )
        //            continue;
        
        /* this routine is called very often, so rather than using the
        *       wxBoundingBox::Intersect routine, do the comparisons directly
        *       to reduce the number of floating point comparisons */
        
        //        const wxBoundingBox &vp_box = vp.GetBBox(), &test_box = pPathDraw->GetBBox();
        const LLBBox &test_box = pPathDraw->GetBBox();
        double test_miny = test_box.GetMinLat(), test_maxy = test_box.GetMaxLat();
        
        if(test_maxy < pivp.lat_min)
            continue;
        
        if(test_miny > pivp.lat_max)
            continue;
        
        double test_minx = test_box.GetMinLon(), test_maxx = test_box.GetMaxLon();
        
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
        if(pPathDraw == m_pSelectedEBL && m_bODPointEditing && !m_pSelectedEBL->m_bAlwaysShowInfo) {
            ODDC dc;
            double brg, dist;
            wxPoint destPoint;
            ODPoint *pStartPoint = m_pSelectedEBL->m_pODPointList->GetFirst()->GetData();
            ODPoint *pEndPoint = m_pSelectedEBL->m_pODPointList->GetLast()->GetData();
            DistanceBearingMercator_Plugin( pEndPoint->m_lat, pEndPoint->m_lon, pStartPoint->m_lat, pStartPoint->m_lon, &brg, &dist );
            GetCanvasPixLL( m_pVP, &destPoint, pEndPoint->m_lat, pEndPoint->m_lon);
            wxString info = CreateExtraPathLegInfo(dc, m_pSelectedEBL, brg, dist, destPoint);
            if(info.length() > 0)
                RenderExtraPathLegInfo( dc, destPoint, info );
        } else if(pPathDraw == m_pSelectedPIL  && m_bPathEditing) {
            ODDC dc;
            wxString info;
            if(m_iEditMode == ID_PIL_MENU_MOVE_INDEX_LINE)  {
                std::list<PILLINE>::iterator it = m_pSelectedPIL->m_PilLineList.begin();
                while (it != m_pSelectedPIL->m_PilLineList.end()) {
                    if (it->iID == m_iPILId) break;
                    ++it;
                }
                if (it != m_pSelectedPIL->m_PilLineList.end()) {
                    info = CreateExtraPathLegInfo(dc, m_pSelectedPIL, m_pSelectedPIL->m_dEBLAngle, it->dOffset, m_cursorPoint);
                }
            }
            else
                info = CreateExtraPathLegInfo(dc, m_pSelectedPIL, m_pSelectedPIL->m_dEBLAngle, m_pSelectedPIL->m_dLength, m_cursorPoint);
            
            if(info.length() > 0)
                RenderExtraPathLegInfo(dc, m_cursorPoint, info);
        }
        
    }
        
    /* ODPoints not drawn as part of routes */
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
        if(NULL == box) {
            free(d);
            return;
        }
        
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

void ocpn_draw_pi::SetToolbarTool( void )
{
    if ( nBoundary_State == 1 || nPoint_State == 1 || nTextPoint_State == 1 || nEBL_State == 1 || nDR_State == 1 || nGZ_State == 1 || nPIL_State == 1) {
        if (m_Mode > m_numModes ) m_Mode = 0;
        switch (m_Mode)
        {
            case ID_MODE_BOUNDARY:
                // Boundary
                m_pCurrentCursor = m_pCursorPencil;
#ifdef ODraw_USE_SVG
                SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_boundary_grey, m_pODicons->m_s_ocpn_draw_boundary, m_pODicons->m_s_ocpn_draw_boundary);
#else
                SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_boundary_grey, m_pODicons->m_p_bm_ocpn_draw_boundary);
#endif
                SetToolbarItemState( m_draw_button_id, true );
                nBoundary_State = 1;
                nPoint_State = 0;
                nTextPoint_State = 0;
                nEBL_State = 0;
                nDR_State = 0;
                nGZ_State = 0;
                nPIL_State = 0;
                break;
                
            case ID_MODE_POINT:
                // Point
                m_pCurrentCursor = m_pCursorCross;
#ifdef ODraw_USE_SVG
                SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_point_grey, m_pODicons->m_s_ocpn_draw_point, m_pODicons->m_s_ocpn_draw_point);
#else
                SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_point_grey, m_pODicons->m_p_bm_ocpn_draw_point);
#endif
                SetToolbarItemState( m_draw_button_id, true );
                nBoundary_State = 0;
                nPoint_State = 1;
                nTextPoint_State = 0;
                nEBL_State = 0;
                nDR_State = 0;
                nGZ_State = 0;
                nPIL_State = 0;
                break;
                
            case ID_MODE_TEXT_POINT:
                // Text Point
                m_pCurrentCursor = m_pTextCursorCross;
#ifdef ODraw_USE_SVG
                SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_textpoint_grey, m_pODicons->m_s_ocpn_draw_textpoint, m_pODicons->m_s_ocpn_draw_textpoint);
#else
                SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_textpoint_grey, m_pODicons->m_p_bm_ocpn_draw_textpoint);
#endif
                SetToolbarItemState( m_draw_button_id, true );
                nPoint_State = 0;
                nBoundary_State = 0;
                nTextPoint_State = 1;
                nEBL_State = 0;
                nDR_State = 0;
                nGZ_State = 0;
                nPIL_State = 0;
                break;
                
            case ID_MODE_EBL:
                // EBL
                m_pCurrentCursor = m_pCursorCross;
#ifdef ODraw_USE_SVG
                SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_ebl_grey, m_pODicons->m_s_ocpn_draw_ebl, m_pODicons->m_s_ocpn_draw_ebl);
#else
                SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_ebl_grey, m_pODicons->m_p_bm_ocpn_draw_ebl);
#endif
                SetToolbarItemState( m_draw_button_id, true );
                nPoint_State = 0;
                nBoundary_State = 0;
                nTextPoint_State = 0;
                nEBL_State = 1;
                nDR_State = 0;
                nGZ_State = 0;
                nPIL_State = 0;
                RequestRefresh( m_parent_window );
                break;
                
            case ID_MODE_DR:
                // EBL
                m_pCurrentCursor = m_pCursorCross;
#ifdef ODraw_USE_SVG
                SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_dr_grey, m_pODicons->m_s_ocpn_draw_dr, m_pODicons->m_s_ocpn_draw_dr);
#else
                SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_dr_grey, m_pODicons->m_p_bm_ocpn_draw_dr);
#endif
                SetToolbarItemState( m_draw_button_id, true );
                nPoint_State = 0;
                nBoundary_State = 0;
                nTextPoint_State = 0;
                nEBL_State = 0;
                nDR_State = 1;
                nGZ_State = 0;
                nPIL_State = 0;
                RequestRefresh( m_parent_window );
                break;
                
            case ID_MODE_GZ:
                // Guard Zone
                m_pCurrentCursor = m_pCursorPencil;
#ifdef ODraw_USE_SVG
                SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_gz_grey, m_pODicons->m_s_ocpn_draw_gz, m_pODicons->m_s_ocpn_draw_gz);
#else
                SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_gz_grey, m_pODicons->m_p_bm_ocpn_draw_gz);
#endif
                SetToolbarItemState( m_draw_button_id, true );
                nPoint_State = 0;
                nBoundary_State = 0;
                nTextPoint_State = 0;
                nEBL_State = 0;
                nDR_State = 0;
                nGZ_State = 1;
                nPIL_State = 0;
                RequestRefresh( m_parent_window );
                break;
                
            case ID_MODE_PIL:
                // PIL
                m_pCurrentCursor = m_pCursorCross;
                #ifdef ODraw_USE_SVG
                SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_pil_grey, m_pODicons->m_s_ocpn_draw_pil, m_pODicons->m_s_ocpn_draw_pil);
                #else
                SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_pil_grey, m_pODicons->m_p_bm_ocpn_draw_pil);
                #endif
                SetToolbarItemState( m_draw_button_id, true );
                nPoint_State = 0;
                nBoundary_State = 0;
                nTextPoint_State = 0;
                nEBL_State = 0;
                nDR_State = 0;
                nGZ_State = 0;
                nPIL_State = 1;
                RequestRefresh( m_parent_window );
                break;

            default:
                // Boundary
                m_Mode = ID_MODE_BOUNDARY;
                m_pCurrentCursor = m_pCursorPencil;
#ifdef ODraw_USE_SVG
                SetToolbarToolBitmapsSVG(m_draw_button_id, m_pODicons->m_s_ocpn_draw_boundary_grey, m_pODicons->m_s_ocpn_draw_boundary, m_pODicons->m_s_ocpn_draw_boundary);
#else
                SetToolbarToolBitmaps(m_draw_button_id, m_pODicons->m_p_bm_ocpn_draw_boundary_grey, m_pODicons->m_p_bm_ocpn_draw_boundary);
#endif
                g_pODToolbar->SetToolbarTool( ID_MODE_BOUNDARY );
                SetToolbarItemState( m_draw_button_id, true );
                nBoundary_State = 1;
                nPoint_State = 0;
                nTextPoint_State = 0;
                nEBL_State = 0;
                nGZ_State = 0;
                break;
        }
    }
    
}

