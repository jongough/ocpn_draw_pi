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

#include "json-schema.hpp"
using nlohmann::json;
using nlohmann::json_uri;
using nlohmann::json_schema_draft4::json_validator;

#include "ocpn_draw_pi.h"
#include "ODJSON.h"
#include "ODPath.h"
#include "ODConfig.h"
#include "ODJSONSchemas.h"
#include "ODSelect.h"
#include "PathMan.h"
#include "Boundary.h"
#include "BoundaryMan.h"
#include "BoundaryPoint.h"
#include "GZ.h"
#include "GZMan.h"
#include "ODAPI.h"
#include "PointMan.h"
#include "TextPoint.h"
#include "version.h"

#include <stdio.h>

extern ocpn_draw_pi         *g_ocpn_draw_pi;
extern PathMan              *g_pPathMan;
extern BoundaryMan          *g_pBoundaryMan;
extern GZMan                *g_pGZMan;
extern PointMan             *g_pODPointMan;
extern BoundaryList         *g_pBoundaryList;
extern wxString             g_sODPointIconName;
extern ODConfig             *g_pODConfig;
extern ODSelect             *g_pODSelect;
extern PathManagerDialog    *g_pPathManagerDialog;
extern double               g_dVar;
extern ODAPI                *g_pODAPI;
extern bool                 g_bExclusionBoundaryPoint;
extern bool                 g_bInclusionBoundaryPoint;

json_validator *gCreateBoundary; 
json_validator *gCreateBoundaryPoint; 
json_validator *gCreateTextPoint;

ODJSON::ODJSON()
{
    // ctor
    gCreateBoundary = NULL;
    gCreateBoundaryPoint = NULL;
    gCreateTextPoint = NULL;
}

ODJSON::~ODJSON()
{
    // dtor
    if(gCreateBoundary) {
        delete gCreateBoundary;
        gCreateBoundary = NULL;
    }
    if(gCreateBoundaryPoint) {
        delete gCreateBoundaryPoint;
        gCreateBoundaryPoint = NULL;
    }
    if(gCreateTextPoint) {
        delete gCreateTextPoint;
        gCreateTextPoint = NULL;
    }
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
    
    DEBUGSL(message_body);
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
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_FindFirstBoundaryLineCrossing );
                jMsg[wxT("OD_FindFirstBoundaryLineCrossing")] = wxString::From8BitData(ptr);
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_CreateBoundary );
                jMsg[wxT("OD_CreateBoundary")] = wxString::From8BitData(ptr);
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_CreateBoundaryPoint );
                jMsg[wxT("OD_CreateBoundaryPoint")] = wxString::From8BitData(ptr);
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_CreateTextPoint );
                jMsg[wxT("OD_CreateTextPoint")] = wxString::From8BitData(ptr);
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
                else if(root[wxT("BoundaryType")].AsString() == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_NEITHER;
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
                else if(root[wxT("BoundaryType")].AsString() == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_NEITHER;
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
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("FindFirstBoundaryLineCrossing")) {
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
                else if(root[wxT("BoundaryType")].AsString() == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_NEITHER;
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
                    else if(root[wxT("BoundaryType")].AsString() == wxT("Neither")) l_BoundaryType = ID_BOUNDARY_NEITHER;
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
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("BoundaryInformation")) {
            
            wxJSONValue boundaries;
            
            wxBoundaryListNode *boundary_node = g_pBoundaryList->GetFirst();
            
            Boundary *pboundary = NULL;
            ODPoint *popSecond;
            
            while( boundary_node ) {  //all boundaries
                wxJSONValue current_boundary;
                wxJSONValue boundary_points;
                
                pboundary = boundary_node->GetData();
                wxODPointListNode *OCPNpoint_node = ( pboundary->m_pODPointList )->GetFirst();
                wxODPointListNode *OCPNpoint_next_node = OCPNpoint_node->GetNext();
                
                while( OCPNpoint_next_node ) {  //specific boundary
                    wxJSONValue current_point;
                    
                    popSecond = OCPNpoint_next_node->GetData();
                    
                    current_point[wxT("lat")] = popSecond->m_lat;
                    current_point[wxT("lon")] = popSecond->m_lon;
                    boundary_points.Append(current_point);
                    
                    OCPNpoint_next_node = OCPNpoint_next_node->GetNext();
                }
                
                current_boundary[wxT("BoundaryPoints")] = boundary_points;
                current_boundary[wxS("GUID")] = pboundary->m_GUID;
                current_boundary[wxS("Name")] = pboundary->m_PathNameString;
                current_boundary[wxS("Description")] = pboundary->m_PathDescription;
                
                if(pboundary->IsActive())
                    current_boundary[wxT("BoundaryState")] = wxT("Active");
                else
                    current_boundary[wxT("BoundaryState")] = wxT("Inactive");
                
                if(pboundary->m_bExclusionBoundary)
                    current_boundary[wxT("BoundaryType")] = wxT("Exclusion");
                else if(pboundary->m_bInclusionBoundary)
                    current_boundary[wxT("BoundaryType")] = wxT("Inclusion");
                else
                    current_boundary[wxT("BoundaryType")] = wxT("Neither");
                
                boundaries.Append(current_boundary);
                
                boundary_node = boundary_node->GetNext();    // next boundary
            }
            
            jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
            jMsg[wxT("Msg")] = root[wxT("Msg")];
            jMsg[wxT("Type")] = wxT("Response");
            jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
            jMsg[wxT("Boundaries")] = boundaries;
            
            writer.Write( jMsg, MsgString );
            SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
            
            return;
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("CreateBoundary")) {
            if(!gCreateBoundary) {
                gCreateBoundary = new json_validator;
                try {
                    gCreateBoundary->set_root_schema(BoundarySchema);
                } catch (const std::exception &e) {
                    DEBUGST("Validation of schema failed, here is why: ");
                    DEBUGEND(e.what());
                    wxString l_errorMsg;
                    l_errorMsg.Append("Validation of schema failed, here is why: ");
                    l_errorMsg.Append(e.what());
                    wxLogMessage( l_errorMsg );
                    bFail = true;
                }
            }
            if(!bFail) {
                try {
                    json message = json::parse(static_cast<const char*>(message_body));
                    gCreateBoundary->validate(message);
                } catch (const std::exception &e) {
                    DEBUGST("Validation failed, here is why: ");
                    DEBUGEND(e.what());
                    wxString l_errorMsg;
                    l_errorMsg.Append("Validation of schema failed, here is why: ");
                    l_errorMsg.Append(e.what());
                    wxLogMessage( l_errorMsg );
                    bFail = true;
                }
            }
            
            if(!bFail) {
                if(root[wxS("Type")].AsString() == _T("Request")) {
                    Boundary *pl_boundary = new Boundary();
                    pl_boundary->m_PathNameString = root[wxS("BoundaryName")].AsString();
                    if(root[wxS("BoundaryType")].AsString() == _T("Exclusion")) {
                        pl_boundary->m_bExclusionBoundary = true;
                        pl_boundary->m_bInclusionBoundary = false;
                    } else if(root[wxS("BoundaryType")].AsString() == _T("Inclusion")) {
                        pl_boundary->m_bExclusionBoundary = false;
                        pl_boundary->m_bInclusionBoundary = true;
                    } else if(root[wxS("BoundaryType")].AsString() == _T("Neither")) {
                        pl_boundary->m_bExclusionBoundary = false;
                        pl_boundary->m_bInclusionBoundary = false;
                    }
                    pl_boundary->m_bPathIsActive = true;
                    pl_boundary->SetVisible(root[wxS("visible")].AsBool());
                    if(root.HasMember(wxS("lineColour"))) pl_boundary->m_wxcActiveLineColour = root[wxS("lineColour")].AsString();
                    if(root.HasMember(wxS("fillColour"))) pl_boundary->m_wxcActiveFillColour = root[wxS("fillColour")].AsString();
                    
                    for(int i = 0; i< root[wxS("BoundaryPoints")].Size(); i++) {
                        wxJSONValue jv_BoundaryPoint = root[wxS("BoundaryPoints")].Item(i);
                        BoundaryPoint *pl_boundarypoint = new BoundaryPoint(jv_BoundaryPoint[wxS("Lat")].AsDouble(),jv_BoundaryPoint[wxS("Lon")].AsDouble(), g_sODPointIconName, jv_BoundaryPoint[wxS("Name")].AsString(),wxEmptyString, false);
                        pl_boundary->AddPoint(pl_boundarypoint, false, true, true);
                        if(jv_BoundaryPoint[wxS("BoundaryPointType")].AsString() == _T("Exclusion")) {
                            pl_boundarypoint->m_bExclusionBoundaryPoint = true;
                            pl_boundarypoint->m_bInclusionBoundaryPoint = false;
                        } else if(jv_BoundaryPoint[wxS("BoundaryPointType")].AsString() == _T("Inclusion")) {
                            pl_boundarypoint->m_bExclusionBoundaryPoint = false;
                            pl_boundarypoint->m_bInclusionBoundaryPoint = true;
                        } else if(jv_BoundaryPoint[wxS("BoundaryPointType")].AsString() == _T("Neither")) {
                            pl_boundarypoint->m_bExclusionBoundaryPoint = false;
                            pl_boundarypoint->m_bInclusionBoundaryPoint = false;
                        }
                        pl_boundarypoint->SetVisible(jv_BoundaryPoint[wxS("visible")].AsBool()); 
                        pl_boundarypoint->SetShowODPointRangeRings(jv_BoundaryPoint[wxS("ringsvisible")].AsBool());
                        pl_boundarypoint->SetODPointRangeRingsNumber(jv_BoundaryPoint[wxS("ringsnumber")].AsInt());
                        pl_boundarypoint->SetODPointRangeRingsStep(jv_BoundaryPoint[wxS("ringssteps")].AsDouble());
                        pl_boundarypoint->SetODPointRangeRingsStepUnits(jv_BoundaryPoint[wxS("ringsunits")].AsInt());
                        pl_boundarypoint->SetODPointRangeRingsColour(jv_BoundaryPoint[wxS("ringscolour")].AsString());
                        pl_boundarypoint->CreateColourSchemes();
                        pl_boundarypoint->SetColourScheme();
                    }
                      
                    pl_boundary->AddPoint(pl_boundary->m_pODPointList->GetFirst()->GetData());
                    pl_boundary->m_bIsBeingCreated = false;
                    pl_boundary->CreateColourSchemes();
                    pl_boundary->SetColourScheme();
                    pl_boundary->SetActiveColours();
                    
                    ODNavObjectChanges *l_ODNavObjectChanges = new ODNavObjectChanges();
                    l_ODNavObjectChanges->InsertPathA(pl_boundary);
                    
                    delete l_ODNavObjectChanges;
                    RequestRefresh(g_ocpn_draw_pi->m_parent_window);
                    
                    jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                    jMsg[wxT("Msg")] = root[wxT("Msg")];
                    jMsg[wxT("Type")] = wxT("Response");
                    jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                    jMsg[wxS("Created")] = true;
                    
                    jMsg[wxS("GUID")] = pl_boundary->m_GUID;
                    writer.Write( jMsg, MsgString );
                    SendPluginMessage( root[wxT("Source")].AsString(), MsgString );

                    pl_boundary = NULL;
                    return;
                }
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("CreateBoundaryPoint")) {
            if(!gCreateBoundaryPoint) {
                gCreateBoundaryPoint = new json_validator;
                try {
                    gCreateBoundaryPoint->set_root_schema(BoundaryPointSchema);
                } catch (const std::exception &e) {
                    DEBUGST("Validation of schema failed, here is why: ");
                    DEBUGEND(e.what());
                    wxString l_errorMsg;
                    l_errorMsg.Append("Validation of schema failed, here is why: ");
                    l_errorMsg.Append(e.what());
                    wxLogMessage( l_errorMsg );
                    bFail = true;
                }
            }
            if(!bFail) {
                try {
                    json message = json::parse(static_cast<const char*>(message_body));
                    gCreateBoundaryPoint->validate(message);
                } catch (const std::exception &e) {
                    DEBUGST("Validation failed, here is why: ");
                    DEBUGEND(e.what());
                    wxString l_errorMsg;
                    l_errorMsg.Append("Validation of schema failed, here is why: ");
                    l_errorMsg.Append(e.what());
                    wxLogMessage( l_errorMsg );
                    bFail = true;
                }
            }

            if(!bFail) {
                if(root[wxS("Type")].AsString() == _T("Request")) {
                    BoundaryPoint *pl_boundarypoint; 
                    if(root[wxS("IconName")].AsString().IsEmpty()) {
                        pl_boundarypoint = new BoundaryPoint(root[wxS("Lat")].AsDouble(), root[wxS("Lon")].AsDouble(), wxEmptyString, root[wxS("BoundayPointName")].AsString());
                    } else {
                        pl_boundarypoint = new BoundaryPoint(root[wxS("Lat")].AsDouble(), root[wxS("Lon")].AsDouble(), root[wxS("IconName")].AsString(), root[wxS("BoundayPointName")].AsString());
                    }
                    
                    if(root.HasMember(wxS("BoundaryPointType"))) {
                        if(root[wxS("BoundaryPointType")].AsString() == _T("Exclusion")) {
                            pl_boundarypoint->m_bExclusionBoundaryPoint = true;
                            pl_boundarypoint->m_bInclusionBoundaryPoint = false;
                        } else if(root[wxS("BoundaryPointType")].AsString() == _T("Inclusion")) {
                            pl_boundarypoint->m_bExclusionBoundaryPoint = false;
                            pl_boundarypoint->m_bInclusionBoundaryPoint = true;
                        } else if(root[wxS("BoundaryPointType")].AsString() == _T("Neither")) {
                            pl_boundarypoint->m_bExclusionBoundaryPoint = false;
                            pl_boundarypoint->m_bInclusionBoundaryPoint = false;
                        } else {
                            pl_boundarypoint->m_bExclusionBoundaryPoint = g_bExclusionBoundaryPoint;
                            pl_boundarypoint->m_bInclusionBoundaryPoint = g_bInclusionBoundaryPoint;
                        }
                    }
                    if(root.HasMember("visible")) pl_boundarypoint->SetVisible(root[wxS("visible")].AsBool()); 
                    if(root.HasMember("ringsvisible")) pl_boundarypoint->SetShowODPointRangeRings(root[wxS("ringsvisible")].AsBool());
                    if(root.HasMember("ringsnumber")) pl_boundarypoint->SetODPointRangeRingsNumber(root[wxS("ringsnumber")].AsInt());
                    if(root.HasMember("ringssteps")) pl_boundarypoint->SetODPointRangeRingsStep(root[wxS("ringssteps")].AsDouble());
                    if(root.HasMember("ringunits")) pl_boundarypoint->SetODPointRangeRingsStepUnits(root[wxS("ringsunits")].AsInt());
                    if(root.HasMember("ringscolour")) pl_boundarypoint->SetODPointRangeRingsColour(root[wxS("ringscolour")].AsString());
                    pl_boundarypoint->m_bIsolatedMark = true;
                    pl_boundarypoint->m_bIsInBoundary = false;
                    pl_boundarypoint->m_bIsInPath = false;
                    pl_boundarypoint->CreateColourSchemes();
                    pl_boundarypoint->SetColourScheme();
                    
                    g_pODConfig->AddNewODPoint( pl_boundarypoint, -1 );    // use auto next num
                    g_pODSelect->AddSelectableODPoint( root[wxS("Lat")].AsDouble(), root[wxS("Lon")].AsDouble(), pl_boundarypoint );
                    if( g_pPathManagerDialog && g_pPathManagerDialog->IsShown() )
                        g_pPathManagerDialog->UpdateODPointsListCtrl();
                    RequestRefresh(g_ocpn_draw_pi->m_parent_window);
                    
                    jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                    jMsg[wxT("Msg")] = root[wxT("Msg")];
                    jMsg[wxT("Type")] = wxT("Response");
                    jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                    jMsg[wxS("Created")] = true;
                    
                    jMsg[wxS("GUID")] = pl_boundarypoint->m_GUID;
                    writer.Write( jMsg, MsgString );
                    SendPluginMessage( root[wxT("Source")].AsString(), MsgString );

                    pl_boundarypoint = NULL;
                    
                    return;
                }
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("CreateTextPoint")) {
            if(!gCreateTextPoint) {
                gCreateTextPoint = new json_validator;
                try {
                    gCreateTextPoint->set_root_schema(TextPointSchema);
                } catch (const std::exception &e) {
                    DEBUGST("Validation of schema failed, here is why: ");
                    DEBUGEND(e.what());
                    wxString l_errorMsg;
                    l_errorMsg.Append("Validation of schema failed, here is why: ");
                    l_errorMsg.Append(e.what());
                    wxLogMessage( l_errorMsg );
                    bFail = true;
                }
            }
            if(!bFail) {
                try {
                    json message = json::parse(static_cast<const char*>(message_body));
                    gCreateTextPoint->validate(message);
                } catch (const std::exception &e) {
                    DEBUGST("Validation failed, here is why: ");
                    DEBUGEND(e.what());
                    wxString l_errorMsg;
                    l_errorMsg.Append("Validation of schema failed, here is why: ");
                    l_errorMsg.Append(e.what());
                    wxLogMessage( l_errorMsg );
                    bFail = true;
                }
            }
            
            if(!bFail) {
                if(root[wxS("Type")].AsString() == _T("Request")) {
                    TextPoint *pl_textpoint; 
                    if(root[wxS("IconName")].AsString().IsEmpty()) {
                        pl_textpoint = new TextPoint(root[wxS("Lat")].AsDouble(), root[wxS("Lon")].AsDouble(), wxEmptyString, root[wxS("TextPointName")].AsString());
                    } else {
                        pl_textpoint = new TextPoint(root[wxS("Lat")].AsDouble(), root[wxS("Lon")].AsDouble(), root[wxS("IconName")].AsString(), root[wxS("TextPointName")].AsString());
                    }
                    
                    if(root.HasMember("visible")) pl_textpoint->SetVisible(root[wxS("visible")].AsBool()); 
                    if(root.HasMember("TextToDisplay")) pl_textpoint->m_TextPointText = (root[wxS("TextToDisplay")].AsString()); 
                    if(root.HasMember("TextPosition")) pl_textpoint->m_iTextPosition = (root[wxS("TextPosition")].AsInt()); 
                    if(root.HasMember("TextColour")) pl_textpoint->m_colourTextColour = (root[wxS("TextColour")].AsString()); 
                    if(root.HasMember("BackgroundColour")) pl_textpoint->m_colourTextBackgroundColour = (root[wxS("BackgroundColour")].AsString()); 
                    if(root.HasMember("BackgroundTransparency")) pl_textpoint->m_iBackgroundTransparency = (root[wxS("BackgroundTransparency")].AsInt()); 
                    if(root.HasMember("ringsvisible")) pl_textpoint->SetShowODPointRangeRings(root[wxS("ringsvisible")].AsBool());
                    if(root.HasMember("ringsnumber")) pl_textpoint->SetODPointRangeRingsNumber(root[wxS("ringsnumber")].AsInt());
                    if(root.HasMember("ringssteps")) pl_textpoint->SetODPointRangeRingsStep(root[wxS("ringssteps")].AsDouble());
                    if(root.HasMember("ringunits")) pl_textpoint->SetODPointRangeRingsStepUnits(root[wxS("ringsunits")].AsInt());
                    if(root.HasMember("ringscolour")) pl_textpoint->SetODPointRangeRingsColour(root[wxS("ringscolour")].AsString());
                    pl_textpoint->m_bIsolatedMark = true;
                    pl_textpoint->m_bIsInPath = false;
                    pl_textpoint->CreateColourSchemes();
                    pl_textpoint->SetColourScheme();
                    
                    g_pODConfig->AddNewODPoint( pl_textpoint, -1 );    // use auto next num
                    g_pODSelect->AddSelectableODPoint( root[wxS("Lat")].AsDouble(), root[wxS("Lon")].AsDouble(), pl_textpoint );
                    if( g_pPathManagerDialog && g_pPathManagerDialog->IsShown() )
                        g_pPathManagerDialog->UpdateODPointsListCtrl();
                    RequestRefresh(g_ocpn_draw_pi->m_parent_window);
                    
                    jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                    jMsg[wxT("Msg")] = root[wxT("Msg")];
                    jMsg[wxT("Type")] = wxT("Response");
                    jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                    jMsg[wxS("Created")] = true;
                    
                    jMsg[wxS("GUID")] = pl_textpoint->m_GUID;
                    writer.Write( jMsg, MsgString );
                    SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                    
                    pl_textpoint = NULL;
                    
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

