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

#ifndef __ODSELECT_H__
#define __ODSELECT_H__

#include "SelectItem.h"
#include "ODPath.h"
#include "ODvector2D.h"

#define SELTYPE_UNKNOWN             0x0001
#define SELTYPE_ODPOINT             0x0002
#define SELTYPE_PATHSEGMENT         0x0004
#define SELTYPE_PATHCREATE          0x0008
#define SELTYPE_BOUNDARYLIST        0x0010
#define SELTYPE_PIL                 0x0020

class ODSelect
{
public:
    ODSelect();
    ~ODSelect();

    bool AddSelectableODPoint( float slat, float slon, ODPoint *pODPointAdd );
    bool AddSelectablePathSegment( float slat1, float slon1, float slat2, float slon2,
            ODPoint *pODPointAdd1, ODPoint *pODPointAdd2, ODPath *pPath, int UserData = 0 );

    SelectItem *FindSelection( float slat, float slon, int fseltype );
    SelectableItemList FindSelectionList( float slat, float slon, int fseltype );

    bool DeleteAllSelectablePathSegments( ODPath * );
    bool DeleteSelectablePathSegment( ODPath *pr, int iUserData );
    bool DeleteAllSelectableODPoints( ODPath * );
    bool AddAllSelectablePathSegments( ODPath *pr );
    bool AddAllSelectableODPoints( ODPath *pr );
    bool UpdateSelectablePathSegments( ODPoint *prp );
    bool IsSegmentSelected( float a, float b, float c, float d, float slat, float slon );
    bool IsSelectableSegmentSelected( float slat, float slon, SelectItem *pFindSel );

    //    Generic Point Support
    //      e.g. Tides/Currents and AIS Targets
    bool DeleteAllPoints( void );
    bool DeleteSelectablePoint( void *data, int SeltypeToDelete );
    bool ModifySelectablePoint( float slat, float slon, void *data, int fseltype );

    //    Delete all selectable points in list by type
    bool DeleteAllSelectableTypePoints( int SeltypeToDelete );

    bool DeleteSelectableODPoint( ODPoint *prp );
    
    int GetSelectPixelRadius( void );
    
    //  Accessors

    SelectableItemList *GetSelectList()
    {
        return pSelectList;
    }

private:
    void CalcSelectRadius();
    double vGetLengthOfNormal( pODVector2D a, pODVector2D b, pODVector2D n );
    double vDotProduct( pODVector2D v0, pODVector2D v1 );
    pODVector2D vAddVectors( pODVector2D v0, pODVector2D v1, pODVector2D v );
    pODVector2D vSubtractVectors( pODVector2D v0, pODVector2D v1, pODVector2D v );
    double vVectorSquared( pODVector2D v0 );
    double vVectorMagnitude( pODVector2D v0 );

    SelectableItemList *pSelectList;
    float selectRadius;
};

#endif
