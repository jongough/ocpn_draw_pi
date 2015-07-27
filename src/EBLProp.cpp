/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Path Properties
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

#include "EBLProp.h"
#include "EBL.h"

extern EBLList         *g_pEBLList;
extern wxColour             g_colourActivePathFillColour;
extern wxColour             g_colourInActivePathFillColour;
extern unsigned int         g_uiFillTransparency;


EBLProp::EBLProp()
{
    //ctor
}

EBLProp::EBLProp( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style ) 
: ODPathPropertiesDialogImpl( parent, id, caption, pos, size, style )
{
    //ctor
    m_staticTextFillColour->Hide();
    m_staticTextFillColour->Enable( false );
    m_colourPickerFillColour->Hide();
    m_colourPickerFillColour->Enable( false );
    m_staticTextFillTransparency->Hide();
    m_staticTextFillTransparency->Enable( false );
    m_sliderFillTransparency->Hide();
    m_sliderFillTransparency->Enable( false );
    
}


EBLProp::~EBLProp()
{
    //dtor
}

bool EBLProp::UpdateProperties( EBL *pInEBL )
{
    m_staticTextFillColour->Hide();
    m_staticTextFillColour->Enable( false );
    m_colourPickerFillColour->Hide();
    m_colourPickerFillColour->Enable( false );
    m_staticTextFillTransparency->Hide();
    m_staticTextFillTransparency->Enable( false );
    m_sliderFillTransparency->Hide();
    m_sliderFillTransparency->Enable( false );

    return ODPathPropertiesDialogImpl::UpdateProperties( pInEBL );
}
