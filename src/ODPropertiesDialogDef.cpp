///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODPropertiesDialogDef.h"

///////////////////////////////////////////////////////////////////////////

ODPropertiesDialogDef::ODPropertiesDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_SizerProperties = new wxBoxSizer( wxVERTICAL );

	m_notebookProperties = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelGeneral = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerGeneral;
	bSizerGeneral = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 0 );
	fgSizer4->AddGrowableCol( 1 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxConfirmObjectDelete = new wxCheckBox( m_panelGeneral, wxID_ANY, _("Confirm Object Delete"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxConfirmObjectDelete->SetValue(true);
	fgSizer4->Add( m_checkBoxConfirmObjectDelete, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_checkBoxRememberPropertyPosition = new wxCheckBox( m_panelGeneral, wxID_ANY, _("Remember properties dialogs positions over a restart"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizer4->Add( m_checkBoxRememberPropertyPosition, 0, wxALL, 5 );

	m_checkBoxShowMagBearings = new wxCheckBox( m_panelGeneral, wxID_ANY, _("Show Magnetic Bearings"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizer4->Add( m_checkBoxShowMagBearings, 0, wxALL, 5 );

	m_checkBoxAllowLeftDrag = new wxCheckBox( m_panelGeneral, wxID_ANY, _("Allow Left Mouse Button Drag"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizer4->Add( m_checkBoxAllowLeftDrag, 0, wxALL, 5 );

	m_checkBoxShowLayers = new wxCheckBox( m_panelGeneral, wxID_ANY, _("Show Layers"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizer4->Add( m_checkBoxShowLayers, 0, wxALL, 5 );


	fgSizer4->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticTextNavObjBackups = new wxStaticText( m_panelGeneral, wxID_ANY, _("OCPN_Draw Nav Object Backups"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextNavObjBackups->Wrap( -1 );
	fgSizer4->Add( m_staticTextNavObjBackups, 0, wxALL, 5 );

	m_spinCtrlNavObjBackups = new wxSpinCtrl( m_panelGeneral, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 5 );
	fgSizer4->Add( m_spinCtrlNavObjBackups, 0, wxALL, 5 );

	m_staticTextEdgePan = new wxStaticText( m_panelGeneral, wxID_ANY, _("Edge Panning Sensitivity\n(percentage screen width) Default 5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEdgePan->Wrap( -1 );
	fgSizer4->Add( m_staticTextEdgePan, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_sliderEdgePan = new wxSlider( m_panelGeneral, wxID_ANY, 5, 0, 10, wxDefaultPosition, wxSize( -1,-1 ), wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer4->Add( m_sliderEdgePan, 0, wxALL|wxEXPAND, 5 );

	m_staticTextInitialEdgePan = new wxStaticText( m_panelGeneral, wxID_ANY, _("Initial Edge Panning Sensitivity\n(percentage screen width) Default 3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInitialEdgePan->Wrap( -1 );
	fgSizer4->Add( m_staticTextInitialEdgePan, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_sliderInitialEdgePan = new wxSlider( m_panelGeneral, wxID_ANY, 3, 0, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer4->Add( m_sliderInitialEdgePan, 0, wxALL|wxEXPAND, 5 );

	m_staticTextToolbar = new wxStaticText( m_panelGeneral, wxID_ANY, _("Display Toolbar"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextToolbar->Wrap( -1 );
	fgSizer4->Add( m_staticTextToolbar, 0, wxALL, 5 );

	wxString m_choiceToolbarChoices[] = { _("Never"), _("Whilst Drawing"), _("Always") };
	int m_choiceToolbarNChoices = sizeof( m_choiceToolbarChoices ) / sizeof( wxString );
	m_choiceToolbar = new wxChoice( m_panelGeneral, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceToolbarNChoices, m_choiceToolbarChoices, 0 );
	m_choiceToolbar->SetSelection( 1 );
	fgSizer4->Add( m_choiceToolbar, 0, wxALL, 5 );

	m_staticTextConfigFileEntriesMsg = new wxStaticText( m_panelGeneral, wxID_ANY, _("Remove OCPN_Draw settings and restore to default on restart (No)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextConfigFileEntriesMsg->Wrap( 200 );
	fgSizer4->Add( m_staticTextConfigFileEntriesMsg, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_buttonConfigFileEntries = new wxButton( m_panelGeneral, wxID_ANY, _("Yes"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_buttonConfigFileEntries, 0, wxALL, 5 );


	bSizerGeneral->Add( fgSizer4, 1, wxEXPAND, 5 );


	m_panelGeneral->SetSizer( bSizerGeneral );
	m_panelGeneral->Layout();
	bSizerGeneral->Fit( m_panelGeneral );
	m_notebookProperties->AddPage( m_panelGeneral, _("General"), false );
	m_panelBoundary = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerBoundarySettings;
	fgSizerBoundarySettings = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizerBoundarySettings->SetFlexibleDirection( wxVERTICAL );
	fgSizerBoundarySettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizerBoundarySettingsColours;
	fgSizerBoundarySettingsColours = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizerBoundarySettingsColours->SetFlexibleDirection( wxBOTH );
	fgSizerBoundarySettingsColours->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextActiveBoundayLineColour = new wxStaticText( m_panelBoundary, wxID_ANY, _("Active Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveBoundayLineColour->Wrap( -1 );
	fgSizerBoundarySettingsColours->Add( m_staticTextActiveBoundayLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerActiveBoundaryLineColour = new wxColourPickerCtrl( m_panelBoundary, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerBoundarySettingsColours->Add( m_colourPickerActiveBoundaryLineColour, 0, wxALL, 5 );

	m_staticTextActiveBoundaryFillColour = new wxStaticText( m_panelBoundary, wxID_ANY, _("Active Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveBoundaryFillColour->Wrap( -1 );
	fgSizerBoundarySettingsColours->Add( m_staticTextActiveBoundaryFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerActiveBoundaryFillColour = new wxColourPickerCtrl( m_panelBoundary, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerBoundarySettingsColours->Add( m_colourPickerActiveBoundaryFillColour, 0, wxALL, 5 );

	m_staticTextInactiveBoundaryLineColour = new wxStaticText( m_panelBoundary, wxID_ANY, _("Inactive Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveBoundaryLineColour->Wrap( -1 );
	fgSizerBoundarySettingsColours->Add( m_staticTextInactiveBoundaryLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerInActiveBoundaryLineColour = new wxColourPickerCtrl( m_panelBoundary, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerBoundarySettingsColours->Add( m_colourPickerInActiveBoundaryLineColour, 0, wxALL, 5 );

	m_staticTextInactiveBoundaryFillColour = new wxStaticText( m_panelBoundary, wxID_ANY, _("Inactive Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveBoundaryFillColour->Wrap( -1 );
	fgSizerBoundarySettingsColours->Add( m_staticTextInactiveBoundaryFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerInActiveBoundaryFillColour = new wxColourPickerCtrl( m_panelBoundary, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerBoundarySettingsColours->Add( m_colourPickerInActiveBoundaryFillColour, 0, wxALL, 5 );

	m_staticTextBoundaryLineWidth = new wxStaticText( m_panelBoundary, wxID_ANY, _("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryLineWidth->Wrap( -1 );
	fgSizerBoundarySettingsColours->Add( m_staticTextBoundaryLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceBoundaryLineWidthChoices[] = { _("1 Pixel"), _("2 Pixels"), _("3 Pixels"), _("4 Pixels"), _("5 Pixels"), _("6 Pixels"), _("7 Pixels"), _("8 Pixels"), _("9 Pixels"), _("10 Pixels") };
	int m_choiceBoundaryLineWidthNChoices = sizeof( m_choiceBoundaryLineWidthChoices ) / sizeof( wxString );
	m_choiceBoundaryLineWidth = new wxChoice( m_panelBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceBoundaryLineWidthNChoices, m_choiceBoundaryLineWidthChoices, 0 );
	m_choiceBoundaryLineWidth->SetSelection( 0 );
	fgSizerBoundarySettingsColours->Add( m_choiceBoundaryLineWidth, 0, wxALL, 5 );

	m_staticTextBoundaryLineStyle = new wxStaticText( m_panelBoundary, wxID_ANY, _("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryLineStyle->Wrap( -1 );
	fgSizerBoundarySettingsColours->Add( m_staticTextBoundaryLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceBoundaryLineStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choiceBoundaryLineStyleNChoices = sizeof( m_choiceBoundaryLineStyleChoices ) / sizeof( wxString );
	m_choiceBoundaryLineStyle = new wxChoice( m_panelBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceBoundaryLineStyleNChoices, m_choiceBoundaryLineStyleChoices, 0 );
	m_choiceBoundaryLineStyle->SetSelection( 0 );
	fgSizerBoundarySettingsColours->Add( m_choiceBoundaryLineStyle, 0, wxALL, 5 );

	m_staticTextFillTransparency = new wxStaticText( m_panelBoundary, wxID_ANY, _("Fill Density"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFillTransparency->Wrap( -1 );
	fgSizerBoundarySettingsColours->Add( m_staticTextFillTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_sliderFillTransparency = new wxSlider( m_panelBoundary, wxID_ANY, 175, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizerBoundarySettingsColours->Add( m_sliderFillTransparency, 0, wxALL|wxEXPAND, 5 );

	m_staticTextInclusionBoundarySize = new wxStaticText( m_panelBoundary, wxID_ANY, _("Inclusion Boundary Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInclusionBoundarySize->Wrap( -1 );
	fgSizerBoundarySettingsColours->Add( m_staticTextInclusionBoundarySize, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_sliderInclusionBoundarySize = new wxSlider( m_panelBoundary, wxID_ANY, 15, 0, 50, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizerBoundarySettingsColours->Add( m_sliderInclusionBoundarySize, 1, wxALL|wxEXPAND, 5 );


	fgSizerBoundarySettings->Add( fgSizerBoundarySettingsColours, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerBoundaryType;
	fgSizerBoundaryType = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerBoundaryType->SetFlexibleDirection( wxBOTH );
	fgSizerBoundaryType->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxString m_radioBoxBoundaryTypeChoices[] = { _("Exclusion"), _("Inclusion"), _("Neither") };
	int m_radioBoxBoundaryTypeNChoices = sizeof( m_radioBoxBoundaryTypeChoices ) / sizeof( wxString );
	m_radioBoxBoundaryType = new wxRadioBox( m_panelBoundary, wxID_ANY, _("Boundary Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxBoundaryTypeNChoices, m_radioBoxBoundaryTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxBoundaryType->SetSelection( 0 );
	fgSizerBoundaryType->Add( m_radioBoxBoundaryType, 0, wxALL, 5 );

	m_checkBoxBoundaryODPointsVisible = new wxCheckBox( m_panelBoundary, wxID_ANY, _("Show Boundary Point Icons"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxBoundaryODPointsVisible->SetValue(true);
	fgSizerBoundaryType->Add( m_checkBoxBoundaryODPointsVisible, 0, wxALL, 5 );


	fgSizerBoundarySettings->Add( fgSizerBoundaryType, 1, wxEXPAND, 5 );


	m_panelBoundary->SetSizer( fgSizerBoundarySettings );
	m_panelBoundary->Layout();
	fgSizerBoundarySettings->Fit( m_panelBoundary );
	m_notebookProperties->AddPage( m_panelBoundary, _("Boundary"), false );
	m_panelBoundaryPoint = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerOCPNPoint;
	bSizerOCPNPoint = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerArrivalRadius;
	bSizerArrivalRadius = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextArrivalRadius = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, _("Arrival Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextArrivalRadius->Wrap( -1 );
	bSizerArrivalRadius->Add( m_staticTextArrivalRadius, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlODPointArrivalRadius = new wxTextCtrl( m_panelBoundaryPoint, wxID_ANY, _("0.5"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerArrivalRadius->Add( m_textCtrlODPointArrivalRadius, 0, wxALL, 5 );


	bSizerOCPNPoint->Add( bSizerArrivalRadius, 0, wxEXPAND, 5 );

	m_SizerNameIcon = new wxBoxSizer( wxHORIZONTAL );

	m_fgSizerODPointIcon = new wxFlexGridSizer( 0, 3, 0, 0 );
	m_fgSizerODPointIcon->AddGrowableCol( 0 );
	m_fgSizerODPointIcon->AddGrowableCol( 2 );
	m_fgSizerODPointIcon->SetFlexibleDirection( wxBOTH );
	m_fgSizerODPointIcon->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxBoundaryPointShowName = new wxCheckBox( m_panelBoundaryPoint, wxID_ANY, _("Show Name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerODPointIcon->Add( m_checkBoxBoundaryPointShowName, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 5 );

	m_staticTextBoundaryPointIcon = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, _("Boundary Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryPointIcon->Wrap( -1 );
	m_fgSizerODPointIcon->Add( m_staticTextBoundaryPointIcon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_bcomboBoxODPointIconName = new wxBitmapComboBox( m_panelBoundaryPoint, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_fgSizerODPointIcon->Add( m_bcomboBoxODPointIconName, 1, wxALL|wxEXPAND, 5 );


	m_SizerNameIcon->Add( m_fgSizerODPointIcon, 1, wxEXPAND, 5 );


	bSizerOCPNPoint->Add( m_SizerNameIcon, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerRingsShow;
	fgSizerRingsShow = new wxFlexGridSizer( 5, 2, 0, 0 );
	fgSizerRingsShow->AddGrowableCol( 0 );
	fgSizerRingsShow->AddGrowableCol( 1 );
	fgSizerRingsShow->SetFlexibleDirection( wxBOTH );
	fgSizerRingsShow->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxShowODPointRangeRings = new wxCheckBox( m_panelBoundaryPoint, wxID_ANY, _("Show Range Rings"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizerRingsShow->Add( m_checkBoxShowODPointRangeRings, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_radioBoxBoundaryPointTypeChoices[] = { _("Exclusion"), _("Inclusion"), _("Neither") };
	int m_radioBoxBoundaryPointTypeNChoices = sizeof( m_radioBoxBoundaryPointTypeChoices ) / sizeof( wxString );
	m_radioBoxBoundaryPointType = new wxRadioBox( m_panelBoundaryPoint, wxID_ANY, _("Boundary Point Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxBoundaryPointTypeNChoices, m_radioBoxBoundaryPointTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxBoundaryPointType->SetSelection( 0 );
	fgSizerRingsShow->Add( m_radioBoxBoundaryPointType, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_fgSizerBoundaryPointFillDensity = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerBoundaryPointFillDensity->AddGrowableCol( 0 );
	m_fgSizerBoundaryPointFillDensity->AddGrowableCol( 1 );
	m_fgSizerBoundaryPointFillDensity->SetFlexibleDirection( wxBOTH );
	m_fgSizerBoundaryPointFillDensity->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextBoundaryPointFillDensity = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, _("Fill Density"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryPointFillDensity->Wrap( -1 );
	m_fgSizerBoundaryPointFillDensity->Add( m_staticTextBoundaryPointFillDensity, 0, wxALL|wxEXPAND, 5 );

	m_sliderBoundaryPointFillTransparency = new wxSlider( m_panelBoundaryPoint, wxID_ANY, 175, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_fgSizerBoundaryPointFillDensity->Add( m_sliderBoundaryPointFillTransparency, 1, wxEXPAND, 5 );


	fgSizerRingsShow->Add( m_fgSizerBoundaryPointFillDensity, 1, wxEXPAND|wxRIGHT, 5 );

	m_fgSizerBoundaryPointSize = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerBoundaryPointSize->AddGrowableCol( 0 );
	m_fgSizerBoundaryPointSize->AddGrowableCol( 1 );
	m_fgSizerBoundaryPointSize->SetFlexibleDirection( wxBOTH );
	m_fgSizerBoundaryPointSize->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextBoundaryPointSize = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, _("Boundary Point Inclusion\nSize"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryPointSize->Wrap( -1 );
	m_fgSizerBoundaryPointSize->Add( m_staticTextBoundaryPointSize, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_sliderInclusionBoundaryPointSize = new wxSlider( m_panelBoundaryPoint, wxID_ANY, 15, 0, 50, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_fgSizerBoundaryPointSize->Add( m_sliderInclusionBoundaryPointSize, 1, wxALL|wxEXPAND, 5 );


	fgSizerRingsShow->Add( m_fgSizerBoundaryPointSize, 1, wxEXPAND, 5 );


	fgSizerRingsShow->Add( 0, 0, 1, wxEXPAND, 5 );


	bSizerOCPNPoint->Add( fgSizerRingsShow, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerRingsDetail;
	fgSizerRingsDetail = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerRingsDetail->AddGrowableCol( 0 );
	fgSizerRingsDetail->SetFlexibleDirection( wxBOTH );
	fgSizerRingsDetail->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextRangeRingNumber = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, _("Number of Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingNumber->Wrap( -1 );
	fgSizerRingsDetail->Add( m_staticTextRangeRingNumber, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceODPointRangeRingNumberChoices[] = { _("None"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9"), _("10") };
	int m_choiceODPointRangeRingNumberNChoices = sizeof( m_choiceODPointRangeRingNumberChoices ) / sizeof( wxString );
	m_choiceODPointRangeRingNumber = new wxChoice( m_panelBoundaryPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceODPointRangeRingNumberNChoices, m_choiceODPointRangeRingNumberChoices, 0 );
	m_choiceODPointRangeRingNumber->SetSelection( 0 );
	fgSizerRingsDetail->Add( m_choiceODPointRangeRingNumber, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextRangeRingSteps = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, _("Distance Between Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingSteps->Wrap( -1 );
	fgSizerRingsDetail->Add( m_staticTextRangeRingSteps, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlODPointRangeRingSteps = new wxTextCtrl( m_panelBoundaryPoint, wxID_ANY, _("0.5"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerRingsDetail->Add( m_textCtrlODPointRangeRingSteps, 0, wxALL|wxEXPAND, 5 );

	m_staticTextDistanceUnit = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, _("Distance Unit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDistanceUnit->Wrap( -1 );
	fgSizerRingsDetail->Add( m_staticTextDistanceUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceODPointDistanceUnitChoices[] = { _("Nautical Miles"), _("Kilometres") };
	int m_choiceODPointDistanceUnitNChoices = sizeof( m_choiceODPointDistanceUnitChoices ) / sizeof( wxString );
	m_choiceODPointDistanceUnit = new wxChoice( m_panelBoundaryPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceODPointDistanceUnitNChoices, m_choiceODPointDistanceUnitChoices, 0 );
	m_choiceODPointDistanceUnit->SetSelection( 0 );
	fgSizerRingsDetail->Add( m_choiceODPointDistanceUnit, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextRangeRingColours = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, _("Range Ring Colours"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingColours->Wrap( -1 );
	fgSizerRingsDetail->Add( m_staticTextRangeRingColours, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerODPointRangeRingColours = new wxColourPickerCtrl( m_panelBoundaryPoint, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerRingsDetail->Add( m_colourPickerODPointRangeRingColours, 0, wxALL|wxALIGN_RIGHT, 5 );

	m_staticTextRangeRingWidth = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, _("Range Ring Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingWidth->Wrap( -1 );
	fgSizerRingsDetail->Add( m_staticTextRangeRingWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceRangeRingWidthChoices[] = { _("1 pixel"), _("2 pixels"), _("3 pixels"), _("4 pixels"), _("5 pixels"), _("6 pixels"), _("7 pixels"), _("8 pixels"), _("9 pixels"), _("10 pixels") };
	int m_choiceRangeRingWidthNChoices = sizeof( m_choiceRangeRingWidthChoices ) / sizeof( wxString );
	m_choiceRangeRingWidth = new wxChoice( m_panelBoundaryPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRangeRingWidthNChoices, m_choiceRangeRingWidthChoices, 0 );
	m_choiceRangeRingWidth->SetSelection( 1 );
	fgSizerRingsDetail->Add( m_choiceRangeRingWidth, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextRangeRingStyle = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, _("Range Ring Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingStyle->Wrap( -1 );
	fgSizerRingsDetail->Add( m_staticTextRangeRingStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceRangeRingStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choiceRangeRingStyleNChoices = sizeof( m_choiceRangeRingStyleChoices ) / sizeof( wxString );
	m_choiceRangeRingStyle = new wxChoice( m_panelBoundaryPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRangeRingStyleNChoices, m_choiceRangeRingStyleChoices, 0 );
	m_choiceRangeRingStyle->SetSelection( 0 );
	fgSizerRingsDetail->Add( m_choiceRangeRingStyle, 0, wxALIGN_RIGHT|wxALL, 5 );


	bSizerOCPNPoint->Add( fgSizerRingsDetail, 0, wxEXPAND, 5 );


	m_panelBoundaryPoint->SetSizer( bSizerOCPNPoint );
	m_panelBoundaryPoint->Layout();
	bSizerOCPNPoint->Fit( m_panelBoundaryPoint );
	m_notebookProperties->AddPage( m_panelBoundaryPoint, _("Boundary Point"), false );
	m_panelTextPoint = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerTextPoint;
	fgSizerTextPoint = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizerTextPoint->AddGrowableCol( 0 );
	fgSizerTextPoint->SetFlexibleDirection( wxBOTH );
	fgSizerTextPoint->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer21;
	fgSizer21 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer21->AddGrowableCol( 1 );
	fgSizer21->SetFlexibleDirection( wxBOTH );
	fgSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxTextPointShowName = new wxCheckBox( m_panelTextPoint, wxID_ANY, _("Show Name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizer21->Add( m_checkBoxTextPointShowName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_fgSizerTextPointIconName = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerTextPointIconName->AddGrowableCol( 1 );
	m_fgSizerTextPointIconName->SetFlexibleDirection( wxBOTH );
	m_fgSizerTextPointIconName->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextTextPointIconName = new wxStaticText( m_panelTextPoint, wxID_ANY, _("Text Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTextPointIconName->Wrap( -1 );
	m_fgSizerTextPointIconName->Add( m_staticTextTextPointIconName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_bcomboBoxTextPointIconName = new wxBitmapComboBox( m_panelTextPoint, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_fgSizerTextPointIconName->Add( m_bcomboBoxTextPointIconName, 1, wxALL|wxEXPAND, 5 );


	fgSizer21->Add( m_fgSizerTextPointIconName, 1, wxALL|wxEXPAND, 5 );

	m_staticTextPosition = new wxStaticText( m_panelTextPoint, wxID_ANY, _("Text position relative to point"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPosition->Wrap( -1 );
	fgSizer21->Add( m_staticTextPosition, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceTextPositionChoices[] = { _("Top"), _("Top Centre"), _("Bottom"), _("Bottom Centre"), _("Centre"), _("Right"), _("Left") };
	int m_choiceTextPositionNChoices = sizeof( m_choiceTextPositionChoices ) / sizeof( wxString );
	m_choiceTextPosition = new wxChoice( m_panelTextPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceTextPositionNChoices, m_choiceTextPositionChoices, 0 );
	m_choiceTextPosition->SetSelection( 1 );
	fgSizer21->Add( m_choiceTextPosition, 0, wxALL, 5 );

	m_staticTextColour = new wxStaticText( m_panelTextPoint, wxID_ANY, _("Text Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextColour->Wrap( -1 );
	fgSizer21->Add( m_staticTextColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerTextColour = new wxColourPickerCtrl( m_panelTextPoint, wxID_ANY, wxColour( 0, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer21->Add( m_colourPickerTextColour, 0, wxALL, 5 );

	m_staticTextBackgroundColour = new wxStaticText( m_panelTextPoint, wxID_ANY, _("Background Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBackgroundColour->Wrap( -1 );
	fgSizer21->Add( m_staticTextBackgroundColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerBackgroundColour = new wxColourPickerCtrl( m_panelTextPoint, wxID_ANY, wxColour( 255, 255, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer21->Add( m_colourPickerBackgroundColour, 0, wxALL, 5 );

	m_staticTextBackgroundTransparency = new wxStaticText( m_panelTextPoint, wxID_ANY, _("Background Density"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBackgroundTransparency->Wrap( -1 );
	fgSizer21->Add( m_staticTextBackgroundTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxFlexGridSizer* fgSizer47;
	fgSizer47 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer47->AddGrowableCol( 0 );
	fgSizer47->AddGrowableCol( 1 );
	fgSizer47->SetFlexibleDirection( wxBOTH );
	fgSizer47->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_sliderBackgroundTransparency = new wxSlider( m_panelTextPoint, wxID_ANY, 175, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer47->Add( m_sliderBackgroundTransparency, 0, wxALL|wxEXPAND, 5 );


	fgSizer47->Add( 0, 0, 1, wxEXPAND, 5 );


	fgSizer21->Add( fgSizer47, 1, wxEXPAND, 5 );

	wxString m_radioBoxWidthTypeChoices[] = { _("Font Based"), _("Character Based") };
	int m_radioBoxWidthTypeNChoices = sizeof( m_radioBoxWidthTypeChoices ) / sizeof( wxString );
	m_radioBoxWidthType = new wxRadioBox( m_panelTextPoint, wxID_ANY, _("Maximum Width Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxWidthTypeNChoices, m_radioBoxWidthTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxWidthType->SetSelection( 0 );
	fgSizer21->Add( m_radioBoxWidthType, 0, wxALL, 5 );


	fgSizer21->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticTextMaxWidth = new wxStaticText( m_panelTextPoint, wxID_ANY, _("Maximum Text Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMaxWidth->Wrap( -1 );
	fgSizer21->Add( m_staticTextMaxWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlTextMaxWidth = new wxTextCtrl( m_panelTextPoint, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer21->Add( m_textCtrlTextMaxWidth, 0, wxALL, 5 );


	fgSizerTextPoint->Add( fgSizer21, 1, wxEXPAND, 5 );

	m_fgSizerTextPointFont = new wxFlexGridSizer( 0, 3, 0, 0 );
	m_fgSizerTextPointFont->AddGrowableCol( 1 );
	m_fgSizerTextPointFont->SetFlexibleDirection( wxBOTH );
	m_fgSizerTextPointFont->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextTextFont = new wxStaticText( m_panelTextPoint, wxID_ANY, _("Text Font"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTextFont->Wrap( -1 );
	m_fgSizerTextPointFont->Add( m_staticTextTextFont, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextFontFaceExample = new wxStaticText( m_panelTextPoint, wxID_ANY, _("Example"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFontFaceExample->Wrap( -1 );
	m_fgSizerTextPointFont->Add( m_staticTextFontFaceExample, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_buttonTextFont = new wxButton( m_panelTextPoint, wxID_ANY, _("Fonts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fgSizerTextPointFont->Add( m_buttonTextFont, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	fgSizerTextPoint->Add( m_fgSizerTextPointFont, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	wxString m_radioBoxShowDisplayTextChoices[] = { _("Always"), _("On Rollover Only"), _("Never") };
	int m_radioBoxShowDisplayTextNChoices = sizeof( m_radioBoxShowDisplayTextChoices ) / sizeof( wxString );
	m_radioBoxShowDisplayText = new wxRadioBox( m_panelTextPoint, wxID_ANY, _("Show Display Text"), wxDefaultPosition, wxDefaultSize, m_radioBoxShowDisplayTextNChoices, m_radioBoxShowDisplayTextChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxShowDisplayText->SetSelection( 0 );
	bSizer11->Add( m_radioBoxShowDisplayText, 0, wxALL, 5 );


	fgSizerTextPoint->Add( bSizer11, 1, wxEXPAND, 5 );


	m_panelTextPoint->SetSizer( fgSizerTextPoint );
	m_panelTextPoint->Layout();
	fgSizerTextPoint->Fit( m_panelTextPoint );
	m_notebookProperties->AddPage( m_panelTextPoint, _("Text Point"), false );
	m_panelPath = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelPath->Enable( false );
	m_panelPath->Hide();

	wxFlexGridSizer* fgSizerPathSettings;
	fgSizerPathSettings = new wxFlexGridSizer( 6, 4, 0, 0 );
	fgSizerPathSettings->SetFlexibleDirection( wxBOTH );
	fgSizerPathSettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextActivePathLineColour = new wxStaticText( m_panelPath, wxID_ANY, _("Active Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActivePathLineColour->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextActivePathLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerActivePathLineColour = new wxColourPickerCtrl( m_panelPath, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerPathSettings->Add( m_colourPickerActivePathLineColour, 0, wxALL, 5 );

	m_staticTextInactivePathLineColour = new wxStaticText( m_panelPath, wxID_ANY, _("Inactive Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactivePathLineColour->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextInactivePathLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerInActivePathLineColour = new wxColourPickerCtrl( m_panelPath, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerPathSettings->Add( m_colourPickerInActivePathLineColour, 0, wxALL, 5 );

	m_staticTextPathLineWidth = new wxStaticText( m_panelPath, wxID_ANY, _("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPathLineWidth->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextPathLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choicePathLineWidthChoices[] = { _("1 Pixel"), _("2 Pixels"), _("3 Pixels"), _("4 Pixels"), _("5 Pixels"), _("6 Pixels"), _("7 Pixels"), _("8 Pixels"), _("9 Pixels"), _("10 Pixels") };
	int m_choicePathLineWidthNChoices = sizeof( m_choicePathLineWidthChoices ) / sizeof( wxString );
	m_choicePathLineWidth = new wxChoice( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePathLineWidthNChoices, m_choicePathLineWidthChoices, 0 );
	m_choicePathLineWidth->SetSelection( 0 );
	fgSizerPathSettings->Add( m_choicePathLineWidth, 0, wxALL, 5 );

	m_staticTextPathLineStyle = new wxStaticText( m_panelPath, wxID_ANY, _("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPathLineStyle->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextPathLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choicePathLineStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choicePathLineStyleNChoices = sizeof( m_choicePathLineStyleChoices ) / sizeof( wxString );
	m_choicePathLineStyle = new wxChoice( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePathLineStyleNChoices, m_choicePathLineStyleChoices, 0 );
	m_choicePathLineStyle->SetSelection( 1 );
	fgSizerPathSettings->Add( m_choicePathLineStyle, 0, wxALL, 5 );


	m_panelPath->SetSizer( fgSizerPathSettings );
	m_panelPath->Layout();
	fgSizerPathSettings->Fit( m_panelPath );
	m_notebookProperties->AddPage( m_panelPath, _("Path"), false );
	m_panelEBL = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_fgSizerEBLSettings = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerEBLSettings->AddGrowableCol( 1 );
	m_fgSizerEBLSettings->SetFlexibleDirection( wxBOTH );
	m_fgSizerEBLSettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextStartPointIcon = new wxStaticText( m_panelEBL, wxID_ANY, _("Start Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextStartPointIcon->Wrap( -1 );
	m_fgSizerEBLSettings->Add( m_staticTextStartPointIcon, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_fgSizerEBLStartIconName = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgSizerEBLStartIconName->AddGrowableCol( 0 );
	m_fgSizerEBLStartIconName->SetFlexibleDirection( wxBOTH );
	m_fgSizerEBLStartIconName->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_bcomboBoxEBLStartIconName = new wxBitmapComboBox( m_panelEBL, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_fgSizerEBLStartIconName->Add( m_bcomboBoxEBLStartIconName, 0, wxALL|wxEXPAND, 5 );


	m_fgSizerEBLSettings->Add( m_fgSizerEBLStartIconName, 1, wxEXPAND, 5 );

	m_staticTextEndPointIcon = new wxStaticText( m_panelEBL, wxID_ANY, _("End Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEndPointIcon->Wrap( -1 );
	m_fgSizerEBLSettings->Add( m_staticTextEndPointIcon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_fgSizerEBLEndPointIcon = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgSizerEBLEndPointIcon->AddGrowableCol( 0 );
	m_fgSizerEBLEndPointIcon->SetFlexibleDirection( wxBOTH );
	m_fgSizerEBLEndPointIcon->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_bcomboBoxEBLEndIconName = new wxBitmapComboBox( m_panelEBL, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_fgSizerEBLEndPointIcon->Add( m_bcomboBoxEBLEndIconName, 0, wxALL|wxEXPAND, 5 );


	m_fgSizerEBLSettings->Add( m_fgSizerEBLEndPointIcon, 1, wxEXPAND, 5 );

	m_staticTextActiveEBLLineColour = new wxStaticText( m_panelEBL, wxID_ANY, _("Active EBL Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveEBLLineColour->Wrap( -1 );
	m_fgSizerEBLSettings->Add( m_staticTextActiveEBLLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerEBLLineColour = new wxColourPickerCtrl( m_panelEBL, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_fgSizerEBLSettings->Add( m_colourPickerEBLLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextInactiveEBLLineColour = new wxStaticText( m_panelEBL, wxID_ANY, _("Inactive EBL Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveEBLLineColour->Wrap( -1 );
	m_fgSizerEBLSettings->Add( m_staticTextInactiveEBLLineColour, 0, wxALL, 5 );

	m_colourPickerInActiveEBLLineColour = new wxColourPickerCtrl( m_panelEBL, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_fgSizerEBLSettings->Add( m_colourPickerInActiveEBLLineColour, 0, wxALL, 5 );

	m_staticTextEBLLineWidth = new wxStaticText( m_panelEBL, wxID_ANY, _("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEBLLineWidth->Wrap( -1 );
	m_fgSizerEBLSettings->Add( m_staticTextEBLLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceEBLLineWidthChoices[] = { _("1 Pixel"), _("2 Pixels"), _("3 Pixels"), _("4 Pixels"), _("5 Pixels"), _("6 Pixels"), _("7 Pixels"), _("8 Pixels"), _("9 Pixels"), _("10 Pixels") };
	int m_choiceEBLLineWidthNChoices = sizeof( m_choiceEBLLineWidthChoices ) / sizeof( wxString );
	m_choiceEBLLineWidth = new wxChoice( m_panelEBL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceEBLLineWidthNChoices, m_choiceEBLLineWidthChoices, 0 );
	m_choiceEBLLineWidth->SetSelection( 0 );
	m_fgSizerEBLSettings->Add( m_choiceEBLLineWidth, 0, wxALL, 5 );

	m_staticTextEBLLineStyle = new wxStaticText( m_panelEBL, wxID_ANY, _("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEBLLineStyle->Wrap( -1 );
	m_fgSizerEBLSettings->Add( m_staticTextEBLLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceEBLLineStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choiceEBLLineStyleNChoices = sizeof( m_choiceEBLLineStyleChoices ) / sizeof( wxString );
	m_choiceEBLLineStyle = new wxChoice( m_panelEBL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceEBLLineStyleNChoices, m_choiceEBLLineStyleChoices, 0 );
	m_choiceEBLLineStyle->SetSelection( 0 );
	m_fgSizerEBLSettings->Add( m_choiceEBLLineStyle, 0, wxALL, 5 );

	m_checkBoxRotateWithBoat = new wxCheckBox( m_panelEBL, wxID_ANY, _("Rotate with Boat"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBLSettings->Add( m_checkBoxRotateWithBoat, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_radioBoxMaintainWithChoices[] = { _("Heading"), _("Course over Ground") };
	int m_radioBoxMaintainWithNChoices = sizeof( m_radioBoxMaintainWithChoices ) / sizeof( wxString );
	m_radioBoxMaintainWith = new wxRadioBox( m_panelEBL, wxID_ANY, _("Maintain with"), wxDefaultPosition, wxDefaultSize, m_radioBoxMaintainWithNChoices, m_radioBoxMaintainWithChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxMaintainWith->SetSelection( 0 );
	m_fgSizerEBLSettings->Add( m_radioBoxMaintainWith, 0, wxALL, 5 );

	m_checkBoxEBLFixedEndPosition = new wxCheckBox( m_panelEBL, wxID_ANY, _("EBL Fixed End Position"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBLSettings->Add( m_checkBoxEBLFixedEndPosition, 0, wxALL, 5 );

	m_checkBoxEBLShowArrow = new wxCheckBox( m_panelEBL, wxID_ANY, _("Show EBL Direction Arrow"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBLSettings->Add( m_checkBoxEBLShowArrow, 0, wxALL, 5 );

	m_checkBoxShowVRM = new wxCheckBox( m_panelEBL, wxID_ANY, _("Show VRM"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBLSettings->Add( m_checkBoxShowVRM, 0, wxALL, 5 );

	m_checkBoxShowPerpLine = new wxCheckBox( m_panelEBL, wxID_ANY, _("Show Perpendicular Index Line"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBLSettings->Add( m_checkBoxShowPerpLine, 0, wxALL, 5 );

	m_checkBoxEBLAllwaysShowInfo = new wxCheckBox( m_panelEBL, wxID_ANY, _("Always Show\nEBL Info"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBLSettings->Add( m_checkBoxEBLAllwaysShowInfo, 0, wxALL, 5 );

	wxString m_radioBoxEBLPersistenceChoices[] = { _("Persistent"), _("Persistent over Crash"), _("Never") };
	int m_radioBoxEBLPersistenceNChoices = sizeof( m_radioBoxEBLPersistenceChoices ) / sizeof( wxString );
	m_radioBoxEBLPersistence = new wxRadioBox( m_panelEBL, wxID_ANY, _("EBL Persistence"), wxDefaultPosition, wxDefaultSize, m_radioBoxEBLPersistenceNChoices, m_radioBoxEBLPersistenceChoices, 2, wxRA_SPECIFY_ROWS );
	m_radioBoxEBLPersistence->SetSelection( 0 );
	m_fgSizerEBLSettings->Add( m_radioBoxEBLPersistence, 0, wxALL, 5 );


	m_panelEBL->SetSizer( m_fgSizerEBLSettings );
	m_panelEBL->Layout();
	m_fgSizerEBLSettings->Fit( m_panelEBL );
	m_notebookProperties->AddPage( m_panelEBL, _("EBL"), false );
	m_panelDR = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerDRSettings;
	fgSizerDRSettings = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizerDRSettings->SetFlexibleDirection( wxBOTH );
	fgSizerDRSettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizerDRDefaultSizes;
	fgSizerDRDefaultSizes = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizerDRDefaultSizes->SetFlexibleDirection( wxBOTH );
	fgSizerDRDefaultSizes->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextActiveDRLineColour = new wxStaticText( m_panelDR, wxID_ANY, _("Active DR Line\nColour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveDRLineColour->Wrap( -1 );
	fgSizerDRDefaultSizes->Add( m_staticTextActiveDRLineColour, 0, wxALL, 5 );

	m_colourPickerDRLineColour = new wxColourPickerCtrl( m_panelDR, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerDRDefaultSizes->Add( m_colourPickerDRLineColour, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextInactiveDRLineColour = new wxStaticText( m_panelDR, wxID_ANY, _("Inactive DR Line\nColour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveDRLineColour->Wrap( -1 );
	fgSizerDRDefaultSizes->Add( m_staticTextInactiveDRLineColour, 0, wxALL, 5 );

	m_colourPickerInActiveDRLineColour = new wxColourPickerCtrl( m_panelDR, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerDRDefaultSizes->Add( m_colourPickerInActiveDRLineColour, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextDRLineWidth = new wxStaticText( m_panelDR, wxID_ANY, _("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRLineWidth->Wrap( -1 );
	fgSizerDRDefaultSizes->Add( m_staticTextDRLineWidth, 0, wxALL, 5 );

	wxString m_choiceDRLineWidthChoices[] = { _("1 Pixel"), _("2 Pixels"), _("3 Pixels"), _("4 Pixels"), _("5 Pixels"), _("6 Pixels"), _("7 Pixels"), _("8 Pixels"), _("9 Pixels"), _("10 Pixels") };
	int m_choiceDRLineWidthNChoices = sizeof( m_choiceDRLineWidthChoices ) / sizeof( wxString );
	m_choiceDRLineWidth = new wxChoice( m_panelDR, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDRLineWidthNChoices, m_choiceDRLineWidthChoices, 0 );
	m_choiceDRLineWidth->SetSelection( 0 );
	fgSizerDRDefaultSizes->Add( m_choiceDRLineWidth, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextDRLineStyle = new wxStaticText( m_panelDR, wxID_ANY, _("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRLineStyle->Wrap( -1 );
	fgSizerDRDefaultSizes->Add( m_staticTextDRLineStyle, 0, wxALL, 5 );

	wxString m_choiceDRLineStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choiceDRLineStyleNChoices = sizeof( m_choiceDRLineStyleChoices ) / sizeof( wxString );
	m_choiceDRLineStyle = new wxChoice( m_panelDR, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDRLineStyleNChoices, m_choiceDRLineStyleChoices, 0 );
	m_choiceDRLineStyle->SetSelection( 0 );
	fgSizerDRDefaultSizes->Add( m_choiceDRLineStyle, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextDefaultSoG = new wxStaticText( m_panelDR, wxID_ANY, _("Speed over Ground"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDefaultSoG->Wrap( -1 );
	fgSizerDRDefaultSizes->Add( m_staticTextDefaultSoG, 0, wxALL, 5 );

	m_textCtrlSOG = new wxTextCtrl( m_panelDR, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerDRDefaultSizes->Add( m_textCtrlSOG, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextCOG = new wxStaticText( m_panelDR, wxID_ANY, _("Course over Ground"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextCOG->Wrap( -1 );
	fgSizerDRDefaultSizes->Add( m_staticTextCOG, 0, wxALL, 5 );

	m_textCtrlCOG = new wxTextCtrl( m_panelDR, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerDRDefaultSizes->Add( m_textCtrlCOG, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextDRLength = new wxStaticText( m_panelDR, wxID_ANY, _("DR Path Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRLength->Wrap( -1 );
	fgSizerDRDefaultSizes->Add( m_staticTextDRLength, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlDRPathLength = new wxTextCtrl( m_panelDR, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerDRDefaultSizes->Add( m_textCtrlDRPathLength, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextDRPointInterval = new wxStaticText( m_panelDR, wxID_ANY, _("DR Point Interval"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRPointInterval->Wrap( -1 );
	fgSizerDRDefaultSizes->Add( m_staticTextDRPointInterval, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlDRPointInterval = new wxTextCtrl( m_panelDR, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerDRDefaultSizes->Add( m_textCtrlDRPointInterval, 0, wxALIGN_RIGHT|wxALL, 5 );


	fgSizerDRSettings->Add( fgSizerDRDefaultSizes, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer16;
	fgSizer16 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer16->SetFlexibleDirection( wxBOTH );
	fgSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxString m_radioBoxDRLengthTypeChoices[] = { _("Time"), _("Distance") };
	int m_radioBoxDRLengthTypeNChoices = sizeof( m_radioBoxDRLengthTypeChoices ) / sizeof( wxString );
	m_radioBoxDRLengthType = new wxRadioBox( m_panelDR, wxID_ANY, _("Length Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxDRLengthTypeNChoices, m_radioBoxDRLengthTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxDRLengthType->SetSelection( 1 );
	fgSizer16->Add( m_radioBoxDRLengthType, 0, wxALL, 5 );

	wxString m_radioBoxDRIntervalTypeChoices[] = { _("Time"), _("Distance") };
	int m_radioBoxDRIntervalTypeNChoices = sizeof( m_radioBoxDRIntervalTypeChoices ) / sizeof( wxString );
	m_radioBoxDRIntervalType = new wxRadioBox( m_panelDR, wxID_ANY, _("Interval Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxDRIntervalTypeNChoices, m_radioBoxDRIntervalTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxDRIntervalType->SetSelection( 1 );
	fgSizer16->Add( m_radioBoxDRIntervalType, 0, wxALL, 5 );

	wxString m_radioBoxDRDistanceUnitsChoices[] = { _("Kilometres"), _("Nautical Miles") };
	int m_radioBoxDRDistanceUnitsNChoices = sizeof( m_radioBoxDRDistanceUnitsChoices ) / sizeof( wxString );
	m_radioBoxDRDistanceUnits = new wxRadioBox( m_panelDR, wxID_ANY, _("Distance Units"), wxDefaultPosition, wxDefaultSize, m_radioBoxDRDistanceUnitsNChoices, m_radioBoxDRDistanceUnitsChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxDRDistanceUnits->SetSelection( 0 );
	fgSizer16->Add( m_radioBoxDRDistanceUnits, 0, wxALL, 5 );

	wxString m_radioBoxDRTimeUnitsChoices[] = { _("Minutes"), _("Hours"), _("Days") };
	int m_radioBoxDRTimeUnitsNChoices = sizeof( m_radioBoxDRTimeUnitsChoices ) / sizeof( wxString );
	m_radioBoxDRTimeUnits = new wxRadioBox( m_panelDR, wxID_ANY, _("Time Units"), wxDefaultPosition, wxDefaultSize, m_radioBoxDRTimeUnitsNChoices, m_radioBoxDRTimeUnitsChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxDRTimeUnits->SetSelection( 0 );
	fgSizer16->Add( m_radioBoxDRTimeUnits, 0, wxALL, 5 );

	m_checkBoxDRShowArrow = new wxCheckBox( m_panelDR, wxID_ANY, _("Show DR Direction Arrow"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxDRShowArrow->SetValue(true);
	fgSizer16->Add( m_checkBoxDRShowArrow, 0, wxALL, 5 );

	wxString m_radioBoxDRPersistenceChoices[] = { _("Persistent"), _("Persistent over Crash"), _("Never") };
	int m_radioBoxDRPersistenceNChoices = sizeof( m_radioBoxDRPersistenceChoices ) / sizeof( wxString );
	m_radioBoxDRPersistence = new wxRadioBox( m_panelDR, wxID_ANY, _("DR Persistence"), wxDefaultPosition, wxDefaultSize, m_radioBoxDRPersistenceNChoices, m_radioBoxDRPersistenceChoices, 2, wxRA_SPECIFY_ROWS );
	m_radioBoxDRPersistence->SetSelection( 2 );
	fgSizer16->Add( m_radioBoxDRPersistence, 0, wxALL, 5 );


	fgSizerDRSettings->Add( fgSizer16, 1, wxEXPAND, 5 );


	m_panelDR->SetSizer( fgSizerDRSettings );
	m_panelDR->Layout();
	fgSizerDRSettings->Fit( m_panelDR );
	m_notebookProperties->AddPage( m_panelDR, _("DR"), false );
	m_panelDRPoint = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );

	m_fgSizerDREndPointIcon = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerDREndPointIcon->AddGrowableCol( 0 );
	m_fgSizerDREndPointIcon->AddGrowableCol( 1 );
	m_fgSizerDREndPointIcon->SetFlexibleDirection( wxBOTH );
	m_fgSizerDREndPointIcon->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextDRPointIcon = new wxStaticText( m_panelDRPoint, wxID_ANY, _("DR Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRPointIcon->Wrap( -1 );
	m_fgSizerDREndPointIcon->Add( m_staticTextDRPointIcon, 0, wxALL|wxEXPAND, 5 );

	m_bcomboBoxDRPointIconName = new wxBitmapComboBox( m_panelDRPoint, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_fgSizerDREndPointIcon->Add( m_bcomboBoxDRPointIconName, 1, wxALL|wxEXPAND, 5 );


	bSizer12->Add( m_fgSizerDREndPointIcon, 0, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerDRPointRangeRings;
	fgSizerDRPointRangeRings = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerDRPointRangeRings->SetFlexibleDirection( wxBOTH );
	fgSizerDRPointRangeRings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxShowDRPointRangeRings = new wxCheckBox( m_panelDRPoint, wxID_ANY, _("Show Range Rings"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizerDRPointRangeRings->Add( m_checkBoxShowDRPointRangeRings, 0, wxALL, 5 );


	fgSizerDRPointRangeRings->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticTextDRPointRangeRingNumber = new wxStaticText( m_panelDRPoint, wxID_ANY, _("Number of Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRPointRangeRingNumber->Wrap( -1 );
	fgSizerDRPointRangeRings->Add( m_staticTextDRPointRangeRingNumber, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceDRPointRangeRingNumberChoices[] = { _("None"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9"), _("10") };
	int m_choiceDRPointRangeRingNumberNChoices = sizeof( m_choiceDRPointRangeRingNumberChoices ) / sizeof( wxString );
	m_choiceDRPointRangeRingNumber = new wxChoice( m_panelDRPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDRPointRangeRingNumberNChoices, m_choiceDRPointRangeRingNumberChoices, 0 );
	m_choiceDRPointRangeRingNumber->SetSelection( 0 );
	fgSizerDRPointRangeRings->Add( m_choiceDRPointRangeRingNumber, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextDRPointRangeRingSteps = new wxStaticText( m_panelDRPoint, wxID_ANY, _("Distance Between Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRPointRangeRingSteps->Wrap( -1 );
	fgSizerDRPointRangeRings->Add( m_staticTextDRPointRangeRingSteps, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlDRPointRangeRingSteps = new wxTextCtrl( m_panelDRPoint, wxID_ANY, _("0.5"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerDRPointRangeRings->Add( m_textCtrlDRPointRangeRingSteps, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextDRPointDistanceUnit = new wxStaticText( m_panelDRPoint, wxID_ANY, _("Distance Unit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRPointDistanceUnit->Wrap( -1 );
	fgSizerDRPointRangeRings->Add( m_staticTextDRPointDistanceUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceDRPointDistanceUnitChoices[] = { _("Nautical Miles"), _("Kilometres") };
	int m_choiceDRPointDistanceUnitNChoices = sizeof( m_choiceDRPointDistanceUnitChoices ) / sizeof( wxString );
	m_choiceDRPointDistanceUnit = new wxChoice( m_panelDRPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDRPointDistanceUnitNChoices, m_choiceDRPointDistanceUnitChoices, 0 );
	m_choiceDRPointDistanceUnit->SetSelection( 0 );
	fgSizerDRPointRangeRings->Add( m_choiceDRPointDistanceUnit, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextDRPointRangeRingColours = new wxStaticText( m_panelDRPoint, wxID_ANY, _("Range Ring Colours"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRPointRangeRingColours->Wrap( -1 );
	fgSizerDRPointRangeRings->Add( m_staticTextDRPointRangeRingColours, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerDRPointRangeRingColours = new wxColourPickerCtrl( m_panelDRPoint, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerDRPointRangeRings->Add( m_colourPickerDRPointRangeRingColours, 0, wxALL|wxALIGN_RIGHT, 5 );

	m_staticTextDRPointRangeRingWidth = new wxStaticText( m_panelDRPoint, wxID_ANY, _("Range Ring Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRPointRangeRingWidth->Wrap( -1 );
	fgSizerDRPointRangeRings->Add( m_staticTextDRPointRangeRingWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceDRPointRangeRingWidthChoices[] = { _("1 pixel"), _("2 pixels"), _("3 pixels"), _("4 pixels"), _("5 pixels"), _("6 pixels"), _("7 pixels"), _("8 pixels"), _("9 pixels"), _("10 pixels") };
	int m_choiceDRPointRangeRingWidthNChoices = sizeof( m_choiceDRPointRangeRingWidthChoices ) / sizeof( wxString );
	m_choiceDRPointRangeRingWidth = new wxChoice( m_panelDRPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDRPointRangeRingWidthNChoices, m_choiceDRPointRangeRingWidthChoices, 0 );
	m_choiceDRPointRangeRingWidth->SetSelection( 1 );
	fgSizerDRPointRangeRings->Add( m_choiceDRPointRangeRingWidth, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextDRPointRangeRingStyle = new wxStaticText( m_panelDRPoint, wxID_ANY, _("Range Ring Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDRPointRangeRingStyle->Wrap( -1 );
	fgSizerDRPointRangeRings->Add( m_staticTextDRPointRangeRingStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceDRPointRangeRingStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choiceDRPointRangeRingStyleNChoices = sizeof( m_choiceDRPointRangeRingStyleChoices ) / sizeof( wxString );
	m_choiceDRPointRangeRingStyle = new wxChoice( m_panelDRPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDRPointRangeRingStyleNChoices, m_choiceDRPointRangeRingStyleChoices, 0 );
	m_choiceDRPointRangeRingStyle->SetSelection( 0 );
	fgSizerDRPointRangeRings->Add( m_choiceDRPointRangeRingStyle, 0, wxALIGN_RIGHT|wxALL, 5 );


	bSizer12->Add( fgSizerDRPointRangeRings, 0, wxEXPAND, 5 );


	m_panelDRPoint->SetSizer( bSizer12 );
	m_panelDRPoint->Layout();
	bSizer12->Fit( m_panelDRPoint );
	m_notebookProperties->AddPage( m_panelDRPoint, _("DR Point"), false );
	m_panelGZ = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerGZSettings;
	fgSizerGZSettings = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizerGZSettings->AddGrowableCol( 0 );
	fgSizerGZSettings->SetFlexibleDirection( wxBOTH );
	fgSizerGZSettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_fgSizerGZIcons = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgSizerGZIcons->AddGrowableCol( 0 );
	m_fgSizerGZIcons->SetFlexibleDirection( wxBOTH );
	m_fgSizerGZIcons->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_fgSizerGZFASIcon = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerGZFASIcon->AddGrowableCol( 0 );
	m_fgSizerGZFASIcon->AddGrowableCol( 1 );
	m_fgSizerGZFASIcon->SetFlexibleDirection( wxBOTH );
	m_fgSizerGZFASIcon->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextGZFirstIcon = new wxStaticText( m_panelGZ, wxID_ANY, _("First Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGZFirstIcon->Wrap( -1 );
	m_fgSizerGZFASIcon->Add( m_staticTextGZFirstIcon, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_bcomboBoxGZFirstIconName = new wxBitmapComboBox( m_panelGZ, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_fgSizerGZFASIcon->Add( m_bcomboBoxGZFirstIconName, 1, wxALL|wxEXPAND, 5 );

	m_staticTextGZSecondPointIcon = new wxStaticText( m_panelGZ, wxID_ANY, _("Second Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGZSecondPointIcon->Wrap( -1 );
	m_fgSizerGZFASIcon->Add( m_staticTextGZSecondPointIcon, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_bcomboBoxGZSecondIconName = new wxBitmapComboBox( m_panelGZ, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_fgSizerGZFASIcon->Add( m_bcomboBoxGZSecondIconName, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );


	m_fgSizerGZIcons->Add( m_fgSizerGZFASIcon, 1, wxEXPAND, 5 );


	fgSizerGZSettings->Add( m_fgSizerGZIcons, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerGZSettingsColours;
	fgSizerGZSettingsColours = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizerGZSettingsColours->SetFlexibleDirection( wxBOTH );
	fgSizerGZSettingsColours->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextActiveGZLineColour = new wxStaticText( m_panelGZ, wxID_ANY, _("Active Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveGZLineColour->Wrap( -1 );
	fgSizerGZSettingsColours->Add( m_staticTextActiveGZLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerActiveGZLineColour = new wxColourPickerCtrl( m_panelGZ, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerGZSettingsColours->Add( m_colourPickerActiveGZLineColour, 0, wxALL, 5 );

	m_staticTextActiveGZFillColour = new wxStaticText( m_panelGZ, wxID_ANY, _("Active Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveGZFillColour->Wrap( -1 );
	fgSizerGZSettingsColours->Add( m_staticTextActiveGZFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerActiveGZFillColour = new wxColourPickerCtrl( m_panelGZ, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerGZSettingsColours->Add( m_colourPickerActiveGZFillColour, 0, wxALL, 5 );

	m_staticTextInactiveGZLineColour = new wxStaticText( m_panelGZ, wxID_ANY, _("Inactive Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveGZLineColour->Wrap( -1 );
	fgSizerGZSettingsColours->Add( m_staticTextInactiveGZLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerInActiveGZLineColour = new wxColourPickerCtrl( m_panelGZ, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerGZSettingsColours->Add( m_colourPickerInActiveGZLineColour, 0, wxALL, 5 );

	m_staticTextInactiveGZFillColour = new wxStaticText( m_panelGZ, wxID_ANY, _("Inactive Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveGZFillColour->Wrap( -1 );
	fgSizerGZSettingsColours->Add( m_staticTextInactiveGZFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerInActiveGZFillColour = new wxColourPickerCtrl( m_panelGZ, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerGZSettingsColours->Add( m_colourPickerInActiveGZFillColour, 0, wxALL, 5 );

	m_staticTextGZLineWidth = new wxStaticText( m_panelGZ, wxID_ANY, _("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGZLineWidth->Wrap( -1 );
	fgSizerGZSettingsColours->Add( m_staticTextGZLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceGZLineWidthChoices[] = { _("1 Pixel"), _("2 Pixels"), _("3 Pixels"), _("4 Pixels"), _("5 Pixels"), _("6 Pixels"), _("7 Pixels"), _("8 Pixels"), _("9 Pixels"), _("10 Pixels") };
	int m_choiceGZLineWidthNChoices = sizeof( m_choiceGZLineWidthChoices ) / sizeof( wxString );
	m_choiceGZLineWidth = new wxChoice( m_panelGZ, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceGZLineWidthNChoices, m_choiceGZLineWidthChoices, 0 );
	m_choiceGZLineWidth->SetSelection( 0 );
	fgSizerGZSettingsColours->Add( m_choiceGZLineWidth, 0, wxALL, 5 );

	m_staticTextGZLineStyle = new wxStaticText( m_panelGZ, wxID_ANY, _("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGZLineStyle->Wrap( -1 );
	fgSizerGZSettingsColours->Add( m_staticTextGZLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceGZLineStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choiceGZLineStyleNChoices = sizeof( m_choiceGZLineStyleChoices ) / sizeof( wxString );
	m_choiceGZLineStyle = new wxChoice( m_panelGZ, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceGZLineStyleNChoices, m_choiceGZLineStyleChoices, 0 );
	m_choiceGZLineStyle->SetSelection( 0 );
	fgSizerGZSettingsColours->Add( m_choiceGZLineStyle, 0, wxALL, 5 );

	m_staticTextGZFillTransparency = new wxStaticText( m_panelGZ, wxID_ANY, _("Fill Density"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGZFillTransparency->Wrap( -1 );
	fgSizerGZSettingsColours->Add( m_staticTextGZFillTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_sliderGZFillTransparency = new wxSlider( m_panelGZ, wxID_ANY, 175, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizerGZSettingsColours->Add( m_sliderGZFillTransparency, 0, wxALL|wxEXPAND, 5 );


	fgSizerGZSettings->Add( fgSizerGZSettingsColours, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerGZRotate;
	fgSizerGZRotate = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizerGZRotate->SetFlexibleDirection( wxBOTH );
	fgSizerGZRotate->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxGZRotateWithBoat = new wxCheckBox( m_panelGZ, wxID_ANY, _("Rotate with Boat"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizerGZRotate->Add( m_checkBoxGZRotateWithBoat, 0, wxALL, 5 );

	wxString m_radioBoxGZMaintainWithChoices[] = { _("Heading"), _("Course over Ground") };
	int m_radioBoxGZMaintainWithNChoices = sizeof( m_radioBoxGZMaintainWithChoices ) / sizeof( wxString );
	m_radioBoxGZMaintainWith = new wxRadioBox( m_panelGZ, wxID_ANY, _("Maintain with"), wxDefaultPosition, wxDefaultSize, m_radioBoxGZMaintainWithNChoices, m_radioBoxGZMaintainWithChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxGZMaintainWith->SetSelection( 1 );
	fgSizerGZRotate->Add( m_radioBoxGZMaintainWith, 0, wxALL, 5 );

	wxString m_radioBoxGZLineTypeChoices[] = { _("Arc"), _("Straight") };
	int m_radioBoxGZLineTypeNChoices = sizeof( m_radioBoxGZLineTypeChoices ) / sizeof( wxString );
	m_radioBoxGZLineType = new wxRadioBox( m_panelGZ, wxID_ANY, _("Line Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxGZLineTypeNChoices, m_radioBoxGZLineTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxGZLineType->SetSelection( 0 );
	fgSizerGZRotate->Add( m_radioBoxGZLineType, 0, wxALL, 5 );


	fgSizerGZSettings->Add( fgSizerGZRotate, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizerGZPersistence;
	fgSizerGZPersistence = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerGZPersistence->SetFlexibleDirection( wxBOTH );
	fgSizerGZPersistence->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxString m_radioBoxGZPersistenceChoices[] = { _("Persistent"), _("Persistent over Crash"), _("Never") };
	int m_radioBoxGZPersistenceNChoices = sizeof( m_radioBoxGZPersistenceChoices ) / sizeof( wxString );
	m_radioBoxGZPersistence = new wxRadioBox( m_panelGZ, wxID_ANY, _("Guard Zone Persistence"), wxDefaultPosition, wxDefaultSize, m_radioBoxGZPersistenceNChoices, m_radioBoxGZPersistenceChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxGZPersistence->SetSelection( 0 );
	fgSizerGZPersistence->Add( m_radioBoxGZPersistence, 1, wxALL, 5 );


	fgSizerGZSettings->Add( fgSizerGZPersistence, 1, wxEXPAND, 5 );


	m_panelGZ->SetSizer( fgSizerGZSettings );
	m_panelGZ->Layout();
	fgSizerGZSettings->Fit( m_panelGZ );
	m_notebookProperties->AddPage( m_panelGZ, _("Guard Zone"), true );
	m_panelPIL = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_fgSizerPILSettings = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerPILSettings->AddGrowableCol( 1 );
	m_fgSizerPILSettings->SetFlexibleDirection( wxBOTH );
	m_fgSizerPILSettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILStartPointIcon = new wxStaticText( m_panelPIL, wxID_ANY, _("Start Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILStartPointIcon->Wrap( -1 );
	m_fgSizerPILSettings->Add( m_staticTextPILStartPointIcon, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_fgSizerPILStartIconName = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgSizerPILStartIconName->AddGrowableCol( 0 );
	m_fgSizerPILStartIconName->SetFlexibleDirection( wxBOTH );
	m_fgSizerPILStartIconName->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_bcomboBoxPILStartIconName = new wxBitmapComboBox( m_panelPIL, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_fgSizerPILStartIconName->Add( m_bcomboBoxPILStartIconName, 0, wxALL|wxEXPAND, 5 );


	m_fgSizerPILSettings->Add( m_fgSizerPILStartIconName, 1, wxEXPAND, 5 );

	m_staticTextPILEndPointIcon = new wxStaticText( m_panelPIL, wxID_ANY, _("End Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILEndPointIcon->Wrap( -1 );
	m_fgSizerPILSettings->Add( m_staticTextPILEndPointIcon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_fgSizerPILEndPointIcon = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgSizerPILEndPointIcon->AddGrowableCol( 0 );
	m_fgSizerPILEndPointIcon->SetFlexibleDirection( wxBOTH );
	m_fgSizerPILEndPointIcon->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_bcomboBoxPILEndIconName = new wxBitmapComboBox( m_panelPIL, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_fgSizerPILEndPointIcon->Add( m_bcomboBoxPILEndIconName, 0, wxALL|wxEXPAND, 5 );


	m_fgSizerPILSettings->Add( m_fgSizerPILEndPointIcon, 1, wxEXPAND, 5 );

	m_staticTextNumIndexLines = new wxStaticText( m_panelPIL, wxID_ANY, _("Initial number of index lines"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextNumIndexLines->Wrap( -1 );
	m_fgSizerPILSettings->Add( m_staticTextNumIndexLines, 0, wxALL, 5 );

	wxString m_choiceNumIndexLinesChoices[] = { _("1"), _("2") };
	int m_choiceNumIndexLinesNChoices = sizeof( m_choiceNumIndexLinesChoices ) / sizeof( wxString );
	m_choiceNumIndexLines = new wxChoice( m_panelPIL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceNumIndexLinesNChoices, m_choiceNumIndexLinesChoices, 0 );
	m_choiceNumIndexLines->SetSelection( 0 );
	m_fgSizerPILSettings->Add( m_choiceNumIndexLines, 0, wxALL, 5 );

	m_staticTextPILDefaultOffset = new wxStaticText( m_panelPIL, wxID_ANY, _("Default Offset (+Stbd/-Port)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILDefaultOffset->Wrap( -1 );
	m_fgSizerPILSettings->Add( m_staticTextPILDefaultOffset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlPILDefaultOffset = new wxTextCtrl( m_panelPIL, wxID_ANY, _("1.0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fgSizerPILSettings->Add( m_textCtrlPILDefaultOffset, 0, wxALIGN_RIGHT|wxALL, 5 );

	wxFlexGridSizer* fgSizer33;
	fgSizer33 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer33->AddGrowableCol( 0 );
	fgSizer33->SetFlexibleDirection( wxBOTH );
	fgSizer33->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILActiveCentreLineColour = new wxStaticText( m_panelPIL, wxID_ANY, _("Active Centre Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILActiveCentreLineColour->Wrap( -1 );
	fgSizer33->Add( m_staticTextPILActiveCentreLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerPILActiveCentreLineColour = new wxColourPickerCtrl( m_panelPIL, wxID_ANY, wxColour( 0, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer33->Add( m_colourPickerPILActiveCentreLineColour, 0, wxALIGN_RIGHT|wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer33, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer35;
	fgSizer35 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer35->AddGrowableCol( 0 );
	fgSizer35->SetFlexibleDirection( wxBOTH );
	fgSizer35->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextInactiveCentreLineColour = new wxStaticText( m_panelPIL, wxID_ANY, _("Inactive Centre Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveCentreLineColour->Wrap( -1 );
	fgSizer35->Add( m_staticTextInactiveCentreLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerPILInActiveCentreLineColour = new wxColourPickerCtrl( m_panelPIL, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer35->Add( m_colourPickerPILInActiveCentreLineColour, 0, wxALIGN_RIGHT|wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer35, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer37;
	fgSizer37 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer37->AddGrowableCol( 0 );
	fgSizer37->SetFlexibleDirection( wxBOTH );
	fgSizer37->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILActiveOffsetLine1Colour = new wxStaticText( m_panelPIL, wxID_ANY, _("Active Offset Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILActiveOffsetLine1Colour->Wrap( -1 );
	fgSizer37->Add( m_staticTextPILActiveOffsetLine1Colour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerPILActiveOffsetLine1Colour = new wxColourPickerCtrl( m_panelPIL, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer37->Add( m_colourPickerPILActiveOffsetLine1Colour, 0, wxALIGN_RIGHT|wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer37, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer36;
	fgSizer36 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer36->AddGrowableCol( 0 );
	fgSizer36->SetFlexibleDirection( wxBOTH );
	fgSizer36->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILInactiveOffsetLine1Colour = new wxStaticText( m_panelPIL, wxID_ANY, _("Inactive Offset Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILInactiveOffsetLine1Colour->Wrap( -1 );
	fgSizer36->Add( m_staticTextPILInactiveOffsetLine1Colour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerPILInActiveOffsetLine1Colour = new wxColourPickerCtrl( m_panelPIL, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer36->Add( m_colourPickerPILInActiveOffsetLine1Colour, 0, wxALIGN_RIGHT|wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer36, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer41;
	fgSizer41 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer41->AddGrowableCol( 0 );
	fgSizer41->SetFlexibleDirection( wxBOTH );
	fgSizer41->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILActiveOffsetLine2Colour = new wxStaticText( m_panelPIL, wxID_ANY, _("Active Offset Line 2 Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILActiveOffsetLine2Colour->Wrap( -1 );
	fgSizer41->Add( m_staticTextPILActiveOffsetLine2Colour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerPILActiveOffsetLine2Colour = new wxColourPickerCtrl( m_panelPIL, wxID_ANY, wxColour( 0, 255, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer41->Add( m_colourPickerPILActiveOffsetLine2Colour, 0, wxALIGN_RIGHT|wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer41, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer42;
	fgSizer42 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer42->AddGrowableCol( 0 );
	fgSizer42->SetFlexibleDirection( wxBOTH );
	fgSizer42->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILInactiveOffsetLine2Colour = new wxStaticText( m_panelPIL, wxID_ANY, _("Inactive Offset Line 2 Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILInactiveOffsetLine2Colour->Wrap( -1 );
	fgSizer42->Add( m_staticTextPILInactiveOffsetLine2Colour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerPILInactiveOffsetLine2Colour = new wxColourPickerCtrl( m_panelPIL, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer42->Add( m_colourPickerPILInactiveOffsetLine2Colour, 0, wxALIGN_RIGHT|wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer42, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer371;
	fgSizer371 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer371->AddGrowableCol( 0 );
	fgSizer371->SetFlexibleDirection( wxBOTH );
	fgSizer371->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILCentreLineWidth = new wxStaticText( m_panelPIL, wxID_ANY, _("Centre Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILCentreLineWidth->Wrap( -1 );
	fgSizer371->Add( m_staticTextPILCentreLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choicePILCentreLineWidthChoices[] = { _("1 Pixel"), _("2 Pixels"), _("3 Pixels"), _("4 Pixels"), _("5 Pixels"), _("6 Pixels"), _("7 Pixels"), _("8 Pixels"), _("9 Pixels"), _("10 Pixels") };
	int m_choicePILCentreLineWidthNChoices = sizeof( m_choicePILCentreLineWidthChoices ) / sizeof( wxString );
	m_choicePILCentreLineWidth = new wxChoice( m_panelPIL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePILCentreLineWidthNChoices, m_choicePILCentreLineWidthChoices, 0 );
	m_choicePILCentreLineWidth->SetSelection( 0 );
	fgSizer371->Add( m_choicePILCentreLineWidth, 0, wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer371, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer38;
	fgSizer38 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer38->AddGrowableCol( 0 );
	fgSizer38->SetFlexibleDirection( wxBOTH );
	fgSizer38->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILCentreLineStyle = new wxStaticText( m_panelPIL, wxID_ANY, _("Centre Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILCentreLineStyle->Wrap( -1 );
	fgSizer38->Add( m_staticTextPILCentreLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choicePILCentreLineStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choicePILCentreLineStyleNChoices = sizeof( m_choicePILCentreLineStyleChoices ) / sizeof( wxString );
	m_choicePILCentreLineStyle = new wxChoice( m_panelPIL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePILCentreLineStyleNChoices, m_choicePILCentreLineStyleChoices, 0 );
	m_choicePILCentreLineStyle->SetSelection( 0 );
	fgSizer38->Add( m_choicePILCentreLineStyle, 0, wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer38, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer39;
	fgSizer39 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer39->AddGrowableCol( 0 );
	fgSizer39->SetFlexibleDirection( wxBOTH );
	fgSizer39->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILOffsetLine1Width = new wxStaticText( m_panelPIL, wxID_ANY, _("Offset Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILOffsetLine1Width->Wrap( -1 );
	fgSizer39->Add( m_staticTextPILOffsetLine1Width, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choicePILOffsetLine1WidthChoices[] = { _("1 Pixel"), _("2 Pixels"), _("3 Pixels"), _("4 Pixels"), _("5 Pixels"), _("6 Pixels"), _("7 Pixels"), _("8 Pixels"), _("9 Pixels"), _("10 Pixels") };
	int m_choicePILOffsetLine1WidthNChoices = sizeof( m_choicePILOffsetLine1WidthChoices ) / sizeof( wxString );
	m_choicePILOffsetLine1Width = new wxChoice( m_panelPIL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePILOffsetLine1WidthNChoices, m_choicePILOffsetLine1WidthChoices, 0 );
	m_choicePILOffsetLine1Width->SetSelection( 0 );
	fgSizer39->Add( m_choicePILOffsetLine1Width, 0, wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer39, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer40;
	fgSizer40 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer40->AddGrowableCol( 0 );
	fgSizer40->SetFlexibleDirection( wxBOTH );
	fgSizer40->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILOffsetLine1Style = new wxStaticText( m_panelPIL, wxID_ANY, _("Offset Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILOffsetLine1Style->Wrap( -1 );
	fgSizer40->Add( m_staticTextPILOffsetLine1Style, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choicePILOffsetLine1StyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choicePILOffsetLine1StyleNChoices = sizeof( m_choicePILOffsetLine1StyleChoices ) / sizeof( wxString );
	m_choicePILOffsetLine1Style = new wxChoice( m_panelPIL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePILOffsetLine1StyleNChoices, m_choicePILOffsetLine1StyleChoices, 0 );
	m_choicePILOffsetLine1Style->SetSelection( 0 );
	fgSizer40->Add( m_choicePILOffsetLine1Style, 0, wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer40, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer43;
	fgSizer43 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer43->AddGrowableCol( 0 );
	fgSizer43->SetFlexibleDirection( wxBOTH );
	fgSizer43->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILOffsetLine2Width = new wxStaticText( m_panelPIL, wxID_ANY, _("Offset Line 2 Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILOffsetLine2Width->Wrap( -1 );
	fgSizer43->Add( m_staticTextPILOffsetLine2Width, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choicePILOffsetLine2WidthChoices[] = { _("1 Pixel"), _("2 Pixels"), _("3 Pixels"), _("4 Pixels"), _("5 Pixels"), _("6 Pixels"), _("7 Pixels"), _("8 Pixels"), _("9 Pixels"), _("10 Pixels") };
	int m_choicePILOffsetLine2WidthNChoices = sizeof( m_choicePILOffsetLine2WidthChoices ) / sizeof( wxString );
	m_choicePILOffsetLine2Width = new wxChoice( m_panelPIL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePILOffsetLine2WidthNChoices, m_choicePILOffsetLine2WidthChoices, 0 );
	m_choicePILOffsetLine2Width->SetSelection( 0 );
	fgSizer43->Add( m_choicePILOffsetLine2Width, 0, wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer43, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer44;
	fgSizer44 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer44->AddGrowableCol( 0 );
	fgSizer44->SetFlexibleDirection( wxBOTH );
	fgSizer44->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILOffsetLine2Style = new wxStaticText( m_panelPIL, wxID_ANY, _("Offset Line 2 Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILOffsetLine2Style->Wrap( -1 );
	fgSizer44->Add( m_staticTextPILOffsetLine2Style, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choicePILOffsetLine2StyleChoices[] = { _("Solid"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choicePILOffsetLine2StyleNChoices = sizeof( m_choicePILOffsetLine2StyleChoices ) / sizeof( wxString );
	m_choicePILOffsetLine2Style = new wxChoice( m_panelPIL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePILOffsetLine2StyleNChoices, m_choicePILOffsetLine2StyleChoices, 0 );
	m_choicePILOffsetLine2Style->SetSelection( 0 );
	fgSizer44->Add( m_choicePILOffsetLine2Style, 0, wxALL, 5 );


	m_fgSizerPILSettings->Add( fgSizer44, 1, wxEXPAND, 5 );

	wxString m_radioBoxPILPersistenceChoices[] = { _("Persistent"), _("Persistent over Crash"), _("Never") };
	int m_radioBoxPILPersistenceNChoices = sizeof( m_radioBoxPILPersistenceChoices ) / sizeof( wxString );
	m_radioBoxPILPersistence = new wxRadioBox( m_panelPIL, wxID_ANY, _("Parallel Index Line Persistence"), wxDefaultPosition, wxDefaultSize, m_radioBoxPILPersistenceNChoices, m_radioBoxPILPersistenceChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxPILPersistence->SetSelection( 0 );
	m_fgSizerPILSettings->Add( m_radioBoxPILPersistence, 0, wxALL, 5 );


	m_panelPIL->SetSizer( m_fgSizerPILSettings );
	m_panelPIL->Layout();
	m_fgSizerPILSettings->Fit( m_panelPIL );
	m_notebookProperties->AddPage( m_panelPIL, _("Parallel Index Line"), false );
	m_panelAbout = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerAbout;
	bSizerAbout = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizerAboutLayout;
	fgSizerAboutLayout = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerAboutLayout->SetFlexibleDirection( wxBOTH );
	fgSizerAboutLayout->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextName = new wxStaticText( m_panelAbout, wxID_ANY, _("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextName->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextName, 0, wxALL, 5 );

	m_staticTextNameVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextNameVal->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextNameVal, 0, wxALL, 5 );

	m_staticTextVersion = new wxStaticText( m_panelAbout, wxID_ANY, _("Version:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextVersion->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextVersion, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextMajor = new wxStaticText( m_panelAbout, wxID_ANY, _("Major:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMajor->Wrap( -1 );
	fgSizer6->Add( m_staticTextMajor, 0, wxALL, 5 );

	m_staticTextMajorVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMajorVal->Wrap( -1 );
	fgSizer6->Add( m_staticTextMajorVal, 0, wxALL, 5 );

	m_staticTextMinor = new wxStaticText( m_panelAbout, wxID_ANY, _("Minor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMinor->Wrap( -1 );
	fgSizer6->Add( m_staticTextMinor, 0, wxALL, 5 );

	m_staticTextMinorVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMinorVal->Wrap( -1 );
	fgSizer6->Add( m_staticTextMinorVal, 0, wxALL, 5 );

	m_staticTextPatcvh = new wxStaticText( m_panelAbout, wxID_ANY, _("Patch:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPatcvh->Wrap( -1 );
	fgSizer6->Add( m_staticTextPatcvh, 0, wxALL, 5 );

	m_staticTextPatchVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPatchVal->Wrap( -1 );
	fgSizer6->Add( m_staticTextPatchVal, 0, wxALL, 5 );


	fgSizerAboutLayout->Add( fgSizer6, 1, wxEXPAND, 5 );

	m_staticTextAPIVersion = new wxStaticText( m_panelAbout, wxID_ANY, _("OCPN Draw API Version:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextAPIVersion->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextAPIVersion, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer51;
	fgSizer51 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer51->SetFlexibleDirection( wxBOTH );
	fgSizer51->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextAPIVersionMajor = new wxStaticText( m_panelAbout, wxID_ANY, _("Major:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextAPIVersionMajor->Wrap( -1 );
	fgSizer51->Add( m_staticTextAPIVersionMajor, 0, wxALL, 5 );

	m_staticTextAPIVersionMajorVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextAPIVersionMajorVal->Wrap( -1 );
	fgSizer51->Add( m_staticTextAPIVersionMajorVal, 0, wxALL, 5 );

	m_staticTextAPIVersionMinor = new wxStaticText( m_panelAbout, wxID_ANY, _("Minor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextAPIVersionMinor->Wrap( -1 );
	fgSizer51->Add( m_staticTextAPIVersionMinor, 0, wxALL, 5 );

	m_staticTextAPIVersionMinorVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextAPIVersionMinorVal->Wrap( -1 );
	fgSizer51->Add( m_staticTextAPIVersionMinorVal, 0, wxALL, 5 );


	fgSizerAboutLayout->Add( fgSizer51, 1, wxEXPAND, 5 );

	m_staticTextOCPNVersionMajor = new wxStaticText( m_panelAbout, wxID_ANY, _("OCPN API Version:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOCPNVersionMajor->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextOCPNVersionMajor, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer52;
	fgSizer52 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer52->SetFlexibleDirection( wxBOTH );
	fgSizer52->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextOCPNAPIVersionMajor = new wxStaticText( m_panelAbout, wxID_ANY, _("Major:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOCPNAPIVersionMajor->Wrap( -1 );
	fgSizer52->Add( m_staticTextOCPNAPIVersionMajor, 0, wxALL, 5 );

	m_staticTextOCPNAPIVersionMajorVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOCPNAPIVersionMajorVal->Wrap( -1 );
	fgSizer52->Add( m_staticTextOCPNAPIVersionMajorVal, 0, wxALL, 5 );

	m_staticTextOCPNAPIVersionMinor = new wxStaticText( m_panelAbout, wxID_ANY, _("Minor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOCPNAPIVersionMinor->Wrap( -1 );
	fgSizer52->Add( m_staticTextOCPNAPIVersionMinor, 0, wxALL, 5 );

	m_staticTextOCPNAPIVersionMinorVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOCPNAPIVersionMinorVal->Wrap( -1 );
	fgSizer52->Add( m_staticTextOCPNAPIVersionMinorVal, 0, wxALL, 5 );


	fgSizerAboutLayout->Add( fgSizer52, 1, wxEXPAND, 5 );

	m_staticTextDate = new wxStaticText( m_panelAbout, wxID_ANY, _("Date:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDate->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextDate, 0, wxALL, 5 );

	m_staticTextDateVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDateVal->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextDateVal, 0, wxALL, 5 );


	bSizerAbout->Add( fgSizerAboutLayout, 0, wxEXPAND, 5 );

	m_staticline1 = new wxStaticLine( m_panelAbout, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerAbout->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );

	m_staticTextPluginDirectories = new wxStaticText( m_panelAbout, wxID_ANY, _("OCPN Draw Directories:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPluginDirectories->Wrap( -1 );
	bSizerAbout->Add( m_staticTextPluginDirectories, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer49;
	fgSizer49 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer49->SetFlexibleDirection( wxBOTH );
	fgSizer49->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextData = new wxStaticText( m_panelAbout, wxID_ANY, _("Data:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextData->Wrap( -1 );
	fgSizer49->Add( m_staticTextData, 0, wxALL, 5 );

	m_staticTextDataDirectory = new wxStaticText( m_panelAbout, wxID_ANY, _("Data Directory"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDataDirectory->Wrap( -1 );
	fgSizer49->Add( m_staticTextDataDirectory, 0, wxALL, 5 );

	m_staticTextIcons = new wxStaticText( m_panelAbout, wxID_ANY, _("User Icons:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextIcons->Wrap( -1 );
	fgSizer49->Add( m_staticTextIcons, 0, wxALL, 5 );

	m_staticTextIconDirectory = new wxStaticText( m_panelAbout, wxID_ANY, _("Icon Directory"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextIconDirectory->Wrap( -1 );
	fgSizer49->Add( m_staticTextIconDirectory, 0, wxALL, 5 );

	m_staticTextLayers = new wxStaticText( m_panelAbout, wxID_ANY, _("Permanent Layers:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLayers->Wrap( -1 );
	fgSizer49->Add( m_staticTextLayers, 0, wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextLayersDirectory = new wxStaticText( m_panelAbout, wxID_ANY, _("Layers Directory"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLayersDirectory->Wrap( -1 );
	fgSizer49->Add( m_staticTextLayersDirectory, 0, wxALL, 5 );


	bSizerAbout->Add( fgSizer49, 0, wxEXPAND, 5 );

	m_staticline11 = new wxStaticLine( m_panelAbout, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizerAbout->Add( m_staticline11, 0, wxEXPAND | wxALL, 5 );

	wxFlexGridSizer* fgSizer50;
	fgSizer50 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer50->SetFlexibleDirection( wxBOTH );
	fgSizer50->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextOther = new wxStaticText( m_panelAbout, wxID_ANY, _("Other:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOther->Wrap( -1 );
	fgSizer50->Add( m_staticTextOther, 0, wxALL, 5 );

	m_staticText34 = new wxStaticText( m_panelAbout, wxID_ANY, _("Please report problems using FlySpray at:\nhttps://www.opencpn.org/flyspray/index.php?project=60&do=index&switch=1\n\nor\n\nat the OpenCPN forum:\nhttp://www.cruisersforum.com/forums/f134\n\nor\n\nRaise an issue in GIT at:\nhttps://github.com/jongough/ocpn_draw_pi/issues\n"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	fgSizer50->Add( m_staticText34, 0, wxALL, 5 );


	bSizerAbout->Add( fgSizer50, 0, wxEXPAND, 5 );


	m_panelAbout->SetSizer( bSizerAbout );
	m_panelAbout->Layout();
	bSizerAbout->Fit( m_panelAbout );
	m_notebookProperties->AddPage( m_panelAbout, _("About"), false );
	m_panelHelp = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer22;
	fgSizer22 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer22->SetFlexibleDirection( wxBOTH );
	fgSizer22->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextHelp = new wxStaticText( m_panelHelp, wxID_ANY, _("This chart shows the interactions between Boundaries and WatchDog alam boundaries"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHelp->Wrap( -1 );
	fgSizer22->Add( m_staticTextHelp, 0, wxALL, 5 );

	m_gridODWDInteractions = new wxGrid( m_panelHelp, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_gridODWDInteractions->CreateGrid( 4, 5 );
	m_gridODWDInteractions->EnableEditing( false );
	m_gridODWDInteractions->EnableGridLines( true );
	m_gridODWDInteractions->EnableDragGridSize( false );
	m_gridODWDInteractions->SetMargins( 0, 0 );

	// Columns
	m_gridODWDInteractions->EnableDragColMove( false );
	m_gridODWDInteractions->EnableDragColSize( false );
	m_gridODWDInteractions->SetColLabelSize( 0 );
	m_gridODWDInteractions->SetColLabelValue( 0, _("WD") );
	m_gridODWDInteractions->SetColLabelValue( 1, _("All") );
	m_gridODWDInteractions->SetColLabelValue( 2, _("Exclusion") );
	m_gridODWDInteractions->SetColLabelValue( 3, _("Inclusion") );
	m_gridODWDInteractions->SetColLabelValue( 4, _("Neither") );
	m_gridODWDInteractions->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_gridODWDInteractions->EnableDragRowSize( true );
	m_gridODWDInteractions->SetRowLabelSize( 0 );
	m_gridODWDInteractions->SetRowLabelValue( 0, _("OD") );
	m_gridODWDInteractions->SetRowLabelValue( 1, _("Exclusion") );
	m_gridODWDInteractions->SetRowLabelValue( 2, _("Inclusion") );
	m_gridODWDInteractions->SetRowLabelValue( 3, _("Neither") );
	m_gridODWDInteractions->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_gridODWDInteractions->SetDefaultCellBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	m_gridODWDInteractions->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	fgSizer22->Add( m_gridODWDInteractions, 0, wxALL, 5 );


	m_panelHelp->SetSizer( fgSizer22 );
	m_panelHelp->Layout();
	fgSizer22->Fit( m_panelHelp );
	m_notebookProperties->AddPage( m_panelHelp, _("Help"), false );
	m_panelLicense = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer48;
	fgSizer48 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer48->AddGrowableCol( 0 );
	fgSizer48->AddGrowableRow( 0 );
	fgSizer48->SetFlexibleDirection( wxBOTH );
	fgSizer48->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );

	m_textCtrlLicense = new wxTextCtrl( m_panelLicense, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	fgSizer48->Add( m_textCtrlLicense, 0, wxALL|wxEXPAND, 5 );


	m_panelLicense->SetSizer( fgSizer48 );
	m_panelLicense->Layout();
	fgSizer48->Fit( m_panelLicense );
	m_notebookProperties->AddPage( m_panelLicense, _("License"), false );

	m_SizerProperties->Add( m_notebookProperties, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizerOKCancelApply;
	bSizerOKCancelApply = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonOK, 0, wxALL, 5 );

	m_buttonCancel = new wxButton( this, wxID_ANY, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonCancel, 0, wxALL, 5 );

	m_buttonApply = new wxButton( this, wxID_ANY, _("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonApply, 0, wxALL, 5 );


	m_SizerProperties->Add( bSizerOKCancelApply, 0, wxEXPAND, 5 );


	this->SetSizer( m_SizerProperties );
	this->Layout();

	// Connect Events
	m_buttonConfigFileEntries->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnClickConfigFileEntries ), NULL, this );
	m_buttonTextFont->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnButtonClickFonts ), NULL, this );
	m_checkBoxRotateWithBoat->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnEBLRotateWithBoat ), NULL, this );
	m_checkBoxEBLFixedEndPosition->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnEBLFixedEndPosition ), NULL, this );
	m_checkBoxGZRotateWithBoat->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnEBLRotateWithBoat ), NULL, this );
	m_choiceNumIndexLines->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ODPropertiesDialogDef::OnPILIndexLineChoice ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesOKClick ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesCancelClick ), NULL, this );
	m_buttonApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesApplyClick ), NULL, this );
}

ODPropertiesDialogDef::~ODPropertiesDialogDef()
{
	// Disconnect Events
	m_buttonConfigFileEntries->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnClickConfigFileEntries ), NULL, this );
	m_buttonTextFont->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnButtonClickFonts ), NULL, this );
	m_checkBoxRotateWithBoat->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnEBLRotateWithBoat ), NULL, this );
	m_checkBoxEBLFixedEndPosition->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnEBLFixedEndPosition ), NULL, this );
	m_checkBoxGZRotateWithBoat->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnEBLRotateWithBoat ), NULL, this );
	m_choiceNumIndexLines->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ODPropertiesDialogDef::OnPILIndexLineChoice ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesOKClick ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesCancelClick ), NULL, this );
	m_buttonApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesApplyClick ), NULL, this );

}
