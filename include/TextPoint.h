/***************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Text points 
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

#ifndef TEXTPOINT_H
#define TEXTPOINT_H

#include "ODPoint.h"

class TextPoint : public ODPoint
{
    public:
    TextPoint();
    TextPoint(double lat, double lon, const wxString& icon_ident, const wxString& name, const wxString &pGUID = wxT("") , bool bAddToList = true);
    TextPoint(const TextPoint& other);
    ~TextPoint();
    
    void Draw(ocpnDC& dc, wxPoint *rpn = NULL);
    
    protected:
    private:
        wxTextCtrl *m_pstText;
        
};

#endif // TEXTPOINT_H
