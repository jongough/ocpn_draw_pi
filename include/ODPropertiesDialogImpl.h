/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  OCPN Draw Properties Dialog support
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

#ifndef __ODPropertiesDialogImpl__
#define __ODPropertiesDialogImpl__

/**
@file
Subclass of ODPropertiesForm, which is generated by wxFormBuilder.
*/

#include "ODPropertiesDialogDef.h"
#include <wx/bmpcbox.h>

// Forward declare class
class wxFontDialog;
class ODIconCombo;


/** Implementing ODPropertiesForm */
class ODPropertiesDialogImpl : public ODPropertiesDialogDef
{
protected:
    // Handlers for ODPropertiesForm events.
    void OnClickConfigFileEntries( wxCommandEvent& event );
    void OnODPointComboboxSelected( wxCommandEvent& event );
    void OnTextPointIconComboboxSelected( wxCommandEvent& event );
    void OnEBLEndIconComboboxSelected( wxCommandEvent& event );
    void OnEBLStartIconComboboxSelected( wxCommandEvent& event );
    void OnEBLRotateWithBoat( wxCommandEvent& event );
    void OnEBLFixedEndPosition( wxCommandEvent& event );
    void OnDRPointIconComboboxSelected( wxCommandEvent& event );
    void OnGZFirstIconComboboxSelected( wxCommandEvent& event );
    void OnGZSecondIconComboboxSelected( wxCommandEvent& event );
    void OnButtonClickFonts( wxCommandEvent& event );
    void OnDrawPropertiesOKClick( wxCommandEvent& event );
    void OnDrawPropertiesCancelClick( wxCommandEvent& event );
    void OnDrawPropertiesApplyClick( wxCommandEvent& event );
    void OnPILIndexLineChoice( wxCommandEvent& event );
    void OnShowLayersClicked( wxCommandEvent& event );
    void OnRestoreLayerVisabilityClicked( wxCommandEvent& event );
    
    void SaveChanges();
    void applyStyle( void );

    void RecalculateSize( void );
    
    wxSize              m_defaultClientSize;
    ODIconCombo         *m_bODIComboBoxODPointIconName;
    ODIconCombo         *m_bODIComboBoxTextPointIconName;
    ODIconCombo         *m_bODIComboBoxEBLStartIconName;
    ODIconCombo         *m_bODIComboBoxEBLEndIconName;
    ODIconCombo         *m_bODIComboBoxDRPointIconName;
    ODIconCombo         *m_bODIComboBoxGZFirstIconName;
    ODIconCombo         *m_bODIComboBoxGZSecondIconName;
    ODIconCombo         *m_bODIComboBoxPILStartIconName;
    ODIconCombo         *m_bODIComboBoxPILEndIconName;
    
public:
    /** Constructor */
    ODPropertiesDialogImpl( wxWindow* parent );
    void UpdateProperties( void );
    void SetDialogSize( void );
    wxFontDialog  *m_pfdDialog;

    
private:
    void        SetTableCellBackgroundColours(void);
    void        SetIndexLineChoice( bool choice );
    
    double      m_dODPointRangeRingStepValidator;
    double      m_dSOGValidator;
    double      m_dODPointArrivalRadiusValidator;
    double      m_dDRPathLengthValidator;
    double      m_dODPointIntervalValidator;
    double      m_dDRPointRangRingStepValidator;
    double      m_dPILOffsetValidator;
    int         m_iCOGValidator;
    int         m_iTextPointTextMaxWidth;
    int         m_iTextPointTextMaxWidthType;
    wxColour    m_cRed;
    wxColour    m_cGreen;
    int         m_CharWidth;
    int         m_CharHeight;
    wxFont      *m_dialogLabelFont;

};

#endif // __ODPropertiesDialogImpl__
