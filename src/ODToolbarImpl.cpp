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

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ODToolbarImpl.h"
#include "ODicons.h"
#include "ocpn_draw_pi.h"

extern ocpn_draw_pi *g_ocpn_draw_pi;

ODToolbarImpl::ODToolbarImpl( wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style  ) : ODToolbarDialog( parent )
{
    m_toolBarODToolbar->SetToolBitmapSize(wxSize(g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary->GetWidth(), g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary->GetHeight()));
    m_toolBoundary = m_toolBarODToolbar->AddCheckTool( ID_BOUNDARY, _("Boundary"), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary, *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary_grey, _("Create Boundary"), wxEmptyString );
    m_toolODPoint = m_toolBarODToolbar->AddCheckTool( ID_ODPOINT, _("Boundary Point"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point, *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point_grey, _("Create Boundary Point"), wxEmptyString );
    m_toolTextPoint = m_toolBarODToolbar->AddCheckTool( ID_TEXTPOINT, _("Text Point"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint, *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint_grey, _("Create Text Point"), wxEmptyString );
    m_toolEBL = m_toolBarODToolbar->AddCheckTool( ID_EBL, _("Electronic Bearing Line"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl, *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl_grey, _("Create EBL"), wxEmptyString );
    m_toolDR = m_toolBarODToolbar->AddCheckTool( ID_DR, _("Dead Reckoning"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr, *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr_grey, _("Create DR"), wxEmptyString );
    m_toolBarODToolbar->SetInitialSize();
    m_toolbarSize = m_toolBarODToolbar->GetSize();
	m_toolBarODToolbar->Realize();
    // this is to ensure the toolbar is the correct size to show icons
    this->GetSizer()->Fit(this);
    this->Layout();
    
    Connect( wxEVT_MENU, wxCommandEventHandler( ODToolbarImpl::OnToolButtonClick ), NULL, this );
}

ODToolbarImpl::~ODToolbarImpl()
{
    Disconnect( wxEVT_MENU, wxCommandEventHandler( ODToolbarImpl::OnToolButtonClick ), NULL, this );
}

void ODToolbarImpl::OnActivate( wxActivateEvent& event )
{
    if( !event.GetActive() ) return;
    
    switch(g_ocpn_draw_pi->m_Mode)
    {
        case ID_MODE_BOUNDARY:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            break;
        }
        case ID_MODE_POINT: 
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            break;
        }
        case ID_MODE_TEXT_POINT: 
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            break;
        }
        case ID_MODE_EBL: 
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            break;
        }
        case ID_MODE_DR: 
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), true );
            break;
        }
        case ID_NONE:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            break;
        }
        default:
            break;
    }
            
}

void ODToolbarImpl::OnToolButtonClick( wxCommandEvent& event )
{
    UpdateIcons();
    
    switch( event.GetId() )
    {
        case ID_BOUNDARY:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            g_ocpn_draw_pi->m_Mode = ID_MODE_BOUNDARY;
            if( g_ocpn_draw_pi->nBoundary_State == 0) g_ocpn_draw_pi->nBoundary_State = 1;
            g_ocpn_draw_pi->SetToolbarTool();
            break;
        }
        case ID_ODPOINT:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            g_ocpn_draw_pi->m_Mode = ID_MODE_POINT;
            if( g_ocpn_draw_pi->nPoint_State == 0) g_ocpn_draw_pi->nPoint_State = 1;
            g_ocpn_draw_pi->SetToolbarTool();
            break;
        }
        case ID_TEXTPOINT:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            g_ocpn_draw_pi->m_Mode = ID_MODE_TEXT_POINT;
            if( g_ocpn_draw_pi->nTextPoint_State == 0) g_ocpn_draw_pi->nTextPoint_State = 1;
            g_ocpn_draw_pi->SetToolbarTool();
            break;
        }
        case ID_EBL:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            g_ocpn_draw_pi->m_Mode = ID_MODE_EBL;
            if( g_ocpn_draw_pi->nEBL_State == 0) g_ocpn_draw_pi->nEBL_State = 1;
            g_ocpn_draw_pi->SetToolbarTool();
            break;
        }
        case ID_DR:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), true );
            g_ocpn_draw_pi->m_Mode = ID_MODE_DR;
            if( g_ocpn_draw_pi->nEBL_State == 0) g_ocpn_draw_pi->nEBL_State = 1;
            g_ocpn_draw_pi->SetToolbarTool();
            break;
        }
        default:
            break;
    }
    
}

void ODToolbarImpl::OnClose( wxCloseEvent& event )
{
    g_ocpn_draw_pi->OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
}

void ODToolbarImpl::SetToolbarTool( int iTool )
{
    switch( iTool )
    {
        case ID_MODE_BOUNDARY:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            break;
        }
        case ID_MODE_POINT:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            break;
        }
        case ID_MODE_TEXT_POINT:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            break;
        }
        case ID_MODE_EBL:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            break;
        }
        case ID_MODE_DR:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), true );
            break;
        }
        case ID_NONE:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            break;
        }
        default:
            break;
    }
}

void ODToolbarImpl::UpdateIcons()
{
    if(!g_ocpn_draw_pi->m_pODicons->ScaleIcons()) return;
    m_toolBarODToolbar->DeleteTool( ID_BOUNDARY );
    m_toolBarODToolbar->DeleteTool( ID_ODPOINT );
    m_toolBarODToolbar->DeleteTool( ID_TEXTPOINT );
    m_toolBarODToolbar->DeleteTool( ID_EBL );
    m_toolBarODToolbar->DeleteTool( ID_DR );
    
    this->InvalidateBestSize();
    m_toolBarODToolbar->SetToolBitmapSize(wxSize(g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary->GetWidth(), g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary->GetHeight()));
    m_toolBoundary = m_toolBarODToolbar->AddCheckTool( ID_BOUNDARY, _("Boundary"), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary, *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary_grey, _("Create Boundary"), wxEmptyString );
    m_toolODPoint = m_toolBarODToolbar->AddCheckTool( ID_ODPOINT, _("Boundary Point"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point, *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point_grey, _("Create Boundary Point"), wxEmptyString );
    m_toolTextPoint = m_toolBarODToolbar->AddCheckTool( ID_TEXTPOINT, _("Text Point"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint, *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint_grey, _("Create Text Point"), wxEmptyString );
    m_toolEBL = m_toolBarODToolbar->AddCheckTool( ID_EBL, _("Electronic Bearing Line"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl, *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl_grey, _("Create EBL"), wxEmptyString );
    m_toolDR = m_toolBarODToolbar->AddCheckTool( ID_DR, _("Dead Reckoning"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr, *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr_grey, _("Create DR"), wxEmptyString );
    m_toolBarODToolbar->Realize();
    
    m_toolBarODToolbar->SetInitialSize();
    this->Layout();
    this->GetSizer()->Fit(this);
    
}

