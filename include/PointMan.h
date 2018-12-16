/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Point Manager
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
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

#ifndef POINTMAN_H
#define POINTMAN_H

#include <wx/imaglist.h>
//#include "nmea0183.h"
#include "ODPoint.h"
#include "ocpn_draw_pi.h"

//----------------------------------------------------------------------------
//   constants
//----------------------------------------------------------------------------
#ifndef PI
#define PI        3.1415926535897931160E0      /* pi */
#endif



//----------------------------------------------------------------------------
//    forward class declarations
//----------------------------------------------------------------------------

class ODPath;
class ODPoint;
class ODPointList;
class wxFontEnumerator;

class BoundaryPoint;
//    List definitions for Point Manager Icons

class markicon_bitmap_list_type;
class markicon_key_list_type;
class markicon_description_list_type;

class PointMan
{
    public:
        PointMan();
        virtual ~PointMan();
        
      wxBitmap *GetIconBitmap(const wxString& icon_key);
      unsigned int GetIconTexture( const wxBitmap *pmb, int &glw, int &glh );
      int GetIconIndex(const wxBitmap *pbm);
      int GetXIconIndex(const wxBitmap *pbm);
      int GetNumIcons(void){ return m_pIconArray->Count(); }
      ODPoint *GetNearbyODPoint(double lat, double lon, double radius_meters);
      ODPoint *GetOtherNearbyODPoint(double lat, double lon, double radius_meters, const wxString &guid);
      void SetColorScheme(PI_ColorScheme cs);
      bool SharedODPointsExist();
      void DeleteAllODPoints(bool b_delete_used);
      ODPoint *FindODPointByGUID(const wxString &guid);
      void DestroyODPoint(ODPoint *pRp, bool b_update_changeset = true);
      void ClearODPointFonts(void);
      //void ProcessIcons( ocpnStyle::Style* style );
      void ProcessIcons( );
      void RemoveIcon(wxString key);
      
      bool DoesIconExist(const wxString & icon_key) const;
      wxBitmap *GetIconBitmap(int index);
      wxString *GetIconDescription(int index);
      wxString *GetIconName( int index );
      wxString *GetIconName( wxString wxIconDescription );
      wxString *GetIconKey(int index);

      wxImageList *Getpmarkicon_image_list(void);

      bool AddODPoint(ODPoint *prp);
      bool RemoveODPoint(ODPoint *prp);
      ODPointList *GetODPointList(void) { return m_pODPointList; }

      void ProcessIcon(wxBitmap pimage, const wxString & key, const wxString & description);
      
      wxArrayString     m_wxasFontFacenames;
      wxFontEnumerator  *m_pFontEnumerator;

      PI_ColorScheme    m_ColourScheme;

      BoundaryPoint *FindLineCrossingBoundaryPtr( double StartLon, double StartLat, double EndLon, double EndLat, int type, int state );

      wxString FindLineCrossingBoundary( double StartLat, double StartLon, double EndLat, double EndLon, 
            int type = ID_BOUNDARY_ANY, int state = ID_POINT_STATE_ANY );

protected:
private:
      //void ProcessUserIcons( ocpnStyle::Style* style );
      void ProcessUserIcons( void );
      wxBitmap *CreateDimBitmap(wxBitmap *pBitmap, double factor);
      double deg2rad(double degree) { return degree*(PI/180.0); };
      bool DistancePointLine( double pLon, double pLat, double StartLon, double StartLat, double EndLon, double EndLat, double Distance );

      ODPointList    *m_pODPointList;
      wxImageList       *pmarkicon_image_list;        // Current wxImageList, updated on colorscheme change
      int               m_markicon_image_list_base_count;
      wxArrayPtrVoid    *m_pIconArray;

      int         m_nGUID;
      
      wxArrayString     m_asFacenames;
};

#endif // POINTMAN_H
