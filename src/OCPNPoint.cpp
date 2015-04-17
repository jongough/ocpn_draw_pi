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

#include "wx/wxprec.h"

#include <wx/dcscreen.h>
#include <wx/tokenzr.h>

//#include "routeman.h"
//#include "chcanv.h"
#include "OCPNPoint.h"
#include "PointMan.h"
#include "PathMan.h"
#include "multiplexer.h"
#include "navutil.h"
#include "FontMgr.h"
#include "cutil.h"
#include "ocpn_draw_pi.h"

extern PointMan     *pOCPNPointMan;
extern bool         g_bODIsNewLayer;
extern int          g_ODLayerIdx;
extern ChartCanvas  *ocpncc1;
extern PathMan      *g_pPathMan;
extern PathList     *pPathList;
extern wxRect       g_blink_rect;
extern Multiplexer  *g_pODMUX;
extern bool         g_btouch;
extern bool         g_bresponsive;
extern ocpnStyle::StyleManager* g_ODStyleManager;
extern double       g_n_arrival_circle_radius;
extern int          g_iOCPNPointRangeRingsNumber;
extern float        g_fOCPNPointRangeRingsStep;
extern int          g_iOCPNPointRangeRingsStepUnits;
extern wxColour     g_colourOCPNPointRangeRingsColour;
extern PlugIn_ViewPort  *g_pivp;
extern ocpn_draw_pi     *g_ocpn_draw_pi;

#include <wx/listimpl.cpp>
WX_DEFINE_LIST ( OCPNPointList );

OCPNPoint::OCPNPoint()
{
    m_pbmIcon = NULL;

    //  Nice defaults
    m_seg_len = 0.0;
    m_seg_vmg = 0.0;
    m_seg_etd = wxInvalidDateTime;
    m_bDynamicName = false;
    m_bPtIsSelected = false;
    m_bIsBeingEdited = false;
    m_bIsActive = false;
    m_bBlink = false;
    m_bIsInRoute = false;
    m_bIsInPath = false;
    m_bIsInBoundary = false;
    m_bIsInTrack = false;
    m_CreateTimeX = wxDateTime::Now();
    m_GPXTrkSegNo = 1;
    m_bIsolatedMark = false;
    m_bShowName = true;
    m_bKeepXPath = false;
    m_bIsVisible = true;
    m_bIsListed = true;
    CurrentRect_in_DC = wxRect( 0, 0, 0, 0 );
    m_NameLocationOffsetX = -10;
    m_NameLocationOffsetY = 8;
    m_pMarkFont = NULL;
    m_btemp = false;
    m_SelectNode = NULL;
    m_ManagerNode = NULL;
    m_sTypeString = wxEmptyString;
    
    m_HyperlinkList = new HyperlinkList;

    m_GUID = pOCPNPointMan->CreateGUID( this );

    m_IconName = wxEmptyString;
    ReLoadIcon();

    m_MarkName = wxEmptyString;

    m_bIsInLayer = false;
    m_LayerID = 0;
    
    m_OCPNPointArrivalRadius = g_n_arrival_circle_radius;
}

// Copy Constructor
OCPNPoint::OCPNPoint( OCPNPoint* orig )
{
    m_MarkName = orig->GetName();
    m_lat = orig->m_lat;
    m_lon = orig->m_lon;
    m_seg_len = orig->m_seg_len;
    m_seg_vmg = orig->m_seg_vmg;
    m_seg_etd = orig->m_seg_etd;
    m_bDynamicName = orig->m_bDynamicName;
    m_bPtIsSelected = orig->m_bPtIsSelected;
    m_bIsBeingEdited = orig->m_bIsBeingEdited;
    m_bIsActive = orig->m_bIsActive;
    m_bBlink = orig->m_bBlink;
    m_bIsInRoute = orig->m_bIsInRoute;
    m_bIsInPath = orig->m_bIsInPath;
    m_bIsInBoundary = orig->m_bIsInBoundary;
    m_bIsInTrack = orig->m_bIsInTrack;
    m_CreateTimeX = orig->m_CreateTimeX;
    m_GPXTrkSegNo = orig->m_GPXTrkSegNo;
    m_bIsolatedMark = orig->m_bIsolatedMark;
    m_bShowName = orig->m_bShowName;
    m_bKeepXPath = orig->m_bKeepXPath;
    m_bIsVisible = orig->m_bIsVisible;
    m_bIsListed = orig->m_bIsListed;
    CurrentRect_in_DC = orig->CurrentRect_in_DC;
    m_NameLocationOffsetX = orig->m_NameLocationOffsetX;
    m_NameLocationOffsetY = orig->m_NameLocationOffsetY;
    m_pMarkFont = orig->m_pMarkFont;
    m_MarkDescription = orig->m_MarkDescription;
    m_btemp = orig->m_btemp;
    m_sTypeString = orig->m_sTypeString;

    m_HyperlinkList = new HyperlinkList;
    m_IconName = orig->m_IconName;
    ReLoadIcon();

    m_bIsInLayer = orig->m_bIsInLayer;
    m_GUID = pOCPNPointMan->CreateGUID( this );
    
    m_SelectNode = NULL;
    m_ManagerNode = NULL;
    
    m_OCPNPointArrivalRadius = orig->GetOCPNPointArrivalRadius();
    
}

OCPNPoint::OCPNPoint( double lat, double lon, const wxString& icon_ident, const wxString& name,
        const wxString &pGUID, bool bAddToList )
{
    //  Establish points
    m_lat = lat;
    m_lon = lon;

    //      Normalize the longitude, to fix any old poorly formed points
    if( m_lon < -180. ) m_lon += 360.;
    else
        if( m_lon > 180. ) m_lon -= 360.;

    //  Nice defaults
    m_seg_len = 0.0;
    m_seg_vmg = 0.0;
    m_seg_etd = wxInvalidDateTime;
    m_bDynamicName = false;
    m_bPtIsSelected = false;
    m_bIsBeingEdited = false;
    m_bIsActive = false;
    m_bBlink = false;
    m_bIsInRoute = false;
    m_bIsInPath = false;
    m_bIsInBoundary = false;
    m_bIsInTrack = false;
    m_CreateTimeX = wxDateTime::Now();
    m_GPXTrkSegNo = 1;
    m_bIsolatedMark = false;
    m_bShowName = true;
    m_bKeepXPath = false;
    m_bIsVisible = true;
    m_bIsListed = true;
    CurrentRect_in_DC = wxRect( 0, 0, 0, 0 );
    m_NameLocationOffsetX = -10;
    m_NameLocationOffsetY = 8;
    m_pMarkFont = NULL;
    m_btemp = false;
    m_sTypeString = wxEmptyString;

    m_SelectNode = NULL;
    m_ManagerNode = NULL;
    
    m_HyperlinkList = new HyperlinkList;

    if( !pGUID.IsEmpty() )
        m_GUID = pGUID;
    else
        m_GUID = pOCPNPointMan->CreateGUID( this );

    //      Get Icon bitmap
    m_IconName = icon_ident;
    ReLoadIcon();

    SetName( name );

    //  Possibly add the waypoint to the global list maintained by the waypoint manager

    if( bAddToList && NULL != pOCPNPointMan )
        pOCPNPointMan->AddOCPNPoint( this );

    m_bIsInLayer = g_bODIsNewLayer;
    if( m_bIsInLayer ) {
        m_LayerID = g_ODLayerIdx;
        m_bIsListed = false;
    } else
        m_LayerID = 0;
    
    SetOCPNPointArrivalRadius( g_n_arrival_circle_radius );

    m_bShowOCPNPointRangeRings = false;
    m_iOCPNPointRangeRingsNumber = g_iOCPNPointRangeRingsNumber;
    m_fOCPNPointRangeRingsStep = g_fOCPNPointRangeRingsStep;
    m_iOCPNPointRangeRingsStepUnits = g_iOCPNPointRangeRingsStepUnits;
    m_wxcOCPNPointRangeRingsColour = g_colourOCPNPointRangeRingsColour;
    
}

OCPNPoint::~OCPNPoint( void )
{
//  Remove this point from the global waypoint list
    if( NULL != pOCPNPointMan )
        pOCPNPointMan->RemoveOCPNPoint( this );

    if( m_HyperlinkList ) {
        m_HyperlinkList->DeleteContents( true );
        delete m_HyperlinkList;
    }
}

wxDateTime OCPNPoint::GetCreateTime()
{
    if(!m_CreateTimeX.IsValid()) {
        if(m_timestring.Len())
            ParseGPXDateTime( m_CreateTimeX, m_timestring );
    }
    return m_CreateTimeX;
}

void OCPNPoint::SetCreateTime( wxDateTime dt )
{
    m_CreateTimeX = dt;
}


void OCPNPoint::SetName(const wxString & name)
{
    m_MarkName = name;
    CalculateNameExtents();
}

void OCPNPoint::CalculateNameExtents( void )
{
    if( m_pMarkFont ) {
        wxScreenDC dc;

        dc.SetFont( *m_pMarkFont );
        m_NameExtents = dc.GetTextExtent( m_MarkName );
    } else
        m_NameExtents = wxSize( 0, 0 );

}

void OCPNPoint::ReLoadIcon( void )
{
    bool icon_exists = pOCPNPointMan->DoesIconExist(m_IconName);
    if( !icon_exists ){
        
        //  Try all lower case as a favor in the case where imported waypoints use mixed case names
        wxString tentative_icon = m_IconName.Lower();
        if(pOCPNPointMan->DoesIconExist(tentative_icon)){
            // if found, convert point's icon name permanently.
            m_IconName = tentative_icon;
        }
        //      Icon name is not in the standard or user lists, so add to the list a generic placeholder
        else{
            ocpnStyle::Style* style = g_ODStyleManager->GetCurrentStyle();
            if(style){
                wxBitmap bmp = style->GetIcon( _T("circle") );
                pOCPNPointMan->ProcessIcon( bmp, m_IconName, m_IconName );
           }
        }
    }
        
    m_pbmIcon = pOCPNPointMan->GetIconBitmap( m_IconName );

#ifdef ocpnUSE_GL
    m_wpBBox_chart_scale = -1;

    m_iTextTexture = 0;
#endif
}

void OCPNPoint::Draw( ocpnDC& dc, wxPoint *rpn )
{
    wxPoint r;
    wxRect hilitebox;

    GetCanvasPixLL( g_pivp, &r,  m_lat, m_lon);

    //  return the home point in this dc to allow "connect the dots"
    if( NULL != rpn ) *rpn = r;

    if( !m_bIsVisible /*&& !m_bIsInTrack*/)     // pjotrc 2010.02.13, 2011.02.24
        return;

    //    Optimization, especially apparent on tracks in normal cases
    if( m_IconName == _T("empty") && !m_bShowName && !m_bPtIsSelected ) return;

    wxPen *pen;
    if( m_bBlink ) pen = g_pPathMan->GetActiveOCPNPointPen();
    else pen = g_pPathMan->GetOCPNPointPen();

//    Substitue icon?
    wxBitmap *pbm;
    if( ( m_bIsActive ) && ( m_IconName != _T("mob") ) ) pbm = pOCPNPointMan->GetIconBitmap(
            _T ( "activepoint" ) );
    else
        pbm = m_pbmIcon;

    int sx2 = pbm->GetWidth() / 2;
    int sy2 = pbm->GetHeight() / 2;

//    Calculate the mark drawing extents
    wxRect r1( r.x - sx2, r.y - sy2, sx2 * 2, sy2 * 2 );           // the bitmap extents

    if( m_bShowName ) {
        if( 0 == m_pMarkFont ) {
            m_pMarkFont = OCPNGetFont( wxS("Marks"), 0 );
            m_FontColor = FontMgr::Get().GetFontColor( wxS( "Marks" ) );
            CalculateNameExtents();
        }

        if( m_pMarkFont ) {
            wxRect r2( r.x + m_NameLocationOffsetX, r.y + m_NameLocationOffsetY, m_NameExtents.x,
                    m_NameExtents.y );
            r1.Union( r2 );
        }
    }

    hilitebox = r1;
    hilitebox.x -= r.x;
    hilitebox.y -= r.y;
    float radius;
    if( g_btouch ){
        hilitebox.Inflate( 20 );
        radius = 20.0f;
    }
    else{
        hilitebox.Inflate( 4 );
        radius = 4.0f;
    }

    wxColour hi_colour;
    hi_colour = pen->GetColour();
    unsigned char transparency = 100;
    if( m_bIsBeingEdited ){
        GetGlobalColor( wxS( "YELO1" ), &hi_colour );
        transparency = 150;
    }
    
        
    //  Highlite any selected point
    if( m_bPtIsSelected || m_bIsBeingEdited) {
        g_ocpn_draw_pi->AlphaBlending( dc, r.x + hilitebox.x, r.y + hilitebox.y, hilitebox.width, hilitebox.height, radius,
                hi_colour, transparency );
    }

    bool bDrawHL = false;

    if( m_bBlink && ( g_ocpn_draw_pi->nBlinkerTick & 1 ) ) bDrawHL = true;

    if( ( !bDrawHL ) && ( NULL != m_pbmIcon ) ) {
        dc.DrawBitmap( *pbm, r.x - sx2, r.y - sy2, true );
        // on MSW, the dc Bounding box is not updated on DrawBitmap() method.
        // Do it explicitely here for all platforms.
        dc.CalcBoundingBox( r.x - sx2, r.y - sy2 );
        dc.CalcBoundingBox( r.x + sx2, r.y + sy2 );
    }

    if( m_bShowName ) {
        if( m_pMarkFont ) {
            dc.SetFont( *m_pMarkFont );
            dc.SetTextForeground( m_FontColor );

            dc.DrawText( m_MarkName, r.x + m_NameLocationOffsetX, r.y + m_NameLocationOffsetY );
        }
    }

    // Draw OCPNPoint range rings if activated
    if( m_iOCPNPointRangeRingsNumber && m_bShowOCPNPointRangeRings ) {
        double factor = 1.00;
        if( m_iOCPNPointRangeRingsStepUnits == 1 )          // nautical miles
            factor = 1 / 1.852;

        factor *= m_fOCPNPointRangeRingsStep;

        double tlat, tlon;
        wxPoint r1;
        ll_gc_ll( m_lat, m_lon, 0, factor, &tlat, &tlon );
        //cc1->GetCanvasPointPix( tlat, tlon, &r1 );
        GetCanvasPixLL( g_pivp, &r1,  tlat, tlon);

        double lpp = sqrt( pow( (double) (r.x - r1.x), 2) +
                           pow( (double) (r.y - r1.y), 2 ) );
        int pix_radius = (int) lpp;

        wxPen ppPen1( m_wxcOCPNPointRangeRingsColour, 2 );
        wxBrush saveBrush = dc.GetBrush();
        wxPen savePen = dc.GetPen();
        dc.SetPen( ppPen1 );
        dc.SetBrush( wxBrush( m_wxcOCPNPointRangeRingsColour, wxTRANSPARENT ) );

        for( int i = 1; i <= m_iOCPNPointRangeRingsNumber; i++ )
            dc.StrokeCircle( r.x, r.y, i * pix_radius );
        dc.SetPen( savePen );
        dc.SetBrush( saveBrush );
    }
    
    //  Save the current draw rectangle in the current DC
    //    This will be useful for fast icon redraws
    CurrentRect_in_DC.x = r.x + hilitebox.x;
    CurrentRect_in_DC.y = r.y + hilitebox.y;
    CurrentRect_in_DC.width = hilitebox.width;
    CurrentRect_in_DC.height = hilitebox.height;

    if( m_bBlink ) g_blink_rect = CurrentRect_in_DC;               // also save for global blinker

}

#ifdef ocpnUSE_GL
void OCPNPoint::DrawGL( PlugIn_ViewPort &pivp )
{
    if( !m_bIsVisible )
    return;

    //    Optimization, especially apparent on tracks in normal cases
    if( m_IconName == _T("empty") && !m_bShowName && !m_bPtIsSelected ) return;

    if(m_wpBBox.GetValid() &&
        pivp.chart_scale == m_wpBBox_chart_scale &&
        pivp.rotation == m_wpBBox_rotation) {
//       vp.chart_scale == m_wpBBox_chart_scale &&
//       vp.rotation == m_wpBBox_rotation) {
        /* see if this waypoint can intersect with bounding box */
//        LLBBox vpBBox = vp.GetBBox();
//        pivp.
/*        if( vpBBox.IntersectOut( m_wpBBox ) ) {
            /* try with vp crossing IDL */
/*            if(vpBBox.GetMinX() < -180 && vpBBox.GetMaxX() > -180) {
                wxPoint2DDouble xlate( -360., 0. );
                wxBoundingBox test_box2 = m_wpBBox;
                test_box2.Translate( xlate );
                if( vp.GetBBox().IntersectOut( test_box2 ) )
                    return;
            } else 
            if(vpBBox.GetMinX() < 180 && vpBBox.GetMaxX() > 180) {
                wxPoint2DDouble xlate( 360., 0. );
                wxBoundingBox test_box2 = m_wpBBox;
                test_box2.Translate( xlate );
                if( vp.GetBBox().IntersectOut( test_box2 ) )
                    return;
            } else
                return;
        }
*/        
    }

    wxPoint r;
    wxRect hilitebox;
    unsigned char transparency = 150;

    GetCanvasPixLL( g_pivp, &r, m_lat, m_lon );

//    Substitue icon?
    wxBitmap *pbm;
    if( ( m_bIsActive ) && ( m_IconName != _T("mob") ) )
        pbm = pOCPNPointMan->GetIconBitmap(  _T ( "activepoint" ) );
    else
        pbm = m_pbmIcon;

    int sx2 = pbm->GetWidth() / 2;
    int sy2 = pbm->GetHeight() / 2;

//    Calculate the mark drawing extents
    wxRect r1( r.x - sx2, r.y - sy2, sx2 * 2, sy2 * 2 );           // the bitmap extents

    wxRect r3 = r1;
    if( m_bShowName ) {
        if( !m_pMarkFont ) {
            m_pMarkFont = FontMgr::Get().GetFont( _( "Marks" ) );
            m_FontColor = FontMgr::Get().GetFontColor( _( "Marks" ) );
            CalculateNameExtents();
        }

        if( m_pMarkFont ) {
            wxRect r2( r.x + m_NameLocationOffsetX, r.y + m_NameLocationOffsetY,
                       m_NameExtents.x, m_NameExtents.y );
            r3.Union( r2 );
        }
    }

    hilitebox = r3;
    hilitebox.x -= r.x;
    hilitebox.y -= r.y;
    float radius;
    if( g_btouch ){
        hilitebox.Inflate( 20 );
        radius = 20.0f;
    }
    else{
        hilitebox.Inflate( 4 );
        radius = 4.0f;
    }
    
    /* update bounding box */
    if(!m_wpBBox.GetValid() || pivp.chart_scale != m_wpBBox_chart_scale || pivp.rotation != m_wpBBox_rotation) {
        double lat1, lon1, lat2, lon2;
        wxPoint wxpoint;
        wxpoint.x = r.x+hilitebox.x;
        wxpoint.y = r.y + hilitebox.height;
        GetCanvasLLPix( &pivp, wxpoint, &lat1, &lon1 );
        wxpoint.x = r.x + hilitebox.x + hilitebox.width;
        wxpoint.y = r.y + hilitebox.y;
        GetCanvasLLPix( &pivp, wxpoint, &lat2, &lon2 );

        m_wpBBox.SetMin(lon1, lat1);
        m_wpBBox.SetMax(lon2, lat2);
        m_wpBBox_chart_scale = pivp.chart_scale;
        m_wpBBox_rotation = pivp.rotation;
    }

    ocpnDC dc;

    //  Highlite any selected point
    if( m_bPtIsSelected ) {
        wxColour hi_colour;
        if( m_bBlink ){
            wxPen *pen = g_pPathMan->GetActiveOCPNPointPen();
            hi_colour = pen->GetColour();
        }
        else{
            GetGlobalColor( wxS( "YELO1" ), &hi_colour );
        }
        
        g_ocpn_draw_pi->AlphaBlending( dc, r.x + hilitebox.x, r.y + hilitebox.y, hilitebox.width, hilitebox.height, radius,
                       hi_colour, transparency );
    }
    
    bool bDrawHL = false;

    MyFrame *gFrame;
    if( m_bBlink && ( gFrame->nBlinkerTick & 1 ) ) bDrawHL = true;

    if( ( !bDrawHL ) && ( NULL != m_pbmIcon ) ) {
        int glw, glh;
        unsigned int IconTexture = pOCPNPointMan->GetIconTexture( pbm, glw, glh );
        
        glBindTexture(GL_TEXTURE_2D, IconTexture);
        
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        
        glColor3f(1, 1, 1);
        
        int x = r1.x, y = r1.y, w = r1.width, h = r1.height;
        float u = (float)w/glw, v = (float)h/glh;
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(u, 0); glVertex2f(x+w, y);
        glTexCoord2f(u, v); glVertex2f(x+w, y+h);
        glTexCoord2f(0, v); glVertex2f(x, y+h);
        glEnd();
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }

    if( m_bShowName && m_pMarkFont ) {
        int w = m_NameExtents.x, h = m_NameExtents.y;
        if(!m_iTextTexture && w && h) {
            wxBitmap tbm(w, h); /* render text on dc */
            wxMemoryDC dc;
            dc.SelectObject( tbm );               
            dc.SetBackground( wxBrush( *wxBLACK ) );
            dc.Clear();
            dc.SetFont( *m_pMarkFont );
            dc.SetTextForeground( *wxWHITE );
            dc.DrawText( m_MarkName, 0, 0);
            dc.SelectObject( wxNullBitmap );
            
            /* make alpha texture for text */
            wxImage image = tbm.ConvertToImage();
            unsigned char *d = image.GetData();
            unsigned char *e = new unsigned char[w * h];
            if(d && e){
                for( int p = 0; p < w*h; p++)
                    e[p] = d[3*p + 0];
            }
            
            /* create texture for rendered text */
            glGenTextures(1, &m_iTextTexture);
            glBindTexture(GL_TEXTURE_2D, m_iTextTexture);
            
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

            m_iTextTextureWidth = NextPow2(w);
            m_iTextTextureHeight = NextPow2(h);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_iTextTextureWidth, m_iTextTextureHeight,
                         0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h,
                            GL_ALPHA, GL_UNSIGNED_BYTE, e);
            delete [] e;
        }

        if(m_iTextTexture) {
            /* draw texture with text */
            glBindTexture(GL_TEXTURE_2D, m_iTextTexture);
            
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
            glColor3ub(m_FontColor.Red(), m_FontColor.Green(), m_FontColor.Blue());
            
            int x = r.x + m_NameLocationOffsetX, y = r.y + m_NameLocationOffsetY;
            float u = (float)w/m_iTextTextureWidth, v = (float)h/m_iTextTextureHeight;
            glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(x, y);
            glTexCoord2f(u, 0); glVertex2f(x+w, y);
            glTexCoord2f(u, v); glVertex2f(x+w, y+h);
            glTexCoord2f(0, v); glVertex2f(x, y+h);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
        }
    }
    
    // Draw OCPNPoint range rings if activated
    if( m_iOCPNPointRangeRingsNumber && m_bShowOCPNPointRangeRings ) {
        double factor = 1.00;
        if( m_iOCPNPointRangeRingsStepUnits == 1 )          // nautical miles
            factor = 1 / 1.852;
        
        factor *= m_fOCPNPointRangeRingsStep;
        
        double tlat, tlon;
        wxPoint r1;
        ll_gc_ll( m_lat, m_lon, 0, factor, &tlat, &tlon );
        GetCanvasPixLL( g_pivp, &r1,  tlat, tlon);
        
        double lpp = sqrt( pow( (double) (r.x - r1.x), 2) +
        pow( (double) (r.y - r1.y), 2 ) );
        int pix_radius = (int) lpp;
        
        wxPen ppPen1( m_wxcOCPNPointRangeRingsColour, 2 );
        wxBrush saveBrush = dc.GetBrush();
        wxPen savePen = dc.GetPen();
        dc.SetPen( ppPen1 );
        dc.SetBrush( wxBrush( m_wxcOCPNPointRangeRingsColour, wxTRANSPARENT ) );
        
        for( int i = 1; i <= m_iOCPNPointRangeRingsNumber; i++ )
            dc.StrokeCircle( r.x, r.y, i * pix_radius );
        dc.SetPen( savePen );
        dc.SetBrush( saveBrush );
    }
    
    if( m_bBlink ) g_blink_rect = CurrentRect_in_DC;               // also save for global blinker
    
    //    This will be useful for fast icon redraws
    CurrentRect_in_DC.x = r.x + hilitebox.x;
    CurrentRect_in_DC.y = r.y + hilitebox.y;
    CurrentRect_in_DC.width = hilitebox.width;
    CurrentRect_in_DC.height = hilitebox.height;

    if( m_bBlink ) g_blink_rect = CurrentRect_in_DC;               // also save for global blinker
}
#endif

void OCPNPoint::SetPosition( double lat, double lon )
{
    m_lat = lat;
    m_lon = lon;
}

void OCPNPoint::CalculateDCRect( wxDC& dc, wxRect *prect )
{
    dc.ResetBoundingBox();
    dc.DestroyClippingRegion();

    // Draw the mark on the dc
    ocpnDC odc( dc );
    Draw( odc, NULL );

    //  Retrieve the drawing extents
    prect->x = dc.MinX() - 1;
    prect->y = dc.MinY() - 1;
    prect->width = dc.MaxX() - dc.MinX() + 2; // Mouse Poop?
    prect->height = dc.MaxY() - dc.MinY() + 2;

}

bool OCPNPoint::IsSame( OCPNPoint *pOtherRP )
{
    bool IsSame = false;

    if( this->m_MarkName == pOtherRP->m_MarkName ) {
        if( fabs( this->m_lat - pOtherRP->m_lat ) < 1.e-6
                && fabs( this->m_lon - pOtherRP->m_lon ) < 1.e-6 ) IsSame = true;
    }
    return IsSame;
}

bool OCPNPoint::SendToGPS(const wxString & com_name, wxGauge *pProgress)
{
  /*
    int result = 0;
    if( g_pODMUX )
        result = g_pODMUX->SendointToGPS( this, com_name, pProgress );

    wxString msg;
    if( 0 == result )
        msg = _("Waypoint(s) Transmitted.");
    else{
        if( result == ERR_GARMIN_INITIALIZE )
            msg = _("Error on Waypoint Upload.  Garmin GPS not connected");
        else               
            msg = _("Error on Waypoint Upload.  Please check logfiles...");
    }

    OCPNMessageBox( NULL, msg, _("OpenCPN Info"), wxOK | wxICON_INFORMATION );

    return (result == 0);
    */
    return TRUE;
}

double OCPNPoint::GetOCPNPointArrivalRadius() {
    if (m_OCPNPointArrivalRadius < 0.001) {
        SetOCPNPointArrivalRadius( g_n_arrival_circle_radius );
        return g_n_arrival_circle_radius;
    }
    else
        return m_OCPNPointArrivalRadius;
}

int   OCPNPoint::GetOCPNPointRangeRingsNumber() { 
    if ( m_iOCPNPointRangeRingsNumber == -1 )
        return g_iOCPNPointRangeRingsNumber;
    else
        return m_iOCPNPointRangeRingsNumber; 
}

float OCPNPoint::GetOCPNPointRangeRingsStep() { 
    if ( m_fOCPNPointRangeRingsStep == -1 )
        return g_fOCPNPointRangeRingsStep;
    else
        return m_fOCPNPointRangeRingsStep; 
}

int   OCPNPoint::GetOCPNPointRangeRingsStepUnits() { 
    if ( m_iOCPNPointRangeRingsStepUnits == -1 )
        return g_iOCPNPointRangeRingsStepUnits;
    else
        return m_iOCPNPointRangeRingsStepUnits ; 
}

wxColour OCPNPoint::GetOCPNPointRangeRingsColour(void) { 
    if ( m_wxcOCPNPointRangeRingsColour.GetAsString(wxC2S_HTML_SYNTAX) == _T("#FFFFFF") )
        return g_colourOCPNPointRangeRingsColour;
    else
        return m_wxcOCPNPointRangeRingsColour; 
}

//-------------------------------------------------------------------------
//
//          Static GPX Support Routines
//
//-------------------------------------------------------------------------
bool OCPNPointIsInRouteList( OCPNPoint *pr )
{
    bool IsInList = false;

    wxPathListNode *node1 = pPathList->GetFirst();
    while( node1 ) {
        Path *pPath = node1->GetData();
        OCPNPointList *pOCPNPointList = pPath->pOCPNPointList;

        wxOCPNPointListNode *node2 = pOCPNPointList->GetFirst();
        OCPNPoint *prp;

        while( node2 ) {
            prp = node2->GetData();

            if( pr->IsSame( prp ) ) {
                IsInList = true;
                break;
            }

            node2 = node2->GetNext();
        }
        node1 = node1->GetNext();
    }
    return IsInList;
}

// This function parses a string containing a GPX time representation
// and returns a wxDateTime containing the UTC corresponding to the
// input. The function return value is a pointer past the last valid
// character parsed (if successful) or NULL (if the string is invalid).
//
// Valid GPX time strings are in ISO 8601 format as follows:
//
//   [-]<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>Z|(+|-<hh>:<mm>)
//
// For example, 2010-10-30T14:34:56Z and 2010-10-30T14:34:56-04:00
// are the same time. The first is UTC and the second is EDT.

const wxChar *ParseGPXDateTime( wxDateTime &dt, const wxChar *datetime )
{
    long sign, hrs_west, mins_west;
    const wxChar *end;

    // Skip any leading whitespace
    while( isspace( *datetime ) )
        datetime++;

    // Skip (and ignore) leading hyphen
    if( *datetime == wxT('-') ) datetime++;

    // Parse and validate ISO 8601 date/time string
    if( ( end = dt.ParseFormat( datetime, wxT("%Y-%m-%dT%T") ) ) != NULL ) {

        // Invalid date/time
        if( *end == 0 ) return NULL;

        // ParseFormat outputs in UTC if the controlling
        // wxDateTime class instance has not been initialized.

        // Date/time followed by UTC time zone flag, so we are done
        else
            if( *end == wxT('Z') ) {
                end++;
                return end;
            }

            // Date/time followed by given number of hrs/mins west of UTC
            else
                if( *end == wxT('+') || *end == wxT('-') ) {

                    // Save direction from UTC
                    if( *end == wxT('+') ) sign = 1;
                    else
                        sign = -1;
                    end++;

                    // Parse hrs west of UTC
                    if( isdigit( *end ) && isdigit( *( end + 1 ) ) && *( end + 2 ) == wxT(':') ) {

                        // Extract and validate hrs west of UTC
                        wxString( end ).ToLong( &hrs_west );
                        if( hrs_west > 12 ) return NULL;
                        end += 3;

                        // Parse mins west of UTC
                        if( isdigit( *end ) && isdigit( *( end + 1 ) ) ) {

                            // Extract and validate mins west of UTC
                            wxChar mins[3];
                            mins[0] = *end;
                            mins[1] = *( end + 1 );
                            mins[2] = 0;
                            wxString( mins ).ToLong( &mins_west );
                            if( mins_west > 59 ) return NULL;

                            // Apply correction
                            dt -= sign * wxTimeSpan( hrs_west, mins_west, 0, 0 );
                            return end + 2;
                        } else
                            // Missing mins digits
                            return NULL;
                    } else
                        // Missing hrs digits or colon
                        return NULL;
                } else
                    // Unknown field after date/time (not UTC, not hrs/mins
                    //  west of UTC)
                    return NULL;
    } else
        // Invalid ISO 8601 date/time
        return NULL;
}

