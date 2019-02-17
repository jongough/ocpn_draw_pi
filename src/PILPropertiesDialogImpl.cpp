/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  OCPN Draw Parallel Index Line Properties Dialog support
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


#include "PILPropertiesDialogImpl.h"
#include "ocpn_draw_pi.h"
#include "ocpn_plugin.h"
#include "PIL.h"
#include "PILProp.h"
#include "ODSelect.h"
#include <wx/clipbrd.h>
#include <wx/menu.h>
#include <wx/window.h>
#include <wx/fontdlg.h>

#if wxCHECK_VERSION(3,0,0)
#include <wx/valnum.h>
#endif

PILPropertiesDialogImpl::PILPropertiesDialogImpl( wxWindow* parent )
:
PILPropertiesDialogDef( parent )
{
#if wxCHECK_VERSION(3,0,0)
    wxDialog::SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);

    wxFloatingPointValidator<double> dPILOffset(3, &m_dPILOffsetValidator, wxNUM_VAL_DEFAULT);

    m_textCtrlOffset->SetValidator( dPILOffset );
#endif // wxCHECK_VERSION(3,0,0)

    DimeWindow( this );


    this->GetSizer()->Fit( this );
    this->Layout();
    if(g_iDefaultPILLinePropertyDialogPostionX == -1 || g_iDefaultPILLinePropertyDialogPostionY == -1) Center();
    else SetPosition(wxPoint(g_iDefaultPILLinePropertyDialogPostionX, g_iDefaultPILLinePropertyDialogPostionY));
    

}

PILPropertiesDialogImpl::~PILPropertiesDialogImpl()
{
}

void PILPropertiesDialogImpl::SaveChanges()
{
    std::list<PILLINE>::iterator it = m_pPIL->m_PilLineList.begin();
    while(it != m_pPIL->m_PilLineList.end()) {
        if(it->iID == m_iID) break;
        ++it;
    }
    assert(it != m_pPIL->m_PilLineList.end());
    it->sName = m_textCtrlName->GetValue();
    it->sDescription = m_textCtrlDesctription->GetValue();
    it->dOffset = wxAtof(m_textCtrlOffset->GetValue());
    it->wxcActiveColour = m_colourPickerLineColour->GetColour();
    m_pPIL->CreateColourSchemes((PILLINE*)&*it);
    m_pPIL->SetColourScheme();
    it->iStyle = (wxPenStyle)::StyleValues[m_choiceLineStyle->GetSelection()];
    it->dWidth = ::WidthValues[m_choiceLineWidth->GetSelection()];

    if(g_pPILPropDialog)
        g_pPILPropDialog->UpdateProperties(m_pPIL);

    m_pPIL->CentreOnBoat(true);;

    g_pODConfig->UpdatePath(m_pPIL);
}

void PILPropertiesDialogImpl:: UpdateProperties( PIL *pPIL, int iID )
{
    m_pPIL = pPIL;
    m_iID = iID;
    std::list<PILLINE>::iterator it = pPIL->m_PilLineList.begin();
    while(it != pPIL->m_PilLineList.end()) {
        if(it->iID == iID) break;
        ++it;
    }
    assert(it != pPIL->m_PilLineList.end());
    m_textCtrlIDNum->SetValue(wxString::Format("%i", iID));
    m_textCtrlName->Clear();
    m_textCtrlName->AppendText(it->sName);
    m_textCtrlDesctription->Clear();
    m_textCtrlDesctription->AppendText(it->sDescription);
    m_textCtrlOffset->Clear();
    m_dPILOffsetValidator = it->dOffset;
    m_colourPickerLineColour->SetColour(it->wxcActiveColour);
    for( unsigned int i = 0; i < sizeof( ::StyleValues ) / sizeof(int); i++ ) {
        if( it->iStyle == ::StyleValues[i] ) {
            m_choiceLineStyle->Select( i );
            break;
        }
    }
    for( unsigned int i = 0; i < sizeof( ::WidthValues ) / sizeof(int); i++ ) {
        if( it->dWidth == ::WidthValues[i] ) {
            m_choiceLineWidth->Select( i );
            break;
        }
    }

}

void PILPropertiesDialogImpl::OnOK( wxCommandEvent& event )
{
    SaveChanges();
    Hide();
    RequestRefresh( GetOCPNCanvasWindow() );
}

void PILPropertiesDialogImpl::OnClose( wxCloseEvent& event )
{
    wxCommandEvent nullEvent;
    OnCancel( nullEvent );
}

void PILPropertiesDialogImpl::OnCancel( wxCommandEvent& event )
{
    Hide();
    RequestRefresh( GetOCPNCanvasWindow() );
}

