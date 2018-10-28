/**************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Path and Point manager dialog implemenation
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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "PathAndPointManagerDialogImpl.h"
#include "ocpn_plugin.h"
#include "ocpn_draw_pi.h"
//#include "styles.h"
#include "dychart.h"
//#include "navutil.h"
#include "ODConfig.h"
#include "ODEventHandler.h"
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
#include "ODNewODPointDialogImpl.h"
#include "ODPoint.h"
#include "ODSelect.h"
#include "chcanv.h"
#include "georef.h"
#include "ODLayer.h"

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
    "...................."
};
    
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
    "..################.."
};
        
enum { colPATHVISIBLE = 0, colPATHNAME, colPATHDESC };
enum { colLAYVISIBLE = 0, colLAYNAME, colLAYITEMS };
enum { colOCPNPOINTICON = 0, colOCPNPOINTNAME, colOCPNPOINTDIST };

// GLOBALS :0
extern ocpn_draw_pi *g_ocpn_draw_pi;
extern PathList     *g_pPathList;
extern BoundaryList *g_pBoundaryList;
extern ODLayerList  *g_pLayerList;
extern ODPathPropertiesDialogImpl     *g_pODPathPropDialog;
extern BoundaryProp *g_pBoundaryPropDialog;
extern EBLProp      *g_pEBLPropDialog;
extern DRProp       *g_pDRPropDialog;
extern GZProp       *g_pGZPropDialog;
extern PILProp      *g_pPILPropDialog;
extern PathMan      *g_pPathMan;
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
extern ODEventHandler   *g_ODEventHandler;
extern wxString     g_sDefaultImportType;
        
int sort_path_on_vis;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortPathOnVis(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortPathOnVis(long item1, long item2, long list)
#endif
{
    wxListCtrl *lc = (wxListCtrl*)list;
    
    wxListItem it1, it2;
    it1.SetId(lc->FindItem(-1, item1));
    it1.SetColumn(0);
    it1.SetMask(it1.GetMask() | wxLIST_MASK_IMAGE);
    
    it2.SetId(lc->FindItem(-1, item2));
    it2.SetColumn(0);
    it2.SetMask(it2.GetMask() | wxLIST_MASK_IMAGE);
    
    lc->GetItem(it1);
    lc->GetItem(it2);
    
    if(sort_path_on_vis & 1)
        if(it2.GetImage() > it1.GetImage())
            return 1;
        else
            return 0;
        else
            if(it2.GetImage() <= it1.GetImage())
                return 1;
            else
                return 0;
            
}

// sort callback. Sort by path name.
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

// sort callback. Sort by path Destination.
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

// sort callback. Sort by path visibility.
int sort_ODPoint_on_vis;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortODPointsOnVis(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortODPointsOnVis(long item1, long item2, long list)
#endif
{
    wxListCtrl *lc = (wxListCtrl*)list;
    
    wxListItem it1, it2;
    it1.SetId(lc->FindItem(-1, item1));
    it1.SetColumn(0);
    it1.SetMask(it1.GetMask() | wxLIST_MASK_IMAGE);
    
    it2.SetId(lc->FindItem(-1, item2));
    it2.SetColumn(0);
    it2.SetMask(it2.GetMask() | wxLIST_MASK_IMAGE);
    
    lc->GetItem(it1);
    lc->GetItem(it2);
    int l_iNumIcons = g_pODPointMan->GetNumIcons();
    int l_iImageVis1 = (it1.GetImage() > l_iNumIcons) ? 0 : 1;
    int l_iImageVis2 = (it2.GetImage() > l_iNumIcons) ? 0 : 1;
    
    if(sort_ODPoint_on_vis & 1)
        if(l_iImageVis2 > l_iImageVis1)
            return 1;
        else
            return 0;
        else
            if(l_iImageVis2 <= l_iImageVis1)
                return 1;
            else
                return 0;
}

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

// sort callback. Sort by layer visibility.
int sort_layer_on_vis;
#if wxCHECK_VERSION(2, 9, 0)
int wxCALLBACK SortLayerOnVis(long item1, long item2, wxIntPtr list)
#else
int wxCALLBACK SortLaerOnVis(long item1, long item2, long list)
#endif
{
    wxListCtrl *lc = (wxListCtrl*)list;
    
    wxListItem it1, it2;
    it1.SetId(lc->FindItem(-1, item1));
    it1.SetColumn(0);
    it1.SetMask(it1.GetMask() | wxLIST_MASK_IMAGE);
    
    it2.SetId(lc->FindItem(-1, item2));
    it2.SetColumn(0);
    it2.SetMask(it2.GetMask() | wxLIST_MASK_IMAGE);
    
    lc->GetItem(it1);
    lc->GetItem(it2);
    
    if(sort_path_on_vis & 1)
        if(it2.GetImage() > it1.GetImage())
            return 1;
        else
            return 0;
        else
            if(it2.GetImage() <= it1.GetImage())
                return 1;
            else
                return 0;
            
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

PathAndPointManagerDialogImpl::PathAndPointManagerDialogImpl(wxWindow* parent) : PathAndPointManagerDialogDef( parent )
{
    m_iPage = -1;
    
    long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER;
    #ifdef __WXOSX__
    style |= wxSTAY_ON_TOP;
    #endif
    
    
    wxFont *qFont = GetOCPNScaledFont_PlugIn(wxS("Dialog"), 0);
    SetFont( *qFont );
    
    m_lastODPointItem = -1;
    m_lastPathItem = -1;
    
    m_buttonImport = NULL;
    m_buttonPointExportSelected = NULL;
    m_buttonExportAllVisible = NULL;
    
    Fit();
    
    SetMinSize( GetBestSize() );
    
    Centre();
    
    // create a image list for the list with just the eye icon
    wxImageList *imglist = new wxImageList( 20, 20, true, 1 );
    imglist->Add( wxBitmap( eye ) );
    imglist->Add( wxBitmap( eyex ) );
    m_listCtrlPath->AssignImageList( imglist, wxIMAGE_LIST_SMALL );
    // Assign will handle destroy, Set will not. It's OK, that's what we want
    m_listCtrlPoints->SetImageList( g_pODPointMan->Getpmarkicon_image_list(), wxIMAGE_LIST_SMALL );
    
    //m_listCtrlLayers->AssignImageList( imglist, wxIMAGE_LIST_SMALL );
    m_listCtrlLayers->SetImageList( imglist, wxIMAGE_LIST_SMALL );
    
    SetColorScheme();
    
    UpdatePathListCtrl();
    UpdateODPointsListCtrl();
    UpdateLayListCtrl();
    
    // This should work under Linux :-(
    //m_notebookPathAndPointManager->Connect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler(PathAndPointManagerDialogImpl::OnTabSwitch), NULL, this);
    
}

void PathAndPointManagerDialogImpl::OnTabSwitch( wxNotebookEvent &event )
{
    if( !m_notebookPathAndPointManager ) return;
    int current_page = m_notebookPathAndPointManager->GetSelection();
    
    switch (current_page)
    {
        case 0: {
            // Path
            if ( m_listCtrlPath ) {
                UpdatePathListCtrl();
                wxString l_sLabel = _("I&mport");
                l_sLabel.Append(_T(" "));
                l_sLabel.Append(g_pODConfig->m_sImport_Type);
                l_sLabel.Append(_T("..."));
                m_buttonImport->SetLabel(l_sLabel);
                break;
            }
        }
        case 1: {
            // Point
            if ( m_listCtrlPoints ) {
                UpdateODPointsListCtrl();
                wxString l_sLabel = _("I&mport");
                l_sLabel.Append(_T(" "));
                l_sLabel.Append(g_pODConfig->m_sImport_Type);
                l_sLabel.Append(_T("..."));
                m_buttonImport->SetLabel(l_sLabel);
                break;
            }
        }
        case 2: {
            // Layer
            if( m_listCtrlLayers ) {
                UpdateLayListCtrl();
                wxString l_sLabel = _("I&mport");
                l_sLabel.Append(_T(" "));
                l_sLabel.Append(_("gpx"));
                l_sLabel.Append(_T("..."));
                m_buttonImport->SetLabel(l_sLabel);
                break;
            }
        }            
        case wxNOT_FOUND:
            break;
            
        default:
            break;
    }
}

void PathAndPointManagerDialogImpl::OnPathDeleteClick( wxCommandEvent &event )
{
    PathList list;
    
    int answer = OCPNMessageBox_PlugIn( this, _("Are you sure you want to delete the selected object(s)"), _("OpenCPN Alert"), wxYES_NO );
    if ( answer != wxID_YES )
        return;
    
    bool busy = false;
    if( m_listCtrlPath->GetSelectedItemCount() ) {
        ::wxBeginBusyCursor();
        busy = true;
    }
    
    long item = -1;
    for ( ;; )
    {
        item = m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;
        
        ODPath *ppath_to_delete = g_pPathList->Item( m_listCtrlPath->GetItemData( item ) )->GetData();
        
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
        if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
            g_pODPointPropDialog->ValidateMark();
        }
        
        // TODO fix up undo
        //ocpncc1->undo->InvalidateUndo();
        RequestRefresh( GetOCPNCanvasWindow() );
        ::wxEndBusyCursor();
    }
    
}

void PathAndPointManagerDialogImpl::OnPathDeleteAllClick( wxCommandEvent &event )
{
    int dialog_ret = OCPNMessageBox_PlugIn( this, _("Are you sure you want to delete <ALL> paths?"), _("OpenCPN Alert"), wxYES_NO );
    
    if( dialog_ret == wxID_YES ) {
        
        g_pPathMan->DeleteAllPaths();
        
        m_lastPathItem = -1;
        UpdatePathListCtrl();
        
        if( g_pODPathPropDialog ) g_pODPathPropDialog->Hide();
        
        if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
            g_pODPointPropDialog->ValidateMark();
        }
        // TODO fix up undo
        //ocpncc1->undo->InvalidateUndo();
        RequestRefresh( GetOCPNCanvasWindow() );
    }
}

void PathAndPointManagerDialogImpl::OnPathPropertiesClick( wxCommandEvent &event )
{
    // Show path properties dialog for selected path
    long item = -1;
    item = m_listCtrlPath->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;
    
    ODPath *path = g_pPathList->Item( m_listCtrlPath->GetItemData( item ) )->GetData();
    
    if( !path ) return;
    
    ShowPathPropertiesDialog ( path );
}

void PathAndPointManagerDialogImpl::ShowPathPropertiesDialog ( ODPath *inpath )
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
        if( NULL == g_pODPathPropDialog )          // There is one global instance of the PathProp Dialog
            g_pODPathPropDialog = new ODPathPropertiesDialogImpl( g_ocpn_draw_pi->m_parent_window );
        g_pODPathPropDialog = g_pODPathPropDialog;
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
}

void PathAndPointManagerDialogImpl::OnPathCenterViewClick( wxCommandEvent &event )
{
    // Zoom into the bounding box of the selected path
    long item = -1;
    item = m_listCtrlPath->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;
    
    // optionally make this path exclusively visible
    if( m_bCtrlDown ) MakeAllPathsInvisible();
    
    ODPath *path = g_pPathList->Item( m_listCtrlPath->GetItemData( item ) )->GetData();
    
    if( !path ) return;
    
    // Ensure path is visible
    if( !path->IsVisible() ) {
        path->SetVisible( true );
        m_listCtrlPath->SetItemImage( item, path->IsVisible() ? 0 : 1 );
        g_pODConfig->UpdatePath( path );
    }
    
    ZoomtoPath( path );
}

void PathAndPointManagerDialogImpl::OnPathExportSelectedClick( wxCommandEvent &event )
{
    PathList list;
    
    wxString suggested_name = _T("paths");
    
    long item = -1;
    for ( ;; )
    {
        item = m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;
        
        ODPath *ppath_to_export = g_pPathList->Item( m_listCtrlPath->GetItemData( item ) )->GetData();
        
        if( ppath_to_export ) {
            list.Append( ppath_to_export );
            if( ppath_to_export->m_PathNameString != wxEmptyString )
                suggested_name = ppath_to_export->m_PathNameString;
        }
    }
    
    g_pODConfig->ExportGPXPaths( this, &list, suggested_name );
}

void PathAndPointManagerDialogImpl::OnPathActivateClick( wxCommandEvent &event )
{
    // Activate the selected path, unless it already is
    long item = -1;
    item = m_listCtrlPath->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    if( item == -1 ) return;
    
    if( m_bCtrlDown ) MakeAllPathsInvisible();
    
    ODPath *ppath = g_pPathList->Item( m_listCtrlPath->GetItemData( item ) )->GetData();
    
    if( !ppath ) return;
    
    if( !ppath->m_bPathIsActive ) {
        if( !ppath->IsVisible() ) {
            ppath->SetVisible( true );
            m_listCtrlPath->SetItemImage( item, 0, 0 );
        }
        
        ZoomtoPath( ppath );
        
        g_pPathMan->ActivatePath( (ODPath *) ppath );
        m_buttonPathActivate->SetLabel( _T("&Deactivate") );
    } else {
        g_pPathMan->DeactivatePath( ppath );
        m_buttonPathActivate->SetLabel( _T("&Activate") );
    }
    
    UpdatePathListCtrl();
    
    g_pODConfig->UpdatePath( ppath );
    
    RequestRefresh( GetOCPNCanvasWindow() );
}

void PathAndPointManagerDialogImpl::OnPathToggleVisibility( wxMouseEvent &event )
{
    wxPoint pos = event.GetPosition();
    int flags = 0;
    long clicked_index = m_listCtrlPath->HitTest( pos, flags );
    
    //    Clicking Visibility column?
    if( clicked_index > -1 && event.GetX() < m_listCtrlPath->GetColumnWidth( colPATHVISIBLE ) ) {
        // Process the clicked item
        ODPath *path = g_pPathList->Item( m_listCtrlPath->GetItemData( clicked_index ) )->GetData();
        
        int ODPoints_set_viz = wxID_YES;
        bool togglesharedODPoints = true;
        bool has_shared_ODPoints = g_pPathMan->DoesPathContainSharedPoints( path );
        
        if( has_shared_ODPoints && path->IsVisible() ) {
            ODPoints_set_viz = OCPNMessageBox_PlugIn(  this, _("Do you also want to make the shared OD Points being part of this boundary invisible?"), _("Question"), wxYES_NO );
            togglesharedODPoints = (ODPoints_set_viz == wxID_YES);
        }
        path->SetVisible( !path->IsVisible(), togglesharedODPoints );
        m_listCtrlPath->SetItemImage( clicked_index, path->IsVisible() ? 0 : 1 );
        //        m_listCtrlPath->SetItemData( clicked_index, path->IsVisible() ? 0 : 1);
        
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

void PathAndPointManagerDialogImpl::SelectedPathToggleVisibility( bool visible )
{
    long item = -1;
    for ( ;; )
    {
        item = m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if ( item == -1 )
            break;
        
        ODPath *ppath = g_pPathList->Item( m_listCtrlPath->GetItemData( item ) )->GetData();
        
        if( ppath ) {
            ppath->SetVisible(visible);
        }
        m_listCtrlPath->SetItemImage( item, ppath->IsVisible() ? 0 : 1 );
    }
}

void PathAndPointManagerDialogImpl::OnPathButtonLeftDown( wxMouseEvent &event )
{
    m_bCtrlDown = event.ControlDown();
    event.Skip();
}

void PathAndPointManagerDialogImpl::OnPathSelected( wxListEvent &event )
{
    long clicked_index = event.m_itemIndex;
    // Process the clicked item
    ODPath *path = g_pPathList->Item( m_listCtrlPath->GetItemData( clicked_index ) )->GetData();
    path->m_bPathManagerBlink = true;
    
    m_listCtrlPath->SetItemImage( clicked_index, path->IsVisible() ? 0 : 1 );
    
    RequestRefresh( GetOCPNCanvasWindow() );
    
    UpdatePathButtons();
    
}

void PathAndPointManagerDialogImpl::OnPathDeSelected( wxListEvent &event )
{
    long clicked_index = event.m_itemIndex;
    // Process the clicked item
    ODPath *path = g_pPathList->Item( m_listCtrlPath->GetItemData( clicked_index ) )->GetData();
    path->m_bPathManagerBlink = false;
    
    m_listCtrlPath->SetItemImage( clicked_index, path->IsVisible() ? 0 : 1 );
    
    RequestRefresh( GetOCPNCanvasWindow() );
    
    UpdatePathButtons();
    
}

void PathAndPointManagerDialogImpl::OnPathColumnClicked( wxListEvent &event )
{
    switch (event.m_col) {
        case 0:
            sort_path_on_vis++;
            m_listCtrlPath->SortItems( SortPathOnVis, (long) m_listCtrlPath );
            break;
        case 1:
            sort_path_name_dir++;
            m_listCtrlPath->SortItems( SortPathOnName, (long) m_listCtrlPath );
            break;
        case 2:
            sort_path_to_dir++;
            m_listCtrlPath->SortItems( SortPathOnTo, (long) m_listCtrlPath );
            break;
    }
}

void PathAndPointManagerDialogImpl::OnPathRightClick( wxListEvent &event )
{
    g_ODEventHandler->SetWindow( ocpncc1 );
    g_ODEventHandler->SetPath( NULL );
    g_ODEventHandler->SetPoint( (ODPoint*)NULL );
    g_ODEventHandler->PopupMenu( TYPE_PATHMGR_PATH_DLG );
}

void PathAndPointManagerDialogImpl::OnPathDefaultAction( wxListEvent &event )
{
    wxCommandEvent evt;
    OnPathPropertiesClick( evt );
}

void PathAndPointManagerDialogImpl::SetColorScheme()
{
    //TODO Need to find out what this is for?
    DimeWindow( this );
}

void PathAndPointManagerDialogImpl::UpdatePathListCtrl()
{
    // if an item was selected, make it selected again if it still exist
    long item = -1;
    item = m_listCtrlPath->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    long selected_id = -1;
    if( item != -1 ) selected_id = m_listCtrlPath->GetItemData( item );
    
    // Delete existing items
    m_listCtrlPath->DeleteAllItems();
    
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
        
        long idx = m_listCtrlPath->InsertItem( li );
        
        wxString name = ( *it )->m_PathNameString;
        if( name.IsEmpty() ) {
            name = _("(Unnamed )");
            #if wxCHECK_VERSION(3,0,0)
            name.append( _(( *it )->m_sTypeString ) );
            #else
            name.append( ( *it )->m_sTypeString );
            #endif
        }
        m_listCtrlPath->SetItem( idx, colPATHNAME, name );
        wxString desc = ( *it ) ->m_PathDescription;
        m_listCtrlPath->SetItem( idx, colPATHDESC, desc );
        //        m_listCtrlPath->SetItemData( idx, ( *it )->IsVisible() ? 0 : 1 );
    }
    
    m_listCtrlPath->SortItems( SortPathOnName, (long) m_listCtrlPath );
    
    // restore selection if possible
    // NOTE this will select a different item, if one is deleted
    // (the next path will get that index).
    if( selected_id > -1 ) {
        item = m_listCtrlPath->FindItem( -1, selected_id );
        m_listCtrlPath->SetItemState( item, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
    }
    
    if( (m_lastPathItem >= 0) && (m_listCtrlPath->GetItemCount()) )
        m_listCtrlPath->EnsureVisible( m_lastPathItem );
    UpdatePathButtons();
    
    for(int i = 0; i < m_listCtrlPath->GetColumnCount(); i++) {
        m_listCtrlPath->SetColumnWidth( i, wxLIST_AUTOSIZE_USEHEADER );
        int h_width = m_listCtrlPath->GetColumnWidth(i);
        m_listCtrlPath->SetColumnWidth( i, wxLIST_AUTOSIZE );
        int a_width = m_listCtrlPath->GetColumnWidth(i);
        m_listCtrlPath->SetColumnWidth(i, (std::max)(a_width, h_width));
    }
    
    this->GetSizer()->Fit( this );
    this->Layout();
    
}

void PathAndPointManagerDialogImpl::UpdatePathButtons()
{
    // enable/disable buttons
    long selected_index_index = m_listCtrlPath->GetNextItem( -1, wxLIST_NEXT_ALL,
                                                              wxLIST_STATE_SELECTED );
    bool enable1 =  m_listCtrlPath->GetSelectedItemCount() == 1;
    
    m_lastPathItem = selected_index_index;
    
    m_buttonPathDelete->Enable( m_listCtrlPath->GetSelectedItemCount() > 0 );
    m_buttonPathCenterView->Enable( enable1 ); 
    m_buttonPathProperties->Enable( enable1 );
    m_buttonPathDeleteAll->Enable( true );
    
    // set activate button text
    ODPath *path = NULL;
    if( enable1 ) {
        path = g_pPathList->Item( m_listCtrlPath->GetItemData( selected_index_index ) )->GetData();
        if ( path ) {
            m_buttonPathActivate->Enable( true );
            if ( path->IsActive() ) m_buttonPathActivate->SetLabel( _("&Deactivate") );
            else m_buttonPathActivate->SetLabel( _("&Activate") );
        }
        else
            m_buttonPathActivate->Enable( false );
    }
    else
        m_buttonPathActivate->Enable( false );
    
}

void PathAndPointManagerDialogImpl::MakeAllPathsInvisible()
{
    PathList::iterator it;
    long index = 0;
    for( it = ( *g_pPathList ).begin(); it != ( *g_pPathList ).end(); ++it, ++index ) {
        if( ( *it )->IsVisible() ) { // avoid config updating as much as possible!
            ( *it )->SetVisible( false );
            m_listCtrlPath->SetItemImage( m_listCtrlPath->FindItem( -1, index ), 1 ); // Likely not same order :0
            g_pODConfig->UpdatePath( *it ); 
        }
    }
}

void PathAndPointManagerDialogImpl::ZoomtoPath( ODPath *path )
{
    
    // Calculate bbox center
    LLBBox RBBox = path->GetBBox();
    double clat = RBBox.GetMinLat() + ( ( RBBox.GetMaxLat() - RBBox.GetMinLat() ) / 2 );
    double clon = RBBox.GetMinLon() + ( ( RBBox.GetMaxLon() - RBBox.GetMinLon() ) / 2 );
    
    if( clon > 180. ) clon -= 360.;
    else if( clon < -180. ) clon += 360.;
        
        // Calculate ppm
        double rw, rh, ppm; // path width, height, final ppm scale to use
        int ww, wh; // chart window width, height
        // path bbox width in nm
        DistanceBearingMercator_Plugin( RBBox.GetMinLat(), RBBox.GetMinLon(), RBBox.GetMinLat(),
                                        RBBox.GetMaxLon(), NULL, &rw );
        // path bbox height in nm
        DistanceBearingMercator_Plugin( RBBox.GetMinLat(), RBBox.GetMinLon(), RBBox.GetMaxLat(),
                                        RBBox.GetMinLon(), NULL, &rh );
        
        ocpncc1->GetSize( &ww, &wh );
        
        ppm = wxMin(ww/(rw*1852), wh/(rh*1852)) * ( 100 - fabs( clat ) ) / 90;
        
        ppm = wxMin(ppm, 1.0);
        
        JumpToPosition( clat, clon, ppm );
}

void PathAndPointManagerDialogImpl::OnPointColumnClick( wxListEvent &event )
{
    switch (event.m_col) {
        case 0:
            sort_ODPoint_on_vis++;
            m_listCtrlPoints->SortItems( SortODPointsOnVis, (long) m_listCtrlPoints );
            break;
        case 1:
            sort_ODPoint_name_dir++;
            m_listCtrlPoints->SortItems( SortODPointsOnName, (long) m_listCtrlPoints );
            break;
        case 2:
            sort_ODPoint_len_dir++;
            m_listCtrlPoints->SortItems( SortODPointsOnDistance, (long) m_listCtrlPoints );
            break;
    }
}

void PathAndPointManagerDialogImpl::ODPointShowPropertiesDialog( ODPoint* wp, wxWindow* parent )
{
    if( NULL == g_pODPointPropDialog )
        g_pODPointPropDialog = new ODPointPropertiesImpl( parent );
    
    DimeWindow( g_pODPointPropDialog );
    g_pODPointPropDialog->SetODPoint( wp );
    g_pODPointPropDialog->UpdateProperties();
    
    if( !g_pODPointPropDialog->IsShown() )
        g_pODPointPropDialog->Show();
    
}


void PathAndPointManagerDialogImpl::UpdateLayButtons()
{
    long item = -1;
    item = m_listCtrlLayers->GetNextItem( item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
    bool enable = ( item != -1 );
    
    //btnLayProperties->Enable(false);
    m_buttonLayerDelete->Enable( enable );
    m_buttonLayerShowOnChart->Enable( enable );
    m_buttonLayerListContents->Enable( enable );
    m_buttonLayerShowPointNames->Enable( enable );
    
    if( item >= 0 ) {
        if( g_pLayerList->Item( m_listCtrlLayers->GetItemData( item ) )->GetData()->IsVisible() ) m_buttonLayerShowOnChart->SetLabel(
            _("Hide from chart") );
        else
            m_buttonLayerShowOnChart->SetLabel( _("Show on chart") );
        
        if( g_pLayerList->Item( m_listCtrlLayers->GetItemData( item ) )->GetData()->HasVisibleNames() ) m_buttonLayerShowPointNames->SetLabel(
            _("Hide Point names") );
        else
            m_buttonLayerShowPointNames->SetLabel( _("Show Point names") );
        
        if( g_pLayerList->Item( m_listCtrlLayers->GetItemData( item ) )->GetData()->IsVisibleOnListing() ) m_buttonLayerListContents->SetLabel(
            _("Unlist contents ") );
        else
            m_buttonLayerListContents->SetLabel( _("List contents ") );
    } else {
        m_buttonLayerShowOnChart->SetLabel( _("Show on chart") );
        m_buttonLayerShowPointNames->SetLabel( _("Show Point names") );
        m_buttonLayerListContents->SetLabel( _("List contents ") );
    }
}
