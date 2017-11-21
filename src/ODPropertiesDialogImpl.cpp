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

extern ocpn_draw_pi *g_ocpn_draw_pi;
extern PointMan     *g_pODPointMan;
extern int          g_path_line_width;
extern wxString     g_OD_default_wp_icon;

extern int          g_BoundaryLineWidth; 
extern int          g_BoundaryLineStyle;
extern wxColour     g_colourActivePathLineColour;
extern wxColour     g_colourInActivePathLineColour;
extern wxColour     g_colourActiveBoundaryLineColour;
extern wxColour     g_colourInActiveBoundaryLineColour;
extern wxColour     g_colourActiveBoundaryFillColour;
extern wxColour     g_colourInActiveBoundaryFillColour;
extern bool         g_bExclusionBoundary;
extern bool         g_bInclusionBoundary;
extern bool         g_bBoundaryODPointsVisible;
extern bool         g_bExclusionBoundaryPoint;
extern bool         g_bInclusionBoundaryPoint;
extern int          g_PathLineWidth; 
extern int          g_PathLineStyle;
extern unsigned int g_uiFillTransparency;
extern int          g_iInclusionBoundarySize;

extern unsigned int g_uiBoundaryPointFillTransparency;
extern int          g_iInclusionBoundaryPointSize;
extern int         g_iBoundaryPointRangeRingLineWidth;
extern int         g_iBoundaryPointRangeRingLineStyle;

extern bool         g_bODPointShowName;
extern bool         g_bODPointShowRangeRings;
extern int          g_iODPointRangeRingsNumber;
extern float        g_fODPointRangeRingsStep;
extern int          g_iODPointRangeRingsStepUnits;
extern wxColour     g_colourODPointRangeRingsColour;
extern wxString     g_sODPointIconName;

extern double       g_n_arrival_circle_radius;

extern wxString     g_sTextPointIconName;
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
extern int          g_iTextPointDisplayTextWhen;

extern wxString     g_sEBLEndIconName;
extern wxString     g_sEBLStartIconName;
extern wxColour     g_colourEBLLineColour;
extern wxColour     g_colourInActiveDRLineColour;
extern bool         g_bEBLRotateWithBoat;
extern int          g_iEBLMaintainWith;
extern bool         g_bEBLFixedEndPosition;
extern int          g_iEBLPersistenceType;
extern bool         g_bEBLShowArrow;
extern bool         g_bEBLVRM;
extern bool         g_bEBLAlwaysShowInfo;
extern bool         g_bEBLPerpLine;
extern int          g_EBLLineWidth;
extern int          g_EBLLineStyle;

extern wxString     g_sPILEndIconName;
extern wxString     g_sPILStartIconName;
extern wxColour     g_colourPILActiveCentreLineColour;
extern wxColour     g_colourPILInActiveCentreLineColour;
extern wxColour     g_colourPILActiveOffsetLine1Colour;
extern wxColour     g_colourPILInActiveOffsetLine1Colour;
extern wxColour     g_colourPILActiveOffsetLine2Colour;
extern wxColour     g_colourPILInActiveOffsetLine2Colour;
extern int          g_iPILPersistenceType;
extern int          g_PILCentreLineWidth;
extern int          g_PILCentreLineStyle;
extern int          g_PILOffsetLine1Width;
extern int          g_PILOffsetLine1Style;
extern int          g_PILOffsetLine2Width;
extern int          g_PILOffsetLine2Style;
extern int          g_PILDefaultNumIndexLines;
extern double       g_dPILOffset;


extern double       g_dDRSOG;
extern int          g_iDRCOG;
extern double       g_dDRLength;
extern double       g_dDRPointInterval;
extern int          g_iDRLengthType;
extern int          g_iDRIntervalType;
extern int          g_iDRDistanceUnits;
extern int          g_iDRTimeUnits;
extern int          g_iDRPersistenceType;
extern wxString     g_sDRPointIconName;
extern wxColour     g_colourDRLineColour;
extern int          g_iDRPersistenceType;
extern bool         g_bDRShowArrow;
extern int          g_DRLineWidth;
extern int          g_DRLineStyle;
extern bool         g_bDRPointShowRangeRings;
extern int          g_iDRPointRangeRingsNumber;
extern float        g_fDRPointRangeRingsStep;
extern int          g_iDRPointRangeRingsStepUnits;
extern wxColour     g_colourDRPointRangeRingsColour;
extern int          g_iDRPointRangeRingLineWidth;
extern int          g_iDRPointRangeRingLineStyle;

extern wxString     g_sGZFirstIconName;
extern wxString     g_sGZSecondIconName;
extern wxColour     g_colourActiveGZLineColour;
extern wxColour     g_colourInActiveGZLineColour;
extern wxColour     g_colourActiveGZFillColour;
extern wxColour     g_colourInActiveGZFillColour;
extern int          g_GZLineWidth; 
extern int          g_GZLineStyle;
extern unsigned int g_uiGZFillTransparency;
extern bool         g_bGZRotateWithBoat;
extern int          g_iGZMaintainWith;
extern int          g_iGZPersistenceType;

extern bool         g_bConfirmObjectDelete;
extern bool         g_bShowMag;
extern bool         g_bAllowLeftDrag;
extern int          g_navobjbackups;

extern int          g_EdgePanSensitivity;
extern int          g_InitialEdgePanSensitivity;

extern int          g_iDisplayToolbar;
extern ODToolbarImpl   *g_pODToolbar;


ODPropertiesDialogImpl::ODPropertiesDialogImpl( wxWindow* parent )
:
ODPropertiesDialogDef( parent )
{
#if wxCHECK_VERSION(3,0,0)
    wxDialog::SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
#endif // wxCHECK_VERSION(3,0,0)
    
    m_staticTextNameVal->SetLabel( wxT("OpenCPN Draw Plugin") );
    m_staticTextMajorVal->SetLabel(wxString::Format(wxT("%i"), PLUGIN_VERSION_MAJOR ));
    m_staticTextMinorVal->SetLabel(wxString::Format(wxT("%i"), PLUGIN_VERSION_MINOR ));
    m_staticTextPatchVal->SetLabel( wxT(TOSTRING(PLUGIN_VERSION_PATCH)) );
    m_staticTextDateVal->SetLabel( wxT(TOSTRING(PLUGIN_VERSION_DATE)) );

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
    
#if wxCHECK_VERSION(3,0,0)  && !defined(_WXMSW_)
    wxFloatingPointValidator<double> dODPointRangeRingStepVal(3, &m_dODPointRangRingStepValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dODPointArrivalRadiusVal(3, &m_dODPointArrivalRadiusValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dDRPathLengthVal(3, &m_dDRPathLengthValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dDRPointIntervalVal(3, &m_dODPointIntervalValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dDRPointRangeRingStepVal(3, &m_dDRPointRangRingStepValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dSOGVal(3, &m_dSOGValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dPILOffset(3, &m_dPILOffsetValidator, wxNUM_VAL_DEFAULT);
    wxIntegerValidator<int> iCOGVal(&m_iCOGValidator, wxNUM_VAL_DEFAULT);
    dODPointRangeRingStepVal.SetMin(0);
    dODPointArrivalRadiusVal.SetMin(0);
    dDRPathLengthVal.SetMin(0);
    dDRPointIntervalVal.SetMin(0);
    dDRPointRangeRingStepVal.SetMin(0);
    dSOGVal.SetMin(0);
    iCOGVal.SetRange(0, 360);

    m_textCtrlODPointRangeRingSteps->SetValidator( dODPointRangeRingStepVal );
    m_textCtrlODPointArrivalRadius->SetValidator( dODPointArrivalRadiusVal );
    m_textCtrlSOG->SetValidator( dSOGVal );
    m_textCtrlCOG->SetValidator( iCOGVal );
    m_textCtrlDRPathLength->SetValidator( dDRPathLengthVal );
    m_textCtrlDRPointInterval->SetValidator( dDRPointIntervalVal );
    m_textCtrlDRPointRangeRingSteps->SetValidator( dDRPointRangeRingStepVal );
    m_textCtrlPILDefaultOffset->SetValidator( dPILOffset );
    
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
    m_SizerTextPointIconName->Replace(m_bcomboBoxTextPointIconName, m_bODIComboBoxTextPointIconName);

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
    g_bODPointShowName =m_checkBoxShowName->GetValue();
    //g_colourActiveBoundaryLineColour = PickerODPointRangeRingColours->GetColour();
    g_colourActiveBoundaryLineColour = m_colourPickerActiveBoundaryLineColour->GetColour();
    g_colourActiveBoundaryFillColour = m_colourPickerActiveBoundaryFillColour->GetColour();
    g_colourInActiveBoundaryLineColour = m_colourPickerInActiveBoundaryLineColour->GetColour();
    g_colourInActiveBoundaryFillColour = m_colourPickerInActiveBoundaryFillColour->GetColour();

    g_BoundaryLineWidth = ::WidthValues[ m_choiceBoundaryLineWidth->GetSelection() ];
    g_BoundaryLineStyle = ::StyleValues[ m_choiceBoundaryLineStyle->GetSelection() ];
    g_uiFillTransparency = m_sliderFillTransparency->GetValue();
    g_uiBoundaryPointFillTransparency = m_sliderBoundaryPointFillTransparency->GetValue();
    g_iBoundaryPointRangeRingLineWidth = ::WidthValues[ m_choiceRangeRingWidth->GetSelection() ];
    g_iBoundaryPointRangeRingLineStyle = ::StyleValues[ m_choiceRangeRingStyle->GetSelection() ];
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
        case ID_BOUNDARY_NIETHER:
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
        case ID_BOUNDARY_NIETHER:
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
    g_EBLLineStyle = ::StyleValues[ m_choiceEBLLineStyle->GetSelection() ];
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
    g_PILCentreLineStyle = ::StyleValues[ m_choicePILCentreLineStyle->GetSelection() ];
    g_PILOffsetLine1Width = ::WidthValues[ m_choicePILOffsetLine1Width->GetSelection() ];
    g_PILOffsetLine1Style = ::StyleValues[ m_choicePILOffsetLine1Style->GetSelection() ];
    g_PILOffsetLine2Width = ::WidthValues[ m_choicePILOffsetLine2Width->GetSelection() ];
    g_PILOffsetLine2Style = ::StyleValues[ m_choicePILOffsetLine2Style->GetSelection() ];
    g_PILDefaultNumIndexLines = m_choiceNumIndexLines->GetSelection();
    g_iPILPersistenceType = m_radioBoxPILPersistence->GetSelection();
    g_bEBLFixedEndPosition = m_checkBoxEBLFixedEndPosition->GetValue();
    g_sPILEndIconName = m_bODIComboBoxPILEndIconName->GetValue();
    g_sPILStartIconName = m_bODIComboBoxPILStartIconName->GetValue();
    m_textCtrlPILDefaultOffset->GetValue().ToDouble( &g_dPILOffset );

    g_sDRPointIconName = m_bODIComboBoxDRPointIconName->GetValue();
    g_colourDRLineColour = m_colourPickerDRLineColour->GetColour();
    g_colourInActiveDRLineColour = m_colourPickerInActiveDRLineColour->GetColour();
    g_DRLineWidth = ::WidthValues[ m_choiceDRLineWidth->GetSelection() ];
    g_DRLineStyle = ::StyleValues[ m_choiceDRLineStyle->GetSelection()];
    g_bDRShowArrow = m_checkBoxDRShowArrow->GetValue();
    m_textCtrlSOG->GetValue().ToDouble( &g_dDRSOG );
    m_textCtrlCOG->GetValue().ToLong( (long*)&g_iDRCOG );
    g_iDRLengthType = m_radioBoxDRLengthType->GetSelection();
    g_iDRIntervalType = m_radioBoxDRIntervalType->GetSelection();
    g_iDRDistanceUnits = m_radioBoxDRDistanceUnits->GetSelection();
    g_iDRTimeUnits = m_radioBoxDRTimeUnits->GetSelection();
    g_iDRPersistenceType = m_radioBoxDRPersistence->GetSelection();
    m_textCtrlDRPathLength->GetValue().ToDouble( &g_dDRLength );
    m_textCtrlDRPointInterval->GetValue().ToDouble( &g_dDRPointInterval );
    g_bDRPointShowRangeRings = m_checkBoxShowDRPointRangeRings->GetValue();
    g_iDRPointRangeRingsNumber = m_choiceDRPointRangeRingNumber->GetSelection();
    g_fDRPointRangeRingsStep = atof( m_textCtrlDRPointRangeRingSteps->GetValue().mb_str() );
    g_iDRPointRangeRingsStepUnits = m_choiceDRPointDistanceUnit->GetSelection();
    g_colourDRPointRangeRingsColour = m_colourPickerDRPointRangeRingColours->GetColour();
    g_iDRPointRangeRingLineWidth = ::WidthValues[ m_choiceDRPointRangeRingWidth->GetSelection() ];
    g_iDRPointRangeRingLineStyle = ::StyleValues[ m_choiceDRPointRangeRingStyle->GetSelection() ];
    
    g_colourActiveGZLineColour = m_colourPickerActiveGZLineColour->GetColour();
    g_colourActiveGZFillColour = m_colourPickerActiveGZFillColour->GetColour();
    g_colourInActiveGZLineColour = m_colourPickerInActiveGZLineColour->GetColour();
    g_colourInActiveGZFillColour = m_colourPickerInActiveGZFillColour->GetColour();
    g_GZLineWidth = ::WidthValues[ m_choiceGZLineWidth->GetSelection() ];
    g_GZLineStyle = ::StyleValues[ m_choiceGZLineStyle->GetSelection() ];
    g_uiGZFillTransparency = m_sliderGZFillTransparency->GetValue();
    g_sGZFirstIconName = m_bODIComboBoxGZFirstIconName->GetValue();
    g_sGZSecondIconName = m_bODIComboBoxGZSecondIconName->GetValue();;
    g_bGZRotateWithBoat = m_checkBoxGZRotateWithBoat->GetValue();
    g_iGZMaintainWith = m_radioBoxGZMaintainWith->GetSelection();
    g_iGZPersistenceType = m_radioBoxGZPersistence->GetSelection();
    
    
    g_iODPointRangeRingsNumber = m_choiceODPointRangeRingNumber->GetSelection();
    g_fODPointRangeRingsStep = atof( m_textCtrlODPointRangeRingSteps->GetValue().mb_str() );
    g_iODPointRangeRingsStepUnits = m_choiceODPointDistanceUnit->GetSelection();
    g_colourODPointRangeRingsColour = m_colourPickerODPointRangeRingColours->GetColour();
    m_textCtrlODPointArrivalRadius->GetValue().ToDouble( &g_n_arrival_circle_radius );
    g_bODPointShowRangeRings = m_checkBoxShowODPointRangeRings->GetValue();
    g_sODPointIconName = m_bODIComboBoxODPointIconName->GetValue();
    
    g_bConfirmObjectDelete = m_checkBoxConfirmObjectDelete->GetValue();
    g_bShowMag = m_checkBoxShowMagBearings->GetValue();
    g_bAllowLeftDrag = m_checkBoxAllowLeftDrag->GetValue();
    g_navobjbackups = m_spinCtrlNavObjBackups->GetValue();
    
    g_sTextPointIconName = m_bODIComboBoxTextPointIconName->GetValue();
    g_iTextPosition = m_choiceTextPosition->GetSelection();
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
    SetTableCellBackgroundColours();
    m_textCtrlODPointArrivalRadius->SetValue( wxString::Format( _T("%.3f"), g_n_arrival_circle_radius ) );
    
    m_checkBoxShowName->SetValue( g_bODPointShowName );
    m_checkBoxShowODPointRangeRings->SetValue( g_bODPointShowRangeRings );
    m_choiceODPointRangeRingNumber->SetSelection( g_iODPointRangeRingsNumber );
    m_textCtrlODPointRangeRingSteps->SetValue( wxString::Format( wxT("%.3f"), g_fODPointRangeRingsStep ) );
    m_choiceODPointDistanceUnit->SetSelection( g_iODPointRangeRingsStepUnits );
    m_colourPickerODPointRangeRingColours->SetColour( g_colourODPointRangeRingsColour );
    
    m_sliderBoundaryPointFillTransparency->SetValue( g_uiBoundaryPointFillTransparency );
    m_sliderInclusionBoundaryPointSize->SetValue( g_iInclusionBoundaryPointSize );
    
    if( g_bExclusionBoundaryPoint && !g_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_EXCLUSION );
    else if( !g_bExclusionBoundaryPoint && g_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_INCLUSION );
    else if( !g_bExclusionBoundaryPoint && !g_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_NIETHER );
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
    else if( !g_bExclusionBoundary && !g_bInclusionBoundary ) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_NIETHER );
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
    m_textCtrlPILDefaultOffset->SetValue( wxString::Format(wxT("%.3f"), g_dPILOffset));
    m_radioBoxPILPersistence->SetSelection( g_iPILPersistenceType );

    m_choiceTextPosition->SetSelection( g_iTextPosition );
    m_colourPickerTextColour->SetColour( g_colourDefaultTextColour );
    m_colourPickerBackgroundColour->SetColour( g_colourDefaultTextBackgroundColour );
    m_sliderBackgroundTransparency->SetValue( g_iTextBackgroundTransparency );
    m_staticTextFontFaceExample->SetFont( g_DisplayTextFont );
    m_radioBoxShowDisplayText->SetSelection( g_iTextPointDisplayTextWhen );
    
    m_checkBoxConfirmObjectDelete->SetValue( g_bConfirmObjectDelete );
    m_checkBoxShowMagBearings->SetValue( g_bShowMag );
    m_checkBoxAllowLeftDrag->SetValue( g_bAllowLeftDrag );
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
    m_textCtrlDRPointRangeRingSteps->SetValue( wxString::Format( wxT("%.3f"), g_fDRPointRangeRingsStep ) );
    m_choiceDRPointDistanceUnit->SetSelection( g_iDRPointRangeRingsStepUnits );
    m_colourPickerDRPointRangeRingColours->SetColour( g_colourDRPointRangeRingsColour );
    
    m_colourPickerActiveGZLineColour->SetColour( g_colourActiveGZLineColour );
    m_colourPickerInActiveGZLineColour->SetColour( g_colourInActiveGZLineColour );
    m_colourPickerActiveGZFillColour->SetColour( g_colourActiveGZFillColour );
    m_colourPickerInActiveGZFillColour->SetColour( g_colourInActiveGZFillColour );
    m_checkBoxGZRotateWithBoat->SetValue( g_bGZRotateWithBoat);
    m_radioBoxGZMaintainWith->SetSelection( g_iGZMaintainWith );
    m_sliderGZFillTransparency->SetValue( g_uiGZFillTransparency );
    m_radioBoxGZPersistence->SetSelection( g_iGZPersistenceType );

    m_textCtrlSOG->SetValue( wxString::Format( _T("%.3f"), g_dDRSOG ) );
    m_textCtrlCOG->SetValue( wxString::Format( _T("%i"), g_iDRCOG ) );
    m_textCtrlDRPathLength->SetValue( wxString::Format( _T("%.3f"), g_dDRLength ) );
    m_textCtrlDRPointInterval->SetValue( wxString::Format( _T("%.3f"), g_dDRPointInterval ) );
    

    SetDialogSize();

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
