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
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/choice.h>
#include <wx/slider.h>
#include <wx/radiobox.h>
#include <wx/listctrl.h>
#include <wx/scrolwin.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include "wxWTranslateCatalog.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class ODPathPropertiesDialogDef
///////////////////////////////////////////////////////////////////////////////
class ODPathPropertiesDialogDef : public wxDialog
{
	private:

	protected:
		wxBoxSizer* m_bSizerDialogBox;
		wxScrolledWindow* m_scrolledWindowProperties;
		wxFlexGridSizer* m_fgSizerProperties;
		wxBoxSizer* m_bSizerNameDescription;
		wxStaticText* m_staticTextName;
		wxTextCtrl* m_textCtrlName;
		wxStaticText* m_staticTextDescription;
		wxTextCtrl* m_textCtrlDesctiption;
		wxStaticText* m_staticTextGUID;
		wxTextCtrl* m_textCtrlGUID;
		wxFlexGridSizer* m_fgSizerBoundary;
		wxCheckBox* m_checkBoxActive;
		wxStaticText* m_staticTextTotalLength;
		wxTextCtrl* m_textCtrlTotalLength;
		wxStaticText* m_staticTextDistUntis;
		wxStaticText* m_staticTextLineColour;
		wxColourPickerCtrl* m_colourPickerLineColour;
		wxStaticText* m_staticTextLineStyle;
		wxChoice* m_choiceLineStyle;
		wxStaticText* m_staticTextLineWidth;
		wxChoice* m_choiceLineWidth;
		wxStaticText* m_staticTextFillColour;
		wxColourPickerCtrl* m_colourPickerFillColour;
		wxStaticText* m_staticTextFillTransparency;
		wxSlider* m_sliderFillTransparency;
		wxStaticText* m_staticTextIncluseionBoundarySize;
		wxSlider* m_sliderInclusionBoundarySize;
		wxBoxSizer* m_bSizerBoundaryType;
		wxRadioBox* m_radioBoxBoundaryType;
		wxCheckBox* m_checkBoxShowBoundaryPoints;
		wxFlexGridSizer* m_fgSizerPIL;
		wxStaticText* m_staticTextPILAngle;
		wxTextCtrl* m_textCtrlPILAngle;
		wxFlexGridSizer* m_fgSizerEBL;
		wxCheckBox* m_checkBoxRotateWithBoat;
		wxRadioBox* m_radioBoxMaintainWith;
		wxStaticText* m_staticTextEBLAngle;
		wxTextCtrl* m_textCtrlEBLAngle;
		wxFlexGridSizer* m_fgSizerEBL1;
		wxCheckBox* m_checkBoxEBLFixedEndPosition;
		wxCheckBox* m_checkBoxShowVRM;
		wxCheckBox* m_checkBoxShowEBLInfo;
		wxCheckBox* m_checkBoxShowPerpLine;
		wxFlexGridSizer* m_fgSizerGZ;
		wxCheckBox* m_checkBoxRotateGZWithBoat;
		wxRadioBox* m_radioBoxMaintainGZWith;
		wxRadioBox* m_radioBoxGZLineType;
		wxStaticText* m_staticTextGZFirstAngle;
		wxTextCtrl* m_textCtrlGZFirstAngle;
		wxStaticText* m_staticTextGZFirstLength;
		wxTextCtrl* m_textCtrlGZFirstLength;
		wxStaticText* m_staticTextGZSecondAngle;
		wxTextCtrl* m_textCtrlGZSecondAngle;
		wxStaticText* m_staticTextGZSecondLength;
		wxTextCtrl* m_textCtrlGZSecondLength;
		wxFlexGridSizer* m_fgSizerPath;
		wxCheckBox* m_checkBoxPathShowArrow;
		wxRadioBox* m_radioBoxPathPersistence;
		wxFlexGridSizer* m_fgSizerPathPoints;
		wxStaticText* m_staticTextODPoints;
		wxBoxSizer* m_bSizerListCtrl;
		wxListCtrl* m_listCtrlODPoints;
		wxBoxSizer* m_bSizerPILLines;
		wxStaticText* m_staticTextPILLines;
		wxListCtrl* m_listCtrlPILList;
		wxBoxSizer* m_SizerOKCancel;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnKillFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnSetFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnColourChangedLineColour( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnChoiceLineStyle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnKillFocusChoiceLineStyle( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnSetFocusChoiceLineStyle( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnChoiceLineWidth( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnKillFocusChoiceLineWidth( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnSetFocusChoiceLineWidth( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnRadioBoxBoundaryType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRotateWithBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFixedEndPosition( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPILCheckbox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLeftDoubleClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnLeftDoubleClickPIL( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnRightClickPIL( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }


	public:

		ODPathPropertiesDialogDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Path Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~ODPathPropertiesDialogDef();

};

