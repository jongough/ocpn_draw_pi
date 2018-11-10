/**************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  HyperLink dialog
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

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ODLinkPropertiesDialogImpl.h"

ODLinkPropertiesDialogImpl::ODLinkPropertiesDialogImpl( wxWindow* parent )
:
ODLinkPropertiesDialogDef( parent )
{
    Layout();
    Fit();
    
    Centre( wxBOTH );
    
}

void ODLinkPropertiesDialogImpl::OnCancelClick(wxCommandEvent& event)
{
    Show( false );
    SetReturnCode(wxID_CANCEL);
    event.Skip();
}

void ODLinkPropertiesDialogImpl::OnOKClick(wxCommandEvent& event)
{
    if(m_textCtrlLinkDescription->GetValue() == wxEmptyString && m_filePickerLocalFile->GetFileName().GetFullPath() != wxEmptyString)
        m_textCtrlLinkDescription->SetValue(m_filePickerLocalFile->GetFileName().GetFullPath());
    if(m_textCtrlURL->GetValue() == wxEmptyString && m_filePickerLocalFile->GetFileName().GetFullPath() != wxEmptyString)
        m_textCtrlURL->SetValue(m_filePickerLocalFile->GetFileName().GetFullPath());
    
    Show( false );
    SetReturnCode(wxID_OK);
    event.Skip();
}

void ODLinkPropertiesDialogImpl::SetODPoint(ODPoint *pODPoint)
{
    m_pODPoint = pODPoint;
    m_textCtrlLinkDescription->SetValue(wxEmptyString);
    m_textCtrlURL->SetValue(wxEmptyString);
}

void ODLinkPropertiesDialogImpl::AddURL(void)
{
    m_textCtrlLinkDescription->SetValue(wxEmptyString);
    m_textCtrlURL->SetValue(wxEmptyString);
    
}

void ODLinkPropertiesDialogImpl::SetLinkDescription(wxString lDesc)
{
    m_textCtrlLinkDescription->SetValue(lDesc);
}

void ODLinkPropertiesDialogImpl::SetLinkURL(wxString lURL)
{
    m_textCtrlURL->SetValue(lURL);
}

void ODLinkPropertiesDialogImpl::OnFileChanged(wxFileDirPickerEvent& event)
{
    m_textCtrlLinkDescription->SetValue(m_filePickerLocalFile->GetFileName().GetFullPath());
    m_textCtrlURL->SetValue(m_filePickerLocalFile->GetFileName().GetFullPath());
}

