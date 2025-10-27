/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPN Draw Utility functions support
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
#include <wx/textwrapper.h>

#include "ODPlatform.h"
#include "ocpn_plugin.h"
#include "ocpn_draw_pi.h"

#ifdef __OCPN__ANDROID__
unsigned int androidColorPicker( unsigned int initialColor);
#endif


// ============================================================================
// ODColourPickerCtrl implementation
// ============================================================================

BEGIN_EVENT_TABLE(ODColourPickerCtrl, wxButton)
#ifdef __WXMSW__
    EVT_PAINT(ODColourPickerCtrl::OnPaint)
#endif
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// ODColourPickerCtrl
// ----------------------------------------------------------------------------

ODColourPickerCtrl::ODColourPickerCtrl(wxWindow *parent,
                   wxWindowID id,
                   const wxBitmap &bitmap,
                   const wxPoint& pos,
                   const wxSize& size,
                   long style,
                   const wxValidator& validator,
                   const wxString& name)
{
    Create(parent, id, bitmap, pos, size, style, validator, name);
}

ODColourPickerCtrl::~ODColourPickerCtrl()
{
    Unbind( wxEVT_BUTTON, &ODColourPickerCtrl::OnButtonClick, this);
}

bool ODColourPickerCtrl::Create( wxWindow *parent, wxWindowID id,
                        const wxBitmap &bitmap, const wxPoint &pos,
                        const wxSize &size, long style,
                        const wxValidator& validator, const wxString &name)
{
    int refDim = parent->GetCharHeight();
#ifdef __WXMSW__
    refDim /= g_dOCPN_DisplayScaleFactor;
#endif
    m_bitmap = wxBitmap( refDim * 4, refDim * 1 );

    // create this button
    if (!wxBitmapButton::Create( parent, id, m_bitmap, pos,
                           size, style | wxBU_AUTODRAW, validator, name ))
    {
        wxFAIL_MSG( wxT("ODColourPickerCtrl creation failed") );
        return false;
    }

    // and handle user clicks on it
    Bind( wxEVT_BUTTON, &ODColourPickerCtrl::OnButtonClick, this);

    m_colour = *wxBLACK;
    UpdateColour();
    InitColourData();

    return true;
}

void ODColourPickerCtrl::InitColourData()
{
#if 0
    ms_data.SetChooseFull(true);
    unsigned char grey = 0;
    for (int i = 0; i < 16; i++, grey += 16)
    {
        // fill with grey tones the custom colors palette
        wxColour colour(grey, grey, grey);
        ms_data.SetCustomColour(i, colour);
    }
#endif
}

void ODColourPickerCtrl::OnButtonClick(wxCommandEvent& WXUNUSED(ev))
{
#ifdef __OCPN__ANDROID__
  unsigned int cco = 0;
  cco |= 0xff;  cco  = cco << 8;
  cco |= m_colour.Red(); cco = cco << 8;
  cco |= m_colour.Green(); cco = cco << 8;
  cco |= m_colour.Blue();
  unsigned int cc = androidColorPicker( cco);

  wxColour cnew;
  unsigned char blue = (unsigned char) cc % 256;
  unsigned char green = (unsigned char) (cc >> 8) % 256;;
  unsigned char red = (unsigned char) (cc >> 16) % 256;
  cnew.Set(red, green, blue);

  SetColour(cnew);

#else
    // update the wxColouData to be shown in the dialog
    ms_data.SetColour(m_colour);

    // create the colour dialog and display it
    wxColourDialog dlg(this, &ms_data);
    if (dlg.ShowModal() == wxID_OK)
    {
         ms_data = dlg.GetColourData();
         SetColour(ms_data.GetColour());
    }
#endif
}

void ODColourPickerCtrl::UpdateColour()
{
#ifndef __OCPN__ANDROID__
    SetBitmapLabel(wxBitmap());
#endif

    wxMemoryDC dc(m_bitmap);
    dc.SetPen( *wxTRANSPARENT_PEN );
    dc.SetBrush( wxBrush(m_colour) );
    dc.DrawRectangle( 0, 0, m_bitmap.GetWidth(), m_bitmap.GetHeight() );

    dc.SelectObject( wxNullBitmap );
    SetBitmapLabel( m_bitmap );
}

void ODColourPickerCtrl::SetColour( const wxColour& c)
{
    m_colour = c;
    UpdateColour();
}

wxColour ODColourPickerCtrl::GetColour( void )
{
    return m_colour;
}

wxSize ODColourPickerCtrl::DoGetBestSize() const
{
    wxSize sz(wxBitmapButton::DoGetBestSize());
#ifdef __WXMAC__
    sz.y += 6;
#else
    sz.y += 2;
#endif
    sz.x += 30;
    if ( HasFlag(wxCLRP_SHOW_LABEL) )
        return sz;

    // if we have no label, then make this button a square
    // (like e.g. native GTK version of this control) ???
    // sz.SetWidth(sz.GetHeight());
    return sz;
}

void ODColourPickerCtrl::OnPaint(wxPaintEvent &event)
{

    wxPaintDC dc(this) ;

    int offset_x = (GetSize().x - m_bitmap.GetWidth()) / 2;
    int offset_y = (GetSize().y - m_bitmap.GetHeight()) / 2;

    dc.SetPen( *wxTRANSPARENT_PEN );
    dc.SetBrush( wxBrush(m_colour) );
    dc.DrawRectangle( offset_x, offset_y, m_bitmap.GetWidth(), m_bitmap.GetHeight() );

    event.Skip() ;
}

