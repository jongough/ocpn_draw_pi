/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  OCPN Draw Nav Objects support
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

#include "ODNavObjectChanges.h"
#include "ocpn_draw_pi.h"
#include "ODSelect.h"
#include "ODConfig.h"
#include "PointMan.h"
#include "PathMan.h"
#include "ODPoint.h"
#include "BoundaryPoint.h"
#include "TextPoint.h"
#include "Boundary.h"
#include "EBL.h"
#include "DR.h"
#include "GZ.h"
#include "PIL.h"
#include "ODUtils.h"
#include "BoundaryCSVImport.h"
#include "BoundaryPointCSVImport.h"
#include "TextPointCSVImport.h"
#include <wx/tokenzr.h>

extern PathList         *g_pPathList;
extern BoundaryList     *g_pBoundaryList;
extern EBLList          *g_pEBLList;
extern GZList           *g_pGZList;
extern PILList          *g_pPILList;
extern ODSelect         *g_pODSelect;
pugi::xml_node          gpx_path_child;
pugi::xml_node          gpx_path_root;
extern ODConfig         *g_pODConfig;
extern PointMan         *g_pODPointMan;
extern PathMan          *g_pPathMan;
extern int              g_iTextPosition;
extern wxColour         g_colourDefaultTextColour;
extern wxColour         g_colourDefaultTextBackgroundColour;
extern int              g_iTextBackgroundTransparency;
extern wxFont           g_DisplayTextFont;
extern int              g_iInclusionBoundaryPointSize;
extern unsigned int     g_uiBoundaryPointFillTransparency;
extern bool             g_bExclusionBoundaryPoint;
extern bool             g_bInclusionBoundaryPoint;
extern int              g_navobjbackups;
extern int              g_iGZMaxNum;
extern PI_ColorScheme   g_global_color_scheme;
extern wxString         g_sODPointIconName;


ODNavObjectChanges::ODNavObjectChanges() : pugi::xml_document()
{
    //ctor
    m_bFirstPath = true;
    m_ODchanges_file = 0;
    m_ptODPointList = new ODPointList;
}

ODNavObjectChanges::ODNavObjectChanges(wxString file_name) : pugi::xml_document()
{
    //ctor
    m_ODfilename = file_name;
    m_ODchanges_file = fopen(m_ODfilename.mb_str(), "a");
    m_bFirstPath = true;
    m_ptODPointList = new ODPointList;
}

ODNavObjectChanges::~ODNavObjectChanges()
{
    //dtor
    if(m_ODchanges_file)
        fclose(m_ODchanges_file);

    if( ::wxFileExists( m_ODfilename ) )
        ::wxRemoveFile( m_ODfilename );
    m_ptODPointList->clear();
    delete m_ptODPointList;
}

void ODNavObjectChanges::RemoveChangesFile( void )
{
    if(m_ODchanges_file)
        fclose(m_ODchanges_file);
    
    CreateRotatingNavObjChangesBackup();
    
    m_ODchanges_file = fopen(m_ODfilename.mb_str(), "a");
}

void ODNavObjectChanges::CreateRotatingNavObjChangesBackup()
{
    //Rotate navobj backups, but just in case there are some changes in the current version
    //to prevent the user trying to "fix" the problem by continuously starting the
    //application to overwrite all of his good backups...
    if( g_navobjbackups > 0 ) {
        wxFile f;
        wxString oldname = m_ODfilename;
        wxString newname = wxString::Format( _T("%s.1"), m_ODfilename.c_str() );
        
        wxFileOffset s_diff = 1;
        if( wxFileExists( newname ) ) {
            
            if( f.Open(oldname) ){
                s_diff = f.Length();
                f.Close();
            }
            
            if( f.Open(newname) ){
                s_diff -= f.Length();
                f.Close();
            }
        }
        
        
        if ( s_diff != 0 )
        {
            for( int i = g_navobjbackups - 1; i >= 1; i-- )
            {
                oldname = wxString::Format( _T("%s.%d"), m_ODfilename.c_str(), i );
                newname = wxString::Format( _T("%s.%d"), m_ODfilename.c_str(), i + 1 );
                if( wxFile::Exists( oldname ) )
                    wxCopyFile( oldname, newname );
            }
            
            if( wxFile::Exists( m_ODfilename ) )
            {
                newname = wxString::Format( _T("%s.1"), m_ODfilename.c_str() );
                wxCopyFile( m_ODfilename, newname );
            }
        }
    }

    wxRemoveFile( m_ODfilename );

    //try to clean the backups the user doesn't want - breaks if he deleted some by hand as it tries to be effective...
    for( int i = g_navobjbackups + 1; i <= 99; i++ )
        if( wxFile::Exists( wxString::Format( _T("%s.%d"), m_ODfilename.c_str(), i ) ) ) 
            ::wxRemoveFile( wxString::Format( _T("%s.%d"), m_ODfilename.c_str(), i ) );
        else
            break;
}

bool ODNavObjectChanges::GPXCreateODPoint( pugi::xml_node node, ODPoint *pop, unsigned int flags )
{
    wxString s;
    pugi::xml_node child;
    pugi::xml_attribute attr;
    
#ifndef __WXMSW__
    wxString *l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, "C");
#else
    setlocale(LC_NUMERIC, "C");
#endif
#endif
    
    s.Printf(_T("%.9f"), pop->m_lat);
    node.append_attribute("lat") = s.mb_str();
    s.Printf(_T("%.9f"), pop->m_lon);
    node.append_attribute("lon") = s.mb_str();

    if(flags & OUT_TYPE) {
        child = node.append_child("opencpn:type");
        child.append_child(pugi::node_pcdata).set_value(pop->GetTypeString().mb_str());
    }
    
    if(flags & OUT_TIME) {
            child = node.append_child("time");
            if( pop->m_timestring.Len() )
                child.append_child(pugi::node_pcdata).set_value(pop->m_timestring.mb_str());
            else {
                wxString t = pop->GetCreateTime().FormatISODate().Append(_T("T")).Append(pop->GetCreateTime().FormatISOTime()).Append(_T("Z"));
                child.append_child(pugi::node_pcdata).set_value(t.mb_str());
            }
    }
        
    if ( (!pop->GetName().IsEmpty() && (flags & OUT_NAME)) || (flags & OUT_NAME_FORCE) ) {
        wxCharBuffer buffer = pop->GetName().ToUTF8();
        if(buffer.data()) {
            child = node.append_child("name");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }       

    if ( (!pop->GetDescription().IsEmpty() && (flags & OUT_DESC)) || (flags & OUT_DESC_FORCE) ) {
        wxCharBuffer buffer = pop->GetDescription().ToUTF8();
        if(buffer.data()) {
            child = node.append_child("desc");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }       

    if(pop->m_sTypeString == wxT("Text Point")) {
        TextPoint *tp = dynamic_cast<TextPoint *>(pop);
        assert(tp !=0);
        if ( !tp->GetPointText().IsEmpty() && (flags & OUT_POINTTEXT) ) {
            wxCharBuffer buffer=tp->GetPointText().ToUTF8();
            if(buffer.data()) {
                child = node.append_child("opencpn:text");
                child.append_child(pugi::node_pcdata).set_value(buffer.data());
                child = node.append_child("opencpn:text_position");
                s.Printf(_T("%i"), tp->m_iTextPosition);
                child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
                child = node.append_child("opencpn:text_maximum_width_type");
                s.Printf(_T("%i"), tp->m_iTextMaxWidthType);
                child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
                child = node.append_child("opencpn:text_maximum_width");
                s.Printf(_T("%i"), tp->m_iWrapLen);
                child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
            }
            child = node.append_child( "opencpn:text_colour" );
            child.append_child(pugi::node_pcdata).set_value( tp->m_colourTextColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() );
            child = node.append_child( "opencpn:background_colour" );
            child.append_child(pugi::node_pcdata).set_value( tp->m_colourTextBackgroundColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() );
            child = node.append_child( "opencpn:background_transparency" );
            s.Printf(_T("%i"), tp->m_iBackgroundTransparency );
            child.append_child(pugi::node_pcdata).set_value( s.mb_str() );
            child = node.append_child( "opencpn:natural_scale" );
            s.Printf(_T("%f"), tp->m_natural_scale );
            child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
            child = node.append_child( "opencpn:display_text_when" );
            s.Printf(_T("%i"), tp->m_iDisplayTextWhen );
            child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
            
            child = node.append_child("opencpn:font_info");
            pugi::xml_attribute size = child.append_attribute( "size" );
            size.set_value( tp->m_DisplayTextFont.GetPointSize() );
            pugi::xml_attribute family = child.append_attribute( "family" );
            family.set_value( tp->m_DisplayTextFont.GetFamily() );
            pugi::xml_attribute style = child.append_attribute( "style" );
            style.set_value( tp->m_DisplayTextFont.GetStyle() );
            pugi::xml_attribute weight = child.append_attribute( "weight" );
            weight.set_value( tp->m_DisplayTextFont.GetWeight() );
            pugi::xml_attribute underline = child.append_attribute( "underline" );
            underline.set_value( tp->m_DisplayTextFont.GetUnderlined() );
#if wxCHECK_VERSION(3,0,0) 
            pugi::xml_attribute strikethrough = child.append_attribute( "strikethrough" );
            strikethrough.set_value( tp->m_DisplayTextFont.GetStrikethrough() );
#endif
            pugi::xml_attribute face = child.append_attribute( "face" );
            face.set_value( tp->m_DisplayTextFont.GetFaceName().ToUTF8() );
            pugi::xml_attribute encoding = child.append_attribute( "encoding" );
            encoding.set_value( tp->m_DisplayTextFont.GetEncoding() );
            
        }
    }
    
    if(pop->m_sTypeString == wxT("Boundary Point")) {
        BoundaryPoint *bp = dynamic_cast<BoundaryPoint *>(pop);
        assert(bp != 0);
        child = node.append_child("opencpn:boundary_type");
        if( bp->m_bExclusionBoundaryPoint && !bp->m_bInclusionBoundaryPoint )
            child.append_child(pugi::node_pcdata).set_value( "Exclusion" );
        else if( !bp->m_bExclusionBoundaryPoint && bp->m_bInclusionBoundaryPoint )
            child.append_child(pugi::node_pcdata).set_value( "Inclusion" );
        else if( !bp->m_bExclusionBoundaryPoint && !bp->m_bInclusionBoundaryPoint )
            child.append_child(pugi::node_pcdata).set_value( "None" );
        else child.append_child(pugi::node_pcdata).set_value( "Exclusion" );
    }
    
    // Hyperlinks
    if(flags & OUT_HYPERLINKS ){
        HyperlinkList *linklist = pop->m_HyperlinkList;
        if( linklist && linklist->GetCount() ) {
            wxHyperlinkListNode *linknode = linklist->GetFirst();
            while( linknode ) {
                Hyperlink *link = linknode->GetData();
                
                pugi::xml_node child_link = node.append_child("link");;
                wxCharBuffer buffer=link->Link.ToUTF8();
                if(buffer.data())
                    child_link.append_attribute("href") = buffer.data();
                                
                buffer=link->DescrText.ToUTF8();
                if(buffer.data()) {
                    child = child_link.append_child("text");
                    child.append_child(pugi::node_pcdata).set_value(buffer.data());
                }
                
                buffer=link->LType.ToUTF8();
                if(buffer.data() && strlen(buffer.data()) > 0) {
                    child = child_link.append_child("type");
                    child.append_child(pugi::node_pcdata).set_value(buffer.data());
                }
                
                linknode = linknode->GetNext();
            }
        }
    }
    
    if (flags & OUT_SYM_FORCE) {
        child = node.append_child("sym");
        if (!pop->GetIconName().IsEmpty()) {
            child.append_child(pugi::node_pcdata).set_value(pop->GetIconName().mb_str());
        }
        else {
            child.append_child("empty");
        }
    }       
    
    if( (flags & OUT_GUID) || (flags & OUT_VIZ) || (flags & OUT_VIZ_NAME) || (flags & OUT_SHARED)
            || (flags & OUT_AUTO_NAME)  || (flags & OUT_EXTENSION) || (flags & OUT_SINGLEUSE) ) {
        
        //pugi::xml_node child_ext = node.append_child("extensions");
        
        if (!pop->m_GUID.IsEmpty() && (flags & OUT_GUID) ) {
            child = node.append_child("opencpn:guid");
            child.append_child(pugi::node_pcdata).set_value(pop->m_GUID.mb_str());
        }
        
        if(flags & OUT_VIZ) {
            child = node.append_child("opencpn:viz");
            if ( pop->m_bIsVisible )
                child.append_child(pugi::node_pcdata).set_value("1");
            else
                child.append_child(pugi::node_pcdata).set_value("0");
        }
            
        if(flags & OUT_VIZ_NAME) {
            child = node.append_child("opencpn:viz_name");
            if ( pop->m_bShowName )
                child.append_child(pugi::node_pcdata).set_value("1");
            else
                child.append_child(pugi::node_pcdata).set_value("0");
        }
        
        if((flags & OUT_AUTO_NAME) && pop->m_bDynamicName) {
            child = node.append_child("opencpn:auto_name");
            child.append_child(pugi::node_pcdata).set_value("1");
        }
        if((flags & OUT_SHARED) && pop->m_bKeepXPath) {
            child = node.append_child("opencpn:shared");
            child.append_child(pugi::node_pcdata).set_value("1");
        }
        if(flags & OUT_ARRIVAL_RADIUS) {
            child = node.append_child("opencpn:arrival_radius");
            s.Printf(_T("%.3f"), pop->GetODPointArrivalRadius());
            child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        }
        if(flags & OUT_OCPNPOINT_RANGE_RINGS) {
            child = node.append_child("opencpn:ODPoint_range_rings");
            pugi::xml_attribute viz = child.append_attribute( "visible" );
            viz.set_value( pop->m_bShowODPointRangeRings );
            pugi::xml_attribute number = child.append_attribute( "number" );
            number.set_value( pop->m_iODPointRangeRingsNumber );
            pugi::xml_attribute step = child.append_attribute( "step" );
            step.set_value( pop->m_fODPointRangeRingsStep );
            pugi::xml_attribute units = child.append_attribute( "units" );
            units.set_value( pop->m_iODPointRangeRingsStepUnits );
            pugi::xml_attribute colour = child.append_attribute( "colour" );
            colour.set_value( pop->m_wxcODPointRangeRingsColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() ) ;
            pugi::xml_attribute width = child.append_attribute( "width" );
            width.set_value( pop->m_iRangeRingWidth );
            pugi::xml_attribute style = child.append_attribute( "line_style" );
            style.set_value( pop->m_iRangeRingStyle );
        }
        if((flags & OUT_SINGLEUSE) && pop->m_bSingleUse) {
            child = node.append_child("opencpn:singleuse");
            child.append_child(pugi::node_pcdata).set_value("1");
        }
    }

#ifndef __WXMSW__
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, l_locale->ToAscii());
#else
    setlocale(LC_NUMERIC, l_locale->ToAscii());
#endif
    delete l_locale;
#endif
    
    return true;
}

bool ODNavObjectChanges::GPXCreatePath( pugi::xml_node node, ODPath *pInPath )
{
    ODPath *pPath;
    Boundary * pBoundary = NULL;
    EBL *pEBL = NULL;
    DR  *pDR = NULL;
    GZ  *pGZ = NULL;
    PIL *pPIL = NULL;
    
#ifndef __WXMSW__
    wxString *l_locale;
    l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, "C");
#else
    setlocale(LC_NUMERIC, "C");
#endif
#endif
    
    if(pInPath->m_sTypeString == wxT("Boundary")) {
        pBoundary = (Boundary *)pInPath;
        pPath = pBoundary;
    } else if(pInPath->m_sTypeString == wxT("EBL")) {
        pEBL = (EBL *)pInPath;
        pPath = pEBL;
    } else if(pInPath->m_sTypeString == wxT("DR")) {
        pDR = (DR *)pInPath;
        pPath = pDR;
    } else if(pInPath->m_sTypeString == wxT("Guard Zone")) {
        pGZ = (GZ *)pInPath;
        pPath = pGZ;
    } else if(pInPath->m_sTypeString == wxT("PIL")) {
        pPIL = (PIL *)pInPath;
        pPath = pPIL;
    } else
        pPath = pInPath;
    
    pugi::xml_node child;
    
    child = node.append_child("opencpn:type");
    child.append_child(pugi::node_pcdata).set_value(pPath->m_sTypeString.mb_str());
    
    if( pPath->m_PathNameString.Len() ) {
        wxCharBuffer buffer=pPath->m_PathNameString.ToUTF8();
        if(buffer.data()) {
            child = node.append_child("name");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }
    
    if( pPath->m_PathDescription.Len() ) {
        wxCharBuffer buffer=pPath->m_PathDescription.ToUTF8();
        if(buffer.data()) {
            child = node.append_child("desc");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }
    
    // Hyperlinks
    HyperlinkList *linklist = pPath->m_HyperlinkList;
    if( linklist && linklist->GetCount() ) {
        wxHyperlinkListNode *linknode = linklist->GetFirst();
        while( linknode ) {
            Hyperlink *link = linknode->GetData();
        
            pugi::xml_node child_link = node.append_child("link");
            wxCharBuffer buffer = link->Link.ToUTF8();
            if(buffer.data())
                child_link.append_attribute("href") = buffer.data();
            
        
            buffer=link->DescrText.ToUTF8();
            if(buffer.data()) {
                child = child_link.append_child("text");
                child.append_child(pugi::node_pcdata).set_value(buffer.data());
            }

            buffer=link->LType.ToUTF8();
            if(buffer.data() && strlen(buffer.data()) > 0) {
                child = child_link.append_child("type");
                child.append_child(pugi::node_pcdata).set_value(buffer.data());
            }
            
            linknode = linknode->GetNext();
        }
    }
    
    child = node.append_child("opencpn:guid");
    child.append_child(pugi::node_pcdata).set_value(pPath->m_GUID.mb_str());
    
    child = node.append_child("opencpn:viz");
    child.append_child(pugi::node_pcdata).set_value(pPath->IsVisible() == true ? "1" : "0");

    child = node.append_child("opencpn:active");
    child.append_child(pugi::node_pcdata).set_value(pPath->m_bPathIsActive == true ? "1" : "0");
    
    child = node.append_child("opencpn:style");
    
    pugi::xml_attribute activecolour = child.append_attribute("active_colour");
    activecolour.set_value( pPath->m_wxcActiveLineColour.GetAsString( wxC2S_CSS_SYNTAX ).utf8_str() );
    if(pBoundary) {
        pugi::xml_attribute activefillcolour = child.append_attribute("active_fillcolour");
        activefillcolour.set_value( pBoundary->m_wxcActiveFillColour.GetAsString( wxC2S_CSS_SYNTAX ).utf8_str() );
    }
    if(pGZ) {
        pugi::xml_attribute activefillcolour = child.append_attribute("active_fillcolour");
        activefillcolour.set_value( pGZ->m_wxcActiveFillColour.GetAsString( wxC2S_CSS_SYNTAX ).utf8_str() );
    }
    pugi::xml_attribute inactivecolour = child.append_attribute("inactive_colour");
    inactivecolour.set_value( pPath->m_wxcInActiveLineColour.GetAsString( wxC2S_CSS_SYNTAX ).utf8_str() );
    if(pBoundary) {
        pugi::xml_attribute inactivefillcolour = child.append_attribute("inactive_fillcolour");
        inactivefillcolour.set_value( pBoundary->m_wxcInActiveFillColour.GetAsString( wxC2S_CSS_SYNTAX ).utf8_str() );
    }
    if(pGZ) {
        pugi::xml_attribute inactivefillcolour = child.append_attribute("inactive_fillcolour");
        inactivefillcolour.set_value( pGZ->m_wxcInActiveFillColour.GetAsString( wxC2S_CSS_SYNTAX ).utf8_str() );
    }
    child.append_attribute("width") = pPath->m_width;
    child.append_attribute("style") = pPath->m_style;
    if(pBoundary) {
        child.append_attribute("fill_transparency") = pBoundary->m_uiFillTransparency;
        child.append_attribute("inclusion_boundary_size") = pBoundary->m_iInclusionBoundarySize;
    }
    if(pGZ) {
        child.append_attribute("fill_transparency") = pGZ->m_uiFillTransparency;
    }
    if(pBoundary) {
        child = node.append_child("opencpn:boundary_type");
        if( pBoundary->m_bExclusionBoundary && !pBoundary->m_bInclusionBoundary )
            child.append_child(pugi::node_pcdata).set_value( "Exclusion" );
        else if( !pBoundary->m_bExclusionBoundary && pBoundary->m_bInclusionBoundary )
            child.append_child(pugi::node_pcdata).set_value( "Inclusion" );
        else if( !pBoundary->m_bExclusionBoundary && !pBoundary->m_bInclusionBoundary )
            child.append_child(pugi::node_pcdata).set_value( "None" );
        else child.append_child(pugi::node_pcdata).set_value( "Exclusion" );
    }
    if(pBoundary) {
        child = node.append_child("opencpn:boundary_show_point_icons");
        child.append_child(pugi::node_pcdata).set_value( pBoundary->m_bODPointsVisible == true ? "1" : "0" );
    }
    if(pEBL) {
        child = node.append_child("opencpn:persistence");
        wxString s;
        s.Printf(_T("%1i"), pEBL->m_iPersistenceType);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:show_arrow");
        child.append_child(pugi::node_pcdata).set_value( pEBL->m_bDrawArrow == true ? "1" : "0" );
        child = node.append_child("opencpn:VRM");
        child.append_child(pugi::node_pcdata).set_value( pEBL->m_bVRM == true ? "1" : "0" );
        child = node.append_child("opencpn:always_show_info");
        child.append_child(pugi::node_pcdata).set_value( pEBL->m_bAlwaysShowInfo == true ? "1" : "0" );
        child = node.append_child("opencpn:PerpLine");
        child.append_child(pugi::node_pcdata).set_value( pEBL->m_bPerpLine == true ? "1" : "0" );
        child = node.append_child("opencpn:fixed_end_position");
        child.append_child(pugi::node_pcdata).set_value( pEBL->m_bFixedEndPosition == true ? "1" : "0" );
        child = node.append_child("opencpn:centre_on_boat");
        child.append_child(pugi::node_pcdata).set_value( pEBL->m_bCentreOnBoat == true ? "1" : "0" );
        child = node.append_child("opencpn:rotate_with_boat");
        child.append_child(pugi::node_pcdata).set_value( pEBL->m_bRotateWithBoat == true ? "1" : "0" );
        child = node.append_child("opencpn:maintain_with");
        s.Printf(_T("%1i"), pEBL->m_iMaintainWith);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:EBL_angle");
        s.Printf(_T("%0.2f"), pEBL->m_dEBLAngle);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        if(pEBL->m_dLength > 0.) {
            child = node.append_child("opencpn:EBL_length");
            s.Printf(_T("%0.2f"), pEBL->m_dLength);
            child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        }
    }
    if(pDR) {
        child = node.append_child("opencpn:persistence");
        wxString s;
        s.Printf(_T("%1i"), pDR->m_iPersistenceType);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:DRSOG");
        s.Printf(_T("%0.3f"), pDR->m_dSoG);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:DRCOG");
        s.Printf(_T("%i"), pDR->m_iCoG);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:DRLength");
        s.Printf(_T("%0.3f"), pDR->m_dDRPathLength);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:DRLengthNM");
        s.Printf(_T("%0.3f"), pDR->m_dTotalLengthNM);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:DRPointIterval");
        s.Printf(_T("%0.3f"), pDR->m_dDRPointInterval);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:DRPointItervalNM");
        s.Printf(_T("%0.3f"), pDR->m_dDRPointIntervalNM);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:DRLengthType");
        s.Printf(_T("%i"), pDR->m_iLengthType);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:DRIntervalType");
        s.Printf(_T("%i"), pDR->m_iIntervalType);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:DRDistanceUnits");
        s.Printf(_T("%i"), pDR->m_iDistanceUnits);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:DRTimeUnits");
        s.Printf(_T("%i"), pDR->m_iTimeUnits);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
    }
    if(pGZ) {
        child = node.append_child("opencpn:persistence");
        wxString s;
        s.Printf(_T("%1i"), pGZ->m_iPersistenceType);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:centre_on_boat");
        child.append_child(pugi::node_pcdata).set_value( pGZ->m_bCentreOnBoat == true ? "1" : "0" );
        child = node.append_child("opencpn:rotate_with_boat");
        child.append_child(pugi::node_pcdata).set_value( pGZ->m_bRotateWithBoat == true ? "1" : "0" );
        child = node.append_child("opencpn:maintain_with");
        s.Printf(_T("%1i"), pGZ->m_iMaintainWith);
        child = node.append_child("opencpn:GZ_LineType");
        s.Printf(_T("%1i"), pGZ->m_iLineType);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:GZ_CentreLat");
        s.Printf(_T("%0.9f"), pGZ->m_dCentreLat);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:GZ_CentreLon");
        s.Printf(_T("%0.9f"), pGZ->m_dCentreLon);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:GZ_FirstLineDirection");
        s.Printf(_T("%0.2f"), pGZ->m_dFirstLineDirection);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:GZ_SecondLineDirection");
        s.Printf(_T("%0.2f"), pGZ->m_dSecondLineDirection);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:GZ_FirstDistance");
        s.Printf(_T("%0.3f"), pGZ->m_dFirstDistance);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:GZ_SecondDistance");
        s.Printf(_T("%0.3f"), pGZ->m_dSecondDistance);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
    }
    if(pPIL) {
        child = node.append_child("opencpn:persistence");
        wxString s;
        s.Printf(_T("%1i"), pPIL->m_iPersistenceType);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        child = node.append_child("opencpn:PIL_angle");
        s.Printf(_T("%0.2f"), pPIL->m_dEBLAngle);
        child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        if(pPIL->m_dLength > 0.) {
            child = node.append_child("opencpn:PIL_length");
            s.Printf(_T("%0.2f"), pPIL->m_dLength);
            child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        }
        std::list<PILLINE>::iterator it = pPIL->m_PilLineList.begin();
        pugi::xml_node PILchild;
        PILchild = node.append_child("opencpn:PILList");
        while(it != pPIL->m_PilLineList.end()) {
            child = PILchild.append_child("opencpn:PILITEM");
            child.append_attribute("ID") = it->iID;
            child.append_attribute("Offset") = it->dOffset;
            child.append_attribute("Style") = it->iStyle;
            child.append_attribute("Width") = it->dWidth;
            child.append_attribute("Description") = it->sDescription.mbc_str();
            child.append_attribute("Name") = it->sName.mbc_str();
            child.append_attribute("ActiveColour") = it->wxcActiveColour.GetAsString( wxC2S_CSS_SYNTAX ).utf8_str();
            child.append_attribute("InActiveColour") = it->wxcInActiveColour.GetAsString( wxC2S_CSS_SYNTAX ).utf8_str();
            ++it;
        }

    }

    ODPointList *pODPointList = pPath->m_pODPointList;
    wxODPointListNode *node2 = pODPointList->GetFirst();
    ODPoint *pop;
    
    while( node2  ) {
        if(pBoundary)
            pop = dynamic_cast<BoundaryPoint *>(node2->GetData());
        else 
            pop = node2->GetData();
        assert(pop != 0);
            
        GPXCreateODPoint(node.append_child("opencpn:ODPoint"), pop, OPT_OCPNPOINT);
            
        node2 = node2->GetNext();
    }
    
#ifndef __WXMSW__
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, l_locale->ToAscii());
#else
    setlocale(LC_NUMERIC, l_locale->ToAscii());
#endif
    delete l_locale;
#endif
    
    return true;
}
                    
bool ODNavObjectChanges::AddPath( ODPath *pb, const char *action )
{
    if( !m_ODchanges_file ) return false;

    SetRootGPXNode();

    pugi::xml_node object = m_gpx_root.append_child("opencpn:path");
    GPXCreatePath(object, pb );
    
    pugi::xml_node child = object.append_child("opencpn:action");
    child.append_child(pugi::node_pcdata).set_value(action);

    pugi::xml_writer_file writer(m_ODchanges_file);
    object.print(writer, " ");
#ifdef __WXMSW__
    fclose(m_ODchanges_file);
    m_ODchanges_file = fopen(m_ODfilename.mb_str(), "a");
#else
    fflush(m_ODchanges_file);
#endif
    
    return true;
}

bool ODNavObjectChanges::AddODPoint( ODPoint *pOP, const char *action )
{
    SetRootGPXNode();
    
    pugi::xml_node object = m_gpx_root.append_child("opencpn:ODPoint");
    GPXCreateODPoint(object, pOP, OPT_OCPNPOINT);

//    pugi::xml_node xchild = child_ext.child("extensions");
    pugi::xml_node child = object.append_child("opencpn:action");
    child.append_child(pugi::node_pcdata).set_value(action);

    pugi::xml_writer_file writer(m_ODchanges_file);
    object.print(writer, " ");
#ifdef __WXMSW__
    fclose(m_ODchanges_file);
    m_ODchanges_file = fopen(m_ODfilename.mb_str(), "a");
#else
    fflush(m_ODchanges_file);
#endif
    
    return true;
}

bool ODNavObjectChanges::AddGPXPathsList( PathList *pPaths )
{
    SetRootGPXNode();
    
    m_bFirstPath = true;
    wxPathListNode* pPath = pPaths->GetFirst();
    while (pPath) {
        ODPath* pPData = pPath->GetData();
        AddGPXPath( pPData);
        pPath = pPath->GetNext();
    }
    
    return true;
}

bool ODNavObjectChanges::AddGPXPath(ODPath *pPath)
{
    SetRootGPXNode();
    if ( m_bFirstPath ) {
        m_bFirstPath = false;
    }
    GPXCreatePath(m_gpx_root.append_child("opencpn:path"), pPath);
    return true;
}

bool ODNavObjectChanges::AddGPXODPoint(ODPoint *pWP )
{
    SetRootGPXNode();
    GPXCreateODPoint(m_gpx_root.append_child("opencpn:ODPoint"), pWP, OPT_OCPNPOINT);
    return true;
}

bool ODNavObjectChanges::AddGPXODPointsList( ODPointList *pODPoints )
{
    SetRootGPXNode();
    
    wxODPointListNode* pODPointNode = pODPoints->GetFirst();
    while (pODPointNode) {
        ODPoint* pRP = pODPointNode->GetData();
        AddGPXODPoint( pRP);
        pODPointNode = pODPointNode->GetNext();
    }
    
    return true;
}

bool ODNavObjectChanges::CreateNavObjGPXPoints( void )
{
    
    //    Iterate over the ODPoint list, creating Nodes for
    //    points that are not in any Path
    //    as indicated by m_bIsolatedMark == false
    
    if(!g_pODPointMan)
        return false;
    
    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();
    
    ODPoint *pOP;
    
    while( node ) {
        pOP = node->GetData();
        
        if( ( pOP->m_bIsolatedMark ) && !( pOP->m_bIsInLayer ) && !(pOP->m_bTemporary) )
        {
            GPXCreateODPoint(m_gpx_root.append_child("opencpn:ODPoint"), pOP, OPT_OCPNPOINT);
        }
        node = node->GetNext();
    }
    
    return true;
}

bool ODNavObjectChanges::CreateNavObjGPXPaths( void )
{
    pugi::xml_node child_ext;
    // Paths
    wxPathListNode *node1 = g_pPathList->GetFirst();
//    child_ext = m_gpx_root.append_child("extensions");
    ODPath *pPath = NULL;
    Boundary *pBoundary = NULL;
    EBL *pEBL = NULL;
    DR *pDR = NULL;
    GZ *pGZ = NULL;
    PIL *pPIL = NULL;
    while( node1 ) {
        pPath = (ODPath *)node1->GetData();
        pBoundary = NULL;
        pEBL = NULL;
        pDR = NULL;
        pGZ = NULL;
        pPIL = NULL;
        if(pPath->m_sTypeString == wxT("Boundary")) {
            pBoundary = (Boundary *)node1->GetData();
            pPath = pBoundary;
        } else if(pPath->m_sTypeString == wxT("EBL")) {
            pEBL = (EBL *)node1->GetData();
            pPath = pEBL;
        } else if(pPath->m_sTypeString == wxT("DR")) {
            pDR = (DR *)node1->GetData();
            pPath = pDR;
        } else if(pPath->m_sTypeString == wxT("Guard Zone")) {
            pGZ = (GZ *)node1->GetData();
            pPath = pGZ;
        } else if(pPath->m_sTypeString == wxT("PIL")) {
            pPIL = (PIL *)node1->GetData();
            pPath = pPIL;
        }
        if(pEBL) {
            if(pEBL->m_iPersistenceType != ID_NOT_PERSISTENT) {
                if( !pPath->m_bIsInLayer && !pPath->m_bTemporary )
                    GPXCreatePath(m_gpx_root.append_child("opencpn:path"), pPath);
            }
        } else if(pDR) {
            if(pDR->m_iPersistenceType != ID_NOT_PERSISTENT) {
                if( !pPath->m_bIsInLayer && !pPath->m_bTemporary )
                    GPXCreatePath(m_gpx_root.append_child("opencpn:path"), pPath);
            }
        } else if(pGZ) {
            if(pGZ->m_iPersistenceType != ID_NOT_PERSISTENT) {
                if( !pPath->m_bIsInLayer && !pPath->m_bTemporary )
                    GPXCreatePath(m_gpx_root.append_child("opencpn:path"), pPath);
            }
        } else if(pPIL) {
            if(pPIL->m_iPersistenceType != ID_NOT_PERSISTENT) {
                if( !pPath->m_bIsInLayer && !pPath->m_bTemporary )
                    GPXCreatePath(m_gpx_root.append_child("opencpn:path"), pPath);
            }
        } else {
            if( !pPath->m_bIsInLayer)
                GPXCreatePath(m_gpx_root.append_child("opencpn:path"), pPath);
        }
        
        node1 = node1->GetNext();
    }
    
    
    return true;
}

bool ODNavObjectChanges::CreateAllGPXObjects()
{
    SetRootGPXNode();
    CreateNavObjGPXPaths();
    CreateNavObjGPXPoints();
    
    return true;
}

void ODNavObjectChanges::SetRootGPXNode(void)
{
    if(!strlen(m_gpx_root.name())) {
        m_gpx_root = append_child("OCPNDraw");
        m_gpx_root.append_attribute ( "version" ) = "0.1";
        m_gpx_root.append_attribute ( "creator" ) = "OpenCPN";
        m_gpx_root.append_attribute( "xmlns:xsi" ) = "http://www.w3.org/2001/XMLSchema-instance";
        m_gpx_root.append_attribute( "xmlns:opencpn" ) = "http://www.opencpn.org";
    }
}

bool ODNavObjectChanges::LoadAllGPXObjects( bool b_full_viz )
{
    pugi::xml_node objects = this->child("OCPNDraw");
    
    for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling())
    {
        if( !strcmp(object.name(), "opencpn:ODPoint") ) {
            ODPoint *pOp = GPXLoadODPoint1( object, _T("circle"), _T(""), b_full_viz, false, false, 0 );
            
            if(pOp) {
                pOp->m_bIsolatedMark = true;      // This is an isolated mark
                ODPoint *pExisting = ODPointExists( pOp->GetName(), pOp->m_lat, pOp->m_lon );
                if( !pExisting ) {
                    if( NULL != g_pODPointMan )
                        g_pODPointMan->AddODPoint( pOp );
                    g_pODSelect->AddSelectableODPoint( pOp->m_lat, pOp->m_lon, pOp );
                }
                else
                    delete pOp;
            }
        }
            else
                if( !strcmp(object.name(), "opencpn:path") ) {
                    // find object type 
                    wxString TypeString;
                    for( pugi::xml_node child = object.first_child(); child != 0; child = child.next_sibling() ) {
                        const char *pcn = child.name();
                        if( !strcmp( pcn, "opencpn:type") ) {
                                TypeString = wxString::FromUTF8( child.first_child().value() );
                                break;
                        }
                    }
                    if ( !TypeString.compare( wxS("Boundary") ) || !TypeString.compare( wxS("EBL") ) || 
                        !TypeString.compare( wxS("DR") ) || !TypeString.compare( wxS("Guard Zone") ) || !TypeString.compare( wxS("PIL") ) ) {
                        ODPath *pPath = GPXLoadPath1( object, b_full_viz, false, false, 0, &TypeString );
                        InsertPathA( pPath );
                    }
                }
                
                
    }
    
    return true;
}

ODPoint * ODNavObjectChanges::GPXLoadODPoint1( pugi::xml_node &opt_node, 
                            wxString def_symbol_name,
                            wxString GUID,
                            bool b_fullviz,
                            bool b_layer,
                            bool b_layerviz,
                            int layer_id,
                            bool b_InPath
                            )
{
#ifndef __WXMSW__
    wxString *l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, "C");
#else
    setlocale(LC_NUMERIC, "C");
#endif
#endif
    
    bool bviz = false;
    bool bviz_name = false;
    bool bauto_name = false;
    bool bshared = false;
    bool bSingleUse = false;
    bool b_propvizname = false;
    bool b_propviz = false;

    wxString SymString = def_symbol_name;       // default icon
    wxString NameString;
    wxString DescString;
    wxString TextString;
    wxString TypeString;
    wxString GuidString = GUID;         // default
    wxString TimeString;
    wxDateTime dt;
    ODPoint *pOP = NULL;
    TextPoint *pTP = NULL;
    BoundaryPoint *pBP = NULL;
    
    HyperlinkList *linklist = NULL;

    double rlat = opt_node.attribute( "lat" ).as_double();
    double rlon = opt_node.attribute( "lon" ).as_double();
    double ArrivalRadius = 0;
    int            l_iTextPointFontSize = g_DisplayTextFont.GetPointSize();
	wxFontFamily   l_iTextPointFontFamily = g_DisplayTextFont.GetFamily();
    int            l_iTextPointFontStyle = g_DisplayTextFont.GetStyle();
    int            l_iTextPointFontWeight = g_DisplayTextFont.GetWeight();
    bool           l_bTextPointFontUnderline = g_DisplayTextFont.GetUnderlined();
#if wxCHECK_VERSION(3,0,0) 
    bool           l_bTextPointFontStrikethrough = g_DisplayTextFont.GetStrikethrough();
#else
    bool           l_bTextPointFontStrikethrough = false;
#endif
    wxString    l_wxsTextPointFontFace = g_DisplayTextFont.GetFaceName();
    int     l_iTextPointFontEncoding = g_DisplayTextFont.GetEncoding();
    int     l_iODPointRangeRingsNumber = -1;
    float   l_fODPointRangeRingsStep = -1;
    int     l_pODPointRangeRingsStepUnits = -1;
    bool    l_bODPointRangeRingsVisible = false;
    int     l_iTextPosition = g_iTextPosition;
    int     l_iTextMaxWidthType = g_iTextMaxWidthType;
    int     l_iWrapLen = g_iTextMaxWidth;
    wxColour    l_wxcODPointRangeRingsColour;
    int     l_iODPointRangeRingWidth = 2;
    wxPenStyle  l_iODPointRangeRingStyle = wxPENSTYLE_SOLID;
    wxColour    l_colourTextColour = g_colourDefaultTextColour;
    wxColour    l_colourBackgroundColour = g_colourDefaultTextBackgroundColour;
    int     l_iBackgroundTransparency = g_iTextBackgroundTransparency;
    bool    l_bExclusionBoundaryPoint = g_bExclusionBoundaryPoint;
    bool    l_bInclusionBoundaryPoint = g_bInclusionBoundaryPoint;
    int     l_iInclusionBoundaryPointSize = g_iInclusionBoundaryPointSize;
    unsigned int l_uiBoundaryPointFillTransparency = g_uiBoundaryPointFillTransparency;
    double  l_natural_scale = g_ocpn_draw_pi->m_chart_scale;
    int     l_display_text_when = ID_TEXTPOINT_DISPLAY_TEXT_SHOW_ALWAYS;
    
    l_wxcODPointRangeRingsColour.Set( _T( "#FFFFFF" ) );

    for( pugi::xml_node child = opt_node.first_child(); child != 0; child = child.next_sibling() ) {
        const char *pcn = child.name();
        if( !strcmp( pcn, "sym" ) ) {
            SymString.clear();
            SymString.append( wxString::FromUTF8( child.first_child().value() ) );
        }
        else if( !strcmp( pcn, "time") ) 
            TimeString.append( wxString::FromUTF8( child.first_child().value() ) );

        else if( !strcmp( pcn, "name") ) {
            NameString.append( wxString::FromUTF8( child.first_child().value() ) );
        } else if( !strcmp( pcn, "desc") ) {
            DescString.append( wxString::FromUTF8( child.first_child().value() ) );
        } else if( !strcmp( pcn, "opencpn:text") ) {
            TextString.append( wxString::FromUTF8( child.first_child().value() ) );
        } else if( !strcmp( pcn, "opencpn:text_position") ) {
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                l_iTextPosition = v;
        } else if( !strcmp( pcn, "opencpn:text_width_type") ) {
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                l_iTextMaxWidthType = v;
        } else if( !strcmp( pcn, "opencpn:text_maximum_width") ) {
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                l_iWrapLen = v;
        } else if( !strcmp( pcn, "opencpn:text_colour") ) {
            l_colourTextColour.Set( wxString::FromUTF8( child.first_child().value() ) );

        } else if ( !strcmp( pcn, "opencpn:font_info") ) {
                for ( pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute() ) {
                    if ( wxString::FromUTF8(attr.name()) == _T("size") )
                        l_iTextPointFontSize = attr.as_int();
                    else if ( wxString::FromUTF8(attr.name()) == _T("family") )
                        l_iTextPointFontFamily = (wxFontFamily)attr.as_int();
                    else if ( wxString::FromUTF8(attr.name()) == _T("style") )
                        l_iTextPointFontStyle = attr.as_int();
                    else if ( wxString::FromUTF8(attr.name()) == _T("weight") )
                        l_iTextPointFontWeight = attr.as_int();
                    else if ( wxString::FromUTF8(attr.name()) == _T("underline") )
                        l_bTextPointFontUnderline =  attr.as_bool();
                    else if ( wxString::FromUTF8(attr.name()) == _T("strikethrough") )
                        l_bTextPointFontStrikethrough =  attr.as_bool();
                    else if ( wxString::FromUTF8(attr.name()) == _T("face") ) {
                        l_wxsTextPointFontFace.clear();
                        l_wxsTextPointFontFace.Append( wxString::FromUTF8( attr.as_string() ) );
                    }
                    else if ( wxString::FromUTF8(attr.name()) == _T("encoding") )
                        l_iTextPointFontEncoding = attr.as_int();
                }
                
        } else if( !strcmp( pcn, "opencpn:background_colour") ) {
            l_colourBackgroundColour.Set( wxString::FromUTF8( child.first_child().value() ) );
        } else if( !strcmp( pcn, "opencpn:background_transparency") ) {
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                l_iBackgroundTransparency = v;
        } else if( !strcmp( pcn, "opencpn:type") ) {
            TypeString.append( wxString::FromUTF8( child.first_child().value() ) );
        } else              // Read hyperlink
        if( !strcmp( pcn, "link") ) {
            wxString HrefString;
            wxString HrefTextString;
            wxString HrefTypeString;
            if( linklist == NULL )
                linklist = new HyperlinkList;
            HrefString.append( wxString::FromUTF8( child.first_attribute().value() ) );

            for( pugi::xml_node child1 = child.first_child(); child1; child1 = child1.next_sibling() ) {
                wxString LinkString = wxString::FromUTF8( child1.name() );

                if( LinkString == _T ( "text" ) )
                    HrefTextString.append( wxString::FromUTF8( child1.first_child().value() ) );
                if( LinkString == _T ( "type" ) ) 
                    HrefTypeString.append( wxString::FromUTF8( child1.first_child().value() ) );
            }
        
            Hyperlink *link = new Hyperlink;
            link->Link = HrefString;
            link->DescrText = HrefTextString;
            link->LType = HrefTypeString;
            linklist->Append( link );
        }

    //    OpenCPN Extensions....
        else if( !strcmp( pcn, "opencpn:viz" ) ) {
            b_propviz = true;
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                bviz = ( v != 0 );
        } else if( !strcmp( pcn, "opencpn:viz_name") ) {
            b_propvizname = true;
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                bviz_name = ( v != 0 );
        } else if( !strcmp( pcn, "opencpn:guid" ) ) {
            GuidString.clear();
            GuidString.append( wxString::FromUTF8(child.first_child().value()) );
        } else if( !strcmp( pcn, "opencpn:auto_name" ) ) {
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                bauto_name = ( v != 0 );
        } else if( !strcmp( pcn, "opencpn:shared" ) ) {
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                bshared = ( v != 0 );
        } else if( !strcmp( pcn, "opencpn:singleuse" ) ) {
                wxString s = wxString::FromUTF8( child.first_child().value() );
                long v = 0;
                if( s.ToLong( &v ) )
                    bSingleUse = ( v != 0 );
        } else if( !strcmp( pcn, "opencpn:arrival_radius" ) ) {
            wxString::FromUTF8(child.first_child().value()).ToDouble(&ArrivalRadius ) ;
        } else if ( !strcmp( pcn, "opencpn:ODPoint_range_rings") ) {
            for ( pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute() ) {
                if ( wxString::FromUTF8(attr.name()) == _T("number") )
                    l_iODPointRangeRingsNumber = attr.as_int();
                else if ( wxString::FromUTF8(attr.name()) == _T("step") )
                    l_fODPointRangeRingsStep = attr.as_float();
                else if ( wxString::FromUTF8(attr.name()) == _T("units") )
                    l_pODPointRangeRingsStepUnits = attr.as_int();
                else if ( wxString::FromUTF8(attr.name()) == _T("visible") )
                    l_bODPointRangeRingsVisible =  attr.as_bool();
                else if ( wxString::FromUTF8(attr.name()) == _T("colour") )
                    l_wxcODPointRangeRingsColour.Set( wxString::FromUTF8( attr.as_string() ) );
                else if ( wxString::FromUTF8(attr.name()) == _T("width") )
                    l_iODPointRangeRingWidth = attr.as_int();
                else if ( wxString::FromUTF8(attr.name()) == _T("line_style") )
                    l_iODPointRangeRingStyle = (wxPenStyle)attr.as_int();
            }
        } else if ( !strcmp( pcn, "opencpn:boundary_type" ) ) {
            wxString s = wxString::FromUTF8( child.first_child().value() );
            if( s == _T("Exclusion") ) {
                l_bExclusionBoundaryPoint = true;
                l_bInclusionBoundaryPoint = false;
            } else if( s == _T("Inclusion") ) {
                l_bExclusionBoundaryPoint = false;
                l_bInclusionBoundaryPoint = true;
            } else if( s == _T("Neither") ) {
                l_bExclusionBoundaryPoint = false;
                l_bInclusionBoundaryPoint = false;
            } else l_bExclusionBoundaryPoint = false;
        } else if ( !strcmp( pcn, "opencpn:boundary_point_style" ) ) {
            for ( pugi::xml_attribute attr = child.first_attribute(); attr; attr = attr.next_attribute() ) {
                if ( wxString::FromUTF8(attr.name()) == _T("fill_transparency") )
                    l_uiBoundaryPointFillTransparency = attr.as_uint();
                else if ( wxString::FromUTF8(attr.name()) == _T("inclusion_fill_size") )
                    l_iInclusionBoundaryPointSize = attr.as_int();
                
            }
        } else if ( !strcmp( pcn, "opencpn:natural_scale" ) ){
        wxString::FromUTF8(child.first_child().value()).ToDouble( &l_natural_scale );
        } else if ( !strcmp( pcn, "opencpn:display_text_when" ) ){
            wxString::FromUTF8(child.first_child().value()).ToLong( (long *) &l_display_text_when );
        }
    }   // for

    // Create ODPoint

    if( b_layer ) {
        if( GuidString.IsEmpty() )
            GuidString = _T("LayGUID");
    }

    // Check to see if this point already exits
    pOP = tempODPointExists( GuidString );
    if(!pOP) pOP = ODPointExists( GuidString );
    if( !pOP ) {
        if( TypeString == wxT("Text Point") ) {
            pTP = new TextPoint( rlat, rlon, SymString, NameString, GuidString, false );
            pOP = pTP;
        } else if( TypeString == wxT("Boundary Point") ) {
            pBP = new BoundaryPoint( rlat, rlon, SymString, NameString, GuidString, false );
            pOP = pBP;
        }
        else {
            pOP = new ODPoint( rlat, rlon, SymString, NameString, GuidString, false ); // do not add to global WP list yet...
            pOP->m_sTypeString = TypeString;
        }
            
        m_ptODPointList->Append( pOP ); 
    } else {
        if(pOP->m_sTypeString == wxT("Text Point")) {
            pTP = dynamic_cast<TextPoint *>(pOP);
            assert(pTP);
        }
        else if(pOP->m_sTypeString == wxT("Boundary Point")){
            pBP = dynamic_cast<BoundaryPoint *>(pOP);
            assert(pBP);
        }
        
        pOP->m_lat = rlat;
        pOP->m_lon = rlon;
        pOP->m_IconName = SymString;
        pOP->SetName( NameString );
        pOP->SetMarkDescription( DescString );
        pOP->m_sTypeString = TypeString;
    }
    pOP->SetODPointArrivalRadius( ArrivalRadius );
    pOP->SetODPointRangeRingsNumber( l_iODPointRangeRingsNumber );
    pOP->SetODPointRangeRingsStep( l_fODPointRangeRingsStep );
    pOP->SetODPointRangeRingsStepUnits( l_pODPointRangeRingsStepUnits );
    pOP->SetShowODPointRangeRings( l_bODPointRangeRingsVisible );
    pOP->SetODPointRangeRingsColour( l_wxcODPointRangeRingsColour );
    pOP->SetODPointRangeRingWidth( l_iODPointRangeRingWidth );
    pOP->SetODPointRangeRingStyle( l_iODPointRangeRingStyle );

    if( pTP && TypeString == _T("Text Point") ) {
        pTP->SetPointText( TextString );
        pTP->m_iTextPosition = l_iTextPosition;
        pTP->m_iTextMaxWidthType = l_iTextMaxWidthType;
        pTP->m_iWrapLen = l_iWrapLen;
        pTP->m_colourTextColour = l_colourTextColour;
        pTP->m_DisplayTextFont.SetPointSize( l_iTextPointFontSize );
        pTP->m_DisplayTextFont.SetFamily( l_iTextPointFontFamily );
        pTP->m_DisplayTextFont.SetStyle( (wxFontStyle)l_iTextPointFontStyle );
        pTP->m_DisplayTextFont.SetWeight( (wxFontWeight)l_iTextPointFontWeight );
        pTP->m_DisplayTextFont.SetUnderlined( l_bTextPointFontUnderline );
#if wxCHECK_VERSION(3,0,0) 
        pTP->m_DisplayTextFont.SetStrikethrough( l_bTextPointFontStrikethrough );
#endif
        pTP->m_DisplayTextFont.SetFaceName( l_wxsTextPointFontFace );
        pTP->m_DisplayTextFont.SetEncoding( (wxFontEncoding)l_iTextPointFontEncoding );
        pTP->m_colourTextBackgroundColour = l_colourBackgroundColour;
        pTP->m_iBackgroundTransparency = l_iBackgroundTransparency;
        pTP->m_natural_scale = l_natural_scale;
        pTP->m_iDisplayTextWhen = l_display_text_when;
        pTP -> CreateColourSchemes();
        pTP->SetColourScheme(g_global_color_scheme);
    } else if ( pBP && TypeString == _T("Boundary Point") ) {
        pBP -> m_bExclusionBoundaryPoint = l_bExclusionBoundaryPoint;
        pBP -> m_bInclusionBoundaryPoint = l_bInclusionBoundaryPoint;
        pBP -> m_iInclusionBoundaryPointSize = l_iInclusionBoundaryPointSize;
        pBP -> m_uiBoundaryPointFillTransparency = l_uiBoundaryPointFillTransparency;
        pBP -> CreateColourSchemes();
        pBP->SetColourScheme(g_global_color_scheme);
    }
    

    if( b_propvizname )
        pOP->m_bShowName = bviz_name;
    else if( b_fullviz )
        pOP->m_bShowName = true;
    else
        pOP->m_bShowName = false;

    if( b_propviz )
        pOP->m_bIsVisible = bviz;
    else if( b_fullviz )
        pOP->m_bIsVisible = true;

    if(b_layer) {
        pOP->m_bIsInLayer = true;
        pOP->m_LayerID = layer_id;
        if(!b_InPath)
            pOP->m_bIsVisible = b_layerviz;
        pOP->SetListed( false );
    }

    pOP->m_bKeepXPath = bshared;
    pOP->m_bDynamicName = bauto_name;
    pOP->m_bSingleUse = bSingleUse;

    if(TimeString.Len()) {
        pOP->m_timestring = TimeString;
        pOP->SetCreateTime(wxInvalidDateTime);          // cause deferred timestamp parsing
    }
        

    if( linklist ) {
        delete pOP->m_HyperlinkList;                    // created in RoutePoint ctor
        pOP->m_HyperlinkList = linklist;
    }

#ifndef __WXMSW__
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, l_locale->ToAscii());
#else
    setlocale(LC_NUMERIC, l_locale->ToAscii());
#endif
    delete l_locale;
#endif    
    return ( pOP );
}

ODPath *ODNavObjectChanges::GPXLoadPath1( pugi::xml_node &odpoint_node  , bool b_fullviz,
                    bool b_layer,
                    bool b_layerviz,
                    int layer_id, wxString *pPathType )
{
    wxString Name = wxString::FromUTF8( odpoint_node.name() );
    if( Name != _T( "opencpn:path" ) ) {
        return 0;
    }
        
#ifndef __WXMSW__
    wxString *l_locale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, "C");
#else
    setlocale(LC_NUMERIC, "C");
#endif
#endif
    
    wxString PathName;
    wxString DescString;
    bool b_propviz = false;
    bool b_viz = true;
    bool b_active = false;
    Boundary    *pTentBoundary = NULL;
    EBL         *pTentEBL = NULL;
    DR          *pTentDR = NULL;
    GZ          *pTentGZ = NULL;
    PIL         *pTentPIL = NULL;
    ODPath        *pTentPath;
    HyperlinkList *linklist = NULL;
    
    if (!strcmp(pPathType->mb_str(), "Boundary" ) ) {
        pTentBoundary = new Boundary();
        pTentPath = pTentBoundary;
    } else if (!strcmp(pPathType->mb_str(), "EBL" ) ) {
        pTentEBL = new EBL();
        pTentPath = pTentEBL;
    } else if (!strcmp(pPathType->mb_str(), "DR" ) ) {
        pTentDR = new DR();
        pTentPath = pTentDR;
    } else if (!strcmp(pPathType->mb_str(), "Guard Zone" ) ) {
        pTentGZ = new GZ();
        pTentPath = pTentGZ;
    } else if (!strcmp(pPathType->mb_str(), "PIL" ) ) {
        pTentPIL = new PIL();
        pTentPath = pTentPIL;
    } else
        pTentPath = new ODPath();
    
    for( pugi::xml_node tschild = odpoint_node.first_child(); tschild; tschild = tschild.next_sibling() ) {
        wxString ChildName = wxString::FromUTF8( tschild.name() );

        if( ChildName == _T ( "opencpn:ODPoint" ) ) {
            ODPoint *tpOp = GPXLoadODPoint1(  tschild, _T("square"), _T(""), b_fullviz, b_layer, b_layerviz, layer_id, true );
            
            pTentPath->AddPoint( tpOp, false, true, true);          // defer BBox calculation
            if(pTentBoundary) {
                BoundaryPoint *l_pBP = dynamic_cast<BoundaryPoint *>(tpOp);
                assert(l_pBP);
                l_pBP->m_bIsInBoundary = true;
            }
            
            tpOp->m_bIsInPath = true;
            if(!ODPointExists( tpOp->m_GUID ))
                g_pODPointMan->AddODPoint(tpOp);
        }
        else if( ChildName == _T ( "name" ) ) {
            wxString l_name = wxString::FromUTF8( tschild.first_child().value() );
            if(l_name.StartsWith(wxT("GZ ")) && pTentGZ) {
                long i;
                l_name.Mid(3).ToLong(&i);
                if(g_iGZMaxNum <= i)
                    g_iGZMaxNum = i++;
            }
            PathName.append( wxString::FromUTF8( tschild.first_child().value() ) );
        }
        else if( ChildName == _T ( "desc" ) ) {
            DescString.append( wxString::FromUTF8( tschild.first_child().value() ) );
        }
            
        else if( ChildName == _T ( "link") ) {
            wxString HrefString;
            wxString HrefTextString;
            wxString HrefTypeString;
            if( linklist == NULL )
                linklist = new HyperlinkList;
            HrefString = wxString::FromUTF8( tschild.first_attribute().value() );

            for( pugi::xml_node child1 = tschild.first_child(); child1; child1 = child1.next_sibling() ) {
                wxString LinkString = wxString::FromUTF8( child1.name() );

                if( LinkString == _T ( "text" ) )
                    HrefTextString.append( wxString::FromUTF8( child1.first_child().value() ) );
                if( LinkString == _T ( "type" ) ) 
                    HrefTypeString.append( wxString::FromUTF8( child1.first_child().value() ) );
            }
        
            Hyperlink *link = new Hyperlink;
            link->Link = HrefString;
            link->DescrText = HrefTextString;
            link->LType = HrefTypeString;
            linklist->Append( link );
        }
        
        else if( ChildName == _T ( "opencpn:viz" ) ) {
            wxString viz = wxString::FromUTF8(tschild.first_child().value());
            b_propviz = true;
            b_viz = ( viz == _T("1") );
        }
        
        else if( ChildName == _T ( "opencpn:active" ) ) {
            wxString active = wxString::FromUTF8(tschild.first_child().value());
            b_active = ( active == _T("1") );
        }
        else if( ChildName == _T ( "opencpn:style" ) ) {
            for (pugi::xml_attribute attr = tschild.first_attribute(); attr; attr = attr.next_attribute())
            {
                if ( wxString::FromUTF8( attr.name() ) == _T("active_colour" ) )
                    pTentPath->m_wxcActiveLineColour.Set( wxString::FromUTF8( attr.as_string() ) );
                else if ( wxString::FromUTF8( attr.name() ) == _T("active_fillcolour" ) ) {
                    if(pTentPath->m_sTypeString == wxT("Boundary"))
                        pTentBoundary->m_wxcActiveFillColour.Set( wxString::FromUTF8( attr.as_string() ) );
                    else
                        pTentGZ->m_wxcActiveFillColour.Set( wxString::FromUTF8( attr.as_string() ) );
                }
                if ( wxString::FromUTF8( attr.name() ) == _T("inactive_colour" ) )
                    pTentPath->m_wxcInActiveLineColour.Set( wxString::FromUTF8( attr.as_string() ) );
                else if ( wxString::FromUTF8( attr.name() ) == _T("inactive_fillcolour" ) ) {
                    if(pTentPath->m_sTypeString == wxT("Boundary"))
                        pTentBoundary->m_wxcInActiveFillColour.Set( wxString::FromUTF8( attr.as_string() ) );
                    else
                        pTentGZ->m_wxcInActiveFillColour.Set( wxString::FromUTF8( attr.as_string() ) );
                }
                else if ( wxString::FromUTF8( attr.name() ) == _T("style" ) )
                    pTentPath->m_style = (wxPenStyle)attr.as_int();
                else if ( wxString::FromUTF8( attr.name() ) == _T("width" ) )
                    pTentPath->m_width = attr.as_int();
                else if ( wxString::FromUTF8( attr.name() ) == _T("fill_transparency") ) {
                    if(pTentPath->m_sTypeString == wxT("Boundary"))
                        pTentBoundary->m_uiFillTransparency = attr.as_uint();
                    else
                        pTentGZ->m_uiFillTransparency = attr.as_uint();
                }
                else if ( wxString::FromUTF8( attr.name() ) == _T("inclusion_boundary_size") )
                    pTentBoundary->m_iInclusionBoundarySize = attr.as_uint();
            }
        } else if( ChildName == _T( "opencpn:boundary_type") ) {
            wxString s = wxString::FromUTF8( tschild.first_child().value() );
            if( s == _T("Exclusion") ) {
                pTentBoundary->m_bExclusionBoundary = true;
                pTentBoundary->m_bInclusionBoundary = false;
            } else if( s == _T("Inclusion") ) {
                pTentBoundary->m_bExclusionBoundary = false;
                pTentBoundary->m_bInclusionBoundary = true;
            } else if( s == _T("Neither") ) {
                pTentBoundary->m_bExclusionBoundary = false;
                pTentBoundary->m_bInclusionBoundary = false;
            } else pTentBoundary->m_bExclusionBoundary = false;
        } else if( ChildName == _T( "opencpn:boundary_show_point_icons") ) {
            wxString active = wxString::FromUTF8(tschild.first_child().value());
            pTentBoundary->m_bODPointsVisible = ( active == _T("1") );
        } else if( ChildName == _T ( "opencpn:guid" ) ) {
            //if ( !g_bODIsNewLayer ) ) 
            pTentPath->m_GUID.clear();
            pTentPath->m_GUID.append( wxString::FromUTF8(tschild.first_child().value()) );
        } else if( ChildName == _T ( "opencpn:time_display" ) ) {
            pTentPath->m_TimeDisplayFormat.append( wxString::FromUTF8(tschild.first_child().value()) );
        } else if( ChildName == _T ( "opencpn:persistence" ) ) {
            wxString s = wxString::FromUTF8( tschild.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) ) {
                if (!strcmp(pPathType->mb_str(), "EBL" ) )
                    pTentEBL->SetPersistence( v );
                else if (!strcmp(pPathType->mb_str(), "DR" ) )
                    pTentDR->SetPersistence( v );
                else if (!strcmp(pPathType->mb_str(), "Guard Zone" ) )
                    pTentGZ->SetPersistence( v );
                else if (!strcmp(pPathType->mb_str(), "PIL" ) )
                    pTentPIL->SetPersistence( v );
            }
        } else if( ChildName == _T ( "opencpn:show_arrow" ) ) {
            wxString s = wxString::FromUTF8( tschild.first_child().value() );
            pTentEBL->m_bDrawArrow = ( s == wxT("1") );
        } else if( ChildName == _T ( "opencpn:VRM" ) ) {
            wxString s = wxString::FromUTF8( tschild.first_child().value() );
            pTentEBL->m_bVRM = ( s == wxT("1") );
        } else if( ChildName == _T ( "opencpn:always_show_info" ) ) {
            wxString s = wxString::FromUTF8( tschild.first_child().value() );
            pTentEBL->m_bAlwaysShowInfo = ( s == wxT("1") );
        } else if( ChildName == _T ( "opencpn:PerpLine" ) ) {
            wxString s = wxString::FromUTF8( tschild.first_child().value() );
            pTentEBL->m_bPerpLine = ( s == wxT("1") );
        } else if( ChildName == _T ( "opencpn:fixed_end_position" ) ) {
            wxString s = wxString::FromUTF8( tschild.first_child().value() );
            pTentEBL->m_bFixedEndPosition = ( s == wxT("1") );
        } else if( ChildName == _T ( "opencpn:centre_on_boat" ) ) {
            wxString s = wxString::FromUTF8( tschild.first_child().value() );
            if(pTentPath->m_sTypeString == wxT("EBL"))
                pTentEBL->m_bCentreOnBoat = ( s == wxT("1") );
            else
                pTentGZ->m_bCentreOnBoat = ( s == wxT("1") );
        } else if( ChildName == _T ( "opencpn:rotate_with_boat" ) ) {
            wxString s = wxString::FromUTF8( tschild.first_child().value() );
            if(pTentPath->m_sTypeString == wxT("EBL"))
                pTentEBL->m_bRotateWithBoat = ( s == wxT("1") );
            else
                pTentGZ->m_bRotateWithBoat = ( s == wxT("1") );
        } else if( ChildName == _T ( "opencpn:maintain_with" ) ) {
            if(pTentPath->m_sTypeString == wxT("EBL"))
                wxString::FromUTF8( tschild.first_child().value() ).ToLong( (long *)&pTentEBL->m_iMaintainWith );
            else
                wxString::FromUTF8( tschild.first_child().value() ).ToLong( (long *)&pTentGZ->m_iMaintainWith );
        } else if( ChildName == _T ( "opencpn:EBL_angle" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentEBL->m_dEBLAngle );
        } else if( ChildName == _T ( "opencpn:PIL_angle" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentPIL->m_dEBLAngle );
        } else if( ChildName == _T ( "opencpn:EBL_length" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentEBL->m_dLength );
        } else if( ChildName == _T ( "opencpn:PIL_length" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentPIL->m_dLength );
        } else if( ChildName == _T ( "opencpn:DRSOG" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentDR->m_dSoG );
        } else if( ChildName == _T ( "opencpn:DRCOG" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToLong( (long *)&pTentDR->m_iCoG );
        } else if( ChildName == _T ( "opencpn:DRLength" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentDR->m_dDRPathLength );
        } else if( ChildName == _T ( "opencpn:DRLengthNM" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentDR->m_dTotalLengthNM );
        } else if( ChildName == _T ( "opencpn:DRPointIterval" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentDR->m_dDRPointInterval );
        } else if( ChildName == _T ( "opencpn:DRPointItervalNM" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentDR->m_dDRPointIntervalNM );
        } else if( ChildName == _T ( "opencpn:DRLengthType" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToLong( (long *)&pTentDR->m_iLengthType );
        } else if( ChildName == _T ( "opencpn:DRIntervalType" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToLong( (long *)&pTentDR->m_iIntervalType );
        } else if( ChildName == _T ( "opencpn:DRDistanceUnits" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToLong( (long *)&pTentDR->m_iDistanceUnits );
        } else if( ChildName == _T ( "opencpn:DRTimeUnits" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToLong( (long *)&pTentDR->m_iTimeUnits );
        } else if( ChildName == _T ( "opencpn:GZ_LineType" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToLong( (long *)&pTentGZ->m_iLineType );
        } else if( ChildName == _T ( "opencpn:GZ_CentreLat" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentGZ->m_dCentreLat );
        } else if( ChildName == _T ( "opencpn:GZ_CentreLon" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentGZ->m_dCentreLon );
        } else if( ChildName == _T ( "opencpn:GZ_FirstLineDirection" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentGZ->m_dFirstLineDirection );
        } else if( ChildName == _T ( "opencpn:GZ_FirstDistance" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentGZ->m_dFirstDistance );
        } else if( ChildName == _T ( "opencpn:GZ_SecondLineDirection" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentGZ->m_dSecondLineDirection );
        } else if( ChildName == _T ( "opencpn:GZ_SecondDistance" ) ) {
            wxString::FromUTF8( tschild.first_child().value() ).ToDouble( &pTentGZ->m_dSecondDistance );
        } else if( ChildName == _T( "opencpn:PILList" ) ) {
            for( pugi::xml_node pilchild = tschild.child("opencpn:PILITEM"); pilchild; pilchild = pilchild.next_sibling() ) {
                PILLINE plNewLine;
                for (pugi::xml_attribute attr = pilchild.first_attribute(); attr; attr = attr.next_attribute()) {
                    if ( wxString::FromUTF8( attr.name() ) == _T("ActiveColour" ) )
                        plNewLine.wxcActiveColour.Set( wxString::FromUTF8( attr.as_string() ) );
                    if ( wxString::FromUTF8( attr.name() ) == _T("InActiveColour" ) )
                        plNewLine.wxcInActiveColour.Set( wxString::FromUTF8( attr.as_string() ) );
                    if ( wxString::FromUTF8( attr.name() ) == _T("ID" ) )
                        plNewLine.iID = attr.as_int();
                    if ( wxString::FromUTF8( attr.name() ) == _T("Name" ) )
                        plNewLine.sName = attr.as_string();
                    if ( wxString::FromUTF8( attr.name() ) == _T("Description" ) )
                        plNewLine.sDescription = attr.as_string();
                    if ( wxString::FromUTF8( attr.name() ) == _T("Offset" ) )
                        plNewLine.dOffset = attr.as_double();
                    if ( wxString::FromUTF8( attr.name() ) == _T("Style" ) )
                        plNewLine.iStyle = (wxPenStyle)attr.as_int();
                    if ( wxString::FromUTF8( attr.name() ) == _T("Width" ) )
                        plNewLine.dWidth = attr.as_double();
                }
                pTentPIL->AddLine(plNewLine);
            }
        }
    }   
    pTentPath->m_PathNameString = PathName;
    pTentPath->m_PathDescription = DescString;

    pTentPath->m_wxcActiveLineColour.Set( pTentPath->m_wxcActiveLineColour.Red(), pTentPath->m_wxcActiveLineColour.Green(), pTentPath->m_wxcActiveLineColour.Blue() );
    pTentPath->m_wxcInActiveLineColour.Set( pTentPath->m_wxcInActiveLineColour.Red(), pTentPath->m_wxcInActiveLineColour.Green(), pTentPath->m_wxcInActiveLineColour.Blue() );
    
    pTentPath->SetPointVisibility();
    
    if( b_propviz )
            pTentPath->SetVisible( b_viz );
    else {
        if( b_fullviz )
            pTentPath->SetVisible();
    }
    
    pTentPath->m_bPathIsActive = b_active;
    
    if( b_layer ){
        pTentPath->SetVisible( b_layerviz );
        pTentPath->m_bIsInLayer = true;
        pTentPath->m_LayerID = layer_id;
        pTentPath->SetListed( false );
    }            

    if( linklist ) {
        delete pTentPath->m_HyperlinkList;                    // created in RoutePoint ctor
        pTentPath->m_HyperlinkList = linklist;
    }
    pTentPath->CreateColourSchemes();
    pTentPath->SetColourScheme(g_global_color_scheme);
    pTentPath->SetActiveColours();
    
    pTentPath->UpdateSegmentDistances();
    pTentPath->m_bIsBeingCreated = false;
    
#ifndef __WXMSW__
#if wxCHECK_VERSION(3,0,0)        
    wxSetlocale(LC_NUMERIC, l_locale->ToAscii());
#else
    setlocale(LC_NUMERIC, l_locale->ToAscii());
#endif
    delete l_locale;
#endif
    
    return pTentPath;
}

ODPoint *ODNavObjectChanges::ODPointExists( const wxString& name, double lat, double lon )
{
    ODPoint *pret = NULL;
    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();
    while( node ) {
        ODPoint *pr = node->GetData();

//        if( pr->m_bIsInLayer ) return NULL;

        if( fabs( lat - pr->m_lat ) < 1.e-6 && fabs( lon - pr->m_lon ) < 1.e-6 && name == pr->GetName() ) {
            pret = pr;
            break;
        }
        node = node->GetNext();
    }

    return pret;
}

ODPoint *ODNavObjectChanges::ODPointExists( const wxString& guid )
{
    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();
    while( node ) {
        ODPoint *pr = node->GetData();

//        if( pr->m_bIsInLayer ) return NULL;

        if( guid == pr->m_GUID ) {
            return pr;
        }
        node = node->GetNext();
    }

    return NULL;
}

ODPoint *ODNavObjectChanges::tempODPointExists( const wxString& guid )
{
    wxODPointListNode *node = m_ptODPointList->GetFirst();
    while( node ) {
        ODPoint *pp = node->GetData();
        
        //        if( pr->m_bIsInLayer ) return NULL;
        //TODO fix crash when pp->m_GUID is not valid. Why is this so? appears to be when a point is updated twice, but.....
        if( !pp->m_GUID.IsEmpty() && pp->m_GUID.length() > 0 && guid == pp->m_GUID ) {
            return pp;
        }
        node = node->GetNext();
    }
    
    return NULL;
}

void ODNavObjectChanges::tempODPointRemove( const wxString& guid )
{
    wxODPointListNode *node = m_ptODPointList->GetFirst();
    while( node ) {
        ODPoint *pp = node->GetData();
        wxODPointListNode *next = node->GetNext();
        
        if( pp && guid == pp->m_GUID ) {
            m_ptODPointList->DeleteNode( node );
            return;
        }
        node = next;
    }
}

void ODNavObjectChanges::InsertPathA( ODPath *pTentPath )
{
    if( !pTentPath )
        return;
    
    bool bAddpath = true;
    //    If the path has only 1 point, don't load it.
    if( pTentPath->GetnPoints() < 2 )
        bAddpath = false;
    
    //    TODO  All this trouble for a tentative path.......Should make some path methods????
    if( bAddpath ) {
        if( PathExists( pTentPath->m_GUID ) ) { //We are importing a different path with the same guid, so let's generate it a new guid
            pTentPath->m_GUID = GetUUID();
            //Now also change guids for the ODPoints
            wxODPointListNode *pthisnode = ( pTentPath->m_pODPointList )->GetFirst();
            while( pthisnode ) {
                ODPoint *pP =  pthisnode->GetData();
                if( pP && pP->m_bIsolatedMark )
                    pP->m_GUID = GetUUID();
                pthisnode = pthisnode->GetNext();
            }
        }
            
        g_pPathList->Append( pTentPath );
        if(pTentPath->m_sTypeString == wxT("Boundary")) g_pBoundaryList->Append( (Boundary *)pTentPath );
        if(pTentPath->m_sTypeString == wxT("EBL")) g_pEBLList->Append( (EBL *)pTentPath );
        if(pTentPath->m_sTypeString == wxT("Guard Zone")) g_pGZList->Append( (GZ *)pTentPath );
        if(pTentPath->m_sTypeString == wxT("PIL")) g_pPILList->Append( (PIL *)pTentPath );

        pTentPath->RebuildGUIDList();                  // ensure the GUID list is intact
        
                
    //    Do the (deferred) calculation of BBox
        pTentPath->FinalizeForRendering();

        
        //    Add the selectable points and segments
        
        int ip = 0;
        float prev_rlat = 0., prev_rlon = 0.;
        ODPoint *prev_pConfPoint = NULL;
        
        wxODPointListNode *node = pTentPath->m_pODPointList->GetFirst();
        while( node ) {
            ODPoint *pop = node->GetData();
            
            if( ip )
                g_pODSelect->AddSelectablePathSegment( prev_rlat, prev_rlon, pop->m_lat,
                                                    pop->m_lon, prev_pConfPoint, pop, pTentPath );
            g_pODSelect->AddSelectableODPoint(pop->m_lat, pop->m_lon, pop);
            prev_rlat = pop->m_lat;
            prev_rlon = pop->m_lon;
            prev_pConfPoint = pop;
            
            ip++;
            
            node = node->GetNext();
        }
         if(pTentPath->m_sTypeString == wxT("Guard Zone")) {
             GZ * pGZ = (GZ *)pTentPath;
             pGZ->UpdateGZSelectablePath();
         }
         pTentPath->CreateColourSchemes();
         pTentPath->SetColourScheme(g_global_color_scheme);
         pTentPath->SetActiveColours();
         
    }
    else {
        
        // walk the path, deleting points used only by this path
        wxODPointListNode *pnode = ( pTentPath->m_pODPointList )->GetFirst();
        while( pnode ) {
            ODPoint *pop = pnode->GetData();
            
            // check all other paths to see if this point appears in any other path
            ODPath *pcontainer_path = g_pPathMan->FindPathContainingODPoint( pop );
            
            if( pcontainer_path == NULL ) {
                pop->m_bIsInPath = false; // Take this point out of this (and only) path
                if( !pop->m_bKeepXPath ) {
                    g_pODConfig->m_bSkipChangeSetUpdate = true;
                    g_pODConfig->DeleteODPoint( pop );
                    g_pODConfig->m_bSkipChangeSetUpdate = false;
                    delete pop;
                }
            }
            
            pnode = pnode->GetNext();
        }
        
        delete pTentPath;
    }
}
                    
ODPath *ODNavObjectChanges::PathExists( const wxString& guid )
{
    wxPathListNode *path_node = g_pPathList->GetFirst();

    while( path_node ) {
        ODPath *ppath = path_node->GetData();

        if( guid == ppath->m_GUID ) return ppath;

        path_node = path_node->GetNext();
    }
    return NULL;
}

ODPath *ODNavObjectChanges::PathExists( ODPath * pTentPath )
{
    wxPathListNode *path_node = g_pPathList->GetFirst();
    while( path_node ) {
        ODPath *ppath = path_node->GetData();

        if( ppath->IsEqualTo( pTentPath ) ) {
            return ppath;
        }

        path_node = path_node->GetNext();       // next path
    }
    return NULL;
}

bool ODNavObjectChanges::SaveFile( const wxString filename )
{
    save_file(filename.fn_str(), "  ");
    return true;
}

bool ODNavObjectChanges::ApplyChanges(void)
{
    //Let's reconstruct the unsaved changes
    
    pugi::xml_node object = this->first_child();
    
    while(strlen(object.name()))
    {
        if( !strcmp(object.name(), "opencpn:ODPoint") ) {
            ODPoint *pOp = GPXLoadODPoint1( object, _T("circle"), _T(""), false, false, false, 0 );
            
            if(pOp && g_pODPointMan) {
                pOp->m_bIsolatedMark = true;
                ODPoint *pExisting = ODPointExists( pOp->GetName(), pOp->m_lat, pOp->m_lon );
                
                pugi::xml_node child = object.child("opencpn:action");
                
                if(!strcmp(child.first_child().value(), "add") ){
                    if( !pExisting ) 
                        g_pODPointMan->AddODPoint( pOp );
                    g_pODSelect->AddSelectableODPoint( pOp->m_lat, pOp->m_lon, pOp );
                }                    
                
                else if(!strcmp(child.first_child().value(), "update") ){
                    if( pExisting )
                        g_pODPointMan->RemoveODPoint( pExisting );
                    g_pODPointMan->AddODPoint( pOp );
                    g_pODSelect->AddSelectableODPoint( pOp->m_lat, pOp->m_lon, pOp );
                }
                
                else if(!strcmp(child.first_child().value(), "delete") ){
                    if( pExisting )
                        g_pODPointMan->DestroyODPoint( pExisting, false );
                }
                
                else {
                    // XXX bogus or corrupted xml ?
                    tempODPointRemove (pOp->m_GUID);
                    delete pOp;
                }
            }
        }
        else
            if( !strcmp(object.name(), "opencpn:path") ) {
                pugi::xml_node typesearch = object.child("opencpn:type");
                wxString wxsType = wxString::FromUTF8( typesearch.first_child().value() );
                ODPath *pPath = GPXLoadPath1( object, false, false, false, 0 , &wxsType );
                
                if(pPath && g_pPathMan) {
                    pugi::xml_node child = object.child("opencpn:action");

                    if(!strcmp(child.first_child().value(), "add") ){
                        InsertPathA( pPath );
                        g_pODConfig->AddNewPath( pPath );
                    }                    
                
                    else if(!strcmp(child.first_child().value(), "update") ){
                        UpdatePathA( pPath );
                        g_pODConfig->UpdatePath( pPath );
                    }
                
                    else if(!strcmp(child.first_child().value(), "delete") ){
                        ODPath *pExisting = PathExists( pPath->m_GUID );
                        if(pExisting){
                            g_pODConfig->m_bSkipChangeSetUpdate = true;
                            g_pPathMan->DeletePath( pExisting );
                            g_pODConfig->m_bSkipChangeSetUpdate = false;
                        }
                    }
                
                    else
                        delete pPath;
                }
            }
    
        object = object.next_sibling();
                
    }

    return true;
}

int ODNavObjectChanges::LoadAllGPXObjectsAsLayer(int layer_id, bool b_layerviz)
{
    if(!g_pODPointMan)
        return 0;
    
    int n_obj = 0;
    pugi::xml_node objects = this->child("OCPNDraw");
    
    for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling())
    {
        if( !strcmp(object.name(), "opencpn:ODPoint") ) {
            ODPoint *pOp = GPXLoadODPoint1( object, _T("circle"), _T(""), true, true, b_layerviz, layer_id );
            if(pOp) {
                pOp->m_bIsolatedMark = true;      // This is an isolated mark
                g_pODPointMan->AddODPoint( pOp );
                g_pODSelect->AddSelectableODPoint( pOp->m_lat, pOp->m_lon, pOp );
                n_obj++;
            }
        }
        else{
            if( !strcmp(object.name(), "opencpn:path") ) {
                wxString TypeString;
                for( pugi::xml_node child = object.first_child(); child != 0; child = child.next_sibling() ) {
                    const char *pcn = child.name();
                    if( !strcmp( pcn, "opencpn:type") ) {
                        TypeString = wxString::FromUTF8( child.first_child().value() );
                        break;
                    }
                }
                if ( !TypeString.compare( wxS("Boundary") ) || !TypeString.compare( wxS("EBL") ) || !TypeString.compare( wxS("DR") ) || !TypeString.compare( wxS("Guard Zone") ) ) {
                    ODPath *pPath = GPXLoadPath1( object, true, true, b_layerviz, layer_id, &TypeString );
                    n_obj++;
                    InsertPathA( pPath );
                }
            }
        }   
    }
    
    return n_obj;
}

void ODNavObjectChanges::UpdatePathA( ODPath *pPathUpdate )
{
    ODPath * pExistingPath = PathExists( pPathUpdate->m_GUID );

    if( pExistingPath ) {
        Boundary *pBoundary = NULL;
        EBL *pEBL = NULL;
        DR *pDR = NULL;
        GZ *pGZ = NULL;
        PIL *pPIL = NULL;
        
        if(pPathUpdate->m_sTypeString == wxT("Boundary")) {
            pBoundary = (Boundary *)pExistingPath;
            Boundary *puBoundary = (Boundary *)pPathUpdate;
            pBoundary->SetColours( puBoundary );
        } else if(pPathUpdate->m_sTypeString == wxT("EBL")) {
            pEBL = (EBL *)pExistingPath;
            EBL *puEBL = (EBL *)pPathUpdate;
            pEBL->SetPersistence(puEBL->m_iPersistenceType);
        } else if(pPathUpdate->m_sTypeString == wxT("DR")) {
            pDR = (DR *)pExistingPath;
            DR *puDR = (DR *)pPathUpdate;
            pDR->SetPersistence(puDR->m_iPersistenceType);
        } else if(pPathUpdate->m_sTypeString == wxT("Guard Zone")) {
            pGZ = (GZ *)pExistingPath;
            GZ *puGZ = (GZ *)pPathUpdate;
            pGZ->SetPersistence(puGZ->m_iPersistenceType);
        } else if(pPathUpdate->m_sTypeString == wxT("PIL")) {
            pPIL = (PIL *)pExistingPath;
            PIL *puPIL = (PIL *)pPathUpdate;
            pPIL->SetPersistence(puPIL->m_iPersistenceType);
        }
        
        if ( pPathUpdate->GetnPoints() < pExistingPath->GetnPoints() ) {
            wxODPointListNode *enode = pExistingPath->m_pODPointList->GetFirst();
            while( enode ) {
                ODPoint *pFP = enode->GetData();
                ODPoint *pOP = pPathUpdate->GetPoint( pFP->m_GUID );
                if (!pOP ) {
                    pExistingPath->RemovePoint( pFP );
                    enode = pExistingPath->m_pODPointList->GetFirst(); // start at begining of list again
                } else enode = enode->GetNext();
            }
        }
        wxODPointListNode *unode = pPathUpdate->m_pODPointList->GetFirst();
        ODPoint *save_ex_op = NULL;
        while( unode ) {
            ODPoint *up_op = unode->GetData();
            ODPoint *ex_op = pExistingPath->GetPoint( up_op->m_GUID );
            if( ex_op ) {
                ex_op->m_lat = up_op->m_lat;
                ex_op->m_lon = up_op->m_lon;
                ex_op->SetIconName( up_op->GetIconName() );
                ex_op->m_ODPointDescription = up_op->m_ODPointDescription;
                ex_op->SetName( up_op->GetName() );
                save_ex_op = ex_op;
            } else {
                pExistingPath->InsertPointAfter( save_ex_op, up_op );
                save_ex_op = up_op;
            }
            unode = unode->GetNext();
        }
        if(pPathUpdate->m_sTypeString == wxT("Guard Zone")) {
            GZ * pGZ = (GZ *)pPathUpdate;
            pGZ->UpdateGZSelectablePath();
        } else {
            g_pODSelect->DeleteAllSelectableODPoints( pExistingPath );
            g_pODSelect->DeleteAllSelectablePathSegments( pExistingPath );
            g_pODSelect->AddAllSelectablePathSegments( pExistingPath );
            g_pODSelect->AddAllSelectableODPoints( pExistingPath );
        }
        pExistingPath->CreateColourSchemes();
        pExistingPath->SetColourScheme(g_global_color_scheme);
        pExistingPath->SetActiveColours();
    } else {
        InsertPathA( pPathUpdate );
    }
}

int ODNavObjectChanges::Load_CSV_File(wxString FileName, int layer_id, bool b_layerviz)
{
    wxTextFile l_TextFile(FileName);
    l_TextFile.Open();
    wxString l_InputLine;
    BoundaryCSVImport *l_BCI;
    BoundaryPointCSVImport *l_BPCI;
    TextPointCSVImport *l_TPCI;
    Boundary *l_boundary = NULL;
    bool    l_bBoundaryStart = false;
    int     l_NumObjs = 0;
    
    for(l_InputLine = l_TextFile.GetFirstLine(); l_TextFile.Eof() == false; l_InputLine = l_TextFile.GetNextLine()) {
        // process line
        wxStringTokenizer l_TokenString(l_InputLine, _T(","));
        //if(l_TokenString.CountTokens() < 3) return;
        wxString l_type = l_TokenString.GetNextToken();

        if(l_type == _T("'B'")) {
            if(l_bBoundaryStart) {
                wxString l_message = _("Error in import file.");
                if(l_boundary) {
                    l_message.Append(_("Boundary '"));
                    l_message.Append(l_boundary->m_PathNameString);
                    l_message.Append(_T("' "));
                    l_message.Append(_("will be deleted. No further boundaries will be imported"));
                }
                OCPNMessageBox_PlugIn( NULL, l_message, _("Import Error"), wxOK );
                if(l_boundary) g_pPathMan->DeletePath(l_boundary);
                return l_NumObjs;
            }
            l_bBoundaryStart = true;
            l_BCI = new BoundaryCSVImport(l_TokenString);
            l_boundary = new Boundary();

            l_boundary->m_PathNameString = l_BCI->m_sName;
            l_boundary->m_bExclusionBoundary = l_BCI->m_bExclusion;
            l_boundary->m_bInclusionBoundary = l_BCI->m_bInclusion;
            l_boundary->m_bPathIsActive = true;
            l_boundary->SetVisible(l_BCI->m_bVisible);
            l_boundary->m_wxcActiveLineColour = l_BCI->m_LineColour;
            l_boundary->m_wxcActiveFillColour = l_BCI->m_FillColour;
            if( layer_id ){
                l_boundary->SetVisible( b_layerviz );
                l_boundary->m_bIsInLayer = true;
                l_boundary->m_LayerID = layer_id;
                l_boundary->SetListed( false );
            }            
            l_NumObjs++;
            delete l_BCI;
        } else if(l_type == _T("'BP'")){
            l_BPCI = new BoundaryPointCSVImport(l_TokenString);
            BoundaryPoint *l_pBP = new BoundaryPoint(l_BPCI->m_dLat, l_BPCI->m_dLon, g_sODPointIconName, l_BPCI->m_sName, wxEmptyString, false);
            if(l_bBoundaryStart) {
                l_boundary->AddPoint(l_pBP, false, true, true);
                l_pBP->m_bIsInBoundary = true;
                l_pBP->m_bIsInPath = true;
            } else {
                g_pODPointMan->AddODPoint(l_pBP);
                l_pBP->m_bIsolatedMark = true;
                g_pODSelect->AddSelectableODPoint(l_BPCI->m_dLat, l_BPCI->m_dLon, l_pBP);
                l_pBP->m_bIsInBoundary = false;
                l_pBP->m_bIsInPath = false;
            }
            l_pBP->m_bExclusionBoundaryPoint = l_BPCI->m_bExclusion;
            l_pBP->m_bInclusionBoundaryPoint = l_BPCI->m_bInclusion;
            l_pBP->SetVisible(l_BPCI->m_bVisible); 
            l_pBP->SetShowODPointRangeRings(l_BPCI->m_bRangeRingsVisible);
            l_pBP->SetODPointRangeRingsNumber(l_BPCI->m_iNumRings);
            l_pBP->SetODPointRangeRingsStep(l_BPCI->m_dStep);
            l_pBP->SetODPointRangeRingsStepUnits(l_BPCI->m_iUnits);
            l_pBP->SetODPointRangeRingsColour(l_BPCI->m_RingColour);
            if( layer_id ) {
                l_pBP->m_bIsInLayer = true;
                l_pBP->m_LayerID = layer_id;
                if(!l_pBP->m_bIsInPath)
                    l_pBP->m_bIsVisible = b_layerviz;
                l_pBP->SetListed( false );
            }
            l_NumObjs++;
            
            delete l_BPCI;
        } else if(l_type == _T("'TP'")) {
            l_TPCI = new TextPointCSVImport(l_TokenString);
            TextPoint *l_pTP = new TextPoint(l_TPCI->m_dLat, l_TPCI->m_dLon, g_sODPointIconName, l_TPCI->m_sName, wxEmptyString, true);
            l_pTP->m_iDisplayTextWhen = l_TPCI->m_iDisplayTextWhen;
            l_pTP->m_iTextPosition = l_TPCI->m_iTextPosition;
            l_pTP->SetPointText(wxString::Format(l_TPCI->m_TextPointText));
            g_pODSelect->AddSelectableODPoint(l_TPCI->m_dLat, l_TPCI->m_dLon, l_pTP);
            if( layer_id ) {
                l_pTP->m_bIsInLayer = true;
                l_pTP->m_LayerID = layer_id;
                l_pTP->SetListed( false );
            }
            l_NumObjs++;

            delete l_TPCI;
        } else if(l_type == _T("'/B'")) {
            // end boundaryg
            l_boundary->AddPoint(l_boundary->m_pODPointList->GetFirst()->GetData());
            l_boundary->m_bIsBeingCreated = false;
            InsertPathA(l_boundary);
            l_boundary = NULL;
            l_bBoundaryStart = false;
        }
    }
    if(l_bBoundaryStart) {
        wxString l_message = _("Import incomplete.");
        if(l_boundary) {
            l_message.Append(_("Boundary '"));
            l_message.Append(l_boundary->m_PathNameString);
            l_message.Append(_T("' "));
            l_message.Append(_("will be deleted."));
        }
        OCPNMessageBox_PlugIn( NULL, l_message, _("Import Error"), wxOK );
        if(l_boundary) g_pPathMan->DeletePath(l_boundary);
        return --l_NumObjs;
    }
    return l_NumObjs;
}
