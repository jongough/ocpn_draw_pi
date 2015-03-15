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

#ifndef __POINT_H__
#define __POINT_H__

#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/gdicmn.h>
#include <wx/gauge.h>
#include "Hyperlink.h"
#include "gpxdocument.h"
#include "ocpn_types.h"
#include "viewport.h"

class ocpnDC;
class wxDC;

class OCPNPoint
{
public:
      OCPNPoint(double lat, double lon, const wxString& icon_ident, const wxString& name, const wxString &pGUID = GPX_EMPTY_STRING, bool bAddToList = true);
      OCPNPoint( OCPNPoint* orig );
      OCPNPoint();
      ~OCPNPoint(void);
      void Draw(ocpnDC& dc, wxPoint *rpn = NULL);
      void ReLoadIcon(void);

      wxDateTime GetCreateTime(void);
      void SetCreateTime( wxDateTime dt );
      
      void SetPosition(double lat, double lon);
      double GetLatitude()  { return m_lat; };
      double GetLongitude() { return m_lon; };
      void CalculateDCRect(wxDC& dc, wxRect *prect);

      bool IsSame(OCPNPoint *pOtherRP);        // toh, 2009.02.11
      bool IsVisible() { return m_bIsVisible; }
      bool IsListed() { return m_bIsListed; }
      bool IsNameShown() { return m_bShowName; }
      void SetVisible(bool viz = true){ m_bIsVisible = viz; }
      void SetListed(bool viz = true){ m_bIsListed = viz; }
      void SetNameShown(bool viz = true) { m_bShowName = viz; }
      wxString GetName(void){ return m_MarkName; }
      wxString GetDescription(void) { return m_MarkDescription; }
      
      wxString GetIconName(void){ return m_IconName; }
      wxBitmap *GetIconBitmap(){ return m_pbmIcon; }
      void SetIconName( wxString name ){ m_IconName = name; }
      
      void *GetSelectNode(void) { return m_SelectNode; }
      void SetSelectNode(void* node) { m_SelectNode = node; }

      void *GetManagerListNode(void) { return m_ManagerNode; }
      void SetManagerListNode(void* node) { m_ManagerNode = node; }
      
      void SetName(const wxString & name);
      void CalculateNameExtents(void);

      void SetCourse( double course) { m_routeprop_course = course; };
      double GetCourse() { return m_routeprop_course; };
      void SetDistance( double distance) { m_routeprop_distance = distance; };
      double GetDistance() { return m_routeprop_distance; };

      void SetOCPNPointArrivalRadius(double dArrivalDistance) { m_OCPNPointArrivalRadius = dArrivalDistance; };
      void SetOCPNPointArrivalRadius( wxString wxArrivalDistance ) { wxArrivalDistance.ToDouble( &m_OCPNPointArrivalRadius ); };
      double GetOCPNPointArrivalRadius();
      bool  GetShowOCPNPointRangeRings(void) { return m_bShowOCPNPointRangeRings; };
      int   GetOCPNPointRangeRingsNumber(void);
      float GetOCPNPointRangeRingsStep(void);
      int   GetOCPNPointRangeRingsStepUnits(void);
      wxColour GetOCPNPointRangeRingsColour(void);
      void  SetShowOCPNPointRangeRings(bool b_showOCPNPointRangeRings) { m_bShowOCPNPointRangeRings = b_showOCPNPointRangeRings; };
      void  SetOCPNPointRangeRingsNumber(int i_OCPNPointRangeRingsNumber) { m_iOCPNPointRangeRingsNumber = i_OCPNPointRangeRingsNumber; };
      void  SetOCPNPointRangeRingsStep(float f_OCPNPointRangeRingsStep) { m_fOCPNPointRangeRingsStep = f_OCPNPointRangeRingsStep; };
      void  SetOCPNPointRangeRingsStepUnits(int i_OCPNPointRangeRingsStepUnits) { m_iOCPNPointRangeRingsStepUnits = i_OCPNPointRangeRingsStepUnits; };
      void  SetOCPNPointRangeRingsColour( wxColour wxc_OCPNPointRangeRingsColour ) { m_wxcOCPNPointRangeRingsColour = wxc_OCPNPointRangeRingsColour; };


      bool SendToGPS(const wxString& com_name, wxGauge *pProgress);


      double             m_lat;
      double             m_lon;
      double             m_seg_len;              // length in NMI to this point
                                                // undefined for starting point
      double            m_seg_vmg;
      wxDateTime        m_seg_etd;

      bool              m_bPtIsSelected;
      bool              m_bIsBeingEdited;

      bool              m_bIsInPath;
      bool              m_bIsInRoute;
      bool              m_bIsInBoundary;
      bool              m_bIsInTrack;

      bool              m_bIsolatedMark;        // This is an isolated mark

      bool              m_bKeepXPath;           // This is a mark which is part of a path
                                                //  and is also an isolated mark, so should not be deleted with
                                                //  route

      bool              m_bIsVisible;           // true if should be drawn, false if invisible
      bool              m_bIsListed;
      bool              m_bIsActive;
      wxString          m_MarkDescription;
      wxString          m_GUID;

      wxFont            *m_pMarkFont;
      wxColour          m_FontColor;

      wxSize            m_NameExtents;

      bool              m_bBlink;
      bool              m_bDynamicName;
      bool              m_bShowName;
      wxRect            CurrentRect_in_DC;
      int               m_NameLocationOffsetX;
      int               m_NameLocationOffsetY;
      wxString          m_timestring;
      int               m_GPXTrkSegNo;
      bool              m_bIsInLayer;
      int               m_LayerID;

      double            m_routeprop_course;         // course from this waypoint to the next waypoint if in a route.
      double            m_routeprop_distance;       // distance from this waypoint to the next waypoint if in a route.

      HyperlinkList     *m_HyperlinkList;
      bool              m_btemp;
      wxString          m_sTypeString;

#ifdef ocpnUSE_GL
      void DrawGL( ViewPort &vp, OCPNRegion &region );
      unsigned int m_iTextTexture;
      int m_iTextTextureWidth, m_iTextTextureHeight;

      LLBBox m_wpBBox;
      double m_wpBBox_chart_scale, m_wpBBox_rotation;

      static bool s_bUpdateWaypointsDisplayList;
#endif

      double m_OCPNPointArrivalRadius;

      bool              m_bShowOCPNPointRangeRings;
      int               m_iOCPNPointRangeRingsNumber;
      float             m_fOCPNPointRangeRingsStep;
      int               m_iOCPNPointRangeRingsStepUnits;
      wxColour          m_wxcOCPNPointRangeRingsColour;
      
      
private:
      wxString          m_MarkName;
      wxDateTime        m_CreateTimeX;
      wxBitmap          *m_pbmIcon;
      wxString          m_IconName;
      
      void              *m_SelectNode;
      void              *m_ManagerNode;
      
};

WX_DECLARE_LIST(OCPNPoint, OCPNPointList);// establish class as list member

#endif
