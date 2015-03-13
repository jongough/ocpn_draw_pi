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

#define ROLLOVER_TIMER  1000
#define HEAD_DOG_TIMER  1001

class OCPNDrawEventHandler : public wxEvtHandler
{
    public:
        OCPNDrawEventHandler(ocpn_draw_pi *parent);
        virtual ~OCPNDrawEventHandler();
        
        void OnTimerEvent( wxTimerEvent &event );
        void PopupMenuHandler( wxCommandEvent & event );
        
    protected:
    private:
        ocpn_draw_pi  *m_parent;
        
        DECLARE_EVENT_TABLE();
};

#endif // OCPNDRAWEVENTHANDLER_H
