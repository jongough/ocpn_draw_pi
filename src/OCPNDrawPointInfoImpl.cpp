/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPNPoint dialog box Support
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

#include "OCPNDrawPointInfoImpl.h"
#include "ocpn_plugin.h"
#include "PointMan.h"
#include "PathMan.h"
#include "pathmanagerdialog.h"
#include "PathProp.h"
#include "OCPNSelect.h"
#include "OCPNDrawConfig.h"

extern PointMan        *pOCPNPointMan;
extern ChartCanvas      *ocpncc1;
extern PathManagerDialog *pPathManagerDialog;
extern PathProp       *pPathPropDialog;
extern OCPNSelect        *pOCPNSelect;
extern PathMan           *g_pPathMan;
extern OCPNDrawConfig     *pOCPNDrawConfig;

OCPNDrawPointInfoImpl::OCPNDrawPointInfoImpl( wxWindow* parent, wxWindowID id, const wxString& title,
        const wxPoint& pos, const wxSize& size, long style ) :
        MarkInfoImpl( parent, id, title, pos, size, style )
{
      m_pMyLinkList = NULL;

}

OCPNDrawPointInfoImpl::~OCPNDrawPointInfoImpl()
{
    //dtor
}

void OCPNDrawPointInfoImpl::SetOCPNPoint( OCPNPoint *pOP )
{
    m_pOCPNPoint = pOP;
    if( m_pOCPNPoint ) {
        m_lat_save = m_pOCPNPoint->m_lat;
        m_lon_save = m_pOCPNPoint->m_lon;
        m_IconName_save = m_pOCPNPoint->GetIconName();
        m_bShowName_save = m_pOCPNPoint->m_bShowName;
        m_bIsVisible_save = m_pOCPNPoint->m_bIsVisible;
        if( m_pMyLinkList )
            delete m_pMyLinkList;
        m_pMyLinkList = new HyperlinkList();
        int NbrOfLinks = m_pOCPNPoint->m_HyperlinkList->GetCount();
//            int len = 0;
        if( NbrOfLinks > 0 ) {
            wxHyperlinkListNode *linknode = m_pOCPNPoint->m_HyperlinkList->GetFirst();
            while( linknode ) {
                Hyperlink *link = linknode->GetData();

                Hyperlink* h = new Hyperlink();
                h->DescrText = link->DescrText;
                h->Link = link->Link;
                h->LType = link->LType;

                m_pMyLinkList->Append( h );

                linknode = linknode->GetNext();
            }
        }
    }
}

bool OCPNDrawPointInfoImpl::UpdateProperties( bool positionOnly )
{
    if( m_pOCPNPoint ) {

        m_textLatitude->SetValue( ::toSDMM_PlugIn( 1, m_pOCPNPoint->m_lat ) );
        m_textLongitude->SetValue( ::toSDMM_PlugIn( 2, m_pOCPNPoint->m_lon ) );
        m_lat_save = m_pOCPNPoint->m_lat;
        m_lon_save = m_pOCPNPoint->m_lon;

        if( positionOnly ) return true;

        //Layer or not?
        if( m_pOCPNPoint->m_bIsInLayer ) {
            m_staticTextLayer->Enable();
            m_staticTextLayer->Show( true );
            m_textName->SetEditable( false );
            m_textDescription->SetEditable( false );
            m_textCtrlExtDescription->SetEditable( false );
            m_textLatitude->SetEditable( false );
            m_textLongitude->SetEditable( false );
            m_bcomboBoxIcon->Enable( false );
            m_buttonAddLink->Enable( false );
            m_toggleBtnEdit->Enable( false );
            m_toggleBtnEdit->SetValue( false );
            m_checkBoxShowName->Enable( false );
            m_checkBoxVisible->Enable( false );
            m_textArrivalRadius->SetEditable ( false );
        } else {
            m_staticTextLayer->Enable( false );
            m_staticTextLayer->Show( false );
            m_textName->SetEditable( true );
            m_textDescription->SetEditable( true );
            m_textCtrlExtDescription->SetEditable( true );
            m_textLatitude->SetEditable( true );
            m_textLongitude->SetEditable( true );
            m_bcomboBoxIcon->Enable( true );
            m_buttonAddLink->Enable( true );
            m_toggleBtnEdit->Enable( true );
            m_checkBoxShowName->Enable( true );
            m_checkBoxVisible->Enable( true );
            m_textArrivalRadius->SetEditable ( true );
        }
        m_textName->SetValue( m_pOCPNPoint->GetName() );

        wxString s_ArrivalRadius;
        s_ArrivalRadius.Printf( _T("%.3f"), m_pOCPNPoint->GetOCPNPointArrivalRadius() );
        m_textArrivalRadius->SetValue( s_ArrivalRadius );        
        
        m_textDescription->SetValue( m_pOCPNPoint->m_MarkDescription );
        m_textCtrlExtDescription->SetValue( m_pOCPNPoint->m_MarkDescription );
        m_bitmapIcon->SetBitmap( *m_pOCPNPoint->GetIconBitmap() );
        wxWindowList kids = m_scrolledWindowLinks->GetChildren();
        for( unsigned int i = 0; i < kids.GetCount(); i++ ) {
            wxWindowListNode *node = kids.Item( i );
            wxWindow *win = node->GetData();

            if( win->IsKindOf( CLASSINFO(wxHyperlinkCtrl) ) ) {
                ( (wxHyperlinkCtrl*) win )->Disconnect( wxEVT_COMMAND_HYPERLINK,
                        wxHyperlinkEventHandler( OCPNDrawPointInfoImpl::OnHyperLinkClick ) );
                ( (wxHyperlinkCtrl*) win )->Disconnect( wxEVT_RIGHT_DOWN,
                        wxMouseEventHandler( OCPNDrawPointInfoImpl::m_hyperlinkContextMenu ) );
            }
        }
        m_scrolledWindowLinks->DestroyChildren();
        m_checkBoxShowName->SetValue( m_pOCPNPoint->m_bShowName );
        m_checkBoxVisible->SetValue( m_pOCPNPoint->m_bIsVisible );
        m_textCtrlGuid->SetValue( m_pOCPNPoint->m_GUID );
        m_checkBoxShowWaypointRangeRings->SetValue( m_pOCPNPoint->GetShowOCPNPointRangeRings() );
        m_choiceWaypointRangeRingsNumber->SetSelection( m_pOCPNPoint->GetOCPNPointRangeRingsNumber() );
        m_choiceWaypointRangeRingsUnits->SetSelection( m_pOCPNPoint->GetOCPNPointRangeRingsStepUnits() );
        wxString buf;
        buf.Printf( _T("%.3f" ), m_pOCPNPoint->GetOCPNPointRangeRingsStep() );
        m_textWaypointRangeRingsStep->SetValue( buf );
        m_colourWaypointRangeRingsColour->SetColour( m_pOCPNPoint->GetOCPNPointRangeRingsColour() );

        int NbrOfLinks = m_pOCPNPoint->m_HyperlinkList->GetCount();
        HyperlinkList *hyperlinklist = m_pOCPNPoint->m_HyperlinkList;
//            int len = 0;
        if( NbrOfLinks > 0 ) {
            wxHyperlinkListNode *linknode = hyperlinklist->GetFirst();
            while( linknode ) {
                Hyperlink *link = linknode->GetData();
                wxString Link = link->Link;
                wxString Descr = link->DescrText;

                wxHyperlinkCtrl* ctrl = new wxHyperlinkCtrl( m_scrolledWindowLinks, wxID_ANY, Descr,
                        Link, wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
                ctrl->Connect( wxEVT_COMMAND_HYPERLINK,
                        wxHyperlinkEventHandler( OCPNDrawPointInfoImpl::OnHyperLinkClick ), NULL, this );
                if( !m_pOCPNPoint->m_bIsInLayer ) ctrl->Connect( wxEVT_RIGHT_DOWN,
                        wxMouseEventHandler( OCPNDrawPointInfoImpl::m_hyperlinkContextMenu ), NULL, this );

                bSizerLinks->Add( ctrl, 0, wxALL, 5 );

                linknode = linknode->GetNext();
            }
        }
        
        // Integrate all of the rebuilt hyperlink controls
        bSizerLinks->Layout();
        
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

void OCPNDrawPointInfoImpl::OnHyperLinkClick( wxHyperlinkEvent &event )
{
    if( m_toggleBtnEdit->GetValue() ) {
        m_pEditedLink = (wxHyperlinkCtrl*) event.GetEventObject();
        OnEditLink( event );
        event.Skip( false );
        return;
    }
    //    Windows has trouble handling local file URLs with embedded anchor points, e.g file://testfile.html#point1
    //    The trouble is with the wxLaunchDefaultBrowser with verb "open"
    //    Workaround is to probe the registry to get the default browser, and open directly
    //
    //    But, we will do this only if the URL contains the anchor point charater '#'
    //    What a hack......

#ifdef __WXMSW__

    wxString cc = event.GetURL();
    if( cc.Find( _T("#") ) != wxNOT_FOUND ) {
        wxRegKey RegKey( wxString( _T("HKEY_CLASSES_ROOT\\HTTP\\shell\\open\\command") ) );
        if( RegKey.Exists() ) {
            wxString command_line;
            RegKey.QueryValue( wxString( _T("") ), command_line );

            //  Remove "
            command_line.Replace( wxString( _T("\"") ), wxString( _T("") ) );

            //  Strip arguments
            int l = command_line.Find( _T(".exe") );
            if( wxNOT_FOUND == l ) l = command_line.Find( _T(".EXE") );

            if( wxNOT_FOUND != l ) {
                wxString cl = command_line.Mid( 0, l + 4 );
                cl += _T(" ");
                cc.Prepend( _T("\"") );
                cc.Append( _T("\"") );
                cl += cc;
                wxExecute( cl );        // Async, so Fire and Forget...
            }
        }
    } else
        event.Skip();
#else
    wxString url = event.GetURL();
    url.Replace(_T(" "), _T("%20") );
    ::wxLaunchDefaultBrowser(url);
//    event.Skip();
#endif
}

void OCPNDrawPointInfoImpl::OnMarkInfoOKClick( wxCommandEvent& event )
{
    if( m_pOCPNPoint ) {
        m_pOCPNPoint->m_iOCPNPointRangeRingsNumber = m_choiceWaypointRangeRingsNumber->GetSelection();
        m_pOCPNPoint->m_fOCPNPointRangeRingsStep = atof( m_textWaypointRangeRingsStep->GetValue().mb_str() );
        m_pOCPNPoint->m_iOCPNPointRangeRingsStepUnits = m_choiceWaypointRangeRingsUnits->GetSelection();
        m_pOCPNPoint->m_wxcOCPNPointRangeRingsColour = m_colourWaypointRangeRingsColour->GetColour();
        OnPositionCtlUpdated( event );
        SaveChanges(); // write changes to globals and update config
        ocpncc1->RefreshRect( m_pOCPNPoint->CurrentRect_in_DC.Inflate( 1000, 100 ), false );
    }
    Show( false );
    if( m_pMyLinkList ) {
        delete m_pMyLinkList;
        m_pMyLinkList = NULL;
    }

    if( pPathManagerDialog && pPathManagerDialog->IsShown() )
        pPathManagerDialog->UpdateOCPNPointsListCtrl();
        
    if( pPathPropDialog && pPathPropDialog->IsShown() )
        pPathPropDialog->UpdateProperties();

    SetClientSize(m_defaultClientSize);
    event.Skip();
}

bool OCPNDrawPointInfoImpl::SaveChanges()
{
    if( m_pOCPNPoint ) {
        if( m_pOCPNPoint->m_bIsInLayer ) return true;

        // Get User input Text Fields
        m_pOCPNPoint->SetName( m_textName->GetValue() );
        m_pOCPNPoint->SetOCPNPointArrivalRadius( m_textArrivalRadius->GetValue() );
        m_pOCPNPoint->SetShowOCPNPointRangeRings( m_checkBoxShowWaypointRangeRings->GetValue() );
        m_pOCPNPoint->m_MarkDescription = m_textDescription->GetValue();
        m_pOCPNPoint->SetVisible( m_checkBoxVisible->GetValue() );
        m_pOCPNPoint->SetNameShown( m_checkBoxShowName->GetValue() );
        m_pOCPNPoint->SetPosition( fromDMM_Plugin( m_textLatitude->GetValue() ),
                fromDMM_Plugin( m_textLongitude->GetValue() ) );
        wxString *icon_name = pOCPNPointMan->GetIconKey( m_bcomboBoxIcon->GetSelection() );
        if(icon_name && icon_name->Length())
            m_pOCPNPoint->SetIconName( *icon_name );
        m_pOCPNPoint->ReLoadIcon();

        // Here is some logic....
        // If the Markname is completely numeric, and is part of a route,
        // Then declare it to be of attribute m_bDynamicName = true
        // This is later used for re-numbering points on actions like
        // Insert Point, Delete Point, Append Point, etc

        if( m_pOCPNPoint->m_bIsInRoute ) {
            bool b_name_is_numeric = true;
            for( unsigned int i = 0; i < m_pOCPNPoint->GetName().Len(); i++ ) {
                if( wxChar( '0' ) > m_pOCPNPoint->GetName()[i] ) b_name_is_numeric = false;
                if( wxChar( '9' ) < m_pOCPNPoint->GetName()[i] ) b_name_is_numeric = false;
            }

            m_pOCPNPoint->m_bDynamicName = b_name_is_numeric;
        } else
            m_pOCPNPoint->m_bDynamicName = false;

        if( m_pOCPNPoint->m_bIsInPath ) {
            // Update the route segment selectables
            pOCPNSelect->UpdateSelectablePathSegments( m_pOCPNPoint );

            // Get an array of all paths using this point
            wxArrayPtrVoid *pEditPathArray = g_pPathMan->GetPathArrayContaining( m_pOCPNPoint );

            if( pEditPathArray ) {
                for( unsigned int ir = 0; ir < pEditPathArray->GetCount(); ir++ ) {
                    Path *pp = (Path *) pEditPathArray->Item( ir );
                    pp->FinalizeForRendering();
                    pp->UpdateSegmentDistances();

                    pOCPNDrawConfig->UpdatePath( pp );
                }
                delete pEditPathArray;
            }
        } else
            pOCPNDrawConfig->UpdateOCPNPoint( m_pOCPNPoint );

        // No general settings need be saved pOCPNDrawConfig->UpdateSettings();
    }
    return true;
}

void OCPNDrawPointInfoImpl::OnMarkInfoCancelClick( wxCommandEvent& event )
{
    if( m_pOCPNPoint ) {
        m_pOCPNPoint->SetVisible( m_bIsVisible_save );
        m_pOCPNPoint->SetNameShown( m_bShowName_save );
        m_pOCPNPoint->SetPosition( m_lat_save, m_lon_save );
        m_pOCPNPoint->SetIconName( m_IconName_save );
        m_pOCPNPoint->ReLoadIcon();

        m_pOCPNPoint->m_HyperlinkList->Clear();

        int NbrOfLinks = m_pMyLinkList->GetCount();
        if( NbrOfLinks > 0 ) {
            wxHyperlinkListNode *linknode = m_pMyLinkList->GetFirst();
            while( linknode ) {
                Hyperlink *link = linknode->GetData();
                Hyperlink* h = new Hyperlink();
                h->DescrText = link->DescrText;
                h->Link = link->Link;
                h->LType = link->LType;

                m_pOCPNPoint->m_HyperlinkList->Append( h );

                linknode = linknode->GetNext();
            }
        }
    }

    Show( false );
    delete m_pMyLinkList;
    m_pMyLinkList = NULL;
    SetClientSize(m_defaultClientSize);

    event.Skip();
}

void OCPNDrawPointInfoImpl::OnPositionCtlUpdated( wxCommandEvent& event )
{
    // Fetch the control values, convert to degrees
    double lat = fromDMM_Plugin( m_textLatitude->GetValue() );
    double lon = fromDMM_Plugin( m_textLongitude->GetValue() );

    if( !m_pOCPNPoint->m_bIsInLayer ) {
        m_pOCPNPoint->SetPosition( lat, lon );
        pOCPNSelect->ModifySelectablePoint( lat, lon, (void *) m_pOCPNPoint, SELTYPE_OCPNPOINT );
    }

    // Update the mark position dynamically
    RequestRefresh( GetOCPNCanvasWindow() );
  
}

