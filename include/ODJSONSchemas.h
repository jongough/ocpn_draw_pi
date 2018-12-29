/******************************************************************************
 * $Id: ocpn_draw_pi.h,v 1.0 2015/01/28 01:54:37 jongough Exp $
 *
 * Project:  OpenCPN
 * Purpose:  OpenCPN General Drawing Plugin
 * Author:   Jon Gough
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *   $EMAIL$   *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

/* The header file contains the schema for the JSON messages that are being used. However it relies on Draf7 of the JSON schema 
 * which is not fully implemented yet in a C++ library. The code is defined out that will use this as the implementation changes the 
 * ABI code of the executable and is then not compatible with OpenCPN. There is basic checking for schema compliance but 
 * it is not robust and it is possible to crash OpenCPN if the values are not valid.
 * 
 */
static json jSchema = R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "OD Schemas",
    "definitions": {
        "schemahead": {
            "description": "Standalone for simple queries or required for complex internactions as a part of the message",
            "type": "object",
            "properties": {
                "Msg": {
                    "description": "Message Name",
                    "type": "string",
                    "enum": ["Version", "GetAPIAddresses", "CreateBoundaryPoint", "CreateBoundary", "CreateTextPoint"]
                },
                "Type": {
                    "description": "Type of Message",
                    "type": "string",
                    "enum": ["Request","Response"]
                },
                "Source": {
                    "description": "Source of message",
                    "type": "string"
                },
                "MsgId": {
                    "description": "Message Name",
                    "type": "string"
                }
            },
            "required": ["Msg", "Type", "Source", "MsgId"],
            "addtionalProperties": false
        },
        "propertieshead": {
            "propertyNames": { "enum": ["Msg","Type","Source","MsgId"] }
        },
        "boundarypoint": {
            "description": "Used to create a boundary point",
            "type": "object",
            "properties": {
                "Name": {
                    "description": "Boundary Point Name",
                    "type": "string"
                },
                "Lat": {
                    "description": "Latitude of Boundary Point",
                    "type": "number",
                    "minimum": -90,
                    "maximum": 90
                },
                "Lon": {
                    "description": "Longitude of Boundary Point",
                    "type": "number",
                    "minimum": -180,
                    "maximum": 180
                },
                "IconName": {
                    "description": "Boundary Point Icon Name",
                    "type": "string"
                },
                "BoundaryPointType": {
                    "description": "Boundary Point Type",
                    "type": "string",
                    "enum": ["Exclusion","Inclusion","Neither"]
                },
                "Visible": {
                    "description": "Visibility of Boundary Point",
                    "type": "boolean",
                    "enum": [true,false]
                },
                "RingsVisible": {
                    "description": "Visibility of Boundary Point Range Rings",
                    "type": "boolean",
                    "enum": [true,false]
                },
                "RingsNumber": {
                    "description": "Number of Boundary Point Rings",
                    "type": "number",
                    "minimum": 0,
                    "maximum": 10
                },
                "RingsSteps": {
                    "description": "Steps between Boundary Point Rings",
                    "type": "number",
                    "minimum": 0.0
                },
                "RingUnits": {
                    "description": "Boundary Point Range Ring Step Units",
                    "type": "number",
                    "enum": [0,1,2,3,4,5,6,7,8,9,10]
                },
                "RingsColour": {
                    "description": "Boundary Point Range Rings Colour",
                    "type": "string"
                },
                "HyperLinks": {
                    "description": "Array of hyperlinks to local or internet documents",
                    "type": array,
                    "Items": [
                        {
                            "LinkDescription": {
                                "description": "Text description of hyper link",
                                "type": "string"
                            },
                            "LinkURL": {
                                "description": "Actual Hyperlink",
                                "type": "string"
                            }
                        },
                        "required": [
                            "LinkDescription",
                            "LinkURL"
                        ]
                    ]
                }
                
            },
            "required": [
                "Lat",
                "Lon",
                "BoundaryPointType"
            ]
        },
        "boundarypointproperties": {
            "propertyNames": {
                    "enum": ["Name","Lat","Lon","IconName","BoundaryPointType","Visible","RingsVisible","RingsNumber","RingsSteps","RingsUnits","RingsColour"]
            }
        },
        "createtextpoint": {
            "description": "Used to create a text point",
            "type": "object",
            "properties": {
                "Name": {
                    "description": "Text Point Name",
                    "type": "string"
                },
                "Lat": {
                    "description": "Latitude of Text Point",
                    "type": "number",
                    "minimum": -90,
                    "maximum": 90
                },
                "Lon": {
                    "description": "Longitude of Text Point",
                    "type": "number",
                    "minimum": -180,
                    "maximum": 180
                },
                "IconName": {
                    "description": "Text Point Icon Name",
                    "type": "string"
                },
                "Visible": {
                    "description": "Visibility of Text Point",
                    "type": "boolean",
                    "enum": [true,false]
                },
                "Temporary": {
                "description": "Text point life, false - exists over restart, true - non persistent",
                "type": "boolean",
                "enum": [true,false]
                },
                "GUID": {
                    "description": "GUID of Text Point",
                    "type": "string",
                    "optional": true
                },
                "TextToDisplay": {
                    "description": "Text Point Text",
                    "type": "string"
                },
                "TextPosition": {
                    "description": "Text Point Text Position",
                    "type": "number",
                    "enum": [0,1,2,3,4,5,6]
                },
                "TextColour": {
                    "description": "Text Point Text Colour",
                    "type": "string"
                },
                "BackgroundColour": {
                    "description": "Text Point Text BackgroundColour",
                    "type": "string"
                },
                "BackgroundTransparancy": {
                    "description": "Text Point Text BackgroundTransparancy",
                    "type": "number",
                    "minimum": 0,
                    "maximum": 255
                },
                "TextFont": {
                    "description": "Text Point Font",
                    "type": "string"
                },
                "TextPointDisplayTextWhen": {
                    "description": "Text Point When To Display",
                    "type": "string"
                },
                "RingsVisible": {
                    "description": "Visibility of TextPoint Range Rings",
                    "type": "boolean",
                    "enum": [true,false]
                },
                "RingsNumber": {
                    "description": "Number of Text Point Rings",
                    "type": "number",
                    "minimum": 0,
                    "maximum": 10
                },
                "RingsSteps": {
                    "description": "Steps between Text Point Rings",
                    "type": "number",
                    "minimum": 0.0
                },
                "RingUnits": {
                    "description": "Text Point Range Ring Step Units",
                    "type": "number",
                    "enum": [0,1,2,3,4,5,6,7,8,9,10]
                },
                "RingsColour": {
                    "description": "Text Point Range Rings Colour",
                    "type": "string"
                },
                "HyperLinks": {
                    "description": "Array of hyperlinks to local or internet documents",
                    "type": array,
                    "Items": [
                        {
                            "LinkDescription": {
                                "description": "Text description of hyper link",
                                "type": "string"
                            },
                            "LinkURL": {
                                "description": "Actual Hyperlink",
                                "type": "string"
                            }
                        },
                        "required": [
                            "LinkDescription",
                            "LinkURL"
                        ]
                    ]
                }
            },
            "required": [
                "Lat",
                "Lon"
            ]
        },
        "createtextpointproperties": {
            "propertyNames": {
            "enum":["Name","Lat","Lon","IconName","TextToDisplay", "TextPosition", "TextColour", "BackgroundColour", "BackgroundTransparancy", "TextFont", "TextPointDisplayTextWhen", "Visible","RingsVisible","RingsNumber","RingsSteps","RingsUnits","RingsColour"]
            }
        },
        "deletetextpoint": {
            "description": "Used to delete a text point",
            "type": "object",
            "properties": {
                "GUID": {
                "description": "GUID of Text Point",
                "type": "string",
                "optional": false
                }
            },
            "required": [
            "GUID"
            ]
        },
        "deletetextpointproperties": {
            "propertyNames": {
            "enum":["GUID"]
            }
        },
        "AddPointIcon": {
            "description": "Used to delete add an point icon",
            "type": "object",
            "properties": {
                "PointIcon": {
                    "description": "Point Icon in Base64",
                    "type": "string",
                    "optional": false
                },
                "PointIconName": {
                    "description": "Point Icon name",
                    "type": "string",
                    "optional": false
                },
                "PointIconDescription": {
                    "description": "Point Icon Description",
                    "type": "string",
                    "optional": false
                }
            },
            "required": [
                "PointIcon",
                "PointIconName",
                "PointIconDescription"
            ]
        },
        "AddPointIconproperties": {
            "propertyNames": {
            "enum":["PointIcon","PointIconName","PointIconDescription"]
            }
        },
        "boundary": {
            "description": "Used to create a boundary",
            "type": "object",
            "properties": {
                "BoundaryName": {
                    "description": "Boundary Name",
                    "type": "string"
                },
                "BoundaryType": {
                    "description": "Boundary Type",
                    "type": "string",
                    "enum": ["Exclusion","Inclusion","Neither"]
                },
                "Active": {
                    "description": "Is Boundary Active",
                    "type": "boolean",
                    "enum": [true,false]
                },
                "Visible": {
                    "description": "Visibility of Boundary",
                    "type": "boolean",
                    "enum": [true,false]
                },
                "Temporary": {
                "description": "Boundary life, false - exists over restart, true - non persistent",
                "type": "boolean",
                "enum": [true,false]
                },
                "boundarypoints": {
                    "description": "Array of boundary points needed to define a boundary",
                    "type": "array",
                    "items": {
                        "$ref": "#/definitions/boundarypoint"
                    },
                    "minItems": 3,
                    "additionalItems": false
                }
            },
            "required": [
                "boundarypoints"
            ]
        },
        "boundaryproperties": {
            "propertyNames": {
                    "enum": ["BoundaryName","BoundaryType","Active","Visible","boundarypoints"]
            }
        }
    },
    "type": "object",
    "oneOf": [
        {
            "allOf": [
                {"$ref": "#/definitions/schemahead"}
            ],
            "anyOf": [
                {"$ref": "#/definitions/propertieshead"}
            ]
        },
        {
            "allOf": [
                {"$ref": "#/definitions/schemahead"},
                {
                    "BoundaryPoint": {
                        "type": "object",
                        "properties": {
                            "$ref": "#/definitions/boundarypoint"
                        }
                    }
                },
                {"$ref": "#/definitions/propertieshead"},
                {"$ref": "#/definitions/boundarypointproperties"}
            ]
        },
        {
            "allOf": [
                {"$ref": "#/definitions/schemahead"},
                {
                    "TextPoint": {
                        "type": "object",
                        "properties": {
                            "$ref": "#/definitions/textpoint"
                        }
                    }
                },
                {"$ref": "#/definitions/propertieshead"},
                {"$ref": "#/definitions/textpointproperties"}
            ]
        },
        {
            "allOf": [
                {"$ref": "#/definitions/schemahead"},
                {"$ref": "#/definitions/boundary"},
                {"$ref": "#/definitions/propertieshead"},
                {"$ref": "#/definitions/boundaryproperties"},
                {"$ref": "#/definitions/boundarypointproperties"}
            ]
        }
    ]
}
    )"_json;

static std::string sSchemaHead = R"(
{
    "title": "Schema Head",
    "properties": {
        "Msg": {
            "description": "Message Name",
            "type": "string",
            "enum": ["CreateBoundaryPoint"],
            "optional": false
        },
        "Type": {
            "description": "Type of Message",
            "type": "string",
            "const": ["Request"],
            "optional": false
        },
        "Source": {
            "description": "Source of message",
            "type": "string",
            "optional": false
        },
        "MsgID": {
            "description": "Message Name",
            "type": "string",
            "optional": false
        }
    },
    "required": [
        "Msg",
        "Type",
        "Source",
        "MsgId"
    ]
}
)";

static std::string sbpSchemaBody = R"(
{
    "title": "Boundary Point Schema Body",
    "BoundaryPoint": {
        "type": "object,"
        "properties": {
            "Name": {
                "description": "Boundary Point Name",
                "type": "string"
            },
            "Lat": {
                "description": "Latitude of Boundary Point",
                "type": "number",
                "minimum": -90,
                "maximum": 90,
                "optional": false
            },
            "Lon": {
                "description": "Longitude of Boundary Point",
                "type": "number",
                "minimum": -180,
                "maximum": 180,
                "optional": false
            },
            "IconName": {
                "description": "Boundary Point Icon Name",
                "type": "string"
            },
            "BoundaryPointType": {
                "description": "Boundary Point Type",
                "type": "string",
                "enum": ["Exclusion","Inclusion","Neither"]
            },
            "Visible": {
                "description": "Visibility of Boundary Point",
                "type": "string",
                "enum": ["true","false"]
            },
            "RingsVisible": {
                "description": "Visibility of Boundary Point Range Rings",
                "type": "string",
                "enum": ["true","false"]
            },
            "RingsNumber": {
                "description": "Number of Boundary Point Rings",
                "type": "number",
                "minimun": 0,
                "maximum": 10
            },
            "RingsSteps": {
                "description": "Steps between Boundary Point Rings",
                "type": "number",
                "minimun": 0.0
            },
            "RingUnits": {
                "description": "Boundary Point Range Ring Step Units",
                "type": "number",
                "enum": [0,1]
            },
            "RingsColour": {
                "description": "Boundary Point Range Rings Colour",
                "type": "string"
            },
            "HyperLinks": {
                "description": "Array of hyperlinks to local or internet documents",
                "type": array,
                "Items": [
                    {
                        "LinkDescription": {
                        "description": "Text description of hyper link",
                        "type": "string"
                        },
                        "LinkURL": {
                        "description": "Actual Hyperlink",
                        "type": "string"
                        }
                    },
                    "required": [
                    "LinkDescription",
                    "LinkURL"
                    ]
                ]
            }
            
        },
    }
    "required": [
        "Lat",
        "Lon",
        "BoundaryPointType"
    ]
    }
    )";


static json BoundaryPointSchema = R"(
    {
        "$schema": "http://json-schema.org/draft-04/schema#",
        "title": "Create Boundary Point",
        "properties": {
            "Msg": {
                "description": "Message Name",
                "type": "string",
                "enum": ["CreateBoundaryPoint"],
                "optional": false
            },
            "Type": {
                "description": "Type of Message",
                "type": "string",
                "const": ["Request"],
                "optional": false
            },
            "Source": {
                "description": "Source of message",
                "type": "string",
                "optional": false
            },
            "MsgID": {
                "description": "Message Name",
                "type": "string",
                "optional": false
            },
            "Name": {
                "description": "Boundary Point Name",
                "type": "string"
            },
            "Lat": {
                "description": "Latitude of Boundary Point",
                "type": "number",
                "minimum": -90,
                "maximum": 90,
                "optional": false
            },
            "Lon": {
                "description": "Longitude of Boundary Point",
                "type": "number",
                "minimum": -180,
                "maximum": 180,
                "optional": false
            },
            "IconName": {
                "description": "Boundary Point Icon Name",
                "type": "string"
            },
            "BoundaryPointType": {
                "description": "Boundary Point Type",
                "type": "string",
                "enum": ["Exclusion","Inclusion","Neither"]
            },
            "Visible": {
                "description": "Visibility of Boundary Point",
                "type": "string",
                "enum": ["true","false"]
            },
            "RingsVisible": {
                "description": "Visibility of Boundary Point Range Rings",
                "type": "string",
                "enum": ["true","false"]
            },
            "RingsNumber": {
                "description": "Number of Boundary Point Rings",
                "type": "number",
                "minimun": 0,
                "maximum": 10
            },
            "RingsSteps": {
                "description": "Steps between Boundary Point Rings",
                "type": "number",
                "minimun": 0.0
            },
            "RingUnits": {
                "description": "Boundary Point Range Ring Step Units",
                "type": "number",
                "enum": [0,1]
            },
            "RingsColour": {
                "description": "Boundary Point Range Rings Colour",
                "type": "string"
            },
            "HyperLinks": {
                "description": "Array of hyperlinks to local or internet documents",
                "type": array,
                "Items": [
                    {
                        "LinkDescription": {
                        "description": "Text description of hyper link",
                        "type": "string"
                        },
                        "LinkURL": {
                        "description": "Actual Hyperlink",
                        "type": "string"
                        }
                    },
                    "required": [
                        "LinkDescription",
                        "LinkURL"
                    ]
                ]
            }
            
        },
        "required": [
            "Msg",
            "Type",
            "Source",
            "MsgId",
            "Lat",
            "Lon",
            "BoundaryPointType"
        ],
        "type": "object"
    }
)"_json;

static json BoundarySchema = R"(
    {
    "$schema": "http://json-schema.org/draft-04/schema#",
    "title": "Create Boundary",
    "properties": {
        "Msg": {
            "description": "Message Name",
            "type": "string",
            "enum": ["CreateBoundary"],
            "optional": false
        },
        "Type": {
            "description": "Type of Message",
            "type": "string",
            "const": ["Request"],
            "optional": false
        },
        "Source": {
            "description": "Source of message",
            "type": "string",
            "optional": false
        },
        "MsgID": {
            "description": "Message Name",
            "type": "string",
            "optional": false
        },
        "Name": {
            "description": "Boundary Name",
            "type": "string"
        },
        "BoundaryType": {
            "description": "Boundary Type",
            "type": "string",
            "enum": ["Exclusion","Inclusion","Neither"]
        },
        "Visible": {
            "description": "Visibility of Boundary",
            "type": "string",
            "enum": ["true","false"]
        },
        "BoundaryPoints": {
            "items": [
                {
                    "Name": {
                        "description": "Boundary Point Name",
                        "type": "string"
                    },
                    "Lat": {
                        "description": "Latitude of Boundary Point",
                        "type": "number",
                        "minimum": -90,
                        "maximum": 90,
                        "optional": false
                    },
                    "Lon": {
                        "description": "Longitude of Boundary Point",
                        "type": "number",
                        "minimum": -180,
                        "maximum": 180,
                        "optional": false
                    },
                    "IconName": {
                        "description": "Boundary Point Icon Name",
                        "type": "string"
                    },
                    "BoundaryPointType": {
                        "description": "Boundary Point Type",
                        "type": "string",
                        "enum": ["Exclusion","Inclusion","Neither"]
                    },
                    "Visible": {
                        "description": "Visibility of Boundary Point",
                        "type": "string",
                        "enum": ["true","false"]
                    },
                    "RingsVisible": {
                        "description": "Visibility of Boundary Point Range Rings",
                        "type": "string",
                        "enum": ["true","false"]
                    },
                    "RingsNumber": {
                        "description": "Number of Boundary Point Rings",
                        "type": "number",
                        "minimun": 0,
                        "maximum": 10
                    },
                    "RingsSteps": {
                        "description": "Steps between Boundary Point Rings",
                        "type": "number",
                        "minimun": 0.0
                    },
                    "RingUnits": {
                        "description": "Boundary Point Range Ring Step Units",
                        "type": "number",
                        "enum": [0,1]
                    },
                    "RingsColour": {
                        "description": "Boundary Point Range Rings Colour",
                        "type": "string"
                    }
                }
            ],
            "minItems": 3,
            "required": [
                "Lat",
                "Lon",
                "BoundaryPointType"
            ]
        }
    },
    "required": [
    "Msg",
    "Type",
    "Source",
    "MsgId"
    ],
    "type": "object"
    }
)"_json;

static json CreateTextPointSchema = R"(
    {
    "$schema": "http://json-schema.org/draft-04/schema#",
    "title": "Create Text Point",
    "TextPoint": {
        "type": "object",
        "properties": {
            "Msg": {
                "description": "Message Name",
                "type": "string",
                "enum": ["CreateTextPoint"],
                "optional": false
            },
            "Type": {
                "description": "Type of Message",
                "type": "string",
                "const": ["Request"],
                "optional": false
            },
            "Source": {
                "description": "Source of message",
                "type": "string",
                "optional": false
            },
            "MsgID": {
                "description": "Message Name",
                "type": "string",
                "optional": false
            },
            "Name": {
                "description": "Text Point Name",
                "type": "string"
            },
            "Lat": {
                "description": "Latitude of Text Point",
                "type": "number",
                "minimum": -90,
                "maximum": 90,
                "optional": false
            },
            "Lon": {
                "description": "Longitude of Text Point",
                "type": "number",
                "minimum": -180,
                "maximum": 180,
                "optional": false
            },
            "IconName": {
                "description": "Text Point Icon Name",
                "type": "string"
            },
            "Visible": {
                "description": "Visibility of Boundary Point",
                "type": "string",
                "enum": ["true","false"]
            },
            "GUID": {
                "description": "GUID of Text Point",
                "type": "string",
                "optional": true
            },
            "TextToDisplay": {
                "description": "Text Point Text",
                "type": "string"
            },
            "TextPosition": {
                "description": "Text Point Text Position",
                "type": "number",
                "enum": [0,1,2,3,4,5,6]
            },
            "TextColour": {
                "description": "Text Point Text Colour",
                "type": "string"
            },
            "BackgroundColour": {
                "description": "Text Point Text BackgroundColour",
                "type": "string"
            },
            "BackgroundTransparancy": {
                "description": "Text Point Text BackgroundTransparancy",
                "type": "number",
                "minimum": 0,
                "maximum": 255
            },
            "TextFont": {
                "description": "Text Point Font",
                "type": "string"
            },
            "TextPointDisplayTextWhen": {
                "description": "Text Point When To Display",
                "type": "string"
            },
            "RingsVisible": {
                "description": "Visibility of Boundary Point Range Rings",
                "type": "string",
                "enum": ["true","false"]
            },
            "RingsNumber": {
                "description": "Number of Text Point Rings",
                "type": "number",
                "minimun": 0,
                "maximum": 10
            },
            "RingsSteps": {
                "description": "Steps between Text Point Rings",
                "type": "number",
                "minimun": 0.0
            },
            "RingUnits": {
                "description": "Text Point Range Ring Step Units",
                "type": "number",
                "enum": [0,1]
            },
            "RingsColour": {
                "description": "Text Point Range Rings Colour",
                "type": "string"
            },
            "RingsWidth": {
                "description": "Text Point Range Rings Colour",
                "type": "number"
            },
            "RingsStyle": {
                "description": "Text Point Range Rings Colour",
                "type": "string"
            },
            "HyperLinks": {
                "description": "Array of hyperlinks to local or internet documents",
                "type": array,
                "Items": [
                    {
                    "LinkDescription": {
                        "description": "Text description of hyper link",
                        "type": "string"
                    },
                    "LinkURL": {
                        "description": "Actual Hyperlink",
                        "type": "string"
                    }
                    },
                    "required": [
                        "LinkDescription",
                        "LinkURL"
                    ]
                ]
            }
            
        }
    },
    "required": [
    "Msg",
    "Type",
    "Source",
    "MsgId",
    "Lat",
    "Lon"
    ],
    "type": "object"
    }
)"_json;
    
static json DeleteTextPointSchema = R"(
    {
    "$schema": "http://json-schema.org/draft-04/schema#",
    "title": "Create Text Point",
    "TextPoint": {
        "type": "object",
        "properties": {
            "Msg": {
                "description": "Message Name",
                "type": "string",
                "enum": ["DeleteTextPoint"],
                "optional": false
            },
            "Type": {
                "description": "Type of Message",
                "type": "string",
                "const": ["Request"],
                "optional": false
            },
            "Source": {
                "description": "Source of message",
                "type": "string",
                "optional": false
            },
            "MsgID": {
                "description": "Message Name",
                "type": "string",
                "optional": false
            },
            "GUID": {
                "description": "GUID of Text Point",
                "type": "string",
                "optional": true
            },
            "PointIconName": {
                "description": "Point Icon bitmap name",
                "type": "string",
                "optional": false
            }
        }
    },
    "required": [
    "Msg",
    "Type",
    "Source",
    "MsgId",
    "GUID",
    "PointIconName"
    ],
    "type": "object"
    }
)"_json;
    
static json AddPointIconSchema = R"(
{
    "$schema": "http://json-schema.org/draft-04/schema#",
    "title": "Add Point Icon",
    "AddPointIcon": {
        "type": "object",
        "properties": {
            "Msg": {
            "description": "Message Name",
            "type": "string",
            "enum": ["DeleteTextPoint"],
            "optional": false
            },
            "Type": {
            "description": "Type of Message",
            "type": "string",
            "const": ["Request"],
            "optional": false
            },
            "Source": {
            "description": "Source of message",
            "type": "string",
            "optional": false
            },
            "MsgID": {
            "description": "Message Name",
            "type": "string",
            "optional": false
            },
            "GUID": {
            "description": "GUID of Text Point",
            "type": "string",
            "optional": false
            },
            "PointIconName": {
            "description": "Point Icon bitmap name",
            "type": "string",
            "optional": false
            },
            "PointIcon": {
            "description": "Point Icon bitmap in base64",
            "type": "string",
            "optional": false
            },
            
            
        }
    },
    "required": [
    "Msg",
    "Type",
    "Source",
    "MsgId",
    "PointIconName",
    "PointIcon"
    ],
    "type": "object"
    }
)"_json;
        
static json DeletePointIconSchema = R"(
    {
        "$schema": "http://json-schema.org/draft-04/schema#",
        "title": "Delete Point Icon",
        "DeletePointIcon": {
            "type": "object",
            "properties": {
                "Msg": {
                    "description": "Message Name",
                    "type": "string",
                    "enum": ["DeleteTextPoint"],
                    "optional": false
                },
                "Type": {
                    "description": "Type of Message",
                    "type": "string",
                    "const": ["Request"],
                    "optional": false
                },
                "Source": {
                    "description": "Source of message",
                    "type": "string",
                    "optional": false
                },
                "MsgID": {
                    "description": "Message Name",
                    "type": "string",
                    "optional": false
                },
                "GUID": {
                    "description": "GUID of Text Point",
                    "type": "string",
                    "optional": false
                }
            }
        },
        "required": [
            "Msg",
            "Type",
            "Source",
            "MsgId",
            "GUID"
        ],
        "type": "object"
    }
)"_json;
        


