/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  OCPN Draw Point Properties Dialog support
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


#include "ODPointPropertiesImpl.h"
#include "ocpn_draw_pi.h"
#include "ocpn_plugin.h"
#include "ODConfig.h"
#include "ODIconCombo.h"
#include "ODPoint.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "ODPathPropertiesDialogImpl.h"
#include "PointMan.h"
#include "GZ.h"
#include "GZMan.h"
#include "ODPositionParser.h"
#include <wx/clipbrd.h>
#include <wx/menu.h>
#include <wx/window.h>
#include <wx/fontdlg.h>

#if wxCHECK_VERSION(3,0,0) 
#include <wx/valnum.h>
#endif

extern ODSelect             *g_pODSelect;
extern ocpn_draw_pi         *g_ocpn_draw_pi;
extern PointMan             *g_pODPointMan;
extern PathMan              *g_pPathMan;
extern GZMan                *g_pGZMan;
extern ODConfig             *g_pODConfig;
extern PathManagerDialog    *g_pPathManagerDialog;
extern ODPathPropertiesDialogImpl *g_pODPathPropDialog;
extern int                  g_iTextPosition;
extern int                  g_iBoundaryPointRangeRingLineWidth;
extern int                  g_iBoundaryPointRangeRingLineStyle;


ODPointPropertiesImpl::ODPointPropertiesImpl( wxWindow* parent )
:
ODPointPropertiesDialog( parent )
{
#if wxCHECK_VERSION(3,0,0)
    wxDialog::SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
#endif // wxCHECK_VERSION(3,0,0)

    this->Connect( ID_RCLK_MENU_COPY, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    this->Connect( ID_RCLK_MENU_COPY_LL, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    this->Connect( ID_RCLK_MENU_PASTE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    this->Connect( ID_RCLK_MENU_PASTE_LL, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    
    DimeWindow( this );

    m_pODPoint = NULL;
    m_pfdDialog = NULL;
    
#if wxCHECK_VERSION(3,0,0)
    SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
#ifndef _WXMSW_
    wxFloatingPointValidator<double> dODPointRangeRingSteps(3, &m_dODPointRangeRingSteps, wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<double> dODPointArrivalRadius(3, &m_dODPointArrivalRadius, wxNUM_VAL_DEFAULT);
    dODPointRangeRingSteps.SetMin(0);
    dODPointArrivalRadius.SetMin(0);
    m_textCtrlODPointRangeRingsSteps->SetValidator( dODPointRangeRingSteps );
    m_textCtrlODPointArrivalRadius->SetValidator( dODPointArrivalRadius );
#endif // not defined _WXMSW_ 
#endif // wxCHECK_VERSION(3,0,0)
    
    // add unsuported wxOwnerDrawnComboBox combo box as it handles scrolling better
    m_bODIComboBoxODPointIconName = new ODIconCombo( m_panelBasicProperties, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY );
    m_bODIComboBoxODPointIconName->SetPopupMaxHeight(::wxGetDisplaySize().y / 2);
    
    //  Accomodate scaling of icon
    int min_size = GetCharHeight() * 2;
    min_size = wxMax( min_size, (32 * GetOCPNChartScaleFactor_Plugin()) + 4 );
    m_bODIComboBoxODPointIconName->SetMinSize( wxSize(-1, min_size) );
    m_fgSizerNameIcon->Replace(m_bcomboBoxODPointIconName, m_bODIComboBoxODPointIconName );
    
    delete m_bcomboBoxODPointIconName;
    
    SetDialogSize();
    
}

ODPointPropertiesImpl::~ODPointPropertiesImpl()
{
    this->Disconnect( ID_RCLK_MENU_COPY, wxEVT_COMMAND_MENU_SELECTED,
                   wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    this->Disconnect( ID_RCLK_MENU_COPY_LL, wxEVT_COMMAND_MENU_SELECTED,
                   wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    this->Disconnect( ID_RCLK_MENU_PASTE, wxEVT_COMMAND_MENU_SELECTED,
                   wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    this->Disconnect( ID_RCLK_MENU_PASTE_LL, wxEVT_COMMAND_MENU_SELECTED,
                   wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
}

void ODPointPropertiesImpl::SetDialogSize( void )
{
    //m_bSizerFill->RecalcSizes();
/*    
    wxSize sz = m_SizerDialogBox->CalcMin();
    sz.IncBy( 20 );   // Account for some decorations?
    wxSize dsize = ::wxGetDisplaySize();
    sz.y = wxMin(sz.y, dsize.y-80);
    SetClientSize(sz);
    m_defaultClientSize = sz;
    //m_panelBasicProperties->SetScrollRate(5, 5);

    wxSize fsize = GetSize();
    fsize.y = wxMin(fsize.y, dsize.y-80);
    fsize.x = wxMin(fsize.x, dsize.x-80);
    SetSize(fsize);
*/    

    m_SizerBasicProperties->Layout();
    m_bSizerOuterProperties->Layout();
    m_SizerODPointRangeRingsSelect->Layout();
    m_SizerDialogBox->Layout();
    this->GetSizer()->Fit( this );
    this->Layout();
}

void ODPointPropertiesImpl::onRightClick( wxMouseEvent& event )
{
    wxMenu* popup = new wxMenu();
    popup->Append( ID_RCLK_MENU_COPY, _("Copy") );
    popup->Append( ID_RCLK_MENU_COPY_LL, _("Copy lat/long") );
    popup->Append( ID_RCLK_MENU_PASTE, _("Paste") );
    popup->Append( ID_RCLK_MENU_PASTE_LL, _("Paste lat/long") );
    m_contextObject = event.GetEventObject();
    PopupMenu( popup );
    delete popup;
    
}

void ODPointPropertiesImpl::OnPositionCtlUpdated( wxCommandEvent& event )
{
    double lat = fromDMM_Plugin( m_textLatitude->GetValue() );
    double lon = fromDMM_Plugin( m_textLongitude->GetValue() );

    if( !m_pODPoint->m_bIsInLayer ) {
        m_pODPoint->SetPosition( lat, lon );
        g_pODSelect->ModifySelectablePoint( lat, lon, (void *) m_pODPoint, SELTYPE_ODPOINT );
    }

    // Update the mark position dynamically
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
}

void ODPointPropertiesImpl::OnArrivalRadiusChange( wxCommandEvent& event )
{
    // TODO: Implement OnArrivalRadiusChange
}

void ODPointPropertiesImpl::OnShowRangeRingsSelect( wxCommandEvent& event )
{
    // TODO: Implement OnShowRangeRingsSelect
}

void ODPointPropertiesImpl::OnRangeRingsStepChange( wxCommandEvent& event )
{
    // TODO: Implement OnRangeRingsStepChange
}

void ODPointPropertiesImpl::OnDescChangedBasic( wxCommandEvent& event )
{
    // TODO: Implement OnDescChangedBasic
}

void ODPointPropertiesImpl::OnComboboxSelected( wxCommandEvent& event )
{
}

void ODPointPropertiesImpl::OnButtonClickFonts( wxCommandEvent& event )
{
    if(m_pfdDialog) delete m_pfdDialog;

    wxFontData l_FontData;
    l_FontData.SetInitialFont( m_pTextPoint->m_DisplayTextFont );
    m_pfdDialog = new wxFontDialog( this, l_FontData );
    m_pfdDialog->Centre( wxBOTH );

    int iRet = m_pfdDialog->ShowModal();
    if(iRet == wxID_OK) {
        m_staticTextFontFaceExample->SetFont(m_pfdDialog->GetFontData().GetChosenFont());
        SetDialogSize();
    }
}

void ODPointPropertiesImpl::OnPointPropertiesOKClick( wxCommandEvent& event )
{
    if( m_pODPoint ) {
        m_pODPoint->m_bIsBeingEdited = FALSE;
        m_pODPoint->m_bPointPropertiesBlink = false;
        m_pODPoint->m_bPtIsSelected = false;
        SaveChanges(); // write changes to globals and update config
        OnPositionCtlUpdated( event );
    }
    Show( false );

    if( g_pPathManagerDialog && g_pPathManagerDialog->IsShown() )
        g_pPathManagerDialog->UpdateODPointsListCtrl();
        
    if( g_pODPathPropDialog && g_pODPathPropDialog->IsShown() )
        g_pODPathPropDialog->UpdateProperties(  );

    SetClientSize(m_defaultClientSize);
    
    g_ocpn_draw_pi->m_pFoundODPoint = NULL;
    m_notebookProperties->ChangeSelection( 0 );
    m_notebookProperties->Refresh();
    
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    
    event.Skip();
}

void ODPointPropertiesImpl::OnPointPropertiesClose( wxCloseEvent& event )
{
    wxCommandEvent nullEvent;
    OnPointPropertiesCancelClick( nullEvent );
}

void ODPointPropertiesImpl::OnPointPropertiesCancelClick( wxCommandEvent& event )
{
    if( m_pODPoint ) {
        m_pODPoint->m_bIsBeingEdited = FALSE;
        m_pODPoint->m_bPointPropertiesBlink = false;
        m_pODPoint->m_bPtIsSelected = false;
        m_pODPoint->SetVisible( m_bIsVisible_save );
        m_pODPoint->SetNameShown( m_bShowName_save );
        m_pODPoint->SetPosition( m_lat_save, m_lon_save );
        m_pODPoint->SetIconName( m_IconName_save );
        m_pODPoint->ReLoadIcon();
        m_pODPoint->m_HyperlinkList->Clear();
    }

    Show( false );
    SetClientSize(m_defaultClientSize);

    g_ocpn_draw_pi->m_pFoundODPoint = NULL;
    m_notebookProperties->ChangeSelection( 0 );
    m_notebookProperties->Refresh();

    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    
    event.Skip();
}

void ODPointPropertiesImpl::OnRadioBoxPointType(wxCommandEvent& event)
{
    switch (m_radioBoxBoundaryPointType->GetSelection()) {
        case ID_BOUNDARY_EXCLUSION:
        case ID_BOUNDARY_NIETHER:
            m_sliderBoundaryPointInclusionSize->Disable();
            break;
        case ID_BOUNDARY_INCLUSION:
            m_sliderBoundaryPointInclusionSize->Enable();
            break;
    }
    
    ODPointPropertiesDialog::OnRadioBoxPointType(event);
}


void ODPointPropertiesImpl::SaveChanges()
{
    if( m_pODPoint ) {
        if( m_pODPoint->m_bIsInLayer ) return;

        // Get User input Text Fields
        m_pODPoint->m_iODPointRangeRingsNumber = m_choicePointRangeRingsNumber->GetSelection();
        m_pODPoint->m_fODPointRangeRingsStep = atof( m_textCtrlODPointRangeRingsSteps->GetValue().mb_str() );
//        m_pODPoint->m_fODPointRangeRingsStep = m_RangeRingSteps;
        m_pODPoint->m_iODPointRangeRingsStepUnits = m_choiceDistanceUnitsString->GetSelection();
        m_pODPoint->m_wxcODPointRangeRingsColour = m_colourPickerRangeRingsColour->GetColour();
        m_pODPoint->CreateColourSchemes();

        m_pODPoint->SetName( m_textName->GetValue() );
        m_pODPoint->SetODPointArrivalRadius( m_textCtrlODPointArrivalRadius->GetValue() );
        m_pODPoint->SetShowODPointRangeRings( m_checkBoxShowODPointRangeRings->GetValue() );
        m_pODPoint->m_ODPointDescription = m_textDescription->GetValue();
        if(m_pODPoint->m_sTypeString == wxT("Text Point")) {
            m_pTextPoint->m_TextPointText = m_textDisplayText->GetValue();
            m_pTextPoint->m_bTextChanged = true;
            m_pTextPoint->m_iTextPosition = m_choicePosition->GetSelection();
            m_pTextPoint->m_colourTextColour = m_colourPickerText->GetColour();
            m_pTextPoint->m_colourTextBackgroundColour = m_colourPickerBacgroundColour->GetColour();
            m_pTextPoint->m_iBackgroundTransparency = m_sliderBackgroundTransparency->GetValue();
            if(m_pfdDialog){
                wxFontData twxfdData = m_pfdDialog->GetFontData();
                wxFont twxfFont = twxfdData.GetChosenFont();
                m_pTextPoint->m_DisplayTextFont = twxfdData.m_chosenFont;
            }
            m_pTextPoint->m_iDisplayTextWhen = m_radioBoxShowDisplayText->GetSelection();
        } else if(m_pODPoint->m_sTypeString == wxT("Boundary Point")){
            m_pBoundaryPoint->m_uiBoundaryPointFillTransparency = m_sliderBoundaryPointFillTransparency->GetValue();
            m_pBoundaryPoint->m_iInclusionBoundaryPointSize = m_sliderBoundaryPointInclusionSize->GetValue();
            m_pBoundaryPoint->m_iRangeRingStyle = ::StyleValues[m_choiceRangeRingLineStyle->GetSelection()];
            m_pBoundaryPoint->m_iRangeRingWidth = ::WidthValues[m_choiceRangeRingLineWidth->GetSelection()];
            int l_BoundaryPointType;
            l_BoundaryPointType = m_radioBoxBoundaryPointType->GetSelection();
            switch (l_BoundaryPointType) {
                case ID_BOUNDARY_EXCLUSION:
                    m_pBoundaryPoint->m_bExclusionBoundaryPoint = true;
                    m_pBoundaryPoint->m_bInclusionBoundaryPoint = false;
                    break;
                case ID_BOUNDARY_INCLUSION:
                    m_pBoundaryPoint->m_bExclusionBoundaryPoint = false;
                    m_pBoundaryPoint->m_bInclusionBoundaryPoint = true;
                    break;
                case ID_BOUNDARY_NIETHER:
                    m_pBoundaryPoint->m_bExclusionBoundaryPoint = false;
                    m_pBoundaryPoint->m_bInclusionBoundaryPoint = false;
                    break;
                default:
                    m_pBoundaryPoint->m_bExclusionBoundaryPoint = true;
                    m_pBoundaryPoint->m_bInclusionBoundaryPoint = false;
                    break;
            }
            
            
        }
        m_pODPoint->SetVisible( m_checkBoxVisible->GetValue() );
        m_pODPoint->SetNameShown( m_checkBoxShowName->GetValue() );
        if(m_pODPoint->m_sTypeString == wxT("Guard Zone Point")) {
            if(m_text_lat != m_textLatitude->GetValue() || m_text_lon != m_textLongitude->GetValue()) {
                m_pODPoint->SetPosition( fromDMM_Plugin( m_textLatitude->GetValue() ), fromDMM_Plugin( m_textLongitude->GetValue() ) );
                wxArrayPtrVoid *l_array = g_pPathMan->GetPathArrayContaining(m_pODPoint);
                if (l_array) {
                    GZ *l_gz = (GZ *)l_array->Item(0);
                    l_gz->UpdateGZ(m_pODPoint);
                    delete l_array;
                }
            }
        } else
            m_pODPoint->SetPosition( fromDMM_Plugin( m_textLatitude->GetValue() ), fromDMM_Plugin( m_textLongitude->GetValue() ) );
        m_pODPoint->SetIconName( m_bODIComboBoxODPointIconName->GetValue() );
        m_pODPoint->ReLoadIcon();
        m_pODPoint->CreateColourSchemes();

        // Here is some logic....
        // If the Markname is completely numeric, and is part of a route,
        // Then declare it to be of attribute m_bDynamicName = true
        // This is later used for re-numbering points on actions like
        // Insert Point, Delete Point, Append Point, etc

        if( m_pODPoint->m_bIsInPath ) {
            bool b_name_is_numeric = true;
            for( unsigned int i = 0; i < m_pODPoint->GetName().Len(); i++ ) {
                if( wxChar( '0' ) > m_pODPoint->GetName()[i] ) b_name_is_numeric = false;
                if( wxChar( '9' ) < m_pODPoint->GetName()[i] ) b_name_is_numeric = false;
            }

            m_pODPoint->m_bDynamicName = b_name_is_numeric;
        } else
            m_pODPoint->m_bDynamicName = false;

        if( m_pODPoint->m_bIsInPath ) {
            // Update the Path segment selectables
            g_pODSelect->UpdateSelectablePathSegments( m_pODPoint );

            // Get an array of all paths using this point
            wxArrayPtrVoid *pEditPathArray = g_pPathMan->GetPathArrayContaining( m_pODPoint );

            if( pEditPathArray ) {
                for( unsigned int ip = 0; ip < pEditPathArray->GetCount(); ip++ ) {
                    ODPath *pp = (ODPath *) pEditPathArray->Item( ip );
                    if(pp->m_sTypeString == wxT("Guard Zone")) {
                        GZ * l_GZ = (GZ *)pp;
                        l_GZ->UpdateGZ( m_pODPoint );
                    }
                    pp->FinalizeForRendering();
                    pp->UpdateSegmentDistances();

                    if(m_checkBoxChangeAllPointIcons->IsChecked()) {
                        wxODPointListNode *pnode = ( pp->m_pODPointList )->GetFirst();
                        while( pnode ) {
                            ODPoint *pop = pnode->GetData();
                            pop->SetIconName( m_bODIComboBoxODPointIconName->GetValue() );
                            pop->ReLoadIcon();
                            pnode = pnode->GetNext();
                        }
                    }
                    
                    g_pODConfig->UpdatePath( pp );
                }
                delete pEditPathArray;
            }
        } else
            g_pODConfig->UpdateODPoint( m_pODPoint );

            // No general settings need be saved pConfig->UpdateSettings();
    }
    return;
}

void ODPointPropertiesImpl::SetODPoint( ODPoint *pOP )
{
    if( m_pODPoint ) {
        m_pODPoint->m_bIsBeingEdited = FALSE;
        m_pODPoint->m_bPtIsSelected = FALSE;
        m_pODPoint->m_bPointPropertiesBlink = false;
    }
    if(pOP->m_sTypeString == wxT("Text Point")) {
        m_pTextPoint = (TextPoint *)pOP;
        m_pODPoint = m_pTextPoint;
    } else if(pOP->m_sTypeString == wxT("Boundary Point")) {
        m_pBoundaryPoint = (BoundaryPoint *)pOP;
        m_pODPoint = m_pBoundaryPoint;
    } else {
        m_pODPoint = pOP;
    }
    
    if( m_pODPoint ) {
        m_pODPoint->m_bIsBeingEdited = TRUE;
        m_pODPoint->m_bPointPropertiesBlink = true;
        m_lat_save = m_pODPoint->m_lat;
        m_lon_save = m_pODPoint->m_lon;
        m_IconName_save = m_pODPoint->GetIconName();
        m_bShowName_save = m_pODPoint->m_bShowName;
        m_bIsVisible_save = m_pODPoint->m_bIsVisible;
        RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    }
}

bool ODPointPropertiesImpl::UpdateProperties( bool positionOnly )
{
    if( m_pODPoint ) {
        if(m_pODPoint->m_sTypeString == wxT("Text Point")) {
            m_panelDisplayText->Enable( true );
            m_panelDisplayText->Show();
            m_radioBoxBoundaryPointType->Enable( false );
            m_radioBoxBoundaryPointType->Hide();
            m_staticTextBoundaryPointInclusionSize->Hide();
            m_sliderBoundaryPointInclusionSize->Enable( false );
            m_sliderBoundaryPointInclusionSize->Hide();
            m_staticTextFillDensity->Hide();
            m_sliderBoundaryPointFillTransparency->Enable( false );
            m_sliderBoundaryPointFillTransparency->Hide();
            m_bSizerOuterProperties->Hide( m_bSizerFill );
        } else if (m_pODPoint->m_sTypeString == wxT("Boundary Point")) {
            m_panelDisplayText->Enable( false );
            m_panelDisplayText->Hide();
            m_radioBoxBoundaryPointType->Enable( true );
            m_radioBoxBoundaryPointType->Show();
            m_bSizerOuterProperties->Show( m_bSizerFill );
            m_staticTextBoundaryPointInclusionSize->Show();
            m_sliderBoundaryPointInclusionSize->Enable( true );
            m_sliderBoundaryPointInclusionSize->Show();
            m_staticTextFillDensity->Show();
            m_sliderBoundaryPointFillTransparency->Enable( true );
            m_sliderBoundaryPointFillTransparency->Show();
        } else if (m_pODPoint->m_sTypeString == wxT("EBL Point") || m_pODPoint->m_sTypeString == wxT("DR Point") || m_pODPoint->m_sTypeString == wxT("Guard Zone Point") || m_pODPoint->m_sTypeString == wxT("PIL Point")) {
            m_panelDisplayText->Enable( false );
            m_panelDisplayText->Hide();
            m_radioBoxBoundaryPointType->Enable( false );
            m_radioBoxBoundaryPointType->Hide();
            m_staticTextBoundaryPointInclusionSize->Hide();
            m_sliderBoundaryPointInclusionSize->Enable( false );
            m_sliderBoundaryPointInclusionSize->Hide();
            m_staticTextFillDensity->Hide();
            m_sliderBoundaryPointFillTransparency->Enable( false );
            m_sliderBoundaryPointFillTransparency->Hide();
            m_bSizerOuterProperties->Hide( m_bSizerFill );
        }
        
        m_text_lat = toSDMM_PlugIn( 1, m_pODPoint->m_lat );
        m_text_lon = toSDMM_PlugIn( 2, m_pODPoint->m_lon );
        m_textLatitude->SetValue( toSDMM_PlugIn( 1, m_pODPoint->m_lat ) );
        m_textLongitude->SetValue( toSDMM_PlugIn( 2, m_pODPoint->m_lon ) );
        m_lat_save = m_pODPoint->m_lat;
        m_lon_save = m_pODPoint->m_lon;

        if( positionOnly ) return true;

        //Layer or not?
        if( m_pODPoint->m_bIsInLayer ) {
            m_textName->SetEditable( false );
            m_textDescription->SetEditable( false );
            m_textDisplayText->SetEditable( false );
            m_textLatitude->SetEditable( false );
            m_textLongitude->SetEditable( false );
            m_bODIComboBoxODPointIconName->Enable( false );
            m_checkBoxShowName->Enable( false );
            m_checkBoxVisible->Enable( false );
            m_checkBoxChangeAllPointIcons->Enable( false );
            m_textCtrlODPointArrivalRadius->SetEditable ( false );
            m_checkBoxShowODPointRangeRings->Enable( false );
            m_choiceDistanceUnitsString->Enable( false );
            m_choicePointRangeRingsNumber->Enable( false );
            m_textCtrlODPointRangeRingsSteps->SetEditable( false );
            m_colourPickerRangeRingsColour->Enable( false );
        } else {
            m_textName->SetEditable( true );
            m_textDescription->SetEditable( true );
            m_textDisplayText->SetEditable( true );
            m_textLatitude->SetEditable( true );
            m_textLongitude->SetEditable( true );
            m_bODIComboBoxODPointIconName->Enable( true );
            m_checkBoxShowName->Enable( true );
            m_checkBoxVisible->Enable( true );
            m_textCtrlODPointArrivalRadius->SetEditable ( true );
            m_checkBoxShowODPointRangeRings->Enable( true );
            m_choiceDistanceUnitsString->Enable( true );
            m_choicePointRangeRingsNumber->Enable( true );
            m_textCtrlODPointRangeRingsSteps->SetEditable( true );
            m_colourPickerRangeRingsColour->Enable( true );
        }
        m_textName->SetValue( m_pODPoint->GetName() );

        wxString s_ArrivalRadius;
        s_ArrivalRadius.Printf( _T("%.3f"), m_pODPoint->GetODPointArrivalRadius() );
        m_textCtrlODPointArrivalRadius->SetValue( s_ArrivalRadius );        
        
        m_textDescription->SetValue( m_pODPoint->m_ODPointDescription );
        if(m_pODPoint->m_sTypeString == wxT("Text Point")) {
            m_textDisplayText->Clear();
            m_textDisplayText->SetValue( m_pTextPoint->m_TextPointText );
            m_choicePosition->SetSelection( m_pTextPoint->m_iTextPosition );
            m_colourPickerText->SetColour( m_pTextPoint->m_colourTextColour );
            m_colourPickerBacgroundColour->SetColour( m_pTextPoint->m_colourTextBackgroundColour );
            m_sliderBackgroundTransparency->SetValue( m_pTextPoint->m_iBackgroundTransparency );
            m_staticTextFontFaceExample->SetFont( m_pTextPoint->m_DisplayTextFont );
            m_radioBoxShowDisplayText->SetSelection( m_pTextPoint->m_iDisplayTextWhen );
        } else if(m_pODPoint->m_sTypeString == wxT("Boundary Point")) {
            if( m_pBoundaryPoint->m_bExclusionBoundaryPoint && !m_pBoundaryPoint->m_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_EXCLUSION );
            else if( !m_pBoundaryPoint->m_bExclusionBoundaryPoint && m_pBoundaryPoint->m_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_INCLUSION );
            else if( !m_pBoundaryPoint->m_bExclusionBoundaryPoint && !m_pBoundaryPoint->m_bInclusionBoundaryPoint ) m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_NIETHER );
            else m_radioBoxBoundaryPointType->SetSelection( ID_BOUNDARY_EXCLUSION );
            m_sliderBoundaryPointInclusionSize->SetValue( m_pBoundaryPoint->m_iInclusionBoundaryPointSize );
            m_sliderBoundaryPointFillTransparency->SetValue( m_pBoundaryPoint->m_uiBoundaryPointFillTransparency );
            for( unsigned int i = 0; i < sizeof( ::WidthValues ) / sizeof(int); i++ ) {
                if( m_pBoundaryPoint->m_iRangeRingWidth == ::WidthValues[i] ) {
                    m_choiceRangeRingLineWidth->SetSelection( i );
                    break;
                }
            }
            for( unsigned int i = 0; i < sizeof( ::StyleValues ) / sizeof(int); i++ ) {
                if( m_pBoundaryPoint->m_iRangeRingStyle == ::StyleValues[i] )
                    m_choiceRangeRingLineStyle->SetSelection( i );
            }
            if(!m_pBoundaryPoint->m_bExclusionBoundaryPoint && m_pBoundaryPoint->m_bInclusionBoundaryPoint)
                m_sliderBoundaryPointInclusionSize->Enable();
            else
                m_sliderBoundaryPointInclusionSize->Disable();
        }
        
        m_checkBoxShowName->SetValue( m_pODPoint->m_bShowName );
        m_checkBoxVisible->SetValue( m_pODPoint->m_bIsVisible );
        m_textCtrlGuid->SetValue( m_pODPoint->m_GUID );
        m_checkBoxShowODPointRangeRings->SetValue( m_pODPoint->GetShowODPointRangeRings() );
        m_choicePointRangeRingsNumber->SetSelection( m_pODPoint->GetODPointRangeRingsNumber() );
        m_choiceDistanceUnitsString->SetSelection( m_pODPoint->GetODPointRangeRingsStepUnits() );
        wxString buf;
        buf.Printf( _T("%.3f" ), m_pODPoint->GetODPointRangeRingsStep() );
        m_textCtrlODPointRangeRingsSteps->SetValue( buf );
//        m_RangeRingSteps = m_pODPoint->GetODPointRangeRingsStep();
        m_colourPickerRangeRingsColour->SetColour( m_pODPoint->GetODPointRangeRingsColour() );
        

        m_bODIComboBoxODPointIconName->Clear();
        //      Iterate on the Icon Descriptions, filling in the combo control
        bool fillCombo = m_bODIComboBoxODPointIconName->GetCount() == 0;
        wxImageList *icons = g_pODPointMan->Getpmarkicon_image_list();

        if( fillCombo  && icons){
            for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
                wxString *ps = g_pODPointMan->GetIconDescription( i );
                m_bODIComboBoxODPointIconName->Append( *ps, icons->GetBitmap( i ) );
            }
        }
        
        // find the correct item in the combo box
        int iconToSelect = -1;
        for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
            if( *g_pODPointMan->GetIconName( i ) == m_pODPoint->GetIconName() ) {
                iconToSelect = i;
                break;
            }
        }
        if( iconToSelect == -1 ) {
            for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
                if( *g_pODPointMan->GetIconDescription( i ) == m_pODPoint->GetIconName() ) {
                    iconToSelect = i;
                    break;
                }
            }
        }
        
        //  not found, so add  it to the list, with a generic bitmap and using the name as description
        // n.b.  This should never happen...
        if( -1 == iconToSelect){    
            m_bODIComboBoxODPointIconName->Append( m_pODPoint->GetIconName(), icons->GetBitmap( 0 ) );
            iconToSelect = m_bODIComboBoxODPointIconName->GetCount() - 1;
        }
        
        m_bODIComboBoxODPointIconName->SetSelection( iconToSelect );
        
        //    Get an array of all paths using this point
        wxArrayPtrVoid *ppath_array = g_pPathMan->GetPathArrayContaining( m_pODPoint );
        if( ppath_array ) {
            ODPath *path = (ODPath *)ppath_array->front();
            if(path->m_sTypeString == wxT("EBL"))
                m_checkBoxChangeAllPointIcons->Disable();
            else
                m_checkBoxChangeAllPointIcons->Enable();
            delete ppath_array;
        } else {
            m_checkBoxChangeAllPointIcons->Disable();
        }
        m_checkBoxChangeAllPointIcons->SetValue( false );
        
        icons = NULL;
        
        wxString caption( wxS("") );
        if(m_pODPoint->m_bIsInLayer) {
            if ( m_pODPoint->GetTypeString().IsNull() || m_pODPoint->GetTypeString().IsEmpty() )
                caption.append( _("OCPN Draw Point, Layer: ") );
            else if(m_pODPoint->m_sTypeString == wxT("Boundary Point"))
                caption.append(_("Boundary Point Properties, Layer: "));
            else if(m_pODPoint->m_sTypeString == wxT("EBL Point"))
                caption.append(_("EBL Point Properties, Layer: "));
            else if(m_pODPoint->m_sTypeString == wxT("DR Point"))
                caption.append(_("DR Point Properties, Layer: "));
#if wxCHECK_VERSION(3,0,0)
            caption.Append( _(g_pPathManagerDialog->GetLayerName( m_pODPoint->m_LayerID )) );
#else
            caption.Append( g_pPathManagerDialog->GetLayerName( m_pODPoint->m_LayerID ) );
#endif
        } else {
            if ( m_pODPoint->GetTypeString().IsNull() || m_pODPoint->GetTypeString().IsEmpty() )
                caption.append( _("OCPN Draw Point") );
            else if(m_pODPoint->m_sTypeString == wxT("Boundary Point"))
                caption.append(_("Boundary Point Properties"));
            else if(m_pODPoint->m_sTypeString == wxT("EBL Point"))
                caption.append(_("EBL Point Properties"));
            else if(m_pODPoint->m_sTypeString == wxT("DR Point"))
                caption.append(_("DR Point Properties"));
            else if(m_pODPoint->m_sTypeString == wxT("Guard Zone Point"))
                caption.append(_("Guard Zone Point Properties"));
        }
        SetTitle( caption );
        
        m_notebookProperties->SetSelection(1);
        m_notebookProperties->SetSelection(0);
    }
    SetDialogSize();
    
    return true;
}

void ODPointPropertiesImpl::OnCopyPasteLatLon( wxCommandEvent& event )
{
    // Fetch the control values, convert to degrees
    double lat = fromDMM_Plugin( m_textLatitude->GetValue() );
    double lon = fromDMM_Plugin( m_textLongitude->GetValue() );

    wxString result;

    switch( event.GetId() ) {
        case ID_RCLK_MENU_PASTE: {
            if( wxTheClipboard->Open() ) {
                wxTextDataObject data;
                wxTheClipboard->GetData( data );
                result = data.GetText();
                ((wxTextCtrl*)m_contextObject)->SetValue( result );
                wxTheClipboard->Close();
            }
            return;
        }
        case ID_RCLK_MENU_PASTE_LL: {
            if( wxTheClipboard->Open() ) {
                wxTextDataObject data;
                wxTheClipboard->GetData( data );
                result = data.GetText();

                PositionParser pparse( result );

                if( pparse.IsOk() ) {
                    m_textLatitude->SetValue( pparse.GetLatitudeString() );
                    m_textLongitude->SetValue( pparse.GetLongitudeString() );
                }
                wxTheClipboard->Close();
            }
            return;
        }
        case ID_RCLK_MENU_COPY: {
            result = ((wxTextCtrl*)m_contextObject)->GetValue();
            break;
        }
        case ID_RCLK_MENU_COPY_LL: {
            result << toSDMM_PlugIn( 1, lat, true ) <<_T('\t');
            result << toSDMM_PlugIn( 2, lon, true );
            break;
        }
    }

    if( wxTheClipboard->Open() ) {
        wxTextDataObject* data = new wxTextDataObject;
        data->SetText( result );
        wxTheClipboard->SetData( data );
        wxTheClipboard->Close();
    }
}

void ODPointPropertiesImpl::ValidateMark( void )
{
    //    Look in the master list of ODPoints to see if the currently selected ODPpoint is still valid
    //    It may have been deleted as part of a route
    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();
    
    bool b_found = false;
    while( node ) {
        ODPoint *op = node->GetData();
        if( m_pODPoint == op ) {
            b_found = true;
            break;
        }
        
        node = node->GetNext();
    }
    
    if( !b_found ) m_pODPoint = NULL;
}

