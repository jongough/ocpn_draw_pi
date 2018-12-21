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
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/clrpicker.h>
#include <wx/choice.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include "wxWTranslateCatalog.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PILPropertiesDialogDef
///////////////////////////////////////////////////////////////////////////////
class PILPropertiesDialogDef : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticTexrID;
		wxTextCtrl* m_textCtrlIDNum;
		wxStaticText* m_staticTextName;
		wxTextCtrl* m_textCtrlName;
		wxStaticText* m_staticTextDesctiption;
		wxTextCtrl* m_textCtrlDesctription;
		wxStaticText* m_staticTextOffset;
		wxTextCtrl* m_textCtrlOffset;
		wxCheckBox* m_checkBoxActive;
		wxStaticText* m_staticTextLineColour;
		wxColourPickerCtrl* m_colourPickerLineColour;
		wxStaticText* m_staticTextLineStyle;
		wxChoice* m_choiceLineStyle;
		wxStaticText* m_staticTextLineWidth;
		wxChoice* m_choiceLineWidth;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnColourChangedLineColour( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnChoiceLineStyle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoiceLineWidth( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }


	public:

		PILPropertiesDialogDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Index Line Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 512,301 ), long style = wxDEFAULT_DIALOG_STYLE );
		~PILPropertiesDialogDef();

};

