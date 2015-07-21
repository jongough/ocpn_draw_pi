/***************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Boundary Manager
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


#ifndef BOUNDARYMAN_H
#define BOUNDARYMAN_H

#include "PathMan.h"

class Boundary;

class BoundaryMan : public PathMan
{
    public:
        wxString FindPointInBoundary( double lat, double lon );
        bool FindPointInBoundary( Boundary *pBoundary, double lat, double lon );
        
    private:
        bool pointInPolygon(int polyCorners, float *polyX, float *polyY, float x, float y);
};

#endif // BOUNDARYMAN_H
