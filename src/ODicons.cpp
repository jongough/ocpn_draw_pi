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

#include <wx/stdpaths.h>

ODicons::ODicons()
{
    m_dScaleFactor = 1.0;
    m_bUpdateIcons = false;
    m_ColourScheme = PI_GLOBAL_COLOR_SCHEME_RGB;
    
    initialize_images();
}

ODicons::~ODicons()
{
    delete g_SData_Locn;
    g_SData_Locn = NULL;
}
void ODicons::initialize_images(void)
{
    wxFileName fn;

    fn.SetPath(GetPluginDataDir( "ocpn_draw_pi"));
    fn.AppendDir(wxT("data"));
    g_SData_Locn = new wxString(fn.GetFullPath().c_str());

    wxString s = _("ocpn_draw_pi data location");
    wxLogMessage( wxT("%s: %s"), s.c_str(), fn.GetFullPath().c_str());
    
    m_failedBitmapLoad = false;
    
    m_p_bm_ocpn_draw_pi = &m_bm_ocpn_draw_pi;
    m_p_bm_ocpn_draw_grey_pi = &m_bm_ocpn_draw_grey_pi;
    m_p_bm_ocpn_draw_boundary = &m_bm_ocpn_draw_boundary;
    m_p_bm_ocpn_draw_boundary_grey = &m_bm_ocpn_draw_boundary_grey;
    m_p_bm_ocpn_draw_point = &m_bm_ocpn_draw_point;
    m_p_bm_ocpn_draw_point_grey = &m_bm_ocpn_draw_point_grey;
    m_p_bm_ocpn_draw_textpoint = &m_bm_ocpn_draw_textpoint;
    m_p_bm_ocpn_draw_textpoint_grey = &m_bm_ocpn_draw_textpoint_grey;
    m_p_bm_ocpn_draw_ebl = &m_bm_ocpn_draw_ebl;
    m_p_bm_ocpn_draw_ebl_grey = &m_bm_ocpn_draw_ebl_grey;
    m_p_bm_ocpn_draw_dr = &m_bm_ocpn_draw_dr;
    m_p_bm_ocpn_draw_dr_grey = &m_bm_ocpn_draw_dr_grey;
    m_p_bm_ocpn_draw_gz = &m_bm_ocpn_draw_gz;
    m_p_bm_ocpn_draw_gz_grey = &m_bm_ocpn_draw_gz_grey;
    m_p_bm_ocpn_draw_pil = &m_bm_ocpn_draw_pil;
    m_p_bm_ocpn_draw_pil_grey = &m_bm_ocpn_draw_pil_grey;
    

#ifdef ODraw_USE_MUI    
    fn.SetFullName(wxT("ODManager_MUI.svg"));
    m_s_ocpn_draw_pi = fn.GetFullPath();
    m_bm_ocpn_draw_pi = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("ODManagergrey_MUI.svg"));
    m_s_ocpn_draw_grey_pi = fn.GetFullPath();
    m_bm_ocpn_draw_grey_pi = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("boundary_MUI.svg"));
    m_bm_ocpn_draw_boundary = LoadSVG( fn.GetFullPath() );
    m_s_ocpn_draw_boundary = fn.GetFullPath();
    fn.SetFullName(wxT("boundarygrey_MUI.svg"));
    m_s_ocpn_draw_boundary_grey = fn.GetFullPath();
    m_bm_ocpn_draw_boundary_grey = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("pointbutton_MUI.svg"));
    m_s_ocpn_draw_point = fn.GetFullPath();
    m_bm_ocpn_draw_point = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("pointbuttongrey_MUI.svg"));
    m_s_ocpn_draw_point_grey = fn.GetFullPath();
    m_bm_ocpn_draw_point_grey = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("textpointbutton_MUI.svg"));
    m_s_ocpn_draw_textpoint = fn.GetFullPath();
    m_bm_ocpn_draw_textpoint = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("textpointbuttongrey_MUI.svg"));
    m_s_ocpn_draw_textpoint_grey = fn.GetFullPath();
    m_bm_ocpn_draw_textpoint_grey = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("EBL_MUI.svg"));
    m_s_ocpn_draw_ebl = fn.GetFullPath();
    m_bm_ocpn_draw_ebl = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("EBLgrey_MUI.svg"));
    m_s_ocpn_draw_ebl_grey = fn.GetFullPath();
    m_bm_ocpn_draw_ebl_grey = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("DR_MUI.svg"));
    m_s_ocpn_draw_dr = fn.GetFullPath();
    m_bm_ocpn_draw_dr = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("DRgrey_MUI.svg"));
    m_s_ocpn_draw_dr_grey = fn.GetFullPath();
    m_bm_ocpn_draw_dr_grey = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("GZ_MUI.svg"));
    m_s_ocpn_draw_gz = fn.GetFullPath();
    m_bm_ocpn_draw_gz = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("GZgrey_MUI.svg"));
    m_s_ocpn_draw_gz_grey = fn.GetFullPath();
    m_bm_ocpn_draw_gz_grey = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("PIL_MUI.svg"));
    m_s_ocpn_draw_pil = fn.GetFullPath();
    m_bm_ocpn_draw_pil = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("PILgrey_MUI.svg"));
    m_s_ocpn_draw_pil_grey = fn.GetFullPath();
    m_bm_ocpn_draw_pil_grey = LoadSVG( fn.GetFullPath() );
#else
#ifdef ODraw_USE_SVG
    fn.SetFullName(wxT("ODManager.svg"));
    m_s_ocpn_draw_pi = fn.GetFullPath();
    m_bm_ocpn_draw_pi = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("ODManagergrey.svg"));
    m_s_ocpn_draw_grey_pi = fn.GetFullPath();
    m_bm_ocpn_draw_grey_pi = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("boundary.svg"));
    m_s_ocpn_draw_boundary = fn.GetFullPath();
    m_bm_ocpn_draw_boundary = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("boundarygrey.svg"));
    m_s_ocpn_draw_boundary_grey = fn.GetFullPath();
    m_bm_ocpn_draw_boundary_grey = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("pointbutton.svg"));
    m_s_ocpn_draw_point = fn.GetFullPath();
    m_bm_ocpn_draw_point = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("pointbuttongrey.svg"));
    m_s_ocpn_draw_point_grey = fn.GetFullPath();
    m_bm_ocpn_draw_point_grey = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("textpointbutton.svg"));
    m_s_ocpn_draw_textpoint = fn.GetFullPath();
    m_bm_ocpn_draw_textpoint = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("textpointbuttongrey.svg"));
    m_s_ocpn_draw_textpoint_grey = fn.GetFullPath();
    m_bm_ocpn_draw_textpoint_grey = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("EBL.svg"));
    m_s_ocpn_draw_ebl = fn.GetFullPath();
    m_bm_ocpn_draw_ebl = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("EBLgrey.svg"));
    m_s_ocpn_draw_ebl_grey = fn.GetFullPath();
    m_bm_ocpn_draw_ebl_grey = LoadSVG( fn.GetFullPath() );
    
    fn.SetFullName(wxT("DR.svg"));
    m_s_ocpn_draw_dr = fn.GetFullPath();
    m_bm_ocpn_draw_dr = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("DRgrey.svg"));
    m_s_ocpn_draw_dr_grey = fn.GetFullPath();
    m_bm_ocpn_draw_dr_grey = LoadSVG( fn.GetFullPath() );

    fn.SetFullName(wxT("GZ.svg"));
    m_s_ocpn_draw_gz = fn.GetFullPath();
    m_bm_ocpn_draw_gz = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("GZgrey.svg"));
    m_s_ocpn_draw_gz_grey = fn.GetFullPath();
    m_bm_ocpn_draw_gz_grey = LoadSVG( fn.GetFullPath() );

    fn.SetFullName(wxT("PIL.svg"));
    m_s_ocpn_draw_pil = fn.GetFullPath();
    m_bm_ocpn_draw_pil = LoadSVG( fn.GetFullPath() );
    fn.SetFullName(wxT("PILgrey.svg"));
    m_s_ocpn_draw_pil_grey = fn.GetFullPath();
    m_bm_ocpn_draw_pil_grey = LoadSVG( fn.GetFullPath() );
#else
    fn.SetFullName(wxT("ODManager.png"));
    m_p_bm_ocpn_draw_pi = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_pi->IsOk())  m_failedBitmapLoad = true;
    fn.SetFullName(wxT("ODManagergrey.png"));
    m_p_bm_ocpn_draw_grey_pi = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_grey_pi->IsOk())  m_failedBitmapLoad = true;
    
    fn.SetFullName(wxT("boundary.png"));
    m_p_bm_ocpn_draw_boundary = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_boundary->IsOk())  m_failedBitmapLoad = true;
    fn.SetFullName(wxT("boundarygrey.png"));
    m_p_bm_ocpn_draw_boundary_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_boundary_grey->IsOk())  m_failedBitmapLoad = true;
    
    fn.SetFullName(wxT("pointbutton.png"));
    m_p_bm_ocpn_draw_point = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_point->IsOk())  m_failedBitmapLoad = true;
    fn.SetFullName(wxT("pointbuttongrey.png"));
    m_p_bm_ocpn_draw_point_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_point_grey->IsOk())  m_failedBitmapLoad = true;
    
    fn.SetFullName(wxT("textpointbutton.png"));
    m_p_bm_ocpn_draw_textpoint = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_textpoint->IsOk())  m_failedBitmapLoad = true;
    fn.SetFullName(wxT("textpointbuttongrey.png"));
    m_p_bm_ocpn_draw_textpoint_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_textpoint_grey->IsOk())  m_failedBitmapLoad = true;
    
    fn.SetFullName(wxT("EBL.png"));
    m_p_bm_ocpn_draw_ebl = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_ebl->IsOk())  m_failedBitmapLoad = true;
    fn.SetFullName(wxT("EBLgrey.png"));
    m_p_bm_ocpn_draw_ebl_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_ebl_grey->IsOk())  m_failedBitmapLoad = true;
    
    fn.SetFullName(wxT("DR.png"));
    m_p_bm_ocpn_draw_dr = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_dr->IsOk())  m_failedBitmapLoad = true;
    fn.SetFullName(wxT("DRgrey.png"));
    m_p_bm_ocpn_draw_dr_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_dr_grey->IsOk())  m_failedBitmapLoad = true;
    
    fn.SetFullName(wxT("GZ.png"));
    m_p_bm_ocpn_draw_gz = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_gz->IsOk())  m_failedBitmapLoad = true;
    fn.SetFullName(wxT("GZgrey.png"));
    m_p_bm_ocpn_draw_gz_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_gz_grey->IsOk())  m_failedBitmapLoad = true;
    
    fn.SetFullName(wxT("PIL.png"));
    m_p_bm_ocpn_draw_pil = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_pil->IsOk())  m_failedBitmapLoad = true;
    fn.SetFullName(wxT("PILgrey.png"));
    m_p_bm_ocpn_draw_pil_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    if(!m_p_bm_ocpn_draw_pil_grey->IsOk())  m_failedBitmapLoad = true;
#endif
#endif    
    if(m_failedBitmapLoad) {
        int ret = OCPNMessageBox_PlugIn( NULL, _("Failed to load all OCPN Draw Plugin icons, check OCPN log for details"), _("OpenCPN Alert"), wxOK );
    } else {
        CreateSchemeIcons();
        ScaleIcons();
    }
}

#ifdef ODraw_USE_SVG
wxBitmap ODicons::LoadSVG( const wxString filename, unsigned int width, unsigned int height )
{
    wxBitmap l__Bitmap = GetBitmapFromSVGFile(filename , width, height);
    if(!l__Bitmap.IsOk()) {
        m_failedBitmapLoad = true;
    }
    
    return l__Bitmap;
}

wxBitmap ODicons::ScaleIcon( wxBitmap bitmap, const wxString filename, double sf )
{
    int w = bitmap.GetWidth();
    int h = bitmap.GetHeight();
    w *= sf;
    h *= sf;

    wxBitmap svgbm = GetBitmapFromSVGFile(filename, w, h);
    if(svgbm.GetWidth() > 0 && svgbm.GetHeight() > 0)
        return svgbm;
    return wxBitmap(32 * sf, 32 * sf); //scalled default blank bitmap
}
#endif // OD_USE_SVG

wxBitmap ODicons::ScaleIcon( wxBitmap bitmap, double sf )
{
    wxImage scaled_image = bitmap.ConvertToImage();
    return wxBitmap(scaled_image.Scale(scaled_image.GetWidth() * sf, scaled_image.GetHeight() * sf, wxIMAGE_QUALITY_HIGH));
}
bool ODicons::ScaleIcons()
{
    if(!SetScaleFactor()) return false;
    

#ifdef ODraw_USE_SVG
    
    // Dont scale the OD manager as that should be done by the OCPN toolbar
    //m_p_bm_ocpn_draw_pi = ScaleIcon( m_p_svgd_ocpn_draw_pi, m_p_img_ocpn_draw_pi, m_dScaleFactor );
    //m_p_bm_ocpn_draw_grey_pi = ScaleIcon( m_p_svgd_ocpn_draw_grey_pi, m_p_img_ocpn_draw_grey_pi, m_dScaleFactor );
    
    m_bm_ocpn_draw_boundary = ScaleIcon( LoadSVG(m_s_ocpn_draw_boundary), m_dScaleFactor );
    m_bm_ocpn_draw_boundary_grey = ScaleIcon( LoadSVG(m_s_ocpn_draw_boundary_grey), m_dScaleFactor );
    
    m_bm_ocpn_draw_point = ScaleIcon( LoadSVG(m_s_ocpn_draw_point), m_dScaleFactor );
    m_bm_ocpn_draw_point_grey = ScaleIcon( LoadSVG(m_s_ocpn_draw_point_grey), m_dScaleFactor );
    
    m_bm_ocpn_draw_textpoint = ScaleIcon( LoadSVG(m_s_ocpn_draw_textpoint),  m_dScaleFactor );
    m_bm_ocpn_draw_textpoint_grey = ScaleIcon( LoadSVG(m_s_ocpn_draw_textpoint_grey), m_dScaleFactor );
    
    m_bm_ocpn_draw_ebl = ScaleIcon( LoadSVG(m_s_ocpn_draw_ebl), m_dScaleFactor );
    m_bm_ocpn_draw_ebl_grey = ScaleIcon( LoadSVG(m_s_ocpn_draw_ebl_grey), m_dScaleFactor );
    
    m_bm_ocpn_draw_dr = ScaleIcon( LoadSVG(m_s_ocpn_draw_dr), m_dScaleFactor );
    m_bm_ocpn_draw_dr_grey = ScaleIcon( LoadSVG(m_s_ocpn_draw_dr_grey), m_dScaleFactor );
    
    m_bm_ocpn_draw_gz = ScaleIcon( LoadSVG(m_s_ocpn_draw_gz), m_dScaleFactor );
    m_bm_ocpn_draw_gz_grey = ScaleIcon( LoadSVG(m_s_ocpn_draw_gz_grey), m_dScaleFactor );

    m_bm_ocpn_draw_pil = ScaleIcon( LoadSVG(m_s_ocpn_draw_pil), m_dScaleFactor );
    m_bm_ocpn_draw_pil_grey = ScaleIcon( LoadSVG(m_s_ocpn_draw_pil_grey), m_dScaleFactor );
#else
    m_bm_ocpn_draw_boundary = ScaleIcon( m_bm_ocpn_draw_boundary, m_dScaleFactor );
    m_bm_ocpn_draw_boundary_grey = ScaleIcon( m_bm_ocpn_draw_boundary_grey, m_dScaleFactor );
    
    m_bm_ocpn_draw_point = ScaleIcon( m_bm_ocpn_draw_point, m_dScaleFactor );
    m_bm_ocpn_draw_point_grey = ScaleIcon( m_bm_ocpn_draw_point_grey, m_dScaleFactor );
    
    m_bm_ocpn_draw_textpoint = ScaleIcon( m_bm_ocpn_draw_textpoint, m_dScaleFactor );
    m_bm_ocpn_draw_textpoint_grey = ScaleIcon( m_bm_ocpn_draw_textpoint_grey, m_dScaleFactor );
    
    m_bm_ocpn_draw_ebl = ScaleIcon( m_bm_ocpn_draw_ebl, m_dScaleFactor );
    m_bm_ocpn_draw_ebl_grey = ScaleIcon( m_bm_ocpn_draw_ebl_grey, m_dScaleFactor );
    
    m_bm_ocpn_draw_dr = ScaleIcon( m_bm_ocpn_draw_dr, m_dScaleFactor );
    m_bm_ocpn_draw_dr_grey = ScaleIcon( m_bm_ocpn_draw_dr_grey, m_dScaleFactor );
    
    m_bm_ocpn_draw_gz = ScaleIcon( m_bm_ocpn_draw_gz, m_dScaleFactor );
    m_bm_ocpn_draw_gz_grey = ScaleIcon( m_bm_ocpn_draw_gz_grey, m_dScaleFactor );

    m_bm_ocpn_draw_pil = ScaleIcon( m_bm_ocpn_draw_pil, m_dScaleFactor );
    m_bm_ocpn_draw_pil_grey = ScaleIcon( m_bm_ocpn_draw_pil_grey, m_dScaleFactor );

#endif // OD_USE_SVG
    
    CreateSchemeIcons();
    
    return true;
}

bool ODicons::SetScaleFactor()
{
    if(m_dScaleFactor != GetOCPNGUIToolScaleFactor_PlugIn()) {
        m_dScaleFactor = GetOCPNGUIToolScaleFactor_PlugIn();
        return true;
    }
    return false;
}

void ODicons::SetColourScheme( PI_ColorScheme cs )
{
    if(m_ColourScheme == cs) m_bUpdateIcons = false;
    else {
        m_bUpdateIcons = true;
        m_ColourScheme = cs;
        ChangeScheme();
    }
}

void ODicons::ChangeScheme(void)
{
    switch(m_ColourScheme) {
        case PI_GLOBAL_COLOR_SCHEME_RGB:
        case PI_GLOBAL_COLOR_SCHEME_DAY:
            m_bm_ocpn_draw_grey_pi = m_bm_day_ocpn_draw_grey_pi;
            m_bm_ocpn_draw_boundary = m_bm_day_ocpn_draw_boundary;
            m_bm_ocpn_draw_boundary_grey = m_bm_day_ocpn_draw_boundary_grey;
            m_bm_ocpn_draw_point = m_bm_day_ocpn_draw_point;
            m_bm_ocpn_draw_point_grey = m_bm_day_ocpn_draw_point_grey;
            m_bm_ocpn_draw_textpoint = m_bm_day_ocpn_draw_textpoint;
            m_bm_ocpn_draw_textpoint_grey = m_bm_day_ocpn_draw_textpoint_grey;
            m_bm_ocpn_draw_ebl = m_bm_day_ocpn_draw_ebl;
            m_bm_ocpn_draw_ebl_grey = m_bm_day_ocpn_draw_ebl_grey;
            m_bm_ocpn_draw_dr = m_bm_day_ocpn_draw_dr;
            m_bm_ocpn_draw_dr_grey = m_bm_day_ocpn_draw_dr_grey;
            m_bm_ocpn_draw_gz = m_bm_day_ocpn_draw_gz;
            m_bm_ocpn_draw_gz_grey = m_bm_day_ocpn_draw_gz_grey;
            m_bm_ocpn_draw_pil = m_bm_day_ocpn_draw_pil;
            m_bm_ocpn_draw_pil_grey = m_bm_day_ocpn_draw_pil_grey;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DUSK:
            m_bm_ocpn_draw_grey_pi = m_bm_dusk_ocpn_draw_grey_pi;
            m_bm_ocpn_draw_boundary = m_bm_dusk_ocpn_draw_boundary;
            m_bm_ocpn_draw_boundary_grey = m_bm_dusk_ocpn_draw_boundary_grey;
            m_bm_ocpn_draw_point = m_bm_dusk_ocpn_draw_point;
            m_bm_ocpn_draw_point_grey = m_bm_dusk_ocpn_draw_point_grey;
            m_bm_ocpn_draw_textpoint = m_bm_dusk_ocpn_draw_textpoint;
            m_bm_ocpn_draw_textpoint_grey = m_bm_dusk_ocpn_draw_textpoint_grey;
            m_bm_ocpn_draw_ebl = m_bm_dusk_ocpn_draw_ebl;
            m_bm_ocpn_draw_ebl_grey = m_bm_dusk_ocpn_draw_ebl_grey;
            m_bm_ocpn_draw_dr = m_bm_dusk_ocpn_draw_dr;
            m_bm_ocpn_draw_dr_grey = m_bm_dusk_ocpn_draw_dr_grey;
            m_bm_ocpn_draw_gz = m_bm_dusk_ocpn_draw_gz;
            m_bm_ocpn_draw_gz_grey = m_bm_dusk_ocpn_draw_gz_grey;
            m_bm_ocpn_draw_pil = m_bm_dusk_ocpn_draw_pil;
            m_bm_ocpn_draw_pil_grey = m_bm_dusk_ocpn_draw_pil_grey;
            break;
        case PI_GLOBAL_COLOR_SCHEME_NIGHT:
            m_bm_ocpn_draw_grey_pi = m_bm_night_ocpn_draw_grey_pi;
            m_bm_ocpn_draw_boundary = m_bm_night_ocpn_draw_boundary;
            m_bm_ocpn_draw_boundary_grey = m_bm_night_ocpn_draw_boundary_grey;
            m_bm_ocpn_draw_point = m_bm_night_ocpn_draw_point;
            m_bm_ocpn_draw_point_grey = m_bm_night_ocpn_draw_point_grey;
            m_bm_ocpn_draw_textpoint = m_bm_night_ocpn_draw_textpoint;
            m_bm_ocpn_draw_textpoint_grey = m_bm_night_ocpn_draw_textpoint_grey;
            m_bm_ocpn_draw_ebl = m_bm_night_ocpn_draw_ebl;
            m_bm_ocpn_draw_ebl_grey = m_bm_night_ocpn_draw_ebl_grey;
            m_bm_ocpn_draw_dr = m_bm_night_ocpn_draw_dr;
            m_bm_ocpn_draw_dr_grey = m_bm_night_ocpn_draw_dr_grey;
            m_bm_ocpn_draw_gz = m_bm_night_ocpn_draw_gz;
            m_bm_ocpn_draw_gz_grey = m_bm_night_ocpn_draw_gz_grey;
            m_bm_ocpn_draw_pil = m_bm_night_ocpn_draw_pil;
            m_bm_ocpn_draw_pil_grey = m_bm_night_ocpn_draw_pil_grey;
            break;
        default:
            break;
    }
}

void ODicons::CreateSchemeIcons()
{
    m_ColourScheme = PI_GLOBAL_COLOR_SCHEME_DAY;
    m_bm_day_ocpn_draw_grey_pi = m_bm_ocpn_draw_grey_pi;
    m_bm_day_ocpn_draw_boundary = m_bm_ocpn_draw_boundary;
    m_bm_day_ocpn_draw_boundary_grey = m_bm_ocpn_draw_boundary_grey;
    m_bm_day_ocpn_draw_point = m_bm_ocpn_draw_point;
    m_bm_day_ocpn_draw_point_grey = m_bm_ocpn_draw_point_grey;
    m_bm_day_ocpn_draw_textpoint = m_bm_ocpn_draw_textpoint;
    m_bm_day_ocpn_draw_textpoint_grey = m_bm_ocpn_draw_textpoint_grey;
    m_bm_day_ocpn_draw_ebl = m_bm_ocpn_draw_ebl;
    m_bm_day_ocpn_draw_ebl_grey = m_bm_ocpn_draw_ebl_grey;
    m_bm_day_ocpn_draw_dr = m_bm_ocpn_draw_dr;
    m_bm_day_ocpn_draw_dr_grey = m_bm_ocpn_draw_dr_grey;
    m_bm_day_ocpn_draw_gz = m_bm_ocpn_draw_gz;
    m_bm_day_ocpn_draw_gz_grey = m_bm_ocpn_draw_gz_grey;
    m_bm_day_ocpn_draw_pil = m_bm_ocpn_draw_pil;
    m_bm_day_ocpn_draw_pil_grey = m_bm_ocpn_draw_pil_grey;

    m_bm_dusk_ocpn_draw_grey_pi = BuildDimmedToolBitmap(m_bm_ocpn_draw_grey_pi, 128);
    m_bm_dusk_ocpn_draw_boundary = BuildDimmedToolBitmap(m_bm_ocpn_draw_boundary, 128);
    m_bm_dusk_ocpn_draw_boundary_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_boundary_grey, 128);
    m_bm_dusk_ocpn_draw_point = BuildDimmedToolBitmap(m_bm_ocpn_draw_point, 128);
    m_bm_dusk_ocpn_draw_point_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_point_grey, 128);
    m_bm_dusk_ocpn_draw_textpoint = BuildDimmedToolBitmap(m_bm_ocpn_draw_textpoint, 128);
    m_bm_dusk_ocpn_draw_textpoint_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_textpoint_grey, 128);
    m_bm_dusk_ocpn_draw_ebl = BuildDimmedToolBitmap(m_bm_ocpn_draw_ebl, 128);
    m_bm_dusk_ocpn_draw_ebl_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_ebl_grey, 128);
    m_bm_dusk_ocpn_draw_dr = BuildDimmedToolBitmap(m_bm_ocpn_draw_dr, 128);
    m_bm_dusk_ocpn_draw_dr_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_dr_grey, 128);
    m_bm_dusk_ocpn_draw_gz = BuildDimmedToolBitmap(m_bm_ocpn_draw_gz, 128);
    m_bm_dusk_ocpn_draw_gz_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_gz_grey, 128);
    m_bm_dusk_ocpn_draw_pil = BuildDimmedToolBitmap(m_bm_ocpn_draw_pil, 128);
    m_bm_dusk_ocpn_draw_pil_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_pil_grey, 128);

    m_bm_night_ocpn_draw_grey_pi = BuildDimmedToolBitmap(m_bm_ocpn_draw_grey_pi, 32);
    m_bm_night_ocpn_draw_boundary = BuildDimmedToolBitmap(m_bm_ocpn_draw_boundary, 32);
    m_bm_night_ocpn_draw_boundary_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_boundary_grey, 32);
    m_bm_night_ocpn_draw_point = BuildDimmedToolBitmap(m_bm_ocpn_draw_point, 32);
    m_bm_night_ocpn_draw_point_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_point_grey, 32);
    m_bm_night_ocpn_draw_textpoint = BuildDimmedToolBitmap(m_bm_ocpn_draw_textpoint, 32);
    m_bm_night_ocpn_draw_textpoint_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_textpoint_grey, 32);
    m_bm_night_ocpn_draw_ebl = BuildDimmedToolBitmap(m_bm_ocpn_draw_ebl, 32);
    m_bm_night_ocpn_draw_ebl_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_ebl_grey, 32);
    m_bm_night_ocpn_draw_dr = BuildDimmedToolBitmap(m_bm_ocpn_draw_dr, 32);
    m_bm_night_ocpn_draw_dr_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_dr_grey, 32);
    m_bm_night_ocpn_draw_gz = BuildDimmedToolBitmap(m_bm_ocpn_draw_gz, 32);
    m_bm_night_ocpn_draw_gz_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_gz_grey, 32);
    m_bm_night_ocpn_draw_pil = BuildDimmedToolBitmap(m_bm_ocpn_draw_pil, 32);
    m_bm_night_ocpn_draw_pil_grey = BuildDimmedToolBitmap(m_bm_ocpn_draw_pil_grey, 32);

}

wxBitmap ODicons::BuildDimmedToolBitmap(wxBitmap bmp_normal, unsigned char dim_ratio)
{
    wxImage img_dup = bmp_normal.ConvertToImage();
    
    if( !img_dup.IsOk() )
        return bmp_normal;
   
    if(dim_ratio < 200)
    {
        //  Create a dimmed version of the image/bitmap
        int gimg_width = img_dup.GetWidth();
        int gimg_height = img_dup.GetHeight();
        
        double factor = (double)(dim_ratio) / 256.0;
        
        for(int iy=0 ; iy < gimg_height ; iy++)
        {
            for(int ix=0 ; ix < gimg_width ; ix++)
            {
                if(!img_dup.IsTransparent(ix, iy))
                {
                    wxImage::RGBValue rgb(img_dup.GetRed(ix, iy), img_dup.GetGreen(ix, iy), img_dup.GetBlue(ix, iy));
                    wxImage::HSVValue hsv = wxImage::RGBtoHSV(rgb);
                    hsv.value = hsv.value * factor;
                    wxImage::RGBValue nrgb = wxImage::HSVtoRGB(hsv);
                    img_dup.SetRGB(ix, iy, nrgb.red, nrgb.green, nrgb.blue);
                }
            }
        }
    }
    
    //  Make a bitmap
    wxBitmap toolBarBitmap;
    
#ifdef __WXMSW__
    wxBitmap tbmp(img_dup.GetWidth(),img_dup.GetHeight(),-1);
    wxMemoryDC dwxdc;
    dwxdc.SelectObject(tbmp);
    
    toolBarBitmap = wxBitmap(img_dup, (wxDC &)dwxdc);
#else
    toolBarBitmap = wxBitmap(img_dup);
#endif
    
    // store it
    return toolBarBitmap;
}
