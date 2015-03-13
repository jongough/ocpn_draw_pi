/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPNPoint dialog box  Support
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
#ifndef OCPNDRAWPOINTINFOIMPL_H
#define OCPNDRAWPOINTINFOIMPL_H

#include <routeprop.h>
#include "OCPNPoint.h"


class OCPNDrawPointInfoImpl : public MarkInfoImpl
{
    public:
        OCPNDrawPointInfoImpl( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("OCPN Draw Point Information"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1, -1 ), long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER );
        //OCPNDrawPointInfoImpl();
        virtual ~OCPNDrawPointInfoImpl();
        void SetOCPNPoint( OCPNPoint *pOP );
        bool UpdateProperties( bool positionOnly = false );
    protected:
    private:
        void OnHyperLinkClick(wxHyperlinkEvent &event);

        OCPNPoint  *m_pOCPNPoint;
        HyperlinkList *m_pMyLinkList;
        wxHyperlinkCtrl* m_pEditedLink;

        int           m_current_icon_Index;
        double        m_lat_save;
        double        m_lon_save;
        wxString      m_IconName_save;
        bool          m_bShowName_save;
        bool          m_bIsVisible_save;

};

#endif // OCPNDRAWPOINTINFOIMPL_H
