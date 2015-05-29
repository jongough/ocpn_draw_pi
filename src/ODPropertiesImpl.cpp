/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  OCPN Draw Properties Dialog support
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

#include "ODPropertiesImpl.h"
#include "ocpn_draw_pi.h"
#include "PointMan.h"

extern PointMan     *pODPointMan;
extern int          g_path_line_width;
extern wxString     g_OD_default_wp_icon;

extern wxString     g_ActiveBoundaryLineColour;
extern wxString     g_InActiveBoundaryLineColour;
extern wxString     g_ActiveBoundaryFillColour;
extern wxString     g_InActiveBoundaryFillColour;
extern int          g_BoundaryLineWidth; 
extern int          g_BoundaryLineStyle;
extern wxString     g_ActivePathLineColour;
extern wxString     g_InActivePathLineColour;
extern wxString     g_ActivePathFillColour;
extern wxString     g_InActivePathFillColour;
extern int          g_PathLineWidth; 
extern int          g_PathLineStyle;

extern bool         g_bODPointShowName;
extern bool         g_bODPointShowRangeRings;
extern int          g_iODPointRangeRingsNumber;
extern float        g_fODPointRangeRingsStep;
extern int          g_iODPointRangeRingsStepUnits;
extern wxColour     g_colourODPointRangeRingsColour;
extern wxString     g_sODPointIconName;

extern double       g_n_arrival_circle_radius;

extern bool         g_bConfirmObjectDelete;
extern int          g_navobjbackups;


ODPropertiesImpl::ODPropertiesImpl( wxWindow* parent )
:
ODPropertiesDialog( parent )
{
    m_bcomboBoxODPointIcon = new wxBitmapComboBox( m_panelPoint, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN ); 
    m_SizerNameIcon->Replace( m_comboBoxODPointIconName, m_bcomboBoxODPointIcon );
    delete( m_comboBoxODPointIconName );
    m_comboBoxODPointIconName = NULL;

}

void ODPropertiesImpl::OnDrawPropertiesOKClick( wxCommandEvent& event )
{
    // TODO: Implement OnDrawPropertiesOKClick
    
    SaveChanges(); // write changes to globals and update config
    Show( false );

    SetClientSize(m_defaultClientSize);

    event.Skip();
}

void ODPropertiesImpl::OnDrawPropertiesCancelClick( wxCommandEvent& event )
{
    // TODO: Implement OnDrawPropertiesCancelClick
    Show( false );
    SetClientSize(m_defaultClientSize);

    event.Skip();
}

void ODPropertiesImpl::OnDrawPropertiesApplyClick( wxCommandEvent& event )
{
    // TODO: Implement OnDrawPropertiesApplyClick
    SaveChanges(); // write changes to globals and update config

    event.Skip();
}

void ODPropertiesImpl::SaveChanges()
{
        if (m_choiceActiveBoundaryLineColour->GetSelection() == 0 ) g_ActiveBoundaryLineColour = wxEmptyString;
        else g_ActiveBoundaryLineColour = ::GpxxColorNames[m_choiceActiveBoundaryLineColour->GetSelection()];
        if (m_choiceInActiveBoundaryLineColour->GetSelection() == 0 ) g_InActiveBoundaryLineColour = wxEmptyString;
        else g_InActiveBoundaryLineColour = ::GpxxColorNames[m_choiceInActiveBoundaryLineColour->GetSelection()];

        if (m_choiceActiveBoundaryFillColour->GetSelection() == 0 ) g_ActiveBoundaryFillColour = wxEmptyString;
        else g_ActiveBoundaryFillColour = ::GpxxColorNames[m_choiceActiveBoundaryFillColour->GetSelection()];
        if (m_choiceInActiveBoundaryFillColour->GetSelection() == 0 ) g_InActiveBoundaryFillColour = wxEmptyString;
        else g_InActiveBoundaryFillColour = ::GpxxColorNames[m_choiceInActiveBoundaryFillColour->GetSelection()];

        g_BoundaryLineWidth = m_choiceBoundaryLineWidth->GetSelection() + 1;
        g_BoundaryLineStyle = ::StyleValues[ m_choiceBoundaryLineStyle->GetSelection()];
        
        if (m_choiceActivePathLineColour->GetSelection() == 0 ) g_ActivePathLineColour = wxEmptyString;
        else g_ActivePathLineColour = ::GpxxColorNames[m_choiceActivePathLineColour->GetSelection()];
        if (m_choiceInActivePathLineColour->GetSelection() == 0 ) g_InActivePathLineColour = wxEmptyString;
        else g_InActivePathLineColour = ::GpxxColorNames[m_choiceInActivePathLineColour->GetSelection()];

        if (m_choiceActivePathFillColour->GetSelection() == 0 ) g_ActivePathFillColour = wxEmptyString;
        else g_ActivePathFillColour = ::GpxxColorNames[m_choiceActivePathFillColour->GetSelection()];
        if (m_choiceInActivePathFillColour->GetSelection() == 0 ) g_InActivePathFillColour = wxEmptyString;
        else g_InActivePathFillColour = ::GpxxColorNames[m_choiceInActivePathFillColour->GetSelection()];

        g_PathLineWidth = m_choicePathLineWidth->GetSelection() + 1;
        g_PathLineStyle = ::StyleValues[ m_choicePathLineStyle->GetSelection()];
        
        g_iODPointRangeRingsNumber = m_choiceODPointRangeRingNumber->GetSelection();
        g_fODPointRangeRingsStep = atof( m_textCtrlODPointRangeRingSteps->GetValue().mb_str() );
        g_iODPointRangeRingsStepUnits = m_choiceODPointDistanceUnit->GetSelection();
        g_colourODPointRangeRingsColour = m_colourPickerODPointRangeRingColours->GetColour();
        m_textCtrlODPointArrivalRadius->GetValue().ToDouble( &g_n_arrival_circle_radius );
        g_bODPointShowRangeRings = m_checkBoxShowODPointRangeRings->GetValue();
        g_sODPointIconName = m_bcomboBoxODPointIcon->GetValue();
        g_bConfirmObjectDelete = m_checkBoxConfirmObjectDelete->GetValue();
        g_navobjbackups = m_spinCtrlNavObjBackups->GetValue();
}

void ODPropertiesImpl::SetDialogSize( void )
{
    wxSize sz = m_SizerProperties->CalcMin();
    sz.IncBy( 20 );   // Account for some decorations?
    wxSize dsize = ::wxGetDisplaySize();
    sz.y = wxMin(sz.y, dsize.y-80);
//    sz = wxSize(600, 400);
    SetClientSize(sz);
    m_defaultClientSize = sz;
    //m_panelBasicProperties->SetScrollRate(5, 5);

    wxSize fsize = GetSize();
    fsize.y = wxMin(fsize.y, dsize.y-80);
    fsize.x = wxMin(fsize.x, dsize.x-80);
    SetSize(fsize);
    
}

void ODPropertiesImpl::UpdateProperties( void )
{
        wxString s_ArrivalRadius;
        s_ArrivalRadius.Printf( _T("%.3f"), g_n_arrival_circle_radius );
        m_textCtrlODPointArrivalRadius->SetValue( s_ArrivalRadius );
        
        m_checkBoxShowName->SetValue( g_bODPointShowName );
        m_checkBoxShowODPointRangeRings->SetValue( g_bODPointShowRangeRings );
        m_choiceODPointRangeRingNumber->SetSelection( g_iODPointRangeRingsNumber );
        wxString s_RangeRingStep;
        s_RangeRingStep.Printf( wxT("%.3f"), g_fODPointRangeRingsStep );
        m_textCtrlODPointRangeRingSteps->SetValue( s_RangeRingStep );
        m_choiceODPointDistanceUnit->SetSelection( g_iODPointRangeRingsStepUnits );
        m_colourPickerODPointRangeRingColours->SetColour( g_colourODPointRangeRingsColour );

        m_bcomboBoxODPointIcon->Clear();
        //      Iterate on the Icon Descriptions, filling in the combo control
        if( pODPointMan == NULL ) pODPointMan = new PointMan();
        
        bool fillCombo = m_bcomboBoxODPointIcon->GetCount() == 0;
        wxImageList *icons = pODPointMan->Getpmarkicon_image_list();

        if( fillCombo  && icons){
            for( int i = 0; i < pODPointMan->GetNumIcons(); i++ ) {
                wxString *ps = pODPointMan->GetIconDescription( i );
                m_bcomboBoxODPointIcon->Append( *ps, icons->GetBitmap( i ) );
            }
        }
        
        // find the correct item in the combo box
        int iconToSelect = -1;
        for( int i = 0; i < pODPointMan->GetNumIcons(); i++ ) {
            if( *pODPointMan->GetIconKey( i ) == g_sODPointIconName ) {
                iconToSelect = i;
                break;
            }
        }

        //  not found, so add  it to the list, with a generic bitmap and using the name as description
        // n.b.  This should never happen...
        if( -1 == iconToSelect){    
            m_bcomboBoxODPointIcon->Append( g_sODPointIconName, icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxODPointIcon->GetCount() - 1;
        }
        
        
        m_bcomboBoxODPointIcon->Select( iconToSelect );
        icons = NULL;

        for( unsigned int i = 0; i < sizeof( ::GpxxColorNames ) / sizeof(wxString); i++ ) {
            if( g_ActiveBoundaryLineColour == ::GpxxColorNames[i] )
                m_choiceActiveBoundaryLineColour->Select( i );
            if( g_InActiveBoundaryLineColour == ::GpxxColorNames[i] )
                m_choiceInActiveBoundaryLineColour->Select( i );
            if( g_ActiveBoundaryFillColour == ::GpxxColorNames[i] )
                m_choiceActiveBoundaryFillColour->Select( i );
            if( g_InActiveBoundaryFillColour == ::GpxxColorNames[i] )
                m_choiceInActiveBoundaryFillColour->Select( i );
            if( g_ActivePathLineColour == ::GpxxColorNames[i] )
                m_choiceActivePathLineColour->Select( i );
            if( g_InActivePathLineColour == ::GpxxColorNames[i] )
                m_choiceInActivePathLineColour->Select( i );
            if( g_ActivePathFillColour == ::GpxxColorNames[i] )
                m_choiceActivePathFillColour->Select( i );
            if( g_InActivePathFillColour == ::GpxxColorNames[i] )
                m_choiceInActivePathFillColour->Select( i );
        }
        
        for( unsigned int i = 0; i < sizeof( ::StyleValues ) / sizeof(int); i++ ) {
            if( g_BoundaryLineStyle == ::StyleValues[i] )
                m_choiceBoundaryLineStyle->Select( i );
            if( g_PathLineStyle == ::StyleValues[i] )
                m_choicePathLineStyle->Select( i );
        }
        m_choiceBoundaryLineWidth->SetSelection( g_BoundaryLineWidth - 1 );
        m_choicePathLineWidth->SetSelection( g_PathLineWidth - 1 );
        
        m_checkBoxConfirmObjectDelete->SetValue( g_bConfirmObjectDelete );
        m_spinCtrlNavObjBackups->SetValue( g_navobjbackups );
        

    return;
}

