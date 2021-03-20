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
#include "jsonreader.h"
#include "jsonwriter.h"

#ifdef OD_JSON_SCHEMA_VALIDATOR
#if defined(snprintf) && defined(_MSC_VER)
    #undef snprintf
#endif
#include "nlohmann/json-schema.hpp"
#include "json-patch.hpp"
#include "jsonval.h"
using nlohmann::json;
using nlohmann::json_schema::json_validator;
#endif

#include "ocpn_draw_pi.h"
#include "ODJSON.h"
#include "ODPath.h"
#include "ODConfig.h"
#ifdef OD_JSON_SCHEMA_VALIDATOR
#include "ODJSONSchemas.h"
#endif
#include "ODSelect.h"
#include "PathMan.h"
#include "PathAndPointManagerDialogImpl.h"
#include "Boundary.h"
#include "BoundaryMan.h"
#include "BoundaryPoint.h"
#include "GZ.h"
#include "GZMan.h"
#include "ODAPI.h"
#include "PointMan.h"
#include "TextPoint.h"
#include "ODPointPropertiesImpl.h"
#include "ODPathPropertiesDialogImpl.h"

#include "version.h"

#include <stdio.h>
#include <wx/base64.h>
#include <wx/mstream.h>

#ifdef OD_JSON_SCHEMA_VALIDATOR 
json_validator *gODJSONMsgValidator;
#endif

ODJSON::ODJSON()
{
    // ctor
#ifdef OD_JSON_SCHEMA_VALIDATOR     
    gODJSONMsgValidator = NULL;

    /* Join both fragments of the schema together. Needed for MSVC limitation on litteral text */
    jSchema = jSchema_defs.flatten();
    json tmp = jSchema_scheme.flatten();
    for(json::iterator it = tmp.begin(); it != tmp.end(); ++it)
    {
        jSchema[it.key()] = it.value();
    }

#endif    
}

ODJSON::~ODJSON()
{
    // dtor
#ifdef OD_JSON_SCHEMA_VALIDATOR 
    if(gODJSONMsgValidator) {
        delete gODJSONMsgValidator;
        gODJSONMsgValidator = NULL;
    }
#endif    
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
#ifdef OD_JSON_SCHEMA_VALIDATOR        
        if(!gODJSONMsgValidator) {
            gODJSONMsgValidator = new json_validator;
            try {
                DEBUGST("Input message: ");
                DEBUGEND(jSchema);
                gODJSONMsgValidator->set_root_schema(jSchema);
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
                gODJSONMsgValidator->validate(message);
            } catch (const std::exception &e) {
                DEBUGST("Validation of message against schema failed, here is why: ");
                DEBUGEND(e.what());
                wxString l_errorMsg;
                l_errorMsg.Append("Validation of message against schema failed, here is why: ");
                l_errorMsg.Append(e.what());
                wxLogMessage( l_errorMsg );
                bFail = true;
            }
        }
#endif
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
        
#ifndef OD_JSON_SCHEMA_VALIDATOR        
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
#endif
        DEBUGSL(root[wxS("Msg")].AsString());
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
            return;
            
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("GetAPIAddresses")) {
            if(root[wxS("Type")].AsString() == wxS("Request")) {
                jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                jMsg[wxT("Msg")] = root[wxT("Msg")];
                jMsg[wxT("Type")] = wxT("Response");
                jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                jMsg[wxT("Found")] = true;
                jMsg[wxS("ODAPIVersionMajor")] = ODAPI_VERSION_MAJOR;
                jMsg[wxS("ODAPIVersionMinor")] = ODAPI_VERSION_MINOR;
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
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_DeleteBoundary );
                jMsg[wxT("OD_DeleteBoundary")] = wxString::From8BitData(ptr);
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_DeleteBoundaryPoint );
                jMsg[wxT("OD_DeleteBoundaryPoint")] = wxString::From8BitData(ptr);
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_DeleteTextPoint );
                jMsg[wxT("OD_DeleteTextPoint")] = wxString::From8BitData(ptr);
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_AddPointIcon );
                jMsg[wxT("OD_AddPointIcon")] = wxString::From8BitData(ptr);
                snprintf(ptr, sizeof ptr, "%p", ODAPI::OD_DeletePointIcon );
                jMsg[wxT("OD_DeletePointIcon")] = wxString::From8BitData(ptr);
                writer.Write( jMsg, MsgString );
                SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                return;
            }
            
        } else if(!bFail && root[wxS("Msg")].AsString() == "FindPathByGUID") {
#ifndef OD_JSON_SCHEMA_VALIDATOR
            if(!root.HasMember( wxS("GUID"))) {
                wxLogMessage( wxS("No GUID found in message") );
                bFail = true;
            }
#endif
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
#ifndef OD_JSON_SCHEMA_VALIDATOR
            if(!root.HasMember( wxS("Lat")) && !root.HasMember( wxS("lat"))) {
                wxLogMessage( wxS("No Latitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("Lon")) && !root.HasMember( wxS("lon"))) {
                wxLogMessage( wxS("No Longitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("BoundaryType"))) {
                wxLogMessage( wxS("No Boundary Type found in message") );
                bFail = true;
            }
#endif
            if(!bFail) {
                if(root.HasMember( wxS("Lat")) )
                    l_dLat = root[wxS("Lat")].AsDouble();
                else
                    l_dLat = root[wxS("lat")].AsDouble();
                if(root.HasMember( wxS("Lon")) )
                    l_dLon = root[wxS("Lon")].AsDouble();
                else
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
                    jMsg[wxS("Lat")] = l_dLat;
                    jMsg[wxS("Lon")] = l_dLon;
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
#ifndef OD_JSON_SCHEMA_VALIDATOR
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
#endif
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
#ifndef OD_JSON_SCHEMA_VALIDATOR
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
#endif
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
#ifndef OD_JSON_SCHEMA_VALIDATOR
            if(!root.HasMember( wxS("GUID"))) {
                wxLogMessage( wxS("No GUID found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("Lat")) && !root.HasMember( wxS("lat"))) {
                wxLogMessage( wxS("No Latitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("Lon")) && !root.HasMember( wxS("lon"))) {
                wxLogMessage( wxS("No Longitude found in message") );
                bFail = true;
            }
#endif
            if(!bFail) {
                wxString l_sGUID = root[wxS("GUID")].AsString();
                if(root.HasMember( wxS("Lat")) )
                    l_dLat = root[wxS("Lat")].AsDouble();
                else
                    l_dLat = root[wxS("lat")].AsDouble();
                if(root.HasMember( wxS("Lon")) )
                    l_dLon = root[wxS("Lon")].AsDouble();
                else
                    l_dLon = root[wxS("lon")].AsDouble();
                
                l_sType = root[wxS("Type")].AsString();
                l_sMsg = root[wxT("Msg")].AsString();
                
                if(l_sType == wxS("Request")) {
                    Boundary *l_boundary = NULL;
                    BoundaryPoint *l_boundarypoint = NULL;
                    if(l_sMsg == wxS("FindPointInBoundary")) {
                        l_dLat = root[wxS("Lat")].AsDouble();
                        l_dLon = root[wxS("Lon")].AsDouble();
                        
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
                            jMsg[wxS("Lat")] = l_dLat;
                            jMsg[wxS("Lon")] = l_dLon;
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
                        jMsg[wxS("Lat")] = l_dLat;
                        jMsg[wxS("Lon")] = l_dLon;
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
#ifndef OD_JSON_SCHEMA_VALIDATOR
            if(!root.HasMember( wxS("GUID"))) {
                wxLogMessage( wxS("No GUID found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("Lat")) && !root.HasMember( wxS("lat"))) {
                wxLogMessage( wxS("No Latitude found in message") );
                bFail = true;
            }
            
            if(!root.HasMember( wxS("Lon")) && !root.HasMember( wxS("lon"))) {
                wxLogMessage( wxS("No Longitude found in message") );
                bFail = true;
            }
#endif
            if(!bFail) {
                wxString l_sGUID = root[wxS("GUID")].AsString();
                if(root.HasMember( wxS("Lat")) )
                    root[wxS("Lat")].AsString().ToDouble( & l_dLat );
                else
                    root[wxS("lat")].AsString().ToDouble( & l_dLat );
                if(root.HasMember( wxS("Lon")) )
                    root[wxS("Lon")].AsString().ToDouble( & l_dLon );
                else
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
                        jMsg[wxS("Lat")] = l_dLat;
                        jMsg[wxS("Lon")] = l_dLon;
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
                    jMsg[wxS("Lat")] = l_dLat;
                    jMsg[wxS("Lon")] = l_dLon;
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
                    
                    current_point[wxT("Lat")] = popSecond->m_lat;
                    current_point[wxT("Lon")] = popSecond->m_lon;
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
            wxJSONValue jv_Boundary;
            wxJSONValue jv_BoundaryPoint;
            wxJSONValue jv_BoundaryPointHyperlink;
            if(root[wxS("Type")].AsString() != _T("Request")) {
                wxLogMessage( wxS("Create Boundary Type not 'Request'") );
                bFail = true;
            }
            if(!root.HasMember( wxS("Boundary"))) {
                wxLogMessage( wxS("No Boundary in message") );
                bFail = true;
            } else {
                jv_Boundary = root[wxS("Boundary")];
            }
            if(!bFail && !jv_Boundary.HasMember( wxS("BoundaryPoints"))) {
                wxLogMessage( wxS("No Boundary Points found in message") );
                bFail = true;
            } else {
                if( jv_Boundary[wxS("BoundaryPoints")].Size() < 3) {
                    wxLogMessage( wxS("Insufficient Boundary Points provided"));
                    bFail = true;
                } else {
                    for(int i = 0; i< jv_Boundary[wxS("BoundaryPoints")].Size(); i++) {
                        jv_BoundaryPoint = jv_Boundary[wxS("BoundaryPoints")].Item(i);
                        if(!(jv_BoundaryPoint.HasMember(wxS("Lat")) || jv_BoundaryPoint.HasMember(wxS("lat"))) || !(jv_BoundaryPoint.HasMember(wxS("Lon")) || jv_BoundaryPoint.HasMember(wxS("lon"))) || !jv_BoundaryPoint.HasMember(wxS("BoundaryPointType")))
                            bFail = true;
                    }
                    if(bFail)
                        wxLogMessage( wxS("Boundary Points missing required information"));
                }
            }
            
            if(!bFail && jv_Boundary.IsValid() && jv_BoundaryPoint.IsValid()) {
                Boundary *pl_boundary = new Boundary();
                pl_boundary->m_PathNameString = jv_Boundary[wxS("BoundaryName")].AsString();
                if(jv_Boundary[wxS("BoundaryType")].AsString() == _T("Exclusion")) {
                    pl_boundary->m_bExclusionBoundary = true;
                    pl_boundary->m_bInclusionBoundary = false;
                } else if(jv_Boundary[wxS("BoundaryType")].AsString() == _T("Inclusion")) {
                    pl_boundary->m_bExclusionBoundary = false;
                    pl_boundary->m_bInclusionBoundary = true;
                } else if(jv_Boundary[wxS("BoundaryType")].AsString() == _T("Neither")) {
                    pl_boundary->m_bExclusionBoundary = false;
                    pl_boundary->m_bInclusionBoundary = false;
                }
                pl_boundary->m_bPathIsActive = true;
                if(jv_Boundary.HasMember(wxS("visible"))) pl_boundary->SetVisible(jv_Boundary[wxS("visible")].AsBool());
                if(jv_Boundary.HasMember(wxS("temporary"))) pl_boundary->m_bTemporary = jv_Boundary[wxS("visible")].AsBool();
                if(jv_Boundary.HasMember(wxS("lineColour"))) pl_boundary->m_wxcActiveLineColour = jv_Boundary[wxS("lineColour")].AsString();
                if(jv_Boundary.HasMember(wxS("fillColour"))) pl_boundary->m_wxcActiveFillColour = jv_Boundary[wxS("fillColour")].AsString();
                
                for(int i = 0; i< jv_Boundary[wxS("BoundaryPoints")].Size(); i++) {
                    jv_BoundaryPoint = jv_Boundary[wxS("BoundaryPoints")].Item(i);
                    if(jv_BoundaryPoint.HasMember( wxS("Lat")) )
                        l_dLat = jv_BoundaryPoint[wxS("Lat")].AsDouble();
                    else
                        l_dLat = jv_BoundaryPoint[wxS("lat")].AsDouble();
                    if(jv_BoundaryPoint.HasMember( wxS("Lon")) )
                        l_dLon = jv_BoundaryPoint[wxS("Lon")].AsDouble();
                    else
                        l_dLon = jv_BoundaryPoint[wxS("lon")].AsDouble();

                    BoundaryPoint *pl_boundarypoint = new BoundaryPoint(l_dLat, l_dLon, g_sODPointIconName, jv_BoundaryPoint[wxS("Name")].AsString(),wxEmptyString, false);
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
                    if(jv_BoundaryPoint.HasMember("visible")) pl_boundarypoint->SetVisible(jv_BoundaryPoint[wxS("visible")].AsBool()); 
                    if(jv_BoundaryPoint.HasMember("ringsvisible")) pl_boundarypoint->SetShowODPointRangeRings(jv_BoundaryPoint[wxS("ringsvisible")].AsBool());
                    if(jv_BoundaryPoint.HasMember("ringsnumber")) pl_boundarypoint->SetODPointRangeRingsNumber(jv_BoundaryPoint[wxS("ringsnumber")].AsInt());
                    if(jv_BoundaryPoint.HasMember("ringssteps")) pl_boundarypoint->SetODPointRangeRingsStep(jv_BoundaryPoint[wxS("ringssteps")].AsDouble());
                    if(jv_BoundaryPoint.HasMember("ringunits")) pl_boundarypoint->SetODPointRangeRingsStepUnits(jv_BoundaryPoint[wxS("ringsunits")].AsInt());
                    if(jv_BoundaryPoint.HasMember("ringscolour")) pl_boundarypoint->SetODPointRangeRingsColour(jv_BoundaryPoint[wxS("ringscolour")].AsString());
                    if(jv_BoundaryPoint.HasMember("HyperLinks")) {
                        for(int i = 0; i< jv_BoundaryPoint[wxS("HyperLinks")].Size(); i++) {
                            jv_BoundaryPointHyperlink = jv_BoundaryPoint[wxS("HyperLinks")].Item(i);
                            if(jv_BoundaryPointHyperlink.HasMember("LinkURL") && jv_BoundaryPointHyperlink.HasMember("LinkDescription"))
                                pl_boundarypoint->AddURL(jv_BoundaryPointHyperlink[wxS("LinkURL")].AsString(), jv_BoundaryPointHyperlink[wxS("LinkDescription")].AsString());
                        }
                    }
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
            } else {
                jMsg[wxS("Created")] = false;
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("CreateBoundaryPoint")) {
            wxJSONValue jv_BoundaryPoint;
            wxJSONValue jv_BoundaryPointHyperlink;
            if(root[wxS("Type")].AsString() != _T("Request")) {
                wxLogMessage( wxS("Create Boundary Point Type not 'Request'") );
                bFail = true;
            }
            if(!root.HasMember( wxS("BoundaryPoint"))) {
                wxLogMessage( wxS("No Boundary Point found in message") );
                bFail = true;
            } else {
                jv_BoundaryPoint = root[wxS("BoundaryPoint")];
            }
            if(!(jv_BoundaryPoint.HasMember(wxS("Lat")) || jv_BoundaryPoint.HasMember(wxS("lat"))) || !(jv_BoundaryPoint.HasMember(wxS("Lon")) ||jv_BoundaryPoint.HasMember(wxS("lon"))) || !jv_BoundaryPoint.HasMember(wxS("BoundaryPointType"))) {
                wxLogMessage( wxS("Boundary Point missing required information") );
                bFail = true;
            }
            
            if(!bFail) {
                BoundaryPoint *pl_boundarypoint; 
                wxString l_name = wxEmptyString;
                if(jv_BoundaryPoint.HasMember("BoudaryPointName")) {
                    if(jv_BoundaryPoint[wxS("BoundayPointName")].AsString().length() > 0)
                        l_name = jv_BoundaryPoint[wxS("BoundayPointName")].AsString();
                }

                if(jv_BoundaryPoint.HasMember( wxS("Lat")) )
                    l_dLat = jv_BoundaryPoint[wxS("Lat")].AsDouble();
                else
                    l_dLat = jv_BoundaryPoint[wxS("lat")].AsDouble();
                if(jv_BoundaryPoint.HasMember( wxS("Lon")) )
                    l_dLon = jv_BoundaryPoint[wxS("Lon")].AsDouble();
                else
                    l_dLon = jv_BoundaryPoint[wxS("lon")].AsDouble();

                if(jv_BoundaryPoint[wxS("IconName")].AsString().IsEmpty()) {
                    pl_boundarypoint = new BoundaryPoint(l_dLat, l_dLon, wxEmptyString, l_name);
                } else {
                    pl_boundarypoint = new BoundaryPoint(l_dLat, l_dLon, jv_BoundaryPoint[wxS("IconName")].AsString(), l_name);
                }
                
                if(jv_BoundaryPoint.HasMember(wxS("BoundaryPointType"))) {
                    if(jv_BoundaryPoint[wxS("BoundaryPointType")].AsString() == _T("Exclusion")) {
                        pl_boundarypoint->m_bExclusionBoundaryPoint = true;
                        pl_boundarypoint->m_bInclusionBoundaryPoint = false;
                    } else if(jv_BoundaryPoint[wxS("BoundaryPointType")].AsString() == _T("Inclusion")) {
                        pl_boundarypoint->m_bExclusionBoundaryPoint = false;
                        pl_boundarypoint->m_bInclusionBoundaryPoint = true;
                    } else if(jv_BoundaryPoint[wxS("BoundaryPointType")].AsString() == _T("Neither")) {
                        pl_boundarypoint->m_bExclusionBoundaryPoint = false;
                        pl_boundarypoint->m_bInclusionBoundaryPoint = false;
                    } else {
                        pl_boundarypoint->m_bExclusionBoundaryPoint = g_bExclusionBoundaryPoint;
                        pl_boundarypoint->m_bInclusionBoundaryPoint = g_bInclusionBoundaryPoint;
                    }
                }
                if(jv_BoundaryPoint.HasMember("visible")) pl_boundarypoint->SetVisible(jv_BoundaryPoint[wxS("visible")].AsBool()); 
                if(jv_BoundaryPoint.HasMember("ringsvisible")) pl_boundarypoint->SetShowODPointRangeRings(jv_BoundaryPoint[wxS("ringsvisible")].AsBool());
                if(jv_BoundaryPoint.HasMember("ringsnumber")) pl_boundarypoint->SetODPointRangeRingsNumber(jv_BoundaryPoint[wxS("ringsnumber")].AsInt());
                if(jv_BoundaryPoint.HasMember("ringssteps")) pl_boundarypoint->SetODPointRangeRingsStep(jv_BoundaryPoint[wxS("ringssteps")].AsDouble());
                if(jv_BoundaryPoint.HasMember("ringunits")) pl_boundarypoint->SetODPointRangeRingsStepUnits(jv_BoundaryPoint[wxS("ringsunits")].AsInt());
                if(jv_BoundaryPoint.HasMember("ringscolour")) pl_boundarypoint->SetODPointRangeRingsColour(jv_BoundaryPoint[wxS("ringscolour")].AsString());
                if(jv_BoundaryPoint.HasMember("HyperLinks")) {
                    for(int i = 0; i< jv_BoundaryPoint[wxS("HyperLinks")].Size(); i++) {
                        jv_BoundaryPointHyperlink = jv_BoundaryPoint[wxS("HyperLinks")].Item(i);
                        if(jv_BoundaryPointHyperlink.HasMember("LinkURL") && jv_BoundaryPointHyperlink.HasMember("LinkDescription"))
                            pl_boundarypoint->AddURL(jv_BoundaryPointHyperlink[wxS("LinkURL")].AsString(), jv_BoundaryPointHyperlink[wxS("LinkDescription")].AsString());
                    }
                }
                pl_boundarypoint->m_bIsolatedMark = true;
                pl_boundarypoint->m_bIsInBoundary = false;
                pl_boundarypoint->m_bIsInPath = false;
                pl_boundarypoint->CreateColourSchemes();
                pl_boundarypoint->SetColourScheme();
                
                g_pODConfig->AddNewODPoint( pl_boundarypoint, -1 );    // use auto next num
                g_pODSelect->AddSelectableODPoint( jv_BoundaryPoint[wxS("Lat")].AsDouble(), jv_BoundaryPoint[wxS("Lon")].AsDouble(), pl_boundarypoint );
                if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
                    g_pPathAndPointManagerDialog->UpdateODPointsListCtrl();
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
            } else {
                jMsg[wxS("Created")] = false;
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("CreateTextPoint")) {
            wxJSONValue jv_TextPoint;
            wxJSONValue jv_TextPointHyperlink;
            if(root[wxS("Type")].AsString() != _T("Request")) {
                wxLogMessage( wxS("Create Text Point Type not 'Request'") );
                jMsg[wxS("Created")] = false;
                bFail = true;
            }
            if(!root.HasMember( wxS("TextPoint"))) {
                wxLogMessage( wxS("No Text Point found in message") );
                    jMsg[wxS("Created")] = false;
                bFail = true;
            } else {
                jv_TextPoint = root[wxS("TextPoint")];
            }
            
            if(!bFail) {
                TextPoint *pl_textpoint; 
                if(jv_TextPoint.HasMember( wxS("Lat")) )
                    l_dLat = jv_TextPoint[wxS("Lat")].AsDouble();
                else
                    l_dLat = jv_TextPoint[wxS("lat")].AsDouble();
                if(jv_TextPoint.HasMember( wxS("Lon")) )
                    l_dLon = jv_TextPoint[wxS("Lon")].AsDouble();
                else
                    l_dLon = jv_TextPoint[wxS("lon")].AsDouble();

                if(jv_TextPoint[wxS("IconName")].AsString().IsEmpty()) {
                    pl_textpoint = new TextPoint(l_dLat, l_dLon, wxEmptyString, jv_TextPoint[wxS("TextPointName")].AsString());
                } else {
                    pl_textpoint = new TextPoint(l_dLat, l_dLat, jv_TextPoint[wxS("IconName")].AsString(), jv_TextPoint[wxS("TextPointName")].AsString());
                }
                
                if(jv_TextPoint.HasMember("visible")) pl_textpoint->SetVisible(jv_TextPoint[wxS("visible")].AsBool()); 
                if(jv_TextPoint.HasMember("temporary")) pl_textpoint->m_bTemporary = jv_TextPoint[wxS("temporary")].AsBool(); 
                if(jv_TextPoint.HasMember("GUID")) pl_textpoint->m_GUID = jv_TextPoint[wxS("GUID")].AsString(); 
                else jv_TextPoint[wxS("GUID")] = wxEmptyString;
                if(jv_TextPoint.HasMember("TextToDisplay")) pl_textpoint->m_TextPointText = (jv_TextPoint[wxS("TextToDisplay")].AsString()); 
                if(jv_TextPoint.HasMember("TextPosition")) pl_textpoint->m_iTextPosition = (jv_TextPoint[wxS("TextPosition")].AsInt()); 
                if(jv_TextPoint.HasMember("TextColour")) pl_textpoint->m_colourTextColour = (jv_TextPoint[wxS("TextColour")].AsString()); 
                if(jv_TextPoint.HasMember("BackgroundColour")) pl_textpoint->m_colourTextBackgroundColour = (jv_TextPoint[wxS("BackgroundColour")].AsString()); 
                if(jv_TextPoint.HasMember("BackgroundTransparency")) pl_textpoint->m_iBackgroundTransparency = (jv_TextPoint[wxS("BackgroundTransparency")].AsInt());
                if(jv_TextPoint.HasMember("defaultRangeRings")) {
                    if(!jv_TextPoint[wxT("defaultRangeRings")].AsBool()) {
                        if(jv_TextPoint.HasMember("ringsvisible")) pl_textpoint->SetShowODPointRangeRings(jv_TextPoint[wxS("ringsvisible")].AsBool());
                        if(jv_TextPoint.HasMember("ringsnumber")) pl_textpoint->SetODPointRangeRingsNumber(jv_TextPoint[wxS("ringsnumber")].AsInt());
                        if(jv_TextPoint.HasMember("ringssteps")) pl_textpoint->SetODPointRangeRingsStep(jv_TextPoint[wxS("ringssteps")].AsDouble());
                        if(jv_TextPoint.HasMember("ringunits")) pl_textpoint->SetODPointRangeRingsStepUnits(jv_TextPoint[wxS("ringsunits")].AsInt());
                        if(jv_TextPoint.HasMember("ringscolour")) pl_textpoint->SetODPointRangeRingsColour(jv_TextPoint[wxS("ringscolour")].AsString());
                    }
                } else {
                    if(jv_TextPoint.HasMember("ringsvisible")) pl_textpoint->SetShowODPointRangeRings(jv_TextPoint[wxS("ringsvisible")].AsBool());
                    if(jv_TextPoint.HasMember("ringsnumber")) pl_textpoint->SetODPointRangeRingsNumber(jv_TextPoint[wxS("ringsnumber")].AsInt());
                    if(jv_TextPoint.HasMember("ringssteps")) pl_textpoint->SetODPointRangeRingsStep(jv_TextPoint[wxS("ringssteps")].AsDouble());
                    if(jv_TextPoint.HasMember("ringunits")) pl_textpoint->SetODPointRangeRingsStepUnits(jv_TextPoint[wxS("ringsunits")].AsInt());
                    if(jv_TextPoint.HasMember("ringscolour")) pl_textpoint->SetODPointRangeRingsColour(jv_TextPoint[wxS("ringscolour")].AsString());
                }
                if(jv_TextPoint.HasMember("HyperLinks")) {
                    for(int i = 0; i< jv_TextPoint[wxS("HyperLinks")].Size(); i++) {
                        jv_TextPointHyperlink = jv_TextPoint[wxS("HyperLinks")].Item(i);
                        if(jv_TextPointHyperlink.HasMember("LinkURL") && jv_TextPointHyperlink.HasMember("LinkDescription"))
                            pl_textpoint->AddURL(jv_TextPointHyperlink[wxS("LinkURL")].AsString(), jv_TextPointHyperlink[wxS("LinkDescription")].AsString());
                    }
                }
                pl_textpoint->m_bIsolatedMark = true;
                pl_textpoint->m_bIsInPath = false;
                pl_textpoint->CreateColourSchemes();
                pl_textpoint->SetColourScheme();
                
                g_pODConfig->AddNewODPoint( pl_textpoint, -1 );    // use auto next num
                g_pODSelect->AddSelectableODPoint( jv_TextPoint[wxS("Lat")].AsDouble(), jv_TextPoint[wxS("Lon")].AsDouble(), pl_textpoint );
                if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
                    g_pPathAndPointManagerDialog->UpdateODPointsListCtrl();
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
            } else {
                jMsg[wxS("Created")] = false;
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("DeleteBoundary")) {
            wxJSONValue jv_Boundary;
            if(root[wxS("Type")].AsString() != _T("Request")) {
                wxLogMessage( wxS("Delete Text Point Type not 'Request'") );
                bFail = true;
            }
            if(!root.HasMember( wxS("GUID"))) {
                wxLogMessage( wxS("No GUID for the Text Point found in message") );
                bFail = true;
            } 
            
            if(!bFail) {
                Boundary *plB = (Boundary *)g_pBoundaryMan->FindPathByGUID(root[wxS("GUID")].AsString());
                if(plB) {
                    g_pBoundaryMan->DeletePath(plB);
                } 
                
                RequestRefresh(g_ocpn_draw_pi->m_parent_window);
                
                jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                jMsg[wxT("Msg")] = root[wxT("Msg")];
                jMsg[wxT("Type")] = wxT("Response");
                jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                jMsg[wxS("Deleted")] = true;
                
                jMsg[wxS("GUID")] = root[wxS("GUID")].AsString();
                writer.Write( jMsg, MsgString );
                SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                
                plB = NULL;
                
                return;
                
            } else {
                jMsg[wxS("Deleted")] = false;
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("DeleteBoundaryPoint")) {
            if(root[wxS("Type")].AsString() != _T("Request")) {
                wxLogMessage( wxS("Delete Text Point Type not 'Request'") );
                bFail = true;
            }
            if(!root.HasMember( wxS("GUID"))) {
                wxLogMessage( wxS("No GUID for the Text Point found in message") );
                bFail = true;
            }
            
            if(!bFail) {
                BoundaryPoint *plBP = (BoundaryPoint *)g_pODPointMan->FindODPointByGUID(root[wxS("GUID")].AsString());
                if(plBP) {
                    if(plBP->m_bIsInBoundary) {
                        wxArrayPtrVoid *ppath_array = g_pPathMan->GetPathArrayContaining(plBP);
                        if( ppath_array ) {
                            for( unsigned int ip = 0; ip < ppath_array->GetCount(); ip++ ) {
                                ODPath *pp = (ODPath *) ppath_array->Item( ip );
                                if(pp->GetnPoints() <= 3)
                                    g_pPathMan->DeletePath(pp);
                                else {
                                    pp->DeletePoint( plBP );
                                    if( g_pODPathPropDialog && g_pODPathPropDialog->IsShown() ) g_pODPathPropDialog->SetPathAndUpdate( pp, true );
                                }
                            }
                        }
                    } else {
                        g_pODPointMan->DestroyODPoint(plBP, false);
                        
                        g_pODSelect->DeleteSelectablePoint( plBP, SELTYPE_ODPOINT );
                        
                        if( NULL != g_pODPointMan )
                            g_pODPointMan->RemoveODPoint( plBP );
                        if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
                            g_pPathAndPointManagerDialog->UpdateODPointsListCtrl();
                        
                        if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
                            g_pODPointPropDialog->ValidateMark();
                        }
                        
                        plBP->m_bPtIsSelected = false;
                        
                        delete (BoundaryPoint *)plBP;
                        
                    }
                }
                RequestRefresh(g_ocpn_draw_pi->m_parent_window);
                
                jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                jMsg[wxT("Msg")] = root[wxT("Msg")];
                jMsg[wxT("Type")] = wxT("Response");
                jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                jMsg[wxS("Deleted")] = true;
                
                jMsg[wxS("GUID")] = root[wxT("GUID")].AsString();
                writer.Write( jMsg, MsgString );
                SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                
                plBP = NULL;
                
                return;
                
            } else {
                jMsg[wxS("Deleted")] = false;
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("DeleteTextPoint")) {
            if(root[wxS("Type")].AsString() != _T("Request")) {
                wxLogMessage( wxS("Delete Text Point Type not 'Request'") );
                bFail = true;
            }
            if(!root.HasMember( wxS("GUID"))) {
                wxLogMessage( wxS("No GUID for the Text Point found in message") );
                bFail = true;
            } 
            
            if(!bFail) {
                TextPoint *pl_textpoint = (TextPoint *)g_pODPointMan->FindODPointByGUID(root[wxS("GUID")].AsString());
                
                if(pl_textpoint) {
                    g_pODPointMan->DestroyODPoint(pl_textpoint);
                    
                    g_pODSelect->DeleteSelectablePoint( pl_textpoint, SELTYPE_ODPOINT );
                    g_pODConfig->DeleteODPoint( pl_textpoint );
                    if( NULL != g_pODPointMan )
                        g_pODPointMan->RemoveODPoint( pl_textpoint );
                    if( g_pPathAndPointManagerDialog && g_pPathAndPointManagerDialog->IsShown() )
                        g_pPathAndPointManagerDialog->UpdateODPointsListCtrl();
                    
                    if( g_pODPointPropDialog && g_pODPointPropDialog->IsShown() ) {
                        g_pODPointPropDialog->ValidateMark();
                    }
                    
                    pl_textpoint->m_bPtIsSelected = false;
                    
                    delete (TextPoint *)pl_textpoint;
                }
                
                RequestRefresh(g_ocpn_draw_pi->m_parent_window);
                
                jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                jMsg[wxT("Msg")] = root[wxT("Msg")];
                jMsg[wxT("Type")] = wxT("Response");
                jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                jMsg[wxS("Deleted")] = true;
                
                jMsg[wxS("GUID")] = root[wxS("GUID")].AsString();
                writer.Write( jMsg, MsgString );
                SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                
                pl_textpoint = NULL;
                
                return;
            } else {
                jMsg[wxS("Deleted")] = false;
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("AddPointIcon")) {
            wxJSONValue jv_AddPointIcon;
            if(root[wxS("Type")].AsString() != _T("Request")) {
                wxLogMessage( wxS("Add Point Icon Type not 'Request'") );
                bFail = true;
            }
            if(!root.HasMember( wxS("AddPointIcon"))) {
                wxLogMessage( wxS("No AddPointIcon not found in message") );
                bFail = true;
            } else {
                jv_AddPointIcon = root[wxS("AddPointIcon")];
            }
            if(!jv_AddPointIcon.HasMember(wxS("PointIcon")) || !jv_AddPointIcon.HasMember(wxS("PointIconName")) || !jv_AddPointIcon.HasMember(wxS("PointIconDescription"))) {
                wxLogMessage( wxS("Add Point Icon missing required information") );
                bFail = true;
            }
            
            if(!bFail) {
                wxMemoryBuffer l_MB = wxBase64Decode(jv_AddPointIcon[wxT("PointIcon")].AsString());
                wxMemoryInputStream l_MIS(l_MB, l_MB.GetDataLen());
                wxBitmap *l_PointIcon = new wxBitmap(wxImage(l_MIS));
                
                g_pODPointMan->ProcessIcon(*l_PointIcon, jv_AddPointIcon[wxT("PointIconName")].AsString(), jv_AddPointIcon[wxT("PointIconDescription")].AsString());
                
                jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                jMsg[wxT("Msg")] = root[wxT("Msg")];
                jMsg[wxT("Type")] = wxT("Response");
                jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                jMsg[wxS("Added")] = true;
                
                writer.Write( jMsg, MsgString );
                SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                
                return;
            } else {
                jMsg[wxS("Added")] = false;
            }
        } else if(!bFail && root[wxS("Msg")].AsString() == wxS("DeletePointIcon")) {
            wxJSONValue jv_DeletePointIcon;
            if(root[wxS("Type")].AsString() != _T("Request")) {
                wxLogMessage( wxS("Delete Point Icon Type not 'Request'") );
                bFail = true;
            }
            if(!root.HasMember( wxS("DeletePointIcon"))) {
                wxLogMessage( wxS("No DeletePointIcon found in message") );
                bFail = true;
            } else {
                jv_DeletePointIcon = root[wxS("DeletePointIcon")];
            }
            if(!jv_DeletePointIcon.HasMember(wxS("PointIconName"))) {
                wxLogMessage( wxS("Delete Point Icon missing required information") );
                bFail = true;
            }
            
            if(!bFail) {
                
                g_pODPointMan->RemoveIcon(jv_DeletePointIcon[wxT("PointIconName")].AsString());
                
                jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
                jMsg[wxT("Msg")] = root[wxT("Msg")];
                jMsg[wxT("Type")] = wxT("Response");
                jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
                jMsg[wxS("Deleted")] = true;
                
                writer.Write( jMsg, MsgString );
                SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
                
                return;
            } else {
                jMsg[wxS("Deleted")] = false;
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

    if(bFail) {
        jMsg[wxT("Source")] = wxT("OCPN_DRAW_PI");
        jMsg[wxT("Msg")] = root[wxT("Msg")];
        jMsg[wxT("Type")] = wxT("Response");
        jMsg[wxT("MsgId")] = root[wxT("MsgId")].AsString();
        
        writer.Write( jMsg, MsgString );
        SendPluginMessage( root[wxT("Source")].AsString(), MsgString );
        
    }
    return;
}

