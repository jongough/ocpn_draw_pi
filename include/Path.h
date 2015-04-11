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

#include "Quilt.h"
#include "ocpn_types.h"
#include "OCPNPoint.h"
#include "Hyperlink.h"


#define STYLE_UNDEFINED -1

#define ROUTE_DEFAULT_SPEED 5.0
#define RTE_TIME_DISP_UTC _T("UTC")
#define RTE_TIME_DISP_PC _T("PC")
#define RTE_TIME_DISP_LOCAL _T("LOCAL")
#define RTE_UNDEF_DEPARTURE wxInvalidDateTime

class ocpnDC;

class Path : public wxObject
{
public:
    Path(void);
    ~Path(void);

    void AddPoint(OCPNPoint *pNewPoint,
                bool b_rename_in_sequence = true,
                bool b_deferBoxCalc = false,
                bool b_isLoading = false);

    void AddTentativePoint(const wxString& GUID);
    OCPNPoint *GetPoint(int nPoint);
    OCPNPoint *GetPoint ( const wxString &guid );
    int GetIndexOf(OCPNPoint *prp);
    OCPNPoint *InsertPointBefore(OCPNPoint *pRP, double rlat, double rlon, bool bRenamePoints = false);
    void DrawPointWhich(ocpnDC& dc, int iPoint, wxPoint *rpn);
    void DrawSegment(ocpnDC& dc, wxPoint *rp1, wxPoint *rp2, PlugIn_ViewPort &VP, bool bdraw_arrow);
    virtual void Draw(ocpnDC& dc, PlugIn_ViewPort &pVP);
    virtual void DrawGL( PlugIn_ViewPort &piVP );
    OCPNPoint *GetLastPoint();
    void DeletePoint(OCPNPoint *rp, bool bRenamePoints = false);
    void RemovePoint(OCPNPoint *rp, bool bRenamePoints = false);
    void DeSelectPath();
    void FinalizeForRendering();
    void UpdateSegmentDistances();
    void CalculateDCRect(wxDC& dc_boundary, wxRect *prect, PlugIn_ViewPort &VP);
    int GetnPoints(void){ return m_nPoints; }
    wxBoundingBox GetBBox();
    void SetnPoints(void){ m_nPoints = pOCPNPointList->GetCount(); }
    void SetHiLite( int width ) {m_hiliteWidth = width; }
    void Reverse(bool bRenamePoints = false);
    void RebuildGUIDList(void);
    void RenameOCPNPoints();
    void RenamePathPoints();
    void ReloadPathPointIcons();
    wxString GetNewMarkSequenced(void);
    void AssemblePath();
    bool IsEqualTo(Path *ptargetboundary);
    void ClonePath(Path *psourceboundary, int start_nPoint, int end_nPoint, const wxString & suffix);
    void CloneAddedOCPNPoint(OCPNPoint *ptargetpoint, OCPNPoint *psourcepoint);
    void ClearHighlights(void);
    void RenderSegment(ocpnDC& dc, int xa, int ya, int xb, int yb, PlugIn_ViewPort &VP, bool bdraw_arrow, int hilite_width = 0);
    void RenderSegmentArrowsGL( int xa, int ya, int xb, int yb, PlugIn_ViewPort &VP);

    bool CrossesIDL(){ return m_bcrosses_idl; }
    void SetVisible(bool visible = true, bool includeWpts = true);
    void SetListed(bool visible = true);
    bool IsVisible() { return m_bVisible; }
    bool IsListed() { return m_bListed; }
    bool IsActive() { return m_bPathIsActive; }
    bool IsSelected() { return m_bPathIsSelected; }

    int SendToGPS(const wxString & com_name, bool bsend_waypoints, wxGauge *pProgress);

    double GetRouteArrivalRadius(void){ return m_ArrivalRadius;}
    void SetRouteArrivalRadius(double radius){m_ArrivalRadius = radius;}
    
    void RemovePointFromPath( OCPNPoint* point, Path* path );

    int         m_ConfigPathNum;
    bool        m_bPathIsSelected;
    bool        m_bPathIsActive;
    OCPNPoint  *m_pPathActivePoint;
    bool        m_bIsBeingCreated;
    bool        m_bIsBeingEdited;
    double      m_path_length;
    double      m_path_time;
    wxString    m_sTypeString;
    wxString    m_PathNameString;
    wxString    m_PathDescription;
    OCPNPoint  *m_pLastAddedPoint;
    OCPNPoint  *m_pFirstAddedPoint;
    bool        m_bDeleteOnArrival;
    wxString    m_GUID;
    bool        m_bIsInLayer;
    int         m_LayerID;
    int         m_width;
    int         m_style;
    int         m_lastMousePointIndex;
    bool        m_NextLegGreatCircle;
    double      m_PlannedSpeed;
    wxDateTime  m_PlannedDeparture;
    wxString    m_TimeDisplayFormat;
    HyperlinkList     *m_HyperlinkList;

    wxArrayString      OCPNPointGUIDList;
    OCPNPointList     *pOCPNPointList;

    wxRect      active_pt_rect;
    wxString    m_LineColour;
    wxString    m_FillColour;
    bool        m_btemp;
    int         m_hiliteWidth;

private:
    bool m_bNeedsUpdateBBox;
    wxBoundingBox     RBBox;

    bool        CalculateCrossesIDL();
    int         m_nPoints;
    int         m_nm_sequence;
    bool        m_bVisible; // should this boundary be drawn?
    bool        m_bListed;
    double      m_ArrivalRadius;
    bool        m_bcrosses_idl;
};

WX_DECLARE_LIST(Path, PathList); // establish class Path as list member

#endif
