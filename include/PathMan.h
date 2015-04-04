/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  Point Manager
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

#ifndef PATHMAN_H
#define PATHMAN_H

#include "Path.h"
#include "PathMan.h"


class PathMan
{
    public:
        PathMan();
        virtual ~PathMan();

        bool DeletePath(Path *pPath);
        void DeleteAllPaths(void);

        bool IsPathValid(Path *pRoute);

        Path *FindPathByGUID(wxString &guid);
        Path *FindPathContainingOCPNPoint(OCPNPoint *pWP);
        wxArrayPtrVoid *GetPathArrayContaining(OCPNPoint *pWP);
        bool DoesPathContainSharedPoints( Path *pPath );

        bool ActivatePath(Path *pPathToActivate);
        bool ActivateOCPNPoint(Path *pA, OCPNPoint *pRP);
        bool ActivateOCPNPoint(Path *pr, bool skipped);
        OCPNPoint *FindBestActivatePoint(Path *pR, double lat, double lon, double cog, double sog);

        bool UpdateProgress();
        bool UpdateAutopilot();
        bool DeactivatePath( Path *pPathToDeactivate );
        bool IsAnyPathActive(void){ return (pActivePath != NULL); }
        void SetColorScheme(ColorScheme cs);

        Path *GetpActivePath(){ return pActivePath;}
        OCPNPoint *GetpActivePoint(){ return pActivePoint;}
        double GetCurrentRngToActivePoint(){ return CurrentRngToActivePoint;}
        double GetCurrentBrgToActivePoint(){ return CurrentBrgToActivePoint;}
        double GetCurrentRngToActiveNormalArrival(){ return CurrentRangeToActiveNormalCrossing;}
        double GetCurrentXTEToActivePoint(){ return CurrentXTEToActivePoint;}
        void   ZeroCurrentXTEToActivePoint();
        double GetCurrentSegmentCourse(){ return CurrentSegmentCourse;}
        int   GetXTEDir(){ return XTEDir;}

        wxPen   * GetPathPen(void){return m_pPathPen;}
        wxPen   * GetSelectedPathPen(void){return m_pSelectedPathPen;}
        wxPen   * GetActivePathPen(void){return m_pActivePathPen;}
        wxPen   * GetActiveOCPNPointPen(void){return m_pActiveOCPNPointPen;}
        wxPen   * GetOCPNPointPen(void){return m_pOCPNPointPen;}
        wxBrush * GetPathBrush(void){return m_pPathBrush;}
        wxBrush * GetSelectedPathBrush(void){return m_pSelectedPathBrush;}
        wxBrush * GetActivePathBrush(void){return m_pActivePathBrush;}
        wxBrush * GetActiveOCPNPointBrush(void){return m_pActiveOCPNPointBrush;}
        wxBrush * GetOCPNPointBrush(void){return m_pOCPNPointBrush;}

        wxString GetRouteReverseMessage(void);

        bool        m_bDataValid;

    private:
        void DoAdvance(void);

        MyApp       *m_pparent_app;
        Path        *pActivePath;
        OCPNPoint   *pActivePoint;
        double       PathBrgToActivePoint;        //TODO all these need to be doubles
        double       CurrentSegmentBeginLat;
        double       CurrentSegmentBeginLon;
        double       CurrentRngToActivePoint;
        double       CurrentBrgToActivePoint;
        double       CurrentXTEToActivePoint;
        double       CourseToPathSegment;
        double       CurrentRangeToActiveNormalCrossing;
        OCPNPoint   *pActivePathSegmentBeginPoint;
        OCPNPoint   *pPathActivatePoint;
        double       CurrentSegmentCourse;
        int         XTEDir;
        bool        m_bArrival;
        wxPen       *m_pPathPen;
        wxPen       *m_pSelectedPathPen;
        wxPen       *m_pActivePathPen;
        wxPen       *m_pActiveOCPNPointPen;
        wxPen       *m_pOCPNPointPen;
        wxBrush     *m_pPathBrush;
        wxBrush     *m_pSelectedPathBrush;
        wxBrush     *m_pActivePathBrush;
        wxBrush     *m_pActiveOCPNPointBrush;
        wxBrush     *m_pOCPNPointBrush;

        double      m_arrival_min;
        int         m_arrival_test;
      
};

#endif // PATHMAN_H
