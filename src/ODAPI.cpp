/******************************************************************************
 * $Id: ocpn_draw_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN General Drawing Plugin
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
 *   $EMAIL$                                                               *
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
 ***************************************************************************
 */
//#define _2_9_x_ // uncomment this to compile for 2.9.x

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ODAPI.h"
#include "ocpn_draw_pi.h"
#include "ODPath.h"
#include "PathMan.h"

extern PathMan  *g_pPathMan;

ODAPI::ODAPI()
{
    // ctor
}

ODAPI::~ODAPI()
{
    // dtor
}

bool ODAPI::OD_FindPathByGUID( wxString sGUID, wxString sName, wxString sDescription )
{
    ODPath *l_path;
    l_path = g_pPathMan->FindPathByGUID( sGUID );
    if(l_path) {
        sDescription.append(l_path->m_PathDescription);
        sName.assign(l_path->m_PathNameString);
        return true;
    } else
        return false;
    
}



