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

#include "ODPropertiesDialogImpl.h"
#include "ocpn_draw_pi.h"
#include "PointMan.h"
#include "version.h"
#include <wx/fontdlg.h>

extern PointMan     *g_pODPointMan;
extern int          g_path_line_width;
extern wxString     g_OD_default_wp_icon;

extern wxString     g_ActiveBoundaryLineColour;
extern wxString     g_InActiveBoundaryLineColour;
extern wxString     g_ActiveBoundaryFillColour;
extern wxString     g_InActiveBoundaryFillColour;
extern wxColour     g_colourActiveBoundaryLineColour;
extern wxColour     g_colourInActiveBoundaryLineColour;
extern wxColour     g_colourActiveBoundaryFillColour;
extern wxColour     g_colourInActiveBoundaryFillColour;
extern int          g_BoundaryLineWidth; 
extern int          g_BoundaryLineStyle;
extern wxColour     g_colourActivePathLineColour;
extern wxColour     g_colourInActivePathLineColour;
extern wxColour     g_colourActiveBoundaryLineColour;
extern wxColour     g_colourInActiveBoundaryLineColour;
extern wxColour     g_colourActiveBoundaryFillColour;
extern wxColour     g_colourInActiveBoundaryFillColour;
extern int          g_PathLineWidth; 
extern int          g_PathLineStyle;
extern unsigned int g_uiFillTransparency;

extern bool         g_bODPointShowName;
extern bool         g_bODPointShowRangeRings;
extern int          g_iODPointRangeRingsNumber;
extern float        g_fODPointRangeRingsStep;
extern int          g_iODPointRangeRingsStepUnits;
extern wxColour     g_colourODPointRangeRingsColour;
extern wxString     g_sODPointIconName;

extern double       g_n_arrival_circle_radius;

extern wxColour     g_colourDefaultTextColour;
extern wxColour     g_colourDefaultTextBackgroundColour;
extern int          g_iTextBackgroundTransparency;
extern wxFont       g_DisplayTextFont;
extern int          g_iTextPosition;
extern int          g_iTextTopOffsetX;
extern int          g_iTextTopOffsetY;
extern int          g_iTextBottomOffsetX;
extern int          g_iTextBottomOffsetY;
extern int          g_iTextRightOffsetX;
extern int          g_iTextRightOffsetY;
extern int          g_iTextLeftOffsetX;
extern int          g_iTextLeftOffsetY;

extern wxString     g_sEBLEndIconName;
extern wxColour     g_colourEBLLineColour;
extern bool         g_bEBLFixedEndPosition;
extern int          g_EBLPersistenceType;
extern int          g_EBLLineWidth;
extern int          g_EBLLineStyle;

extern bool         g_bConfirmObjectDelete;
extern int          g_navobjbackups;

extern int          g_EdgePanSensitivity;
extern int          g_InitialEdgePanSensitivity;

extern int          g_iDisplayToolbar;

ODPropertiesDialogImpl::ODPropertiesDialogImpl( wxWindow* parent )
:
ODPropertiesDialogDef( parent )
{
    
    m_staticTextNameVal->SetLabel( wxT("OpenCPN Draw Plugin") );
    m_staticTextMajorVal->SetLabel(wxString::Format(wxT("%i"), PLUGIN_VERSION_MAJOR ));
    m_staticTextMinorVal->SetLabel(wxString::Format(wxT("%i"), PLUGIN_VERSION_MINOR ));
    m_staticTextPatchVal->SetLabel( wxT(TOSTRING(PLUGIN_VERSION_PATCH)) );
    m_staticTextDateVal->SetLabel(PLUGIN_VERSION_DATE);
    
    m_pfdDialog = NULL;

}

void ODPropertiesDialogImpl::OnODPointComboboxSelected( wxCommandEvent& event )
{
    m_bitmapPointBitmap->SetBitmap( m_bcomboBoxODPointIconName->GetItemBitmap( m_bcomboBoxODPointIconName->GetSelection() ) );
}

void ODPropertiesDialogImpl::OnEBLEndIconComboboxSelected( wxCommandEvent& event )
{
    m_bitmapEBLEndBitmap->SetBitmap( m_bcomboBoxEBLEndIconName->GetItemBitmap( m_bcomboBoxEBLEndIconName->GetSelection() ) );
}

void ODPropertiesDialogImpl::OnButtonClickFonts( wxCommandEvent& event )
{
    if(m_pfdDialog) delete m_pfdDialog;
    
    wxFontData l_FontData;
    l_FontData.SetInitialFont( g_DisplayTextFont );
    m_pfdDialog = new wxFontDialog( this, l_FontData );
    m_pfdDialog->Centre( wxBOTH );
    
    int iRet = m_pfdDialog->ShowModal();
    if(iRet == wxID_OK) {
        //wxFontData wsfdData = m_pfdDialog->GetFontData();
        m_staticTextFontFaceExample->SetFont(m_pfdDialog->GetFontData().GetChosenFont());
    }
}

void ODPropertiesDialogImpl::OnDrawPropertiesOKClick( wxCommandEvent& event )
{
    SaveChanges(); // write changes to globals and update config
    Show( false );
#ifdef __WXOSX__    
    EndModal(wxID_OK);
#endif
    SetClientSize(m_defaultClientSize);

    event.Skip();
}

void ODPropertiesDialogImpl::OnDrawPropertiesCancelClick( wxCommandEvent& event )
{
    Show( false );
#ifdef __WXOSX__    
	EndModal(wxID_CANCEL);
#endif
    SetClientSize(m_defaultClientSize);

    event.Skip();
}

void ODPropertiesDialogImpl::OnDrawPropertiesApplyClick( wxCommandEvent& event )
{
    SaveChanges(); // write changes to globals and update config

    event.Skip();
}

void ODPropertiesDialogImpl::SaveChanges()
{
    //g_colourActiveBoundaryLineColour = PickerODPointRangeRingColours->GetColour();
    g_colourActiveBoundaryLineColour = m_colourPickerActiveBoundaryLineColour->GetColour();
    g_colourActiveBoundaryFillColour = m_colourPickerActiveBoundaryFillColour->GetColour();
    g_colourInActiveBoundaryLineColour = m_colourPickerInActiveBoundaryLineColour->GetColour();
    g_colourInActiveBoundaryFillColour = m_colourPickerInActiveBoundaryFillColour->GetColour();

    g_BoundaryLineWidth = m_choiceBoundaryLineWidth->GetSelection() + 1;
    g_BoundaryLineStyle = ::StyleValues[ m_choiceBoundaryLineStyle->GetSelection()];
    g_uiFillTransparency = m_sliderFillTransparency->GetValue();
    
    g_colourActivePathLineColour = m_colourPickerActivePathLineColour->GetColour();
    g_colourInActivePathLineColour = m_colourPickerInActivePathLineColour->GetColour();

    g_PathLineWidth = m_choicePathLineWidth->GetSelection() + 1;
    g_PathLineStyle = ::StyleValues[ m_choicePathLineStyle->GetSelection()];
    
    g_colourEBLLineColour = m_colourPickerEBLLineColour->GetColour();
    g_EBLLineWidth = m_choiceEBLLineWidth->GetSelection() + 1;
    g_EBLLineStyle = ::StyleValues[ m_choiceEBLLineStyle->GetSelection()];
    g_EBLPersistenceType = m_radioBoxEBLPersistence->GetSelection();
    g_bEBLFixedEndPosition = m_checkBoxEBLFixedEndPosition->GetValue();
    g_sEBLEndIconName = m_bcomboBoxEBLEndIconName->GetValue();

    g_iODPointRangeRingsNumber = m_choiceODPointRangeRingNumber->GetSelection();
    g_fODPointRangeRingsStep = atof( m_textCtrlODPointRangeRingSteps->GetValue().mb_str() );
    g_iODPointRangeRingsStepUnits = m_choiceODPointDistanceUnit->GetSelection();
    g_colourODPointRangeRingsColour = m_colourPickerODPointRangeRingColours->GetColour();
    m_textCtrlODPointArrivalRadius->GetValue().ToDouble( &g_n_arrival_circle_radius );
    g_bODPointShowRangeRings = m_checkBoxShowODPointRangeRings->GetValue();
    g_sODPointIconName = m_bcomboBoxODPointIconName->GetValue();
    
    g_bConfirmObjectDelete = m_checkBoxConfirmObjectDelete->GetValue();
    g_navobjbackups = m_spinCtrlNavObjBackups->GetValue();
    
    g_iTextPosition = m_choiceTextPosition->GetSelection();
    g_colourDefaultTextColour = m_colourPickerTextColour->GetColour();
    g_colourDefaultTextBackgroundColour = m_colourPickerBackgroundColour->GetColour();
    g_iTextBackgroundTransparency = m_sliderBackgroundTransparency->GetValue();
    if(m_pfdDialog) g_DisplayTextFont = m_pfdDialog->GetFontData().GetChosenFont();
    
    g_EdgePanSensitivity = m_sliderEdgePan->GetValue();
    g_InitialEdgePanSensitivity = m_sliderInitialEdgePan->GetValue();
    
    g_iDisplayToolbar = m_choiceToolbar->GetSelection();
}

void ODPropertiesDialogImpl::SetDialogSize( void )
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
    this->Layout();
    this->GetSizer()->Fit(this);
}

void ODPropertiesDialogImpl::UpdateProperties( void )
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

        m_bcomboBoxODPointIconName->Clear();
        m_bcomboBoxEBLEndIconName->Clear();
        //      Iterate on the Icon Descriptions, filling in the combo control
        if( g_pODPointMan == NULL ) g_pODPointMan = new PointMan();
        
        bool fillCombo = m_bcomboBoxODPointIconName->GetCount() == 0;
        wxImageList *icons = g_pODPointMan->Getpmarkicon_image_list();

        if( fillCombo  && icons){
            for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
                wxString *ps = g_pODPointMan->GetIconDescription( i );
                m_bcomboBoxODPointIconName->Append( *ps, icons->GetBitmap( i ) );
                m_bcomboBoxEBLEndIconName->Append( *ps, icons->GetBitmap( i ) );
            }
        }
        
        // find the correct item in the combo box
        int iconToSelect = -1;
        for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
            if( *g_pODPointMan->GetIconDescription( i ) == g_sODPointIconName ) {
                iconToSelect = i;
                break;
            }
        }
        //  not found, so add  it to the list, with a generic bitmap and using the name as description
        // n.b.  This should never happen...
        if( -1 == iconToSelect){    
            m_bcomboBoxODPointIconName->Append( g_sODPointIconName, icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxODPointIconName->GetCount() - 1;
        } 
        
        
        m_bcomboBoxODPointIconName->SetSelection( iconToSelect );
        m_bitmapPointBitmap->SetBitmap( m_bcomboBoxODPointIconName->GetItemBitmap( m_bcomboBoxODPointIconName->GetSelection() ) );
        
        m_bcomboBoxODPointIconName->SetSelection( iconToSelect );
        m_bitmapPointBitmap->SetBitmap( m_bcomboBoxODPointIconName->GetItemBitmap( m_bcomboBoxODPointIconName->GetSelection() ) );
        
        iconToSelect = -1;
        for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
            if( *g_pODPointMan->GetIconDescription( i ) == g_sEBLEndIconName ) {
                iconToSelect = i;
                break;
            }
        }
        //  not found, so add  it to the list, with a generic bitmap and using the name as description
        // n.b.  This should never happen...
        if( -1 == iconToSelect){    
            m_bcomboBoxEBLEndIconName->Append( g_sODPointIconName, icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxEBLEndIconName->GetCount() - 1;
        } 
        
        
        m_bcomboBoxEBLEndIconName->SetSelection( iconToSelect );
        m_bitmapEBLEndBitmap->SetBitmap( m_bcomboBoxEBLEndIconName->GetItemBitmap( m_bcomboBoxEBLEndIconName->GetSelection() ) );
        
        m_bcomboBoxEBLEndIconName->SetSelection( iconToSelect );
        m_bitmapEBLEndBitmap->SetBitmap( m_bcomboBoxEBLEndIconName->GetItemBitmap( m_bcomboBoxEBLEndIconName->GetSelection() ) );
        
        icons = NULL;

        m_colourPickerActiveBoundaryLineColour->SetColour( g_colourActiveBoundaryLineColour );
        m_colourPickerInActiveBoundaryLineColour->SetColour( g_colourInActiveBoundaryLineColour );
        m_colourPickerActiveBoundaryFillColour->SetColour( g_colourActiveBoundaryFillColour );
        m_colourPickerInActiveBoundaryFillColour->SetColour( g_colourInActiveBoundaryFillColour );
        
        m_colourPickerActivePathLineColour->SetColour( g_colourActivePathLineColour );
        m_colourPickerInActivePathLineColour->SetColour( g_colourInActivePathLineColour );
        
        for( unsigned int i = 0; i < sizeof( ::StyleValues ) / sizeof(int); i++ ) {
            if( g_BoundaryLineStyle == ::StyleValues[i] )
                m_choiceBoundaryLineStyle->Select( i );
            if( g_PathLineStyle == ::StyleValues[i] )
                m_choicePathLineStyle->Select( i );
            if( g_EBLLineStyle == ::StyleValues[i] )
                m_choiceEBLLineStyle->Select( i );
        }
        m_choiceBoundaryLineWidth->SetSelection( g_BoundaryLineWidth - 1 );
        m_choicePathLineWidth->SetSelection( g_PathLineWidth - 1 );
        m_sliderFillTransparency->SetValue( g_uiFillTransparency );
        
        m_colourPickerEBLLineColour->SetColour( g_colourEBLLineColour );
        m_checkBoxEBLFixedEndPosition->SetValue( g_bEBLFixedEndPosition );
        m_radioBoxEBLPersistence->SetSelection( g_EBLPersistenceType );
        m_choiceEBLLineWidth->SetSelection( g_EBLLineWidth - 1 );
        
        m_choiceTextPosition->SetSelection( g_iTextPosition );
        m_colourPickerTextColour->SetColour( g_colourDefaultTextColour );
        m_colourPickerBackgroundColour->SetColour( g_colourDefaultTextBackgroundColour );
        m_sliderBackgroundTransparency->SetValue( g_iTextBackgroundTransparency );
        m_staticTextFontFaceExample->SetFont( g_DisplayTextFont );
        
        m_checkBoxConfirmObjectDelete->SetValue( g_bConfirmObjectDelete );
        m_spinCtrlNavObjBackups->SetValue( g_navobjbackups );
        m_sliderInitialEdgePan->SetValue( g_InitialEdgePanSensitivity );
        m_sliderEdgePan->SetValue( g_EdgePanSensitivity );
        m_choiceToolbar->Select( g_iDisplayToolbar );
        

    return;
}

