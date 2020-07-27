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

ODToolbarImpl::ODToolbarImpl( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint &pos, const wxSize &size, long style  ) : ODToolbarDialog( parent, id, title, pos, size, style )
{
    m_ColourScheme = PI_GLOBAL_COLOR_SCHEME_RGB;
    
    AddTools();
    
    m_Mode = ID_NONE;
    
    Connect( wxEVT_MENU, wxCommandEventHandler( ODToolbarImpl::OnToolButtonClick ), NULL, this );
}

ODToolbarImpl::~ODToolbarImpl()
{
    Disconnect( wxEVT_MENU, wxCommandEventHandler( ODToolbarImpl::OnToolButtonClick ), NULL, this );
}

void ODToolbarImpl::OnToolButtonClick( wxCommandEvent& event )
{
    g_ocpn_draw_pi->m_iCallerId = g_ocpn_draw_pi->m_draw_button_id;
    if(m_Mode == ID_MODE_BOUNDARY && event.GetId() != ID_MODE_BOUNDARY && g_ocpn_draw_pi->nBoundary_State > 1) {
        m_toolBarODToolbar->ToggleTool(event.GetId(), false);
        return; // if creating a boundary must finish before clicking another button
    }
    
    switch( event.GetId() )
    {
        case ID_MODE_BOUNDARY:
        {
            g_ocpn_draw_pi->m_Mode = ID_MODE_BOUNDARY;
            if(m_Mode == event.GetId()) {
                m_Mode = -1;
                g_ocpn_draw_pi->OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
            } else {
                m_Mode = ID_MODE_BOUNDARY;
                if( g_ocpn_draw_pi->nBoundary_State == 0) g_ocpn_draw_pi->nBoundary_State = 1;
            }
            break;
        }
        case ID_MODE_POINT:
        {
            g_ocpn_draw_pi->m_Mode = ID_MODE_POINT;
            if(m_Mode == event.GetId()) {
                m_Mode = -1;
                g_ocpn_draw_pi->OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
            } else {
                m_Mode = ID_MODE_POINT;
                if( g_ocpn_draw_pi->nPoint_State == 0) g_ocpn_draw_pi->nPoint_State = 1;
            }
            break;
        }
        case ID_MODE_TEXT_POINT:
        {
            g_ocpn_draw_pi->m_Mode = ID_MODE_TEXT_POINT;
            if(m_Mode == event.GetId()) {
                m_Mode = -1;
                g_ocpn_draw_pi->OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
            } else {
                m_Mode = ID_MODE_TEXT_POINT;
                if( g_ocpn_draw_pi->nTextPoint_State == 0) g_ocpn_draw_pi->nTextPoint_State = 1;
            }
            break;
        }
        case ID_MODE_EBL:
        {
            g_ocpn_draw_pi->m_Mode = ID_MODE_EBL;
            if(m_Mode == event.GetId()) {
                m_Mode = -1;
                g_ocpn_draw_pi->OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
            } else {
                m_Mode = ID_MODE_EBL;
                if( g_ocpn_draw_pi->nEBL_State == 0) g_ocpn_draw_pi->nEBL_State = 1;
            }
            break;
        }
        case ID_MODE_DR:
        {
            g_ocpn_draw_pi->m_Mode = ID_MODE_DR;
            if(m_Mode == event.GetId()) {
                m_Mode = -1;
                g_ocpn_draw_pi->OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
            } else {
                m_Mode = ID_MODE_DR;
                if( g_ocpn_draw_pi->nDR_State == 0) g_ocpn_draw_pi->nDR_State = 1;
            }
            break;
        }
        case ID_MODE_GZ:
        {
            g_ocpn_draw_pi->m_Mode = ID_MODE_GZ;
            if(m_Mode == event.GetId()) {
                m_Mode = -1;
                g_ocpn_draw_pi->OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
            } else {
                m_Mode = ID_MODE_GZ;
                if( g_ocpn_draw_pi->nGZ_State == 0) g_ocpn_draw_pi->nGZ_State = 1;
            }
            break;
        }
        case ID_MODE_PIL:
        {
            g_ocpn_draw_pi->m_Mode = ID_MODE_PIL;
            if(m_Mode == event.GetId()) {
                m_Mode = -1;
                g_ocpn_draw_pi->OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
            } else {
                m_Mode = ID_MODE_PIL;
                if( g_ocpn_draw_pi->nPIL_State == 0) g_ocpn_draw_pi->nPIL_State = 1;
            }
            break;
        }
        default:
            break;
    }
    g_ocpn_draw_pi->SetToolbarTool();
    SetToolbarTool( m_Mode );
}

void ODToolbarImpl::OnClose( wxCloseEvent& event )
{
    g_ocpn_draw_pi->OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
}

void ODToolbarImpl::OnKeyDown(wxKeyEvent& event)
{
    if( event.GetKeyCode() == 27 ) { // esc key
        m_Mode = -1;
        g_ocpn_draw_pi->OnToolbarToolDownCallback( g_ocpn_draw_pi->m_draw_button_id);
    }
    ODToolbarDialog::OnKeyDown(event);
}

void ODToolbarImpl::SetToolbarTool( int iTool )
{
    UpdateIcons();
    SetToolbarToolToggle( iTool );
    SetToolbarToolBitmap( iTool );
}    

void ODToolbarImpl::SetToolbarToolToggle( int iTool )
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
            m_toolBarODToolbar->ToggleTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_POINT:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_TEXT_POINT:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_EBL:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_DR:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_GZ:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolGZ->GetId(), true );
            m_toolBarODToolbar->ToggleTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_PIL:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolPIL->GetId(), true );
            break;
        }
        case ID_NONE:
        {
            m_toolBarODToolbar->ToggleTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->ToggleTool( m_toolPIL->GetId(), false );
            m_Mode = ID_NONE;
            break;
        }
        default:
            break;
    }
    m_toolBarODToolbar->Realize();
}

void ODToolbarImpl::SetToolbarToolEnableOnly( int iTool )
{
    switch( iTool )
    {
        case ID_MODE_BOUNDARY:
        {
            m_toolBarODToolbar->EnableTool( m_toolBoundary->GetId(), true );
            m_toolBarODToolbar->EnableTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_POINT:
        {
            m_toolBarODToolbar->EnableTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolODPoint->GetId(), true );
            m_toolBarODToolbar->EnableTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_TEXT_POINT:
        {
            m_toolBarODToolbar->EnableTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolTextPoint->GetId(), true );
            m_toolBarODToolbar->EnableTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_EBL:
        {
            m_toolBarODToolbar->EnableTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolEBL->GetId(), true );
            m_toolBarODToolbar->EnableTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_DR:
        {
            m_toolBarODToolbar->EnableTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolDR->GetId(), true );
            m_toolBarODToolbar->EnableTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_GZ:
        {
            m_toolBarODToolbar->EnableTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolGZ->GetId(), true );
            m_toolBarODToolbar->EnableTool( m_toolPIL->GetId(), false );
            break;
        }
        case ID_MODE_PIL:
        {
            m_toolBarODToolbar->EnableTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolPIL->GetId(), true );
            break;
        }
        case ID_NONE:
        {
            m_toolBarODToolbar->EnableTool( m_toolBoundary->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolODPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolTextPoint->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolEBL->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolDR->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolGZ->GetId(), false );
            m_toolBarODToolbar->EnableTool( m_toolPIL->GetId(), false );
            m_Mode = ID_NONE;
            break;
        }
        default:
            break;
    }
    m_toolBarODToolbar->Realize();
}

void ODToolbarImpl::SetToolbarToolEnableAll( void )
{
    m_toolBarODToolbar->EnableTool( m_toolBoundary->GetId(), true );
    m_toolBarODToolbar->EnableTool( m_toolODPoint->GetId(), true );
    m_toolBarODToolbar->EnableTool( m_toolTextPoint->GetId(), true );
    m_toolBarODToolbar->EnableTool( m_toolEBL->GetId(), true );
    m_toolBarODToolbar->EnableTool( m_toolDR->GetId(), true );
    m_toolBarODToolbar->EnableTool( m_toolGZ->GetId(), true );
    m_toolBarODToolbar->EnableTool( m_toolPIL->GetId(), true );
    m_toolBarODToolbar->Realize();
}

void ODToolbarImpl::SetToolbarToolBitmap( int iTool )
{
    m_Mode = iTool;
    switch( iTool )
    {
        case ID_MODE_BOUNDARY:
        {
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolBoundary->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary_grey );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolODPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolTextPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolEBL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolDR->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolGZ->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_gz );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolPIL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_pil );
            break;
        }
        case ID_MODE_POINT:
        {
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolBoundary->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolODPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point_grey );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolTextPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolEBL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolDR->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolGZ->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_gz );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolPIL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_pil );
            break;
        }
        case ID_MODE_TEXT_POINT:
        {
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolBoundary->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolODPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolTextPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint_grey );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolEBL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolDR->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolGZ->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_gz );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolPIL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_pil );
            break;
        }
        case ID_MODE_EBL:
        {
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolBoundary->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolODPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolTextPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolEBL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl_grey );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolDR->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolGZ->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_gz );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolPIL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_pil );
            break;
        }
        case ID_MODE_DR:
        {
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolBoundary->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolODPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolTextPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolEBL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolDR->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr_grey );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolGZ->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_gz );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolPIL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_pil );
            break;
        }
        case ID_MODE_GZ:
        {
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolBoundary->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolODPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolTextPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolEBL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolDR->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolGZ->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_gz_grey );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolPIL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_pil );
            break;
        }
        case ID_MODE_PIL:
        {
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolBoundary->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolODPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolTextPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolEBL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolDR->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolGZ->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_gz );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolPIL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_pil_grey );
            break;
        }
        case ID_NONE:
        {
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolBoundary->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolODPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolTextPoint->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolEBL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolDR->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolGZ->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_gz );
            m_toolBarODToolbar->SetToolNormalBitmap( m_toolPIL->GetId(), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_pil );
            break;
        }
        default:
            m_Mode = ID_NONE;
            break;
        
    }
    m_toolBarODToolbar->Realize();
}

void ODToolbarImpl::UpdateIcons( void )
{
    if(g_ocpn_draw_pi->m_pODicons->ScaleIcons() || g_ocpn_draw_pi->m_pODicons->m_bUpdateIcons) {
        m_toolBarODToolbar->DeleteTool( ID_MODE_BOUNDARY );
        m_toolBarODToolbar->DeleteTool( ID_MODE_POINT );
        m_toolBarODToolbar->DeleteTool( ID_MODE_TEXT_POINT );
        m_toolBarODToolbar->DeleteTool( ID_MODE_EBL );
        m_toolBarODToolbar->DeleteTool( ID_MODE_DR );
        m_toolBarODToolbar->DeleteTool( ID_MODE_GZ );
        m_toolBarODToolbar->DeleteTool( ID_MODE_PIL );

        this->InvalidateBestSize();
        AddTools();
    }
}

void ODToolbarImpl::AddTools(void)
{
    Disconnect( wxEVT_MENU, wxCommandEventHandler( ODToolbarImpl::OnToolButtonClick ), NULL, this );
    DEBUGST("Width: ");
    DEBUGCONT(g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary->GetWidth());
    DEBUGCONT(", Height: ");
    DEBUGEND(g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary->GetHeight());
    m_toolBarODToolbar->SetToolBitmapSize(wxSize(g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary->GetWidth(), g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary->GetHeight()));
    m_toolBoundary = m_toolBarODToolbar->AddCheckTool( ID_MODE_BOUNDARY, _("Boundary"), *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_boundary, wxNullBitmap, _("Create Boundary"), wxEmptyString );
    m_toolODPoint = m_toolBarODToolbar->AddCheckTool( ID_MODE_POINT, _("Boundary Point"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_point, wxNullBitmap, _("Create Boundary Point"), wxEmptyString );
    m_toolTextPoint = m_toolBarODToolbar->AddCheckTool( ID_MODE_TEXT_POINT, _("Text Point"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_textpoint, wxNullBitmap, _("Create Text Point"), wxEmptyString );
    m_toolEBL = m_toolBarODToolbar->AddCheckTool( ID_MODE_EBL, _("Electronic Bearing Line"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_ebl, wxNullBitmap, _("Create EBL"), wxEmptyString );
    m_toolDR = m_toolBarODToolbar->AddCheckTool( ID_MODE_DR, _("Dead Reckoning"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_dr, wxNullBitmap, _("Create DR"), wxEmptyString );
    m_toolGZ = m_toolBarODToolbar->AddCheckTool( ID_MODE_GZ, _("Guard Zone"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_gz, wxNullBitmap, _("Create GZ"), wxEmptyString );
    m_toolPIL = m_toolBarODToolbar->AddCheckTool( ID_MODE_PIL, _("Parallel Index Line"),  *g_ocpn_draw_pi->m_pODicons->m_p_bm_ocpn_draw_pil, wxNullBitmap, _("Create PIL"), wxEmptyString );

    m_toolBarODToolbar->Realize();
    m_toolBarODToolbar->SetInitialSize();
    this->Layout();
    this->GetSizer()->Fit(this);
    //SetToolbarToolToggle(ID_NONE);
    
    Connect( wxEVT_MENU, wxCommandEventHandler( ODToolbarImpl::OnToolButtonClick ), NULL, this );
}

void ODToolbarImpl::SetColourScheme( PI_ColorScheme cs )
{
    if(m_ColourScheme != cs){
        m_ColourScheme = cs;
        wxColour col, window_back_color, gridline, uitext, udkrd, ctrl_back_color, text_color;
        GetGlobalColor( _T("DILG0"), &col );       // Dialog Background white
        GetGlobalColor( _T("DILG1"), &window_back_color );      // Dialog Background
        GetGlobalColor( _T("DILG1"), &ctrl_back_color );      // Control Background
        GetGlobalColor( _T("DILG3"), &text_color );      // Text
        GetGlobalColor( _T("UITX1"), &uitext );    // Menu Text, derived from UINFF
        GetGlobalColor( _T("UDKRD"), &udkrd );
        GetGlobalColor( _T("GREY2"), &gridline );
//TODO this does not appear to work correctly
        this->SetBackgroundColour( window_back_color );
        this->SetForegroundColour( window_back_color );
        this->ClearBackground();
        this->Refresh(true);
        this->m_toolBarODToolbar->SetBackgroundColour( window_back_color );
        this->m_toolBarODToolbar->SetForegroundColour( window_back_color );
        this->m_toolBarODToolbar->ClearBackground();
        this->m_toolBarODToolbar->Refresh(true);
    }
}

