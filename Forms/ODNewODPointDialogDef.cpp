///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODNewODPointDialogDef.h"

///////////////////////////////////////////////////////////////////////////

ODNewODPointDialogDef::ODNewODPointDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxString m_radioBoxODPointTypeChoices[] = { _("Boundary"), _("Text") };
	int m_radioBoxODPointTypeNChoices = sizeof( m_radioBoxODPointTypeChoices ) / sizeof( wxString );
	m_radioBoxODPointType = new wxRadioBox( this, wxID_ANY, _("Point Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxODPointTypeNChoices, m_radioBoxODPointTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxODPointType->SetSelection( 0 );
	fgSizer4->Add( m_radioBoxODPointType, 0, wxALL, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonOK, 0, wxALL, 5 );

	m_buttonCancel = new wxButton( this, wxID_ANY, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonCancel, 0, wxALL, 5 );


	fgSizer4->Add( bSizer2, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizer4 );
	this->Layout();
	fgSizer4->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODNewODPointDialogDef::OnOK ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODNewODPointDialogDef::OnCancel ), NULL, this );
}

ODNewODPointDialogDef::~ODNewODPointDialogDef()
{
	// Disconnect Events
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODNewODPointDialogDef::OnOK ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODNewODPointDialogDef::OnCancel ), NULL, this );

}
