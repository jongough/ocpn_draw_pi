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

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ODPath.h"
#include "georef.h"
#include "ODdc.h"
#include "cutil.h"
#include "ODSelect.h"
#include "PointMan.h"
#include "PathMan.h"
#include "ODPathPropertiesDialogImpl.h"
#include "ODConfig.h"
#include "ocpn_draw_pi.h"
#include "ODUtils.h"
#include "bbox.h"
#include "ODdc.h"
//#include "dychart.h"
#include <wx/gdicmn.h>
#include "gl.h"

#include <wx/listimpl.cpp>
WX_DEFINE_LIST ( PathList );

ODPath::ODPath( void )
{
    m_sTypeString = wxS("Path");
    m_bPathIsSelected = false;
    m_bPathIsActive = true;
    m_pPathActivePoint = NULL;
    m_bIsBeingEdited = false;
    m_bIsBeingCreated = true;
    m_bDrawArrow = false;
    m_nPoints = 0;
    m_nm_sequence = 1;
    m_path_length = 0.0;
    m_bVisible = true;
    m_bODPointsVisible = true;
    m_bListed = true;
    m_bPathManagerBlink = false;
    m_bPathPropertiesBlink = false;
    m_bDeleteOnArrival = false;
    m_width = STYLE_UNDEFINED;
    m_style = wxPENSTYLE_INVALID;
    m_hiliteWidth = 0;
    m_bSaveUpdates = true;

    m_pODPointList = new ODPointList;
    m_pLastAddedPoint = NULL;
    m_pFirstAddedPoint = NULL;
    m_GUID = GetUUID();
    m_bTemporary = false;
    
    m_bNeedsUpdateBBox = true;
    RBBox.Set(0.0, 0.0, 0.0, 0.0);
    m_bcrosses_idl = false;

    m_LayerID = 0;
    m_bIsInLayer = false;

    m_ColourScheme = PI_GLOBAL_COLOR_SCHEME_RGB;
    m_wxcActiveLineColour = g_colourActivePathLineColour;
    m_wxcInActiveLineColour = g_colourInActivePathLineColour;
    CreateColourSchemes();
    SetColourScheme(g_global_color_scheme);
    SetActiveColours();
    
    m_lastMousePointIndex = 0;
    m_NextLegGreatCircle = false;
    
    m_HyperlinkList = new HyperlinkList;
}

ODPath::~ODPath( void )
{
    m_pODPointList->DeleteContents( false );            // do not delete Marks
    m_pODPointList->Clear();
    delete m_pODPointList;
    m_HyperlinkList->Clear();
    delete m_HyperlinkList;
}

void ODPath::CreateColourSchemes(void)
{
    m_wxcActiveLineColourRGB = m_wxcActiveLineColour;
    m_wxcInActiveLineColourRGB = m_wxcInActiveLineColour;
    m_wxcActiveLineColourDay = m_wxcActiveLineColour;
    m_wxcInActiveLineColourDay = m_wxcInActiveLineColour;
    m_wxcActiveLineColourDusk.Set( m_wxcActiveLineColour.Red()/2, m_wxcActiveLineColour.Green()/2, m_wxcActiveLineColour.Blue()/2, m_wxcActiveLineColour.Alpha());
    m_wxcInActiveLineColourDusk.Set( m_wxcInActiveLineColour.Red()/2, m_wxcInActiveLineColour.Green()/2, m_wxcInActiveLineColour.Blue()/2, m_wxcInActiveLineColour.Alpha());
    m_wxcActiveLineColourNight.Set( m_wxcActiveLineColour.Red()/4, m_wxcActiveLineColour.Green()/4, m_wxcActiveLineColour.Blue()/4, m_wxcActiveLineColour.Alpha());
    m_wxcInActiveLineColourNight.Set( m_wxcInActiveLineColour.Red()/4, m_wxcInActiveLineColour.Green()/4, m_wxcInActiveLineColour.Blue()/4, m_wxcInActiveLineColour.Alpha());
}

void ODPath::AddPoint( ODPoint *pNewPoint, bool b_rename_in_sequence, bool b_deferBoxCalc, bool b_isLoading )
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

void ODPath::AddTentativePoint( const wxString& GUID )
{
    ODPointGUIDList.Add( GUID );
    return;
}

ODPoint *ODPath::GetPoint( int nWhichPoint )
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

ODPoint *ODPath::GetPoint( const wxString &guid )
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

void ODPath::DrawPointWhich( ODDC& dc, int iPoint, wxPoint *odp)
{
    if( iPoint <= GetnPoints() )
        GetPoint( iPoint )->Draw( dc, odp);
}

void ODPath::DrawSegment( ODDC& dc, wxPoint *rp1, wxPoint *rp2, PlugIn_ViewPort &VP, bool bdraw_arrow )
{
    if( m_bPathIsSelected ) dc.SetPen( *g_pPathMan->GetSelectedPathPen() );
    else
        if( m_bPathIsActive ) dc.SetPen( *g_pPathMan->GetActivePathPen() );
        else
            dc.SetPen( *g_pPathMan->GetPathPen() );
    
    RenderSegment( dc, rp1->x, rp1->y, rp2->x, rp2->y, VP, bdraw_arrow );
}

void ODPath::DrawArcSegment( ODDC& dc, wxPoint *rpc, wxPoint *rp1, wxPoint *rp2, wxPoint *rp3, wxPoint *rp4, PlugIn_ViewPort &VP, bool bdraw_arrow )
{
    if( m_bPathIsSelected ) dc.SetPen( *g_pPathMan->GetSelectedPathPen() );
    else {
        if( m_bPathIsActive ) dc.SetPen( *g_pPathMan->GetActivePathPen() );
        else
            dc.SetPen( *g_pPathMan->GetPathPen() );
    }
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
        
    RenderArcSegment(dc, rpc, rp1, rp2, rp3, rp4, VP, false);
}

void ODPath::Draw( ODDC& dc, PlugIn_ViewPort &VP )
{
    wxString colour;
    wxPenStyle style = wxPENSTYLE_SOLID;
    int width = g_path_line_width;

    if( m_nPoints == 0 || !m_bVisible ) return;

    if( m_style != STYLE_UNDEFINED ) style = m_style;
    if( m_width != STYLE_UNDEFINED ) width = m_width;

    SetActiveColours();
    
    dc.SetPen( *wxThePenList->FindOrCreatePen( m_col, width, style ) );
    dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_col, wxBRUSHSTYLE_SOLID ) );

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
        GetCanvasPixLL( &VP, &ppt2,  pOp2->m_lat, pOp2->m_lon);
        m_bpts[ j++ ] = ( ppt2 );

        if ( m_bVisible )
        {
            //    Handle offscreen points
            LLBBox llbb;
            llbb.Set(VP.lat_min, VP.lon_min, VP.lat_max, VP.lon_max);
            bool b_2_on = llbb.Contains( pOp2->m_lat, pOp2->m_lon );
            bool b_1_on = llbb.Contains( pOp1->m_lat, pOp1->m_lon );

            //Simple case
            if( b_1_on && b_2_on ) RenderSegment( dc, ppt1.x, ppt1.y, ppt2.x, ppt2.y, VP, m_bDrawArrow, m_hiliteWidth ); // with no arrows

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

                RenderSegment( dc, ppt1.x, ppt1.y, ppt2.x + adder, ppt2.y, VP, m_bDrawArrow, m_hiliteWidth );
            } else
                if( !b_1_on ) {
                    if( ppt1.x < ppt2.x ) adder = (int) pix_full_circle;
                    else
                        adder = -(int) pix_full_circle;
                    
                    float rxd = ppt2.x - ( ppt1.x + adder );
                    float ryd = ppt1.y - ppt2.y;
                    dtest = rxd*rxd + ryd*ryd;
                    
                    if( dp < dtest ) adder = 0;

                    RenderSegment( dc, ppt1.x + adder, ppt1.y, ppt2.x, ppt2.y, VP, m_bDrawArrow, m_hiliteWidth );
                }
        }
        
        ppt1 = ppt2;
        pOp1 = pOp2;

        node = node->GetNext();
    }
    
    for(wxODPointListNode *node  = m_pODPointList->GetFirst(); node; node = node->GetNext()) {
        ODPoint *pOp = node->GetData();
        wxPoint r;
        GetCanvasPixLL( &VP, &r, pOp->m_lat, pOp->m_lon );
        if ( m_bVisible || pOp->m_bKeepXPath )
            pOp->Draw( dc, &r );
    }        
    wxDELETEA( m_bpts );
}

void ODPath::DrawGL( PlugIn_ViewPort &piVP )
{
#ifdef ocpnUSE_GL
    if( m_nPoints < 1 || !m_bVisible ) return;

    ODDC dc;
    
    /* determine color and width */
    wxPenStyle style = wxPENSTYLE_SOLID;
    int width = g_path_line_width;
    
    if( m_style != STYLE_UNDEFINED ) style = m_style;
    if( m_width != STYLE_UNDEFINED ) width = m_width;
    
    SetActiveColours();

    int j = 0;
    wxPoint r;

    m_bpts = new wxPoint[ m_pODPointList->GetCount() ];
    for(wxODPointListNode *node = m_pODPointList->GetFirst(); node; node = node->GetNext()) {
        ODPoint *pOp = node->GetData();
        GetCanvasPixLL( &piVP, &r, pOp->m_lat, pOp->m_lon );
        m_bpts[ j++ ] = r;
    }
    
    dc.SetPen( *wxThePenList->FindOrCreatePen( m_col, width, style ) );
    dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_col, wxBRUSHSTYLE_TRANSPARENT ) );
    dc.SetGLStipple();
    
    for( size_t i = 1; i < m_pODPointList->GetCount(); i++ ){
        dc.DrawLine(m_bpts[i-1].x, m_bpts[i - 1].y, m_bpts[i].x, m_bpts[i].y);
    }
    
    glDisable( GL_LINE_STIPPLE );

    dc.SetBrush( *wxTheBrushList->FindOrCreateBrush( m_col, wxBRUSHSTYLE_SOLID ) );
    
    for(size_t i = 1; i < m_pODPointList->GetCount(); i++) {
        if(m_bDrawArrow)
            RenderSegmentArrowsGL( m_bpts[i - 1].x, m_bpts[i - 1].y, m_bpts[i].x, m_bpts[i].y, piVP );
    }
    
    /*  ODPoints  */
    for(wxODPointListNode *node = m_pODPointList->GetFirst(); node; node = node->GetNext()) {
    ODPoint *pOp = node->GetData();
    if ( m_bVisible || pOp->m_bKeepXPath )
        pOp->DrawGL( piVP );
    }   
    
    wxDELETEA( m_bpts );
#endif
}

static int s_arrow_icon[] = { 0, 0, 5, 2, 18, 6, 12, 0, 18, -6, 5, -2, 0, 0 };

void ODPath::RenderSegment( ODDC& dc, int xa, int ya, int xb, int yb, PlugIn_ViewPort &VP,
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

            wxPen HiPen( hilt, hilite_width, wxPENSTYLE_SOLID );

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

        int xc, yc; // move the pointer back from the icon
        xc = xb - ( ( xb - xa ) / 20 );
        yc = yb - ( ( yb - ya ) / 20 );
        for( int i = 0; i < 7; i++ ) {
            int j = i * 2;
            double pxa = (double) ( s_arrow_icon[j] );
            double pya = (double) ( s_arrow_icon[j + 1] );

            pya *= icon_scale_factor;
            pxa *= icon_scale_factor;

            double px = ( pxa * sin( theta ) ) + ( pya * cos( theta ) );
            double py = ( pya * sin( theta ) ) - ( pxa * cos( theta ) );

            icon[i].x = (int) ( px ) + xc;
            icon[i].y = (int) ( py ) + yc;
        }
        wxPen savePen = dc.GetPen();
        dc.SetPen( *wxTRANSPARENT_PEN );
        dc.StrokePolygon( 6, &icon[0], 0, 0 );
        dc.SetPen( savePen );
    }
}

void ODPath::RenderArcSegment( ODDC& dc, wxPoint *rpc, wxPoint *rp1, wxPoint *rp2, wxPoint *rp3, wxPoint *rp4, PlugIn_ViewPort &VP, bool bdraw_arrow ) 
{
    RenderArcSegment( dc, rpc->x, rpc->y, rp1->x, rp1->y, rp2->x, rp2->y, rp3->x, rp3->y, rp4->x, rp4->y, VP, bdraw_arrow );
}

void ODPath::RenderArcSegment( ODDC& dc, int centre_x, int centre_y, int xa, int ya, int xb, int yb, int xc, int yc, int xd, int yd, PlugIn_ViewPort &VP,
                            bool bdraw_arrow, int hilite_width )
{
    //    Get the dc boundary
    int sx, sy;
    sx = VP.pix_width;
    sy = VP.pix_height;
    
    //    Try to exit early if the segment is nowhere near the screen
    wxRect r( 0, 0, sx, sy );
    wxRect w( centre_x, centre_y, 1, 1 );
    // These may causes the circular objects not to display early enough
    wxRect s( xa, ya, 1, 1 );
    wxRect t( xb, yb, 1, 1 );
    wxRect u( xc, yc, 1, 1 );
    wxRect v( xd, yd, 1, 1 );
    w.Union( s );
    w.Union( t );
    w.Union( u );
    w.Union( v );
    if( !r.Intersects( w ) ) {
        return;
    }
    
    //    If hilite is desired, use a Native Graphics context to render alpha colours
    //    That is, if wxGraphicsContext is available.....
    
    if( hilite_width ) {
        wxPen psave = dc.GetPen();
        
        wxColour y;
        GetGlobalColor( wxS( "YELO1" ), &y );
        wxColour hilt( y.Red(), y.Green(), y.Blue(), 128 );
        
        wxPen HiPen( hilt, hilite_width, wxPENSTYLE_SOLID );
        
        dc.SetPen( HiPen );
        dc.StrokeSector( centre_x, centre_y, xa, ya, xb, yb, xc, yc, xd, yd );
        
        dc.SetPen( psave );
        dc.StrokeSector( centre_x, centre_y, xa, ya, xb, yb, xc, yc, xd, yd );
    } else {
        dc.StrokeSector( centre_x, centre_y, xa, ya, xb, yb, xc, yc, xd, yd );
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
        
        int xc, yc; // move the pointer back from the icon
        xc = xb - ( ( xb - xa ) / 20 );
        yc = yb - ( ( yb - ya ) / 20 );
        for( int i = 0; i < 7; i++ ) {
            int j = i * 2;
            double pxa = (double) ( s_arrow_icon[j] );
            double pya = (double) ( s_arrow_icon[j + 1] );
            
            pya *= icon_scale_factor;
            pxa *= icon_scale_factor;
            
            double px = ( pxa * sin( theta ) ) + ( pya * cos( theta ) );
            double py = ( pya * sin( theta ) ) - ( pxa * cos( theta ) );
            
            icon[i].x = (int) ( px ) + xc;
            icon[i].y = (int) ( py ) + yc;
        }
        wxPen savePen = dc.GetPen();
        dc.SetPen( *wxTRANSPARENT_PEN );
        dc.StrokePolygon( 6, &icon[0], 0, 0 );
        dc.SetPen( savePen );
    }
}

void ODPath::RenderSegmentArrowsGL( int xa, int ya, int xb, int yb, PlugIn_ViewPort &VP)
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
    int xc, yc; // move the pointer back from the icon
    xc = xb - ( ( xb - xa ) / 20 );
    yc = yb - ( ( yb - ya ) / 20 );
    glTranslatef(xc, yc, 0);
    glScalef(icon_scale_factor, icon_scale_factor, 1);
    glRotatef(theta * 180/PI, 0, 0, 1);

    glBegin(GL_POLYGON);
    for( int i = 0; i < 14; i+=2 )
        glVertex2f(s_arrow_icon[i], s_arrow_icon[i+1]);
    glEnd();

    glPopMatrix();
#endif
}

void ODPath::ClearHighlights( void )
{
    ODPoint *pop = NULL;
    wxODPointListNode *node = m_pODPointList->GetFirst();

    while( node ) {
        pop = node->GetData();
        if( pop ) pop->m_bPtIsSelected = false;
        node = node->GetNext();
    }
}

wxString ODPath::GetNewMarkSequenced( void )
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

ODPoint *ODPath::GetLastPoint()
{
    ODPoint *data_m1 = NULL;
    wxODPointListNode *node = m_pODPointList->GetFirst();

    while( node ) {
        data_m1 = node->GetData();
        node = node->GetNext();
    }
    return ( data_m1 );
}

int ODPath::GetIndexOf( ODPoint *pOp )
{
    int ret = m_pODPointList->IndexOf( pOp ) + 1;
    if( ret == wxNOT_FOUND ) return 0;
    else
        return ret;

}

void ODPath::DeletePoint( ODPoint *rp, bool bRenamePoints )
{
    //    n.b. must delete Selectables  and update config before deleting the point
    if( rp->m_bIsInLayer ) return;

    g_pODSelect->DeleteAllSelectableODPoints( (ODPath *) this );
    g_pODSelect->DeleteAllSelectablePathSegments( (ODPath *) this );
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

void ODPath::RemovePoint( ODPoint *op, bool bRenamePoints )
{
    g_pODSelect->DeleteAllSelectableODPoints( this );
    g_pODSelect->DeleteAllSelectablePathSegments( this );

    m_pODPointList->DeleteObject( op );
    if( wxNOT_FOUND != ODPointGUIDList.Index( op->m_GUID ) ) ODPointGUIDList.Remove(
            op->m_GUID );
    m_nPoints -= 1;

    // check all other paths to see if this point appears in any other path
    ODPath *pcontainer_path = g_pPathMan->FindPathContainingODPoint( op );

    if( pcontainer_path == NULL ) {
        op->m_bIsInPath = false;          // Take this point out of this (and only) path
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

void ODPath::DeSelectPath()
{
    wxODPointListNode *node = m_pODPointList->GetFirst();

    ODPoint *rp;
    while( node ) {
        rp = node->GetData();
        rp->m_bPtIsSelected = false;

        node = node->GetNext();
    }
}

void ODPath::ReloadPathPointIcons()
{
    wxODPointListNode *node = m_pODPointList->GetFirst();

    ODPoint *op;
    while( node ) {
        op = node->GetData();
        op->ReLoadIcon();

        node = node->GetNext();
    }
}

void ODPath::FinalizeForRendering()
{
    m_bNeedsUpdateBBox = true;
}

LLBBox ODPath::GetBBox( void )
{
    if(!m_bNeedsUpdateBBox)
        return RBBox;

    double bbox_xmin = 180.;                        // set defaults
    double bbox_ymin = 90.;
    double bbox_xmax = -180;
    double bbox_ymax = -90.;

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

    RBBox.Set(bbox_ymin, bbox_xmin, bbox_ymax, bbox_xmax);

    m_bNeedsUpdateBBox = false;
    return RBBox;
}

bool ODPath::CalculateCrossesIDL( void )
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

void ODPath::CalculateDCRect( wxDC& dc_boundary, wxRect *prect, PlugIn_ViewPort &VP )
{
    dc_boundary.ResetBoundingBox();
    dc_boundary.DestroyClippingRegion();
    
    wxRect update_rect;

    // Draw the path in skeleton form on the dc
    // That is, draw only the ODPoints, assuming that the segements will
    // always be fully contained within the resulting rectangle.
    // Can we prove this?
    if( m_bVisible ) {
        wxODPointListNode *node = m_pODPointList->GetFirst();
        while( node ) {

            ODPoint *pOp2 = node->GetData();
            bool pathpropblinksave = pOp2->m_bPathManagerBlink;
            bool pointpropblinksave = pOp2->m_bPointPropertiesBlink;
            pOp2->m_bPathManagerBlink = false;
            pOp2->m_bPointPropertiesBlink = false;
            ODDC odc_boundary( dc_boundary );
            pOp2->Draw( odc_boundary, NULL );
            pOp2->m_bPathManagerBlink = pathpropblinksave;
            pOp2->m_bPointPropertiesBlink = pointpropblinksave;

            wxRect r =  pOp2->CurrentRect_in_DC ;
            r.Inflate(m_hiliteWidth, m_hiliteWidth);        // allow for large hilite circles at segment ends
                
            update_rect.Union( r );
            node = node->GetNext();
        }
    }

    *prect = update_rect;
}

void ODPath::RebuildGUIDList( void )
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
void ODPath::SetVisible( bool visible, bool includeODPoints )
{
    m_bVisible = visible;

    wxODPointListNode *node = m_pODPointList->GetFirst();
    ODPoint *op;
    while( node ) {
        op = node->GetData();
        if ( (!op->m_bKeepXPath  || includeODPoints) && m_bODPointsVisible )
        {
            op->SetVisible( visible );
        }
        node = node->GetNext();
    }
}

void ODPath::SetListed( bool visible )
{
    m_bListed = visible;
}

void ODPath::AssemblePath( void )
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

void ODPath::RenameODPoints( void )
{
    //    iterate on the ODPoints.
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
bool ODPath::IsEqualTo( ODPath *ptargetpath )
{
    wxODPointListNode *pthisnode = ( this->m_pODPointList )->GetFirst();
    wxODPointListNode *pthatnode = ( ptargetpath->m_pODPointList )->GetFirst();

    if( NULL == pthisnode ) return false;

    if( this->m_bIsInLayer || ptargetpath->m_bIsInLayer ) return false;

    if( this->GetnPoints() != ptargetpath->GetnPoints() ) return false;

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
void ODPath::UpdateSegmentDistances()
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

            node = node->GetNext();
        }
    }

    m_path_length = path_len;
}

ODPoint *ODPath::InsertPointBefore( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    ODPoint *newpoint = new ODPoint( lat, lon, g_sODPointIconName, GetNewMarkSequenced(), wxT("") );
    newpoint->m_bIsInPath = true;
    newpoint->m_bDynamicName = true;
    newpoint->SetNameShown( false );
    newpoint->SetTypeString( wxT("Boundary Point") );
    
    int nOP = m_pODPointList->IndexOf( pOP );
    if ( nOP == 0 ) {
        m_pODPointList->Insert( m_pODPointList->GetCount() - 1, newpoint );
        nOP = m_pODPointList->GetCount();
    }
    else {
        m_pODPointList->Insert( nOP, newpoint );
    }

    ODPointGUIDList.Insert( pOP->m_GUID, nOP );

    m_nPoints++;

    if( bRenamePoints ) RenameODPoints();

    FinalizeForRendering();
    UpdateSegmentDistances();

    return ( newpoint );
}

ODPoint *ODPath::InsertPointAfter( ODPoint *pOP, double lat, double lon, bool bRenamePoints )
{
    int nOP = m_pODPointList->IndexOf( pOP );
    if( nOP >= m_nPoints - 1 )
        return NULL;
    nOP++;
    
    ODPoint *newpoint = new ODPoint( lat, lon, g_sODPointIconName, GetNewMarkSequenced(), wxT("") );
    newpoint->m_bIsInPath = true;
    newpoint->m_bDynamicName = true;
    newpoint->SetNameShown( false );
    newpoint->SetTypeString( wxT("OD Point") );
    
    m_pODPointList->Insert( nOP, newpoint );
    
    ODPointGUIDList.Insert( pOP->m_GUID, nOP );
    
    m_nPoints++;
    
    if( bRenamePoints ) RenameODPoints();
    
    FinalizeForRendering();
    UpdateSegmentDistances();
    
    return ( newpoint );
}

void ODPath::InsertPointAfter( ODPoint *pOP, ODPoint *pnOP, bool bRenamePoints )
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

void ODPath::RemovePointFromPath( ODPoint* point, ODPath* path )
{
    //  Rebuild the path selectables
    //g_pODSelect->DeleteAllSelectableODPoints( path );
    //g_pODSelect->DeleteAllSelectablePathSegments( path );

    path->RemovePoint( point );

    //  Check for 1 point paths. If we are creating a path, this is an undo, so keep the 1 point.
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

void ODPath::SetActiveColours( void )
{
    wxString colour;
    
    if( m_bVisible && m_bPathIsActive ) {
        if((m_bPathManagerBlink || m_bPathPropertiesBlink) && (g_ocpn_draw_pi->nBlinkerTick & 1))
            m_col= m_wxcSchemeInActiveLineColour;
        else
            m_col = m_wxcSchemeActiveLineColour;
    }
    else {
        if((m_bPathManagerBlink || m_bPathPropertiesBlink) && (g_ocpn_draw_pi->nBlinkerTick & 1))
            m_col= m_wxcSchemeActiveLineColour;
        else
            m_col = m_wxcSchemeInActiveLineColour;
    }
}

void ODPath::SetColourScheme(PI_ColorScheme cs)
{
    m_ColourScheme = cs;
    switch (cs) {
        case PI_GLOBAL_COLOR_SCHEME_RGB:
            m_wxcSchemeActiveLineColour = m_wxcActiveLineColourRGB;
            m_wxcSchemeInActiveLineColour = m_wxcInActiveLineColourRGB;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DAY:
            m_wxcSchemeActiveLineColour = m_wxcActiveLineColourDay;
            m_wxcSchemeInActiveLineColour = m_wxcInActiveLineColourDay;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DUSK:
            m_wxcSchemeActiveLineColour = m_wxcActiveLineColourDusk;
            m_wxcSchemeInActiveLineColour = m_wxcInActiveLineColourDusk;
            break;
        case PI_GLOBAL_COLOR_SCHEME_NIGHT:
            m_wxcSchemeActiveLineColour = m_wxcActiveLineColourNight;
            m_wxcSchemeInActiveLineColour = m_wxcInActiveLineColourNight;
            break;
        default:
            m_wxcSchemeActiveLineColour = m_wxcActiveLineColourDay;
            m_wxcSchemeInActiveLineColour = m_wxcInActiveLineColourDay;
            break;
    }

}

wxColour ODPath::GetCurrentColour(void)
{
    return m_col;
}


void ODPath::MoveAllPoints( double inc_lat, double inc_lon )
{
    wxODPointListNode *node = m_pODPointList->GetFirst();
    while(node) {
        ODPoint *op = (ODPoint *)node->GetData();
        op->m_lat -= inc_lat;
        op->m_lon -= inc_lon;
        node = node->GetNext();
    }
}

void ODPath::MoveSegment( double inc_lat, double inc_lon, ODPoint* firstPoint, ODPoint* secondPoint )
{
    firstPoint->m_lat -= inc_lat;
    firstPoint->m_lon -= inc_lon;
    secondPoint->m_lat -= inc_lat;
    secondPoint->m_lon -= inc_lon;
}

void ODPath::SetPointVisibility()
{
    for(wxODPointListNode *node = m_pODPointList->GetFirst(); node; node = node->GetNext()) {
        ODPoint *pOp = node->GetData();
        pOp->SetVisible( m_bODPointsVisible );
    }   
    
}

void ODPath::SetColours( ODPath *pPath )
{
    m_wxcActiveLineColour = pPath->m_wxcActiveLineColour; 
    m_wxcInActiveLineColour = pPath->m_wxcInActiveLineColour;
}
