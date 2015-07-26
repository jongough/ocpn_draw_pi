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

extern void initialize_images(void);

extern void *g_ppimgr;
extern wxBitmap *_img_ocpn_draw_pi;
extern wxBitmap *_img_ocpn_draw_gray_pi;
extern wxBitmap *_img_ocpn_draw;
extern wxBitmap *_img_ocpn_draw_boundary;
extern wxBitmap *_img_ocpn_draw_boundary_gray;
extern wxBitmap *_img_ocpn_draw_point;
extern wxBitmap *_img_ocpn_draw_point_gray;
extern wxBitmap *_img_ocpn_draw_textpoint;
extern wxBitmap *_img_ocpn_draw_textpoint_gray;
extern wxBitmap *_img_ocpn_draw_ebl;
extern wxBitmap *_img_ocpn_draw_ebl_gray;
extern const wxBitmap *_img_Bullet_green;
extern const wxBitmap *_img_Bullet_red;
extern const wxBitmap *_img_Bullet_yellow;
#endif /* ODICONS_H */
