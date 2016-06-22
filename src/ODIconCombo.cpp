/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  OD Icon Combobox
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

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ODIconCombo.h"
#include "ocpn_plugin.h"

#include <wx/listimpl.cpp>
#include <wx/arrimpl.cpp> 
#include <wx/odcombo.h>
#include <wx/gdicmn.h>


WX_DEFINE_LIST(BitmapList);
WX_DEFINE_OBJARRAY(ArrayOfBitmaps);

ODIconCombo::ODIconCombo (wxWindow* parent, wxWindowID id, const wxString& value,
                                  const wxPoint& pos, const wxSize& size, int n, const wxString choices[],
                                  long style, const wxValidator& validator, const wxString& name)
                        :wxOwnerDrawnComboBox(parent, id, value, pos, size, n, choices, style, validator, name)
{
    wxScreenDC dc;
    wxFont l_Font = GetOCPNGUIScaledFont_PlugIn(wxT("H"));
    wxSize sz = dc.GetTextExtent(_T("H"));
    double pt_per_pixel = 12.0 / (double)sz.y;
    itemHeight = l_Font.GetPointSize()/ pt_per_pixel;
    
}

ODIconCombo::~ODIconCombo ()
{
}

void ODIconCombo::OnDrawItem( wxDC& dc,
                                       const wxRect& rect,
                                       int item,
                                       int flags ) const
{
    int offset_x = bmpArray.Item(item).GetWidth();
    int bmpHeight = bmpArray.Item(item).GetHeight();
    dc.DrawBitmap(bmpArray.Item(item), rect.x, rect.y + (rect.height - bmpHeight)/2, true);
    
    if ( flags & wxODCB_PAINTING_CONTROL )
    {
        wxString text = GetValue();
        int margin_x = 2;
        
#if wxCHECK_VERSION(2, 9, 0)
        if ( ShouldUseHintText() )
        {
            text = GetHint();
            wxColour col = wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT);
            dc.SetTextForeground(col);
        }
        
        margin_x = GetMargins().x;
#endif

        dc.DrawText( text,
                     rect.x + margin_x + offset_x,
                     (rect.height-dc.GetCharHeight())/2 + rect.y );
    }
    else
    {
        dc.DrawText( GetVListBoxComboPopup()->GetString(item), rect.x + 2 + offset_x, (rect.height-dc.GetCharHeight())/2 + rect.y );
    }
}

wxCoord ODIconCombo::OnMeasureItem( size_t item ) const
{
    int bmpHeight = bmpArray.Item(item).GetHeight();
    
    return wxMax(itemHeight, bmpHeight);
}

wxCoord ODIconCombo::OnMeasureItemWidth( size_t item ) const
{
    return -1;
}

int ODIconCombo::Append(const wxString& item, wxBitmap bmp)
{
    bmpArray.Add(bmp);
    int idx = wxOwnerDrawnComboBox::Append(item);
    
    return idx;
}

void ODIconCombo::Clear( void )
{
    wxOwnerDrawnComboBox::Clear();
    bmpArray.Clear();
}
