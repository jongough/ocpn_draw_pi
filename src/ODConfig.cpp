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
#include "ODLayer.h"
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

#ifdef OD_USE_CXX11
#include <unordered_map>
#else
#include <map>
#endif

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

void ODConfig::LateInit(void)
{
    m_sImport_Path = g_sDefaultImportPath;
    m_sImport_Type = g_sDefaultImportType;
}

void ODConfig::DeInit(void)
{
    g_sDefaultImportPath = m_sImport_Path;
    g_sDefaultImportType = m_sImport_Type;
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
    wxFileDialog saveDialog( NULL, _( "Export GPX file" ), m_sGPX_Path, suggestedName,
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
    m_sGPX_Path = fn.GetPath();

    if( response == wxID_OK ) {
        fn.SetExt( _T ( "gpx" ) );

        if( wxFileExists( fn.GetFullPath() ) ) {
            int answer = OCPNMessageBox_PlugIn( parent, _("Overwrite existing file?"), _("Confirm"),
                    wxICON_QUESTION | wxYES_NO | wxCANCEL | wxSTAY_ON_TOP );
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
    wxFileDialog saveDialog( NULL, _( "Export GPX file" ), m_sGPX_Path, suggestedName,
            wxT ( "GPX files (*.gpx)|*.gpx" ), wxFD_SAVE );

    int response = saveDialog.ShowModal();

    wxString path = saveDialog.GetPath();
    wxFileName fn( path );
    m_sGPX_Path = fn.GetPath();

    if( response == wxID_OK ) {
        fn.SetExt( _T ( "gpx" ) );

        if( wxFileExists( fn.GetFullPath() ) ) {
            int answer = OCPNMessageBox_PlugIn( parent,  _("Overwrite existing file?"), _("Confirm"),
                    wxICON_QUESTION | wxYES_NO | wxCANCEL | wxSTAY_ON_TOP );
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
    ODLayer *l;
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
                    l = new ODLayer();
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
                    
                    l->m_bIsVisible = bLayerViz;
                    
                    wxString laymsg;
                    laymsg.Printf( wxT("New layer %d: %s"), l->m_LayerID, l->m_LayerName.c_str() );
                    wxLogMessage( laymsg );
                    
                    g_pLayerList->Insert( l );
                    
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
    wxFileDialog saveDialog( NULL, _( "Export GPX file" ), m_sGPX_Path, wxT ( "" ),
            wxT ( "GPX files (*.gpx)|*.gpx" ), wxFD_SAVE );

    int response = saveDialog.ShowModal();

    wxString path = saveDialog.GetPath();
    wxFileName fn( path );
    m_sGPX_Path = fn.GetPath();

    if( response == wxID_OK ) {
        fn.SetExt( _T ( "gpx" ) );

        if( wxFileExists( fn.GetFullPath() ) ) {
            int answer = OCPNMessageBox_PlugIn( parent, _("Overwrite existing file?"), _("Confirm"),
                    wxICON_QUESTION | wxYES_NO | wxCANCEL | wxSTAY_ON_TOP );
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

        wxPathListNode *node1 = g_pPathList->GetFirst();
//#ifdef COMPILER_SUPPORTS_CXX11
#ifdef OD_USE_CXX11
        //build an unordered map list for high speed access
        std::unordered_map<ODPoint *, ODPoint *> tp_hash;
        tp_hash.reserve(count);
#else        
        std::map<ODPoint *, ODPoint *> tp_hash;
#endif        
        while (node1) {
            ODPath *pPath = node1->GetData();
            ODPointList *pODPointList = pPath->m_pODPointList;
            wxODPointListNode *node2 = pODPointList->GetFirst();
            ODPoint *pop;
            while (node2) {
                pop = node2->GetData();
                tp_hash[pop] = pop;
                node2 = node2->GetNext();
            }
            node1 = node1->GetNext();
        }

        wxODPointListNode *node = g_pODPointMan->GetODPointList()->GetFirst();
        ODPoint *pr;
        time_t l_tStart = time(0);
        time_t l_tCurrent;
        while( node ) {
            l_tCurrent = time(0);
            if(pprog && l_tStart != l_tCurrent) {
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
                if( pr->m_bKeepXPath || tp_hash.find( pr ) == tp_hash.end() )
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
    ODLayer *l = NULL;

    if( !islayer || dirpath.IsSameAs( _T("") ) ) {
        wxString l_ext_list = "";
        wxFileDialog openDialog( NULL, _( "Import GPX file" ), m_sImport_Path, wxT ( "" ),
                wxT ( "GPX files (*.gpx)|*.gpx|CSV files (*.csv)|*.csv|All files (*.*)|*.*" ),
                wxFD_OPEN | wxFD_MULTIPLE );
        openDialog.Centre();
        response = openDialog.ShowModal();
        if( response == wxID_OK ) {
            openDialog.GetPaths( file_array );

            //    Record the currently selected directory for later use
            if( file_array.GetCount() ) {
                wxFileName fn( file_array[0] );
                m_sGPX_Path = fn.GetPath();
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
            l = new ODLayer();
            l->m_LayerID = ++g_LayerIdx;
            l->m_LayerFileName = file_array[0];
            if( file_array.GetCount() <= 1 ) wxFileName::SplitPath( file_array[0], NULL, NULL,
                    &( l->m_LayerName ), NULL, NULL );
            else {
                if( dirpath.IsSameAs( _T("") ) ) wxFileName::SplitPath( m_sGPX_Path, NULL, NULL,
                        &( l->m_LayerName ), NULL, NULL );
                else
                    wxFileName::SplitPath( dirpath, NULL, NULL, &( l->m_LayerName ), NULL, NULL );
            }

            bool bLayerViz = g_bShowLayers;
            if( g_VisibleLayers.Contains( l->m_LayerName ) )
                bLayerViz = true;
            if( g_InvisibleLayers.Contains( l->m_LayerName ) )
                bLayerViz = false;
            l->m_bIsVisible = bLayerViz;

            wxString laymsg;
            laymsg.Printf( _("New layer %d: %s"), l->m_LayerID, l->m_LayerName.c_str() );
            wxLogMessage( laymsg );

            g_pLayerList->Insert( l );
        }

        for( unsigned int i = 0; i < file_array.GetCount(); i++ ) {
            wxString path = file_array[i];

            if( wxFileExists( path ) ) {

                ODNavObjectChanges *pSet = new ODNavObjectChanges;
                if(m_sImport_Type == _T("gpx")) {
                    pSet->load_file(path.fn_str());
                    if(islayer){
                        l->m_NoOfItems += pSet->LoadAllGPXObjectsAsLayer(l->m_LayerID, l->m_bIsVisible);
                    }
                    else
                        pSet->LoadAllGPXObjects( true );    // Import with full vizibility of names and objects
                } else {
                    if(islayer) {
                        l->m_NoOfItems += pSet->Load_CSV_File(path.fn_str(), l->m_LayerID, l->m_bIsVisible);
                    } else {
                        pSet->Load_CSV_File(path.fn_str());
                        
                    }
                }

                delete pSet;
            }
        }

        // make sure any EBL hanging off the boat is repositioned
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

void ODConfig::UI_Import( wxWindow* parent, bool islayer, bool isTemporary, wxString dirpath, bool isdirectory )
{
    int response = wxID_CANCEL;
    wxArrayString file_array;
    ODLayer *l = NULL;
    
    if(islayer && !isTemporary) {
        m_sWildcardString = _T("GPX files (*.gpx)|*.gpx|All files (*.*)|*.*");
    } else {
        if(m_sImport_Type == _T("csv")) {
            m_sWildcardString = _T("CSV files (*.csv)|*.csv|GPX files (*.gpx)|*.gpx|All files (*.*)|*.*");
        } else if(m_sImport_Type == _T("gpx")) {
            m_sWildcardString = _T("GPX files (*.gpx)|*.gpx|CSV files (*.csv)|*.csv|All files (*.*)|*.*");
        } else if(m_sImport_Type == _T("*")) {
            m_sWildcardString = _T("All files (*.*)|*.*|GPX files (*.gpx)|*.gpx|CSV files (*.csv)|*.csv");
        } else {
            m_sWildcardString = _T("GPX files (*.gpx)|*.gpx|CSV files (*.csv)|*.csv|All files (*.*)|*.*");
        }
    }
    wxFileDialog openDialog( NULL, _( "Import file" ), m_sImport_Path, _T(""), m_sWildcardString,
                                wxFD_OPEN | wxFD_MULTIPLE );
    openDialog.Centre();
    response = openDialog.ShowModal();
    if( response == wxID_OK ) {
        openDialog.GetPaths( file_array );
        
        //    Record the currently selected directory for later use
        if( file_array.GetCount() ) {
            wxFileName fn( file_array[0] );
            m_sImport_Path = fn.GetPath();
            m_sImport_Type = fn.GetExt();
        }
        
    } else { 
        if(dirpath.IsEmpty())
            response = wxID_CANCEL;
        else {
            if( isdirectory ) {
                if( wxDir::GetAllFiles( dirpath, &file_array, wxT("*.gpx") ) )
                    response = wxID_OK;
            } else {
                file_array.Add( dirpath );
                response = wxID_OK;
            }
        }
    }
    
    if( response == wxID_OK ) {
        
        if( islayer ) {
            l = new ODLayer();
            l->m_LayerID = ++g_LayerIdx;
            l->m_LayerFileName = file_array[0];
            if( file_array.GetCount() <= 1 ) wxFileName::SplitPath( file_array[0], NULL, NULL,
                &( l->m_LayerName ), NULL, NULL );
            else {
                if( dirpath.IsSameAs( _T("") ) ) wxFileName::SplitPath( m_sGPX_Path, NULL, NULL,
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
            
            g_pLayerList->Insert( l );
        }
        for( unsigned int i = 0; i < file_array.GetCount(); i++ ) {
            wxString path = file_array[i];
            wxFileName l_fn(file_array[i]);
            
            if( wxFileExists( path ) ) {
                
                ODNavObjectChanges *pSet = new ODNavObjectChanges;
                if(l_fn.GetExt() == _T("csv")) {
                    if(l) {
                        l->m_NoOfItems += pSet->Load_CSV_File(path.fn_str(), l->m_LayerID, l->m_bIsVisibleOnChart);
                    } else {
                        pSet->Load_CSV_File(path.fn_str(), 0, true);
                    }
                }else if(l_fn.GetExt() == _T("gpx")) {
                    pSet->load_file(path.fn_str());
                    if(islayer){
                        l->m_NoOfItems += pSet->LoadAllGPXObjectsAsLayer(l->m_LayerID, l->m_bIsVisibleOnChart);
                    }
                    else
                        pSet->LoadAllGPXObjects( true );    // Import with full vizibility of names and objects
                }
                
                    
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

