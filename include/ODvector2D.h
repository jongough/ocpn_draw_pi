/******************************************************************************
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
 ***************************************************************************
 */

#ifndef __ODVECTOR2D_H__
#define __ODVECTOR2D_H__

#include "ODPoint.h"

class ODvector2D
{
public:
    ODvector2D() { x = 0.0; y = 0.0; }
    ODvector2D( double a, double b ) { x = a; y = b; }
    void Set( ODPoint* p ) { lat = p->m_lat; lon = p->m_lon; }
    friend bool operator==( ODvector2D &a, ODvector2D &b ) { return a.x == b.x && a.y == b.y; }
    friend bool operator!=( ODvector2D &a, ODvector2D &b ) { return a.x != b.x || a.y != b.y; }
    friend ODvector2D operator-( ODvector2D a, ODvector2D b ) { return ODvector2D( a.x - b.x, a.y - b.y ); }
    friend ODvector2D operator+( ODvector2D a, ODvector2D b ) { return ODvector2D( a.x + b.x, a.y + b.y ); }
    friend ODvector2D operator*( double t, ODvector2D a ) { return ODvector2D( a.x * t, a.y * t ); }

    union{ double x; double lon; };
    union{ double y; double lat; };
};

typedef ODvector2D* pODVector2D;

#endif
