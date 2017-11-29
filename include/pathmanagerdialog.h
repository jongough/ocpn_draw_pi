/*
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA  02110-1301, USA.

    ---
    Copyright (C) 2010, Anders Lund <anders@alweb.dk>
 */

#ifndef _PathManagerDialog_h_
#define _PathManagerDialog_h_

#include <wx/dialog.h>
#include <wx/timer.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>

enum {
      SORT_ON_DISTANCE  = 1,
      SORT_ON_NAME
};

enum TrackContextMenu {
      TRACK_MERGE  = 1,
      TRACK_COPY_TEXT,
      TRACK_CLEAN
};

class wxButton;
class ODPath;
class ODPoint;
class Layer;

class PathManagerDialog : public wxDialog {
      DECLARE_EVENT_TABLE()

      public:
            PathManagerDialog(wxWindow *parent);
            ~PathManagerDialog();
            void SetColorScheme();
            void UpdatePathListCtrl();     // Rebuild route list
            void UpdateODPointsListCtrl(ODPoint *rp_select = NULL, bool b_retain_sort = false);
            void UpdateLayListCtrl();
            void UpdateODPointsListCtrlViz();
            
            void OnTabSwitch(wxNotebookEvent& event);
            static void ODPointShowPropertiesDialog( ODPoint* wp, wxWindow* parent );
            void ShowPathPropertiesDialog ( ODPath *path );
//            void TrackToRoute( Track *track );
            static wxString GetLayerName( int id );

      private:
            void Create();
            void MakeAllPathsInvisible();  // Mark all boundaries as invisible. Does not flush settings.
            void ZoomtoPath(ODPath *path); // Attempt to zoom path into the view
            void UpdatePathButtons();
            void UpdateODPointButtons();
            void UpdateLayButtons();           // Correct button state
            
            void ToggleLayerContentsOnChart(Layer *layer);
            void ToggleLayerContentsOnListing(Layer *layer);
            void ToggleLayerContentsNames(Layer *layer);

            // event handlers
            void OnPathSelected(wxListEvent &event);
            void OnPathDeSelected(wxListEvent &event);
            void OnPathDefaultAction(wxListEvent &event);
            void OnPathToggleVisibility(wxMouseEvent &event);
            void OnPathColumnClicked(wxListEvent &event);
            void OnPathPropertiesClick(wxCommandEvent &event);
            void OnPathBtnLeftDown(wxMouseEvent &event); // record control key state for some action buttons
            void OnPathZoomtoClick(wxCommandEvent &event);
            void OnPathDeleteClick(wxCommandEvent &event);
            void OnPathExportClick(wxCommandEvent &event);
            void OnPathDeleteAllClick(wxCommandEvent &event);
            void OnPathActivateClick(wxCommandEvent &event);
            void OnTrkDefaultAction(wxListEvent &event);
            void OnODPointDefaultAction(wxListEvent &event);
            void OnODPointNewClick(wxCommandEvent &event);
            void OnODPointPropertiesClick(wxCommandEvent &event);
            void OnODPointZoomtoClick(wxCommandEvent &event);
            void OnODPointDeleteClick(wxCommandEvent &event);
//            void OnODPointGoToClick(wxCommandEvent &event);
            void OnODPointExportClick(wxCommandEvent &event);
//            void OnODPointSendToGPSClick(wxCommandEvent &event);
            void OnODPointDeleteAllClick(wxCommandEvent &event);
            void OnODPointSelected(wxListEvent &event);
            void OnODPointDeSelected(wxListEvent &event);
            void OnODPointToggleVisibility(wxMouseEvent &event);
            void OnODPointColumnClicked(wxListEvent &event);

            void OnLayDefaultAction(wxListEvent &event);
            void OnLayNewClick(wxCommandEvent &event);
            void OnLayPropertiesClick(wxCommandEvent &event);
            void OnLayToggleChartClick(wxCommandEvent &event);
            void OnLayToggleListingClick(wxCommandEvent &event);
            void OnLayToggleNamesClick(wxCommandEvent &event);
            void OnLayDeleteClick(wxCommandEvent &event);
            void OnLaySelected(wxListEvent &event);
            void OnLayToggleVisibility(wxMouseEvent &event);
            void OnLayColumnClicked(wxListEvent &event);

            void OnImportClick(wxCommandEvent &event);
            void OnExportClick(wxCommandEvent &event);
            void OnExportVizClick(wxCommandEvent &event);
            
            void OnClose(wxCloseEvent &event);
            void OnOK(wxCommandEvent &event);
            void DeSelectPaths( void );
            void DeSelectODPoints( void );
            
            // properties
            wxNotebook *m_pNotebook;
            wxPanel    *m_pPanelODPoint;
            wxPanel    *m_pPanelPath;
            wxPanel     *m_pPanelLay;
            wxListCtrl *m_pPathListCtrl;
            wxListCtrl *m_pODPointListCtrl;
            wxListCtrl  *m_pLayListCtrl;
            

            wxButton *btnPathDelete;
            wxButton *btnPathExport;
            wxButton *btnPathZoomto;
            wxButton *btnPathProperties;
            wxButton *btnPathDeleteAll;
            wxButton *btnPathActivate;
            
            wxButton *btnODPointNew;
            wxButton *btnODPointProperties;
            wxButton *btnODPointZoomto;
            wxButton *btnODPointDelete;
            wxButton *btnODPointExport;
            wxButton *btnODPointSendToGPS;
            wxButton *btnODPointDeleteAll;
            
            wxButton *btnLayNew;
            //wxButton *btnLayProperties;
            wxButton *btnLayToggleChart;
            wxButton *btnLayToggleListing;
            wxButton *btnLayToggleNames;
            wxButton *btnLayDelete;
            
            wxButton *btnImport;
            wxButton *btnExport;
            wxButton *btnExportViz;
            
            wxButton *btnPathOK;
            
            bool m_bPossibleClick;    // do
            bool m_bCtrlDown;         // record control key state for some action buttons
            bool m_bNeedConfigFlush;  // if true, update config in destructor

            int m_lastODPointItem;
            int m_lastPathItem;
            
            wxWindow *m_wParent;
};

#endif // _PathManagerDialog_h_
// kate: indent-width 6; indent-mode cstyle; space-indent on;
