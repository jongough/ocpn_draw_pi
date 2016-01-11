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
#include "ODToolbarImpl.h"
#include "PointMan.h"
#include "version.h"
#include <wx/fontdlg.h>

#if wxCHECK_VERSION(3,0,0) 
#include <wx/valnum.h>
#endif

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
extern bool         g_bExclusionBoundary;
extern bool         g_bInclusionBoundary;
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
extern bool         g_bEBLFixedEndPosition;
extern int          g_EBLPersistenceType;
extern bool         g_bEBLShowArrow;
extern bool         g_bEBLVRM;
extern int          g_EBLLineWidth;
extern int          g_EBLLineStyle;

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
extern int          g_DRPersistenceType;
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

extern bool         g_bConfirmObjectDelete;
extern bool         g_bShowMag;
extern int          g_navobjbackups;

extern int          g_EdgePanSensitivity;
extern int          g_InitialEdgePanSensitivity;

extern int          g_iDisplayToolbar;
extern ODToolbarImpl   *g_pODToolbar;


ODPropertiesDialogImpl::ODPropertiesDialogImpl( wxWindow* parent )
:
ODPropertiesDialogDef( parent )
{
    
    m_staticTextNameVal->SetLabel( wxT("OpenCPN Draw Plugin") );
    m_staticTextMajorVal->SetLabel(wxString::Format(wxT("%i"), PLUGIN_VERSION_MAJOR ));
    m_staticTextMinorVal->SetLabel(wxString::Format(wxT("%i"), PLUGIN_VERSION_MINOR ));
    m_staticTextPatchVal->SetLabel( wxT(TOSTRING(PLUGIN_VERSION_PATCH)) );
    m_staticTextDateVal->SetLabel( wxT(TOSTRING(PLUGIN_VERSION_DATE)) );
    
    m_pfdDialog = NULL;
    
#if wxCHECK_VERSION(3,0,0) 
    wxFloatingPointValidator<double> dODPointRangeRingStepVal(3, &m_dODPointRangRingStepValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dODPointArrivalRadiusVal(3, &m_dODPointArrivalRadiusValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dDRPathLengthVal(3, &m_dDRPathLengthValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dDRPointIntervalVal(3, &m_dODPointIntervalValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dDRPointRangeRingStepVal(3, &m_dDRPointRangRingStepValidator, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dSOGVal(3, &m_dSOGValidator, wxNUM_VAL_DEFAULT);
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
#endif
}

void ODPropertiesDialogImpl::OnODPointComboboxSelected( wxCommandEvent& event )
{
    m_bitmapPointBitmap->SetBitmap( m_bcomboBoxODPointIconName->GetItemBitmap( m_bcomboBoxODPointIconName->GetSelection() ) );
}

void ODPropertiesDialogImpl::OnTextPointIconComboboxSelected( wxCommandEvent& event )
{
    m_bitmapTextPointBitmap->SetBitmap( m_bcomboBoxTextPointIconName->GetItemBitmap( m_bcomboBoxTextPointIconName->GetSelection() ) );
}

void ODPropertiesDialogImpl::OnEBLEndIconComboboxSelected( wxCommandEvent& event )
{
    m_bitmapEBLEndBitmap->SetBitmap( m_bcomboBoxEBLEndIconName->GetItemBitmap( m_bcomboBoxEBLEndIconName->GetSelection() ) );
}

void ODPropertiesDialogImpl::OnEBLStartIconComboboxSelected( wxCommandEvent& event )
{
    m_bitmapEBLStartBitmap->SetBitmap( m_bcomboBoxEBLStartIconName->GetItemBitmap( m_bcomboBoxEBLStartIconName->GetSelection() ) );
}

void ODPropertiesDialogImpl::OnDRPointIconComboboxSelected( wxCommandEvent& event )
{
    m_bitmapDRPointBitmap->SetBitmap( m_bcomboBoxDRPointIconName->GetItemBitmap( m_bcomboBoxDRPointIconName->GetSelection() ) );
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
    
    g_colourActivePathLineColour = m_colourPickerActivePathLineColour->GetColour();
    g_colourInActivePathLineColour = m_colourPickerInActivePathLineColour->GetColour();

    g_PathLineWidth = ::WidthValues[ m_choicePathLineWidth->GetSelection() ];
    g_PathLineStyle = ::StyleValues[ m_choicePathLineStyle->GetSelection()];
    
    g_colourEBLLineColour = m_colourPickerEBLLineColour->GetColour();
    g_EBLLineWidth = ::WidthValues[ m_choiceEBLLineWidth->GetSelection() ];
    g_EBLLineStyle = ::StyleValues[ m_choiceEBLLineStyle->GetSelection() ];
    g_bEBLShowArrow = m_checkBoxEBLShowArrow->GetValue();
    g_bEBLVRM = m_checkBoxShowVRM->GetValue();
    g_EBLPersistenceType = m_radioBoxEBLPersistence->GetSelection();
    g_bEBLFixedEndPosition = m_checkBoxEBLFixedEndPosition->GetValue();
    g_sEBLEndIconName = m_bcomboBoxEBLEndIconName->GetValue();
    g_sEBLStartIconName = g_sEBLEndIconName;
    
    g_sDRPointIconName = m_bcomboBoxDRPointIconName->GetValue();
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
    
    g_iODPointRangeRingsNumber = m_choiceODPointRangeRingNumber->GetSelection();
    g_fODPointRangeRingsStep = atof( m_textCtrlODPointRangeRingSteps->GetValue().mb_str() );
    g_iODPointRangeRingsStepUnits = m_choiceODPointDistanceUnit->GetSelection();
    g_colourODPointRangeRingsColour = m_colourPickerODPointRangeRingColours->GetColour();
    m_textCtrlODPointArrivalRadius->GetValue().ToDouble( &g_n_arrival_circle_radius );
    g_bODPointShowRangeRings = m_checkBoxShowODPointRangeRings->GetValue();
    g_sODPointIconName = m_bcomboBoxODPointIconName->GetValue();
    
    g_bConfirmObjectDelete = m_checkBoxConfirmObjectDelete->GetValue();
    g_bShowMag = m_checkBoxShowMagBearings->GetValue();
    g_navobjbackups = m_spinCtrlNavObjBackups->GetValue();
    
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
        wxString s;
        s.Printf( _T("%.3f"), g_n_arrival_circle_radius );
        m_textCtrlODPointArrivalRadius->SetValue( s );
        
        m_checkBoxShowName->SetValue( g_bODPointShowName );
        m_checkBoxShowODPointRangeRings->SetValue( g_bODPointShowRangeRings );
        m_choiceODPointRangeRingNumber->SetSelection( g_iODPointRangeRingsNumber );
        wxString s_RangeRingStep;
        s_RangeRingStep.Printf( wxT("%.3f"), g_fODPointRangeRingsStep );
        m_textCtrlODPointRangeRingSteps->SetValue( s_RangeRingStep );
        m_choiceODPointDistanceUnit->SetSelection( g_iODPointRangeRingsStepUnits );
        m_colourPickerODPointRangeRingColours->SetColour( g_colourODPointRangeRingsColour );
        
        m_sliderBoundaryPointFillTransparency->SetValue( g_uiBoundaryPointFillTransparency );
        m_sliderInclusionBoundaryPointSize->SetValue( g_iInclusionBoundaryPointSize );
        
        if( g_bExclusionBoundaryPoint && !g_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_EXCLUSION );
        else if( !g_bExclusionBoundaryPoint && g_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_INCLUSION );
        else if( !g_bExclusionBoundaryPoint && !g_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_NIETHER );
        else m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_EXCLUSION );

        m_bcomboBoxODPointIconName->Clear();
        m_bcomboBoxTextPointIconName->Clear();
        m_bcomboBoxEBLEndIconName->Clear();
        m_bcomboBoxEBLStartIconName->Clear();
        m_bcomboBoxDRPointIconName->Clear();
        //      Iterate on the Icon Descriptions, filling in the combo control
        if( g_pODPointMan == NULL ) g_pODPointMan = new PointMan();
        
        bool fillCombo = m_bcomboBoxODPointIconName->GetCount() == 0;
        wxImageList *icons = g_pODPointMan->Getpmarkicon_image_list();

        if( fillCombo  && icons){
            for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
                wxString *ps = g_pODPointMan->GetIconDescription( i );
                m_bcomboBoxODPointIconName->Append( *ps, icons->GetBitmap( i ) );
                m_bcomboBoxTextPointIconName->Append( *ps, icons->GetBitmap( i ) );
                m_bcomboBoxEBLStartIconName->Append( *ps, icons->GetBitmap( i ) );
                m_bcomboBoxEBLEndIconName->Append( *ps, icons->GetBitmap( i ) );
                m_bcomboBoxDRPointIconName->Append( *ps, icons->GetBitmap( i ) );
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
            m_bcomboBoxODPointIconName->Append( g_sODPointIconName, icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxODPointIconName->GetCount() - 1;
        } 
        
        m_bcomboBoxODPointIconName->SetSelection( iconToSelect );
        m_bitmapPointBitmap->SetBitmap( m_bcomboBoxODPointIconName->GetItemBitmap( m_bcomboBoxODPointIconName->GetSelection() ) );
        
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
            m_bcomboBoxTextPointIconName->Append( g_sTextPointIconName, icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxTextPointIconName->GetCount() - 1;
        } 
        
        m_bcomboBoxTextPointIconName->SetSelection( iconToSelect );
        m_bitmapTextPointBitmap->SetBitmap( m_bcomboBoxTextPointIconName->GetItemBitmap( m_bcomboBoxTextPointIconName->GetSelection() ) );
        
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
            m_bcomboBoxEBLEndIconName->Append( g_sEBLEndIconName, icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxEBLEndIconName->GetCount() - 1;
        } 
        
        m_bcomboBoxEBLEndIconName->SetSelection( iconToSelect );
        m_bitmapEBLEndBitmap->SetBitmap( m_bcomboBoxEBLEndIconName->GetItemBitmap( m_bcomboBoxEBLEndIconName->GetSelection() ) );
        
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
            m_bcomboBoxEBLStartIconName->Append( g_sEBLStartIconName, icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxEBLStartIconName->GetCount() - 1;
        } 
        
        m_bcomboBoxEBLStartIconName->SetSelection( iconToSelect );
        m_bitmapEBLStartBitmap->SetBitmap( m_bcomboBoxEBLStartIconName->GetItemBitmap( m_bcomboBoxEBLStartIconName->GetSelection() ) );
        
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
            m_bcomboBoxDRPointIconName->Append( g_sDRPointIconName, icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxDRPointIconName->GetCount() - 1;
        } 
        
        m_bcomboBoxDRPointIconName->SetSelection( iconToSelect );
        m_bitmapDRPointBitmap->SetBitmap( m_bcomboBoxDRPointIconName->GetItemBitmap( m_bcomboBoxDRPointIconName->GetSelection() ) );
        
        icons = NULL;

        m_colourPickerActiveBoundaryLineColour->SetColour( g_colourActiveBoundaryLineColour );
        m_colourPickerInActiveBoundaryLineColour->SetColour( g_colourInActiveBoundaryLineColour );
        m_colourPickerActiveBoundaryFillColour->SetColour( g_colourActiveBoundaryFillColour );
        m_colourPickerInActiveBoundaryFillColour->SetColour( g_colourInActiveBoundaryFillColour );
        
        m_colourPickerActivePathLineColour->SetColour( g_colourActivePathLineColour );
        m_colourPickerInActivePathLineColour->SetColour( g_colourInActivePathLineColour );
        
        for( unsigned int i = 0; i < sizeof( ::StyleValues ) / sizeof(int); i++ ) {
            if( g_BoundaryLineStyle == ::StyleValues[i] )
                m_choiceBoundaryLineStyle->SetSelection( i );
            if( g_PathLineStyle == ::StyleValues[i] )
                m_choicePathLineStyle->SetSelection( i );
            if( g_EBLLineStyle == ::StyleValues[i] )
                m_choiceEBLLineStyle->SetSelection( i );
            if( g_DRLineStyle == ::StyleValues[i] )
                m_choiceDRLineStyle->SetSelection( i );
            if( g_iDRPointRangeRingLineStyle == ::StyleValues[i] )
                m_choiceDRPointRangeRingStyle->SetSelection( i );
            if( g_iBoundaryPointRangeRingLineStyle == ::StyleValues[i] )
                m_choiceRangeRingStyle->SetSelection( i );
        }
        for( unsigned int i = 0; i < sizeof( ::WidthValues ) / sizeof(int); i++ ) {
            if( g_BoundaryLineWidth == ::WidthValues[i] )
                m_choiceBoundaryLineWidth->SetSelection( i );
            if( g_PathLineWidth == ::WidthValues[i] )
                m_choicePathLineWidth->SetSelection( i );
            if( g_EBLLineWidth == ::WidthValues[i] )
                m_choiceEBLLineWidth->SetSelection( i );
            if( g_iBoundaryPointRangeRingLineWidth == ::WidthValues[i] )
                m_choiceRangeRingWidth->SetSelection( i );
            if( g_iDRPointRangeRingLineWidth == ::WidthValues[i] )
                m_choiceDRPointRangeRingWidth->SetSelection( i );
            if( g_DRLineWidth == ::WidthValues[i] )
                m_choiceDRLineWidth->SetSelection( i );
        }
        m_sliderFillTransparency->SetValue( g_uiFillTransparency );
        m_sliderInclusionBoundarySize->SetValue( g_iInclusionBoundarySize );
        if( g_bExclusionBoundary && !g_bInclusionBoundary ) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_EXCLUSION );
        else if( !g_bExclusionBoundary && g_bInclusionBoundary ) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_INCLUSION );
        else if( !g_bExclusionBoundary && !g_bInclusionBoundary ) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_NIETHER );
        else m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_EXCLUSION );
        
        m_colourPickerEBLLineColour->SetColour( g_colourEBLLineColour );
        m_checkBoxEBLFixedEndPosition->SetValue( g_bEBLFixedEndPosition );
        m_radioBoxEBLPersistence->SetSelection( g_EBLPersistenceType );
        m_checkBoxEBLShowArrow->SetValue( g_bEBLShowArrow );
        m_checkBoxShowVRM->SetValue( g_bEBLVRM );
        
        m_choiceTextPosition->SetSelection( g_iTextPosition );
        m_colourPickerTextColour->SetColour( g_colourDefaultTextColour );
        m_colourPickerBackgroundColour->SetColour( g_colourDefaultTextBackgroundColour );
        m_sliderBackgroundTransparency->SetValue( g_iTextBackgroundTransparency );
        m_staticTextFontFaceExample->SetFont( g_DisplayTextFont );
        m_radioBoxShowDisplayText->SetSelection( g_iTextPointDisplayTextWhen );
        
        m_checkBoxConfirmObjectDelete->SetValue( g_bConfirmObjectDelete );
        m_checkBoxShowMagBearings->SetValue( g_bShowMag );
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
        wxString s_DRRangeRingStep;
        s_DRRangeRingStep.Printf( wxT("%.3f"), g_fDRPointRangeRingsStep );
        m_textCtrlDRPointRangeRingSteps->SetValue( s_DRRangeRingStep );
        m_choiceDRPointDistanceUnit->SetSelection( g_iDRPointRangeRingsStepUnits );
        m_colourPickerDRPointRangeRingColours->SetColour( g_colourDRPointRangeRingsColour );
        
        

        s.Printf( _T("%.3f"), g_dDRSOG );
        m_textCtrlSOG->SetValue( s );
        s.Printf( _T("%i"), g_iDRCOG );
        m_textCtrlCOG->SetValue( s );
        s.Printf( _T("%.3f"), g_dDRLength );
        m_textCtrlDRPathLength->SetValue( s );
        s.Printf( _T("%.3f"), g_dDRPointInterval );
        m_textCtrlDRPointInterval->SetValue( s );
        

        SetDialogSize();

    return;
}

