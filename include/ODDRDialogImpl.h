/***************************************************************************
 * Project:  OpenCPN
 * Purpose:  DR
 * Author:   Jon Gough
 *
 * Project:  OpenCPN
 *
 ***************************************************************************
 *   Copyright (C) 2013 by David S. Register                               *
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

#ifndef __ODDRDialogImpl__
#define __ODDRDialogImpl__

/**
@file
Subclass of MyDialogDR, which is generated by wxFormBuilder.
*/

#include "ODDRDialogDef.h"
#include "DR.h"

//// end generated include

/** Implementing ODDRDialogImpl */
class ODDRDialogImpl : public ODDRDialogDef
{
	protected:
		// Handlers for MyDialogDR events.
		void OnOK( wxCommandEvent& event );
		void OnCancel( wxCommandEvent& event );
        
	public:
		/** Constructor */
		ODDRDialogImpl( wxWindow* parent );
        //// end generated class members
        
        void SetupDialog();
        void UpdateDialog( DR *dr );
        
private:
        double  m_dValidator;
        double  m_dSOGValidator;
        double  m_dLengthValidator;
        double  m_dIntervalValidator;
        int     m_iValidator;
        int     m_iCOGValidator;
        wxFont *m_dialogLabelFont;

        
        DR      *m_pDR;
        
};

#endif // __ODDRDialogImpl__
