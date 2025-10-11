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
// #include "dychart.h"
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
#include "georef.h"
#include "ODLayer.h"

#include <algorithm>

#define DIALOG_MARGIN 3

/* XPM */
static const char* eye[] = {"20 20 7 1",
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
static const char* eyex[] = {"20 20 8 1",
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

static int sort_path_on_vis;
#if wxCHECK_VERSION(2, 9, 0)
static int wxCALLBACK SortPathOnVis(wxIntPtr item1, wxIntPtr item2,
                                    wxIntPtr list)
#else
int wxCALLBACK SortPathOnVis(long item1, long item2, long list)
#endif
{
  bool it1, it2;
  it1 = (reinterpret_cast<ODPath*>(item1))->IsVisible();
  it2 = (reinterpret_cast<ODPath*>(item2))->IsVisible();

  if (it2 == it1) return 0;
  if (sort_path_on_vis & 1)
    if (it2 == true && it1 == false)
      return 1;
    else
      return -1;
  else if (it2 == false && it1 == true)
    return 1;
  else
    return -1;
}

// sort callback. Sort by path name.
static int sort_path_name_dir;
#if wxCHECK_VERSION(2, 9, 0)
static int wxCALLBACK SortPathOnName(wxIntPtr item1, wxIntPtr item2,
                                     wxIntPtr list)
#else
int wxCALLBACK SortPathOnName(long item1, long item2, long list)
#endif
{
  wxString it1, it2;
  it1 = (reinterpret_cast<ODPath*>(item1))->GetName();
  it2 = (reinterpret_cast<ODPath*>(item2))->GetName();
  if (sort_path_name_dir & 1)
    return it2.CmpNoCase(it1);
  else
    return it1.CmpNoCase(it2);
}

// sort callback. Sort by path Description.
static int sort_path_desc_dir;
#if wxCHECK_VERSION(2, 9, 0)
static int wxCALLBACK SortPathOnDesc(wxIntPtr item1, wxIntPtr item2,
                                     wxIntPtr list)
#else
int wxCALLBACK SortPathOnDesc(long item1, long item2, long list)
#endif
{
  wxString it1, it2;
  it1 = (reinterpret_cast<ODPath*>(item1))->GetDescription();
  it2 = (reinterpret_cast<ODPath*>(item2))->GetDescription();
  if (sort_path_desc_dir & 1)
    return it2.CmpNoCase(it1);
  else
    return it1.CmpNoCase(it2);
}

int sort_ODPoint_key;

// sort callback. Sort by path visibility.
static int sort_ODPoint_on_vis;
#if wxCHECK_VERSION(2, 9, 0)
static int wxCALLBACK SortODPointsOnVis(wxIntPtr item1, wxIntPtr item2,
                                        wxIntPtr list)
#else
int wxCALLBACK SortODPointsOnVis(long item1, long item2, long list)
#endif
{
  wxString it1, it2;
  it1 = (reinterpret_cast<ODPoint*>(item1))->GetIconName();
  it2 = (reinterpret_cast<ODPoint*>(item2))->GetIconName();

  if (it1 == it2) return 0;
  if (sort_ODPoint_on_vis & 1) {
    if (it2 > it1)
      return 1;
    else
      return -1;
  } else {
    if (it1 > it2)
      return 1;
    else
      return -1;
  }
}

// sort callback. Sort by point name.
static int sort_ODPoint_name_dir;
#if wxCHECK_VERSION(2, 9, 0)
static int wxCALLBACK SortODPointsOnName(wxIntPtr item1, wxIntPtr item2,
                                         wxIntPtr list)
#else
int wxCALLBACK SortODPointsOnName(long item1, long item2, long list)
#endif

{
  ODPoint* pODP1 = (ODPoint*)item1;
  ODPoint* pODP2 = (ODPoint*)item2;

  if (pODP1 && pODP2) {
    if (sort_ODPoint_name_dir & 1)
      return pODP2->GetName().CmpNoCase(pODP1->GetName());
    else
      return pODP1->GetName().CmpNoCase(pODP2->GetName());
  } else
    return 0;
}

// sort callback. Sort by point distance.
static int sort_ODPoint_len_dir;
#if wxCHECK_VERSION(2, 9, 0)
static int wxCALLBACK SortODPointsOnDistance(wxIntPtr item1, wxIntPtr item2,
                                             wxIntPtr list)
#else
int wxCALLBACK SortODPointsOnDistance(long item1, long item2, long list)
#endif
{
  wxListCtrl* lc = (wxListCtrl*)list;

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
  s1 = it1.GetText().c_str();
  s2 = it2.GetText().c_str();
  s1 = s1.Strip(wxString::leading);
  s2 = s2.Strip(wxString::leading);

  double l1, l2;
  s1.ToDouble(&l1);
  s2.ToDouble(&l2);

  if (l1 == l2) return 0;
  if (sort_ODPoint_len_dir & 1)
    if (l1 < l2)
      return 1;
    else
      return -1;
  else if (l2 < l1)
    return 1;
  else
    return -1;
}

// sort callback. Sort by layer visibility.
static int sort_layer_on_vis;
#if wxCHECK_VERSION(2, 9, 0)
static int wxCALLBACK SortLayersOnVis(wxIntPtr item1, wxIntPtr item2,
                                      wxIntPtr list)
#else
int wxCALLBACK SortLayersOnVis(long item1, long item2, long list)
#endif
{
  wxListCtrl* lc = (wxListCtrl*)list;

  wxListItem it1, it2;
  it1.SetId(lc->FindItem(-1, item1));
  it1.SetColumn(0);
  it1.SetMask(it1.GetMask() | wxLIST_MASK_IMAGE);

  it2.SetId(lc->FindItem(-1, item2));
  it2.SetColumn(0);
  it2.SetMask(it2.GetMask() | wxLIST_MASK_IMAGE);

  lc->GetItem(it1);
  lc->GetItem(it2);

  if (it1.GetImage() == it2.GetImage()) return 0;
  if (sort_layer_on_vis & 1)
    if (it2.GetImage() > it1.GetImage())
      return 1;
    else
      return -1;
  else if (it2.GetImage() <= it1.GetImage())
    return 1;
  else
    return -1;
}

// sort callback. Sort by layer name.
static int sort_layer_name_dir;
#if wxCHECK_VERSION(2, 9, 0)
static int wxCALLBACK SortLayersOnName(wxIntPtr item1, wxIntPtr item2,
                                       wxIntPtr list)
#else
int wxCALLBACK SortLayersOnName(long item1, long item2, long list)
#endif
{
  wxString it1, it2;
  it1 = (reinterpret_cast<ODLayer*>(item1))->GetName();
  it2 = (reinterpret_cast<ODLayer*>(item2))->GetName();
  if (sort_layer_name_dir & 1)
    return it2.CmpNoCase(it1);
  else
    return it1.CmpNoCase(it2);
}

// sort callback. Sort by layer size.
static int sort_layer_len_dir;
#if wxCHECK_VERSION(2, 9, 0)
static int wxCALLBACK SortLayersOnSize(wxIntPtr item1, wxIntPtr item2,
                                       wxIntPtr list)
#else
int wxCALLBACK SortLayersOnSize(long item1, long item2, long list)
#endif
{
  long it1, it2;
  it1 = (reinterpret_cast<ODLayer*>(item1))->GetNoOfItems();
  it2 = (reinterpret_cast<ODLayer*>(item2))->GetNoOfItems();

  if (it1 == it2) return 0;
  if (sort_layer_len_dir & 1)
    if (it1 < it2)
      return 1;
    else
      return -1;
  else if (it2 < it1)
    return 1;
  else
    return -1;
}

PathAndPointManagerDialogImpl::PathAndPointManagerDialogImpl(wxWindow* parent) {
  m_parent_window = parent;
  m_iPage = -1;
  m_bCtrlDown = false;

  m_dialogLabelFont = GetOCPNScaledFont_PlugIn(wxS("Dialog"), 0);
  SetFont(*m_dialogLabelFont);

  Create(parent);

  GetTextExtent(_T("W"), &m_CharWidth, &m_CharHeight, NULL, NULL,
                m_dialogLabelFont);

  wxDialog::EnableLayoutAdaptation(true);
  wxDialog::SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);

#ifdef __WXOSX__
  long style = GetWindowStyle();
  style |= wxSTAY_ON_TOP;
  SetWindowStyleFlag(style);
  Refresh();
#endif

  m_lastODPointItem = -1;
  m_lastPathItem = -1;

  m_listCtrlPath->InsertColumn(colPATHVISIBLE, _("Show"), wxLIST_FORMAT_LEFT,
                               wxLIST_AUTOSIZE);
  m_listCtrlPath->InsertColumn(colPATHNAME, _("Path Name"), wxLIST_FORMAT_LEFT,
                               wxLIST_AUTOSIZE);
  m_listCtrlPath->InsertColumn(colPATHDESC, _("Desc"), wxLIST_FORMAT_LEFT,
                               wxLIST_AUTOSIZE);
  m_listCtrlODPoints->InsertColumn(colOCPNPOINTICON, _("Icon"),
                                   wxLIST_FORMAT_LEFT, 44);
  m_listCtrlODPoints->InsertColumn(colOCPNPOINTNAME, _("OD Point Name"),
                                   wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
  m_listCtrlODPoints->InsertColumn(colOCPNPOINTDIST, _("Distance from Ownship"),
                                   wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
  m_listCtrlLayers->InsertColumn(colLAYVISIBLE, _("Show"), wxLIST_FORMAT_LEFT,
                                 wxLIST_AUTOSIZE);
  m_listCtrlLayers->InsertColumn(colLAYNAME, _("Layer Name"),
                                 wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
  m_listCtrlLayers->InsertColumn(colLAYITEMS, _("No. of items"),
                                 wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);

  m_panelPath->SetFocus();

  if (g_iDefaultPathAnPointManagerDialogPostionX == -1 ||
      g_iDefaultPathAnPointManagerDialogPostionY == -1)
    Center();
  else
    SetPosition(wxPoint(g_iDefaultPathAnPointManagerDialogPostionX,
                        g_iDefaultPathAnPointManagerDialogPostionY));

  // create a image list for the list with just the eye icon
  wxImageList* imglist = new wxImageList(20, 20, true, 1);
  wxBitmap l_bmEye = wxBitmap(eye);
  wxBitmap l_bmEyex = wxBitmap(eyex);
  int w = l_bmEye.GetWidth();
  int h = l_bmEye.GetHeight();
  w *= g_dOCPN_DisplayScaleFactor;
  h *= g_dOCPN_DisplayScaleFactor;
  l_bmEye =
      wxBitmap(l_bmEye.ConvertToImage().Scale(w, h, wxIMAGE_QUALITY_HIGH));
  l_bmEyex =
      wxBitmap(l_bmEyex.ConvertToImage().Scale(w, h, wxIMAGE_QUALITY_HIGH));
  imglist->Add(wxBitmap(l_bmEye));
  imglist->Add(wxBitmap(l_bmEyex));
  m_listCtrlPath->AssignImageList(imglist, wxIMAGE_LIST_SMALL);
  // Assign will handle destroy, Set will not. It's OK, that's what we want
  m_listCtrlODPoints->SetImageList(g_pODPointMan->Getpmarkicon_image_list(),
                                   wxIMAGE_LIST_SMALL);

  // m_listCtrlLayers->AssignImageList( imglist, wxIMAGE_LIST_SMALL );
  m_listCtrlLayers->SetImageList(imglist, wxIMAGE_LIST_SMALL);

  SetColorScheme();
  SetImportButtonText();
  //    m_buttonImport->SetFont(*m_dialogLabelFont);
  //    m_buttonExportAllVisible->SetFont(*m_dialogLabelFont);
  //    m_buttonOK->SetFont(*m_dialogLabelFont);
  UpdatePathListCtrl();
  UpdateODPointsListCtrl();
  UpdateLayerListCtrl();

  RecalculateSize();
  m_szDialogSize = GetSize();

  this - Layout();

  SetSizerAndFit(m_bSizerDialog);

  // This should work under Linux :-(
  // m_notebookPathAndPointManager->Connect(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,
  // wxNotebookEventHandler(PathAndPointManagerDialogImpl::OnTabSwitch), NULL,
  // this);
}

PathAndPointManagerDialogImpl::~PathAndPointManagerDialogImpl() { return; }

void PathAndPointManagerDialogImpl::OnTabSwitch(wxNotebookEvent& event) {
  SetImportButtonText();
}

void PathAndPointManagerDialogImpl::SetImportButtonText(void) {
  if (!m_notebookPathAndPointManager) return;
  int current_page = m_notebookPathAndPointManager->GetSelection();

  m_buttonImport->SetFont(*m_dialogLabelFont);

  switch (current_page) {
    case 0: {
      // Path
      if (m_listCtrlPath) {
        UpdatePathListCtrl();
        wxString l_sLabel = _("I&mport");
        l_sLabel.Append(_T(" "));
        l_sLabel.Append(g_pODConfig->m_sImport_Type);
        l_sLabel.Append(_T("..."));
        m_buttonImport->SetLabel(l_sLabel);
        m_buttonImport->Enable();
      }
      break;
    }
    case 1: {
      // Point
      if (m_listCtrlODPoints) {
        UpdateODPointsListCtrl();
        wxString l_sLabel = _("I&mport");
        l_sLabel.Append(_T(" "));
        l_sLabel.Append(g_pODConfig->m_sImport_Type);
        l_sLabel.Append(_T("..."));
        m_buttonImport->SetLabel(l_sLabel);
        m_buttonImport->Enable();
      }
      break;
    }
    case 2: {
      // Layer
      if (m_listCtrlLayers) {
        m_buttonImport->Disable();
        UpdateLayerListCtrl();
      }
      break;
    }
    case wxNOT_FOUND:
      break;

    default:
      break;
  }
}

void PathAndPointManagerDialogImpl::OnPathDeleteClick(wxCommandEvent& event) {
  PathList list;

  int answer = OCPNMessageBox_PlugIn(
      this, _("Are you sure you want to delete the selected object(s)"),
      _("OpenCPN Alert"), wxYES_NO);
  if (answer != wxID_YES) return;

  bool busy = false;
  if (m_listCtrlPath->GetSelectedItemCount()) {
    busy = true;
  }

  long item = -1;
  for (;;) {
    item = m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);
    if (item == -1) break;

    ODPath* ppath_to_delete =
        reinterpret_cast<ODPath*>(m_listCtrlPath->GetItemData(item));

    if (ppath_to_delete) list.Append(ppath_to_delete);
  }

  if (busy) {
    for (unsigned int i = 0; i < list.GetCount(); i++) {
      ODPath* path = list.Item(i)->GetData();
      if (path) {
        g_pODConfig->DeleteConfigPath(path);
        g_pPathMan->DeletePath(path);
      }
    }

    m_lastPathItem = -1;
    UpdatePathListCtrl(false);
    if (g_pODPointPropDialog && g_pODPointPropDialog->IsShown()) {
      g_pODPointPropDialog->ValidateMark();
    }

    // TODO fix up undo
    // m_parent_window->undo->InvalidateUndo();
    RequestRefresh(GetOCPNCanvasWindow());
  }
}

void PathAndPointManagerDialogImpl::OnPathDeleteAllClick(
    wxCommandEvent& event) {
  int dialog_ret = OCPNMessageBox_PlugIn(
      this, _("Are you sure you want to delete <ALL> paths?"),
      _("OpenCPN Alert"), wxYES_NO);

  if (dialog_ret == wxID_YES) {
    g_pPathMan->DeleteAllPaths();

    m_lastPathItem = -1;
    UpdatePathListCtrl(false);

    if (g_pODPathPropDialog) g_pODPathPropDialog->Hide();

    if (g_pODPointPropDialog && g_pODPointPropDialog->IsShown()) {
      g_pODPointPropDialog->ValidateMark();
    }
    // TODO fix up undo
    // m_parent_window->undo->InvalidateUndo();
    RequestRefresh(GetOCPNCanvasWindow());
  }
}

void PathAndPointManagerDialogImpl::OnPathPropertiesClick(
    wxCommandEvent& event) {
  // Show path properties dialog for selected path
  long item = -1;
  item =
      m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  if (item == -1) return;

  ODPath* path = reinterpret_cast<ODPath*>(m_listCtrlPath->GetItemData(item));

  if (!path) return;

  ShowPathPropertiesDialog(path);
}

void PathAndPointManagerDialogImpl::ShowPathPropertiesDialog(ODPath* inpath) {
  ODPath* l_pPath = NULL;
  ;
  Boundary* l_pBoundary = NULL;
  EBL* l_pEBL = NULL;
  DR* l_pDR = NULL;
  GZ* l_pGZ = NULL;
  PIL* l_pPIL = NULL;

  if (inpath->m_sTypeString == wxT("Boundary")) {
    wxFont* l_dialogFont = GetOCPNScaledFont_PlugIn(wxS("Dialog"), 0);
    if (g_dialogFont != l_dialogFont) {
      g_dialogFont = l_dialogFont;
      if (NULL != g_pBoundaryPropDialog) {
        g_ocpn_draw_pi->DeleteWindow(g_pBoundaryPropDialog);
        g_pBoundaryPropDialog = NULL;
      }
    }

    if (NULL == g_pBoundaryPropDialog)  // There is one global instance of the
                                        // BoundaryProp Dialog
      g_pBoundaryPropDialog = new BoundaryProp(m_parent_window);
    g_pODPathPropDialog = g_pBoundaryPropDialog;
    l_pBoundary = (Boundary*)inpath;
    l_pPath = l_pBoundary;
    g_pBoundaryPropDialog->SetPath(l_pBoundary);
    g_pBoundaryPropDialog->UpdateProperties(l_pBoundary);
  } else if (inpath->m_sTypeString == wxT("EBL")) {
    wxFont* l_dialogFont = GetOCPNScaledFont_PlugIn(wxS("Dialog"), 0);
    if (g_dialogFont != l_dialogFont) {
      g_dialogFont = l_dialogFont;
      if (NULL != g_pEBLPropDialog) {
        g_ocpn_draw_pi->DeleteWindow(g_pEBLPropDialog);
        g_pEBLPropDialog = NULL;
      }
    }

    if (NULL ==
        g_pEBLPropDialog)  // There is one global instance of the ELBProp Dialog
      g_pEBLPropDialog = new EBLProp(m_parent_window);
    g_pODPathPropDialog = g_pEBLPropDialog;
    l_pEBL = (EBL*)inpath;
    l_pPath = l_pEBL;
    g_pEBLPropDialog->SetPath(l_pEBL);
    g_pEBLPropDialog->UpdateProperties(l_pEBL);
  } else if (inpath->m_sTypeString == wxT("DR")) {
    wxFont* l_dialogFont = GetOCPNScaledFont_PlugIn(wxS("Dialog"), 0);
    if (g_dialogFont != l_dialogFont) {
      g_dialogFont = l_dialogFont;
      if (NULL != g_pDRPropDialog) {
        g_ocpn_draw_pi->DeleteWindow(g_pDRPropDialog);
        g_pDRPropDialog = NULL;
      }
    }

    if (NULL ==
        g_pDRPropDialog)  // There is one global instance of the DRProp Dialog
      g_pDRPropDialog = new DRProp(m_parent_window);
    g_pODPathPropDialog = g_pDRPropDialog;
    l_pDR = (DR*)inpath;
    l_pPath = l_pDR;
    g_pDRPropDialog->SetPath(l_pDR);
    g_pDRPropDialog->UpdateProperties(l_pDR);
  } else if (inpath->m_sTypeString == wxT("Guard Zone")) {
    wxFont* l_dialogFont = GetOCPNScaledFont_PlugIn(wxS("Dialog"), 0);
    if (g_dialogFont != l_dialogFont) {
      g_dialogFont = l_dialogFont;
      if (NULL != g_pGZPropDialog) {
        g_ocpn_draw_pi->DeleteWindow(g_pGZPropDialog);
        g_pGZPropDialog = NULL;
      }
    }

    if (NULL ==
        g_pGZPropDialog)  // There is one global instance of the GZProp Dialog
      g_pGZPropDialog = new GZProp(m_parent_window);
    g_pODPathPropDialog = g_pGZPropDialog;
    l_pGZ = (GZ*)inpath;
    l_pPath = l_pGZ;
    g_pGZPropDialog->SetPath(l_pGZ);
    g_pGZPropDialog->UpdateProperties(l_pGZ);
  } else if (inpath->m_sTypeString == wxT("PIL")) {
    wxFont* l_dialogFont = GetOCPNScaledFont_PlugIn(wxS("Dialog"), 0);
    if (g_dialogFont != l_dialogFont) {
      g_dialogFont = l_dialogFont;
      if (NULL != g_pPILPropDialog) {
        g_ocpn_draw_pi->DeleteWindow(g_pPILPropDialog);
        g_pPILPropDialog = NULL;
      }
    }

    if (NULL ==
        g_pPILPropDialog)  // There is one global instance of the PILProp Dialog
      g_pPILPropDialog = new PILProp(m_parent_window);
    g_pODPathPropDialog = g_pPILPropDialog;
    l_pPIL = (PIL*)inpath;
    l_pPath = l_pPIL;
    g_pPILPropDialog->SetPath(l_pPIL);
    g_pPILPropDialog->UpdateProperties(l_pPIL);
  } else {
    wxFont* l_dialogFont = GetOCPNScaledFont_PlugIn(wxS("Dialog"), 0);
    if (g_dialogFont != l_dialogFont) {
      g_dialogFont = l_dialogFont;
      if (NULL != g_pODPathPropDialog) {
        g_ocpn_draw_pi->DeleteWindow(g_pODPathPropDialog);
        g_pODPathPropDialog = NULL;
      }
    }

    if (NULL == g_pODPathPropDialog)  // There is one global instance of the
                                      // PathProp Dialog
      g_pODDefaultPathPropDialog =
          new ODPathPropertiesDialogImpl(m_parent_window);
    g_pODPathPropDialog = g_pODDefaultPathPropDialog;
    l_pPath = inpath;
    g_pODPathPropDialog->SetPath(l_pPath);
    g_pODPathPropDialog->UpdateProperties(l_pPath);
  }

  if (!l_pPath->m_bIsInLayer) {
    if (l_pPath->m_sTypeString.IsNull() || l_pPath->m_sTypeString.IsEmpty())
      g_pODPathPropDialog->SetDialogTitle(_("Path Properties"));
    else if (l_pPath->m_sTypeString == wxT("Boundary"))
      g_pODPathPropDialog->SetDialogTitle(_("Boundary Properties"));
    else if (l_pPath->m_sTypeString == wxT("EBL"))
      g_pODPathPropDialog->SetDialogTitle(_("EBL Properties"));
    else if (l_pPath->m_sTypeString == wxT("DR"))
      g_pODPathPropDialog->SetDialogTitle(_("DR Properties"));
    else if (l_pPath->m_sTypeString == wxT("Guard Zone"))
      g_pODPathPropDialog->SetDialogTitle(_("Guard Zone Properties"));
    else if (l_pPath->m_sTypeString == wxT("PIL"))
      g_pODPathPropDialog->SetDialogTitle(_("Parallel Index Line Properties"));
  } else {
    wxString caption(wxS(""));
    if (l_pPath->m_sTypeString.IsNull() || l_pPath->m_sTypeString.IsEmpty())
      caption.append(_("Path Properties, Layer: "));
    else if (l_pPath->m_sTypeString == wxT("Boundary"))
      caption.append(_("Boundary Properties, Layer: "));
    else if (l_pPath->m_sTypeString == wxT("EBL"))
      caption.append(_("EBL Properties, Layer: "));
    else if (l_pPath->m_sTypeString == wxT("DR"))
      caption.append(_("DR Properties, Layer: "));

    caption.append(GetLayerName(l_pPath->m_LayerID));
    g_pODPathPropDialog->SetDialogTitle(caption);
  }

  DimeWindow(g_pODPathPropDialog);
  if (!g_pODPathPropDialog->IsShown()) g_pODPathPropDialog->Show();
  g_pODPathPropDialog->Raise();

  UpdatePathListCtrl();
}

void PathAndPointManagerDialogImpl::OnPathCenterViewClick(
    wxCommandEvent& event) {
  // Zoom into the bounding box of the selected path
  long item = -1;
  item =
      m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  if (item == -1) return;

  // optionally make this path exclusively visible
  if (m_bCtrlDown) MakeAllPathsInvisible();

  ODPath* path = (ODPath*)m_listCtrlPath->GetItemData(item);

  if (!path) return;

  // Ensure path is visible
  if (!path->IsVisible()) {
    path->SetVisible(true);
    m_listCtrlPath->SetItemImage(item, path->IsVisible() ? 0 : 1);
    g_pODConfig->UpdatePath(path);
  }

  ZoomtoPath(path);
  m_bCtrlDown = false;
}

void PathAndPointManagerDialogImpl::OnPathExportSelectedClick(
    wxCommandEvent& event) {
  PathList list;

  wxString suggested_name = _T("paths");

  long item = -1;
  for (;;) {
    item = m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);
    if (item == -1) break;

    ODPath* ppath_to_export =
        reinterpret_cast<ODPath*>(m_listCtrlPath->GetItemData(item));
    if (ppath_to_export) {
      list.Append(ppath_to_export);
      if (ppath_to_export->m_PathNameString != wxEmptyString)
        suggested_name = ppath_to_export->m_PathNameString;
    }
  }

  g_pODConfig->ExportGPXPaths(this, &list, suggested_name);
}

void PathAndPointManagerDialogImpl::OnPathActivateClick(wxCommandEvent& event) {
  // Activate the selected path, unless it already is
  long item = -1;

  if (m_bCtrlDown) MakeAllPathsInvisible();
  int select_count = m_listCtrlPath->GetSelectedItemCount();

  int item_num = 1;
  bool b_activate_paths = true;
  for (;; ++item_num) {
    item = m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);
    if (item == -1) break;

    ODPath* ppath =
        reinterpret_cast<ODPath*>(m_listCtrlPath->GetItemData(item));

    if (!ppath) continue;

    if (item_num == 1) {
      if (!ppath->m_bPathIsActive) {
        m_buttonPathActivate->SetLabel(_T("&Deactivate"));
        b_activate_paths = true;
      } else {
        m_buttonPathActivate->SetLabel(_T("&Activate"));
        b_activate_paths = false;
      }
    }

    if (select_count == 1) {
      if (!ppath->IsVisible()) {
        ppath->SetVisible(true);
        m_listCtrlPath->SetItemImage(item, 0, 0);
      }
      if (!ppath->m_bPathIsActive) ZoomtoPath(ppath);
    }

    if (b_activate_paths)
      g_pPathMan->ActivatePath(ppath);
    else
      g_pPathMan->DeactivatePath(ppath);

    g_pODConfig->UpdatePath(ppath);
  }

  UpdatePathListCtrl();

  m_bCtrlDown = false;

  RequestRefresh(GetOCPNCanvasWindow());
}

void PathAndPointManagerDialogImpl::OnPathToggleVisibility(
    wxMouseEvent& event) {
  wxPoint pos = event.GetPosition();
  int flags = 0;
  long clicked_index = m_listCtrlPath->HitTest(pos, flags);

  //    Clicking Visibility column?
  if (clicked_index > -1 &&
      event.GetX() < m_listCtrlPath->GetColumnWidth(colPATHVISIBLE)) {
    // Process the clicked item
    ODPath* path = (ODPath*)m_listCtrlPath->GetItemData(clicked_index);

    int ODPoints_set_viz = wxID_YES;
    bool togglesharedODPoints = true;
    bool has_shared_ODPoints = g_pPathMan->DoesPathContainSharedPoints(path);

    if (has_shared_ODPoints && path->IsVisible()) {
      ODPoints_set_viz = OCPNMessageBox_PlugIn(
          this,
          _("Do you also want to make the shared OD Points being part of this "
            "boundary invisible?"),
          _("Question"), wxYES_NO);
      togglesharedODPoints = (ODPoints_set_viz == wxID_YES);
    }
    path->SetVisible(!path->IsVisible(), togglesharedODPoints);
    m_listCtrlPath->SetItemImage(clicked_index, path->IsVisible() ? 0 : 1);
    //        m_listCtrlPath->SetItemData( clicked_index, path->IsVisible() ? 0
    //        : 1);

    g_pODConfig->UpdatePath(path);
    RequestRefresh(GetOCPNCanvasWindow());

    //   We need to update the ODPoint list control only if the visibility of
    //   shared ODPoints might have changed.
    if (has_shared_ODPoints) UpdateODPointListCtrlViz();
  }

  // Allow wx to process...
  event.Skip();
}

void PathAndPointManagerDialogImpl::SelectedPathToggleVisibility(bool visible) {
  long item = -1;
  for (;;) {
    item = m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);
    if (item == -1) break;

    ODPath* ppath = (ODPath*)m_listCtrlPath->GetItemData(item);

    if (ppath) {
      ppath->SetVisible(visible);
    }
    m_listCtrlPath->SetItemImage(item, ppath->IsVisible() ? 0 : 1);
  }
}

void PathAndPointManagerDialogImpl::OnPathButtonLeftDown(wxMouseEvent& event) {
  m_bCtrlDown = event.ControlDown();
  event.Skip();
}

void PathAndPointManagerDialogImpl::OnPathSelected(wxListEvent& event) {
  long clicked_index = event.m_itemIndex;
  // Process the clicked item
  ODPath* path = (ODPath*)m_listCtrlPath->GetItemData(clicked_index);
  // ODPath *path = (ODPath *) m_listCtrlPath->GetItemData( item );
  //     ODPath *path = g_pPathList->Item( m_listCtrlPath->GetId() )->GetData();
  path->m_bPathManagerBlink = true;

  m_listCtrlPath->SetItemImage(clicked_index, path->IsVisible() ? 0 : 1);

  RequestRefresh(GetOCPNCanvasWindow());

  UpdatePathButtons();
}

void PathAndPointManagerDialogImpl::OnPathDeSelected(wxListEvent& event) {
  long clicked_index = event.m_itemIndex;
  // Process the clicked item
  ODPath* path = (ODPath*)m_listCtrlPath->GetItemData(clicked_index);
  path->m_bPathManagerBlink = false;
  m_listCtrlPath->SetItemImage(clicked_index, path->IsVisible() ? 0 : 1);

  RequestRefresh(GetOCPNCanvasWindow());

  UpdatePathButtons();
}

void PathAndPointManagerDialogImpl::OnPathColumnClicked(wxListEvent& event) {
  switch (event.m_col) {
    case 0:
      sort_path_on_vis++;
      m_listCtrlPath->SortItems(SortPathOnVis, (long)m_listCtrlPath);
      break;
    case 1:
      sort_path_name_dir++;
      m_listCtrlPath->SortItems(SortPathOnName, (long)m_listCtrlPath);
      break;
    case 2:
      sort_path_desc_dir++;
      m_listCtrlPath->SortItems(SortPathOnDesc, (long)m_listCtrlPath);
      break;
  }
}

void PathAndPointManagerDialogImpl::OnPathRightClick(wxListEvent& event) {
  g_ODEventHandler->SetWindow(m_parent_window);
  g_ODEventHandler->SetPath(NULL);
  g_ODEventHandler->SetPoint((ODPoint*)NULL);
  g_ODEventHandler->PopupMenu(TYPE_PATHMGR_PATH_DLG);
}

void PathAndPointManagerDialogImpl::OnPathDefaultAction(wxListEvent& event) {
  wxCommandEvent evt;
  OnPathPropertiesClick(evt);
}

void PathAndPointManagerDialogImpl::SetColorScheme() {
  // TODO Need to find out what this is for?
  DimeWindow(this);
}

void PathAndPointManagerDialogImpl::UpdatePathListCtrl(
    bool b_retain_selection) {
  // if an item was selected, make it selected again if it still exist
  long item = -1;
  int i_itemcount = m_listCtrlPath->GetSelectedItemCount();
  long* l_selection = new long[i_itemcount];
  if (b_retain_selection && i_itemcount > 0) {
    item = m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);
    int i_element = 0;
    for (;; ++i_element) {
      l_selection[i_element] = item;
      item = m_listCtrlPath->GetNextItem(item, wxLIST_NEXT_ALL,
                                         wxLIST_STATE_SELECTED);
      if (item == -1) {
        item = 0;
        break;
      }
    }
  }
  long selected_id = -1;
  if (item != -1) selected_id = l_selection[0];

  // Delete existing items
  if (!m_listCtrlPath->DeleteAllItems()) return;

  // then add path to the listctrl
  PathList::iterator it;
  int index = 0;
  for (it = (*g_pPathList).begin(); it != (*g_pPathList).end(); ++it, ++index) {
    if (!(*it)->IsListed()) continue;

    wxListItem li;
    li.SetId(index);
    li.SetImage((*it)->IsVisible() ? 0 : 1);
    li.SetData(*it);
    li.SetText(_T(""));

    if ((*it)->m_bPathIsActive) {
      wxFont font = *wxNORMAL_FONT;
      font.SetWeight(wxFONTWEIGHT_BOLD);
      li.SetFont(font);
    }

    long idx = m_listCtrlPath->InsertItem(li);

    wxString name = (*it)->m_PathNameString;
    if (name.IsEmpty()) {
      name = _("(Unnamed )");
#if wxCHECK_VERSION(3, 0, 0)
      name.append(_((*it)->m_sTypeString));
#else
      name.append((*it)->m_sTypeString);
#endif
    }
    m_listCtrlPath->SetItem(idx, colPATHNAME, name);
    wxString desc = (*it)->m_PathDescription;
    m_listCtrlPath->SetItem(idx, colPATHDESC, desc);
  }

  m_listCtrlPath->SortItems(SortPathOnName, (wxIntPtr)m_listCtrlPath);

  // restore selection if possible
  // NOTE this will select a different item, if one is deleted
  // (the next path will get that index).
  if (selected_id > -1) {
    for (int i = 0; i < i_itemcount; ++i) {
      m_listCtrlPath->SetItemState(l_selection[i], wxLIST_STATE_SELECTED,
                                   wxLIST_STATE_SELECTED);
    }
  }

  if ((m_lastPathItem >= 0) && (m_listCtrlPath->GetItemCount()))
    m_listCtrlPath->EnsureVisible(m_lastPathItem);
  UpdatePathButtons();

  for (int i = 0; i < m_listCtrlPath->GetColumnCount(); i++) {
    m_listCtrlPath->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    int h_width = m_listCtrlPath->GetColumnWidth(i);
    m_listCtrlPath->SetColumnWidth(i, wxLIST_AUTOSIZE);
    int a_width = m_listCtrlPath->GetColumnWidth(i);
    m_listCtrlPath->SetColumnWidth(i, (std::max)(a_width, h_width));
  }

  m_bSizerPathButtons->Layout();
  SetSizerAndFit(m_bSizerDialog);

  delete[] l_selection;
}

void PathAndPointManagerDialogImpl::UpdatePathButtons() {
  // enable/disable buttons
  long selected_index_index =
      m_listCtrlPath->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  bool enable1 = m_listCtrlPath->GetSelectedItemCount() == 1;
  bool enablemultiple = m_listCtrlPath->GetSelectedItemCount() > 1;

  m_lastPathItem = selected_index_index;

  m_buttonPathProperties->SetFont(*m_dialogLabelFont);
  m_buttonPathActivate->SetFont(*m_dialogLabelFont);
  m_buttonPathCenterView->SetFont(*m_dialogLabelFont);
  m_buttonPathDelete->SetFont(*m_dialogLabelFont);
  m_buttonPathExportSelected->SetFont(*m_dialogLabelFont);
  m_buttonPathDeleteAll->SetFont(*m_dialogLabelFont);

  m_buttonPathDelete->Enable(m_listCtrlPath->GetSelectedItemCount() > 0);
  m_buttonPathCenterView->Enable(enable1);
  m_buttonPathProperties->Enable(enable1);
  m_buttonPathDeleteAll->Enable(true);
  if (enable1 || enablemultiple) {
    m_buttonPathExportSelected->Enable(true);
  } else {
    m_buttonPathActivate->Enable(false);
    m_buttonPathExportSelected->Enable(false);
  }

  // activate button text
  ODPath* path = NULL;
  if (enable1 || enablemultiple) {
    path = reinterpret_cast<ODPath*>(
        m_listCtrlPath->GetItemData(selected_index_index));
    if (path) {
      m_buttonPathActivate->Enable(true);
      if (path->IsActive())
        m_buttonPathActivate->SetLabel(_("&Deactivate"));
      else
        m_buttonPathActivate->SetLabel(_("&Activate"));
    } else
      m_buttonPathActivate->Enable(false);
  } else
    m_buttonPathActivate->Enable(false);
}

void PathAndPointManagerDialogImpl::MakeAllPathsInvisible() {
  PathList::iterator it;
  long index = 0;
  for (it = (*g_pPathList).begin(); it != (*g_pPathList).end(); ++it, ++index) {
    if ((*it)->IsVisible()) {  // avoid config updating as much as possible!
      (*it)->SetVisible(false);
      m_listCtrlPath->SetItemImage(m_listCtrlPath->FindItem(-1, index),
                                   1);  // Likely not same order :0
      g_pODConfig->UpdatePath(*it);
    }
  }
}

void PathAndPointManagerDialogImpl::ZoomtoPath(ODPath* path) {
  // Calculate bbox center
  LLBBox RBBox = path->GetBBox();
  double clat =
      RBBox.GetMinLat() + ((RBBox.GetMaxLat() - RBBox.GetMinLat()) / 2);
  double clon =
      RBBox.GetMinLon() + ((RBBox.GetMaxLon() - RBBox.GetMinLon()) / 2);

  if (clon > 180.)
    clon -= 360.;
  else if (clon < -180.)
    clon += 360.;

  // Calculate ppm
  double rw, rh, ppm;  // path width, height, final ppm scale to use
  int ww, wh;          // chart window width, height
  // path bbox width in nm
  DistanceBearingMercator_Plugin(RBBox.GetMinLat(), RBBox.GetMinLon(),
                                 RBBox.GetMinLat(), RBBox.GetMaxLon(), NULL,
                                 &rw);
  // path bbox height in nm
  DistanceBearingMercator_Plugin(RBBox.GetMinLat(), RBBox.GetMinLon(),
                                 RBBox.GetMaxLat(), RBBox.GetMinLon(), NULL,
                                 &rh);

  m_parent_window->GetSize(&ww, &wh);

  ppm = wxMin(ww / (rw * 1852), wh / (rh * 1852)) * (100 - fabs(clat)) / 90;

  ppm = wxMin(ppm, 1.0);

  JumpToPosition(clat, clon, ppm);
}

void PathAndPointManagerDialogImpl::UpdatePointListCtrl(ODPoint* op_select,
                                                        bool b_retain_sort) {
  long selected_id = -1;
  long item = -1;

  if (NULL == op_select) {
    // if an item was selected, make it selected again if it still exists
    item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                           wxLIST_STATE_SELECTED);

    if (item != -1) selected_id = m_listCtrlODPoints->GetItemData(item);
  }

  //  Freshen the image list
  m_listCtrlODPoints->SetImageList(g_pODPointMan->Getpmarkicon_image_list(),
                                   wxIMAGE_LIST_SMALL);

  m_listCtrlODPoints->DeleteAllItems();

  wxODPointListNode* node = g_pODPointMan->GetODPointList()->GetFirst();

  long index = 0;
  while (node) {
    ODPoint* op = node->GetData();
    if (op && op->IsListed()) {
      if (op->m_bIsInPath) {
        if (!op->m_bKeepXPath) {
          node = node->GetNext();
          continue;
        }
      }

      wxListItem li;
      li.SetId(index);
      li.SetImage(op->IsVisible()
                      ? g_pODPointMan->GetIconIndex(op->GetIconBitmap())
                      : g_pODPointMan->GetXIconIndex(op->GetIconBitmap()));
      li.SetData(op);
      li.SetText(_T(""));
      long idx = m_listCtrlODPoints->InsertItem(li);

      wxString name = op->GetName();
      if (name.IsEmpty()) {
        name.append(_("(Unnamed) "));
#if wxCHECK_VERSION(3, 0, 0)
        name.append(_(op->m_sTypeString));
#else
        name.append(op->m_sTypeString);
#endif
      }
      m_listCtrlODPoints->SetItem(idx, colOCPNPOINTNAME, name);

      double dst;
      DistanceBearingMercator_Plugin(op->m_lat, op->m_lon, g_pfFix.Lat,
                                     g_pfFix.Lon, NULL, &dst);
      wxString dist;
      dist.Printf(_T("%5.2f ") + getUsrDistanceUnit_Plugin(),
                  toUsrDistance_Plugin(dst));
      m_listCtrlODPoints->SetItem(idx, colOCPNPOINTDIST, dist);

      if (op == op_select) selected_id = (long)op_select;  // index;

      index++;
    }

    node = node->GetNext();
  }

  if (!b_retain_sort) {
    m_listCtrlODPoints->SortItems(SortODPointsOnName, (long)m_listCtrlODPoints);
    sort_ODPoint_key = SORT_ON_NAME;
  } else {
    switch (sort_ODPoint_key) {
      case SORT_ON_NAME:
        m_listCtrlODPoints->SortItems(SortODPointsOnName,
                                      (long)m_listCtrlODPoints);
        break;
      case SORT_ON_DISTANCE:
        m_listCtrlODPoints->SortItems(SortODPointsOnDistance,
                                      (long)m_listCtrlODPoints);
        break;
    }
  }

  if (selected_id > -1) {
    item = m_listCtrlODPoints->FindItem(-1, selected_id);
    m_listCtrlODPoints->SetItemState(item, wxLIST_STATE_SELECTED,
                                     wxLIST_STATE_SELECTED);
  }

  if ((m_lastODPointItem >= 0) && (m_listCtrlODPoints->GetItemCount()))
    m_listCtrlODPoints->EnsureVisible(m_lastODPointItem);
  UpdateODPointButtons();

  for (int i = 0; i < m_listCtrlODPoints->GetColumnCount(); i++) {
    m_listCtrlODPoints->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    int h_width = m_listCtrlODPoints->GetColumnWidth(i);
    m_listCtrlODPoints->SetColumnWidth(i, wxLIST_AUTOSIZE);
    int a_width = m_listCtrlODPoints->GetColumnWidth(i);
    m_listCtrlODPoints->SetColumnWidth(i, (std::max)(a_width, h_width));
  }

  this->GetSizer()->Fit(this);
  this->Layout();
}

void PathAndPointManagerDialogImpl::UpdateODPointListCtrlViz() {
  long item = -1;
  for (;;) {
    item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                           wxLIST_STATE_DONTCARE);
    if (item == -1) break;

    ODPoint* pODP = (ODPoint*)m_listCtrlODPoints->GetItemData(item);
    int image = pODP->IsVisible()
                    ? g_pODPointMan->GetIconIndex(pODP->GetIconBitmap())
                    : g_pODPointMan->GetXIconIndex(pODP->GetIconBitmap());

    m_listCtrlODPoints->SetItemImage(item, image);
  }
}

void PathAndPointManagerDialogImpl::OnODPointDefaultAction(wxListEvent& event) {
  wxCommandEvent evt;
  OnODPointPropertiesClick(evt);
}

void PathAndPointManagerDialogImpl::OnODPointSelected(wxListEvent& event) {
  ODPoint* point = (ODPoint*)m_listCtrlODPoints->GetItemData(event.m_itemIndex);
  point->m_bPathManagerBlink = true;
  UpdateODPointButtons();
}

void PathAndPointManagerDialogImpl::OnODPointDeSelected(wxListEvent& event) {
  ODPoint* point = (ODPoint*)m_listCtrlODPoints->GetItemData(event.m_itemIndex);
  point->m_bPathManagerBlink = false;

  RequestRefresh(GetOCPNCanvasWindow());

  UpdateODPointButtons();
}

void PathAndPointManagerDialogImpl::OnODPointColumnClick(wxListEvent& event) {
  switch (event.m_col) {
    case 0:
      sort_ODPoint_on_vis++;
      m_listCtrlODPoints->SortItems(SortODPointsOnVis,
                                    (long)m_listCtrlODPoints);
      break;
    case 1:
      sort_ODPoint_name_dir++;
      m_listCtrlODPoints->SortItems(SortODPointsOnName,
                                    (long)m_listCtrlODPoints);
      break;
    case 2:
      sort_ODPoint_len_dir++;
      m_listCtrlODPoints->SortItems(SortODPointsOnDistance,
                                    (long)m_listCtrlODPoints);
      break;
  }
}

void PathAndPointManagerDialogImpl::UpdateODPointButtons() {
  long item = -1;
  item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                         wxLIST_STATE_SELECTED);
  bool enable1 = (m_listCtrlODPoints->GetSelectedItemCount() == 1);
  bool enablemultiple = (m_listCtrlODPoints->GetSelectedItemCount() >= 1);

  if (enable1)
    m_lastODPointItem = item;
  else
    m_lastODPointItem = -1;

  //  Check selection to see if it is in a layer
  //  If so, disable the "delete" button
  bool b_delete_enable = true;
  item = -1;
  for (;;) {
    item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                           wxLIST_STATE_SELECTED);
    if (item == -1) break;

    ODPoint* odp = (ODPoint*)m_listCtrlODPoints->GetItemData(item);

    if (odp && odp->m_bIsInLayer) {
      b_delete_enable = false;
      break;
    }
  }

  m_buttonODPointNew->SetFont(*m_dialogLabelFont);
  m_buttonODPointProperties->SetFont(*m_dialogLabelFont);
  m_buttonODPointCenterView->SetFont(*m_dialogLabelFont);
  m_buttonODPointDelete->SetFont(*m_dialogLabelFont);
  m_buttonODPointExportSelected->SetFont(*m_dialogLabelFont);
  m_buttonODPointDeleteAll->SetFont(*m_dialogLabelFont);

  m_buttonODPointProperties->Enable(enable1);
  m_buttonODPointCenterView->Enable(enable1);
  m_buttonODPointDeleteAll->Enable(TRUE);
  m_buttonODPointDelete->Enable(b_delete_enable && enablemultiple);
  if (enable1 || enablemultiple)
    m_buttonODPointExportSelected->Enable(true);
  else
    m_buttonODPointExportSelected->Enable(false);
}

void PathAndPointManagerDialogImpl::OnODPointToggleVisibility(
    wxMouseEvent& event) {
  wxPoint pos = event.GetPosition();
  int flags = 0;
  long clicked_index = m_listCtrlODPoints->HitTest(pos, flags);

  //    Clicking Visibility column?
  if (clicked_index > -1 &&
      event.GetX() < m_listCtrlODPoints->GetColumnWidth(colPATHVISIBLE)) {
    // Process the clicked item
    ODPoint* pODP = (ODPoint*)m_listCtrlODPoints->GetItemData(clicked_index);

    pODP->SetVisible(!pODP->IsVisible());
    m_listCtrlODPoints->SetItemImage(
        clicked_index,
        pODP->IsVisible()
            ? g_pODPointMan->GetIconIndex(pODP->GetIconBitmap())
            : g_pODPointMan->GetXIconIndex(pODP->GetIconBitmap()));

    g_pODConfig->UpdateODPoint(pODP);

    RequestRefresh(GetOCPNCanvasWindow());
  }

  // Allow wx to process...
  event.Skip();
}

void PathAndPointManagerDialogImpl::OnODPointNewClick(wxCommandEvent& event) {
  ODNewODPointDialogImpl* l_pType = new ODNewODPointDialogImpl(this);

#ifndef __WXOSX__
  DimeWindow(l_pType);
#endif
  l_pType->ShowModal();

  if (l_pType->m_bOK) {
    ODPoint* pODP = NULL;
    if (l_pType->m_iSelection == ID_ODNEWPOINTDIALOGBUTTON_BOUNDARY) {
      BoundaryPoint* pBP = new BoundaryPoint(g_dLat, g_dLon, g_sODPointIconName,
                                             wxEmptyString, wxT(""));
      pBP->m_bIsolatedMark = true;  // This is an isolated mark
      pODP = pBP;
    } else {
      TextPoint* pTP = new TextPoint(g_dLat, g_dLon, g_sODPointIconName,
                                     wxEmptyString, wxT(""));
      pODP = pTP;
    }
    g_pODSelect->AddSelectableODPoint(g_dLat, g_dLon, pODP);
    g_pODConfig->AddNewODPoint(pODP, -1);  // use auto next num

    RequestRefresh(GetOCPNCanvasWindow());

    ODPointShowPropertiesDialog(pODP, m_parent_window);
    UpdateODPointsListCtrl(pODP);
  }

  l_pType->Destroy();
}

void PathAndPointManagerDialogImpl::OnODPointPropertiesClick(
    wxCommandEvent& event) {
  long item = -1;
  item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                         wxLIST_STATE_SELECTED);
  if (item == -1) return;

  ODPoint* odp = (ODPoint*)m_listCtrlODPoints->GetItemData(item);

  if (!odp) return;

  ODPointShowPropertiesDialog(odp, m_parent_window);

  UpdateODPointsListCtrl();
}

void PathAndPointManagerDialogImpl::ODPointShowPropertiesDialog(
    ODPoint* odp, wxWindow* parent) {
  if (NULL == g_pODPointPropDialog)
    g_pODPointPropDialog = new ODPointPropertiesImpl(parent);

  g_pODPointPropDialog->SetODPoint(odp);
  g_pODPointPropDialog->UpdateProperties();

  DimeWindow(g_pODPointPropDialog);
  long l_lStyle = g_pODPointPropDialog->GetExtraStyle();
  g_pODPointPropDialog->SetExtraStyle(l_lStyle | wxSTAY_ON_TOP);
  if (!g_pODPointPropDialog->IsShown()) g_pODPointPropDialog->Show();
  g_pODPointPropDialog->Raise();
  g_pODPointPropDialog->SetExtraStyle(l_lStyle);
}

void PathAndPointManagerDialogImpl::OnODPointCenterViewClick(
    wxCommandEvent& event) {
  long item = -1;
  item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                         wxLIST_STATE_SELECTED);
  if (item == -1) return;

  ODPoint* odp = (ODPoint*)m_listCtrlODPoints->GetItemData(item);

  if (!odp) return;

  JumpToPosition(odp->m_lat, odp->m_lon, g_ocpn_draw_pi->m_view_scale);
}

void PathAndPointManagerDialogImpl::OnODPointDeleteClick(
    wxCommandEvent& event) {
  ODPointList list;

  int answer = OCPNMessageBox_PlugIn(
      this, _("Are you sure you want to delete the selected object(s)"),
      wxString(_("OpenCPN Alert")), wxYES_NO);
  if (answer != wxID_YES) return;

  bool busy = false;
  if (m_listCtrlODPoints->GetSelectedItemCount()) {
    busy = true;
  }

  long item = -1;
  long item_last_selected = -1;
  for (;;) {
    item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                           wxLIST_STATE_SELECTED);
    if (item == -1) break;

    item_last_selected = item;
    ODPoint* odp = (ODPoint*)m_listCtrlODPoints->GetItemData(item);

    if (odp && !odp->m_bIsInLayer) list.Append(odp);
  }

  if (busy) {
    for (unsigned int i = 0; i < list.GetCount(); i++) {
      ODPoint* odp = list.Item(i)->GetData();
      if (odp) {
        if (odp->m_bIsInPath) {
          if (wxYES == OCPNMessageBox_PlugIn(
                           this,
                           _("The OD Point you want to delete is used in a "
                             "path, do you really want to delete it?"),
                           _("OpenCPN Alert"), wxYES_NO))
            g_pODPointMan->DestroyODPoint(odp);
        } else
          g_pODPointMan->DestroyODPoint(odp);
      }
    }

    long item_next =
        m_listCtrlODPoints->GetNextItem(item_last_selected);  // next in list
    ODPoint* odp_next = NULL;
    if (item_next > -1)
      odp_next = (ODPoint*)m_listCtrlODPoints->GetItemData(item_next);

    m_lastODPointItem = item_next;

    UpdatePathListCtrl();
    UpdateODPointsListCtrl(odp_next, true);

    // TODO fix up undo
    // m_parent_window->undo->InvalidateUndo();
    RequestRefresh(GetOCPNCanvasWindow());
  }
}

void PathAndPointManagerDialogImpl::OnODPointExportSecectedClick(
    wxCommandEvent& event) {
  ODPointList list;

  wxString suggested_name = _T("OD Points");

  long item = -1;
  for (;;) {
    item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                           wxLIST_STATE_SELECTED);
    if (item == -1) break;

    ODPoint* odp = (ODPoint*)m_listCtrlODPoints->GetItemData(item);

    if (odp && !odp->m_bIsInLayer) {
      list.Append(odp);
      if (odp->GetName() != wxEmptyString) suggested_name = odp->GetName();
    }
  }

  g_pODConfig->ExportGPXODPoints(this, &list, suggested_name);
}

void PathAndPointManagerDialogImpl::OnODPointDeleteAllClick(
    wxCommandEvent& event) {
  wxString prompt;
  int buttons, type;
  if (!g_pODPointMan->SharedODPointsExist()) {
    prompt = _("Are you sure you want to delete <ALL> OD points?");
    buttons = wxYES_NO;
    type = 1;
  } else {
    prompt =
        _("There are some OD points used in paths or anchor alarms.\n Do you "
          "want to delete them as well?\n This will change the paths and "
          "disable the anchor alarms.\n Answering No keeps the OD points used "
          "in paths or alarms.");
    buttons = wxYES_NO | wxCANCEL;
    type = 2;
  }
  int answer = OCPNMessageBox_PlugIn(this, prompt, _("OpenCPN Alert"), buttons);
  if (answer == wxID_YES && type == 1)
    g_pODPointMan->DeleteAllODPoints(true);
  else if (answer == wxID_NO && type == 2)
    g_pODPointMan->DeleteAllODPoints(false);  // only delete unused OD points
  else
    return;

  m_lastODPointItem = -1;
  UpdatePathListCtrl();
  UpdateODPointsListCtrl();
  // m_parent_window->undo->InvalidateUndo();
  RequestRefresh(GetOCPNCanvasWindow());
}

void PathAndPointManagerDialogImpl::SelectedODPointToggleVisibility(
    bool visible) {
  long item = -1;
  for (;;) {
    item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                           wxLIST_STATE_SELECTED);
    if (item == -1) break;

    wxListItem li;
    li.SetId(item);
    m_listCtrlODPoints->GetItem(li);
    ODPoint* ppoint = (ODPoint*)li.GetData();

    if (ppoint) {
      ppoint->SetVisible(visible);
    }
    m_listCtrlODPoints->SetItemImage(
        item, ppoint->IsVisible()
                  ? g_pODPointMan->GetIconIndex(ppoint->GetIconBitmap())
                  : g_pODPointMan->GetXIconIndex(ppoint->GetIconBitmap()));
  }
}

void PathAndPointManagerDialogImpl::OnODPointRightClick(wxListEvent& event) {
  g_ODEventHandler->SetWindow(m_parent_window);
  g_ODEventHandler->SetPath(NULL);
  g_ODEventHandler->SetPoint((ODPoint*)NULL);
  g_ODEventHandler->PopupMenu(TYPE_PATHMGR_POINT_DLG);
}

void PathAndPointManagerDialogImpl::OnLayerSelected(wxListEvent& event) {
  UpdateLayerButtons();
}

void PathAndPointManagerDialogImpl::OnLayerColumnClick(wxListEvent& event) {
  switch (event.m_col) {
    case 0:
      sort_layer_on_vis++;
      m_listCtrlLayers->SortItems(SortLayersOnVis, (long)m_listCtrlLayers);
      break;
    case 1:
      sort_layer_name_dir++;
      m_listCtrlLayers->SortItems(SortLayersOnName, (long)m_listCtrlLayers);
      break;
    case 2:
      sort_layer_len_dir++;
      m_listCtrlLayers->SortItems(SortLayersOnSize, (long)m_listCtrlLayers);
      break;
  }
}

void PathAndPointManagerDialogImpl::UpdateLayerButtons() {
  long item = -1;
  item = m_listCtrlLayers->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);
  bool enable = (item != -1);

  m_buttonLayerTemporary->SetFont(*m_dialogLabelFont);
  m_buttonLayerPersistent->SetFont(*m_dialogLabelFont);
  m_buttonLayerDelete->SetFont(*m_dialogLabelFont);
  m_buttonLayerShowOnChart->SetFont(*m_dialogLabelFont);
  m_buttonLayerListContents->SetFont(*m_dialogLabelFont);
  m_buttonLayerShowPointNames->SetFont(*m_dialogLabelFont);

  // btnLayProperties->Enable(false);
  m_buttonLayerDelete->Enable(enable);
  m_buttonLayerShowOnChart->Enable(enable);
  m_buttonLayerListContents->Enable(enable);
  m_buttonLayerShowPointNames->Enable(enable);

  if (item >= 0) {
    ODLayer* layer;
    layer = reinterpret_cast<ODLayer*>(m_listCtrlLayers->GetItemData(item));
    if (layer->IsVisible())
      m_buttonLayerShowOnChart->SetLabel(_("Hide from chart"));
    else
      m_buttonLayerShowOnChart->SetLabel(_("Show on chart"));

    if (layer->HasVisibleNames())
      m_buttonLayerShowPointNames->SetLabel(_("Hide Point names"));
    else
      m_buttonLayerShowPointNames->SetLabel(_("Show Point names"));

    if (layer->IsVisibleOnListing())
      m_buttonLayerListContents->SetLabel(_("Unlist contents "));
    else
      m_buttonLayerListContents->SetLabel(_("List contents "));
  } else {
    m_buttonLayerShowOnChart->SetLabel(_("Show on chart"));
    m_buttonLayerShowPointNames->SetLabel(_("Show Point names"));
    m_buttonLayerListContents->SetLabel(_("List contents "));
  }
  SetSizerAndFit(m_bSizerDialog);
}

void PathAndPointManagerDialogImpl::OnLayerToggleVisibility(
    wxMouseEvent& event) {
  wxPoint pos = event.GetPosition();
  int flags = 0;
  long clicked_index = m_listCtrlLayers->HitTest(pos, flags);

  //    Clicking Visibility column?
  if (clicked_index > -1 &&
      event.GetX() < m_listCtrlLayers->GetColumnWidth(colLAYVISIBLE)) {
    // Process the clicked item
    ODLayer* layer = (ODLayer*)m_listCtrlLayers->GetItemData(clicked_index);

    layer->SetVisible(!layer->IsVisible());
    m_listCtrlLayers->SetItemImage(clicked_index, layer->IsVisible() ? 0 : 1);

    ToggleLayerContentsOnChart(layer);
  }

  // Allow wx to process...
  event.Skip();
}

void PathAndPointManagerDialogImpl::OnLayerTemporaryClick(
    wxCommandEvent& event) {
  CreateLayer(true);
}

void PathAndPointManagerDialogImpl::OnLayerPersistentClick(
    wxCommandEvent& event) {
  CreateLayer(false);
}

void PathAndPointManagerDialogImpl::CreateLayer(bool isTemporary) {
  bool show_flag = g_bShowLayers;
  g_bShowLayers = true;

#ifdef __WXOSX__
  HideWithEffect(wxSHOW_EFFECT_BLEND);
#endif

  g_pODConfig->UI_Import(this, true, isTemporary);

#ifdef __WXOSX__
  ShowWithEffect(wxSHOW_EFFECT_BLEND);
#endif

  g_bShowLayers = show_flag;

  UpdatePathListCtrl();
  UpdateODPointsListCtrl();
  UpdateLayerListCtrl();
  RequestRefresh(GetOCPNCanvasWindow());
}

void PathAndPointManagerDialogImpl::OnLayerDeleteClick(wxCommandEvent& event) {
  long item = -1;
  int l_answer = wxID_YES;
  ODLayerList::compatibility_iterator* l_LayerIterator =
      new ODLayerList::compatibility_iterator[m_listCtrlLayers
                                                  ->GetSelectedItemCount()];
  int l_item_count = 0;

  item =
      m_listCtrlLayers->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  while (item != -1) {
    ODLayer* layer =
        reinterpret_cast<ODLayer*>(m_listCtrlLayers->GetItemData(item));

    if (layer) {
      if (g_bConfirmObjectDelete) {
        wxString prompt =
            _("Are you sure you want to delete this layer and <ALL> of its "
              "contents?");
        if (layer->m_LayerName != wxEmptyString) {
          prompt.Append(_T("\n"));
          prompt.Append(
              _("The file will also be deleted from OCPN Draw's layers "
                "directory."));
          prompt.Append(_T("\n (") + layer->m_LayerName + _T(")" ));
        }
        l_answer =
            OCPNMessageBox_PlugIn(this, prompt, _("OpenCPN Alert"), wxYES_NO);
      }

      if (l_answer == wxID_YES) {
        // if layer is permanent delete file
        if (wxFileExists(layer->m_LayerFileName))
          wxRemoveFile(layer->m_LayerFileName);

        // Process Paths in this layer
        wxPathListNode* node1 = g_pPathList->GetFirst();
        wxPathListNode* node2;
        while (node1) {
          ODPath* pPath = node1->GetData();
          node2 = node1->GetNext();
          if (pPath->m_bIsInLayer && (pPath->m_LayerID == layer->m_LayerID)) {
            pPath->m_bIsInLayer = false;
            pPath->m_LayerID = 0;
            g_pPathMan->DeletePath(pPath);
          }
          node1 = node2;
          node2 = NULL;
        }

        // Process ODPoints in this layer
        wxODPointListNode* node = g_pODPointMan->GetODPointList()->GetFirst();
        wxODPointListNode* node3;

        while (node) {
          node3 = node->GetNext();
          ODPoint* pp = node->GetData();
          if (pp && (pp->m_LayerID == layer->m_LayerID)) {
            pp->m_bIsInLayer = false;
            pp->m_LayerID = 0;
            g_pODPointMan->DestroyODPoint(
                pp, false);  // no need to update the change set on layer ops
            delete pp;
          }

          node = node3;
          node3 = NULL;
        }

        l_LayerIterator[l_item_count++] = g_pLayerList->Find(layer);
      }
      l_answer = wxID_YES;
    }
    item = m_listCtrlLayers->GetNextItem(item, wxLIST_NEXT_ALL,
                                         wxLIST_STATE_SELECTED);
  }

  while (l_item_count > 0) {
    g_pLayerList->DeleteNode(l_LayerIterator[--l_item_count]);
  }

  UpdatePathListCtrl();
  UpdateODPointsListCtrl();
  UpdateLayerListCtrl();

  RequestRefresh(GetOCPNCanvasWindow());
}

void PathAndPointManagerDialogImpl::SelectedLayerToggleVisibility(
    bool visible) {
  long item = -1;
  for (;;) {
    item = m_listCtrlLayers->GetNextItem(item, wxLIST_NEXT_ALL,
                                         wxLIST_STATE_SELECTED);
    if (item == -1) break;

    ODLayer* player =
        reinterpret_cast<ODLayer*>(m_listCtrlLayers->GetItemData(item));

    if (player) {
      player->SetVisible(visible);
    }
    m_listCtrlLayers->SetItemImage(item, player->IsVisible() ? 0 : 1);
    ToggleLayerContentsOnChart(player);
  }
}

void PathAndPointManagerDialogImpl::OnLayerRightClick(wxListEvent& event) {
  g_ODEventHandler->SetWindow(m_parent_window);
  g_ODEventHandler->SetPath(NULL);
  g_ODEventHandler->SetPoint((ODPoint*)NULL);
  g_ODEventHandler->PopupMenu(TYPE_PATHMGR_LAYER_DLG);
}

void PathAndPointManagerDialogImpl::OnLayerShowOnChartClick(
    wxCommandEvent& event) {
  // Toggle  visibility on chart for selected layer
  long item = -1;
  item = m_listCtrlLayers->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);
  if (item == -1) return;

  ODLayer* player =
      reinterpret_cast<ODLayer*>(m_listCtrlLayers->GetItemData(item));

  if (!player) return;

  player->SetVisible(!player->IsVisible());
  m_listCtrlLayers->SetItemImage(item, player->IsVisible() ? 0 : 1);

  ToggleLayerContentsOnChart(player);
}

void PathAndPointManagerDialogImpl::ToggleLayerContentsOnChart(ODLayer* layer) {
  // Process Paths in this layer
  wxPathListNode* node1 = g_pPathList->GetFirst();
  while (node1) {
    ODPath* pPath = node1->GetData();
    if (pPath->m_bIsInLayer && (pPath->m_LayerID == layer->m_LayerID)) {
      pPath->SetVisible(layer->IsVisible());
      g_pODConfig->UpdatePath(pPath);
    }
    node1 = node1->GetNext();
  }

  // Process OD points in this layer
  wxODPointListNode* node = g_pODPointMan->GetODPointList()->GetFirst();

  while (node) {
    ODPoint* pp = node->GetData();
    if (pp && !pp->m_bIsInPath && (pp->m_LayerID == layer->m_LayerID)) {
      pp->SetVisible(layer->IsVisible());
    }

    node = node->GetNext();
  }

  UpdatePathListCtrl();
  UpdateODPointsListCtrl();
  UpdateLayerListCtrl();
  UpdateLayerButtons();

  RequestRefresh(GetOCPNCanvasWindow());
}

void PathAndPointManagerDialogImpl::OnLayerShowPointNamesClick(
    wxCommandEvent& event) {
  // Toggle Point names visibility on chart for selected layer
  long item = -1;
  item = m_listCtrlLayers->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);
  if (item == -1) return;

  ODLayer* layer =
      reinterpret_cast<ODLayer*>(m_listCtrlLayers->GetItemData(item));

  if (!layer) return;

  layer->SetVisibleNames(!layer->HasVisibleNames());

  ToggleLayerContentsNames(layer);
}

void PathAndPointManagerDialogImpl::ToggleLayerContentsNames(ODLayer* layer) {
  // Process Paths in this layer
  wxPathListNode* node1 = g_pPathList->GetFirst();
  while (node1) {
    ODPath* pPath = node1->GetData();
    if (pPath->m_bIsInLayer && (pPath->m_LayerID == layer->m_LayerID)) {
      wxODPointListNode* node = pPath->m_pODPointList->GetFirst();
      ODPoint* podp1 = node->GetData();
      while (node) {
        podp1->m_bShowName = layer->HasVisibleNames();
        node = node->GetNext();
      }
    }
    node1 = node1->GetNext();
  }

  // Process OD points in this layer
  wxODPointListNode* node = g_pODPointMan->GetODPointList()->GetFirst();

  while (node) {
    ODPoint* pp = node->GetData();
    if (pp && (pp->m_LayerID == layer->m_LayerID)) {
      pp->SetNameShown(layer->HasVisibleNames());
    }

    node = node->GetNext();
  }

  UpdateLayerButtons();

  RequestRefresh(GetOCPNCanvasWindow());
}

void PathAndPointManagerDialogImpl::OnLayerListContentsClick(
    wxCommandEvent& event) {
  // Toggle  visibility on listing for selected layer
  long item = -1;
  item = m_listCtrlLayers->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);
  if (item == -1) return;

  ODLayer* layer =
      reinterpret_cast<ODLayer*>(m_listCtrlLayers->GetItemData(item));

  if (!layer) return;

  layer->SetVisibleOnListing(!layer->IsVisibleOnListing());

  ToggleLayerContentsOnListing(layer);
}

void PathAndPointManagerDialogImpl::ToggleLayerContentsOnListing(
    ODLayer* layer) {
  // Process Paths in this layer
  wxPathListNode* node1 = g_pPathList->GetFirst();
  while (node1) {
    ODPath* pPath = node1->GetData();
    if (pPath->m_bIsInLayer && (pPath->m_LayerID == layer->m_LayerID)) {
      pPath->SetListed(layer->IsVisibleOnListing());
      g_pODConfig->UpdatePath(pPath);
    }
    node1 = node1->GetNext();
  }

  // Process OD points in this layer
  //  n.b.  If the OD point belongs to a track, and is not shared, then do not
  //  list it. This is a performance optimization, allowing large track support.

  wxODPointListNode* node = g_pODPointMan->GetODPointList()->GetFirst();

  while (node) {
    ODPoint* op = node->GetData();
    if (op->m_bIsolatedMark && (op->m_LayerID == layer->m_LayerID)) {
      op->SetListed(layer->IsVisibleOnListing());
    }

    node = node->GetNext();
  }

  UpdatePathListCtrl();
  UpdateODPointsListCtrl();
  UpdateLayerListCtrl();

  RequestRefresh(GetOCPNCanvasWindow());
}

void PathAndPointManagerDialogImpl::OnLayerDefaultAction(wxListEvent& event) {
  wxCommandEvent evt;
}

void PathAndPointManagerDialogImpl::UpdateLayerListCtrl() {
  // if an item was selected, make it selected again if it still exist
  long item = -1;
  item = m_listCtrlLayers->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);

  long selected_id = -1;
  if (item != -1) selected_id = m_listCtrlLayers->GetItemData(item);

  // Delete existing items
  m_listCtrlLayers->DeleteAllItems();

  // then add paths to the listctrl
  ODLayerList::iterator it;
  int index = 0;
  for (it = (*g_pLayerList).begin(); it != (*g_pLayerList).end();
       ++it, ++index) {
    wxListItem li;
    li.SetId(index);
    li.SetImage((*it)->IsVisible() ? 0 : 1);
    li.SetData(*it);
    li.SetText(_T(""));

    long idx = m_listCtrlLayers->InsertItem(li);

    wxString name = (*it)->m_LayerName;
    if (name.IsEmpty()) {
      name = _("(Unnamed Layer)");
    }
    m_listCtrlLayers->SetItem(idx, colLAYNAME, name);

    wxString len;
    len.Printf(wxT("%d"), (*it)->m_NoOfItems);
    m_listCtrlLayers->SetItem(idx, colLAYITEMS, len);
  }

  m_listCtrlLayers->SortItems(SortLayersOnName, (long)m_listCtrlLayers);

  // restore selection if possible
  // NOTE this will select a different item, if one is deleted
  // (the next path will get that index).
  if (selected_id > -1) {
    item = m_listCtrlLayers->FindItem(-1, selected_id);
    m_listCtrlLayers->SetItemState(item, wxLIST_STATE_SELECTED,
                                   wxLIST_STATE_SELECTED);
  }

  UpdateLayerButtons();

  for (int i = 0; i < m_listCtrlLayers->GetColumnCount(); i++) {
    m_listCtrlLayers->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    int h_width = m_listCtrlLayers->GetColumnWidth(i);
    m_listCtrlLayers->SetColumnWidth(i, wxLIST_AUTOSIZE);
    int a_width = m_listCtrlLayers->GetColumnWidth(i);
    m_listCtrlLayers->SetColumnWidth(i, (std::max)(a_width, h_width));
  }

  m_bSizerLayerButtons->Layout();
  SetSizerAndFit(m_bSizerDialog);
}

void PathAndPointManagerDialogImpl::OnImportClick(wxCommandEvent& event) {
#ifdef __WXOSX__
  HideWithEffect(wxSHOW_EFFECT_BLEND);
#endif
  wxString l_sLabel = _("I&mport");
  l_sLabel.Append(_T(" "));
  g_pODConfig->UI_Import(this);
  l_sLabel.Append(g_pODConfig->m_sImport_Type);
  l_sLabel.Append(_T("..."));
  m_buttonImport->SetLabel(l_sLabel);

#ifdef __WXOSX__
  ShowWithEffect(wxSHOW_EFFECT_BLEND);
#endif

  UpdatePathListCtrl();
  UpdateODPointsListCtrl();
  UpdateLayerListCtrl();

  RequestRefresh(GetOCPNCanvasWindow());
}

void PathAndPointManagerDialogImpl::OnExportAllVisibleClick(
    wxCommandEvent& event) {
  g_pODConfig->ExportGPX(this, true,
                         true);  // only visible objects, layers included
}

void PathAndPointManagerDialogImpl::OnOKClick(wxCommandEvent& event) {
  g_ocpn_draw_pi->OnToolbarToolDownCallback(g_ocpn_draw_pi->m_config_button_id);
  DeSelectODPoints();
  DeSelectPaths();
  m_szDialogSize = GetSize();
}

void PathAndPointManagerDialogImpl::OnClose(wxCloseEvent& event) {
  g_ocpn_draw_pi->OnToolbarToolDownCallback(g_ocpn_draw_pi->m_config_button_id);
  DeSelectODPoints();
  DeSelectPaths();
}

void PathAndPointManagerDialogImpl::DeSelectPaths(void) {
  long selected_item = -1;

  for (;;) {
    selected_item = m_listCtrlPath->GetNextItem(selected_item, wxLIST_NEXT_ALL,
                                                wxLIST_STATE_SELECTED);
    if (selected_item == -1) break;
    m_listCtrlPath->SetItemState(selected_item, 0, wxLIST_STATE_SELECTED);
  }
  Show(false);
}

void PathAndPointManagerDialogImpl::DeSelectODPoints(void) {
  long selected_item = -1;

  for (;;) {
    selected_item = m_listCtrlODPoints->GetNextItem(
        selected_item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selected_item == -1) break;
    m_listCtrlODPoints->SetItemState(selected_item, 0, wxLIST_STATE_SELECTED);
  }
  Show(false);
}

wxString PathAndPointManagerDialogImpl::GetLayerName(int id) {
  wxString name(_T("unknown layer"));
  if (id <= 0) return (name);
  ODLayerList::iterator it;
  int index = 0;
  for (it = (*g_pLayerList).begin(); it != (*g_pLayerList).end();
       ++it, ++index) {
    ODLayer* lay = (ODLayer*)(*it);
    if (lay->m_LayerID == id) return (lay->m_LayerName);
  }
  return (name);
}

void PathAndPointManagerDialogImpl::UpdateODPointsListCtrl(ODPoint* op_select,
                                                           bool b_retain_sort) {
  long selected_id = -1;
  long item = -1;

  if (NULL == op_select) {
    // if an item was selected, make it selected again if it still exists
    item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                           wxLIST_STATE_SELECTED);

    if (item != -1) selected_id = m_listCtrlODPoints->GetItemData(item);
  }

  //  Freshen the image list
  m_listCtrlODPoints->SetImageList(g_pODPointMan->Getpmarkicon_image_list(),
                                   wxIMAGE_LIST_SMALL);

  m_listCtrlODPoints->DeleteAllItems();

  wxODPointListNode* node = g_pODPointMan->GetODPointList()->GetFirst();

  long index = 0;
  while (node) {
    ODPoint* op = node->GetData();
    if (op && op->IsListed()) {
      if (op->m_bIsInPath) {
        if (!op->m_bKeepXPath) {
          node = node->GetNext();
          continue;
        }
      }

      wxListItem li;
      li.SetId(index);
      li.SetImage(op->IsVisible()
                      ? g_pODPointMan->GetIconIndex(op->GetIconBitmap())
                      : g_pODPointMan->GetXIconIndex(op->GetIconBitmap()));
      li.SetData(op);
      li.SetText(_T(""));
      long idx = m_listCtrlODPoints->InsertItem(li);

      wxString name = op->GetName();
      if (name.IsEmpty()) {
        name.append(_("(Unnamed) "));
#if wxCHECK_VERSION(3, 0, 0)
        name.append(_(op->m_sTypeString));
#else
        name.append(op->m_sTypeString);
#endif
      }
      m_listCtrlODPoints->SetItem(idx, colOCPNPOINTNAME, name);

      double dst;
      DistanceBearingMercator_Plugin(op->m_lat, op->m_lon, g_pfFix.Lat,
                                     g_pfFix.Lon, NULL, &dst);
      wxString dist;
      dist.Printf(_T("%5.2f ") + getUsrDistanceUnit_Plugin(),
                  toUsrDistance_Plugin(dst));
      m_listCtrlODPoints->SetItem(idx, colOCPNPOINTDIST, dist);

      if (op == op_select) selected_id = (long)op_select;  // index;

      index++;
    }

    node = node->GetNext();
  }

  if (!b_retain_sort) {
    m_listCtrlODPoints->SortItems(SortODPointsOnName,
                                  (wxIntPtr)m_listCtrlODPoints);
    sort_ODPoint_key = SORT_ON_NAME;
  } else {
    switch (sort_ODPoint_key) {
      case SORT_ON_NAME:
        m_listCtrlODPoints->SortItems(SortODPointsOnName,
                                      (long)m_listCtrlODPoints);
        break;
      case SORT_ON_DISTANCE:
        m_listCtrlODPoints->SortItems(SortODPointsOnDistance,
                                      (long)m_listCtrlODPoints);
        break;
    }
  }

  if (selected_id > -1) {
    item = m_listCtrlODPoints->FindItem(-1, selected_id);
    m_listCtrlODPoints->SetItemState(item, wxLIST_STATE_SELECTED,
                                     wxLIST_STATE_SELECTED);
  }

  if ((m_lastODPointItem >= 0) && (m_listCtrlODPoints->GetItemCount()))
    m_listCtrlODPoints->EnsureVisible(m_lastODPointItem);
  UpdateODPointButtons();

  for (int i = 0; i < m_listCtrlODPoints->GetColumnCount(); i++) {
    m_listCtrlODPoints->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    int h_width = m_listCtrlODPoints->GetColumnWidth(i);
    m_listCtrlODPoints->SetColumnWidth(i, wxLIST_AUTOSIZE);
    int a_width = m_listCtrlODPoints->GetColumnWidth(i);
    m_listCtrlODPoints->SetColumnWidth(i, (std::max)(a_width, h_width));
  }

  SetSizerAndFit(m_bSizerDialog);
}

void PathAndPointManagerDialogImpl::UpdateODPointsListCtrlViz() {
  long item = -1;
  for (;;) {
    item = m_listCtrlODPoints->GetNextItem(item, wxLIST_NEXT_ALL,
                                           wxLIST_STATE_DONTCARE);
    if (item == -1) break;

    ODPoint* pODP = (ODPoint*)m_listCtrlODPoints->GetItemData(item);
    int image = pODP->IsVisible()
                    ? g_pODPointMan->GetIconIndex(pODP->GetIconBitmap())
                    : g_pODPointMan->GetXIconIndex(pODP->GetIconBitmap());

    m_listCtrlODPoints->SetItemImage(item, image);
  }
}

void PathAndPointManagerDialogImpl::RecalculateSize() {
  //  All of this dialog layout is expandable, so we need to set a specific size
  //  target for the onscreen display. The size will then be adjusted so that it
  //  fits within the parent's client area, with some padding

  //  Get a text height metric for reference

  int char_width, char_height;
  GetTextExtent(_T("W"), &char_width, &char_height, NULL, NULL,
                GetOCPNScaledFont_PlugIn(wxS("Dialog"), 0));

  wxSize sz;
  sz.x = 60 * char_width;
  sz.y = 30 * char_height;

  wxSize dsize = GetParent()->GetClientSize();
  sz.y = wxMin(sz.y, dsize.y - (0 * char_height));
  sz.x = wxMin(sz.x, dsize.x - (0 * char_height));
  SetClientSize(sz);

  wxSize fsize = GetSize();
  fsize.y = wxMin(fsize.y, dsize.y - (0 * char_height));
  fsize.x = wxMin(fsize.x, dsize.x - (0 * char_height));
  SetSize(fsize);

  CentreOnParent();
}
