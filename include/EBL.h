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

#include "ODPath.h"

enum {
    ID_EBL_PERSISTENT = 0,
    ID_EBL_PERSISTENT_CRASH,
    ID_EBL_NOT_PERSISTENT,
    
    ID_EBL_PERSISTENT_LAST
};

enum {
    ID_EBL_MAINTAIN_WITH_HEADING = 0,
    ID_EBL_MAINTAIN_WITH_COG
};

class EBL : public ODPath
{
    public:
        EBL();
        virtual ~EBL();
        void Draw( ODDC& dc, PlugIn_ViewPort &VP );
        void DrawGL( PlugIn_ViewPort &piVP );
        void RemovePoint( ODPoint *op, bool bRenamePoints = false );
        void ResizeVRM( double lat, double lon );
        void ResizeVRM( void );
        void MoveEndPoint( bool bUpdateEBL );
        void MoveEndPoint( void );
        void MoveStartPoint( double lat, double lon );
        void AddPoint(ODPoint* pNewPoint, bool b_rename_in_sequence = true, bool b_deferBoxCalc = false, bool b_isLoading = false);
        ODPoint *InsertPointBefore(ODPoint *pOP, double lat, double lon, bool bRenamePoints = false);
        ODPoint *InsertPointAfter( ODPoint *pOP, double lat, double lon, bool bRenamePoints = false);
        void InsertPointAfter( ODPoint *pOP, ODPoint *pnOP, bool bRenamePoints = false);
        
        void SetPersistence( int PersistenceType );
        void CentreOnBoat( bool bMoveEndPoint );
        void CentreOnLatLon( double lat, double lon );

        bool        m_bFixedEndPosition;
        int         m_iPersistenceType;
        bool        m_bVRM;
        bool        m_bCentreOnBoat;
        bool        m_bRotateWithBoat;
        int         m_iMaintainWith;
        double      m_dBoatHeading;
        double      m_dBoatCOG;
        double      m_dEBLAngle;
        double      m_dLength;
        bool        m_bEndPointMoving;
        
    protected:
        void        UpdateEBL( void );
        
    private:
        void MaintainWith( void );
        wxColour    m_wxcLineColour;
};

WX_DECLARE_LIST(EBL, EBLList); // establish class EBL list member

#endif // EBL_H
