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
 
#ifndef OCPNDRAWNAVOBJECTCHANGES_H
#define OCPNDRAWNAVOBJECTCHANGES_H

#include <NavObjectCollection.h>
#include "Path.h"

#define OUT_OCPNPOINT_RANGE_RINGS OUT_WAYPOINT_RANGE_RINGS << 1

#define OPT_OCPNPOINT   (OPT_WPT) +\
                        (OUT_OCPNPOINT_RANGE_RINGS)


class OCPNDrawNavObjectChanges : public NavObjectChanges
{
    public:
        OCPNDrawNavObjectChanges();
        OCPNDrawNavObjectChanges( wxString file_name );
        virtual ~OCPNDrawNavObjectChanges();

    bool CreateNavObjGPXPaths(void);
    
    bool AddPath ( Path *pb, const char *action );
    bool AddOCPNPoint( OCPNPoint *pr, const char *action );
    bool AddGPXPathsList( PathList *pPaths );
    bool AddGPXPath(Path *pPath);
    bool AddGPXOCPNPoint(OCPNPoint *pWP );
    bool AddGPXOCPNPointsList( OCPNPointList *pOCPNPoints );
    bool LoadAllGPXObjects( bool b_full_viz = false);


    bool CreateAllGPXObjects();
    void SetRootGPXNode(void);
    
    FILE        *m_OCPNDrawchanges_file;

    protected:
    private:
        OCPNPoint *GPXLoadOCPNPoint1( pugi::xml_node &opt_node, 
                               wxString def_symbol_name,
                               wxString GUID,
                               bool b_fullviz,
                               bool b_layer,
                               bool b_layerviz,
                               int layer_id
                             );
        Path *GPXLoadPath1( pugi::xml_node &wpt_node, bool b_fullviz,
                      bool b_layer, bool b_layerviz, int layer_id, wxString *TypeString );
        OCPNPoint *OCPNPointExists( const wxString& guid );
        OCPNPoint *OCPNPointExists( const wxString& name, double lat, double lon );
        void InsertPathA( Path *pTentPath );
        Path *PathExists( const wxString& guid);
        Path *PathExists( Path * pTentPath );
        wxString m_ODfilename;


};

#endif // OCPNDRAWNAVOBJECTCHANGES_H
