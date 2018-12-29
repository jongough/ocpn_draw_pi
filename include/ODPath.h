/***************************************************************************
 *
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2013 by David S. Register                               *
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

#ifndef __PATH_H__
#define __PATH_H__

#include <wx/object.h>
#include <wx/list.h>
#include <wx/hashmap.h>

#include "ODPoint.h"
#include "Hyperlink.h"

#include <list>

#define STYLE_UNDEFINED -1

#define ROUTE_DEFAULT_SPEED 5.0
#define RTE_TIME_DISP_UTC _T("UTC")
#define RTE_TIME_DISP_PC _T("PC")
#define RTE_TIME_DISP_LOCAL _T("LOCAL")
#define RTE_UNDEF_DEPARTURE wxInvalidDateTime

enum {
    ID_MAINTAIN_WITH_HEADING = 0,
    ID_MAINTAIN_WITH_COG
};


class ODDC;

class ODPath : public wxObject
{
public:
    ODPath(void);
    virtual ~ODPath(void);

    virtual void AddPoint(ODPoint* pNewPoint, bool b_rename_in_sequence = true, bool b_deferBoxCalc = false, bool b_isLoading = false);
    void AddTentativePoint(const wxString& GUID);
    ODPoint *GetPoint(int nPoint);
    ODPoint *GetPoint ( const wxString &guid );
    int GetIndexOf(ODPoint *prp);
    virtual ODPoint *InsertPointBefore(ODPoint *pOP, double lat, double lon, bool bRenamePoints = false);
    virtual ODPoint *InsertPointAfter(ODPoint *pOP, double lat, double lon, bool bRenamePoints = false);
    virtual void InsertPointAfter( ODPoint *pOP, ODPoint *pnOP, bool bRenamePoints = false);
    void DrawPointWhich(ODDC& dc, int iPoint, wxPoint *rpn);
    void DrawSegment(ODDC& dc, wxPoint *rp1, wxPoint *rp2, PlugIn_ViewPort &VP, bool bdraw_arrow);
    void DrawArcSegment(ODDC& dc, wxPoint *rc, wxPoint *rp1, wxPoint *rp2, wxPoint *rp3, wxPoint *rp4, PlugIn_ViewPort &VP, bool bdraw_arrow);
    void RenderArcSegment( ODDC& dc, wxPoint *rpc, wxPoint *rp1, wxPoint *rp2, wxPoint *rp3, wxPoint *rp4, PlugIn_ViewPort &VP, bool bdraw_arrow );
    virtual void Draw(ODDC& dc, PlugIn_ViewPort &pVP);
    virtual void DrawGL( PlugIn_ViewPort &piVP );
    ODPoint *GetLastPoint();
    virtual void DeletePoint(ODPoint *rp, bool bRenamePoints = false);
    virtual void RemovePoint(ODPoint *rp, bool bRenamePoints = false);
    void DeSelectPath();
    void FinalizeForRendering();
    void UpdateSegmentDistances();
    void CalculateDCRect(wxDC& dc_boundary, wxRect *prect, PlugIn_ViewPort &VP);
    int GetnPoints(void){ return m_nPoints; }
    virtual LLBBox GetBBox();
    void SetnPoints(void){ m_nPoints = m_pODPointList->GetCount(); }
    void SetHiLite( int width ) {m_hiliteWidth = width; }
    void Reverse(bool bRenamePoints = false);
    void RebuildGUIDList(void);
    void RenameODPoints();
    void RenamePathPoints();
    void ReloadPathPointIcons();
    wxString GetNewMarkSequenced(void);
    void AssemblePath();
    bool IsEqualTo(ODPath *ptargetboundary);
    void ClonePath(ODPath *psourceboundary, int start_nPoint, int end_nPoint, const wxString & suffix);
    void CloneAddedODPoint(ODPoint *ptargetpoint, ODPoint *psourcepoint);
    void ClearHighlights(void);
    void RenderSegment(ODDC& dc, int xa, int ya, int xb, int yb, PlugIn_ViewPort &VP, bool bdraw_arrow, int hilite_width = 0);
    void RenderSegmentArrowsGL( int xa, int ya, int xb, int yb, PlugIn_ViewPort &VP);
    void RenderArcSegment(ODDC& dc, int centre_x, int centre_y, int xa, int ya, int xb, int yb, int xc, int yc, int xd, int yd, PlugIn_ViewPort &VP, bool bdraw_arrow, int hilite_width = 0);
    virtual void SetColours( ODPath * );
    virtual void SetActiveColours( void );
    virtual void SetColourScheme( PI_ColorScheme cs = PI_GLOBAL_COLOR_SCHEME_RGB );
    virtual void CreateColourSchemes( void );
    virtual wxColour GetCurrentColour( void );

    bool CrossesIDL(){ return m_bcrosses_idl; }
    void SetVisible(bool visible = true, bool includeODPoints = true);
    void SetListed(bool visible = true);
    bool IsVisible() { return m_bVisible; }
    bool IsListed() { return m_bListed; }
    bool IsActive() { return m_bPathIsActive; }
    bool IsSelected() { return m_bPathIsSelected; }

    int SendToGPS(const wxString & com_name, bool bsend_waypoints, wxGauge *pProgress);

    double GetRouteArrivalRadius(void){ return m_ArrivalRadius;}
    void SetRouteArrivalRadius(double radius){m_ArrivalRadius = radius;}
    
    virtual void RemovePointFromPath( ODPoint* point, ODPath* path );
    virtual void MoveAllPoints( double inc_lat, double inc_lon );
    virtual void MoveSegment( double inc_lat, double inc_lon, ODPoint* firstPoint, ODPoint* secondPoint );
    virtual void SetPointVisibility( void );

    int         m_ConfigPathNum;
    bool        m_bPathIsSelected;
    bool        m_bPathIsActive;
    ODPoint  *m_pPathActivePoint;
    bool        m_bIsBeingCreated;
    bool        m_bIsBeingEdited;
    double      m_path_length;
    double      m_path_time;
    wxString    m_sTypeString;
    wxString    m_PathNameString;
    wxString    m_PathDescription;
    ODPoint  *m_pLastAddedPoint;
    ODPoint  *m_pFirstAddedPoint;
    bool        m_bDeleteOnArrival;
    wxString    m_GUID;
    bool        m_bIsInLayer;
    int         m_LayerID;
    int         m_width;
    wxPenStyle  m_style;
    int         m_lastMousePointIndex;
    bool        m_NextLegGreatCircle;
    double      m_PlannedSpeed;
    wxDateTime  m_PlannedDeparture;
    wxString    m_TimeDisplayFormat;
    HyperlinkList     *m_HyperlinkList;

    wxArrayString      ODPointGUIDList;
    ODPointList     *m_pODPointList;

    wxRect      active_pt_rect;
    wxColour    m_wxcActiveLineColour;
    wxColour    m_wxcInActiveLineColour;
    bool        m_bTemporary;
    int         m_hiliteWidth;
    wxPoint     *m_bpts;
    
    bool        m_bPathManagerBlink;
    bool        m_bPathPropertiesBlink;
    
    bool        m_bDrawArrow;
    bool        m_bSaveUpdates;
    
    bool        m_bODPointsVisible; // should the ODPoints on a path be drawn 
    
protected:    
    bool        m_bNeedsUpdateBBox;
    LLBBox      RBBox;

    bool        CalculateCrossesIDL();
    int         m_nPoints;
    int         m_nm_sequence;
    bool        m_bVisible; // should this boundary be drawn?
    bool        m_bListed;
    double      m_ArrivalRadius;
    bool        m_bcrosses_idl;
    wxColour    m_col;

    PI_ColorScheme m_ColourScheme;
    wxColour    m_wxcSchemeActiveLineColour;
    wxColour    m_wxcSchemeInActiveLineColour;
    wxColour    m_wxcActiveLineColourDay;
    wxColour    m_wxcInActiveLineColourDay;
    wxColour    m_wxcActiveLineColourDusk;
    wxColour    m_wxcInActiveLineColourDusk;
    wxColour    m_wxcActiveLineColourNight;
    wxColour    m_wxcInActiveLineColourNight;
    wxColour    m_wxcActiveLineColourRGB;
    wxColour    m_wxcInActiveLineColourRGB;

    
private:

    
};

WX_DECLARE_LIST(ODPath, PathList); // establish class Path as list member

#endif
