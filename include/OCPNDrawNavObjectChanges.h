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

#define OPT_OCPNPOINT     OPT_WPT                        


class OCPNDrawNavObjectChanges : public NavObjectChanges
{
    public:
        OCPNDrawNavObjectChanges();
        OCPNDrawNavObjectChanges( wxString file_name ) : NavObjectChanges( file_name ) {};
        virtual ~OCPNDrawNavObjectChanges();

    bool CreateNavObjGPXPaths(void);
    
    bool AddPath ( Path *pb, const char *action );
    bool AddOCPNPoint( OCPNPoint *pr, const char *action );
    bool AddGPXPathsList( PathList *pPaths );
    bool AddGPXPath(Path *pPath);
    bool AddGPXOCPNPoint(OCPNPoint *pWP );
    bool AddGPXOCPNPointsList( OCPNPointList *pOCPNPoints );

    bool CreateAllGPXObjects();

    protected:
    private:
};

#endif // OCPNDRAWNAVOBJECTCHANGES_H
