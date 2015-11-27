/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OCPN Draw Utility functions support
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
 
#include "ODUtils.h"
#include "ocpn_plugin.h"
#include "FontMgr.h"

/*!
 * Helper stuff for calculating Path
 */

#define    pi        (4.*atan(1.0))
#define    tpi        (2.*pi)
#define    twopi    (2.*pi)
#define    degs    (180./pi)
#define    rads    (pi/180.)


// This function parses a string containing a GPX time representation
// and returns a wxDateTime containing the UTC corresponding to the
// input. The function return value is a pointer past the last valid
// character parsed (if successful) or NULL (if the string is invalid).
//
// Valid GPX time strings are in ISO 8601 format as follows:
//
//   [-]<YYYY>-<MM>-<DD>T<hh>:<mm>:<ss>Z|(+|-<hh>:<mm>)
//
// For example, 2010-10-30T14:34:56Z and 2010-10-30T14:34:56-04:00
// are the same time. The first is UTC and the second is EDT.

const wxChar *ParseGPXDateTime( wxDateTime &dt, const wxChar *datetime )
{
    long sign, hrs_west, mins_west;
    const wxChar *end;

    // Skip any leading whitespace
    while( isspace( *datetime ) )
        datetime++;

    // Skip (and ignore) leading hyphen
    if( *datetime == wxT('-') ) datetime++;

    // Parse and validate ISO 8601 date/time string
    if( ( end = dt.ParseFormat( datetime, wxT("%Y-%m-%dT%T") ) ) != NULL ) {

        // Invalid date/time
        if( *end == 0 ) return NULL;

        // ParseFormat outputs in UTC if the controlling
        // wxDateTime class instance has not been initialized.

        // Date/time followed by UTC time zone flag, so we are done
        else
            if( *end == wxT('Z') ) {
                end++;
                return end;
            }

            // Date/time followed by given number of hrs/mins west of UTC
            else
                if( *end == wxT('+') || *end == wxT('-') ) {

                    // Save direction from UTC
                    if( *end == wxT('+') ) sign = 1;
                    else
                        sign = -1;
                    end++;

                    // Parse hrs west of UTC
                    if( isdigit( *end ) && isdigit( *( end + 1 ) ) && *( end + 2 ) == wxT(':') ) {

                        // Extract and validate hrs west of UTC
                        wxString( end ).ToLong( &hrs_west );
                        if( hrs_west > 12 ) return NULL;
                        end += 3;

                        // Parse mins west of UTC
                        if( isdigit( *end ) && isdigit( *( end + 1 ) ) ) {

                            // Extract and validate mins west of UTC
                            wxChar mins[3];
                            mins[0] = *end;
                            mins[1] = *( end + 1 );
                            mins[2] = 0;
                            wxString( mins ).ToLong( &mins_west );
                            if( mins_west > 59 ) return NULL;

                            // Apply correction
                            dt -= sign * wxTimeSpan( hrs_west, mins_west, 0, 0 );
                            return end + 2;
                        } else
                            // Missing mins digits
                            return NULL;
                    } else
                        // Missing hrs digits or colon
                        return NULL;
                } else
                    // Unknown field after date/time (not UTC, not hrs/mins
                    //  west of UTC)
                    return NULL;
    } else
        // Invalid ISO 8601 date/time
        return NULL;
}

// RFC4122 version 4 compliant random UUIDs generator.
wxString GetUUID(void)
{
    wxString str;
    struct {
        int time_low;
        int time_mid;
        int time_hi_and_version;
        int clock_seq_hi_and_rsv;
        int clock_seq_low;
        int node_hi;
        int node_low;
    } uuid;
    
    uuid.time_low = GetRandomNumber(0, 2147483647);//FIXME: the max should be set to something like MAXINT32, but it doesn't compile un gcc...
    uuid.time_mid = GetRandomNumber(0, 65535);
    uuid.time_hi_and_version = GetRandomNumber(0, 65535);
    uuid.clock_seq_hi_and_rsv = GetRandomNumber(0, 255);
    uuid.clock_seq_low = GetRandomNumber(0, 255);
    uuid.node_hi = GetRandomNumber(0, 65535);
    uuid.node_low = GetRandomNumber(0, 2147483647);
    
    /* Set the two most significant bits (bits 6 and 7) of the
     * clock_seq_hi_and_rsv to zero and one, respectively. */
    uuid.clock_seq_hi_and_rsv = (uuid.clock_seq_hi_and_rsv & 0x3F) | 0x80;
    
    /* Set the four most significant bits (bits 12 through 15) of the
     * time_hi_and_version field to 4 */
    uuid.time_hi_and_version = (uuid.time_hi_and_version & 0x0fff) | 0x4000;
    
    str.Printf(_T("%08x-%04x-%04x-%02x%02x-%04x%08x"),
               uuid.time_low,
               uuid.time_mid,
               uuid.time_hi_and_version,
               uuid.clock_seq_hi_and_rsv,
               uuid.clock_seq_low,
               uuid.node_hi,
               uuid.node_low);
    
    return str;
}

int GetRandomNumber(int range_min, int range_max)
{
    long u = (long)wxRound(((double)rand() / ((double)(RAND_MAX) + 1) * (range_max - range_min)) + range_min);
    return (int)u;
}

//-------------------------------------------------------------------------------
//          Popup Menu Handling
//-------------------------------------------------------------------------------

void MenuPrepend( wxMenu *menu, int id, wxString label)
{
    wxMenuItem *item = new wxMenuItem(menu, id, label);
#ifdef __WXMSW__
//    wxFont *qFont = OCPNGetFont( _("Menu"), 0 );
    wxFont *qFont = OCPNGetScaledFont_PlugIn(_("Menu"));
    item->SetFont(*qFont);
#endif
#ifdef __WXQT__
    wxFont sFont = GetOCPNGUIScaledFont_PlugIn(_T("Menu"));
    item->SetFont(sFont);
#endif
    
    menu->Prepend(item);
}

void MenuAppend( wxMenu *menu, int id, wxString label)
{
    wxMenuItem *item = new wxMenuItem(menu, id, label);
#ifdef __WXMSW__
//    wxFont *qFont = OCPNGetFont(wxS("Menu"), 0);
    wxFont *qFont = OCPNGetScaledFont_PlugIn(_("Menu"));
    item->SetFont(*qFont);
#endif
#ifdef __WXQT__
    wxFont sFont = GetOCPNGUIScaledFont_PlugIn(_T("Menu"));
    item->SetFont(sFont);
#endif
    menu->Append(item);
}


double sign( double x )
{
    if( x < 0. ) return -1.;
    else
        return 1.;
}

double FNipart( double x )
{
    return ( sign( x ) * (int) ( fabs( x ) ) );
}

double FNday( int y, int m, int d, int h )
{
    long fd = ( 367 * y - 7 * ( y + ( m + 9 ) / 12 ) / 4 + 275 * m / 9 + d );
    return ( (double) fd - 730531.5 + h / 24. );
}

double FNrange( double x )
{
    double b = x / tpi;
    double a = tpi * ( b - FNipart( b ) );
    if( a < 0. ) a = tpi + a;
    return ( a );
}

double getLMT( double ut, double lon )
{
    double t = ut + lon / 15.;
    if( t >= 0. ) if( t <= 24. ) return ( t );
    else
        return ( t - 24. );
    else
        return ( t + 24. );
}

