/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPN Draw Utility functions support
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
 
#ifndef ODUTILS_H
#define ODUTILS_H
 
 const wxChar   *ParseGPXDateTime( wxDateTime &dt, const wxChar *datetime );
 wxString       GetUUID(void);
 int            GetRandomNumber(int range_min, int range_max); 
 void           MenuPrepend( wxMenu *menu, int id, wxString label);
 void           MenuAppend( wxMenu *menu, int id, wxString label);
 double         sign( double x );
 double         FNipart( double x );
 double         FNday( int y, int m, int d, int h );
 double         FNrange( double x );
 double         getLMT( double ut, double lon );
 void           SetGlobalLocale( void );
 void           ResetGlobalLocale( void );
 bool           pointInPolygon(int polyCorners, double *polyX, double *polyY, double x, double y);
 bool           GetLineIntersection(double p0_x, double p0_y, double p1_x, double p1_y, double p2_x, double p2_y, double p3_x, double p3_y, double *i_x, double *i_y);
 int            ArcSectorPoints( wxPoint *&points, wxCoord xc, wxCoord yc, wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord x3, wxCoord y3, wxCoord x4, wxCoord y4, bool bHighQuality );
 wxString       WrapText(wxWindow *win, const wxString& text, int widthMax);
 wxString       WrapString(const wxString &text, int widthMax);
#endif
