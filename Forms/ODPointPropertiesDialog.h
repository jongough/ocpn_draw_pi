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
#include <wx/bmpcbox.h>
#include <wx/radiobox.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/hyperlink.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/scrolwin.h>
#include <wx/button.h>
#include <wx/tglbtn.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

#include "wxWTranslateCatalog.h"

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ODPointPropertiesDialog
///////////////////////////////////////////////////////////////////////////////
class ODPointPropertiesDialog : public wxDialog
{
	private:

	protected:
		wxBoxSizer* m_SizerDialogBox;
		wxNotebook* m_notebookProperties;
		wxScrolledWindow* m_scrolledWindowBasicProperties;
		wxBoxSizer* m_SizerBasicProperties;
		wxStaticText* m_staticText2;
		wxBoxSizer* m_SizerOuterProperties;
		wxStaticText* m_staticTextName;
		wxTextCtrl* m_textName;
		wxBoxSizer* m_SizerNameIcon;
		wxFlexGridSizer* m_fgSizerNameIcon;
		wxCheckBox* m_checkBoxShowName;
		wxBitmapComboBox* m_bcomboBoxODPointIconName;
		wxBoxSizer* m_bSizerChangeAllPointIcons;
		wxCheckBox* m_checkBoxChangeAllPointIcons;
		wxStaticText* m_staticTextLatitude;
		wxTextCtrl* m_textLatitude;
		wxStaticText* m_staticTextLogitude;
		wxTextCtrl* m_textLongitude;
		wxBoxSizer* m_bSizerEBLPointWarning;
		wxStaticText* m_staticTextEBLPointWarning;
		wxStaticText* m_staticTextArrivalRadius;
		wxTextCtrl* m_textCtrlODPointArrivalRadius;
		wxBoxSizer* m_bSizerRangeRingsControl;
		wxCheckBox* m_checkBoxShowODPointRangeRings;
		wxRadioBox* m_radioBoxBoundaryPointType;
		wxBoxSizer* m_SizerFill;
		wxStaticText* m_staticTextFillDensity;
		wxSlider* m_sliderBoundaryPointFillTransparency;
		wxStaticText* m_staticTextBoundaryPointInclusionSize;
		wxSlider* m_sliderBoundaryPointInclusionSize;
		wxBoxSizer* m_SizerODPointRangeRingsSelect;
		wxStaticText* m_staticTextPointRangeRings;
		wxChoice* m_choicePointRangeRingsNumber;
		wxFlexGridSizer* m_SizerPointRangeGrid;
		wxStaticText* m_staticTextPointRangeRingSteps;
		wxTextCtrl* m_textCtrlODPointRangeRingsSteps;
		wxStaticText* m_staticTextDistanceUnit;
		wxChoice* m_choiceDistanceUnitsString;
		wxStaticText* m_staticTextRangeRingColours;
		wxColourPickerCtrl* m_colourPickerRangeRingsColour;
		wxStaticText* m_staticTextRangeRingWidth;
		wxChoice* m_choiceRangeRingLineWidth;
		wxStaticText* m_staticTextRangeRingStyle;
		wxChoice* m_choiceRangeRingLineStyle;
		wxBoxSizer* m_SizerDescription;
		wxStaticText* m_staticTextDescription;
		wxBoxSizer* m_SizerDescriptionExt;
		wxTextCtrl* m_textDescription;
		wxBoxSizer* m_bSizerHyperlink;
		wxStaticText* m_staticTextHyperlink;
		wxBoxSizer* m_bSizerHyperlinkExt;
		wxScrolledWindow* m_scrolledWindowLinks;
		wxBoxSizer* m_bSizerLinks;
		wxHyperlinkCtrl* m_hyperlinkCtrl;
		wxMenu* m_menuLink;
		wxMenuItem* m_menuItemDelete;
		wxMenuItem* m_menuItemEdit;
		wxMenuItem* m_menuItemAddNew;
		wxStaticText* m_staticTextLinkInfo;
		wxBoxSizer* m_bSizerLinkButtons;
		wxButton* m_buttonAddLink;
		wxToggleButton* m_toggleBtnEditLink;
		wxToggleButton* m_toggleBtnDeleteLink;
		wxPanel* m_panelDisplayText;
		wxBoxSizer* m_SizerDisplayText;
		wxStaticText* m_staticTextDisplayText;
		wxTextCtrl* m_textDisplayText;
		wxFlexGridSizer* m_fgSizerTextProperties;
		wxStaticText* m_staticTextPosition;
		wxChoice* m_choicePosition;
		wxStaticText* m_staticTextColour;
		wxColourPickerCtrl* m_colourPickerText;
		wxStaticText* m_staticTextBackgroundColour;
		wxColourPickerCtrl* m_colourPickerBacgroundColour;
		wxStaticText* m_staticTextBackgroundTransparency;
		wxSlider* m_sliderBackgroundTransparency;
		wxRadioBox* m_radioBoxWidthType;
		wxStaticText* m_staticTextMaxWidth;
		wxTextCtrl* m_textCtrlTextMaxWidth;
		wxStaticText* m_staticTextFont;
		wxStaticText* m_staticTextFontFaceExample;
		wxButton* m_buttonTextFont;
		wxRadioBox* m_radioBoxShowDisplayText;
		wxPanel* m_panelExtended;
		wxCheckBox* m_checkBoxVisible;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrlGuid;
		wxBoxSizer* m_SizerButtons;
		wxButton* m_OK;
		wxButton* m_Cancel;

		// Virtual event handlers, overide them in your derived class
		virtual void OnPointPropertiesClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnPositionCtlUpdated( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowRangeRingsSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRadioBoxPointType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDescChangedBasic( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddLink( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditLinkToggle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteLinkToggle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickFonts( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPointPropertiesOKClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPointPropertiesCancelClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		ODPointPropertiesDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("OD Point Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~ODPointPropertiesDialog();

		void m_scrolledWindowLinksOnContextMenu( wxMouseEvent &event )
		{
			m_scrolledWindowLinks->PopupMenu( m_menuLink, event.GetPosition() );
		}

};

