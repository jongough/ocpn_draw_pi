/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPN Draw Event Handler Support
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

#ifndef OCPNDRAWEVENTHANDLER_H
#define OCPNDRAWEVENTHANDLER_H

#include <wx/event.h>
#include <wx/timer.h>
#include "ocpn_draw_pi.h"
#include "ODRolloverWin.h"

// Forward declarations
class SelectItem;
class TextPoint;

#define TIMER_OD_1 999

enum
{
    HEAD_DOG_TIMER  = 1000,
    ODROPOPUP_TIMER,
    ROLLOVER_TIMER,
    OD_TIMER_1,
    LAST_TIMER
};

class ODEventHandler : public wxEvtHandler
{
    public:
        ODEventHandler(ocpn_draw_pi *parent);
        ODEventHandler(ChartCanvas *parentCanvas, Path *selectedPath, ODPoint *selectedODPoint);
        ODEventHandler(ChartCanvas *parentCanvas, Path *selectedPath, TextPoint *selectedTextPoint);
        ~ODEventHandler();
        
        void OnODTimer1(wxTimerEvent& event);
        void PopupMenuHandler( wxCommandEvent & event );
        void OnRolloverPopupTimerEvent( wxTimerEvent &event );
        void PopupMenu( int x, int y, int seltype );
        void SetPath( Path *path );
        void SetPoint ( ODPoint *point );
        void SetPoint ( TextPoint *point );
        void SetCanvas( ChartCanvas *canvas );
        void SetLatLon( double lat, double lon );
        void DeletePath( void );
        
    protected:
    private:
        ocpn_draw_pi    *m_parent;
        ChartCanvas     *m_parentcanvas;
        int             popx, popy;
        Path            *m_pSelectedPath;
        ODPoint       *m_pFoundODPoint;
        double          m_cursor_lat;
        double          m_cursor_lon;
        wxTimer         ODTimer1;
        
        DECLARE_EVENT_TABLE();
};

#endif // OCPNDRAWEVENTHANDLER_H
