///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 16 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ODPOINTPROPERTIESDIALOG_H__
#define __ODPOINTPROPERTIESDIALOG_H__

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
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
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
		wxPanel* m_panelBasicProperties;
		wxBoxSizer* m_SizerBasicProperties;
		wxStaticText* m_staticText2;
		wxBoxSizer* m_bSizerOuterProperties;
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
		wxStaticText* m_staticTextArrivalRadius;
		wxTextCtrl* m_textCtrlODPointArrivalRadius;
		wxBoxSizer* m_bSizerRangeRingsControl;
		wxCheckBox* m_checkBoxShowODPointRangeRings;
		wxRadioBox* m_radioBoxBoundaryPointType;
		wxBoxSizer* m_bSizerFill;
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
		wxStaticText* m_staticTextDescription;
		wxTextCtrl* m_textDescription;
		wxPanel* m_panelDisplayText;
		wxBoxSizer* m_bSizerDisplayText;
		wxStaticText* m_staticTextDisplayText;
		wxTextCtrl* m_textDisplayText;
		wxStaticText* m_staticTextPosition;
		wxChoice* m_choicePosition;
		wxStaticText* m_staticTextColour;
		wxColourPickerCtrl* m_colourPickerText;
		wxStaticText* m_staticTextBackgroundColour;
		wxColourPickerCtrl* m_colourPickerBacgroundColour;
		wxStaticText* m_staticTextBackgroundTransparency;
		wxSlider* m_sliderBackgroundTransparency;
		wxStaticText* m_staticTextFont;
		wxStaticText* m_staticTextFontFaceExample;
		wxButton* m_buttonTextFont;
		wxRadioBox* m_radioBoxShowDisplayText;
		wxPanel* m_panelExtended;
		wxCheckBox* m_checkBoxVisible;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrlGuid;
		wxButton* m_OK;
		wxButton* m_Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPointPropertiesClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnLeaveLatitude( wxFocusEvent& event ) { event.Skip(); }
		virtual void onRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnEnterLatitude( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnPositionCtlUpdated( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLeaveLongitude( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnRightClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnEnterLongitude( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnLeaveArrivalRadius( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnEnterArrivalRadius( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnArrivalRadiusChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowRangeRingsSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRadioBoxPointType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRangeRingsStepChange( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDescChangedBasic( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickFonts( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPointPropertiesOKClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPointPropertiesCancelClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ODPointPropertiesDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("OD Point Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ODPointPropertiesDialog();
	
};

#endif //__ODPOINTPROPERTIESDIALOG_H__
