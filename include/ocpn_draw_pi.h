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
#ifndef _OCPNDRAWPI_H_
#define _OCPNDRAWPI_H_

#ifdef __WXMSW__
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <sstream>

#  define DEBUGSL(x) do { \
std::ostringstream oss; \
oss << x; \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::string s1(oss.str()); \
std::string s = stime; \
s += " :: "; \
s += s1; \
s += "\n"; \
std::wstring stemp = std::wstring(s.begin(), s.end()); \
LPCWSTR sw = stemp.c_str(); \
OutputDebugString(sw); } while (0) 

#  define DEBUGST(x) do { \
std::string s(""); \
std::ostringstream oss; \
oss << x; \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
do { \
std::string s1(oss.str()); \
s += stime; \
s += " :: "; \
s += s1; } while (0);

#  define DEBUGCONT(x) do { \
std::ostringstream oss; \
oss << x; \
std::string s1(oss.str()); \
s += s1 ; } while (0);

#  define DEBUGEND(x) do { \
std::string s1(""); \
std::ostringstream oss; \
oss << x; \
s1 = oss.str(); \
s += s1; } while (0); \
s += "\n" ; \
std::wstring stemp = std::wstring(s.begin(), s.end()); \
LPCWSTR sw = stemp.c_str(); \
OutputDebugString(sw); } while (0) 
#else
#  define DEBUGSL(x) do {} while (0)
#  define DEBUGST(x) do {} while (0)
#  define DEBUGCONT(x) do {} while (0)
#  define DEBUGEND(x) do {} while (0)
#endif
#else
#ifdef DEBUG_BUILD
#  define DEBUGSL(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::cout << stime << " :: "; \
std::cout << x << std::endl ;} while (0)

#  define DEBUGST(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::cout << stime << " :: " ; \
std::cout << x; } while (0)

#  define DEBUGCONT(x) do { \
std::cout << x ; } while (0)

#  define DEBUGEND(x) do { \
std::cout << x  << std::endl ; } while (0)
#else
#  define DEBUGSL(x) do {} while (0)
#  define DEBUGST(x) do {} while (0)
#  define DEBUGCONT(x) do {} while (0)
#  define DEBUGEND(x) do {} while (0)
#endif
#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

typedef enum ColorScheme : int
{
    GLOBAL_COLOR_SCHEME_RGB,
    GLOBAL_COLOR_SCHEME_DAY,
    GLOBAL_COLOR_SCHEME_DUSK,
    GLOBAL_COLOR_SCHEME_NIGHT,
    N_COLOR_SCHEMES
}_ColorScheme;

#include "wxWTranslateCatalog.h"
#include "ocpn_plugin.h"
#include "globals.h"
#include "ocpn_plugin.h"
#include "ODUndo.h"
#include "ODPoint.h"
#include "ODConfig.h"
#include "ODRolloverWin.h"

//#include "georef.h"
#include "wx28compat.h"
#include <wx/aui/aui.h>
#include <wx/string.h>
#include <wx/settings.h>
#include <wx/statline.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>
#include <wx/splitter.h>
#include <wx/fileconf.h>
#include <wx/dynarray.h>
#include <list>

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define OCPN_DRAW_POSITION -1
#define ID_NONE -1
//    Constants for right click menus
enum
{
    ID_MENU_NOITEM = 0,
    ID_ODPOINT_MENU_GOTO,
    ID_ODPOINT_MENU_MOVE,
    ID_ODPOINT_MENU_DELPOINT,
    ID_ODPOINT_MENU_PROPERTIES,
    ID_ODPOINT_MENU_COPY,
    ID_ODPOINT_MENU_COPY_GUID,

    ID_PATH_MENU_ACTIVATE,
    ID_PATH_MENU_DEACTIVATE,
    ID_PATH_MENU_MOVE_POINT,
    ID_PATH_MENU_MOVE_PATH,
    ID_PATH_MENU_MOVE_PATH_SEGMENT,
    ID_PATH_MENU_INSERT,
    ID_PATH_MENU_APPEND,
    ID_PATH_MENU_COPY,
    ID_OCPNPOPINT_MENU_COPY,
    ID_PASTE_OCPNPOINT,
    ID_PASTE_PATH,
    ID_PATH_MENU_DELETE,
    ID_PATH_MENU_REVERSE,
    ID_PATH_MENU_DELPOINT,
    ID_PATH_MENU_ACTPOINT,
    ID_PATH_MENU_DEACTPOINT,
    ID_PATH_MENU_ACTNXTPOINT,
    ID_PATH_MENU_REMPOINT,
    ID_PATH_MENU_PROPERTIES,
    ID_PATH_MENU_COPY_GUID,
    ID_PATH_MENU_SHOW_ICONS,
    ID_EBL_MENU_CENTRE_ON_BOAT,
    ID_EBL_MENU_CENTRE_ON_BOAT_LATLON,
    ID_EBL_MENU_PICK_NEW_START,
    ID_EBL_MENU_VRM_MATCH_EBL_COLOUR,
    ID_DR_MENU_UPDATE_INITIAL_CONDITIONS,
    ID_BOUNDARY_LIST_KEEP_MENU,
    ID_BOUNDARY_LIST_DELETE_MENU,
    ID_PIL_MENU_INDEX_LINE_PROPERTIES,
    ID_PIL_MENU_ADD_INDEX_LINE,
    ID_PIL_MENU_MOVE_INDEX_LINE,
    ID_PIL_MENU_DELETE_INDEX_LINE,
    ID_PIL_MENU_CENTRE_ON_BOAT,
    ID_PIL_MENU_CENTRE_ON_BOAT_LATLON,
    ID_PATH_MGR_PATH_RIGHT_CLICK_HIDE,
    ID_PATH_MGR_PATH_RIGHT_CLICK_SHOW,
    ID_PATH_MGR_ODPOINT_RIGHT_CLICK_HIDE,
    ID_PATH_MGR_ODPOINT_RIGHT_CLICK_SHOW,
    ID_PATH_MGR_LAYER_RIGHT_CLICK_HIDE,
    ID_PATH_MGR_LAYER_RIGHT_CLICK_SHOW,
    
    ID_UNDO,
    ID_REDO,

    ID_ODPOINT_MENU_ADDITIONAL_INFO,

    ID_DEF_MENU_GROUPBASE,  

    
    ID_DEF_MENU_LAST
};

//Constants for the drawing mode
enum
{
    ID_MODE_BOUNDARY = 0,
    ID_MODE_POINT,
    ID_MODE_TEXT_POINT,
    ID_MODE_EBL,
    ID_MODE_DR,
    ID_MODE_GZ,
    ID_MODE_PIL,
    
    ID_MODE_LAST
};

// Text Point Text information
enum {
    ID_TEXT_TOP = 0,
    ID_TEXT_CENTRE_TOP,
    ID_TEXT_BOTTOM,
    ID_TEXT_CENTRE_BOTTOM,
    ID_TEXT_CENTRE,
    ID_TEXT_RIGHT,
    ID_TEXT_LEFT,
    
    ID_TEXT_POSTION_LAST
};

enum {
    ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ALWAYS = 0,
    ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ON_ROLLOVER,
    ID_TEXTPOINT_DISPLAY_TEXT_SHOW_NEVER
};


// Boundary types
enum {
    ID_BOUNDARY_EXCLUSION = 0,
    ID_BOUNDARY_INCLUSION,
    ID_BOUNDARY_NEITHER,
    ID_BOUNDARY_ANY,
    
    ID_BOUNDARY_TYPE_LAST
};

// Path states
enum {
    ID_PATH_STATE_ANY = 0,
    ID_PATH_STATE_ACTIVE,
    ID_PATH_STATE_INACTIVE,
    
    ID_PATH_STATE_LAST
};

// Point states
enum {
    ID_POINT_STATE_ANY = 0,
    ID_POINT_STATE_ACTIVE,
    ID_POINT_STATE_INACTIVE,
    
    ID_POINT_STATE_LAST
};

enum {
    ID_PERSISTENT = 0,
    ID_PERSISTENT_CRASH,
    ID_NOT_PERSISTENT,
    
    ID_PERSISTENT_LAST
};

enum {
    ID_NAUTICAL_MILES = 0,
    ID_KILOMETERS
};

#define SELTYPE_UNKNOWN             0x0001
#define SELTYPE_ODPOINT             0x0002
#define SELTYPE_PATHSEGMENT         0x0004
#define SELTYPE_PATHCREATE          0x0008
#define SELTYPE_BOUNDARYLIST        0x0010
#define SELTYPE_PIL                 0x0020
#define TYPE_PATHMGR_PATH_DLG       0x0040
#define TYPE_PATHMGR_POINT_DLG      0x0080
#define TYPE_PATHMGR_LAYER_DLG      0x0100

class Boundary;
class BoundaryProp;
class Boundarylist;
class EBL;
class DR;
class GZ;
class PIL;
class SelectItem;
class ODicons;

const int StyleValues[] = { wxPENSTYLE_SOLID, wxPENSTYLE_DOT, wxPENSTYLE_LONG_DASH, wxPENSTYLE_SHORT_DASH, wxPENSTYLE_DOT_DASH };
const int WidthValues[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

class  ODPlugIn_Position_Fix_Ex : public PlugIn_Position_Fix_Ex
{
public:
    bool    valid;
    bool    validHdt;
    bool    validHdm;
    bool    validCog;
};


class ocpn_draw_pi : public opencpn_plugin_116
{
public:

    ocpn_draw_pi(void *ppimgr);
    ~ocpn_draw_pi();

    wxWindow            *m_parent_window;
    wxFileConfig        *m_pODConfig;
//    ODConfig        *m_pODConfig;
    wxTimer             *m_timer;
    wxColour            col,col1,gridline,uitext,udkrd,back_color,text_color;
    wxColour            mcol,mcol1,mgridline, muitext,mudkrd,mback_color,mtext_color;
    wxString            lastODPointInPath;

    wxLocale*        local;
    int                state;
    bool            eventsEnabled;
    int             m_iCallerId;
    bool            m_bBoundaryEditing;
    bool            m_bPathEditing;
    bool            m_bODPointEditing;
    bool            m_bTextPointEditing;
    bool            m_bEBLEditing;
    bool            m_bEBLMoveOrigin;
    bool            m_bGZEditing;
    int             m_iEditMode;

    

    //    The required PlugIn Methods
    int Init(void);
    bool DeInit(void);

    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    wxBitmap *GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();
    void UpdateAuiStatus(void);
//    void SetColorScheme(PI_ColorScheme cs);
    void GetOriginalColors();
    void SetOriginalColors();
    void LateInit(void);

    //    The optional method overrides
    void OnContextMenuItemCallback(int id);
    void latlong_to_chartpix(double lat, double lon, double &pixx, double &pixy);
    void SetColorScheme(PI_ColorScheme cs);
    void SendVectorChartObjectInfo(wxString &chart, wxString &feature, wxString &objname, double lat, double lon, double scale, int nativescale) ;

    //    The required override PlugIn Methods
    //     bool RenderOverlay(wxMemoryDC *pmdc, PlugIn_ViewPort *vp);
    //      void SetCursorLatLon(double lat, double lon);


    void SetDefaults(void);
    int GetToolbarToolCount(void);
    void ShowPreferencesDialog( wxWindow* parent );
    void OnToolbarToolDownCallback(int id);
    void OnToolbarToolUpCallback(int id);
    void SetPluginMessage(wxString &message_id, wxString &message_body);

    void loadLayouts(wxWindow * parent);
//    void startLogbook();
    void shutdown(bool menu);
    
    bool MouseEventHook( wxMouseEvent &event );
    bool KeyboardEventHook( wxKeyEvent &event );
    void SetCursorLatLon(double lat, double lon);
    void SetCurrentViewPort(PlugIn_ViewPort &vp);
    bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
    bool RenderGLOverlays(wxGLContext *pcontext, PlugIn_ViewPort *vp);
    bool RenderGLOverlayMultiCanvas(wxGLContext *pcontext, PlugIn_ViewPort *vp, int canvas_index);
    bool RenderOverlay(wxMemoryDC *pmdc, PlugIn_ViewPort *vp);
    bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
    bool RenderOverlays(wxDC &dc, PlugIn_ViewPort *pivp);
    bool RenderOverlayMultiCanvas(wxDC &dc, PlugIn_ViewPort *vp, int canvas_index);
    wxString FormatDistanceAdaptive( double distance );
    void DrawAllPathsInBBox(ODDC &dc,  LLBBox& BltBBox);
    void DrawAllPathsAndODPoints( PlugIn_ViewPort &pivp );
    void DrawAllODPointsInBBox( ODDC &dc, LLBBox& BltBBox );
    void CanvasPopupMenu( int x, int y, int seltype );
    double  GetTrueOrMag(double a);
    void SetPositionFixEx( PlugIn_Position_Fix_Ex &pfix );
    
    void RenderPathLegs( ODDC &dc );
    
    // OD Methods
    void    ProcessTimerEvent(wxTimerEvent& ev);
    void    PopupMenuHandler(wxCommandEvent& ev);
    
    void    SaveConfig( void );
    
    void    AlphaBlending( ODDC &dc, int x, int y, int size_x, int size_y, float radius, wxColour color, unsigned char transparency );

    void    SetToolbarTool( void );
    
    void    RenderExtraPathLegInfo(ODDC &dc, wxPoint ref_point, wxString s );
    wxString CreateExtraPathLegInfo(ODDC &dc, ODPath *path, double brg, double dist, wxPoint ref_point);

    void    ODRequestRefresh( int canvas_index, bool bFullRefresh = FALSE );
    
    wxCursor    *pCursorLeft;
    wxCursor    *pCursorRight;
    wxCursor    *pCursorUp;
    wxCursor    *pCursorDown;

    wxCursor    *pCursorUpLeft;
    wxCursor    *pCursorUpRight;
    wxCursor    *pCursorDownLeft;
    wxCursor    *pCursorDownRight;
    wxCursor    *pCursorArrow;
    wxCursor    *m_pCursorCross;
    wxCursor    *m_pCursorPencil;
    wxCursor    *m_pTextCursorCross;
    
    wxCursor    *m_pCurrentCursor;
    
    int         nPath_State;
    int         nBoundary_State;
    int         nPoint_State;
    int         nTextPoint_State;
    int         nEBL_State;
    int         nDR_State;
    int         nGZ_State;
    int         nPIL_State;
    bool        bKey_Path_Pressed;
    bool        bKey_Boundary_Pressed;
    bool        bKey_Point_Pressed;
    bool        bKey_TextPoint_Pressed;
    bool        bKey_EBL_Pressed;
    bool        bKey_DR_Pressed;
    bool        bKey_GZ_Pressed;
    bool        bKey_PIL_Pressed;
    Boundary    *m_pMouseBoundary;
    EBL         *m_pMouseEBL;
    GZ          *m_pMouseGZ;
    PIL         *m_pMousePIL;
    ODPoint     *m_pEBLBoatPoint;
    ODPath        *m_pSelectedPath;
    int         m_iPILId;
    ODPoint   *m_pFoundODPoint;
    ODPoint   *m_pFoundODPointSecond;
    wxPoint     r_rband;
    double      m_dStartLat;
    double      m_dStartLon;
    double      m_cursor_lon, m_cursor_lat;
    double      m_prev_rlat, m_prev_rlon;
    double      m_true_scale_ppm;
    ODPoint   *m_prev_pMousePoint;
    bool           m_disable_edge_pan;
    double      m_lat, m_lon;
    double      m_pixx, m_pixy;
    wxPoint     m_cursorPoint;
    
    Undo        *undo;
    
    wxGLContext     *m_pcontext;
    wxMemoryDC      *pmdc;
//    wxGLCanvas      *m_glcc;
    
    int         nBlinkerTick;
    int         m_Mode;
    int         m_config_button_id;
    int         m_draw_button_id;
    
    void    appendOSDirSlash(wxString* pString);  
    
    double  m_chart_scale;
    double  m_view_scale;
    
    ODicons     *m_pODicons;
    
    bool        m_bRecreateConfig;
    
    int         m_drawing_canvas_index;
    
private:
    void    OnTimer(wxTimerEvent& ev);

    void    LoadConfig();
    void    FinishBoundary();
//    ArrayOfGridColWidth    readCols(ArrayOfGridColWidth ar, wxString str);
//    void                    writeCols(wxFileConfig *pConf, ArrayOfGridColWidth ar, wxString entry);
//    ArrayOfGridColWidth    readColsOld(wxFileConfig *pConf, ArrayOfGridColWidth ar, wxString entry);
    void                    dialogDimmer(PI_ColorScheme cs,wxWindow* ctrl,wxColour col,wxColour col1, wxColour back_color,wxColour text_color, wxColour uitext, wxColour udkrd);
//    wxAuiManager     *m_pauimgr;

    bool    CreateBoundaryLeftClick(wxMouseEvent &event);
    bool    CreatePointLeftClick( wxMouseEvent &event );
    bool    CreateTextPointLeftClick( wxMouseEvent &event );
    bool    CreateEBLLeftClick( wxMouseEvent &event );
    bool    CreateDRLeftClick( wxMouseEvent &event );
    bool    CreateGZLeftClick( wxMouseEvent &event );
    bool    CreatePILLeftClick( wxMouseEvent &event );

    void    MenuPrepend( wxMenu *menu, int id, wxString label);
    void    MenuAppend( wxMenu *menu, int id, wxString label);
    void    FindSelectedObject( void );
    
    void    ItemProcess(int id);
    
    wxTimer         m_RolloverPopupTimer;
    
    PlugIn_ViewPort m_VP;
    
    int     m_show_id;
    int     m_hide_id;
    int     m_iODToolContextId;
    wxMenuItem *m_pODToolContextMenuItem;
    bool    show;

    bool    m_bLOGShowIcon;
    
    Boundary    *m_pSelectedBoundary;
    EBL         *m_pSelectedEBL;
    DR          *m_pSelectedDR;
    GZ          *m_pSelectedGZ;
    PIL         *m_pSelectedPIL;

    bool        m_bDrawingBoundary;
    bool        m_bDrawingGZ;
    
    int         popx, popy;
    wxString m_sNavObjSetChangesFile;
    
    wxString    m_Data;

    int         m_numModes;

    int         m_rollover_popup_timer_msec;
    
    int         m_seltype;
    
    double      m_PathMove_cursor_start_lat;
    double      m_PathMove_cursor_start_lon;
    
    wxDateTime  m_LastFixTime;
    
    int         m_mouse_canvas_index;
    int         m_current_canvas_index;
    
    std::list<Boundary*> m_pBoundaryList;
    
    
};

#endif



