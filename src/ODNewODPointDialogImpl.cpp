/**************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Dead Reckoning Support
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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "ODNewODPointDialogImpl.h"

ODNewODPointDialogImpl::ODNewODPointDialogImpl( wxWindow* parent ) : ODNewODPointDialogDef( parent )
{
}
 
void ODNewODPointDialogImpl::OnOK( wxCommandEvent& event )
{
    m_bOK = true;
    if(m_radioBoxODPointType->GetSelection() == 0)
        m_iSelection = ID_ODNEWPOINTDIALOGBUTTON_BOUNDARY;
    else
        m_iSelection = ID_ODNEWPOINTDIALOGBUTTON_TEXT;
    
    Show(false);
}

void ODNewODPointDialogImpl::OnCancel( wxCommandEvent& event )
{
    m_bOK = false;
    m_iSelection = ID_ODNEWPOINTDIALOGBUTTON_BOUNDARY;
    
    Show(false);
}
 
