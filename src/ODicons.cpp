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

#ifdef OD_USE_SVG
#include "svg.h"
#endif // OD_USE_SVG


extern wxString g_SData_Locn;
extern wxString *g_pHome_locn;
extern wxString *g_pData;
extern wxString *g_pImage;

ODicons::ODicons()
{
    //m_dScaleFactor = GetOCPNGUIToolScaleFactor_PlugIn();
    m_dScaleFactor = 1.0;
    
    initialize_images();
}

ODicons::~ODicons()
{
    
}
void ODicons::initialize_images(void)
{
    wxFileName fn;
    fn.SetPath(*GetpSharedDataLocation());
    fn.AppendDir( wxT("plugins") );
    fn.AppendDir(wxT("ocpn_draw_pi"));
    fn.AppendDir(wxT("data"));
    wxString s = _("ocpn_draw_pi data location");
    wxLogMessage( wxT("%s: %s"), s.c_str(), fn.GetFullPath().c_str());
    

#ifdef OD_USE_SVG
    fn.SetFullName(wxT("ODManager.svg"));
    m_p_bm_ocpn_draw_pi = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_pi, &m_p_img_ocpn_draw_pi );
    fn.SetFullName(wxT("ODManagergrey.svg"));
    m_p_bm_ocpn_draw_grey_pi = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_grey_pi, &m_p_img_ocpn_draw_grey_pi );
    
    fn.SetFullName(wxT("boundary.svg"));
    m_p_bm_ocpn_draw_boundary = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_boundary, &m_p_img_ocpn_draw_boundary );
    fn.SetFullName(wxT("boundarygrey.svg"));
    m_p_bm_ocpn_draw_boundary_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_boundary_grey, &m_p_img_ocpn_draw_boundary_grey );
    
    fn.SetFullName(wxT("pointbutton.svg"));
    m_p_bm_ocpn_draw_point = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_point, &m_p_img_ocpn_draw_point );
    fn.SetFullName(wxT("pointbuttongrey.svg"));
    m_p_bm_ocpn_draw_point_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_point_grey, &m_p_img_ocpn_draw_point_grey );
    
    fn.SetFullName(wxT("textpointbutton.svg"));
    m_p_bm_ocpn_draw_textpoint = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_textpoint, &m_p_img_ocpn_draw_textpoint );
    fn.SetFullName(wxT("textpointbuttongrey.svg"));
    m_p_bm_ocpn_draw_textpoint_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_textpoint_grey, &m_p_img_ocpn_draw_textpoint_grey );
    
    fn.SetFullName(wxT("EBL.svg"));
    m_p_bm_ocpn_draw_ebl = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_ebl, &m_p_img_ocpn_draw_ebl );
    fn.SetFullName(wxT("EBLgrey.svg"));
    m_p_bm_ocpn_draw_ebl_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_ebl_grey, &m_p_img_ocpn_draw_ebl_grey );
    
    fn.SetFullName(wxT("DR.svg"));
    m_p_bm_ocpn_draw_dr = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_dr, &m_p_img_ocpn_draw_dr );
    fn.SetFullName(wxT("DRgrey.svg"));
    m_p_bm_ocpn_draw_dr_grey = LoadSVG( fn.GetFullPath(), &m_p_svgd_ocpn_draw_dr_grey, &m_p_img_ocpn_draw_dr_grey );
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
#endif
}

#ifdef OD_USE_SVG
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
        return new wxBitmap(width, height);
}

wxBitmap *ODicons::ScaleIcon( wxSVGDocument *p_svgDoc, wxImage *p_wxImage, double sf )
{
    if(p_svgDoc) {
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
    //if(m_dScaleFactor == GetOCPNGUIToolScaleFactor_PlugIn()) return false;
    //m_dScaleFactor = GetOCPNGUIToolScaleFactor_PlugIn();
    
    m_dScaleFactor = 1.0;

    #ifdef OD_USE_SVG
    
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
    
#endif // OD_USE_SVG
    return true;
}

