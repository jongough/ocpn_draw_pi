/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Path 
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

#include "wx/wxprec.h"

#include "Path.h"
#include "georef.h"
//#include "routeman.h"
#include "ocpndc.h"
#include "cutil.h"
//#include "navutil.h"
#include "multiplexer.h"
#include "OCPNSelect.h"
#include "PointMan.h"
#include "PathMan.h"
#include "PathProp.h"
#include "OCPNDrawConfig.h"
#include "ocpn_draw_pi.h"
#include "ODUtils.h"
#include "bbox.h"
#include "ocpndc.h"


extern PointMan *pOCPNPointMan;
extern bool g_bODIsNewLayer;
extern int g_ODLayerIdx;
extern PathMan *g_pPathMan;
extern int g_path_line_width;
extern OCPNSelect *pOCPNSelect;
extern OCPNDrawConfig *pOCPNDrawConfig;
extern Multiplexer *g_pODMUX;
extern float g_ODGLMinSymbolLineWidth;
extern wxString    g_ActivePathLineColour;
extern wxString    g_InActivePathLineColour;
extern wxString    g_ActivePathFillColour;
extern wxString    g_InActivePathFillColour;
extern PathProp    *pPathPropDialog;
extern ocpn_draw_pi *g_ocpn_draw_pi;
extern wxString     g_sOCPNPointIconName;

#include <wx/listimpl.cpp>
WX_DEFINE_LIST ( PathList );

Path::Path( void )
{
    m_sTypeString = wxS("Path");
    m_bPathIsSelected = false;
    m_bPathIsActive = true;
    m_pPathActivePoint = NULL;
    m_bIsBeingEdited = false;
    m_bIsBeingCreated = false;
    m_nPoints = 0;
    m_nm_sequence = 1;
    m_path_length = 0.0;
    m_bVisible = true;
    m_bListed = true;
    m_bDeleteOnArrival = false;
    m_width = STYLE_UNDEFINED;
    m_style = STYLE_UNDEFINED;
    m_hiliteWidth = 0;

    pOCPNPointList = new OCPNPointList;
    m_pLastAddedPoint = NULL;
    m_pFirstAddedPoint = NULL;
    m_GUID = GetUUID();
    m_btemp = false;
    
    m_bNeedsUpdateBBox = true;
    RBBox.Reset();
    m_bcrosses_idl = false;

    m_LayerID = 0;
    m_bIsInLayer = false;

    m_ActiveLineColour = g_ActivePathLineColour;
    m_ActiveFillColour = g_ActivePathFillColour;
    m_InActiveLineColour = g_InActivePathLineColour;
    m_InActiveFillColour = g_InActivePathFillColour;

    m_lastMousePointIndex = 0;
    m_NextLegGreatCircle = false;
    
    m_HyperlinkList = new HyperlinkList;
}

Path::~Path( void )
{
    pOCPNPointList->DeleteContents( false );            // do not delete Marks
    pOCPNPointList->Clear();
    delete pOCPNPointList;
    m_HyperlinkList->Clear();
    delete m_HyperlinkList;
}

void Path::AddPoint( OCPNPoint *pNewPoint, bool b_rename_in_sequence, bool b_deferBoxCalc, bool b_isLoading )
{
    if( pNewPoint->m_bIsolatedMark ) {
        pNewPoint->m_bKeepXPath = true;
    }
    pNewPoint->m_bIsolatedMark = false;       // definitely no longer isolated
    pNewPoint->m_bIsInPath = true;

    pOCPNPointList->Append( pNewPoint );

    m_nPoints++;

    if( !b_deferBoxCalc )
        FinalizeForRendering();

    if ( m_pFirstAddedPoint == NULL )
        m_pFirstAddedPoint = pNewPoint;
    
    if (!b_isLoading)
        UpdateSegmentDistances();
    m_pLastAddedPoint = pNewPoint;

    if( b_rename_in_sequence && pNewPoint->GetName().IsEmpty() && !pNewPoint->m_bKeepXPath ) {
        wxString name;
        name.Printf( _T ( "%03d" ), m_nPoints );
        pNewPoint->SetName( name );
        pNewPoint->m_bDynamicName = true;
    }
    return;
}

void Path::AddTentativePoint( const wxString& GUID )
{
    OCPNPointGUIDList.Add( GUID );
    return;
}

OCPNPoint *Path::GetPoint( int nWhichPoint )
{
    OCPNPoint *prp;
    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();

    int i = 1;
    while( node ) {
        prp = node->GetData();
        if( i == nWhichPoint ) return prp;

        i++;
        node = node->GetNext();
    }

    return ( NULL );
}

OCPNPoint *Path::GetPoint( const wxString &guid )
{
    OCPNPoint *prp;
    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();

    while( node ) {
        prp = node->GetData();
        if( guid == prp->m_GUID ) return prp;

        node = node->GetNext();
    }

    return ( NULL );
}

void Path::DrawPointWhich( ocpnDC& dc, int iPoint, wxPoint *rpn )
{
    if( iPoint <= GetnPoints() )
        GetPoint( iPoint )->Draw( dc, rpn );
}

void Path::DrawSegment( ocpnDC& dc, wxPoint *rp1, wxPoint *rp2, PlugIn_ViewPort &VP, bool bdraw_arrow )
{
/*    if( m_bPathIsSelected ) dc.SetPen( *g_pPathMan->GetSelectedPathPen() );
    else
        if( m_bPathIsActive ) dc.SetPen( *g_pPathMan->GetActivePathPen() );
        else
            dc.SetPen( *g_pPathMan->GetPathPen() );
*/    
    wxColour col;
    wxString colour;

    if( m_bVisible && m_bPathIsActive ) {
        colour = m_ActiveLineColour;
    }
    else if ( m_bVisible && !m_bPathIsActive) {
        colour = m_InActiveLineColour;
    }

    if( colour.IsNull() ) {
        colour = m_ActiveLineColour;
    } 

    for( unsigned int i = 0; i < sizeof( ::GpxxColorNames ) / sizeof(wxString); i++ ) {
        if( colour == ::GpxxColorNames[i] ) {
            col = ::GpxxColors[i];
            break;
        }
    }
    dc.SetPen( *wxThePenList->FindOrCreatePen( col, g_path_line_width, m_style ) );

    RenderSegment( dc, rp1->x, rp1->y, rp2->x, rp2->y, VP, bdraw_arrow );
}

void Path::Draw( ocpnDC& dc, PlugIn_ViewPort &VP )
{
    wxString colour, fillcolour;
    int style = wxSOLID;
    int width = g_path_line_width;


    if( m_nPoints == 0 ) return;

    if( m_bVisible && m_bPathIsActive ) {
        colour = m_ActiveLineColour;
        fillcolour = m_ActiveFillColour;
    }
    else {
        colour = m_InActiveLineColour;
        fillcolour = m_InActiveFillColour;
        
    }

    if( colour.IsNull() ) {
        colour = m_ActiveLineColour;
    }
    for( unsigned int i = 0; i < sizeof( ::GpxxColorNames ) / sizeof(wxString); i++ ) {
        if( colour == ::GpxxColorNames[i] ) {
            m_col = ::GpxxColors[i];
            break;
        }
    }

    if( fillcolour.IsNull() ) {
        fillcolour = m_ActiveFillColour;
    }
    for( unsigned int i = 0; i < sizeof( ::GpxxColorNames ) / sizeof(wxString); i++ ) {
        if( fillcolour == ::GpxxColorNames[i] ) {
            m_fillcol = ::GpxxColors[i];
            break;
        }
    }

    if( m_style != STYLE_UNDEFINED ) style = m_style;
    if( m_width != STYLE_UNDEFINED ) width = m_width;

    if ( m_bVisible ) {
        dc.SetPen( *wxThePenList->FindOrCreatePen( m_col, width, style ) );
        dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_fillcol, wxCROSSDIAG_HATCH ) );
    }

    wxPoint rpt1, rpt2;
    m_bpts = new wxPoint[ pOCPNPointList->GetCount() ];
    int j = 0;
    
    if ( m_bVisible )
        DrawPointWhich( dc, 1, &rpt1 );

    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();
    OCPNPoint *prp1 = node->GetData();
    node = node->GetNext();
    
    m_bpts[ j++ ] = rpt1;
        
    if ( !m_bVisible && prp1->m_bKeepXPath )
            prp1->Draw( dc );

    while( node ) {

        OCPNPoint *prp2 = node->GetData();
        if ( !m_bVisible && prp2->m_bKeepXPath )
            prp2->Draw( dc );
        else if (m_bVisible)
            prp2->Draw( dc, &rpt2 );
        m_bpts[ j++ ] = ( rpt2 );

        if ( m_bVisible )
        {
            //    Handle offscreen points
            LLBBox llbb;
            llbb.SetMin(VP.lon_min, VP.lat_min);
            llbb.SetMax(VP.lon_max, VP.lat_max);
            bool b_2_on = llbb.PointInBox( prp2->m_lon, prp2->m_lat, 0 );
            bool b_1_on = llbb.PointInBox( prp1->m_lon, prp1->m_lat, 0 );

            //Simple case
            if( b_1_on && b_2_on ) RenderSegment( dc, rpt1.x, rpt1.y, rpt2.x, rpt2.y, VP, false, m_hiliteWidth ); // with no arrows

            //    In the cases where one point is on, and one off
            //    we must decide which way to go in longitude
            //     Arbitrarily, we will go the shortest way

            double pix_full_circle = WGS84_semimajor_axis_meters * mercator_k0 * 2 * PI
                * VP.view_scale_ppm;
            double dp = pow( (double) ( rpt1.x - rpt2.x ), 2 ) + pow( (double) ( rpt1.y - rpt2.y ), 2 );
            double dtest;
            int adder;
            if( b_1_on && !b_2_on ) {
                if( rpt2.x < rpt1.x ) adder = (int) pix_full_circle;
                else
                    adder = -(int) pix_full_circle;

                dtest = pow( (double) ( rpt1.x - ( rpt2.x + adder ) ), 2 )
                    + pow( (double) ( rpt1.y - rpt2.y ), 2 );

                if( dp < dtest ) adder = 0;

                RenderSegment( dc, rpt1.x, rpt1.y, rpt2.x + adder, rpt2.y, VP, false, m_hiliteWidth );
            } else
                if( !b_1_on ) {
                    if( rpt1.x < rpt2.x ) adder = (int) pix_full_circle;
                    else
                        adder = -(int) pix_full_circle;
                    
                    float rxd = rpt2.x - ( rpt1.x + adder );
                    float ryd = rpt1.y - rpt2.y;
                    dtest = rxd*rxd + ryd*ryd;
                    
                    if( dp < dtest ) adder = 0;

                    RenderSegment( dc, rpt1.x + adder, rpt1.y, rpt2.x, rpt2.y, VP, false, m_hiliteWidth );
                }
        }

        rpt1 = rpt2;
        prp1 = prp2;

        node = node->GetNext();
    }

}

extern ChartCanvas *ocpncc1; /* hopefully can eventually remove? */

void Path::DrawGL( PlugIn_ViewPort &piVP )
{
#ifdef ocpnUSE_GL
    if( m_nPoints < 1 || !m_bVisible ) return;
    //  Hiliting first
    //  Being special case to draw something for a 1 point route....
    ocpnDC dc;
    if(m_hiliteWidth){
        wxColour y;
        GetGlobalColor( wxS( "YELO1" ), &y );
        wxColour hilt( y.Red(), y.Green(), y.Blue(), 128 );
        wxPen HiPen( hilt, m_hiliteWidth, wxSOLID );
        dc.SetPen( HiPen );
        
        wxOCPNPointListNode *node = pOCPNPointList->GetFirst();
        OCPNPoint *prp0 = node->GetData();
        wxPoint r0;
        GetCanvasPixLL( &piVP, &r0, prp0->m_lat, prp0->m_lon );

        if( m_nPoints == 1 ) {
            dc.StrokeLine( r0.x, r0.y, r0.x + 2, r0.y + 2 );
//            return;
        }
            
        node = node->GetNext();
    
        while( node ){
            
            OCPNPoint *prp = node->GetData();
            wxPoint r1;
            GetCanvasPixLL( &piVP, &r1, prp->m_lat, prp->m_lon );

            dc.StrokeLine( r0.x, r0.y, r1.x, r1.y );
                    
            r0 = r1;
            node = node->GetNext();
        }
    }
    
//    if( m_nPoints < 2  )
//        return;
    
    /* determine color and width */
    int width = m_width;
    
    wxString colour, fillcolour;
    
    if( m_bVisible && m_bPathIsActive ) {
        colour = m_ActiveLineColour;
        fillcolour = m_ActiveFillColour;
    }
    else {
        colour = m_InActiveLineColour;
        fillcolour = m_InActiveFillColour;
        
    }

    if( colour.IsNull() ) {
        colour = m_ActiveLineColour;
    }
    for( unsigned int i = 0; i < sizeof( ::GpxxColorNames ) / sizeof(wxString); i++ ) {
        if( colour == ::GpxxColorNames[i] ) {
            m_col = ::GpxxColors[i];
            break;
        }
    }

    if( fillcolour.IsNull() ) {
        m_fillcol = m_ActiveFillColour;
    }
    for( unsigned int i = 0; i < sizeof( ::GpxxColorNames ) / sizeof(wxString); i++ ) {
        if( fillcolour == ::GpxxColorNames[i] ) {
            m_fillcol = ::GpxxColors[i];
            break;
        }
    }
   
    int style = wxSOLID;
    if( m_style != STYLE_UNDEFINED ) style = m_style;
    dc.SetPen( *wxThePenList->FindOrCreatePen( m_col, width, style ) );
    dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_fillcol, wxCROSSDIAG_HATCH ) );
    
    glColor3ub(m_col.Red(), m_col.Green(), m_col.Blue());
    glLineWidth( wxMax( g_ODGLMinSymbolLineWidth, width ) );
    
    dc.SetGLStipple();

    glBegin(GL_LINE_STRIP);
    float lastlon = 0;
    float lastlat = 0;
    unsigned short int FromSegNo = 1;

    int j = 0;

    m_bpts = new wxPoint[ pOCPNPointList->GetCount() ];

    for(wxOCPNPointListNode *node = pOCPNPointList->GetFirst();
        node; node = node->GetNext()) {
        OCPNPoint *prp = node->GetData();
        unsigned short int ToSegNo = prp->m_GPXTrkSegNo;
        
        /* crosses IDL? if so break up into two segments */
        int dir = 0;
        if(prp->m_lon > 150 && lastlon < -150)
            dir = -1;
        else if(prp->m_lon < -150 && lastlon > 150)
            dir = 1;
        
        wxPoint r;
        if (FromSegNo != ToSegNo)
        {
            glEnd();
            FromSegNo = ToSegNo;
            glBegin(GL_LINE_STRIP);
        }
        if(dir)
        {
            double crosslat = lat_rl_crosses_meridian(lastlat, lastlon, prp->m_lat, prp->m_lon, 180.0);
            GetCanvasPixLL( &piVP, &r, crosslat, dir*180 );
            glVertex2i(r.x, r.y);
            glEnd();
            glBegin(GL_LINE_STRIP);
            GetCanvasPixLL( &piVP, &r, crosslat, -dir*180 );
            glVertex2i(r.x, r.y);
        }
        lastlat=prp->m_lat;
        lastlon=prp->m_lon;
        
        GetCanvasPixLL( &piVP, &r, prp->m_lat, prp->m_lon );
        glVertex2i(r.x, r.y);

        m_bpts[ j++ ] = r;
    }
    glEnd();
    glDisable (GL_LINE_STIPPLE);
        
#endif
}

static int s_arrow_icon[] = { 0, 0, 5, 2, 18, 6, 12, 0, 18, -6, 5, -2, 0, 0 };

void Path::RenderSegment( ocpnDC& dc, int xa, int ya, int xb, int yb, PlugIn_ViewPort &VP,
        bool bdraw_arrow, int hilite_width )
{
    //    Get the dc boundary
    int sx, sy;
    sx = VP.pix_width;
    sy = VP.pix_height;

    //    Try to exit early if the segment is nowhere near the screen
    wxRect r( 0, 0, sx, sy );
    wxRect s( xa, ya, 1, 1 );
    wxRect t( xb, yb, 1, 1 );
    s.Union( t );
    if( !r.Intersects( s ) ) return;

    //    Clip the line segment to the dc boundary
    int x0 = xa;
    int y0 = ya;
    int x1 = xb;
    int y1 = yb;

    //    If hilite is desired, use a Native Graphics context to render alpha colours
    //    That is, if wxGraphicsContext is available.....

    if( hilite_width ) {
        if( Visible == cohen_sutherland_line_clip_i( &x0, &y0, &x1, &y1, 0, sx, 0, sy ) ) {
            wxPen psave = dc.GetPen();

            wxColour y;
            GetGlobalColor( wxS( "YELO1" ), &y );
            wxColour hilt( y.Red(), y.Green(), y.Blue(), 128 );

            wxPen HiPen( hilt, hilite_width, wxSOLID );

            dc.SetPen( HiPen );
            dc.StrokeLine( x0, y0, x1, y1 );

            dc.SetPen( psave );
            dc.StrokeLine( x0, y0, x1, y1 );
        }
    } else {
        if( Visible == cohen_sutherland_line_clip_i( &x0, &y0, &x1, &y1, 0, sx, 0, sy ) )
            dc.StrokeLine( x0, y0, x1, y1 );
    }

    if( bdraw_arrow ) {
        //    Draw a direction arrow

        double theta = atan2( (double) ( yb - ya ), (double) ( xb - xa ) );
        theta -= PI / 2.;

        wxPoint icon[10];
        double icon_scale_factor = 100 * VP.view_scale_ppm;
        icon_scale_factor = fmin ( icon_scale_factor, 1.5 );              // Sets the max size
        icon_scale_factor = fmax ( icon_scale_factor, .10 );

        //    Get the absolute line length
        //    and constrain the arrow to be no more than xx% of the line length
        double nom_arrow_size = 20.;
        double max_arrow_to_leg = .20;
        double lpp = sqrt( pow( (double) ( xa - xb ), 2 ) + pow( (double) ( ya - yb ), 2 ) );

        double icon_size = icon_scale_factor * nom_arrow_size;
        if( icon_size > ( lpp * max_arrow_to_leg ) ) icon_scale_factor = ( lpp * max_arrow_to_leg )
                / nom_arrow_size;

        for( int i = 0; i < 7; i++ ) {
            int j = i * 2;
            double pxa = (double) ( s_arrow_icon[j] );
            double pya = (double) ( s_arrow_icon[j + 1] );

            pya *= icon_scale_factor;
            pxa *= icon_scale_factor;

            double px = ( pxa * sin( theta ) ) + ( pya * cos( theta ) );
            double py = ( pya * sin( theta ) ) - ( pxa * cos( theta ) );

            icon[i].x = (int) ( px ) + xb;
            icon[i].y = (int) ( py ) + yb;
        }
        wxPen savePen = dc.GetPen();
        dc.SetPen( *wxTRANSPARENT_PEN );
        dc.StrokePolygon( 6, &icon[0], 0, 0 );
        dc.SetPen( savePen );
    }
}

void Path::RenderSegmentArrowsGL( int xa, int ya, int xb, int yb, PlugIn_ViewPort &VP)
{
#ifdef ocpnUSE_GL
    //    Draw a direction arrow        
    wxPoint icon[10];
    float icon_scale_factor = 100 * VP.view_scale_ppm;
    icon_scale_factor = fmin ( icon_scale_factor, 1.5 );              // Sets the max size
    icon_scale_factor = fmax ( icon_scale_factor, .10 );
    
    //    Get the absolute line length
    //    and constrain the arrow to be no more than xx% of the line length
    float nom_arrow_size = 20.;
    float max_arrow_to_leg = (float).20;
    float lpp = sqrtf( powf( (float) (xa - xb), 2) + powf( (float) (ya - yb), 2) );
    
    float icon_size = icon_scale_factor * nom_arrow_size;
    if( icon_size > ( lpp * max_arrow_to_leg ) )
        icon_scale_factor = ( lpp * max_arrow_to_leg )
            / nom_arrow_size;

    float theta = atan2f( (float)yb - ya, (float)xb - xa );
    theta -= (float)PI;

    glPushMatrix();
    glTranslatef(xb, yb, 0);
    glScalef(icon_scale_factor, icon_scale_factor, 1);
    glRotatef(theta * 180/PI, 0, 0, 1);

    glBegin(GL_POLYGON);
    for( int i = 0; i < 14; i+=2 )
        glVertex2f(s_arrow_icon[i], s_arrow_icon[i+1]);
    glEnd();

    glPopMatrix();
#endif
}

void Path::ClearHighlights( void )
{
    OCPNPoint *pop = NULL;
    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();

    while( node ) {
        pop = node->GetData();
        if( pop ) pop->m_bPtIsSelected = false;
        node = node->GetNext();
    }
}

wxString Path::GetNewMarkSequenced( void )
{
    wxString ret;
    int num;
    num = 0;
    OCPNPoint *pop = NULL;
    
    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();
    while ( node ) {
        pop = node->GetData();
        wxString  sPointName = pop->GetName();
        if ( sPointName.length() == 5 ){
            if( sPointName.Left( 2 ) == wxT("NM") ) {
                num = atoi(sPointName.Mid( 2, sPointName.length() ));
                if( num >= m_nm_sequence ) m_nm_sequence = num + 1;
            }
        }
        node = node->GetNext();
    }

    ret.Printf( _T ( "NM%03d" ), m_nm_sequence );
    m_nm_sequence++;

    return ret;
}

OCPNPoint *Path::GetLastPoint()
{
    OCPNPoint *data_m1 = NULL;
    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();

    while( node ) {
        data_m1 = node->GetData();
        node = node->GetNext();
    }
    return ( data_m1 );
}

int Path::GetIndexOf( OCPNPoint *prp )
{
    int ret = pOCPNPointList->IndexOf( prp ) + 1;
    if( ret == wxNOT_FOUND ) return 0;
    else
        return ret;

}

void Path::DeletePoint( OCPNPoint *rp, bool bRenamePoints )
{
    //    n.b. must delete Selectables  and update config before deleting the point
    if( rp->m_bIsInLayer ) return;

    pOCPNSelect->DeleteAllSelectableOCPNPoints( (Path *) this );
    pOCPNSelect->DeleteAllSelectablePathSegments( (Path *) this );
    pOCPNDrawConfig->DeleteOCPNPoint( rp );

    pOCPNPointList->DeleteObject( rp );

    if( ( rp->m_GUID.Len() ) && ( wxNOT_FOUND != OCPNPointGUIDList.Index( rp->m_GUID ) ) ) OCPNPointGUIDList.Remove(
            rp->m_GUID );

    delete rp;

    m_nPoints -= 1;

    if( bRenamePoints ) RenameOCPNPoints();

    if( m_nPoints > 1 ) {
        pOCPNSelect->AddAllSelectablePathSegments( this );
        pOCPNSelect->AddAllSelectableOCPNPoints( this );

        pOCPNDrawConfig->UpdatePath( this );
        RebuildGUIDList();                  // ensure the GUID list is intact and good

        FinalizeForRendering();
        UpdateSegmentDistances();
    }
}

void Path::RemovePoint( OCPNPoint *rp, bool bRenamePoints )
{
    pOCPNSelect->DeleteAllSelectableOCPNPoints( this );
    pOCPNSelect->DeleteAllSelectablePathSegments( this );

    pOCPNPointList->DeleteObject( rp );
    if( wxNOT_FOUND != OCPNPointGUIDList.Index( rp->m_GUID ) ) OCPNPointGUIDList.Remove(
            rp->m_GUID );
    m_nPoints -= 1;

    // check all other routes to see if this point appears in any other route
//    Route *pcontainer_route = g_pRouteMan->FindRouteContainingWaypoint( rp );

//    if( pcontainer_route == NULL ) {
        rp->m_bIsInPath = false;          // Take this point out of this (and only) route
//        rp->m_bDynamicName = false;
//        rp->m_bIsolatedMark = true;        // This has become an isolated mark
//    }

    if( bRenamePoints ) RenameOCPNPoints();

//      if ( m_nPoints > 1 )
    {
        pOCPNSelect->AddAllSelectablePathSegments( this );
        pOCPNSelect->AddAllSelectableOCPNPoints( this );

        pOCPNDrawConfig->UpdatePath( this );
        RebuildGUIDList();                  // ensure the GUID list is intact and good

        FinalizeForRendering();
        UpdateSegmentDistances();
    }

}

void Path::DeSelectPath()
{
    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();

    OCPNPoint *rp;
    while( node ) {
        rp = node->GetData();
        rp->m_bPtIsSelected = false;

        node = node->GetNext();
    }
}

void Path::ReloadPathPointIcons()
{
    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();

    OCPNPoint *rp;
    while( node ) {
        rp = node->GetData();
        rp->ReLoadIcon();

        node = node->GetNext();
    }
}

void Path::FinalizeForRendering()
{
    m_bNeedsUpdateBBox = true;
}

wxBoundingBox Path::GetBBox( void )
{
    if(!m_bNeedsUpdateBBox)
        return RBBox;

    double bbox_xmin = 180.;                        // set defaults
    double bbox_ymin = 90.;
    double bbox_xmax = -180;
    double bbox_ymax = -90.;

    RBBox.Reset();
    m_bcrosses_idl = CalculateCrossesIDL();

    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();
    OCPNPoint *data;

    if( !m_bcrosses_idl ) {
        while( node ) {
            data = node->GetData();

            if( data->m_lon > bbox_xmax ) bbox_xmax = data->m_lon;
            if( data->m_lon < bbox_xmin ) bbox_xmin = data->m_lon;
            if( data->m_lat > bbox_ymax ) bbox_ymax = data->m_lat;
            if( data->m_lat < bbox_ymin ) bbox_ymin = data->m_lat;

            node = node->GetNext();
        }
    } else {
        //    For Routes that cross the IDL, we compute and store
        //    the bbox as positive definite
        while( node ) {
            data = node->GetData();
            double lon = data->m_lon;
            if( lon < 0. ) lon += 360.;

            if( lon > bbox_xmax ) bbox_xmax = lon;
            if( lon < bbox_xmin ) bbox_xmin = lon;
            if( data->m_lat > bbox_ymax ) bbox_ymax = data->m_lat;
            if( data->m_lat < bbox_ymin ) bbox_ymin = data->m_lat;

            node = node->GetNext();
        }
    }

    RBBox.Expand( bbox_xmin, bbox_ymin );
    RBBox.Expand( bbox_xmax, bbox_ymax );

    m_bNeedsUpdateBBox = false;
    return RBBox;
}

bool Path::CalculateCrossesIDL( void )
{
    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();
    if( NULL == node ) return false;

    bool idl_cross = false;
    OCPNPoint *data = node->GetData();             // first node

    double lon0 = data->m_lon;
    node = node->GetNext();

    while( node ) {
        data = node->GetData();
        if( ( lon0 < -150. ) && ( data->m_lon > 150. ) ) {
            idl_cross = true;
            break;
        }

        if( ( lon0 > 150. ) && ( data->m_lon < -150. ) ) {
            idl_cross = true;
            break;
        }

        lon0 = data->m_lon;

        node = node->GetNext();
    }

    return idl_cross;
}

void Path::CalculateDCRect( wxDC& dc_boundary, wxRect *prect, PlugIn_ViewPort &VP )
{
    dc_boundary.ResetBoundingBox();
    dc_boundary.DestroyClippingRegion();
    
    wxRect update_rect;

    // Draw the route in skeleton form on the dc
    // That is, draw only the route points, assuming that the segements will
    // always be fully contained within the resulting rectangle.
    // Can we prove this?
    if( m_bVisible ) {
        wxOCPNPointListNode *node = pOCPNPointList->GetFirst();
        while( node ) {

            OCPNPoint *prp2 = node->GetData();
            bool blink_save = prp2->m_bBlink;
            prp2->m_bBlink = false;
            ocpnDC odc_boundary( dc_boundary );
            prp2->Draw( odc_boundary, NULL );
            prp2->m_bBlink = blink_save;

            wxRect r =  prp2->CurrentRect_in_DC ;
            r.Inflate(m_hiliteWidth, m_hiliteWidth);        // allow for large hilite circles at segment ends
                
            update_rect.Union( r );
            node = node->GetNext();
        }
    }

    *prect = update_rect;
}

void Path::RebuildGUIDList( void )
{
    OCPNPointGUIDList.Clear();               // empty the GUID list

    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();

    OCPNPoint *rp;
    while( node ) {
        rp = node->GetData();
        OCPNPointGUIDList.Add( rp->m_GUID );

        node = node->GetNext();
    }
}
void Path::SetVisible( bool visible, bool includeWpts )
{
    m_bVisible = visible;

    if ( !includeWpts )
        return;

    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();
    OCPNPoint *rp;
    while( node ) {
        rp = node->GetData();
        if ( rp->m_bKeepXPath )
        {
            rp->SetVisible( visible );
            //pOCPNDrawConfig->UpdateWayPoint( rp );
        }
        node = node->GetNext();
    }
}

void Path::SetListed( bool visible )
{
    m_bListed = visible;
}

void Path::AssemblePath( void )
{
    //    iterate over the OCPNPointGUIDs
    for( unsigned int ip = 0; ip < OCPNPointGUIDList.GetCount(); ip++ ) {
        wxString GUID = OCPNPointGUIDList[ip];

        //    And on the OCPNPoints themselves
        wxOCPNPointListNode *prpnode = pOCPNPointMan->GetOCPNPointList()->GetFirst();
        while( prpnode ) {
            OCPNPoint *prp = prpnode->GetData();

            if( prp->m_GUID == GUID ) {
                AddPoint( prp );
                break;
            }
            prpnode = prpnode->GetNext(); //OCPNPoint
        }
    }
}

void Path::RenameOCPNPoints( void )
{
    //    iterate on the route points.
    //    If dynamically named, rename according to current list position

    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();

    int i = 1;
    while( node ) {
        OCPNPoint *prp = node->GetData();
        if( prp->m_bDynamicName ) {
            wxString name;
            name.Printf( _T ( "%03d" ), i );
            prp->SetName( name );
        }

        node = node->GetNext();
        i++;
    }
}

//    Is this Path equal to another, meaning,
//    Do all OCPNPoint positions and names match?
bool Path::IsEqualTo( Path *ptargetroute )
{
    wxOCPNPointListNode *pthisnode = ( this->pOCPNPointList )->GetFirst();
    wxOCPNPointListNode *pthatnode = ( ptargetroute->pOCPNPointList )->GetFirst();

    if( NULL == pthisnode ) return false;

    if( this->m_bIsInLayer || ptargetroute->m_bIsInLayer ) return false;

    if( this->GetnPoints() != ptargetroute->GetnPoints() ) return false;

    while( pthisnode ) {
        if( NULL == pthatnode ) return false;

        OCPNPoint *pthisrp = pthisnode->GetData();
        OCPNPoint *pthatrp = pthatnode->GetData();

        if( ( fabs( pthisrp->m_lat - pthatrp->m_lat ) > 1.0e-6 )
                || ( fabs( pthisrp->m_lon - pthatrp->m_lon ) > 1.0e-6 ) ) return false;

        if( !pthisrp->GetName().IsSameAs( pthatrp->GetName() ) ) return false;

        pthisnode = pthisnode->GetNext();
        pthatnode = pthatnode->GetNext();
    }

    return true;                              // success, they are the same
}
/*
 Update the boundary segment lengths, storing each segment length in <destination> point.
 Also, compute total boundary length by summing segment distances.
 */
void Path::UpdateSegmentDistances()
{
    wxPoint rpt, rptn;
    float slat1, slon1, slat2, slon2;

    double path_len = 0.0;

    wxOCPNPointListNode *node = pOCPNPointList->GetFirst();

    if( node ) {
        OCPNPoint *prp0 = node->GetData();
        slat1 = prp0->m_lat;
        slon1 = prp0->m_lon;

        node = node->GetNext();

        while( node ) {
            OCPNPoint *prp = node->GetData();
            slat2 = prp->m_lat;
            slon2 = prp->m_lon;

//    Calculate the absolute distance from 1->2

            double brg, dd;
            DistanceBearingMercator_Plugin( slat1, slon1, slat2, slon2, &brg, &dd );

//    And store in Point 2
            prp->m_seg_len = dd;

            path_len += dd;

            slat1 = slat2;
            slon1 = slon2;

            prp0 = prp;

            node = node->GetNext();
        }
    }

    m_path_length = path_len;
}

OCPNPoint *Path::InsertPointBefore( OCPNPoint *pRP, double rlat, double rlon,
        bool bRenamePoints )
{
    OCPNPoint *newpoint = new OCPNPoint( rlat, rlon, g_sOCPNPointIconName,
            GetNewMarkSequenced(), wxT("") );
    newpoint->m_bIsInPath = true;
    newpoint->m_bDynamicName = true;
    newpoint->SetNameShown( false );

    int nRP = pOCPNPointList->IndexOf( pRP );
    if ( nRP == 0 ) {
        pOCPNPointList->Insert( pOCPNPointList->GetCount() - 1, newpoint );
        nRP = pOCPNPointList->GetCount();
    }
    else {
        pOCPNPointList->Insert( nRP, newpoint );
    }

    OCPNPointGUIDList.Insert( pRP->m_GUID, nRP );

    m_nPoints++;

    if( bRenamePoints ) RenameOCPNPoints();

    FinalizeForRendering();
    UpdateSegmentDistances();

    return ( newpoint );
}

OCPNPoint *Path::InsertPointAfter( OCPNPoint *pOP, double rlat, double rlon, bool bRenamePoints )
{
    int nOP = pOCPNPointList->IndexOf( pOP );
    if( nOP >= m_nPoints - 1 )
        return NULL;
    nOP++;
    
    OCPNPoint *newpoint = new OCPNPoint( rlat, rlon, g_sOCPNPointIconName, GetNewMarkSequenced(), wxT("") );
    newpoint->m_bIsInPath = true;
    newpoint->m_bDynamicName = true;
    newpoint->SetNameShown( false );
    
    pOCPNPointList->Insert( nOP, newpoint );
    
    OCPNPointGUIDList.Insert( pOP->m_GUID, nOP );
    
    m_nPoints++;
    
    if( bRenamePoints ) RenameOCPNPoints();
    
    FinalizeForRendering();
    UpdateSegmentDistances();
    
    return ( newpoint );
}


void Path::RemovePointFromPath( OCPNPoint* point, Path* path )
{
    //  Rebuild the route selectables
    pOCPNSelect->DeleteAllSelectableOCPNPoints( path );
    pOCPNSelect->DeleteAllSelectablePathSegments( path );

    path->RemovePoint( point );

    //  Check for 1 point routes. If we are creating a route, this is an undo, so keep the 1 point.
    if( (path->GetnPoints() <= 1) && (g_ocpn_draw_pi->nBoundary_State == 0) ) {
        pOCPNDrawConfig->DeleteConfigPath( path );
        g_pPathMan->DeletePath( path );
        path = NULL;
    }
    //  Add this point back into the selectables
    pOCPNSelect->AddSelectableOCPNPoint( point->m_lat, point->m_lon, point );

    if( pPathPropDialog && ( pPathPropDialog->IsShown() ) ) {
        pPathPropDialog->SetPathAndUpdate( path, true );
    }

}
