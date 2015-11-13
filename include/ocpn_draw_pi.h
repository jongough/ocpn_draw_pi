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

#ifdef DEBUG_BUILD
#  define DEBUGSL(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::cout << stime << " : " << x << std::endl; } while (0)

#  define DEBUGST(x) do { \
time_t now = time(0); \
tm* localtm = localtime(&now); \
char *stime = asctime(localtm); \
stime[strlen(stime) - 1 ] = 0; \
std::cout << stime << " : " << x; } while (0)

#  define DEBUGCONT(x) do { \
std::cout << x ; } while (0)

#  define DEBUGEND(x) do { \
std::cout << x << std::endl; } while (0)
#else
#  define DEBUGSL(x) do {} while (0)
#  define DEBUGST(x) do {} while (0)
#  define DEBUGCONT(x) do {} while (0)
#  define DEBUGEND(x) do {} while (0)
#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifndef _OCPNDRAWPI_H_
#define _OCPNDRAWPI_H_

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "wx/wxprec.h"

#include "ocpn_plugin.h"
#include "undo.h"
#include "ODPoint.h"
#include "ODConfig.h"
#include "pathmanagerdialog.h"
#include "ODRolloverWin.h"

#include "georef.h"
//#include "chartbarwin.h"

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

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

#define OCPN_DRAW_POSITION -1

//    Constants for right click menus
enum
{
    ID_OCPNPOINT_MENU_GOTO = 1,
    ID_OCPNPOINT_MENU_MOVE,
    ID_OCPNPOINT_MENU_DELPOINT,
    ID_OCPNPOINT_MENU_PROPERTIES,
    ID_OCPNPOINT_MENU_COPY,

    ID_PATH_MENU_ACTIVATE,
    ID_PATH_MENU_DEACTIVATE,
    ID_PATH_MENU_MOVE_POINT,
    ID_PATH_MENU_MOVE_PATH,
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
    ID_EBL_MENU_CENTRE_ON_BOAT,
    ID_EBL_MENU_CENTRE_ON_BOAT_LATLON,
    ID_EBL_MENU_PICK_NEW_START,
    ID_EBL_MENU_VRM_MATCH_EBL_COLOUR,

    ID_UNDO,
    ID_REDO,

    ID_OCPNPOINT_MENU_ADDITIONAL_INFO,

    ID_DEF_MENU_GROUPBASE,  // Must be last entry, as chart group identifiers are created dynamically

    
    ID_DEF_MENU_LAST
};

//Constants for the drawing mode
enum
{
    ID_MODE_BOUNDARY = 0,
    ID_MODE_POINT,
    ID_MODE_TEXT_POINT,
    ID_MODE_EBL,
    
    ID_LAST_MODE
};

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

// Boundary types
enum {
    ID_BOUNDARY_EXCLUSION = 0,
    ID_BOUNDARY_INCLUSION,
    ID_BOUNDARY_NONE,
    
    ID_BOUNDARY_TYPE_LAST
};

class Boundary;
class BoundaryProp;
class EBL;
class SelectItem;

const int StyleValues[] = { wxSOLID, wxDOT, wxLONG_DASH, wxSHORT_DASH, wxDOT_DASH };
const int WidthValues[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

extern DECL_EXP bool FindPointInBoundary( double lat, double lon );
extern DECL_EXP bool FindPointInBoundary( Boundary *pBoundary, double lat, double lon);
extern DECL_EXP wxString GetBoundaryWithPointInBoundary( double lat, double lon );

class  ODPlugIn_Position_Fix_Ex : public PlugIn_Position_Fix_Ex
{
public:
    bool    valid;
};


class ocpn_draw_pi : public opencpn_plugin_113
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
    bool RenderOverlay(wxMemoryDC *pmdc, PlugIn_ViewPort *vp);
    bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
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
    
    void    SaveConfig();
    
    void    AlphaBlending( ODDC &dc, int x, int y, int size_x, int size_y, float radius, wxColour color, unsigned char transparency );

    void    DimeControl( wxWindow* ctrl );
    void    DimeControl( wxWindow* ctrl, wxColour col, wxColour window_back_color, wxColour ctrl_back_color,
                  wxColour text_color, wxColour uitext, wxColour udkrd, wxColour gridline );
    void    SetToolbarTool( void );

    wxCursor    *pCursorLeft;
    wxCursor    *pCursorRight;
    wxCursor    *pCursorUp;
    wxCursor    *pCursorDown;

    wxCursor    *pCursorUpLeft;
    wxCursor    *pCursorUpRight;
    wxCursor    *pCursorDownLeft;
    wxCursor    *pCursorDownRight;
    wxCursor    *pCursorPencil;
    wxCursor    *pCursorArrow;
    wxCursor    *pCursorCross;
    wxCursor    *m_pTextCursorCross;
    
    wxCursor    *m_pCurrentCursor;
    
    int         nConfig_State;
    
    int         nPath_State;
    int         nBoundary_State;
    Boundary    *m_pMouseBoundary;
    EBL         *m_pMouseEBL;
    ODPoint     *m_pEBLBoatPoint;
    ODPath        *m_pSelectedPath;
    ODPoint   *m_pFoundODPoint;
    ODPoint   *m_pFoundODPointSecond;
    wxPoint     r_rband;
    int         nPoint_State;
    int         nTextPoint_State;
    int         nEBL_State;
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
    
    PlugIn_ViewPort *m_vp;
    wxGLContext     *m_pcontext;
    wxMemoryDC      *pmdc;
//    wxGLCanvas      *m_glcc;
    
    int         nBlinkerTick;
    int         m_Mode;
    int                m_draw_button_id;
    
    void    appendOSDirSlash(wxString* pString);  
    
    double  m_chart_scale;
    double  m_view_scale;
    

private:
    void    OnTimer(wxTimerEvent& ev);

    void    LoadConfig();
    void    RenderExtraPathLegInfo(ODDC &dc, wxPoint ref_point, wxString s );
    wxString CreateExtraPathLegInfo(ODDC &dc, ODPath *path, double brg, double dist, wxPoint ref_point);
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
    
    void    MenuPrepend( wxMenu *menu, int id, wxString label);
    void    MenuAppend( wxMenu *menu, int id, wxString label);
    void    FindSelectedObject( void )    ;
    
    wxTimer         m_RolloverPopupTimer;
    
    
    int               m_show_id;
    int               m_hide_id;
    bool                show;
    int                m_config_button_id;

    bool              m_bLOGShowIcon;
    PI_ColorScheme               global_color_scheme;
    
    Boundary    *m_pSelectedBoundary;
    EBL         *m_pSelectedEBL;
    
    bool        m_bDrawingBoundary;
    
    int         popx, popy;
    wxString m_sNavObjSetChangesFile;
    
    wxString    m_Data;

    int         m_numModes;

    int         m_rollover_popup_timer_msec;
    
    int         m_seltype;
    
    double      m_PathMove_cursor_start_lat;
    double      m_PathMove_cursor_start_lon;
    
    wxDateTime  m_LastFixTime;
    
};

#endif



