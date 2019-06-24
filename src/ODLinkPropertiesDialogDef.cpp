///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODLinkPropertiesDialogDef.h"

///////////////////////////////////////////////////////////////////////////

ODLinkPropertiesDialogDef::ODLinkPropertiesDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizerLink;
	bSizerLink = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizerLink;
	sbSizerLink = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Link") ), wxVERTICAL );

	m_staticTextLinkDescription = new wxStaticText( sbSizerLink->GetStaticBox(), wxID_ANY, _("Link Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLinkDescription->Wrap( -1 );
	sbSizerLink->Add( m_staticTextLinkDescription, 0, wxALL, 5 );

	m_textCtrlLinkDescription = new wxTextCtrl( sbSizerLink->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerLink->Add( m_textCtrlLinkDescription, 0, wxALL|wxEXPAND, 5 );

	m_staticTextURL = new wxStaticText( sbSizerLink->GetStaticBox(), wxID_ANY, _("URL"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextURL->Wrap( -1 );
	sbSizerLink->Add( m_staticTextURL, 0, wxALL, 5 );

	m_textCtrlURL = new wxTextCtrl( sbSizerLink->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizerLink->Add( m_textCtrlURL, 0, wxALL|wxEXPAND, 5 );

	m_filePickerLocalFile = new wxFilePickerCtrl( sbSizerLink->GetStaticBox(), wxID_ANY, wxEmptyString, _("Local file..."), _("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	sbSizerLink->Add( m_filePickerLocalFile, 0, wxALL, 5 );


	bSizerLink->Add( sbSizerLink, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonOK, 0, wxALL, 5 );

	m_buttonCancel = new wxButton( this, wxID_ANY, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_buttonCancel, 0, wxALL, 5 );


	bSizerLink->Add( bSizer2, 1, wxEXPAND, 5 );


	this->SetSizer( bSizerLink );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_filePickerLocalFile->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ODLinkPropertiesDialogDef::OnFileChanged ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODLinkPropertiesDialogDef::OnOKClick ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODLinkPropertiesDialogDef::OnCancelClick ), NULL, this );
}

ODLinkPropertiesDialogDef::~ODLinkPropertiesDialogDef()
{
	// Disconnect Events
	m_filePickerLocalFile->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ODLinkPropertiesDialogDef::OnFileChanged ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODLinkPropertiesDialogDef::OnOKClick ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODLinkPropertiesDialogDef::OnCancelClick ), NULL, this );

}
