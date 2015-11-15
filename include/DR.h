/***************************************************************************
 * Project:  OpenCPN
 * Purpose:  DR
 * Author:   Jon Gough
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

#include "ODPath.h"

enum {
    ID_IT_TIME = 0,
    ID_IT_DISTANCE,
    
    ID_IT_LAST
};

enum {
    ID_LT_TIME = 0,
    ID_LT_DISTANCE,
    
    ID_LT_LAST
};

enum {
    ID_DU_KILOMETERS = 0,
    ID_DU_NAUTICAL_MILES,
    
    ID_DU_LAST
};

enum {
    ID_TU_MINUTES = 0,
    ID_TU_HOURS,
    ID_TU_DAYS,
    
    ID_TU_LAST
};

class DR : public ODPath
{
    public:
        DR();
        virtual ~DR();
        void RemovePoint( ODPoint *op, bool bRenamePoints = false );
        void AddPoint(ODPoint* pNewPoint, bool b_rename_in_sequence = true, bool b_deferBoxCalc = false, bool b_isLoading = false);
        ODPoint *InsertPointBefore(ODPoint *pOP, double lat, double lon, bool bRenamePoints = false);
        ODPoint *InsertPointAfter( ODPoint *pOP, double lat, double lon, bool bRenamePoints = false);
        void SetPersistence( int PersistenceType );
        
        int         m_PersistenceType;
        float       m_fSoG;
        int         m_iCoG;
        float       m_fLength;
        float       m_fTotalLengthNM;
        float       m_fWaypointInterval;
        float       m_fWaypointIntervalNM;
        int         m_iLengthType;
        int         m_iIntervalType;
        int         m_iDistanceUnits;
        int         m_iTimeUnits;
        float       m_fStartLat;
        float       m_fStartLon;
        float       m_fEndLat;
        float       m_fEndLon;
        
    protected:
        
    private:
        wxColour    m_wxcLineColour;
};

WX_DECLARE_LIST(DR, DRList); // establish class EBL list member

#endif // EBL_H
