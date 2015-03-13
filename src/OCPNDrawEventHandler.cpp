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

#include "OCPNDrawEventHandler.h"
#include "ocpn_draw_pi.h"

// Event Handler implementation 
BEGIN_EVENT_TABLE ( OCPNDrawEventHandler, wxEvtHandler ) 
  EVT_TIMER ( ROLLOVER_TIMER, OCPNDrawEventHandler::OnTimerEvent ) 
  EVT_TIMER ( HEAD_DOG_TIMER, OCPNDrawEventHandler::OnTimerEvent ) 
END_EVENT_TABLE()

OCPNDrawEventHandler::OCPNDrawEventHandler(ocpn_draw_pi *parent)
{
    //ctor
    m_parent = parent;
}

OCPNDrawEventHandler::~OCPNDrawEventHandler()
{
    //dtor
}

void OCPNDrawEventHandler::OnTimerEvent(wxTimerEvent& event) 
{ 
    m_parent->ProcessTimerEvent( event ); 
} 

void OCPNDrawEventHandler::PopupMenuHandler(wxCommandEvent& event ) 
{
    m_parent->PopupMenuHandler( event ); 
} 
