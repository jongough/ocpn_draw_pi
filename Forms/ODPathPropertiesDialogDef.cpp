///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ODPathPropertiesDialogDef.h"

///////////////////////////////////////////////////////////////////////////

ODPathPropertiesDialogDef::ODPathPropertiesDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_bSizerDialogBox = new wxBoxSizer( wxVERTICAL );

	m_scrolledWindowProperties = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_scrolledWindowProperties->SetScrollRate( 0, 2 );
	m_fgSizerProperties = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgSizerProperties->SetFlexibleDirection( wxVERTICAL );
	m_fgSizerProperties->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_bSizerNameDescription = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizerDescription;
	fgSizerDescription = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerDescription->AddGrowableCol( 1 );
	fgSizerDescription->SetFlexibleDirection( wxBOTH );
	fgSizerDescription->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextName = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextName->Wrap( -1 );
	fgSizerDescription->Add( m_staticTextName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlName = new wxTextCtrl( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerDescription->Add( m_textCtrlName, 0, wxALL|wxEXPAND, 5 );

	m_staticTextDescription = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDescription->Wrap( -1 );
	fgSizerDescription->Add( m_staticTextDescription, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlDesctiption = new wxTextCtrl( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerDescription->Add( m_textCtrlDesctiption, 0, wxALL|wxEXPAND, 5 );

	m_staticTextGUID = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("GUID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGUID->Wrap( -1 );
	fgSizerDescription->Add( m_staticTextGUID, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlGUID = new wxTextCtrl( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizerDescription->Add( m_textCtrlGUID, 0, wxALL|wxEXPAND, 5 );


	m_bSizerNameDescription->Add( fgSizerDescription, 1, wxEXPAND, 5 );


	m_fgSizerProperties->Add( m_bSizerNameDescription, 0, wxEXPAND, 5 );

	m_fgSizerBoundary = new wxFlexGridSizer( 0, 6, 0, 0 );
	m_fgSizerBoundary->SetFlexibleDirection( wxBOTH );
	m_fgSizerBoundary->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxActive = new wxCheckBox( m_scrolledWindowProperties, wxID_ANY, _("Active"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxActive->SetValue(true);
	m_fgSizerBoundary->Add( m_checkBoxActive, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextTotalLength = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Total Length"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTotalLength->Wrap( -1 );
	m_fgSizerBoundary->Add( m_staticTextTotalLength, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlTotalLength = new wxTextCtrl( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_fgSizerBoundary->Add( m_textCtrlTotalLength, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextDistUntis = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDistUntis->Wrap( -1 );
	m_fgSizerBoundary->Add( m_staticTextDistUntis, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	m_fgSizerBoundary->Add( 0, 0, 1, wxEXPAND, 5 );


	m_fgSizerBoundary->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticTextLineColour = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLineColour->Wrap( -1 );
	m_fgSizerBoundary->Add( m_staticTextLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerLineColour = new wxColourPickerCtrl( m_scrolledWindowProperties, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_fgSizerBoundary->Add( m_colourPickerLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextLineStyle = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLineStyle->Wrap( -1 );
	m_fgSizerBoundary->Add( m_staticTextLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceLineStyleChoices[] = { _("Solid"), _("Dot"), _("Long Dash"), _("Short Dash"), _("Dot Dash") };
	int m_choiceLineStyleNChoices = sizeof( m_choiceLineStyleChoices ) / sizeof( wxString );
	m_choiceLineStyle = new wxChoice( m_scrolledWindowProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceLineStyleNChoices, m_choiceLineStyleChoices, 0 );
	m_choiceLineStyle->SetSelection( 0 );
	m_fgSizerBoundary->Add( m_choiceLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextLineWidth = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextLineWidth->Wrap( -1 );
	m_fgSizerBoundary->Add( m_staticTextLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_choiceLineWidthChoices[] = { _("1 pixel"), _("2 pixels"), _("3 pixels"), _("4 pixels"), _("5 pixels"), _("6 pixels"), _("7 pixels"), _("8 pixels"), _("9 pixels"), _("10 pixels") };
	int m_choiceLineWidthNChoices = sizeof( m_choiceLineWidthChoices ) / sizeof( wxString );
	m_choiceLineWidth = new wxChoice( m_scrolledWindowProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceLineWidthNChoices, m_choiceLineWidthChoices, 0 );
	m_choiceLineWidth->SetSelection( 1 );
	m_fgSizerBoundary->Add( m_choiceLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextFillColour = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFillColour->Wrap( -1 );
	m_fgSizerBoundary->Add( m_staticTextFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_colourPickerFillColour = new wxColourPickerCtrl( m_scrolledWindowProperties, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	m_fgSizerBoundary->Add( m_colourPickerFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_staticTextFillTransparency = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Fill Density"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFillTransparency->Wrap( -1 );
	m_fgSizerBoundary->Add( m_staticTextFillTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_sliderFillTransparency = new wxSlider( m_scrolledWindowProperties, wxID_ANY, 150, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_fgSizerBoundary->Add( m_sliderFillTransparency, 0, wxALL|wxEXPAND, 5 );

	m_staticTextIncluseionBoundarySize = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Inclusion Boundary\nSize"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextIncluseionBoundarySize->Wrap( -1 );
	m_fgSizerBoundary->Add( m_staticTextIncluseionBoundarySize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_sliderInclusionBoundarySize = new wxSlider( m_scrolledWindowProperties, wxID_ANY, 15, 0, 50, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	m_fgSizerBoundary->Add( m_sliderInclusionBoundarySize, 1, wxALL|wxEXPAND, 5 );


	m_fgSizerProperties->Add( m_fgSizerBoundary, 0, wxEXPAND, 5 );

	m_bSizerBoundaryType = new wxBoxSizer( wxHORIZONTAL );

	wxString m_radioBoxBoundaryTypeChoices[] = { _("Exclusion"), _("Inclusion"), _("Neither") };
	int m_radioBoxBoundaryTypeNChoices = sizeof( m_radioBoxBoundaryTypeChoices ) / sizeof( wxString );
	m_radioBoxBoundaryType = new wxRadioBox( m_scrolledWindowProperties, wxID_ANY, _("Boundary Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxBoundaryTypeNChoices, m_radioBoxBoundaryTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxBoundaryType->SetSelection( 0 );
	m_bSizerBoundaryType->Add( m_radioBoxBoundaryType, 0, wxALL|wxEXPAND, 5 );

	m_checkBoxShowBoundaryPoints = new wxCheckBox( m_scrolledWindowProperties, wxID_ANY, _("Show Boundary Point Icons"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_bSizerBoundaryType->Add( m_checkBoxShowBoundaryPoints, 0, wxALL, 5 );


	m_fgSizerProperties->Add( m_bSizerBoundaryType, 0, wxEXPAND, 5 );

	m_fgSizerPIL = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerPIL->SetFlexibleDirection( wxBOTH );
	m_fgSizerPIL->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextPILAngle = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Parallel Index Line Angle (+Stbd/-Port)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILAngle->Wrap( -1 );
	m_fgSizerPIL->Add( m_staticTextPILAngle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlPILAngle = new wxTextCtrl( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_fgSizerPIL->Add( m_textCtrlPILAngle, 0, wxALL, 5 );


	m_fgSizerProperties->Add( m_fgSizerPIL, 0, wxEXPAND, 5 );

	m_fgSizerEBL = new wxFlexGridSizer( 0, 4, 0, 0 );
	m_fgSizerEBL->SetFlexibleDirection( wxBOTH );
	m_fgSizerEBL->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxRotateWithBoat = new wxCheckBox( m_scrolledWindowProperties, wxID_ANY, _("Rotate with\nBoat"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBL->Add( m_checkBoxRotateWithBoat, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	wxString m_radioBoxMaintainWithChoices[] = { _("Heading"), _("Course over Ground") };
	int m_radioBoxMaintainWithNChoices = sizeof( m_radioBoxMaintainWithChoices ) / sizeof( wxString );
	m_radioBoxMaintainWith = new wxRadioBox( m_scrolledWindowProperties, wxID_ANY, _("Maintain with"), wxDefaultPosition, wxDefaultSize, m_radioBoxMaintainWithNChoices, m_radioBoxMaintainWithChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxMaintainWith->SetSelection( 0 );
	m_radioBoxMaintainWith->Enable( false );

	m_fgSizerEBL->Add( m_radioBoxMaintainWith, 0, wxALL, 5 );

	m_staticTextEBLAngle = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("EBL Angle (-P/+S)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEBLAngle->Wrap( -1 );
	m_fgSizerEBL->Add( m_staticTextEBLAngle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlEBLAngle = new wxTextCtrl( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_fgSizerEBL->Add( m_textCtrlEBLAngle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	m_fgSizerProperties->Add( m_fgSizerEBL, 0, wxEXPAND, 5 );

	m_fgSizerEBL1 = new wxFlexGridSizer( 0, 4, 0, 0 );
	m_fgSizerEBL1->SetFlexibleDirection( wxBOTH );
	m_fgSizerEBL1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxEBLFixedEndPosition = new wxCheckBox( m_scrolledWindowProperties, wxID_ANY, _("EBL Fixed\nEnd Position"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBL1->Add( m_checkBoxEBLFixedEndPosition, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	m_checkBoxShowVRM = new wxCheckBox( m_scrolledWindowProperties, wxID_ANY, _("Show VRM"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBL1->Add( m_checkBoxShowVRM, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_checkBoxShowEBLInfo = new wxCheckBox( m_scrolledWindowProperties, wxID_ANY, _("Always Show\nEBL Info"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBL1->Add( m_checkBoxShowEBLInfo, 0, wxALL, 5 );

	m_checkBoxShowPerpLine = new wxCheckBox( m_scrolledWindowProperties, wxID_ANY, _("Show Perpendicular\nIndex Line"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerEBL1->Add( m_checkBoxShowPerpLine, 0, wxALL, 5 );


	m_fgSizerProperties->Add( m_fgSizerEBL1, 1, wxEXPAND, 5 );

	m_fgSizerGZ = new wxFlexGridSizer( 0, 1, 0, 0 );
	m_fgSizerGZ->SetFlexibleDirection( wxBOTH );
	m_fgSizerGZ->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxRotateGZWithBoat = new wxCheckBox( m_scrolledWindowProperties, wxID_ANY, _("Rotate with\nBoat"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizer10->Add( m_checkBoxRotateGZWithBoat, 0, wxALL, 5 );

	wxString m_radioBoxMaintainGZWithChoices[] = { _("Heading"), _("Course over Ground") };
	int m_radioBoxMaintainGZWithNChoices = sizeof( m_radioBoxMaintainGZWithChoices ) / sizeof( wxString );
	m_radioBoxMaintainGZWith = new wxRadioBox( m_scrolledWindowProperties, wxID_ANY, _("Maintain with"), wxDefaultPosition, wxDefaultSize, m_radioBoxMaintainGZWithNChoices, m_radioBoxMaintainGZWithChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxMaintainGZWith->SetSelection( 0 );
	m_radioBoxMaintainGZWith->Enable( false );

	fgSizer10->Add( m_radioBoxMaintainGZWith, 0, wxALL, 5 );

	wxString m_radioBoxGZLineTypeChoices[] = { _("Arc"), _("Straight") };
	int m_radioBoxGZLineTypeNChoices = sizeof( m_radioBoxGZLineTypeChoices ) / sizeof( wxString );
	m_radioBoxGZLineType = new wxRadioBox( m_scrolledWindowProperties, wxID_ANY, _("Line Type"), wxDefaultPosition, wxDefaultSize, m_radioBoxGZLineTypeNChoices, m_radioBoxGZLineTypeChoices, 1, wxRA_SPECIFY_ROWS );
	m_radioBoxGZLineType->SetSelection( 0 );
	fgSizer10->Add( m_radioBoxGZLineType, 0, wxALL, 5 );


	m_fgSizerGZ->Add( fgSizer10, 1, wxEXPAND, 5 );

	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextGZFirstAngle = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("First Angle (-P/+S)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGZFirstAngle->Wrap( -1 );
	fgSizer11->Add( m_staticTextGZFirstAngle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlGZFirstAngle = new wxTextCtrl( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer11->Add( m_textCtrlGZFirstAngle, 0, wxALL, 5 );

	m_staticTextGZFirstLength = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("First distance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGZFirstLength->Wrap( -1 );
	fgSizer11->Add( m_staticTextGZFirstLength, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlGZFirstLength = new wxTextCtrl( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer11->Add( m_textCtrlGZFirstLength, 0, wxALL, 5 );

	m_staticTextGZSecondAngle = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Second Angle (-P/+S)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGZSecondAngle->Wrap( -1 );
	fgSizer11->Add( m_staticTextGZSecondAngle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_textCtrlGZSecondAngle = new wxTextCtrl( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer11->Add( m_textCtrlGZSecondAngle, 0, wxALL, 5 );

	m_staticTextGZSecondLength = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Second distance"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextGZSecondLength->Wrap( -1 );
	fgSizer11->Add( m_staticTextGZSecondLength, 0, wxALL, 5 );

	m_textCtrlGZSecondLength = new wxTextCtrl( m_scrolledWindowProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer11->Add( m_textCtrlGZSecondLength, 0, wxALL, 5 );


	m_fgSizerGZ->Add( fgSizer11, 1, wxEXPAND, 5 );


	m_fgSizerProperties->Add( m_fgSizerGZ, 0, wxEXPAND, 5 );

	m_fgSizerPath = new wxFlexGridSizer( 0, 2, 0, 0 );
	m_fgSizerPath->SetFlexibleDirection( wxBOTH );
	m_fgSizerPath->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_checkBoxPathShowArrow = new wxCheckBox( m_scrolledWindowProperties, wxID_ANY, _("Show Path Direction Arrow"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_fgSizerPath->Add( m_checkBoxPathShowArrow, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );

	wxString m_radioBoxPathPersistenceChoices[] = { _("Persistent"), _("Persistent over Crash"), _("Never") };
	int m_radioBoxPathPersistenceNChoices = sizeof( m_radioBoxPathPersistenceChoices ) / sizeof( wxString );
	m_radioBoxPathPersistence = new wxRadioBox( m_scrolledWindowProperties, wxID_ANY, _("Path Persistence"), wxDefaultPosition, wxDefaultSize, m_radioBoxPathPersistenceNChoices, m_radioBoxPathPersistenceChoices, 3, wxRA_SPECIFY_COLS );
	m_radioBoxPathPersistence->SetSelection( 2 );
	m_fgSizerPath->Add( m_radioBoxPathPersistence, 0, wxALL, 5 );


	m_fgSizerProperties->Add( m_fgSizerPath, 1, wxEXPAND, 5 );

	m_fgSizerPathPoints = new wxFlexGridSizer( 2, 1, 0, 0 );
	m_fgSizerPathPoints->SetFlexibleDirection( wxVERTICAL );
	m_fgSizerPathPoints->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticTextODPoints = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Points"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextODPoints->Wrap( -1 );
	m_fgSizerPathPoints->Add( m_staticTextODPoints, 0, wxALL, 5 );

	m_bSizerListCtrl = new wxBoxSizer( wxVERTICAL );

	m_listCtrlODPoints = new wxListCtrl( m_scrolledWindowProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
	m_bSizerListCtrl->Add( m_listCtrlODPoints, 1, wxALL|wxEXPAND, 5 );


	m_fgSizerPathPoints->Add( m_bSizerListCtrl, 1, wxEXPAND, 5 );


	m_fgSizerProperties->Add( m_fgSizerPathPoints, 1, wxEXPAND, 5 );

	m_bSizerPILLines = new wxBoxSizer( wxVERTICAL );

	m_staticTextPILLines = new wxStaticText( m_scrolledWindowProperties, wxID_ANY, _("Index Lines (+ Stbd/- Port)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPILLines->Wrap( -1 );
	m_bSizerPILLines->Add( m_staticTextPILLines, 0, wxALL, 5 );

	m_listCtrlPILList = new wxListCtrl( m_scrolledWindowProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_EDIT_LABELS|wxLC_HRULES|wxLC_REPORT|wxLC_VRULES );
	m_bSizerPILLines->Add( m_listCtrlPILList, 0, wxALL|wxEXPAND, 5 );


	m_fgSizerProperties->Add( m_bSizerPILLines, 0, wxEXPAND, 5 );


	m_scrolledWindowProperties->SetSizer( m_fgSizerProperties );
	m_scrolledWindowProperties->Layout();
	m_fgSizerProperties->Fit( m_scrolledWindowProperties );
	m_bSizerDialogBox->Add( m_scrolledWindowProperties, 1, wxEXPAND | wxALL, 5 );

	m_SizerOKCancel = new wxBoxSizer( wxHORIZONTAL );

	m_buttonOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );

	m_buttonOK->SetDefault();
	m_SizerOKCancel->Add( m_buttonOK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_buttonCancel = new wxButton( this, wxID_ANY, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SizerOKCancel->Add( m_buttonCancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	m_bSizerDialogBox->Add( m_SizerOKCancel, 0, wxEXPAND, 5 );


	this->SetSizer( m_bSizerDialogBox );
	this->Layout();
	m_bSizerDialogBox->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODPathPropertiesDialogDef::OnClose ) );
	m_textCtrlTotalLength->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlTotalLength->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_colourPickerLineColour->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ODPathPropertiesDialogDef::OnColourChangedLineColour ), NULL, this );
	m_choiceLineStyle->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnChoiceLineStyle ), NULL, this );
	m_choiceLineStyle->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocusChoiceLineStyle ), NULL, this );
	m_choiceLineStyle->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocusChoiceLineStyle ), NULL, this );
	m_choiceLineWidth->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnChoiceLineWidth ), NULL, this );
	m_choiceLineWidth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocusChoiceLineWidth ), NULL, this );
	m_choiceLineWidth->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocusChoiceLineWidth ), NULL, this );
	m_radioBoxBoundaryType->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnRadioBoxBoundaryType ), NULL, this );
	m_checkBoxRotateWithBoat->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnRotateWithBoat ), NULL, this );
	m_textCtrlEBLAngle->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlEBLAngle->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_checkBoxEBLFixedEndPosition->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnFixedEndPosition ), NULL, this );
	m_checkBoxShowPerpLine->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnPILCheckbox ), NULL, this );
	m_checkBoxRotateGZWithBoat->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnRotateWithBoat ), NULL, this );
	m_textCtrlGZFirstAngle->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlGZFirstAngle->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_textCtrlGZFirstLength->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlGZFirstLength->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_textCtrlGZSecondAngle->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlGZSecondAngle->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_textCtrlGZSecondLength->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlGZSecondLength->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_listCtrlODPoints->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( ODPathPropertiesDialogDef::OnLeftDoubleClick ), NULL, this );
	m_listCtrlODPoints->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPathPropertiesDialogDef::OnRightClick ), NULL, this );
	m_listCtrlPILList->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( ODPathPropertiesDialogDef::OnLeftDoubleClickPIL ), NULL, this );
	m_listCtrlPILList->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPathPropertiesDialogDef::OnRightClickPIL ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnOK ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnCancel ), NULL, this );
}

ODPathPropertiesDialogDef::~ODPathPropertiesDialogDef()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ODPathPropertiesDialogDef::OnClose ) );
	m_textCtrlTotalLength->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlTotalLength->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_colourPickerLineColour->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ODPathPropertiesDialogDef::OnColourChangedLineColour ), NULL, this );
	m_choiceLineStyle->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnChoiceLineStyle ), NULL, this );
	m_choiceLineStyle->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocusChoiceLineStyle ), NULL, this );
	m_choiceLineStyle->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocusChoiceLineStyle ), NULL, this );
	m_choiceLineWidth->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnChoiceLineWidth ), NULL, this );
	m_choiceLineWidth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocusChoiceLineWidth ), NULL, this );
	m_choiceLineWidth->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocusChoiceLineWidth ), NULL, this );
	m_radioBoxBoundaryType->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnRadioBoxBoundaryType ), NULL, this );
	m_checkBoxRotateWithBoat->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnRotateWithBoat ), NULL, this );
	m_textCtrlEBLAngle->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlEBLAngle->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_checkBoxEBLFixedEndPosition->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnFixedEndPosition ), NULL, this );
	m_checkBoxShowPerpLine->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnPILCheckbox ), NULL, this );
	m_checkBoxRotateGZWithBoat->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnRotateWithBoat ), NULL, this );
	m_textCtrlGZFirstAngle->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlGZFirstAngle->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_textCtrlGZFirstLength->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlGZFirstLength->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_textCtrlGZSecondAngle->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlGZSecondAngle->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_textCtrlGZSecondLength->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnKillFocus ), NULL, this );
	m_textCtrlGZSecondLength->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( ODPathPropertiesDialogDef::OnSetFocus ), NULL, this );
	m_listCtrlODPoints->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( ODPathPropertiesDialogDef::OnLeftDoubleClick ), NULL, this );
	m_listCtrlODPoints->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPathPropertiesDialogDef::OnRightClick ), NULL, this );
	m_listCtrlPILList->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( ODPathPropertiesDialogDef::OnLeftDoubleClickPIL ), NULL, this );
	m_listCtrlPILList->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( ODPathPropertiesDialogDef::OnRightClickPIL ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnOK ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPathPropertiesDialogDef::OnCancel ), NULL, this );

}
