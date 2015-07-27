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

#include "EBL.h"
#include "ODdc.h"
#include "ocpn_draw_pi.h"

#ifdef __WXMSW__
#include "GL/gl.h"            // local copy for Windows
#include <GL/glu.h>
#else

#ifndef __OCPN__ANDROID__
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include "qopengl.h"                  // this gives us the qt runtime gles2.h
#include "GL/gl_private.h"
#endif

#endif

#ifdef ocpnUSE_GL
#include <wx/glcanvas.h>
#endif


#include <wx/graphics.h>

#include <wx/listimpl.cpp>
WX_DEFINE_LIST ( EBLList );

extern wxColour    g_colourEBLLineColour;
extern ocpn_draw_pi *g_ocpn_draw_pi;
extern EBLList      *g_pEBLList;

EBL::EBL() : Path()
{
    m_sTypeString = _T("EBL");
    m_wxcLineColour = g_colourEBLLineColour;
}

EBL::~EBL()
{
    //dtor
}

void EBL::Draw( ODDC& dc, PlugIn_ViewPort &VP )
{
    Path::Draw( dc, VP );
}

void EBL::DrawGL( PlugIn_ViewPort &piVP )
{
#ifdef ocpnUSE_GL
    Path::DrawGL( piVP );
#endif
}

void EBL::AddPoint( ODPoint *pNewPoint, bool b_rename_in_sequence, bool b_deferBoxCalc, bool b_isLoading )
{
    Path::AddPoint( pNewPoint, b_rename_in_sequence, b_deferBoxCalc, b_isLoading );
    
    if(pNewPoint->GetName() == wxT("Boat")) g_ocpn_draw_pi->m_pEBLBoatPoint = pNewPoint;
}

void EBL::MovePoint( double inc_lat, double inc_lon )
{
    wxODPointListNode *node = m_pODPointList->GetLast();
    ODPoint *bp = (ODPoint *)node->GetData();
    bp->m_lat -= inc_lat;
    bp->m_lon -= inc_lon;
}