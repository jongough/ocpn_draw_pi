///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 16 2016)
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
	m_panelBasicProperties = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	m_SizerBasicProperties = new wxBoxSizer( wxVERTICAL );
	
	m_staticText2 = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Properties"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	m_SizerBasicProperties->Add( m_staticText2, 0, wxALL|wxEXPAND, 5 );
	
	m_bSizerOuterProperties = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerName;
	bSizerName = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextName = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextName->Wrap( -1 );
	bSizerName->Add( m_staticTextName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textName = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerName->Add( m_textName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	m_bSizerOuterProperties->Add( bSizerName, 0, wxEXPAND, 5 );
	
	m_SizerNameIcon = new wxBoxSizer( wxHORIZONTAL );
	
	m_fgSizerNameIcon = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerNameIcon->AddGrowableCol( 0 );
	m_fgSizerNameIcon->AddGrowableCol( 1 );
	m_fgSizerNameIcon->SetFlexibleDirection( wxBOTH );
	m_fgSizerNameIcon->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_checkBoxShowName = new wxCheckBox( m_panelBasicProperties, wxID_ANY, _("Show name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerNameIcon->Add( m_checkBoxShowName, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 2 );
	
	m_bcomboBoxODPointIconName = new wxBitmapComboBox( m_panelBasicProperties, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_fgSizerNameIcon->Add( m_bcomboBoxODPointIconName, 0, wxALL|wxEXPAND, 5 );
	
	
	m_SizerNameIcon->Add( m_fgSizerNameIcon, 1, wxEXPAND, 5 );
	
	
	m_bSizerOuterProperties->Add( m_SizerNameIcon, 0, wxEXPAND, 5 );
	
	m_bSizerChangeAllPointIcons = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxChangeAllPointIcons = new wxCheckBox( m_panelBasicProperties, wxID_ANY, _("Change all Path Point Icons"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_bSizerChangeAllPointIcons->Add( m_checkBoxChangeAllPointIcons, 0, wxALL, 5 );
	
	
	m_bSizerOuterProperties->Add( m_bSizerChangeAllPointIcons, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerLatLon;
	bSizerLatLon = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextLatitude = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLatitude->Wrap( -1 );
	bSizerLatLon->Add( m_staticTextLatitude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_textLatitude = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizerLatLon->Add( m_textLatitude, 1, wxALL, 2 );
	
	m_staticTextLogitude = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Longitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLogitude->Wrap( -1 );
	bSizerLatLon->Add( m_staticTextLogitude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_textLongitude = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizerLatLon->Add( m_textLongitude, 1, wxALL, 2 );
	
	
	m_bSizerOuterProperties->Add( bSizerLatLon, 0, wxEXPAND, 5 );
	
	m_bSizerEBLPointWarning = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextEBLPointWarning = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Warning: Lat and Lon cannot be changed when EBL attached to Boat"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEBLPointWarning->Wrap( -1 );
	m_bSizerEBLPointWarning->Add( m_staticTextEBLPointWarning, 0, wxALL, 5 );
	
	
	m_bSizerOuterProperties->Add( m_bSizerEBLPointWarning, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerArrivalRadius;
	bSizerArrivalRadius = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextArrivalRadius = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Arrival Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextArrivalRadius->Wrap( -1 );
	bSizerArrivalRadius->Add( m_staticTextArrivalRadius, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_textCtrlODPointArrivalRadius = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizerArrivalRadius->Add( m_textCtrlODPointArrivalRadius, 0, wxALL, 2 );
	
	
	m_bSizerOuterProperties->Add( bSizerArrivalRadius, 0, wxEXPAND, 5 );
	
	m_bSizerRangeRingsControl = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxShowODPointRangeRings = new wxCheckBox( m_panelBasicProperties, wxID_ANY, _("Show Point Range Rings"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_bSizerRangeRingsControl->Add( m_checkBoxShowODPointRangeRings, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	wxString m_radioBoxBoundaryPointTypeChoices[] = { _("Exclusion"), _("Inclusion"), _("Neither") };
	int m_radioBoxBoundaryPointTypeNChoices = sizeof( m_radioBoxBoundaryPointTypeChoices ) / sizeof( wxString );
	m_radioBoxBoundaryPointType = new wxRadioBox( m_panelBasicProperties, wxID_ANY, _("Point Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxBoundaryPointTypeNChoices, m_radioBoxBoundaryPointTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxBoundaryPointType->SetSelection( 0 );
	m_bSizerRangeRingsControl->Add( m_radioBoxBoundaryPointType, 0, wxALL, 5 );
	
	
	m_bSizerOuterProperties->Add( m_bSizerRangeRingsControl, 0, wxEXPAND, 2 );
	
	m_bSizerFill = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextFillDensity = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Fill Density"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFillDensity->Wrap( -1 );
	m_bSizerFill->Add( m_staticTextFillDensity, 0, wxALL, 5 );
	
	m_sliderBoundaryPointFillTransparency = new wxSlider( m_panelBasicProperties, wxID_ANY, 175, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_bSizerFill->Add( m_sliderBoundaryPointFillTransparency, 1, wxALL|wxEXPAND, 5 );
	
	m_staticTextBoundaryPointInclusionSize = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Point Inclusion Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryPointInclusionSize->Wrap( -1 );
	m_bSizerFill->Add( m_staticTextBoundaryPointInclusionSize, 0, wxALL, 5 );
	
	m_sliderBoundaryPointInclusionSize = new wxSlider( m_panelBasicProperties, wxID_ANY, 15, 0, 50, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_bSizerFill->Add( m_sliderBoundaryPointInclusionSize, 1, wxALL, 5 );
	
	
	m_bSizerOuterProperties->Add( m_bSizerFill, 0, wxEXPAND, 5 );
	
	m_SizerODPointRangeRingsSelect = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizerODPointRangeRingsSelect;
	fgSizerODPointRangeRingsSelect = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizerODPointRangeRingsSelect->AddGrowableCol( 0 );
	fgSizerODPointRangeRingsSelect->SetFlexibleDirection( wxBOTH );
	fgSizerODPointRangeRingsSelect->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextPointRangeRings = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Number of Point Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPointRangeRings->Wrap( -1 );
	fgSizerODPointRangeRingsSelect->Add( m_staticTextPointRangeRings, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2 );
	
	wxString m_choicePointRangeRingsNumberChoices[] = { _("None"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9"), _("10") };
	int m_choicePointRangeRingsNumberNChoices = sizeof( m_choicePointRangeRingsNumberChoices ) / sizeof( wxString );
	m_choicePointRangeRingsNumber = new wxChoice( m_panelBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePointRangeRingsNumberNChoices, m_choicePointRangeRingsNumberChoices, 0 );
	m_choicePointRangeRingsNumber->SetSelection( 0 );
	fgSizerODPointRangeRingsSelect->Add( m_choicePointRangeRingsNumber, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	m_SizerODPointRangeRingsSelect->Add( fgSizerODPointRangeRingsSelect, 0, wxEXPAND, 2 );
	
	m_SizerPointRangeGrid = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_SizerPointRangeGrid->AddGrowableCol( 0 );
	m_SizerPointRangeGrid->SetFlexibleDirection( wxBOTH );
	m_SizerPointRangeGrid->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextPointRangeRingSteps = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Distance between Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPointRangeRingSteps->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextPointRangeRingSteps, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_textCtrlODPointRangeRingsSteps = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_SizerPointRangeGrid->Add( m_textCtrlODPointRangeRingsSteps, 0, wxALL|wxALIGN_RIGHT, 2 );
	
	m_staticTextDistanceUnit = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Distance Unit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDistanceUnit->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextDistanceUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	wxString m_choiceDistanceUnitsStringChoices[] = { _("Nautical Miles"), _("Kilometres") };
	int m_choiceDistanceUnitsStringNChoices = sizeof( m_choiceDistanceUnitsStringChoices ) / sizeof( wxString );
	m_choiceDistanceUnitsString = new wxChoice( m_panelBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDistanceUnitsStringNChoices, m_choiceDistanceUnitsStringChoices, 0 );
	m_choiceDistanceUnitsString->SetSelection( 0 );
	m_SizerPointRangeGrid->Add( m_choiceDistanceUnitsString, 0, wxALIGN_RIGHT|wxALL, 0 );
	
	m_staticTextRangeRingColours = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Range Ring Colours"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingColours->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextRangeRingColours, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_colourPickerRangeRingsColour = new wxColourPickerCtrl( m_panelBasicProperties, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, 0 );
	m_SizerPointRangeGrid->Add( m_colourPickerRangeRingsColour, 0, wxALIGN_RIGHT|wxALL, 0 );
	
	m_staticTextRangeRingWidth = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Range Ring Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingWidth->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextRangeRingWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	wxString m_choiceRangeRingLineWidthChoices[] = { _("1 pixel"), _("2 pixels"), _("3 pixels"), _("4 pixels"), _("5 pixels"), _("6 pixels"), _("7 pixels"), _("8 pixels"), _("9 pixels"), _("10 pixels") };
	int m_choiceRangeRingLineWidthNChoices = sizeof( m_choiceRangeRingLineWidthChoices ) / sizeof( wxString );
	m_choiceRangeRingLineWidth = new wxChoice( m_panelBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRangeRingLineWidthNChoices, m_choiceRangeRingLineWidthChoices, 0 );
	m_choiceRangeRingLineWidth->SetSelection( 1 );
	m_SizerPointRangeGrid->Add( m_choiceRangeRingLineWidth, 0, wxALIGN_RIGHT|wxALL, 2 );
	
	m_staticTextRangeRingStyle = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Range Ring Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingStyle->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextRangeRingStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	wxString m_choiceRangeRingLineStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choiceRangeRingLineStyleNChoices = sizeof( m_choiceRangeRingLineStyleChoices ) / sizeof( wxString );
	m_choiceRangeRingLineStyle = new wxChoice( m_panelBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRangeRingLineStyleNChoices, m_choiceRangeRingLineStyleChoices, 0 );
	m_choiceRangeRingLineStyle->SetSelection( 0 );
	m_SizerPointRangeGrid->Add( m_choiceRangeRingLineStyle, 0, wxALIGN_RIGHT|wxALL, 2 );
	
	
	m_SizerODPointRangeRingsSelect->Add( m_SizerPointRangeGrid, 0, wxEXPAND, 2 );
	
	
	m_bSizerOuterProperties->Add( m_SizerODPointRangeRingsSelect, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerDescription;
	bSizerDescription = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextDescription = new wxStaticText( m_panelBasicProperties, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDescription->Wrap( -1 );
	bSizerDescription->Add( m_staticTextDescription, 0, wxALL, 2 );
	
	wxBoxSizer* bSizerDescriptionExt;
	bSizerDescriptionExt = new wxBoxSizer( wxHORIZONTAL );
	
	m_textDescription = new wxTextCtrl( m_panelBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	m_textDescription->SetMinSize( wxSize( -1,60 ) );
	
	bSizerDescriptionExt->Add( m_textDescription, 1, wxALL|wxEXPAND, 2 );
	
	
	bSizerDescription->Add( bSizerDescriptionExt, 1, wxEXPAND, 5 );
	
	
	m_bSizerOuterProperties->Add( bSizerDescription, 0, wxEXPAND, 5 );
	
	
	m_SizerBasicProperties->Add( m_bSizerOuterProperties, 0, wxEXPAND, 5 );
	
	
	m_panelBasicProperties->SetSizer( m_SizerBasicProperties );
	m_panelBasicProperties->Layout();
	m_SizerBasicProperties->Fit( m_panelBasicProperties );
	m_notebookProperties->AddPage( m_panelBasicProperties, _("Basic"), true );
	m_panelDisplayText = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_bSizerDisplayText = new wxBoxSizer( wxVERTICAL );
	
	m_staticTextDisplayText = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Text to Display with Point"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDisplayText->Wrap( -1 );
	m_bSizerDisplayText->Add( m_staticTextDisplayText, 0, wxALL, 5 );
	
	m_textDisplayText = new wxTextCtrl( m_panelDisplayText, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	m_bSizerDisplayText->Add( m_textDisplayText, 1, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizerTextProperties;
	fgSizerTextProperties = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerTextProperties->SetFlexibleDirection( wxBOTH );
	fgSizerTextProperties->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextPosition = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Text position relative to point"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPosition->Wrap( -1 );
	fgSizerTextProperties->Add( m_staticTextPosition, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choicePositionChoices[] = { _("Top"), _("Top Centre"), _("Bottom"), _("Bottom Centre"), _("Centre"), _("Right"), _("Left") };
	int m_choicePositionNChoices = sizeof( m_choicePositionChoices ) / sizeof( wxString );
	m_choicePosition = new wxChoice( m_panelDisplayText, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePositionNChoices, m_choicePositionChoices, 0 );
	m_choicePosition->SetSelection( 0 );
	fgSizerTextProperties->Add( m_choicePosition, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextColour = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Text Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextColour->Wrap( -1 );
	fgSizerTextProperties->Add( m_staticTextColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerText = new wxColourPickerCtrl( m_panelDisplayText, wxID_ANY, wxColour( 0, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerTextProperties->Add( m_colourPickerText, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextBackgroundColour = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Background Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBackgroundColour->Wrap( -1 );
	fgSizerTextProperties->Add( m_staticTextBackgroundColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerBacgroundColour = new wxColourPickerCtrl( m_panelDisplayText, wxID_ANY, wxColour( 255, 255, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerTextProperties->Add( m_colourPickerBacgroundColour, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextBackgroundTransparency = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Background Transparency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBackgroundTransparency->Wrap( -1 );
	fgSizerTextProperties->Add( m_staticTextBackgroundTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sliderBackgroundTransparency = new wxSlider( m_panelDisplayText, wxID_ANY, 100, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_sliderBackgroundTransparency->SetMinSize( wxSize( 100,-1 ) );
	
	fgSizerTextProperties->Add( m_sliderBackgroundTransparency, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizerFontFace;
	bSizerFontFace = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextFont = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Text Font"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFont->Wrap( -1 );
	bSizerFontFace->Add( m_staticTextFont, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticTextFontFaceExample = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Example"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticTextFontFaceExample->Wrap( -1 );
	bSizerFontFace->Add( m_staticTextFontFaceExample, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	fgSizerTextProperties->Add( bSizerFontFace, 1, wxEXPAND, 5 );
	
	m_buttonTextFont = new wxButton( m_panelDisplayText, wxID_ANY, _("Fonts"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerTextProperties->Add( m_buttonTextFont, 0, wxALL, 5 );
	
	wxString m_radioBoxShowDisplayTextChoices[] = { _("Always"), _("On Rollover Only"), _("Never") };
	int m_radioBoxShowDisplayTextNChoices = sizeof( m_radioBoxShowDisplayTextChoices ) / sizeof( wxString );
	m_radioBoxShowDisplayText = new wxRadioBox( m_panelDisplayText, wxID_ANY, _("Show Display Text"), wxDefaultPosition, wxDefaultSize, m_radioBoxShowDisplayTextNChoices, m_radioBoxShowDisplayTextChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxShowDisplayText->SetSelection( 0 );
	fgSizerTextProperties->Add( m_radioBoxShowDisplayText, 0, wxALL, 5 );
	
	
	m_bSizerDisplayText->Add( fgSizerTextProperties, 1, wxEXPAND, 5 );
	
	
	m_panelDisplayText->SetSizer( m_bSizerDisplayText );
	m_panelDisplayText->Layout();
	m_bSizerDisplayText->Fit( m_panelDisplayText );
	m_notebookProperties->AddPage( m_panelDisplayText, _("Display text"), false );
	m_panelExtended = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_checkBoxVisible = new wxCheckBox( m_panelExtended, wxID_ANY, _("Show on chart"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBoxVisible->SetValue(true); 
	bSizer3->Add( m_checkBoxVisible, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1 = new wxStaticText( m_panelExtended, wxID_ANY, _("GUID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer4->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlGuid = new wxTextCtrl( m_panelExtended, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_textCtrlGuid, 1, wxALL, 5 );
	
	
	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );
	
	
	m_panelExtended->SetSizer( bSizer3 );
	m_panelExtended->Layout();
	bSizer3->Fit( m_panelExtended );
	m_notebookProperties->AddPage( m_panelExtended, _("Extended"), false );
	
	m_SizerDialogBox->Add( m_notebookProperties, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_OK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_OK, 0, wxALL, 5 );
	
	m_Cancel = new wxButton( this, wxID_ANY, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
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
	m_textCtrlODPointArrivalRadius->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnArrivalRadiusChange ), NULL, this );
	m_checkBoxShowODPointRangeRings->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnShowRangeRingsSelect ), NULL, this );
	m_radioBoxBoundaryPointType->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( ODPointPropertiesDialog::OnRadioBoxPointType ), NULL, this );
	m_textCtrlODPointRangeRingsSteps->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnRangeRingsStepChange ), NULL, this );
	m_textDescription->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ODPointPropertiesDialog::OnDescChangedBasic ), NULL, this );
	m_buttonTextFont->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnButtonClickFonts ), NULL, this );
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
	m_textCtrlODPointArrivalRadius->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnArrivalRadiusChange ), NULL, this );
	m_checkBoxShowODPointRangeRings->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnShowRangeRingsSelect ), NULL, this );
	m_radioBoxBoundaryPointType->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( ODPointPropertiesDialog::OnRadioBoxPointType ), NULL, this );
	m_textCtrlODPointRangeRingsSteps->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnRangeRingsStepChange ), NULL, this );
	m_textDescription->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ODPointPropertiesDialog::OnDescChangedBasic ), NULL, this );
	m_buttonTextFont->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnButtonClickFonts ), NULL, this );
	m_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnPointPropertiesOKClick ), NULL, this );
	m_Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnPointPropertiesCancelClick ), NULL, this );
	
}
