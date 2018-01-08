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

#include "ODConfig.h"
#include "ODNavObjectChanges.h"
#include "EBL.h"
#include "Layer.h"
#include "PointMan.h"
#include "pugixml.hpp"
#include "wxWTranslateCatalog.h"
#include <wx/progdlg.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/dir.h>
#ifdef __WXOSX__
#include <wx/stdpaths.h>
#endif

#include <map>

extern wxString         *g_pData;
extern int              g_LayerIdx;
extern bool             g_bShowLayers;
extern wxString         g_VisibleLayers;
extern wxString         g_InvisibleLayers;
extern LayerList        *pLayerList;
extern PointMan         *g_pODPointMan;  
extern PathList         *g_pPathList;
extern int              g_navobjbackups;
extern EBLList          *g_pEBLList;


//ODConfig::ODConfig(const wxString &appName, const wxString &vendorName,
//                              const wxString &LocalFileName) : MyConfig( appName, vendorName, LocalFileName)
ODConfig::ODConfig(const wxString &appName, const wxString &vendorName, const wxString &LocalFileName)
{
    //ODConfig *pCF = new MyConfig::MyConfig( wxString( _T("") ), wxString( _T("") ), gConfig_File );
    //g_pODConfig->LoadMyConfig( 0 );

    m_sODNavObjSetFile = *g_pData;

    m_sODNavObjSetFile += wxS( "ODnavobj.xml" );
    m_sODNavObjSetChangesFile = m_sODNavObjSetFile + _T( ".changes" );
 
    m_pODNavObjectInputSet = NULL;
    m_pODNavObjectChangesSet = NULL;
    m_bSkipChangeSetUpdate = FALSE;
    
}

ODConfig::~ODConfig()
{
    //dtor
}

bool ODConfig::AddNewPath( ODPath *pb, int crm )
{
    if( pb->m_bIsInLayer )
        return true;


    if( !m_bSkipChangeSetUpdate ) {
        m_pODNavObjectChangesSet->AddPath( pb, "add" );
    }

    return true;
}

bool ODConfig::UpdatePath( ODPath *pb )
{
    if( pb->m_bIsInLayer ) return true;

    if( !m_bSkipChangeSetUpdate ) {
        if(pb->m_bSaveUpdates)
            m_pODNavObjectChangesSet->AddPath( pb, "update" );

    }

    return true;
}

bool ODConfig::DeleteConfigPath( ODPath *pb )
{
    if( pb->m_bIsInLayer ) return true;
    
    if( !m_bSkipChangeSetUpdate ) {
        m_pODNavObjectChangesSet->AddPath( pb, "delete" );
        
    }
    return true;
}

bool ODConfig::AddNewODPoint( ODPoint *pWP, int crm )
{
    if( pWP->m_bIsInLayer )
        return true;

    if(!pWP->m_bIsolatedMark)
        return true;

    if( !m_bSkipChangeSetUpdate ) {
        m_pODNavObjectChangesSet->AddODPoint( pWP, "add" );
    }

    return true;
}

bool ODConfig::UpdateODPoint( ODPoint *pWP )
{
    if( pWP->m_bIsInLayer )
        return true;

    if( !m_bSkipChangeSetUpdate ) {
        m_pODNavObjectChangesSet->AddODPoint( pWP, "update" );
    }

    return true;
}

bool ODConfig::DeleteODPoint( ODPoint *pWP )
{
    if( pWP->m_bIsInLayer )
        return true;

    if( !m_bSkipChangeSetUpdate ) {
        m_pODNavObjectChangesSet->AddODPoint( pWP, "delete" );
    }

    return true;
}

bool ODConfig::ExportGPXPaths( wxWindow* parent, PathList *pPaths, const wxString suggestedName )
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
            int answer = OCPNMessageBox_PlugIn( NULL, _("Overwrite existing file?"), _("Confirm"),
                    wxICON_QUESTION | wxYES_NO | wxCANCEL );
            if( answer != wxID_YES ) return false;
        }

        ODNavObjectChanges *pgpx = new ODNavObjectChanges;
        pgpx->AddGPXPathsList( pPaths );
        pgpx->SaveFile(fn.GetFullPath());
        delete pgpx;

        return true;
    } else
        return false;
}

bool ODConfig::ExportGPXODPoints( wxWindow* parent, ODPointList *pODPoints, const wxString suggestedName )
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
            int answer = OCPNMessageBox_PlugIn(NULL,  _("Overwrite existing file?"), _("Confirm"),
                    wxICON_QUESTION | wxYES_NO | wxCANCEL );
            if( answer != wxID_YES ) return false;
        }

        ODNavObjectChanges *pgpx = new ODNavObjectChanges;
        pgpx->AddGPXODPointsList( pODPoints );
        pgpx->SaveFile(fn.GetFullPath());
        delete pgpx;

        return true;
    } else
        return false;
}

void ODConfig::UpdateNavObj( void )
{
//   Create the NavObjectCollection, and save to specified file
    ODNavObjectChanges *pNavObjectSet = new ODNavObjectChanges();

    pNavObjectSet->CreateAllGPXObjects();
    pNavObjectSet->SaveFile( m_sODNavObjSetFile );
    
    delete pNavObjectSet;

    if( ::wxFileExists( m_sODNavObjSetChangesFile ) ){
        wxLogNull logNo;        
        delete m_pODNavObjectChangesSet;
    }
}

void ODConfig::LoadNavObjects()
{
    //      next thing to do is read tracks, etc from the NavObject XML file,
    wxString sLogMessage;
    sLogMessage.append( _("Loading navobjects from ") );
    sLogMessage.append(m_sODNavObjSetFile );
    wxLogMessage( sLogMessage );
    CreateRotatingNavObjBackup();

    if( NULL == m_pODNavObjectInputSet )
        m_pODNavObjectInputSet = new ODNavObjectChanges();

    if( wxFileExists( m_sODNavObjSetFile ) &&
        m_pODNavObjectInputSet->load_file( m_sODNavObjSetFile.fn_str() ) )
        m_pODNavObjectInputSet->LoadAllGPXObjects();

    wxLogMessage( _T("Done loading ODnavobjects") );
    delete m_pODNavObjectInputSet;

    if( wxFileExists( m_sODNavObjSetChangesFile ) ) {

        wxULongLong size = wxFileName::GetSize(m_sODNavObjSetChangesFile);

        //We crashed last time :(
        //That's why this file still exists...
        //Let's reconstruct the unsaved changes
        m_pODNavObjectChangesSet->load_file( m_sODNavObjSetChangesFile.fn_str() );
        

        //  Remove the file before applying the changes,
        //  just in case the changes file itself causes a fault.
        //  If it does fault, at least the next restart will proceed without fault.
        m_pODNavObjectChangesSet->RemoveChangesFile();
        
        if(size != 0){
            wxString sLogMessage;
            sLogMessage.append( _("Applying changes from ") );
            sLogMessage.append( m_sODNavObjSetChangesFile );
            wxLogMessage( sLogMessage );
            m_pODNavObjectChangesSet->ApplyChanges();
        }
        
    }
}

bool ODConfig::LoadLayers(wxString &path)
{
    wxArrayString file_array;
    wxDir dir;
    Layer *l;
    dir.Open( path );
    if( dir.IsOpened() ) {
        wxString filename;
        bool cont = dir.GetFirst( &filename );
        while( cont ) {
            file_array.Clear();
            filename.Prepend( wxFileName::GetPathSeparator() );
            filename.Prepend( path );
            wxFileName f( filename );
            size_t nfiles = 0;
            if( f.GetExt().IsSameAs( wxT("gpx") ) )
                file_array.Add( filename); // single-gpx-file layer
                else{
                    if(wxDir::Exists( filename ) ){
                        wxDir dir( filename );
                        if( dir.IsOpened() ){
                            nfiles = dir.GetAllFiles( filename, &file_array, wxT("*.gpx") );      // layers subdirectory set
                        }
                    }
                }
                
                if( file_array.GetCount() ){
                    l = new Layer();
                    l->m_LayerID = ++g_LayerIdx;
                    l->m_LayerFileName = file_array[0];
                    if( file_array.GetCount() <= 1 )
                        wxFileName::SplitPath( file_array[0], NULL, NULL, &( l->m_LayerName ), NULL, NULL );
                    else
                        wxFileName::SplitPath( filename, NULL, NULL, &( l->m_LayerName ), NULL, NULL );
                    
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
                    
                    //  Load the entire file array as a single layer
                    
                    for( unsigned int i = 0; i < file_array.GetCount(); i++ ) {
                        wxString file_path = file_array[i];
                        
                        if( ::wxFileExists( file_path ) ) {
                            ODNavObjectChanges *pSet = new ODNavObjectChanges;
                            pSet->load_file(file_path.fn_str());
                            long nItems = pSet->LoadAllGPXObjectsAsLayer(l->m_LayerID, bLayerViz);
                            l->m_NoOfItems += nItems;
                            
                            wxString objmsg;
                            objmsg.Printf( wxT("Loaded GPX file %s with %ld items."), file_path.c_str(), nItems );
                            wxLogMessage( objmsg );
                            
                            delete pSet;
                        }
                    }
                }
                
                cont = dir.GetNext( &filename );
        }
    }
    
    // make sure any EBL hanging of the boat is repositioned
    wxEBLListNode *node = g_pEBLList->GetFirst();
    for(size_t i = 0; i < g_pEBLList->GetCount(); i++) {
        EBL *ebl = (EBL *)node->GetData();
        if(ebl->m_bCentreOnBoat)  {
            bool l_bSaveUpdatesState = ebl->m_bSaveUpdates;
            ebl->m_bSaveUpdates = false;
            ebl->CentreOnBoat(true);
            ebl->m_bSaveUpdates = l_bSaveUpdatesState;
        }
        node = node->GetNext();
    }
    
    return true;
}


void ODConfig::ExportGPX( wxWindow* parent, bool bviz_only, bool blayer )
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
            int answer = OCPNMessageBox_PlugIn( NULL, _("Overwrite existing file?"), _("Confirm"),
                    wxICON_QUESTION | wxYES_NO | wxCANCEL );
            if( answer != wxID_YES ) return;
        }

        ::wxBeginBusyCursor();

        ODNavObjectChanges *pgpx = new ODNavObjectChanges;

        wxProgressDialog *pprog = NULL;
        int count = g_pODPointMan->GetODPointList()->GetCount();
        if( count > 200) {
            pprog = new wxProgressDialog( _("Export GPX file"), _T("0/0"), count, NULL,
                                          wxPD_APP_MODAL | wxPD_SMOOTH |
                                          wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME );
            pprog->SetSize( 400, wxDefaultCoord );
            pprog->Centre();
        }

        //Points
        int ic = 0;
        // build a temporary O(1)
        std::map<ODPoint *, ODPoint *>   tp_hash;
        wxPathListNode *node1 = g_pPathList->GetFirst();
        while( node1 ) {
            ODPath *pPath = node1->GetData();
            ODPointList *pODPointList = pPath->m_pODPointList;
            wxODPointListNode *node2 = pODPointList->GetFirst();
            ODPoint *prp;
            while( node2 ) {
                prp = node2->GetData();
                tp_hash[prp] = prp;
                node2 = node2->GetNext();
            }
            node1 = node1->GetNext();
        }

        wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();
        ODPoint *pr;
        while( node ) {
            if(pprog && !(ic % 200)) {
                wxString msg;
                msg.Printf(_T("%d/%d"), ic, count);
                pprog->Update( ic, msg );
            }
            ic++;

            pr = node->GetData();

            bool b_add = true;

            if( bviz_only && !pr->m_bIsVisible )
                b_add = false;

            if( pr->m_bIsInLayer && !blayer )
                b_add = false;
            if( b_add) {
                if( pr->m_bKeepXPath || tp_hash.find( pr ) == tp_hash.end( ) )
                    pgpx->AddGPXODPoint( pr);
            }

            node = node->GetNext();
        }
        //Paths
        node1 = g_pPathList->GetFirst();
        while( node1 ) {
            ODPath *pPath = node1->GetData();

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

void ODConfig::UI_ImportGPX( wxWindow* parent, bool islayer, wxString dirpath, bool isdirectory )
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
            laymsg.Printf( _("New layer %d: %s"), l->m_LayerID, l->m_LayerName.c_str() );
            wxLogMessage( laymsg );

            pLayerList->Insert( l );
        }

        for( unsigned int i = 0; i < file_array.GetCount(); i++ ) {
            wxString path = file_array[i];

            if( wxFileExists( path ) ) {

                ODNavObjectChanges *pSet = new ODNavObjectChanges;
                pSet->load_file(path.fn_str());

                if(islayer){
                    l->m_NoOfItems += pSet->LoadAllGPXObjectsAsLayer(l->m_LayerID, l->m_bIsVisibleOnChart);
                }
                else
                    pSet->LoadAllGPXObjects( true );    // Import with full vizibility of names and objects

                delete pSet;
            }
        }

        // make sure any EBL hanging of the boat is repositioned
        wxEBLListNode *node = g_pEBLList->GetFirst();
        for(size_t i = 0; i < g_pEBLList->GetCount(); i++) {
            EBL *ebl = (EBL *)node->GetData();
            if(ebl->m_bCentreOnBoat)  {
                bool l_bSaveUpdatesState = ebl->m_bSaveUpdates;
                ebl->m_bSaveUpdates = false;
                ebl->CentreOnBoat(true);
                ebl->m_bSaveUpdates = l_bSaveUpdatesState;
            }
            node = node->GetNext();
        }
    }
}

void ODConfig::CreateRotatingNavObjBackup()
{
    //Rotate navobj backups, but just in case there are some changes in the current version
    //to prevent the user trying to "fix" the problem by continuously starting the
    //application to overwrite all of his good backups...
    if( g_navobjbackups > 0 ) {
        wxFile f;
        wxString oldname = m_sODNavObjSetFile;
        wxString newname = wxString::Format( _T("%s.1"), m_sODNavObjSetFile.c_str() );

        wxFileOffset s_diff = 1;
        if( wxFileExists( newname ) ) {

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
                oldname = wxString::Format( _T("%s.%d"), m_sODNavObjSetFile.c_str(), i );
                newname = wxString::Format( _T("%s.%d"), m_sODNavObjSetFile.c_str(), i + 1 );
                if( wxFile::Exists( oldname ) )
                    wxCopyFile( oldname, newname );
            }

            if( wxFile::Exists( m_sODNavObjSetFile ) )
            {
                newname = wxString::Format( _T("%s.1"), m_sODNavObjSetFile.c_str() );
                wxCopyFile( m_sODNavObjSetFile, newname );
            }
        }
    }
    //try to clean the backups the user doesn't want - breaks if he deleted some by hand as it tries to be effective...
    for( int i = g_navobjbackups + 1; i <= 99; i++ )
        if( wxFile::Exists( wxString::Format( _T("%s.%d"), m_sODNavObjSetFile.c_str(), i ) ) ) 
            ::wxRemoveFile( wxString::Format( _T("%s.%d"), m_sODNavObjSetFile.c_str(), i ) );
        else
            break;
}

bool ODConfig::ODPointIsInPathList( ODPoint *pr )
{
    bool IsInList = false;

    wxPathListNode *node1 = g_pPathList->GetFirst();
    while( node1 ) {
        ODPath *pPath = node1->GetData();
        ODPointList *pODPointList = pPath->m_pODPointList;

        wxODPointListNode *node2 = pODPointList->GetFirst();
        ODPoint *prp;

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

