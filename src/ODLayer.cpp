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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ODLayer.h"
#include "ocpn_draw_pi.h"

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(ODLayerList);

ODLayer::ODLayer( void )
{
    m_bIsVisibleOnListing = false;
    m_bIsVisible = g_bShowLayers;
    m_bHasVisibleNames = true;
    m_NoOfItems = 0;

    m_LayerName = _T("");
    m_LayerFileName = _T("");
    m_LayerDescription = _T("");
    m_CreateTime = wxDateTime::Now();
}

ODLayer::~ODLayer( void )
{
//  Remove this layer from the global layer list
    if( NULL != g_pLayerList ) g_pLayerList->DeleteObject( this );

}


