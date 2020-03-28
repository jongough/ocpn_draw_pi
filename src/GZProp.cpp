/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  GZ Properties
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

#include "GZProp.h"
#include "GZ.h"
#include "ocpn_draw_pi.h"
#include "ODConfig.h"
#include "ODUtils.h"

#if wxCHECK_VERSION(3,0,0) 
#include <wx/valnum.h>
#endif

GZProp::GZProp()
{
    //ctor
}

GZProp::GZProp( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) 
: ODPathPropertiesDialogImpl( parent, id, caption, pos, size, style )
{
    m_staticTextFillColour->Show();
    m_staticTextFillColour->Enable( true );
    m_colourPickerFillColour->Show();
    m_colourPickerFillColour->Enable( true );
    m_staticTextFillTransparency->Show();
    m_staticTextFillTransparency->Enable( true );
    m_sliderFillTransparency->Show();
    m_sliderFillTransparency->Enable( true );
    m_fgSizerGZ->ShowItems( true );
    m_staticTextGZFirstAngle->Show();
    m_staticTextGZFirstAngle->Enable( true );
    m_textCtrlGZFirstAngle->Show();
    m_textCtrlGZFirstAngle->Enable( true );
    m_staticTextGZFirstLength->Show();
    m_staticTextGZFirstLength->Enable( true );
    m_textCtrlGZFirstLength->Show();
    m_textCtrlGZFirstLength->Enable( true );
    m_staticTextGZSecondAngle->Show();
    m_staticTextGZSecondAngle->Enable( true );
    m_textCtrlGZSecondAngle->Show();
    m_textCtrlGZSecondAngle->Enable( true );
    m_staticTextGZSecondLength->Show();
    m_staticTextGZSecondLength->Enable( true );
    m_textCtrlGZSecondLength->Show();
    m_textCtrlGZSecondLength->Enable( true );
    m_fgSizerPath->ShowItems( true );
    m_radioBoxPathPersistence->Show();
    m_radioBoxPathPersistence->Enable( true );
    m_radioBoxPathPersistence->SetLabel( _("Guard Zone Persistence") );
    m_fgSizerPathPoints->ShowItems( true );
    m_listCtrlODPoints->Show();
    

#if wxCHECK_VERSION(3,0,0)
    wxFloatingPointValidator<double> dODGZFirstAngle(2, &m_dODGZFirstAngleValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dODGZSecondAngle(2, &m_dODGZSecondAngleValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dODGZFirstLength(3, &m_dODGZFirstLengthValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dODGZSecondLength(3, &m_dODGZSecondLengthValidator, wxNUM_VAL_DEFAULT);
    dODGZFirstAngle.SetRange(-180, 180);
    dODGZSecondAngle.SetRange(-180, 180);
    dODGZFirstLength.SetMin(0);
    dODGZSecondLength.SetMin(0);
    
    m_textCtrlGZFirstAngle->SetValidator( dODGZFirstAngle );
    m_textCtrlGZSecondAngle->SetValidator( dODGZSecondAngle );
    m_textCtrlGZFirstLength->SetValidator( dODGZFirstLength );
    m_textCtrlGZSecondLength->SetValidator( dODGZSecondLength );
    #endif
    
    m_scrolledWindowProperties->SetMinClientSize(m_fgSizerProperties->ComputeFittingClientSize(this));
    this->GetSizer()->Fit( this );
    this->Layout();
    
    if(g_iDefaultGZPropertyDialogPostionX == -1 || g_iDefaultGZPropertyDialogPostionY == -1) Center();
    else SetPosition(wxPoint(g_iDefaultGZPropertyDialogPostionX, g_iDefaultGZPropertyDialogPostionY));
 
    m_bLockGZAngle = false;
    m_bLockGZLength = false;
    m_bLockUpdate = false;
}


GZProp::~GZProp()
{
    //dtor
}

bool GZProp::UpdateProperties( ODPath *pInGZ )
{
    GZ *lpInGZ = (GZ *)pInGZ;
    SetGlobalLocale();
    
    wxString s;

    m_colourPickerFillColour->SetColour( m_pGZ->m_wxcActiveFillColour );
    m_sliderFillTransparency->SetValue( m_pGZ->m_uiFillTransparency );

    m_checkBoxRotateGZWithBoat->SetValue( m_pGZ->m_bRotateWithBoat );
    m_radioBoxMaintainGZWith->SetSelection( m_pGZ->m_iMaintainWith );
    m_checkBoxRotateGZWithBoat->Enable(true);

    m_radioBoxMaintainGZWith->Enable(true);
    m_textCtrlGZFirstAngle->SetEditable(true);
    m_textCtrlGZSecondAngle->SetEditable(true);
    m_textCtrlGZFirstLength->SetEditable(true);
    m_textCtrlGZSecondLength->SetEditable(true);

    m_radioBoxGZLineType->Enable(true);
    m_radioBoxGZLineType->SetSelection( m_pGZ->m_iLineType );

#if wxCHECK_VERSION(3,0,0)
    if(!m_bLockGZAngle) {
        if(lpInGZ->m_dFirstLineDirection > 180)
            m_dODGZFirstAngleValidator = lpInGZ->m_dFirstLineDirection - 360;
        else
            m_dODGZFirstAngleValidator = lpInGZ->m_dFirstLineDirection;
        if(lpInGZ->m_dSecondLineDirection > 180)
            m_dODGZSecondAngleValidator = lpInGZ->m_dSecondLineDirection - 360;
        else
            m_dODGZSecondAngleValidator = lpInGZ->m_dSecondLineDirection;
        m_textCtrlGZFirstAngle->GetValidator()->TransferToWindow();
        m_textCtrlGZSecondAngle->GetValidator()->TransferToWindow();
    }

    if(!m_bLockGZLength) {
        m_dODGZFirstLengthValidator = lpInGZ->m_dFirstDistance;
        m_dODGZSecondLengthValidator = lpInGZ->m_dSecondDistance;
        m_textCtrlGZFirstLength->GetValidator()->TransferToWindow();
        m_textCtrlGZSecondLength->GetValidator()->TransferToWindow();
    }
#else
    if(!m_bLockGZAngle) {
        if(lpInGZ->m_dFirstLineDirection > 180)
            s.Printf( _T("%.2f"), lpInGZ->m_dFirstLineDirection - 360 );
        else
            s.Printf( _T("%.3f"), lpInGZ->m_dFirstLineDirection );
        
        m_textCtrlGZFirstAngle->SetValue(s);
        
        if(lpInGZ->m_dSecondLineDirection > 180)
            s.Printf( _T("%.2f"), lpInGZ->m_dSecondLineDirection - 360 );
        else
            s.Printf( _T("%.3f"), lpInGZ->m_dSecondLineDirection );
        m_textCtrlGZSecondAngle->SetValue(s);
    }

    if(!m_bLockGZLength) {
        s.Printf( _T("%.3f"), toUsrDistance_Plugin(lpInGZ->m_dFirstDistance) );
        m_textCtrlGZFirstLength->SetValue(s);
        s.Printf( _T("%.3f"), toUsrDistance_Plugin(lpInGZ->m_dSecondDistance) );
        m_textCtrlGZSecondLength->SetValue(s);
    }
#endif

    m_bLockGZAngle = false;
    m_bLockGZLength = false;
    
    m_radioBoxPathPersistence->SetSelection( lpInGZ->m_iPersistenceType );

    m_bLockUpdate = false;
    ResetGlobalLocale();
    
    return ODPathPropertiesDialogImpl::UpdateProperties( pInGZ );
}

bool GZProp::UpdateProperties( void )
{
    wxString s;
    
    if(m_bLockUpdate) return true;
    
    ODPathPropertiesDialogImpl::UpdateProperties();
    
#if wxCHECK_VERSION(3,0,0)
    if(!m_bLockGZAngle) {
        if(m_pGZ->m_dFirstLineDirection > 180)
            m_dODGZFirstAngleValidator = m_pGZ->m_dFirstLineDirection - 360;
        else
            m_dODGZFirstAngleValidator = m_pGZ->m_dFirstLineDirection;
        if(m_pGZ->m_dSecondLineDirection > 180)
            m_dODGZSecondAngleValidator = m_pGZ->m_dSecondLineDirection - 360;
        else
            m_dODGZSecondAngleValidator = m_pGZ->m_dSecondLineDirection;
        m_textCtrlGZFirstAngle->GetValidator()->TransferToWindow();
        m_textCtrlGZSecondAngle->GetValidator()->TransferToWindow();
    }

    if(!m_bLockGZLength) {
        m_dODGZFirstLengthValidator = m_pGZ->m_dFirstDistance;
        m_dODGZSecondLengthValidator = m_pGZ->m_dSecondDistance;
        m_textCtrlGZFirstLength->GetValidator()->TransferToWindow();
        m_textCtrlGZSecondLength->GetValidator()->TransferToWindow();
    }
#else
    if(!m_bLockGZAngle) {
        if(m_pGZ->m_dFirstLineDirection > 180)
            s.Printf( _T("%.2f"), m_pGZ->m_dFirstLineDirection - 360 );
        else
            s.Printf( _T("%.3f"), m_pGZ->m_dFirstLineDirection );

        m_textCtrlGZFirstAngle->SetValue(s);

        if(m_pGZ->m_dSecondLineDirection > 180)
            s.Printf( _T("%.2f"), m_pGZ->m_dSecondLineDirection - 360 );
        else
            s.Printf( _T("%.3f"), m_pGZ->m_dSecondLineDirection );
        m_textCtrlGZSecondAngle->SetValue(s);
    }

    if(!m_bLockGZLength) {
        s.Printf( _T("%.3f"), toUsrDistance_Plugin(m_pGZ->m_dFirstDistance) );
        m_textCtrlGZFirstLength->SetValue(s);
        s.Printf( _T("%.3f"), toUsrDistance_Plugin(m_pGZ->m_dSecondDistance) );
        m_textCtrlGZSecondLength->SetValue(s);
    }
#endif

    return  true;
}

bool GZProp::SaveChanges( void )
{
    m_pGZ->m_wxcActiveFillColour = m_colourPickerFillColour->GetColour();    
    m_pGZ->m_uiFillTransparency = m_sliderFillTransparency->GetValue();
    
    wxColour l_GZOrigColour = m_pGZ->GetCurrentColour();

    bool l_bUpdatePath = false;
    double l_dLength;
    m_textCtrlGZFirstLength->GetValue().ToDouble( &l_dLength );
    l_dLength = fromUsrDistance_Plugin( l_dLength );
    if(m_pGZ->m_dFirstDistance != l_dLength) {
        l_bUpdatePath = true;
        m_pGZ->m_dFirstDistance = l_dLength;
    }
    
    m_textCtrlGZSecondLength->GetValue().ToDouble( &l_dLength );
    l_dLength = fromUsrDistance_Plugin( l_dLength );
    if(m_pGZ->m_dSecondDistance != l_dLength) {
        l_bUpdatePath = true;
        m_pGZ->m_dSecondDistance = l_dLength;
    }
    
    m_pGZ->m_bRotateWithBoat = m_checkBoxRotateGZWithBoat->GetValue();
    m_pGZ->m_iMaintainWith = m_radioBoxMaintainGZWith->GetSelection();
    m_pGZ->m_iLineType = m_radioBoxGZLineType->GetSelection();

    double l_dGZAngle;
    m_textCtrlGZFirstAngle->GetValue().ToDouble( &l_dGZAngle );
        if(l_dGZAngle != m_pGZ->m_dFirstLineDirection) {
            l_bUpdatePath = true;
            m_pGZ->m_dFirstLineDirection = l_dGZAngle;
    }
    
    m_textCtrlGZSecondAngle->GetValue().ToDouble( &l_dGZAngle );
        if(l_dGZAngle != m_pGZ->m_dSecondLineDirection) {
            l_bUpdatePath = true;
            m_pGZ->m_dSecondLineDirection = l_dGZAngle;
    }
    
    m_pGZ->m_iPersistenceType = m_radioBoxPathPersistence->GetSelection();
    if(m_pGZ->m_iPersistenceType == ID_NOT_PERSISTENT || m_pGZ->m_iPersistenceType == ID_PERSISTENT_CRASH)
        m_pGZ->m_bTemporary = true;
    else
        m_pGZ->m_bTemporary = false;
    bool ret;
    if(m_pGZ->m_iPersistenceType == ID_NOT_PERSISTENT)
        ret = g_pODConfig->DeleteConfigPath( m_pGZ );
    else
        ret = ODPathPropertiesDialogImpl::SaveChanges();

    if(l_bUpdatePath) {
        m_pGZ->UpdateGZ();
    }
        
    return ret;
}

void GZProp::OnRotateWithBoat(wxCommandEvent& event)
{
    if(m_checkBoxRotateWithBoat->IsChecked()) {
        m_radioBoxMaintainWith->Enable(true);
        m_textCtrlGZFirstAngle->SetEditable(true);
        m_textCtrlGZSecondAngle->SetEditable(true);
        m_textCtrlGZFirstLength->SetEditable(true);
        m_textCtrlGZSecondLength->SetEditable(true);
    }
    
    ODPathPropertiesDialogDef::OnRotateWithBoat(event);
}

void GZProp::OnSetFocus( wxFocusEvent& event )
{
    if(event.GetId() == m_textCtrlGZFirstLength->GetId() || event.GetId() == m_textCtrlGZSecondLength->GetId())
        m_bLockGZLength = true;
    if(event.GetId() == m_textCtrlGZFirstAngle->GetId() || event.GetId() == m_textCtrlGZSecondAngle->GetId())
        m_bLockGZAngle = true;
    ODPathPropertiesDialogDef::OnSetFocus(event);
}

void GZProp::OnKillFocus( wxFocusEvent& event )
{
    ODPathPropertiesDialogDef::OnKillFocus(event);
}

void GZProp::OnOK( wxCommandEvent& event )
{
    m_bLockGZLength = false;
    m_bLockGZAngle = false;
    ODPathPropertiesDialogImpl::OnOK(event);
}

void GZProp::OnClose( wxCloseEvent& event )
{
    m_bLockGZLength = false;
    m_bLockGZAngle = false;
    ODPathPropertiesDialogImpl::OnClose(event);
    
    ResetGlobalLocale();
}

void GZProp::OnCancel( wxCommandEvent& event )
{
    m_bLockGZLength = false;
    m_bLockGZAngle = false;
    ODPathPropertiesDialogImpl::OnCancel(event);

    ResetGlobalLocale();
}


void GZProp::OnChoiceLineWidth( wxCommandEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnChoiceLineWidth(event);
}

void GZProp::OnChoiceLineStyle( wxCommandEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnChoiceLineStyle(event);
}

void GZProp::OnKillFocusChoiceLineWidth( wxFocusEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnKillFocusChoiceLineWidth(event);
}

void GZProp::OnSetFocusChoiceLineWidth( wxFocusEvent& event )
{
    m_bLockUpdate = true;
    ODPathPropertiesDialogImpl::OnSetFocusChoiceLineWidth(event);
}

void GZProp::OnKillFocusChoiceLineStyle( wxFocusEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnKillFocusChoiceLineStyle(event);
}

void GZProp::OnSetFocusChoiceLineStyle( wxFocusEvent& event )
{
    m_bLockUpdate = true;
    ODPathPropertiesDialogImpl::OnSetFocusChoiceLineStyle(event);
}
