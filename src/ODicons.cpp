/******************************************************************************
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
 ***************************************************************************
 */

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "ODicons.h"
#include "ocpn_plugin.h"
#include "ocpn_draw_pi.h"

#include <wx/mstream.h>
#include <wx/filename.h>

#ifdef ODUSE_SVG
#include "svg.h"
#endif // OD_USE_SVG


extern wxString g_SData_Locn;
extern wxString *g_pHome_locn;
extern wxString *g_pData;
extern wxString *g_pImage;;

void initialize_images(void)
{
    wxFileName fn;
    fn.SetPath(*GetpSharedDataLocation());
    fn.AppendDir( wxT("plugins") );
    fn.AppendDir(wxT("ocpn_draw_pi"));
    fn.AppendDir(wxT("data"));
    wxString s = _("ocpn_draw_pi data location");
    wxLogMessage( wxT("%s: %s"), s.c_str(), fn.GetFullPath().c_str());

#if ODUSE_SVG
    fn.SetFullName(wxT("ODManager.svg"));
    _img_ocpn_draw_pi = LoadSVG( fn.GetFullPath(), 32, 32 );
    fn.SetFullName(wxT("ODManagergrey.svg"));
    _img_ocpn_draw_grey_pi = LoadSVG( fn.GetFullPath(), 32, 32 );
    
    fn.SetFullName(wxT("boundary.svg"));
    _img_ocpn_draw_boundary = LoadSVG( fn.GetFullPath(), 32, 32 );
    fn.SetFullName(wxT("boundarygrey.svg"));
    _img_ocpn_draw_boundary_grey = LoadSVG( fn.GetFullPath(), 32, 32 );
    
    fn.SetFullName(wxT("pointbutton.svg"));
    _img_ocpn_draw_point = LoadSVG( fn.GetFullPath(), 32, 32 );
    fn.SetFullName(wxT("pointbuttongrey.svg"));
    _img_ocpn_draw_point_grey = LoadSVG( fn.GetFullPath(), 32, 32 );
    
    fn.SetFullName(wxT("textpointbutton.svg"));
    _img_ocpn_draw_textpoint = LoadSVG( fn.GetFullPath(), 32, 32 );
    fn.SetFullName(wxT("textpointbuttongrey.svg"));
    _img_ocpn_draw_textpoint_grey = LoadSVG( fn.GetFullPath(), 32, 32 );
    
    fn.SetFullName(wxT("EBL.svg"));
    _img_ocpn_draw_ebl = LoadSVG( fn.GetFullPath(), 32, 32 );
    fn.SetFullName(wxT("EBLgrey.svg"));
    _img_ocpn_draw_ebl_grey = LoadSVG( fn.GetFullPath(), 32, 32 );
    
    fn.SetFullName(wxT("DR.svg"));
    _img_ocpn_draw_dr = LoadSVG( fn.GetFullPath(), 32, 32 );
    fn.SetFullName(wxT("DRgrey.svg"));
    _img_ocpn_draw_dr_grey = LoadSVG( fn.GetFullPath(), 32, 32 );
#else
    fn.SetFullName(wxT("ODManager.png"));
    _img_ocpn_draw_pi = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("ODManagergrey.png"));
    _img_ocpn_draw_grey_pi = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("boundary.png"));
    _img_ocpn_draw_boundary = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("boundarygrey.png"));
    _img_ocpn_draw_boundary_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("pointbutton.png"));
    _img_ocpn_draw_point = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("pointbuttongrey.png"));
    _img_ocpn_draw_point_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("textpointbutton.png"));
    _img_ocpn_draw_textpoint = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("textpointbuttongrey.png"));
    _img_ocpn_draw_textpoint_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("EBL.png"));
    _img_ocpn_draw_ebl = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("EBLgrey.png"));
    _img_ocpn_draw_ebl_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("DR.png"));
    _img_ocpn_draw_dr = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("DRgrey.png"));
    _img_ocpn_draw_dr_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
#endif
}

#ifdef ODUSE_SVG
wxBitmap *LoadSVG( const wxString filename, unsigned int width, unsigned int height )
{
    wxSVGDocument *svgDoc = new wxSVGDocument;;
    if( svgDoc->Load(filename) )
        return new wxBitmap( svgDoc->Render( width, height, NULL, true, true) );
    else
        return new wxBitmap(width, height);
}
#endif // OD_USE_SVG
