///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODPointPropertiesDialog.h"

///////////////////////////////////////////////////////////////////////////

ODPointPropertiesDialog::ODPointPropertiesDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_SizerDialogBox = new wxBoxSizer( wxVERTICAL );

	m_notebookProperties = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_scrolledWindowBasicProperties = new wxScrolledWindow( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_scrolledWindowBasicProperties->SetScrollRate( 0, 2 );
	m_SizerBasicProperties = new wxBoxSizer( wxVERTICAL );

	m_staticText2 = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Properties"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	m_SizerBasicProperties->Add( m_staticText2, 0, wxALL|wxEXPAND, 5 );

	m_SizerOuterProperties = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizerName;
	bSizerName = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextName = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextName->Wrap( -1 );
	bSizerName->Add( m_staticTextName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textName = new wxTextCtrl( m_scrolledWindowBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizerName->Add( m_textName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	m_SizerOuterProperties->Add( bSizerName, 0, wxEXPAND, 5 );

	m_SizerNameIcon = new wxBoxSizer( wxHORIZONTAL );

	m_fgSizerNameIcon = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerNameIcon->AddGrowableCol( 0 );
	m_fgSizerNameIcon->AddGrowableCol( 1 );
	m_fgSizerNameIcon->SetFlexibleDirection( wxBOTH );
	m_fgSizerNameIcon->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxShowName = new wxCheckBox( m_scrolledWindowBasicProperties, wxID_ANY, _("Show name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerNameIcon->Add( m_checkBoxShowName, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 2 );

	m_bcomboBoxODPointIconName = new wxBitmapComboBox( m_scrolledWindowBasicProperties, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_fgSizerNameIcon->Add( m_bcomboBoxODPointIconName, 0, wxALL|wxEXPAND, 5 );


	m_SizerNameIcon->Add( m_fgSizerNameIcon, 1, wxEXPAND, 5 );


	m_SizerOuterProperties->Add( m_SizerNameIcon, 0, wxEXPAND, 5 );

	m_bSizerChangeAllPointIcons = new wxBoxSizer( wxHORIZONTAL );

	m_checkBoxChangeAllPointIcons = new wxCheckBox( m_scrolledWindowBasicProperties, wxID_ANY, _("Change all Path Point Icons"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_bSizerChangeAllPointIcons->Add( m_checkBoxChangeAllPointIcons, 0, wxALL, 5 );


	m_SizerOuterProperties->Add( m_bSizerChangeAllPointIcons, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizerLatLon;
	bSizerLatLon = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextLatitude = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Latitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLatitude->Wrap( -1 );
	bSizerLatLon->Add( m_staticTextLatitude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	m_textLatitude = new wxTextCtrl( m_scrolledWindowBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizerLatLon->Add( m_textLatitude, 1, wxALL, 2 );

	m_staticTextLogitude = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Longitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLogitude->Wrap( -1 );
	bSizerLatLon->Add( m_staticTextLogitude, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	m_textLongitude = new wxTextCtrl( m_scrolledWindowBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizerLatLon->Add( m_textLongitude, 1, wxALL, 2 );


	m_SizerOuterProperties->Add( bSizerLatLon, 0, wxEXPAND, 5 );

	m_bSizerEBLPointWarning = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextEBLPointWarning = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Warning: Lat and Lon cannot be changed when EBL end point moving"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEBLPointWarning->Wrap( -1 );
	m_bSizerEBLPointWarning->Add( m_staticTextEBLPointWarning, 0, wxALL, 5 );


	m_SizerOuterProperties->Add( m_bSizerEBLPointWarning, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizerArrivalRadius;
	bSizerArrivalRadius = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextArrivalRadius = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Arrival Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextArrivalRadius->Wrap( -1 );
	bSizerArrivalRadius->Add( m_staticTextArrivalRadius, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	m_textCtrlODPointArrivalRadius = new wxTextCtrl( m_scrolledWindowBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	bSizerArrivalRadius->Add( m_textCtrlODPointArrivalRadius, 0, wxALL, 2 );


	m_SizerOuterProperties->Add( bSizerArrivalRadius, 0, wxEXPAND, 5 );

	m_bSizerRangeRingsControl = new wxBoxSizer( wxHORIZONTAL );

	m_checkBoxShowODPointRangeRings = new wxCheckBox( m_scrolledWindowBasicProperties, wxID_ANY, _("Show Point Range Rings"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_bSizerRangeRingsControl->Add( m_checkBoxShowODPointRangeRings, 0, wxALL|wxEXPAND, 0 );

	wxString m_radioBoxBoundaryPointTypeChoices[] = { _("Exclusion"), _("Inclusion"), _("Neither") };
	int m_radioBoxBoundaryPointTypeNChoices = sizeof( m_radioBoxBoundaryPointTypeChoices ) / sizeof( wxString );
	m_radioBoxBoundaryPointType = new wxRadioBox( m_scrolledWindowBasicProperties, wxID_ANY, _("Point Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxBoundaryPointTypeNChoices, m_radioBoxBoundaryPointTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxBoundaryPointType->SetSelection( 0 );
	m_bSizerRangeRingsControl->Add( m_radioBoxBoundaryPointType, 1, wxALL|wxEXPAND, 5 );


	m_SizerOuterProperties->Add( m_bSizerRangeRingsControl, 0, wxEXPAND, 2 );

	m_SizerFill = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextFillDensity = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Fill Density"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFillDensity->Wrap( -1 );
	m_SizerFill->Add( m_staticTextFillDensity, 0, wxALL, 5 );

	m_sliderBoundaryPointFillTransparency = new wxSlider( m_scrolledWindowBasicProperties, wxID_ANY, 175, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS|wxALWAYS_SHOW_SB );
	m_SizerFill->Add( m_sliderBoundaryPointFillTransparency, 1, wxALL|wxEXPAND, 5 );

	m_staticTextBoundaryPointInclusionSize = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Point Inclusion Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryPointInclusionSize->Wrap( -1 );
	m_SizerFill->Add( m_staticTextBoundaryPointInclusionSize, 0, wxALL, 5 );

	m_sliderBoundaryPointInclusionSize = new wxSlider( m_scrolledWindowBasicProperties, wxID_ANY, 15, 0, 50, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_SizerFill->Add( m_sliderBoundaryPointInclusionSize, 1, wxALL, 5 );


	m_SizerOuterProperties->Add( m_SizerFill, 0, wxEXPAND, 5 );

	m_SizerODPointRangeRingsSelect = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizerODPointRangeRingsSelect;
	fgSizerODPointRangeRingsSelect = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizerODPointRangeRingsSelect->AddGrowableCol( 0 );
	fgSizerODPointRangeRingsSelect->SetFlexibleDirection( wxBOTH );
	fgSizerODPointRangeRingsSelect->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPointRangeRings = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Number of Point Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPointRangeRings->Wrap( -1 );
	fgSizerODPointRangeRingsSelect->Add( m_staticTextPointRangeRings, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2 );

	wxString m_choicePointRangeRingsNumberChoices[] = { _("None"), _("1"), _("2"), _("3"), _("4"), _("5"), _("6"), _("7"), _("8"), _("9"), _("10") };
	int m_choicePointRangeRingsNumberNChoices = sizeof( m_choicePointRangeRingsNumberChoices ) / sizeof( wxString );
	m_choicePointRangeRingsNumber = new wxChoice( m_scrolledWindowBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePointRangeRingsNumberNChoices, m_choicePointRangeRingsNumberChoices, 0 );
	m_choicePointRangeRingsNumber->SetSelection( 0 );
	fgSizerODPointRangeRingsSelect->Add( m_choicePointRangeRingsNumber, 0, wxALIGN_RIGHT|wxALL, 5 );


	m_SizerODPointRangeRingsSelect->Add( fgSizerODPointRangeRingsSelect, 0, wxEXPAND, 2 );

	m_SizerPointRangeGrid = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_SizerPointRangeGrid->AddGrowableCol( 0 );
	m_SizerPointRangeGrid->SetFlexibleDirection( wxBOTH );
	m_SizerPointRangeGrid->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPointRangeRingSteps = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Distance between Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPointRangeRingSteps->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextPointRangeRingSteps, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	m_textCtrlODPointRangeRingsSteps = new wxTextCtrl( m_scrolledWindowBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_SizerPointRangeGrid->Add( m_textCtrlODPointRangeRingsSteps, 0, wxALL|wxALIGN_RIGHT, 2 );

	m_staticTextDistanceUnit = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Distance Unit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDistanceUnit->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextDistanceUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	wxString m_choiceDistanceUnitsStringChoices[] = { _("Nautical Miles"), _("Kilometres") };
	int m_choiceDistanceUnitsStringNChoices = sizeof( m_choiceDistanceUnitsStringChoices ) / sizeof( wxString );
	m_choiceDistanceUnitsString = new wxChoice( m_scrolledWindowBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceDistanceUnitsStringNChoices, m_choiceDistanceUnitsStringChoices, 0 );
	m_choiceDistanceUnitsString->SetSelection( 0 );
	m_SizerPointRangeGrid->Add( m_choiceDistanceUnitsString, 0, wxALIGN_RIGHT|wxALL, 0 );

	m_staticTextRangeRingColours = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Range Ring Colours"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingColours->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextRangeRingColours, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	m_colourPickerRangeRingsColour = new wxColourPickerCtrl( m_scrolledWindowBasicProperties, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, 0 );
	m_SizerPointRangeGrid->Add( m_colourPickerRangeRingsColour, 0, wxALIGN_RIGHT|wxALL, 0 );

	m_staticTextRangeRingWidth = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Range Ring Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingWidth->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextRangeRingWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	wxString m_choiceRangeRingLineWidthChoices[] = { _("1 pixel"), _("2 pixels"), _("3 pixels"), _("4 pixels"), _("5 pixels"), _("6 pixels"), _("7 pixels"), _("8 pixels"), _("9 pixels"), _("10 pixels") };
	int m_choiceRangeRingLineWidthNChoices = sizeof( m_choiceRangeRingLineWidthChoices ) / sizeof( wxString );
	m_choiceRangeRingLineWidth = new wxChoice( m_scrolledWindowBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRangeRingLineWidthNChoices, m_choiceRangeRingLineWidthChoices, 0 );
	m_choiceRangeRingLineWidth->SetSelection( 1 );
	m_SizerPointRangeGrid->Add( m_choiceRangeRingLineWidth, 0, wxALIGN_RIGHT|wxALL, 2 );

	m_staticTextRangeRingStyle = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Range Ring Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingStyle->Wrap( -1 );
	m_SizerPointRangeGrid->Add( m_staticTextRangeRingStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

	wxString m_choiceRangeRingLineStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choiceRangeRingLineStyleNChoices = sizeof( m_choiceRangeRingLineStyleChoices ) / sizeof( wxString );
	m_choiceRangeRingLineStyle = new wxChoice( m_scrolledWindowBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceRangeRingLineStyleNChoices, m_choiceRangeRingLineStyleChoices, 0 );
	m_choiceRangeRingLineStyle->SetSelection( 0 );
	m_SizerPointRangeGrid->Add( m_choiceRangeRingLineStyle, 0, wxALIGN_RIGHT|wxALL, 2 );


	m_SizerODPointRangeRingsSelect->Add( m_SizerPointRangeGrid, 0, wxEXPAND, 2 );


	m_SizerOuterProperties->Add( m_SizerODPointRangeRingsSelect, 0, wxEXPAND, 5 );

	m_SizerDescription = new wxBoxSizer( wxVERTICAL );

	m_staticTextDescription = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDescription->Wrap( -1 );
	m_SizerDescription->Add( m_staticTextDescription, 0, wxALL, 2 );

	m_SizerDescriptionExt = new wxBoxSizer( wxHORIZONTAL );

	m_textDescription = new wxTextCtrl( m_scrolledWindowBasicProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	m_SizerDescriptionExt->Add( m_textDescription, 1, wxALL|wxEXPAND, 2 );


	m_SizerDescription->Add( m_SizerDescriptionExt, 1, wxEXPAND, 5 );


	m_SizerOuterProperties->Add( m_SizerDescription, 1, wxEXPAND, 5 );

	m_bSizerHyperlink = new wxBoxSizer( wxVERTICAL );

	m_staticTextHyperlink = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Links"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextHyperlink->Wrap( -1 );
	m_bSizerHyperlink->Add( m_staticTextHyperlink, 0, wxALL, 5 );

	m_bSizerHyperlinkExt = new wxBoxSizer( wxVERTICAL );

	m_scrolledWindowLinks = new wxScrolledWindow( m_scrolledWindowBasicProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL );
	m_scrolledWindowLinks->SetScrollRate( 0, 2 );
	m_bSizerLinks = new wxBoxSizer( wxVERTICAL );

	m_hyperlinkCtrl = new wxHyperlinkCtrl( m_scrolledWindowLinks, wxID_ANY, _("wxFB Website"), wxT("http://www.wxformbuilder.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	m_bSizerLinks->Add( m_hyperlinkCtrl, 1, wxALIGN_LEFT|wxALL, 0 );


	m_scrolledWindowLinks->SetSizer( m_bSizerLinks );
	m_scrolledWindowLinks->Layout();
	m_bSizerLinks->Fit( m_scrolledWindowLinks );
	m_menuLink = new wxMenu();
	m_menuItemDelete = new wxMenuItem( m_menuLink, wxID_ANY, wxString( _("Delete") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuLink->Append( m_menuItemDelete );

	m_menuItemEdit = new wxMenuItem( m_menuLink, wxID_ANY, wxString( _("Edit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuLink->Append( m_menuItemEdit );

	m_menuItemAddNew = new wxMenuItem( m_menuLink, wxID_ANY, wxString( _("Add New") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuLink->Append( m_menuItemAddNew );

	m_scrolledWindowLinks->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPointPropertiesDialog::m_scrolledWindowLinksOnContextMenu ), NULL, this );

	m_bSizerHyperlinkExt->Add( m_scrolledWindowLinks, 1, wxEXPAND | wxALL, 0 );

	m_staticTextLinkInfo = new wxStaticText( m_scrolledWindowBasicProperties, wxID_ANY, _("Left click opens in browser"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLinkInfo->Wrap( -1 );
	m_bSizerHyperlinkExt->Add( m_staticTextLinkInfo, 0, wxALL, 5 );

	m_bSizerLinkButtons = new wxBoxSizer( wxHORIZONTAL );

	m_buttonAddLink = new wxButton( m_scrolledWindowBasicProperties, wxID_ANY, _("&Add"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLinkButtons->Add( m_buttonAddLink, 0, wxALL, 5 );

	m_toggleBtnEditLink = new wxToggleButton( m_scrolledWindowBasicProperties, wxID_ANY, _("&Edit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLinkButtons->Add( m_toggleBtnEditLink, 0, wxALL, 5 );

	m_toggleBtnDeleteLink = new wxToggleButton( m_scrolledWindowBasicProperties, wxID_ANY, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLinkButtons->Add( m_toggleBtnDeleteLink, 0, wxALL, 5 );


	m_bSizerHyperlinkExt->Add( m_bSizerLinkButtons, 0, 0, 5 );


	m_bSizerHyperlink->Add( m_bSizerHyperlinkExt, 1, wxEXPAND, 5 );


	m_SizerOuterProperties->Add( m_bSizerHyperlink, 0, wxEXPAND, 5 );


	m_SizerBasicProperties->Add( m_SizerOuterProperties, 0, wxEXPAND, 5 );


	m_scrolledWindowBasicProperties->SetSizer( m_SizerBasicProperties );
	m_scrolledWindowBasicProperties->Layout();
	m_SizerBasicProperties->Fit( m_scrolledWindowBasicProperties );
	m_notebookProperties->AddPage( m_scrolledWindowBasicProperties, _("Basic"), true );
	m_panelDisplayText = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_SizerDisplayText = new wxBoxSizer( wxVERTICAL );

	m_staticTextDisplayText = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Text to Display with Point"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDisplayText->Wrap( -1 );
	m_SizerDisplayText->Add( m_staticTextDisplayText, 0, wxALL, 5 );

	m_textDisplayText = new wxTextCtrl( m_panelDisplayText, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	m_SizerDisplayText->Add( m_textDisplayText, 1, wxALL|wxEXPAND, 5 );

	m_fgSizerTextProperties = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerTextProperties->SetFlexibleDirection( wxBOTH );
	m_fgSizerTextProperties->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPosition = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Text position relative to point"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPosition->Wrap( -1 );
	m_fgSizerTextProperties->Add( m_staticTextPosition, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choicePositionChoices[] = { _("Top"), _("Top Centre"), _("Bottom"), _("Bottom Centre"), _("Centre"), _("Right"), _("Left") };
	int m_choicePositionNChoices = sizeof( m_choicePositionChoices ) / sizeof( wxString );
	m_choicePosition = new wxChoice( m_panelDisplayText, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePositionNChoices, m_choicePositionChoices, 0 );
	m_choicePosition->SetSelection( 0 );
	m_fgSizerTextProperties->Add( m_choicePosition, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextColour = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Text Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextColour->Wrap( -1 );
	m_fgSizerTextProperties->Add( m_staticTextColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerText = new wxColourPickerCtrl( m_panelDisplayText, wxID_ANY, wxColour( 0, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_fgSizerTextProperties->Add( m_colourPickerText, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextBackgroundColour = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Background Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBackgroundColour->Wrap( -1 );
	m_fgSizerTextProperties->Add( m_staticTextBackgroundColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerBacgroundColour = new wxColourPickerCtrl( m_panelDisplayText, wxID_ANY, wxColour( 255, 255, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_fgSizerTextProperties->Add( m_colourPickerBacgroundColour, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_staticTextBackgroundTransparency = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Background Transparency"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBackgroundTransparency->Wrap( -1 );
	m_fgSizerTextProperties->Add( m_staticTextBackgroundTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_sliderBackgroundTransparency = new wxSlider( m_panelDisplayText, wxID_ANY, 100, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_sliderBackgroundTransparency->SetMinSize( wxSize( 100,-1 ) );

	m_fgSizerTextProperties->Add( m_sliderBackgroundTransparency, 0, wxALL|wxEXPAND, 5 );

	wxString m_radioBoxWidthTypeChoices[] = { _("Font Based"), _("Character Based") };
	int m_radioBoxWidthTypeNChoices = sizeof( m_radioBoxWidthTypeChoices ) / sizeof( wxString );
	m_radioBoxWidthType = new wxRadioBox( m_panelDisplayText, wxID_ANY, _("Maximum Width Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxWidthTypeNChoices, m_radioBoxWidthTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxWidthType->SetSelection( 0 );
	m_fgSizerTextProperties->Add( m_radioBoxWidthType, 0, wxALL, 5 );


	m_fgSizerTextProperties->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticTextMaxWidth = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Maximum Text Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMaxWidth->Wrap( -1 );
	m_fgSizerTextProperties->Add( m_staticTextMaxWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlTextMaxWidth = new wxTextCtrl( m_panelDisplayText, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_fgSizerTextProperties->Add( m_textCtrlTextMaxWidth, 0, wxALL, 5 );

	wxBoxSizer* bSizerFontFace;
	bSizerFontFace = new wxBoxSizer( wxHORIZONTAL );

	m_staticTextFont = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Text Font"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFont->Wrap( -1 );
	bSizerFontFace->Add( m_staticTextFont, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextFontFaceExample = new wxStaticText( m_panelDisplayText, wxID_ANY, _("Example"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticTextFontFaceExample->Wrap( -1 );
	bSizerFontFace->Add( m_staticTextFontFaceExample, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	m_fgSizerTextProperties->Add( bSizerFontFace, 1, wxEXPAND, 5 );

	m_buttonTextFont = new wxButton( m_panelDisplayText, wxID_ANY, _("Fonts"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fgSizerTextProperties->Add( m_buttonTextFont, 0, wxALL, 5 );

	wxString m_radioBoxShowDisplayTextChoices[] = { _("Always"), _("On Rollover Only"), _("Never") };
	int m_radioBoxShowDisplayTextNChoices = sizeof( m_radioBoxShowDisplayTextChoices ) / sizeof( wxString );
	m_radioBoxShowDisplayText = new wxRadioBox( m_panelDisplayText, wxID_ANY, _("Show Display Text"), wxDefaultPosition, wxDefaultSize, m_radioBoxShowDisplayTextNChoices, m_radioBoxShowDisplayTextChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxShowDisplayText->SetSelection( 1 );
	m_fgSizerTextProperties->Add( m_radioBoxShowDisplayText, 0, wxALL, 5 );


	m_SizerDisplayText->Add( m_fgSizerTextProperties, 1, wxEXPAND, 5 );


	m_panelDisplayText->SetSizer( m_SizerDisplayText );
	m_panelDisplayText->Layout();
	m_SizerDisplayText->Fit( m_panelDisplayText );
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

	m_SizerButtons = new wxBoxSizer( wxHORIZONTAL );

	m_OK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SizerButtons->Add( m_OK, 0, wxALL|wxFIXED_MINSIZE, 5 );

	m_Cancel = new wxButton( this, wxID_ANY, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );

	m_Cancel->SetDefault();
	m_SizerButtons->Add( m_Cancel, 0, wxALL, 5 );


	m_SizerDialogBox->Add( m_SizerButtons, 0, wxEXPAND|wxFIXED_MINSIZE, 5 );


	this->SetSizer( m_SizerDialogBox );
	this->Layout();
	m_SizerDialogBox->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODPointPropertiesDialog::OnPointPropertiesClose ) );
	m_textLatitude->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPointPropertiesDialog::OnRightClick ), NULL, this );
	m_textLatitude->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnPositionCtlUpdated ), NULL, this );
	m_textLongitude->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPointPropertiesDialog::OnRightClick ), NULL, this );
	m_textLongitude->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnPositionCtlUpdated ), NULL, this );
	m_checkBoxShowODPointRangeRings->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnShowRangeRingsSelect ), NULL, this );
	m_radioBoxBoundaryPointType->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( ODPointPropertiesDialog::OnRadioBoxPointType ), NULL, this );
	m_textDescription->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ODPointPropertiesDialog::OnDescChangedBasic ), NULL, this );
	m_menuLink->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ODPointPropertiesDialog::OnMenuSelection ), this, m_menuItemDelete->GetId());
	m_menuLink->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ODPointPropertiesDialog::OnMenuSelection ), this, m_menuItemEdit->GetId());
	m_menuLink->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ODPointPropertiesDialog::OnMenuSelection ), this, m_menuItemAddNew->GetId());
	m_buttonAddLink->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnAddLink ), NULL, this );
	m_toggleBtnEditLink->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnEditLinkToggle ), NULL, this );
	m_toggleBtnDeleteLink->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnDeleteLinkToggle ), NULL, this );
	m_buttonTextFont->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnButtonClickFonts ), NULL, this );
	m_OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnPointPropertiesOKClick ), NULL, this );
	m_Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnPointPropertiesCancelClick ), NULL, this );
}

ODPointPropertiesDialog::~ODPointPropertiesDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODPointPropertiesDialog::OnPointPropertiesClose ) );
	m_textLatitude->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPointPropertiesDialog::OnRightClick ), NULL, this );
	m_textLatitude->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnPositionCtlUpdated ), NULL, this );
	m_textLongitude->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPointPropertiesDialog::OnRightClick ), NULL, this );
	m_textLongitude->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ODPointPropertiesDialog::OnPositionCtlUpdated ), NULL, this );
	m_checkBoxShowODPointRangeRings->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnShowRangeRingsSelect ), NULL, this );
	m_radioBoxBoundaryPointType->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( ODPointPropertiesDialog::OnRadioBoxPointType ), NULL, this );
	m_textDescription->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( ODPointPropertiesDialog::OnDescChangedBasic ), NULL, this );
	m_buttonAddLink->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnAddLink ), NULL, this );
	m_toggleBtnEditLink->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnEditLinkToggle ), NULL, this );
	m_toggleBtnDeleteLink->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnDeleteLinkToggle ), NULL, this );
	m_buttonTextFont->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnButtonClickFonts ), NULL, this );
	m_OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnPointPropertiesOKClick ), NULL, this );
	m_Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPointPropertiesDialog::OnPointPropertiesCancelClick ), NULL, this );

	delete m_menuLink;
}
