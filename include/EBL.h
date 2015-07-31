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

#ifndef EBL_H
#define EBL_H

#include "Path.h"

enum {
    ID_EBL_PERSISTENT = 0,
    ID_EBL_PERSISTENT_CRASH,
    ID_EBL_NOT_PERSISTENT,
    
    ID_EBL_PERSISTENT_LAST
};

class EBL : public Path
{
    public:
        EBL();
        virtual ~EBL();
        void MovePoint( double inc_lat, double inc_lon );
        void AddPoint(ODPoint* pNewPoint, bool b_rename_in_sequence = true, bool b_deferBoxCalc = false, bool b_isLoading = false);
        
        void SetPersistence( int PersistenceType );

        bool        m_bFixedEndPosition;
        int         m_PersistenceType;
        
    protected:
        
    private:
        wxColour   m_wxcLineColour;
        
};

WX_DECLARE_LIST(EBL, EBLList); // establish class Path as list member

#endif // EBL_H
