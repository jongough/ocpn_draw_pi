/******************************************************************************
 * $Id: ocpn_draw_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN General Drawing Plugin Toolbar
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
 *   $EMAIL$                                                               *
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
//#define _2_9_x_ // uncomment this to compile for 2.9.x

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "wx/wxprec.h"

#include "ODToolbarImpl.h"
#include "ODicons.h"
#include "ocpn_draw_pi.h"

ODToolbarImpl::ODToolbarImpl( wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style  ) : ODToolbarDialog( parent )
{
    m_toolBoundary = m_toolBarODToolbar->AddTool( ID_BOUNDARY, wxS("Boundary"), *_img_ocpn_draw_boundary_gray, wxNullBitmap, wxITEM_CHECK, wxEmptyString, wxEmptyString );
    //m_toolBarODToolbar->AddTool( ID_ODPOINT, wxS("Point"), *_img_ocpn_draw_point_gray, wxS("ODPoint"), wxITEM_CHECK );
    m_toolBarODToolbar->SetInitialSize();
    
    //    m_toolBarODToolbar->AddTool( ID_TEXTPOINT, wxT("Text Point"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxT("Text Point"), wxEmptyString );
/*    m_toolBarODToolbar->Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( ODToolbarImpl::OnLeaveWindow ), NULL, this );
    m_toolBarODToolbar->Connect( wxEVT_MOTION, wxMouseEventHandler( ODToolbarImpl::OnMotion ), NULL, this );
    m_toolBarODToolbar->Connect( wxEVT_PAINT, wxPaintEventHandler( ODToolbarImpl::OnPaint ), NULL, this );
    m_toolBarODToolbar->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODToolbarImpl::OnSetFocus ), NULL, this );
*/    
    
}

ODToolbarImpl::~ODToolbarImpl()
{
/*    m_toolBarODToolbar->Disconnect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( ODToolbarImpl::OnLeaveWindow ), NULL, this );
    m_toolBarODToolbar->Disconnect( wxEVT_MOTION, wxMouseEventHandler( ODToolbarImpl::OnMotion ), NULL, this );
    m_toolBarODToolbar->Disconnect( wxEVT_PAINT, wxPaintEventHandler( ODToolbarImpl::OnPaint ), NULL, this );
    m_toolBarODToolbar->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODToolbarImpl::OnSetFocus ), NULL, this );
*/    
}


