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
#include "OCPNDrawConfig.h"
#include "OCPNPoint.h"
#include "OCPNSelect.h"
#include "PathMan.h"
#include "PathProp.h"
#include "PointMan.h"
#include "ODPositionParser.h"
#include <wx/clipbrd.h>
#include <wx/menu.h>
#include <wx/window.h>

extern OCPNSelect   *pOCPNSelect;
extern ocpn_draw_pi *g_ocpn_draw_pi;
extern PointMan     *pOCPNPointMan;
extern PathMan      *g_pPathMan;
extern OCPNDrawConfig  *pOCPNDrawConfig;
extern PathManagerDialog *pPathManagerDialog;
extern PathProp     *pPathPropDialog;


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

    // TODO check if wxFrameBuilder supports BitmapComboBox. This code is to handle the case when it does not
    m_bcomboBoxIcon = new wxBitmapComboBox( m_panelBasicProperties, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN ); 
    m_SizerNameIcon->Replace( m_comboBoxIcon, m_bcomboBoxIcon );
    delete( m_comboBoxIcon );
    m_comboBoxIcon = NULL;

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
    // TODO: Implement onRightClick
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
    // TODO: Implement OnPositionCtlUpdated
    // Fetch the control values, convert to degrees
    double lat = fromDMM_Plugin( m_textLatitude->GetValue() );
    double lon = fromDMM_Plugin( m_textLongitude->GetValue() );

    if( !m_pOCPNPoint->m_bIsInLayer ) {
        m_pOCPNPoint->SetPosition( lat, lon );
        pOCPNSelect->ModifySelectablePoint( lat, lon, (void *) m_pOCPNPoint, SELTYPE_OCPNPOINT );
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

void ODPointPropertiesImpl::OnExtDescriptionClick( wxCommandEvent& event )
{
    // TODO: Implement OnExtDescriptionClick
}

void ODPointPropertiesImpl::OnPointPropertiesOKClick( wxCommandEvent& event )
{
    // TODO: Implement OnPointPropertiesOKClick
    if( m_pOCPNPoint ) {
        SaveChanges(); // write changes to globals and update config
        OnPositionCtlUpdated( event );

        RequestRefresh( g_ocpn_draw_pi->m_parent_window );
    }
    Show( false );

    if( pPathManagerDialog && pPathManagerDialog->IsShown() )
        pPathManagerDialog->UpdateOCPNPointsListCtrl();
        
    if( pPathPropDialog && pPathPropDialog->IsShown() )
        pPathPropDialog->UpdateProperties();

    SetClientSize(m_defaultClientSize);
    event.Skip();
}

void ODPointPropertiesImpl::OnPointPropertiesCancelClick( wxCommandEvent& event )
{
    // TODO: Implement OnPointPropertiesCancelClick
    if( m_pOCPNPoint ) {
        m_pOCPNPoint->SetVisible( m_bIsVisible_save );
        m_pOCPNPoint->SetNameShown( m_bShowName_save );
        m_pOCPNPoint->SetPosition( m_lat_save, m_lon_save );
        m_pOCPNPoint->SetIconName( m_IconName_save );
        m_pOCPNPoint->ReLoadIcon();

        m_pOCPNPoint->m_HyperlinkList->Clear();
    }

    Show( false );
    SetClientSize(m_defaultClientSize);

    event.Skip();
}

void ODPointPropertiesImpl::SaveChanges()
{
    if( m_pOCPNPoint ) {
        if( m_pOCPNPoint->m_bIsInLayer ) return;

        // Get User input Text Fields
        m_pOCPNPoint->m_iOCPNPointRangeRingsNumber = m_choicePointRangeRingsNumber->GetSelection();
        m_pOCPNPoint->m_fOCPNPointRangeRingsStep = atof( m_textCtrlPointRangeRingsSteps->GetValue().mb_str() );
        m_pOCPNPoint->m_iOCPNPointRangeRingsStepUnits = m_choiceDistanceUnitsString->GetSelection();
        m_pOCPNPoint->m_wxcOCPNPointRangeRingsColour = m_colourPickerRangeRingsColour->GetColour();
        m_pOCPNPoint->SetName( m_textName->GetValue() );
        m_pOCPNPoint->SetOCPNPointArrivalRadius( m_textArrivalRadius->GetValue() );
        m_pOCPNPoint->SetShowOCPNPointRangeRings( m_checkBoxShowODPointRangeRings->GetValue() );
        m_pOCPNPoint->m_MarkDescription = m_textDescription->GetValue();
        m_pOCPNPoint->SetVisible( m_checkBoxVisible->GetValue() );
        m_pOCPNPoint->SetNameShown( m_checkBoxShowName->GetValue() );
        m_pOCPNPoint->SetPosition( fromDMM_Plugin( m_textLatitude->GetValue() ), fromDMM_Plugin( m_textLongitude->GetValue() ) );
        wxString *icon_name = pOCPNPointMan->GetIconKey( m_bcomboBoxIcon->GetSelection() );
        if(icon_name && icon_name->Length())
            m_pOCPNPoint->SetIconName( *icon_name );
        m_pOCPNPoint->ReLoadIcon();

        // Here is some logic....
        // If the Markname is completely numeric, and is part of a route,
        // Then declare it to be of attribute m_bDynamicName = true
        // This is later used for re-numbering points on actions like
        // Insert Point, Delete Point, Append Point, etc

        if( m_pOCPNPoint->m_bIsInPath ) {
            bool b_name_is_numeric = true;
            for( unsigned int i = 0; i < m_pOCPNPoint->GetName().Len(); i++ ) {
                if( wxChar( '0' ) > m_pOCPNPoint->GetName()[i] ) b_name_is_numeric = false;
                if( wxChar( '9' ) < m_pOCPNPoint->GetName()[i] ) b_name_is_numeric = false;
            }

            m_pOCPNPoint->m_bDynamicName = b_name_is_numeric;
        } else
            m_pOCPNPoint->m_bDynamicName = false;

        if( m_pOCPNPoint->m_bIsInPath ) {
            // Update the Path segment selectables
            pOCPNSelect->UpdateSelectablePathSegments( m_pOCPNPoint );

            // Get an array of all paths using this point
            wxArrayPtrVoid *pEditPathArray = g_pPathMan->GetPathArrayContaining( m_pOCPNPoint );

            if( pEditPathArray ) {
                for( unsigned int ip = 0; ip < pEditPathArray->GetCount(); ip++ ) {
                    Path *pp = (Path *) pEditPathArray->Item( ip );
                    pp->FinalizeForRendering();
                    pp->UpdateSegmentDistances();

                    pOCPNDrawConfig->UpdatePath( pp );
                }
                delete pEditPathArray;
            }
        } else
            pOCPNDrawConfig->UpdateOCPNPoint( m_pOCPNPoint );

        // No general settings need be saved pConfig->UpdateSettings();
    }
    return;
}

void ODPointPropertiesImpl::SetOCPNPoint( OCPNPoint *pOP )
{
    m_pOCPNPoint = pOP;
    if( m_pOCPNPoint ) {
        m_lat_save = m_pOCPNPoint->m_lat;
        m_lon_save = m_pOCPNPoint->m_lon;
        m_IconName_save = m_pOCPNPoint->GetIconName();
        m_bShowName_save = m_pOCPNPoint->m_bShowName;
        m_bIsVisible_save = m_pOCPNPoint->m_bIsVisible;
    }
}

bool ODPointPropertiesImpl::UpdateProperties( bool positionOnly )
{
    if( m_pOCPNPoint ) {

        m_textLatitude->SetValue( toSDMM_PlugIn( 1, m_pOCPNPoint->m_lat ) );
        m_textLongitude->SetValue( toSDMM_PlugIn( 2, m_pOCPNPoint->m_lon ) );
        m_lat_save = m_pOCPNPoint->m_lat;
        m_lon_save = m_pOCPNPoint->m_lon;

        if( positionOnly ) return true;

        //Layer or not?
        if( m_pOCPNPoint->m_bIsInLayer ) {
            m_textName->SetEditable( false );
            m_textDescription->SetEditable( false );
            m_textCtrlExtDescription->SetEditable( false );
            m_textLatitude->SetEditable( false );
            m_textLongitude->SetEditable( false );
            m_bcomboBoxIcon->Enable( false );
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
            m_bcomboBoxIcon->Enable( true );
            m_checkBoxShowName->Enable( true );
            m_checkBoxVisible->Enable( true );
            m_textArrivalRadius->SetEditable ( true );
            m_checkBoxShowODPointRangeRings->Enable( true );
            m_choiceDistanceUnitsString->Enable( true );
            m_choicePointRangeRingsNumber->Enable( true );
            m_textCtrlPointRangeRingsSteps->SetEditable( true );
            m_colourPickerRangeRingsColour->Enable( true );
        }
        m_textName->SetValue( m_pOCPNPoint->GetName() );

        wxString s_ArrivalRadius;
        s_ArrivalRadius.Printf( _T("%.3f"), m_pOCPNPoint->GetOCPNPointArrivalRadius() );
        m_textArrivalRadius->SetValue( s_ArrivalRadius );        
        
        m_textDescription->SetValue( m_pOCPNPoint->m_MarkDescription );
        m_textCtrlExtDescription->SetValue( m_pOCPNPoint->m_MarkDescription );
        m_checkBoxShowName->SetValue( m_pOCPNPoint->m_bShowName );
        m_checkBoxVisible->SetValue( m_pOCPNPoint->m_bIsVisible );
        m_textCtrlGuid->SetValue( m_pOCPNPoint->m_GUID );
        m_checkBoxShowODPointRangeRings->SetValue( m_pOCPNPoint->GetShowOCPNPointRangeRings() );
        m_choicePointRangeRingsNumber->SetSelection( m_pOCPNPoint->GetOCPNPointRangeRingsNumber() );
        m_choiceDistanceUnitsString->SetSelection( m_pOCPNPoint->GetOCPNPointRangeRingsStepUnits() );
        wxString buf;
        buf.Printf( _T("%.3f" ), m_pOCPNPoint->GetOCPNPointRangeRingsStep() );
        m_textCtrlPointRangeRingsSteps->SetValue( buf );
        m_colourPickerRangeRingsColour->SetColour( m_pOCPNPoint->GetOCPNPointRangeRingsColour() );
        wxCommandEvent eDummy;
        OnShowRangeRingsSelect( eDummy );
        

        m_bcomboBoxIcon->Clear();
        //      Iterate on the Icon Descriptions, filling in the combo control
        bool fillCombo = m_bcomboBoxIcon->GetCount() == 0;
        wxImageList *icons = pOCPNPointMan->Getpmarkicon_image_list();

        if( fillCombo  && icons){
            for( int i = 0; i < pOCPNPointMan->GetNumIcons(); i++ ) {
                wxString *ps = pOCPNPointMan->GetIconDescription( i );
                m_bcomboBoxIcon->Append( *ps, icons->GetBitmap( i ) );
            }
        }
        
        // find the correct item in the combo box
        int iconToSelect = -1;
        for( int i = 0; i < pOCPNPointMan->GetNumIcons(); i++ ) {
            if( *pOCPNPointMan->GetIconKey( i ) == m_pOCPNPoint->GetIconName() )
                iconToSelect = i;
        }

        //  not found, so add  it to the list, with a generic bitmap and using the name as description
        // n.b.  This should never happen...
        if( -1 == iconToSelect){    
            m_bcomboBoxIcon->Append( m_pOCPNPoint->GetIconName(), icons->GetBitmap( 0 ) );
            iconToSelect = m_bcomboBoxIcon->GetCount() - 1;
        }
        
        
        m_bcomboBoxIcon->Select( iconToSelect );
        icons = NULL;
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
    wxOCPNPointListNode *node = pOCPNPointMan->GetOCPNPointList()->GetFirst();
    
    bool b_found = false;
    while( node ) {
        OCPNPoint *op = node->GetData();
        if( m_pOCPNPoint == op ) {
            b_found = true;
            break;
        }
        
        node = node->GetNext();
    }
    
    if( !b_found ) m_pOCPNPoint = NULL;
}
