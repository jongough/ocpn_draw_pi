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

#ifndef DR_H
#define DR_H

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
    ID_DU_KILOMETRES = 0,
    ID_DU_NAUTICAL_MILES,
    
    ID_DU_LAST
};

enum {
    ID_TU_MINUTES = 0,
    ID_TU_HOURS,
    ID_TU_DAYS,
    
    ID_TU_LAST
};

enum {
    ID_DR_PERSISTENT = 0,
    ID_DR_PERSISTENT_CRASH,
    ID_DR_NOT_PERSISTENT,
    
    ID_DR_PERSISTENT_LAST
};

class DR : public ODPath
{
    public:
        DR();
        virtual ~DR();
        ODPoint *InsertPointBefore(ODPoint *pOP, double lat, double lon, bool bRenamePoints = false);
        ODPoint *InsertPointAfter( ODPoint *pOP, double lat, double lon, bool bRenamePoints = false);
        void SetPersistence( int PersistenceType );
        
        int         m_iPersistenceType;
        double      m_dSoG;
        int         m_iCoG;
        double      m_dDRPathLength;
        double      m_dTotalLengthNM;
        double      m_dDRPointInterval;
        double      m_dDRPointIntervalNM;
        int         m_iLengthType;
        int         m_iIntervalType;
        int         m_iDistanceUnits;
        int         m_iTimeUnits;
        double      m_dMagCOG;
        
    protected:
        
    private:
        wxColour    m_wxcLineColour;
};

WX_DECLARE_LIST(DR, DRList); // establish class DR list member

#endif // DR_H
