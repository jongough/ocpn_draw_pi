/***************************************************************************
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

#ifndef PIL_H
#define PIL_H

#include "EBL.h"

struct PILLINE {
    int         iID;
    wxString    sName;
    wxString    sDescription;
    double      dOffset;
    wxColour    wxcActiveColour;
    wxColour    wxcInActiveColour;
    double      dStyle;
    double      dWidth;
};


class PIL :  public EBL
{
    public:
        PIL();
        virtual ~PIL();
        int     AddLine(wxString sName, wxString sDescription, double dOffset);
        void    AddLine(PILLINE PilLine);
        void    DelLine(int iID);
        void    ChangeOffset(int iID, double dOffset);
        void    Draw( ODDC& dc, PlugIn_ViewPort &VP );
        void    DrawGL( PlugIn_ViewPort &piVP );
        void    DrawSegment(ODDC& dc, wxPoint *rp1, wxPoint *rp2, PlugIn_ViewPort &VP, bool bdraw_arrow);
        void    CentreOnBoat( bool bMoveEndPoint );
        void    MovePILLine(double dLat, double dLon, int iPILId);

        std::list<PILLINE> PilLineList;
        
    protected:

    private:
        void    RenderPIL( ODDC &dc, PlugIn_ViewPort &piVP );
        void    CalcOffsetPoints( wxPoint Centreppt, wxPoint *FirstPoint, wxPoint *SecondPoint );
        int     m_iStyleOffsetLine;
        int     m_iWidthOffsetLine;
};

WX_DECLARE_LIST(PIL, PILList); // establish class PIL list member

#endif // PIL_H
