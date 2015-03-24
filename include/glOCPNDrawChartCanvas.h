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

#ifndef GLOCPNDRAWCHARTCANVAS_H
#define GLOCPNDRAWCHARTCANVAS_H

#include <glChartCanvas.h>
#include "ocpn_plugin.h"


class glOCPNDrawChartCanvas : public glChartCanvas
{
    public:
        glOCPNDrawChartCanvas(wxWindow *parent);
        virtual ~glOCPNDrawChartCanvas();
        
        void DrawAllPathsAndOCPNPoints( PlugIn_ViewPort &vp, OCPNRegion &region );

    protected:
    private:
//#ifdef ocpnUSE_GL
      glChartCanvas *m_glcc;
      wxGLContext   *m_pGLcontext;
//#endif
    
};

#endif // GLOCPNDRAWCHARTCANVAS_H
