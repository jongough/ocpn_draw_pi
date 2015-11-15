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
extern bool         g_bExclusionBoundary;
extern bool         g_bInclusionBoundary;
extern int          g_PathLineWidth; 
extern int          g_PathLineStyle;
extern unsigned int g_uiFillTransparency;
extern int          g_iInclusionBoundarySize;

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
extern wxString     g_sDREndIconName;
extern wxString     g_sDRStartIconName;
extern wxColour     g_colourDRLineColour;
extern int          g_DRPersistenceType;
extern bool         g_bDRShowArrow;
extern int          g_DRLineWidth;
extern int          g_DRLineStyle;

extern bool         g_bConfirmObjectDelete;
extern bool         g_bShowMag;
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

void ODPropertiesDialogImpl::OnEBLStartIconComboboxSelected( wxCommandEvent& event )
{
    m_bitmapEBLStartBitmap->SetBitmap( m_bcomboBoxEBLStartIconName->GetItemBitmap( m_bcomboBoxEBLStartIconName->GetSelection() ) );
}

void ODPropertiesDialogImpl::OnDREndIconComboboxSelected( wxCommandEvent& event )
{
    m_bitmapDREndBitmap->SetBitmap( m_bcomboBoxDREndIconName->GetItemBitmap( m_bcomboBoxDREndIconName->GetSelection() ) );
}

void ODPropertiesDialogImpl::OnDRStartIconComboboxSelected( wxCommandEvent& event )
{
    m_bitmapDRStartBitmap->SetBitmap( m_bcomboBoxDRStartIconName->GetItemBitmap( m_bcomboBoxDRStartIconName->GetSelection() ) );
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
    g_iInclusionBoundarySize = m_sliderInclusionBoundarySize->GetValue();
    
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
        case ID_BOUNDARY_NONE:
            g_bExclusionBoundary = false;
            g_bInclusionBoundary = false;
            break;
        default:
            g_bExclusionBoundary = true;
            g_bInclusionBoundary = false;
            break;
    }
    
    g_colourActivePathLineColour = m_colourPickerActivePathLineColour->GetColour();
    g_colourInActivePathLineColour = m_colourPickerInActivePathLineColour->GetColour();

    g_PathLineWidth = m_choicePathLineWidth->GetSelection() + 1;
    g_PathLineStyle = ::StyleValues[ m_choicePathLineStyle->GetSelection()];
    
    g_colourEBLLineColour = m_colourPickerEBLLineColour->GetColour();
    g_EBLLineWidth = m_choiceEBLLineWidth->GetSelection() + 1;
    g_EBLLineStyle = ::StyleValues[ m_choiceEBLLineStyle->GetSelection()];
    g_bEBLShowArrow = m_checkBoxEBLShowArrow->GetValue();
    g_bEBLVRM = m_checkBoxShowVRM->GetValue();
    g_EBLPersistenceType = m_radioBoxEBLPersistence->GetSelection();
    g_bEBLFixedEndPosition = m_checkBoxEBLFixedEndPosition->GetValue();
    g_sEBLEndIconName = m_bcomboBoxEBLEndIconName->GetValue();
    g_sEBLStartIconName = g_sEBLEndIconName;
    
    g_sDREndIconName = m_bcomboBoxDREndIconName->GetValue();
    g_sDRStartIconName = m_bcomboBoxDRStartIconName->GetValue();
    g_colourDRLineColour = m_colourPickerDRLineColour->GetColour();
    g_DRLineWidth = m_choiceDRLineWidth->GetSelection() + 1;
    g_DRLineStyle = ::StyleValues[ m_choiceDRLineStyle->GetSelection()];
    g_bDRShowArrow = m_checkBoxDRShowArrow->GetValue();
    m_textCtrlSOG->GetValue().ToDouble( &g_dDRSOG );
    g_iDRCOG = wxAtoi( m_textCtrlCOG->GetValue() );
    g_iDRLengthType = m_radioBoxDRLengthType->GetSelection();
    g_iDRIntervalType = m_radioBoxDRIntervalType->GetSelection();
    g_iDRDistanceUnits = m_radioBoxDRDistanceUnits->GetSelection();
    g_iDRTimeUnits = m_radioBoxDRTimeUnits->GetSelection();
    g_iDRPersistenceType = m_radioBoxDRPersistence->GetSelection();
    m_textCtrlDRPathLength->GetValue().ToDouble( &g_dDRLength );
    m_textCtrlDRPointInterval->GetValue().ToDouble( &g_dDRPointInterval );
    
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

        m_bcomboBoxODPointIconName->Clear();
        m_bcomboBoxEBLEndIconName->Clear();
        m_bcomboBoxEBLStartIconName->Clear();
        m_bcomboBoxDREndIconName->Clear();
        m_bcomboBoxDRStartIconName->Clear();
        //      Iterate on the Icon Descriptions, filling in the combo control
        if( g_pODPointMan == NULL ) g_pODPointMan = new PointMan();
        
        bool fillCombo = m_bcomboBoxODPointIconName->GetCount() == 0;
        wxImageList *icons = g_pODPointMan->Getpmarkicon_image_list();

        if( fillCombo  && icons){
            for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
                wxString *ps = g_pODPointMan->GetIconDescription( i );
                m_bcomboBoxODPointIconName->Append( *ps, icons->GetBitmap( i ) );
                m_bcomboBoxEBLStartIconName->Append( *ps, icons->GetBitmap( i ) );
                m_bcomboBoxEBLEndIconName->Append( *ps, icons->GetBitmap( i ) );
                m_bcomboBoxDRStartIconName->Append( *ps, icons->GetBitmap( i ) );
                m_bcomboBoxDREndIconName->Append( *ps, icons->GetBitmap( i ) );
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

        iconToSelect = -1;
        for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
            if( *g_pODPointMan->GetIconDescription( i ) == g_sDREndIconName ) {
                iconToSelect = i;
                break;
            }
        }
        //  not found, so add  it to the list, with a generic bitmap and using the name as description
        // n.b.  This should never happen...
        if( -1 == iconToSelect){    
            m_bcomboBoxDREndIconName->Append( g_sDREndIconName, icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxDREndIconName->GetCount() - 1;
        } 
        
        m_bcomboBoxDREndIconName->SetSelection( iconToSelect );
        m_bitmapDREndBitmap->SetBitmap( m_bcomboBoxDREndIconName->GetItemBitmap( m_bcomboBoxDREndIconName->GetSelection() ) );
        
        iconToSelect = -1;
        for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
            if( *g_pODPointMan->GetIconDescription( i ) == g_sDRStartIconName ) {
                iconToSelect = i;
                break;
            }
        }
        //  not found, so add  it to the list, with a generic bitmap and using the name as description
        // n.b.  This should never happen...
        if( -1 == iconToSelect){    
            m_bcomboBoxDRStartIconName->Append( g_sDRStartIconName, icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxDRStartIconName->GetCount() - 1;
        } 
        
        m_bcomboBoxDRStartIconName->SetSelection( iconToSelect );
        m_bitmapDRStartBitmap->SetBitmap( m_bcomboBoxDRStartIconName->GetItemBitmap( m_bcomboBoxDRStartIconName->GetSelection() ) );
        
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
            if( g_DRLineStyle == ::StyleValues[i] )
                m_choiceDRLineStyle->Select( i );
        }
        m_choiceBoundaryLineWidth->SetSelection( g_BoundaryLineWidth - 1 );
        m_choicePathLineWidth->SetSelection( g_PathLineWidth - 1 );
        m_sliderFillTransparency->SetValue( g_uiFillTransparency );
        m_sliderInclusionBoundarySize->SetValue( g_iInclusionBoundarySize );
        if( g_bExclusionBoundary && !g_bInclusionBoundary ) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_EXCLUSION );
        else if( !g_bExclusionBoundary && g_bInclusionBoundary ) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_INCLUSION );
        else if( !g_bExclusionBoundary && !g_bInclusionBoundary ) m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_NONE );
        else m_radioBoxBoundaryType->SetSelection( ID_BOUNDARY_EXCLUSION );
        
        m_colourPickerEBLLineColour->SetColour( g_colourEBLLineColour );
        m_checkBoxEBLFixedEndPosition->SetValue( g_bEBLFixedEndPosition );
        m_radioBoxEBLPersistence->SetSelection( g_EBLPersistenceType );
        m_choiceEBLLineWidth->SetSelection( g_EBLLineWidth - 1 );
        m_checkBoxEBLShowArrow->SetValue( g_bEBLShowArrow );
        m_checkBoxShowVRM->SetValue( g_bEBLVRM );
        
        m_choiceTextPosition->SetSelection( g_iTextPosition );
        m_colourPickerTextColour->SetColour( g_colourDefaultTextColour );
        m_colourPickerBackgroundColour->SetColour( g_colourDefaultTextBackgroundColour );
        m_sliderBackgroundTransparency->SetValue( g_iTextBackgroundTransparency );
        m_staticTextFontFaceExample->SetFont( g_DisplayTextFont );
        
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

