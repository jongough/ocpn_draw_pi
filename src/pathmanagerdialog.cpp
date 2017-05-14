/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPN Draw ODPath Manager Dialog support
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 **************************************************************************/
 
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "pathmanagerdialog.h"

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/progdlg.h>
#include <wx/clipbrd.h>
#include <wx/defs.h>
#include <wx/window.h>

#include <iostream>

#include "ocpn_plugin.h"
#include "ocpn_draw_pi.h"
//#include "styles.h"
#include "dychart.h"
//#include "navutil.h"
#include "ODConfig.h"
#include "ODPath.h"
#include "ODPathPropertiesDialogImpl.h"
#include "ODPointPropertiesImpl.h"
#include "Boundary.h"
#include "BoundaryProp.h"
#include "EBL.h"
#include "EBLProp.h"
#include "DR.h"
#include "DRProp.h"
#include "GZ.h"
#include "GZProp.h"
#include "PIL.h"
#include "PILProp.h"
#include "PathMan.h"
#include "PointMan.h"
#include "ODPoint.h"
#include "ODSelect.h"
#include "chcanv.h"
#include "georef.h"
#include "Layer.h"

#define DIALOG_MARGIN 3

/* XPM */
static const char *eye[]={
"20 20 7 1",
". c none",
"# c #000000",
"a c #333333",
"b c #666666",
"c c #999999",
"d c #cccccc",
"e c #ffffff",
"....................",
"....................",
"....................",
"....................",
".......######.......",
".....#aabccb#a#.....",
"....#deeeddeebcb#...",
"..#aeeeec##aceaec#..",
".#bedaeee####dbcec#.",
"#aeedbdabc###bcceea#",
".#bedad######abcec#.",
"..#be#d######dadb#..",
"...#abac####abba#...",
".....##acbaca##.....",
".......######.......",
"....................",
"....................",
"....................",
"....................",
"...................."};

/* XPM */
static const char *eyex[]={
    "20 20 8 1",
    "# c None",
    "a c #000000",
    "b c #333333",
    "c c #666666",
    "d c #999999",
    "f c #cccccc",
    ". c #ff0000",
    "e c #ffffff",
    ".##################.",
    "..################..",
    "#..##############..#",
    "##..############..##",
    "###..##aaaaaa##..###",
    "####..bbcddcab..####",
    "####a..eeffee..ca###",
    "##abee..daab..beda##",
    "#acefbe..aa..fcdeda#",
    "abeefcfb....acddeeba",
    "#acefbfaa..aabcdeda#",
    "##aceafa....afbfca##",
    "###abcb..aa..ccba###",
    "#####a..dcbd..a#####",
    "#####..aaaaaa..#####",
    "####..########..####",
    "###..##########..###",
    "##..############..##",
    "#..##############..#",
    "..################.."};

enum { colPATHVISIBLE = 0, colPATHNAME, colPATHDESC };
enum { colLAYVISIBLE = 0, colLAYNAME, colLAYITEMS };
enum { colOCPNPOINTICON = 0, colOCPNPOINTNAME, colOCPNPOINTDIST };

// GLOBALS :0
extern ocpn_draw_pi *g_ocpn_draw_pi;
extern PathList     *g_pPathList;
extern BoundaryList *g_pBoundaryList;
extern LayerList    *pLayerList;
extern ODPathPropertiesDialogImpl     *g_pODPathPropDialog;
extern ODPathPropertiesDialogImpl     *g_pPathPropDialog;
extern BoundaryProp *g_pBoundaryPropDialog;
extern EBLProp      *g_pEBLPropDialog;
extern DRProp       *g_pDRPropDialog;
extern GZProp       *g_pGZPropDialog;
extern PILProp      *g_pPILPropDialog;
extern PathMan      *g_pPathMan;
extern ODPointList  *g_pODPointList;
extern ODConfig     *g_pODConfig;
extern ChartCanvas *ocpncc1;
extern ChartBase *Current_Ch;
extern PointMan      *g_pODPointMan;
extern ODPointPropertiesImpl *g_pODPointPropDialog;
extern ODSelect     *g_pODSelect;
extern double       g_dLat, g_dLon;
extern double       gCog, gSog;
extern bool         g_bShowLayers;
extern wxString     g_sODPointIconName;
extern ODPlugIn_Position_Fix_Ex  g_pfFix;

//extern AIS_Decoder      *g_pAIS;
extern PlugIn_ViewPort  *g_pVP;

// sort callback. Sort by route name.
int sort_path_name_dir;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortPathOnName(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortPathOnName(long item1, long item2, long list)
#endif
{
    wxListCtrl *lc = (wxListCtrl*)list;

    wxListItem it1, it2;
    it1.SetId(lc->FindItem(-1, item1));
    it1.SetColumn(1);
    it1.SetMask(it1.GetMask() | wxLIST_MASK_TEXT);

    it2.SetId(lc->FindItem(-1, item2));
    it2.SetColumn(1);
    it2.SetMask(it2.GetMask() | wxLIST_MASK_TEXT);

    lc->GetItem(it1);
    lc->GetItem(it2);

    if(sort_path_name_dir & 1)
    return it2.GetText().CmpNoCase(it1.GetText());
    else
    return it1.GetText().CmpNoCase(it2.GetText());

}

// sort callback. Sort by route Destination.
int sort_path_to_dir;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortPathOnTo(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortPathOnTo(long item1, long item2, long list)
#endif
{
    wxListCtrl *lc = (wxListCtrl*)list;

    wxListItem it1, it2;
    it1.SetId(lc->FindItem(-1, item1));
    it1.SetColumn(2);
    it1.SetMask(it1.GetMask() | wxLIST_MASK_TEXT);

    it2.SetId(lc->FindItem(-1, item2));
    it2.SetColumn(2);
    it2.SetMask(it2.GetMask() | wxLIST_MASK_TEXT);

    lc->GetItem(it1);
    lc->GetItem(it2);

    if(sort_path_to_dir & 1)
    return it2.GetText().CmpNoCase(it1.GetText());
    else
    return it1.GetText().CmpNoCase(it2.GetText());
}

int sort_ODPoint_key;

// sort callback. Sort by point name.
int sort_ODPoint_name_dir;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortODPointsOnName(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortODPointsOnName(long item1, long item2, long list)
#endif

{
    ODPoint *pRP1 = (ODPoint *)item1;
    ODPoint *pRP2 = (ODPoint *)item2;

    if(pRP1 && pRP2) {
        if(sort_ODPoint_name_dir & 1)
            return pRP2->GetName().CmpNoCase(pRP1->GetName());
        else
            return pRP1->GetName().CmpNoCase(pRP2->GetName());
    }
    else
        return 0;
    
}

// sort callback. Sort by point distance.
int sort_ODPoint_len_dir;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortODPointsOnDistance(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortODPointsOnDistance(long item1, long item2, long list)
#endif

{
    wxListCtrl *lc = (wxListCtrl*)list;

    wxListItem it1, it2;
    it1.SetId(lc->FindItem(-1, item1));
    it1.SetColumn(2);
    it1.SetMask(it1.GetMask() | wxLIST_MASK_TEXT);

    it2.SetId(lc->FindItem(-1, item2));
    it2.SetColumn(2);
    it2.SetMask(it2.GetMask() | wxLIST_MASK_TEXT);

    lc->GetItem(it1);
    lc->GetItem(it2);

    wxString s1, s2;
    s1.Printf(_T("%11s"), it1.GetText().c_str());
    s2.Printf(_T("%11s"), it2.GetText().c_str());

    double l1, l2;
    s1.ToDouble(&l1);
    s2.ToDouble(&l2);

    if(sort_ODPoint_len_dir & 1)
    return(l1 < l2);
    else
    return(l2 < l1);

}

// sort callback. Sort by layer name.
int sort_layer_name_dir;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortLayersOnName(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortLayersOnName(long item1, long item2, long list)
#endif
{
    wxListCtrl *lc = (wxListCtrl*)list;

    wxListItem it1, it2;
    it1.SetId(lc->FindItem(-1, item1));
    it1.SetColumn(1);
    it1.SetMask(it1.GetMask() | wxLIST_MASK_TEXT);

    it2.SetId(lc->FindItem(-1, item2));
    it2.SetColumn(1);
    it2.SetMask(it2.GetMask() | wxLIST_MASK_TEXT);

    lc->GetItem(it1);
    lc->GetItem(it2);

    if(sort_layer_name_dir & 1)
    return it2.GetText().CmpNoCase(it1.GetText());
    else
    return it1.GetText().CmpNoCase(it2.GetText());

}

// sort callback. Sort by layer size.
int sort_layer_len_dir;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortLayersOnSize(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortLayersOnSize(long item1, long item2, long list)
#endif
{
    wxListCtrl *lc = (wxListCtrl*)list;

    wxListItem it1, it2;
    it1.SetId(lc->FindItem(-1, item1));
    it1.SetColumn(2);
    it1.SetMask(it1.GetMask() | wxLIST_MASK_TEXT);

    it2.SetId(lc->FindItem(-1, item2));
    it2.SetColumn(2);
    it2.SetMask(it2.GetMask() | wxLIST_MASK_TEXT);

    lc->GetItem(it1);
    lc->GetItem(it2);

    wxString s1, s2;
    s1.Printf(_T("%11s"), it1.GetText().c_str());
    s2.Printf(_T("%11s"), it2.GetText().c_str());

    double l1, l2;
    s1.ToDouble(&l1);
    s2.ToDouble(&l2);

    if(sort_layer_len_dir & 1)
    return(l1 < l2);
    else
    return(l2 < l1);

}

// event table. Empty, because I find it much easier to see what is connected to what
// using Connect() where possible, so that it is visible in the code.
BEGIN_EVENT_TABLE(PathManagerDialog, wxDialog)
    EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, PathManagerDialog::OnTabSwitch) // This should work under Windows :-(
    EVT_CLOSE(PathManagerDialog::OnClose)
END_EVENT_TABLE()

void PathManagerDialog::OnTabSwitch( wxNotebookEvent &event )
{
    if( !m_pNotebook ) return;
    int current_page = m_pNotebook->GetSelection();
    
    switch (current_page)
    {
        case 0:
            // Path
            if ( m_pPathListCtrl ) UpdatePathListCtrl();
            break;
            
        case 1:
            // Point
            if ( m_pODPointListCtrl ) UpdateODPointsListCtrl();
            break;
            
        case 2:
            // Layer
            break;
            
        case wxNOT_FOUND:
            break;
            
        default:
            break;
    }
/*    if( current_page == 4 ) {
//        if( btnImport ) btnImport->Enable( false );
//        if( btnExport ) btnExport->Enable( false );
//        if( btnExportViz ) btnExportViz->Enable( false );
    } else {
        if( btnImport ) btnImport->Enable( true );
        if( btnExport ) btnExport->Enable( true );
        if( btnExportViz ) btnExportViz->Enable( true );

    }
*/    
    event.Skip(); // remove if using event table... why?
}

// implementation
PathManagerDialog::PathManagerDialog( wxWindow *parent )
{
    long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER;
#ifdef __WXOSX__
    style |= wxSTAY_ON_TOP;
#endif

    wxDialog::Create( parent, -1, wxString( _("Path & Point Manager") ), wxDefaultPosition, wxDefaultSize,
            style );
    
    wxFont *qFont = GetOCPNScaledFont_PlugIn(wxS("Dialog"), 0);
    SetFont( *qFont );
    
    m_lastODPointItem = -1;
    m_lastPathItem = -1;

    btnImport = NULL;
    btnExport = NULL;
    btnExportViz = NULL;

    Create();

}

void PathManagerDialog::Create()
{
    wxBoxSizer* itemBoxSizer1 = new wxBoxSizer( wxVERTICAL );
    SetSizer( itemBoxSizer1 );
    m_pPathListCtrl = NULL;
    m_pODPointListCtrl = NULL;

    m_pNotebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( -1, -1 ), wxNB_TOP );
    itemBoxSizer1->Add( m_pNotebook, 1,
            wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 5 );

    //  Create "Path" panel
    m_pPanelPath = new wxPanel( m_pNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize,
            wxNO_BORDER | wxTAB_TRAVERSAL );
    wxBoxSizer *sbsPaths = new wxBoxSizer( wxHORIZONTAL );
    m_pPanelPath->SetSizer( sbsPaths );
    m_pNotebook->AddPage( m_pPanelPath, _("Paths") );

    m_pPathListCtrl = new wxListCtrl( m_pPanelPath, -1, wxDefaultPosition, wxSize( 400, -1 ),
            wxLC_REPORT  | wxLC_SORT_ASCENDING | wxLC_HRULES
                    | wxBORDER_SUNKEN/*|wxLC_VRULES*/);
    m_pPathListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED,
            wxListEventHandler(PathManagerDialog::OnPathSelected), NULL, this );
    m_pPathListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED,
            wxListEventHandler(PathManagerDialog::OnPathDeSelected), NULL, this );
    m_pPathListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            wxListEventHandler(PathManagerDialog::OnPathDefaultAction), NULL, this );
    m_pPathListCtrl->Connect( wxEVT_LEFT_DOWN,
            wxMouseEventHandler(PathManagerDialog::OnPathToggleVisibility), NULL, this );
    m_pPathListCtrl->Connect( wxEVT_COMMAND_LIST_COL_CLICK,
            wxListEventHandler(PathManagerDialog::OnPathColumnClicked), NULL, this );
    sbsPaths->Add( m_pPathListCtrl, 1, wxEXPAND | wxALL, DIALOG_MARGIN );

    // Columns: visibility ctrl, name
    // note that under MSW for SetColumnWidth() to work we need to create the
    // items with images initially even if we specify dummy image id

    m_pPathListCtrl->InsertColumn( colPATHVISIBLE, _("Show"), wxLIST_FORMAT_LEFT, 40 );
    m_pPathListCtrl->InsertColumn( colPATHNAME, _("Path Name"), wxLIST_FORMAT_LEFT, 120 );
    m_pPathListCtrl->InsertColumn( colPATHDESC, _("Desc"), wxLIST_FORMAT_LEFT, 230 );

    // Buttons: Delete, Properties...
    wxBoxSizer *bsPathButtons = new wxBoxSizer( wxVERTICAL );
    sbsPaths->Add( bsPathButtons, 0, wxALIGN_RIGHT );

    btnPathProperties = new wxButton( m_pPanelPath, -1, _("&Properties...") );
    bsPathButtons->Add( btnPathProperties, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnPathProperties->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnPathPropertiesClick), NULL, this );

    btnPathActivate = new wxButton( m_pPanelPath, -1, _("&Activate") );
    bsPathButtons->Add( btnPathActivate, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnPathActivate->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnPathActivateClick), NULL, this );
    btnPathActivate->Connect( wxEVT_LEFT_DOWN,
            wxMouseEventHandler(PathManagerDialog::OnPathBtnLeftDown), NULL, this );

    btnPathZoomto = new wxButton( m_pPanelPath, -1, _("&Center View") );
    bsPathButtons->Add( btnPathZoomto, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnPathZoomto->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnPathZoomtoClick), NULL, this );
    btnPathZoomto->Connect( wxEVT_LEFT_DOWN,
            wxMouseEventHandler(PathManagerDialog::OnPathBtnLeftDown), NULL, this );

    btnPathDelete = new wxButton( m_pPanelPath, -1, _("&Delete") );
    bsPathButtons->Add( btnPathDelete, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnPathDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnPathDeleteClick), NULL, this );

    btnPathExport = new wxButton( m_pPanelPath, -1, _("&Export selected...") );
    bsPathButtons->Add( btnPathExport, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnPathExport->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnPathExportClick), NULL, this );

    bsPathButtons->AddSpacer( 10 );

    btnPathDeleteAll = new wxButton( m_pPanelPath, -1, _("&Delete All") );
    bsPathButtons->Add( btnPathDeleteAll, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnPathDeleteAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnPathDeleteAllClick), NULL, this );

    //  Create "OD points" panel
    m_pPanelODPoint = new wxPanel( m_pNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize,
            wxNO_BORDER | wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer( wxHORIZONTAL );
    m_pPanelODPoint->SetSizer( itemBoxSizer4 );
    m_pNotebook->AddPage( m_pPanelODPoint, _("OD Points") );

    m_pODPointListCtrl = new wxListCtrl( m_pPanelODPoint, -1, wxDefaultPosition, wxSize( 400, -1 ),
            wxLC_REPORT | wxLC_SORT_ASCENDING | wxLC_HRULES | wxBORDER_SUNKEN/*|wxLC_VRULES*/);
    m_pODPointListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED,
            wxListEventHandler(PathManagerDialog::OnODPointSelected), NULL, this );
    m_pODPointListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED,
            wxListEventHandler(PathManagerDialog::OnODPointDeSelected), NULL, this );
    m_pODPointListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            wxListEventHandler(PathManagerDialog::OnODPointDefaultAction), NULL, this );
    m_pODPointListCtrl->Connect( wxEVT_LEFT_DOWN,
            wxMouseEventHandler(PathManagerDialog::OnODPointToggleVisibility), NULL, this );
    m_pODPointListCtrl->Connect( wxEVT_COMMAND_LIST_COL_CLICK,
            wxListEventHandler(PathManagerDialog::OnODPointColumnClicked), NULL, this );
    itemBoxSizer4->Add( m_pODPointListCtrl, 1, wxEXPAND | wxALL, DIALOG_MARGIN );

    m_pODPointListCtrl->InsertColumn( colOCPNPOINTICON, _("Icon"), wxLIST_FORMAT_LEFT, 44 );
    m_pODPointListCtrl->InsertColumn( colOCPNPOINTNAME, _("OD Point Name"), wxLIST_FORMAT_LEFT, 180 );
    m_pODPointListCtrl->InsertColumn( colOCPNPOINTDIST, _("Distance from Ownship"), wxLIST_FORMAT_LEFT, 180 );

    wxBoxSizer *bsODPointButtons = new wxBoxSizer( wxVERTICAL );
    itemBoxSizer4->Add( bsODPointButtons, 0, wxALIGN_RIGHT );

    btnODPointNew = new wxButton( m_pPanelODPoint, -1, _("&New") );
    bsODPointButtons->Add( btnODPointNew, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnODPointNew->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnODPointNewClick), NULL, this );

    btnODPointProperties = new wxButton( m_pPanelODPoint, -1, _("&Properties") );
    bsODPointButtons->Add( btnODPointProperties, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnODPointProperties->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnODPointPropertiesClick), NULL, this );

    btnODPointZoomto = new wxButton( m_pPanelODPoint, -1, _("&Center View") );
    bsODPointButtons->Add( btnODPointZoomto, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnODPointZoomto->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnODPointZoomtoClick), NULL, this );

    btnODPointDelete = new wxButton( m_pPanelODPoint, -1, _("&Delete") );
    bsODPointButtons->Add( btnODPointDelete, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnODPointDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnODPointDeleteClick), NULL, this );

    btnODPointExport = new wxButton( m_pPanelODPoint, -1, _("&Export selected...") );
    bsODPointButtons->Add( btnODPointExport, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnODPointExport->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnODPointExportClick), NULL, this );

/*    btnODPointSendToGPS = new wxButton( m_pPanelODPoint, -1, _("&Send to GPS") );
    bsODPointButtons->Add( btnODPointSendToGPS, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnODPointSendToGPS->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnODPointSendToGPSClick), NULL, this );
*/
    bsODPointButtons->AddSpacer( 10 );

    btnODPointDeleteAll = new wxButton( m_pPanelODPoint, -1, _("Delete All") );
    bsODPointButtons->Add( btnODPointDeleteAll, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnODPointDeleteAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnODPointDeleteAllClick), NULL, this );

    wxBoxSizer *itemBoxSizer5 = new wxBoxSizer( wxHORIZONTAL );
    itemBoxSizer1->Add( itemBoxSizer5, 0, wxALL | wxEXPAND );

    wxBoxSizer *itemBoxSizer6 = new wxBoxSizer( wxHORIZONTAL );
    itemBoxSizer5->Add( itemBoxSizer6, 1, wxALL | wxEXPAND | wxALIGN_LEFT );

    btnImport = new wxButton( this, -1, _("I&mport GPX...") );
    itemBoxSizer6->Add( btnImport, 0, wxALL | wxALIGN_LEFT, DIALOG_MARGIN );
    btnImport->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnImportClick), NULL, this );
/*
    btnExport = new wxButton( this, -1, _("E&xport All...") );
    itemBoxSizer6->Add( btnExport, 0, wxALL | wxALIGN_LEFT, DIALOG_MARGIN );
    btnExport->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnExportClick), NULL, this );
*/
    btnExportViz = new wxButton( this, -1, _("Export All Visible...") );
    itemBoxSizer6->Add( btnExportViz, 0, wxALL | wxALIGN_LEFT, DIALOG_MARGIN );
    btnExportViz->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                        wxCommandEventHandler(PathManagerDialog::OnExportVizClick), NULL, this );

    //  Create "Layers" panel
    m_pPanelLay = new wxPanel( m_pNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize,
            wxNO_BORDER | wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer( wxHORIZONTAL );
    m_pPanelLay->SetSizer( itemBoxSizer7 );
    m_pNotebook->AddPage( m_pPanelLay, _("Layers") );

    m_pLayListCtrl = new wxListCtrl( m_pPanelLay, -1, wxDefaultPosition, wxSize( 400, -1 ),
            wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_SORT_ASCENDING | wxLC_HRULES
                    | wxBORDER_SUNKEN);
    m_pLayListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED,
            wxListEventHandler(PathManagerDialog::OnLaySelected), NULL, this );
    m_pLayListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED,
            wxListEventHandler(PathManagerDialog::OnLaySelected), NULL, this );
    m_pLayListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            wxListEventHandler(PathManagerDialog::OnLayDefaultAction), NULL, this );
    m_pLayListCtrl->Connect( wxEVT_LEFT_DOWN,
            wxMouseEventHandler(PathManagerDialog::OnLayToggleVisibility), NULL, this );
    m_pLayListCtrl->Connect( wxEVT_COMMAND_LIST_COL_CLICK,
            wxListEventHandler(PathManagerDialog::OnLayColumnClicked), NULL, this );
    itemBoxSizer7->Add( m_pLayListCtrl, 1, wxEXPAND | wxALL, DIALOG_MARGIN );

    m_pLayListCtrl->InsertColumn( colLAYVISIBLE, _("Show"), wxLIST_FORMAT_LEFT, 44 );
    m_pLayListCtrl->InsertColumn( colLAYNAME, _("Layer Name"), wxLIST_FORMAT_LEFT, 250 );
    m_pLayListCtrl->InsertColumn( colLAYITEMS, _("No. of items"), wxLIST_FORMAT_LEFT, 100 );

    wxBoxSizer *bsLayButtons = new wxBoxSizer( wxVERTICAL );
    itemBoxSizer7->Add( bsLayButtons, 0, wxALIGN_RIGHT );

    btnLayNew = new wxButton( m_pPanelLay, -1, _("Temporary layer") );
    bsLayButtons->Add( btnLayNew, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnLayNew->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnLayNewClick), NULL, this );

    //btnLayProperties = new wxButton(m_pPanelLay, -1, _("&Properties"));
    //bsLayButtons->Add(btnLayProperties, 0, wxALL|wxEXPAND, DIALOG_MARGIN);
    //btnLayProperties->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
    //                     wxCommandEventHandler(PathManagerDialog::OnLayPropertiesClick), NULL, this);

    btnLayDelete = new wxButton( m_pPanelLay, -1, _("&Delete") );
    bsLayButtons->Add( btnLayDelete, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnLayDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnLayDeleteClick), NULL, this );

    btnLayToggleChart = new wxButton( m_pPanelLay, -1, _("Show on chart") );
    bsLayButtons->Add( btnLayToggleChart, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnLayToggleChart->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnLayToggleChartClick), NULL, this );

    btnLayToggleNames = new wxButton( m_pPanelLay, -1, _("Show Point names") );
    bsLayButtons->Add( btnLayToggleNames, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnLayToggleNames->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnLayToggleNamesClick), NULL, this );

    btnLayToggleListing = new wxButton( m_pPanelLay, -1, _("List contents ") );
    bsLayButtons->Add( btnLayToggleListing, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnLayToggleListing->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnLayToggleListingClick), NULL, this );

    // Dialog buttons
    //wxSizer *szButtons = CreateButtonSizer( wxOK );
    //this->Connect( -1, wxEVT_SHOW, wxCloseEventHandler(PathManagerDialog::OnOK), NULL, this);

    btnPathOK = new wxButton( this, -1, _("&OK") );
    itemBoxSizer5->Add( btnPathOK, 0, wxALL | wxALIGN_RIGHT, DIALOG_MARGIN );
    btnPathOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
                            wxCommandEventHandler(PathManagerDialog::OnOK), NULL, this );
    
    Fit();

    SetMinSize( GetBestSize() );
    
    Centre();

    // create a image list for the list with just the eye icon
    wxImageList *imglist = new wxImageList( 20, 20, true, 1 );
    imglist->Add( wxBitmap( eye ) );
    imglist->Add( wxBitmap( eyex ) );
    m_pPathListCtrl->AssignImageList( imglist, wxIMAGE_LIST_SMALL );
    // Assign will handle destroy, Set will not. It's OK, that's what we want
    m_pODPointListCtrl->SetImageList( g_pODPointMan->Getpmarkicon_image_list(), wxIMAGE_LIST_SMALL );
    
    //m_pLayListCtrl->AssignImageList( imglist, wxIMAGE_LIST_SMALL );
    m_pLayListCtrl->SetImageList( imglist, wxIMAGE_LIST_SMALL );

    SetColorScheme();

    UpdatePathListCtrl();
    UpdateODPointsListCtrl();
    UpdateLayListCtrl();

    // This should work under Linux :-(
    //m_pNotebook->Connect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler(PathManagerDialog::OnTabSwitch), NULL, this);

    m_bNeedConfigFlush = false;
}

PathManagerDialog::~PathManagerDialog()
{
    m_pPathListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED,
                              wxListEventHandler(PathManagerDialog::OnPathSelected), NULL, this );
    m_pPathListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED,
                              wxListEventHandler(PathManagerDialog::OnPathDeSelected), NULL, this );
    m_pPathListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                              wxListEventHandler(PathManagerDialog::OnPathDefaultAction), NULL, this );
    m_pPathListCtrl->Disconnect( wxEVT_LEFT_DOWN,
                              wxMouseEventHandler(PathManagerDialog::OnPathToggleVisibility), NULL, this );
    m_pPathListCtrl->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK,
                              wxListEventHandler(PathManagerDialog::OnPathColumnClicked), NULL, this );
    btnPathProperties->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(PathManagerDialog::OnPathPropertiesClick), NULL, this );
    
    btnPathActivate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                              wxCommandEventHandler(PathManagerDialog::OnPathActivateClick), NULL, this );
    btnPathActivate->Disconnect( wxEVT_LEFT_DOWN,
                              wxMouseEventHandler(PathManagerDialog::OnPathBtnLeftDown), NULL, this );
    
    btnPathZoomto->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                            wxCommandEventHandler(PathManagerDialog::OnPathZoomtoClick), NULL, this );
    btnPathZoomto->Disconnect( wxEVT_LEFT_DOWN,
                            wxMouseEventHandler(PathManagerDialog::OnPathBtnLeftDown), NULL, this );
    
    btnPathDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                            wxCommandEventHandler(PathManagerDialog::OnPathDeleteClick), NULL, this );
    
    btnPathExport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                            wxCommandEventHandler(PathManagerDialog::OnPathExportClick), NULL, this );
    
    btnPathDeleteAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                               wxCommandEventHandler(PathManagerDialog::OnPathDeleteAllClick), NULL, this );
    
    m_pODPointListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED,
                                 wxListEventHandler(PathManagerDialog::OnODPointSelected), NULL, this );
    m_pODPointListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED,
                                 wxListEventHandler(PathManagerDialog::OnODPointDeSelected), NULL, this );
    m_pODPointListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                                 wxListEventHandler(PathManagerDialog::OnODPointDefaultAction), NULL, this );
    m_pODPointListCtrl->Disconnect( wxEVT_LEFT_DOWN,
                                 wxMouseEventHandler(PathManagerDialog::OnODPointToggleVisibility), NULL, this );
    m_pODPointListCtrl->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK,
                                 wxListEventHandler(PathManagerDialog::OnODPointColumnClicked), NULL, this );
    btnODPointNew->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                            wxCommandEventHandler(PathManagerDialog::OnODPointNewClick), NULL, this );
    
    btnODPointProperties->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(PathManagerDialog::OnODPointPropertiesClick), NULL, this );
    
    btnODPointZoomto->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                               wxCommandEventHandler(PathManagerDialog::OnODPointZoomtoClick), NULL, this );
    
    btnODPointDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                               wxCommandEventHandler(PathManagerDialog::OnODPointDeleteClick), NULL, this );
    
    btnODPointExport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                               wxCommandEventHandler(PathManagerDialog::OnODPointExportClick), NULL, this );
    
    btnODPointDeleteAll->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                                  wxCommandEventHandler(PathManagerDialog::OnODPointDeleteAllClick), NULL, this );
    
    btnImport->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                        wxCommandEventHandler(PathManagerDialog::OnImportClick), NULL, this );
    btnExportViz->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                           wxCommandEventHandler(PathManagerDialog::OnExportVizClick), NULL, this );
    
    m_pLayListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED,
                             wxListEventHandler(PathManagerDialog::OnLaySelected), NULL, this );
    m_pLayListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_DESELECTED,
                             wxListEventHandler(PathManagerDialog::OnLaySelected), NULL, this );
    m_pLayListCtrl->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
                             wxListEventHandler(PathManagerDialog::OnLayDefaultAction), NULL, this );
    m_pLayListCtrl->Disconnect( wxEVT_LEFT_DOWN,
                             wxMouseEventHandler(PathManagerDialog::OnLayToggleVisibility), NULL, this );
    m_pLayListCtrl->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK,
                             wxListEventHandler(PathManagerDialog::OnLayColumnClicked), NULL, this );
    btnLayNew->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                        wxCommandEventHandler(PathManagerDialog::OnLayNewClick), NULL, this );
    
    btnLayDelete->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                           wxCommandEventHandler(PathManagerDialog::OnLayDeleteClick), NULL, this );
    
    btnLayToggleChart->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(PathManagerDialog::OnLayToggleChartClick), NULL, this );
    
    btnLayToggleNames->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(PathManagerDialog::OnLayToggleNamesClick), NULL, this );
    
    btnLayToggleListing->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                                  wxCommandEventHandler(PathManagerDialog::OnLayToggleListingClick), NULL, this );
    
    btnPathOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED,
                        wxCommandEventHandler(PathManagerDialog::OnOK), NULL, this );
    
    delete m_pPathListCtrl;
    delete m_pODPointListCtrl;
    delete m_pLayListCtrl;

    delete btnODPointNew;
    delete btnODPointProperties;
    delete btnODPointZoomto;
    delete btnODPointDelete;
    delete btnODPointExport;
    delete btnODPointDeleteAll;
    delete btnImport;
    delete btnExport;
    delete btnExportViz;
    delete btnLayNew;
    delete btnLayDelete;
    delete btnLayToggleChart;
    delete btnLayToggleNames;
    delete btnLayToggleListing;
    btnImport = NULL;
    btnExport = NULL;
    btnExportViz = NULL;

    delete m_pNotebook;

    //    Does not need to be done here at all, since this dialog is autommatically deleted as a child of the frame.
    //    By that time, the config has already been updated for shutdown.

    // Do this just once!!
//      if (m_bNeedConfigFlush)
//            g_pODConfig->UpdateSettings();
}

void PathManagerDialog::SetColorScheme()
{
    //TODO Need to find out what this is for?
    DimeWindow( this );
}

void PathManagerDialog::UpdatePathListCtrl()
{
    // if an item was selected, make it selected again if it still exist
    long item = -1;
    item = m_pPathListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    long selected_id = -1;
    if( item != -1 ) selected_id = m_pPathListCtrl->GetItemData( item );

    // Delete existing items
    m_pPathListCtrl->DeleteAllItems();

    // then add path to the listctrl
    PathList::iterator it;
    int index = 0;
    for( it = ( *g_pPathList ).begin(); it != ( *g_pPathList ).end(); ++it, ++index ) {
        if( !( *it )->IsListed() ) continue;

        wxListItem li;
        li.SetId( index );
        li.SetImage( ( *it )->IsVisible() ? 0 : 1 );
        li.SetData( index );
        li.SetText( _T("") );

        if( ( *it )->m_bPathIsActive ) {
            wxFont font = *wxNORMAL_FONT;
            font.SetWeight( wxFONTWEIGHT_BOLD );
            li.SetFont( font );
        }

        long idx = m_pPathListCtrl->InsertItem( li );

        wxString name = ( *it )->m_PathNameString;
        if( name.IsEmpty() ) {
            name = _("(Unnamed )");
#if wxCHECK_VERSION(3,0,0)
            name.append( _(( *it )->m_sTypeString ) );
#else
            name.append( ( *it )->m_sTypeString );
#endif
        }
        m_pPathListCtrl->SetItem( idx, colPATHNAME, name );
        wxString desc = ( *it ) ->m_PathDescription;
        m_pPathListCtrl->SetItem( idx, colPATHDESC, desc );
    }

    m_pPathListCtrl->SortItems( SortPathOnName, (long) m_pPathListCtrl );

    // restore selection if possible
    // NOTE this will select a different item, if one is deleted
    // (the next route will get that index).
    if( selected_id > -1 ) {
        item = m_pPathListCtrl->FindItem( -1, selected_id );
        m_pPathListCtrl->SetItemState( item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
    }

    if( (m_lastPathItem >= 0) && (m_pPathListCtrl->GetItemCount()) )
        m_pPathListCtrl->EnsureVisible( m_lastPathItem );
    UpdatePathButtons();

    for(int i = 0; i < m_pPathListCtrl->GetColumnCount(); i++) {
#ifdef WIN32
        m_pPathListCtrl->SetColumnWidth( i, wxLIST_AUTOSIZE_USEHEADER );
#else
        m_pPathListCtrl->SetColumnWidth( i, wxLIST_AUTOSIZE );
#endif 
    }
    
    this->GetSizer()->Fit( this );
    this->Layout();
    
}

void PathManagerDialog::UpdatePathButtons()
{
    // enable/disable buttons
    long selected_index_index = m_pPathListCtrl->GetNextItem( -1, wxLIST_NEXT_ALL,
            wxLIST_STATE_SELECTED );
    bool enable1 =  m_pPathListCtrl->GetSelectedItemCount() == 1;

    m_lastPathItem = selected_index_index;

    btnPathDelete->Enable( m_pPathListCtrl->GetSelectedItemCount() > 0 );
    btnPathZoomto->Enable( enable1 ); 
    btnPathProperties->Enable( enable1 );
    btnPathDeleteAll->Enable( true );

    // set activate button text
    ODPath *path = NULL;
    if( enable1 ) {
        path = g_pPathList->Item( m_pPathListCtrl->GetItemData( selected_index_index ) )->GetData();
        if ( path ) {
            btnPathActivate->Enable( true );
            if ( path->IsActive() ) btnPathActivate->SetLabel( _("&Deactivate") );
            else btnPathActivate->SetLabel( _("&Activate") );
        }
        else
            btnPathActivate->Enable( false );
    }
    else
        btnPathActivate->Enable( false );
    
}

void PathManagerDialog::MakeAllPathsInvisible()
{
    PathList::iterator it;
    long index = 0;
    for( it = ( *g_pPathList ).begin(); it != ( *g_pPathList ).end(); ++it, ++index ) {
        if( ( *it )->IsVisible() ) { // avoid config updating as much as possible!
            ( *it )->SetVisible( false );
            m_pPathListCtrl->SetItemImage( m_pPathListCtrl->FindItem( -1, index ), 1 ); // Likely not same order :0
            g_pODConfig->UpdatePath( *it ); 
        }
    }
}

void PathManagerDialog::ZoomtoPath( ODPath *path )
{

    // Calculate bbox center
    LLBBox RBBox = path->GetBBox();
    double clat = RBBox.GetMinLat() + ( ( RBBox.GetMaxLat() - RBBox.GetMinLat() ) / 2 );
    double clon = RBBox.GetMinLon() + ( ( RBBox.GetMaxLon() - RBBox.GetMinLon() ) / 2 );

    if( clon > 180. ) clon -= 360.;
    else
        if( clon < -180. ) clon += 360.;

    // Calculate ppm
    double rw, rh, ppm; // route width, height, final ppm scale to use
    int ww, wh; // chart window width, height
    // route bbox width in nm
    DistanceBearingMercator_Plugin( RBBox.GetMinLat(), RBBox.GetMinLon(), RBBox.GetMinLat(),
            RBBox.GetMaxLon(), NULL, &rw );
    // route bbox height in nm
    DistanceBearingMercator_Plugin( RBBox.GetMinLat(), RBBox.GetMinLon(), RBBox.GetMaxLat(),
            RBBox.GetMinLon(), NULL, &rh );

    ocpncc1->GetSize( &ww, &wh );

    ppm = wxMin(ww/(rw*1852), wh/(rh*1852)) * ( 100 - fabs( clat ) ) / 90;

    ppm = wxMin(ppm, 1.0);

    JumpToPosition( clat, clon, ppm );

    m_bNeedConfigFlush = true;
}

//BEGIN Event handlers
void PathManagerDialog::OnPathDeleteClick( wxCommandEvent &event )
{
    PathList list;

    int answer = OCPNMessageBox_PlugIn( this, _("Are you sure you want to delete the selected object(s)"), _("OpenCPN Alert"), wxYES_NO );
    if ( answer != wxID_YES )
        return;

    bool busy = false;
    if( m_pPathListCtrl->GetSelectedItemCount() ) {
        ::wxBeginBusyCursor();
        m_bNeedConfigFlush = true;
        busy = true;
    }

    long item = -1;
    for ( ;; )
    {
        item = m_pPathListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;

        ODPath *ppath_to_delete = g_pPathList->Item( m_pPathListCtrl->GetItemData( item ) )->GetData();

        if( ppath_to_delete )
            list.Append( ppath_to_delete );
    }

    if( busy ) {

        for(unsigned int i=0 ; i < list.GetCount() ; i++) {
            ODPath *path = list.Item(i)->GetData();
            if( path ) {
                g_pODConfig->DeleteConfigPath( path );
                g_pPathMan->DeletePath( path );
            }
        }

        m_lastPathItem = -1;
        UpdatePathListCtrl();

        // TODO fix up undo
        //ocpncc1->undo->InvalidateUndo();
        RequestRefresh( GetOCPNCanvasWindow() );
        ::wxEndBusyCursor();
    }

}

void PathManagerDialog::OnPathDeleteAllClick( wxCommandEvent &event )
{
    int dialog_ret = OCPNMessageBox_PlugIn( this, _("Are you sure you want to delete <ALL> paths?"), _("OpenCPN Alert"), wxYES_NO );

    if( dialog_ret == wxID_YES ) {

        g_pPathMan->DeleteAllPaths();

        m_lastPathItem = -1;
        UpdatePathListCtrl();

        if( g_pODPathPropDialog ) g_pODPathPropDialog->Hide();
        // TODO fix up undo
        //ocpncc1->undo->InvalidateUndo();
        RequestRefresh( GetOCPNCanvasWindow() );

        m_bNeedConfigFlush = true;
    }
}

void PathManagerDialog::OnPathPropertiesClick( wxCommandEvent &event )
{
    // Show path properties dialog for selected path
    long item = -1;
    item = m_pPathListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    ODPath *path = g_pPathList->Item( m_pPathListCtrl->GetItemData( item ) )->GetData();

    if( !path ) return;

    ShowPathPropertiesDialog ( path );
}

void PathManagerDialog::ShowPathPropertiesDialog ( ODPath *inpath )
{
    ODPath *l_pPath = NULL;;
    Boundary *l_pBoundary = NULL;
    EBL *l_pEBL = NULL;
    DR  *l_pDR = NULL;
    GZ  *l_pGZ = NULL;
    PIL *l_pPIL = NULL;
    
    if(inpath->m_sTypeString == wxT( "Boundary") ) {
        if( NULL == g_pBoundaryPropDialog )          // There is one global instance of the BoundaryProp Dialog
            g_pBoundaryPropDialog = new BoundaryProp( g_ocpn_draw_pi->m_parent_window );
        g_pODPathPropDialog = g_pBoundaryPropDialog;
        l_pBoundary = (Boundary *) inpath;
        l_pPath = l_pBoundary;
        g_pBoundaryPropDialog->SetPath( l_pBoundary );
        g_pBoundaryPropDialog->UpdateProperties( l_pBoundary );
    } else if(inpath->m_sTypeString == wxT("EBL")) {
        if( NULL == g_pEBLPropDialog )          // There is one global instance of the ELBProp Dialog
            g_pEBLPropDialog = new EBLProp( GetParent() );
        g_pODPathPropDialog = g_pEBLPropDialog;
        l_pEBL = (EBL *) inpath;
        l_pPath = l_pEBL;
        g_pEBLPropDialog->SetPath( l_pEBL );
        g_pEBLPropDialog->UpdateProperties( l_pEBL );
    } else if(inpath->m_sTypeString == wxT("DR")) {
        if( NULL == g_pDRPropDialog )          // There is one global instance of the DRProp Dialog
            g_pDRPropDialog = new DRProp( GetParent() );
        g_pODPathPropDialog = g_pDRPropDialog;
        l_pDR = (DR *) inpath;
        l_pPath = l_pDR;
        g_pDRPropDialog->SetPath( l_pDR );
        g_pDRPropDialog->UpdateProperties( l_pDR );
    } else if(inpath->m_sTypeString == wxT("Guard Zone")) {
        if( NULL == g_pGZPropDialog )          // There is one global instance of the DRProp Dialog
            g_pGZPropDialog = new GZProp( GetParent() );
        g_pODPathPropDialog = g_pGZPropDialog;
        l_pGZ = (GZ *) inpath;
        l_pPath = l_pGZ;
        g_pGZPropDialog->SetPath( l_pGZ );
        g_pGZPropDialog->UpdateProperties( l_pGZ );
    } else if(inpath->m_sTypeString == wxT("PIL")) {
        if( NULL == g_pPILPropDialog )          // There is one global instance of the ELBProp Dialog
            g_pPILPropDialog = new PILProp( GetParent() );
        g_pODPathPropDialog = g_pPILPropDialog;
        l_pPIL = (PIL *) inpath;
        l_pPath = l_pPIL;
        g_pPILPropDialog->SetPath( l_pPIL );
        g_pPILPropDialog->UpdateProperties( l_pPIL );
    } else {
        if( NULL == g_pPathPropDialog )          // There is one global instance of the PathProp Dialog
            g_pPathPropDialog = new ODPathPropertiesDialogImpl( g_ocpn_draw_pi->m_parent_window );
        g_pODPathPropDialog = g_pPathPropDialog;
        l_pPath = inpath;
        g_pODPathPropDialog->SetPath( l_pPath );
        g_pODPathPropDialog->UpdateProperties( l_pPath );
    }

    if( !l_pPath->m_bIsInLayer ) {
        if ( l_pPath->m_sTypeString.IsNull() || l_pPath->m_sTypeString.IsEmpty() )
            g_pODPathPropDialog->SetDialogTitle( _("Path Properties") );
        else if(l_pPath->m_sTypeString == wxT("Boundary")) 
            g_pODPathPropDialog->SetDialogTitle(_("Boundary Properties"));
        else if(l_pPath->m_sTypeString == wxT("EBL")) 
            g_pODPathPropDialog->SetDialogTitle(_("EBL Properties"));
        else if(l_pPath->m_sTypeString == wxT("DR")) 
            g_pODPathPropDialog->SetDialogTitle(_("DR Properties"));
        else if(l_pPath->m_sTypeString == wxT("Guard Zone")) 
            g_pODPathPropDialog->SetDialogTitle(_("Guard Zone Properties"));
        else if(l_pPath->m_sTypeString == wxT("PIL"))
            g_pODPathPropDialog->SetDialogTitle(_("Parallel Index Line Properties"));
    }
    else {
        wxString caption( wxS("") );
        if ( l_pPath->m_sTypeString.IsNull() || l_pPath->m_sTypeString.IsEmpty() ) 
            caption.append( _("Path Properties, Layer: ") );
        else if(l_pPath->m_sTypeString == wxT("Boundary")) 
            caption.append(_("Boundary Properties, Layer: "));
        else if(l_pPath->m_sTypeString == wxT("EBL")) 
            caption.append(_("EBL Properties, Layer: "));
        else if(l_pPath->m_sTypeString == wxT("DR")) 
            caption.append(_("DR Properties, Layer: "));
        
        caption.append( GetLayerName( l_pPath->m_LayerID ) );
        g_pODPathPropDialog->SetDialogTitle( caption );
        
    }

    if( !g_pODPathPropDialog->IsShown() )
        g_pODPathPropDialog->Show();
    
    UpdatePathListCtrl();

    m_bNeedConfigFlush = true;
}

void PathManagerDialog::OnPathZoomtoClick( wxCommandEvent &event )
{
    // Zoom into the bounding box of the selected route
    long item = -1;
    item = m_pPathListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    // optionally make this path exclusively visible
    if( m_bCtrlDown ) MakeAllPathsInvisible();

    ODPath *path = g_pPathList->Item( m_pPathListCtrl->GetItemData( item ) )->GetData();

    if( !path ) return;

    // Ensure path is visible
    if( !path->IsVisible() ) {
        path->SetVisible( true );
        m_pPathListCtrl->SetItemImage( item, path->IsVisible() ? 0 : 1 );
        g_pODConfig->UpdatePath( path );
    }

    ZoomtoPath( path );
}

void PathManagerDialog::OnPathExportClick( wxCommandEvent &event )
{
    PathList list;

    wxString suggested_name = _T("paths");

    long item = -1;
    for ( ;; )
    {
        item = m_pPathListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;

        ODPath *ppath_to_export = g_pPathList->Item( m_pPathListCtrl->GetItemData( item ) )->GetData();

        if( ppath_to_export ) {
            list.Append( ppath_to_export );
            if( ppath_to_export->m_PathNameString != wxEmptyString )
                suggested_name = ppath_to_export->m_PathNameString;
        }
    }

    g_pODConfig->ExportGPXPaths( this, &list, suggested_name );
}

void PathManagerDialog::OnPathActivateClick( wxCommandEvent &event )
{
    // Activate the selected path, unless it already is
    long item = -1;
    item = m_pPathListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    if( m_bCtrlDown ) MakeAllPathsInvisible();

    ODPath *ppath = g_pPathList->Item( m_pPathListCtrl->GetItemData( item ) )->GetData();

    if( !ppath ) return;

    if( !ppath->m_bPathIsActive ) {
        if( !ppath->IsVisible() ) {
            ppath->SetVisible( true );
            m_pPathListCtrl->SetItemImage( item, 0, 0 );
        }

        ZoomtoPath( ppath );

        g_pPathMan->ActivatePath( (ODPath *) ppath );
        btnPathActivate->SetLabel( _T("&Deactivate") );
    } else {
        g_pPathMan->DeactivatePath( ppath );
        btnPathActivate->SetLabel( _T("&Activate") );
    }

    UpdatePathListCtrl();

    g_pODConfig->UpdatePath( ppath );

    RequestRefresh( GetOCPNCanvasWindow() );

//      btnRteActivate->SetLabel(route->m_bRtIsActive ? wxT("Deactivate") : wxT("Activate"));

    m_bNeedConfigFlush = true;
}

void PathManagerDialog::OnPathToggleVisibility( wxMouseEvent &event )
{
    wxPoint pos = event.GetPosition();
    int flags = 0;
    long clicked_index = m_pPathListCtrl->HitTest( pos, flags );

    //    Clicking Visibility column?
    if( clicked_index > -1 && event.GetX() < m_pPathListCtrl->GetColumnWidth( colPATHVISIBLE ) ) {
        // Process the clicked item
        ODPath *path = g_pPathList->Item( m_pPathListCtrl->GetItemData( clicked_index ) )->GetData();

        int ODPoints_set_viz = wxID_YES;
        bool togglesharedODPoints = true;
        bool has_shared_ODPoints = g_pPathMan->DoesPathContainSharedPoints( path );
        
        if( has_shared_ODPoints && path->IsVisible() ) {
            ODPoints_set_viz = OCPNMessageBox_PlugIn(  this, _("Do you also want to make the shared OD Points being part of this boundary invisible?"), _("Question"), wxYES_NO );
            togglesharedODPoints = (ODPoints_set_viz == wxID_YES);
        }
        path->SetVisible( !path->IsVisible(), togglesharedODPoints );
        m_pPathListCtrl->SetItemImage( clicked_index, path->IsVisible() ? 0 : 1 );

        ::wxBeginBusyCursor();

        g_pODConfig->UpdatePath( path );
        RequestRefresh( GetOCPNCanvasWindow() );

        //   We need to update the ODPoint list control only if the visibility of shared ODPoints might have changed.
        if( has_shared_ODPoints )
            UpdateODPointsListCtrlViz();

        ::wxEndBusyCursor();

    }

    // Allow wx to process...
    event.Skip();
}

void PathManagerDialog::OnPathBtnLeftDown( wxMouseEvent &event )
{
    m_bCtrlDown = event.ControlDown();
    event.Skip();
}

void PathManagerDialog::OnPathSelected( wxListEvent &event )
{
    long clicked_index = event.m_itemIndex;
    // Process the clicked item
    ODPath *path = g_pPathList->Item( m_pPathListCtrl->GetItemData( clicked_index ) )->GetData();
    path->m_bPathManagerBlink = true;
    
    m_pPathListCtrl->SetItemImage( clicked_index, path->IsVisible() ? 0 : 1 );

    RequestRefresh( GetOCPNCanvasWindow() );

    UpdatePathButtons();

}

void PathManagerDialog::OnPathDeSelected( wxListEvent &event )
{
    long clicked_index = event.m_itemIndex;
    // Process the clicked item
    ODPath *path = g_pPathList->Item( m_pPathListCtrl->GetItemData( clicked_index ) )->GetData();
    path->m_bPathManagerBlink = false;
    
    m_pPathListCtrl->SetItemImage( clicked_index, path->IsVisible() ? 0 : 1 );
    
    RequestRefresh( GetOCPNCanvasWindow() );
    
    UpdatePathButtons();
    
}

void PathManagerDialog::OnPathColumnClicked( wxListEvent &event )
{
    if( event.m_col == 1 ) {
        sort_path_name_dir++;
        m_pPathListCtrl->SortItems( SortPathOnName, (long) m_pPathListCtrl );
    } else
        if( event.m_col == 2 ) {
            sort_path_to_dir++;
            m_pPathListCtrl->SortItems( SortPathOnTo, (long) m_pPathListCtrl );
        }
}

void PathManagerDialog::OnPathDefaultAction( wxListEvent &event )
{
    wxCommandEvent evt;
    OnPathPropertiesClick( evt );
}

void PathManagerDialog::UpdateODPointsListCtrl( ODPoint *op_select, bool b_retain_sort )
{
    long selected_id = -1;
    long item = -1;

    if( NULL == op_select ) {
        // if an item was selected, make it selected again if it still exists
        item = m_pODPointListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );

        if( item != -1 ) selected_id = m_pODPointListCtrl->GetItemData( item );
    }

    //  Freshen the image list
    m_pODPointListCtrl->SetImageList( g_pODPointMan->Getpmarkicon_image_list(), wxIMAGE_LIST_SMALL );
    
    m_pODPointListCtrl->DeleteAllItems();

    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();

    int index = 0;
    while( node ) {
        ODPoint *op = node->GetData();
        if( op && op->IsListed() ) {
            if( op->m_bIsInPath ) {
                if( !op->m_bKeepXPath ) {
                    node = node->GetNext();
                    continue;
                }
            }

            wxListItem li;
            li.SetId( index );
            li.SetImage( op->IsVisible() ? g_pODPointMan->GetIconIndex( op->GetIconBitmap() )
                                    : g_pODPointMan->GetXIconIndex( op->GetIconBitmap() ) );
            li.SetData( op );
            li.SetText( _T("") );
            long idx = m_pODPointListCtrl->InsertItem( li );

            wxString name = op->GetName();
            if( name.IsEmpty() ) {
                name.append( _("(Unnamed) ") );
#if wxCHECK_VERSION(3,0,0)
                name.append( _(op->m_sTypeString) );
#else
                name.append( op->m_sTypeString );
#endif
            }
            m_pODPointListCtrl->SetItem( idx, colOCPNPOINTNAME, name );

            double dst;
            DistanceBearingMercator_Plugin( op->m_lat, op->m_lon, g_pfFix.Lat, g_pfFix.Lon, NULL, &dst );
            wxString dist;
            dist.Printf( _T("%5.2f ") + getUsrDistanceUnit_Plugin(), toUsrDistance_Plugin( dst ) );
            m_pODPointListCtrl->SetItem( idx, colOCPNPOINTDIST, dist );

            if( op == op_select ) selected_id = (long) op_select; //index; 

            index++;
        }

        node = node->GetNext();
    }

    if( !b_retain_sort ) {
        m_pODPointListCtrl->SortItems( SortODPointsOnName, (long) m_pODPointListCtrl );
        sort_ODPoint_key = SORT_ON_NAME;
    } else {
        switch( sort_ODPoint_key ){
            case SORT_ON_NAME:
                m_pODPointListCtrl->SortItems( SortODPointsOnName, (long) m_pODPointListCtrl );
                break;
            case SORT_ON_DISTANCE:
                m_pODPointListCtrl->SortItems( SortODPointsOnDistance, (long) m_pODPointListCtrl );
                break;
        }
    }

    if( selected_id > -1 ) {
        item = m_pODPointListCtrl->FindItem( -1, selected_id );
        m_pODPointListCtrl->SetItemState( item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
    }

    if( (m_lastODPointItem >= 0) && (m_pODPointListCtrl->GetItemCount()) )
        m_pODPointListCtrl->EnsureVisible( m_lastODPointItem );
    UpdateODPointButtons();
    
    for(int i = 0; i < m_pODPointListCtrl->GetColumnCount(); i++) {
#ifdef WIN32
        m_pODPointListCtrl->SetColumnWidth( i, wxLIST_AUTOSIZE_USEHEADER );
#else
        m_pODPointListCtrl->SetColumnWidth( i, wxLIST_AUTOSIZE );
#endif
    }
    this->GetSizer()->Fit( this );
    this->Layout();
    
}

void PathManagerDialog::UpdateODPointsListCtrlViz( )
{
    long item = -1;
    for ( ;; )
    {
        item = m_pODPointListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
        if ( item == -1 )
            break;
        
        ODPoint *pRP = (ODPoint *)m_pODPointListCtrl->GetItemData(item);
        int image = pRP->IsVisible() ? g_pODPointMan->GetIconIndex( pRP->GetIconBitmap() )
        : g_pODPointMan->GetXIconIndex( pRP->GetIconBitmap() ) ;
                        
        m_pODPointListCtrl->SetItemImage(item, image);
    }

}


void PathManagerDialog::OnODPointDefaultAction( wxListEvent &event )
{
    wxCommandEvent evt;
    OnODPointPropertiesClick( evt );
}

void PathManagerDialog::OnODPointSelected( wxListEvent &event )
{
    ODPoint *point = (ODPoint *)m_pODPointListCtrl->GetItemData( event.m_itemIndex );
    point->m_bPathManagerBlink = true;
    UpdateODPointButtons();
}

void PathManagerDialog::OnODPointDeSelected( wxListEvent &event )
{
    ODPoint *point = (ODPoint *)m_pODPointListCtrl->GetItemData( event.m_itemIndex );
    point->m_bPathManagerBlink = false;

    RequestRefresh( GetOCPNCanvasWindow() );

    UpdateODPointButtons();
}

void PathManagerDialog::OnODPointColumnClicked( wxListEvent &event )
{
    if( event.m_col == 1 ) {
        sort_ODPoint_name_dir++;
        m_pODPointListCtrl->SortItems( SortODPointsOnName, (long) m_pODPointListCtrl );
        sort_ODPoint_key = SORT_ON_NAME;
    } else
        if( event.m_col == 2 ) {
            sort_ODPoint_len_dir++;
            m_pODPointListCtrl->SortItems( SortODPointsOnDistance, (long) m_pODPointListCtrl );
            sort_ODPoint_key = SORT_ON_DISTANCE;
        }
}

void PathManagerDialog::UpdateODPointButtons()
{
    long item = -1;
    item = m_pODPointListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    bool enable1 = ( m_pODPointListCtrl->GetSelectedItemCount() == 1 );
    bool enablemultiple = ( m_pODPointListCtrl->GetSelectedItemCount() >= 1 );

    if( enable1 )
        m_lastODPointItem = item;
    else
        m_lastODPointItem = -1;

    //  Check selection to see if it is in a layer
    //  If so, disable the "delete" button
    bool b_delete_enable = true;
    item = -1;
    for ( ;; )
    {
        item = m_pODPointListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;

        ODPoint *wp = (ODPoint *) m_pODPointListCtrl->GetItemData( item );

        if( wp && wp->m_bIsInLayer) {
            b_delete_enable = false;
            break;
        }
    }


    btnODPointProperties->Enable( enable1 );
    btnODPointZoomto->Enable( enable1 );
    btnODPointDeleteAll->Enable( TRUE );
    btnODPointDelete->Enable( b_delete_enable && enablemultiple );
    btnODPointExport->Enable( enablemultiple );
    //btnODPointSendToGPS->Enable( enable1 );
}

void PathManagerDialog::OnODPointToggleVisibility( wxMouseEvent &event )
{
    wxPoint pos = event.GetPosition();
    int flags = 0;
    long clicked_index = m_pODPointListCtrl->HitTest( pos, flags );

    //    Clicking Visibility column?
    if( clicked_index > -1 && event.GetX() < m_pODPointListCtrl->GetColumnWidth( colPATHVISIBLE ) ) {
        // Process the clicked item
        ODPoint *wp = (ODPoint *) m_pODPointListCtrl->GetItemData( clicked_index );

        wp->SetVisible( !wp->IsVisible() );
        m_pODPointListCtrl->SetItemImage( clicked_index,
                                      wp->IsVisible() ? g_pODPointMan->GetIconIndex( wp->GetIconBitmap() )
                                                      : g_pODPointMan->GetXIconIndex( wp->GetIconBitmap() ) );

        g_pODConfig->UpdateODPoint( wp );

        RequestRefresh( GetOCPNCanvasWindow() );
    }

    // Allow wx to process...
    event.Skip();
}

void PathManagerDialog::OnODPointNewClick( wxCommandEvent &event )
{
    ODPoint *pWP = new ODPoint( g_dLat, g_dLon, g_sODPointIconName, wxEmptyString,
            wxT("") );
    pWP->m_bIsolatedMark = true;                      // This is an isolated mark
    pWP->SetTypeString( wxS("Boundary Point") );
    g_pODSelect->AddSelectableODPoint( g_dLat, g_dLon, pWP );
    g_pODConfig->AddNewODPoint( pWP, -1 );    // use auto next num
    RequestRefresh( GetOCPNCanvasWindow() );

    ODPointShowPropertiesDialog( pWP, GetParent() );
}

void PathManagerDialog::OnODPointPropertiesClick( wxCommandEvent &event )
{
    long item = -1;
    item = m_pODPointListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    ODPoint *wp = ( ODPoint *) m_pODPointListCtrl->GetItemData( item );

    if( !wp ) return;

    ODPointShowPropertiesDialog( wp, g_ocpn_draw_pi->m_parent_window );

    UpdateODPointsListCtrl();
    m_bNeedConfigFlush = true;
}

void PathManagerDialog::ODPointShowPropertiesDialog( ODPoint* wp, wxWindow* parent )
{
    if( NULL == g_pODPointPropDialog )
        g_pODPointPropDialog = new ODPointPropertiesImpl( parent );

    DimeWindow( g_pODPointPropDialog );
    g_pODPointPropDialog->SetODPoint( wp );
    g_pODPointPropDialog->UpdateProperties();
    
    if( !g_pODPointPropDialog->IsShown() )
        g_pODPointPropDialog->Show();

}

void PathManagerDialog::OnODPointZoomtoClick( wxCommandEvent &event )
{
    long item = -1;
    item = m_pODPointListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    ODPoint *wp = (ODPoint *) m_pODPointListCtrl->GetItemData( item );

    if( !wp ) return;

    JumpToPosition( wp->m_lat, wp->m_lon, g_ocpn_draw_pi->m_view_scale );
    
}

void PathManagerDialog::OnODPointDeleteClick( wxCommandEvent &event )
{
    ODPointList list;

    int answer = OCPNMessageBox_PlugIn( this, _("Are you sure you want to delete the selected object(s)"), wxString( _("OpenCPN Alert") ), wxYES_NO );
    if ( answer != wxID_YES )
        return;

    bool busy = false;
    if( m_pODPointListCtrl->GetSelectedItemCount() ) {
        ::wxBeginBusyCursor();
        m_bNeedConfigFlush = true;
        busy = true;
    }

    long item = -1;
    long item_last_selected = -1;
    for ( ;; )
    {
        item = m_pODPointListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;

        item_last_selected = item;
        ODPoint *wp = (ODPoint *) m_pODPointListCtrl->GetItemData( item );

        if( wp && !wp->m_bIsInLayer)
            list.Append( wp );
    }

    if( busy ) {
        for(unsigned int i=0 ; i < list.GetCount() ; i++) {
            ODPoint *wp = list.Item(i)->GetData();
            if( wp ) {

                if ( wp->m_bIsInPath )
                {
                    if ( wxYES == OCPNMessageBox_PlugIn(this,  _( "The OD Point you want to delete is used in a path, do you really want to delete it?" ), _( "OpenCPN Alert" ), wxYES_NO ))
                            g_pODPointMan->DestroyODPoint( wp );
                }
                else
                    g_pODPointMan->DestroyODPoint( wp );

            }
        }

        long item_next = m_pODPointListCtrl->GetNextItem( item_last_selected );         // next in list
        ODPoint *wp_next = NULL;
        if( item_next > -1 )
            wp_next = (ODPoint *) m_pODPointListCtrl->GetItemData( item_next );

        m_lastODPointItem = item_next;

        UpdatePathListCtrl();
        UpdateODPointsListCtrl( wp_next, true );

        // TODO fix up undo
        //ocpncc1->undo->InvalidateUndo();
        RequestRefresh( GetOCPNCanvasWindow() );
        ::wxEndBusyCursor();
    }

}

void PathManagerDialog::OnODPointExportClick( wxCommandEvent &event )
{
    ODPointList list;

    wxString suggested_name = _T("OD Points");

    long item = -1;
    for ( ;; )
    {
        item = m_pODPointListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;

        ODPoint *wp = (ODPoint *) m_pODPointListCtrl->GetItemData( item );

        if( wp && !wp->m_bIsInLayer) {
            list.Append( wp );
            if( wp->GetName() != wxEmptyString )
                suggested_name = wp->GetName();
        }
    }

    g_pODConfig->ExportGPXODPoints( this, &list, suggested_name );
}

void PathManagerDialog::OnODPointDeleteAllClick( wxCommandEvent &event )
{
    wxString prompt;
    int buttons, type;
    if ( !g_pODPointMan->SharedODPointsExist() )
    {
        prompt = _("Are you sure you want to delete <ALL> OD points?");
        buttons = wxYES_NO;
        type = 1;
    }
    else
    {
        prompt = _("There are some OD points used in paths or anchor alarms.\n Do you want to delete them as well?\n This will change the paths and disable the anchor alarms.\n Answering No keeps the OD points used in paths or alarms.");
        buttons = wxYES_NO | wxCANCEL;
        type = 2;
    }
    int answer = OCPNMessageBox_PlugIn( this, prompt, _("OpenCPN Alert"), buttons );
    if ( answer == wxID_YES )
        g_pODPointMan->DeleteAllODPoints( true );
    if ( answer == wxID_NO && type == 2 )
        g_pODPointMan->DeleteAllODPoints( false );          // only delete unused OD points

    m_lastODPointItem = -1;
    UpdatePathListCtrl();
    UpdateODPointsListCtrl();
    // TODO fix up undo
    //ocpncc1->undo->InvalidateUndo();
    RequestRefresh( GetOCPNCanvasWindow() );
}

void PathManagerDialog::OnLaySelected( wxListEvent &event )
{
    UpdateLayButtons();
}

void PathManagerDialog::OnLayColumnClicked( wxListEvent &event )
{
    if( event.m_col == 1 ) {
        sort_layer_name_dir++;
        m_pLayListCtrl->SortItems( SortLayersOnName, (long) m_pLayListCtrl );
    } else
        if( event.m_col == 2 ) {
            sort_layer_len_dir++;
            m_pLayListCtrl->SortItems( SortLayersOnSize, (long) m_pLayListCtrl );
        }
}

void PathManagerDialog::UpdateLayButtons()
{
    long item = -1;
    item = m_pLayListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    bool enable = ( item != -1 );

    //btnLayProperties->Enable(false);
    btnLayDelete->Enable( enable );
    btnLayToggleChart->Enable( enable );
    btnLayToggleListing->Enable( enable );
    btnLayToggleNames->Enable( enable );

    if( item >= 0 ) {
        if( pLayerList->Item( m_pLayListCtrl->GetItemData( item ) )->GetData()->IsVisibleOnChart() ) btnLayToggleChart->SetLabel(
                _("Hide from chart") );
        else
            btnLayToggleChart->SetLabel( _("Show on chart") );

        if( pLayerList->Item( m_pLayListCtrl->GetItemData( item ) )->GetData()->HasVisibleNames() ) btnLayToggleNames->SetLabel(
                _("Hide Point names") );
        else
            btnLayToggleNames->SetLabel( _("Show Point names") );

        if( pLayerList->Item( m_pLayListCtrl->GetItemData( item ) )->GetData()->IsVisibleOnListing() ) btnLayToggleListing->SetLabel(
                _("Unlist contents ") );
        else
            btnLayToggleListing->SetLabel( _("List contents ") );
    } else {
        btnLayToggleChart->SetLabel( _("Show on chart") );
        btnLayToggleNames->SetLabel( _("Show Point names") );
        btnLayToggleListing->SetLabel( _("List contents ") );
    }
}

void PathManagerDialog::OnLayToggleVisibility( wxMouseEvent &event )
{
    wxPoint pos = event.GetPosition();
    int flags = 0;
    long clicked_index = m_pLayListCtrl->HitTest( pos, flags );

    //    Clicking Visibility column?
    if( clicked_index > -1 && event.GetX() < m_pLayListCtrl->GetColumnWidth( colLAYVISIBLE ) ) {
        // Process the clicked item
        Layer *layer = pLayerList->Item( m_pLayListCtrl->GetItemData( clicked_index ) )->GetData();

        layer->SetVisibleOnChart( !layer->IsVisibleOnChart() );
        m_pLayListCtrl->SetItemImage( clicked_index, layer->IsVisibleOnChart() ? 0 : 1 );

        ToggleLayerContentsOnChart( layer );
    }

    // Allow wx to process...
    event.Skip();
}

void PathManagerDialog::OnLayNewClick( wxCommandEvent &event )
{
    bool show_flag = g_bShowLayers;
    g_bShowLayers = true;
    
#ifdef __WXOSX__
    HideWithEffect(wxSHOW_EFFECT_BLEND );
#endif
    
    g_pODConfig->UI_ImportGPX( this, true, _T("") );
    
#ifdef __WXOSX__
    ShowWithEffect(wxSHOW_EFFECT_BLEND );
#endif
    
    
    g_bShowLayers = show_flag;

    UpdatePathListCtrl();
    UpdateODPointsListCtrl();
    UpdateLayListCtrl();
    RequestRefresh( GetOCPNCanvasWindow() );
}

void PathManagerDialog::OnLayPropertiesClick( wxCommandEvent &event )
{
    // Show layer properties dialog for selected layer - todo
    long item = -1;
    item = m_pLayListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;
}

void PathManagerDialog::OnLayDeleteClick( wxCommandEvent &event )
{
    long item = -1;
    item = m_pLayListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    Layer *layer = pLayerList->Item( m_pLayListCtrl->GetItemData( item ) )->GetData();

    if( !layer ) return;

    wxString prompt = _("Are you sure you want to delete this layer and <ALL> of its contents?");
    int answer = OCPNMessageBox_PlugIn( this, prompt, _("OpenCPN Alert"), wxYES_NO );
    if ( answer == wxID_NO )
        return;
    
    // Process Tracks and Routes in this layer
    wxPathListNode *node1 = g_pPathList->GetFirst();
    wxPathListNode *node2;
    while( node1 ) {
        ODPath *pPath = node1->GetData();
        node2 = node1->GetNext();
        if( pPath->m_bIsInLayer && ( pPath->m_LayerID == layer->m_LayerID ) ) {
            pPath->m_bIsInLayer = false;
            pPath->m_LayerID = 0;
            g_pPathMan->DeletePath( pPath );
        }
        node1 = node2;
        node2 = NULL;
    }

    // Process ODPoints in this layer
    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();
    wxODPointListNode *node3;

    while( node ) {
        node3 = node->GetNext();
        ODPoint *rp = node->GetData();
        if( rp && ( rp->m_LayerID == layer->m_LayerID ) ) {
            rp->m_bIsInLayer = false;
            rp->m_LayerID = 0;
            g_pODPointMan->DestroyODPoint( rp, false );         // no need to update the change set on layer ops
        }

        node = node3;
        node3 = NULL;
    }

    pLayerList->DeleteObject( layer );

    UpdatePathListCtrl();
    UpdateODPointsListCtrl();
    UpdateLayListCtrl();

    RequestRefresh( GetOCPNCanvasWindow() );

    m_bNeedConfigFlush = false;
}

void PathManagerDialog::OnLayToggleChartClick( wxCommandEvent &event )
{
    // Toggle  visibility on chart for selected layer
    long item = -1;
    item = m_pLayListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    Layer *layer = pLayerList->Item( m_pLayListCtrl->GetItemData( item ) )->GetData();

    if( !layer ) return;

    layer->SetVisibleOnChart( !layer->IsVisibleOnChart() );
    m_pLayListCtrl->SetItemImage( item, layer->IsVisibleOnChart() ? 0 : 1 );

    ToggleLayerContentsOnChart( layer );
}

void PathManagerDialog::ToggleLayerContentsOnChart( Layer *layer )
{
    // Process Paths in this layer
    wxPathListNode *node1 = g_pPathList->GetFirst();
    while( node1 ) {
        ODPath *pPath = node1->GetData();
        if( pPath->m_bIsInLayer && ( pPath->m_LayerID == layer->m_LayerID ) ) {
            pPath->SetVisible( layer->IsVisibleOnChart() );
            g_pODConfig->UpdatePath( pPath );
        }
        node1 = node1->GetNext();
    }

    // Process OD points in this layer
    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();

    while( node ) {
        ODPoint *rp = node->GetData();
        if( rp && ( rp->m_LayerID == layer->m_LayerID ) ) {
            rp->SetVisible( layer->IsVisibleOnChart() );
        }

        node = node->GetNext();
    }

    UpdatePathListCtrl();
    UpdateODPointsListCtrl();
    UpdateLayListCtrl();
    UpdateLayButtons();

    RequestRefresh( GetOCPNCanvasWindow() );
}

void PathManagerDialog::OnLayToggleNamesClick( wxCommandEvent &event )
{
    // Toggle Point names visibility on chart for selected layer
    long item = -1;
    item = m_pLayListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    Layer *layer = pLayerList->Item( m_pLayListCtrl->GetItemData( item ) )->GetData();

    if( !layer ) return;

    layer->SetVisibleNames( !layer->HasVisibleNames() );

    ToggleLayerContentsNames( layer );
}

void PathManagerDialog::ToggleLayerContentsNames( Layer *layer )
{
    // Process Paths in this layer
    wxPathListNode *node1 = g_pPathList->GetFirst();
    while( node1 ) {
        ODPath *pPath = node1->GetData();
        if( pPath->m_bIsInLayer && ( pPath->m_LayerID == layer->m_LayerID ) ) {
            wxODPointListNode *node = pPath->m_pODPointList->GetFirst();
            ODPoint *prp1 = node->GetData();
            while( node ) {
                prp1->m_bShowName = layer->HasVisibleNames();
                node = node->GetNext();
            }
        }
        node1 = node1->GetNext();
    }

    // Process OD points in this layer
    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();

    while( node ) {
        ODPoint *rp = node->GetData();
        if( rp && ( rp->m_LayerID == layer->m_LayerID ) ) {
            rp->SetNameShown( layer->HasVisibleNames() );
        }

        node = node->GetNext();
    }

    UpdateLayButtons();

    RequestRefresh( GetOCPNCanvasWindow() );
}

void PathManagerDialog::OnLayToggleListingClick( wxCommandEvent &event )
{
    // Toggle  visibility on listing for selected layer
    long item = -1;
    item = m_pLayListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    Layer *layer = pLayerList->Item( m_pLayListCtrl->GetItemData( item ) )->GetData();

    if( !layer ) return;

    layer->SetVisibleOnListing( !layer->IsVisibleOnListing() );

    ToggleLayerContentsOnListing( layer );
}

void PathManagerDialog::ToggleLayerContentsOnListing( Layer *layer )
{
    ::wxBeginBusyCursor();

    // Process Paths in this layer
    wxPathListNode *node1 = g_pPathList->GetFirst();
    while( node1 ) {
        ODPath *pPath = node1->GetData();
        if( pPath->m_bIsInLayer && ( pPath->m_LayerID == layer->m_LayerID ) ) {
            pPath->SetListed( layer->IsVisibleOnListing() );
            g_pODConfig->UpdatePath(pPath);
        }
        node1 = node1->GetNext();
    }

    // Process OD points in this layer
    //  n.b.  If the OD point belongs to a track, and is not shared, then do not list it.
    //  This is a performance optimization, allowing large track support.

    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();

    while( node ) {
        ODPoint *rp = node->GetData();
        if( rp && !rp->m_bIsInTrack && rp->m_bIsolatedMark && ( rp->m_LayerID == layer->m_LayerID ) ) {
            rp->SetListed( layer->IsVisibleOnListing() );
        }

        node = node->GetNext();
    }

    UpdatePathListCtrl();
    UpdateODPointsListCtrl();
    UpdateLayListCtrl();

    ::wxEndBusyCursor();

    RequestRefresh( GetOCPNCanvasWindow() );
}

void PathManagerDialog::OnLayDefaultAction( wxListEvent &event )
{
    wxCommandEvent evt;
    OnLayPropertiesClick( evt );
}

void PathManagerDialog::UpdateLayListCtrl()
{
    // if an item was selected, make it selected again if it still exist
    long item = -1;
    item = m_pLayListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );

    long selected_id = -1;
    if( item != -1 ) selected_id = m_pLayListCtrl->GetItemData( item );

    // Delete existing items
    m_pLayListCtrl->DeleteAllItems();

    // then add routes to the listctrl
    LayerList::iterator it;
    int index = 0;
    for( it = ( *pLayerList ).begin(); it != ( *pLayerList ).end(); ++it, ++index ) {
        Layer *lay = (Layer *) ( *it );

        wxListItem li;
        li.SetId( index );
        li.SetImage( lay->IsVisibleOnChart() ? 0 : 1 );
        li.SetData( index );
        li.SetText( _T("") );

        long idx = m_pLayListCtrl->InsertItem( li );

        wxString name = lay->m_LayerName;
        if( name.IsEmpty() ) {
            //RoutePoint *rp = trk->GetPoint(1);
            //if (rp)
            //      name = rp->m_CreateTime.FormatISODate() + _T(" ") + rp->m_CreateTime.FormatISOTime();   //name = rp->m_CreateTime.Format();
            //else
            name = _("(Unnamed Layer)");
        }
        m_pLayListCtrl->SetItem( idx, colLAYNAME, name );

        wxString len;
        len.Printf( wxT("%d"), (int) lay->m_NoOfItems );
        m_pLayListCtrl->SetItem( idx, colLAYITEMS, len );
    }

    m_pLayListCtrl->SortItems( SortLayersOnName, (long) m_pLayListCtrl );

    // restore selection if possible
    // NOTE this will select a different item, if one is deleted
    // (the next route will get that index).
    if( selected_id > -1 ) {
        item = m_pLayListCtrl->FindItem( -1, selected_id );
        m_pLayListCtrl->SetItemState( item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
    }
    UpdateLayButtons();

    for(int i = 0; i < m_pLayListCtrl->GetColumnCount(); i++) {
#ifdef WIN32
        m_pLayListCtrl->SetColumnWidth( i, wxLIST_AUTOSIZE_USEHEADER );
#else
        m_pLayListCtrl->SetColumnWidth( i, wxLIST_AUTOSIZE );
#endif
    }
}

void PathManagerDialog::OnImportClick( wxCommandEvent &event )
{
#ifdef __WXOSX__
    HideWithEffect(wxSHOW_EFFECT_BLEND );
#endif
    
    g_pODConfig->UI_ImportGPX( this );

#ifdef __WXOSX__
    ShowWithEffect(wxSHOW_EFFECT_BLEND );
#endif
    
    UpdatePathListCtrl();
    UpdateODPointsListCtrl();
    UpdateLayListCtrl();

    RequestRefresh( GetOCPNCanvasWindow() );
}
void PathManagerDialog::OnExportClick( wxCommandEvent &event )
{
    g_pODConfig->ExportGPX( this );
}

void PathManagerDialog::OnExportVizClick( wxCommandEvent &event )
{
    g_pODConfig->ExportGPX( this, true, true );     // only visible objects, layers included
}

wxString PathManagerDialog::GetLayerName( int id )
{
    wxString name( _T("unknown layer") );
    if( id <= 0 ) return ( name );
    LayerList::iterator it;
    int index = 0;
    for( it = ( *pLayerList ).begin(); it != ( *pLayerList ).end(); ++it, ++index ) {
        Layer *lay = (Layer *) ( *it );
        if( lay->m_LayerID == id ) return ( lay->m_LayerName );
    }
    return ( name );
}

void PathManagerDialog::OnClose(wxCloseEvent& event)
{
    DeSelectODPoints();
    DeSelectPaths();
}

void PathManagerDialog::OnOK(wxCommandEvent &event)
{
    DeSelectODPoints();
    DeSelectPaths();
}

void PathManagerDialog::DeSelectPaths( void )
{
    long selected_item = -1;
    
    for( ;; )
    {
        selected_item = m_pPathListCtrl->GetNextItem( selected_item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
        if( selected_item == -1 ) break;
        //ODPath *path = g_pPathList->Item( m_pPathListCtrl->GetItemData( selected_item ) )->GetData();
        m_pPathListCtrl->SetItemState( selected_item, 0, wxLIST_STATE_SELECTED );
    }
    Show( false );
}

void PathManagerDialog::DeSelectODPoints( void )
{
    long selected_item = -1;
    
    for( ;; )
    {
        selected_item = m_pODPointListCtrl->GetNextItem( selected_item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
        if( selected_item == -1 ) break;
        //ODPoint *point = (ODPoint *)m_pODPointListCtrl->GetItemData( selected_item );
        m_pODPointListCtrl->SetItemState( selected_item, 0, wxLIST_STATE_SELECTED );
    }
    Show( false );
}

//END Event handlers
