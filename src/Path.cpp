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
#include "ocpndc.h"
#include "cutil.h"
#include "ODSelect.h"
#include "PointMan.h"
#include "PathMan.h"
#include "ODPathPropertiesDialogImpl.h"
#include "ODConfig.h"
#include "ocpn_draw_pi.h"
#include "ODUtils.h"
#include "bbox.h"
#include "ocpndc.h"
#include "dychart.h"
#include <wx/gdicmn.h>

extern PointMan *g_pODPointMan;
extern bool g_bODIsNewLayer;
extern int g_ODLayerIdx;
extern PathMan *g_pPathMan;
extern int g_path_line_width;
extern ODSelect *g_pODSelect;
extern ODConfig *g_pODConfig;
extern float g_ODGLMinSymbolLineWidth;
extern ODPathPropertiesDialogImpl *g_pODPathPropDialog;
extern ocpn_draw_pi *g_ocpn_draw_pi;
extern wxString     g_sODPointIconName;
extern wxColour    g_colourActivePathLineColour;
extern wxColour    g_colourInActivePathLineColour;


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
    m_iBlink = 0;
    m_bDeleteOnArrival = false;
    m_width = STYLE_UNDEFINED;
    m_style = STYLE_UNDEFINED;
    m_hiliteWidth = 0;

    m_pODPointList = new ODPointList;
    m_pLastAddedPoint = NULL;
    m_pFirstAddedPoint = NULL;
    m_GUID = GetUUID();
    m_btemp = false;
    
    m_bNeedsUpdateBBox = true;
    RBBox.Reset();
    m_bcrosses_idl = false;

    m_LayerID = 0;
    m_bIsInLayer = false;

    m_wxcActiveLineColour = g_colourActivePathLineColour;
    m_wxcInActiveLineColour = g_colourInActivePathLineColour;
    SetActiveColours();
    
    m_lastMousePointIndex = 0;
    m_NextLegGreatCircle = false;
    
    m_HyperlinkList = new HyperlinkList;
}

Path::~Path( void )
{
    m_pODPointList->DeleteContents( false );            // do not delete Marks
    m_pODPointList->Clear();
    delete m_pODPointList;
    m_HyperlinkList->Clear();
    delete m_HyperlinkList;
}

void Path::AddPoint( ODPoint *pNewPoint, bool b_rename_in_sequence, bool b_deferBoxCalc, bool b_isLoading )
{
    if( pNewPoint->m_bIsolatedMark ) {
        pNewPoint->m_bKeepXPath = true;
    }
    pNewPoint->m_bIsolatedMark = false;       // definitely no longer isolated
    pNewPoint->m_bIsInPath = true;

    m_pODPointList->Append( pNewPoint );

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
    ODPointGUIDList.Add( GUID );
    return;
}

ODPoint *Path::GetPoint( int nWhichPoint )
{
    ODPoint *pOp;
    wxODPointListNode *node = m_pODPointList->GetFirst();

    int i = 1;
    while( node ) {
        pOp = node->GetData();
        if( i == nWhichPoint ) return pOp;

        i++;
        node = node->GetNext();
    }

    return ( NULL );
}

ODPoint *Path::GetPoint( const wxString &guid )
{
    ODPoint *pOp;
    wxODPointListNode *node = m_pODPointList->GetFirst();

    while( node ) {
        pOp = node->GetData();
        if( guid == pOp->m_GUID ) return pOp;

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
    if( m_bPathIsSelected ) dc.SetPen( *g_pPathMan->GetSelectedPathPen() );
    else
        if( m_bPathIsActive ) dc.SetPen( *g_pPathMan->GetActivePathPen() );
        else
            dc.SetPen( *g_pPathMan->GetPathPen() );
    
    wxColour col;
    wxString colour;

    dc.SetPen( *wxThePenList->FindOrCreatePen( m_col, g_path_line_width, m_style ) );

    RenderSegment( dc, rp1->x, rp1->y, rp2->x, rp2->y, VP, bdraw_arrow );
}

void Path::Draw( ocpnDC& dc, PlugIn_ViewPort &VP )
{
    wxString colour;
    int style = wxSOLID;
    int width = g_path_line_width;


    if( m_nPoints == 0 ) return;


    if( m_style != STYLE_UNDEFINED ) style = m_style;
    if( m_width != STYLE_UNDEFINED ) width = m_width;

    if ( m_bVisible ) {
        dc.SetPen( *wxThePenList->FindOrCreatePen( m_col, width, style ) );
    }

    wxPoint ppt1, ppt2;
    m_bpts = new wxPoint[ m_pODPointList->GetCount() ];
    int j = 0;
    
    if ( m_bVisible )
        DrawPointWhich( dc, 1, &ppt1 );

    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *pOp1 = node->GetData();
    node = node->GetNext();
    
    m_bpts[ j++ ] = ppt1;
        
    if ( !m_bVisible && pOp1->m_bKeepXPath )
            pOp1->Draw( dc );

    while( node ) {

        ODPoint *pOp2 = node->GetData();
        if ( !m_bVisible && pOp2->m_bKeepXPath )
            pOp2->Draw( dc );
        else if (m_bVisible)
            pOp2->Draw( dc, &ppt2 );
        m_bpts[ j++ ] = ( ppt2 );

        if ( m_bVisible )
        {
            //    Handle offscreen points
            LLBBox llbb;
            llbb.SetMin(VP.lon_min, VP.lat_min);
            llbb.SetMax(VP.lon_max, VP.lat_max);
            bool b_2_on = llbb.PointInBox( pOp2->m_lon, pOp2->m_lat, 0 );
            bool b_1_on = llbb.PointInBox( pOp1->m_lon, pOp1->m_lat, 0 );

            //Simple case
            if( b_1_on && b_2_on ) RenderSegment( dc, ppt1.x, ppt1.y, ppt2.x, ppt2.y, VP, false, m_hiliteWidth ); // with no arrows

            //    In the cases where one point is on, and one off
            //    we must decide which way to go in longitude
            //     Arbitrarily, we will go the shortest way

            double pix_full_circle = WGS84_semimajor_axis_meters * mercator_k0 * 2 * PI
                * VP.view_scale_ppm;
            double dp = pow( (double) ( ppt1.x - ppt2.x ), 2 ) + pow( (double) ( ppt1.y - ppt2.y ), 2 );
            double dtest;
            int adder;
            if( b_1_on && !b_2_on ) {
                if( ppt2.x < ppt1.x ) adder = (int) pix_full_circle;
                else
                    adder = -(int) pix_full_circle;

                dtest = pow( (double) ( ppt1.x - ( ppt2.x + adder ) ), 2 )
                    + pow( (double) ( ppt1.y - ppt2.y ), 2 );

                if( dp < dtest ) adder = 0;

                RenderSegment( dc, ppt1.x, ppt1.y, ppt2.x + adder, ppt2.y, VP, false, m_hiliteWidth );
            } else
                if( !b_1_on ) {
                    if( ppt1.x < ppt2.x ) adder = (int) pix_full_circle;
                    else
                        adder = -(int) pix_full_circle;
                    
                    float rxd = ppt2.x - ( ppt1.x + adder );
                    float ryd = ppt1.y - ppt2.y;
                    dtest = rxd*rxd + ryd*ryd;
                    
                    if( dp < dtest ) adder = 0;

                    RenderSegment( dc, ppt1.x + adder, ppt1.y, ppt2.x, ppt2.y, VP, false, m_hiliteWidth );
                }
        }

        ppt1 = ppt2;
        pOp1 = pOp2;

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
        
        wxODPointListNode *node = m_pODPointList->GetFirst();
        ODPoint *pOp0 = node->GetData();
        wxPoint r0;
        GetCanvasPixLL( &piVP, &r0, pOp0->m_lat, pOp0->m_lon );

        if( m_nPoints == 1 ) {
            dc.StrokeLine( r0.x, r0.y, r0.x + 2, r0.y + 2 );
//            return;
        }
            
        node = node->GetNext();
    
        while( node ){
            
            ODPoint *pOp = node->GetData();
            wxPoint r1;
            GetCanvasPixLL( &piVP, &r1, pOp->m_lat, pOp->m_lon );

            dc.StrokeLine( r0.x, r0.y, r1.x, r1.y );
                    
            r0 = r1;
            node = node->GetNext();
        }
    }
    
//    if( m_nPoints < 2  )
//        return;
    
    /* determine color and width */
    int width = m_width;
    
    
    int style = wxSOLID;
    if( m_style != STYLE_UNDEFINED ) style = m_style;
    dc.SetPen( *wxThePenList->FindOrCreatePen( m_col, width, style ) );

    glColor3ub(m_col.Red(), m_col.Green(), m_col.Blue());
    glLineWidth( wxMax( g_ODGLMinSymbolLineWidth, width ) );
    
    dc.SetGLStipple();

    glBegin(GL_LINE_STRIP);
    float lastlon = 0;
    float lastlat = 0;
    unsigned short int FromSegNo = 1;

    int j = 0;

    m_bpts = new wxPoint[ m_pODPointList->GetCount() ];

    for(wxODPointListNode *node = m_pODPointList->GetFirst();
        node; node = node->GetNext()) {
        ODPoint *pOp = node->GetData();
        unsigned short int ToSegNo = pOp->m_GPXTrkSegNo;
        
        /* crosses IDL? if so break up into two segments */
        int dir = 0;
        if(pOp->m_lon > 150 && lastlon < -150)
            dir = -1;
        else if(pOp->m_lon < -150 && lastlon > 150)
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
            double crosslat = lat_rl_crosses_meridian(lastlat, lastlon, pOp->m_lat, pOp->m_lon, 180.0);
            GetCanvasPixLL( &piVP, &r, crosslat, dir*180 );
            glVertex2i(r.x, r.y);
            glEnd();
            glBegin(GL_LINE_STRIP);
            GetCanvasPixLL( &piVP, &r, crosslat, -dir*180 );
            glVertex2i(r.x, r.y);
        }
        lastlat=pOp->m_lat;
        lastlon=pOp->m_lon;
        
        GetCanvasPixLL( &piVP, &r, pOp->m_lat, pOp->m_lon );
        glVertex2i(r.x, r.y);

        //if ( m_bVisible || pOp->m_bKeepXPath )
        //    pOp->DrawGL( piVP );

        m_bpts[ j++ ] = r;
    }
    glEnd();
    glDisable (GL_LINE_STIPPLE);

    /*  ODPoints  */
    for(wxODPointListNode *node = m_pODPointList->GetFirst(); node; node = node->GetNext()) {
        ODPoint *pOp = node->GetData();
        if ( m_bVisible || pOp->m_bKeepXPath )
            pOp->DrawGL( piVP );
    }        
    
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
    ODPoint *pop = NULL;
    wxODPointListNode *node = m_pODPointList->GetFirst();

    while( node ) {
        pop = node->GetData();
        if( pop ) pop->m_bPtIsSelected = false;
        node = node->GetNext();
    }
}

wxString Path::GetNewMarkSequenced( void )
{
    wxString ret;
    long num;
    num = 0;
    ODPoint *pop = NULL;
    
    wxODPointListNode *node = m_pODPointList->GetFirst();
    while ( node ) {
        pop = node->GetData();
        wxString  sPointName = pop->GetName();
        if ( sPointName.length() == 5 ){
            if( sPointName.Left( 2 ) == wxT("NM") ) {
                sPointName.Mid( 2, sPointName.length() ).ToLong( &num );
                if( num >= m_nm_sequence ) m_nm_sequence = num + 1;
            }
        }
        node = node->GetNext();
    }

    ret.Printf( _T ( "NM%03d" ), m_nm_sequence );
    m_nm_sequence++;

    return ret;
}

ODPoint *Path::GetLastPoint()
{
    ODPoint *data_m1 = NULL;
    wxODPointListNode *node = m_pODPointList->GetFirst();

    while( node ) {
        data_m1 = node->GetData();
        node = node->GetNext();
    }
    return ( data_m1 );
}

int Path::GetIndexOf( ODPoint *pOp )
{
    int ret = m_pODPointList->IndexOf( pOp ) + 1;
    if( ret == wxNOT_FOUND ) return 0;
    else
        return ret;

}

void Path::DeletePoint( ODPoint *rp, bool bRenamePoints )
{
    //    n.b. must delete Selectables  and update config before deleting the point
    if( rp->m_bIsInLayer ) return;

    g_pODSelect->DeleteAllSelectableODPoints( (Path *) this );
    g_pODSelect->DeleteAllSelectablePathSegments( (Path *) this );
    g_pODConfig->DeleteODPoint( rp );

    m_pODPointList->DeleteObject( rp );

    if( ( rp->m_GUID.Len() ) && ( wxNOT_FOUND != ODPointGUIDList.Index( rp->m_GUID ) ) ) ODPointGUIDList.Remove(
            rp->m_GUID );

    delete rp;

    m_nPoints -= 1;

    if( bRenamePoints ) RenameODPoints();

    if( m_nPoints > 1 ) {
        g_pODSelect->AddAllSelectablePathSegments( this );
        g_pODSelect->AddAllSelectableODPoints( this );

        g_pODConfig->UpdatePath( this );
        RebuildGUIDList();                  // ensure the GUID list is intact and good

        FinalizeForRendering();
        UpdateSegmentDistances();
    }
}

void Path::RemovePoint( ODPoint *op, bool bRenamePoints )
{
    g_pODSelect->DeleteAllSelectableODPoints( this );
    g_pODSelect->DeleteAllSelectablePathSegments( this );

    m_pODPointList->DeleteObject( op );
    if( wxNOT_FOUND != ODPointGUIDList.Index( op->m_GUID ) ) ODPointGUIDList.Remove(
            op->m_GUID );
    m_nPoints -= 1;

    // check all other routes to see if this point appears in any other route
    Path *pcontainer_path = g_pPathMan->FindPathContainingODPoint( op );

    if( pcontainer_path == NULL ) {
        op->m_bIsInPath = false;          // Take this point out of this (and only) route
        op->m_bDynamicName = false;
        op->m_bIsolatedMark = true;        // This has become an isolated mark
        op->SetTypeString (wxT("Boundary Point") );
        g_pODConfig->AddNewODPoint( op );
    }

    if( bRenamePoints ) RenameODPoints();

    if ( m_nPoints > 1 ) {
        g_pODSelect->AddAllSelectablePathSegments( this );
        g_pODSelect->AddAllSelectableODPoints( this );

        g_pODConfig->UpdatePath( this );
        RebuildGUIDList();                  // ensure the GUID list is intact and good

        FinalizeForRendering();
        UpdateSegmentDistances();
    }

}

void Path::DeSelectPath()
{
    wxODPointListNode *node = m_pODPointList->GetFirst();

    ODPoint *rp;
    while( node ) {
        rp = node->GetData();
        rp->m_bPtIsSelected = false;

        node = node->GetNext();
    }
}

void Path::ReloadPathPointIcons()
{
    wxODPointListNode *node = m_pODPointList->GetFirst();

    ODPoint *rp;
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

    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *data;

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
    wxODPointListNode *node = m_pODPointList->GetFirst();
    if( NULL == node ) return false;

    bool idl_cross = false;
    ODPoint *data = node->GetData();             // first node

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
        wxODPointListNode *node = m_pODPointList->GetFirst();
        while( node ) {

            ODPoint *pOp2 = node->GetData();
            int blink_save = pOp2->m_iBlink;
            pOp2->m_iBlink = false;
            ocpnDC odc_boundary( dc_boundary );
            pOp2->Draw( odc_boundary, NULL );
            pOp2->m_iBlink = blink_save;

            wxRect r =  pOp2->CurrentRect_in_DC ;
            r.Inflate(m_hiliteWidth, m_hiliteWidth);        // allow for large hilite circles at segment ends
                
            update_rect.Union( r );
            node = node->GetNext();
        }
    }

    *prect = update_rect;
}

void Path::RebuildGUIDList( void )
{
    ODPointGUIDList.Clear();               // empty the GUID list

    wxODPointListNode *node = m_pODPointList->GetFirst();

    ODPoint *rp;
    while( node ) {
        rp = node->GetData();
        ODPointGUIDList.Add( rp->m_GUID );

        node = node->GetNext();
    }
}
void Path::SetVisible( bool visible, bool includeWpts )
{
    m_bVisible = visible;

    if ( !includeWpts )
        return;

    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *rp;
    while( node ) {
        rp = node->GetData();
        if ( rp->m_bKeepXPath )
        {
            rp->SetVisible( visible );
            //g_pODConfig->UpdateWayPoint( rp );
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
    //    iterate over the ODPointGUIDs
    for( unsigned int ip = 0; ip < ODPointGUIDList.GetCount(); ip++ ) {
        wxString GUID = ODPointGUIDList[ip];

        //    And on the ODPoints themselves
        wxODPointListNode *pOpnode = g_pODPointMan->GetODPointList()->GetFirst();
        while( pOpnode ) {
            ODPoint *pOp = pOpnode->GetData();

            if( pOp->m_GUID == GUID ) {
                AddPoint( pOp );
                break;
            }
            pOpnode = pOpnode->GetNext(); //ODPoint
        }
    }
}

void Path::RenameODPoints( void )
{
    //    iterate on the route points.
    //    If dynamically named, rename according to current list position

    wxODPointListNode *node = m_pODPointList->GetFirst();

    int i = 1;
    while( node ) {
        ODPoint *pOp = node->GetData();
        if( pOp->m_bDynamicName ) {
            wxString name;
            name.Printf( _T ( "%03d" ), i );
            pOp->SetName( name );
        }

        node = node->GetNext();
        i++;
    }
}

//    Is this Path equal to another, meaning,
//    Do all ODPoint positions and names match?
bool Path::IsEqualTo( Path *ptargetroute )
{
    wxODPointListNode *pthisnode = ( this->m_pODPointList )->GetFirst();
    wxODPointListNode *pthatnode = ( ptargetroute->m_pODPointList )->GetFirst();

    if( NULL == pthisnode ) return false;

    if( this->m_bIsInLayer || ptargetroute->m_bIsInLayer ) return false;

    if( this->GetnPoints() != ptargetroute->GetnPoints() ) return false;

    while( pthisnode ) {
        if( NULL == pthatnode ) return false;

        ODPoint *pthisrp = pthisnode->GetData();
        ODPoint *pthatrp = pthatnode->GetData();

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
    wxPoint ppt, pptn;
    float slat1, slon1, slat2, slon2;

    double path_len = 0.0;

    wxODPointListNode *node = m_pODPointList->GetFirst();

    if( node ) {
        ODPoint *pOp0 = node->GetData();
        slat1 = pOp0->m_lat;
        slon1 = pOp0->m_lon;

        node = node->GetNext();

        while( node ) {
            ODPoint *pOp = node->GetData();
            slat2 = pOp->m_lat;
            slon2 = pOp->m_lon;

//    Calculate the absolute distance from 1->2

            double brg, dd;
            DistanceBearingMercator_Plugin( slat1, slon1, slat2, slon2, &brg, &dd );

//    And store in Point 2
            pOp->m_seg_len = dd;

            path_len += dd;

            slat1 = slat2;
            slon1 = slon2;

            pOp0 = pOp;

            node = node->GetNext();
        }
    }

    m_path_length = path_len;
}

ODPoint *Path::InsertPointBefore( ODPoint *pRP, double rlat, double rlon,
        bool bRenamePoints )
{
    ODPoint *newpoint = new ODPoint( rlat, rlon, g_sODPointIconName, GetNewMarkSequenced(), wxT("") );
    newpoint->m_bIsInPath = true;
    newpoint->m_bDynamicName = true;
    newpoint->SetNameShown( false );
    newpoint->SetTypeString( wxT("Boundary Point") );
    
    int nRP = m_pODPointList->IndexOf( pRP );
    if ( nRP == 0 ) {
        m_pODPointList->Insert( m_pODPointList->GetCount() - 1, newpoint );
        nRP = m_pODPointList->GetCount();
    }
    else {
        m_pODPointList->Insert( nRP, newpoint );
    }

    ODPointGUIDList.Insert( pRP->m_GUID, nRP );

    m_nPoints++;

    if( bRenamePoints ) RenameODPoints();

    FinalizeForRendering();
    UpdateSegmentDistances();

    return ( newpoint );
}

ODPoint *Path::InsertPointAfter( ODPoint *pOP, double rlat, double rlon, bool bRenamePoints )
{
    int nOP = m_pODPointList->IndexOf( pOP );
    if( nOP >= m_nPoints - 1 )
        return NULL;
    nOP++;
    
    ODPoint *newpoint = new ODPoint( rlat, rlon, g_sODPointIconName, GetNewMarkSequenced(), wxT("") );
    newpoint->m_bIsInPath = true;
    newpoint->m_bDynamicName = true;
    newpoint->SetNameShown( false );
    newpoint->SetTypeString( wxT("Boundary Point") );
    
    m_pODPointList->Insert( nOP, newpoint );
    
    ODPointGUIDList.Insert( pOP->m_GUID, nOP );
    
    m_nPoints++;
    
    if( bRenamePoints ) RenameODPoints();
    
    FinalizeForRendering();
    UpdateSegmentDistances();
    
    return ( newpoint );
}

void Path::InsertPointAfter( ODPoint *pOP, ODPoint *pnOP, bool bRenamePoints )
{
    int nOP = m_pODPointList->IndexOf( pOP );
    if( nOP >= m_nPoints - 1 )
        return;
    nOP++;
    
    m_pODPointList->Insert( nOP, pnOP );
    
    ODPointGUIDList.Insert( pnOP->m_GUID, nOP );
    
    m_nPoints++;
    
    if( bRenamePoints ) RenameODPoints();
    
    FinalizeForRendering();
    UpdateSegmentDistances();
    
    return;
}

void Path::RemovePointFromPath( ODPoint* point, Path* path )
{
    //  Rebuild the route selectables
    //g_pODSelect->DeleteAllSelectableODPoints( path );
    //g_pODSelect->DeleteAllSelectablePathSegments( path );

    path->RemovePoint( point );

    //  Check for 1 point routes. If we are creating a route, this is an undo, so keep the 1 point.
    if( (path->GetnPoints() <= 1) && (g_ocpn_draw_pi->nBoundary_State == 0) ) {
        g_pODConfig->DeleteConfigPath( path );
        g_pPathMan->DeletePath( path );
        path = NULL;
    } 
    
    //  Add this point back into the selectables
    g_pODSelect->AddSelectableODPoint( point->m_lat, point->m_lon, point );

    if( g_pODPathPropDialog && ( g_pODPathPropDialog->IsShown() ) ) {
        g_pODPathPropDialog->SetPathAndUpdate( path, true );
    }

}

void Path::SetActiveColours( void )
{
    wxString colour;
    
    if( m_bVisible && m_bPathIsActive ) m_col = m_wxcActiveLineColour;
    else m_col = m_wxcInActiveLineColour;

/*    if( m_bVisible && m_bPathIsActive ) {
        colour = m_ActiveLineColour;
    }
    else {
        colour = m_InActiveLineColour;
    }
    
    if( colour.IsNull() ) {
        colour = m_ActiveLineColour;
    }
    
    if( m_bVisible && m_iBlink && ( g_ocpn_draw_pi->nBlinkerTick & 1 ) )
        colour = m_InActiveLineColour;
    
    for( unsigned int i = 0; i < sizeof( ::GpxxColorNames ) / sizeof(wxString); i++ ) {
        if( colour == ::GpxxColorNames[i] ) {
            m_col = ::GpxxColors[i];
            break;
        }
    }
*/    
}
