///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 29 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODPropertiesDialog.h"

///////////////////////////////////////////////////////////////////////////

ODPropertiesDialog::ODPropertiesDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_SizerProperties = new wxBoxSizer( wxVERTICAL );
	
	m_notebookProperties = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelPoint = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerOCPNPoint;
	bSizerOCPNPoint = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerArrivalRadius;
	bSizerArrivalRadius = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText14 = new wxStaticText( m_panelPoint, wxID_ANY, wxT("Arrival Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	bSizerArrivalRadius->Add( m_staticText14, 1, wxALIGN_CENTER|wxALL, 5 );
	
	m_textCtrlODPointArrivalRadius = new wxTextCtrl( m_panelPoint, wxID_ANY, wxT("0.5"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerArrivalRadius->Add( m_textCtrlODPointArrivalRadius, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	bSizerOCPNPoint->Add( bSizerArrivalRadius, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerShowRangeRings;
	bSizerShowRangeRings = new wxBoxSizer( wxHORIZONTAL );
	
	bSizerOCPNPoint->Add( bSizerShowRangeRings, 0, 0, 5 );
	
	m_SizerNameIcon = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxShowName = new wxCheckBox( m_panelPoint, wxID_ANY, wxT("Show Name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_SizerNameIcon->Add( m_checkBoxShowName, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticTextIcon = new wxStaticText( m_panelPoint, wxID_ANY, wxT("Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextIcon->Wrap( -1 );
	m_SizerNameIcon->Add( m_staticTextIcon, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_comboBoxODPointIconName = new wxComboBox( m_panelPoint, wxID_ANY, wxT("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_SizerNameIcon->Add( m_comboBoxODPointIconName, 0, wxALL, 5 );
	
	bSizerOCPNPoint->Add( m_SizerNameIcon, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer3->AddGrowableCol( 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_checkBoxShowODPointRangeRings = new wxCheckBox( m_panelPoint, wxID_ANY, wxT("Show Range Rings"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizer3->Add( m_checkBoxShowODPointRangeRings, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	fgSizer3->Add( bSizer9, 1, wxEXPAND, 5 );
	
	m_staticTextRangeRingNumber = new wxStaticText( m_panelPoint, wxID_ANY, wxT("Number of Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingNumber->Wrap( -1 );
	fgSizer3->Add( m_staticTextRangeRingNumber, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceODPointRangeRingNumberChoices[] = { wxT("None"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10") };
	int m_choiceODPointRangeRingNumberNChoices = sizeof( m_choiceODPointRangeRingNumberChoices ) / sizeof( wxString );
	m_choiceODPointRangeRingNumber = new wxChoice( m_panelPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceODPointRangeRingNumberNChoices, m_choiceODPointRangeRingNumberChoices, 0 );
	m_choiceODPointRangeRingNumber->SetSelection( 0 );
	fgSizer3->Add( m_choiceODPointRangeRingNumber, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextRangeRingSteps = new wxStaticText( m_panelPoint, wxID_ANY, wxT("Distance Between Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingSteps->Wrap( -1 );
	fgSizer3->Add( m_staticTextRangeRingSteps, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlODPointRangeRingSteps = new wxTextCtrl( m_panelPoint, wxID_ANY, wxT("0.5"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_textCtrlODPointRangeRingSteps, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	m_staticTextDistanceUnit = new wxStaticText( m_panelPoint, wxID_ANY, wxT("Distance Unit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDistanceUnit->Wrap( -1 );
	fgSizer3->Add( m_staticTextDistanceUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceODPointDistanceUnitChoices[] = { wxT("Nautical Miles"), wxT("Kilometers") };
	int m_choiceODPointDistanceUnitNChoices = sizeof( m_choiceODPointDistanceUnitChoices ) / sizeof( wxString );
	m_choiceODPointDistanceUnit = new wxChoice( m_panelPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceODPointDistanceUnitNChoices, m_choiceODPointDistanceUnitChoices, 0 );
	m_choiceODPointDistanceUnit->SetSelection( 0 );
	fgSizer3->Add( m_choiceODPointDistanceUnit, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	m_staticTextRangeRingColours = new wxStaticText( m_panelPoint, wxID_ANY, wxT("Range Ring Colours"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingColours->Wrap( -1 );
	fgSizer3->Add( m_staticTextRangeRingColours, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerODPointRangeRingColours = new wxColourPickerCtrl( m_panelPoint, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer3->Add( m_colourPickerODPointRangeRingColours, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	bSizerOCPNPoint->Add( fgSizer3, 0, wxEXPAND, 5 );
	
	m_panelPoint->SetSizer( bSizerOCPNPoint );
	m_panelPoint->Layout();
	bSizerOCPNPoint->Fit( m_panelPoint );
	m_notebookProperties->AddPage( m_panelPoint, wxT("OCPN Point"), true );
	m_panelPath = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerPathSettings;
	fgSizerPathSettings = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizerPathSettings->SetFlexibleDirection( wxBOTH );
	fgSizerPathSettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextActivePathLineColour = new wxStaticText( m_panelPath, wxID_ANY, wxT("Active Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActivePathLineColour->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextActivePathLineColour, 0, wxALL, 5 );
	
	wxString m_choiceActivePathLineColourChoices[] = { wxT("Black"), wxT("Dark Red"), wxT("Dark Green"), wxT("Dark Yellow"), wxT("Dark Blue"), wxT("Dark Magenta"), wxT("Dark Cyan"), wxT("Light Gray"), wxT("Dark Gray"), wxT("Red"), wxT("Green"), wxT("Yellow"), wxT("Blue"), wxT("Magenta"), wxT("Cyan"), wxT("White") };
	int m_choiceActivePathLineColourNChoices = sizeof( m_choiceActivePathLineColourChoices ) / sizeof( wxString );
	m_choiceActivePathLineColour = new wxChoice( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceActivePathLineColourNChoices, m_choiceActivePathLineColourChoices, 0 );
	m_choiceActivePathLineColour->SetSelection( 9 );
	fgSizerPathSettings->Add( m_choiceActivePathLineColour, 0, wxALL, 5 );
	
	m_staticTextInactivePathLineColour = new wxStaticText( m_panelPath, wxID_ANY, wxT("Inactive Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactivePathLineColour->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextInactivePathLineColour, 0, wxALL, 5 );
	
	wxString m_choiceInActivePathLineColourChoices[] = { wxT("Black"), wxT("Dark Red"), wxT("Dark Green"), wxT("Dark Yellow"), wxT("Dark Blue"), wxT("Dark Magenta"), wxT("Dark Cyan"), wxT("Light Gray"), wxT("Dark Gray"), wxT("Red"), wxT("Green"), wxT("Yellow"), wxT("Blue"), wxT("Magenta"), wxT("Cyan"), wxT("White") };
	int m_choiceInActivePathLineColourNChoices = sizeof( m_choiceInActivePathLineColourChoices ) / sizeof( wxString );
	m_choiceInActivePathLineColour = new wxChoice( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceInActivePathLineColourNChoices, m_choiceInActivePathLineColourChoices, 0 );
	m_choiceInActivePathLineColour->SetSelection( 7 );
	fgSizerPathSettings->Add( m_choiceInActivePathLineColour, 0, wxALL, 5 );
	
	m_staticTextPathLineWidth = new wxStaticText( m_panelPath, wxID_ANY, wxT("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPathLineWidth->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextPathLineWidth, 0, wxALL, 5 );
	
	wxString m_choicePathLineWidthChoices[] = { wxT("1 Pixel"), wxT("2 Pixels"), wxT("3 Pixels"), wxT("4 Pixels"), wxT("5 Pixels"), wxT("6 Pixels"), wxT("7 Pixels"), wxT("8 Pixels"), wxT("9 Pixels"), wxT("10 Pixels") };
	int m_choicePathLineWidthNChoices = sizeof( m_choicePathLineWidthChoices ) / sizeof( wxString );
	m_choicePathLineWidth = new wxChoice( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePathLineWidthNChoices, m_choicePathLineWidthChoices, 0 );
	m_choicePathLineWidth->SetSelection( 0 );
	fgSizerPathSettings->Add( m_choicePathLineWidth, 0, wxALL, 5 );
	
	m_staticTextPathLineStyle = new wxStaticText( m_panelPath, wxID_ANY, wxT("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPathLineStyle->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextPathLineStyle, 0, wxALL, 5 );
	
	wxString m_choicePathLineStyleChoices[] = { wxT("Solid"), wxT("Dot"), wxT("Long Dash"), wxT("Short Dash"), wxT("Dot Dash") };
	int m_choicePathLineStyleNChoices = sizeof( m_choicePathLineStyleChoices ) / sizeof( wxString );
	m_choicePathLineStyle = new wxChoice( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePathLineStyleNChoices, m_choicePathLineStyleChoices, 0 );
	m_choicePathLineStyle->SetSelection( 1 );
	fgSizerPathSettings->Add( m_choicePathLineStyle, 0, wxALL, 5 );
	
	m_staticTextActivePathFillColour = new wxStaticText( m_panelPath, wxID_ANY, wxT("Active Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActivePathFillColour->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextActivePathFillColour, 0, wxALL, 5 );
	
	wxString m_choiceActivePathFillColourChoices[] = { wxT("Black"), wxT("Dark Red"), wxT("Dark Green"), wxT("Dark Yellow"), wxT("Dark Blue"), wxT("Dark Magenta"), wxT("Dark Cyan"), wxT("Light Gray"), wxT("Dark Gray"), wxT("Red"), wxT("Green"), wxT("Yellow"), wxT("Blue"), wxT("Magenta"), wxT("Cyan"), wxT("White") };
	int m_choiceActivePathFillColourNChoices = sizeof( m_choiceActivePathFillColourChoices ) / sizeof( wxString );
	m_choiceActivePathFillColour = new wxChoice( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceActivePathFillColourNChoices, m_choiceActivePathFillColourChoices, 0 );
	m_choiceActivePathFillColour->SetSelection( 9 );
	fgSizerPathSettings->Add( m_choiceActivePathFillColour, 0, wxALL, 5 );
	
	m_staticTextInactivePathFillColour = new wxStaticText( m_panelPath, wxID_ANY, wxT("Inactive Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactivePathFillColour->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextInactivePathFillColour, 0, wxALL, 5 );
	
	wxString m_choiceInActivePathFillColourChoices[] = { wxT("Black"), wxT("Dark Red"), wxT("Dark Green"), wxT("Dark Yellow"), wxT("Dark Blue"), wxT("Dark Magenta"), wxT("Dark Cyan"), wxT("Light Gray"), wxT("Dark Gray"), wxT("Red"), wxT("Green"), wxT("Yellow"), wxT("Blue"), wxT("Magenta"), wxT("Cyan"), wxT("White") };
	int m_choiceInActivePathFillColourNChoices = sizeof( m_choiceInActivePathFillColourChoices ) / sizeof( wxString );
	m_choiceInActivePathFillColour = new wxChoice( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceInActivePathFillColourNChoices, m_choiceInActivePathFillColourChoices, 0 );
	m_choiceInActivePathFillColour->SetSelection( 7 );
	fgSizerPathSettings->Add( m_choiceInActivePathFillColour, 0, wxALL, 5 );
	
	m_panelPath->SetSizer( fgSizerPathSettings );
	m_panelPath->Layout();
	fgSizerPathSettings->Fit( m_panelPath );
	m_notebookProperties->AddPage( m_panelPath, wxT("Path"), false );
	m_panelBoundary = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerBoundarySettings;
	fgSizerBoundarySettings = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizerBoundarySettings->SetFlexibleDirection( wxBOTH );
	fgSizerBoundarySettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextActiveBoundayLineColour = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Active Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveBoundayLineColour->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextActiveBoundayLineColour, 0, wxALL, 5 );
	
	wxString m_choiceActiveBoundaryLineColourChoices[] = { wxT("Black"), wxT("Dark Red"), wxT("Dark Green"), wxT("Dark Yellow"), wxT("Dark Blue"), wxT("Dark Magenta"), wxT("Dark Cyan"), wxT("Light Gray"), wxT("Dark Gray"), wxT("Red"), wxT("Green"), wxT("Yellow"), wxT("Blue"), wxT("Magenta"), wxT("Cyan"), wxT("White") };
	int m_choiceActiveBoundaryLineColourNChoices = sizeof( m_choiceActiveBoundaryLineColourChoices ) / sizeof( wxString );
	m_choiceActiveBoundaryLineColour = new wxChoice( m_panelBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceActiveBoundaryLineColourNChoices, m_choiceActiveBoundaryLineColourChoices, 0 );
	m_choiceActiveBoundaryLineColour->SetSelection( 9 );
	fgSizerBoundarySettings->Add( m_choiceActiveBoundaryLineColour, 0, wxALL, 5 );
	
	m_staticTextInactiveBoundaryLineColour = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Inactive Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveBoundaryLineColour->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextInactiveBoundaryLineColour, 0, wxALL, 5 );
	
	wxString m_choiceInActiveBoundaryLineColourChoices[] = { wxT("Black"), wxT("Dark Red"), wxT("Dark Green"), wxT("Dark Yellow"), wxT("Dark Blue"), wxT("Dark Magenta"), wxT("Dark Cyan"), wxT("Light Gray"), wxT("Dark Gray"), wxT("Red"), wxT("Green"), wxT("Yellow"), wxT("Blue"), wxT("Magenta"), wxT("Cyan"), wxT("White") };
	int m_choiceInActiveBoundaryLineColourNChoices = sizeof( m_choiceInActiveBoundaryLineColourChoices ) / sizeof( wxString );
	m_choiceInActiveBoundaryLineColour = new wxChoice( m_panelBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceInActiveBoundaryLineColourNChoices, m_choiceInActiveBoundaryLineColourChoices, 0 );
	m_choiceInActiveBoundaryLineColour->SetSelection( 7 );
	fgSizerBoundarySettings->Add( m_choiceInActiveBoundaryLineColour, 0, wxALL, 5 );
	
	m_staticTextBoundaryLineWidth = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryLineWidth->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextBoundaryLineWidth, 0, wxALL, 5 );
	
	wxString m_choiceBoundaryLineWidthChoices[] = { wxT("1 Pixel"), wxT("2 Pixels"), wxT("3 Pixels"), wxT("4 Pixels"), wxT("5 Pixels"), wxT("6 Pixels"), wxT("7 Pixels"), wxT("8 Pixels"), wxT("9 Pixels"), wxT("10 Pixels") };
	int m_choiceBoundaryLineWidthNChoices = sizeof( m_choiceBoundaryLineWidthChoices ) / sizeof( wxString );
	m_choiceBoundaryLineWidth = new wxChoice( m_panelBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceBoundaryLineWidthNChoices, m_choiceBoundaryLineWidthChoices, 0 );
	m_choiceBoundaryLineWidth->SetSelection( 0 );
	fgSizerBoundarySettings->Add( m_choiceBoundaryLineWidth, 0, wxALL, 5 );
	
	m_staticTextBoundaryLineStyle = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryLineStyle->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextBoundaryLineStyle, 0, wxALL, 5 );
	
	wxString m_choiceBoundaryLineStyleChoices[] = { wxT("Solid"), wxT("Dot"), wxT("Long Dash"), wxT("Short Dash"), wxT("Dot Dash") };
	int m_choiceBoundaryLineStyleNChoices = sizeof( m_choiceBoundaryLineStyleChoices ) / sizeof( wxString );
	m_choiceBoundaryLineStyle = new wxChoice( m_panelBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceBoundaryLineStyleNChoices, m_choiceBoundaryLineStyleChoices, 0 );
	m_choiceBoundaryLineStyle->SetSelection( 0 );
	fgSizerBoundarySettings->Add( m_choiceBoundaryLineStyle, 0, wxALL, 5 );
	
	m_staticTextActiveBoundaryFillColour = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Active Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveBoundaryFillColour->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextActiveBoundaryFillColour, 0, wxALL, 5 );
	
	wxString m_choiceActiveBoundaryFillColourChoices[] = { wxT("Black"), wxT("Dark Red"), wxT("Dark Green"), wxT("Dark Yellow"), wxT("Dark Blue"), wxT("Dark Magenta"), wxT("Dark Cyan"), wxT("Light Gray"), wxT("Dark Gray"), wxT("Red"), wxT("Green"), wxT("Yellow"), wxT("Blue"), wxT("Magenta"), wxT("Cyan"), wxT("White") };
	int m_choiceActiveBoundaryFillColourNChoices = sizeof( m_choiceActiveBoundaryFillColourChoices ) / sizeof( wxString );
	m_choiceActiveBoundaryFillColour = new wxChoice( m_panelBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceActiveBoundaryFillColourNChoices, m_choiceActiveBoundaryFillColourChoices, 0 );
	m_choiceActiveBoundaryFillColour->SetSelection( 9 );
	fgSizerBoundarySettings->Add( m_choiceActiveBoundaryFillColour, 0, wxALL, 5 );
	
	m_staticTextInactiveBoundaryFillColour = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Inactive Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveBoundaryFillColour->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextInactiveBoundaryFillColour, 0, wxALL, 5 );
	
	wxString m_choiceInActiveBoundaryFillColourChoices[] = { wxT("Black"), wxT("Dark Red"), wxT("Dark Green"), wxT("Dark Yellow"), wxT("Dark Blue"), wxT("Dark Magenta"), wxT("Dark Cyan"), wxT("Light Gray"), wxT("Dark Gray"), wxT("Red"), wxT("Green"), wxT("Yellow"), wxT("Blue"), wxT("Magenta"), wxT("Cyan"), wxT("White") };
	int m_choiceInActiveBoundaryFillColourNChoices = sizeof( m_choiceInActiveBoundaryFillColourChoices ) / sizeof( wxString );
	m_choiceInActiveBoundaryFillColour = new wxChoice( m_panelBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceInActiveBoundaryFillColourNChoices, m_choiceInActiveBoundaryFillColourChoices, 0 );
	m_choiceInActiveBoundaryFillColour->SetSelection( 7 );
	fgSizerBoundarySettings->Add( m_choiceInActiveBoundaryFillColour, 0, wxALL, 5 );
	
	m_panelBoundary->SetSizer( fgSizerBoundarySettings );
	m_panelBoundary->Layout();
	fgSizerBoundarySettings->Fit( m_panelBoundary );
	m_notebookProperties->AddPage( m_panelBoundary, wxT("Boundary"), false );
	
	m_SizerProperties->Add( m_notebookProperties, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerOKCancelApply;
	bSizerOKCancelApply = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonOK, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonCancel, 0, wxALL, 5 );
	
	m_buttonAApply = new wxButton( this, wxID_ANY, wxT("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonAApply, 0, wxALL, 5 );
	
	m_SizerProperties->Add( bSizerOKCancelApply, 0, wxEXPAND, 5 );
	
	this->SetSizer( m_SizerProperties );
	this->Layout();
	m_SizerProperties->Fit( this );
	
	// Connect Events
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialog::OnDrawPropertiesOKClick ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialog::OnDrawPropertiesCancelClick ), NULL, this );
	m_buttonAApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialog::OnDrawPropertiesApplyClick ), NULL, this );
}

ODPropertiesDialog::~ODPropertiesDialog()
{
	// Disconnect Events
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialog::OnDrawPropertiesOKClick ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialog::OnDrawPropertiesCancelClick ), NULL, this );
	m_buttonAApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialog::OnDrawPropertiesApplyClick ), NULL, this );
}
