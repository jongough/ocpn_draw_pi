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

extern EBLList         *g_pEBLList;

EBLProp::EBLProp()
{
    //ctor
}

EBLProp::EBLProp( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) 
: ODPathPropertiesDialogImpl( parent, id, caption, pos, size, style )
{
    //ctor
    m_staticTextFillColour->Hide();
    m_staticTextFillColour->Enable( false );
    m_colourPickerFillColour->Hide();
    m_colourPickerFillColour->Enable( false );
    m_staticTextFillTransparency->Hide();
    m_staticTextFillTransparency->Enable( false );
    m_sliderFillTransparency->Hide();
    m_sliderFillTransparency->Enable( false );

    m_fgSizerEBL->ShowItems( true );
    m_checkBoxEBLFixedEndPosition->Show();
    m_checkBoxEBLFixedEndPosition->Enable( true );
    
    m_fgSizerPath->ShowItems( true );
    m_checkBoxPathShowArrow->Show();
    m_checkBoxPathShowArrow->Enable( true );
    m_radioBoxPathPersistence->Show();
    m_radioBoxPathPersistence->Enable( true );
    m_radioBoxPathPersistence->SetLabel( _("EBL Persistence") );

    this->GetSizer()->Fit( this );
    this->Layout();
}


EBLProp::~EBLProp()
{
    //dtor
}

bool EBLProp::UpdateProperties( EBL *pInEBL )
{
    m_checkBoxEBLFixedEndPosition->SetValue( pInEBL->m_bFixedEndPosition );
    m_radioBoxPathPersistence->SetSelection( pInEBL->m_iPersistenceType );
    m_checkBoxPathShowArrow->SetValue( pInEBL->m_bDrawArrow );
    m_checkBoxShowVRM->SetValue( pInEBL->m_bVRM );
    return ODPathPropertiesDialogImpl::UpdateProperties( pInEBL );
}

bool EBLProp::UpdateProperties( void )
{
    return ODPathPropertiesDialogImpl::UpdateProperties();
}

bool EBLProp::SaveChanges( void )
{
    wxColour l_EBLOrigColour = m_pEBL->GetCurrentColour();
    ODPoint *pFirstPoint = m_pEBL->m_pODPointList->GetFirst()->GetData();

    bool ret = ODPathPropertiesDialogImpl::SaveChanges();

    if(pFirstPoint->GetODPointRangeRingsColour() == l_EBLOrigColour)
        pFirstPoint->SetODPointRangeRingsColour( m_pEBL->GetCurrentColour() );

    m_pEBL->m_bFixedEndPosition = m_checkBoxEBLFixedEndPosition->GetValue();
    m_pEBL->m_iPersistenceType = m_radioBoxPathPersistence->GetSelection();
    if(m_pEBL->m_iPersistenceType == ID_EBL_NOT_PERSISTENT || m_pEBL->m_iPersistenceType == ID_EBL_PERSISTENT_CRASH)
        m_pEBL->m_bTemporary = true;
    else
        m_pEBL->m_bTemporary = false;
    m_pEBL->m_bDrawArrow = m_checkBoxPathShowArrow->GetValue();
    m_pEBL->m_bVRM = m_checkBoxShowVRM->GetValue();
    if(m_pEBL->m_bVRM) {
        pFirstPoint->m_bShowODPointRangeRings = true;
    } else
        pFirstPoint->m_bShowODPointRangeRings = false;
    
    return ret;
}
