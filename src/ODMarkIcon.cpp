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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "ODMarkIcon.h"

#ifndef __OCPN__ANDROID__
#include <GL/gl.h>
#else
#include "qopengl.h"                  // this gives us the qt runtime gles2.h
#include "GL/gl_private.h"
#endif

//--------------------------------------------------------------------------------
//      ODMarkIcon   Implementation
//--------------------------------------------------------------------------------

// ODMarkIcon
ODMarkIcon::ODMarkIcon()
{
    picon_bitmap = 0;
    picon_bitmap_RGB = 0;
    picon_bitmap_Day = 0;
    picon_bitmap_Dusk = 0;
    picon_bitmap_Night = 0;
    icon_texture_RGB = 0;
    icon_texture_Day = 0;
    icon_texture_Dusk = 0;
    icon_texture_Night = 0;
}

ODMarkIcon::~ODMarkIcon()
{
    Delete( );
}

void ODMarkIcon::Delete()
{
    delete picon_bitmap;
    delete picon_bitmap_RGB;
    delete picon_bitmap_Day;
    delete picon_bitmap_Dusk;
    delete picon_bitmap_Night;
    
#ifdef ocpnUSE_GL
    if (icon_texture_RGB != 0) {
        glDeleteTextures(1, &icon_texture_RGB);
        icon_texture_RGB = 0;
    }
    if (icon_texture_Day != 0) {
        glDeleteTextures(1, &icon_texture_Day);
        icon_texture_Day = 0;
    }
    if (icon_texture_Dusk != 0) {
        glDeleteTextures(1, &icon_texture_Dusk);
        icon_texture_Dusk = 0;
    }
    
    if (icon_texture_Night != 0) {
        glDeleteTextures(1, &icon_texture_Night);
        icon_texture_Night = 0;
    }
#endif
}
    
