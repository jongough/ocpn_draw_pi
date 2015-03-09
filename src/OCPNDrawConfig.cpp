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

#include "OCPNDrawConfig.h"
#include "OCPNDrawNavObjectChanges.h"

extern OCPNDrawConfig *pConfig;

OCPNDrawConfig::OCPNDrawConfig(const wxString &appName, const wxString &vendorName,
                              const wxString &LocalFileName) : MyConfig( appName, vendorName, LocalFileName)
{
    //OCPNDrawConfig *pCF = new MyConfig::MyConfig( wxString( _T("") ), wxString( _T("") ), gConfig_File );
    pConfig = (OCPNDrawConfig *) this;
    //pConfig->LoadMyConfig( 0 );
}

OCPNDrawConfig::~OCPNDrawConfig()
{
    //dtor
}

bool OCPNDrawConfig::AddNewPath( Path *pb, int crm )
{
    if( pb->m_bIsInLayer )
        return true;


    if( !m_bSkipChangeSetUpdate ) {
        m_pOCPNDrawNavObjectChangesSet->AddPath( pb, "add" );
    }

    return true;
}

bool OCPNDrawConfig::UpdatePath( Path *pb )
{
    if( pb->m_bIsInLayer ) return true;


    if( !m_bSkipChangeSetUpdate ) {
        m_pOCPNDrawNavObjectChangesSet->AddPath( pb, "update" );

    }

    return true;
}

bool OCPNDrawConfig::DeleteConfigPath( Path *pb )
{
    return true;
}

bool OCPNDrawConfig::AddNewOCPNPoint( OCPNPoint *pWP, int crm )
{
    if( pWP->m_bIsInLayer )
        return true;

    if(!pWP->m_bIsolatedMark)
        return true;

    if( !m_bSkipChangeSetUpdate ) {
        m_pOCPNDrawNavObjectChangesSet->AddOCPNPoint( pWP, "add" );
    }

    return true;
}

bool OCPNDrawConfig::UpdateOCPNPoint( OCPNPoint *pWP )
{
    if( pWP->m_bIsInLayer )
        return true;

    if( !m_bSkipChangeSetUpdate ) {
        m_pOCPNDrawNavObjectChangesSet->AddOCPNPoint( pWP, "update" );
    }

    return true;
}

bool OCPNDrawConfig::DeleteOCPNPoint( OCPNPoint *pWP )
{
    if( pWP->m_bIsInLayer )
        return true;

    if( !m_bSkipChangeSetUpdate ) {
        m_pOCPNDrawNavObjectChangesSet->AddOCPNPoint( pWP, "delete" );
    }

    return true;
}

bool OCPNDrawConfig::ExportGPXPaths( wxWindow* parent, PathList *pPaths, const wxString suggestedName )
{
    wxFileDialog saveDialog( NULL, _( "Export GPX file" ), m_gpx_path, suggestedName,
            wxT ( "GPX files (*.gpx)|*.gpx" ), wxFD_SAVE );

#ifdef __WXOSX__
    if(parent)
        parent->HideWithEffect(wxSHOW_EFFECT_BLEND );
#endif

     int response = saveDialog.ShowModal();

#ifdef __WXOSX__
    if(parent)
        parent->ShowWithEffect(wxSHOW_EFFECT_BLEND );
#endif

    wxString path = saveDialog.GetPath();
    wxFileName fn( path );
    m_gpx_path = fn.GetPath();

    if( response == wxID_OK ) {
        fn.SetExt( _T ( "gpx" ) );

        if( wxFileExists( fn.GetFullPath() ) ) {
            int answer = OCPNMessageBox( NULL, _("Overwrite existing file?"), _T("Confirm"),
                    wxICON_QUESTION | wxYES_NO | wxCANCEL );
            if( answer != wxID_YES ) return false;
        }

        OCPNDrawNavObjectChanges *pgpx = new OCPNDrawNavObjectChanges;
        pgpx->AddGPXPathsList( pPaths );
        pgpx->SaveFile(fn.GetFullPath());
        delete pgpx;

        return true;
    } else
        return false;
}

bool OCPNDrawConfig::ExportGPXOCPNPoints( wxWindow* parent, OCPNPointList *pOCPNPoints, const wxString suggestedName )
{
    wxFileDialog saveDialog( NULL, _( "Export GPX file" ), m_gpx_path, suggestedName,
            wxT ( "GPX files (*.gpx)|*.gpx" ), wxFD_SAVE );

    int response = saveDialog.ShowModal();

    wxString path = saveDialog.GetPath();
    wxFileName fn( path );
    m_gpx_path = fn.GetPath();

    if( response == wxID_OK ) {
        fn.SetExt( _T ( "gpx" ) );

        if( wxFileExists( fn.GetFullPath() ) ) {
            int answer = OCPNMessageBox(NULL,  _("Overwrite existing file?"), _T("Confirm"),
                    wxICON_QUESTION | wxYES_NO | wxCANCEL );
            if( answer != wxID_YES ) return false;
        }

        OCPNDrawNavObjectChanges *pgpx = new OCPNDrawNavObjectChanges;
        pgpx->AddGPXOCPNPointsList( pOCPNPoints );
        pgpx->SaveFile(fn.GetFullPath());
        delete pgpx;

        return true;
    } else
        return false;
}

void OCPNDrawConfig::UpdateNavObj( void )
{
//   Create the NavObjectCollection, and save to specified file
    OCPNDrawNavObjectChanges *pNavObjectSet = new OCPNDrawNavObjectChanges();

    pNavObjectSet->CreateAllGPXObjects();
    pNavObjectSet->SaveFile( m_sNavObjSetFile );

    delete pNavObjectSet;

    if( ::wxFileExists( m_sNavObjSetChangesFile ) )
        wxRemoveFile( m_sNavObjSetChangesFile );

    delete m_pNavObjectChangesSet;
    m_pNavObjectChangesSet = new NavObjectChanges(m_sNavObjSetChangesFile);

}
