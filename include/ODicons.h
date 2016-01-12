/***************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  ODicons
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

#ifndef ODICONS_H
#define ODICONS_H 1

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

//forward declare
class wxSVGDocument;

class ODicons
{
public:
    ODicons();
    ~ODicons();
    
    void    initialize_images(void);
    bool    ScaleIcons(void);
    wxSize  GetIconSize(void);
    
    wxBitmap    *m_p_bm_ocpn_draw_pi;
    wxBitmap    *m_p_bm_ocpn_draw_grey_pi;
    wxBitmap    *m_p_bm_ocpn_draw_boundary;
    wxBitmap    *m_p_bm_ocpn_draw_boundary_grey;
    wxBitmap    *m_p_bm_ocpn_draw_point;
    wxBitmap    *m_p_bm_ocpn_draw_point_grey;
    wxBitmap    *m_p_bm_ocpn_draw_textpoint;
    wxBitmap    *m_p_bm_ocpn_draw_textpoint_grey;
    wxBitmap    *m_p_bm_ocpn_draw_ebl;
    wxBitmap    *m_p_bm_ocpn_draw_ebl_grey;
    wxBitmap    *m_p_bm_ocpn_draw_dr;
    wxBitmap    *m_p_bm_ocpn_draw_dr_grey;
    
    wxString    m_s_ocpn_draw_pi;
    wxString    m_s_ocpn_draw_grey_pi;
    wxString    m_s_ocpn_draw_boundary;
    wxString    m_s_ocpn_draw_boundary_grey;
    wxString    m_s_ocpn_draw_point;
    wxString    m_s_ocpn_draw_point_grey;
    wxString    m_s_ocpn_draw_textpoint;
    wxString    m_s_ocpn_draw_textpoint_grey;
    wxString    m_s_ocpn_draw_ebl;
    wxString    m_s_ocpn_draw_ebl_grey;
    wxString    m_s_ocpn_draw_dr;
    wxString    m_s_ocpn_draw_dr_grey;

private:
    wxBitmap *ScaleIcon( wxBitmap *p_wxBitmap, double sf );

    #ifdef OD_USE_SVG
    wxBitmap *LoadSVG( const wxString filename, wxSVGDocument **svgDco, wxImage **Image, unsigned int width = -1, unsigned int height = -1 );
    wxBitmap *ScaleIcon( wxSVGDocument *p_svgDoc, wxImage *p_wxImage, double sf );

    wxSVGDocument   *m_p_svgd_ocpn_draw_pi;
    wxSVGDocument   *m_p_svgd_ocpn_draw_grey_pi;
    wxSVGDocument   *m_p_svgd_ocpn_draw_boundary;
    wxSVGDocument   *m_p_svgd_ocpn_draw_boundary_grey;
    wxSVGDocument   *m_p_svgd_ocpn_draw_point;
    wxSVGDocument   *m_p_svgd_ocpn_draw_point_grey;
    wxSVGDocument   *m_p_svgd_ocpn_draw_textpoint;
    wxSVGDocument   *m_p_svgd_ocpn_draw_textpoint_grey;
    wxSVGDocument   *m_p_svgd_ocpn_draw_ebl;
    wxSVGDocument   *m_p_svgd_ocpn_draw_ebl_grey;
    wxSVGDocument   *m_p_svgd_ocpn_draw_dr;
    wxSVGDocument   *m_p_svgd_ocpn_draw_dr_grey;
#endif ODraw_USE_SVG
    
    wxImage         *m_p_img_ocpn_draw_pi;
    wxImage         *m_p_img_ocpn_draw_grey_pi;
    wxImage         *m_p_img_ocpn_draw_boundary;
    wxImage         *m_p_img_ocpn_draw_boundary_grey;
    wxImage         *m_p_img_ocpn_draw_point;
    wxImage         *m_p_img_ocpn_draw_point_grey;
    wxImage         *m_p_img_ocpn_draw_textpoint;
    wxImage         *m_p_img_ocpn_draw_textpoint_grey;
    wxImage         *m_p_img_ocpn_draw_ebl;
    wxImage         *m_p_img_ocpn_draw_ebl_grey;
    wxImage         *m_p_img_ocpn_draw_dr;
    wxImage         *m_p_img_ocpn_draw_dr_grey;
    
    double          m_dScaleFactor;
};
#endif /* ODICONS_H */
