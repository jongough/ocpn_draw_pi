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

#ifndef ODICONCOMBO_H
#define ODICONCOMBO_H

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include <wx/odcombo.h>
// forward definitions

WX_DECLARE_OBJARRAY(wxBitmap,      ArrayOfBitmaps);


class  ODIconCombo : public wxOwnerDrawnComboBox
{
public:
    
    ODIconCombo(wxWindow* parent, wxWindowID id, const wxString& value = _T(""),
                  const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                  int n = 0, const wxString choices[] = NULL,
                  long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name = _T("OCPNIconCombo") );
    
    
    
    ~ODIconCombo ();
    
    void OnDrawItem(wxDC& dc, const wxRect& rect, int item, int flags) const;
    wxCoord OnMeasureItem(size_t item) const;
    wxCoord OnMeasureItemWidth(size_t item) const;
    
    int Append(const wxString& item, wxBitmap bmp);
    void Clear( void );
    
private:
    int         itemHeight;
    ArrayOfBitmaps  bmpArray;
    
};

WX_DECLARE_LIST(wxBitmap, BitmapList);



#endif // ODICONCOMBO_H
