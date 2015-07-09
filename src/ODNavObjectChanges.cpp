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
#include "ODUtils.h"

extern PathList         *g_pPathList;
extern ODPointList      *g_pODPointList;
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
    if( ::wxFileExists( m_ODfilename.mb_str() ) )
        ::wxRemoveFile( m_ODfilename.mb_str() );
    
    m_ODchanges_file = fopen(m_ODfilename.mb_str(), "a");
}

bool ODNavObjectChanges::GPXCreateODPoint( pugi::xml_node node, ODPoint *pop, unsigned int flags )
{
    wxString s;
    pugi::xml_node child;
    pugi::xml_attribute attr;
    TextPoint *tp;
    BoundaryPoint *bp;
    ODPoint *pp;
    
    if(pop->m_sTypeString == wxT("Text Point")) 
        tp = (TextPoint *)pop;
    else if(pop->m_sTypeString == wxT("Boundary Point"))
        bp = (BoundaryPoint *)pop;
    pp = pop;
    
    s.Printf(_T("%.9f"), pp->m_lat);
    node.append_attribute("lat") = s.mb_str();
    s.Printf(_T("%.9f"), pp->m_lon);
    node.append_attribute("lon") = s.mb_str();

    if(flags & OUT_TYPE) {
        child = node.append_child("opencpn:type");
        child.append_child(pugi::node_pcdata).set_value(pp->GetTypeString().mb_str());
    }
    
    if(flags & OUT_TIME) {
            child = node.append_child("time");
            if( pp->m_timestring.Len() )
                child.append_child(pugi::node_pcdata).set_value(pp->m_timestring.mb_str());
            else {
                wxString t = pp->GetCreateTime().FormatISODate().Append(_T("T")).Append(pp->GetCreateTime().FormatISOTime()).Append(_T("Z"));
                child.append_child(pugi::node_pcdata).set_value(t.mb_str());
            }
    }
        
    if ( (!pp->GetName().IsEmpty() && (flags & OUT_NAME)) || (flags & OUT_NAME_FORCE) ) {
        wxCharBuffer buffer=pp->GetName().ToUTF8();
        if(buffer.data()) {
            child = node.append_child("name");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }       

    if ( (!pp->GetDescription().IsEmpty() && (flags & OUT_DESC)) || (flags & OUT_DESC_FORCE) ) {
        wxCharBuffer buffer=pp->GetDescription().ToUTF8();
        if(buffer.data()) {
            child = node.append_child("desc");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }       

    if(pp->m_sTypeString == wxT("Text Point")) {
        if ( !tp->GetPointText().IsEmpty() && (flags & OUT_POINTTEXT) ) {
            wxCharBuffer buffer=tp->GetPointText().ToUTF8();
            if(buffer.data()) {
                child = node.append_child("opencpn:text");
                child.append_child(pugi::node_pcdata).set_value(buffer.data());
                child = node.append_child("opencpn:text_position");
                s.Printf(_T("%i"), tp->m_iTextPosition);
                child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
            }
            child = node.append_child( "opencpn:text_colour" );
            child.append_child(pugi::node_pcdata).set_value( tp->m_colourTextColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() );
            child = node.append_child( "opencpn:background_colour" );
            child.append_child(pugi::node_pcdata).set_value( tp->m_colourTextBackgroundColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() );
            child = node.append_child( "opencpn:background_transparency" );
            s.Printf(_T("%i"), tp->m_iBackgroundTransparency );
            child.append_child(pugi::node_pcdata).set_value( s.mb_str() );
        }
    }
    
    if(pp->m_sTypeString == wxT("Boundary Point")) {
        child = node.append_child("opencpn:fill");
        if(bp->m_bFill)
            child.append_child(pugi::node_pcdata).set_value( "1" );
        else
            child.append_child(pugi::node_pcdata).set_value( "0" );
    }
    
    // Hyperlinks
    if(flags & OUT_HYPERLINKS ){
        HyperlinkList *linklist = pp->m_HyperlinkList;
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
        if (!pp->GetIconName().IsEmpty()) {
            child.append_child(pugi::node_pcdata).set_value(pp->GetIconName().mb_str());
        }
        else {
            child.append_child("empty");
        }
    }       
    
    if( (flags & OUT_GUID) || (flags & OUT_VIZ) || (flags & OUT_VIZ_NAME) || (flags & OUT_SHARED)
            || (flags & OUT_AUTO_NAME)  || (flags & OUT_EXTENSION) ) {
        
        //pugi::xml_node child_ext = node.append_child("extensions");
        
        if (!pp->m_GUID.IsEmpty() && (flags & OUT_GUID) ) {
            child = node.append_child("opencpn:guid");
            child.append_child(pugi::node_pcdata).set_value(pp->m_GUID.mb_str());
        }
        
        if(flags & OUT_VIZ) {
            child = node.append_child("opencpn:viz");
            if ( pp->m_bIsVisible )
                child.append_child(pugi::node_pcdata).set_value("1");
            else
                child.append_child(pugi::node_pcdata).set_value("0");
        }
            
        if(flags & OUT_VIZ_NAME) {
            child = node.append_child("opencpn:viz_name");
            if ( pp->m_bShowName )
                child.append_child(pugi::node_pcdata).set_value("1");
            else
                child.append_child(pugi::node_pcdata).set_value("0");
        }
        
        if((flags & OUT_AUTO_NAME) && pp->m_bDynamicName) {
            child = node.append_child("opencpn:auto_name");
            child.append_child(pugi::node_pcdata).set_value("1");
        }
        if((flags & OUT_SHARED) && pp->m_bKeepXPath) {
            child = node.append_child("opencpn:shared");
            child.append_child(pugi::node_pcdata).set_value("1");
        }
        if(flags & OUT_ARRIVAL_RADIUS) {
            child = node.append_child("opencpn:arrival_radius");
            s.Printf(_T("%.3f"), pp->GetODPointArrivalRadius());
            child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        }
        if(flags & OUT_OCPNPOINT_RANGE_RINGS) {
            child = node.append_child("opencpn:ODPoint_range_rings");
            pugi::xml_attribute viz = child.append_attribute( "visible" );
            viz.set_value( pp->m_bShowODPointRangeRings );
            pugi::xml_attribute number = child.append_attribute( "number" );
            number.set_value( pp->m_iODPointRangeRingsNumber );
            pugi::xml_attribute step = child.append_attribute( "step" );
            step.set_value( pp->m_fODPointRangeRingsStep );
            pugi::xml_attribute units = child.append_attribute( "units" );
            units.set_value( pp->m_iODPointRangeRingsStepUnits );
            pugi::xml_attribute colour = child.append_attribute( "colour" );
            colour.set_value( pp->m_wxcODPointRangeRingsColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() ) ;
        }
    }
    
    return true;
}

bool ODNavObjectChanges::GPXCreatePath( pugi::xml_node node, Path *pInPath )
{
    Path *pPath;
    Boundary * pBoundary = NULL;
    if(pInPath->m_sTypeString == wxT("Boundary")) {
        pBoundary = (Boundary *)pInPath;
        pPath = pBoundary;
    }
    else
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
    activecolour.set_value( pPath->m_wxcActiveLineColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() );
    if(pBoundary) {
        pugi::xml_attribute activefillcolour = child.append_attribute("active_fillcolour");
        activefillcolour.set_value( pBoundary->m_wxcActiveFillColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() );
    }
    pugi::xml_attribute inactivecolour = child.append_attribute("inactive_colour");
    inactivecolour.set_value( pPath->m_wxcInActiveLineColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() );
    if(pBoundary) {
        pugi::xml_attribute inactivefillcolour = child.append_attribute("inactive_fillcolour");
        inactivefillcolour.set_value( pBoundary->m_wxcInActiveFillColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() );
    }
    child.append_attribute("width") = pPath->m_width;
    child.append_attribute("style") = pPath->m_style;

    ODPointList *pODPointList = pPath->m_pODPointList;
    wxODPointListNode *node2 = pODPointList->GetFirst();
    ODPoint *pop;
    
    while( node2  ) {
        pop = node2->GetData();
            
        GPXCreateODPoint(node.append_child("opencpn:ODPoint"), pop, OPT_OCPNPOINT);
            
        node2 = node2->GetNext();
    }
    
    return true;
}
                    
bool ODNavObjectChanges::AddPath( Path *pb, const char *action )
{
    if( !m_ODchanges_file )
        return false;
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
#else __WXMSW__    
    fflush(m_ODchanges_file);
#endif __WXMSW__
    
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
#else __WXMSW__    
    fflush(m_ODchanges_file);
#endif __WXMSW__
    
    return true;
}

bool ODNavObjectChanges::AddGPXPathsList( PathList *pPaths )
{
    SetRootGPXNode();
    
    m_bFirstPath = true;
    wxPathListNode* pPath = pPaths->GetFirst();
    while (pPath) {
        Path* pPData = pPath->GetData();
        AddGPXPath( pPData);
        pPath = pPath->GetNext();
    }
    
    return true;
}

bool ODNavObjectChanges::AddGPXPath(Path *pPath)
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
    
    ODPoint *pr;
    
    while( node ) {
        pr = node->GetData();
        
        if( ( pr->m_bIsolatedMark ) && !( pr->m_bIsInLayer ) && !(pr->m_btemp) )
        {
            GPXCreateODPoint(m_gpx_root.append_child("opencpn:ODPoint"), pr, OPT_OCPNPOINT);
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
    while( node1 ) {
        Path *pBoundary = node1->GetData();
        
        if( !pBoundary->m_bIsInLayer && !pBoundary->m_btemp )
            GPXCreatePath(m_gpx_root.append_child("opencpn:path"), pBoundary);
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
                    if ( !TypeString.compare( wxS("Boundary") ) ) {
                        Path *pPath = GPXLoadPath1( object, b_full_viz, false, false, 0, &TypeString );
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
                            int layer_id
                            )
{
    bool bviz = false;
    bool bviz_name = false;
    bool bauto_name = false;
    bool bshared = false;
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
    int     l_iODPointRangeRingsNumber = -1;
    float   l_fODPointRangeRingsStep = -1;
    int     l_pODPointRangeRingsStepUnits = -1;
    bool    l_bODPointRangeRingsVisible = false;
    int     l_iTextPosition = g_iTextPosition;
    wxColour    l_wxcODPointRangeRingsColour;
    wxColour    l_colourTextColour = g_colourDefaultTextColour;
    wxColour    l_colourBackgroundColour = g_colourDefaultTextBackgroundColour;
    int     l_iBackgroundTransparency = g_iTextBackgroundTransparency;
    bool    l_bFill = false;
    
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
        } else if( !strcmp( pcn, "opencpn:text_colour") ) {
            l_colourTextColour.Set( wxString::FromUTF8( child.first_child().value() ) );
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
        else
        if( !strcmp( pcn, "opencpn:viz" ) ) {
            b_propviz = true;
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                bviz = ( v != 0 );
        }
        else
        if( !strcmp( pcn, "opencpn:viz_name") ) {
            b_propvizname = true;
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                bviz_name = ( v != 0 );
        }
        else
        if( !strcmp( pcn, "opencpn:guid" ) ) {
            GuidString.clear();
            GuidString.append( wxString::FromUTF8(child.first_child().value()) );
        }
        else
        if( !strcmp( pcn, "opencpn:auto_name" ) ) {
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                bauto_name = ( v != 0 );
        }
        else
        if( !strcmp( pcn, "opencpn:shared" ) ) {
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                bshared = ( v != 0 );
        }
        else
        if( !strcmp( pcn, "opencpn:arrival_radius" ) ) {
            wxString::FromUTF8(child.first_child().value()).ToDouble(&ArrivalRadius ) ;
        }
        else
        if ( !strcmp( pcn, "opencpn:ODPoint_range_rings") ) {
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
            }
        } else if ( !strcmp( pcn, "opencpn:fill" ) ) {
            wxString s = wxString::FromUTF8( child.first_child().value() );
            long v = 0;
            if( s.ToLong( &v ) )
                l_bFill = ( v != 0 );
        }
    }   // for

    // Create waypoint

    if( b_layer ) {
        if( GuidString.IsEmpty() )
            GuidString = _T("LayGUID");
    }

    // Check to see if this point already exits
    pOP = tempODPointExists( GuidString );
    if( !pOP ) {
        if( TypeString == wxT("Text Point") ) {
            pTP = new TextPoint( rlat, rlon, SymString, NameString, GuidString, false );
            pOP = pTP;
        } else if( TypeString == wxT("Boundary Point") ) {
            pBP = new BoundaryPoint( rlat, rlon, SymString, NameString, GuidString, false );
            pOP = pBP;
        }
        else
            pOP = new ODPoint( rlat, rlon, SymString, NameString, GuidString, false ); // do not add to global WP list yet...
            
        m_ptODPointList->Append( pOP ); 
        if( TypeString == "Text Point" ) {
            pTP->SetPointText( TextString );
            pTP->m_iTextPosition = l_iTextPosition;
            pTP->m_colourTextColour = l_colourTextColour;
            pTP->m_colourTextBackgroundColour = l_colourBackgroundColour;
            pTP->m_iBackgroundTransparency = l_iBackgroundTransparency;
        } else if ( TypeString == "Boundary Point" )
            pBP -> m_bFill = l_bFill;
        
        pOP->SetMarkDescription( DescString );
        pOP->m_sTypeString = TypeString;
        pOP->SetODPointArrivalRadius( ArrivalRadius );
        pOP->SetODPointRangeRingsNumber( l_iODPointRangeRingsNumber );
        pOP->SetODPointRangeRingsStep( l_fODPointRangeRingsStep );
        pOP->SetODPointRangeRingsStepUnits( l_pODPointRangeRingsStepUnits );
        pOP->SetShowODPointRangeRings( l_bODPointRangeRingsVisible );
        pOP->SetODPointRangeRingsColour( l_wxcODPointRangeRingsColour );
    }

    if( b_propvizname )
        pOP->m_bShowName = bviz_name;
    else
        if( b_fullviz )
            pOP->m_bShowName = true;
        else
            pOP->m_bShowName = false;

    if( b_propviz )
        pOP->m_bIsVisible = bviz;
    else
        if( b_fullviz )
            pOP->m_bIsVisible = true;

    if(b_layer) {
        pOP->m_bIsInLayer = true;
        pOP->m_LayerID = layer_id;
        pOP->m_bIsVisible = b_layerviz;
        pOP->SetListed( false );
    }

    pOP->m_bKeepXPath = bshared;
    pOP->m_bDynamicName = bauto_name;

    if(TimeString.Len()) {
        pOP->m_timestring = TimeString;
        pOP->SetCreateTime(wxInvalidDateTime);          // cause deferred timestamp parsing
    }
        

    if( linklist ) {
        delete pOP->m_HyperlinkList;                    // created in RoutePoint ctor
        pOP->m_HyperlinkList = linklist;
    }

    return ( pOP );
}

Path *ODNavObjectChanges::GPXLoadPath1( pugi::xml_node &wpt_node, bool b_fullviz,
                    bool b_layer,
                    bool b_layerviz,
                    int layer_id, wxString *pPathType )
{
    wxString PathName;
    wxString DescString;
    bool b_propviz = false;
    bool b_viz = true;
    bool b_active = false;
    Boundary    *pTentBoundary = NULL;
    Path        *pTentPath = NULL;
    HyperlinkList *linklist = NULL;
    
    wxString Name = wxString::FromUTF8( wpt_node.name() );
    if( Name == _T ( "opencpn:path" ) ) {
        if (!strcmp(pPathType->mb_str(), "Boundary" ) ) {
            pTentBoundary = new Boundary();
            pTentPath = pTentBoundary;
        } else pTentPath = new Path();
        
        m_ptODPointList->clear();
        for( pugi::xml_node tschild = wpt_node.first_child(); tschild; tschild = tschild.next_sibling() ) {
            wxString ChildName = wxString::FromUTF8( tschild.name() );

            if( ChildName == _T ( "opencpn:ODPoint" ) ) {
                ODPoint *tpOp = GPXLoadODPoint1(  tschild, _T("square"), _T(""), b_fullviz, b_layer, b_layerviz, layer_id);
                
                pTentPath->AddPoint( tpOp, false, true, true);          // defer BBox calculation
                tpOp->m_bIsInBoundary = true;                      // Hack
            }
            else
            if( ChildName == _T ( "name" ) ) {
                PathName.append( wxString::FromUTF8( tschild.first_child().value() ) );
            }
            else
            if( ChildName == _T ( "desc" ) ) {
                DescString.append( wxString::FromUTF8( tschild.first_child().value() ) );
            }
                
            else
            if( ChildName == _T ( "link") ) {
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
            
            else
            if( ChildName == _T ( "opencpn:viz" ) ) {
                        wxString viz = wxString::FromUTF8(tschild.first_child().value());
                        b_propviz = true;
                        b_viz = ( viz == _T("1") );
            }
            
            else
            if( ChildName == _T ( "opencpn:active" ) ) {
                        wxString active = wxString::FromUTF8(tschild.first_child().value());
                        b_active = ( active == _T("1") );
            }
            
            else
            if( ChildName == _T ( "opencpn:style" ) ) {
                for (pugi::xml_attribute attr = tschild.first_attribute(); attr; attr = attr.next_attribute())
                {
                    if ( wxString::FromUTF8( attr.name() ) == _T("active_colour" ) )
                        pTentPath->m_wxcActiveLineColour.Set( wxString::FromUTF8( attr.as_string() ) );
                    else if ( wxString::FromUTF8( attr.name() ) == _T("active_fillcolour" ) )
                        pTentBoundary->m_wxcActiveFillColour.Set( wxString::FromUTF8( attr.as_string() ) );
                    if ( wxString::FromUTF8( attr.name() ) == _T("inactive_colour" ) )
                        pTentPath->m_wxcInActiveLineColour.Set( wxString::FromUTF8( attr.as_string() ) );
                    else if ( wxString::FromUTF8( attr.name() ) == _T("inactive_fillcolour" ) )
                        pTentBoundary->m_wxcInActiveFillColour.Set( wxString::FromUTF8( attr.as_string() ) );
                    else if ( wxString::FromUTF8( attr.name() ) == _T("style" ) )
                        pTentPath->m_style = attr.as_int();
                    else if ( wxString::FromUTF8( attr.name() ) == _T("width" ) )
                        pTentPath->m_width = attr.as_int();
                }
            }
            
            else
            if( ChildName == _T ( "opencpn:guid" ) ) {
                //if ( !g_bODIsNewLayer ) ) 
                pTentBoundary->m_GUID.clear();
                pTentBoundary->m_GUID.append( wxString::FromUTF8(tschild.first_child().value()) );
            }
            
            else
            if( ChildName == _T ( "opencpn:time_display" ) ) {
                pTentBoundary->m_TimeDisplayFormat.append( wxString::FromUTF8(tschild.first_child().value()) );
            }
        }
                    
        pTentPath->m_PathNameString = PathName;
        pTentPath->m_PathDescription = DescString;

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

    }
    if( linklist ) {
        delete pTentPath->m_HyperlinkList;                    // created in RoutePoint ctor
        pTentPath->m_HyperlinkList = linklist;
    }
    pTentPath->SetActiveColours();
    pTentPath->UpdateSegmentDistances();
    return pTentPath;
}

ODPoint *ODNavObjectChanges::ODPointExists( const wxString& name, double lat, double lon )
{
    ODPoint *pret = NULL;
    wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();
    while( node ) {
        ODPoint *pr = node->GetData();

//        if( pr->m_bIsInLayer ) return NULL;

        if( name == pr->GetName() ) {
            if( fabs( lat - pr->m_lat ) < 1.e-6 && fabs( lon - pr->m_lon ) < 1.e-6 ) {
                pret = pr;
                break;
            }
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
        
        if( guid == pp->m_GUID ) {
            return pp;
        }
        node = node->GetNext();
    }
    
    return NULL;
}

void ODNavObjectChanges::InsertPathA( Path *pTentPath )
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
            //Now also change guids for the routepoints
            wxODPointListNode *pthisnode = ( pTentPath->m_pODPointList )->GetFirst();
            while( pthisnode ) {
                ODPoint *pP =  pthisnode->GetData();
                if( pP && pP->m_bIsolatedMark )
                    pP->m_GUID = GetUUID();
                pthisnode = pthisnode->GetNext();
            }
        }
            
        g_pPathList->Append( pTentPath );
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
    }
    else {
        
        // walk the path, deleting points used only by this path
        wxODPointListNode *pnode = ( pTentPath->m_pODPointList )->GetFirst();
        while( pnode ) {
            ODPoint *pop = pnode->GetData();
            
            // check all other paths to see if this point appears in any other path
            Path *pcontainer_path = g_pPathMan->FindPathContainingODPoint( pop );
            
            if( pcontainer_path == NULL ) {
                pop->m_bIsInPath = false; // Take this point out of this (and only) track/route
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
                    
Path *ODNavObjectChanges::PathExists( const wxString& guid )
{
    wxPathListNode *path_node = g_pPathList->GetFirst();

    while( path_node ) {
        Path *ppath = path_node->GetData();

        if( guid == ppath->m_GUID ) return ppath;

        path_node = path_node->GetNext();
    }
    return NULL;
}

Path *ODNavObjectChanges::PathExists( Path * pTentPath )
{
    wxPathListNode *path_node = g_pPathList->GetFirst();
    while( path_node ) {
        Path *ppath = path_node->GetData();

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
                
                else
                    delete pOp;
            }
        }
        else
            if( !strcmp(object.name(), "opencpn:path") ) {
                pugi::xml_node typesearch = object.child("opencpn:type");
                wxString wxsType = wxString::FromUTF8( typesearch.first_child().value() );
                Path *pPath = GPXLoadPath1( object, false, false, false, 0 , &wxsType );
                
                if(pPath && g_pPathMan) {
                    pugi::xml_node child = object.child("opencpn:action");

                    if(!strcmp(child.first_child().value(), "add") ){
                        InsertPathA( pPath );
                    }                    
                
                    else if(!strcmp(child.first_child().value(), "update") ){
                        UpdatePathA( pPath );
                    }
                
                    else if(!strcmp(child.first_child().value(), "delete") ){
                        Path *pExisting = PathExists( pPath->m_GUID );
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
            pOp->m_bIsolatedMark = true;      // This is an isolated mark
            
            if(pOp) {
                g_pODPointMan->AddODPoint( pOp );
                g_pODSelect->AddSelectableODPoint( pOp->m_lat, pOp->m_lon, pOp );
                n_obj++;
            }
            else
                delete pOp;
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
                if ( !TypeString.compare( wxS("Boundary") ) ) {
                    Path *pPath = GPXLoadPath1( object, true, true, b_layerviz, layer_id, &TypeString );
                    n_obj++;
                    InsertPathA( pPath );
                }
            }
        }   
    }
    
    return n_obj;
}

void ODNavObjectChanges::UpdatePathA( Path *pTentPath )
{
    Path * path = PathExists( pTentPath->m_GUID );

    if( path ) {
        if ( pTentPath->GetnPoints() < path->GetnPoints() ) {
            wxODPointListNode *node = path->m_pODPointList->GetFirst();
            while( node ) {
                ODPoint *pFP = node->GetData();
                ODPoint *pOP = pTentPath->GetPoint( pFP->m_GUID );
                if (!pOP ) {
                    path->RemovePoint( pFP );
                    node = path->m_pODPointList->GetFirst(); // start at begining of list again
                } else node = node->GetNext();
            }
        }
        wxODPointListNode *node = pTentPath->m_pODPointList->GetFirst();
        ODPoint *save_ex_op = NULL;
        while( node ) {
            ODPoint *pop = node->GetData();
            ODPoint *ex_op = path->GetPoint( pop->m_GUID );
            if( ex_op ) {
                ex_op->m_lat = pop->m_lat;
                ex_op->m_lon = pop->m_lon;
                ex_op->SetIconName( pop->GetIconName() );
                ex_op->m_MarkDescription = pop->m_MarkDescription;
                ex_op->SetName( pop->GetName() );
                save_ex_op = ex_op;
            } else {
                path->InsertPointAfter( save_ex_op, pop );
                save_ex_op = pop;
            }
            node = node->GetNext();
        }
        g_pODSelect->DeleteAllSelectableODPoints( path );
        g_pODSelect->DeleteAllSelectablePathSegments( path );
        g_pODSelect->AddAllSelectablePathSegments( path );
        g_pODSelect->AddAllSelectableODPoints( path );
    } else {
        InsertPathA( pTentPath );
    }
}
                    
