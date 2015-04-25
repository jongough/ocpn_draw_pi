/******************************************************************************
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
 ***************************************************************************
 */

#include "wx/wxprec.h"

#include <wx/bitmap.h>
#include <wx/dcmemory.h>
#include <wx/dcscreen.h>

#include "ocpn_draw_pi.h"
#include "ocpn_plugin.h"
#include "ODRolloverWin.h"
#include "ocpndc.h"
#include "timers.h"
#include "chart1.h"
//#include "navutil.h"
#include "FontMgr.h"

extern ocpn_draw_pi     *g_ocpn_draw_pi;

BEGIN_EVENT_TABLE(ODRolloverWin, wxWindow)
    EVT_PAINT(ODRolloverWin::OnPaint)
    EVT_TIMER(ROLLOVER_TIMER, ODRolloverWin::OnTimer)
    EVT_MOUSE_EVENTS ( ODRolloverWin::OnMouseEvent )

END_EVENT_TABLE()

// Define a constructor
ODRolloverWin::ODRolloverWin( wxWindow *parent, int timeout ) :
    wxWindow( parent, wxID_ANY, wxPoint( 0, 0 ), wxSize( 1, 1 ), wxNO_BORDER )
{
    m_pbm = NULL;
    m_parent = parent;
    m_timer_timeout.SetOwner( this, ROLLOVER_TIMER );
    m_timeout_sec = timeout;
    m_mmouse_propogate = 0;
    isActive = false;
    m_plabelFont = NULL;
    Hide();
}

ODRolloverWin::~ODRolloverWin()
{
    delete m_pbm;
}
void ODRolloverWin::OnTimer( wxTimerEvent& event )
{
    if( IsShown() ) Hide();
}

void ODRolloverWin::OnMouseEvent( wxMouseEvent& event )
{
    //    If directed, send mouse events up the window family tree,
    //    until some parent window does NOT call event.Skip()
    if( m_mmouse_propogate ) {
        event.ResumePropagation( m_mmouse_propogate );
        event.Skip();
    }
}

void ODRolloverWin::SetBitmap( int rollover )
{
    wxDC* cdc = new wxScreenDC();
//    wxPoint canvasPos = GetParent()->GetScreenPosition();
    wxPoint canvasPos = g_ocpn_draw_pi->m_parent_window->GetScreenPosition();
    
    wxMemoryDC mdc;
    delete m_pbm;
    m_pbm = new wxBitmap( m_size.x, m_size.y, -1 );
    mdc.SelectObject( *m_pbm );

    mdc.Blit( 0, 0, m_size.x, m_size.y, cdc, m_position.x + canvasPos.x,
              m_position.y + canvasPos.y );
    delete cdc;

    ocpnDC dc( mdc );

    switch( rollover ) {
        default:
        case PATH_ROLLOVER:
            wxColour wxCol;
            GetGlobalColor( wxT("YEL01"), &wxCol );
            g_ocpn_draw_pi->AlphaBlending( dc, 0, 0, m_size.x, m_size.y, 6.0, wxCol, 172 );
            mdc.SetTextForeground( FontMgr::Get().GetFontColor( _("PathLegInfoRollover") ) );
            break;
            
    }


    if(m_plabelFont && m_plabelFont->IsOk()) {

    //    Draw the text
        mdc.SetFont( *m_plabelFont );

        mdc.DrawLabel( m_string, wxRect( 0, 0, m_size.x, m_size.y ), wxALIGN_CENTRE_HORIZONTAL | wxALIGN_CENTRE_VERTICAL);
    }

    SetSize( m_position.x, m_position.y, m_size.x, m_size.y );   // Assumes a nominal 32 x 32 cursor

    // Retrigger the auto timeout
    if( m_timeout_sec > 0 ) m_timer_timeout.Start( m_timeout_sec * 1000, wxTIMER_ONE_SHOT );
}

void ODRolloverWin::OnPaint( wxPaintEvent& event )
{
    int width, height;
    GetClientSize( &width, &height );
    wxPaintDC dc( this );

    if( m_string.Len() ) {
        wxMemoryDC mdc;
        mdc.SelectObject( *m_pbm );
        dc.Blit( 0, 0, width, height, &mdc, 0, 0 );
    }
}

void ODRolloverWin::SetBestPosition( int x, int y, int off_x, int off_y, int rollover,
                                   wxSize parent_size )
{
    int h, w;

    wxFont *dFont;
    switch( rollover ) {

    default:
    case PATH_ROLLOVER:
        dFont = FontMgr::Get().GetFont( _("PathLegInfoRollover") );
        break;

    }

    int font_size = wxMax(8, dFont->GetPointSize());
    m_plabelFont = wxTheFontList->FindOrCreateFont( font_size, dFont->GetFamily(),
                         dFont->GetStyle(), dFont->GetWeight(), false, dFont->GetFaceName() );

    if(m_plabelFont && m_plabelFont->IsOk()) {
#ifdef __WXMAC__
        wxScreenDC sdc;
        sdc.GetMultiLineTextExtent(m_string, &w, &h, NULL, m_plabelFont);
#else
//        wxClientDC cdc( GetParent() );
        wxClientDC cdc( g_ocpn_draw_pi->m_parent_window );
        cdc.GetMultiLineTextExtent( m_string, &w, &h, NULL, m_plabelFont );
#endif
    }
    else {
        w = 10;
        h = 10;
    }

    m_size.x = w + 8;
    m_size.y = h + 8;

    int xp, yp;
    if( ( x + off_x + m_size.x ) > parent_size.x ) {
        xp = x - ( off_x / 2 ) - m_size.x;
        xp = wxMax(0, xp);
    } else
        xp = x + off_x;

    if( ( y + off_y + m_size.y ) > parent_size.y ) {
        yp = y - ( off_y / 2 ) - m_size.y;
    } else
        yp = y + off_y;

    SetPosition( wxPoint( xp, yp ) );

}

