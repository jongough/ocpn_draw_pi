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
class Boundary;
class TextPoint;
class EBL;
class DR;
class GZ;
class PIL;

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
        ODEventHandler(ChartCanvas *parentCanvas, ODPath *selectedPath, ODPoint *selectedODPoint);
        ODEventHandler(ChartCanvas *parentCanvas, ODPath *selectedPath, TextPoint *selectedTextPoint);
        ~ODEventHandler();
        
        void OnODTimer1(wxTimerEvent& event);
        void PopupMenuHandler( wxCommandEvent & event );
        void OnRolloverPopupTimerEvent( wxTimerEvent &event );
        void PopupMenu( int seltype );
        void SetPath( ODPath *path );
        void SetBoundaryList( std::list<Boundary*> pBoundaryList );
        void SetPoint ( ODPoint *point );
        void SetPoint ( TextPoint *point );
        void SetPIL( int iPIL );
        void SetCanvas( ChartCanvas *canvas );
        void SetLatLon( double lat, double lon );
        void DeletePath( void );
        void DeletePaths( void );
        void DeletePIL( void );
        
    protected:
    private:
        ocpn_draw_pi    *m_parent;
        ChartCanvas     *m_parentcanvas;
        ODPath          *m_pSelectedPath;
        ODPoint         *m_pFoundODPoint;
        TextPoint       *m_pFoundTextPoint;
        double          m_cursor_lat;
        double          m_cursor_lon;
        wxTimer         ODTimer1;
        Boundary        *m_pBoundary;
        EBL             *m_pEBL;
        DR              *m_pDR;
        GZ              *m_pGZ;
        PIL             *m_pPIL;
        std::list<Boundary*> m_pBoundaryList;
        int             m_iFoundPIL;

        DECLARE_EVENT_TABLE();
};

#endif // OCPNDRAWEVENTHANDLER_H
