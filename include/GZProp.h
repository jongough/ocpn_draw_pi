/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  ODPath Properties
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

#ifndef GZPROP_H
#define GZPROP_H

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "ODPathPropertiesDialogImpl.h"

class GZ;

class GZProp : public ODPathPropertiesDialogImpl
{
    public:
        GZProp();
        GZProp( wxWindow* parent, wxWindowID id = SYMBOL_PATHPROP_IDNAME, const wxString& caption = SYMBOL_PATHPROP_TITLE, const wxPoint& pos = SYMBOL_PATHPROP_POSITION,
                        const wxSize& size = SYMBOL_PATHPROP_SIZE, long style = SYMBOL_PATHPROP_STYLE );
        virtual ~GZProp();

        void OnRotateWithBoat( wxCommandEvent& event );
        void OnSetFocus( wxFocusEvent& event );
        void OnKillFocus( wxFocusEvent& event );
        using ODPathPropertiesDialogImpl::UpdateProperties;
        bool UpdateProperties( ODPath *pInGZ );
        bool UpdateProperties( void );
        bool SaveChanges( void );
        
        void OnOK( wxCommandEvent& event );
        void OnClose( wxCloseEvent& event );
        void OnCancel( wxCommandEvent& event );
        void OnChoiceLineWidth( wxCommandEvent& event );
        void OnChoiceLineStyle( wxCommandEvent& event );
        void OnSetFocusChoiceLineWidth( wxFocusEvent& event );
        void OnKillFocusChoiceLineWidth( wxFocusEvent& event );
        void OnSetFocusChoiceLineStyle( wxFocusEvent& event );
        void OnKillFocusChoiceLineStyle( wxFocusEvent& event );
        
    protected:
        
    private:
        double  m_dODGZFirstAngleValidator;
        double  m_dODGZSecondAngleValidator;
        double  m_dODGZFirstLengthValidator;
        double  m_dODGZSecondLengthValidator;
        bool    m_bLockGZAngle;
        bool    m_bLockGZLength;
        bool    m_bSetLocale;
        bool    m_bLockUpdate;
        
};

#endif // EBLPROP_H
