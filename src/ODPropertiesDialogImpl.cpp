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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ODPropertiesDialogImpl.h"
#include "ocpn_draw_pi.h"
#include "ODicons.h"
#include "ODIconCombo.h"
#include "ODToolbarImpl.h"
#include "ODUtils.h"
#include "PointMan.h"
#include "version.h"
#include <wx/fontdlg.h>

#if wxCHECK_VERSION(3,0,0) 
#include <wx/valnum.h>
#endif

ODPropertiesDialogImpl::ODPropertiesDialogImpl( wxWindow* parent )
:
ODPropertiesDialogDef( parent )
{
    wxString l_sTitle;
    l_sTitle.Printf(wxT("%s v%d.%d.%d.%d %s"), g_ocpn_draw_display_name->ToStdString(), PLUGIN_VERSION_MAJOR, PLUGIN_VERSION_MINOR, PLUGIN_VERSION_PATCH, PLUGIN_VERSION_TWEAK, ("Preferences"));
    SetTitle(l_sTitle);
    
#if wxCHECK_VERSION(3,0,0)
    wxDialog::SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
#endif // wxCHECK_VERSION(3,0,0)
    
    m_staticTextNameVal->SetLabel( wxT("OpenCPN Draw Plugin") );
    m_staticTextMajorVal->SetLabel(wxString::Format(wxT("%i"), PLUGIN_VERSION_MAJOR ));
    m_staticTextMinorVal->SetLabel(wxString::Format(wxT("%i"), PLUGIN_VERSION_MINOR ));
    m_staticTextPatchVal->SetLabel( wxT(TOSTRING(PLUGIN_VERSION_PATCH)) );
    m_staticTextTweakVal->SetLabel( wxT(TOSTRING(PLUGIN_VERSION_TWEAK)) );
    m_staticTextDateVal->SetLabel( wxT(TOSTRING(PLUGIN_VERSION_DATE)) );
    m_staticTextAPIVersionMajorVal->SetLabel( wxT(TOSTRING(OD_API_VERSION_MAJOR)) );
    m_staticTextAPIVersionMinorVal->SetLabel( wxT(TOSTRING(OD_API_VERSION_MINOR)) );
    m_staticTextOCPNAPIVersionMajorVal->SetLabel( wxT(TOSTRING(OCPN_API_VERSION_MAJOR)) );
    m_staticTextOCPNAPIVersionMinorVal->SetLabel( wxT(TOSTRING(OCPN_API_VERSION_MINOR)) );
    m_staticTextDataDirectory->SetLabel( *g_pData );
    m_staticTextIconDirectory->SetLabel( *g_pUserIconsDir );
    m_staticTextLayersDirectory->SetLabel( *g_pLayerDir );

    m_gridODWDInteractions->SetCellValue(0, 0, _("OD\\WD"));
    m_gridODWDInteractions->SetCellValue(0, 1, _("All"));
    m_gridODWDInteractions->SetCellValue(0, 2, _("Exclusion"));
    m_gridODWDInteractions->SetCellValue(0, 3, _("Inclusion"));
    m_gridODWDInteractions->SetCellValue(0, 4, _("Neither"));
    m_gridODWDInteractions->SetCellValue(1, 0, _("Exclusion"));
    m_gridODWDInteractions->SetCellValue(2, 0, _("Inclusion"));
    m_gridODWDInteractions->SetCellValue(3, 0, _("Neither"));

    m_gridODWDInteractions->SetCellValue(1, 1, _("Ring"));
    m_gridODWDInteractions->SetCellValue(2, 1, _("Ring"));
    m_gridODWDInteractions->SetCellValue(3, 1, _("Ring"));
    m_gridODWDInteractions->SetCellValue(1, 2, _("Ring"));
    m_gridODWDInteractions->SetCellValue(2, 2, _("No alarm"));
    m_gridODWDInteractions->SetCellValue(3, 2, _("No alarm"));
    m_gridODWDInteractions->SetCellValue(1, 3, _("No alarm"));
    m_gridODWDInteractions->SetCellValue(2, 3, _("Ring"));
    m_gridODWDInteractions->SetCellValue(3, 3, _("No alarm"));
    m_gridODWDInteractions->SetCellValue(1, 4, _("No alarm"));
    m_gridODWDInteractions->SetCellValue(2, 4, _("No alarm"));
    m_gridODWDInteractions->SetCellValue(3, 4, _("Ring"));
    
    SetTableCellBackgroundColours();
    
    m_pfdDialog = NULL;
    
#if wxCHECK_VERSION(3,0,0)
    wxFloatingPointValidator<double> dODPointRangeRingStepVal(3, &m_dODPointRangeRingStepValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dODPointArrivalRadiusVal(3, &m_dODPointArrivalRadiusValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dDRPathLengthVal(3, &m_dDRPathLengthValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dDRPointIntervalVal(3, &m_dODPointIntervalValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dDRPointRangeRingStepVal(3, &m_dDRPointRangRingStepValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dSOGVal(3, &m_dSOGValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dPILOffset(3, &m_dPILOffsetValidator, wxNUM_VAL_DEFAULT);
    wxIntegerValidator<int> iCOGVal(&m_iCOGValidator, wxNUM_VAL_DEFAULT);
    wxIntegerValidator<int> iTextPointTextMaxWidth( &m_iTextPointTextMaxWidth, wxNUM_VAL_THOUSANDS_SEPARATOR);
    
    dODPointRangeRingStepVal.SetMin(0);
    dODPointArrivalRadiusVal.SetMin(0);
    dDRPathLengthVal.SetMin(0);
    dDRPointIntervalVal.SetMin(0);
    dDRPointRangeRingStepVal.SetMin(0);
    dSOGVal.SetMin(0);
    iCOGVal.SetRange(0, 360);
    iTextPointTextMaxWidth.SetMin(-1);

    m_textCtrlODPointRangeRingSteps->SetValidator( dODPointRangeRingStepVal );
    m_textCtrlODPointArrivalRadius->SetValidator( dODPointArrivalRadiusVal );
    m_textCtrlSOG->SetValidator( dSOGVal );
    m_textCtrlCOG->SetValidator( iCOGVal );
    m_textCtrlDRPathLength->SetValidator( dDRPathLengthVal );
    m_textCtrlDRPointInterval->SetValidator( dDRPointIntervalVal );
    m_textCtrlDRPointRangeRingSteps->SetValidator( dDRPointRangeRingStepVal );
    m_textCtrlPILDefaultOffset->SetValidator( dPILOffset );
    m_textCtrlTextMaxWidth->SetValidator( iTextPointTextMaxWidth );
    
    int l_iPage = m_notebookProperties->FindPage(m_panelPath);
    m_notebookProperties->DeletePage(l_iPage);
#else
    for(size_t i = 0; i < m_notebookProperties->GetPageCount(); i++) {
        if(m_notebookProperties->GetPageText(i) == _("Path")) {
            m_notebookProperties->DeletePage(i);
            break;
        }
    }
#endif

    // Boundary point Icon
    m_bODIComboBoxODPointIconName = new ODIconCombo( m_panelBoundaryPoint, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
    m_bODIComboBoxODPointIconName->SetPopupMaxHeight(::wxGetDisplaySize().y / 2);

    //  Accomodate scaling of icon
    int min_size = GetCharHeight() * 2;
    //min_size = wxMax( min_size, (32 *g_ChartScaleFactorExp) + 4 );
    m_bODIComboBoxODPointIconName->SetMinSize( wxSize(-1, min_size) );
    //m_SizerNameIcon->Add(m_bODIComboBoxODPointIconName, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
    m_fgSizerODPointIcon->Replace(m_bcomboBoxODPointIconName, m_bODIComboBoxODPointIconName);
    
    // Textpoint Icon
    m_bODIComboBoxTextPointIconName = new ODIconCombo( m_panelTextPoint, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
    m_bODIComboBoxTextPointIconName->SetPopupMaxHeight(::wxGetDisplaySize().y / 2);
    
    //  Accomodate scaling of icon
    //min_size = wxMax( min_size, (32 *g_ChartScaleFactorExp) + 4 );
    m_bODIComboBoxTextPointIconName->SetMinSize( wxSize(-1, min_size) );
    m_fgSizerTextPointIconName->Replace(m_bcomboBoxTextPointIconName, m_bODIComboBoxTextPointIconName);

    // EBL Start point Icon
    m_bODIComboBoxEBLStartIconName = new ODIconCombo( m_panelEBL, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
    m_bODIComboBoxEBLStartIconName->SetPopupMaxHeight(::wxGetDisplaySize().y / 2);
    
    //  Accomodate scaling of icon
    //min_size = wxMax( min_size, (32 *g_ChartScaleFactorExp) + 4 );
    m_bODIComboBoxEBLStartIconName->SetMinSize( wxSize(-1, min_size) );
    m_fgSizerEBLStartIconName->Replace(m_bcomboBoxEBLStartIconName, m_bODIComboBoxEBLStartIconName);
    
    // EBL End point Icon
    m_bODIComboBoxEBLEndIconName = new ODIconCombo( m_panelEBL, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
    m_bODIComboBoxEBLEndIconName->SetPopupMaxHeight(::wxGetDisplaySize().y / 2);
    
    //  Accomodate scaling of icon
    //min_size = wxMax( min_size, (32 *g_ChartScaleFactorExp) + 4 );
    m_bODIComboBoxEBLEndIconName->SetMinSize( wxSize(-1, min_size) );
    m_fgSizerEBLEndPointIcon->Replace(m_bcomboBoxEBLEndIconName, m_bODIComboBoxEBLEndIconName);
    
    // DR point Icon
    m_bODIComboBoxDRPointIconName = new ODIconCombo( m_panelDRPoint, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
    m_bODIComboBoxDRPointIconName->SetPopupMaxHeight(::wxGetDisplaySize().y / 2);
    
    //  Accomodate scaling of icon
    //min_size = wxMax( min_size, (32 *g_ChartScaleFactorExp) + 4 );
    m_bODIComboBoxDRPointIconName->SetMinSize( wxSize(-1, min_size) );
    m_fgSizerDREndPointIcon->Replace(m_bcomboBoxDRPointIconName, m_bODIComboBoxDRPointIconName);
    
    // GZ First point Icon
    m_bODIComboBoxGZFirstIconName = new ODIconCombo( m_panelGZ, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
    m_bODIComboBoxGZFirstIconName->SetPopupMaxHeight(::wxGetDisplaySize().y / 2);
    
    //  Accomodate scaling of icon
    //min_size = wxMax( min_size, (32 *g_ChartScaleFactorExp) + 4 );
    m_bODIComboBoxGZFirstIconName->SetMinSize( wxSize(-1, min_size) );
    m_fgSizerGZFASIcon->Replace(m_bcomboBoxGZFirstIconName, m_bODIComboBoxGZFirstIconName);
    
    // GZ End point Icon
    m_bODIComboBoxGZSecondIconName = new ODIconCombo( m_panelGZ, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
    m_bODIComboBoxGZSecondIconName->SetPopupMaxHeight(::wxGetDisplaySize().y / 2);
    
    //  Accomodate scaling of icon
    //min_size = wxMax( min_size, (32 *g_ChartScaleFactorExp) + 4 );
    m_bODIComboBoxGZSecondIconName->SetMinSize( wxSize(-1, min_size) );
    m_fgSizerGZFASIcon->Replace(m_bcomboBoxGZSecondIconName, m_bODIComboBoxGZSecondIconName);
    
    // PIL Start point Icon
    m_bODIComboBoxPILStartIconName = new ODIconCombo( m_panelPIL, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
    m_bODIComboBoxPILStartIconName->SetPopupMaxHeight(::wxGetDisplaySize().y / 2);

    //  Accomodate scaling of icon
    //min_size = wxMax( min_size, (32 *g_ChartScaleFactorExp) + 4 );
    m_bODIComboBoxPILStartIconName->SetMinSize( wxSize(-1, min_size) );
    m_fgSizerPILStartIconName->Replace(m_bcomboBoxPILStartIconName, m_bODIComboBoxPILStartIconName);

    // PIL End point Icon
    m_bODIComboBoxPILEndIconName = new ODIconCombo( m_panelPIL, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
    m_bODIComboBoxPILEndIconName->SetPopupMaxHeight(::wxGetDisplaySize().y / 2);

    //  Accomodate scaling of icon
    //min_size = wxMax( min_size, (32 *g_ChartScaleFactorExp) + 4 );
    m_bODIComboBoxPILEndIconName->SetMinSize( wxSize(-1, min_size) );
    m_fgSizerPILEndPointIcon->Replace(m_bcomboBoxPILEndIconName, m_bODIComboBoxPILEndIconName);

    delete m_bcomboBoxODPointIconName;
    delete m_bcomboBoxTextPointIconName;    
    delete m_bcomboBoxEBLStartIconName;    
    delete m_bcomboBoxEBLEndIconName;    
    delete m_bcomboBoxDRPointIconName;    
    delete m_bcomboBoxGZFirstIconName;    
    delete m_bcomboBoxGZSecondIconName;    
    delete m_bcomboBoxPILStartIconName;
    delete m_bcomboBoxPILEndIconName;

    wxTextFile license_filea( g_SData_Locn->c_str() + _T("license.txt") );
    wxString l_LicenseText = wxEmptyString;
    if ( license_filea.Open() ) {
        for ( wxString str = license_filea.GetFirstLine(); !license_filea.Eof() ; str = license_filea.GetNextLine() )
            l_LicenseText.Append( str + _T("\n") );
        license_filea.Close();
        m_textCtrlLicense->SetDefaultStyle(wxTextAttr(*wxBLACK));
        m_textCtrlLicense->Clear();
        m_textCtrlLicense->SetValue(l_LicenseText);
    } else {
        wxLogMessage( _T("Could not open License file: ") + g_SData_Locn->c_str() );
    }
    
    SetDialogSize();
}

void ODPropertiesDialogImpl::OnODPointComboboxSelected( wxCommandEvent& event )
{
}

void ODPropertiesDialogImpl::OnTextPointIconComboboxSelected( wxCommandEvent& event )
{
}

void ODPropertiesDialogImpl::OnEBLEndIconComboboxSelected( wxCommandEvent& event )
{
}

void ODPropertiesDialogImpl::OnEBLStartIconComboboxSelected( wxCommandEvent& event )
{
}

void ODPropertiesDialogImpl::OnDRPointIconComboboxSelected( wxCommandEvent& event )
{
}

void ODPropertiesDialogImpl::OnGZFirstIconComboboxSelected( wxCommandEvent& event )
{
}

void ODPropertiesDialogImpl::OnGZSecondIconComboboxSelected( wxCommandEvent& event )
{
}

void ODPropertiesDialogImpl::OnButtonClickFonts( wxCommandEvent& event )
{
    if(m_pfdDialog) delete m_pfdDialog;
    
    wxFontData l_FontData;
    l_FontData.SetInitialFont( g_DisplayTextFont );
    m_pfdDialog = new wxFontDialog( this, l_FontData );
    m_pfdDialog->Centre( wxBOTH );
    
    DimeWindow(m_pfdDialog);
    int iRet = m_pfdDialog->ShowModal();
    if(iRet == wxID_OK) {
        //wxFontData wsfdData = m_pfdDialog->GetFontData();
        m_staticTextFontFaceExample->SetFont(m_pfdDialog->GetFontData().GetChosenFont());
        m_fgSizerTextPointFont->RecalcSizes();
        m_panelTextPoint->Layout();
        SendSizeEvent();
#if wxCHECK_VERSION(3,0,0) 
        m_notebookProperties->SetSelection(m_notebookProperties->FindPage(m_panelTextPoint));
#else
        m_notebookProperties->SetSelection(3);
#endif
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

    ResetGlobalLocale();
    event.Skip();
}

void ODPropertiesDialogImpl::OnDrawPropertiesCancelClick( wxCommandEvent& event )
{
    Show( false );
#ifdef __WXOSX__    
	EndModal(wxID_CANCEL);
#endif
    SetClientSize(m_defaultClientSize);

    ResetGlobalLocale();
    event.Skip();
}

void ODPropertiesDialogImpl::OnDrawPropertiesApplyClick( wxCommandEvent& event )
{
    SaveChanges(); // write changes to globals and update config

    event.Skip();
}

void ODPropertiesDialogImpl::OnEBLRotateWithBoat( wxCommandEvent& event )
{
    if(m_checkBoxRotateWithBoat->IsChecked())
        m_checkBoxEBLFixedEndPosition->SetValue(false);
}

void ODPropertiesDialogImpl::OnEBLFixedEndPosition( wxCommandEvent& event )
{
    if(m_checkBoxEBLFixedEndPosition->IsChecked())
        m_checkBoxRotateWithBoat->SetValue(false);
}

void ODPropertiesDialogImpl::OnPILIndexLineChoice( wxCommandEvent& event )
{
    if(m_choiceNumIndexLines->GetSelection() == 0) 
        SetIndexLineChoice( false );
    else 
        SetIndexLineChoice( true );
}

void ODPropertiesDialogImpl::SetIndexLineChoice( bool choice )
{
    m_staticTextPILActiveOffsetLine2Colour->Enable(choice);
    m_colourPickerPILActiveOffsetLine2Colour->Enable(choice);
    m_staticTextPILInactiveOffsetLine2Colour->Enable(choice);
    m_colourPickerPILInActiveCentreLineColour->Enable(choice);
    m_staticTextPILOffsetLine2Style->Enable(choice);
    m_choicePILOffsetLine2Style->Enable(choice);
    m_staticTextPILOffsetLine2Width->Enable(choice);
    m_choicePILOffsetLine2Width->Enable(choice);
}

void ODPropertiesDialogImpl::SaveChanges()
{
    TransferDataFromWindow();
    g_bBoundaryPointShowName = m_checkBoxBoundaryPointShowName->GetValue();
    g_bTextPointShowName = m_checkBoxTextPointShowName->GetValue();
    //g_colourActiveBoundaryLineColour = PickerODPointRangeRingColours->GetColour();
    g_colourActiveBoundaryLineColour = m_colourPickerActiveBoundaryLineColour->GetColour();
    g_colourActiveBoundaryFillColour = m_colourPickerActiveBoundaryFillColour->GetColour();
    g_colourInActiveBoundaryLineColour = m_colourPickerInActiveBoundaryLineColour->GetColour();
    g_colourInActiveBoundaryFillColour = m_colourPickerInActiveBoundaryFillColour->GetColour();

    g_BoundaryLineWidth = ::WidthValues[ m_choiceBoundaryLineWidth->GetSelection() ];
    g_BoundaryLineStyle = (wxPenStyle)::StyleValues[ m_choiceBoundaryLineStyle->GetSelection() ];
    g_uiFillTransparency = m_sliderFillTransparency->GetValue();
    g_uiBoundaryPointFillTransparency = m_sliderBoundaryPointFillTransparency->GetValue();
    g_iBoundaryPointRangeRingLineWidth = ::WidthValues[ m_choiceRangeRingWidth->GetSelection() ];
    g_iBoundaryPointRangeRingLineStyle = (wxPenStyle)::StyleValues[ m_choiceRangeRingStyle->GetSelection() ];
    g_iInclusionBoundarySize = m_sliderInclusionBoundarySize->GetValue();
    g_iInclusionBoundaryPointSize = m_sliderInclusionBoundaryPointSize->GetValue();
    g_bBoundaryODPointsVisible = m_checkBoxBoundaryODPointsVisible->GetValue();
    
    int l_BoundaryType;
    l_BoundaryType = m_radioBoxBoundaryType->GetSelection();
    switch (l_BoundaryType) {
        case ID_BOUNDARY_EXCLUSION:
            g_bExclusionBoundary = true;
            g_bInclusionBoundary = false;
            break;
        case ID_BOUNDARY_INCLUSION:
            g_bExclusionBoundary = false;
            g_bInclusionBoundary = true;
            break;
        case ID_BOUNDARY_NEITHER:
            g_bExclusionBoundary = false;
            g_bInclusionBoundary = false;
            break;
        default:
            g_bExclusionBoundary = true;
            g_bInclusionBoundary = false;
            break;
    }
    
    int l_BoundaryPointType;
    l_BoundaryPointType = m_radioBoxBoundaryPointType->GetSelection();
    switch (l_BoundaryPointType) {
        case ID_BOUNDARY_EXCLUSION:
            g_bExclusionBoundaryPoint = true;
            g_bInclusionBoundaryPoint = false;
            break;
        case ID_BOUNDARY_INCLUSION:
            g_bExclusionBoundaryPoint = false;
            g_bInclusionBoundaryPoint = true;
            break;
        case ID_BOUNDARY_NEITHER:
            g_bExclusionBoundaryPoint = false;
            g_bInclusionBoundaryPoint = false;
            break;
        default:
            g_bExclusionBoundaryPoint = true;
            g_bInclusionBoundaryPoint = false;
            break;
    }
    
//    g_colourActivePathLineColour = m_colourPickerActivePathLineColour->GetColour();
//    g_colourInActivePathLineColour = m_colourPickerInActivePathLineColour->GetColour();

//    g_PathLineWidth = ::WidthValues[ m_choicePathLineWidth->GetSelection() ];
//    g_PathLineStyle = ::StyleValues[ m_choicePathLineStyle->GetSelection()];
    
    g_colourEBLLineColour = m_colourPickerEBLLineColour->GetColour();
    g_EBLLineWidth = ::WidthValues[ m_choiceEBLLineWidth->GetSelection() ];
    g_EBLLineStyle = (wxPenStyle)::StyleValues[ m_choiceEBLLineStyle->GetSelection() ];
    g_bEBLRotateWithBoat = m_checkBoxRotateWithBoat->GetValue();
    g_iEBLMaintainWith = m_radioBoxMaintainWith->GetSelection();
    g_bEBLShowArrow = m_checkBoxEBLShowArrow->GetValue();
    g_bEBLVRM = m_checkBoxShowVRM->GetValue();
    g_bEBLAlwaysShowInfo = m_checkBoxEBLAllwaysShowInfo->GetValue();
    g_bEBLPerpLine = m_checkBoxShowPerpLine->GetValue();
    g_iEBLPersistenceType = m_radioBoxEBLPersistence->GetSelection();
    g_bEBLFixedEndPosition = m_checkBoxEBLFixedEndPosition->GetValue();
    g_sEBLEndIconName = m_bODIComboBoxEBLEndIconName->GetValue();
    g_sEBLStartIconName = m_bODIComboBoxEBLStartIconName->GetValue();;
    
    g_colourPILActiveCentreLineColour = m_colourPickerPILActiveCentreLineColour->GetColour();
    g_colourPILInActiveCentreLineColour = m_colourPickerPILInActiveCentreLineColour->GetColour();
    g_colourPILActiveOffsetLine1Colour = m_colourPickerPILActiveOffsetLine1Colour->GetColour();
    g_colourPILInActiveOffsetLine1Colour = m_colourPickerPILInActiveOffsetLine1Colour->GetColour();
    g_colourPILActiveOffsetLine2Colour = m_colourPickerPILActiveOffsetLine2Colour->GetColour();
    g_colourPILInActiveOffsetLine2Colour = m_colourPickerPILInactiveOffsetLine2Colour->GetColour();
    g_PILCentreLineWidth = ::WidthValues[ m_choicePILCentreLineWidth->GetSelection() ];
    g_PILCentreLineStyle = (wxPenStyle)::StyleValues[ m_choicePILCentreLineStyle->GetSelection() ];
    g_PILOffsetLine1Width = ::WidthValues[ m_choicePILOffsetLine1Width->GetSelection() ];
    g_PILOffsetLine1Style = (wxPenStyle)::StyleValues[ m_choicePILOffsetLine1Style->GetSelection() ];
    g_PILOffsetLine2Width = ::WidthValues[ m_choicePILOffsetLine2Width->GetSelection() ];
    g_PILOffsetLine2Style = (wxPenStyle)::StyleValues[ m_choicePILOffsetLine2Style->GetSelection() ];
    g_PILDefaultNumIndexLines = m_choiceNumIndexLines->GetSelection();
    g_iPILPersistenceType = m_radioBoxPILPersistence->GetSelection();
    g_bEBLFixedEndPosition = m_checkBoxEBLFixedEndPosition->GetValue();
    g_sPILEndIconName = m_bODIComboBoxPILEndIconName->GetValue();
    g_sPILStartIconName = m_bODIComboBoxPILStartIconName->GetValue();
    g_dPILOffset = m_dPILOffsetValidator;

    g_sDRPointIconName = m_bODIComboBoxDRPointIconName->GetValue();
    g_colourDRLineColour = m_colourPickerDRLineColour->GetColour();
    g_colourInActiveDRLineColour = m_colourPickerInActiveDRLineColour->GetColour();
    g_DRLineWidth = ::WidthValues[ m_choiceDRLineWidth->GetSelection() ];
    g_DRLineStyle = (wxPenStyle)::StyleValues[ m_choiceDRLineStyle->GetSelection()];
    g_bDRShowArrow = m_checkBoxDRShowArrow->GetValue();
    g_dDRSOG = m_dSOGValidator;
    g_iDRCOG = m_iCOGValidator;
    g_iDRLengthType = m_radioBoxDRLengthType->GetSelection();
    g_iDRIntervalType = m_radioBoxDRIntervalType->GetSelection();
    g_iDRDistanceUnits = m_radioBoxDRDistanceUnits->GetSelection();
    g_iDRTimeUnits = m_radioBoxDRTimeUnits->GetSelection();
    g_iDRPersistenceType = m_radioBoxDRPersistence->GetSelection();
    g_dDRLength = m_dDRPathLengthValidator;
    g_dDRPointInterval = m_dODPointIntervalValidator;
    g_bDRPointShowRangeRings = m_checkBoxShowDRPointRangeRings->GetValue();
    g_iDRPointRangeRingsNumber = m_choiceDRPointRangeRingNumber->GetSelection();
    g_fDRPointRangeRingsStep = m_dDRPointRangRingStepValidator;
    g_iDRPointRangeRingsStepUnits = m_choiceDRPointDistanceUnit->GetSelection();
    g_colourDRPointRangeRingsColour = m_colourPickerDRPointRangeRingColours->GetColour();
    g_iDRPointRangeRingLineWidth = ::WidthValues[ m_choiceDRPointRangeRingWidth->GetSelection() ];
    g_iDRPointRangeRingLineStyle = (wxPenStyle)::StyleValues[ m_choiceDRPointRangeRingStyle->GetSelection() ];
    
    g_colourActiveGZLineColour = m_colourPickerActiveGZLineColour->GetColour();
    g_colourActiveGZFillColour = m_colourPickerActiveGZFillColour->GetColour();
    g_colourInActiveGZLineColour = m_colourPickerInActiveGZLineColour->GetColour();
    g_colourInActiveGZFillColour = m_colourPickerInActiveGZFillColour->GetColour();
    g_GZLineWidth = ::WidthValues[ m_choiceGZLineWidth->GetSelection() ];
    g_GZLineStyle = (wxPenStyle)::StyleValues[ m_choiceGZLineStyle->GetSelection() ];
    g_uiGZFillTransparency = m_sliderGZFillTransparency->GetValue();
    g_sGZFirstIconName = m_bODIComboBoxGZFirstIconName->GetValue();
    g_sGZSecondIconName = m_bODIComboBoxGZSecondIconName->GetValue();;
    g_bGZRotateWithBoat = m_checkBoxGZRotateWithBoat->GetValue();
    g_iGZMaintainWith = m_radioBoxGZMaintainWith->GetSelection();
    g_iGZLineType = m_radioBoxGZLineType->GetSelection();
    g_iGZPersistenceType = m_radioBoxGZPersistence->GetSelection();
    
    
    g_iODPointRangeRingsNumber = m_choiceODPointRangeRingNumber->GetSelection();
    g_fODPointRangeRingsStep = m_dODPointRangeRingStepValidator;
    g_iODPointRangeRingsStepUnits = m_choiceODPointDistanceUnit->GetSelection();
    g_colourODPointRangeRingsColour = m_colourPickerODPointRangeRingColours->GetColour();
    g_dODPointArrivalCircleRadius = m_dODPointArrivalRadiusValidator;
    g_bODPointShowRangeRings = m_checkBoxShowODPointRangeRings->GetValue();
    g_sODPointIconName = m_bODIComboBoxODPointIconName->GetValue();
    
    g_bConfirmObjectDelete = m_checkBoxConfirmObjectDelete->GetValue();
    g_bRememberPropertyDialogPosition = m_checkBoxRememberPropertyPosition->GetValue();
    g_bShowMag = m_checkBoxShowMagBearings->GetValue();
    g_bAllowLeftDrag = m_checkBoxAllowLeftDrag->GetValue();
    g_bShowLayers = m_checkBoxShowLayers->GetValue();
    g_navobjbackups = m_spinCtrlNavObjBackups->GetValue();
    
    g_sTextPointIconName = m_bODIComboBoxTextPointIconName->GetValue();
    g_iTextPosition = m_choiceTextPosition->GetSelection();
    g_iTextMaxWidth = m_iTextPointTextMaxWidth;
    g_iTextMaxWidthType = m_radioBoxWidthType->GetSelection();
    g_colourDefaultTextColour = m_colourPickerTextColour->GetColour();
    g_colourDefaultTextBackgroundColour = m_colourPickerBackgroundColour->GetColour();
    g_iTextBackgroundTransparency = m_sliderBackgroundTransparency->GetValue();
    if(m_pfdDialog) g_DisplayTextFont = m_pfdDialog->GetFontData().GetChosenFont();
    g_iTextPointDisplayTextWhen  = m_radioBoxShowDisplayText->GetSelection();
    
    g_EdgePanSensitivity = m_sliderEdgePan->GetValue();
    g_InitialEdgePanSensitivity = m_sliderInitialEdgePan->GetValue();
    
    g_iDisplayToolbar = m_choiceToolbar->GetSelection();
    switch (g_iDisplayToolbar) {
        case ID_DISPLAY_NEVER:
            g_pODToolbar->Hide();
            break;
        case ID_DISPLAY_ALWAYS:
            g_pODToolbar->Show();
            break;
    }
}

void ODPropertiesDialogImpl::SetDialogSize( void )
{
#if wxCHECK_VERSION(3,0,0) 
    m_notebookProperties->SetSelection(m_notebookProperties->FindPage(m_panelGeneral));
#else
    m_notebookProperties->SetSelection(0);
#endif
    
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
    m_notebookProperties->Layout();
    this->GetSizer()->Fit(this);
    
}

void ODPropertiesDialogImpl::UpdateProperties( void )
{
    SetGlobalLocale();
    SetTableCellBackgroundColours();
    m_dODPointArrivalRadiusValidator = g_dODPointArrivalCircleRadius;
    
    m_checkBoxBoundaryPointShowName->SetValue( g_bBoundaryPointShowName );
    m_checkBoxTextPointShowName->SetValue( g_bTextPointShowName );
    m_checkBoxShowODPointRangeRings->SetValue( g_bODPointShowRangeRings );
    m_choiceODPointRangeRingNumber->SetSelection( g_iODPointRangeRingsNumber );
    m_dODPointRangeRingStepValidator = g_fODPointRangeRingsStep;
    //m_textCtrlODPointRangeRingSteps->SetValue( wxString::Format( wxT("%.3f"), g_fODPointRangeRingsStep ) );
    m_choiceODPointDistanceUnit->SetSelection( g_iODPointRangeRingsStepUnits );
    m_colourPickerODPointRangeRingColours->SetColour( g_colourODPointRangeRingsColour );
    
    m_sliderBoundaryPointFillTransparency->SetValue( g_uiBoundaryPointFillTransparency );
    m_sliderInclusionBoundaryPointSize->SetValue( g_iInclusionBoundaryPointSize );
    
    if( g_bExclusionBoundaryPoint && !g_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_EXCLUSION );
    else if( !g_bExclusionBoundaryPoint && g_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_INCLUSION );
    else if( !g_bExclusionBoundaryPoint && !g_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_NEITHER );
    else m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_EXCLUSION );

    m_bODIComboBoxODPointIconName->Clear();
    m_bODIComboBoxTextPointIconName->Clear();
    m_bODIComboBoxEBLEndIconName->Clear();
    m_bODIComboBoxEBLStartIconName->Clear();
    m_bODIComboBoxDRPointIconName->Clear();
    m_bODIComboBoxPILEndIconName->Clear();
    m_bODIComboBoxPILStartIconName->Clear();
    //      Iterate on the Icon Descriptions, filling in the combo control
    if( g_pODPointMan == NULL ) g_pODPointMan = new PointMan();
    
    bool fillCombo = m_bODIComboBoxODPointIconName->GetCount() == 0;
    wxImageList *icons = g_pODPointMan->Getpmarkicon_image_list();

    if( fillCombo  && icons){
        for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
            wxString *ps = g_pODPointMan->GetIconDescription( i );
            m_bODIComboBoxODPointIconName->Append( *ps, icons->GetBitmap( i ) );
            m_bODIComboBoxTextPointIconName->Append( *ps, icons->GetBitmap( i ) );
            m_bODIComboBoxEBLStartIconName->Append( *ps, icons->GetBitmap( i ) );
            m_bODIComboBoxEBLEndIconName->Append( *ps, icons->GetBitmap( i ) );
            m_bODIComboBoxDRPointIconName->Append( *ps, icons->GetBitmap( i ) );
            m_bODIComboBoxGZFirstIconName->Append( *ps, icons->GetBitmap( i ) );
            m_bODIComboBoxGZSecondIconName->Append( *ps, icons->GetBitmap( i ) );
            m_bODIComboBoxPILStartIconName->Append( *ps, icons->GetBitmap( i ) );
            m_bODIComboBoxPILEndIconName->Append( *ps, icons->GetBitmap( i ) );
        }
    }
    
    // find the correct item in the ODPoint Icon combo box
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
        m_bODIComboBoxODPointIconName->Append( g_sODPointIconName, icons->GetBitmap( 0 ) );
        iconToSelect = m_bODIComboBoxODPointIconName->GetCount() - 1;
    } 
    
    m_bODIComboBoxODPointIconName->SetSelection( iconToSelect );
    
    // find the correct item in the Text Point Icon combo box
    iconToSelect = -1;
    for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
        if( *g_pODPointMan->GetIconDescription( i ) == g_sTextPointIconName ) {
            iconToSelect = i;
            break;
        }
    }
    //  not found, so add  it to the list, with a generic bitmap and using the name as description
    // n.b.  This should never happen...
    if( -1 == iconToSelect){    
        m_bODIComboBoxTextPointIconName->Append( g_sTextPointIconName, icons->GetBitmap( 0 ) );
        iconToSelect = m_bODIComboBoxTextPointIconName->GetCount() - 1;
    } 
    
    m_bODIComboBoxTextPointIconName->SetSelection( iconToSelect );
    
    // find the correct item in the EBL End Point Icon combo box
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
        m_bODIComboBoxEBLEndIconName->Append( g_sEBLEndIconName, icons->GetBitmap( 0 ) );
        iconToSelect = m_bODIComboBoxEBLEndIconName->GetCount() - 1;
    } 
    
    m_bODIComboBoxEBLEndIconName->SetSelection( iconToSelect );
    
    // find the correct item in the EBL Start Point Icon combo box
    iconToSelect = -1;
    for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
        if( *g_pODPointMan->GetIconDescription( i ) == g_sEBLStartIconName ) {
            iconToSelect = i;
            break;
        }
    }
    //  not found, so add  it to the list, with a generic bitmap and using the name as description
    // n.b.  This should never happen...
    if( -1 == iconToSelect){    
        m_bODIComboBoxEBLStartIconName->Append( g_sEBLStartIconName, icons->GetBitmap( 0 ) );
        iconToSelect = m_bODIComboBoxEBLStartIconName->GetCount() - 1;
    } 
    
    m_bODIComboBoxEBLStartIconName->SetSelection( iconToSelect );
    
    // find the correct item in the DR Point Icon combo box
    iconToSelect = -1;
    for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
        if( *g_pODPointMan->GetIconDescription( i ) == g_sDRPointIconName ) {
            iconToSelect = i;
            break;
        }
    }
    //  not found, so add  it to the list, with a generic bitmap and using the name as description
    // n.b.  This should never happen...
    if( -1 == iconToSelect){    
        m_bODIComboBoxDRPointIconName->Append( g_sDRPointIconName, icons->GetBitmap( 0 ) );
        iconToSelect = m_bODIComboBoxDRPointIconName->GetCount() - 1;
    } 
    
    m_bODIComboBoxDRPointIconName->SetSelection( iconToSelect );
    
    // find the correct item in the First GZ Point Icon combo box
    iconToSelect = -1;
    for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
        if( *g_pODPointMan->GetIconDescription( i ) == g_sGZFirstIconName ) {
            iconToSelect = i;
            break;
        }
    }
    //  not found, so add  it to the list, with a generic bitmap and using the name as description
    // n.b.  This should never happen...
    if( -1 == iconToSelect){    
        m_bODIComboBoxGZFirstIconName->Append( g_sGZFirstIconName, icons->GetBitmap( 0 ) );
        iconToSelect = m_bODIComboBoxGZFirstIconName->GetCount() - 1;
    } 
    
    m_bODIComboBoxGZFirstIconName->SetSelection( iconToSelect );
    
    // find the correct item in the Second GZ Point Icon combo box
    iconToSelect = -1;
    for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
        if( *g_pODPointMan->GetIconDescription( i ) == g_sGZSecondIconName ) {
            iconToSelect = i;
            break;
        }
    }
    //  not found, so add  it to the list, with a generic bitmap and using the name as description
    // n.b.  This should never happen...
    if( -1 == iconToSelect){    
        m_bODIComboBoxGZSecondIconName->Append( g_sGZSecondIconName, icons->GetBitmap( 0 ) );
        iconToSelect = m_bODIComboBoxGZSecondIconName->GetCount() - 1;
    } 
    
    m_bODIComboBoxGZSecondIconName->SetSelection( iconToSelect );
    
    // find the correct item in the PIL End Point Icon combo box
    iconToSelect = -1;
    for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
        if( *g_pODPointMan->GetIconDescription( i ) == g_sPILEndIconName ) {
            iconToSelect = i;
            break;
        }
    }
    //  not found, so add  it to the list, with a generic bitmap and using the name as description
    // n.b.  This should never happen...
    if( -1 == iconToSelect){
        m_bODIComboBoxPILEndIconName->Append( g_sPILEndIconName, icons->GetBitmap( 0 ) );
        iconToSelect = m_bODIComboBoxPILEndIconName->GetCount() - 1;
    }

    m_bODIComboBoxPILEndIconName->SetSelection( iconToSelect );

    // find the correct item in the PIL Start Point Icon combo box
    iconToSelect = -1;
    for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
        if( *g_pODPointMan->GetIconDescription( i ) == g_sPILStartIconName ) {
            iconToSelect = i;
            break;
        }
    }
    //  not found, so add  it to the list, with a generic bitmap and using the name as description
    // n.b.  This should never happen...
    if( -1 == iconToSelect){
        m_bODIComboBoxPILStartIconName->Append( g_sPILStartIconName, icons->GetBitmap( 0 ) );
        iconToSelect = m_bODIComboBoxPILStartIconName->GetCount() - 1;
    }

    m_bODIComboBoxPILStartIconName->SetSelection( iconToSelect );

    icons = NULL;

    m_colourPickerActiveBoundaryLineColour->SetColour( g_colourActiveBoundaryLineColour );
    m_colourPickerInActiveBoundaryLineColour->SetColour( g_colourInActiveBoundaryLineColour );
    m_colourPickerActiveBoundaryFillColour->SetColour( g_colourActiveBoundaryFillColour );
    m_colourPickerInActiveBoundaryFillColour->SetColour( g_colourInActiveBoundaryFillColour );
    
//        m_colourPickerActivePathLineColour->SetColour( g_colourActivePathLineColour );
//        m_colourPickerInActivePathLineColour->SetColour( g_colourInActivePathLineColour );
    
    for( unsigned int i = 0; i < sizeof( ::StyleValues ) / sizeof(int); i++ ) {
        if( g_BoundaryLineStyle == ::StyleValues[i] )
            m_choiceBoundaryLineStyle->SetSelection( i );
//            if( g_PathLineStyle == ::StyleValues[i] )
//                m_choicePathLineStyle->SetSelection( i );
        if( g_EBLLineStyle == ::StyleValues[i] )
            m_choiceEBLLineStyle->SetSelection( i );
        if( g_DRLineStyle == ::StyleValues[i] )
            m_choiceDRLineStyle->SetSelection( i );
        if( g_GZLineStyle == ::StyleValues[i] )
            m_choiceGZLineStyle->SetSelection( i );
        if( g_iDRPointRangeRingLineStyle == ::StyleValues[i] )
            m_choiceDRPointRangeRingStyle->SetSelection( i );
        if( g_iBoundaryPointRangeRingLineStyle == ::StyleValues[i] )
            m_choiceRangeRingStyle->SetSelection( i );
        if( g_PILCentreLineStyle == ::StyleValues[i] )
            m_choicePILCentreLineStyle->SetSelection( i );
        if( g_PILOffsetLine1Style == ::StyleValues[i] )
            m_choicePILOffsetLine1Style->SetSelection( i );
        if( g_PILOffsetLine2Style == ::StyleValues[i] )
            m_choicePILOffsetLine2Style->SetSelection( i );
    }
    for( unsigned int i = 0; i < sizeof( ::WidthValues ) / sizeof(int); i++ ) {
        if( g_BoundaryLineWidth == ::WidthValues[i] )
            m_choiceBoundaryLineWidth->SetSelection( i );
//            if( g_PathLineWidth == ::WidthValues[i] )
//                m_choicePathLineWidth->SetSelection( i );
        if( g_EBLLineWidth == ::WidthValues[i] )
            m_choiceEBLLineWidth->SetSelection( i );
        if( g_iBoundaryPointRangeRingLineWidth == ::WidthValues[i] )
            m_choiceRangeRingWidth->SetSelection( i );
        if( g_iDRPointRangeRingLineWidth == ::WidthValues[i] )
            m_choiceDRPointRangeRingWidth->SetSelection( i );
        if( g_DRLineWidth == ::WidthValues[i] )
            m_choiceDRLineWidth->SetSelection( i );
        if( g_GZLineWidth == ::WidthValues[i] )
            m_choiceGZLineWidth->SetSelection( i );
        if( g_PILCentreLineWidth == ::WidthValues[i] )
            m_choicePILCentreLineWidth->SetSelection( i );
        if( g_PILOffsetLine1Width == ::WidthValues[i] )
            m_choicePILOffsetLine1Width->SetSelection( i );
        if( g_PILOffsetLine2Width == ::WidthValues[i] )
            m_choicePILOffsetLine2Width->SetSelection( i );
    }
    m_sliderFillTransparency->SetValue( g_uiFillTransparency );
    m_sliderInclusionBoundarySize->SetValue( g_iInclusionBoundarySize );
    if( g_bExclusionBoundary && !g_bInclusionBoundary ) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_EXCLUSION );
    else if( !g_bExclusionBoundary && g_bInclusionBoundary ) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_INCLUSION );
    else if( !g_bExclusionBoundary && !g_bInclusionBoundary ) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_NEITHER );
    else m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_EXCLUSION );
    m_checkBoxBoundaryODPointsVisible->SetValue( g_bBoundaryODPointsVisible );
    
    m_colourPickerEBLLineColour->SetColour( g_colourEBLLineColour );
    m_checkBoxRotateWithBoat->SetValue( g_bEBLRotateWithBoat);
    m_radioBoxMaintainWith->SetSelection( g_iEBLMaintainWith );
    m_checkBoxEBLFixedEndPosition->SetValue( g_bEBLFixedEndPosition );
    m_radioBoxEBLPersistence->SetSelection( g_iEBLPersistenceType );
    m_checkBoxEBLShowArrow->SetValue( g_bEBLShowArrow );
    m_checkBoxShowVRM->SetValue( g_bEBLVRM );
    m_checkBoxEBLAllwaysShowInfo->SetValue( g_bEBLAlwaysShowInfo );
    m_checkBoxShowPerpLine->SetValue( g_bEBLPerpLine );

    m_colourPickerPILActiveCentreLineColour->SetColour( g_colourPILActiveCentreLineColour );
    m_colourPickerPILInActiveCentreLineColour->SetColour( g_colourPILInActiveCentreLineColour );
    m_colourPickerPILActiveOffsetLine1Colour->SetColour( g_colourPILActiveOffsetLine1Colour );
    m_colourPickerPILInActiveOffsetLine1Colour->SetColour( g_colourPILInActiveOffsetLine1Colour );
    m_colourPickerPILActiveOffsetLine2Colour->SetColour( g_colourPILActiveOffsetLine2Colour );
    m_colourPickerPILInactiveOffsetLine2Colour->SetColour( g_colourPILInActiveOffsetLine2Colour );
    m_choiceNumIndexLines->SetSelection( g_PILDefaultNumIndexLines );
    if(g_PILDefaultNumIndexLines == 0)
        SetIndexLineChoice( false );
    else
        SetIndexLineChoice( true );
    m_dPILOffsetValidator = g_dPILOffset;
    m_radioBoxPILPersistence->SetSelection( g_iPILPersistenceType );

    m_choiceTextPosition->SetSelection( g_iTextPosition );
    m_iTextPointTextMaxWidth = g_iTextMaxWidth;
    m_radioBoxWidthType->SetSelection(g_iTextMaxWidthType);
    m_colourPickerTextColour->SetColour( g_colourDefaultTextColour );
    m_colourPickerBackgroundColour->SetColour( g_colourDefaultTextBackgroundColour );
    m_sliderBackgroundTransparency->SetValue( g_iTextBackgroundTransparency );
    m_staticTextFontFaceExample->SetFont( g_DisplayTextFont );
    m_radioBoxShowDisplayText->SetSelection( g_iTextPointDisplayTextWhen );
    
    m_checkBoxConfirmObjectDelete->SetValue( g_bConfirmObjectDelete );
    m_checkBoxRememberPropertyPosition->SetValue( g_bRememberPropertyDialogPosition );
    m_checkBoxShowMagBearings->SetValue( g_bShowMag );
    m_checkBoxAllowLeftDrag->SetValue( g_bAllowLeftDrag );
    m_checkBoxShowLayers->SetValue( g_bShowLayers );
    m_spinCtrlNavObjBackups->SetValue( g_navobjbackups );
    m_sliderInitialEdgePan->SetValue( g_InitialEdgePanSensitivity );
    m_sliderEdgePan->SetValue( g_EdgePanSensitivity );
    m_choiceToolbar->Select( g_iDisplayToolbar );
    
    m_colourPickerDRLineColour->SetColour( g_colourDRLineColour );
    m_colourPickerInActiveDRLineColour->SetColour( g_colourInActiveDRLineColour );
    m_choiceDRLineWidth->SetSelection( g_DRLineWidth - 1 );
    m_radioBoxDRLengthType->SetSelection( g_iDRLengthType );
    m_radioBoxDRIntervalType->SetSelection( g_iDRIntervalType );
    m_radioBoxDRDistanceUnits->SetSelection( g_iDRDistanceUnits );
    m_radioBoxDRTimeUnits->SetSelection( g_iDRTimeUnits );
    m_radioBoxDRPersistence->SetSelection( g_iDRPersistenceType );
    m_checkBoxShowDRPointRangeRings->SetValue( g_bDRPointShowRangeRings );
    m_choiceDRPointRangeRingNumber->SetSelection( g_iDRPointRangeRingsNumber );
    m_dDRPointRangRingStepValidator = g_fDRPointRangeRingsStep;
    m_choiceDRPointDistanceUnit->SetSelection( g_iDRPointRangeRingsStepUnits );
    m_colourPickerDRPointRangeRingColours->SetColour( g_colourDRPointRangeRingsColour );
    
    m_colourPickerActiveGZLineColour->SetColour( g_colourActiveGZLineColour );
    m_colourPickerInActiveGZLineColour->SetColour( g_colourInActiveGZLineColour );
    m_colourPickerActiveGZFillColour->SetColour( g_colourActiveGZFillColour );
    m_colourPickerInActiveGZFillColour->SetColour( g_colourInActiveGZFillColour );
    m_checkBoxGZRotateWithBoat->SetValue( g_bGZRotateWithBoat);
    m_radioBoxGZMaintainWith->SetSelection( g_iGZMaintainWith );
    m_radioBoxGZLineType->SetSelection( g_iGZLineType);
    m_sliderGZFillTransparency->SetValue( g_uiGZFillTransparency );
    m_radioBoxGZPersistence->SetSelection( g_iGZPersistenceType );

    m_dSOGValidator = g_dDRSOG;
    m_iCOGValidator = g_iDRCOG;
    m_dDRPathLengthValidator = g_dDRLength;
    m_dODPointIntervalValidator = g_dDRPointInterval;
    

    if(g_ocpn_draw_pi->m_bRecreateConfig) {
        m_buttonConfigFileEntries->SetLabel(_("No"));
        m_staticTextConfigFileEntriesMsg->SetLabel(_("Delete current OCPN_Draw config entries (Yes)"));
    }
    TransferDataToWindow();
    SetDialogSize();
    
    ResetGlobalLocale();
    
    return;
}

void ODPropertiesDialogImpl::SetTableCellBackgroundColours()
{
    wxColour wxRed;
    GetGlobalColor( wxS( "URED" ), &wxRed );
    wxColour wxGreen;
    GetGlobalColor( wxS( "UGREN" ), &wxGreen );
    wxColour wxGray;
    GetGlobalColor( wxS( "GREY1" ), &wxGray );
    
    m_gridODWDInteractions->SetCellBackgroundColour(0, 0, wxGray);
    m_gridODWDInteractions->SetCellBackgroundColour(0, 1, wxGray);
    m_gridODWDInteractions->SetCellBackgroundColour(0, 2, wxGray);
    m_gridODWDInteractions->SetCellBackgroundColour(0, 3, wxGray);
    m_gridODWDInteractions->SetCellBackgroundColour(0, 4, wxGray);
    m_gridODWDInteractions->SetCellBackgroundColour(1, 0, wxGray);
    m_gridODWDInteractions->SetCellBackgroundColour(2, 0, wxGray);
    m_gridODWDInteractions->SetCellBackgroundColour(3, 0, wxGray);
    m_gridODWDInteractions->SetCellBackgroundColour(1, 1, wxGreen);
    m_gridODWDInteractions->SetCellBackgroundColour(2, 1, wxGreen);
    m_gridODWDInteractions->SetCellBackgroundColour(3, 1, wxGreen);
    m_gridODWDInteractions->SetCellBackgroundColour(1, 2, wxGreen);
    m_gridODWDInteractions->SetCellBackgroundColour(2, 2, wxRed);
    m_gridODWDInteractions->SetCellBackgroundColour(3, 2, wxRed);
    m_gridODWDInteractions->SetCellBackgroundColour(1, 3, wxRed);
    m_gridODWDInteractions->SetCellBackgroundColour(2, 3, wxGreen);
    m_gridODWDInteractions->SetCellBackgroundColour(3, 3, wxRed);
    m_gridODWDInteractions->SetCellBackgroundColour(1, 4, wxRed);
    m_gridODWDInteractions->SetCellBackgroundColour(2, 4, wxRed);
    m_gridODWDInteractions->SetCellBackgroundColour(3, 4, wxGreen);
    
    return;
}

void ODPropertiesDialogImpl::OnClickConfigFileEntries( wxCommandEvent& event )
{
    if(!g_ocpn_draw_pi->m_bRecreateConfig) {
        g_ocpn_draw_pi->m_bRecreateConfig = true;
        m_buttonConfigFileEntries->SetLabel(_("No"));
        m_staticTextConfigFileEntriesMsg->SetLabel(_("Delete current OCPN_Draw config entries (Yes)"));
    } else {
        g_ocpn_draw_pi->m_bRecreateConfig = false;
        m_buttonConfigFileEntries->SetLabel(_("Yes"));
        m_staticTextConfigFileEntriesMsg->SetLabel(_("Delete current OCPN_Draw config entries (No)"));
    }
}
