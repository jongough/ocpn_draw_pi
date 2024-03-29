///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "PathAndPointManagerDialogDef.h"

///////////////////////////////////////////////////////////////////////////

PathAndPointManagerDialogDef::PathAndPointManagerDialogDef()
{
}

PathAndPointManagerDialogDef::PathAndPointManagerDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style )
{
	this->Create( parent, id, title, pos, size, style );
}

bool PathAndPointManagerDialogDef::Create( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style )
{
	if ( !wxDialog::Create( parent, id, title, pos, size, style ) )
	{
		return false;
	}

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_bSizerDialog = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );

	m_notebookPathAndPointManager = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP );
	m_panelPath = new wxPanel( m_notebookPathAndPointManager, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer202;
	bSizer202 = new wxBoxSizer( wxVERTICAL );

	m_listCtrlPath = new wxListCtrl( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SORT_ASCENDING|wxLC_VRULES|wxBORDER_SUNKEN );
	bSizer202->Add( m_listCtrlPath, 1, wxALL|wxEXPAND, 5 );


	bSizer17->Add( bSizer202, 1, wxEXPAND, 5 );

	m_bSizerPathButtons = new wxBoxSizer( wxVERTICAL );

	m_buttonPathProperties = new wxButton( m_panelPath, wxID_ANY, _("&Properties"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_bSizerPathButtons->Add( m_buttonPathProperties, 1, wxALL|wxEXPAND, 5 );

	m_buttonPathActivate = new wxButton( m_panelPath, wxID_ANY, _("&Activate"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_bSizerPathButtons->Add( m_buttonPathActivate, 1, wxALL|wxEXPAND, 5 );

	m_buttonPathCenterView = new wxButton( m_panelPath, wxID_ANY, _("&Center View"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_bSizerPathButtons->Add( m_buttonPathCenterView, 1, wxALL|wxEXPAND, 5 );

	m_buttonPathDelete = new wxButton( m_panelPath, wxID_ANY, _("&Delete"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_bSizerPathButtons->Add( m_buttonPathDelete, 1, wxALL|wxEXPAND, 5 );

	m_buttonPathExportSelected = new wxButton( m_panelPath, wxID_ANY, _("&Export Selected..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_bSizerPathButtons->Add( m_buttonPathExportSelected, 1, wxALL|wxEXPAND, 5 );

	m_buttonPathDeleteAll = new wxButton( m_panelPath, wxID_ANY, _("Dele&te All"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	m_bSizerPathButtons->Add( m_buttonPathDeleteAll, 1, wxALL|wxEXPAND, 5 );


	bSizer17->Add( m_bSizerPathButtons, 0, wxEXPAND, 5 );


	bSizer2->Add( bSizer17, 1, wxEXPAND, 5 );


	m_panelPath->SetSizer( bSizer2 );
	m_panelPath->Layout();
	bSizer2->Fit( m_panelPath );
	m_notebookPathAndPointManager->AddPage( m_panelPath, _("Paths"), true );
	m_panelODPoint = new wxPanel( m_notebookPathAndPointManager, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );

	m_listCtrlODPoints = new wxListCtrl( m_panelODPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SORT_ASCENDING|wxLC_VRULES );
	bSizer21->Add( m_listCtrlODPoints, 1, wxALL|wxEXPAND, 5 );


	bSizer18->Add( bSizer21, 1, wxEXPAND, 5 );

	m_bSizerODPointsButtons = new wxBoxSizer( wxVERTICAL );

	m_buttonODPointNew = new wxButton( m_panelODPoint, wxID_ANY, _("&New"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerODPointsButtons->Add( m_buttonODPointNew, 1, wxALL|wxEXPAND, 5 );

	m_buttonODPointProperties = new wxButton( m_panelODPoint, wxID_ANY, _("&Properties"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerODPointsButtons->Add( m_buttonODPointProperties, 1, wxALL|wxEXPAND, 5 );

	m_buttonODPointCenterView = new wxButton( m_panelODPoint, wxID_ANY, _("&Center View"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerODPointsButtons->Add( m_buttonODPointCenterView, 1, wxALL|wxEXPAND, 5 );

	m_buttonODPointDelete = new wxButton( m_panelODPoint, wxID_ANY, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerODPointsButtons->Add( m_buttonODPointDelete, 1, wxALL|wxEXPAND, 5 );

	m_buttonODPointExportSelected = new wxButton( m_panelODPoint, wxID_ANY, _("&Export Selected..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerODPointsButtons->Add( m_buttonODPointExportSelected, 1, wxALL|wxEXPAND, 5 );

	m_buttonODPointDeleteAll = new wxButton( m_panelODPoint, wxID_ANY, _("Dele&te All"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerODPointsButtons->Add( m_buttonODPointDeleteAll, 1, wxALL|wxEXPAND, 5 );


	bSizer18->Add( m_bSizerODPointsButtons, 0, wxEXPAND, 5 );


	bSizer3->Add( bSizer18, 1, wxEXPAND, 5 );


	m_panelODPoint->SetSizer( bSizer3 );
	m_panelODPoint->Layout();
	bSizer3->Fit( m_panelODPoint );
	m_notebookPathAndPointManager->AddPage( m_panelODPoint, _("OD Points"), false );
	m_panelLayer = new wxPanel( m_notebookPathAndPointManager, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer191;
	bSizer191 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );

	m_listCtrlLayers = new wxListCtrl( m_panelLayer, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT|wxLC_SORT_ASCENDING|wxLC_VRULES );
	bSizer22->Add( m_listCtrlLayers, 1, wxALL|wxEXPAND, 5 );


	bSizer191->Add( bSizer22, 1, wxEXPAND, 5 );

	m_bSizerLayerButtons = new wxBoxSizer( wxVERTICAL );

	m_buttonLayerTemporary = new wxButton( m_panelLayer, wxID_ANY, _("New &temporary Layer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerTemporary, 1, wxALL|wxEXPAND, 5 );

	m_buttonLayerPersistent = new wxButton( m_panelLayer, wxID_ANY, _("New persistent &Layer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerPersistent, 1, wxALL|wxEXPAND, 5 );

	m_buttonLayerDelete = new wxButton( m_panelLayer, wxID_ANY, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerDelete, 1, wxALL|wxEXPAND, 5 );

	m_buttonLayerShowOnChart = new wxButton( m_panelLayer, wxID_ANY, _("Show on &chart"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerShowOnChart, 1, wxALL|wxEXPAND, 5 );

	m_buttonLayerShowPointNames = new wxButton( m_panelLayer, wxID_ANY, _("Show &Point names"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerShowPointNames, 1, wxALL|wxEXPAND, 5 );

	m_buttonLayerListContents = new wxButton( m_panelLayer, wxID_ANY, _("&List contents"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerListContents, 1, wxALL|wxEXPAND, 5 );


	bSizer191->Add( m_bSizerLayerButtons, 0, wxEXPAND, 5 );


	bSizer4->Add( bSizer191, 1, wxEXPAND, 5 );


	m_panelLayer->SetSizer( bSizer4 );
	m_panelLayer->Layout();
	bSizer4->Fit( m_panelLayer );
	m_notebookPathAndPointManager->AddPage( m_panelLayer, _("Layers"), false );

	bSizer16->Add( m_notebookPathAndPointManager, 1, wxALL|wxEXPAND, 5 );


	bSizer19->Add( bSizer16, 1, wxEXPAND, 5 );


	m_bSizerDialog->Add( bSizer19, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );

	m_buttonImport = new wxButton( this, wxID_ANY, _("Import gpx"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonImport, 0, wxALL, 5 );

	m_buttonExportAllVisible = new wxButton( this, wxID_ANY, _("E&xport All Visible"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer20->Add( m_buttonExportAllVisible, 0, wxALL, 5 );


	bSizer8->Add( bSizer20, 1, wxEXPAND, 0 );

	m_buttonOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_buttonOK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	m_bSizerDialog->Add( bSizer8, 0, wxEXPAND, 5 );


	this->SetSizer( m_bSizerDialog );
	this->Layout();
	m_bSizerDialog->Fit( this );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PathAndPointManagerDialogDef::OnClose ) );
	m_notebookPathAndPointManager->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( PathAndPointManagerDialogDef::OnTabSwitch ), NULL, this );
	m_listCtrlPath->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PathAndPointManagerDialogDef::OnPathToggleVisibility ), NULL, this );
	m_listCtrlPath->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnPathColumnClicked ), NULL, this );
	m_listCtrlPath->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( PathAndPointManagerDialogDef::OnPathDefaultAction ), NULL, this );
	m_listCtrlPath->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnPathDeSelected ), NULL, this );
	m_listCtrlPath->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnPathRightClick ), NULL, this );
	m_listCtrlPath->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnPathSelected ), NULL, this );
	m_buttonPathProperties->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathPropertiesClick ), NULL, this );
	m_buttonPathActivate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathActivateClick ), NULL, this );
	m_buttonPathActivate->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PathAndPointManagerDialogDef::OnPathButtonLeftDown ), NULL, this );
	m_buttonPathCenterView->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathCenterViewClick ), NULL, this );
	m_buttonPathCenterView->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PathAndPointManagerDialogDef::OnPathButtonLeftDown ), NULL, this );
	m_buttonPathDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathDeleteClick ), NULL, this );
	m_buttonPathExportSelected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathExportSelectedClick ), NULL, this );
	m_buttonPathDeleteAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathDeleteAllClick ), NULL, this );
	m_listCtrlODPoints->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PathAndPointManagerDialogDef::OnODPointToggleVisibility ), NULL, this );
	m_listCtrlODPoints->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnODPointColumnClick ), NULL, this );
	m_listCtrlODPoints->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( PathAndPointManagerDialogDef::OnODPointDefaultAction ), NULL, this );
	m_listCtrlODPoints->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnODPointDeSelected ), NULL, this );
	m_listCtrlODPoints->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnODPointRightClick ), NULL, this );
	m_listCtrlODPoints->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnODPointSelected ), NULL, this );
	m_buttonODPointNew->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointNewClick ), NULL, this );
	m_buttonODPointProperties->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointPropertiesClick ), NULL, this );
	m_buttonODPointCenterView->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointCenterViewClick ), NULL, this );
	m_buttonODPointDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointDeleteClick ), NULL, this );
	m_buttonODPointExportSelected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointExportSecectedClick ), NULL, this );
	m_buttonODPointDeleteAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointDeleteAllClick ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PathAndPointManagerDialogDef::OnLayerToggleVisibility ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerColumnClick ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerDefaultAction ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerSelected ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerRightClick ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerSelected ), NULL, this );
	m_buttonLayerTemporary->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerTemporaryClick ), NULL, this );
	m_buttonLayerPersistent->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerPersistentClick ), NULL, this );
	m_buttonLayerDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerDeleteClick ), NULL, this );
	m_buttonLayerShowOnChart->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerShowOnChartClick ), NULL, this );
	m_buttonLayerShowPointNames->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerShowPointNamesClick ), NULL, this );
	m_buttonLayerListContents->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerListContentsClick ), NULL, this );
	m_buttonImport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnImportClick ), NULL, this );
	m_buttonExportAllVisible->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnExportAllVisibleClick ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnOKClick ), NULL, this );

	return true;
}

PathAndPointManagerDialogDef::~PathAndPointManagerDialogDef()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PathAndPointManagerDialogDef::OnClose ) );
	m_notebookPathAndPointManager->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( PathAndPointManagerDialogDef::OnTabSwitch ), NULL, this );
	m_listCtrlPath->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PathAndPointManagerDialogDef::OnPathToggleVisibility ), NULL, this );
	m_listCtrlPath->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnPathColumnClicked ), NULL, this );
	m_listCtrlPath->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( PathAndPointManagerDialogDef::OnPathDefaultAction ), NULL, this );
	m_listCtrlPath->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnPathDeSelected ), NULL, this );
	m_listCtrlPath->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnPathRightClick ), NULL, this );
	m_listCtrlPath->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnPathSelected ), NULL, this );
	m_buttonPathProperties->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathPropertiesClick ), NULL, this );
	m_buttonPathActivate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathActivateClick ), NULL, this );
	m_buttonPathActivate->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PathAndPointManagerDialogDef::OnPathButtonLeftDown ), NULL, this );
	m_buttonPathCenterView->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathCenterViewClick ), NULL, this );
	m_buttonPathCenterView->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PathAndPointManagerDialogDef::OnPathButtonLeftDown ), NULL, this );
	m_buttonPathDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathDeleteClick ), NULL, this );
	m_buttonPathExportSelected->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathExportSelectedClick ), NULL, this );
	m_buttonPathDeleteAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathDeleteAllClick ), NULL, this );
	m_listCtrlODPoints->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PathAndPointManagerDialogDef::OnODPointToggleVisibility ), NULL, this );
	m_listCtrlODPoints->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnODPointColumnClick ), NULL, this );
	m_listCtrlODPoints->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( PathAndPointManagerDialogDef::OnODPointDefaultAction ), NULL, this );
	m_listCtrlODPoints->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnODPointDeSelected ), NULL, this );
	m_listCtrlODPoints->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnODPointRightClick ), NULL, this );
	m_listCtrlODPoints->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnODPointSelected ), NULL, this );
	m_buttonODPointNew->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointNewClick ), NULL, this );
	m_buttonODPointProperties->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointPropertiesClick ), NULL, this );
	m_buttonODPointCenterView->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointCenterViewClick ), NULL, this );
	m_buttonODPointDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointDeleteClick ), NULL, this );
	m_buttonODPointExportSelected->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointExportSecectedClick ), NULL, this );
	m_buttonODPointDeleteAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnODPointDeleteAllClick ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( PathAndPointManagerDialogDef::OnLayerToggleVisibility ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerColumnClick ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerDefaultAction ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerSelected ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerRightClick ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerSelected ), NULL, this );
	m_buttonLayerTemporary->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerTemporaryClick ), NULL, this );
	m_buttonLayerPersistent->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerPersistentClick ), NULL, this );
	m_buttonLayerDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerDeleteClick ), NULL, this );
	m_buttonLayerShowOnChart->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerShowOnChartClick ), NULL, this );
	m_buttonLayerShowPointNames->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerShowPointNamesClick ), NULL, this );
	m_buttonLayerListContents->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerListContentsClick ), NULL, this );
	m_buttonImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnImportClick ), NULL, this );
	m_buttonExportAllVisible->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnExportAllVisibleClick ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnOKClick ), NULL, this );

}
