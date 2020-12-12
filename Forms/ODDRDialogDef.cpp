///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODDRDialogDef.h"

///////////////////////////////////////////////////////////////////////////

ODDRDialogDef::ODDRDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizerDR;
	fgSizerDR = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizerDR->SetFlexibleDirection( wxBOTH );
	fgSizerDR->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextSOG = new wxStaticText( this, wxID_ANY, _("Speed over Ground"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextSOG->Wrap( -1 );
	fgSizer6->Add( m_staticTextSOG, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlSOG = new wxTextCtrl( this, wxID_ANY, _("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_textCtrlSOG, 0, wxALL, 5 );

	m_staticTextCOG = new wxStaticText( this, wxID_ANY, _("Course over Ground"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextCOG->Wrap( -1 );
	fgSizer6->Add( m_staticTextCOG, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlCOG = new wxTextCtrl( this, wxID_ANY, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_textCtrlCOG, 0, wxALL, 5 );

	m_staticTextLength = new wxStaticText( this, wxID_ANY, _("DR Path Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLength->Wrap( -1 );
	fgSizer6->Add( m_staticTextLength, 0, wxALL, 5 );

	m_textCtrlLength = new wxTextCtrl( this, wxID_ANY, _("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_textCtrlLength, 0, wxALL, 5 );

	m_staticTextDRPointInterval = new wxStaticText( this, wxID_ANY, _("DR Point Interval"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRPointInterval->Wrap( -1 );
	fgSizer6->Add( m_staticTextDRPointInterval, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlDRPointInterval = new wxTextCtrl( this, wxID_ANY, _("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( m_textCtrlDRPointInterval, 0, wxALL, 5 );

	wxString m_radioBoxLengthTypeChoices[] = { _("Time"), _("Distance") };
	int m_radioBoxLengthTypeNChoices = sizeof( m_radioBoxLengthTypeChoices ) / sizeof( wxString );
	m_radioBoxLengthType = new wxRadioBox( this, wxID_ANY, _("Length Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxLengthTypeNChoices, m_radioBoxLengthTypeChoices, 1, 0 );
	m_radioBoxLengthType->SetSelection( 1 );
	fgSizer6->Add( m_radioBoxLengthType, 0, wxALL, 5 );

	wxString m_radioBoxIntervalTypeChoices[] = { _("Time"), _("Distance") };
	int m_radioBoxIntervalTypeNChoices = sizeof( m_radioBoxIntervalTypeChoices ) / sizeof( wxString );
	m_radioBoxIntervalType = new wxRadioBox( this, wxID_ANY, _("Interval Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxIntervalTypeNChoices, m_radioBoxIntervalTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxIntervalType->SetSelection( 1 );
	fgSizer6->Add( m_radioBoxIntervalType, 0, wxALL, 5 );


	fgSizerDR->Add( fgSizer6, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerIntervalType;
	fgSizerIntervalType = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizerIntervalType->SetFlexibleDirection( wxBOTH );
	fgSizerIntervalType->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxString m_radioBoxDistanceUnitsChoices[] = { _("Kilometres"), _("Nautical Miles") };
	int m_radioBoxDistanceUnitsNChoices = sizeof( m_radioBoxDistanceUnitsChoices ) / sizeof( wxString );
	m_radioBoxDistanceUnits = new wxRadioBox( this, wxID_ANY, _("Distance Units"), wxDefaultPosition, wxDefaultSize, m_radioBoxDistanceUnitsNChoices, m_radioBoxDistanceUnitsChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxDistanceUnits->SetSelection( 1 );
	fgSizerIntervalType->Add( m_radioBoxDistanceUnits, 0, wxALL, 5 );

	wxString m_radioBoxTimeUnitsChoices[] = { _("Minutes"), _("Hours"), _("Days") };
	int m_radioBoxTimeUnitsNChoices = sizeof( m_radioBoxTimeUnitsChoices ) / sizeof( wxString );
	m_radioBoxTimeUnits = new wxRadioBox( this, wxID_ANY, _("Time Units"), wxDefaultPosition, wxDefaultSize, m_radioBoxTimeUnitsNChoices, m_radioBoxTimeUnitsChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxTimeUnits->SetSelection( 0 );
	fgSizerIntervalType->Add( m_radioBoxTimeUnits, 0, wxALL, 5 );


	fgSizerDR->Add( fgSizerIntervalType, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizerOKCancel;
	bSizerOKCancel = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );

	m_buttonOK->SetDefault();
	bSizerOKCancel->Add( m_buttonOK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_buttonCancel = new wxButton( this, wxID_ANY, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancel->Add( m_buttonCancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	fgSizerDR->Add( bSizerOKCancel, 1, wxEXPAND, 5 );


	this->SetSizer( fgSizerDR );
	this->Layout();
	fgSizerDR->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODDRDialogDef::OnOK ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODDRDialogDef::OnCancel ), NULL, this );
}

ODDRDialogDef::~ODDRDialogDef()
{
	// Disconnect Events
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODDRDialogDef::OnOK ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODDRDialogDef::OnCancel ), NULL, this );

}
