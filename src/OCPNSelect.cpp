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

#include "OCPNSelect.h"
#include "georef.h"
#include "vector2D.h"
#include "navutil.h"
//#include "chcanv.h"
#include "Path.h"

extern ChartCanvas *ocpncc1;

OCPNSelect::OCPNSelect()
{
    pSelectList = new SelectableItemList;
    pixelRadius = 8;
    int w,h;
    wxDisplaySize( &w, &h );
    if( h > 800 ) pixelRadius = 10;
    if( h > 1024 ) pixelRadius = 12;
    
}

OCPNSelect::~OCPNSelect()
{
    pSelectList->DeleteContents( true );
    pSelectList->Clear();
    delete pSelectList;

}

bool OCPNSelect::AddSelectableOCPNPoint( float slat, float slon, OCPNPoint *pOCPNPointAdd )
{
    SelectItem *pSelItem = new SelectItem;
    pSelItem->m_slat = slat;
    pSelItem->m_slon = slon;
    pSelItem->m_seltype = SELTYPE_OCPNPOINT;
    pSelItem->m_bIsSelected = false;
    pSelItem->m_pData1 = pOCPNPointAdd;

    wxSelectableItemListNode *node;
    
    if( pOCPNPointAdd->m_bIsInLayer )
        node = pSelectList->Append( pSelItem );
    else
        node = pSelectList->Insert( pSelItem );

    pOCPNPointAdd->SetSelectNode(node);
    
    return true;
}

bool OCPNSelect::AddSelectablePathSegment( float slat1, float slon1, float slat2, float slon2,
        OCPNPoint *pOCPNPointAdd1, OCPNPoint *pOCPNPointAdd2, Path *pPath )
{
    SelectItem *pSelItem = new SelectItem;
    pSelItem->m_slat = slat1;
    pSelItem->m_slon = slon1;
    pSelItem->m_slat2 = slat2;
    pSelItem->m_slon2 = slon2;
    pSelItem->m_seltype = SELTYPE_PATHSEGMENT;
    pSelItem->m_bIsSelected = false;
    pSelItem->m_pData1 = pOCPNPointAdd1;
    pSelItem->m_pData2 = pOCPNPointAdd2;
    pSelItem->m_pData3 = pPath;

    if( pPath->m_bIsInLayer ) pSelectList->Append( pSelItem );
    else
        pSelectList->Insert( pSelItem );

    return true;
}

bool OCPNSelect::DeleteAllSelectablePathSegments( Path *pr )
{
    SelectItem *pFindSel;

//    Iterate on the select list
    wxSelectableItemListNode *node = pSelectList->GetFirst();

    while( node ) {
        pFindSel = node->GetData();
        if( pFindSel->m_seltype == SELTYPE_PATHSEGMENT ) {

//                  RoutePoint *ps1 = (RoutePoint *)pFindSel->m_pData1;
//                  RoutePoint *ps2 = (RoutePoint *)pFindSel->m_pData2;

            if( (Path *) pFindSel->m_pData3 == pr ) {
                delete pFindSel;
                pSelectList->DeleteNode( node );   //delete node;

                node = pSelectList->GetFirst();     // reset the top node

                goto got_next_outer_node;
            }
        }

        node = node->GetNext();
        got_next_outer_node: continue;
    }

    return true;
}

bool OCPNSelect::DeleteAllSelectableOCPNPoints( Path *pr )
{
    SelectItem *pFindSel;

//    Iterate on the select list
    wxSelectableItemListNode *node = pSelectList->GetFirst();

    while( node ) {
        pFindSel = node->GetData();
        if( pFindSel->m_seltype == SELTYPE_OCPNPOINT ) {
            OCPNPoint *ps = (OCPNPoint *) pFindSel->m_pData1;

            //    inner loop iterates on the route's point list
            wxOCPNPointListNode *pnode = ( pr->pOCPNPointList )->GetFirst();
            while( pnode ) {
                OCPNPoint *prp = pnode->GetData();

                if( prp == ps ) {
                    delete pFindSel;
                    pSelectList->DeleteNode( node );   //delete node;
                    prp->SetSelectNode( NULL );
                    
                    node = pSelectList->GetFirst();

                    goto got_next_outer_node;
                }
                pnode = pnode->GetNext();
            }
        }

        node = node->GetNext();
got_next_outer_node: continue;
    }
    return true;
}

bool OCPNSelect::AddAllSelectableOCPNPoints( Path *pr )
{
    if( pr->pOCPNPointList->GetCount() ) {
        wxOCPNPointListNode *node = ( pr->pOCPNPointList )->GetFirst();

        while( node ) {
            OCPNPoint *prp = node->GetData();
            AddSelectableOCPNPoint( prp->m_lat, prp->m_lon, prp );
            node = node->GetNext();
        }
        return true;
    } else
        return false;
}

bool OCPNSelect::AddAllSelectablePathSegments( Path *pr )
{
    wxPoint rpt, rptn;
    float slat1, slon1, slat2, slon2;

    if( pr->pOCPNPointList->GetCount() ) {
        wxOCPNPointListNode *node = ( pr->pOCPNPointList )->GetFirst();

        OCPNPoint *prp0 = node->GetData();
        slat1 = prp0->m_lat;
        slon1 = prp0->m_lon;

        node = node->GetNext();

        while( node ) {
            OCPNPoint *prp = node->GetData();
            slat2 = prp->m_lat;
            slon2 = prp->m_lon;

            AddSelectablePathSegment( slat1, slon1, slat2, slon2, prp0, prp, pr );

            slat1 = slat2;
            slon1 = slon2;
            prp0 = prp;

            node = node->GetNext();
        }
        return true;
    } else
        return false;
}

bool OCPNSelect::UpdateSelectablePathSegments( OCPNPoint *prp )
{
    SelectItem *pFindSel;
    bool ret = false;

//    Iterate on the select list
    wxSelectableItemListNode *node = pSelectList->GetFirst();

    while( node ) {
        pFindSel = node->GetData();
        if( pFindSel->m_seltype == SELTYPE_PATHSEGMENT ) {
            if( pFindSel->m_pData1 == prp ) {
                pFindSel->m_slat = prp->m_lat;
                pFindSel->m_slon = prp->m_lon;
                ret = true;
                ;
            }

            else
                if( pFindSel->m_pData2 == prp ) {
                    pFindSel->m_slat2 = prp->m_lat;
                    pFindSel->m_slon2 = prp->m_lon;
                    ret = true;
                }
        }
        node = node->GetNext();
    }

    return ret;
}

SelectItem *OCPNSelect::AddSelectablePoint( float slat, float slon, const void *pdata, int fseltype )
{
    SelectItem *pSelItem = new SelectItem;
    if( pSelItem ) {
        pSelItem->m_slat = slat;
        pSelItem->m_slon = slon;
        pSelItem->m_seltype = fseltype;
        pSelItem->m_bIsSelected = false;
        pSelItem->m_pData1 = pdata;

        pSelectList->Append( pSelItem );
    }

    return pSelItem;
}

/*
bool OCPNSelect::DeleteAllPoints( void )
{
    pSelectList->DeleteContents( true );
    pSelectList->Clear();
    return true;
}
*/

bool OCPNSelect::DeleteSelectablePoint( void *pdata, int SeltypeToDelete )
{
    SelectItem *pFindSel;

    if( NULL != pdata ) {
//    Iterate on the list
        wxSelectableItemListNode *node = pSelectList->GetFirst();

        while( node ) {
            pFindSel = node->GetData();
            if( pFindSel->m_seltype == SeltypeToDelete ) {
                if( pdata == pFindSel->m_pData1 ) {
                    delete pFindSel;
                    delete node;
                    
                    if( SELTYPE_OCPNPOINT == SeltypeToDelete ){
                        RoutePoint *prp = (RoutePoint *)pdata;
                        prp->SetSelectNode( NULL );
                    }
                    
                    return true;
                }
            }
            node = node->GetNext();
        }
    }
    return false;
}

bool OCPNSelect::DeleteAllSelectableTypePoints( int SeltypeToDelete )
{
    SelectItem *pFindSel;

//    Iterate on the list
    wxSelectableItemListNode *node = pSelectList->GetFirst();

    while( node ) {
        pFindSel = node->GetData();
        if( pFindSel->m_seltype == SeltypeToDelete ) {
            delete node;
            
            if( SELTYPE_OCPNPOINT == SeltypeToDelete ){
                OCPNPoint *prp = (OCPNPoint *)pFindSel->m_pData1;
                prp->SetSelectNode( NULL );
            }
            delete pFindSel;
            
            node = pSelectList->GetFirst();
            goto got_next_node;
        }

        node = node->GetNext();
        got_next_node: continue;
    }
    return true;
}

bool OCPNSelect::DeleteSelectableOCPNPoint( OCPNPoint *prp )
{
    
    if( NULL != prp ) {
        wxSelectableItemListNode *node = (wxSelectableItemListNode *)prp->GetSelectNode();
        if(node){
            SelectItem *pFindSel = node->GetData();
            if(pFindSel){
                delete pFindSel;
                delete node;            // automatically removes from list
                prp->SetSelectNode( NULL );
                return true;
            }
        }
        else
            return DeleteSelectablePoint( prp, SELTYPE_OCPNPOINT );
        
    }
    return false;
}

bool OCPNSelect::ModifySelectablePoint( float lat, float lon, void *data, int SeltypeToModify )
{
    SelectItem *pFindSel;

//    Iterate on the list
    wxSelectableItemListNode *node = pSelectList->GetFirst();

    while( node ) {
        pFindSel = node->GetData();
        if( pFindSel->m_seltype == SeltypeToModify ) {
            if( data == pFindSel->m_pData1 ) {
                pFindSel->m_slat = lat;
                pFindSel->m_slon = lon;
                return true;
            }
        }

        node = node->GetNext();
    }
    return false;
}

bool OCPNSelect::IsSegmentSelected( float a, float b, float c, float d, float slat, float slon )
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
        vector2D va, vb, vn;

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

void OCPNSelect::CalcSelectRadius()
{
    selectRadius = pixelRadius / ( ocpncc1->GetCanvasTrueScale() * 1852 * 60 );
}

SelectItem *OCPNSelect::FindSelection( float slat, float slon, int fseltype )
{
    float a, b, c, d;
    SelectItem *pFindSel;

    CalcSelectRadius();

//    Iterate on the list
    wxSelectableItemListNode *node = pSelectList->GetFirst();

    while( node ) {
        pFindSel = node->GetData();
        if( pFindSel->m_seltype == fseltype ) {
            switch( fseltype ){
                case SELTYPE_OCPNPOINT:
                    a = fabs( slat - pFindSel->m_slat );
                    b = fabs( slon - pFindSel->m_slon );

                    if( ( fabs( slat - pFindSel->m_slat ) < selectRadius )
                            && ( fabs( slon - pFindSel->m_slon ) < selectRadius ) ) goto find_ok;
                    break;
                case SELTYPE_PATHSEGMENT: 
                    a = pFindSel->m_slat;
                    b = pFindSel->m_slat2;
                    c = pFindSel->m_slon;
                    d = pFindSel->m_slon2;

//                    if( IsSegmentSelected( a, b, c, d, slat, slon ) ) goto find_ok;
                    if( IsSegmentSelected( a, b, c, d, slat, slon ) ) return pFindSel;
                    break;
                default:
                    break;
            }
        }

        node = node->GetNext();
    }

    return NULL;
    find_ok: return pFindSel;
}

bool OCPNSelect::IsSelectableSegmentSelected( float slat, float slon, SelectItem *pFindSel )
{
    CalcSelectRadius();

    float a = pFindSel->m_slat;
    float b = pFindSel->m_slat2;
    float c = pFindSel->m_slon;
    float d = pFindSel->m_slon2;

    return IsSegmentSelected( a, b, c, d, slat, slon );
}

SelectableItemList OCPNSelect::FindSelectionList( float slat, float slon, int fseltype )
{
    float a, b, c, d;
    SelectItem *pFindSel;
    SelectableItemList ret_list;

    CalcSelectRadius();

//    Iterate on the list
    wxSelectableItemListNode *node = pSelectList->GetFirst();

    while( node ) {
        pFindSel = node->GetData();
        if( pFindSel->m_seltype == fseltype ) {
            switch( fseltype ){
                case SELTYPE_OCPNPOINT:
                    if( ( fabs( slat - pFindSel->m_slat ) < selectRadius )
                            && ( fabs( slon - pFindSel->m_slon ) < selectRadius ) ) {
                        ret_list.Append( pFindSel );
                    }
                    break;
                case SELTYPE_PATHSEGMENT:
                    a = pFindSel->m_slat;
                    b = pFindSel->m_slat2;
                    c = pFindSel->m_slon;
                    d = pFindSel->m_slon2;

                    if( IsSegmentSelected( a, b, c, d, slat, slon ) ) ret_list.Append( pFindSel );

                    break;
                default:
                    break;
            }
        }

        node = node->GetNext();
    }

    return ret_list;
}

