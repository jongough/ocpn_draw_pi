/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Layer to use wxDC or opengl
 * Author:   Jon Gough based on work by Sean D'Epagnier
 *
 ***************************************************************************
 *   Copyright (C) 2011 by Sean D'Epagnier                                 *
 *   sean at depagnier dot com                                             *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 *
 *f
 */


#ifndef __ODDC_H__
#define __ODDC_H__

#include "TexFont.h"

//----------------------------------------------------------------------------
// ODDC
//----------------------------------------------------------------------------

class wxGLCanvas;

class ODDC
{
public:
     ODDC(wxGLCanvas &canvas);
     ODDC(wxDC &pdc);
     ODDC();

     ~ODDC();

     void SetBackground( const wxBrush &brush );
     void SetPen( const wxPen &pen);
     void SetBrush( const wxBrush &brush);
     void SetTextForeground(const wxColour &colour);
     void SetFont(const wxFont& font);
     static void SetGLAttrs( bool highQuality );
     void SetGLStipple() const;

     const wxPen& GetPen() const;
     const wxBrush& GetBrush() const;
     const wxFont& GetFont() const;

     void GetSize(wxCoord *width, wxCoord *height) const;

     void DrawLine( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, bool b_hiqual = true);
     void DrawLines( int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0, bool b_hiqual = true);

     void StrokeLine( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);
     void StrokeLine( wxPoint a, wxPoint b) { StrokeLine(a.x, a.y, b.x, b.y); }
     void StrokeLines( int n, wxPoint *points);
     
     void DrawArc( wxCoord xc, wxCoord yc, wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, bool b_hiqual = true);
     void StrokeArc( wxCoord xc, wxCoord yc, wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);
     void StrokeSector( wxCoord xc, wxCoord yc, wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord x3, wxCoord y3, wxCoord x4, wxCoord y4 );
     
     void Clear();
     void DrawRectangle( wxCoord x, wxCoord y, wxCoord w, wxCoord h );
     void DrawRoundedRectangle( wxCoord x, wxCoord y, wxCoord w, wxCoord h, wxCoord rr );
     void DrawCircle(wxCoord x, wxCoord y, wxCoord radius);
     void DrawCircle(const wxPoint &pt, wxCoord radius) { DrawCircle(pt.x, pt.y, radius); }
     void DrawDisk( wxCoord x, wxCoord y, wxCoord innerRadius, wxCoord outerRadius );
     void DrawSector( wxCoord xc, wxCoord yc, wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord x3, wxCoord y3, wxCoord x4, wxCoord y4  );
     void StrokeCircle(wxCoord x, wxCoord y, wxCoord radius);

     void DrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
     void DrawPolygon(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0, float scale =1.0);
     void DrawPolygonTessellated(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0);
     void DrawPolygonsTessellated(int n, int npoint[], wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0);
     void StrokePolygon(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0, float scale = 1.0);

     void DrawBitmap(const wxBitmap &bitmap, wxCoord x, wxCoord y, bool usemask);

     void DrawText(const wxString &text, wxCoord x, wxCoord y);
     void GetTextExtent(const wxString &string, wxCoord *w, wxCoord *h, wxCoord *descent = NULL,
                        wxCoord *externalLeading = NULL, wxFont *font = NULL) const;

     void ResetBoundingBox();
     void CalcBoundingBox(wxCoord x, wxCoord y);

     void DestroyClippingRegion() {}

     wxDC *GetDC() const { return dc; }
     
     void SetTextureSize( int width, int height );

protected:
     bool ConfigurePen();
     bool ConfigureBrush();

     void GLDrawBlendData(wxCoord x, wxCoord y, wxCoord w, wxCoord h,
                          int format, const unsigned char *data);
     void DrawGLThickLine( float x1, float y1, float x2, float y2, wxPen pen, bool b_hiqual );
	 void DrawGLThickLines(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset, wxPen pen, bool b_hiqual);
	 void DrawEndCap(float x1, float y1, float t1, float angle);
     
     wxGLCanvas *glcanvas;
     wxDC *dc;
     wxPen m_pen;
     wxBrush m_brush;
     wxColour m_textforegroundcolour;
     wxFont m_font;

#ifdef ocpnUSE_GL     
     TexFont m_texfont;
#endif
     

#if  wxUSE_GRAPHICS_CONTEXT == 1
     wxGraphicsContext *pgc;
#endif
};

#endif
