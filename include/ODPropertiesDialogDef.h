///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun 17 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ODPROPERTIESDIALOGDEF_H__
#define __ODPROPERTIESDIALOGDEF_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/clrpicker.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
#include <wx/bmpcbox.h>
#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyPanel1
///////////////////////////////////////////////////////////////////////////////
class MyPanel1 : public wxPanel 
{
	private:
	
	protected:
	
	public:
		
		MyPanel1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL ); 
		~MyPanel1();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ODPropertiesDialogDef
///////////////////////////////////////////////////////////////////////////////
class ODPropertiesDialogDef : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_SizerProperties;
		wxNotebook* m_notebookProperties;
		wxPanel* m_panelGeneral;
		wxCheckBox* m_checkBoxConfirmObjectDelete;
		wxCheckBox* m_checkBoxShowMagBearings;
		wxStaticText* m_staticTextNavObjBackups;
		wxSpinCtrl* m_spinCtrlNavObjBackups;
		wxStaticText* m_staticTextEdgePan;
		wxSlider* m_sliderEdgePan;
		wxStaticText* m_staticTextInitialEdgePan;
		wxSlider* m_sliderInitialEdgePan;
		wxStaticText* m_staticTextToolbar;
		wxPanel* m_panelBoundary;
		wxStaticText* m_staticTextActiveBoundayLineColour;
		wxColourPickerCtrl* m_colourPickerActiveBoundaryLineColour;
		wxStaticText* m_staticTextActiveBoundaryFillColour;
		wxColourPickerCtrl* m_colourPickerActiveBoundaryFillColour;
		wxStaticText* m_staticTextInactiveBoundaryLineColour;
		wxColourPickerCtrl* m_colourPickerInActiveBoundaryLineColour;
		wxStaticText* m_staticTextInactiveBoundaryFillColour;
		wxColourPickerCtrl* m_colourPickerInActiveBoundaryFillColour;
		wxStaticText* m_staticTextBoundaryLineWidth;
		wxChoice* m_choiceBoundaryLineWidth;
		wxStaticText* m_staticTextBoundaryLineStyle;
		wxChoice* m_choiceBoundaryLineStyle;
		wxStaticText* m_staticTextFillTransparency;
		wxSlider* m_sliderFillTransparency;
		wxStaticText* m_staticTextInclusionBoundarySize;
		wxSlider* m_sliderInclusionBoundarySize;
		wxRadioBox* m_radioBoxBoundaryType;
		wxPanel* m_panelBoundaryPoint;
		wxStaticText* m_staticTextArrivalRadius;
		wxTextCtrl* m_textCtrlODPointArrivalRadius;
		wxBoxSizer* m_SizerNameIcon;
		wxCheckBox* m_checkBoxShowName;
		wxStaticText* m_staticTextIcon;
		wxStaticBitmap* m_bitmapPointBitmap;
		wxBitmapComboBox* m_bcomboBoxODPointIconName;
		wxCheckBox* m_checkBoxShowODPointRangeRings;
		wxRadioBox* m_radioBoxBoundaryPointType;
		wxFlexGridSizer* m_fgSizerBoundaryPointFillDensity;
		wxStaticText* m_staticTextBoundaryPointFillDensity;
		wxSlider* m_sliderBoundaryPointFillTransparency;
		wxFlexGridSizer* m_fgSizerBoundaryPointSize;
		wxStaticText* m_staticTextBoundaryPointSize;
		wxSlider* m_sliderInclusionBoundaryPointSize;
		wxStaticText* m_staticTextRangeRingNumber;
		wxChoice* m_choiceODPointRangeRingNumber;
		wxStaticText* m_staticTextRangeRingSteps;
		wxTextCtrl* m_textCtrlODPointRangeRingSteps;
		wxStaticText* m_staticTextDistanceUnit;
		wxChoice* m_choiceODPointDistanceUnit;
		wxStaticText* m_staticTextRangeRingColours;
		wxColourPickerCtrl* m_colourPickerODPointRangeRingColours;
		wxStaticText* m_staticTextRangeRingWidth;
		wxChoice* m_choiceRangeRingWidth;
		wxStaticText* m_staticTextRangeRingStyle;
		wxChoice* m_choiceRangeRingStyle;
		wxPanel* m_panelTextPoint;
		wxBoxSizer* m_SizerTextPointIconName;
		wxStaticText* m_staticTextTextPointIconName;
		wxStaticBitmap* m_bitmapTextPointBitmap;
		wxBitmapComboBox* m_bcomboBoxTextPointIconName;
		wxStaticText* m_staticTextPosition;
		wxChoice* m_choiceTextPosition;
		wxStaticText* m_staticTextColour;
		wxColourPickerCtrl* m_colourPickerTextColour;
		wxStaticText* m_staticTextBackgroundColour;
		wxColourPickerCtrl* m_colourPickerBackgroundColour;
		wxStaticText* m_staticTextBackgroundTransparency;
		wxSlider* m_sliderBackgroundTransparency;
		wxStaticText* m_staticTextTextFont;
		wxStaticText* m_staticTextFontFaceExample;
		wxButton* m_buttonTextFont;
		wxRadioBox* m_radioBoxShowDisplayText;
		wxPanel* m_panelPath;
		wxStaticText* m_staticTextActivePathLineColour;
		wxColourPickerCtrl* m_colourPickerActivePathLineColour;
		wxStaticText* m_staticTextInactivePathLineColour;
		wxColourPickerCtrl* m_colourPickerInActivePathLineColour;
		wxStaticText* m_staticTextPathLineWidth;
		wxChoice* m_choicePathLineWidth;
		wxStaticText* m_staticTextPathLineStyle;
		wxChoice* m_choicePathLineStyle;
		wxPanel* m_panelEBL;
		wxStaticText* m_staticTextStartPointIcon;
		wxStaticBitmap* m_bitmapEBLStartBitmap;
		wxBitmapComboBox* m_bcomboBoxEBLStartIconName;
		wxStaticText* m_staticTextEndPointIcon;
		wxStaticBitmap* m_bitmapEBLEndBitmap;
		wxBitmapComboBox* m_bcomboBoxEBLEndIconName;
		wxStaticText* m_staticTextActiveEBLLineColour;
		wxColourPickerCtrl* m_colourPickerEBLLineColour;
		wxStaticText* m_staticTextInactiveEBLLineColour;
		wxColourPickerCtrl* m_colourPickerInActiveEBLLineColour;
		wxStaticText* m_staticTextEBLLineWidth;
		wxChoice* m_choiceEBLLineWidth;
		wxStaticText* m_staticTextEBLLineStyle;
		wxChoice* m_choiceEBLLineStyle;
		wxCheckBox* m_checkBoxRotateWithBoat;
		wxRadioBox* m_radioBoxMaintainWith;
		wxCheckBox* m_checkBoxEBLFixedEndPosition;
		wxCheckBox* m_checkBoxEBLShowArrow;
		wxRadioBox* m_radioBoxEBLPersistence;
		wxCheckBox* m_checkBoxShowVRM;
		wxPanel* m_panelDR;
		wxStaticText* m_staticTextActiveDRLineColour;
		wxColourPickerCtrl* m_colourPickerDRLineColour;
		wxStaticText* m_staticTextInactiveDRLineColour;
		wxColourPickerCtrl* m_colourPickerInActiveDRLineColour;
		wxStaticText* m_staticTextDRLineWidth;
		wxChoice* m_choiceDRLineWidth;
		wxStaticText* m_staticTextDRLineStyle;
		wxChoice* m_choiceDRLineStyle;
		wxStaticText* m_staticTextDefaultSoG;
		wxTextCtrl* m_textCtrlSOG;
		wxStaticText* m_staticTextCOG;
		wxTextCtrl* m_textCtrlCOG;
		wxStaticText* m_staticTextDRLength;
		wxTextCtrl* m_textCtrlDRPathLength;
		wxStaticText* m_staticTextDRPointInterval;
		wxTextCtrl* m_textCtrlDRPointInterval;
		wxRadioBox* m_radioBoxDRLengthType;
		wxRadioBox* m_radioBoxDRIntervalType;
		wxRadioBox* m_radioBoxDRDistanceUnits;
		wxRadioBox* m_radioBoxDRTimeUnits;
		wxCheckBox* m_checkBoxDRShowArrow;
		wxRadioBox* m_radioBoxDRPersistence;
		wxPanel* m_panelDRPoint;
		wxStaticText* m_staticTextDRPointIcon;
		wxStaticBitmap* m_bitmapDRPointBitmap;
		wxBitmapComboBox* m_bcomboBoxDRPointIconName;
		wxCheckBox* m_checkBoxShowDRPointRangeRings;
		wxStaticText* m_staticTextDRPointRangeRingNumber;
		wxChoice* m_choiceDRPointRangeRingNumber;
		wxStaticText* m_staticTextDRPointRangeRingSteps;
		wxTextCtrl* m_textCtrlDRPointRangeRingSteps;
		wxStaticText* m_staticTextDRPointDistanceUnit;
		wxChoice* m_choiceDRPointDistanceUnit;
		wxStaticText* m_staticTextDRPointRangeRingColours;
		wxColourPickerCtrl* m_colourPickerDRPointRangeRingColours;
		wxStaticText* m_staticTextDRPointRangeRingWidth;
		wxChoice* m_choiceDRPointRangeRingWidth;
		wxStaticText* m_staticTextDRPointRangeRingStyle;
		wxChoice* m_choiceDRPointRangeRingStyle;
		wxPanel* m_panelAbout;
		wxStaticText* m_staticTextName;
		wxStaticText* m_staticTextNameVal;
		wxStaticText* m_staticTextVersion;
		wxStaticText* m_staticTextMajor;
		wxStaticText* m_staticTextMajorVal;
		wxStaticText* m_staticTextMinor;
		wxStaticText* m_staticTextMinorVal;
		wxStaticText* m_staticTextPatcvh;
		wxStaticText* m_staticTextPatchVal;
		wxStaticText* m_staticTextDate;
		wxStaticText* m_staticTextDateVal;
		wxStaticText* m_staticTextOther;
		wxStaticText* m_staticText34;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		wxButton* m_buttonApply;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnODPointComboboxSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextPointIconComboboxSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickFonts( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEBLStartIconComboboxSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEBLEndIconComboboxSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEBLRotateWithBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEBLFixedEndPosition( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDRPointIconComboboxSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDrawPropertiesOKClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDrawPropertiesCancelClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDrawPropertiesApplyClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxChoice* m_choiceToolbar;
		
		ODPropertiesDialogDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("OCPN Draw Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 591,554 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~ODPropertiesDialogDef();
	
};

#endif //__ODPROPERTIESDIALOGDEF_H__
