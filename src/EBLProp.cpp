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

EBLProp::EBLProp()
{
    //ctor
}

EBLProp::EBLProp( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) 
: ODPathPropertiesDialogImpl( parent, id, caption, pos, size, style )
{
    //ctor
    m_bLockEBLLength = false;
    m_bLockEBLAngle = false;
    m_bLockUpdate = false;

    m_staticTextTotalLength->Show();
    m_textCtrlTotalLength->Show();
    m_fgSizerEBL->ShowItems( true );
    m_fgSizerEBL1->ShowItems( true );
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
    m_fgSizerPathPoints->ShowItems( true );
    m_listCtrlODPoints->Show();
    

#if wxCHECK_VERSION(3,0,0)
    wxFloatingPointValidator<double> dODEBLAngle(2, &m_dODEBLAngleValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dODEBLLength(2, &m_dODEBLLengthValidator, wxNUM_VAL_DEFAULT);
    dODEBLAngle.SetRange(-180, 180);
    dODEBLLength.SetMin(0);
    
    m_textCtrlEBLAngle->SetValidator( dODEBLAngle );
    m_textCtrlTotalLength->SetValidator( dODEBLLength );
#endif
    
    m_scrolledWindowProperties->SetMinClientSize(m_fgSizerProperties->ComputeFittingClientSize(this));
    this->GetSizer()->Fit( this );
    this->Layout();
    if(g_iDefaultEBLPropertyDialogPostionX == -1 || g_iDefaultEBLPropertyDialogPostionY == -1) Center();
    else SetPosition(wxPoint(g_iDefaultEBLPropertyDialogPostionX, g_iDefaultEBLPropertyDialogPostionY));
    
}


EBLProp::~EBLProp()
{
    //dtor
}

bool EBLProp::UpdateProperties( ODPath *pInEBL )
{
    EBL *lpInEBL = (EBL *)pInEBL;
    SetGlobalLocale();
    
    wxString s;

    m_checkBoxEBLFixedEndPosition->SetValue( lpInEBL->m_bFixedEndPosition );
    m_radioBoxPathPersistence->SetSelection( lpInEBL->m_iPersistenceType );
    m_checkBoxPathShowArrow->SetValue( lpInEBL->m_bDrawArrow );
    m_checkBoxShowVRM->SetValue( lpInEBL->m_bVRM );
    m_checkBoxShowEBLInfo->SetValue( lpInEBL->m_bAlwaysShowInfo );
    m_checkBoxShowPerpLine->SetValue( lpInEBL->m_bPerpLine );
    m_checkBoxRotateWithBoat->SetValue( lpInEBL->m_bRotateWithBoat );
    m_radioBoxMaintainWith->SetSelection( lpInEBL->m_iMaintainWith );
    if(lpInEBL->m_bCentreOnBoat)
        m_checkBoxRotateWithBoat->Enable(true);
    else
        m_checkBoxRotateWithBoat->Enable(false);
    if(lpInEBL->m_bFixedEndPosition) {
        m_radioBoxMaintainWith->Enable(false);
        m_textCtrlEBLAngle->Enable(false);
        m_textCtrlTotalLength->SetEditable(false);
    } else {
        m_radioBoxMaintainWith->Enable(true);
        m_textCtrlEBLAngle->Enable(true);
        m_textCtrlTotalLength->SetEditable(true);
    }
    
#if wxCHECK_VERSION(3,0,0)
    if(lpInEBL->m_dEBLAngle > 180)
        m_dODEBLAngleValidator = lpInEBL->m_dEBLAngle - 360;
    else
        m_dODEBLAngleValidator = lpInEBL->m_dEBLAngle;
    
    m_dODEBLLengthValidator = toUsrDistance_Plugin(lpInEBL->m_dLength);
#else
    if(lpInEBL->m_dEBLAngle > 180)
        s.Printf( _T("%.2f"), lpInEBL->m_dEBLAngle - 360 );
    else
        s.Printf( _T("%.2f"), lpInEBL->m_dEBLAngle );
    
    m_textCtrlEBLAngle->SetValue(s);
    
    s.Printf( _T("%5.2f"), toUsrDistance_Plugin(m_pEBL->m_dLength) );
    m_textCtrlTotalLength->SetValue(s);
#endif
    
    if(lpInEBL->m_bRotateWithBoat) {
        m_checkBoxEBLFixedEndPosition->Enable(false);
    } else {
        m_checkBoxEBLFixedEndPosition->Enable(true);
    }
    
    m_bLockUpdate = false;
    
    ResetGlobalLocale();
    
    return ODPathPropertiesDialogImpl::UpdateProperties( lpInEBL );
}

bool EBLProp::UpdateProperties( void )
{
    wxString s;
    
    if(m_bLockUpdate) return true;
    
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
    if(m_pEBL->m_iPersistenceType == ID_NOT_PERSISTENT || m_pEBL->m_iPersistenceType == ID_PERSISTENT_CRASH)
        m_pEBL->m_bTemporary = true;
    else
        m_pEBL->m_bTemporary = false;

    m_pEBL->m_bRotateWithBoat = m_checkBoxRotateWithBoat->GetValue();
    if(m_pEBL->m_bRotateWithBoat)
        m_pEBL->m_bFixedEndPosition = false;
    m_pEBL->m_iMaintainWith = m_radioBoxMaintainWith->GetSelection();
    
    double l_dEBLAngle;
    m_textCtrlEBLAngle->GetValue().ToDouble( &l_dEBLAngle );
    if(m_pEBL->m_bRotateWithBoat) {
        if(l_dEBLAngle != m_pEBL->m_dEBLAngle) {
            l_bUpdatePath = true;
            m_pEBL->m_dEBLAngle = l_dEBLAngle;
        }
    } else if(!m_pEBL->m_bFixedEndPosition) {
        switch (m_pEBL->m_iMaintainWith) {
            case ID_MAINTAIN_WITH_HEADING:
                m_pEBL->m_dEBLAngle = l_dEBLAngle;
                break;
            case ID_MAINTAIN_WITH_COG:
                m_pEBL->m_dEBLAngle = l_dEBLAngle;
                break;
        }
    }

    if(l_bUpdatePath)
        m_pEBL->MoveEndPoint(true);

    m_pEBL->m_bDrawArrow = m_checkBoxPathShowArrow->GetValue();
    m_pEBL->m_bVRM = m_checkBoxShowVRM->GetValue();
    m_pEBL->m_bAlwaysShowInfo = m_checkBoxShowEBLInfo->GetValue();
    if(m_pEBL->m_bVRM) {
        pFirstPoint->m_bShowODPointRangeRings = true;
    } else
        pFirstPoint->m_bShowODPointRangeRings = false;
    m_pEBL->m_bPerpLine = m_checkBoxShowPerpLine->GetValue();

    bool ret = ODPathPropertiesDialogImpl::SaveChanges();
    
    return ret;
}

void EBLProp::OnRotateWithBoat(wxCommandEvent& event)
{
    if(m_checkBoxRotateWithBoat->IsChecked()) {
        m_checkBoxEBLFixedEndPosition->Enable(false);
        m_radioBoxMaintainWith->Enable(true);
        m_textCtrlEBLAngle->Enable(true);
        m_textCtrlTotalLength->SetEditable(true);
    } else {
        m_checkBoxEBLFixedEndPosition->Enable(true);
        if(m_checkBoxEBLFixedEndPosition->IsChecked()) {
            m_radioBoxMaintainWith->Enable(false);
            m_textCtrlEBLAngle->Enable(false);
            m_textCtrlTotalLength->SetEditable(false);
        }
    }
    ODPathPropertiesDialogDef::OnRotateWithBoat(event);
}

void EBLProp::OnFixedEndPosition(wxCommandEvent& event)
{
    if(m_checkBoxEBLFixedEndPosition->IsChecked()) {
        m_radioBoxMaintainWith->Enable(false);
        m_textCtrlEBLAngle->Enable(false);
        m_textCtrlTotalLength->SetEditable(false);
        m_checkBoxShowPerpLine->SetValue(false);
    } else {
        m_radioBoxMaintainWith->Enable(true);
        m_textCtrlEBLAngle->Enable(true);
        m_textCtrlTotalLength->SetEditable(true);
    }
    ODPathPropertiesDialogDef::OnFixedEndPosition(event);
}

void EBLProp::OnPILCheckbox(wxCommandEvent& event)
{
    if(m_checkBoxShowPerpLine->IsChecked()) {
        m_checkBoxEBLFixedEndPosition->SetValue(false);
        m_radioBoxMaintainWith->Enable(true);
        m_textCtrlEBLAngle->Enable(true);
        m_textCtrlTotalLength->SetEditable(true);
    }
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


void EBLProp::OnChoiceLineWidth( wxCommandEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnChoiceLineWidth(event);
}

void EBLProp::OnChoiceLineStyle( wxCommandEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnChoiceLineStyle(event);
}

void EBLProp::OnKillFocusChoiceLineWidth( wxFocusEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnKillFocusChoiceLineWidth(event);
}

void EBLProp::OnSetFocusChoiceLineWidth( wxFocusEvent& event )
{
    m_bLockUpdate = true;
    ODPathPropertiesDialogImpl::OnSetFocusChoiceLineWidth(event);
}

void EBLProp::OnKillFocusChoiceLineStyle( wxFocusEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnKillFocusChoiceLineStyle(event);
}

void EBLProp::OnSetFocusChoiceLineStyle( wxFocusEvent& event )
{
    m_bLockUpdate = true;
    ODPathPropertiesDialogImpl::OnSetFocusChoiceLineStyle(event);
}
