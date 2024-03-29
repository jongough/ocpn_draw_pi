///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/filepicker.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>

#include "extra_formbuilder_headers.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ODLinkPropertiesDialogDef
///////////////////////////////////////////////////////////////////////////////
class ODLinkPropertiesDialogDef : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticTextLinkDescription;
		wxTextCtrl* m_textCtrlLinkDescription;
		wxStaticText* m_staticTextURL;
		wxTextCtrl* m_textCtrlURL;
		wxFilePickerCtrl* m_filePickerLocalFile;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;

		// Virtual event handlers, override them in your derived class
		virtual void OnFileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnOKClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancelClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		ODLinkPropertiesDialogDef();
		ODLinkPropertiesDialogDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 189,299 ), long style = wxDEFAULT_DIALOG_STYLE );
		bool Create( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 189,299 ), long style = wxDEFAULT_DIALOG_STYLE );

		~ODLinkPropertiesDialogDef();

};

