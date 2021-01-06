///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
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
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include "extra_formbuilder_headers.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ODDRDialogDef
///////////////////////////////////////////////////////////////////////////////
class ODDRDialogDef : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticTextSOG;
		wxTextCtrl* m_textCtrlSOG;
		wxStaticText* m_staticTextCOG;
		wxTextCtrl* m_textCtrlCOG;
		wxStaticText* m_staticTextLength;
		wxTextCtrl* m_textCtrlLength;
		wxStaticText* m_staticTextDRPointInterval;
		wxTextCtrl* m_textCtrlDRPointInterval;
		wxRadioBox* m_radioBoxLengthType;
		wxRadioBox* m_radioBoxIntervalType;
		wxRadioBox* m_radioBoxDistanceUnits;
		wxRadioBox* m_radioBoxTimeUnits;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }


	public:

		ODDRDialogDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Dead Reckoning"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~ODDRDialogDef();

};

