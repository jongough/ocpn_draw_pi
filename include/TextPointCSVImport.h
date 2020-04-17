/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPN Draw Import Boundary Point support
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
 
#ifndef TEXTPOINTCSVIMPORT_H
#define TEXTPOINTCSVIMPORT_H

class wxStringTokenizer;

class TextPointCSVImport 
{
    public:
        TextPointCSVImport();
        TextPointCSVImport(wxStringTokenizer *TextPointCSV);
        virtual ~TextPointCSVImport();

        wxString    m_sName;
        double      m_dLat;
        double      m_dLon;
        int         m_iTextPosition;
        int         m_iDisplayTextWhen;
        wxString    m_TextPointText;
        bool        m_bExclusion;
        bool        m_bInclusion;
        bool        m_bVisible;
        bool        m_bIsInBoundary;
        bool        m_bIsInPath;
        bool        m_bRangeRingsVisible;
        int         m_iNumRings;
        double      m_dStep;
        int         m_iUnits;
        wxColour    m_RingColour;
    protected:
    private:

};

#endif // TEXTPOINTCSVIMPORT_H
