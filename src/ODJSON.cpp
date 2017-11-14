/******************************************************************************
 * $Id: ocpn_draw_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN General Drawing Plugin
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register                               *
 *   $EMAIL$                                                               *
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
 ***************************************************************************
 */
//#define _2_9_x_ // uncomment this to compile for 2.9.x

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers
#include <wx/jsonreader.h>
#include "wx/jsonwriter.h"

#include "ocpn_draw_pi.h"
#include "ODJSON.h"
#include "ODPath.h"
#include "PathMan.h"
#include "Boundary.h"
#include "BoundaryMan.h"
#include "BoundaryPoint.h"
#include "GZ.h"
#include "GZMan.h"
#include "ODAPI.h"
#include "PointMan.h"
#include "version.h"

#include <stdio.h>

extern PathMan              *g_pPathMan;
extern BoundaryMan          *g_pBoundaryMan;
extern GZMan                *g_pGZMan;
extern PointMan             *g_pODPointMan;

extern double               g_dVar;
extern ODAPI                *g_pODAPI;

ODJSON::ODJSON()
{
    // ctor
}

ODJSON::~ODJSON()
{
    // dtor
}

void ODJSON::ProcessMessage(wxString &message_id, wxString &message_body)
{
    wxJSONValue     root;
    wxJSONReader    reader;
    wxJSONValue     jMsg;
    wxJSONWriter    writer;
    wxString        MsgString;
    
    wxString    sLogMessage;
    wxString    l_sType;
    wxString    l_sMsg;
    double      l_dLat;
    double      l_dLon;
    double      l_dStartLat;
    double      l_dStartLon;
    double      l_dEndLat;
    double      l_dEndLon;
    double      l_dCrossingLat;
    double      l_dCrossingLon;
    double      l_dCrossingDist;
    wxString    l_GUID;
    int         l_BoundaryType;
    int         l_BoundaryState;
    bool        bFail = false;
    
    if(message_id == wxS("OCPN_DRAW_PI")) {
        // now read the JSON text and store it in the 'root' structure
        // check for errors before retreiving values...
        int numErrors = reader.Parse( message_body, &root );
        if ( numErrors > 0 )  {
            const wxArrayString& errors = reader.GetErrors();
            for(size_t i = 0; i < errors.GetCount(); i++)
            {
                if(i == 0) {
                    sLogMessage.clear();
                    sLogMessage.Append(wxT("ocpn_draw_pi: Error parsing JSON message - "));
                    sLogMessage.Append( message_id );
                }
                else sLogMessage.Append(wxT(" "));
                sLogMessage.append( errors.Item( i ) );
                wxLogMessage( sLogMessage );
            }
            return;
        }
        
        if(!root.HasMember( wxS("Source"))) {
            // Originator
            wxLogMessage( wxS("No Source found in message") );
            bFail = true;
        }
        
        if(!root.HasMember( wxS("Msg"))) {
            // Message identifier
            wxLogMessage( wxS("No Msg found in message") );
            bFail = true;
        }
        
        if(!root.HasMember( wxS("Type"))) {
            // Message type, orig or resp
            wxLogMessage( wxS("No Type found in message") );
            bFail = true;
        }
        
        if(!root.HasMember( wxS("MsgId"))) {
            // Unique (?) Msg number
            wxLogMessage( wxS("No MsgId found in message") );
            bFail = true;
        }

        if(!bFail && root[wxS("Msg")].AsString() == wxS("Version")) {
            jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
            jMsg[wxT("Msg")] = root[wxT("Msg")];
            jMsg[wxT("Type")] = wxT("Response");
            jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
            jMsg[wxS("Major")] = PLUGIN_VERSION_MAJOR;
            jMsg[wxS("Minor")] = PLUGIN_VERSION_MINOR;
            jMsg[wxS("Patch")] = PLUGIN_VERSION_PATCH;
            jMsg[wxS("Date")] = PLUGIN_VERSION_DATE;
            writer.Write( jMsg, MsgString );
            SendPluginMessage( root[wxS("Source")].AsString(), MsgString );
            
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("GetAPIAddresses")) {
            if(root[wxS("Type")].AsString() == wxS("Request")) {
                jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                jMsg[wxT("Msg")] = root[wxT("Msg")];
                jMsg[wxT("Type")] = wxT("Response");
                jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                jMsg[wxT("Found")] = true;
                char ptr[64];
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_FindPathByGUID );
                jMsg[wxT("OD_FindPathByGUID")] = wxString::From8BitData(ptr);
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_FindPointInAnyBoundary );
                jMsg[wxT("OD_FindPointInAnyBoundary")] = wxString::From8BitData(ptr);
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_FindClosestBoundaryLineCrossing );
                jMsg[wxT("OD_FindClosestBoundaryLineCrossing")] = wxString::From8BitData(ptr);
                writer.Write( jMsg, MsgString );
                
                SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                return;
            }
            
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("FindPathByGUID")) {
            if(!root.HasMember( wxS("GUID"))) {
                wxLogMessage( wxS("No GUID found in message") );
                bFail = true;
            }
            
            if(!bFail) {
                wxString l_sGUID = root[wxS("GUID")].AsString();
                l_sType = root[wxS("Type")].AsString();
                l_sMsg = root[wxT("Msg")].AsString();
                
                if(l_sType == wxS("Request")) {
                    ODPath *l_path = NULL;
                        l_path = g_pPathMan->FindPathByGUID( l_sGUID );
                        if(!l_path) {
                            wxString l_msg;
                            l_msg.append( wxS("Path, with GUID: ") );
                            l_msg.append( l_sGUID );
                            l_msg.append( wxS(", not found") );
                            wxLogMessage( l_msg );
                            jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                            jMsg[wxT("Msg")] = root[wxT("Msg")];
                            jMsg[wxT("Type")] = wxT("Response");
                            jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                            jMsg[wxS("Found")] = false;
                            jMsg[wxS("GUID")] = root[wxS("GUID")];
                            writer.Write( jMsg, MsgString );
                            SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                            return;
                        }
                        jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                        jMsg[wxT("Msg")] = root[wxT("Msg")];
                        jMsg[wxT("Type")] = wxT("Response");
                        jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                        jMsg[wxT("Found")] = true;
                        jMsg[wxT("GUID")] = root[wxS("GUID")];
                        jMsg[wxT("Name")] = l_path->m_PathNameString;
                        jMsg[wxT("Description")] = l_path->m_PathDescription;
                        writer.Write( jMsg, MsgString );
                        SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                        return;
                }
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("FindPointInAnyBoundary")) {
            if(!root.HasMember( wxS("lat"))) {
                wxLogMessage( wxS("No Latitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("lon"))) {
                wxLogMessage( wxS("No Longitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("BoundaryType"))) {
                wxLogMessage( wxS("No Boundary Type found in message") );
                bFail = true;
            }
            
            if(!bFail) {
                l_dLat = root[wxS("lat")].AsDouble();
                l_dLon = root[wxS("lon")].AsDouble();
                
                l_sType = root[wxS("Type")].AsString();
                l_sMsg = root[wxT("Msg")].AsString();
                
                if(root[wxT("BoundaryType")].AsString() == wxT("Exclusion")) l_BoundaryType = ID_BOUNDARY_EXCLUSION;
                else if(root[wxT("BoundaryType")].AsString() == wxT("Inclusion")) l_BoundaryType = ID_BOUNDARY_INCLUSION;
                else if(root[wxT("BoundaryType")].AsString() == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_NIETHER;
                else if(root[wxT("BoundaryType")].AsString() == wxT("Any")) l_BoundaryType = ID_BOUNDARY_ANY;
                else l_BoundaryType = ID_BOUNDARY_ANY;
                
                l_BoundaryState = ID_BOUNDARY_ANY;
                if(root[wxT("BoundaryState")].AsString() == wxT("Active")) l_BoundaryState = ID_PATH_STATE_ACTIVE;
                else if(root[wxT("BoundaryState")].AsString() == wxT("Inactive")) l_BoundaryState = ID_PATH_STATE_INACTIVE;
                else if(root[wxT("BoundaryState")].AsString() == wxT("Any")) l_BoundaryState = ID_PATH_STATE_ANY;
                
                if(l_sType == wxS("Request")) {
                    bool    l_bFoundBoundary = false;
                    bool    l_bFoundBoundaryPoint = false;
                    wxString l_sGUID = g_pBoundaryMan->FindPointInBoundary( l_dLat, l_dLon, l_BoundaryType, l_BoundaryState );
                    if(l_sGUID.length() > 0) 
                        l_bFoundBoundary = true;
                    else {
                        l_sGUID = g_pBoundaryMan->FindPointInBoundaryPoint( l_dLat, l_dLon, l_BoundaryType );
                        if(l_sGUID.length() > 0)
                            l_bFoundBoundaryPoint = true;
                    }
                    jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                    jMsg[wxT("Msg")] = root[wxT("Msg")];
                    jMsg[wxT("Type")] = wxT("Response");
                    jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                    jMsg[wxS("GUID")] = l_sGUID;
                    jMsg[wxS("lat")] = l_dLat;
                    jMsg[wxS("lon")] = l_dLon;
                    if(l_bFoundBoundary ) {
                        Boundary *l_boundary = (Boundary *)g_pBoundaryMan->FindPathByGUID( l_sGUID );
                        jMsg[wxS("Name")] = l_boundary->m_PathNameString;
                        jMsg[wxS("Description")] = l_boundary->m_PathDescription;
                        jMsg[wxS("Found")] = true;
                        jMsg[wxS("BoundaryObjectType")] = wxT("Boundary");
                        if( l_boundary->m_bExclusionBoundary && !l_boundary->m_bInclusionBoundary)
                            jMsg[wxS("BoundaryType")] = wxT("Exclusion");
                        else if( !l_boundary->m_bExclusionBoundary && l_boundary->m_bInclusionBoundary)
                            jMsg[wxS("BoundaryType")] = wxT("Inclusion");
                        else if( !l_boundary->m_bExclusionBoundary && !l_boundary->m_bInclusionBoundary)
                            jMsg[wxS("BoundaryType")] = wxT("Neither");
                        else
                            jMsg[wxS("BoundaryType")] = wxT("Unknown");
                    }
                    else if(l_bFoundBoundaryPoint ) {
                        jMsg[wxS("Found")] = true;
                        BoundaryPoint *l_boundarypoint = (BoundaryPoint *)g_pODPointMan->FindODPointByGUID( l_sGUID );
                        jMsg[wxS("Name")] = l_boundarypoint->m_ODPointName;
                        jMsg[wxS("Description")] = l_boundarypoint->m_ODPointDescription;
                        jMsg[wxS("BoundaryObjectType")] = wxT("Boundary Point");
                        if( l_boundarypoint->m_bExclusionBoundaryPoint && !l_boundarypoint->m_bInclusionBoundaryPoint)
                            jMsg[wxS("BoundaryType")] = wxT("Exclusion");
                        else if( !l_boundarypoint->m_bExclusionBoundaryPoint && l_boundarypoint->m_bInclusionBoundaryPoint)
                            jMsg[wxS("BoundaryType")] = wxT("Inclusion");
                        else if( !l_boundarypoint->m_bExclusionBoundaryPoint && !l_boundarypoint->m_bInclusionBoundaryPoint)
                            jMsg[wxS("BoundaryType")] = wxT("Neither");
                        else
                            jMsg[wxS("BoundaryType")] = wxT("Unknown");
                    }
                    else jMsg[wxS("Found")] = false;
                    writer.Write( jMsg, MsgString );
                    SendPluginMessage( root[wxS("Source")].AsString(), MsgString );
                    return;
                }
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("FindClosestBoundaryLineCrossing")) {
            if(!root.HasMember( wxS("StartLat"))) {
                wxLogMessage( wxS("No Start Latitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("StartLon"))) {
                wxLogMessage( wxS("No Start Longitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("EndLat"))) {
                wxLogMessage( wxS("No End Latitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("EndLon"))) {
                wxLogMessage( wxS("No End Longitude found in message") );
                bFail = true;
            }
            
            if(!bFail) {
                root[wxS("StartLat")].AsString().ToDouble( & l_dStartLat );
                root[wxS("StartLon")].AsString().ToDouble( & l_dStartLon );
                root[wxS("EndLat")].AsString().ToDouble( & l_dEndLat );
                root[wxS("EndLon")].AsString().ToDouble( & l_dEndLon );
                
                l_sType = root[wxS("Type")].AsString();
                l_sMsg = root[wxT("Msg")].AsString();
                
                if(root[wxT("BoundaryType")].AsString() == wxT("Exclusion")) l_BoundaryType = ID_BOUNDARY_EXCLUSION;
                else if(root[wxT("BoundaryType")].AsString() == wxT("Inclusion")) l_BoundaryType = ID_BOUNDARY_INCLUSION;
                else if(root[wxT("BoundaryType")].AsString() == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_NIETHER;
                else if(root[wxT("BoundaryType")].AsString() == wxT("Any")) l_BoundaryType = ID_BOUNDARY_ANY;
                else l_BoundaryType = ID_BOUNDARY_ANY;
                
                l_BoundaryState = ID_BOUNDARY_ANY;
                if(root[wxT("BoundaryState")].AsString() == wxT("Active")) l_BoundaryState = ID_PATH_STATE_ACTIVE;
                else if(root[wxT("BoundaryState")].AsString() == wxT("Inactive")) l_BoundaryState = ID_PATH_STATE_INACTIVE;
                else if(root[wxT("BoundaryState")].AsString() == wxT("Any")) l_BoundaryState = ID_PATH_STATE_ANY;
                
                
                if(l_sType == wxS("Request")) {
                    bool    l_bFoundBoundary = false;
                    wxString l_sGUID = g_pBoundaryMan->FindLineCrossingBoundary( l_dStartLon, l_dStartLat, l_dEndLon, l_dEndLat, &l_dCrossingLon, &l_dCrossingLat, &l_dCrossingDist, l_BoundaryType, l_BoundaryState );
                    if(l_sGUID.length() > 0) 
                        l_bFoundBoundary = true;

                    jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                    jMsg[wxT("Msg")] = root[wxT("Msg")];
                    jMsg[wxT("Type")] = wxT("Response");
                    jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                    jMsg[wxS("GUID")] = l_sGUID;
                    jMsg[wxS("CrossingLat")] = l_dCrossingLat;
                    jMsg[wxS("CrossingLon")] = l_dCrossingLon;
                    jMsg[wxS("CrossingDist")] = l_dCrossingDist;
                    if(l_bFoundBoundary ) {
                        Boundary *l_boundary = (Boundary *)g_pBoundaryMan->FindPathByGUID( l_sGUID );
                        jMsg[wxS("Name")] = l_boundary->m_PathNameString;
                        jMsg[wxS("Description")] = l_boundary->m_PathDescription;
                        jMsg[wxS("Found")] = true;
                        jMsg[wxS("BoundaryObjectType")] = wxT("Boundary");
                        if( l_boundary->m_bExclusionBoundary && !l_boundary->m_bInclusionBoundary)
                            jMsg[wxS("BoundaryType")] = wxT("Exclusion");
                        else if( !l_boundary->m_bExclusionBoundary && l_boundary->m_bInclusionBoundary)
                            jMsg[wxS("BoundaryType")] = wxT("Inclusion");
                        else if( !l_boundary->m_bExclusionBoundary && !l_boundary->m_bInclusionBoundary)
                            jMsg[wxS("BoundaryType")] = wxT("Neither");
                        else
                            jMsg[wxS("BoundaryType")] = wxT("Unknown");
                    }
                    else jMsg[wxS("Found")] = false;
                    writer.Write( jMsg, MsgString );
                    SendPluginMessage( root[wxS("Source")].AsString(), MsgString );
                    return;
                }
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("FindPointInBoundary")) {
            if(!root.HasMember( wxS("GUID"))) {
                wxLogMessage( wxS("No GUID found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("lat"))) {
                wxLogMessage( wxS("No Latitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("lon"))) {
                wxLogMessage( wxS("No Longitude found in message") );
                bFail = true;
            }
            
            if(!bFail) {
                wxString l_sGUID = root[wxS("GUID")].AsString();
                l_dLat = root[wxS("lat")].AsDouble();
                l_dLon = root[wxS("lon")].AsDouble();
                
                l_sType = root[wxS("Type")].AsString();
                l_sMsg = root[wxT("Msg")].AsString();
                
                if(l_sType == wxS("Request")) {
                    Boundary *l_boundary = NULL;
                    BoundaryPoint *l_boundarypoint = NULL;
                    if(l_sMsg == wxS("FindPointInBoundary")) {
                        l_dLat = root[wxS("lat")].AsDouble();
                        l_dLon = root[wxS("lon")].AsDouble();
                        
                        l_boundary = (Boundary *)g_pBoundaryMan->FindPathByGUID( l_sGUID );
                        if(!l_boundary) l_boundarypoint = (BoundaryPoint *)g_pODPointMan->FindODPointByGUID( l_sGUID );
                        if(!l_boundary && !l_boundarypoint) {
                            wxString l_msg;
                            l_msg.append( wxS("Boundary, with GUID: ") );
                            l_msg.append( l_sGUID );
                            l_msg.append( wxS(", not found") );
                            wxLogMessage( l_msg );
                            jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                            jMsg[wxT("Msg")] = root[wxT("Msg")];
                            jMsg[wxT("Type")] = wxT("Response");
                            jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                            jMsg[wxS("Found")] = false;
                            jMsg[wxS("lat")] = l_dLat;
                            jMsg[wxS("lon")] = l_dLon;
                            jMsg[wxS("GUID")] = root[wxS("GUID")];
                            writer.Write( jMsg, MsgString );
                            SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                            return;
                        }
                        bool l_bFound = false;
                        if(l_boundary) l_bFound = g_pBoundaryMan->FindPointInBoundary( l_boundary, l_dLat, l_dLon );
                        else if(l_boundarypoint) l_bFound = g_pBoundaryMan->FindPointInBoundaryPoint( l_boundarypoint, l_dLat, l_dLon );
                        jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                        jMsg[wxT("Msg")] = root[wxT("Msg")];
                        jMsg[wxT("Type")] = wxT("Response");
                        jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                        jMsg[wxS("Found")] = l_bFound;
                        jMsg[wxS("lat")] = l_dLat;
                        jMsg[wxS("lon")] = l_dLon;
                        jMsg[wxS("GUID")] = root[wxS("GUID")];
                        if(l_boundary) {
                            jMsg[wxS("BoundaryObjectType")] = wxT("Boundary");
                            if( l_boundary->m_bExclusionBoundary && !l_boundary->m_bInclusionBoundary)
                                jMsg[wxS("BoundaryType")] = wxT("Exclusion");
                            else if( !l_boundary->m_bExclusionBoundary && l_boundary->m_bInclusionBoundary)
                                jMsg[wxS("BoundaryType")] = wxT("Inclusion");
                            else if( !l_boundary->m_bExclusionBoundary && !l_boundary->m_bInclusionBoundary)
                                jMsg[wxS("BoundaryType")] = wxT("Neither");
                            else
                                jMsg[wxS("BoundaryType")] = wxT("Unknown");
                        }
                        else {
                            jMsg[wxS("BoundaryObjectType")] = wxT("Boundary Point");
                            if( l_boundarypoint->m_bExclusionBoundaryPoint && !l_boundarypoint->m_bInclusionBoundaryPoint)
                                jMsg[wxS("BoundaryType")] = wxT("Exclusion");
                            else if( !l_boundarypoint->m_bExclusionBoundaryPoint && l_boundarypoint->m_bInclusionBoundaryPoint)
                                jMsg[wxS("BoundaryType")] = wxT("Inclusion");
                            else if( !l_boundarypoint->m_bExclusionBoundaryPoint && !l_boundarypoint->m_bInclusionBoundaryPoint)
                                jMsg[wxS("BoundaryType")] = wxT("Neither");
                            else
                                jMsg[wxS("BoundaryType")] = wxT("Unknown");
                        }
                        writer.Write( jMsg, MsgString );
                        SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                        return;
                    }
                }
            }
            
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("FindPointInGuardZone")) {
            if(!root.HasMember( wxS("GUID"))) {
                wxLogMessage( wxS("No GUID found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("lat"))) {
                wxLogMessage( wxS("No Latitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("lon"))) {
                wxLogMessage( wxS("No Longitude found in message") );
                bFail = true;
            }
            
            if(!bFail) {
                wxString l_sGUID = root[wxS("GUID")].AsString();
                root[wxS("lat")].AsString().ToDouble( & l_dLat );
                root[wxS("lon")].AsString().ToDouble( & l_dLon );
                
                l_sType = root[wxS("Type")].AsString();
                l_sMsg = root[wxT("Msg")].AsString();
                
                if(l_sType == wxS("Request")) {
                    ODPath *l_path = NULL;
                    BoundaryPoint *l_boundarypoint = NULL;

                    l_path = g_pPathMan->FindPathByGUID( l_sGUID );
                    if(!l_path) l_boundarypoint = (BoundaryPoint *)g_pODPointMan->FindODPointByGUID( l_sGUID );
                    if(!l_path && !l_boundarypoint) {
                        wxString l_msg;
                        l_msg.append( wxS("Guard Zone, with GUID: ") );
                        l_msg.append( l_sGUID );
                        l_msg.append( wxS(", not found") );
                        wxLogMessage( l_msg );
                        jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                        jMsg[wxT("Msg")] = root[wxT("Msg")];
                        jMsg[wxT("Type")] = wxT("Response");
                        jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                        jMsg[wxS("Found")] = false;
                        jMsg[wxS("lat")] = l_dLat;
                        jMsg[wxS("lon")] = l_dLon;
                        jMsg[wxS("GUID")] = root[wxS("GUID")];
                        writer.Write( jMsg, MsgString );
                        SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                        return;
                    }
                    bool l_bFound = false;
                    if(root[wxT("BoundaryType")].AsString() == wxT("Exclusion")) l_BoundaryType = ID_BOUNDARY_EXCLUSION;
                    else if(root[wxT("BoundaryType")].AsString() == wxT("Inclusion")) l_BoundaryType = ID_BOUNDARY_INCLUSION;
                    else if(root[wxT("BoundaryType")].AsString() == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_NIETHER;
                    else if(root[wxT("BoundaryType")].AsString() == wxT("Any")) l_BoundaryType = ID_BOUNDARY_ANY;
                    else l_BoundaryType = ID_BOUNDARY_ANY;
                    
                    l_BoundaryState = ID_BOUNDARY_ANY;
                    if(root[wxT("BoundaryState")].AsString() == wxT("Active")) l_BoundaryState = ID_PATH_STATE_ACTIVE;
                    else if(root[wxT("BoundaryState")].AsString() == wxT("Inactive")) l_BoundaryState = ID_PATH_STATE_INACTIVE;
                    else if(root[wxT("BoundaryState")].AsString() == wxT("Any")) l_BoundaryState = ID_PATH_STATE_ANY;
                    
                    if(l_path) {
                        if(l_path->m_sTypeString == wxT("Guard Zone"))
                            l_bFound = g_pGZMan->FindPointInGZ( (GZ *)l_path, l_dLat, l_dLon, l_BoundaryType, l_BoundaryState  );
                        else
                            l_bFound = g_pBoundaryMan->FindPointInBoundary( (Boundary*)l_path, l_dLat, l_dLon, l_BoundaryType, l_BoundaryState );
                    } else
                        l_bFound = g_pBoundaryMan->FindPointInBoundaryPoint( l_boundarypoint, l_dLat, l_dLon );
                    jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                    jMsg[wxT("Msg")] = root[wxT("Msg")];
                    jMsg[wxT("Type")] = wxT("Response");
                    jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                    jMsg[wxS("Found")] = l_bFound;
                    jMsg[wxS("lat")] = l_dLat;
                    jMsg[wxS("lon")] = l_dLon;
                    if(l_path) {
                        jMsg[wxS("Active")] = l_path->m_bPathIsActive;
                        jMsg[wxS("Name")] = l_path->m_PathNameString;
                        jMsg[wxS("Description")] = l_path->m_PathDescription;
                    } else {
                        jMsg[wxS("Name")] = l_boundarypoint->m_ODPointName;
                        jMsg[wxS("Description")] = l_boundarypoint->m_ODPointDescription;
                    }
                    
                    jMsg[wxS("GUID")] = root[wxS("GUID")];
                    writer.Write( jMsg, MsgString );
                    SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                    return;
                }
            }
        }
        
    } else if(message_id == _T("WMM_VARIATION_BOAT")) {

    // construct the JSON root object
        wxJSONValue  root;
    // construct a JSON parser
        wxJSONReader reader;

    // now read the JSON text and store it in the 'root' structure
    // check for errors before retreiving values...
        int numErrors = reader.Parse( message_body, &root );
        if ( numErrors > 0 )  {
//              const wxArrayString& errors = reader.GetErrors();
            return;
        }

        // get the DECL value from the JSON message
        wxString decl = root[_T("Decl")].AsString();
        double decl_val;
        decl.ToDouble(&decl_val);

        g_dVar = decl_val;
    }
    
    return;
}

