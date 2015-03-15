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

#include "pathmanagerdialog.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/progdlg.h>
#include <wx/clipbrd.h>

#include <iostream>

#include "styles.h"
#include "dychart.h"
#include "navutil.h"
#include "OCPNDrawConfig.h"
#include "Path.h"
#include "PathProp.h"
#include "OCPNDrawPointInfoImpl.h"
#include "Boundary.h"
#include "BoundaryProp.h"
#include "PathMan.h"
#include "PointMan.h"
#include "OCPNPoint.h"
#include "OCPNSelect.h"
#include "georef.h"
#include "chartbase.h"
#include "Layer.h"
#include "SendToGpsDlg.h"
#include "TrackPropDlg.h"
#include "AIS_Decoder.h"

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
extern PathList *pPathList;
extern BoundaryList *pBoundaryList;
extern LayerList *pLayerList;
extern wxString GetLayerName(int id);
extern PathProp *pPathPropDialog;
extern PathMan  *g_pPathMan;
extern OCPNPointList    *pOCPNPointList;
extern OCPNDrawConfig  *pConfig;
extern ChartCanvas *cc1;
extern ChartBase *Current_Ch;
extern PointMan      *pOCPNPointMan;
extern OCPNDrawPointInfoImpl *pOCPNPointPropDialog;
//extern MarkInfoImpl     *pMarkPropDialog;
extern MyFrame          *gFrame;
extern OCPNSelect           *pSelect;
extern double           gLat, gLon;
extern double           gCog, gSog;
extern bool             g_bShowLayers;
extern wxString         g_default_OCPNPoint_icon;

extern AIS_Decoder      *g_pAIS;

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

int sort_OCPNPoint_key;

// sort callback. Sort by wpt name.
int sort_OCPNPoint_name_dir;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortOCPNPointsOnName(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortOCPNPointsOnName(long item1, long item2, long list)
#endif

{
    wxListCtrl *lc = (wxListCtrl*)list;

    OCPNPoint *pRP1 = (OCPNPoint *)item1;
    OCPNPoint *pRP2 = (OCPNPoint *)item2;

    if(pRP1 && pRP2) {
        if(sort_OCPNPoint_name_dir & 1)
            return pRP2->GetName().CmpNoCase(pRP1->GetName());
        else
            return pRP1->GetName().CmpNoCase(pRP2->GetName());
    }
    else
        return 0;
    
}

// sort callback. Sort by wpt distance.
int sort_OCPNPoint_len_dir;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortOCPNPointsOnDistance(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortOCPNPointsOnDistance(long item1, long item2, long list)
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

    if(sort_OCPNPoint_len_dir & 1)
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
END_EVENT_TABLE()

void PathManagerDialog::OnTabSwitch( wxNotebookEvent &event )
{
    if( !m_pNotebook ) return;
    int current_page = m_pNotebook->GetSelection();
    if( current_page == 4 ) {
//        if( btnImport ) btnImport->Enable( false );
//        if( btnExport ) btnExport->Enable( false );
//        if( btnExportViz ) btnExportViz->Enable( false );
    } else {
        if( btnImport ) btnImport->Enable( true );
        if( btnExport ) btnExport->Enable( true );
        if( btnExportViz ) btnExportViz->Enable( true );

    }
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
    
    wxFont *qFont = GetOCPNScaledFont(_("Dialog"));
    SetFont( *qFont );

    m_lastOCPNPointItem = -1;
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
            wxListEventHandler(PathManagerDialog::OnPathSelected), NULL, this );
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

    //  Create "Boundary" panel
/*    m_pBoundaryPath = new wxPanel( m_pNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize,
            wxNO_BORDER | wxTAB_TRAVERSAL );
    wxBoxSizer *sbsBoundaries = new wxBoxSizer( wxHORIZONTAL );
    m_pPanelBoundary->SetSizer( sbsBoundaries );
    m_pNotebook->AddPage( m_pPanelBoundary, _("Boundaries") );

    m_pBoundaryListCtrl = new wxListCtrl( m_pPanelBoundary, -1, wxDefaultPosition, wxSize( 400, -1 ),
            wxLC_REPORT  | wxLC_SORT_ASCENDING | wxLC_HRULES
                    | wxBORDER_SUNKEN);
    m_pBoundaryListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED,
            wxListEventHandler(PathManagerDialog::OnBoundarySelected), NULL, this );
    m_pBoundaryListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED,
            wxListEventHandler(PathManagerDialog::OnBoundarySelected), NULL, this );
    m_pBoundaryListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            wxListEventHandler(PathManagerDialog::OnBoundaryDefaultAction), NULL, this );
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
*/
    //  Create "OCPN points" panel
    m_pPanelOCPNPoint = new wxPanel( m_pNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize,
            wxNO_BORDER | wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer( wxHORIZONTAL );
    m_pPanelOCPNPoint->SetSizer( itemBoxSizer4 );
    m_pNotebook->AddPage( m_pPanelOCPNPoint, _("OCPN Points") );

    m_pOCPNPointListCtrl = new wxListCtrl( m_pPanelOCPNPoint, -1, wxDefaultPosition, wxSize( 400, -1 ),
            wxLC_REPORT | wxLC_SORT_ASCENDING | wxLC_HRULES | wxBORDER_SUNKEN/*|wxLC_VRULES*/);
    m_pOCPNPointListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED,
            wxListEventHandler(PathManagerDialog::OnOCPNPointSelected), NULL, this );
    m_pOCPNPointListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_DESELECTED,
            wxListEventHandler(PathManagerDialog::OnOCPNPointSelected), NULL, this );
    m_pOCPNPointListCtrl->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
            wxListEventHandler(PathManagerDialog::OnOCPNPointDefaultAction), NULL, this );
    m_pOCPNPointListCtrl->Connect( wxEVT_LEFT_DOWN,
            wxMouseEventHandler(PathManagerDialog::OnOCPNPointToggleVisibility), NULL, this );
    m_pOCPNPointListCtrl->Connect( wxEVT_COMMAND_LIST_COL_CLICK,
            wxListEventHandler(PathManagerDialog::OnOCPNPointColumnClicked), NULL, this );
    itemBoxSizer4->Add( m_pOCPNPointListCtrl, 1, wxEXPAND | wxALL, DIALOG_MARGIN );

    m_pOCPNPointListCtrl->InsertColumn( colOCPNPOINTICON, _("Icon"), wxLIST_FORMAT_LEFT, 44 );
    m_pOCPNPointListCtrl->InsertColumn( colOCPNPOINTNAME, _("OCPN Point Name"), wxLIST_FORMAT_LEFT, 180 );
    m_pOCPNPointListCtrl->InsertColumn( colOCPNPOINTDIST, _("Distance from Ownship"), wxLIST_FORMAT_LEFT, 180 );

    wxBoxSizer *bsOCPNPointButtons = new wxBoxSizer( wxVERTICAL );
    itemBoxSizer4->Add( bsOCPNPointButtons, 0, wxALIGN_RIGHT );

    btnOCPNPointNew = new wxButton( m_pPanelOCPNPoint, -1, _("&New") );
    bsOCPNPointButtons->Add( btnOCPNPointNew, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnOCPNPointNew->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnOCPNPointNewClick), NULL, this );

    btnOCPNPointProperties = new wxButton( m_pPanelOCPNPoint, -1, _("&Properties") );
    bsOCPNPointButtons->Add( btnOCPNPointProperties, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnOCPNPointProperties->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnOCPNPointPropertiesClick), NULL, this );

    btnOCPNPointZoomto = new wxButton( m_pPanelOCPNPoint, -1, _("&Center View") );
    bsOCPNPointButtons->Add( btnOCPNPointZoomto, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnOCPNPointZoomto->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnOCPNPointZoomtoClick), NULL, this );

    btnOCPNPointDelete = new wxButton( m_pPanelOCPNPoint, -1, _("&Delete") );
    bsOCPNPointButtons->Add( btnOCPNPointDelete, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnOCPNPointDelete->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnOCPNPointDeleteClick), NULL, this );

    btnOCPNPointGoTo = new wxButton( m_pPanelOCPNPoint, -1, _("&Go To") );
    bsOCPNPointButtons->Add( btnOCPNPointGoTo, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
//    btnOCPNPointGoTo->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
//            wxCommandEventHandler(PathManagerDialog::OnOCPNPointGoToClick), NULL, this );

    btnOCPNPointExport = new wxButton( m_pPanelOCPNPoint, -1, _("&Export selected...") );
    bsOCPNPointButtons->Add( btnOCPNPointExport, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnOCPNPointExport->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnOCPNPointExportClick), NULL, this );

/*    btnOCPNPointSendToGPS = new wxButton( m_pPanelOCPNPoint, -1, _("&Send to GPS") );
    bsOCPNPointButtons->Add( btnOCPNPointSendToGPS, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnOCPNPointSendToGPS->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnOCPNPointSendToGPSClick), NULL, this );
*/
    bsOCPNPointButtons->AddSpacer( 10 );

    btnOCPNPointDeleteAll = new wxButton( m_pPanelOCPNPoint, -1, _("Delete All") );
    bsOCPNPointButtons->Add( btnOCPNPointDeleteAll, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnOCPNPointDeleteAll->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnOCPNPointDeleteAllClick), NULL, this );

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

    m_pLayListCtrl->InsertColumn( colLAYVISIBLE, _T(""), wxLIST_FORMAT_LEFT, 28 );
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

    btnLayToggleNames = new wxButton( m_pPanelLay, -1, _("Show WPT names") );
    bsLayButtons->Add( btnLayToggleNames, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnLayToggleNames->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnLayToggleNamesClick), NULL, this );

    btnLayToggleListing = new wxButton( m_pPanelLay, -1, _("List contents") );
    bsLayButtons->Add( btnLayToggleListing, 0, wxALL | wxEXPAND, DIALOG_MARGIN );
    btnLayToggleListing->Connect( wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(PathManagerDialog::OnLayToggleListingClick), NULL, this );

    // Dialog buttons
    wxSizer *szButtons = CreateButtonSizer( wxOK );

    itemBoxSizer5->Add( szButtons, 0, wxALL | wxALIGN_RIGHT, DIALOG_MARGIN );

    Fit();

    SetMinSize( GetBestSize() );
    
    Centre();

    // create a image list for the list with just the eye icon
    wxImageList *imglist = new wxImageList( 20, 20, true, 1 );
    imglist->Add( wxBitmap( eye ) );
    imglist->Add( wxBitmap( eyex ) );
    m_pPathListCtrl->AssignImageList( imglist, wxIMAGE_LIST_SMALL );
    // Assign will handle destroy, Set will not. It's OK, that's what we want
    m_pOCPNPointListCtrl->SetImageList( pOCPNPointMan->Getpmarkicon_image_list(), wxIMAGE_LIST_SMALL );
//    m_pLayListCtrl->SetImageList( imglist, wxIMAGE_LIST_SMALL );

    SetColorScheme();

    UpdatePathListCtrl();
    UpdateOCPNPointsListCtrl();
    UpdateLayListCtrl();

    // This should work under Linux :-(
    //m_pNotebook->Connect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler(PathManagerDialog::OnTabSwitch), NULL, this);

    m_bNeedConfigFlush = false;
}

PathManagerDialog::~PathManagerDialog()
{
    delete m_pPathListCtrl;
    delete m_pOCPNPointListCtrl;

    delete btnOCPNPointNew;
    delete btnOCPNPointProperties;
    delete btnOCPNPointZoomto;
    delete btnOCPNPointDelete;
    delete btnOCPNPointGoTo;
    delete btnOCPNPointExport;
//    delete btnOCPNPointSendToGPS;
    delete btnOCPNPointDeleteAll;
    delete btnImport;
    delete btnExport;
    delete btnExportViz;
    btnImport = NULL;
    btnExport = NULL;
    btnExportViz = NULL;

    delete m_pNotebook;

    //    Does not need to be done here at all, since this dialog is autommatically deleted as a child of the frame.
    //    By that time, the config has already been updated for shutdown.

    // Do this just once!!
//      if (m_bNeedConfigFlush)
//            pConfig->UpdateSettings();
}

void PathManagerDialog::SetColorScheme()
{
    DimeControl( this );
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
    for( it = ( *pPathList ).begin(); it != ( *pPathList ).end(); ++it, ++index ) {
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
            name.append( ( *it )->m_sTypeString );
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
}

void PathManagerDialog::UpdatePathButtons()
{
    // enable/disable buttons
    long selected_index_index = m_pPathListCtrl->GetNextItem( -1, wxLIST_NEXT_ALL,
            wxLIST_STATE_SELECTED );
    bool enable1 =  m_pPathListCtrl->GetSelectedItemCount() == 1;
    bool enablemultiple =  m_pPathListCtrl->GetSelectedItemCount() >= 1;

    m_lastPathItem = selected_index_index;

    btnPathDelete->Enable( m_pPathListCtrl->GetSelectedItemCount() > 0 );
    btnPathZoomto->Enable( enable1 ); 
    btnPathProperties->Enable( enable1 );
    btnPathDeleteAll->Enable( enablemultiple );

    // set activate button text
    Path *path = NULL;
    if( enable1 ) {
        path = pPathList->Item( m_pPathListCtrl->GetItemData( selected_index_index ) )->GetData();
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
    for( it = ( *pPathList ).begin(); it != ( *pPathList ).end(); ++it, ++index ) {
        if( ( *it )->IsVisible() ) { // avoid config updating as much as possible!
            ( *it )->SetVisible( false );
            m_pPathListCtrl->SetItemImage( m_pPathListCtrl->FindItem( -1, index ), 1 ); // Likely not same order :0
            pConfig->UpdatePath( *it ); // auch, flushes config to disk. FIXME
        }
    }
}

void PathManagerDialog::ZoomtoPath( Path *path )
{

    // Calculate bbox center
    wxBoundingBox RBBox = path->GetBBox();
    double clat = RBBox.GetMinY() + ( RBBox.GetHeight() / 2 );
    double clon = RBBox.GetMinX() + ( RBBox.GetWidth() / 2 );

    if( clon > 180. ) clon -= 360.;
    else
        if( clon < -180. ) clon += 360.;

    // Calculate ppm
    double rw, rh, ppm; // route width, height, final ppm scale to use
    int ww, wh; // chart window width, height
    // route bbox width in nm
    DistanceBearingMercator( RBBox.GetMinY(), RBBox.GetMinX(), RBBox.GetMinY(),
            RBBox.GetMaxX(), NULL, &rw );
    // route bbox height in nm
    DistanceBearingMercator( RBBox.GetMinY(), RBBox.GetMinX(), RBBox.GetMaxY(),
            RBBox.GetMinX(), NULL, &rh );

    cc1->GetSize( &ww, &wh );

    ppm = wxMin(ww/(rw*1852), wh/(rh*1852)) * ( 100 - fabs( clat ) ) / 90;

    ppm = wxMin(ppm, 1.0);

//      cc1->ClearbFollow();
//      cc1->SetViewPoint(clat, clon, ppm, 0, cc1->GetVPRotation(), CURRENT_RENDER);
//      cc1->Refresh();

    gFrame->JumpToPosition( clat, clon, ppm );

    m_bNeedConfigFlush = true;
}

//BEGIN Event handlers
void PathManagerDialog::OnPathDeleteClick( wxCommandEvent &event )
{
    PathList list;

    int answer = OCPNMessageBox( this, _("Are you sure you want to delete the selected object(s)"), wxString( _("OpenCPN Alert") ), wxYES_NO );
    if ( answer != wxID_YES )
        return;

    bool busy = false;
    if( m_pPathListCtrl->GetSelectedItemCount() ) {
        ::wxBeginBusyCursor();
//        cc1->CancelMousePath();
        m_bNeedConfigFlush = true;
        busy = true;
    }

    long item = -1;
    for ( ;; )
    {
        item = m_pPathListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;

        Path *ppath_to_delete = pPathList->Item( m_pPathListCtrl->GetItemData( item ) )->GetData();

        if( ppath_to_delete )
            list.Append( ppath_to_delete );
    }

    if( busy ) {

        for(unsigned int i=0 ; i < list.GetCount() ; i++) {
            Path *path = list.Item(i)->GetData();
            if( path ) {
                pConfig->DeleteConfigPath( path );
                g_pPathMan->DeletePath( path );
            }
        }

        m_lastPathItem = -1;
        UpdatePathListCtrl();

        cc1->undo->InvalidateUndo();
        cc1->Refresh();
        ::wxEndBusyCursor();
    }

}

void PathManagerDialog::OnPathDeleteAllClick( wxCommandEvent &event )
{
    int dialog_ret = OCPNMessageBox( this, _("Are you sure you want to delete <ALL> paths?"),
            wxString( _("OpenCPN Alert") ), wxYES_NO );

    if( dialog_ret == wxID_YES ) {

//        cc1->CancelMousePath();

        g_pPathMan->DeleteAllPaths();
// TODO Seth
//            m_pSelectedRoute = NULL;
//            m_pFoundRoutePoint = NULL;
//            m_pFoundRoutePointSecond = NULL;

        m_lastPathItem = -1;
        UpdatePathListCtrl();

        if( pPathPropDialog ) pPathPropDialog->Hide();
        cc1->undo->InvalidateUndo();
        cc1->Refresh();

        m_bNeedConfigFlush = true;
    }
}

void PathManagerDialog::OnPathPropertiesClick( wxCommandEvent &event )
{
    // Show path properties dialog for selected path
    long item = -1;
    item = m_pPathListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    Path *path = pPathList->Item( m_pPathListCtrl->GetItemData( item ) )->GetData();

    if( !path ) return;

    ShowPathPropertiesDialog ( path );
/*    if( NULL == pPathPropDialog )          // There is one global instance of the PathProp Dialog
        pPathPropDialog = new PathProp( GetParent() );

    pPathPropDialog->SetPathAndUpdate( path );
    pPathPropDialog->UpdateProperties( path );
    if( !path->m_bIsInLayer ) {
        if ( path->m_sTypeString.IsNull() || path->m_sTypeString.IsEmpty() )
            pPathPropDialog->SetDialogTitle( wxS("Path Properties") );
        else {
            wxString sTitle( path->m_sTypeString );
            sTitle.append( wxS(" Properties") );
            pPathPropDialog->SetDialogTitle( sTitle );
        }
    }
    else {
        wxString caption("");
        if ( path->m_sTypeString.IsNull() || path->m_sTypeString.IsEmpty() ) {
            caption.append( wxS("Path Properties, Layer: ") );
        }
        else {
            caption.append( path->m_sTypeString );
            caption.append( wxS(" Properties, Layer:") );
        }
        caption.append( GetLayerName( path->m_LayerID ) );
        pPathPropDialog->SetDialogTitle( caption );
        
    }

    if( !pPathPropDialog->IsShown() )
        pPathPropDialog->Show();
    
    UpdatePathListCtrl();

    m_bNeedConfigFlush = true;
*/    
}

void PathManagerDialog::ShowPathPropertiesDialog ( Path *path )
{
    if( NULL == pPathPropDialog )          // There is one global instance of the PathProp Dialog
        pPathPropDialog = new PathProp( GetParent() );

    pPathPropDialog->SetPathAndUpdate( path );
    pPathPropDialog->UpdateProperties( path );
    if( !path->m_bIsInLayer ) {
        if ( path->m_sTypeString.IsNull() || path->m_sTypeString.IsEmpty() )
            pPathPropDialog->SetDialogTitle( wxS("Path Properties") );
        else {
            wxString sTitle( path->m_sTypeString );
            sTitle.append( wxS(" Properties") );
            pPathPropDialog->SetDialogTitle( sTitle );
        }
    }
    else {
        wxString caption( wxS("") );
        if ( path->m_sTypeString.IsNull() || path->m_sTypeString.IsEmpty() ) {
            caption.append( wxS("Path Properties, Layer: ") );
        }
        else {
            caption.append( path->m_sTypeString );
            caption.append( wxS(" Properties, Layer:") );
        }
        caption.append( GetLayerName( path->m_LayerID ) );
        pPathPropDialog->SetDialogTitle( caption );
        
    }

    if( !pPathPropDialog->IsShown() )
        pPathPropDialog->Show();
    
    UpdatePathListCtrl();

    m_bNeedConfigFlush = true;
}
void PathManagerDialog::OnPathZoomtoClick( wxCommandEvent &event )
{
//      if (cc1->m_bFollow)
//            return;

    // Zoom into the bounding box of the selected route
    long item = -1;
    item = m_pPathListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    // optionally make this route exclusively visible
    if( m_bCtrlDown ) MakeAllPathsInvisible();

    Path *path = pPathList->Item( m_pPathListCtrl->GetItemData( item ) )->GetData();

    if( !path ) return;

    // Ensure path is visible
    if( !path->IsVisible() ) {
        path->SetVisible( true );
        m_pPathListCtrl->SetItemImage( item, path->IsVisible() ? 0 : 1 );
        pConfig->UpdatePath( path );
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

        Path *ppath_to_export = pPathList->Item( m_pPathListCtrl->GetItemData( item ) )->GetData();

        if( ppath_to_export ) {
            list.Append( ppath_to_export );
            if( ppath_to_export->m_PathNameString != wxEmptyString )
                suggested_name = ppath_to_export->m_PathNameString;
        }
    }

    pConfig->ExportGPXPaths( this, &list, suggested_name );
}

void PathManagerDialog::OnPathActivateClick( wxCommandEvent &event )
{
    // Activate the selected path, unless it already is
    long item = -1;
    item = m_pPathListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    if( m_bCtrlDown ) MakeAllPathsInvisible();

    Path *ppath = pPathList->Item( m_pPathListCtrl->GetItemData( item ) )->GetData();

    if( !ppath ) return;

    if( !ppath->m_bPathIsActive ) {
        if( !ppath->IsVisible() ) {
            ppath->SetVisible( true );
            m_pPathListCtrl->SetItemImage( item, 0, 0 );
        }

        ZoomtoPath( ppath );

        g_pPathMan->ActivatePath( (Path *) ppath );
        btnPathActivate->SetLabel( _T("&Deactivate") );
    } else {
        g_pPathMan->DeactivatePath( ppath );
        btnPathActivate->SetLabel( _T("&Activate") );
    }

    UpdatePathListCtrl();

    pConfig->UpdatePath( ppath );

    cc1->Refresh();

//      btnRteActivate->SetLabel(route->m_bRtIsActive ? _("Deactivate") : _("Activate"));

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
        Path *path = pPathList->Item( m_pPathListCtrl->GetItemData( clicked_index ) )->GetData();

        int OCPNPoints_set_viz = wxID_YES;
        bool togglesharedOCPNPoints = true;
        bool has_shared_OCPNPoints = g_pPathMan->DoesPathContainSharedPoints( path );
        
        if( has_shared_OCPNPoints && path->IsVisible() ) {
            OCPNPoints_set_viz = OCPNMessageBox(  this, _("Do you also want to make the shared OCPN Points being part of this boundary invisible?"), _("Question"), wxYES_NO );
            togglesharedOCPNPoints = (OCPNPoints_set_viz == wxID_YES);
        }
        path->SetVisible( !path->IsVisible(), togglesharedOCPNPoints );
        m_pPathListCtrl->SetItemImage( clicked_index, path->IsVisible() ? 0 : 1 );

        ::wxBeginBusyCursor();

        pConfig->UpdatePath( path );
        cc1->Refresh();

        //   We need to update the waypoint list control only if the visibility of shared waypoints might have changed.
        if( has_shared_OCPNPoints )
            UpdateOCPNPointsListCtrlViz();

        ::wxEndBusyCursor();

    }

    // Allow wx to process...
    event.Skip();
}

// FIXME add/remove route segments/waypoints from selectable items, so there are no
// hidden selectables! This should probably be done outside this class!
// The problem is that the current waypoint class does not provide good support
// for this, there is a "visible" property, but no means for proper management.
// Jan. 28 2010: Ideas:
// - Calculate on the fly how many visible routes use a waypoint.
//   This requires a semidouble loop (routes, waypoints in visible routes). It could
//   be done by the function getting the selection. Potentially somewhat slow?
// - OR keep a property in waypoints telling that
//   (A number, increased/decreased for each waypoint by Route::SetVisible()).
//   Immediate result when detecting the selectable object, small overhead in
//   Route::SetVisible(). I prefer this.
// - We also need to know if the waypoint should otherwise be visible,
//   ie it is a "normal" waypoint used in the route (then it should be visible
//   in all cases). Is this possible with current code?
// - Get rid of the Select objects, they do no good! They should be replaced with a function
//   in the application, the search would reqire equal amount of looping, but less
//   dereferencing pointers, and it would remove the overhead of keeping and maintaining
//   the extra pointer lists.

void PathManagerDialog::OnPathBtnLeftDown( wxMouseEvent &event )
{
    m_bCtrlDown = event.ControlDown();
    event.Skip();
}

void PathManagerDialog::OnPathSelected( wxListEvent &event )
{
    long clicked_index = event.m_itemIndex;
    // Process the clicked item
    Path *path = pPathList->Item( m_pPathListCtrl->GetItemData( clicked_index ) )->GetData();
    m_pPathListCtrl->SetItemImage( clicked_index, path->IsVisible() ? 0 : 1 );

    if( cc1 )
        cc1->Refresh();

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

void PathManagerDialog::UpdateOCPNPointsListCtrl( OCPNPoint *rp_select, bool b_retain_sort )
{
    long selected_id = -1;
    long item = -1;

    if( NULL == rp_select ) {
        // if an item was selected, make it selected again if it still exists
        item = m_pOCPNPointListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );

        if( item != -1 ) selected_id = m_pOCPNPointListCtrl->GetItemData( item );
    }

    //  Freshen the image list
    m_pOCPNPointListCtrl->SetImageList( pOCPNPointMan->Getpmarkicon_image_list(), wxIMAGE_LIST_SMALL );
    
    m_pOCPNPointListCtrl->DeleteAllItems();

    wxOCPNPointListNode *node = pOCPNPointMan->GetOCPNPointList()->GetFirst();

    int index = 0;
    while( node ) {
        OCPNPoint *rp = node->GetData();
        if( rp && rp->IsListed() ) {
            if( rp->m_bIsInPath ) {
                if( !rp->m_bKeepXPath ) {
                    node = node->GetNext();
                    continue;
                }
            }

            wxListItem li;
            li.SetId( index );
            li.SetImage( rp->IsVisible() ? pOCPNPointMan->GetIconIndex( rp->GetIconBitmap() )
                                    : pOCPNPointMan->GetXIconIndex( rp->GetIconBitmap() ) );
            li.SetData( rp );
            li.SetText( _T("") );
            long idx = m_pOCPNPointListCtrl->InsertItem( li );

            wxString name = rp->GetName();
            if( name.IsEmpty() ) name = _("(Unnamed OCPN Point)");
            m_pOCPNPointListCtrl->SetItem( idx, colOCPNPOINTNAME, name );

            double dst;
            DistanceBearingMercator( rp->m_lat, rp->m_lon, gLat, gLon, NULL, &dst );
            wxString dist;
            dist.Printf( _T("%5.2f ") + getUsrDistanceUnit(), toUsrDistance( dst ) );
            m_pOCPNPointListCtrl->SetItem( idx, colOCPNPOINTDIST, dist );

            if( rp == rp_select ) selected_id = (long) rp_select; //index; //m_pWptListCtrl->GetItemData(item);

            index++;
        }

        node = node->GetNext();
    }

    if( !b_retain_sort ) {
        m_pOCPNPointListCtrl->SortItems( SortOCPNPointsOnName, (long) m_pOCPNPointListCtrl );
        sort_OCPNPoint_key = SORT_ON_NAME;
    } else {
        switch( sort_OCPNPoint_key ){
            case SORT_ON_NAME:
                m_pOCPNPointListCtrl->SortItems( SortOCPNPointsOnName, (long) m_pOCPNPointListCtrl );
                break;
            case SORT_ON_DISTANCE:
                m_pOCPNPointListCtrl->SortItems( SortOCPNPointsOnDistance, (long) m_pOCPNPointListCtrl );
                break;
        }
    }

    if( selected_id > -1 ) {
        item = m_pOCPNPointListCtrl->FindItem( -1, selected_id );
        m_pOCPNPointListCtrl->SetItemState( item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
    }

    if( (m_lastOCPNPointItem >= 0) && (m_pOCPNPointListCtrl->GetItemCount()) )
        m_pOCPNPointListCtrl->EnsureVisible( m_lastOCPNPointItem );
    UpdateOCPNPointButtons();
}

void PathManagerDialog::UpdateOCPNPointsListCtrlViz( )
{
    long item = -1;
    for ( ;; )
    {
        item = m_pOCPNPointListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);
        if ( item == -1 )
            break;
        
        OCPNPoint *pRP = (OCPNPoint *)m_pOCPNPointListCtrl->GetItemData(item);
        int image = pRP->IsVisible() ? pOCPNPointMan->GetIconIndex( pRP->GetIconBitmap() )
        : pOCPNPointMan->GetXIconIndex( pRP->GetIconBitmap() ) ;
                        
        m_pOCPNPointListCtrl->SetItemImage(item, image);
    }
}


void PathManagerDialog::OnOCPNPointDefaultAction( wxListEvent &event )
{
    wxCommandEvent evt;
    OnOCPNPointPropertiesClick( evt );
}

void PathManagerDialog::OnOCPNPointSelected( wxListEvent &event )
{
    UpdateOCPNPointButtons();
}

void PathManagerDialog::OnOCPNPointColumnClicked( wxListEvent &event )
{
    if( event.m_col == 1 ) {
        sort_OCPNPoint_name_dir++;
        m_pOCPNPointListCtrl->SortItems( SortOCPNPointsOnName, (long) m_pOCPNPointListCtrl );
        sort_OCPNPoint_key = SORT_ON_NAME;
    } else
        if( event.m_col == 2 ) {
            sort_OCPNPoint_len_dir++;
            m_pOCPNPointListCtrl->SortItems( SortOCPNPointsOnDistance, (long) m_pOCPNPointListCtrl );
            sort_OCPNPoint_key = SORT_ON_DISTANCE;
        }
}

void PathManagerDialog::UpdateOCPNPointButtons()
{
    long item = -1;
    item = m_pOCPNPointListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    bool enable1 = ( m_pOCPNPointListCtrl->GetSelectedItemCount() == 1 );
    bool enablemultiple = ( m_pOCPNPointListCtrl->GetSelectedItemCount() >= 1 );

    if( enable1 )
        m_lastOCPNPointItem = item;
    else
        m_lastOCPNPointItem = -1;

    //  Check selection to see if it is in a layer
    //  If so, disable the "delete" button
    bool b_delete_enable = true;
    item = -1;
    for ( ;; )
    {
        item = m_pOCPNPointListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;

        OCPNPoint *wp = (OCPNPoint *) m_pOCPNPointListCtrl->GetItemData( item );

        if( wp && wp->m_bIsInLayer) {
            b_delete_enable = false;
            break;
        }
    }


    btnOCPNPointProperties->Enable( enable1 );
    btnOCPNPointZoomto->Enable( enable1 );
    btnOCPNPointDeleteAll->Enable( enablemultiple );
    btnOCPNPointDelete->Enable( b_delete_enable && enablemultiple );
    btnOCPNPointGoTo->Enable( enable1 );
    btnOCPNPointExport->Enable( enablemultiple );
    //btnOCPNPointSendToGPS->Enable( enable1 );
}

void PathManagerDialog::OnOCPNPointToggleVisibility( wxMouseEvent &event )
{
    wxPoint pos = event.GetPosition();
    int flags = 0;
    long clicked_index = m_pOCPNPointListCtrl->HitTest( pos, flags );

    //    Clicking Visibility column?
    if( clicked_index > -1 && event.GetX() < m_pOCPNPointListCtrl->GetColumnWidth( colPATHVISIBLE ) ) {
        // Process the clicked item
        OCPNPoint *wp = (OCPNPoint *) m_pOCPNPointListCtrl->GetItemData( clicked_index );

        wp->SetVisible( !wp->IsVisible() );
        m_pOCPNPointListCtrl->SetItemImage( clicked_index,
                                      wp->IsVisible() ? pOCPNPointMan->GetIconIndex( wp->GetIconBitmap() )
                                                      : pOCPNPointMan->GetXIconIndex( wp->GetIconBitmap() ) );

        pConfig->UpdateOCPNPoint( wp );

        cc1->Refresh();
    }

    // Allow wx to process...
    event.Skip();
}

void PathManagerDialog::OnOCPNPointNewClick( wxCommandEvent &event )
{
    OCPNPoint *pWP = new OCPNPoint( gLat, gLon, g_default_OCPNPoint_icon, wxEmptyString,
            GPX_EMPTY_STRING );
    pWP->m_bIsolatedMark = true;                      // This is an isolated mark
    pSelect->AddSelectableOCPNPoint( gLat, gLon, pWP );
    pConfig->AddNewOCPNPoint( pWP, -1 );    // use auto next num
    cc1->Refresh( false );      // Needed for MSW, why not GTK??

    OCPNPointShowPropertiesDialog( pWP, GetParent() );
}

void PathManagerDialog::OnOCPNPointPropertiesClick( wxCommandEvent &event )
{
    long item = -1;
    item = m_pOCPNPointListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    OCPNPoint *wp = ( OCPNPoint *) m_pOCPNPointListCtrl->GetItemData( item );

    if( !wp ) return;

    OCPNPointShowPropertiesDialog( wp, GetParent() );

    UpdateOCPNPointsListCtrl();
    m_bNeedConfigFlush = true;
}

void PathManagerDialog::OCPNPointShowPropertiesDialog( OCPNPoint* wp, wxWindow* parent )
{
// TODO (jon#1#): May need to show OCPNPoint properties dialog here
    // There is one global instance of the MarkProp Dialog
    
    if( NULL == pOCPNPointPropDialog )
        pOCPNPointPropDialog = new OCPNDrawPointInfoImpl( parent );

    pOCPNPointPropDialog->SetOCPNPoint( wp );
    pOCPNPointPropDialog->UpdateProperties();

    wxString caption( wxS("") );
    if ( wp->m_sTypeString.IsNull() || wp->m_sTypeString.IsEmpty() )
        caption.append( wxS("OCPN Draw Point") );
    else
        caption.append( wp->m_sTypeString );
    caption.append( wxS(" Properties") );

    if( wp->m_bIsInLayer ) {
        caption.append( wxS(", Layer: ") );
        caption.Append( GetLayerName( wp->m_LayerID ) );
    }
    pOCPNPointPropDialog->SetDialogTitle( caption );

    if( !pOCPNPointPropDialog->IsShown() )
        pOCPNPointPropDialog->Show();

}

void PathManagerDialog::OnOCPNPointZoomtoClick( wxCommandEvent &event )
{
    long item = -1;
    item = m_pOCPNPointListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    OCPNPoint *wp = (OCPNPoint *) m_pOCPNPointListCtrl->GetItemData( item );

    if( !wp ) return;

//      cc1->ClearbFollow();
//      cc1->SetViewPoint(wp->m_lat, wp->m_lon, cc1->GetVPScale(), 0, cc1->GetVPRotation(), CURRENT_RENDER);
//      cc1->Refresh();
    gFrame->JumpToPosition( wp->m_lat, wp->m_lon, cc1->GetVPScale() );

}

void PathManagerDialog::OnOCPNPointDeleteClick( wxCommandEvent &event )
{
    OCPNPointList list;

    int answer = OCPNMessageBox( this, _("Are you sure you want to delete the selected object(s)"), wxString( _("OpenCPN Alert") ), wxYES_NO );
    if ( answer != wxID_YES )
        return;

    bool busy = false;
    if( m_pOCPNPointListCtrl->GetSelectedItemCount() ) {
        ::wxBeginBusyCursor();
        m_bNeedConfigFlush = true;
        busy = true;
    }

    long item = -1;
    long item_last_selected = -1;
    for ( ;; )
    {
        item = m_pOCPNPointListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;

        item_last_selected = item;
        OCPNPoint *wp = (OCPNPoint *) m_pOCPNPointListCtrl->GetItemData( item );

        if( wp && !wp->m_bIsInLayer)
            list.Append( wp );
    }

    if( busy ) {
        for(unsigned int i=0 ; i < list.GetCount() ; i++) {
            OCPNPoint *wp = list.Item(i)->GetData();
            if( wp ) {

                if ( wp->m_bIsInPath )
                {
                    if ( wxYES == OCPNMessageBox(this,  _( "The OCPN Point you want to delete is used in a path, do you really want to delete it?" ), _( "OpenCPN Alert" ), wxYES_NO ))
                            pOCPNPointMan->DestroyOCPNPoint( wp );
                }
                else
                    pOCPNPointMan->DestroyOCPNPoint( wp );

            }
        }

        long item_next = m_pOCPNPointListCtrl->GetNextItem( item_last_selected );         // next in list
        OCPNPoint *wp_next = NULL;
        if( item_next > -1 )
            wp_next = (OCPNPoint *) m_pOCPNPointListCtrl->GetItemData( item_next );

        m_lastOCPNPointItem = item_next;

        UpdatePathListCtrl();
        UpdateOCPNPointsListCtrl( wp_next, true );

/*        if( pMarkPropDialog ) {
            pMarkPropDialog->SetRoutePoint( NULL );
            pMarkPropDialog->UpdateProperties();
        }
*/
        cc1->undo->InvalidateUndo();
        cc1->Refresh();
        ::wxEndBusyCursor();
    }

}
/*
void PathManagerDialog::OnOCPNPointGoToClick( wxCommandEvent &event )
{
    long item = -1;
    item = m_pOCPNPointListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    OCPNPoint *wp = (OCPNPoint *) m_pOCPNPointListCtrl->GetItemData( item );

    if( !wp ) return;

    OCPNPoint *pWP_src = new OCPNPoint( gLat, gLon, g_default_OCPNPoint_icon, wxEmptyString,
            GPX_EMPTY_STRING );
    pSelect->AddSelectableOCPNPoint( gLat, gLon, pWP_src );

    Path *temp_path = new Path();
    pPathList->Append( temp_path );

    temp_path->AddPoint( pWP_src );
    temp_path->AddPoint( wp );

    pSelect->AddSelectablePathSegment( gLat, gLon, wp->m_lat, wp->m_lon, pWP_src, wp, temp_path );

    wxString name = wp->GetName();
    if( name.IsEmpty() ) name = _("(Unnamed OCPN Point)");
    wxString pathName = _("Go to ");
    pathName.Append( name );
    temp_path->m_PathNameString = pathName;
    
    temp_path->m_bDeleteOnArrival = true;

    if( g_pPathMan->GetpActivePath() ) g_pPathMan->DeactivatePath();

    g_pPathMan->ActivatePath( temp_path );

    UpdatePathListCtrl();
}
*/
void PathManagerDialog::OnOCPNPointExportClick( wxCommandEvent &event )
{
    OCPNPointList list;

    wxString suggested_name = _T("OCPN Points");

    long item = -1;
    for ( ;; )
    {
        item = m_pOCPNPointListCtrl->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;

        OCPNPoint *wp = (OCPNPoint *) m_pOCPNPointListCtrl->GetItemData( item );

        if( wp && !wp->m_bIsInLayer) {
            list.Append( wp );
            if( wp->GetName() != wxEmptyString )
                suggested_name = wp->GetName();
        }
    }

    pConfig->ExportGPXOCPNPoints( this, &list, suggested_name );
}
/*
void PathManagerDialog::OnOCPNPointSendToGPSClick( wxCommandEvent &event )
{
    long item = -1;
    item = m_pOCPNPointListCtrl->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;

    OCPNPoint *wp = (OCPNPoint *) m_pOCPNPointListCtrl->GetItemData( item );

    if( !wp ) return;

    SendToGpsDlg *pdlg = new SendToGpsDlg();
    pdlg->SetOCPNPoint( wp );

    wxString source;
    pdlg->Create( NULL, -1, _( "Send To GPS..." ), source );
    pdlg->ShowModal();

    delete pdlg;
}
*/
void PathManagerDialog::OnOCPNPointDeleteAllClick( wxCommandEvent &event )
{
    wxString prompt;
    int buttons, type;
    if ( !pOCPNPointMan->SharedOCPNPointsExist() )
    {
        prompt = _("Are you sure you want to delete <ALL> OCPN points?");
        buttons = wxYES_NO;
        type = 1;
    }
    else
    {
        prompt = _("There are some OCPN points used in paths or anchor alarms.\n Do you want to delete them as well?\n This will change the paths and disable the anchor alarms.\n Answering No keeps the OCPN points used in paths or alarms.");
        buttons = wxYES_NO | wxCANCEL;
        type = 2;
    }
    int answer = OCPNMessageBox( this, prompt, wxString( _("OpenCPN Alert") ), buttons );
    if ( answer == wxID_YES )
        pOCPNPointMan->DeleteAllOCPNPoints( true );
    if ( answer == wxID_NO && type == 2 )
        pOCPNPointMan->DeleteAllOCPNPoints( false );          // only delete unused OCPN points

/*    if( pMarkPropDialog ) {
        pMarkPropDialog->SetOCPNPoint( NULL );
        pMarkPropDialog->UpdateProperties();
    }
*/
    m_lastOCPNPointItem = -1;
    UpdatePathListCtrl();
    UpdateOCPNPointsListCtrl();
    cc1->undo->InvalidateUndo();
    cc1->Refresh();
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
                _("Hide WPT names") );
        else
            btnLayToggleNames->SetLabel( _("Show WPT names") );

        if( pLayerList->Item( m_pLayListCtrl->GetItemData( item ) )->GetData()->IsVisibleOnListing() ) btnLayToggleListing->SetLabel(
                _("Unlist contents") );
        else
            btnLayToggleListing->SetLabel( _("List contents") );
    } else {
        btnLayToggleChart->SetLabel( _("Show on chart") );
        btnLayToggleNames->SetLabel( _("Show WPT names") );
        btnLayToggleListing->SetLabel( _("List contents") );
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
    
    pConfig->UI_ImportGPX( this, true, _T("") );
    
#ifdef __WXOSX__
    ShowWithEffect(wxSHOW_EFFECT_BLEND );
#endif
    
    
    g_bShowLayers = show_flag;

    UpdatePathListCtrl();
    UpdateOCPNPointsListCtrl();
    UpdateLayListCtrl();
    cc1->Refresh();
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
    int answer = OCPNMessageBox( this, prompt, wxString( _("OpenCPN Alert") ), wxYES_NO );
    if ( answer == wxID_NO )
        return;
    
    // Process Tracks and Routes in this layer
    wxPathListNode *node1 = pPathList->GetFirst();
    wxPathListNode *node2;
    while( node1 ) {
        Path *pPath = node1->GetData();
        node2 = node1->GetNext();
        if( pPath->m_bIsInLayer && ( pPath->m_LayerID == layer->m_LayerID ) ) {
            pPath->m_bIsInLayer = false;
            pPath->m_LayerID = 0;
            g_pPathMan->DeletePath( pPath );
        }
        node1 = node2;
        node2 = NULL;
    }

    //m_pSelectedRoute = NULL;
    //m_pSelectedTrack = NULL;
    //m_pFoundRoutePoint = NULL;
    //m_pFoundRoutePointSecond = NULL;

    //if ( pRoutePropDialog )
    //{
    //      //pRoutePropDialog->SetRouteAndUpdate ( track );
    //      pRoutePropDialog->UpdateProperties();
    //}

    // Process waypoints in this layer
    wxOCPNPointListNode *node = pOCPNPointMan->GetOCPNPointList()->GetFirst();
    wxOCPNPointListNode *node3;

    while( node ) {
        node3 = node->GetNext();
        OCPNPoint *rp = node->GetData();
        if( rp && ( rp->m_LayerID == layer->m_LayerID ) ) {
            rp->m_bIsInLayer = false;
            rp->m_LayerID = 0;
            pOCPNPointMan->DestroyOCPNPoint( rp, false );         // no need to update the change set on layer ops
        }

        node = node3;
        node3 = NULL;
    }

/*    if( pMarkPropDialog ) {
        pMarkPropDialog->SetOCPNPoint( NULL );
        pMarkPropDialog->UpdateProperties();
    }
*/
    pLayerList->DeleteObject( layer );

    UpdatePathListCtrl();
    UpdateOCPNPointsListCtrl();
    UpdateLayListCtrl();

    cc1->Refresh();

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
    wxPathListNode *node1 = pPathList->GetFirst();
    while( node1 ) {
        Path *pPath = node1->GetData();
        if( pPath->m_bIsInLayer && ( pPath->m_LayerID == layer->m_LayerID ) ) {
            pPath->SetVisible( layer->IsVisibleOnChart() );
            pConfig->UpdatePath( pPath );
        }
        node1 = node1->GetNext();
    }

    // Process OCPN points in this layer
    wxOCPNPointListNode *node = pOCPNPointMan->GetOCPNPointList()->GetFirst();

    while( node ) {
        OCPNPoint *rp = node->GetData();
        if( rp && ( rp->m_LayerID == layer->m_LayerID ) ) {
            rp->SetVisible( layer->IsVisibleOnChart() );
        }

        node = node->GetNext();
    }

    UpdatePathListCtrl();
    UpdateOCPNPointsListCtrl();
    UpdateLayListCtrl();
    UpdateLayButtons();

    cc1->Refresh();
}

void PathManagerDialog::OnLayToggleNamesClick( wxCommandEvent &event )
{
    // Toggle WPT names visibility on chart for selected layer
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
    wxPathListNode *node1 = pPathList->GetFirst();
    while( node1 ) {
        Path *pPath = node1->GetData();
        if( pPath->m_bIsInLayer && ( pPath->m_LayerID == layer->m_LayerID ) ) {
            wxOCPNPointListNode *node = pPath->pOCPNPointList->GetFirst();
            OCPNPoint *prp1 = node->GetData();
            while( node ) {
                prp1->m_bShowName = layer->HasVisibleNames();
                node = node->GetNext();
            }
        }
        node1 = node1->GetNext();
    }

    // Process OCPN points in this layer
    wxOCPNPointListNode *node = pOCPNPointMan->GetOCPNPointList()->GetFirst();

    while( node ) {
        OCPNPoint *rp = node->GetData();
        if( rp && ( rp->m_LayerID == layer->m_LayerID ) ) {
            rp->SetNameShown( layer->HasVisibleNames() );
        }

        node = node->GetNext();
    }

    UpdateLayButtons();

    cc1->Refresh();
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
    wxPathListNode *node1 = pPathList->GetFirst();
    while( node1 ) {
        Path *pPath = node1->GetData();
        if( pPath->m_bIsInLayer && ( pPath->m_LayerID == layer->m_LayerID ) ) {
            pPath->SetListed( layer->IsVisibleOnListing() );
            pConfig->UpdatePath(pPath);
        }
        node1 = node1->GetNext();
    }

    // Process OCPN points in this layer
    //  n.b.  If the OCPN point belongs to a track, and is not shared, then do not list it.
    //  This is a performance optimization, allowing large track support.

    wxOCPNPointListNode *node = pOCPNPointMan->GetOCPNPointList()->GetFirst();

    while( node ) {
        OCPNPoint *rp = node->GetData();
        if( rp && !rp->m_bIsInTrack && rp->m_bIsolatedMark && ( rp->m_LayerID == layer->m_LayerID ) ) {
            rp->SetListed( layer->IsVisibleOnListing() );
        }

        node = node->GetNext();
    }

    UpdatePathListCtrl();
    UpdateOCPNPointsListCtrl();
    UpdateLayListCtrl();

    ::wxEndBusyCursor();

    cc1->Refresh();
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
}

void PathManagerDialog::OnImportClick( wxCommandEvent &event )
{
    // Import routes
    // FIXME there is no way to instruct this function about what to import.
    // Suggest to add that!
    
#ifdef __WXOSX__
    HideWithEffect(wxSHOW_EFFECT_BLEND );
#endif
    
    pConfig->UI_ImportGPX( this );

#ifdef __WXOSX__
    ShowWithEffect(wxSHOW_EFFECT_BLEND );
#endif
    
    UpdatePathListCtrl();
    UpdateOCPNPointsListCtrl();
    UpdateLayListCtrl();

    cc1->Refresh();
}
void PathManagerDialog::OnExportClick( wxCommandEvent &event )
{
    pConfig->ExportGPX( this );
}

void PathManagerDialog::OnExportVizClick( wxCommandEvent &event )
{
    pConfig->ExportGPX( this, true, true );     // only visible objects, layers included
}

//END Event handlers
