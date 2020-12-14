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

DR::DR() : ODPath()
{
    m_sTypeString = _T("DR");
    m_wxcActiveLineColour = g_colourDRLineColour;
    m_wxcInActiveLineColour = g_colourInActiveDRLineColour;
    m_width = g_DRLineWidth;
    m_style = g_DRLineStyle;
    m_bDrawArrow = g_bDRShowArrow;
    SetPersistence( g_iDRPersistenceType );
    m_dMagCOG = g_dVar;
    SetActiveColours();
}

DR::~DR()
{
    //dtor
}

ODPoint *DR::InsertPointBefore( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    ODPoint *newpoint = ODPath::InsertPointBefore( pOP, lat, lon );
    newpoint->m_IconName = g_sDRPointIconName;
    newpoint->m_sTypeString = wxT("DR Point");
    newpoint->m_ODPointName = wxT("");
    newpoint->m_bShowODPointRangeRings = g_bDRPointShowRangeRings;
    newpoint->m_iODPointRangeRingsNumber = g_iDRPointRangeRingsNumber;
    newpoint->m_fODPointRangeRingsStep = g_fDRPointRangeRingsStep;
    newpoint->m_iODPointRangeRingsStepUnits = g_iDRPointRangeRingsStepUnits;
    newpoint->m_wxcODPointRangeRingsColour = g_colourDRPointRangeRingsColour;
    newpoint->m_iRangeRingStyle = g_iDRPointRangeRingLineStyle;
    newpoint->m_iRangeRingWidth = g_iDRPointRangeRingLineWidth;
    FinalizeForRendering();
    return newpoint;
}

ODPoint *DR::InsertPointAfter( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    ODPoint *newpoint = ODPath::InsertPointAfter( pOP, lat, lon );
    newpoint->m_IconName = g_sDRPointIconName;
    newpoint->m_sTypeString = wxT("DR Point");
    newpoint->m_ODPointName = wxT("");
    newpoint->m_bShowODPointRangeRings = g_bDRPointShowRangeRings;
    newpoint->m_iODPointRangeRingsNumber = g_iDRPointRangeRingsNumber;
    newpoint->m_fODPointRangeRingsStep = g_fDRPointRangeRingsStep;
    newpoint->m_iODPointRangeRingsStepUnits = g_iDRPointRangeRingsStepUnits;
    newpoint->m_wxcODPointRangeRingsColour = g_colourDRPointRangeRingsColour;
    newpoint->m_iRangeRingStyle = g_iDRPointRangeRingLineStyle;
    newpoint->m_iRangeRingWidth = g_iDRPointRangeRingLineWidth;
    ReloadPathPointIcons();
    FinalizeForRendering();
    return newpoint;
}

void DR::SetPersistence( int PersistenceType )
{
    m_iPersistenceType = PersistenceType;
    if(PersistenceType == ID_NOT_PERSISTENT || PersistenceType == ID_PERSISTENT_CRASH)
        m_bTemporary = true;
    else
        m_bTemporary = false;
}

