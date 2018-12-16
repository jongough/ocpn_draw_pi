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
 
#ifndef ODCONFIG_H
#define ODCONFIG_H

#include "ODNavObjectChanges.h"

class NavObjectCollection;
//class NavObjectChanges;

//class ODConfig : public MyConfig
class ODConfig
{
    public:
//        ODConfig(const wxString &appName, const wxString &vendorName, const wxString &LocalFileName) : MyConfig( appName, vendorName, LocalFileName) {}
        ODConfig(const wxString &appName, const wxString &vendorName, const wxString &LocalFileName);
        virtual ~ODConfig();
        void    LateInit(void);
        void    DeInit(void);
        
        virtual bool AddNewPath(ODPath *pr, int ConfigRouteNum = -1);
        virtual bool UpdatePath(ODPath *pr);
        virtual bool DeleteConfigPath(ODPath *pr);
        
        virtual bool AddNewODPoint(ODPoint *pWP, int ConfigRouteNum = -1);
        virtual bool UpdateODPoint(ODPoint *pWP);
        virtual bool DeleteODPoint(ODPoint *pWP);

        virtual void UpdateNavObj(void);
        virtual void LoadNavObjects();
        virtual bool LoadLayers(wxString &path);

        void ExportGPX(wxWindow* parent, bool bviz_only = false, bool blayer = false);
        void UI_ImportGPX(wxWindow* parent, bool islayer = false, wxString dirpath = _T(""), bool isdirectory = true);
        void UI_Import(wxWindow* parent, bool islayer = false, bool isTemporary = false, wxString dirpath = _T(""), bool isdirectory = true);
        
        bool ExportGPXPaths(wxWindow* parent, PathList *pPathss, const wxString suggestedName = _T("paths"));
        bool ExportGPXODPoints(wxWindow* parent, ODPointList *pODPoints, const wxString suggestedName = _T("OD Points"));
        
        void CreateRotatingNavObjBackup();
        bool ODPointIsInPathList( ODPoint *pr );

        ODNavObjectChanges  *m_pODNavObjectChangesSet;  
        wxString                m_sODNavObjSetFile;
        wxString                m_sODNavObjSetChangesFile;
          
        ODNavObjectChanges    *m_pODNavObjectInputSet;

        bool                    m_bSkipChangeSetUpdate;
        
        wxString                m_sGPX_Path;
        wxString                m_sCSV_Path;
        wxString                m_sImport_Path;
        wxString                m_sImport_Type;
        wxString                m_sWildcardString;
        
      
      
    protected:
    private:
};

#endif // OCPNDRAWCONFIG_H
