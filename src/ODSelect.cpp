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

#include "ODSelect.h"
#include "ocpn_draw_pi.h"
#include "georef.h"
//#include "vector2D.h"
#include "ODPath.h"

ODSelect::ODSelect()
{
    pSelectList = new ODSelectableItemList;

}

ODSelect::~ODSelect()
{
    pSelectList->m_pSelectItem.clear();
    delete pSelectList;

}

int ODSelect::GetSelectPixelRadius( void )
{
    int pixelRadius;
    if(IsTouchInterface_PlugIn())
        pixelRadius = 50;
    else {
        pixelRadius = 8;
        int w,h;
        wxDisplaySize( &w, &h );
        if( h > 800 ) pixelRadius = 10;
        if( h > 1024 ) {
          pixelRadius = 12;
          pixelRadius *= g_dOCPN_DisplayScaleFactor;
        }
    }
    return pixelRadius;
}

bool ODSelect::AddSelectableODPoint( float slat, float slon, ODPoint *pODPointAdd )
{
    ODSelectItem *pSelItem = new ODSelectItem;
    pSelItem->m_slat = slat;
    pSelItem->m_slon = slon;
    pSelItem->m_slat2 = slat;
    pSelItem->m_slon2 = slon;
    pSelItem->m_seltype = SELTYPE_ODPOINT;
    pSelItem->m_bIsSelected = false;
    pSelItem->m_pData1 = pODPointAdd;

    if( pODPointAdd->m_bIsInLayer )
        pSelectList->m_pSelectItem.push_back( pSelItem );
    else
        pSelectList->m_pSelectItem.push_back( pSelItem );

    pODPointAdd->SetSelectNode(pSelectList);

    return true;
}

bool ODSelect::AddSelectablePathSegment( float slat1, float slon1, float slat2, float slon2,
        ODPoint *pODPointAdd1, ODPoint *pODPointAdd2, ODPath *pPath, int iUserData )
{
    ODSelectItem *pSelItem = new ODSelectItem;
    pSelItem->m_slat = slat1;
    pSelItem->m_slon = slon1;
    pSelItem->m_slat2 = slat2;
    pSelItem->m_slon2 = slon2;
    if(iUserData)
        pSelItem->m_seltype = SELTYPE_PIL;
    else
        pSelItem->m_seltype = SELTYPE_PATHSEGMENT;
    pSelItem->m_bIsSelected = false;
    pSelItem->m_pData1 = pODPointAdd1;
    pSelItem->m_pData2 = pODPointAdd2;
    pSelItem->m_pData3 = pPath;
    pSelItem->SetUserData(iUserData);

    if( pPath->m_bIsInLayer ) pSelectList->m_pSelectItem.push_back( pSelItem );
    else
        pSelectList->m_pSelectItem.push_back( pSelItem );

    return true;
}

bool ODSelect::DeleteAllSelectablePathSegments( ODPath *pr )
{
    ODSelectItem *pFindSel;

//    Iterate on the select list
    for(std::list<ODSelectItem *>::iterator it = pSelectList->m_pSelectItem.begin(); it != pSelectList->m_pSelectItem.end(); ++it) {
        pFindSel = *it;
        if( pFindSel->m_seltype == SELTYPE_PATHSEGMENT || pFindSel->m_seltype == SELTYPE_PIL ) {

            if( (ODPath *) pFindSel->m_pData3 == pr ) {
                delete pFindSel;
                it = pSelectList->m_pSelectItem.erase( it );   //delete node;
            }
        }
    }

    return true;
}

bool ODSelect::DeleteSelectablePathSegment( ODPath *pr, int iUserData )
{
    ODSelectItem *pFindSel;

    //    Iterate on the select list
    for(std::list<ODSelectItem *>::iterator it = pSelectList->m_pSelectItem.begin(); it != pSelectList->m_pSelectItem.end(); ++it) {
        pFindSel = *it;
        if( pFindSel->m_seltype == SELTYPE_PIL ) {
            if( (ODPath *) pFindSel->m_pData3 == pr && pFindSel->GetUserData() == iUserData) {
                delete pFindSel;
                it = pSelectList->m_pSelectItem.erase( it );   //delete node;
            }
        }
    }

    return true;
}


bool ODSelect::DeleteAllSelectableODPoints( ODPath *pp )
{
    ODSelectItem *pFindSel;

//    Iterate on the select list
    for(std::list<ODSelectItem *>::iterator it = pSelectList->m_pSelectItem.begin(); it != pSelectList->m_pSelectItem.end(); ++it) {
        pFindSel = *it;
        if( pFindSel->m_seltype == SELTYPE_ODPOINT ) {
            ODPoint *ps = (ODPoint *) pFindSel->m_pData1;
            //    inner loop iterates on the path's point list
            for(std::list<ODPoint *>::iterator it1 = pp->m_ODPointList.m_pODPoint.begin(); it1 != pp->m_ODPointList.m_pODPoint.end(); ++it1) {
                ODPoint *prp = *it1;

                if( prp == ps ) {
                    delete pFindSel;
                    it = pSelectList->m_pSelectItem.erase( it );   //delete node;
                    prp->SetSelectNode( NULL );
                    break;
                }
            }
        }
    }
    return true;
}

bool ODSelect::AddAllSelectableODPoints( ODPath *pp )
{
    if( pp->m_ODPointList.m_pODPoint.size() ) {
        for(std::list<ODPoint *>::iterator it = pp->m_ODPointList.m_pODPoint.begin(); it != pp->m_ODPointList.m_pODPoint.end(); ++it) {
            ODPoint *podp = *it;
            AddSelectableODPoint( podp->m_lat, podp->m_lon, podp );
        }
        return true;
    } else
        return false;
}

bool ODSelect::AddAllSelectablePathSegments( ODPath *pp )
{
    wxPoint rpt, rptn;
    float slat1, slon1, slat2, slon2;

    if( pp->m_ODPointList.m_pODPoint.size() ) {
        std::list<ODPoint *>::iterator it = pp->m_ODPointList.m_pODPoint.begin();

        ODPoint *podp0 = *it;
        slat1 = podp0->m_lat;
        slon1 = podp0->m_lon;

        for(++it; it != pp->m_ODPointList.m_pODPoint.end(); ++it) {
            ODPoint *podp = *it;
            slat2 = podp->m_lat;
            slon2 = podp->m_lon;

            AddSelectablePathSegment( slat1, slon1, slat2, slon2, podp0, podp, pp );

            slat1 = slat2;
            slon1 = slon2;
            podp0 = podp;
        }
        return true;
    } else
        return false;
}

bool ODSelect::UpdateSelectablePathSegments( ODPoint *ppp )
{
    ODSelectItem *pFindSel;
    bool ret = false;

//    Iterate on the select list
    for(std::list<ODSelectItem *>::iterator it = pSelectList->m_pSelectItem.begin(); it != pSelectList->m_pSelectItem.end(); ++it) {
        pFindSel = *it;
        if( pFindSel->m_seltype == SELTYPE_PATHSEGMENT ) {
            if( pFindSel->m_pData1 == ppp ) {
                pFindSel->m_slat = ppp->m_lat;
                pFindSel->m_slon = ppp->m_lon;
                ret = true;
            }
            else
                if( pFindSel->m_pData2 == ppp ) {
                    pFindSel->m_slat2 = ppp->m_lat;
                    pFindSel->m_slon2 = ppp->m_lon;
                    ret = true;
                }
        }
    }

    return ret;
}

/*
bool ODSelect::DeleteAllPoints( void )
{
    pSelectList->m_pSelectItem.clear();
    return true;
}
*/

bool ODSelect::DeleteSelectablePoint( void *pdata, int SeltypeToDelete )
{
    ODSelectItem *pFindSel;
    bool found = false;

    if( NULL != pdata ) {
//    Iterate on the list
        for(std::list<ODSelectItem *>::iterator it = pSelectList->m_pSelectItem.begin(); it != pSelectList->m_pSelectItem.end(); ++it) {
            pFindSel = *it;
            if( pFindSel->m_seltype == SeltypeToDelete ) {
                if( pdata == pFindSel->m_pData1 ) {
                    delete pFindSel;
                    it = pSelectList->m_pSelectItem.erase( it );
                    g_pRolloverPoint = NULL;

                    if( SELTYPE_ODPOINT == SeltypeToDelete ){
                        ODPoint *podp = (ODPoint *)pdata;
                        podp->SetSelectNode( NULL );
                    }
                    found = true;
                }
            }
        }
    }
    return found;
}

bool ODSelect::DeleteAllSelectableTypePoints( int SeltypeToDelete )
{
    ODSelectItem *pFindSel;

//    Iterate on the list
    for(std::list<ODSelectItem *>::iterator it = pSelectList->m_pSelectItem.begin(); it != pSelectList->m_pSelectItem.end(); ++it) {
        pFindSel = *it;
        if( pFindSel->m_seltype == SeltypeToDelete ) {
            it = pSelectList->m_pSelectItem.erase(it);

            if( SELTYPE_ODPOINT == SeltypeToDelete ){
                ODPoint *podp = (ODPoint *)pFindSel->m_pData1;
                podp->SetSelectNode( NULL );
            }
            delete pFindSel;
        }
    }
    return true;
}

bool ODSelect::DeleteSelectableODPoint( ODPoint *podp )
{

    if( NULL != podp ) {
        //wxODSelectableItemListNode *node = (wxODSelectableItemListNode *)podp->GetSelectNode();
        ODSelectableItemList * node = (ODSelectableItemList *)podp->GetSelectNode();
        if(node){
            ODSelectItem *pFindSel = node->m_pSelectItem.front();
            if(pFindSel){
                //delete pFindSel;
                //delete node;            // automatically removes from list
                podp->SetSelectNode( NULL );
                return true;
            }
        }
        else
            return DeleteSelectablePoint( podp, SELTYPE_ODPOINT );

    }
    return false;
}

bool ODSelect::ModifySelectablePoint( float lat, float lon, void *data, int SeltypeToModify )
{
    ODSelectItem *pFindSel;

//    Iterate on the list
    for(std::list<ODSelectItem *>::iterator it = pSelectList->m_pSelectItem.begin(); it != pSelectList->m_pSelectItem.end(); ++it) {
        pFindSel = *it;
        if( pFindSel->m_seltype == SeltypeToModify ) {
            if( data == pFindSel->m_pData1 ) {
                pFindSel->m_slat = lat;
                pFindSel->m_slon = lon;
                return true;
            }
        }
    }
    return false;
}

bool ODSelect::IsSegmentSelected( float a, float b, float c, float d, float slat, float slon )
{
    double adder = 0.;

    if( ( c * d ) < 0. ) {
        //    Arrange for points to be increasing longitude, c to d
        double dist, brg;
        DistanceBearingMercator_Plugin( a, c, b, d, &brg, &dist );
        if( brg < 180. )             // swap points?
                {
            double tmp;
            tmp = c;
            c = d;
            d = tmp;
            tmp = a;
            a = b;
            b = tmp;
        }
        if( d < 0. )     // idl?
                {
            d += 360.;
            if( slon < 0. ) adder = 360.;
        }
    }

//    As a course test, use segment bounding box test
    if( ( slat >= ( fmin ( a,b ) - selectRadius ) ) && ( slat <= ( fmax ( a,b ) + selectRadius ) )
            && ( ( slon + adder ) >= ( fmin ( c,d ) - selectRadius ) )
            && ( ( slon + adder ) <= ( fmax ( c,d ) + selectRadius ) ) ) {
        //    Use vectors to do hit test....
        ODvector2D va, vb, vn;

        //    Assuming a Mercator projection
        double ap, cp;
        toSM( a, c, 0., 0., &cp, &ap );
        double bp, dp;
        toSM( b, d, 0., 0., &dp, &bp );
        double slatp, slonp;
        toSM( slat, slon + adder, 0., 0., &slonp, &slatp );

        va.x = slonp - cp;
        va.y = slatp - ap;
        vb.x = dp - cp;
        vb.y = bp - ap;

        double delta = vGetLengthOfNormal( &va, &vb, &vn );
        if( fabs( delta ) < ( selectRadius * 1852 * 60 ) ) return true;
    }
    return false;
}

void ODSelect::CalcSelectRadius()
{
    if( !g_VP.bValid )
        selectRadius = 0;
    else
        selectRadius = GetSelectPixelRadius() / ( g_VP.view_scale_ppm * 1852 * 60 );
}

ODSelectItem *ODSelect::FindSelection( float slat, float slon, int fseltype )
{
    float a, b, c, d;
    ODSelectItem *pFindSel;

    CalcSelectRadius();

//    Iterate on the list
    for(std::list<ODSelectItem *>::iterator it = pSelectList->m_pSelectItem.begin(); it != pSelectList->m_pSelectItem.end(); ++it) {
        pFindSel = *it;
        if( pFindSel->m_seltype == fseltype ) {
            switch( fseltype ){
                case SELTYPE_ODPOINT:
                    if( ( fabs( slat - pFindSel->m_slat ) < selectRadius )
                            && ( fabs( slon - pFindSel->m_slon ) < selectRadius ) ) return pFindSel;
                    break;
                case SELTYPE_PATHSEGMENT:
                    a = pFindSel->m_slat;
                    b = pFindSel->m_slat2;
                    c = pFindSel->m_slon;
                    d = pFindSel->m_slon2;

                    if( IsSegmentSelected( a, b, c, d, slat, slon ) ) return pFindSel;
                    break;
                case SELTYPE_PIL:
                    a = pFindSel->m_slat;
                    b = pFindSel->m_slat2;
                    c = pFindSel->m_slon;
                    d = pFindSel->m_slon2;

                    if( IsSegmentSelected( a, b, c, d, slat, slon) ) return pFindSel;
                default:
                    break;
            }
        }
    }

    return NULL;
}

bool ODSelect::IsSelectableSegmentSelected( float slat, float slon, ODSelectItem *pFindSel )
{
    CalcSelectRadius();

    if(pFindSel->m_seltype == SELTYPE_ODPOINT) {
        if( ( fabs( slat - pFindSel->m_slat ) < selectRadius )
            && ( fabs( slon - pFindSel->m_slon ) < selectRadius ) ) return true;
    }
    float a = pFindSel->m_slat;
    float b = pFindSel->m_slat2;
    float c = pFindSel->m_slon;
    float d = pFindSel->m_slon2;

    return IsSegmentSelected( a, b, c, d, slat, slon );
}

ODSelectableItemList ODSelect::FindSelectionList( float slat, float slon, int fseltype )
{
    float a, b, c, d;
    ODSelectItem *pFindSel;
    ODSelectableItemList ret_list;

    CalcSelectRadius();

    //    Iterate on the list
    if(pSelectList->m_pSelectItem.size() == 0) return ret_list;
    for(std::list<ODSelectItem *>::iterator it = pSelectList->m_pSelectItem.begin(); it != pSelectList->m_pSelectItem.end(); ++it) {
        pFindSel = *it;
        if( pFindSel->m_seltype & fseltype ) {
            if(fseltype & SELTYPE_ODPOINT) {
                if( ( fabs( slat - pFindSel->m_slat ) < selectRadius )
                    && ( fabs( slon - pFindSel->m_slon ) < selectRadius ) ) {
                    ret_list.m_pSelectItem.push_back( pFindSel );
                    }
            } else if(fseltype & SELTYPE_PATHSEGMENT) {
                a = pFindSel->m_slat;
                b = pFindSel->m_slat2;
                c = pFindSel->m_slon;
                d = pFindSel->m_slon2;

                if( IsSegmentSelected( a, b, c, d, slat, slon ) )
                    ret_list.m_pSelectItem.push_back( pFindSel );
            } else if(fseltype & SELTYPE_PIL) {
                ret_list.m_pSelectItem.push_back( pFindSel );
            }
        }
    }

    return ret_list;
}

double ODSelect::vGetLengthOfNormal( pODVector2D a, pODVector2D b, pODVector2D n )
{
    ODvector2D c, vNormal;
    vNormal.x = 0;
    vNormal.y = 0;
    //
    //Obtain projection vector.
    //
    //c = ((a * b)/(|b|^2))*b
    //
    c.x = b->x * ( vDotProduct( a, b ) / vDotProduct( b, b ) );
    c.y = b->y * ( vDotProduct( a, b ) / vDotProduct( b, b ) );
//
    //Obtain perpendicular projection : e = a - c
    //
    vSubtractVectors( a, &c, &vNormal );
    //
    //Fill PROJECTION structure with appropriate values.
    //
    *n = vNormal;

    return ( vVectorMagnitude( &vNormal ) );
}

double ODSelect::vDotProduct( pODVector2D v0, pODVector2D v1 )
{
    double dotprod;

    dotprod = ( v0 == NULL || v1 == NULL ) ? 0.0 : ( v0->x * v1->x ) + ( v0->y * v1->y );

    return ( dotprod );
}

pODVector2D ODSelect::vAddVectors( pODVector2D v0, pODVector2D v1, pODVector2D v )
{
    if( v0 == NULL || v1 == NULL ) v = (pODVector2D) NULL;
    else {
        v->x = v0->x + v1->x;
        v->y = v0->y + v1->y;
    }
    return ( v );
}

pODVector2D ODSelect::vSubtractVectors( pODVector2D v0, pODVector2D v1, pODVector2D v )
{
    if( v0 == NULL || v1 == NULL ) v = (pODVector2D) NULL;
    else {
        v->x = v0->x - v1->x;
        v->y = v0->y - v1->y;
    }
    return ( v );
}

double ODSelect::vVectorSquared( pODVector2D v0 )
{
    double dS;

    if( v0 == NULL ) dS = 0.0;
    else
        dS = ( ( v0->x * v0->x ) + ( v0->y * v0->y ) );
    return ( dS );
}

double ODSelect::vVectorMagnitude( pODVector2D v0 )
{
    double dMagnitude;

    if( v0 == NULL ) dMagnitude = 0.0;
    else
        dMagnitude = sqrt( vVectorSquared( v0 ) );
    return ( dMagnitude );
}


