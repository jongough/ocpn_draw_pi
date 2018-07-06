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

static json TextPointSchema = R"(
    {
    "$schema": "http://json-schema.org/draft-04/schema#",
    "title": "Create Text Point",
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
    
    
