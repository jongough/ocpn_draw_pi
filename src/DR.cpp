/***************************************************************************
 * Project:  OpenCPN
 * Purpose:  DR
 * Author:   Jon Gough
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

#include "DR.h"
#include "ODdc.h"
#include "ocpn_draw_pi.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "EBLProp.h"

#include "ODConfig.h"

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
WX_DEFINE_LIST ( DRList );

extern wxColour    g_colourEBLLineColour;
extern wxString    g_sEBLEndIconName;
extern wxString    g_sEBLStartIconName;
extern bool        g_bEBLFixedEndPosition;
extern int         g_EBLPersistenceType;
extern int         g_EBLLineWidth; 
extern int         g_EBLLineStyle;

extern ocpn_draw_pi *g_ocpn_draw_pi;
extern EBLList      *g_pEBLList;
extern ODSelect     *g_pODSelect;
extern PathMan      *g_pPathMan;
extern ODPlugIn_Position_Fix_Ex  g_pfFix;
extern wxString      g_sODPointIconName;
extern ODConfig     *g_pODConfig;
extern EBLProp      *g_pEBLPropDialog;
extern bool         g_bEBLShowArrow;
extern bool         g_bEBLVRM;

DR::DR() : ODPath()
{
    m_sTypeString = _T("EBL");
    m_wxcActiveLineColour = g_colourEBLLineColour;
    m_width = g_EBLLineWidth;
    m_style = g_EBLLineStyle;
    m_bDrawArrow = g_bEBLShowArrow;
    m_bVRM = g_bEBLVRM;
    m_bVRM = g_bEBLVRM;
    m_bCentreOnBoat = true;
    m_bFixedEndPosition = g_bEBLFixedEndPosition;
    SetPersistence( g_EBLPersistenceType );
    SetActiveColours();
}

DR::~EBL()
{
    //dtor
}

void DR::MoveEndPoint( double inc_lat, double inc_lon )
{
    ODPoint *pEndPoint = m_pODPointList->GetLast()->GetData();
    pEndPoint->m_lat -= inc_lat;
    pEndPoint->m_lon -= inc_lon;
    if(m_bVRM) {
        ODPoint *pStartPoint = m_pODPointList->GetFirst()->GetData();
        pStartPoint->SetODPointRangeRingsStep( pEndPoint->m_seg_len / pStartPoint->GetODPointRangeRingsNumber() );
    }
    if(g_pEBLPropDialog && g_pEBLPropDialog->IsShown())
        g_pEBLPropDialog->UpdateProperties();
    
}

ODPoint *DR::InsertPointBefore( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    ODPoint *newpoint = ODPath::InsertPointBefore( pOP, lat, lon );
    newpoint->m_IconName = g_sEBLStartIconName;
    newpoint->m_sTypeString = wxT("EBL Point");
    newpoint->m_MarkName = wxT("Start");
    FinalizeForRendering();
    return newpoint;
}

ODPoint *DR::InsertPointAfter( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    ODPoint *newpoint = ODPath::InsertPointAfter( pOP, lat, lon );
    newpoint->m_IconName = g_sEBLStartIconName;
    newpoint->m_sTypeString = wxT("EBL Point");
    newpoint->m_MarkName = wxT("Start");
    ReloadPathPointIcons();
    FinalizeForRendering();
    return newpoint;
}

void DR::SetPersistence( int PersistenceType )
{
    m_PersistenceType = PersistenceType;
    if(PersistenceType == ID_EBL_NOT_PERSISTENT || PersistenceType == ID_EBL_PERSISTENT_CRASH)
        m_bTemporary = true;
    else
        m_bTemporary = false;
}

