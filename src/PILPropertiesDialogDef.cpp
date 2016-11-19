///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 16 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "PILPropertiesDialogDef.h"

///////////////////////////////////////////////////////////////////////////

PILPropertiesDialogDef::PILPropertiesDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizerDescription;
	fgSizerDescription = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerDescription->AddGrowableCol( 1 );
	fgSizerDescription->SetFlexibleDirection( wxBOTH );
	fgSizerDescription->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextName = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextName->Wrap( -1 );
	fgSizerDescription->Add( m_staticTextName, 0, wxALL, 5 );
	
	m_textCtrlName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_textCtrlName->HasFlag( wxTE_MULTILINE ) )
	{
	m_textCtrlName->SetMaxLength(  );
	}
	#else
	m_textCtrlName->SetMaxLength(  );
	#endif
	fgSizerDescription->Add( m_textCtrlName, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextDesctiption = new wxStaticText( this, wxID_ANY, wxT("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDesctiption->Wrap( -1 );
	fgSizerDescription->Add( m_staticTextDesctiption, 0, wxALL, 5 );
	
	m_textCtrlDesctription = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !m_textCtrlDesctription->HasFlag( wxTE_MULTILINE ) )
	{
	m_textCtrlDesctription->SetMaxLength(  );
	}
	#else
	m_textCtrlDesctription->SetMaxLength(  );
	#endif
	fgSizerDescription->Add( m_textCtrlDesctription, 0, wxALL|wxEXPAND, 5 );
	
	m_checkBoxActive = new wxCheckBox( this, wxID_ANY, wxT("Active"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxActive->SetValue(true); 
	fgSizerDescription->Add( m_checkBoxActive, 0, wxALL, 5 );
	
	
	fgSizerDescription->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer1->Add( fgSizerDescription, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextLineColour = new wxStaticText( this, wxID_ANY, wxT("Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLineColour->Wrap( -1 );
	fgSizer3->Add( m_staticTextLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerActiveLineColour = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer3->Add( m_colourPickerActiveLineColour, 0, wxALL, 5 );
	
	m_staticTextLineStyle = new wxStaticText( this, wxID_ANY, wxT("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLineStyle->Wrap( -1 );
	fgSizer3->Add( m_staticTextLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceLineStyleChoices[] = { wxT("Solid"), wxT("Dot"), wxT("Long Dash"), wxT("Short Dash"), wxT("Dot Dash") };
	int m_choiceLineStyleNChoices = sizeof( m_choiceLineStyleChoices ) / sizeof( wxString );
	m_choiceLineStyle = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceLineStyleNChoices, m_choiceLineStyleChoices, 0 );
	m_choiceLineStyle->SetSelection( 0 );
	fgSizer3->Add( m_choiceLineStyle, 0, wxALL, 5 );
	
	m_staticTextLineWidth = new wxStaticText( this, wxID_ANY, wxT("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLineWidth->Wrap( -1 );
	fgSizer3->Add( m_staticTextLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceLineWidthChoices[] = { wxT("1 pixel"), wxT("2 pixels"), wxT("3 pixels"), wxT("4 pixels"), wxT("5 pixels"), wxT("6 pixels"), wxT("7 pixels"), wxT("8 pixels"), wxT("9 pixels"), wxT("10 pixels") };
	int m_choiceLineWidthNChoices = sizeof( m_choiceLineWidthChoices ) / sizeof( wxString );
	m_choiceLineWidth = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceLineWidthNChoices, m_choiceLineWidthChoices, 0 );
	m_choiceLineWidth->SetSelection( 0 );
	fgSizer3->Add( m_choiceLineWidth, 0, wxALL, 5 );
	
	
	fgSizer1->Add( fgSizer3, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_buttonOK, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1->Add( m_buttonCancel, 0, wxALL, 5 );
	
	
	fgSizer1->Add( bSizer1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_colourPickerActiveLineColour->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( PILPropertiesDialogDef::OnColourChangedLineColour ), NULL, this );
	m_choiceLineStyle->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PILPropertiesDialogDef::OnChoiceLineStyle ), NULL, this );
	m_choiceLineWidth->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PILPropertiesDialogDef::OnChoiceLineWidth ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PILPropertiesDialogDef::OnOK ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PILPropertiesDialogDef::OnCancel ), NULL, this );
}

PILPropertiesDialogDef::~PILPropertiesDialogDef()
{
	// Disconnect Events
	m_colourPickerActiveLineColour->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( PILPropertiesDialogDef::OnColourChangedLineColour ), NULL, this );
	m_choiceLineStyle->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PILPropertiesDialogDef::OnChoiceLineStyle ), NULL, this );
	m_choiceLineWidth->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PILPropertiesDialogDef::OnChoiceLineWidth ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PILPropertiesDialogDef::OnOK ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PILPropertiesDialogDef::OnCancel ), NULL, this );
	
}
