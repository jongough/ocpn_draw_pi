/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  EBL Properties
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

#include "EBLProp.h"
#include "EBL.h"
#include "ocpn_draw_pi.h"
#include "ODUtils.h"

#if wxCHECK_VERSION(3,0,0) 
#include <wx/valnum.h>
#endif

extern EBLList                 *g_pEBLList;
extern ocpn_draw_pi            *g_ocpn_draw_pi;
extern PlugIn_Position_Fix_Ex   g_pfFix;

EBLProp::EBLProp()
{
    //ctor
}

EBLProp::EBLProp( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) 
: ODPathPropertiesDialogImpl( parent, id, caption, pos, size, style )
{
    //ctor
    m_fgSizerEBL->ShowItems( true );
    m_checkBoxEBLFixedEndPosition->Show();
    m_checkBoxEBLFixedEndPosition->Enable( true );
    
    m_fgSizerPath->ShowItems( true );
    m_checkBoxPathShowArrow->Show();
    m_checkBoxPathShowArrow->Enable( true );
    m_radioBoxPathPersistence->Show();
    m_radioBoxPathPersistence->Enable( true );
    m_radioBoxPathPersistence->SetLabel( _("EBL Persistence") );
    m_checkBoxRotateWithBoat->Enable(true);
    m_radioBoxMaintainWith->Enable(false);
    m_checkBoxEBLFixedEndPosition->Enable(true);
    m_checkBoxRotateWithBoat->Show();
    m_checkBoxRotateWithBoat->Enable(true);
    m_radioBoxMaintainWith->Show();
    m_radioBoxMaintainWith->Enable(true);
    m_textCtrlTotalLength->SetEditable(true);
    m_staticTextEBLAngle->Show();
    m_staticTextEBLAngle->Enable(true);
    m_textCtrlEBLAngle->Show();
    m_textCtrlEBLAngle->Enable(true);
    m_textCtrlEBLAngle->SetEditable(true);
    
#if wxCHECK_VERSION(3,0,0) 
    wxFloatingPointValidator<double> dODEBLAngle(2, &m_dODEBLAngleValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dODEBLLength(2, &m_dODEBLLengthValidator, wxNUM_VAL_DEFAULT);
    dODEBLAngle.SetRange(-180, 180);
    dODEBLLength.SetMin(0);
    
    m_textCtrlEBLAngle->SetValidator( dODEBLAngle );
    m_textCtrlTotalLength->SetValidator( dODEBLLength );
#endif
    
    this->GetSizer()->Fit( this );
    this->Layout();
    
}


EBLProp::~EBLProp()
{
    //dtor
    //m_checkBoxRotateWithBoat->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( EBLProp::OnRotateWithBoat ), NULL, this );
}

bool EBLProp::UpdateProperties( EBL *pInEBL )
{
    SetGlobalLocale();
    
    wxString s;

    m_checkBoxEBLFixedEndPosition->SetValue( pInEBL->m_bFixedEndPosition );
    m_radioBoxPathPersistence->SetSelection( pInEBL->m_iPersistenceType );
    m_checkBoxPathShowArrow->SetValue( pInEBL->m_bDrawArrow );
    m_checkBoxShowVRM->SetValue( pInEBL->m_bVRM );
    m_checkBoxRotateWithBoat->SetValue( pInEBL->m_bRotateWithBoat );
    m_radioBoxMaintainWith->SetSelection( pInEBL->m_iMaintainWith );
    if(pInEBL->m_bCentreOnBoat)
        m_checkBoxRotateWithBoat->Enable(true);
    else
        m_checkBoxRotateWithBoat->Enable(false);
    if(pInEBL->m_bFixedEndPosition) {
        m_radioBoxMaintainWith->Enable(false);
        m_textCtrlEBLAngle->Enable(false);
    } else {
        m_radioBoxMaintainWith->Enable(true);
        m_textCtrlEBLAngle->Enable(true);
    }
    
#if wxCHECK_VERSION(3,0,0) 
    if(pInEBL->m_dEBLAngle > 180)
        m_dODEBLAngleValidator = pInEBL->m_dEBLAngle - 360;
    else
        m_dODEBLAngleValidator = pInEBL->m_dEBLAngle;
    
    m_dODEBLLengthValidator = toUsrDistance_Plugin(pInEBL->m_dLength);
#else
    if(pInEBL->m_dEBLAngle > 180)
        s.Printf( _T("%.2f"), pInEBL->m_dEBLAngle - 360 );
    else
        s.Printf( _T("%.2f"), pInEBL->m_dEBLAngle );
    
    m_textCtrlEBLAngle->SetValue(s);
    
    s.Printf( _T("%5.2f"), toUsrDistance_Plugin(m_pEBL->m_dLength) );
    m_textCtrlTotalLength->SetValue(s);
#endif
    
    if(pInEBL->m_bRotateWithBoat) {
        m_checkBoxEBLFixedEndPosition->Enable(false);
    } else {
        m_checkBoxEBLFixedEndPosition->Enable(true);
    }
    
    return ODPathPropertiesDialogImpl::UpdateProperties( pInEBL );
}

bool EBLProp::UpdateProperties( void )
{
    wxString s;
    
    ODPathPropertiesDialogImpl::UpdateProperties();
    
    if(!m_bLockEBLAngle){
        if(m_pEBL->m_dEBLAngle > 180)
            s.Printf( _T("%.2f"), m_pEBL->m_dEBLAngle - 360 );
        else
            s.Printf( _T("%.2f"), m_pEBL->m_dEBLAngle );
        
        m_textCtrlEBLAngle->SetValue(s);
    }
    
    if(!m_bLockEBLLength) {
        s.Printf( _T("%.2f"), toUsrDistance_Plugin(m_pEBL->m_dLength) );
        m_textCtrlTotalLength->SetValue(s);
    }
    
    return  true;
}

bool EBLProp::SaveChanges( void )
{
    wxColour l_EBLOrigColour = m_pEBL->GetCurrentColour();
    ODPoint *pFirstPoint = m_pEBL->m_pODPointList->GetFirst()->GetData();

    bool l_bUpdatePath = false;
    double l_dLength;
    m_textCtrlTotalLength->GetValue().ToDouble( &l_dLength );
    l_dLength = fromUsrDistance_Plugin( l_dLength );
    if(m_pEBL->m_dLength != l_dLength) {
        l_bUpdatePath = true;
        m_pEBL->m_dLength = l_dLength;
    }
    
    if(pFirstPoint->GetODPointRangeRingsColour() == l_EBLOrigColour)
        pFirstPoint->SetODPointRangeRingsColour( m_pEBL->GetCurrentColour() );

    m_pEBL->m_bFixedEndPosition = m_checkBoxEBLFixedEndPosition->GetValue();
    m_pEBL->m_iPersistenceType = m_radioBoxPathPersistence->GetSelection();
    if(m_pEBL->m_iPersistenceType == ID_EBL_NOT_PERSISTENT || m_pEBL->m_iPersistenceType == ID_EBL_PERSISTENT_CRASH)
        m_pEBL->m_bTemporary = true;
    else
        m_pEBL->m_bTemporary = false;

    m_pEBL->m_bRotateWithBoat = m_checkBoxRotateWithBoat->GetValue();
    if(m_pEBL->m_bRotateWithBoat)
        m_pEBL->m_bFixedEndPosition = false;
    m_pEBL->m_iMaintainWith = m_radioBoxMaintainWith->GetSelection();
    
    double l_dEBLAngle;
    m_textCtrlEBLAngle->GetValue().ToDouble( &l_dEBLAngle );
    if(l_dEBLAngle != m_pEBL->m_dEBLAngle) {
        l_bUpdatePath = true;
        m_pEBL->m_dEBLAngle = l_dEBLAngle;
    }

    if(l_bUpdatePath)
        m_pEBL->MoveEndPoint(true);

    m_pEBL->m_bDrawArrow = m_checkBoxPathShowArrow->GetValue();
    m_pEBL->m_bVRM = m_checkBoxShowVRM->GetValue();
    if(m_pEBL->m_bVRM) {
        pFirstPoint->m_bShowODPointRangeRings = true;
    } else
        pFirstPoint->m_bShowODPointRangeRings = false;
    
    bool ret = ODPathPropertiesDialogImpl::SaveChanges();
    
    return ret;
}

void EBLProp::OnRotateWithBoat(wxCommandEvent& event)
{
    if(m_checkBoxRotateWithBoat->IsChecked()) {
        m_checkBoxEBLFixedEndPosition->Enable(false);
        m_radioBoxMaintainWith->Enable(true);
        m_textCtrlEBLAngle->Enable(true);
    } else {
        m_checkBoxEBLFixedEndPosition->Enable(true);
        if(m_checkBoxEBLFixedEndPosition->IsChecked()) {
            m_radioBoxMaintainWith->Enable(false);
            m_textCtrlEBLAngle->Enable(false);
        }
    }
    ODPathPropertiesDialogDef::OnRotateWithBoat(event);
}

void EBLProp::OnFixedEndPosition(wxCommandEvent& event)
{
    if(m_checkBoxEBLFixedEndPosition->IsChecked()) {
        m_radioBoxMaintainWith->Enable(false);
        m_textCtrlEBLAngle->Enable(false);
    } else {
        m_radioBoxMaintainWith->Enable(true);
        m_textCtrlEBLAngle->Enable(true);
    }
    ODPathPropertiesDialogDef::OnFixedEndPosition(event);
}

void EBLProp::OnSetFocus( wxFocusEvent& event )
{
    if(event.GetId() == m_textCtrlTotalLength->GetId())
        m_bLockEBLLength = true;
    if(event.GetId() == m_textCtrlEBLAngle->GetId())
        m_bLockEBLAngle = true;
    ODPathPropertiesDialogDef::OnSetFocus(event);
}

void EBLProp::OnKillFocus( wxFocusEvent& event )
{
    ODPathPropertiesDialogDef::OnKillFocus(event);
}

void EBLProp::OnOK( wxCommandEvent& event )
{
    m_bLockEBLLength = false;
    m_bLockEBLAngle = false;
    ODPathPropertiesDialogImpl::OnOK(event);
}

void EBLProp::OnClose( wxCloseEvent& event )
{
    m_bLockEBLLength = false;
    m_bLockEBLAngle = false;
    ODPathPropertiesDialogImpl::OnClose(event);
    
    ResetGlobalLocale();
}

void EBLProp::OnCancel( wxCommandEvent& event )
{
    m_bLockEBLLength = false;
    m_bLockEBLAngle = false;
    ODPathPropertiesDialogImpl::OnCancel(event);

    ResetGlobalLocale();
}