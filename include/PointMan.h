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

#include "chart1.h"                 // for ColorScheme definition
#include <wx/imaglist.h>
#include "styles.h"
//#include "Select.h"
#include "nmea0183.h"
#include "OCPNPoint.h"

//----------------------------------------------------------------------------
//   constants
//----------------------------------------------------------------------------
#ifndef PI
#define PI        3.1415926535897931160E0      /* pi */
#endif



//----------------------------------------------------------------------------
//    forward class declarations
//----------------------------------------------------------------------------

class Path;
class OCPNPoint;
class OCPNPointList;

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
      wxString CreateGUID(OCPNPoint *pRP);
      OCPNPoint *GetNearbyOCPNPoint(double lat, double lon, double radius_meters);
      OCPNPoint *GetOtherNearbyOCPNPoint(double lat, double lon, double radius_meters, const wxString &guid);
      void SetColorScheme(ColorScheme cs);
      bool SharedOCPNPointsExist();
      void DeleteAllOCPNPoints(bool b_delete_used);
      OCPNPoint *FindOCPNPointByGUID(const wxString &guid);
      void DestroyOCPNPoint(OCPNPoint *pRp, bool b_update_changeset = true);
      void ClearOCPNPointFonts(void);
      void ProcessIcons( ocpnStyle::Style* style );
      
      bool DoesIconExist(const wxString & icon_key) const;
      wxBitmap *GetIconBitmap(int index);
      wxString *GetIconDescription(int index);
      wxString *GetIconKey(int index);

      wxImageList *Getpmarkicon_image_list(void);

      bool AddOCPNPoint(OCPNPoint *prp);
      bool RemoveOCPNPoint(OCPNPoint *prp);
      OCPNPointList *GetOCPNPointList(void) { return m_pOCPNPointList; }

      void ProcessIcon(wxBitmap pimage, const wxString & key, const wxString & description);
protected:
private:
      void ProcessUserIcons( ocpnStyle::Style* style );
      OCPNPointList    *m_pOCPNPointList;
      wxBitmap *CreateDimBitmap(wxBitmap *pBitmap, double factor);

      wxImageList       *pmarkicon_image_list;        // Current wxImageList, updated on colorscheme change
      int               m_markicon_image_list_base_count;
      wxArrayPtrVoid    *m_pIconArray;

      int         m_nGUID;
};

#endif // POINTMAN_H
