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

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "PointMan.h"

#include "ocpn_draw_pi.h"
#include "ODMarkIcon.h"
#include "ODConfig.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "ODUtils.h"
#include "cutil.h"
#include "TextPoint.h"
#include "BoundaryPoint.h"

#include <stddef.h>                     // for NULL

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/apptrait.h>
#include <wx/fontenum.h>

#ifndef __OCPN__ANDROID__
#include <GL/gl.h>
#else
#include "qopengl.h"                  // this gives us the qt runtime gles2.h
#include "GL/gl_private.h"
#endif

//--------------------------------------------------------------------------------
//      PointMan   Implementation
//--------------------------------------------------------------------------------

PointMan::PointMan()
{

    m_pODPointList = new ODPointList;

    pmarkicon_image_list = NULL;

    //ocpnStyle::Style* style = g_ODStyleManager->GetCurrentStyle();
    m_pIconArray = new wxArrayPtrVoid();
    ProcessIcons();

    m_pFontEnumerator = new wxFontEnumerator();
    m_pFontEnumerator->EnumerateFacenames();
    m_wxasFontFacenames = wxFontEnumerator::GetFacenames();

    m_nGUID = 0;
    
}

PointMan::~PointMan()
{
    //    Two step here, since the ODPoint dtor also touches the
    //    ODPoint list.
    //    Copy the master ODPoint list to a temporary list,
    //    then clear and delete objects from the temp list

    ODPointList temp_list;

    wxODPointListNode *node = m_pODPointList->GetFirst();
    while( node ) {
        ODPoint *pr = node->GetData();

        temp_list.Append( pr );
        node = node->GetNext();
    }

    temp_list.DeleteContents( true );
    temp_list.Clear();

    m_pODPointList->Clear();
    delete m_pODPointList;

    for( unsigned int i = 0; i < m_pIconArray->GetCount(); i++ ) {
        ODMarkIcon *pmi = (ODMarkIcon *) m_pIconArray->Item( i );
        delete pmi;
    }

    m_pIconArray->Clear();
    delete m_pIconArray;

    if( pmarkicon_image_list ) pmarkicon_image_list->RemoveAll();
    delete pmarkicon_image_list;
    
    delete m_pFontEnumerator;
}

bool PointMan::AddODPoint(ODPoint *prp)
{
    if(!prp)
        return false;
    
    wxODPointListNode *prpnode = m_pODPointList->Append(prp);
    prp->SetManagerListNode( prpnode );
    
    return true;
}

bool PointMan::RemoveODPoint(ODPoint *prp)
{
    if(!prp)
        return false;
    
    wxODPointListNode *prpnode = (wxODPointListNode *)prp->GetManagerListNode();
    
    if(prpnode) 
        delete prpnode;
    else
        m_pODPointList->DeleteObject(prp);
    
    prp->SetManagerListNode( NULL );
    
    return true;
}

void PointMan::ProcessUserIcons( )
{
    
    if( wxDir::Exists( *g_pUserIconsDir ) ) {
        wxArrayString FileList;
        
        wxDir dir( *g_pUserIconsDir );
        int n_files = dir.GetAllFiles( *g_pUserIconsDir, &FileList );
        
        for( int ifile = 0; ifile < n_files; ifile++ ) {
            wxString name = FileList.Item( ifile );
            
            wxFileName fn( name );
            wxString iconname = fn.GetName();
            wxBitmap icon1;
            
            if( fn.GetExt().Lower() == _T("xpm") ) {
                if( icon1.LoadFile( name, wxBITMAP_TYPE_XPM ) ) {
                    ProcessIcon( icon1, iconname, iconname );
                }
            }
            if( fn.GetExt().Lower() == _T("png") ) {
                if( icon1.LoadFile( name, wxBITMAP_TYPE_PNG ) ) {
                    ProcessIcon( icon1, iconname, iconname );
                }
            }
        }
    }
}


void PointMan::ProcessIcons( )
{
    ProcessIcon( GetIcon_PlugIn( _T("empty") ), _T("empty"), _T("Empty") );
    ProcessIcon( GetIcon_PlugIn( _T("airplane") ), _T("airplane"), _T("Airplane") );
    ProcessIcon( GetIcon_PlugIn( _T("anchorage") ), _T("anchorage"), _T("Anchorage") );
    ProcessIcon( GetIcon_PlugIn( _T("anchor") ), _T("anchor"), _T("Anchor") );
    ProcessIcon( GetIcon_PlugIn( _T("boarding") ), _T("boarding"), _T("Boarding Location") );
    ProcessIcon( GetIcon_PlugIn( _T("boundary") ), _T("boundary"), _T("Boundary Mark") );
    ProcessIcon( GetIcon_PlugIn( _T("bouy1") ), _T("bouy1"), _T("Bouy Type A") );
    ProcessIcon( GetIcon_PlugIn( _T("bouy2") ), _T("bouy2"), _T("Bouy Type B") );
    ProcessIcon( GetIcon_PlugIn( _T("campfire") ), _T("campfire"), _T("Campfire") );
    ProcessIcon( GetIcon_PlugIn( _T("camping") ), _T("camping"), _T("Camping Spot") );
    ProcessIcon( GetIcon_PlugIn( _T("coral") ), _T("coral"), _T("Coral") );
    ProcessIcon( GetIcon_PlugIn( _T("fishhaven") ), _T("fishhaven"), _T("Fish Haven") );
    ProcessIcon( GetIcon_PlugIn( _T("fishing") ), _T("fishing"), _T("Fishing Spot") );
    ProcessIcon( GetIcon_PlugIn( _T("fish") ), _T("fish"), _T("Fish") );
    ProcessIcon( GetIcon_PlugIn( _T("float") ), _T("float"), _T("Float") );
    ProcessIcon( GetIcon_PlugIn( _T("food") ), _T("food"), _T("Food") );
    ProcessIcon( GetIcon_PlugIn( _T("fuel") ), _T("fuel"), _T("Fuel") );
    ProcessIcon( GetIcon_PlugIn( _T("greenlite") ), _T("greenlite"), _T("Green Light") );
    ProcessIcon( GetIcon_PlugIn( _T("kelp") ), _T("kelp"), _T("Kelp") );
    ProcessIcon( GetIcon_PlugIn( _T("light") ), _T("light1"), _T("Light Type A") );
    ProcessIcon( GetIcon_PlugIn( _T("light1") ), _T("light"), _T("Light Type B") );
    ProcessIcon( GetIcon_PlugIn( _T("litevessel") ), _T("litevessel"), _T("Light Vessel") );
    ProcessIcon( GetIcon_PlugIn( _T("mob") ), _T("mob"), _T("MOB") );
    ProcessIcon( GetIcon_PlugIn( _T("mooring") ), _T("mooring"), _T("Mooring Bouy") );
    ProcessIcon( GetIcon_PlugIn( _T("oilbouy") ), _T("oilbouy"), _T("Oil Bouy") );
    ProcessIcon( GetIcon_PlugIn( _T("platform") ), _T("platform"), _T("Platform") );
    ProcessIcon( GetIcon_PlugIn( _T("redgreenlite") ), _T("redgreenlite"), _T("Red/Green Light") );
    ProcessIcon( GetIcon_PlugIn( _T("redlite") ), _T("redlite"), _T("Red Light") );
    ProcessIcon( GetIcon_PlugIn( _T("rock1") ), _T("rock1"), _T("Rock (exposed)") );
    ProcessIcon( GetIcon_PlugIn( _T("rock2") ), _T("rock2"), _T("Rock, (awash)") );
    ProcessIcon( GetIcon_PlugIn( _T("sand") ), _T("sand"), _T("Sand") );
    ProcessIcon( GetIcon_PlugIn( _T("scuba") ), _T("scuba"), _T("Scuba") );
    ProcessIcon( GetIcon_PlugIn( _T("shoal") ), _T("shoal"), _T("Shoal") );
    ProcessIcon( GetIcon_PlugIn( _T("snag") ), _T("snag"), _T("Snag") );
    ProcessIcon( GetIcon_PlugIn( _T("square") ), _T("square"), _T("Square") );
    ProcessIcon( GetIcon_PlugIn( _T("triangle") ), _T("triangle"), _T("Triangle") );
    ProcessIcon( GetIcon_PlugIn( _T("diamond") ), _T("diamond"), _T("Diamond") );
    ProcessIcon( GetIcon_PlugIn( _T("circle") ), _T("circle"), _T("Circle") );
    ProcessIcon( GetIcon_PlugIn( _T("wreck1") ), _T("wreck1"), _T("Wreck A") );
    ProcessIcon( GetIcon_PlugIn( _T("wreck2") ), _T("wreck2"), _T("Wreck B") );
    ProcessIcon( GetIcon_PlugIn( _T("xmblue") ), _T("xmblue"), _T("Blue X") );
    ProcessIcon( GetIcon_PlugIn( _T("xmgreen") ), _T("xmgreen"), _T("Green X") );
    ProcessIcon( GetIcon_PlugIn( _T("xmred") ), _T("xmred"), _T("Red X") );
    ProcessIcon( GetIcon_PlugIn( _T("activepoint") ), _T("activepoint"), _T("Active WP") );
// Load user defined icons.
    // Done after default icons are initialized,
    // so that user may substitute an icon by using the same name in the Usericons file.
    ProcessUserIcons( );
    
}

void PointMan::ProcessIcon(wxBitmap pimage, const wxString & key, const wxString & description)
{
    ODMarkIcon *pmi;

    bool newIcon = true;

    // avoid adding duplicates
    for( unsigned int i = 0; i < m_pIconArray->GetCount(); i++ ) {
        pmi = (ODMarkIcon *) m_pIconArray->Item( i );
        if( pmi->icon_name.IsSameAs( key ) ) {
            newIcon = false;
            delete pmi->picon_bitmap;
            break;
        }
    }

    if( newIcon ) {
        pmi = new ODMarkIcon;
        m_pIconArray->Add( (void *) pmi );
    }

    pmi->icon_name = key;
    pmi->icon_description = description;
    pmi->picon_bitmap = new wxBitmap( pimage );
    pmi->picon_bitmap_Day = new wxBitmap( pimage );
    pmi->picon_bitmap_RGB = new wxBitmap( pimage );
    pmi->picon_bitmap_Dusk = CreateDimBitmap( pmi-> picon_bitmap, 0.5 );
    pmi->picon_bitmap_Night = CreateDimBitmap( pmi->picon_bitmap, 0.25 );
}

void PointMan::RemoveIcon(wxString key)
{
    ODMarkIcon *pmi;
    
    for(size_t i = 0; i < m_pIconArray->GetCount(); i++) {
        pmi = (ODMarkIcon *) m_pIconArray->Item( i );
        if(pmi->icon_name.IsSameAs( key )) {
            m_pIconArray->RemoveAt(i);
            delete pmi;
        }
    }
}
wxImageList *PointMan::Getpmarkicon_image_list( void )
{
    // First find the largest bitmap size
    int w = 0;
    int h = 0;

    ODMarkIcon *pmi;

    for( unsigned int i = 0; i < m_pIconArray->GetCount(); i++ ) {
        pmi = (ODMarkIcon *) m_pIconArray->Item( i );
        w = wxMax(w, pmi->picon_bitmap->GetWidth());
        h = wxMax(h, pmi->picon_bitmap->GetHeight());

        // toh, 10.09.29
        // User defined icons won't be displayed in the list if they are larger than 32x32 pixels (why???)
        // Work-around: limit size
        if( w > 32 ) w = 32;
        if( h > 32 ) h = 32;

    }

    // Build an image list large enough

    if( NULL != pmarkicon_image_list ) {
        pmarkicon_image_list->RemoveAll();
        delete pmarkicon_image_list;
    }
    pmarkicon_image_list = new wxImageList( w, h );

    // Add the icons
    for( unsigned int ii = 0; ii < m_pIconArray->GetCount(); ii++ ) {
        pmi = (ODMarkIcon *) m_pIconArray->Item( ii );
        wxImage icon_image = pmi->picon_bitmap->ConvertToImage();

        // toh, 10.09.29
        // After limiting size user defined icons will be cut off
        // Work-around: rescale in one or both directions
        int h0 = icon_image.GetHeight();
        int w0 = icon_image.GetWidth();

        wxImage icon_larger;
        if( h0 <= h && w0 <= w ) {
            // Resize & Center smaller icons in the bitmap, so menus won't look so weird.
            icon_larger = icon_image.Resize( wxSize( w, h ), wxPoint( (w-w0)/2, (h-h0)/2 ) );
        } else {
            // rescale in one or two directions to avoid cropping, then resize to fit to cell
            int h1 = h;
            int w1 = w;
            if( h0 > h ) w1 = wxRound( (double) w0 * ( (double) h / (double) h0 ) );

            else if( w0 > w ) h1 = wxRound( (double) h0 * ( (double) w / (double) w0 ) );

            icon_larger = icon_image.Rescale( w1, h1 );
            icon_larger = icon_larger.Resize( wxSize( w, h ), wxPoint( 0, 0 ) );
        }

        pmarkicon_image_list->Add( icon_larger );
    }
    
    m_markicon_image_list_base_count = pmarkicon_image_list->GetImageCount(); 

    // Create and add "x-ed out" icons,
    // Being careful to preserve (some) transparency
    for( unsigned int ii = 0; ii < m_pIconArray->GetCount(); ii++ ) {

        wxImage img = pmarkicon_image_list->GetBitmap( ii ).ConvertToImage() ;
        img.ConvertAlphaToMask( 128 );

        unsigned char r,g,b;
        img.GetOrFindMaskColour(&r, &g, &b);
        wxColour unused_color(r,g,b);

        wxBitmap bmp0( img );
    
        wxBitmap bmp(w, h, -1 );
        wxMemoryDC mdc( bmp );
        mdc.SetBackground( wxBrush( unused_color) );
        mdc.Clear();
        mdc.DrawBitmap( bmp0, 0, 0 );
        wxColour wxRed;
        GetGlobalColor( wxS( "URED" ), &wxRed );
        wxPen red( wxRed );
        mdc.SetPen( red );
        int xm = bmp.GetWidth();
        int ym = bmp.GetHeight();
        mdc.DrawLine( 2, 2, xm-2, ym-2 );
        mdc.DrawLine( xm-2, 2, 2, ym-2 );
        mdc.SelectObject( wxNullBitmap );
        
        wxMask *pmask = new wxMask(bmp, unused_color);
        bmp.SetMask( pmask );

        pmarkicon_image_list->Add( bmp );
    }
        
    return pmarkicon_image_list;
}

wxBitmap *PointMan::CreateDimBitmap( wxBitmap *pBitmap, double factor )
{
    wxImage img = pBitmap->ConvertToImage();
    int sx = img.GetWidth();
    int sy = img.GetHeight();

    wxImage new_img( img );

    for( int i = 0; i < sx; i++ ) {
        for( int j = 0; j < sy; j++ ) {
            if( !img.IsTransparent( i, j ) ) {
                new_img.SetRGB( i, j, (unsigned char) ( img.GetRed( i, j ) * factor ),
                        (unsigned char) ( img.GetGreen( i, j ) * factor ),
                        (unsigned char) ( img.GetBlue( i, j ) * factor ) );
            }
        }
    }

    wxBitmap *pret = new wxBitmap( new_img );

    return pret;

}

void PointMan::SetColorScheme( PI_ColorScheme cs )
{
    //ProcessIcons( g_ODStyleManager->GetCurrentStyle() );

    //    Iterate on the ODPoint list, requiring each to reload icon
    m_ColourScheme = cs;
    wxODPointListNode *node = m_pODPointList->GetFirst();
    while( node ) {
        ODPoint *pr = node->GetData();
        pr->SetColourScheme(cs);
        pr->ReLoadIcon();
        node = node->GetNext();
    }
}

bool PointMan::DoesIconExist(const wxString & icon_key) const
{
    ODMarkIcon *pmi;
    unsigned int i;

    for( i = 0; i < m_pIconArray->GetCount(); i++ ) {
        pmi = (ODMarkIcon *) m_pIconArray->Item( i );
        if( pmi->icon_name.IsSameAs( icon_key ) ) return true;
        if( pmi->icon_description.IsSameAs( icon_key ) ) return true;
    }

    return false;
}

wxBitmap *PointMan::GetIconBitmap( const wxString& icon_key )
{
    wxBitmap *pret = NULL;
    ODMarkIcon *pmi = NULL;
    unsigned int i;

    for( i = 0; i < m_pIconArray->GetCount(); i++ ) {
        pmi = (ODMarkIcon *) m_pIconArray->Item( i );
        if( pmi->icon_name.IsSameAs( icon_key ) ) break;
        if( pmi->icon_description.IsSameAs( icon_key ) ) break;
    }

    if( i == m_pIconArray->GetCount() )              // key not found
    {
        // find and return bitmap for "circle"
        for( i = 0; i < m_pIconArray->GetCount(); i++ ) {
            pmi = (ODMarkIcon *) m_pIconArray->Item( i );
            if( pmi->icon_name.IsSameAs( _T("circle") ) )
                break;
        }

        if( i == m_pIconArray->GetCount() )              // "circle" not found
            pmi = (ODMarkIcon *) m_pIconArray->Item( 0 );       // use item 0
    }
    
    if( pmi ) {
        switch (m_ColourScheme) {
            case PI_GLOBAL_COLOR_SCHEME_RGB:
                pret = pmi->picon_bitmap_RGB;
                break;
            case PI_GLOBAL_COLOR_SCHEME_DAY:
                pret = pmi->picon_bitmap_Day;
                break;
            case PI_GLOBAL_COLOR_SCHEME_DUSK:
                pret = pmi->picon_bitmap_Dusk;
                break;
            case PI_GLOBAL_COLOR_SCHEME_NIGHT:
                pret = pmi->picon_bitmap_Night;
                break;
            default:
                pret = pmi->picon_bitmap_RGB;
                break;
        }
    }

    return pret;
}

unsigned int PointMan::GetIconTexture( const wxBitmap *pbm, int &glw, int &glh )
{
#ifdef ocpnUSE_GL 
    int index = GetIconIndex( pbm );
    ODMarkIcon *pmi = (ODMarkIcon *) m_pIconArray->Item( index );
    assert(pmi != 0);

    unsigned int *IconTexture;
    switch (m_ColourScheme) {
        case PI_GLOBAL_COLOR_SCHEME_RGB:
            IconTexture = &pmi->icon_texture_RGB;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DAY:
            IconTexture = &pmi->icon_texture_Day;
            break;
        case PI_GLOBAL_COLOR_SCHEME_DUSK:
            IconTexture = &pmi->icon_texture_Dusk;
            break;
        case PI_GLOBAL_COLOR_SCHEME_NIGHT:
            IconTexture = &pmi->icon_texture_Night;
            break;
        default:
            IconTexture = &pmi->icon_texture_Day;
            break;
    }
    if(*IconTexture == 0) {
        /* make rgba texture */       
        glGenTextures(1, IconTexture);
        glBindTexture(GL_TEXTURE_2D, *IconTexture);
                
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

        wxImage image = pbm->ConvertToImage();
        int w = image.GetWidth(), h = image.GetHeight();
        
        pmi->tex_w = NextPow2(w);
        pmi->tex_h = NextPow2(h);
        
        unsigned char *d = image.GetData();
        unsigned char *a = image.GetAlpha();
            
        unsigned char mr, mg, mb;
        if(!a)
            image.GetOrFindMaskColour( &mr, &mg, &mb );
    
        unsigned char *e = new unsigned char[4 * w * h];
        if(d && e){
            for( int y = 0; y < h; y++ )
                for( int x = 0; x < w; x++ ) {
                    unsigned char r, g, b;
                    int off = ( y * image.GetWidth() + x );
                    r = d[off * 3 + 0];
                    g = d[off * 3 + 1];
                    b = d[off * 3 + 2];
                    
                    e[off * 4 + 0] = r;
                    e[off * 4 + 1] = g;
                    e[off * 4 + 2] = b;
                    
                    e[off * 4 + 3] =  a ? a[off] : ( ( r == mr ) && ( g == mg ) && ( b == mb ) ? 0 : 255 );
                }
        }
    
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pmi->tex_w, pmi->tex_h,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h,
                        GL_RGBA, GL_UNSIGNED_BYTE, e);

        delete [] e;
    }

    glw = pmi->tex_w;
    glh = pmi->tex_h;

    return *IconTexture;
#else
    return 0;
#endif
}

wxBitmap *PointMan::GetIconBitmap( int index )
{
    wxBitmap *pret = NULL;

    if( index >= 0 ) {
        ODMarkIcon *pmi = (ODMarkIcon *) m_pIconArray->Item( index );
        switch (m_ColourScheme) {
            case PI_GLOBAL_COLOR_SCHEME_RGB:
                pret = pmi->picon_bitmap_RGB;
                break;
            case PI_GLOBAL_COLOR_SCHEME_DAY:
                pret = pmi->picon_bitmap_Day;
                break;
            case PI_GLOBAL_COLOR_SCHEME_DUSK:
                pret = pmi->picon_bitmap_Dusk;
                break;
            case PI_GLOBAL_COLOR_SCHEME_NIGHT:
                pret = pmi->picon_bitmap_Night;
                break;
            default:
                pret = pmi->picon_bitmap_RGB;
                break;
        }

    }
    return pret;
}

wxString *PointMan::GetIconDescription( int index )
{
    wxString *pret = NULL;

    if( index >= 0 ) {
        ODMarkIcon *pmi = (ODMarkIcon *) m_pIconArray->Item( index );
        pret = &pmi->icon_description;
    }
    return pret;
}

wxString *PointMan::GetIconName( wxString wxIconDescription )
{
    wxString *pret = NULL;
    
    for( int i = 0; i < (int)m_pIconArray->Count(); i++ ) {
        ODMarkIcon *pmi = (ODMarkIcon *) m_pIconArray->Item( i );
        if( wxIconDescription.IsSameAs( pmi->icon_description) ) {
            pret = &pmi->icon_name;
            break;
        }
    }
    return pret;
}

wxString *PointMan::GetIconName( int index )
{
    wxString *pret = NULL;
    
    if( index >= 0 ) {
        ODMarkIcon *pmi = (ODMarkIcon *) m_pIconArray->Item( index );
        pret = &pmi->icon_name;
    }
    return pret;
}

wxString *PointMan::GetIconKey( int index )
{
    wxString *pret = NULL;

    if( (index >= 0)  && ((unsigned int)index < m_pIconArray->GetCount()) ) {
        ODMarkIcon *pmi = (ODMarkIcon *) m_pIconArray->Item( index );
        pret = &pmi->icon_name;
    }
    return pret;
}

int PointMan::GetIconIndex( const wxBitmap *pbm )
{
    unsigned int i;

    for( i = 0; i < m_pIconArray->GetCount(); i++ ) {
        ODMarkIcon *pmi = (ODMarkIcon *) m_pIconArray->Item( i );
        //if( pmi->picon_bitmap == pbm || pmi->picon_bitmap_RGB == pbm || pmi->picon_bitmap_Day == pbm || pmi->picon_bitmap_Dusk == pbm || pmi->picon_bitmap_Night == pbm) break;
        if( pmi->picon_bitmap == pbm ) break;
        if( pmi->picon_bitmap_RGB == pbm ) break;
        if( pmi->picon_bitmap_Day == pbm ) break;
        if( pmi->picon_bitmap_Dusk == pbm ) break;
        if( pmi->picon_bitmap_Night == pbm) break;

    }

    return i;                                           // index of base icon in the image list

}

int PointMan::GetXIconIndex( const wxBitmap *pbm )
{
    unsigned int i;
    
    for( i = 0; i < m_pIconArray->GetCount(); i++ ) {
        ODMarkIcon *pmi = (ODMarkIcon *) m_pIconArray->Item( i );
        if( pmi->picon_bitmap == pbm ) break;
        if( pmi->picon_bitmap_RGB == pbm ) break;
        if( pmi->picon_bitmap_Day == pbm ) break;
        if( pmi->picon_bitmap_Dusk == pbm ) break;
        if( pmi->picon_bitmap_Night == pbm) break;
    }

    return i + m_markicon_image_list_base_count;        // index of "X-ed out" icon in the image list
}

ODPoint *PointMan::FindODPointByGUID(const wxString &guid)
{
    wxODPointListNode *prpnode = m_pODPointList->GetFirst();
    while( prpnode ) {
        ODPoint *prp = prpnode->GetData();

        if( prp->m_GUID == guid ) return ( prp );

        prpnode = prpnode->GetNext(); //ODPoint
    }

    return NULL;
}

ODPoint *PointMan::GetNearbyODPoint( double lat, double lon, double radius_meters )
{
    //    Iterate on the ODPoint list, checking distance

    wxODPointListNode *node = m_pODPointList->GetFirst();
    while( node ) {
        ODPoint *pr = node->GetData();

        double a = lat - pr->m_lat;
        double b = lon - pr->m_lon;
        double l = sqrt( ( a * a ) + ( b * b ) );

        if( !pr->m_bSingleUse && ( l * 60. * 1852. ) < radius_meters ) return pr;

        node = node->GetNext();
    }
    return NULL;

}

ODPoint *PointMan::GetOtherNearbyODPoint( double lat, double lon, double radius_meters,
        const wxString &guid )
{
    //    Iterate on the ODPoint list, checking distance

    wxODPointListNode *node = m_pODPointList->GetFirst();
    while( node ) {
        ODPoint *pr = node->GetData();

        double a = lat - pr->m_lat;
        double b = lon - pr->m_lon;
        double l = sqrt( ( a * a ) + ( b * b ) );

        if( ( l * 60. * 1852. ) < radius_meters ) if( pr->m_GUID != guid ) return pr;

        node = node->GetNext();
    }
    return NULL;

}

void PointMan::ClearODPointFonts( void )
{
    //    Iterate on the ODPoint list, clearing Font pointers
    //    This is typically done globally after a font switch

    wxODPointListNode *node = m_pODPointList->GetFirst();
    while( node ) {
        ODPoint *pr = node->GetData();

        pr->m_pMarkFont = NULL;
        node = node->GetNext();
    }
}

bool PointMan::SharedODPointsExist()
{
    wxODPointListNode *node = m_pODPointList->GetFirst();
    while( node ) {
        ODPoint *prp = node->GetData();
        if (prp->m_bKeepXPath && prp->m_bIsInPath )
            return true;
        node = node->GetNext();
    }
    return false;
}

void PointMan::DeleteAllODPoints( bool b_delete_used )
{
    //    Iterate on the ODPoint list, deleting all
    wxODPointListNode *node = m_pODPointList->GetFirst();
    while( node ) {
        ODPoint *prp = node->GetData();
        // if argument is false, then only delete non-path ODPoints
        if( !prp->m_bIsInLayer && ( prp->GetIconName() != _T("mob") )
            && ( ( b_delete_used && prp->m_bKeepXPath )
                        ||  !prp->m_bIsInPath   ) ) {
            DestroyODPoint(prp);
			delete prp;
            node = m_pODPointList->GetFirst();
        } else
            node = node->GetNext();
    }
    return;

}

void PointMan::DestroyODPoint( ODPoint *pRp, bool b_update_changeset )
{
    if( ! b_update_changeset )
        g_pODConfig->m_bSkipChangeSetUpdate = true;             // turn OFF change-set updating if requested
        
    if( pRp ) {
        // Get a list of all boundaries containing this point
        // and remove the point from them all
        wxArrayPtrVoid *ppath_array = g_pPathMan->GetPathArrayContaining( pRp );
        if( ppath_array ) {
            for( unsigned int ib = 0; ib < ppath_array->GetCount(); ib++ ) {
                ODPath *pb = (ODPath *) ppath_array->Item( ib );

                pb->RemovePoint( pRp );

            }

            //    Scrub the paths, looking for one-point paths
            for( unsigned int ib = 0; ib < ppath_array->GetCount(); ib++ ) {
                ODPath *pb = (ODPath *) ppath_array->Item( ib );
                if( pb->GetnPoints() < 2 ) {
                    bool prev_bskip = g_pODConfig->m_bSkipChangeSetUpdate;
                    g_pODConfig->m_bSkipChangeSetUpdate = true;
                    g_pODConfig->DeleteConfigPath( pb );
                    g_pPathMan->DeletePath( pb );
                    g_pODConfig->m_bSkipChangeSetUpdate = prev_bskip;
                }
            }

            delete ppath_array;
        }

        // Now it is safe to delete the point
        g_pODConfig->DeleteODPoint( pRp );
        g_pODConfig->m_bSkipChangeSetUpdate = false;
        
        g_pODSelect->DeleteSelectableODPoint( pRp );


        RemoveODPoint( pRp);

    }
}


// XXX FIXME 0 and 360
bool PointMan::DistancePointLine( double pLon, double pLat, double StartLon, double StartLat, double EndLon, double EndLat, double Distance )
{
   double sx, sy;
   double ex, ey;
   double px, py;
   double r = Distance;
   double radius = 6371007.2; // in meters

   sx = radius *cos(deg2rad(pLat)) * deg2rad(StartLon);
   sy = radius *deg2rad(StartLat);

   ex = radius *cos(deg2rad(pLat)) * deg2rad(EndLon);
   ey = radius *deg2rad(EndLat);

   // center point
   px = radius *cos(deg2rad(pLat)) * deg2rad(pLon);
   py = radius *deg2rad(pLat);

   double a,b,c;
   double bb4ac;
   double x,y;
   double t;

   x = ex - sx;
   y = ey - sy;
   a = x * x + y * y;
   b = 2 * (x * (sx - px) + y * (sy - py));
   c = px * px + py * py;
   c += sx * sx + sy * sy;
   c -= 2 * (px * sx + py * sy );
   c -= r * r;
   bb4ac = b * b - 4 * a * c;

   if (fabs(a) < 1.e-6 || bb4ac < 0.) {
      return false;
   }
   else if (bb4ac == 0.) {
      // One solution.
      t = -b / (2 * a);
      if (t < 0. || t > 1.)
          return false;
   }
   else {
      t = (-b + sqrt( bb4ac)) / (2. * a);
      if (t < 0. || t > 1.) {
          t = (-b - sqrt( bb4ac)) / (2. * a);
          if (t < 0. || t > 1.) 
              return false;
      }
   }
   return true;
}

BoundaryPoint *PointMan::FindLineCrossingBoundaryPtr( double StartLon, double StartLat, double EndLon, double EndLat, int type, int state )
{
    // search boundary point
    wxODPointListNode *node = GetODPointList()->GetFirst();
    while( node ) {
        ODPoint *od = static_cast<ODPoint *>(node->GetData());
        if( od->IsListed() ) {
            if( od->m_bIsInPath && !od->m_bKeepXPath ) {
                node = node->GetNext();
                continue;
            }
            // if there's no ring there's nothing to do
            if (!od->GetShowODPointRangeRings() || 
                    od->GetODPointRangeRingsNumber() == 0 ||
                    od->GetODPointRangeRingsStep() == 0.f)
            {
                node = node->GetNext();
                continue;
            }
            BoundaryPoint *op = dynamic_cast<BoundaryPoint *>(node->GetData());
            if (!op) {
                node = node->GetNext();
                continue;
            }
            bool l_bNext = false;
            switch (type) {
                case ID_BOUNDARY_ANY:
                    l_bNext = false;
                    break;
                case ID_BOUNDARY_EXCLUSION:
                    if(!op->m_bExclusionBoundaryPoint) l_bNext = true;
                    break;
                case ID_BOUNDARY_INCLUSION:
                    if(!op->m_bInclusionBoundaryPoint) l_bNext = true;
                    break;
                case ID_BOUNDARY_NEITHER:
                    if(op->m_bExclusionBoundaryPoint || op->m_bInclusionBoundaryPoint) l_bNext = true;
                    break;
            }
            if (!l_bNext) {
               // 0 nautical miles 1 kilometer
               double f = (op->m_iODPointRangeRingsStepUnits == 1)?1000.0:1852.31;
               // in meters
               double dst = op->GetODPointRangeRingsNumber() * op->GetODPointRangeRingsStep() * f;
               if (DistancePointLine( op->m_lon, op->m_lat, StartLon, StartLat, EndLon, EndLat, dst )) {
                  return op;
               }
            }
        }
        node = node->GetNext();
    }
    return 0;
}

wxString PointMan::FindLineCrossingBoundary( double StartLon, double StartLat, double EndLon, double EndLat, int type, int state )
{
    BoundaryPoint *op;
    wxString l_sGUID;
    op = FindLineCrossingBoundaryPtr( StartLon, StartLat, EndLon, EndLat, type, state );
    if ( op != 0)
        return op->m_GUID;
    return _T("");
    
}
