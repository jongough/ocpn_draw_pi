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
#ifndef _OCPNDRAWGLOBALS_H_
#define _OCPNDRAWGLOBALS_H_
class ocpn_draw_pi;
class PathList;
class PointMan;
class ODSelect;
class ODConfig;
class PathMan;
class BoundaryMan;
class GZMan;
class ODPathPropertiesDialogImpl;
class BoundaryProp;
class EBLProp;
class DRProp;
class GZProp;
class ODLinkPropertiesDialogImpl;
class PILProp;
class PathAndPointManagerDialogImpl;
class ODPointPropertiesImpl;
class ODPropertiesDialogImpl;
class PILPropertiesDialogImpl;
class ODDRDialogImpl;
class BoundaryList;
class EBLList;
class DRList;
class GZList;
class PILList;
class ODPath;
class ODRolloverWin;
class ODEventHandler;
class ODLayerList;
class ODToolbarImpl;
class ODJSON;
class ODPlugIn_Position_Fix_Ex;
class ODAPI;
class ODDC;

class PlugIn_ViewPort;

class SelectItem;



extern ocpn_draw_pi            *g_ocpn_draw_pi;
extern wxString                *g_ocpn_draw_display_name;
extern wxWindow                *g_parent_window;
extern PathList                *g_pPathList;
extern PointMan                *g_pODPointMan;
extern bool                    g_bODIsNewLayer;
extern int                     g_ODLayerIdx;
extern int                     g_path_line_width;
extern wxString                g_OD_default_wp_icon;
extern double                  g_dLat;
extern double                  g_dLon;
extern int                     g_cursor_x;
extern int                     g_cursor_y;
extern ODSelect                *g_pODSelect;
extern ODConfig                *g_pODConfig;
extern wxString                *g_SData_Locn;
extern PathMan                 *g_pPathMan;
extern BoundaryMan             *g_pBoundaryMan;
extern GZMan                   *g_pGZMan;
extern wxString                g_default_ODPoint_icon;
extern ODPathPropertiesDialogImpl   *g_pODPathPropDialog;
extern BoundaryProp            *g_pBoundaryPropDialog;
extern EBLProp                 *g_pEBLPropDialog;
extern DRProp                  *g_pDRPropDialog;
extern GZProp                  *g_pGZPropDialog;
extern ODLinkPropertiesDialogImpl *g_pODLinkPropertiesDialog;
extern PILProp                 *g_pPILPropDialog;
extern PathAndPointManagerDialogImpl *g_pPathAndPointManagerDialog;
extern ODPointPropertiesImpl   *g_pODPointPropDialog;
extern ODPropertiesDialogImpl  *g_pOCPNDrawPropDialog;
extern PILPropertiesDialogImpl *g_PILIndexLinePropDialog;
extern ODDRDialogImpl          *g_pODDRDialog;
extern BoundaryList            *g_pBoundaryList;
extern EBLList                 *g_pEBLList;
extern DRList                  *g_pDRList;
extern GZList                  *g_pGZList;
extern PILList                 *g_pPILList;
extern ODPath                  *g_PathToEdit;
extern int                     g_PILToEdit;
extern ODRolloverWin           *g_pODRolloverWin;
extern SelectItem              *g_pRolloverPathSeg;
extern SelectItem              *g_pRolloverPoint;
extern PI_ColorScheme          g_global_color_scheme;


extern wxColour    g_colourActiveBoundaryLineColour;
extern wxColour    g_colourInActiveBoundaryLineColour;
extern wxColour    g_colourActiveBoundaryFillColour;
extern wxColour    g_colourInActiveBoundaryFillColour;
extern bool        g_bExclusionBoundary;
extern bool        g_bInclusionBoundary;
extern bool        g_bExclusionBoundaryPoint;
extern bool        g_bInclusionBoundaryPoint;
extern bool        g_bBoundaryODPointsVisible;
extern unsigned int g_uiFillTransparency;
extern unsigned int g_uiBoundaryPointFillTransparency;
extern int         g_iBoundaryPointRangeRingLineWidth;
extern wxPenStyle  g_iBoundaryPointRangeRingLineStyle;
extern int         g_BoundaryLineWidth; 
extern wxPenStyle  g_BoundaryLineStyle;
extern bool        g_bBoundaryPointShowName;
extern int         g_iInclusionBoundarySize;
extern int         g_iInclusionBoundaryPointSize;
extern wxString    g_sEBLEndIconName;
extern wxString    g_sEBLStartIconName;
extern wxColour    g_colourEBLLineColour;
extern bool        g_bEBLFixedEndPosition;
extern int         g_iEBLPersistenceType;
extern bool        g_bEBLShowArrow;
extern bool        g_bEBLVRM;
extern bool        g_bEBLAlwaysShowInfo;
extern bool        g_bEBLPerpLine;
extern int         g_EBLLineWidth; 
extern wxPenStyle  g_EBLLineStyle;
extern bool        g_bEBLRotateWithBoat;
extern int         g_iEBLMaintainWith;

extern wxString    g_sPILEndIconName;
extern wxString    g_sPILStartIconName;
extern wxColour    g_colourPILActiveCentreLineColour;
extern wxColour    g_colourPILInActiveCentreLineColour;
extern wxColour    g_colourPILActiveOffsetLine1Colour;
extern wxColour    g_colourPILInActiveOffsetLine1Colour;
extern wxColour    g_colourPILActiveOffsetLine2Colour;
extern wxColour    g_colourPILInActiveOffsetLine2Colour;
extern int         g_PILCentreLineWidth;
extern wxPenStyle  g_PILCentreLineStyle;
extern int         g_PILOffsetLine1Width;
extern wxPenStyle  g_PILOffsetLine1Style;
extern int         g_PILOffsetLine2Width;
extern wxPenStyle  g_PILOffsetLine2Style;
extern double      g_dPILOffset;
extern int         g_PILDefaultNumIndexLines;
extern int         g_iPILPersistenceType;

extern wxString    g_sDRPointIconName;
extern wxColour    g_colourDRLineColour;
extern wxColour    g_colourInActiveDRLineColour;
extern bool        g_bDRShowArrow;
extern int         g_DRLineWidth;
extern wxPenStyle  g_DRLineStyle;
extern bool        g_bDRPointShowRangeRings;
extern int         g_iDRPointRangeRingsNumber;
extern float       g_fDRPointRangeRingsStep;
extern int         g_iDRPointRangeRingsStepUnits;
extern wxColour    g_colourDRPointRangeRingsColour;
extern int         g_iDRPointRangeRingLineWidth;
extern wxPenStyle  g_iDRPointRangeRingLineStyle;

extern wxColour     g_colourActiveGZLineColour;
extern wxColour     g_colourInActiveGZLineColour;
extern wxColour     g_colourActiveGZFillColour;
extern wxColour     g_colourInActiveGZFillColour;
extern int          g_GZLineWidth; 
extern wxPenStyle   g_GZLineStyle;
extern unsigned int g_uiGZFillTransparency;
extern bool         g_bGZRotateWithBoat;
extern int          g_iGZMaintainWith;
extern int          g_iGZLineType;
extern wxString    g_sGZFirstIconName;
extern wxString    g_sGZSecondIconName;
extern int         g_iGZPersistenceType;
extern int         g_iGZMaxNum;

extern wxColour    g_colourActivePathLineColour;
extern wxColour    g_colourInActivePathLineColour;
extern int         g_PathLineWidth; 
//extern int         g_PathLineStyle;

extern wxString    *g_PrivateDataDir;

extern wxString    *g_pHome_Locn;
extern wxString    *g_pData;
extern wxString    *g_pUserIconsDir;
extern wxString    *g_pLayerDir;

extern ODEventHandler   *g_ODEventHandler;

extern bool            g_bODPointShowRangeRings;
extern int             g_iODPointRangeRingsNumber;
extern float           g_fODPointRangeRingsStep;
extern int             g_iODPointRangeRingsStepUnits;
extern wxColour        g_colourODPointRangeRingsColour;
extern wxString        g_sODPointIconName;
extern bool            g_bTextPointShowName;
extern wxString        g_sTextPointIconName;
extern wxColour        g_colourDefaultTextColour;
extern wxFont          g_DisplayTextFont;
extern int             g_DisplayTextFontPointSize;
extern wxColour        g_colourDefaultTextBackgroundColour;
extern int             g_iTextBackgroundTransparency;
extern int             g_iTextPosition;
extern int             g_iTextTopOffsetX;
extern int             g_iTextTopOffsetY;
extern int             g_iTextBottomOffsetX;
extern int             g_iTextBottomOffsetY;
extern int             g_iTextBottomNameExtraOffsetY;
extern int             g_iTextCentreOffsetX;
extern int             g_iTextCentreOffsetY;
extern int             g_iTextRightOffsetX;
extern int             g_iTextRightOffsetY;
extern int             g_iTextLeftOffsetX;
extern int             g_iTextLeftOffsetY;
extern int             g_iTextPointDisplayTextWhen;
extern int             g_iTextMaxWidth;
extern int             g_iTextMaxWidthType;

extern PlugIn_ViewPort g_VP;
extern ODDC            *g_pDC;
extern bool            g_bShowMag;
extern bool            g_bAllowLeftDrag;
extern double          g_dVar;
extern double          g_UserVar;
extern double          g_dODPointArrivalCircleRadius;
extern wxRect          g_blink_rect;

extern int             g_LayerIdx;
extern bool            g_bShowLayers;
extern wxString        g_VisibleLayers;
extern wxString        g_InvisibleLayers;
extern ODLayerList     *g_pLayerList;
extern int             g_navobjbackups;
extern int             g_EdgePanSensitivity;
extern int             g_InitialEdgePanSensitivity;

extern wxString        *g_ODlocale;
extern int             g_iLocaleDepth;

extern int             g_click_stop;
extern bool            g_bConfirmObjectDelete;

extern ODToolbarImpl   *g_pODToolbar;
extern int             g_iToolbarPosX;
extern int             g_iToolbarPosY;
extern int             g_iDisplayToolbar;

extern double          g_dScale;
extern int             g_iNSScale;
extern float           g_ChartScaleFactorExp;

extern double          g_dDRSOG;
extern int             g_iDRCOG;
extern double          g_dDRLength;
extern double          g_dDRPointInterval;
extern int             g_iDRLengthType;
extern int             g_iDRIntervalType;
extern int             g_iDRDistanceUnits;
extern int             g_iDRTimeUnits;
extern int             g_iDRPersistenceType;

extern wxString        g_sDefaultImportPath;
extern wxString        g_sDefaultImportType;

extern ODPlugIn_Position_Fix_Ex  g_pfFix;

extern ODJSON          *g_pODJSON;
extern ODAPI           *g_pODAPI;

extern bool    g_bRememberPropertyDialogPosition;
extern int     g_iDefaultBoundaryPropertyDialogPostionX;
extern int     g_iDefaultBoundaryPropertyDialogPostionY;
extern int     g_iDefaultDRPropertyDialogPostionX;
extern int     g_iDefaultDRPropertyDialogPostionY;
extern int     g_iDefaultEBLPropertyDialogPostionX;
extern int     g_iDefaultEBLPropertyDialogPostionY;
extern int     g_iDefaultGZPropertyDialogPostionX;
extern int     g_iDefaultGZPropertyDialogPostionY;
extern int     g_iDefaultPathPropertyDialogPostionX;
extern int     g_iDefaultPathPropertyDialogPostionY;
extern int     g_iDefaultPointPropertyDialogPostionX;
extern int     g_iDefaultPointPropertyDialogPostionY;
extern int     g_iDefaultPILPropertyDialogPostionX;
extern int     g_iDefaultPILPropertyDialogPostionY;
extern int     g_iDefaultPILLinePropertyDialogPostionX;
extern int     g_iDefaultPILLinePropertyDialogPostionY;
extern int     g_iDefaultPathAnPointManagerDialogPostionX;
extern int     g_iDefaultPathAnPointManagerDialogPostionY;

extern wxImage ICursorLeft;
extern wxImage ICursorRight;
extern wxImage ICursorUp;
extern wxImage ICursorDown;
extern wxImage ICursorPencil;
extern wxImage ICursorCross;

// Needed for ocpndc.cpp to compile. Normally would be in glChartCanvas.cpp
extern float g_GLMinSymbolLineWidth;

extern wxWindow *g_current_canvas;
extern wxWindow *g_current_timer_canvas;
extern int      g_current_canvas_index;
extern int      g_current_timer_canvas_index;

#endif
