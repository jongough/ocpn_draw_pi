///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/listctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PathAndPointManagerDialogDef
///////////////////////////////////////////////////////////////////////////////
class PathAndPointManagerDialogDef : public wxDialog
{
	private:

	protected:
		wxBoxSizer* m_bSizerDialog;
		wxNotebook* m_notebookPathAndPointManager;
		wxPanel* m_panelPath;
		wxListCtrl* m_listCtrlPath;
		wxBoxSizer* m_bSizerPathButtons;
		wxButton* m_buttonPathProperties;
		wxButton* m_buttonPathActivate;
		wxButton* m_buttonPathCenterView;
		wxButton* m_buttonPathDelete;
		wxButton* m_buttonPathExportSelected;
		wxButton* m_buttonPathDeleteAll;
		wxPanel* m_panelODPoint;
		wxListCtrl* m_listCtrlODPoints;
		wxBoxSizer* m_bSizerODPointsButtons;
		wxButton* m_buttonODPointNew;
		wxButton* m_buttonODPointProperties;
		wxButton* m_buttonODPointCenterView;
		wxButton* m_buttonODPointDelete;
		wxButton* m_buttonODPointExportSelected;
		wxButton* m_buttonODPointDeleteAll;
		wxPanel* m_panelLayer;
		wxListCtrl* m_listCtrlLayers;
		wxBoxSizer* m_bSizerLayerButtons;
		wxButton* m_buttonLayerTemporary;
		wxButton* m_buttonLayerDelete;
		wxButton* m_buttonLayerShowOnChart;
		wxButton* m_buttonLayerShowPointNames;
		wxButton* m_buttonLayerListContents;
		wxButton* m_buttonImport;
		wxButton* m_buttonExportAllVisible;
		wxButton* m_buttonOK;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnTabSwitch( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnPathToggleVisibility( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPathColumnClicked( wxListEvent& event ) { event.Skip(); }
		virtual void OnPathDefaultAction( wxListEvent& event ) { event.Skip(); }
		virtual void OnPathDeSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnPathRightClick( wxListEvent& event ) { event.Skip(); }
		virtual void OnPathSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnPathPropertiesClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPathActivateClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPathButtonLeftDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPathCenterViewClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPathDeleteClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPathExportSelectedClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPathDeleteAllClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnODPointToggleVisibility( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnODPointColumnClick( wxListEvent& event ) { event.Skip(); }
		virtual void OnODPointDefaultAction( wxListEvent& event ) { event.Skip(); }
		virtual void OnODPointDeSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnODPointRightClick( wxListEvent& event ) { event.Skip(); }
		virtual void OnODPointSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnODPointNewClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnODPointPropertiesClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnODPointCenterViewClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnODPointDeleteClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnODPointExportSecectedClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnODPointDeleteAllClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayerToggleVisibility( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnLayerColumnClick( wxListEvent& event ) { event.Skip(); }
		virtual void OnLayerDefaultAction( wxListEvent& event ) { event.Skip(); }
		virtual void OnLayerSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnLayerRightClick( wxListEvent& event ) { event.Skip(); }
		virtual void OnLayerTemporaryClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayerDeleteClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayerShowOnChartClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayerShowPointNamesClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayerListContentsClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImportClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportAllVisibleClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOKClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		PathAndPointManagerDialogDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Path & Point Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 527,450 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
		~PathAndPointManagerDialogDef();

};

