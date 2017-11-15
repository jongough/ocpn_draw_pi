/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  PIL Properties
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

#include "PILProp.h"
#include "PIL.h"
#include "ocpn_draw_pi.h"
#include "ODUtils.h"

#if wxCHECK_VERSION(3,0,0) 
#include <wx/valnum.h>
#endif

enum {
    ID_INDEX_ID = 0,
    ID_INDEX_NAME,
    ID_INDEX_OFFSET,
    ID_INDEX_DESCRIPTION,

    ID_POINTS_LIST_LAST
};


extern PILList                  *g_pPILList;
extern ocpn_draw_pi             *g_ocpn_draw_pi;
extern ODPlugIn_Position_Fix_Ex g_pfFix;

PILProp::PILProp()
{
    //ctor
}

PILProp::PILProp( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
: ODPathPropertiesDialogImpl( parent, id, caption, pos, size, style )
{
    //ctor
    m_bLockUpdate = false;
    m_fgSizerPIL->ShowItems( true );

    
    //m_fgSizerPath->ShowItems( true );
    m_radioBoxPathPersistence->Show();
    m_radioBoxPathPersistence->Enable( true );
    m_radioBoxPathPersistence->SetLabel( _("PIL Persistence") );
    m_staticTextPILAngle->Show();
    m_staticTextPILAngle->Enable(true);
    m_textCtrlPILAngle->Show();
    m_textCtrlPILAngle->Enable(true);
    m_textCtrlPILAngle->SetEditable(true);
    m_bSizerPathPoints->ShowItems( false );
    m_bSizerPILLines->ShowItems( true );
    

#if wxCHECK_VERSION(3,0,0) && !defined(__WXMSW__)
    wxFloatingPointValidator<double> dODPILAngle(2, &m_dODPILAngleValidator, wxNUM_VAL_DEFAULT);
    dODPILAngle.SetRange(-180, 180);
    
    m_textCtrlPILAngle->SetValidator( dODPILAngle );
#endif

    SetPointsListHeadings();
    
    this->GetSizer()->Fit( this );
    this->Layout();
    
}


PILProp::~PILProp()
{
    //dtor
}

bool PILProp::UpdateProperties( PIL *pInPIL )
{
    if( NULL == pInPIL ) return false;

    ::wxBeginBusyCursor();

    SetGlobalLocale();

    wxString s;

    m_textCtrlName->SetValue( pInPIL->m_PathNameString );
    m_textCtrlDesctiption->SetValue( pInPIL->m_PathDescription);
    m_textCtrlGUID->SetValue( pInPIL->m_GUID );
    m_checkBoxActive->SetValue( pInPIL->IsActive() );

    m_radioBoxPathPersistence->SetSelection( pInPIL->m_iPersistenceType );
    
#if wxCHECK_VERSION(3,0,0) && !defined(__WXMSW__)
    if(pInPIL->m_dEBLAngle > 180)
        m_dODPILAngleValidator = pInPIL->m_dEBLAngle - 360;
    else
        m_dODPILAngleValidator = pInPIL->m_dEBLAngle;
    
    m_dODPILLengthValidator = toUsrDistance_Plugin(pInPIL->m_dLength);
#else
    if(pInPIL->m_dEBLAngle > 180)
        s.Printf( _T("%.2f"), pInPIL->m_dEBLAngle - 360 );
    else
        s.Printf( _T("%.2f"), pInPIL->m_dEBLAngle );
    
    m_textCtrlPILAngle->SetValue(s);
    
    s.Printf( _T("%5.2f"), toUsrDistance_Plugin(m_pPIL->m_dLength) );
    m_textCtrlTotalLength->SetValue(s);
#endif
    
    long item_line_index = 0;
    std::list<PILLINE>::iterator it = pInPIL->m_PilLineList.begin();
    while(it != pInPIL->m_PilLineList.end()) {
        m_listCtrlPILList->SetItem( item_line_index, ID_INDEX_ID, wxString::Format("%i", it->iID) );
        m_listCtrlPILList->SetItem( item_line_index, ID_INDEX_NAME, it->sName );
        m_listCtrlPILList->SetItem( item_line_index, ID_INDEX_OFFSET, wxString::Format("%.3f", it->dOffset) );
        if(it->sDescription.Len() > 0)
            m_listCtrlPILList->SetItem( item_line_index, ID_INDEX_DESCRIPTION, it->sDescription );
        else
            m_listCtrlPILList->SetItem( item_line_index, ID_INDEX_DESCRIPTION, _T(" ") );
        ++it;
        item_line_index++;
    }

    m_bLockUpdate = false;
    
    for( unsigned int i = 0; i < sizeof( ::StyleValues ) / sizeof(int); i++ ) {
        if( pInPIL->m_style == ::StyleValues[i] ) {
            m_choiceLineStyle->Select( i );
            break;
        }
    }

    for( unsigned int i = 0; i < sizeof( ::WidthValues ) / sizeof(int); i++ ) {
        if( pInPIL->m_width == ::WidthValues[i] ) {
            m_choiceLineWidth->Select( i );
            break;
        }
    }

    // Set column width correctly for data
    for(int i = 0; i < m_listCtrlPILList->GetColumnCount(); i++) {
#ifdef WIN32
        m_listCtrlPILList->SetColumnWidth( i, wxLIST_AUTOSIZE_USEHEADER );
#else
        m_listCtrlPILList->SetColumnWidth( i, wxLIST_AUTOSIZE );
#endif
    }

    ::wxEndBusyCursor();

    return true;

}

bool PILProp::UpdateProperties( void )
{
    wxString s;
    
    if(m_bLockUpdate) return true;
    
    if(!m_bLockPILAngle){
        if(m_pPIL->m_dEBLAngle > 180)
            s.Printf( _T("%.2f"), m_pPIL->m_dEBLAngle - 360 );
        else
            s.Printf( _T("%.2f"), m_pPIL->m_dEBLAngle );
        
        m_textCtrlPILAngle->SetValue(s);
    }
    
    return  true;
}

bool PILProp::SaveChanges( void )
{
    wxColour l_PILOrigColour = m_pPIL->GetCurrentColour();
    ODPoint *pFirstPoint = m_pPIL->m_pODPointList->GetFirst()->GetData();

    bool l_bUpdatePath = false;
    
    if(pFirstPoint->GetODPointRangeRingsColour() == l_PILOrigColour)
        pFirstPoint->SetODPointRangeRingsColour( m_pPIL->GetCurrentColour() );

    m_pPIL->m_iPersistenceType = m_radioBoxPathPersistence->GetSelection();
    if(m_pPIL->m_iPersistenceType == ID_NOT_PERSISTENT || m_pPIL->m_iPersistenceType == ID_PERSISTENT_CRASH)
        m_pPIL->m_bTemporary = true;
    else
        m_pPIL->m_bTemporary = false;

    double l_dAngle;
    m_textCtrlPILAngle->GetValue().ToDouble( &l_dAngle );
    if(l_dAngle != m_pPIL->m_dEBLAngle) {
        m_pPIL->m_dEBLAngle = l_dAngle;
        l_bUpdatePath = true;
    }

    if(l_bUpdatePath)
        m_pPIL->MoveEndPoint(true);

    bool ret = ODPathPropertiesDialogImpl::SaveChanges();
    
    return ret;
}

void PILProp::OnSetFocus( wxFocusEvent& event )
{
    if(event.GetId() == m_textCtrlTotalLength->GetId())
        m_bLockPILLength = true;
    if(event.GetId() == m_textCtrlPILAngle->GetId())
        m_bLockPILAngle = true;
    ODPathPropertiesDialogDef::OnSetFocus(event);
}

void PILProp::OnKillFocus( wxFocusEvent& event )
{
    ODPathPropertiesDialogDef::OnKillFocus(event);
}

void PILProp::OnOK( wxCommandEvent& event )
{
    m_bLockPILLength = false;
    m_bLockPILAngle = false;
    ODPathPropertiesDialogImpl::OnOK(event);
}

void PILProp::OnClose( wxCloseEvent& event )
{
    m_bLockPILLength = false;
    m_bLockPILAngle = false;
    ODPathPropertiesDialogImpl::OnClose(event);
    
    ResetGlobalLocale();
}

void PILProp::OnCancel( wxCommandEvent& event )
{
    m_bLockPILLength = false;
    m_bLockPILAngle = false;
    ODPathPropertiesDialogImpl::OnCancel(event);

    ResetGlobalLocale();
}


void PILProp::OnChoiceLineWidth( wxCommandEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnChoiceLineWidth(event);
}

void PILProp::OnChoiceLineStyle( wxCommandEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnChoiceLineStyle(event);
}

void PILProp::OnKillFocusChoiceLineWidth( wxFocusEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnKillFocusChoiceLineWidth(event);
}

void PILProp::OnSetFocusChoiceLineWidth( wxFocusEvent& event )
{
    m_bLockUpdate = true;
    ODPathPropertiesDialogImpl::OnSetFocusChoiceLineWidth(event);
}

void PILProp::OnKillFocusChoiceLineStyle( wxFocusEvent& event )
{
    m_bLockUpdate = false;
    ODPathPropertiesDialogImpl::OnKillFocusChoiceLineStyle(event);
}

void PILProp::OnSetFocusChoiceLineStyle( wxFocusEvent& event )
{
    m_bLockUpdate = true;
    ODPathPropertiesDialogImpl::OnSetFocusChoiceLineStyle(event);
}

void PILProp::SetPointsListHeadings()
{
    m_listCtrlPILList->DeleteAllColumns();
    m_listCtrlPILList->InsertColumn( ID_INDEX_ID, _("ID"), wxLIST_FORMAT_LEFT );
    m_listCtrlPILList->InsertColumn( ID_INDEX_NAME, _("Name"), wxLIST_FORMAT_LEFT );
    m_listCtrlPILList->InsertColumn( ID_INDEX_OFFSET, _("Offset"), wxLIST_FORMAT_RIGHT );
    m_listCtrlPILList->InsertColumn( ID_INDEX_DESCRIPTION, _("Description"), wxLIST_FORMAT_LEFT );
}

void PILProp::InitializeList( void )
{
    if( NULL == m_pPath ) return;

    //  Iterate on Offset lines, inserting blank fields starting with index 0
    std::list<PILLINE>::iterator it = m_pPIL->m_PilLineList.begin();
    int in = 0;
    while(it != m_pPIL->m_PilLineList.end()) {
        m_listCtrlPILList->InsertItem( in, _T(""), 0);
        in++;
        ++it;
    }
}

void PILProp::SetPath( ODPath *pP )
{
    if( NULL == pP )
        return;

    m_tz_selection = 1;

    if( m_pPath ) {
        m_pPath->m_bPathPropertiesBlink = false;
    }
    if(pP->m_sTypeString == wxT("PIL")) {
        m_pPIL = (PIL *)pP;
        m_pPath = m_pPIL;
    } else return;

    m_pPath->m_bPathPropertiesBlink = true;

    m_textCtrlName->SetValue( m_pPath->m_PathNameString );

    m_textCtrlName->SetFocus();
    m_listCtrlPILList->DeleteAllItems();

    InitializeList();

    if( m_pPath )
        m_listCtrlPILList->Show();

    Refresh( false );

}



