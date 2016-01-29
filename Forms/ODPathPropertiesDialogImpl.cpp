#include "ODPathPropertiesDialogImpl.h"

ODPathPropertiesDialogImpl::ODPathPropertiesDialogImpl( wxWindow* parent )
:
ODPathPropertiesDialogDef( parent )
{

}

void ODPathPropertiesDialogImpl::OnClose( wxCloseEvent& event )
{
// TODO: Implement OnClose
}

void ODPathPropertiesDialogImpl::OnOK( wxCommandEvent& event )
{
// TODO: Implement OnOK
}

void ODPathPropertiesDialogImpl::OnCancel( wxCommandEvent& event )
{
// TODO: Implement OnCancel
}

void ODPathPropertiesDialogImpl::OnRightClick( wxMouseEvent& event )
{
// TODO: Implement OnRightClick
   wxMenu menu;

    if( ! m_pPath->m_bIsInLayer ) {
        wxString sPropertiesType( wxS("") );
        if ( m_pPath->m_sTypeString.IsNull() || m_pPath->m_sTypeString.IsEmpty() )
            sPropertiesType.append( wxS("OCPN Draw Point") );
        else {
            sPropertiesType.append( m_pPath->m_sTypeString );
            sPropertiesType.append( wxT(" Point") );
        }
        sPropertiesType.append( wxS(" &Properties...") );
        wxMenuItem* editItem = menu.Append( ID_PATHPROP_MENU_EDIT_WP, sPropertiesType );
        editItem->Enable( m_opList->GetSelectedItemCount() == 1 );

        wxMenuItem* delItem = menu.Append( ID_PATHPROP_MENU_DELETE, wxS("&Remove Selected") );
        delItem->Enable( m_opList->GetSelectedItemCount() > 0 && m_opList->GetItemCount() > 2 );
    }

    PopupMenu( &menu );
}

void ODPathPropertiesDialogImpl::OnPathPropertiesDoubleClick( wxMouseEvent& event )
{
// TODO: Implement OnPathPropertiesDoubleClick
    long item = -1;
    
    item = m_dataViewListCtrlPoints->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    
    if( item == -1 ) return;
    
    ODPoint *op = (ODPoint *) m_opList->GetItemData( item );
    if( !op ) return;
    
    PathManagerDialog::ODPointShowPropertiesDialog( op, this );
}
#include "ODPathPropertiesDialogImpl.h"
ODPathPropertiesDialogImpl::ODPathPropertiesDialogImpl( wxWindow* parent )
:
MyDialogPathProperties( parent )
{

}