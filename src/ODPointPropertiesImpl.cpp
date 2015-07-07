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
#include "ODPoint.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "PathProp.h"
#include "PointMan.h"
#include "ODPositionParser.h"
#include <wx/clipbrd.h>
#include <wx/menu.h>
#include <wx/window.h>

extern ODSelect             *g_pODSelect;
extern ocpn_draw_pi         *g_ocpn_draw_pi;
extern PointMan             *g_pODPointMan;
extern PathMan              *g_pPathMan;
extern ODConfig             *g_pODConfig;
extern PathManagerDialog    *g_pPathManagerDialog;
extern PathProp             *g_pPathPropDialog;
extern int                  g_iTextPosition;


ODPointPropertiesImpl::ODPointPropertiesImpl( wxWindow* parent )
:
ODPointPropertiesDialog( parent )
{
    this->Connect( ID_RCLK_MENU_COPY, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    this->Connect( ID_RCLK_MENU_COPY_LL, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    this->Connect( ID_RCLK_MENU_PASTE, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    this->Connect( ID_RCLK_MENU_PASTE_LL, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler( ODPointPropertiesImpl::OnCopyPasteLatLon ) );
    
    DimeWindow( this );

    // TODO check if wxFrameBuilder supports BitmapComboBox. This code is to handle the case when it does not
    m_pODPoint = NULL;

}

void ODPointPropertiesImpl::SetDialogSize( void )
{
    wxSize sz = m_SizerDialogBox->CalcMin();
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
        g_pODSelect->ModifySelectablePoint( lat, lon, (void *) m_pODPoint, SELTYPE_OCPNPOINT );
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
/*    if( m_checkBoxShowODPointRangeRings->GetValue() == FALSE ) {
        m_SizerODPointRangeRingsSelect->ShowItems( false );
        m_SizerPointRangeGrid->ShowItems( false );
    } else {
        m_SizerODPointRangeRingsSelect->ShowItems( true );
        if ( m_choicePointRangeRingsNumber->GetSelection() != 0 ) 
            m_SizerPointRangeGrid->ShowItems( true );
    }
    m_SizerBasicProperties->Layout();
    RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    event.Skip();
*/    
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
    m_bitmapPointBitmap->SetBitmap( m_bcomboBoxODPointIconName->GetItemBitmap( m_bcomboBoxODPointIconName->GetSelection() ) );
}

void ODPointPropertiesImpl::OnPointPropertiesOKClick( wxCommandEvent& event )
{
    if( m_pODPoint ) {
        m_pODPoint->m_bIsBeingEdited = FALSE;
        m_pODPoint->m_iBlink--;
        m_pODPoint->m_bPtIsSelected = false;
        if( m_pODPoint->m_iBlink < 0 ) m_pODPoint->m_iBlink = 0;
        SaveChanges(); // write changes to globals and update config
        OnPositionCtlUpdated( event );
    }
    Show( false );

    if( g_pPathManagerDialog && g_pPathManagerDialog->IsShown() )
        g_pPathManagerDialog->UpdateODPointsListCtrl();
        
    if( g_pPathPropDialog && g_pPathPropDialog->IsShown() )
        g_pPathPropDialog->UpdateProperties();

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
        m_pODPoint->m_iBlink--;
        m_pODPoint->m_bPtIsSelected = false;
        if( m_pODPoint->m_iBlink < 0 ) m_pODPoint->m_iBlink = 0;
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

void ODPointPropertiesImpl::SaveChanges()
{
    if( m_pODPoint ) {
        if( m_pODPoint->m_bIsInLayer ) return;

        // Get User input Text Fields
        m_pODPoint->m_iODPointRangeRingsNumber = m_choicePointRangeRingsNumber->GetSelection();
        m_pODPoint->m_fODPointRangeRingsStep = atof( m_textCtrlPointRangeRingsSteps->GetValue().mb_str() );
        m_pODPoint->m_iODPointRangeRingsStepUnits = m_choiceDistanceUnitsString->GetSelection();
        m_pODPoint->m_wxcODPointRangeRingsColour = m_colourPickerRangeRingsColour->GetColour();
        m_pODPoint->SetName( m_textName->GetValue() );
        m_pODPoint->SetODPointArrivalRadius( m_textArrivalRadius->GetValue() );
        m_pODPoint->SetShowODPointRangeRings( m_checkBoxShowODPointRangeRings->GetValue() );
        m_pODPoint->m_MarkDescription = m_textDescription->GetValue();
        if(m_pODPoint->m_sTypeString == wxT("Text Point")) {
            m_pTextPoint->m_TextPointText = m_textCtrlExtDescription->GetValue();
            m_pTextPoint->m_iTextPosition = m_choicePosition->GetSelection();
            m_pTextPoint->m_colourTextColour = m_colourPickerText->GetColour();
            m_pTextPoint->m_colourTextBackgroundColour = m_colourPickerBacgroundColour->GetColour();
            m_pTextPoint->m_iBackgroundTransparency = m_sliderBackgroundTransparency->GetValue();
        } else if(m_pODPoint->m_sTypeString == wxT("Boundary Point")){
            m_pBoundaryPoint->m_bFill = m_checkBoxFill->GetValue();
        }
        m_pODPoint->SetVisible( m_checkBoxVisible->GetValue() );
        m_pODPoint->SetNameShown( m_checkBoxShowName->GetValue() );
        m_pODPoint->SetPosition( fromDMM_Plugin( m_textLatitude->GetValue() ), fromDMM_Plugin( m_textLongitude->GetValue() ) );
        m_pODPoint->SetIconName( m_bcomboBoxODPointIconName->GetValue() );
        m_pODPoint->ReLoadIcon();

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
                    Path *pp = (Path *) pEditPathArray->Item( ip );
                    pp->FinalizeForRendering();
                    pp->UpdateSegmentDistances();

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
        m_pODPoint->m_iBlink--;
        if( m_pODPoint->m_iBlink < 0 ) m_pODPoint->m_iBlink = 0;
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
        m_pODPoint->m_iBlink++;
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
        m_textLatitude->SetValue( toSDMM_PlugIn( 1, m_pODPoint->m_lat ) );
        m_textLongitude->SetValue( toSDMM_PlugIn( 2, m_pODPoint->m_lon ) );
        m_lat_save = m_pODPoint->m_lat;
        m_lon_save = m_pODPoint->m_lon;

        if( positionOnly ) return true;

        //Layer or not?
        if( m_pODPoint->m_bIsInLayer ) {
            m_textName->SetEditable( false );
            m_textDescription->SetEditable( false );
            m_textCtrlExtDescription->SetEditable( false );
            m_textLatitude->SetEditable( false );
            m_textLongitude->SetEditable( false );
            m_bcomboBoxODPointIconName->Enable( false );
            m_checkBoxShowName->Enable( false );
            m_checkBoxVisible->Enable( false );
            m_textArrivalRadius->SetEditable ( false );
            m_checkBoxShowODPointRangeRings->Enable( false );
            m_choiceDistanceUnitsString->Enable( false );
            m_choicePointRangeRingsNumber->Enable( false );
            m_textCtrlPointRangeRingsSteps->SetEditable( false );
            m_colourPickerRangeRingsColour->Enable( false );
        } else {
            m_textName->SetEditable( true );
            m_textDescription->SetEditable( true );
            m_textCtrlExtDescription->SetEditable( true );
            m_textLatitude->SetEditable( true );
            m_textLongitude->SetEditable( true );
            m_bcomboBoxODPointIconName->Enable( true );
            m_checkBoxShowName->Enable( true );
            m_checkBoxVisible->Enable( true );
            m_textArrivalRadius->SetEditable ( true );
            m_checkBoxShowODPointRangeRings->Enable( true );
            m_choiceDistanceUnitsString->Enable( true );
            m_choicePointRangeRingsNumber->Enable( true );
            m_textCtrlPointRangeRingsSteps->SetEditable( true );
            m_colourPickerRangeRingsColour->Enable( true );
        }
        m_textName->SetValue( m_pODPoint->GetName() );

        wxString s_ArrivalRadius;
        s_ArrivalRadius.Printf( _T("%.3f"), m_pODPoint->GetODPointArrivalRadius() );
        m_textArrivalRadius->SetValue( s_ArrivalRadius );        
        
        m_textDescription->SetValue( m_pODPoint->m_MarkDescription );
        if(m_pODPoint->m_sTypeString == wxT("Text Point")) {
            m_textCtrlExtDescription->Clear();
            m_textCtrlExtDescription->SetValue( m_pTextPoint->m_TextPointText );
            m_choicePosition->SetSelection( m_pTextPoint->m_iTextPosition );
            m_colourPickerText->SetColour( m_pTextPoint->m_colourTextColour );
            m_colourPickerBacgroundColour->SetColour( m_pTextPoint->m_colourTextBackgroundColour );
            m_sliderBackgroundTransparency->SetValue( m_pTextPoint->m_iBackgroundTransparency );
            m_checkBoxFill->Enable( false );
        } else if(m_pODPoint->m_sTypeString == wxT("Boundary Point")) {
            m_checkBoxFill->Enable( true );
            m_checkBoxFill->SetValue( m_pBoundaryPoint->m_bFill );
        }
        
        m_checkBoxShowName->SetValue( m_pODPoint->m_bShowName );
        m_checkBoxVisible->SetValue( m_pODPoint->m_bIsVisible );
        m_textCtrlGuid->SetValue( m_pODPoint->m_GUID );
        m_checkBoxShowODPointRangeRings->SetValue( m_pODPoint->GetShowODPointRangeRings() );
        m_choicePointRangeRingsNumber->SetSelection( m_pODPoint->GetODPointRangeRingsNumber() );
        m_choiceDistanceUnitsString->SetSelection( m_pODPoint->GetODPointRangeRingsStepUnits() );
        wxString buf;
        buf.Printf( _T("%.3f" ), m_pODPoint->GetODPointRangeRingsStep() );
        m_textCtrlPointRangeRingsSteps->SetValue( buf );
        m_colourPickerRangeRingsColour->SetColour( m_pODPoint->GetODPointRangeRingsColour() );
        
        wxCommandEvent eDummy;
        OnShowRangeRingsSelect( eDummy );
        

        m_bcomboBoxODPointIconName->Clear();
        //      Iterate on the Icon Descriptions, filling in the combo control
        bool fillCombo = m_bcomboBoxODPointIconName->GetCount() == 0;
        wxImageList *icons = g_pODPointMan->Getpmarkicon_image_list();

        if( fillCombo  && icons){
            for( int i = 0; i < g_pODPointMan->GetNumIcons(); i++ ) {
                wxString *ps = g_pODPointMan->GetIconDescription( i );
                m_bcomboBoxODPointIconName->Append( *ps, icons->GetBitmap( i ) );
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
            m_bcomboBoxODPointIconName->Append( m_pODPoint->GetIconName(), icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxODPointIconName->GetCount() - 1;
        }
        
        m_bcomboBoxODPointIconName->SetSelection( iconToSelect );
        m_bitmapPointBitmap->SetBitmap( m_bcomboBoxODPointIconName->GetItemBitmap( iconToSelect ) );
        
        icons = NULL;
        
        wxString caption( wxS("") );
        if ( m_pODPoint->GetTypeString().IsNull() || m_pODPoint->GetTypeString().IsEmpty() )
            caption.append( wxS("OCPN Draw Point") );
        else
            caption.append( m_pODPoint->GetTypeString() );
        caption.append( wxS(" Properties") );
        
        if( m_pODPoint->m_bIsInLayer ) {
            caption.append( wxS(", Layer: ") );
            caption.Append( g_pPathManagerDialog->GetLayerName( m_pODPoint->m_LayerID ) );
        }
        SetTitle( caption );
        if( m_pODPoint->m_sTypeString == wxT("Text Point") ) {
            m_panelDescription->Show( true );
            m_panelDescription->Enable();
        }
        else {
            m_panelDescription->Show( false );
            m_panelDescription->Disable();
        }
        
        m_notebookProperties->SetSelection(1);
        m_notebookProperties->SetSelection(0);
//        m_panelBasicProperties->Refresh( true );
        
    }

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
    //    Look in the master list of Waypoints to see if the currently selected waypoint is still valid
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

