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

#ifndef __ODROLLOVERWIN_H__
#define __ODROLLOVERWIN_H__

#include <wx/window.h>
#include <wx/timer.h>


//constants for rollovers fonts
enum
{
    PATH_ROLLOVER = 1,
    POINT_ROLLOVER
};

class ODRolloverWin: public wxWindow
{
public:
    ODRolloverWin( wxWindow *parent, int timeout = -1 );
    ~ODRolloverWin();

    void OnPaint( wxPaintEvent& event );

    void SetColorScheme( ColorScheme cs );
    void SetString(const wxString &s) { m_string = s; }
    void SetPosition( wxPoint pt ) { m_position = pt; }
    void SetBitmap( int rollover );
    wxBitmap* GetBitmap() { return m_pbm; }
    void SetBestPosition( int x, int y, int off_x, int off_y, int rollover, wxSize parent_size );
    void OnTimer( wxTimerEvent& event );
    void OnMouseEvent( wxMouseEvent& event );
    void SetMousePropogation( int level ) { m_mmouse_propogate = level; }
    bool IsActive() { return isActive; }
    void IsActive( bool state ) { isActive = state; }
    void SetParent( wxWindow *parent );
    wxWindow *GetParent( void ) { return m_parent; }

private:
    wxWindow    *m_parent;
    wxString    m_string;
    wxSize      m_size;
    wxPoint     m_position;
    wxTimer     m_timer_timeout;
    int         m_timeout_sec;
    int         m_mmouse_propogate;
    bool        isActive;
    wxFont      *m_plabelFont;
    wxBitmap    *m_pbm;
    
DECLARE_EVENT_TABLE()
};


#endif
