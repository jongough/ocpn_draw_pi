/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Framework for Undo features
 * Author:   Jesper Weissglas
 *
 ***************************************************************************
 *   Copyright (C) 2012 by David S. Register                               *
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
 *
 *
 */

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/file.h>
#include <wx/datetime.h>
#include <wx/clipbrd.h>

#include "ODundo.h"
#include "ODConfig.h"
#include "ODSelect.h"
#include "ODPathPropertiesDialogImpl.h"
#include "PointMan.h"
#include "styles.h"
#include "PathMan.h"
#include "PathAndPointManagerDialogImpl.h"
#include "ODPointPropertiesImpl.h"
#include "tinyxml.h"

Undo::Undo()
{
    depthSetting = 10;
    stackpointer = 0;
    isInsideUndoableAction = false;
    candidate = NULL;
}

Undo::~Undo()
{
    for( unsigned int i=0; i<undoStack.size(); i++ ) {
        if( undoStack[i] ) {
            delete undoStack[i];
            undoStack[i] = NULL;
        }
    }
    undoStack.clear();
}

wxString UndoAction::Description()
{
    wxString descr;
    switch( type ){
        case Undo_CreateODPoint:
            descr = wxT("Create ODPoint");
            break;
        case Undo_DeleteODPoint:
            descr = wxT("Delete ODPoint");
            break;
        case Undo_MoveODPoint:
            descr = wxT("Move ODPoint");
            break;
        case Undo_AppendODPoint:
            descr = wxT("Append ODPoint");
            break;
    default:
            descr = _T("");
        break;
    }
    return descr;
}

void doUndoMoveODPoint( UndoAction* action ) {
    double lat, lon;
    ODPoint* currentPoint = (ODPoint*) action->after[0];
    wxRealPoint* lastPoint = (wxRealPoint*) action->before[0];
    lat = currentPoint->m_lat;
    lon = currentPoint->m_lon;
    currentPoint->m_lat = lastPoint->y;
    currentPoint->m_lon = lastPoint->x;
    lastPoint->y = lat;
    lastPoint->x = lon;
    SelectItem* selectable = (SelectItem*) action->selectable[0];
    selectable->m_slat = currentPoint->m_lat;
    selectable->m_slon = currentPoint->m_lon;

    if( ( NULL != pMarkPropDialog ) && ( pMarkPropDialog->IsShown() ) ){
       if( currentPoint == pMarkPropDialog->GetRoutePoint() ) pMarkPropDialog->UpdateProperties(true);
       }
        
    wxArrayPtrVoid* pathArray = g_pPathMan->GetPathArrayContaining( currentPoint );
    if( pathArray ) {
        for( unsigned int ir = 0; ir < pathArray->GetCount(); ir++ ) {
            ODPath *pp = (ODPath *) pathArray->Item( ir );
            pp->FinalizeForRendering();
            pp->UpdateSegmentDistances();
            g_pODConfig->UpdatePath( pp );
        }
        delete pathArray;
    }
}

void doUndoDeleteODPoint( UndoAction* action )
{
    ODPoint* point = (OCPNPPoint*) action->before[0];
    g_pODSelect->AddSelectableODPoint( point->m_lat, point->m_lon, point );
    g_pODConfig->AddNewODPoint( point, -1 );
    if( NULL != g_pODPointMan ) g_pODPointMan->AddODPoint( point );
    if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() ) g_pPathAndPointManagerDialog->UpdateODPointsListCtrl();
}

void doRedoDeleteODPoint( UndoAction* action )
{
    ODPoint* point = (ODPoint*) action->before[0];
    g_pODConfig->DeleteODPoint( point );
    g_pODSelect->DeleteSelectablePoint( point, SELTYPE_ODPOINT );
    if( NULL != g_pODPointMan ) g_pODPointMan->RemoveODPoint( point );
    if( g_pPathAManagerDialog && g_pPathAManagerDialog->IsShown() ) g_pPathAManagerDialog->UpdateODPointsListCtrl();
}

void doUndoAppenODPoint( UndoAction* action )
{
    ODPoint* point = (ODPoint*) action->before[0];
    ODPath* path = (ODPath*) action->after[0];

    bool noPathLeftToRedo = false;
    if( (path->GetnPoints() == 2) && (gFrame->nPath_State == 0) )
        noPathLeftToRedo = true;

    m_pPath->RemovePointFromPath( point, path );

    if( action->beforeType[0] == Undo_IsOrphanded ) {
        pConfig->DeleteWayPoint( point );
        pSelect->DeleteSelectablePoint( point, SELTYPE_ROUTEPOINT );
        if( NULL != pWayPointMan ) pWayPointMan->RemoveRoutePoint( point );
    }

    if( noRouteLeftToRedo ) {
        cc1->undo->InvalidateRedo();
    }

    if( pRouteManagerDialog && pRouteManagerDialog->IsShown() ) pRouteManagerDialog->UpdateWptListCtrl();

    if( gFrame->nRoute_State > 1 ) {
        gFrame->nRoute_State--;
        cc1->m_prev_pMousePoint = route->GetLastPoint();
        cc1->m_prev_rlat = cc1->m_prev_pMousePoint->m_lat;
        cc1->m_prev_rlon = cc1->m_prev_pMousePoint->m_lon;
        route->m_lastMousePointIndex = route->GetnPoints();
    }
}

void doRedoAppendWaypoint( UndoAction* action )
{
    ODPoint* point = (ODPoint*) action->before[0];
    ODPath* path = (ODPath*) action->after[0];

    if( action->beforeType[0] == Undo_IsOrphanded ) {
        pConfig->AddNewODPoint( point, -1 );
        pSelect->AddSelectableODPoint( point->m_lat, point->m_lon, point );
    }

    ODPoint* prevpoint = ODPath->GetLastPoint();

    path->AddPoint( point );
    pSelect->AddSelectablePathSegment( prevpoint->m_lat, prevpoint->m_lon,
            point->m_lat, point->m_lon, prevpoint, point, path );

    if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() ) g_pPathAndPointManagerDialog->UpdateODPointsListCtrl();

    if( gFrame->nPath_State > 1 ) {
        gFrame->nPath_State++;
        cc1->m_prev_pMousePoint = path->GetLastPoint();
        cc1->m_prev_rlat = cc1->m_prev_pMousePoint->m_lat;
        cc1->m_prev_rlon = cc1->m_prev_pMousePoint->m_lon;
        path->m_lastMousePointIndex = path->GetnPoints();
    }
}

bool Undo::AnythingToUndo()
{
    return undoStack.size() > stackpointer;
}

bool Undo::AnythingToRedo()
{
    return stackpointer > 0;
}

UndoAction* Undo::GetNextUndoableAction()
{
    return undoStack[stackpointer];
}

UndoAction* Undo::GetNextRedoableAction()
{
    return undoStack[stackpointer-1];
}

void Undo::InvalidateRedo()
{
    if( stackpointer == 0 ) return;

    // Make sure we are not deleting any objects pointed to by
    // potential redo actions.

    for( unsigned int i=0; i<stackpointer; i++ ) {
        switch( undoStack[i]->type ) {
            case Undo_DeleteWaypoint:
                undoStack[i]->before[0] = NULL;
                break;
            case Undo_CreateWaypoint:
            case Undo_MoveWaypoint:
            case Undo_AppendWaypoint:
                break;
        }
        delete undoStack[i];
    }

    undoStack.erase( undoStack.begin(), undoStack.begin() + stackpointer );
    stackpointer = 0;
}

void Undo::InvalidateUndo()
{
    undoStack.clear();
    stackpointer = 0;
}

bool Undo::UndoLastAction()
{
    if( !AnythingToUndo() ) return false;
    UndoAction* action = GetNextUndoableAction();

    switch( action->type ){

        case Undo_CreateWaypoint:
            doRedoDeleteWaypoint( action ); // Same as delete but reversed.
            stackpointer++;
            break;

        case Undo_MoveWaypoint:
            doUndoMoveWaypoint( action );
            stackpointer++;
            break;

        case Undo_DeleteWaypoint:
            doUndoDeleteWaypoint( action );
            stackpointer++;
            break;

        case Undo_AppendWaypoint:
            stackpointer++;
            doUndoAppendWaypoint( action );
            break;
    }
    return true;
}

bool Undo::RedoNextAction()
{
    if( !AnythingToRedo() ) return false;
    UndoAction* action = GetNextRedoableAction();

    switch( action->type ){

        case Undo_CreateWaypoint:
            doUndoDeleteWaypoint( action ); // Same as delete but reversed.
            stackpointer--;
            break;

        case Undo_MoveWaypoint:
            doUndoMoveWaypoint( action ); // For point move, redo is same as undo (swap lat/long);
            stackpointer--;
            break;

        case Undo_DeleteWaypoint:
            doRedoDeleteWaypoint( action );
            stackpointer--;
            break;

        case Undo_AppendWaypoint:
            doRedoAppendWaypoint( action );
            stackpointer--;
            break;
    }
    return true;
}

bool Undo::BeforeUndoableAction( UndoType type, UndoItemPointer before, UndoBeforePointerType beforeType,
        UndoItemPointer selectable )
{
    if( CancelUndoableAction() ) return false;;
    InvalidateRedo();

    candidate = new UndoAction;
    candidate->before.clear();
    candidate->beforeType.clear();
    candidate->selectable.clear();
    candidate->after.clear();

    candidate->type = type;
    UndoItemPointer subject = before;

    switch( beforeType ){
        case Undo_NeedsCopy: {
            switch( candidate->type ) {
                case Undo_MoveWaypoint: {
                    wxRealPoint* point = new wxRealPoint;
                    RoutePoint* rp = (RoutePoint*) before;
                    point->x = rp->m_lon;
                    point->y = rp->m_lat;
                    subject = point;
                    break;
                }
                case Undo_CreateWaypoint: break;
                case Undo_DeleteWaypoint: break;
                case Undo_AppendWaypoint: break;
            }
            break;
        }
        case Undo_IsOrphanded: break;
        case Undo_HasParent: break;
    }

    candidate->before.push_back( subject );
    candidate->beforeType.push_back( beforeType );
    candidate->selectable.push_back( selectable );

    isInsideUndoableAction = true;
    return true;
}

bool Undo::AfterUndoableAction( UndoItemPointer after )
{
    if( !isInsideUndoableAction ) return false;

    candidate->after.push_back( after );
    undoStack.push_front( candidate );

    if( undoStack.size() > depthSetting ) {
        undoStack.pop_back();
    }

    isInsideUndoableAction = false;
    return true;
}

bool Undo::CancelUndoableAction( bool noDataDelete )
{
    if( isInsideUndoableAction ) {
        if( noDataDelete ) {
            for( unsigned int i = 0; i < candidate->beforeType.size(); i++ ) {
                if( candidate->beforeType[i] == Undo_IsOrphanded ) {
                    candidate->beforeType[i] = Undo_HasParent;
                }
            }
        }
        if( candidate ) delete candidate;
        candidate = NULL;
        isInsideUndoableAction = false;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------

UndoAction::~UndoAction()
{
    assert( before.size() == beforeType.size() );

    for( unsigned int i = 0; i < before.size(); i++ ) {
        switch( beforeType[i] ){
            case Undo_NeedsCopy: {
                switch( type ){
                    case Undo_MoveWaypoint:
                        if( before[i] ) {
                            delete (wxRealPoint*) before[i];
                            before[i] = NULL;
                        }
                        break;
                    case Undo_DeleteWaypoint: break;
                    case Undo_CreateWaypoint: break;
                    case Undo_AppendWaypoint: break;
                }
                break;
            }
            case Undo_IsOrphanded: {
                switch( type ){
                    case Undo_DeleteWaypoint:
                        if( before[i] ) {
                            delete (RoutePoint*) before[i];
                        }
                        break;
                    case Undo_CreateWaypoint: break;
                    case Undo_MoveWaypoint: break;
                    case Undo_AppendWaypoint:
                        if( before[i] ) {
                            delete (RoutePoint*) before[i];
                            before[i] = NULL;
                        }
                        break;
                }
                break;
            }
            case Undo_HasParent: break;
        }
    }
    before.clear();
}
