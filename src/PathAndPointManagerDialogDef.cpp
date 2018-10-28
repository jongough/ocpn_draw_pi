///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "PathAndPointManagerDialogDef.h"

///////////////////////////////////////////////////////////////////////////

PathAndPointManagerDialogDef::PathAndPointManagerDialogDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_bSizerDialog = new wxBoxSizer( wxVERTICAL );

	m_notebookPathAndPointManager = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelPath = new wxPanel( m_notebookPathAndPointManager, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	m_listCtrlPath = new wxListCtrl( m_panelPath, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_ICON|wxLC_REPORT|wxLC_SORT_ASCENDING|wxLC_VRULES|wxBORDER_SUNKEN );
	bSizer2->Add( m_listCtrlPath, 1, wxALL, 5 );

	m_bSizerPathButtons = new wxBoxSizer( wxVERTICAL );

	m_buttonPathProperties = new wxButton( m_panelPath, wxID_ANY, _("&Properties"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPathButtons->Add( m_buttonPathProperties, 0, wxALL|wxEXPAND, 5 );

	m_buttonPathActivate = new wxButton( m_panelPath, wxID_ANY, _("&Activate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPathButtons->Add( m_buttonPathActivate, 0, wxALL|wxEXPAND, 5 );

	m_buttonPathCenterView = new wxButton( m_panelPath, wxID_ANY, _("&Center View"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPathButtons->Add( m_buttonPathCenterView, 0, wxALL|wxEXPAND, 5 );

	m_buttonPathDelete = new wxButton( m_panelPath, wxID_ANY, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPathButtons->Add( m_buttonPathDelete, 0, wxALL|wxEXPAND, 5 );

	m_buttonPathExportSelected = new wxButton( m_panelPath, wxID_ANY, _("&Export Selected..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPathButtons->Add( m_buttonPathExportSelected, 0, wxALL|wxEXPAND, 5 );


	m_bSizerPathButtons->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonPathDeleteAll = new wxButton( m_panelPath, wxID_ANY, _("Dele&te All"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPathButtons->Add( m_buttonPathDeleteAll, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );


	bSizer2->Add( m_bSizerPathButtons, 0, wxEXPAND, 5 );


	m_panelPath->SetSizer( bSizer2 );
	m_panelPath->Layout();
	bSizer2->Fit( m_panelPath );
	m_notebookPathAndPointManager->AddPage( m_panelPath, _("Paths"), true );
	m_panelPoint = new wxPanel( m_notebookPathAndPointManager, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_listCtrlPoints = new wxListCtrl( m_panelPoint, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_ICON|wxLC_REPORT|wxLC_SORT_ASCENDING|wxLC_VRULES );
	bSizer3->Add( m_listCtrlPoints, 1, wxALL, 5 );

	m_bSizerPointsButtons = new wxBoxSizer( wxVERTICAL );

	m_buttonPointNew = new wxButton( m_panelPoint, wxID_ANY, _("&New"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPointsButtons->Add( m_buttonPointNew, 0, wxALL|wxEXPAND, 5 );

	m_buttonPointProperties = new wxButton( m_panelPoint, wxID_ANY, _("&Properties"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPointsButtons->Add( m_buttonPointProperties, 0, wxALL|wxEXPAND, 5 );

	m_buttonPointActivate = new wxButton( m_panelPoint, wxID_ANY, _("&Activate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPointsButtons->Add( m_buttonPointActivate, 0, wxALL|wxEXPAND, 5 );

	m_buttonPointCenterView = new wxButton( m_panelPoint, wxID_ANY, _("&Center View"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPointsButtons->Add( m_buttonPointCenterView, 0, wxALL|wxEXPAND, 5 );

	m_buttonPointDelete = new wxButton( m_panelPoint, wxID_ANY, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPointsButtons->Add( m_buttonPointDelete, 0, wxALL|wxEXPAND, 5 );

	m_buttonPointExportSelected = new wxButton( m_panelPoint, wxID_ANY, _("&Export Selected..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPointsButtons->Add( m_buttonPointExportSelected, 0, wxALL|wxEXPAND, 5 );


	m_bSizerPointsButtons->Add( 0, 0, 1, wxEXPAND, 5 );

	m_buttonPointDeleteAll = new wxButton( m_panelPoint, wxID_ANY, _("Dele&te All"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerPointsButtons->Add( m_buttonPointDeleteAll, 0, wxALL|wxEXPAND, 5 );


	bSizer3->Add( m_bSizerPointsButtons, 0, wxEXPAND, 5 );


	m_panelPoint->SetSizer( bSizer3 );
	m_panelPoint->Layout();
	bSizer3->Fit( m_panelPoint );
	m_notebookPathAndPointManager->AddPage( m_panelPoint, _("OD Points"), false );
	m_panelLayer = new wxPanel( m_notebookPathAndPointManager, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_listCtrlLayers = new wxListCtrl( m_panelLayer, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_ICON|wxLC_REPORT|wxLC_SORT_ASCENDING|wxLC_VRULES );
	bSizer4->Add( m_listCtrlLayers, 1, wxALL, 5 );

	m_bSizerLayerButtons = new wxBoxSizer( wxVERTICAL );

	m_buttonLayerTemporary = new wxButton( m_panelLayer, wxID_ANY, _("&Temporary layer"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerTemporary, 0, wxALL|wxEXPAND, 5 );

	m_buttonLayerDelete = new wxButton( m_panelLayer, wxID_ANY, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerDelete, 0, wxALL|wxEXPAND, 5 );

	m_buttonLayerShowOnChart = new wxButton( m_panelLayer, wxID_ANY, _("Show on &chart"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerShowOnChart, 0, wxALL|wxEXPAND, 5 );

	m_buttonLayerShowPointNames = new wxButton( m_panelLayer, wxID_ANY, _("Show &Point names"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerShowPointNames, 0, wxALL|wxEXPAND, 5 );

	m_buttonLayerListContents = new wxButton( m_panelLayer, wxID_ANY, _("&List contents"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bSizerLayerButtons->Add( m_buttonLayerListContents, 0, wxALL|wxEXPAND, 5 );


	bSizer4->Add( m_bSizerLayerButtons, 0, wxEXPAND, 5 );


	m_panelLayer->SetSizer( bSizer4 );
	m_panelLayer->Layout();
	bSizer4->Fit( m_panelLayer );
	m_notebookPathAndPointManager->AddPage( m_panelLayer, _("Layers"), false );

	m_bSizerDialog->Add( m_notebookPathAndPointManager, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );

	m_buttonImport = new wxButton( this, wxID_ANY, _("Import"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_buttonImport, 0, wxALL, 5 );

	m_buttonExportAllVisible = new wxButton( this, wxID_ANY, _("E&xport All Visible"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_buttonExportAllVisible, 0, wxALL, 5 );


	bSizer8->Add( bSizer9, 1, wxEXPAND, 5 );

	m_buttonOK = new wxButton( this, wxID_ANY, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_buttonOK, 0, wxALIGN_RIGHT|wxALL, 5 );


	m_bSizerDialog->Add( bSizer8, 1, wxEXPAND, 5 );


	this->SetSizer( m_bSizerDialog );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
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
	m_buttonPathDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathDeleteClick ), NULL, this );
	m_buttonPathExportSelected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathExportSelectedClick ), NULL, this );
	m_buttonPathDeleteAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathDeleteAllClick ), NULL, this );
	m_listCtrlPoints->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnPointColumnClick ), NULL, this );
	m_listCtrlPoints->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( PathAndPointManagerDialogDef::OnPointDefaultAction ), NULL, this );
	m_listCtrlPoints->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnPointDeSelected ), NULL, this );
	m_listCtrlPoints->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnPointRightClick ), NULL, this );
	m_listCtrlPoints->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnPointSelected ), NULL, this );
	m_buttonPointNew->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointNewClick ), NULL, this );
	m_buttonPointProperties->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointPropertiesClick ), NULL, this );
	m_buttonPointActivate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointActivateClick ), NULL, this );
	m_buttonPointCenterView->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointCenterViewClick ), NULL, this );
	m_buttonPointDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointDeleteClick ), NULL, this );
	m_buttonPointExportSelected->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointExportSecectedClick ), NULL, this );
	m_buttonPointDeleteAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointDeleteAllClick ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerColumnClick ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerDefaultAction ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerDeSelected ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerRightClick ), NULL, this );
	m_listCtrlLayers->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerSelected ), NULL, this );
	m_buttonLayerTemporary->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerTemporaryClick ), NULL, this );
	m_buttonLayerDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerDeleteClick ), NULL, this );
	m_buttonLayerShowOnChart->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerShowOnChartClick ), NULL, this );
	m_buttonLayerShowPointNames->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerShowPointNamesClick ), NULL, this );
	m_buttonLayerListContents->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerListContentsClick ), NULL, this );
	m_buttonImport->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnImportClick ), NULL, this );
	m_buttonExportAllVisible->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnExportAllClick ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnOKClick ), NULL, this );
}

PathAndPointManagerDialogDef::~PathAndPointManagerDialogDef()
{
	// Disconnect Events
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
	m_buttonPathDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathDeleteClick ), NULL, this );
	m_buttonPathExportSelected->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathExportSelectedClick ), NULL, this );
	m_buttonPathDeleteAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPathDeleteAllClick ), NULL, this );
	m_listCtrlPoints->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnPointColumnClick ), NULL, this );
	m_listCtrlPoints->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( PathAndPointManagerDialogDef::OnPointDefaultAction ), NULL, this );
	m_listCtrlPoints->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnPointDeSelected ), NULL, this );
	m_listCtrlPoints->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnPointRightClick ), NULL, this );
	m_listCtrlPoints->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnPointSelected ), NULL, this );
	m_buttonPointNew->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointNewClick ), NULL, this );
	m_buttonPointProperties->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointPropertiesClick ), NULL, this );
	m_buttonPointActivate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointActivateClick ), NULL, this );
	m_buttonPointCenterView->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointCenterViewClick ), NULL, this );
	m_buttonPointDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointDeleteClick ), NULL, this );
	m_buttonPointExportSelected->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointExportSecectedClick ), NULL, this );
	m_buttonPointDeleteAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnPointDeleteAllClick ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerColumnClick ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerDefaultAction ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerDeSelected ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerRightClick ), NULL, this );
	m_listCtrlLayers->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( PathAndPointManagerDialogDef::OnLayerSelected ), NULL, this );
	m_buttonLayerTemporary->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerTemporaryClick ), NULL, this );
	m_buttonLayerDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerDeleteClick ), NULL, this );
	m_buttonLayerShowOnChart->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerShowOnChartClick ), NULL, this );
	m_buttonLayerShowPointNames->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerShowPointNamesClick ), NULL, this );
	m_buttonLayerListContents->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnLayerListContentsClick ), NULL, this );
	m_buttonImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnImportClick ), NULL, this );
	m_buttonExportAllVisible->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnExportAllClick ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PathAndPointManagerDialogDef::OnOKClick ), NULL, this );

}
