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
 
#ifndef ODNAVOBJECTCHANGES_H
#define ODNAVOBJECTCHANGES_H

#include "pugixml.hpp"
#include "ODPath.h"

//      Bitfield definition controlling the GPX nodes output for point objects
#define         OUT_TYPE        1 << 1          //  Output point type
#define         OUT_TIME        1 << 2          //  Output time as ISO string
#define         OUT_NAME        1 << 3          //  Output point name if defined
#define         OUT_NAME_FORCE  1 << 4          //  Output point name, even if empty
#define         OUT_DESC        1 << 5          //  Output description if defined
#define         OUT_DESC_FORCE  1 << 6          //  Output description, even if empty
#define         OUT_SYM_FORCE   1 << 7          //  Output symbol name, using "empty" if undefined
#define         OUT_GUID        1 << 8          //  Output GUID if defined
#define         OUT_VIZ         1 << 9          //  Output point viz, if non-zero(true)
#define         OUT_VIZ_NAME    1 << 10         //  Output point name viz, if non-zero(true)
#define         OUT_SHARED      1 << 11         //  Output point shared state, if non-zero(true)
#define         OUT_AUTO_NAME   1 << 12         //  Output point auto_name state, if non-zero(true)
#define         OUT_HYPERLINKS  1 << 13         //  Output point Hyperlinks, if present
#define         OUT_ACTION_ADD  1 << 14         //  opencpn:action node support
#define         OUT_ACTION_DEL  1 << 15
#define         OUT_ACTION_UPD  1 << 16
#define         OUT_EXTENSION   1 << 17
#define         OUT_ARRIVAL_RADIUS 1 << 18
#define         OUT_OCPNPOINT_RANGE_RINGS 1 << 19
#define         OUT_POINTTEXT 1 << 20
#define         OUT_SINGLEUSE 1<<21             // Output use only in a single object

#define  OPT_OCPNPOINT  (OUT_TYPE) +\
                        (OUT_TIME) +\
                        (OUT_NAME) +\
                        (OUT_DESC) +\
                        (OUT_SYM_FORCE) +\
                        (OUT_GUID) +\
                        (OUT_VIZ) +\
                        (OUT_VIZ_NAME) +\
                        (OUT_SHARED) +\
                        (OUT_AUTO_NAME) +\
                        (OUT_HYPERLINKS) +\
                        (OUT_ARRIVAL_RADIUS) +\
                        (OUT_OCPNPOINT_RANGE_RINGS) +\
                        (OUT_POINTTEXT)+\
                        (OUT_SINGLEUSE)

//class ODNavObjectChanges : public NavObjectChanges
class ODNavObjectChanges : public pugi::xml_document
{
    public:
        ODNavObjectChanges();
        ODNavObjectChanges( wxString file_name );
        virtual ~ODNavObjectChanges();

    bool CreateNavObjGPXPaths(void);
    bool CreateNavObjGPXPoints( void );    
    
    bool AddPath ( ODPath *pb, const char *action );
    bool AddODPoint( ODPoint *pr, const char *action );
    bool AddGPXPathsList( PathList *pPaths );
    bool AddGPXPath(ODPath *pPath);
    bool AddGPXODPoint(ODPoint *pWP );
    bool AddGPXODPointsList( ODPointList *pODPoints );
    bool GPXCreatePath( pugi::xml_node node, ODPath *pPath );
    bool GPXCreateODPoint( pugi::xml_node node, ODPoint *pop, unsigned int flags );
    bool LoadAllGPXObjects( bool b_full_viz = false);
    int  LoadAllGPXObjectsAsLayer(int layer_id, bool b_layerviz);
    int Load_CSV_File(wxString file, int layer_id = 0, bool b_layerviz = false);
    void InsertPathA( ODPath *pTentPath );
    //bool LoadAllCSVObjects( bool b_full_viz = false);
    //ODPoint * GPXLoadODPoint1( pugi::xml_node &odpt_node, wxString def_symbol_name, wxString GUID, bool b_fullviz, bool b_layer, bool b_layerviz, int layer_id );

    bool CreateAllGPXObjects();
    void SetRootGPXNode(void);
    
    bool ApplyChanges(void);
    bool SaveFile( const wxString filename );
    void RemoveChangesFile( void );
    
    FILE        *m_ODchanges_file;
    
    pugi::xml_node      m_gpx_root;
    
    bool            m_bFirstPath;

    protected:
    private:
        ODPoint *GPXLoadODPoint1( pugi::xml_node &opt_node, 
                               wxString def_symbol_name,
                               wxString GUID,
                               bool b_fullviz,
                               bool b_layer,
                               bool b_layerviz,
                               int layer_id,
                               bool b_InPath = false
                             );
        ODPath *GPXLoadPath1( pugi::xml_node &odpoint_node, bool b_fullviz,
                      bool b_layer, bool b_layerviz, int layer_id, wxString *TypeString );
        ODPoint *ODPointExists( const wxString& guid );
        ODPoint *ODPointExists( const wxString& name, double lat, double lon );

        ODPoint *tempODPointExists( const wxString& guid );
        void tempODPointRemove( const wxString& guid );

        void UpdatePathA( ODPath *pTentPath );
        ODPath *PathExists( const wxString& guid);
        ODPath *PathExists( ODPath * pTentPath );
        void CreateRotatingNavObjChangesBackup();
        
        wxString m_ODfilename;
        ODPointList *m_ptODPointList;

};

#endif // OCPNDRAWNAVOBJECTCHANGES_H
