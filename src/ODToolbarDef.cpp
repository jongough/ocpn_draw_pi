///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODToolbarDef.h"

///////////////////////////////////////////////////////////////////////////

ODToolbarDialog::ODToolbarDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerToolbar;
	bSizerToolbar = new wxBoxSizer( wxVERTICAL );
	
	bSizerToolbar->SetMinSize( wxSize( 20,20 ) ); 
	m_toolBarODToolbar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	m_toolBarODToolbar->Realize(); 
	
	bSizerToolbar->Add( m_toolBarODToolbar, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerToolbar );
	this->Layout();
}

ODToolbarDialog::~ODToolbarDialog()
{
}
