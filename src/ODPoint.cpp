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

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "ODPoint.h"
#include "PointMan.h"
#include "PathMan.h"
#include "cutil.h"
#include "ocpn_draw_pi.h"
#include "ODUtils.h"
#include "ODdc.h"

#include "GL/gl.h"

#include <wx/listimpl.cpp>
WX_DEFINE_LIST ( ODPointList );

ODPoint::ODPoint()
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
    m_bPointPropertiesBlink = false;
    m_bPathManagerBlink = false;
    m_bIsInPath = false;
    m_bSingleUse = false;
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
    m_bTemporary = false;
    m_SelectNode = NULL;
    m_ManagerNode = NULL;
    m_fIconScaleFactor = 1.0;
    m_sTypeString = wxEmptyString;
    
    m_HyperlinkList = new HyperlinkList;

    m_GUID = GetUUID();

    m_IconName = wxEmptyString;
    ReLoadIcon();

    m_ODPointName = wxEmptyString;

    m_bIsInLayer = false;
    m_LayerID = 0;
    
    m_ODPointArrivalRadius = g_dODPointArrivalCircleRadius;
    
    m_bShowODPointRangeRings = g_bODPointShowRangeRings;
    m_iODPointRangeRingsNumber = g_iODPointRangeRingsNumber;
    m_fODPointRangeRingsStep = g_fODPointRangeRingsStep;
    m_iODPointRangeRingsStepUnits = g_iODPointRangeRingsStepUnits;
    m_wxcODPointRangeRingsColour = g_colourODPointRangeRingsColour;
    m_iRangeRingStyle = wxPENSTYLE_SOLID;
    m_iRangeRingWidth = 2;
    
    CreateColourSchemes();
    SetColourScheme(g_global_color_scheme);

}

// Copy Constructor
ODPoint::ODPoint( ODPoint* orig )
{
    m_ODPointName = orig->GetName();
    m_lat = orig->m_lat;
    m_lon = orig->m_lon;
    m_seg_len = orig->m_seg_len;
    m_seg_vmg = orig->m_seg_vmg;
    m_seg_etd = orig->m_seg_etd;
    m_bDynamicName = orig->m_bDynamicName;
    m_bPtIsSelected = orig->m_bPtIsSelected;
    m_bIsBeingEdited = orig->m_bIsBeingEdited;
    m_bIsActive = orig->m_bIsActive;
    m_bPointPropertiesBlink = orig->m_bPointPropertiesBlink;
    m_bPathManagerBlink = orig->m_bPathManagerBlink;
    m_bIsInPath = orig->m_bIsInPath;
    m_bSingleUse = orig->m_bSingleUse;
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
    m_ODPointDescription = orig->m_ODPointDescription;
    m_bTemporary = orig->m_bTemporary;
    m_sTypeString = orig->m_sTypeString;

    m_HyperlinkList = new HyperlinkList;
    m_IconName = orig->m_IconName;
    ReLoadIcon();

    m_bIsInLayer = orig->m_bIsInLayer;
    m_GUID = GetUUID();
    
    m_SelectNode = NULL;
    m_ManagerNode = NULL;
    m_fIconScaleFactor = 1.0;
    
    m_ODPointArrivalRadius = orig->GetODPointArrivalRadius();
    
    m_bShowODPointRangeRings = orig->m_bShowODPointRangeRings;
    m_iODPointRangeRingsNumber = orig->m_iODPointRangeRingsNumber;
    m_fODPointRangeRingsStep = orig->m_fODPointRangeRingsStep;
    m_iODPointRangeRingsStepUnits = orig->m_iODPointRangeRingsStepUnits;
    m_wxcODPointRangeRingsColour = orig->m_wxcODPointRangeRingsColour;
    m_iRangeRingStyle = wxPENSTYLE_SOLID;
    m_iRangeRingWidth = 2;
    SetRangeRingBBox();

    CreateColourSchemes();
    SetColourScheme(g_global_color_scheme);

}

ODPoint::ODPoint( double lat, double lon, const wxString& icon_ident, const wxString& name,
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
    m_bPointPropertiesBlink = false;
    m_bPathManagerBlink = false;
    m_bIsInPath = false;
    m_bSingleUse = false;
    m_CreateTimeX = wxDateTime::Now();
    m_GPXTrkSegNo = 1;
    m_bIsolatedMark = false;
    m_bShowName = false;
    m_bKeepXPath = false;
    m_bIsVisible = true;
    m_bIsListed = true;
    CurrentRect_in_DC = wxRect( 0, 0, 0, 0 );
    m_NameLocationOffsetX = -10;
    m_NameLocationOffsetY = 8;
    m_pMarkFont = NULL;
    m_bTemporary = false;
    m_sTypeString = wxEmptyString;

    m_SelectNode = NULL;
    m_ManagerNode = NULL;
    m_fIconScaleFactor = 1.0;
    
    m_HyperlinkList = new HyperlinkList;

    if( !pGUID.IsEmpty() )
        m_GUID = pGUID;
    else
        m_GUID = GetUUID();

    //      Get Icon bitmap
    m_IconName = icon_ident;
    ReLoadIcon();

    SetName( name );

    //  Possibly add the ODPoint to the global list maintained by the ODPoint manager

    if( bAddToList && NULL != g_pODPointMan ) {
        g_pODPointMan->AddODPoint( this );
    }

    m_bIsInLayer = g_bODIsNewLayer;
    if( m_bIsInLayer ) {
        m_LayerID = g_ODLayerIdx;
        m_bIsListed = false;
    } else
        m_LayerID = 0;
    
    SetODPointArrivalRadius( g_dODPointArrivalCircleRadius );

    m_bShowODPointRangeRings = g_bODPointShowRangeRings;
    m_iODPointRangeRingsNumber = g_iODPointRangeRingsNumber;
    m_fODPointRangeRingsStep = g_fODPointRangeRingsStep;
    m_iODPointRangeRingsStepUnits = g_iODPointRangeRingsStepUnits;
    m_wxcODPointRangeRingsColour = g_colourODPointRangeRingsColour;
    m_iRangeRingStyle = wxPENSTYLE_SOLID;
    m_iRangeRingWidth = 2;
    SetRangeRingBBox();
    
    CreateColourSchemes();
    SetColourScheme(g_global_color_scheme);
    
}

ODPoint::~ODPoint( void )
{
//  Remove this point from the global ODPoint list
    if( NULL != g_pODPointMan )
        g_pODPointMan->RemoveODPoint( this );

    if( m_HyperlinkList ) {
        m_HyperlinkList->DeleteContents( true );
        delete m_HyperlinkList;
    }
}

void ODPoint::CreateColourSchemes(void)
{
    m_wxcODPointRangeRingsColourRGB = m_wxcODPointRangeRingsColour;
    m_wxcODPointRangeRingsColourDay = m_wxcODPointRangeRingsColour;
    m_wxcODPointRangeRingsColourDusk.Set( m_wxcODPointRangeRingsColour.Red()/2, m_wxcODPointRangeRingsColour.Green()/2, m_wxcODPointRangeRingsColour.Blue()/2, m_wxcODPointRangeRingsColour.Alpha());
    m_wxcODPointRangeRingsColourNight.Set( m_wxcODPointRangeRingsColour.Red()/4, m_wxcODPointRangeRingsColour.Green()/4, m_wxcODPointRangeRingsColour.Blue()/4, m_wxcODPointRangeRingsColour.Alpha());
}

wxDateTime ODPoint::GetCreateTime()
{
    if(!m_CreateTimeX.IsValid()) {
        if(m_timestring.Len())
            ParseGPXDateTime( m_CreateTimeX, m_timestring );
    }
    return m_CreateTimeX;
}

void ODPoint::SetCreateTime( wxDateTime dt )
{
    m_CreateTimeX = dt;
}


void ODPoint::SetName(const wxString & name)
{
    m_ODPointName = name;
    CalculateNameExtents();
}

void ODPoint::CalculateNameExtents( void )
{
    if( m_pMarkFont ) {
        wxScreenDC dc;

        dc.SetFont( *m_pMarkFont );
        m_NameExtents = dc.GetTextExtent( m_ODPointName );
    } else
        m_NameExtents = wxSize( 0, 0 );

}

void ODPoint::ReLoadIcon( void )
{
    bool icon_exists = g_pODPointMan->DoesIconExist(m_IconName);
    if( !icon_exists ){
        
        //  Try all lower case as a favor in the case where imported ODPoints use mixed case names
        wxString tentative_icon = m_IconName.Lower();
        if(g_pODPointMan->DoesIconExist(tentative_icon)){
            // if found, convert point's icon name permanently.
            m_IconName = tentative_icon;
        }
        //      Icon name is not in the standard or user lists, so add to the list a generic placeholder
        else{
            //ocpnStyle::Style* style = g_ODStyleManager->GetCurrentStyle();
            //if(style){
                //wxBitmap bmp = style->GetIcon( _T("circle") );
                wxBitmap bmp = GetIcon_PlugIn( _T("circle") );
                g_pODPointMan->ProcessIcon( bmp, m_IconName, m_IconName );
           //}
        }
    }
        
    m_pbmIcon = g_pODPointMan->GetIconBitmap( m_IconName );

#ifdef ocpnUSE_GL
    m_wpBBox_chart_scale = -1;

    m_iTextTexture = 0;
#endif
    
    m_fIconScaleFactor = -1;
}

void ODPoint::Draw( ODDC& dc, wxPoint *odp)
{
    wxPoint l_odp;
    wxRect hilitebox;

    GetCanvasPixLL( &g_VP, &l_odp,  m_lat, m_lon);

    //  return the home point in this dc to allow "connect the dots"
    if( NULL != odp) *odp= l_odp;

    if( !m_bIsVisible /*&& !m_bIsInTrack*/)     // pjotrc 2010.02.13, 2011.02.24
        return;

    //    Optimization, especially apparent on tracks in normal cases
    if( m_IconName == _T("empty") && !m_bShowName && !m_bPtIsSelected ) return;

    wxPen *pen;
    if( m_bPointPropertiesBlink || m_bPathManagerBlink ) pen = g_pPathMan->GetActiveODPointPen();
    else pen = g_pPathMan->GetODPointPen();

//    Substitue icon?
    wxBitmap *pbm;
    if( ( m_bIsActive ) && ( m_IconName != _T("mob") ) ) pbm = g_pODPointMan->GetIconBitmap( _T ( "activepoint" ) );
    else
        pbm = m_pbmIcon;
    
    float l_ChartScaleFactorExp = GetOCPNChartScaleFactor_Plugin();
    if(m_fIconScaleFactor != l_ChartScaleFactorExp) {
        m_fIconScaleFactor = l_ChartScaleFactorExp;
        if(m_fIconScaleFactor != 0) {
            wxImage scaled_image = pbm->ConvertToImage();
            int new_width = pbm->GetWidth() * m_fIconScaleFactor;
            int new_height = pbm->GetHeight() * m_fIconScaleFactor;
            m_ScaledBMP = wxBitmap(scaled_image.Scale(new_width, new_height, wxIMAGE_QUALITY_HIGH));
        }
    }
    if( m_fIconScaleFactor != 0 && m_ScaledBMP.IsOk() )
        pbm = &m_ScaledBMP;
    
    int sx2 = pbm->GetWidth() / 2;
    int sy2 = pbm->GetHeight() / 2;

//    Calculate the mark drawing extents
    wxRect r1( l_odp.x - sx2, l_odp.y - sy2, sx2 * 2, sy2 * 2 );           // the bitmap extents

    if( m_bShowName ) {
        if( 0 == m_pMarkFont ) {
            m_pMarkFont = GetOCPNScaledFont_PlugIn( wxS("Marks") );
            m_FontColor = GetFontColour_PlugIn( wxS( "Marks" ) );
            CalculateNameExtents();
        }

        if( m_pMarkFont ) {
            wxRect r2( l_odp.x + (m_NameLocationOffsetX * m_fIconScaleFactor), l_odp.y + (m_NameLocationOffsetY * m_fIconScaleFactor), m_NameExtents.x,
                    m_NameExtents.y );
            r1.Union( r2 );
        }
    }

    hilitebox = r1;
    hilitebox.x -= l_odp.x;
    hilitebox.y -= l_odp.y;
    float radius;
    if( IsTouchInterface_PlugIn() ){
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
    if( m_bPtIsSelected || m_bIsBeingEdited ) {
        g_ocpn_draw_pi->AlphaBlending( dc, l_odp.x + hilitebox.x, l_odp.y + hilitebox.y, hilitebox.width, hilitebox.height, radius, hi_colour, transparency );
    }

    bool bDrawHL = false;

    if( (m_bPointPropertiesBlink || m_bPathManagerBlink) && ( g_ocpn_draw_pi->nBlinkerTick & 1 ) ) bDrawHL = true;

    if( ( !bDrawHL ) && ( NULL != m_pbmIcon ) ) {
        dc.DrawBitmap( *pbm, l_odp.x - sx2, l_odp.y - sy2, true );
        // on MSW, the dc Bounding box is not updated on DrawBitmap() method.
        // Do it explicitely here for all platforms.
        dc.CalcBoundingBox( l_odp.x - sx2, l_odp.y - sy2 );
        dc.CalcBoundingBox( l_odp.x + sx2, l_odp.y + sy2 );
    }

    if( m_bShowName ) {
        if( m_pMarkFont ) {
            dc.SetFont( *m_pMarkFont );
            dc.SetTextForeground( m_FontColor );

            dc.DrawText( m_ODPointName, l_odp.x + (m_NameLocationOffsetX * m_fIconScaleFactor), l_odp.y + (m_NameLocationOffsetY * m_fIconScaleFactor) );
        }
    }

    // Draw ODPoint range rings if activated
    if( m_iODPointRangeRingsNumber && m_bShowODPointRangeRings ) {
        double factor = 1.00;
        if( m_iODPointRangeRingsStepUnits == 1 )          // nautical miles
            factor = 1 / 1.852;

        factor *= m_fODPointRangeRingsStep;

        double tlat, tlon;
        wxPoint r1;
        ll_gc_ll( m_lat, m_lon, 0, factor, &tlat, &tlon );
        GetCanvasPixLL( &g_VP, &r1,  tlat, tlon);

        double lpp = sqrt( pow( (double) (l_odp.x - r1.x), 2) +
                           pow( (double) (l_odp.y - r1.y), 2 ) );
        int pix_radius = (int) lpp;

        wxBrush saveBrush = dc.GetBrush();
        wxPen savePen = dc.GetPen();
        dc.SetPen( wxPen( m_wxcODPointRangeRingsSchemeColour, m_iRangeRingWidth, m_iRangeRingStyle ) );
        dc.SetBrush( wxBrush( m_wxcODPointRangeRingsSchemeColour, wxBRUSHSTYLE_TRANSPARENT ) );

        for( int i = 1; i <= m_iODPointRangeRingsNumber; i++ )
            dc.StrokeCircle( l_odp.x, l_odp.y, i * pix_radius );
        dc.SetPen( savePen );
        dc.SetBrush( saveBrush );
    }
    
    //  Save the current draw rectangle in the current DC
    //    This will be useful for fast icon redraws
    CurrentRect_in_DC.x = l_odp.x + hilitebox.x;
    CurrentRect_in_DC.y = l_odp.y + hilitebox.y;
    CurrentRect_in_DC.width = hilitebox.width;
    CurrentRect_in_DC.height = hilitebox.height;

    if( m_bPointPropertiesBlink || m_bPathManagerBlink ) g_blink_rect = CurrentRect_in_DC;               // also save for global blinker

}

void ODPoint::DrawGL( PlugIn_ViewPort &pivp )
{
#ifdef ocpnUSE_GL
    if( !m_bIsVisible )
    return;

    //    Optimization, especially apparent on tracks in normal cases
    if( m_IconName == _T("empty") && !m_bShowName && !m_bPtIsSelected ) return;

    if(m_wpBBox.GetValid() &&
        pivp.chart_scale == m_wpBBox_chart_scale &&
        pivp.rotation == m_wpBBox_rotation) {
    }

    wxPoint l_odp;
    wxRect hilitebox;
    unsigned char transparency = 150;

    GetCanvasPixLL( &g_VP, &l_odp, m_lat, m_lon );

//    Substitue icon?
    wxBitmap *pbm;
    if( ( m_bIsActive ) && ( m_IconName != _T("mob") ) )
        pbm = g_pODPointMan->GetIconBitmap(  _T ( "activepoint" ) );
    else
        pbm = m_pbmIcon;

    int sx2 = pbm->GetWidth() / 2;
    int sy2 = pbm->GetHeight() / 2;

//    Calculate the mark drawing extents
    wxRect r1( l_odp.x - sx2, l_odp.y - sy2, sx2 * 2, sy2 * 2 );          // the bitmap extents
    
    float  l_fIconScaleFactor = GetOCPNChartScaleFactor_Plugin();
    wxRect r3 = r1;
    if( m_bShowName ) {
        if( !m_pMarkFont ) {
            m_pMarkFont = GetOCPNScaledFont_PlugIn( wxT( "Marks" ) );
            m_FontColor = GetFontColour_PlugIn( wxS( "Marks" ) );
            CalculateNameExtents();
        }

        if( m_pMarkFont ) {
            wxRect r2( l_odp.x + (m_NameLocationOffsetX * l_fIconScaleFactor), l_odp.y + (m_NameLocationOffsetY * l_fIconScaleFactor),
                       m_NameExtents.x, m_NameExtents.y );
            r3.Union( r2 );
        }
    }

    hilitebox = r3;
    hilitebox.x -= l_odp.x;
    hilitebox.y -= l_odp.y;
    float radius;
    if( IsTouchInterface_PlugIn() ){
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
        wxpoint.x = l_odp.x+hilitebox.x;
        wxpoint.y = l_odp.y + hilitebox.height;
        GetCanvasLLPix( &pivp, wxpoint, &lat1, &lon1 );
        wxpoint.x = l_odp.x + hilitebox.x + hilitebox.width;
        wxpoint.y = l_odp.y + hilitebox.y;
        GetCanvasLLPix( &pivp, wxpoint, &lat2, &lon2 );

        if(lon1 > lon2)
            m_wpBBox.Set(lat1, lon1, lat2, lon2+360);
        else
            m_wpBBox.Set(lat1, lon1, lat2, lon2);
        m_wpBBox_chart_scale = pivp.chart_scale;
        m_wpBBox_rotation = pivp.rotation;
    }

    ODDC dc;

    //  Highlite any selected point
    if( m_bPtIsSelected || m_bIsBeingEdited ) {
        wxColour hi_colour;
        if( m_bPointPropertiesBlink || m_bPathManagerBlink ){
            wxPen *pen = g_pPathMan->GetActiveODPointPen();
            hi_colour = pen->GetColour();
        }
        else{
            GetGlobalColor( wxS( "YELO1" ), &hi_colour );
        }
        
        g_ocpn_draw_pi->AlphaBlending( dc, l_odp.x + hilitebox.x, l_odp.y + hilitebox.y, hilitebox.width, hilitebox.height, radius,
                       hi_colour, transparency );
    }
    
    bool bDrawHL = false;

    if( (m_bPointPropertiesBlink || m_bPathManagerBlink) && ( g_ocpn_draw_pi->nBlinkerTick & 1 ) ) bDrawHL = true;

    if( ( !bDrawHL ) && ( NULL != m_pbmIcon ) ) {
        int glw, glh;
        unsigned int IconTexture = g_pODPointMan->GetIconTexture( pbm, glw, glh );
        
        glBindTexture(GL_TEXTURE_2D, IconTexture);
        
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        
        glColor3f(1, 1, 1);
        
        float l_ChartScaleFactorExp = GetOCPNChartScaleFactor_Plugin();
        float w = r1.width * l_ChartScaleFactorExp;
        float h = r1.height * l_ChartScaleFactorExp;
        float x = l_odp.x - w/2; 
        float y = l_odp.y - h/2;
        float u = (float)r1.width/glw, v = (float)r1.height/glh;
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
            dc.DrawText( m_ODPointName, 0, 0);
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
            
            int x = l_odp.x + (m_NameLocationOffsetX * l_fIconScaleFactor), y = l_odp.y + (m_NameLocationOffsetY * l_fIconScaleFactor);
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
    
    // Draw ODPoint range rings if activated
    if( m_iODPointRangeRingsNumber && m_bShowODPointRangeRings ) {
        double factor = 1.00;
        if( m_iODPointRangeRingsStepUnits == 1 )          // nautical miles
            factor = 1 / 1.852;
        
        factor *= m_fODPointRangeRingsStep;
        
        double tlat, tlon;
        wxPoint r1;
        ll_gc_ll( m_lat, m_lon, 0, factor, &tlat, &tlon );
        GetCanvasPixLL( &g_VP, &r1,  tlat, tlon);
        
        double lpp = sqrt( pow( (double) (l_odp.x - r1.x), 2) +
        pow( (double) (l_odp.y - r1.y), 2 ) );
        int pix_radius = (int) lpp;
        
        wxPen ppPen1( m_wxcODPointRangeRingsSchemeColour, m_iRangeRingWidth, m_iRangeRingStyle );
        wxBrush saveBrush = dc.GetBrush();
        wxPen savePen = dc.GetPen();
        dc.SetPen( ppPen1 );
        dc.SetBrush( wxBrush( m_wxcODPointRangeRingsSchemeColour, wxBRUSHSTYLE_TRANSPARENT ) );
        dc.SetGLStipple();
        
        for( int i = 1; i <= m_iODPointRangeRingsNumber; i++ )
            dc.StrokeCircle( l_odp.x, l_odp.y, i * pix_radius );
        
        glDisable( GL_LINE_STIPPLE );
        
        dc.SetPen( savePen );
        dc.SetBrush( saveBrush );
    }
    
    if( m_bPointPropertiesBlink || m_bPathManagerBlink ) g_blink_rect = CurrentRect_in_DC;               // also save for global blinker
    
    //    This will be useful for fast icon redraws
    CurrentRect_in_DC.x = l_odp.x + hilitebox.x;
    CurrentRect_in_DC.y = l_odp.y + hilitebox.y;
    CurrentRect_in_DC.width = hilitebox.width;
    CurrentRect_in_DC.height = hilitebox.height;

    if( m_bPointPropertiesBlink || m_bPathManagerBlink ) g_blink_rect = CurrentRect_in_DC;               // also save for global blinker
#endif
}

void ODPoint::SetPosition( double lat, double lon )
{
    m_lat = lat;
    m_lon = lon;
}

void ODPoint::CalculateDCRect( wxDC& dc, wxRect *prect )
{
    dc.ResetBoundingBox();
    dc.DestroyClippingRegion();

    // Draw the mark on the dc
    ODDC odc( dc );
    Draw( odc, NULL );

    //  Retrieve the drawing extents
    prect->x = dc.MinX() - 1;
    prect->y = dc.MinY() - 1;
    prect->width = dc.MaxX() - dc.MinX() + 2; // Mouse Poop?
    prect->height = dc.MaxY() - dc.MinY() + 2;

}

bool ODPoint::IsSame( ODPoint *pOtherRP )
{
    bool IsSame = false;

    if( this->m_ODPointName == pOtherRP->m_ODPointName ) {
        if( fabs( this->m_lat - pOtherRP->m_lat ) < 1.e-6
                && fabs( this->m_lon - pOtherRP->m_lon ) < 1.e-6 ) IsSame = true;
    }
    return IsSame;
}

bool ODPoint::SendToGPS(const wxString & com_name, wxGauge *pProgress)
{
    return TRUE;
}

double ODPoint::GetODPointArrivalRadius() {
    if (m_ODPointArrivalRadius < 0.001) {
        double l_arrive_radius = g_dODPointArrivalCircleRadius;
        l_arrive_radius += 0.1;
        SetODPointArrivalRadius( l_arrive_radius );
    }
    return m_ODPointArrivalRadius;
}

int   ODPoint::GetODPointRangeRingsNumber() { 
    if ( m_iODPointRangeRingsNumber == -1 )
        return g_iODPointRangeRingsNumber;
    else
        return m_iODPointRangeRingsNumber; 
}

float ODPoint::GetODPointRangeRingsStep() { 
    if ( m_fODPointRangeRingsStep == -1 )
        return g_fODPointRangeRingsStep;
    else
        return m_fODPointRangeRingsStep; 
}

int ODPoint::GetODPointRangeRingsStepUnits() { 
    if ( m_iODPointRangeRingsStepUnits == -1 )
        return g_iODPointRangeRingsStepUnits;
    else
        return m_iODPointRangeRingsStepUnits ; 
}

wxColour ODPoint::GetODPointRangeRingsColour(void) { 
    if ( m_wxcODPointRangeRingsColour.GetAsString(wxC2S_HTML_SYNTAX) == _T("#FFFFFF") )
        return g_colourODPointRangeRingsColour;
    else
        return m_wxcODPointRangeRingsColour;
}

void ODPoint::SetMarkDescription(wxString sMarkDescription)
{
    m_ODPointDescription = sMarkDescription;
}

void ODPoint::SetColourScheme(PI_ColorScheme cs)
{
    m_ColourScheme = cs;
    switch (cs) {
        case PI_GLOBAL_COLOR_SCHEME_RGB:
            m_wxcODPointRangeRingsSchemeColour = m_wxcODPointRangeRingsColourRGB;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DAY:
            m_wxcODPointRangeRingsSchemeColour = m_wxcODPointRangeRingsColourDay;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DUSK:
            m_wxcODPointRangeRingsSchemeColour = m_wxcODPointRangeRingsColourDusk;
            break;
        case PI_GLOBAL_COLOR_SCHEME_NIGHT:
            m_wxcODPointRangeRingsSchemeColour = m_wxcODPointRangeRingsColourNight;
            break;
        default:
            m_wxcODPointRangeRingsSchemeColour = m_wxcODPointRangeRingsColourDay;
            break;
    }

}

void ODPoint::SetRangeRingBBox(void)
{
    if(m_iODPointRangeRingsNumber > 0 && m_fODPointRangeRingsStep > 0) {
        double l_dist = m_iODPointRangeRingsNumber * m_fODPointRangeRingsStep;
        double l_minlat;
        double l_minlon;
        double l_maxlat;
        double l_maxlon;
        double l_lat;
        double l_lon;
        PositionBearingDistanceMercator_Plugin(m_lat, m_lon, 0, l_dist, &l_maxlat, &l_lon);
        PositionBearingDistanceMercator_Plugin(m_lat, m_lon, 90, l_dist, &l_lat, &l_maxlon);
        PositionBearingDistanceMercator_Plugin(m_lat, m_lon, 180, l_dist, &l_minlat, &l_lon);
        PositionBearingDistanceMercator_Plugin(m_lat, m_lon, 270, l_dist, &l_lat, &l_minlon);
        m_RangeRingsBBox.Set(l_minlat, l_minlon, l_maxlat, l_maxlon);
    }
}

void ODPoint::SetODPointRangeRingsNumber(int i_ODPointRangeRingsNumber) 
{ 
    m_iODPointRangeRingsNumber = i_ODPointRangeRingsNumber; 
    SetRangeRingBBox();
}

void ODPoint::SetODPointRangeRingsStep(float f_ODPointRangeRingsStep) 
{ 
    m_fODPointRangeRingsStep = f_ODPointRangeRingsStep; 
    SetRangeRingBBox();
}

void ODPoint::AddURL(wxString URL, wxString URLDescription)
{
    Hyperlink *l_Link = new Hyperlink();
    l_Link->Link = URL;
    l_Link->DescrText = URLDescription;
    l_Link->LType = wxEmptyString;
    
    m_HyperlinkList->Insert(l_Link);
}
