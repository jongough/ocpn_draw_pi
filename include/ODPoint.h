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
class wxMemoryDC;
class wxDialog;

#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/gdicmn.h>
#include <wx/gauge.h>
#include <wx/colour.h>
#include <wx/pen.h>
#include "Hyperlink.h"
#include "ocpn_plugin.h"
//#include "OCPNRegion.h"
//#include "viewport.h"
#include "bbox.h"
#include "georef.h"

class ODDC;
class wxDC;

class ODPoint
{
public:
      ODPoint(double lat, double lon, const wxString& icon_ident, const wxString& name, const wxString &pGUID = wxT("") , bool bAddToList = true);
      ODPoint( ODPoint* orig );
      ODPoint();
      virtual ~ODPoint(void);
      virtual void Draw(ODDC& dc, wxPoint *odp= NULL );
      void ReLoadIcon(void);

      wxDateTime GetCreateTime(void);
      void SetCreateTime( wxDateTime dt );
      
      void SetPosition(double lat, double lon);
      double GetLatitude()  { return m_lat; };
      double GetLongitude() { return m_lon; };
      void CalculateDCRect(wxDC& dc, wxRect *prect);

      bool IsSame(ODPoint *pOtherRP);        // toh, 2009.02.11
      bool IsActive() { return m_bIsActive; }
      bool IsVisible() { return m_bIsVisible; }
      bool IsListed() { return m_bIsListed; }
      bool IsNameShown() { return m_bShowName; }
      void SetActive(bool viz = true){ m_bIsActive = viz; }
      void SetVisible(bool viz = true){ m_bIsVisible = viz; }
      void SetListed(bool viz = true){ m_bIsListed = viz; }
      void SetNameShown(bool viz = true) { m_bShowName = viz; }
      wxString GetName(void){ return m_ODPointName; }
      wxString GetDescription(void) { return m_ODPointDescription; }
      
      wxString GetIconName(void){ return m_IconName; }
      wxBitmap *GetIconBitmap(){ return m_pbmIcon; }
      void SetIconName( wxString name ){ m_IconName = name; }
      
      void *GetSelectNode(void) { return m_SelectNode; }
      void SetSelectNode(void* node) { m_SelectNode = node; }

      void *GetManagerListNode(void) { return m_ManagerNode; }
      void SetManagerListNode(void* node) { m_ManagerNode = node; }
      
      void SetName(const wxString & name);
      void CalculateNameExtents(void);

      void SetDistance( double distance) { m_pathprop_distance = distance; };
      double GetDistance() { return m_pathprop_distance; };

      void SetODPointArrivalRadius(double dArrivalDistance) { m_ODPointArrivalRadius = dArrivalDistance; };
      void SetODPointArrivalRadius( wxString wxArrivalDistance ) { wxArrivalDistance.ToDouble( &m_ODPointArrivalRadius ); };
      double GetODPointArrivalRadius();
      bool  GetShowODPointRangeRings(void) { return m_bShowODPointRangeRings; };
      int   GetODPointRangeRingsNumber(void);
      float GetODPointRangeRingsStep(void);
      int   GetODPointRangeRingsStepUnits(void);
      wxColour GetODPointRangeRingsColour(void);
      wxString GetTypeString(void) { return m_sTypeString; }
      void  SetShowODPointRangeRings(bool b_showODPointRangeRings) { m_bShowODPointRangeRings = b_showODPointRangeRings; };
      void  SetODPointRangeRingsNumber(int i_ODPointRangeRingsNumber);
      void  SetODPointRangeRingsStep(float f_ODPointRangeRingsStep);
      void  SetODPointRangeRingsStepUnits(int i_ODPointRangeRingsStepUnits) { m_iODPointRangeRingsStepUnits = i_ODPointRangeRingsStepUnits; };
      void  SetODPointRangeRingsColour( wxColour wxc_ODPointRangeRingsColour ) { m_wxcODPointRangeRingsColour = wxc_ODPointRangeRingsColour; };
      void  SetODPointRangeRingWidth( int i_ODPointRangeRingWidth ) { m_iRangeRingWidth = i_ODPointRangeRingWidth; };
      void  SetODPointRangeRingStyle( wxPenStyle i_ODPointRangeRingStyle ) { m_iRangeRingStyle = i_ODPointRangeRingStyle; };
      void  SetTypeString( wxString sTypeString ) { m_sTypeString = sTypeString; }
      void  SetMarkDescription( wxString sMarkDescription );
      virtual void SetColourScheme( PI_ColorScheme cs = PI_GLOBAL_COLOR_SCHEME_RGB);
      virtual void CreateColourSchemes( void );

      bool SendToGPS(const wxString& com_name, wxGauge *pProgress);
      void SetRangeRingBBox(void);
      
      void AddURL(wxString URL, wxString URLDescription);

      double             m_lat;
      double             m_lon;
      double             m_seg_len;              // length in NMI to this point
                                                // undefined for starting point
      double            m_seg_vmg;
      wxDateTime        m_seg_etd;

      bool              m_bPtIsSelected;
      bool              m_bIsBeingEdited;

      bool              m_bIsInPath;
      bool              m_bSingleUse;           // Can only be used in one path, i.e. EBL

      bool              m_bIsolatedMark;        // This is an isolated mark

      bool              m_bKeepXPath;           // This is a mark which is part of a path
                                                //  and is also an isolated mark, so should not be deleted with
                                                //  route

      bool              m_bIsVisible;           // true if should be drawn, false if invisible
      bool              m_bIsListed;
      bool              m_bIsActive;
      wxString          m_ODPointDescription;
      wxString          m_GUID;

      wxFont            *m_pMarkFont;
      wxColour          m_FontColor;

      wxSize            m_NameExtents;

      bool              m_bPointPropertiesBlink;
      bool              m_bPathManagerBlink;
      
      bool              m_bDynamicName;
      bool              m_bShowName;
      wxRect            CurrentRect_in_DC;
      int               m_NameLocationOffsetX;
      int               m_NameLocationOffsetY;
      wxString          m_timestring;
      int               m_GPXTrkSegNo;
      bool              m_bIsInLayer;
      int               m_LayerID;

      double            m_pathprop_course;         // course from this ODPoint to the next ODPoint if in a path.
      double            m_pathprop_distance;       // distance from this ODPoint to the next ODPoint if in a path.

      HyperlinkList     *m_HyperlinkList;
      bool              m_bTemporary;

      virtual void DrawGL( PlugIn_ViewPort &pivp );
      unsigned int m_iTextTexture;
      int m_iTextTextureWidth, m_iTextTextureHeight;

      LLBBox m_wpBBox;
      double m_wpBBox_chart_scale, m_wpBBox_rotation;

      static bool s_bUpdateWaypointsDisplayList;

      double m_ODPointArrivalRadius;

      bool              m_bShowODPointRangeRings;
      int               m_iODPointRangeRingsNumber;
      float             m_fODPointRangeRingsStep;
      int               m_iODPointRangeRingsStepUnits;
      int               m_iRangeRingWidth;
      wxPenStyle        m_iRangeRingStyle;
      wxColour          m_wxcODPointRangeRingsColour;
      wxString          m_sTypeString;
      wxString          m_IconName;
      wxString          m_ODPointName;
      wxDateTime        m_CreateTimeX;
      wxBitmap          *m_pbmIcon;
      
      LLBBox            m_RangeRingsBBox;
      
protected:
    PI_ColorScheme  m_ColourScheme;
    wxColour    m_wxcODPointRangeRingsSchemeColour;
    wxColour    m_wxcODPointRangeRingsColourRGB;
    wxColour    m_wxcODPointRangeRingsColourDay;
    wxColour    m_wxcODPointRangeRingsColourDusk;
    wxColour    m_wxcODPointRangeRingsColourNight;

private:
      //wxString          m_ODPointName;
      //wxDateTime        m_CreateTimeX;
      //wxBitmap          *m_pbmIcon;
      //wxString          m_IconName;
      //wxString          m_sTypeString;
      
      void              *m_SelectNode;
      void              *m_ManagerNode;
      float             m_fIconScaleFactor;
      wxBitmap          m_ScaledBMP;
      
};

WX_DECLARE_LIST(ODPoint, ODPointList);// establish class as list member

#endif
