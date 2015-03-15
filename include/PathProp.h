/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Boundary Properties Support
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

#ifndef PATHPROP_H
#define PATHPROP_H

#include <wx/clrpicker.h>
#include <wx/listbase.h>
#include <wx/listctrl.h>

#include "ocpn_types.h"
#include "Path.h"


/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PATHPROP                     9000
#define SYMBOL_PATHPROP_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PATHPROP_TITLE _("Path Properties")
#define SYMBOL_PATHPROP_IDNAME ID_PATHPROP
#define SYMBOL_PATHPROP_SIZE wxSize(450, 300)
#define SYMBOL_PATHPROP_POSITION wxDefaultPosition

#define ID_PATHPROP_TEXTCTRL            9001
#define ID_PATHPROP_TEXTCTRL2           9002
#define ID_PATHPROP_TEXTCTRL1           9003
#define ID_PATHPROP_TEXTCTRL3           9005
#define ID_PATHPROP_LISTCTRL            9004
#define ID_PATHPROP_CANCEL              9006
#define ID_PATHPROP_OK                  9007
#define ID_PATHPROP_SPLIT               9008
#define ID_PATHPROP_EXTEND              9009
#define ID_PATHPROP_COPYTXT             9010
#define ID_PATHPROP_PRINT               9011
#define ID_PATHPROP_TEXTCTRL4           9012
#define ID_PATHPROP_TEXTCTRLDESC        9013
#define ID_PATHPROP_STARTTIMECTL        9014
#define ID_PATHPROP_ACTIVE              9015
#define ID_PATHPROP_MENU_COPY_TEXT      9016
#define ID_PATHPROP_MENU_EDIT_WP        9017
#define ID_PATHPROP_MENU_DELETE         9018


class PathProp: public wxDialog
{
    DECLARE_DYNAMIC_CLASS( PathProp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    PathProp( );
    PathProp(wxWindow* parent, wxWindowID id = SYMBOL_PATHPROP_IDNAME,
        const wxString& caption = SYMBOL_PATHPROP_TITLE,
        const wxPoint& pos = SYMBOL_PATHPROP_POSITION,
        const wxSize& size = SYMBOL_PATHPROP_SIZE,
        long style = SYMBOL_PATHPROP_STYLE );
    ~PathProp( );

    void CreateControls();

    void SetColorScheme(ColorScheme cs);
    void SetDialogTitle(const wxString & title);
    void OnPathPropCancelClick( wxCommandEvent& event );
    void OnPathPropOkClick( wxCommandEvent& event );
    void OnPathPropListClick( wxListEvent& event );
//    void OnRoutepropPrintClick( wxCommandEvent& event );
//    void OnRoutepropCopyTxtClick( wxCommandEvent& event );
    void OnPathPropRightClick( wxListEvent &event );
    void OnPathPropMenuSelected( wxCommandEvent &event );
//    bool IsThisRouteExtendable();
    void OnEvtColDragEnd(wxListEvent& event);
    void InitializeList();


    /// Should we show tooltips?
    static bool ShowToolTips();

    void SetPathAndUpdate(Path *pB, bool only_points = false);
    Path *GetPath(void){return m_pPath;}

    bool UpdateProperties( Path *pPath );
    bool UpdateProperties();
    wxString MakeTideInfo(int jx, time_t tm, int tz_selection, long LMT_Offset);
    bool SaveChanges(void);

    wxTextCtrl  *m_TotalDistCtl;

    wxTextCtrl  *m_PathNameCtl;
    wxTextCtrl  *m_PathStartCtl;
    wxTextCtrl  *m_PathDestCtl;
    wxTextCtrl  *m_textDescription;

    wxCheckBox  *m_pPathActive;

    wxListCtrl  *m_wpList;

    wxButton*     m_CancelButton;
    wxButton*     m_OKButton;
    wxButton*     m_CopyTxtButton;

    Path     *m_pPath;
    OCPNPoint *m_pExtendPoint;
    Path    *m_pExtendPath;
    OCPNPoint    *m_pEnroutePoint;
    bool          m_bStartNow;

    int         m_nSelected; // index of point selected in Properties dialog row
    int         m_tz_selection;

    wxDateTime	 m_starttime; // kept as UTC
//    wxRadioBox	*pDispTz;
    wxStaticText  *m_staticText1;
    wxStaticText  *m_staticText2;
    wxStaticText  *m_staticText3;
    wxStaticText  *m_staticText4;
    wxChoice      *m_chColor;
    wxChoice      *m_chLineColor;
    wxChoice      *m_chStyle;
    wxChoice      *m_chWidth;

    wxStaticBoxSizer* m_pListSizer;
    wxScrolledWindow *itemDialog1;

private:
    int GetTZSelection(void);
    wxRadioButton  *m_prb_tzUTC;
    wxRadioButton  *m_prb_tzLocal;
    wxRadioButton  *m_prb_tzLMT;

};


#endif // PATHPROP_H
