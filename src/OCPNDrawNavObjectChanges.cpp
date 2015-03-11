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

extern PathList *pPathList;
extern OCPNSelect *pSelect;
pugi::xml_node  gpx_path_child;
bool            m_bFirstPath;
extern OCPNDrawConfig *pConfig;


OCPNDrawNavObjectChanges::OCPNDrawNavObjectChanges()
{
    //ctor
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
        child = node.append_child("type");
//        child.append_child(pugi::node_pcdata).set_value(pr->m_sTypeString);
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
            s.Printf(_T("%.3f"), pr->GetWaypointArrivalRadius());
            child.append_child(pugi::node_pcdata).set_value( s.mbc_str() );
        }
    }
    
    return true;
}

bool GPXCreatePath( pugi::xml_node node, Path *pPath )
{
    pugi::xml_node child;
    
    child = node.append_child("opencpn:type");
//    child.append_child(pugi::node_pcdata).set_value(pPath->m_sTypeString);
    
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
    
/*    pugi::xml_node child_ext = node.append_child("extensions");
    
    child = child_ext.append_child("opencpn:type");
    child.append_child(pugi::node_pcdata).set_value(pPath->m_sTypeString);
    
    child = child_ext.append_child("opencpn:guid");
    child.append_child(pugi::node_pcdata).set_value(pPath->m_GUID.mb_str());
    
    child = child_ext.append_child("opencpn:viz");
    child.append_child(pugi::node_pcdata).set_value(pPath->IsVisible() == true ? "1" : "0");

    child = child_ext.append_child("opencpn:active");
    child.append_child(pugi::node_pcdata).set_value(pPath->m_bPathIsActive == true ? "1" : "0");
    
//    if( pBoundary->m_width != STYLE_UNDEFINED || pBoundary->m_style != STYLE_UNDEFINED  || pBoundary->m_Colour.length() > 0 || pBoundary->m_FillColour.length() > 0 ) {
        child = child_ext.append_child("opencpn:style");
        
        pugi::xml_attribute colour = child.append_attribute("colour");
        colour.set_value( pPath->m_LineColour.ToAscii() );
        pugi::xml_attribute fillcolour = child.append_attribute("fillcolour");
        fillcolour.set_value( pPath->m_FillColour.ToAscii() );
        child.append_attribute("width") = pPath->m_width;
        child.append_attribute("style") = pPath->m_style;
//    }
*/    
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
//        GPXCreateWpt(node.append_child("bndpt"), prp, OPT_BOUNDARYPT);
            
        node2 = node2->GetNext();
    }
    
    return true;
}
                       
bool OCPNDrawNavObjectChanges::AddPath( Path *pb, const char *action )
{
    SetRootGPXNode();
    
    pugi::xml_node child_ext = m_gpx_root.append_child("extensions");
    GPXCreatePath(child_ext.append_child("opencpn:path"), pb);
    
//    pugi::xml_node xchild = object.child("extensions");
    pugi::xml_node xchild = child_ext.child("extensions");
    //FIXME  What if extensions do not exist?
    pugi::xml_node child = xchild.append_child("opencpn:action");
    child.append_child(pugi::node_pcdata).set_value(action);

    pugi::xml_writer_file writer(m_changes_file);
    child_ext.print(writer, " ");
    fflush(m_changes_file);
    
    return true;
}

bool OCPNDrawNavObjectChanges::AddOCPNPoint( OCPNPoint *pWP, const char *action )
{
    SetRootGPXNode();
    
    pugi::xml_node child_ext = m_gpx_root.append_child("extensions");
    GPXCreateOCPNPoint(child_ext.append_child("opencpn:OCPNPoint"), pWP, OPT_OCPNPOINT);

    pugi::xml_node xchild = child_ext.child("extensions");
    pugi::xml_node child = xchild.append_child("opencpn:action");
    child.append_child(pugi::node_pcdata).set_value(action);

    pugi::xml_writer_file writer(m_changes_file);
    child_ext.print(writer, " ");
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
        gpx_path_child = m_gpx_root.append_child("extensions");
    }
    GPXCreatePath(gpx_path_child.append_child("opencpn:path"), pPath);
//    GPXCreateBoundary(m_gpx_root.append_child("bnd"), pBoundary);
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
    child_ext = m_gpx_root.append_child("extensions");
     while( node1 ) {
        Path *pBoundary = node1->GetData();
        
        if( !pBoundary->m_bIsInLayer && !pBoundary->m_btemp )
            GPXCreatePath(child_ext.append_child("opencpn:path"), pBoundary);
        node1 = node1->GetNext();
    }
    
    
    return true;
}

bool OCPNDrawNavObjectChanges::CreateAllGPXObjects()
{
    bool bret = NavObjectCollection1::CreateAllGPXObjects();
    CreateNavObjGPXPaths();
    
    return true;
}

