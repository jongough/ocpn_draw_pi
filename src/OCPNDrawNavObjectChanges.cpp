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
 
#include "OCPNDrawNavObjectChanges.h"
#include "OCPNSelect.h"
#include "OCPNDrawConfig.h"
#include "PointMan.h"
#include "PathMan.h"
#include "OCPNPoint.h"
#include "Boundary.h"

extern PathList *pPathList;
extern OCPNSelect *pOCPNSelect;
pugi::xml_node  gpx_path_child;
pugi::xml_node  gpx_path_root;
bool            m_bFirstPath;
extern OCPNDrawConfig *pOCPNDrawConfig;
extern PointMan        *pOCPNPointMan;
extern PathMan          *g_pPathMan;



OCPNDrawNavObjectChanges::OCPNDrawNavObjectChanges() : NavObjectChanges()
{
    //ctor
    m_bFirstPath = true;
}

OCPNDrawNavObjectChanges::OCPNDrawNavObjectChanges(wxString file_name) : NavObjectChanges( file_name )
{
    //ctor
    
    m_ODfilename = file_name;
    
    m_bFirstPath = true;
}

OCPNDrawNavObjectChanges::~OCPNDrawNavObjectChanges()
{
    //dtor
}

bool GPXCreateOCPNPoint( pugi::xml_node node, OCPNPoint *pr, unsigned int flags )
{
    wxString s;
    pugi::xml_node child;
    pugi::xml_attribute attr;
    
    s.Printf(_T("%.9f"), pr->m_lat);
    node.append_attribute("lat") = s.mb_str();
    s.Printf(_T("%.9f"), pr->m_lon);
    node.append_attribute("lon") = s.mb_str();
 
    if(flags & OUT_TYPE) {
        child = node.append_child("opencpn:type");
		child.append_child(pugi::node_pcdata).set_value(pr->GetTypeString().mb_str());
    }
    
    if(flags & OUT_TIME) {
            child = node.append_child("time");
            if( pr->m_timestring.Len() )
                child.append_child(pugi::node_pcdata).set_value(pr->m_timestring.mb_str());
            else {
                wxString t = pr->GetCreateTime().FormatISODate().Append(_T("T")).Append(pr->GetCreateTime().FormatISOTime()).Append(_T("Z"));
                child.append_child(pugi::node_pcdata).set_value(t.mb_str());
            }
    }
        
    if ( (!pr->GetName().IsEmpty() && (flags & OUT_NAME)) || (flags & OUT_NAME_FORCE) ) {
        wxCharBuffer buffer=pr->GetName().ToUTF8();
        if(buffer.data()) {
            child = node.append_child("name");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }       

    if ( (!pr->GetDescription().IsEmpty() && (flags & OUT_DESC)) || (flags & OUT_DESC_FORCE) ) {
        wxCharBuffer buffer=pr->GetDescription().ToUTF8();
        if(buffer.data()) {
            child = node.append_child("desc");
            child.append_child(pugi::node_pcdata).set_value(buffer.data());
        }
    }       

    // Hyperlinks
    if(flags & OUT_HYPERLINKS ){
        HyperlinkList *linklist = pr->m_HyperlinkList;
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
        if (!pr->GetIconName().IsEmpty()) {
            child.append_child(pugi::node_pcdata).set_value(pr->GetIconName().mb_str());
        }
        else {
            child.append_child("empty");
        }
    }       
    
    if( (flags & OUT_GUID) || (flags & OUT_VIZ) || (flags & OUT_VIZ_NAME) || (flags & OUT_SHARED)
            || (flags & OUT_AUTO_NAME)  || (flags & OUT_EXTENSION) ) {
        
        //pugi::xml_node child_ext = node.append_child("extensions");
        
        if (!pr->m_GUID.IsEmpty() && (flags & OUT_GUID) ) {
            child = node.append_child("opencpn:guid");
            child.append_child(pugi::node_pcdata).set_value(pr->m_GUID.mb_str());
        }
         
         if((flags & OUT_VIZ) && !pr->m_bIsVisible) {
             child = node.append_child("opencpn:viz");
             child.append_child(pugi::node_pcdata).set_value("0");
         }
            
         if((flags & OUT_VIZ_NAME) && pr->m_bShowName) {
             child = node.append_child("opencpn:viz_name");
             child.append_child(pugi::node_pcdata).set_value("1");
         }
         
         if((flags & OUT_AUTO_NAME) && pr->m_bDynamicName) {
             child = node.append_child("opencpn:auto_name");
             child.append_child(pugi::node_pcdata).set_value("1");
         }
         if((flags & OUT_SHARED) && pr->m_bKeepXPath) {
             child = node.append_child("opencpn:shared");
             child.append_child(pugi::node_pcdata).set_value("1");
         }
        if(flags & OUT_ARRIVAL_RADIUS) {
            child = node.append_child("opencpn:arrival_radius");
            s.Printf(_T("%.3f"), pr->GetOCPNPointArrivalRadius());
            child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        }
        if(flags & OUT_OCPNPOINT_RANGE_RINGS) {
            child = node.append_child("opencpn:OCPNPoint_range_rings");
            pugi::xml_attribute viz = child.append_attribute( "visible" );
            viz.set_value( pr->m_bShowOCPNPointRangeRings );
            pugi::xml_attribute number = child.append_attribute( "number" );
            number.set_value( pr->m_iOCPNPointRangeRingsNumber );
            pugi::xml_attribute step = child.append_attribute( "step" );
            step.set_value( pr->m_fOCPNPointRangeRingsStep );
            pugi::xml_attribute units = child.append_attribute( "units" );
            units.set_value( pr->m_iOCPNPointRangeRingsStepUnits );
            pugi::xml_attribute colour = child.append_attribute( "colour" );
            colour.set_value( pr->m_wxcOCPNPointRangeRingsColour.GetAsString( wxC2S_HTML_SYNTAX ).utf8_str() ) ;
        }
    }
    
    return true;
}

bool GPXCreatePath( pugi::xml_node node, Path *pPath )
{
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
    
    pugi::xml_attribute colour = child.append_attribute("colour");
    colour.set_value( pPath->m_LineColour.ToAscii() );
    pugi::xml_attribute fillcolour = child.append_attribute("fillcolour");
    fillcolour.set_value( pPath->m_FillColour.ToAscii() );
    child.append_attribute("width") = pPath->m_width;
    child.append_attribute("style") = pPath->m_style;

    OCPNPointList *pOCPNPointList = pPath->pOCPNPointList;
    wxOCPNPointListNode *node2 = pOCPNPointList->GetFirst();
    OCPNPoint *prp;
    
    unsigned short int GPXTrkSegNo1 = 1;
    
    while( node2  ) {
        prp = node2->GetData();
            
        GPXCreateOCPNPoint(node.append_child("opencpn:OCPNPoint"), prp, OPT_OCPNPOINT);
            
        node2 = node2->GetNext();
    }
    
    return true;
}
                       
bool OCPNDrawNavObjectChanges::AddPath( Path *pb, const char *action )
{
    SetRootGPXNode();

    pugi::xml_node object = m_gpx_root.append_child("opencpn:path");
    GPXCreatePath(object, pb );
    
//    pugi::xml_node child_ext = m_gpx_root.append_child("extensions");
    
//    pugi::xml_node xchild = child_ext.child("extensions");
    //FIXME  What if extensions do not exist?
    pugi::xml_node child = object.append_child("opencpn:action");
    child.append_child(pugi::node_pcdata).set_value(action);

    pugi::xml_writer_file writer(m_changes_file);
    object.print(writer, " ");
    fflush(m_changes_file);
    
    return true;
}

bool OCPNDrawNavObjectChanges::AddOCPNPoint( OCPNPoint *pOP, const char *action )
{
    SetRootGPXNode();
    
    pugi::xml_node object = m_gpx_root.append_child("opencpn:OCPNPoint");
    GPXCreateOCPNPoint(object, pOP, OPT_OCPNPOINT);

//    pugi::xml_node xchild = child_ext.child("extensions");
    pugi::xml_node child = object.append_child("opencpn:action");
    child.append_child(pugi::node_pcdata).set_value(action);

    pugi::xml_writer_file writer(m_changes_file);
    m_gpx_root.print(writer, " ");
    fflush(m_changes_file);
    
    return true;
}

bool OCPNDrawNavObjectChanges::AddGPXPathsList( PathList *pPaths )
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

bool OCPNDrawNavObjectChanges::AddGPXPath(Path *pPath)
{
    SetRootGPXNode();
    if ( m_bFirstPath ) {
        m_bFirstPath = false;
    }
    GPXCreatePath(m_gpx_root.append_child("opencpn:path"), pPath);
    return true;
}

bool OCPNDrawNavObjectChanges::AddGPXOCPNPoint(OCPNPoint *pWP )
{
    SetRootGPXNode();
    GPXCreateOCPNPoint(m_gpx_root.append_child("OCPNPoint"), pWP, OPT_OCPNPOINT);
    return true;
}

bool OCPNDrawNavObjectChanges::AddGPXOCPNPointsList( OCPNPointList *pOCPNPoints )
{
    SetRootGPXNode();
    
    wxOCPNPointListNode* pOCPNPointNode = pOCPNPoints->GetFirst();
    while (pOCPNPointNode) {
        OCPNPoint* pRP = pOCPNPointNode->GetData();
        AddGPXOCPNPoint( pRP);
        pOCPNPointNode = pOCPNPointNode->GetNext();
    }
    
    return true;
}

bool OCPNDrawNavObjectChanges::CreateNavObjGPXPaths( void )
{
    pugi::xml_node child_ext;
    // Paths
    wxPathListNode *node1 = pPathList->GetFirst();
//    child_ext = m_gpx_root.append_child("extensions");
     while( node1 ) {
        Path *pBoundary = node1->GetData();
        
        if( !pBoundary->m_bIsInLayer && !pBoundary->m_btemp )
            GPXCreatePath(m_gpx_root.append_child("opencpn:path"), pBoundary);
        node1 = node1->GetNext();
    }
    
    
    return true;
}

bool OCPNDrawNavObjectChanges::CreateAllGPXObjects()
{
    SetRootGPXNode();
    CreateNavObjGPXPaths();
    
    return true;
}

void OCPNDrawNavObjectChanges::SetRootGPXNode(void)
{
    if(!strlen(m_gpx_root.name())) {
        m_gpx_root = append_child("OCPNDraw");
        m_gpx_root.append_attribute ( "version" ) = "0.1";
        m_gpx_root.append_attribute ( "creator" ) = "OpenCPN";
        m_gpx_root.append_attribute( "xmlns:xsi" ) = "http://www.w3.org/2001/XMLSchema-instance";
        m_gpx_root.append_attribute( "xmlns:opencpn" ) = "http://www.opencpn.org";
    }
}

bool OCPNDrawNavObjectChanges::LoadAllGPXObjects( bool b_full_viz )
{
    pugi::xml_node objects = this->child("OCPNDraw");
    
    for (pugi::xml_node object = objects.first_child(); object; object = object.next_sibling())
    {
        if( !strcmp(object.name(), "OCPNPoint") ) {
            OCPNPoint *pOp = GPXLoadOCPNPoint1( object, _T("circle"), _T(""), b_full_viz, false, false, 0 );
            
            if(pOp) {
                pOp->m_bIsolatedMark = true;      // This is an isolated mark
                OCPNPoint *pExisting = OCPNPointExists( pOp->GetName(), pOp->m_lat, pOp->m_lon );
                if( !pExisting ) {
                    if( NULL != pOCPNPointMan )
                        pOCPNPointMan->AddOCPNPoint( pOp );
                     pOCPNSelect->AddSelectableOCPNPoint( pOp->m_lat, pOp->m_lon, pOp );
                }
                else
                    delete pOp;
            }
        }
 /*       else
            if( !strcmp(object.name(), "trk") ) {
                Track *pTrack = GPXLoadTrack1( object, b_full_viz, false, false, 0);
                InsertTrack( pTrack );
            }
*/            else
                if( !strcmp(object.name(), "opencpn:path") ) {
                    // find object type 
                    wxString TypeString;
                    for( pugi::xml_node child = object.first_child(); child != 0; child = child.next_sibling() ) {
                        const char *pcn = child.name();
                        if( strcmp( pcn, "opencpn:type") ) {
                                TypeString = wxString::FromUTF8( child.first_child().value() );
                        }
                    }
                    if ( TypeString.compare( wxS("Boundary") ) ) {
                        Path *pPath = GPXLoadPath1( object, b_full_viz, false, false, 0, &TypeString );
                        InsertPathA( pPath );
                    }
                }
                
                
    }
    
    return true;
}

OCPNPoint * OCPNDrawNavObjectChanges::GPXLoadOCPNPoint1( pugi::xml_node &opt_node, 
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
    bool bviz_OCPNPointrangerings = false;

    wxString SymString = def_symbol_name;       // default icon
    wxString NameString;
    wxString DescString;
    wxString TypeString;
    wxString GuidString = GUID;         // default
    wxString TimeString;
    wxDateTime dt;
    OCPNPoint *pOP = NULL;
    
    HyperlinkList *linklist = NULL;

    double rlat = opt_node.attribute( "lat" ).as_double();
    double rlon = opt_node.attribute( "lon" ).as_double();
    double ArrivalRadius = 0;
    int     l_iOCPNPointRangeRingsNumber = -1;
    float   l_fOCPNPointRangeRingsStep = -1;
    int     l_pOCPNPointRangeRingsStepUnits = -1;
    bool    l_bOCPNPointRangeRingsVisible = false;
    wxColour    l_wxcOCPNPointRangeRingsColour;
    l_wxcOCPNPointRangeRingsColour.Set( _T( "#FFFFFF" ) );

    for( pugi::xml_node child = opt_node.first_child(); child != 0; child = child.next_sibling() ) {
        const char *pcn = child.name();
        
        if( !strcmp( pcn, "sym" ) ) {
            SymString = wxString::FromUTF8( child.first_child().value() );
        }
        else
        if( !strcmp( pcn, "time") ) 
            TimeString = wxString::FromUTF8( child.first_child().value() );

        else
        if( !strcmp( pcn, "name") ) {
                NameString = wxString::FromUTF8( child.first_child().value() );
        } 
        
        else
        if( !strcmp( pcn, "desc") ) {
                DescString = wxString::FromUTF8( child.first_child().value() );
        }
        
        else
        if( !strcmp( pcn, "type") ) {
                TypeString = wxString::FromUTF8( child.first_child().value() );
        }
        
        else              // Read hyperlink
        if( !strcmp( pcn, "link") ) {
            wxString HrefString;
            wxString HrefTextString;
            wxString HrefTypeString;
            if( linklist == NULL )
                linklist = new HyperlinkList;
            HrefString = wxString::FromUTF8( child.first_attribute().value() );

            for( pugi::xml_node child1 = child.first_child(); child1; child1 = child1.next_sibling() ) {
                wxString LinkString = wxString::FromUTF8( child1.name() );

                if( LinkString == _T ( "text" ) )
                    HrefTextString = wxString::FromUTF8( child1.first_child().value() );
                if( LinkString == _T ( "type" ) ) 
                    HrefTypeString = wxString::FromUTF8( child1.first_child().value() );
            }
          
            Hyperlink *link = new Hyperlink;
            link->Link = HrefString;
            link->DescrText = HrefTextString;
            link->LType = HrefTypeString;
            linklist->Append( link );
        }

    //    OpenCPN Extensions....
        else
        if( !strcmp( pcn, "extensions") ) {
            for( pugi::xml_node ext_child = child.first_child(); ext_child; ext_child = ext_child.next_sibling() ) {
                wxString ext_name = wxString::FromUTF8( ext_child.name() );
                if( ext_name == _T ( "opencpn:guid" ) ) {
                  GuidString = wxString::FromUTF8( ext_child.first_child().value() );
                }
                else
                if( ext_name == _T ( "opencpn:viz" ) ) {
                    b_propviz = true;
                    wxString s = wxString::FromUTF8( ext_child.first_child().value() );
                    long v = 0;
                    if( s.ToLong( &v ) )
                        bviz = ( v != 0 );
                }
                else
                if( ext_name == _T ( "opencpn:viz_name" ) ) {
                    b_propvizname = true;
                    wxString s = wxString::FromUTF8( ext_child.first_child().value() );
                    long v = 0;
                    if( s.ToLong( &v ) )
                        bviz_name = ( v != 0 );
                }
                else
                if( ext_name == _T ( "opencpn:auto_name" ) ) {
                    wxString s = wxString::FromUTF8( ext_child.first_child().value() );
                    long v = 0;
                    if( s.ToLong( &v ) )
                        bauto_name = ( v != 0 );
                }
                else
                if( ext_name  == _T ( "opencpn:shared" ) ) {
                    wxString s = wxString::FromUTF8( ext_child.first_child().value() );
                    long v = 0;
                    if( s.ToLong( &v ) )
                        bshared = ( v != 0 );
                }
                if( ext_name == _T ( "opencpn:arrival_radius" ) ) {
                    wxString::FromUTF8(ext_child.first_child().value()).ToDouble(&ArrivalRadius ) ;
                }
                if ( ext_name == _T("opencpn:OCPNPoint_range_rings") ) {
                    for ( pugi::xml_attribute attr = ext_child.first_attribute(); attr; attr = attr.next_attribute() ) {
                        if ( wxString::FromUTF8(attr.name()) == _T("number") )
                            l_iOCPNPointRangeRingsNumber = attr.as_int();
                        else if ( wxString::FromUTF8(attr.name()) == _T("step") )
                            l_fOCPNPointRangeRingsStep = attr.as_float();
                        else if ( wxString::FromUTF8(attr.name()) == _T("units") )
                            l_pOCPNPointRangeRingsStepUnits = attr.as_int();
                        else if ( wxString::FromUTF8(attr.name()) == _T("visible") )
                            l_bOCPNPointRangeRingsVisible =  attr.as_bool();
                        else if ( wxString::FromUTF8(attr.name()) == _T("colour") )
                            l_wxcOCPNPointRangeRingsColour.Set( wxString::FromUTF8( attr.as_string() ) );
                    }
                }
             }// for 
        } //extensions
    }   // for

    // Create waypoint

    if( b_layer ) {
        if( GuidString.IsEmpty() )
            GuidString = _T("LayGUID");
    }

    pOP = new OCPNPoint( rlat, rlon, SymString, NameString, GuidString, false ); // do not add to global WP list yet...
    pOP->m_MarkDescription = DescString;
    pOP->m_bIsolatedMark = bshared;      // This is an isolated mark
    pOP->SetOCPNPointArrivalRadius( ArrivalRadius );
    pOP->SetOCPNPointRangeRingsNumber( l_iOCPNPointRangeRingsNumber );
    pOP->SetOCPNPointRangeRingsStep( l_fOCPNPointRangeRingsStep );
    pOP->SetOCPNPointRangeRingsStepUnits( l_pOCPNPointRangeRingsStepUnits );
    pOP->SetShowOCPNPointRangeRings( l_bOCPNPointRangeRingsVisible );
    pOP->SetOCPNPointRangeRingsColour( l_wxcOCPNPointRangeRingsColour );

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

Path *OCPNDrawNavObjectChanges::GPXLoadPath1( pugi::xml_node &wpt_node, bool b_fullviz,
                      bool b_layer,
                      bool b_layerviz,
                      int layer_id, wxString *pPathType )
{
    wxString PathName;
    wxString DescString;
    bool b_propviz = false;
    bool b_viz = true;
    bool b_propactive = false;
    bool b_active = false;
    Boundary    *pTentBoundary = NULL;
    Path        *pTentPath = NULL;
    HyperlinkList *linklist = NULL;
    
    wxString Name = wxString::FromUTF8( wpt_node.name() );
    if( Name == _T ( "opencpn:path" ) ) {
//    if( Name == _T ( "bnd" ) ) {
        if ( pPathType->compare( wxS("Boundary") ) ) {
            pTentBoundary = new Boundary();
            pTentPath = pTentBoundary;
        } else pTentPath = new Path();
        
        OCPNPoint *pOp = NULL;
        
        for( pugi::xml_node tschild = wpt_node.first_child(); tschild; tschild = tschild.next_sibling() ) {
            wxString ChildName = wxString::FromUTF8( tschild.name() );

            if( ChildName == _T ( "opencpn:OCPNPoint" ) ) {
//            if( ChildName == _T ( "bndpt" ) ) {
                OCPNPoint *tpOp = GPXLoadOCPNPoint1(  tschild, _T("square"), _T(""), b_fullviz, b_layer, b_layerviz, layer_id);
                OCPNPoint *epp = OCPNPointExists( tpOp->m_GUID );
                if( epp != NULL )
                    pOp = epp;
                else
                    pOp = tpOp;
                
                pTentPath->AddPoint( pOp, false, true, true );          // defer BBox calculation
                pOp->m_bIsInBoundary = true;                      // Hack
                
                if( epp == NULL )
                    pOCPNPointMan->AddOCPNPoint( pOp );
                else
                    delete tpOp;
                    
            }
            else
            if( ChildName == _T ( "name" ) ) {
                PathName = wxString::FromUTF8( tschild.first_child().value() );
            }
            else
            if( ChildName == _T ( "desc" ) ) {
                DescString = wxString::FromUTF8( tschild.first_child().value() );
            }
                
            else
            //TODO: This is wrong, left here just to save data of the 3.3 beta series users. 
/*            if( ChildName.EndsWith( _T ( "BoundaryExtension" ) ) ) //Parse GPXX color
            {
                for( pugi::xml_node gpxx_child = tschild.first_child(); gpxx_child; gpxx_child = gpxx_child.next_sibling() ) {
                    wxString gpxx_name = wxString::FromUTF8( gpxx_child.name() );
                    if( gpxx_name.EndsWith( _T ( "LineColour" ) ) )
                         pTentBoundary->m_FillColour = wxString::FromUTF8(gpxx_child.first_child().value() );
                    if( gpxx_name.EndsWith( _T ( "FillColour" ) ) )
                         pTentBoundary->m_Colour = wxString::FromUTF8(gpxx_child.first_child().value() );
                }
            }
            
            else
*/            if( ChildName == _T ( "link") ) {
                wxString HrefString;
                wxString HrefTextString;
                wxString HrefTypeString;
                if( linklist == NULL )
                    linklist = new HyperlinkList;
                HrefString = wxString::FromUTF8( tschild.first_attribute().value() );

                for( pugi::xml_node child1 = tschild.first_child(); child1; child1 = child1.next_sibling() ) {
                    wxString LinkString = wxString::FromUTF8( child1.name() );

                    if( LinkString == _T ( "text" ) )
                        HrefTextString = wxString::FromUTF8( child1.first_child().value() );
                    if( LinkString == _T ( "type" ) ) 
                        HrefTypeString = wxString::FromUTF8( child1.first_child().value() );
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
                        b_propactive = true;
                        b_active = ( active == _T("1") );
            }
            
            else
            if( ChildName == _T ( "opencpn:style" ) ) {
                for (pugi::xml_attribute attr = tschild.first_attribute(); attr; attr = attr.next_attribute())
                {
                    if ( wxString::FromUTF8( attr.name() ) == _T("colour" ) )
                        pTentPath->m_LineColour = wxString::FromUTF8( attr.as_string() );
                    else if ( wxString::FromUTF8( attr.name() ) == _T("fillcolour" ) )
                        pTentPath->m_FillColour = wxString::FromUTF8( attr.as_string() );
                    else if ( wxString::FromUTF8( attr.name() ) == _T("style" ) )
                        pTentPath->m_style = attr.as_int();
                    else if ( wxString::FromUTF8( attr.name() ) == _T("width" ) )
                        pTentPath->m_width = attr.as_int();
                }
             }
             
             else
             if( ChildName == _T ( "opencpn:guid" ) ) {
                //if ( !g_bODIsNewLayer ) ) 
                pTentBoundary->m_GUID =  wxString::FromUTF8(tschild.first_child().value());
             }
             
             else
             if( ChildName == _T ( "opencpn:time_display" ) ) {
                pTentBoundary->m_TimeDisplayFormat, wxString::FromUTF8(tschild.first_child().value());
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
    pTentPath->UpdateSegmentDistances();
    return pTentPath;
}

OCPNPoint *OCPNDrawNavObjectChanges::OCPNPointExists( const wxString& name, double lat, double lon )
{
    OCPNPoint *pret = NULL;
//    if( g_bODIsNewLayer ) return NULL;
    wxOCPNPointListNode *node = pOCPNPointMan->GetOCPNPointList()->GetFirst();
    bool Exists = false;
    while( node ) {
        OCPNPoint *pr = node->GetData();

//        if( pr->m_bIsInLayer ) return NULL;

        if( name == pr->GetName() ) {
            if( fabs( lat - pr->m_lat ) < 1.e-6 && fabs( lon - pr->m_lon ) < 1.e-6 ) {
                Exists = true;
                pret = pr;
                break;
            }
        }
        node = node->GetNext();
    }

    return pret;
}

OCPNPoint *OCPNDrawNavObjectChanges::OCPNPointExists( const wxString& guid )
{
    wxOCPNPointListNode *node = pOCPNPointMan->GetOCPNPointList()->GetFirst();
    while( node ) {
        OCPNPoint *pr = node->GetData();

//        if( pr->m_bIsInLayer ) return NULL;

        if( guid == pr->m_GUID ) {
            return pr;
        }
        node = node->GetNext();
    }

    return NULL;
}

void OCPNDrawNavObjectChanges::InsertPathA( Path *pTentPath )
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
                               pTentPath->m_GUID = pOCPNPointMan->CreateGUID( NULL );
                               //Now also change guids for the routepoints
                               wxOCPNPointListNode *pthisnode = ( pTentPath->pOCPNPointList )->GetFirst();
                               while( pthisnode ) {
                                   OCPNPoint *pP =  pthisnode->GetData();
                                   if( pP && pP->m_bIsolatedMark )
                                        pP->m_GUID = pOCPNPointMan->CreateGUID( NULL );
                                   pthisnode = pthisnode->GetNext();
                               }
            }
            
        pPathList->Append( pTentPath );
        pTentPath->RebuildGUIDList();                  // ensure the GUID list is intact
        
                 
    //    Do the (deferred) calculation of BBox
        pTentPath->FinalizeForRendering();

        
        //    Add the selectable points and segments
        
        int ip = 0;
        float prev_rlat = 0., prev_rlon = 0.;
        OCPNPoint *prev_pConfPoint = NULL;
        
        wxOCPNPointListNode *node = pTentPath->pOCPNPointList->GetFirst();
        while( node ) {
            OCPNPoint *pop = node->GetData();
            
            if( ip )
                pOCPNSelect->AddSelectablePathSegment( prev_rlat, prev_rlon, pop->m_lat,
                                                    pop->m_lon, prev_pConfPoint, pop, pTentPath );
            pOCPNSelect->AddSelectableOCPNPoint(pop->m_lat, pop->m_lon, pop);
            prev_rlat = pop->m_lat;
            prev_rlon = pop->m_lon;
            prev_pConfPoint = pop;
            
            ip++;
            
            node = node->GetNext();
        }
    }
    else {
        
        // walk the path, deleting points used only by this path
        wxOCPNPointListNode *pnode = ( pTentPath->pOCPNPointList )->GetFirst();
        while( pnode ) {
            OCPNPoint *pop = pnode->GetData();
            
            // check all other paths to see if this point appears in any other path
            Path *pcontainer_path = g_pPathMan->FindPathContainingOCPNPoint( pop );
            
            if( pcontainer_path == NULL ) {
                pop->m_bIsInPath = false; // Take this point out of this (and only) track/route
                if( !pop->m_bKeepXPath ) {
                    pOCPNDrawConfig->m_bSkipChangeSetUpdate = true;
                    pOCPNDrawConfig->DeleteOCPNPoint( pop );
                    pOCPNDrawConfig->m_bSkipChangeSetUpdate = false;
                    delete pop;
                }
            }
            
            pnode = pnode->GetNext();
        }
        
        delete pTentPath;
    }
}
                       
Path *OCPNDrawNavObjectChanges::PathExists( const wxString& guid )
{
    wxPathListNode *path_node = pPathList->GetFirst();

    while( path_node ) {
        Path *ppath = path_node->GetData();

        if( guid == ppath->m_GUID ) return ppath;

        path_node = path_node->GetNext();
    }
    return NULL;
}

Path *OCPNDrawNavObjectChanges::PathExists( Path * pTentPath )
{
    wxPathListNode *path_node = pPathList->GetFirst();
    while( path_node ) {
        Path *ppath = path_node->GetData();

        if( ppath->IsEqualTo( pTentPath ) ) {
            return ppath;
        }

        path_node = path_node->GetNext();       // next path
    }
    return NULL;
}

