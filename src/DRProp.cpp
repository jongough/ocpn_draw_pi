/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  DR Properties
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

#include "DRProp.h"
#include "DR.h"
#include "ocpn_draw_pi.h"

DRProp::DRProp()
{
    //ctor
}

DRProp::DRProp( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) 
: ODPathPropertiesDialogImpl( parent, id, caption, pos, size, style )
{
    //ctor
    
    m_fgSizerPath->ShowItems( true );
    
    m_staticTextTotalLength->Show();
    m_textCtrlTotalLength->Show();
    m_checkBoxPathShowArrow->Show();
    m_checkBoxPathShowArrow->Enable( true );
    m_radioBoxPathPersistence->Show();
    m_radioBoxPathPersistence->Enable( true );
    m_fgSizerPathPoints->ShowItems( true );
    m_listCtrlODPoints->Show();
    
    m_scrolledWindowProperties->SetMinClientSize(m_fgSizerProperties->ComputeFittingClientSize(this));
    this->GetSizer()->Fit( this );
    this->Layout();
    if(g_iDefaultDRPropertyDialogPostionX == -1 || g_iDefaultDRPropertyDialogPostionY == -1) Center();
    else SetPosition(wxPoint(g_iDefaultDRPropertyDialogPostionX, g_iDefaultDRPropertyDialogPostionY));
}


DRProp::~DRProp()
{
    //dtor
}

bool DRProp::UpdateProperties( ODPath *pDR )
{
    m_checkBoxPathShowArrow->SetValue( m_pDR->m_bDrawArrow );
    m_radioBoxPathPersistence->SetSelection( m_pDR->m_iPersistenceType);
    
    return ODPathPropertiesDialogImpl::UpdateProperties( pDR );
    
}

bool DRProp::SaveChanges( void )
{
    m_pDR->m_bDrawArrow = m_checkBoxPathShowArrow->GetValue();
    m_pDR->m_iPersistenceType = m_radioBoxPathPersistence->GetSelection();
    ODPathPropertiesDialogImpl::SaveChanges();

    return true;
}

