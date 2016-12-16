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

#ifdef ODraw_USE_SVG
#include "svg.h"
#endif // ODraw_USE_SVG

#include <wx/stdpaths.h>


extern wxString g_SData_Locn;
extern wxString *g_pHome_locn;
extern wxString *g_pData;
extern wxString *g_pImage;

ODicons::ODicons()
{
    m_dScaleFactor = 1.0;
    m_bUpdateIcons = false;
    
    initialize_images();
}

ODicons::~ODicons()
{
    
}
void ODicons::initialize_images(void)
{
    wxFileName fn;
//#ifdef __WXOSX__
// Not in this case - the icons are part of the plugin package, not it's configuration data, so they have nothing to do in the user's preferences directory
//    wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
//    fn.SetPath(std_path.GetUserConfigDir());  // should be ~/Library/Preferences
//    fn.AppendDir(_T("opencpn"));
//    fn.AppendDir( wxT("plugins") );
//    fn.AppendDir(wxT("ocpn_draw_pi"));
//#else
    fn.SetPath(*GetpSharedDataLocation());
    fn.AppendDir( wxT("plugins") );
    fn.AppendDir(wxT("ocpn_draw_pi"));
    fn.AppendDir(wxT("data"));
//#endif
    wxString s = _("ocpn_draw_pi data location");
    wxLogMessage( wxT("%s: %s"), s.c_str(), fn.GetFullPath().c_str());
    

#ifdef ODraw_USE_SVG
    fn.SetFullName(wxT("ODManager.svg"));
    m_s_ocpn_draw_pi = fn.GetFullPath();
    m_p_bm_ocpn_draw_pi = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_pi, &m_p_img_ocpn_draw_pi );
    fn.SetFullName(wxT("ODManagergrey.svg"));
    m_s_ocpn_draw_grey_pi = fn.GetFullPath();
    m_p_bm_ocpn_draw_grey_pi = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_grey_pi, &m_p_img_ocpn_draw_grey_pi );
    
    fn.SetFullName(wxT("boundary.svg"));
    m_p_bm_ocpn_draw_boundary = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_boundary, &m_p_img_ocpn_draw_boundary );
    m_s_ocpn_draw_boundary = fn.GetFullPath();
    fn.SetFullName(wxT("boundarygrey.svg"));
    m_s_ocpn_draw_boundary_grey = fn.GetFullPath();
    m_p_bm_ocpn_draw_boundary_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_boundary_grey, &m_p_img_ocpn_draw_boundary_grey );
    
    fn.SetFullName(wxT("pointbutton.svg"));
    m_s_ocpn_draw_point = fn.GetFullPath();
    m_p_bm_ocpn_draw_point = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_point, &m_p_img_ocpn_draw_point );
    fn.SetFullName(wxT("pointbuttongrey.svg"));
    m_s_ocpn_draw_point_grey = fn.GetFullPath();
    m_p_bm_ocpn_draw_point_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_point_grey, &m_p_img_ocpn_draw_point_grey );
    
    fn.SetFullName(wxT("textpointbutton.svg"));
    m_s_ocpn_draw_textpoint = fn.GetFullPath();
    m_p_bm_ocpn_draw_textpoint = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_textpoint, &m_p_img_ocpn_draw_textpoint );
    fn.SetFullName(wxT("textpointbuttongrey.svg"));
    m_s_ocpn_draw_textpoint_grey = fn.GetFullPath();
    m_p_bm_ocpn_draw_textpoint_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_textpoint_grey, &m_p_img_ocpn_draw_textpoint_grey );
    
    fn.SetFullName(wxT("EBL.svg"));
    m_s_ocpn_draw_ebl = fn.GetFullPath();
    m_p_bm_ocpn_draw_ebl = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_ebl, &m_p_img_ocpn_draw_ebl );
    fn.SetFullName(wxT("EBLgrey.svg"));
    m_s_ocpn_draw_ebl_grey = fn.GetFullPath();
    m_p_bm_ocpn_draw_ebl_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_ebl_grey, &m_p_img_ocpn_draw_ebl_grey );
    
    fn.SetFullName(wxT("DR.svg"));
    m_s_ocpn_draw_dr = fn.GetFullPath();
    m_p_bm_ocpn_draw_dr = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_dr, &m_p_img_ocpn_draw_dr );
    fn.SetFullName(wxT("DRgrey.svg"));
    m_s_ocpn_draw_dr_grey = fn.GetFullPath();
    m_p_bm_ocpn_draw_dr_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_dr_grey, &m_p_img_ocpn_draw_dr_grey );

    fn.SetFullName(wxT("GZ.svg"));
    m_s_ocpn_draw_gz = fn.GetFullPath();
    m_p_bm_ocpn_draw_gz = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_gz, &m_p_img_ocpn_draw_gz );
    fn.SetFullName(wxT("GZgrey.svg"));
    m_s_ocpn_draw_gz_grey = fn.GetFullPath();
    m_p_bm_ocpn_draw_gz_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_gz_grey, &m_p_img_ocpn_draw_gz_grey );

    fn.SetFullName(wxT("PIL.svg"));
    m_s_ocpn_draw_pil = fn.GetFullPath();
    m_p_bm_ocpn_draw_pil = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_pil, &m_p_img_ocpn_draw_pil );
    fn.SetFullName(wxT("PILgrey.svg"));
    m_s_ocpn_draw_pil_grey = fn.GetFullPath();
    m_p_bm_ocpn_draw_pil_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_pil_grey, &m_p_img_ocpn_draw_pil_grey );
#else
    fn.SetFullName(wxT("ODManager.png"));
    m_p_bm_ocpn_draw_pi = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("ODManagergrey.png"));
    m_p_bm_ocpn_draw_grey_pi = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("boundary.png"));
    m_p_bm_ocpn_draw_boundary = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("boundarygrey.png"));
    m_p_bm_ocpn_draw_boundary_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("pointbutton.png"));
    m_p_bm_ocpn_draw_point = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("pointbuttongrey.png"));
    m_p_bm_ocpn_draw_point_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("textpointbutton.png"));
    m_p_bm_ocpn_draw_textpoint = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("textpointbuttongrey.png"));
    m_p_bm_ocpn_draw_textpoint_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("EBL.png"));
    m_p_bm_ocpn_draw_ebl = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("EBLgrey.png"));
    m_p_bm_ocpn_draw_ebl_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("DR.png"));
    m_p_bm_ocpn_draw_dr = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("DRgrey.png"));
    m_p_bm_ocpn_draw_dr_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    
    fn.SetFullName(wxT("GZ.png"));
    m_p_bm_ocpn_draw_gz = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("GZgrey.png"));
    m_p_bm_ocpn_draw_gz_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );

    fn.SetFullName(wxT("PIL.png"));
    m_p_bm_ocpn_draw_pil = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
    fn.SetFullName(wxT("PILgrey.png"));
    m_p_bm_ocpn_draw_pil_grey = new wxBitmap( fn.GetFullPath(), wxBITMAP_TYPE_PNG );
#endif
    
    CreateSchemeIcons();
    ScaleIcons();
}

#ifdef ODraw_USE_SVG
wxBitmap *ODicons::LoadSVG( const wxString filename, wxSVGDocument **svgDoc, wxImage **Image, unsigned int width, unsigned int height )
{
    wxSVGDocument *newDoc = new wxSVGDocument;
    *svgDoc = newDoc;
    if( newDoc->Load(filename) ) {
        wxImage *newImage = new wxImage(newDoc->Render( width, height, NULL, true, true));
        *Image = newImage;
        return new wxBitmap( *newImage );
    }
    else
        *Image = new wxImage(width, height);
    return new wxBitmap(width, height);
}

wxBitmap *ODicons::ScaleIcon( wxSVGDocument *p_svgDoc, wxImage *p_wxImage, double sf )
{
    if( p_svgDoc && p_wxImage ) {
        wxImage *p_Image = new wxImage(p_svgDoc->Render( p_wxImage->GetWidth() * sf, p_wxImage->GetHeight() * sf, NULL, true, true));
        return new wxBitmap( *p_Image );
    }
    else
        return new wxBitmap(32 * sf, 32 * sf); //scalled default blank bitmap
}
#endif // OD_USE_SVG

wxBitmap *ODicons::ScaleIcon( wxBitmap *p_wxBitmap, double sf )
{
    wxImage scaled_image = p_wxBitmap->ConvertToImage();
    return new wxBitmap(scaled_image.Scale(scaled_image.GetWidth() * sf, scaled_image.GetHeight() * sf, wxIMAGE_QUALITY_HIGH));
}
bool ODicons::ScaleIcons()
{
    if(!SetScaleFactor()) return false;
    

#ifdef ODraw_USE_SVG
    
    // Dont scale the OD manager as that should be done by the OCPN toolbar
    //m_p_bm_ocpn_draw_pi = ScaleIcon( m_p_svgd_ocpn_draw_pi, m_p_img_ocpn_draw_pi, m_dScaleFactor );
    //m_p_bm_ocpn_draw_grey_pi = ScaleIcon( m_p_svgd_ocpn_draw_grey_pi, m_p_img_ocpn_draw_grey_pi, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_boundary = ScaleIcon( m_p_svgd_ocpn_draw_boundary, m_p_img_ocpn_draw_boundary, m_dScaleFactor );
    m_p_bm_ocpn_draw_boundary_grey = ScaleIcon( m_p_svgd_ocpn_draw_boundary_grey, m_p_img_ocpn_draw_boundary_grey, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_point = ScaleIcon( m_p_svgd_ocpn_draw_point, m_p_img_ocpn_draw_point, m_dScaleFactor );
    m_p_bm_ocpn_draw_point_grey = ScaleIcon( m_p_svgd_ocpn_draw_point_grey, m_p_img_ocpn_draw_point_grey, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_textpoint = ScaleIcon( m_p_svgd_ocpn_draw_textpoint, m_p_img_ocpn_draw_textpoint, m_dScaleFactor );
    m_p_bm_ocpn_draw_textpoint_grey = ScaleIcon( m_p_svgd_ocpn_draw_textpoint_grey, m_p_img_ocpn_draw_textpoint_grey, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_ebl = ScaleIcon( m_p_svgd_ocpn_draw_ebl, m_p_img_ocpn_draw_ebl, m_dScaleFactor );
    m_p_bm_ocpn_draw_ebl_grey = ScaleIcon( m_p_svgd_ocpn_draw_ebl_grey, m_p_img_ocpn_draw_ebl_grey, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_dr = ScaleIcon( m_p_svgd_ocpn_draw_dr, m_p_img_ocpn_draw_dr, m_dScaleFactor );
    m_p_bm_ocpn_draw_dr_grey = ScaleIcon( m_p_svgd_ocpn_draw_dr_grey, m_p_img_ocpn_draw_dr_grey, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_gz = ScaleIcon( m_p_svgd_ocpn_draw_gz, m_p_img_ocpn_draw_gz, m_dScaleFactor );
    m_p_bm_ocpn_draw_gz_grey = ScaleIcon( m_p_svgd_ocpn_draw_gz_grey, m_p_img_ocpn_draw_gz_grey, m_dScaleFactor );

    m_p_bm_ocpn_draw_pil = ScaleIcon( m_p_svgd_ocpn_draw_pil, m_p_img_ocpn_draw_pil, m_dScaleFactor );
    m_p_bm_ocpn_draw_pil_grey = ScaleIcon( m_p_svgd_ocpn_draw_pil_grey, m_p_img_ocpn_draw_pil_grey, m_dScaleFactor );
#else
    m_p_bm_ocpn_draw_boundary = ScaleIcon( m_p_bm_ocpn_draw_boundary, m_dScaleFactor );
    m_p_bm_ocpn_draw_boundary_grey = ScaleIcon( m_p_bm_ocpn_draw_boundary_grey, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_point = ScaleIcon( m_p_bm_ocpn_draw_point, m_dScaleFactor );
    m_p_bm_ocpn_draw_point_grey = ScaleIcon( m_p_bm_ocpn_draw_point_grey, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_textpoint = ScaleIcon( m_p_bm_ocpn_draw_textpoint, m_dScaleFactor );
    m_p_bm_ocpn_draw_textpoint_grey = ScaleIcon( m_p_bm_ocpn_draw_textpoint_grey, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_ebl = ScaleIcon( m_p_bm_ocpn_draw_ebl, m_dScaleFactor );
    m_p_bm_ocpn_draw_ebl_grey = ScaleIcon( m_p_bm_ocpn_draw_ebl_grey, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_dr = ScaleIcon( m_p_bm_ocpn_draw_dr, m_dScaleFactor );
    m_p_bm_ocpn_draw_dr_grey = ScaleIcon( m_p_bm_ocpn_draw_dr_grey, m_dScaleFactor );
    
    m_p_bm_ocpn_draw_gz = ScaleIcon( m_p_bm_ocpn_draw_gz, m_dScaleFactor );
    m_p_bm_ocpn_draw_gz_grey = ScaleIcon( m_p_bm_ocpn_draw_gz_grey, m_dScaleFactor );

    m_p_bm_ocpn_draw_pil = ScaleIcon( m_p_bm_ocpn_draw_pil, m_dScaleFactor );
    m_p_bm_ocpn_draw_pil_grey = ScaleIcon( m_p_bm_ocpn_draw_pil_grey, m_dScaleFactor );

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
            m_p_bm_ocpn_draw_grey_pi = m_p_bm_day_ocpn_draw_grey_pi;
            m_p_bm_ocpn_draw_boundary = m_p_bm_day_ocpn_draw_boundary;
            m_p_bm_ocpn_draw_boundary_grey = m_p_bm_day_ocpn_draw_boundary_grey;
            m_p_bm_ocpn_draw_point = m_p_bm_day_ocpn_draw_point;
            m_p_bm_ocpn_draw_point_grey = m_p_bm_day_ocpn_draw_point_grey;
            m_p_bm_ocpn_draw_textpoint = m_p_bm_day_ocpn_draw_textpoint;
            m_p_bm_ocpn_draw_textpoint_grey = m_p_bm_day_ocpn_draw_textpoint_grey;
            m_p_bm_ocpn_draw_ebl = m_p_bm_day_ocpn_draw_ebl;
            m_p_bm_ocpn_draw_ebl_grey = m_p_bm_day_ocpn_draw_ebl_grey;
            m_p_bm_ocpn_draw_dr = m_p_bm_day_ocpn_draw_dr;
            m_p_bm_ocpn_draw_dr_grey = m_p_bm_day_ocpn_draw_dr_grey;
            m_p_bm_ocpn_draw_gz = m_p_bm_day_ocpn_draw_gz;
            m_p_bm_ocpn_draw_gz_grey = m_p_bm_day_ocpn_draw_gz_grey;
            m_p_bm_ocpn_draw_pil = m_p_bm_day_ocpn_draw_pil;
            m_p_bm_ocpn_draw_pil_grey = m_p_bm_day_ocpn_draw_pil_grey;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DUSK:
            m_p_bm_ocpn_draw_grey_pi = m_p_bm_dusk_ocpn_draw_grey_pi;
            m_p_bm_ocpn_draw_boundary = m_p_bm_dusk_ocpn_draw_boundary;
            m_p_bm_ocpn_draw_boundary_grey = m_p_bm_dusk_ocpn_draw_boundary_grey;
            m_p_bm_ocpn_draw_point = m_p_bm_dusk_ocpn_draw_point;
            m_p_bm_ocpn_draw_point_grey = m_p_bm_dusk_ocpn_draw_point_grey;
            m_p_bm_ocpn_draw_textpoint = m_p_bm_dusk_ocpn_draw_textpoint;
            m_p_bm_ocpn_draw_textpoint_grey = m_p_bm_dusk_ocpn_draw_textpoint_grey;
            m_p_bm_ocpn_draw_ebl = m_p_bm_dusk_ocpn_draw_ebl;
            m_p_bm_ocpn_draw_ebl_grey = m_p_bm_dusk_ocpn_draw_ebl_grey;
            m_p_bm_ocpn_draw_dr = m_p_bm_dusk_ocpn_draw_dr;
            m_p_bm_ocpn_draw_dr_grey = m_p_bm_dusk_ocpn_draw_dr_grey;
            m_p_bm_ocpn_draw_gz = m_p_bm_dusk_ocpn_draw_gz;
            m_p_bm_ocpn_draw_gz_grey = m_p_bm_dusk_ocpn_draw_gz_grey;
            m_p_bm_ocpn_draw_pil = m_p_bm_dusk_ocpn_draw_pil;
            m_p_bm_ocpn_draw_pil_grey = m_p_bm_dusk_ocpn_draw_pil_grey;
            break;
        case PI_GLOBAL_COLOR_SCHEME_NIGHT:
            m_p_bm_ocpn_draw_grey_pi = m_p_bm_night_ocpn_draw_grey_pi;
            m_p_bm_ocpn_draw_boundary = m_p_bm_night_ocpn_draw_boundary;
            m_p_bm_ocpn_draw_boundary_grey = m_p_bm_night_ocpn_draw_boundary_grey;
            m_p_bm_ocpn_draw_point = m_p_bm_night_ocpn_draw_point;
            m_p_bm_ocpn_draw_point_grey = m_p_bm_night_ocpn_draw_point_grey;
            m_p_bm_ocpn_draw_textpoint = m_p_bm_night_ocpn_draw_textpoint;
            m_p_bm_ocpn_draw_textpoint_grey = m_p_bm_night_ocpn_draw_textpoint_grey;
            m_p_bm_ocpn_draw_ebl = m_p_bm_night_ocpn_draw_ebl;
            m_p_bm_ocpn_draw_ebl_grey = m_p_bm_night_ocpn_draw_ebl_grey;
            m_p_bm_ocpn_draw_dr = m_p_bm_night_ocpn_draw_dr;
            m_p_bm_ocpn_draw_dr_grey = m_p_bm_night_ocpn_draw_dr_grey;
            m_p_bm_ocpn_draw_gz = m_p_bm_night_ocpn_draw_gz;
            m_p_bm_ocpn_draw_gz_grey = m_p_bm_night_ocpn_draw_gz_grey;
            m_p_bm_ocpn_draw_pil = m_p_bm_night_ocpn_draw_pil;
            m_p_bm_ocpn_draw_pil_grey = m_p_bm_night_ocpn_draw_pil_grey;
            break;
        default:
            break;
    }
}

void ODicons::CreateSchemeIcons()
{
    m_p_bm_day_ocpn_draw_grey_pi = m_p_bm_ocpn_draw_grey_pi;
    m_p_bm_day_ocpn_draw_boundary = m_p_bm_ocpn_draw_boundary;
    m_p_bm_day_ocpn_draw_boundary_grey = m_p_bm_ocpn_draw_boundary_grey;
    m_p_bm_day_ocpn_draw_point = m_p_bm_ocpn_draw_point;
    m_p_bm_day_ocpn_draw_point_grey = m_p_bm_ocpn_draw_point_grey;
    m_p_bm_day_ocpn_draw_textpoint = m_p_bm_ocpn_draw_textpoint;
    m_p_bm_day_ocpn_draw_textpoint_grey = m_p_bm_ocpn_draw_textpoint_grey;
    m_p_bm_day_ocpn_draw_ebl = m_p_bm_ocpn_draw_ebl;
    m_p_bm_day_ocpn_draw_ebl_grey = m_p_bm_ocpn_draw_ebl_grey;
    m_p_bm_day_ocpn_draw_dr = m_p_bm_ocpn_draw_dr;
    m_p_bm_day_ocpn_draw_dr_grey = m_p_bm_ocpn_draw_dr_grey;
    m_p_bm_day_ocpn_draw_gz = m_p_bm_ocpn_draw_gz;
    m_p_bm_day_ocpn_draw_gz_grey = m_p_bm_ocpn_draw_gz_grey;
    m_p_bm_day_ocpn_draw_pil = m_p_bm_ocpn_draw_pil;
    m_p_bm_day_ocpn_draw_pil_grey = m_p_bm_ocpn_draw_pil_grey;

    m_p_bm_dusk_ocpn_draw_grey_pi = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_grey_pi, 128);
    m_p_bm_dusk_ocpn_draw_boundary = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_boundary, 128);
    m_p_bm_dusk_ocpn_draw_boundary_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_boundary_grey, 128);
    m_p_bm_dusk_ocpn_draw_point = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_point, 128);
    m_p_bm_dusk_ocpn_draw_point_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_point_grey, 128);
    m_p_bm_dusk_ocpn_draw_textpoint = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_textpoint, 128);
    m_p_bm_dusk_ocpn_draw_textpoint_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_textpoint_grey, 128);
    m_p_bm_dusk_ocpn_draw_ebl = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_ebl, 128);
    m_p_bm_dusk_ocpn_draw_ebl_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_ebl_grey, 128);
    m_p_bm_dusk_ocpn_draw_dr = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_dr, 128);
    m_p_bm_dusk_ocpn_draw_dr_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_dr_grey, 128);
    m_p_bm_dusk_ocpn_draw_gz = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_gz, 128);
    m_p_bm_dusk_ocpn_draw_gz_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_gz_grey, 128);
    m_p_bm_dusk_ocpn_draw_pil = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_pil, 128);
    m_p_bm_dusk_ocpn_draw_pil_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_pil_grey, 128);

    m_p_bm_night_ocpn_draw_grey_pi = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_grey_pi, 32);
    m_p_bm_night_ocpn_draw_boundary = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_boundary, 32);
    m_p_bm_night_ocpn_draw_boundary_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_boundary_grey, 32);
    m_p_bm_night_ocpn_draw_point = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_point, 32);
    m_p_bm_night_ocpn_draw_point_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_point_grey, 32);
    m_p_bm_night_ocpn_draw_textpoint = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_textpoint, 32);
    m_p_bm_night_ocpn_draw_textpoint_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_textpoint_grey, 32);
    m_p_bm_night_ocpn_draw_ebl = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_ebl, 32);
    m_p_bm_night_ocpn_draw_ebl_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_ebl_grey, 32);
    m_p_bm_night_ocpn_draw_dr = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_dr, 32);
    m_p_bm_night_ocpn_draw_dr_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_dr_grey, 32);
    m_p_bm_night_ocpn_draw_gz = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_gz, 32);
    m_p_bm_night_ocpn_draw_gz_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_gz_grey, 32);
    m_p_bm_night_ocpn_draw_pil = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_pil, 32);
    m_p_bm_night_ocpn_draw_pil_grey = BuildDimmedToolBitmap(m_p_bm_ocpn_draw_pil_grey, 32);

}

wxBitmap *ODicons::BuildDimmedToolBitmap(wxBitmap *pbmp_normal, unsigned char dim_ratio)
{
    wxImage img_dup = pbmp_normal->ConvertToImage();
    
    if( !img_dup.IsOk() ) return NULL;
    
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
    wxBitmap *ptoolBarBitmap;
    
#ifdef __WXMSW__
    wxBitmap tbmp(img_dup.GetWidth(),img_dup.GetHeight(),-1);
    wxMemoryDC dwxdc;
    dwxdc.SelectObject(tbmp);
    
    ptoolBarBitmap = new wxBitmap(img_dup, (wxDC &)dwxdc);
#else
    ptoolBarBitmap = new wxBitmap(img_dup);
#endif
    
    // store it
    return ptoolBarBitmap;
}
