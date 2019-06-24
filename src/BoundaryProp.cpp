/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Boundary Properties
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

#include "BoundaryProp.h"
#include "Boundary.h"
#include "ocpn_draw_pi.h"

BoundaryProp::BoundaryProp()
{
    //ctor
}

//BoundaryProp::BoundaryProp( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) 
//: PathProp( parent, id, caption, pos, size, style )
BoundaryProp::BoundaryProp( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) 
: ODPathPropertiesDialogImpl( parent, id, caption, pos, size, style )
{
    //ctor
    m_fgSizerBoundary->ShowItems( true );
    m_bSizerBoundaryType->ShowItems( true );
    m_fgSizerPathPoints->ShowItems( true );
    m_staticTextTotalLength->Show();
    m_textCtrlTotalLength->Show();
    m_staticTextFillColour->Show();
    m_staticTextFillColour->Enable( true );
    m_colourPickerFillColour->Show();
    m_colourPickerFillColour->Enable( true );
    m_staticTextFillTransparency->Show();
    m_staticTextFillTransparency->Enable( true );
    m_sliderFillTransparency->Show();
    m_sliderFillTransparency->Enable( true );
    m_staticTextIncluseionBoundarySize->Show();
    m_staticTextIncluseionBoundarySize->Enable( true );
    m_sliderInclusionBoundarySize->Show();
    m_sliderInclusionBoundarySize->Enable( true );
    
    m_radioBoxBoundaryType->Show();
    m_radioBoxBoundaryType->Enable( true );
    m_checkBoxShowBoundaryPoints->Show();
    m_checkBoxShowBoundaryPoints->Enable( true );
    m_fgSizerPathPoints->ShowItems( true );
    m_listCtrlODPoints->Show();
    
    m_scrolledWindowProperties->SetMinClientSize(m_fgSizerProperties->ComputeFittingClientSize(this));
    this->GetSizer()->Fit( this );
    this->Layout();
    m_uiFillTransparency = g_uiFillTransparency;
    if(g_iDefaultBoundaryPropertyDialogPostionX == -1 || g_iDefaultBoundaryPropertyDialogPostionY == -1) Center();
    SetPosition(wxPoint(g_iDefaultBoundaryPropertyDialogPostionX, g_iDefaultBoundaryPropertyDialogPostionY));
}


BoundaryProp::~BoundaryProp()
{
    //dtor
}

bool BoundaryProp::UpdateProperties( ODPath *pBoundary )
{
    m_colourPickerFillColour->SetColour( m_pBoundary->m_wxcActiveFillColour );
    m_sliderFillTransparency->SetValue( m_pBoundary->m_uiFillTransparency );
    m_sliderInclusionBoundarySize->SetValue( m_pBoundary->m_iInclusionBoundarySize );
    if(m_pBoundary->m_bExclusionBoundary && !m_pBoundary->m_bInclusionBoundary) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_EXCLUSION );
    else if(!m_pBoundary->m_bExclusionBoundary && m_pBoundary->m_bInclusionBoundary) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_INCLUSION );
    else if(!m_pBoundary->m_bExclusionBoundary && !m_pBoundary->m_bInclusionBoundary) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_NEITHER );
    else m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_EXCLUSION );
    
    if(!m_pBoundary->m_bExclusionBoundary && m_pBoundary->m_bInclusionBoundary)
        m_sliderInclusionBoundarySize->Enable();
    else
        m_sliderInclusionBoundarySize->Disable();
    
    m_checkBoxShowBoundaryPoints->SetValue( m_pBoundary->m_bODPointsVisible);
    ODPathPropertiesDialogImpl::UpdateProperties( pBoundary );
    
    return true;
}

bool BoundaryProp::SaveChanges( void )
{
    if( m_pPath && !m_pPath->m_bIsInLayer ) {
        m_pBoundary->m_wxcActiveFillColour = m_colourPickerFillColour->GetColour();  
        m_pBoundary->m_uiFillTransparency = m_sliderFillTransparency->GetValue();
        m_pBoundary->m_iInclusionBoundarySize = m_sliderInclusionBoundarySize->GetValue();
        switch (m_radioBoxBoundaryType->GetSelection()) {
            case ID_BOUNDARY_EXCLUSION:
                m_pBoundary->m_bExclusionBoundary = true;
                m_pBoundary->m_bInclusionBoundary = false;
                break;
            case ID_BOUNDARY_INCLUSION:
                m_pBoundary->m_bExclusionBoundary = false;
                m_pBoundary->m_bInclusionBoundary = true;
                break;
            case ID_BOUNDARY_NEITHER:
                m_pBoundary->m_bExclusionBoundary = false;
                m_pBoundary->m_bInclusionBoundary = false;
                break;
            default:
                m_pBoundary->m_bExclusionBoundary = true;
                m_pBoundary->m_bInclusionBoundary = false;
                break;
        }
    }
    m_pBoundary->m_bODPointsVisible = m_checkBoxShowBoundaryPoints->GetValue();
    m_pBoundary->SetPointVisibility();
    
    ODPathPropertiesDialogImpl::SaveChanges();

    return true;
}

void BoundaryProp::OnRadioBoxBoundaryType(wxCommandEvent& event)
{
    switch (m_radioBoxBoundaryType->GetSelection()) {
        case ID_BOUNDARY_EXCLUSION:
        case ID_BOUNDARY_NEITHER:
            m_sliderInclusionBoundarySize->Disable();
            break;
        case ID_BOUNDARY_INCLUSION:
            m_sliderInclusionBoundarySize->Enable();
            break;
    }
    ODPathPropertiesDialogImpl::OnRadioBoxBoundaryType(event);
}
