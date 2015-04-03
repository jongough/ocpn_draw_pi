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
#include "Layer.h"
#include "PointMan.h"
#include <wx/progdlg.h>

//extern OCPNDrawConfig   *pOCPNDrawConfig;
extern wxString         *g_pNavObjs;
extern int              g_LayerIdx;
extern bool             g_bShowLayers;
extern wxString         g_VisibleLayers;
extern wxString         g_InvisibleLayers;
extern LayerList        *pLayerList;
extern PointMan         *pOCPNPointMan;  
extern PathList         *pPathList;
extern int              g_navobjbackups;

//OCPNDrawConfig::OCPNDrawConfig(const wxString &appName, const wxString &vendorName,
//                              const wxString &LocalFileName) : MyConfig( appName, vendorName, LocalFileName)
OCPNDrawConfig::OCPNDrawConfig(const wxString &appName, const wxString &vendorName, const wxString &LocalFileName)
{
    //OCPNDrawConfig *pCF = new MyConfig::MyConfig( wxString( _T("") ), wxString( _T("") ), gConfig_File );
    //pOCPNDrawConfig->LoadMyConfig( 0 );
    m_sOCPNDrawNavObjSetFile = *g_pNavObjs;
    m_sOCPNDrawNavObjSetFile += wxS( "ODnavobj.xml" );
    m_sOCPNDrawNavObjSetChangesFile = m_sOCPNDrawNavObjSetFile + _T( ".changes" );
 
    m_pOCPNDrawNavObjectInputSet = NULL;
    m_pOCPNDrawNavObjectChangesSet = NULL;

    
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
            int answer = OCPNMessageBox_PlugIn( NULL, _("Overwrite existing file?"), _T("Confirm"),
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
    pNavObjectSet->SaveFile( m_sOCPNDrawNavObjSetFile );

    delete pNavObjectSet;

    if( ::wxFileExists( m_sOCPNDrawNavObjSetChangesFile ) )
        wxRemoveFile( m_sOCPNDrawNavObjSetChangesFile );

    delete m_pOCPNDrawNavObjectChangesSet;
    m_pOCPNDrawNavObjectChangesSet = new OCPNDrawNavObjectChanges(m_sOCPNDrawNavObjSetChangesFile);

}

void OCPNDrawConfig::LoadNavObjects()
{
    //      next thing to do is read tracks, etc from the NavObject XML file,
    wxLogMessage( _T("Loading navobjects from ODnavobj.xml") );
    CreateRotatingNavObjBackup();

    if( NULL == m_pOCPNDrawNavObjectInputSet )
        m_pOCPNDrawNavObjectInputSet = new OCPNDrawNavObjectChanges();

    if( ::wxFileExists( m_sOCPNDrawNavObjSetFile ) &&
        m_pOCPNDrawNavObjectInputSet->load_file( m_sOCPNDrawNavObjSetFile.fn_str() ) )
        m_pOCPNDrawNavObjectInputSet->LoadAllGPXObjects();

    wxLogMessage( _T("Done loading ODnavobjects") );
    delete m_pOCPNDrawNavObjectInputSet;

    if( ::wxFileExists( m_sOCPNDrawNavObjSetChangesFile ) ) {

        wxULongLong size = wxFileName::GetSize(m_sOCPNDrawNavObjSetChangesFile);

        //We crashed last time :(
        //That's why this file still exists...
        //Let's reconstruct the unsaved changes
        OCPNDrawNavObjectChanges *pOCPNDrawNavObjectChangesSet = new OCPNDrawNavObjectChanges();
        pOCPNDrawNavObjectChangesSet->load_file( m_sOCPNDrawNavObjSetChangesFile.fn_str() );

        //  Remove the file before applying the changes,
        //  just in case the changes file itself causes a fault.
        //  If it does fault, at least the next restart will proceed without fault.
        if( ::wxFileExists( m_sOCPNDrawNavObjSetChangesFile ) )
            ::wxRemoveFile( m_sOCPNDrawNavObjSetChangesFile );
        
        if(size != 0){
            wxLogMessage( _T("Applying NavObjChanges") );
            pOCPNDrawNavObjectChangesSet->ApplyChanges();
            UpdateNavObj();
        }
        
        delete pOCPNDrawNavObjectChangesSet;
           
    }

    m_pOCPNDrawNavObjectChangesSet = new OCPNDrawNavObjectChanges(m_sOCPNDrawNavObjSetChangesFile);
}

void OCPNDrawConfig::ExportGPX( wxWindow* parent, bool bviz_only, bool blayer )
{
    wxFileDialog saveDialog( NULL, _( "Export GPX file" ), m_gpx_path, wxT ( "" ),
            wxT ( "GPX files (*.gpx)|*.gpx" ), wxFD_SAVE );

    int response = saveDialog.ShowModal();

    wxString path = saveDialog.GetPath();
    wxFileName fn( path );
    m_gpx_path = fn.GetPath();

    if( response == wxID_OK ) {
        fn.SetExt( _T ( "gpx" ) );

        if( wxFileExists( fn.GetFullPath() ) ) {
            int answer = OCPNMessageBox( NULL, _("Overwrite existing file?"), _T("Confirm"),
                    wxICON_QUESTION | wxYES_NO | wxCANCEL );
            if( answer != wxID_YES ) return;
        }

        ::wxBeginBusyCursor();

        OCPNDrawNavObjectChanges *pgpx = new OCPNDrawNavObjectChanges;

        wxProgressDialog *pprog = NULL;
        int count = pOCPNPointMan->GetOCPNPointList()->GetCount();
        if( count > 200) {
            pprog = new wxProgressDialog( _("Export GPX file"), _T("0/0"), count, NULL,
                                          wxPD_APP_MODAL | wxPD_SMOOTH |
                                          wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME );
            pprog->SetSize( 400, wxDefaultCoord );
            pprog->Centre();
        }

        //WPTs
        int ic = 0;

        wxOCPNPointListNode *node = pOCPNPointMan->GetOCPNPointList()->GetFirst();
        OCPNPoint *pr;
        while( node ) {
            if(pprog) {
                wxString msg;
                msg.Printf(_T("%d/%d"), ic, count);
                pprog->Update( ic, msg );
                ic++;
            }

            pr = node->GetData();

            bool b_add = true;

            if( bviz_only && !pr->m_bIsVisible )
                b_add = false;

            if( pr->m_bIsInLayer && !blayer )
                b_add = false;
            if( b_add) {
                if( pr->m_bKeepXPath || !OCPNPointIsInPathList( pr ) )
                    pgpx->AddGPXOCPNPoint( pr);
            }

            node = node->GetNext();
        }
        //Paths
        wxPathListNode *node1 = pPathList->GetFirst();
        while( node1 ) {
            Path *pPath = node1->GetData();

            bool b_add = true;

            if( bviz_only && !pPath->IsVisible() )
                b_add = false;

            if(  pPath->m_bIsInLayer && !blayer )
                b_add = false;

            if( b_add ) {
                pgpx->AddGPXPath( pPath ); 
                }
            node1 = node1->GetNext();
        }

        pgpx->SaveFile( fn.GetFullPath() );
        delete pgpx;
        ::wxEndBusyCursor();

        if( pprog)
            delete pprog;

    }
}

void OCPNDrawConfig::UI_ImportGPX( wxWindow* parent, bool islayer, wxString dirpath, bool isdirectory )
{
    int response = wxID_CANCEL;
    wxArrayString file_array;
    Layer *l = NULL;

    if( !islayer || dirpath.IsSameAs( _T("") ) ) {
        wxFileDialog openDialog( NULL, _( "Import GPX file" ), m_gpx_path, wxT ( "" ),
                wxT ( "GPX files (*.gpx)|*.gpx|All files (*.*)|*.*" ),
                wxFD_OPEN | wxFD_MULTIPLE );
        openDialog.Centre();
        response = openDialog.ShowModal();
        if( response == wxID_OK ) {
            openDialog.GetPaths( file_array );

            //    Record the currently selected directory for later use
            if( file_array.GetCount() ) {
                wxFileName fn( file_array[0] );
                m_gpx_path = fn.GetPath();
            }
        }

    } else {
        if( isdirectory ) {
            if( wxDir::GetAllFiles( dirpath, &file_array, wxT("*.gpx") ) )
                response = wxID_OK;
        } else {
            file_array.Add( dirpath );
            response = wxID_OK;
        }
    }

    if( response == wxID_OK ) {

        if( islayer ) {
            l = new Layer();
            l->m_LayerID = ++g_LayerIdx;
            l->m_LayerFileName = file_array[0];
            if( file_array.GetCount() <= 1 ) wxFileName::SplitPath( file_array[0], NULL, NULL,
                    &( l->m_LayerName ), NULL, NULL );
            else {
                if( dirpath.IsSameAs( _T("") ) ) wxFileName::SplitPath( m_gpx_path, NULL, NULL,
                        &( l->m_LayerName ), NULL, NULL );
                else
                    wxFileName::SplitPath( dirpath, NULL, NULL, &( l->m_LayerName ), NULL, NULL );
            }

            bool bLayerViz = g_bShowLayers;
            if( g_VisibleLayers.Contains( l->m_LayerName ) )
                bLayerViz = true;
            if( g_InvisibleLayers.Contains( l->m_LayerName ) )
                bLayerViz = false;
            l->m_bIsVisibleOnChart = bLayerViz;

            wxString laymsg;
            laymsg.Printf( wxT("New layer %d: %s"), l->m_LayerID, l->m_LayerName.c_str() );
            wxLogMessage( laymsg );

            pLayerList->Insert( l );
        }

        for( unsigned int i = 0; i < file_array.GetCount(); i++ ) {
            wxString path = file_array[i];

            if( ::wxFileExists( path ) ) {

                NavObjectCollection1 *pSet = new NavObjectCollection1;
                pSet->load_file(path.fn_str());

                if(islayer){
                    l->m_NoOfItems = pSet->LoadAllGPXObjectsAsLayer(l->m_LayerID, l->m_bIsVisibleOnChart);
                }
                else
                    pSet->LoadAllGPXObjects( true );    // Import with full vizibility of names and objects

                delete pSet;
            }
        }
    }
}

void OCPNDrawConfig::CreateRotatingNavObjBackup()
{
    //Rotate navobj backups, but just in case there are some changes in the current version
    //to prevent the user trying to "fix" the problem by continuously starting the
    //application to overwrite all of his good backups...
    if( g_navobjbackups > 0 ) {
        wxFile f;
        wxString oldname = m_sOCPNDrawNavObjSetFile;
        wxString newname = wxString::Format( _T("%s.1"), m_sOCPNDrawNavObjSetFile.c_str() );

        wxFileOffset s_diff = 1;
        if( ::wxFileExists( newname ) ) {

            if( f.Open(oldname) ){
                s_diff = f.Length();
                f.Close();
            }

            if( f.Open(newname) ){
                s_diff -= f.Length();
                f.Close();
            }
        }


        if ( s_diff != 0 )
        {
            for( int i = g_navobjbackups - 1; i >= 1; i-- )
            {
                oldname = wxString::Format( _T("%s.%d"), m_sOCPNDrawNavObjSetFile.c_str(), i );
                newname = wxString::Format( _T("%s.%d"), m_sOCPNDrawNavObjSetFile.c_str(), i + 1 );
                if( wxFile::Exists( oldname ) )
                    wxCopyFile( oldname, newname );
            }

            if( wxFile::Exists( m_sOCPNDrawNavObjSetFile ) )
            {
                newname = wxString::Format( _T("%s.1"), m_sOCPNDrawNavObjSetFile.c_str() );
                wxCopyFile( m_sOCPNDrawNavObjSetFile, newname );
            }
        }
    }
    //try to clean the backups the user doesn't want - breaks if he deleted some by hand as it tries to be effective...
    for( int i = g_navobjbackups + 1; i <= 99; i++ )
        if( wxFile::Exists( wxString::Format( _T("%s.%d"), m_sOCPNDrawNavObjSetFile.c_str(), i ) ) ) wxRemoveFile(
                wxString::Format( _T("%s.%d"), m_sOCPNDrawNavObjSetFile.c_str(), i ) );
        else
            break;
}

bool OCPNDrawConfig::OCPNPointIsInPathList( OCPNPoint *pr )
{
    bool IsInList = false;

    wxPathListNode *node1 = pPathList->GetFirst();
    while( node1 ) {
        Path *pPath = node1->GetData();
        OCPNPointList *pOCPNPointList = pPath->pOCPNPointList;

        wxOCPNPointListNode *node2 = pOCPNPointList->GetFirst();
        OCPNPoint *prp;

        while( node2 ) {
            prp = node2->GetData();

            if( pr->IsSame( prp ) ) {
                IsInList = true;
                break;
            }

            node2 = node2->GetNext();
        }
        node1 = node1->GetNext();
    }
    return IsInList;
}

