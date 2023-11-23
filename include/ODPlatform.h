/***************************************************************************
 *
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2013 by David S. Register                               *
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

#ifndef ODPLATFORM_H
#define ODPLATFORM_H

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include <wx/log.h>
#include <wx/stdpaths.h>
#include <wx/clrpicker.h>
#include <wx/colourdata.h>
#include <wx/colordlg.h>

#include <stdio.h>
#include <vector>


//--------------------------------------------------------------------------
//      Private colourPicker control
//--------------------------------------------------------------------------

class ODColourPickerCtrl : public wxBitmapButton
{
public:
    ODColourPickerCtrl();
    ODColourPickerCtrl(wxWindow *parent,
                   wxWindowID id,
                   const wxBitmap &bitmap = wxNullBitmap,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = _T(""));
    ~ODColourPickerCtrl();

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxBitmap &bitmap = wxNullBitmap,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = _T(""));

    void OnButtonClick(wxCommandEvent& WXUNUSED(ev));
    void InitColourData();
    void SetColour(const wxColour& c);
    wxColour GetColour( void );

protected:
    virtual void UpdateColour();
    wxSize DoGetBestSize() const;

    void OnPaint(wxPaintEvent &event) ;

    DECLARE_EVENT_TABLE() ;

private:
    wxBitmap m_bitmap;
    wxColour m_colour;
    wxColourData ms_data;

};





#endif          //guard
