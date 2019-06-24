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
#include <wx/textwrapper.h>
 
#include "ODUtils.h"
#include "ocpn_plugin.h"
#include "ocpn_draw_pi.h"

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
    wxFont *qFont = GetOCPNScaledFont_PlugIn(_T("Menu"), 0);
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
    wxFont *qFont = GetOCPNScaledFont_PlugIn(_T("Menu"));
    item->SetFont(*qFont);
#endif
#ifdef __WXQT__
    wxFont sFont = GetOCPNGUIScaledFont_PlugIn(_T("Menu"));
    item->SetFont(sFont);
#endif
    menu->Append(item);
}

// Helper functions
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

// International helper functions
void SetGlobalLocale( void )
{
#ifndef __WXMSW__
    if(g_iLocaleDepth == 0) { 
        g_ODlocale = new wxString(wxSetlocale(LC_NUMERIC, NULL));
#if wxCHECK_VERSION(3,0,0)        
        wxSetlocale(LC_NUMERIC, "");
#else
        setlocale(LC_NUMERIC, "");
#endif
    }
    g_iLocaleDepth++;
#endif
}

void ResetGlobalLocale( void )
{
#ifndef __WXMSW__
    g_iLocaleDepth--;
    if(g_iLocaleDepth < 0) 
        g_iLocaleDepth = 0;
    if(g_iLocaleDepth == 0 && g_ODlocale) {
#if wxCHECK_VERSION(3,0,0)        
        wxSetlocale(LC_NUMERIC, g_ODlocale->ToAscii());
#else
        setlocale(LC_NUMERIC, g_ODlocale->ToAscii());
#endif
        delete g_ODlocale;
        g_ODlocale = NULL;
    } 
#endif
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in the floats i_x and i_y.
bool GetLineIntersection(double p0_x, double p0_y, double p1_x, double p1_y, double p2_x, double p2_y, double p3_x, double p3_y, double* i_x, double* i_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;
    
    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);
    
    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x);
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y);
        return true;
    }
    
    return false; // No collision
}

//  Parameters for this function:
//
//  int    polyCorners  =  how many corners the polygon has
//  double  polyX[]      =  horizontal coordinates of corners
//  double  polyY[]      =  vertical coordinates of corners
//  double  x, y         =  point to be tested
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.

bool pointInPolygon(int polyCorners, double *polyX, double *polyY, double x, double y) 
{
    
    int   i, j=polyCorners-1 ;
    bool  oddNodes=false      ;
    
    for (i=0; i<polyCorners; i++) {
        if (((polyY[i]< y && polyY[j]>=y)
            ||   (polyY[j]< y && polyY[i]>=y))
            &&  (polyX[i]<=x || polyX[j]<=x)) {
            oddNodes^=(polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x); }
            j=i; 
        
    }
    
    return oddNodes; 
    
}

int ArcSectorPoints( wxPoint *&points, wxCoord xc, wxCoord yc, wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord x3, wxCoord y3, wxCoord x4, wxCoord y4, bool bHighQuality )
{
    double y1yc, x1xc, y4yc, x4xc;
    y1yc = y1-yc;
    x1xc = x1-xc;
    y4yc = y4-yc;
    x4xc = x4-xc;
    wxDouble  l_dFirstAngle;
    l_dFirstAngle = atan2(y1yc, x1xc);
    
    wxDouble  l_dSecondAngle;
    l_dSecondAngle = atan2(y4yc, x4xc);
    
    wxDouble  l_OuterRadius = sqrt(pow((y2-yc), 2.0) + pow((x2-xc), 2.0));
    wxDouble l_InnerRadius = sqrt(pow((y1-yc), 2.0) + pow((x1-xc), 2.0));
    float innerSteps;
    float outerSteps;
    if(bHighQuality) {
        innerSteps = floorf(wxMax(sqrtf(sqrtf( ((l_InnerRadius * 2.) * (l_InnerRadius * 2.)) * 2.) ), 1) * M_PI );
        outerSteps = floorf(wxMax(sqrtf(sqrtf( ((l_OuterRadius * 2.) * (l_OuterRadius * 2.)) * 2.) ), 1) * M_PI );
    } else {
        innerSteps = 24;
        outerSteps = 24;
    }
    
    points = new wxPoint[ (int) innerSteps +(int) outerSteps + 5 ];
    double dxc1 = xc-x1;
    double dxc4 = xc-x4;
    double dyc1 = yc-y1;
    double dyc4 = yc-y4;
    double angle = atan2(dxc1*dyc4-dyc1*dxc4, dxc1*dxc4+dyc1*dyc4);
    if(angle < 0) angle += (2*PI);
    int numpoints_outer = ceil(abs(outerSteps * (angle / (2.*M_PI))));
    int numpoints_inner = ceil(abs(innerSteps * (angle / (2.*M_PI))));
    //if(numpoints_outer == 0) return;
    points[0].x = x1;
    points[0].y = y1;
    float a = l_dFirstAngle;
    for( int i = 0; i < (int) numpoints_outer; i++ ) {
        points[i + 1].x = xc + l_OuterRadius * cosf( a );
        points[i + 1].y = yc + l_OuterRadius * sinf( a );
        a += 2 * M_PI /outerSteps;
    }
    a = l_dSecondAngle;
    points[ (int) numpoints_outer + 1].x = x3;
    points[ (int) numpoints_outer + 1].y = y3;
    points[ (int) numpoints_outer + 2].x = x4;
    points[ (int) numpoints_outer + 2].y = y4;
    for( int i = 0; i < (int) numpoints_inner; i++) {
        points[i + (int) numpoints_outer + 3].x = xc + l_InnerRadius * cosf( a );
        points[i + (int) numpoints_outer + 3].y = yc + l_InnerRadius * sinf( a );
        a -= 2 * M_PI / innerSteps;
    }
    int npoints[1];
    npoints[0] = numpoints_inner + numpoints_outer + 4;
    points[ npoints[0] -1 ].x = x1;
    points[ npoints[0] -1 ].y = y1;
    return npoints[0];
}

wxString WrapText(wxWindow *win, const wxString& text, int widthMax)
{
    class HardBreakWrapper : public wxTextWrapper
    {
    public:
        HardBreakWrapper(wxWindow *win, const wxString& text, int widthMax)
        {
            Wrap(win, text, widthMax);
        }
        wxString const& GetWrapped() const { return m_wrapped; }
    protected:
        virtual void OnOutputLine(const wxString& line)
        {
            m_wrapped += line;
        }
        virtual void OnNewLine()
        {
            m_wrapped += '\n';
        }
    private:
        wxString m_wrapped;
    };
    HardBreakWrapper wrapper(win, text, widthMax);
    return wrapper.GetWrapped();
}

wxString WrapString(const wxString &text, int widthMax)
{
    wxString l_string = wxEmptyString;
    int textlen = text.Len();
    int l_count = 0;
    int l_spacePos = 0;
    int l_lastSpacePos = 0;
    for(int i = 0; i < textlen; ++i) {
        auto c = text[i].GetValue();
        ++l_count;
        if(c == ' ') l_spacePos = i;
        if(widthMax <= l_count) {
            if(l_spacePos != i) {
                if(l_spacePos == l_lastSpacePos) {
                    l_string.append(text.SubString(l_lastSpacePos, i));
                    l_string.append('\n');
                    l_spacePos = i + 1;
                    l_lastSpacePos = l_spacePos;
                    l_count = 0;
                } else {
                    l_string.append(text.SubString(l_lastSpacePos, l_spacePos));
                    l_string.append('\n');
                    ++l_spacePos;
                    i = l_spacePos;
                    l_lastSpacePos = l_spacePos;
                    l_count = 0;
                }
                continue;
            } else {
                l_string.append(text.SubString(l_lastSpacePos, i));
                l_string.append('\n');
                l_spacePos = i;
                l_lastSpacePos = l_spacePos;
                l_count = 0;
            }
        }
        if(c == '\n') {
            l_string.append(text.SubString(l_lastSpacePos, i));
            l_spacePos = i + 1;
            l_lastSpacePos = l_spacePos;
            l_count = 0;
        }
    }
    if(l_count != 0) {
        l_string.append(text.SubString(l_lastSpacePos, textlen));
    }
    return l_string;
}
