/***************************************************************************
 *
 * Project:  OpenCPN
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
 ***************************************************************************
 */

#ifndef __ODMARKICON_H__
#define __ODMARKICON_H__

#include <wx/string.h>
#include <wx/bitmap.h>

class ODMarkIcon
{
public:
    ODMarkIcon();
    ~ODMarkIcon();

    void Delete();

    wxBitmap *picon_bitmap;
    wxBitmap *picon_bitmap_RGB;
    wxBitmap *picon_bitmap_Day;
    wxBitmap *picon_bitmap_Dusk;
    wxBitmap *picon_bitmap_Night;
    wxString icon_name;
    wxString icon_description;

    unsigned int icon_texture_RGB;
    unsigned int icon_texture_Day;
    unsigned int icon_texture_Dusk;
    unsigned int icon_texture_Night;
    unsigned int tex_w, tex_h;
};

#endif
