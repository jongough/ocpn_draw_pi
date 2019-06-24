/******************************************************************************
 *
 * Project:  OpenCPN
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
 ***************************************************************************
 */

#ifndef __ODLAYER_H__
#define __ODLAYER_H__

#include <wx/string.h>
#include <wx/list.h>
#include <wx/datetime.h>

class ODLayer
{
public:
      ODLayer(void);
      ~ODLayer(void);
      wxString CreatePropString(void) { return m_LayerFileName; }
      bool IsVisible() { return m_bIsVisible; }
      void SetVisible(bool viz = true){ m_bIsVisible = viz; }
      bool IsVisibleOnListing() { return m_bIsVisibleOnListing; }
      void SetVisibleOnListing(bool viz = true){ m_bIsVisibleOnListing = viz; }
      bool HasVisibleNames() { return m_bHasVisibleNames; }
      void SetVisibleNames(bool viz = true){ m_bHasVisibleNames = viz; }

      bool m_bIsVisibleOnListing;
      bool m_bIsVisible;
      bool m_bHasVisibleNames;
      long m_NoOfItems;
      int m_LayerID;

      wxString          m_LayerName;
      wxString          m_LayerFileName;
      wxString          m_LayerDescription;
      bool              m_bIsVisibleOnChart;
      wxDateTime        m_CreateTime;
};

WX_DECLARE_LIST(ODLayer, ODLayerList);// establish class as list member

#endif
