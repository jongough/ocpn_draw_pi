///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
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
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_checkBoxConfirmObjectDelete = new wxCheckBox( m_panelGeneral, wxID_ANY, wxT("Confirm Object Delete"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxConfirmObjectDelete->SetValue(true); 
	fgSizer4->Add( m_checkBoxConfirmObjectDelete, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizerFiller1;
	bSizerFiller1 = new wxBoxSizer( wxVERTICAL );
	
	
	fgSizer4->Add( bSizerFiller1, 1, wxEXPAND, 5 );
	
	m_staticTextNavObjBackups = new wxStaticText( m_panelGeneral, wxID_ANY, wxT("Nav Object Backups"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextNavObjBackups->Wrap( -1 );
	fgSizer4->Add( m_staticTextNavObjBackups, 0, wxALL, 5 );
	
	m_spinCtrlNavObjBackups = new wxSpinCtrl( m_panelGeneral, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 5 );
	fgSizer4->Add( m_spinCtrlNavObjBackups, 0, wxALL, 5 );
	
	m_staticTextEdgePan = new wxStaticText( m_panelGeneral, wxID_ANY, wxT("Edge Panning Sensitivity\n(percentage screen width) Default 5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEdgePan->Wrap( -1 );
	fgSizer4->Add( m_staticTextEdgePan, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sliderEdgePan = new wxSlider( m_panelGeneral, wxID_ANY, 5, 0, 10, wxDefaultPosition, wxSize( -1,-1 ), wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer4->Add( m_sliderEdgePan, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextInitialEdgePan = new wxStaticText( m_panelGeneral, wxID_ANY, wxT("Initial Edge Panning Sensitivity\n(percentage screen width) Default 3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInitialEdgePan->Wrap( -1 );
	fgSizer4->Add( m_staticTextInitialEdgePan, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sliderInitialEdgePan = new wxSlider( m_panelGeneral, wxID_ANY, 3, 0, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer4->Add( m_sliderInitialEdgePan, 0, wxALL|wxEXPAND, 5 );
	
	m_staticTextToolbar = new wxStaticText( m_panelGeneral, wxID_ANY, wxT("Display Toolbar"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextToolbar->Wrap( -1 );
	fgSizer4->Add( m_staticTextToolbar, 0, wxALL, 5 );
	
	wxString m_choiceToolbarChoices[] = { wxT("Never"), wxT("Whilst Drawing"), wxT("Always") };
	int m_choiceToolbarNChoices = sizeof( m_choiceToolbarChoices ) / sizeof( wxString );
	m_choiceToolbar = new wxChoice( m_panelGeneral, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceToolbarNChoices, m_choiceToolbarChoices, 0 );
	m_choiceToolbar->SetSelection( 1 );
	fgSizer4->Add( m_choiceToolbar, 0, wxALL, 5 );
	
	
	bSizerGeneral->Add( fgSizer4, 1, wxEXPAND, 5 );
	
	
	m_panelGeneral->SetSizer( bSizerGeneral );
	m_panelGeneral->Layout();
	bSizerGeneral->Fit( m_panelGeneral );
	m_notebookProperties->AddPage( m_panelGeneral, wxT("General"), true );
	m_panelBoundaryPoint = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerOCPNPoint;
	bSizerOCPNPoint = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizerArrivalRadius;
	bSizerArrivalRadius = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextArrivalRadius = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, wxT("Arrival Radius"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextArrivalRadius->Wrap( -1 );
	bSizerArrivalRadius->Add( m_staticTextArrivalRadius, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_textCtrlODPointArrivalRadius = new wxTextCtrl( m_panelBoundaryPoint, wxID_ANY, wxT("0.5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlODPointArrivalRadius->SetMaxLength( 0 ); 
	bSizerArrivalRadius->Add( m_textCtrlODPointArrivalRadius, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	bSizerOCPNPoint->Add( bSizerArrivalRadius, 0, wxEXPAND, 5 );
	
	m_SizerNameIcon = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkBoxShowName = new wxCheckBox( m_panelBoundaryPoint, wxID_ANY, wxT("Show Name"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_SizerNameIcon->Add( m_checkBoxShowName, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	
	m_SizerNameIcon->Add( 0, 0, 1, wxALIGN_CENTER, 5 );
	
	m_staticTextIcon = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, wxT("Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextIcon->Wrap( -1 );
	m_SizerNameIcon->Add( m_staticTextIcon, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_bitmapPointBitmap = new wxStaticBitmap( m_panelBoundaryPoint, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	m_SizerNameIcon->Add( m_bitmapPointBitmap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_bcomboBoxODPointIconName = new wxBitmapComboBox( m_panelBoundaryPoint, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	m_SizerNameIcon->Add( m_bcomboBoxODPointIconName, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	
	bSizerOCPNPoint->Add( m_SizerNameIcon, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizerRingsShow;
	fgSizerRingsShow = new wxFlexGridSizer( 5, 2, 0, 0 );
	fgSizerRingsShow->AddGrowableCol( 0 );
	fgSizerRingsShow->SetFlexibleDirection( wxBOTH );
	fgSizerRingsShow->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_checkBoxShowODPointRangeRings = new wxCheckBox( m_panelBoundaryPoint, wxID_ANY, wxT("Show Range Rings"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizerRingsShow->Add( m_checkBoxShowODPointRangeRings, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_checkBoxFill = new wxCheckBox( m_panelBoundaryPoint, wxID_ANY, wxT("Fill Range Rings"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_checkBoxFill->SetValue(true); 
	fgSizerRingsShow->Add( m_checkBoxFill, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	
	bSizerOCPNPoint->Add( fgSizerRingsShow, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizerRingsDetail;
	fgSizerRingsDetail = new wxFlexGridSizer( 5, 2, 0, 0 );
	fgSizerRingsDetail->AddGrowableCol( 0 );
	fgSizerRingsDetail->SetFlexibleDirection( wxBOTH );
	fgSizerRingsDetail->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextRangeRingNumber = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, wxT("Number of Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingNumber->Wrap( -1 );
	fgSizerRingsDetail->Add( m_staticTextRangeRingNumber, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceODPointRangeRingNumberChoices[] = { wxT("None"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10") };
	int m_choiceODPointRangeRingNumberNChoices = sizeof( m_choiceODPointRangeRingNumberChoices ) / sizeof( wxString );
	m_choiceODPointRangeRingNumber = new wxChoice( m_panelBoundaryPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceODPointRangeRingNumberNChoices, m_choiceODPointRangeRingNumberChoices, 0 );
	m_choiceODPointRangeRingNumber->SetSelection( 0 );
	fgSizerRingsDetail->Add( m_choiceODPointRangeRingNumber, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextRangeRingSteps = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, wxT("Distance Between Range Rings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingSteps->Wrap( -1 );
	fgSizerRingsDetail->Add( m_staticTextRangeRingSteps, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrlODPointRangeRingSteps = new wxTextCtrl( m_panelBoundaryPoint, wxID_ANY, wxT("0.5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_textCtrlODPointRangeRingSteps->SetMaxLength( 0 ); 
	fgSizerRingsDetail->Add( m_textCtrlODPointRangeRingSteps, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextDistanceUnit = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, wxT("Distance Unit"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDistanceUnit->Wrap( -1 );
	fgSizerRingsDetail->Add( m_staticTextDistanceUnit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceODPointDistanceUnitChoices[] = { wxT("Nautical Miles"), wxT("Kilometers") };
	int m_choiceODPointDistanceUnitNChoices = sizeof( m_choiceODPointDistanceUnitChoices ) / sizeof( wxString );
	m_choiceODPointDistanceUnit = new wxChoice( m_panelBoundaryPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceODPointDistanceUnitNChoices, m_choiceODPointDistanceUnitChoices, 0 );
	m_choiceODPointDistanceUnit->SetSelection( 0 );
	fgSizerRingsDetail->Add( m_choiceODPointDistanceUnit, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextRangeRingColours = new wxStaticText( m_panelBoundaryPoint, wxID_ANY, wxT("Range Ring Colours"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextRangeRingColours->Wrap( -1 );
	fgSizerRingsDetail->Add( m_staticTextRangeRingColours, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerODPointRangeRingColours = new wxColourPickerCtrl( m_panelBoundaryPoint, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerRingsDetail->Add( m_colourPickerODPointRangeRingColours, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	
	bSizerOCPNPoint->Add( fgSizerRingsDetail, 0, wxEXPAND, 5 );
	
	
	m_panelBoundaryPoint->SetSizer( bSizerOCPNPoint );
	m_panelBoundaryPoint->Layout();
	bSizerOCPNPoint->Fit( m_panelBoundaryPoint );
	m_notebookProperties->AddPage( m_panelBoundaryPoint, wxT("Boundary Point"), false );
	m_panelTextPoint = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerTextPoint;
	fgSizerTextPoint = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerTextPoint->SetFlexibleDirection( wxBOTH );
	fgSizerTextPoint->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextPosition = new wxStaticText( m_panelTextPoint, wxID_ANY, wxT("Text position relative to point"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPosition->Wrap( -1 );
	fgSizerTextPoint->Add( m_staticTextPosition, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceTextPositionChoices[] = { wxT("Top"), wxT("Bottom"), wxT("Right"), wxT("Left") };
	int m_choiceTextPositionNChoices = sizeof( m_choiceTextPositionChoices ) / sizeof( wxString );
	m_choiceTextPosition = new wxChoice( m_panelTextPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceTextPositionNChoices, m_choiceTextPositionChoices, 0 );
	m_choiceTextPosition->SetSelection( 1 );
	fgSizerTextPoint->Add( m_choiceTextPosition, 0, wxALL, 5 );
	
	m_staticTextColour = new wxStaticText( m_panelTextPoint, wxID_ANY, wxT("Text Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextColour->Wrap( -1 );
	fgSizerTextPoint->Add( m_staticTextColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerTextColour = new wxColourPickerCtrl( m_panelTextPoint, wxID_ANY, wxColour( 0, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerTextPoint->Add( m_colourPickerTextColour, 0, wxALL, 5 );
	
	m_staticTextBackgroundColour = new wxStaticText( m_panelTextPoint, wxID_ANY, wxT("Background Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBackgroundColour->Wrap( -1 );
	fgSizerTextPoint->Add( m_staticTextBackgroundColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerBackgroundColour = new wxColourPickerCtrl( m_panelTextPoint, wxID_ANY, wxColour( 255, 255, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerTextPoint->Add( m_colourPickerBackgroundColour, 0, wxALL, 5 );
	
	m_staticTextBackgroundTransparency = new wxStaticText( m_panelTextPoint, wxID_ANY, wxT("Background Density"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBackgroundTransparency->Wrap( -1 );
	fgSizerTextPoint->Add( m_staticTextBackgroundTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sliderBackgroundTransparency = new wxSlider( m_panelTextPoint, wxID_ANY, 175, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizerTextPoint->Add( m_sliderBackgroundTransparency, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizerTextFont;
	bSizerTextFont = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticTextTextFont = new wxStaticText( m_panelTextPoint, wxID_ANY, wxT("Text Font"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextTextFont->Wrap( -1 );
	bSizerTextFont->Add( m_staticTextTextFont, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticTextFontFaceExample = new wxStaticText( m_panelTextPoint, wxID_ANY, wxT("Example"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFontFaceExample->Wrap( -1 );
	bSizerTextFont->Add( m_staticTextFontFaceExample, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	fgSizerTextPoint->Add( bSizerTextFont, 1, wxEXPAND, 5 );
	
	m_buttonTextFont = new wxButton( m_panelTextPoint, wxID_ANY, wxT("Fonts"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerTextPoint->Add( m_buttonTextFont, 0, wxALL, 5 );
	
	
	m_panelTextPoint->SetSizer( fgSizerTextPoint );
	m_panelTextPoint->Layout();
	fgSizerTextPoint->Fit( m_panelTextPoint );
	m_notebookProperties->AddPage( m_panelTextPoint, wxT("Text Point"), false );
	m_panelPath = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerPathSettings;
	fgSizerPathSettings = new wxFlexGridSizer( 6, 2, 0, 0 );
	fgSizerPathSettings->SetFlexibleDirection( wxBOTH );
	fgSizerPathSettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextActivePathLineColour = new wxStaticText( m_panelPath, wxID_ANY, wxT("Active Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActivePathLineColour->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextActivePathLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerActivePathLineColour = new wxColourPickerCtrl( m_panelPath, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerPathSettings->Add( m_colourPickerActivePathLineColour, 0, wxALL, 5 );
	
	m_staticTextInactivePathLineColour = new wxStaticText( m_panelPath, wxID_ANY, wxT("Inactive Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactivePathLineColour->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextInactivePathLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerInActivePathLineColour = new wxColourPickerCtrl( m_panelPath, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerPathSettings->Add( m_colourPickerInActivePathLineColour, 0, wxALL, 5 );
	
	m_staticTextPathLineWidth = new wxStaticText( m_panelPath, wxID_ANY, wxT("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPathLineWidth->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextPathLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choicePathLineWidthChoices[] = { wxT("1 Pixel"), wxT("2 Pixels"), wxT("3 Pixels"), wxT("4 Pixels"), wxT("5 Pixels"), wxT("6 Pixels"), wxT("7 Pixels"), wxT("8 Pixels"), wxT("9 Pixels"), wxT("10 Pixels") };
	int m_choicePathLineWidthNChoices = sizeof( m_choicePathLineWidthChoices ) / sizeof( wxString );
	m_choicePathLineWidth = new wxChoice( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePathLineWidthNChoices, m_choicePathLineWidthChoices, 0 );
	m_choicePathLineWidth->SetSelection( 0 );
	fgSizerPathSettings->Add( m_choicePathLineWidth, 0, wxALL, 5 );
	
	m_staticTextPathLineStyle = new wxStaticText( m_panelPath, wxID_ANY, wxT("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPathLineStyle->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextPathLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choicePathLineStyleChoices[] = { wxT("Solid"), wxT("Dot"), wxT("Long Dash"), wxT("Short Dash"), wxT("Dot Dash") };
	int m_choicePathLineStyleNChoices = sizeof( m_choicePathLineStyleChoices ) / sizeof( wxString );
	m_choicePathLineStyle = new wxChoice( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choicePathLineStyleNChoices, m_choicePathLineStyleChoices, 0 );
	m_choicePathLineStyle->SetSelection( 1 );
	fgSizerPathSettings->Add( m_choicePathLineStyle, 0, wxALL, 5 );
	
	m_staticTextActivePathFillColour = new wxStaticText( m_panelPath, wxID_ANY, wxT("Active Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActivePathFillColour->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextActivePathFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerActivePathFillColour = new wxColourPickerCtrl( m_panelPath, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerPathSettings->Add( m_colourPickerActivePathFillColour, 0, wxALL, 5 );
	
	m_staticTextInactivePathFillColour = new wxStaticText( m_panelPath, wxID_ANY, wxT("Inactive Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactivePathFillColour->Wrap( -1 );
	fgSizerPathSettings->Add( m_staticTextInactivePathFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerInActivePathFillColour = new wxColourPickerCtrl( m_panelPath, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerPathSettings->Add( m_colourPickerInActivePathFillColour, 0, wxALL, 5 );
	
	
	m_panelPath->SetSizer( fgSizerPathSettings );
	m_panelPath->Layout();
	fgSizerPathSettings->Fit( m_panelPath );
	m_notebookProperties->AddPage( m_panelPath, wxT("Path"), false );
	m_panelBoundary = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerBoundarySettings;
	fgSizerBoundarySettings = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerBoundarySettings->SetFlexibleDirection( wxBOTH );
	fgSizerBoundarySettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextActiveBoundayLineColour = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Active Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveBoundayLineColour->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextActiveBoundayLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerActiveBoundaryLineColour = new wxColourPickerCtrl( m_panelBoundary, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerBoundarySettings->Add( m_colourPickerActiveBoundaryLineColour, 0, wxALL, 5 );
	
	m_staticTextInactiveBoundaryLineColour = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Inactive Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveBoundaryLineColour->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextInactiveBoundaryLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerInActiveBoundaryLineColour = new wxColourPickerCtrl( m_panelBoundary, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerBoundarySettings->Add( m_colourPickerInActiveBoundaryLineColour, 0, wxALL, 5 );
	
	m_staticTextBoundaryLineWidth = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryLineWidth->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextBoundaryLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceBoundaryLineWidthChoices[] = { wxT("1 Pixel"), wxT("2 Pixels"), wxT("3 Pixels"), wxT("4 Pixels"), wxT("5 Pixels"), wxT("6 Pixels"), wxT("7 Pixels"), wxT("8 Pixels"), wxT("9 Pixels"), wxT("10 Pixels") };
	int m_choiceBoundaryLineWidthNChoices = sizeof( m_choiceBoundaryLineWidthChoices ) / sizeof( wxString );
	m_choiceBoundaryLineWidth = new wxChoice( m_panelBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceBoundaryLineWidthNChoices, m_choiceBoundaryLineWidthChoices, 0 );
	m_choiceBoundaryLineWidth->SetSelection( 0 );
	fgSizerBoundarySettings->Add( m_choiceBoundaryLineWidth, 0, wxALL, 5 );
	
	m_staticTextBoundaryLineStyle = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextBoundaryLineStyle->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextBoundaryLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceBoundaryLineStyleChoices[] = { wxT("Solid"), wxT("Dot"), wxT("Long Dash"), wxT("Short Dash"), wxT("Dot Dash") };
	int m_choiceBoundaryLineStyleNChoices = sizeof( m_choiceBoundaryLineStyleChoices ) / sizeof( wxString );
	m_choiceBoundaryLineStyle = new wxChoice( m_panelBoundary, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceBoundaryLineStyleNChoices, m_choiceBoundaryLineStyleChoices, 0 );
	m_choiceBoundaryLineStyle->SetSelection( 0 );
	fgSizerBoundarySettings->Add( m_choiceBoundaryLineStyle, 0, wxALL, 5 );
	
	m_staticTextActiveBoundaryFillColour = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Active Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveBoundaryFillColour->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextActiveBoundaryFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerActiveBoundaryFillColour = new wxColourPickerCtrl( m_panelBoundary, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerBoundarySettings->Add( m_colourPickerActiveBoundaryFillColour, 0, wxALL, 5 );
	
	m_staticTextInactiveBoundaryFillColour = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Inactive Fill Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveBoundaryFillColour->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextInactiveBoundaryFillColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerInActiveBoundaryFillColour = new wxColourPickerCtrl( m_panelBoundary, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerBoundarySettings->Add( m_colourPickerInActiveBoundaryFillColour, 0, wxALL, 5 );
	
	m_staticTextFillTransparency = new wxStaticText( m_panelBoundary, wxID_ANY, wxT("Fill Density"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextFillTransparency->Wrap( -1 );
	fgSizerBoundarySettings->Add( m_staticTextFillTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sliderFillTransparency = new wxSlider( m_panelBoundary, wxID_ANY, 175, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizerBoundarySettings->Add( m_sliderFillTransparency, 0, wxALL|wxEXPAND, 5 );
	
	
	m_panelBoundary->SetSizer( fgSizerBoundarySettings );
	m_panelBoundary->Layout();
	fgSizerBoundarySettings->Fit( m_panelBoundary );
	m_notebookProperties->AddPage( m_panelBoundary, wxT("Boundary"), false );
	m_panelEBL = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizerEBLSettings;
	fgSizerEBLSettings = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerEBLSettings->AddGrowableCol( 1 );
	fgSizerEBLSettings->SetFlexibleDirection( wxBOTH );
	fgSizerEBLSettings->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextStartPointIcon = new wxStaticText( m_panelEBL, wxID_ANY, wxT("Start Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextStartPointIcon->Wrap( -1 );
	fgSizerEBLSettings->Add( m_staticTextStartPointIcon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxFlexGridSizer* fgSizerEBLEndPointIcon;
	fgSizerEBLEndPointIcon = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizerEBLEndPointIcon->AddGrowableCol( 1 );
	fgSizerEBLEndPointIcon->SetFlexibleDirection( wxBOTH );
	fgSizerEBLEndPointIcon->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bitmapEBLStartBitmap = new wxStaticBitmap( m_panelEBL, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerEBLEndPointIcon->Add( m_bitmapEBLStartBitmap, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	m_bcomboBoxEBLStartIconName = new wxBitmapComboBox( m_panelEBL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizerEBLEndPointIcon->Add( m_bcomboBoxEBLStartIconName, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	
	fgSizerEBLSettings->Add( fgSizerEBLEndPointIcon, 1, wxALIGN_RIGHT, 5 );
	
	m_staticTextEndPointIcon = new wxStaticText( m_panelEBL, wxID_ANY, wxT("End Point Icon"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEndPointIcon->Wrap( -1 );
	fgSizerEBLSettings->Add( m_staticTextEndPointIcon, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxFlexGridSizer* fgSizerEBLStartPointIcon;
	fgSizerEBLStartPointIcon = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerEBLStartPointIcon->AddGrowableCol( 1 );
	fgSizerEBLStartPointIcon->SetFlexibleDirection( wxBOTH );
	fgSizerEBLStartPointIcon->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bitmapEBLEndBitmap = new wxStaticBitmap( m_panelEBL, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizerEBLStartPointIcon->Add( m_bitmapEBLEndBitmap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_bcomboBoxEBLEndIconName = new wxBitmapComboBox( m_panelEBL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizerEBLStartPointIcon->Add( m_bcomboBoxEBLEndIconName, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	
	fgSizerEBLSettings->Add( fgSizerEBLStartPointIcon, 1, wxALIGN_RIGHT, 5 );
	
	m_staticTextActiveEBLLineColour = new wxStaticText( m_panelEBL, wxID_ANY, wxT("Active EBL Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextActiveEBLLineColour->Wrap( -1 );
	fgSizerEBLSettings->Add( m_staticTextActiveEBLLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_colourPickerEBLLineColour = new wxColourPickerCtrl( m_panelEBL, wxID_ANY, wxColour( 255, 0, 0 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerEBLSettings->Add( m_colourPickerEBLLineColour, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextInactiveEBLLineColour1 = new wxStaticText( m_panelEBL, wxID_ANY, wxT("Inactive EBL Line Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextInactiveEBLLineColour1->Wrap( -1 );
	fgSizerEBLSettings->Add( m_staticTextInactiveEBLLineColour1, 0, wxALL, 5 );
	
	m_colourPickerInActiveEBLLineColour1 = new wxColourPickerCtrl( m_panelEBL, wxID_ANY, wxColour( 214, 214, 214 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizerEBLSettings->Add( m_colourPickerInActiveEBLLineColour1, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextEBLLineWidth = new wxStaticText( m_panelEBL, wxID_ANY, wxT("Line Width"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEBLLineWidth->Wrap( -1 );
	fgSizerEBLSettings->Add( m_staticTextEBLLineWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceEBLLineWidthChoices[] = { wxT("1 Pixel"), wxT("2 Pixels"), wxT("3 Pixels"), wxT("4 Pixels"), wxT("5 Pixels"), wxT("6 Pixels"), wxT("7 Pixels"), wxT("8 Pixels"), wxT("9 Pixels"), wxT("10 Pixels") };
	int m_choiceEBLLineWidthNChoices = sizeof( m_choiceEBLLineWidthChoices ) / sizeof( wxString );
	m_choiceEBLLineWidth = new wxChoice( m_panelEBL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceEBLLineWidthNChoices, m_choiceEBLLineWidthChoices, 0 );
	m_choiceEBLLineWidth->SetSelection( 0 );
	fgSizerEBLSettings->Add( m_choiceEBLLineWidth, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_staticTextEBLLineStyle = new wxStaticText( m_panelEBL, wxID_ANY, wxT("Line Style"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextEBLLineStyle->Wrap( -1 );
	fgSizerEBLSettings->Add( m_staticTextEBLLineStyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_choiceEBLLineStyleChoices[] = { wxT("Solid"), wxT("Dot"), wxT("Long Dash"), wxT("Short Dash"), wxT("Dot Dash") };
	int m_choiceEBLLineStyleNChoices = sizeof( m_choiceEBLLineStyleChoices ) / sizeof( wxString );
	m_choiceEBLLineStyle = new wxChoice( m_panelEBL, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceEBLLineStyleNChoices, m_choiceEBLLineStyleChoices, 0 );
	m_choiceEBLLineStyle->SetSelection( 0 );
	fgSizerEBLSettings->Add( m_choiceEBLLineStyle, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_checkBoxEBLFixedEndPosition = new wxCheckBox( m_panelEBL, wxID_ANY, wxT("EBL Fixed End Position"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	fgSizerEBLSettings->Add( m_checkBoxEBLFixedEndPosition, 0, wxALL, 5 );
	
	
	fgSizerEBLSettings->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxString m_radioBoxEBLPersistenceChoices[] = { wxT("Persistent"), wxT("Persistent over Crash"), wxT("Never") };
	int m_radioBoxEBLPersistenceNChoices = sizeof( m_radioBoxEBLPersistenceChoices ) / sizeof( wxString );
	m_radioBoxEBLPersistence = new wxRadioBox( m_panelEBL, wxID_ANY, wxT("EBL Persistence"), wxDefaultPosition, wxDefaultSize, m_radioBoxEBLPersistenceNChoices, m_radioBoxEBLPersistenceChoices, 2, wxRA_SPECIFY_ROWS );
	m_radioBoxEBLPersistence->SetSelection( 0 );
	fgSizerEBLSettings->Add( m_radioBoxEBLPersistence, 0, wxALL, 5 );
	
	
	m_panelEBL->SetSizer( fgSizerEBLSettings );
	m_panelEBL->Layout();
	fgSizerEBLSettings->Fit( m_panelEBL );
	m_notebookProperties->AddPage( m_panelEBL, wxT("EBL"), false );
	m_panelAbout = new wxPanel( m_notebookProperties, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizerAbout;
	bSizerAbout = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizerAboutLayout;
	fgSizerAboutLayout = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizerAboutLayout->SetFlexibleDirection( wxBOTH );
	fgSizerAboutLayout->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextName = new wxStaticText( m_panelAbout, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextName->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextName, 0, wxALL, 5 );
	
	m_staticTextNameVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextNameVal->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextNameVal, 0, wxALL, 5 );
	
	m_staticTextVersion = new wxStaticText( m_panelAbout, wxID_ANY, wxT("Version:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextVersion->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextVersion, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 6, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticTextMajor = new wxStaticText( m_panelAbout, wxID_ANY, wxT("Major:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMajor->Wrap( -1 );
	fgSizer6->Add( m_staticTextMajor, 0, wxALL, 5 );
	
	m_staticTextMajorVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMajorVal->Wrap( -1 );
	fgSizer6->Add( m_staticTextMajorVal, 0, wxALL, 5 );
	
	m_staticTextMinor = new wxStaticText( m_panelAbout, wxID_ANY, wxT("Minor:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMinor->Wrap( -1 );
	fgSizer6->Add( m_staticTextMinor, 0, wxALL, 5 );
	
	m_staticTextMinorVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextMinorVal->Wrap( -1 );
	fgSizer6->Add( m_staticTextMinorVal, 0, wxALL, 5 );
	
	m_staticTextPatcvh = new wxStaticText( m_panelAbout, wxID_ANY, wxT("Patch:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPatcvh->Wrap( -1 );
	fgSizer6->Add( m_staticTextPatcvh, 0, wxALL, 5 );
	
	m_staticTextPatchVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextPatchVal->Wrap( -1 );
	fgSizer6->Add( m_staticTextPatchVal, 0, wxALL, 5 );
	
	
	fgSizerAboutLayout->Add( fgSizer6, 1, wxEXPAND, 5 );
	
	m_staticTextDate = new wxStaticText( m_panelAbout, wxID_ANY, wxT("Date:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDate->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextDate, 0, wxALL, 5 );
	
	m_staticTextDateVal = new wxStaticText( m_panelAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextDateVal->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextDateVal, 0, wxALL, 5 );
	
	m_staticTextOther = new wxStaticText( m_panelAbout, wxID_ANY, wxT("Other:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTextOther->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticTextOther, 0, wxALL, 5 );
	
	m_staticText34 = new wxStaticText( m_panelAbout, wxID_ANY, wxT("Please report problems using FlySpray at:\nhttp://willkamp.com/opencpn/flyspray/index.php?project=0&do=index&switch=1\n\nor\n\nat the OpenCPN forum:\nhttp://www.cruisersforum.com/forums/f134\n\nor\n\nRaise an issue in GIT at:\nhttps://github.com/jongough/ocpn_draw_pi/issues\n"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	fgSizerAboutLayout->Add( m_staticText34, 0, wxALL, 5 );
	
	
	bSizerAbout->Add( fgSizerAboutLayout, 1, wxEXPAND, 5 );
	
	
	m_panelAbout->SetSizer( bSizerAbout );
	m_panelAbout->Layout();
	bSizerAbout->Fit( m_panelAbout );
	m_notebookProperties->AddPage( m_panelAbout, wxT("About"), false );
	
	m_SizerProperties->Add( m_notebookProperties, 0, wxALL, 5 );
	
	wxBoxSizer* bSizerOKCancelApply;
	bSizerOKCancelApply = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonOK, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonCancel, 0, wxALL, 5 );
	
	m_buttonApply = new wxButton( this, wxID_ANY, wxT("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerOKCancelApply->Add( m_buttonApply, 0, wxALL, 5 );
	
	
	m_SizerProperties->Add( bSizerOKCancelApply, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( m_SizerProperties );
	this->Layout();
	
	// Connect Events
	m_bcomboBoxODPointIconName->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( ODPropertiesDialogDef::OnODPointComboboxSelected ), NULL, this );
	m_buttonTextFont->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnButtonClickFonts ), NULL, this );
	m_bcomboBoxEBLStartIconName->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( ODPropertiesDialogDef::OnEBLStartIconComboboxSelected ), NULL, this );
	m_bcomboBoxEBLEndIconName->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( ODPropertiesDialogDef::OnEBLEndIconComboboxSelected ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesOKClick ), NULL, this );
	m_buttonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesCancelClick ), NULL, this );
	m_buttonApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesApplyClick ), NULL, this );
}

ODPropertiesDialogDef::~ODPropertiesDialogDef()
{
	// Disconnect Events
	m_bcomboBoxODPointIconName->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( ODPropertiesDialogDef::OnODPointComboboxSelected ), NULL, this );
	m_buttonTextFont->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnButtonClickFonts ), NULL, this );
	m_bcomboBoxEBLStartIconName->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( ODPropertiesDialogDef::OnEBLStartIconComboboxSelected ), NULL, this );
	m_bcomboBoxEBLEndIconName->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( ODPropertiesDialogDef::OnEBLEndIconComboboxSelected ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesOKClick ), NULL, this );
	m_buttonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesCancelClick ), NULL, this );
	m_buttonApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ODPropertiesDialogDef::OnDrawPropertiesApplyClick ), NULL, this );
	
}
