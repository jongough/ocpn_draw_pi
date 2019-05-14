/***************************************************************************
 * 
 * Project:  OpenCPN
 * Purpose:  Text points 
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

#ifndef TEXTPOINT_H
#define TEXTPOINT_H

#include "ODPoint.h"

class ODText;

class TextPoint : public ODPoint
{
    public:
        TextPoint();
        TextPoint(double lat, double lon, const wxString& icon_ident, const wxString& name, const wxString &pGUID = wxT("") , bool bAddToList = true);
        TextPoint(const TextPoint& other);
        ~TextPoint();
        
        void Draw(ODDC& dc, wxPoint *odp= NULL );
        void DrawGL( PlugIn_ViewPort &pivp );
        void SetPointText( wxString sTextPointText );
        wxString GetPointText( void ) { return m_TextPointText; }
        void SetColourScheme( PI_ColorScheme cs = PI_GLOBAL_COLOR_SCHEME_RGB);
        void CreateColourSchemes(void);

        wxString        m_TextPointText;
        int             m_iTextPosition;
        int             m_iWrapLen;
        int             m_iTextMaxWidthType;
        int             m_iBackgroundTransparency;
        wxColour        m_colourTextColour;
        wxColour        m_colourTextBackgroundColour;
        wxFont          *m_pDisplayTextFont;
        wxFont          m_DisplayTextFont;
        int             m_DisplayTextFontPointSize;
        bool            m_bTextChanged;
        double          m_natural_scale;
        bool            m_bShowDisplayTextOnRollover;
        int             m_iDisplayTextWhen;
        
    protected:
        wxColour        m_colourSchemeTextColour;
        wxColour        m_colourSchemeTextBackgroundColour;
        wxColour        m_colourTextColourRBG;
        wxColour        m_colourTextBackgroundColourRGB;
        wxColour        m_colourTextColourDay;
        wxColour        m_colourTextBackgroundColourDay;
        wxColour        m_colourTextColourDusk;
        wxColour        m_colourTextBackgroundColourDusk;
        wxColour        m_colourTextColourNight;
        wxColour        m_colourTextBackgroundColourNight;

    private:
        void CalculateTextExtents( void );
        void CalculateTextExtents( wxString TextPointText );
        
        wxStaticText    *m_pstText;
        wxTextCtrl      *m_ptcText;
        wxSize          m_TextExtents;
        int             m_TextLocationOffsetX;
        int             m_TextLocationOffsetY;
        
        unsigned int    m_iDisplayTextTexture;
        int             m_iDisplayTextTextureWidth, m_iDisplayTextTextureHeight;
        wxWindow        *m_Window;
        double          m_scale_factor;
        
};

#endif // TEXTPOINT_H
