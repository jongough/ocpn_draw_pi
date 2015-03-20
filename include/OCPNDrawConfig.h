/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPN Draw Config support
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
 
#ifndef OCPNDRAWCONFIG_H
#define OCPNDRAWCONFIG_H

#include <navutil.h>
#include "OCPNDrawNavObjectChanges.h"

class NavObjectCollection;
//class NavObjectChanges;

class OCPNDrawConfig : public MyConfig
{
    public:
//        OCPNDrawConfig(const wxString &appName, const wxString &vendorName, const wxString &LocalFileName) : MyConfig( appName, vendorName, LocalFileName) {}
        OCPNDrawConfig(const wxString &appName, const wxString &vendorName, const wxString &LocalFileName);
        virtual ~OCPNDrawConfig();

        virtual bool AddNewPath(Path *pr, int ConfigRouteNum = -1);
        virtual bool UpdatePath(Path *pr);
        virtual bool DeleteConfigPath(Path *pr);

        virtual bool AddNewOCPNPoint(OCPNPoint *pWP, int ConfigRouteNum = -1);
        virtual bool UpdateOCPNPoint(OCPNPoint *pWP);
        virtual bool DeleteOCPNPoint(OCPNPoint *pWP);

        virtual void UpdateNavObj();
        virtual void LoadNavObjects();

        bool ExportGPXPaths(wxWindow* parent, PathList *pPathss, const wxString suggestedName = _T("paths"));
        bool ExportGPXOCPNPoints(wxWindow* parent, OCPNPointList *pOCPNPoints, const wxString suggestedName = _T("OCPN points"));


        OCPNDrawNavObjectChanges  *m_pOCPNDrawNavObjectChangesSet;  
        wxString                m_sOCPNDrawNavObjSetFile;
        wxString                m_sOCPNDrawNavObjSetChangesFile;
          
        OCPNDrawNavObjectChanges    *m_pOCPNDrawNavObjectInputSet;

      
      
    protected:
    private:
};

#endif // OCPNDRAWCONFIG_H
