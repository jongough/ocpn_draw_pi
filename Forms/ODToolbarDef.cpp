///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODToolbarDef.h"

///////////////////////////////////////////////////////////////////////////

ODToolbarDialog::ODToolbarDialog()
{
}

ODToolbarDialog::ODToolbarDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style )
{
	this->Create( parent, id, title, pos, size, style );
}

bool ODToolbarDialog::Create( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style )
{
	if ( !wxDialog::Create( parent, id, title, pos, size, style ) )
	{
		return false;
	}

	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );

	m_bSizerToolbar = new wxBoxSizer( wxVERTICAL );

	m_toolBarODToolbar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxBORDER_NONE );
	m_toolBarODToolbar->Realize();

	m_bSizerToolbar->Add( m_toolBarODToolbar, 1, wxEXPAND, 5 );


	this->SetSizer( m_bSizerToolbar );
	this->Layout();
	m_bSizerToolbar->Fit( this );

	// Connect Events
	this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( ODToolbarDialog::OnActivate ) );
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODToolbarDialog::OnClose ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ODToolbarDialog::OnKeyDown ) );
	this->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ODToolbarDialog::OnLeftDown ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( ODToolbarDialog::OnSize ) );
	m_toolBarODToolbar->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ODToolbarDialog::OnKeyDown ), NULL, this );
	m_toolBarODToolbar->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ODToolbarDialog::OnLeftDown ), NULL, this );

	return true;
}

ODToolbarDialog::~ODToolbarDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ACTIVATE, wxActivateEventHandler( ODToolbarDialog::OnActivate ) );
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODToolbarDialog::OnClose ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ODToolbarDialog::OnKeyDown ) );
	this->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ODToolbarDialog::OnLeftDown ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( ODToolbarDialog::OnSize ) );
	m_toolBarODToolbar->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ODToolbarDialog::OnKeyDown ), NULL, this );
	m_toolBarODToolbar->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ODToolbarDialog::OnLeftDown ), NULL, this );

}
