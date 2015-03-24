/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  glOCPNDrawChartCanvas 
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

#include "glOCPNDrawChartCanvas.h"
#include "Path.h"
#include "PointMan.h"
#include "ocpndc.h"
#include "chcanv.h"

extern PathList        *pPathList;
extern PointMan        *pOCPNPointMan;

glOCPNDrawChartCanvas::glOCPNDrawChartCanvas(wxWindow *parent) : glChartCanvas( parent )
{
    //ctor
}

glOCPNDrawChartCanvas::~glOCPNDrawChartCanvas()
{
    //dtor
}

void glOCPNDrawChartCanvas::DrawAllPathsAndOCPNPoints( PlugIn_ViewPort &pivp, OCPNRegion &region )
{
    ocpnDC dc(*this);

    for(wxPathListNode *node = pPathList->GetFirst();
        node; node = node->GetNext() ) {
        Path *pPathDraw = node->GetData();
        if( !pPathDraw )
            continue;

        /* defer rendering active routes until later */ 
        //if( pPathDraw->IsActive() || pPathDraw->IsSelected() )
//            continue;

        /* this routine is called very often, so rather than using the
           wxBoundingBox::Intersect routine, do the comparisons directly
           to reduce the number of floating point comparisons */

//        const wxBoundingBox &vp_box = vp.GetBBox(), &test_box = pPathDraw->GetBBox();
        const wxBoundingBox &test_box = pPathDraw->GetBBox();
        if(test_box.GetMaxY() < pivp.lat_min)
            continue;
//        if(test_box.GetMaxY() < vp_box.GetMinY())
//            continue;

        if(test_box.GetMinY() > pivp.lon_max)
            continue;

        //double vp_minx = vp_box.GetMinX(), vp_maxx = vp_box.GetMaxX();
        double test_minx = test_box.GetMinX(), test_maxx = test_box.GetMaxX();

        // Path is not wholly outside viewport
        if(test_maxx >= pivp.lon_min && test_minx <= pivp.lon_max) {
            pPathDraw->DrawGL( (ViewPort &)pivp, region );
        } else if( pivp.lat_max > 180. ) {
            if(test_minx + 360 <= pivp.lon_max && test_maxx + 360 >= pivp.lon_min)
                pPathDraw->DrawGL( (ViewPort &)pivp, region );
        } else if( pPathDraw->CrossesIDL() || pivp.lon_min < -180. ) {
            if(test_maxx - 360 >= pivp.lon_min && test_minx - 360 <= pivp.lon_max)
                pPathDraw->DrawGL( (ViewPort &)pivp, region );
        }

    }

    /* OCPNPoints not drawn as part of routes */
    ViewPort vp = (ViewPort &)pivp;
    if( vp.GetBBox().GetValid() && pOCPNPointMan) {
        for(wxOCPNPointListNode *pnode = pOCPNPointMan->GetOCPNPointList()->GetFirst(); pnode; pnode = pnode->GetNext() ) {
            OCPNPoint *pOP = pnode->GetData();
            //if( pOP && (!pOP->m_bIsInPath && !pOP->m_bIsInTrack ) )
                pOP->DrawGL( (ViewPort &)vp, region );
        }
    }
    
}

