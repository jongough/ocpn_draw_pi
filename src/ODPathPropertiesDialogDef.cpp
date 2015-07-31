///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODPathPropertiesDialogDef.h"

///////////////////////////////////////////////////////////////////////////

ODPathPropertiesDialogDef::ODPathPropertiesDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizerProperties;
	bSizerProperties = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizerNameDescription;
	bSizerNameDescription = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizerDescription;
	fgSizerDescription = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerDescription->AddGrowableCol( 1 );
	fgSizerDescription->SetFlexibleDirection( wxBOTH );
	fgSizerDescription->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextName = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextName->Wrap( -1 );
	fgSizerDescription->Add( m_staticTextName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerDescription->Add( m_textCtrlName, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextDescription = new wxStaticText( this, wxID_ANY, wxT("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDescription->Wrap( -1 );
	fgSizerDescription->Add( m_staticTextDescription, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlDesctiption = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerDescription->Add( m_textCtrlDesctiption, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextGUID = new wxStaticText( this, wxID_ANY, wxT("GUID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGUID->Wrap( -1 );
	fgSizerDescription->Add( m_staticTextGUID, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlGUID = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizerDescription->Add( m_textCtrlGUID, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizerNameDescription->Add( fgSizerDescription, 1, wxEXPAND, 5 );
	
	
	fgSizer1->Add( bSizerNameDescription, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_checkBoxActive = new wxCheckBox( this, wxID_ANY, wxT("Active"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxActive->SetValue(true); 
	fgSizer3->Add( m_checkBoxActive, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticTextTotalLength = new wxStaticText( this, wxID_ANY, wxT("Total Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTotalLength->Wrap( -1 );
	fgSizer3->Add( m_staticTextTotalLength, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlTotalLength = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_textCtrlTotalLength, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticTextLineColour = new wxStaticText( this, wxID_ANY, wxT("Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLineColour->Wrap( -1 );
	fgSizer3->Add( m_staticTextLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerLineColour = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer3->Add( m_colourPickerLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticTextLineStyle = new wxStaticText( this, wxID_ANY, wxT("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLineStyle->Wrap( -1 );
	fgSizer3->Add( m_staticTextLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceLineStyleChoices[] = { wxT("Solid"), wxT("Dot"), wxT("Long Dash"), wxT("Short Dash"), wxT("Dot Dash") };
	int m_choiceLineStyleNChoices = sizeof( m_choiceLineStyleChoices ) / sizeof( wxString );
	m_choiceLineStyle = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceLineStyleNChoices, m_choiceLineStyleChoices, 0 );
	m_choiceLineStyle->SetSelection( 0 );
	fgSizer3->Add( m_choiceLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticTextLineWidth = new wxStaticText( this, wxID_ANY, wxT("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLineWidth->Wrap( -1 );
	fgSizer3->Add( m_staticTextLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceLineWidthChoices[] = { wxT("1 pixel"), wxT("2 pixels"), wxT("3 pixels"), wxT("4 pixels"), wxT("5 pixels"), wxT("6 pixels"), wxT("7 pixels"), wxT("8 pixels"), wxT("9 pixels"), wxT("10 pixels") };
	int m_choiceLineWidthNChoices = sizeof( m_choiceLineWidthChoices ) / sizeof( wxString );
	m_choiceLineWidth = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceLineWidthNChoices, m_choiceLineWidthChoices, 0 );
	m_choiceLineWidth->SetSelection( 1 );
	fgSizer3->Add( m_choiceLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticTextFillColour = new wxStaticText( this, wxID_ANY, wxT("Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFillColour->Wrap( -1 );
	fgSizer3->Add( m_staticTextFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerFillColour = new wxColourPickerCtrl( this, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer3->Add( m_colourPickerFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticTextFillTransparency = new wxStaticText( this, wxID_ANY, wxT("Fill Density"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFillTransparency->Wrap( -1 );
	fgSizer3->Add( m_staticTextFillTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sliderFillTransparency = new wxSlider( this, wxID_ANY, 175, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer3->Add( m_sliderFillTransparency, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer1->Add( fgSizer3, 1, wxEXPAND, 5 );
	
	m_bSizerEBL = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxEBLFixedEndPosition = new wxCheckBox( this, wxID_ANY, wxT("EBL Fixed End Position"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_bSizerEBL->Add( m_checkBoxEBLFixedEndPosition, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_radioBoxEBLPersistenceChoices[] = { wxT("Persistent"), wxT("Persistent over Crash"), wxT("Never") };
	int m_radioBoxEBLPersistenceNChoices = sizeof( m_radioBoxEBLPersistenceChoices ) / sizeof( wxString );
	m_radioBoxEBLPersistence = new wxRadioBox( this, wxID_ANY, wxT("EBL Persistence"), wxDefaultPosition, wxDefaultSize, m_radioBoxEBLPersistenceNChoices, m_radioBoxEBLPersistenceChoices, 3, wxRA_SPECIFY_COLS );
	m_radioBoxEBLPersistence->SetSelection( 0 );
	m_bSizerEBL->Add( m_radioBoxEBLPersistence, 0, wxALL, 5 );
	
	
	fgSizer1->Add( m_bSizerEBL, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerPathPoints;
	bSizerPathPoints = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextODPoints = new wxStaticText( this, wxID_ANY, wxT("Points"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextODPoints->Wrap( -1 );
	bSizerPathPoints->Add( m_staticTextODPoints, 0, wxALL, 5 );
	
	m_listCtrlODPoints = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_EDIT_LABELS|wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
	bSizerPathPoints->Add( m_listCtrlODPoints, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer1->Add( bSizerPathPoints, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerOKCancel;
	bSizerOKCancel = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	bSizerOKCancel->Add( m_buttonOK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancel->Add( m_buttonCancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	fgSizer1->Add( bSizerOKCancel, 1, wxEXPAND, 5 );
	
	
	bSizerProperties->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizerProperties );
	this->Layout();
	bSizerProperties->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODPathPropertiesDialogDef::OnClose ) );
	m_listCtrlODPoints->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( ODPathPropertiesDialogDef::OnLeftDoubleClick ), NULL, this );
	m_listCtrlODPoints->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPathPropertiesDialogDef::OnRightClick ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnOK ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnCancel ), NULL, this );
}

ODPathPropertiesDialogDef::~ODPathPropertiesDialogDef()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODPathPropertiesDialogDef::OnClose ) );
	m_listCtrlODPoints->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( ODPathPropertiesDialogDef::OnLeftDoubleClick ), NULL, this );
	m_listCtrlODPoints->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPathPropertiesDialogDef::OnRightClick ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnOK ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnCancel ), NULL, this );
	
}
