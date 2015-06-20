///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODPointPropertiesDialog.h"

///////////////////////////////////////////////////////////////////////////

ODPointPropertiesDialog::ODPointPropertiesDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_SizerDialogBox = new wxBoxSizer( wxVERTICAL );
	
	m_notebookProperties = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelBasicProperties = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_SizerBasicProperties = new wxBoxSizer( wxVERTICAL );
	
	m_staticText2 = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Properties"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	m_SizerBasicProperties->Add( m_staticText2, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizerInnerProperties;
	bSizerInnerProperties = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bColSpacer;
	bColSpacer = new wxBoxSizer( wxVERTICAL );
	
	bColSpacer->SetMinSize( wxSize( 20,-1 ) ); 
	
	bSizerInnerProperties->Add( bColSpacer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerName;
	bSizerName = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText3 = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Name"), wxPoint( 20,-1 ), wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	bSizerName->Add( m_staticText3, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_textName = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textName->SetMaxLength( 0 ); 
	bSizerName->Add( m_textName, 1, wxALL, 2 );
	
	
	bSizer12->Add( bSizerName, 0, wxEXPAND, 5 );
	
	m_SizerNameIcon = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxShowName = new wxCheckBox( m_panelBasicProperties, wxID_ANY, wxT("Show name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_SizerNameIcon->Add( m_checkBoxShowName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_staticTextIcon = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextIcon->Wrap( -1 );
	m_SizerNameIcon->Add( m_staticTextIcon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_bcomboBoxIcon = new wxBitmapComboBox( m_panelBasicProperties, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_SizerNameIcon->Add( m_bcomboBoxIcon, 0, wxALL, 5 );
	
	
	bSizer12->Add( m_SizerNameIcon, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerLatLon;
	bSizerLatLon = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextLatitude = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLatitude->Wrap( -1 );
	bSizerLatLon->Add( m_staticTextLatitude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_textLatitude = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textLatitude->SetMaxLength( 0 ); 
	bSizerLatLon->Add( m_textLatitude, 1, wxALL, 2 );
	
	m_staticTextLogitude = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Logitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLogitude->Wrap( -1 );
	bSizerLatLon->Add( m_staticTextLogitude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_textLongitude = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textLongitude->SetMaxLength( 0 ); 
	bSizerLatLon->Add( m_textLongitude, 1, wxALL, 2 );
	
	
	bSizer12->Add( bSizerLatLon, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizerArrivalRadius;
	bSizerArrivalRadius = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextArrivalRadius = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Arrival Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextArrivalRadius->Wrap( -1 );
	bSizerArrivalRadius->Add( m_staticTextArrivalRadius, 1, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_textArrivalRadius = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textArrivalRadius->SetMaxLength( 0 ); 
	bSizerArrivalRadius->Add( m_textArrivalRadius, 1, wxALL, 2 );
	
	
	bSizer12->Add( bSizerArrivalRadius, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerSpacer;
	bSizerSpacer = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer12->Add( bSizerSpacer, 0, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	m_checkBoxShowODPointRangeRings = new wxCheckBox( m_panelBasicProperties, wxID_ANY, wxT("Show Point Range Rings"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	bSizer18->Add( m_checkBoxShowODPointRangeRings, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 0 );
	
	
	bSizer12->Add( bSizer18, 0, wxEXPAND, 2 );
	
	m_SizerODPointRangeRingsSelect = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizerODPointRangeRingsSelect;
	fgSizerODPointRangeRingsSelect = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizerODPointRangeRingsSelect->AddGrowableCol( 0 );
	fgSizerODPointRangeRingsSelect->SetFlexibleDirection( wxBOTH );
	fgSizerODPointRangeRingsSelect->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextPointRangeRinngs11 = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Number of Point Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPointRangeRinngs11->Wrap( -1 );
	fgSizerODPointRangeRingsSelect->Add( m_staticTextPointRangeRinngs11, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2 );
	
	wxString m_choicePointRangeRingsNumberChoices[] = { wxT("None"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10") };
	int m_choicePointRangeRingsNumberNChoices = sizeof( m_choicePointRangeRingsNumberChoices ) / sizeof( wxString );
	m_choicePointRangeRingsNumber = new wxChoice( m_panelBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePointRangeRingsNumberNChoices, m_choicePointRangeRingsNumberChoices, 0 );
	m_choicePointRangeRingsNumber->SetSelection( 0 );
	fgSizerODPointRangeRingsSelect->Add( m_choicePointRangeRingsNumber, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	m_SizerODPointRangeRingsSelect->Add( fgSizerODPointRangeRingsSelect, 0, wxALIGN_RIGHT|wxEXPAND, 2 );
	
	m_SizerPointRangeGrid = new wxFlexGridSizer( 3, 2, 0, 0 );
	m_SizerPointRangeGrid->AddGrowableCol( 0 );
	m_SizerPointRangeGrid->SetFlexibleDirection( wxBOTH );
	m_SizerPointRangeGrid->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextPointRangeRingSteps = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Distance between Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPointRangeRingSteps->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextPointRangeRingSteps, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_textCtrlPointRangeRingsSteps = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlPointRangeRingsSteps->SetMaxLength( 0 ); 
	m_SizerPointRangeGrid->Add( m_textCtrlPointRangeRingsSteps, 0, wxALL|wxALIGN_RIGHT, 2 );
	
	m_staticTextDistanceUnit = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Distance Unit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDistanceUnit->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextDistanceUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	wxString m_choiceDistanceUnitsStringChoices[] = { wxT("Nautical Miles"), wxT("Kilometers") };
	int m_choiceDistanceUnitsStringNChoices = sizeof( m_choiceDistanceUnitsStringChoices ) / sizeof( wxString );
	m_choiceDistanceUnitsString = new wxChoice( m_panelBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDistanceUnitsStringNChoices, m_choiceDistanceUnitsStringChoices, 0 );
	m_choiceDistanceUnitsString->SetSelection( 0 );
	m_SizerPointRangeGrid->Add( m_choiceDistanceUnitsString, 0, wxALIGN_RIGHT|wxALL, 0 );
	
	m_staticTextRangeRingColours = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Range Ring Colours"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingColours->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextRangeRingColours, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_colourPickerRangeRingsColour = new wxColourPickerCtrl( m_panelBasicProperties, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, 0 );
	m_SizerPointRangeGrid->Add( m_colourPickerRangeRingsColour, 0, wxALIGN_RIGHT|wxALL, 0 );
	
	
	m_SizerODPointRangeRingsSelect->Add( m_SizerPointRangeGrid, 0, wxEXPAND, 2 );
	
	
	bSizer12->Add( m_SizerODPointRangeRingsSelect, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerDescription;
	bSizerDescription = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextDescription = new wxStaticText( m_panelBasicProperties, wxID_ANY, wxT("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDescription->Wrap( -1 );
	bSizerDescription->Add( m_staticTextDescription, 0, wxALL, 2 );
	
	wxBoxSizer* bSizerDescriptionExt;
	bSizerDescriptionExt = new wxBoxSizer( wxHORIZONTAL );
	
	m_textDescription = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	m_textDescription->SetMinSize( wxSize( -1,60 ) );
	
	bSizerDescriptionExt->Add( m_textDescription, 1, wxALL|wxEXPAND, 2 );
	
	m_buttonExtDescription = new wxButton( m_panelBasicProperties, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize( 20,-1 ), 0 );
	bSizerDescriptionExt->Add( m_buttonExtDescription, 0, wxALL|wxEXPAND, 2 );
	
	
	bSizerDescription->Add( bSizerDescriptionExt, 1, wxEXPAND, 5 );
	
	
	bSizer12->Add( bSizerDescription, 0, wxEXPAND, 5 );
	
	
	bSizerInnerProperties->Add( bSizer12, 1, wxEXPAND, 5 );
	
	
	m_SizerBasicProperties->Add( bSizerInnerProperties, 0, wxEXPAND, 5 );
	
	
	m_panelBasicProperties->SetSizer( m_SizerBasicProperties );
	m_panelBasicProperties->Layout();
	m_SizerBasicProperties->Fit( m_panelBasicProperties );
	m_notebookProperties->AddPage( m_panelBasicProperties, wxT("Basic"), true );
	m_panelDescription = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_textCtrlExtDescription = new wxTextCtrl( m_panelDescription, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	bSizer5->Add( m_textCtrlExtDescription, 1, wxALL|wxEXPAND, 5 );
	
	
	m_panelDescription->SetSizer( bSizer5 );
	m_panelDescription->Layout();
	bSizer5->Fit( m_panelDescription );
	m_notebookProperties->AddPage( m_panelDescription, wxT("Description"), false );
	m_Extended = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_checkBoxVisible = new wxCheckBox( m_Extended, wxID_ANY, wxT("Show on chart"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxVisible->SetValue(true); 
	bSizer3->Add( m_checkBoxVisible, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( m_Extended, wxID_ANY, wxT("GUID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer4->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlGuid = new wxTextCtrl( m_Extended, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlGuid->SetMaxLength( 0 ); 
	bSizer4->Add( m_textCtrlGuid, 1, wxALL, 5 );
	
	
	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );
	
	
	m_Extended->SetSizer( bSizer3 );
	m_Extended->Layout();
	bSizer3->Fit( m_Extended );
	m_notebookProperties->AddPage( m_Extended, wxT("Extended"), false );
	
	m_SizerDialogBox->Add( m_notebookProperties, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_OK = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_OK, 0, wxALL, 5 );
	
	m_Cancel = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Cancel->SetDefault(); 
	bSizer2->Add( m_Cancel, 0, wxALL, 5 );
	
	
	m_SizerDialogBox->Add( bSizer2, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_SizerDialogBox );
	this->Layout();
	m_SizerDialogBox->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODPointPropertiesDialog::OnPointPropertiesClose ) );
	m_textLatitude->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPointPropertiesDialog::onRightClick ), NULL, this );
	m_textLatitude->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnPositionCtlUpdated ), NULL, this );
	m_textLongitude->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPointPropertiesDialog::OnRightClick ), NULL, this );
	m_textLongitude->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnPositionCtlUpdated ), NULL, this );
	m_textArrivalRadius->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnArrivalRadiusChange ), NULL, this );
	m_checkBoxShowODPointRangeRings->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnShowRangeRingsSelect ), NULL, this );
	m_textCtrlPointRangeRingsSteps->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnRangeRingsStepChange ), NULL, this );
	m_textDescription->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ODPointPropertiesDialog::OnDescChangedBasic ), NULL, this );
	m_buttonExtDescription->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnExtDescriptionClick ), NULL, this );
	m_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnPointPropertiesOKClick ), NULL, this );
	m_Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnPointPropertiesCancelClick ), NULL, this );
}

ODPointPropertiesDialog::~ODPointPropertiesDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODPointPropertiesDialog::OnPointPropertiesClose ) );
	m_textLatitude->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPointPropertiesDialog::onRightClick ), NULL, this );
	m_textLatitude->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnPositionCtlUpdated ), NULL, this );
	m_textLongitude->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPointPropertiesDialog::OnRightClick ), NULL, this );
	m_textLongitude->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnPositionCtlUpdated ), NULL, this );
	m_textArrivalRadius->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnArrivalRadiusChange ), NULL, this );
	m_checkBoxShowODPointRangeRings->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnShowRangeRingsSelect ), NULL, this );
	m_textCtrlPointRangeRingsSteps->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnRangeRingsStepChange ), NULL, this );
	m_textDescription->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ODPointPropertiesDialog::OnDescChangedBasic ), NULL, this );
	m_buttonExtDescription->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnExtDescriptionClick ), NULL, this );
	m_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnPointPropertiesOKClick ), NULL, this );
	m_Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnPointPropertiesCancelClick ), NULL, this );
	
}
