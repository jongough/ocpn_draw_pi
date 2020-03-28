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
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/clrpicker.h>
#include <wx/radiobox.h>
#include <wx/textctrl.h>
#include <wx/bmpcbox.h>
#include <wx/statline.h>
#include <wx/grid.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

#include "wxWTranslateCatalog.h"

///////////////////////////////////////////////////////////////////////////


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
		wxCheckBox* m_checkBoxRememberPropertyPosition;
		wxCheckBox* m_checkBoxShowMagBearings;
		wxCheckBox* m_checkBoxAllowLeftDrag;
		wxCheckBox* m_checkBoxShowLayers;
		wxStaticText* m_staticTextNavObjBackups;
		wxSpinCtrl* m_spinCtrlNavObjBackups;
		wxStaticText* m_staticTextEdgePan;
		wxSlider* m_sliderEdgePan;
		wxStaticText* m_staticTextInitialEdgePan;
		wxSlider* m_sliderInitialEdgePan;
		wxStaticText* m_staticTextToolbar;
		wxStaticText* m_staticTextConfigFileEntriesMsg;
		wxButton* m_buttonConfigFileEntries;
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
		wxCheckBox* m_checkBoxBoundaryODPointsVisible;
		wxPanel* m_panelBoundaryPoint;
		wxStaticText* m_staticTextArrivalRadius;
		wxTextCtrl* m_textCtrlODPointArrivalRadius;
		wxBoxSizer* m_SizerNameIcon;
		wxFlexGridSizer* m_fgSizerODPointIcon;
		wxCheckBox* m_checkBoxBoundaryPointShowName;
		wxStaticText* m_staticTextBoundaryPointIcon;
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
		wxCheckBox* m_checkBoxTextPointShowName;
		wxFlexGridSizer* m_fgSizerTextPointIconName;
		wxStaticText* m_staticTextTextPointIconName;
		wxBitmapComboBox* m_bcomboBoxTextPointIconName;
		wxStaticText* m_staticTextPosition;
		wxChoice* m_choiceTextPosition;
		wxStaticText* m_staticTextColour;
		wxColourPickerCtrl* m_colourPickerTextColour;
		wxStaticText* m_staticTextBackgroundColour;
		wxColourPickerCtrl* m_colourPickerBackgroundColour;
		wxStaticText* m_staticTextBackgroundTransparency;
		wxSlider* m_sliderBackgroundTransparency;
		wxRadioBox* m_radioBoxWidthType;
		wxStaticText* m_staticTextMaxWidth;
		wxTextCtrl* m_textCtrlTextMaxWidth;
		wxFlexGridSizer* m_fgSizerTextPointFont;
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
		wxFlexGridSizer* m_fgSizerEBLSettings;
		wxStaticText* m_staticTextStartPointIcon;
		wxFlexGridSizer* m_fgSizerEBLStartIconName;
		wxBitmapComboBox* m_bcomboBoxEBLStartIconName;
		wxStaticText* m_staticTextEndPointIcon;
		wxFlexGridSizer* m_fgSizerEBLEndPointIcon;
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
		wxCheckBox* m_checkBoxShowVRM;
		wxCheckBox* m_checkBoxShowPerpLine;
		wxCheckBox* m_checkBoxEBLAllwaysShowInfo;
		wxRadioBox* m_radioBoxEBLPersistence;
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
		wxFlexGridSizer* m_fgSizerDREndPointIcon;
		wxStaticText* m_staticTextDRPointIcon;
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
		wxPanel* m_panelGZ;
		wxFlexGridSizer* m_fgSizerGZIcons;
		wxFlexGridSizer* m_fgSizerGZFASIcon;
		wxStaticText* m_staticTextGZFirstIcon;
		wxBitmapComboBox* m_bcomboBoxGZFirstIconName;
		wxStaticText* m_staticTextGZSecondPointIcon;
		wxBitmapComboBox* m_bcomboBoxGZSecondIconName;
		wxStaticText* m_staticTextActiveGZLineColour;
		wxColourPickerCtrl* m_colourPickerActiveGZLineColour;
		wxStaticText* m_staticTextActiveGZFillColour;
		wxColourPickerCtrl* m_colourPickerActiveGZFillColour;
		wxStaticText* m_staticTextInactiveGZLineColour;
		wxColourPickerCtrl* m_colourPickerInActiveGZLineColour;
		wxStaticText* m_staticTextInactiveGZFillColour;
		wxColourPickerCtrl* m_colourPickerInActiveGZFillColour;
		wxStaticText* m_staticTextGZLineWidth;
		wxChoice* m_choiceGZLineWidth;
		wxStaticText* m_staticTextGZLineStyle;
		wxChoice* m_choiceGZLineStyle;
		wxStaticText* m_staticTextGZFillTransparency;
		wxSlider* m_sliderGZFillTransparency;
		wxCheckBox* m_checkBoxGZRotateWithBoat;
		wxRadioBox* m_radioBoxGZMaintainWith;
		wxRadioBox* m_radioBoxGZLineType;
		wxRadioBox* m_radioBoxGZPersistence;
		wxPanel* m_panelPIL;
		wxFlexGridSizer* m_fgSizerPILSettings;
		wxStaticText* m_staticTextPILStartPointIcon;
		wxFlexGridSizer* m_fgSizerPILStartIconName;
		wxBitmapComboBox* m_bcomboBoxPILStartIconName;
		wxStaticText* m_staticTextPILEndPointIcon;
		wxFlexGridSizer* m_fgSizerPILEndPointIcon;
		wxBitmapComboBox* m_bcomboBoxPILEndIconName;
		wxStaticText* m_staticTextNumIndexLines;
		wxChoice* m_choiceNumIndexLines;
		wxStaticText* m_staticTextPILDefaultOffset;
		wxTextCtrl* m_textCtrlPILDefaultOffset;
		wxStaticText* m_staticTextPILActiveCentreLineColour;
		wxColourPickerCtrl* m_colourPickerPILActiveCentreLineColour;
		wxStaticText* m_staticTextInactiveCentreLineColour;
		wxColourPickerCtrl* m_colourPickerPILInActiveCentreLineColour;
		wxStaticText* m_staticTextPILActiveOffsetLine1Colour;
		wxColourPickerCtrl* m_colourPickerPILActiveOffsetLine1Colour;
		wxStaticText* m_staticTextPILInactiveOffsetLine1Colour;
		wxColourPickerCtrl* m_colourPickerPILInActiveOffsetLine1Colour;
		wxStaticText* m_staticTextPILActiveOffsetLine2Colour;
		wxColourPickerCtrl* m_colourPickerPILActiveOffsetLine2Colour;
		wxStaticText* m_staticTextPILInactiveOffsetLine2Colour;
		wxColourPickerCtrl* m_colourPickerPILInactiveOffsetLine2Colour;
		wxStaticText* m_staticTextPILCentreLineWidth;
		wxChoice* m_choicePILCentreLineWidth;
		wxStaticText* m_staticTextPILCentreLineStyle;
		wxChoice* m_choicePILCentreLineStyle;
		wxStaticText* m_staticTextPILOffsetLine1Width;
		wxChoice* m_choicePILOffsetLine1Width;
		wxStaticText* m_staticTextPILOffsetLine1Style;
		wxChoice* m_choicePILOffsetLine1Style;
		wxStaticText* m_staticTextPILOffsetLine2Width;
		wxChoice* m_choicePILOffsetLine2Width;
		wxStaticText* m_staticTextPILOffsetLine2Style;
		wxChoice* m_choicePILOffsetLine2Style;
		wxRadioBox* m_radioBoxPILPersistence;
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
		wxStaticText* m_staticTextAPIVersion;
		wxStaticText* m_staticTextAPIVersionMajor;
		wxStaticText* m_staticTextAPIVersionMajorVal;
		wxStaticText* m_staticTextAPIVersionMinor;
		wxStaticText* m_staticTextAPIVersionMinorVal;
		wxStaticText* m_staticTextOCPNVersionMajor;
		wxStaticText* m_staticTextOCPNAPIVersionMajor;
		wxStaticText* m_staticTextOCPNAPIVersionMajorVal;
		wxStaticText* m_staticTextOCPNAPIVersionMinor;
		wxStaticText* m_staticTextOCPNAPIVersionMinorVal;
		wxStaticText* m_staticTextDate;
		wxStaticText* m_staticTextDateVal;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticTextPluginDirectories;
		wxStaticText* m_staticTextData;
		wxStaticText* m_staticTextDataDirectory;
		wxStaticText* m_staticTextIcons;
		wxStaticText* m_staticTextIconDirectory;
		wxStaticText* m_staticTextLayers;
		wxStaticText* m_staticTextLayersDirectory;
		wxStaticLine* m_staticline11;
		wxStaticText* m_staticTextOther;
		wxStaticText* m_staticText34;
		wxPanel* m_panelHelp;
		wxStaticText* m_staticTextHelp;
		wxPanel* m_panelLicense;
		wxTextCtrl* m_textCtrlLicense;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		wxButton* m_buttonApply;

		// Virtual event handlers, overide them in your derived class
		virtual void OnClickConfigFileEntries( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickFonts( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEBLRotateWithBoat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEBLFixedEndPosition( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPILIndexLineChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDrawPropertiesOKClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDrawPropertiesCancelClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDrawPropertiesApplyClick( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxChoice* m_choiceToolbar;
		wxGrid* m_gridODWDInteractions;

		ODPropertiesDialogDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("OCPN Draw Properties"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 591,607 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
		~ODPropertiesDialogDef();

};

