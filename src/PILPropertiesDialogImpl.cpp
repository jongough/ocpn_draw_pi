/***************************************************************************
*
* Project:  OpenCPN
* Purpose:  OCPN Draw Parallel Index Line Properties Dialog support
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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers


#include "PILPropertiesImpl.h"
#include "ocpn_draw_pi.h"
#include "ocpn_plugin.h"
#include <wx/clipbrd.h>
#include <wx/menu.h>
#include <wx/window.h>
#include <wx/fontdlg.h>

#if wxCHECK_VERSION(3,0,0)
#include <wx/valnum.h>
#endif

extern ocpn_draw_pi         *g_ocpn_draw_pi;
extern ODConfig             *g_pODConfig;


PILPropertiesImpl::PILPropertiesImpl( wxWindow* parent )
:
PILPropertiesDialog( parent )
{
#if wxCHECK_VERSION(3,0,0)
    wxDialog::SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
#endif // wxCHECK_VERSION(3,0,0)

    DimeWindow( this );

#if wxCHECK_VERSION(3,0,0)
    SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);
#endif // wxCHECK_VERSION(3,0,0)

    this->GetSizer()->Fit( this );
    this->Layout();


}

ODPointPropertiesImpl::~ODPointPropertiesImpl()
{
}

