/***************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  OpenGL helper 
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
#endif

#ifdef __MSVC__
#include <windows.h>
#endif

#ifdef __WXMSW__
    #include "GL/gl.h"            // local copy for Windows
    #include <GL/glu.h>
#else

    #ifndef __OCPN__ANDROID__
        #include <GL/gl.h>
        #include <GL/glu.h>
    #else
        #include "qopengl.h"                  // this gives us the qt runtime gles2.h
        #include "GL/gl_private.h"
    #endif

#endif

#ifdef ocpnUSE_GL
#include <wx/glcanvas.h>
#endif

#include <wx/graphics.h>
#include <wx/dcclient.h>

#include <vector>

#include "ocpndc.h"


#ifdef ocpnUSE_GL

typedef union {
    GLdouble data[6];
    struct sGLvertex {
        GLdouble x;
        GLdouble y;
        GLdouble z;
        GLdouble r;
        GLdouble g;
        GLdouble b;
    } info;
} GLvertex;

void APIENTRY ocpnDCcombineCallback( GLdouble coords[3], GLdouble *vertex_data[4], GLfloat weight[4],
        GLdouble **dataOut )
{
    GLvertex *vertex;

    vertex = new GLvertex();
    gTesselatorVertices.Add(vertex );

    vertex->info.x = coords[0];
    vertex->info.y = coords[1];
    vertex->info.z = coords[2];

    for( int i = 3; i < 6; i++ ) {
        vertex->data[i] = weight[0] * vertex_data[0][i] + weight[1] * vertex_data[1][i];
    }

    *dataOut = &(vertex->data[0]);
}

void APIENTRY ocpnDCvertexCallback( GLvoid* arg )
{
    GLvertex* vertex;
    vertex = (GLvertex*) arg;
    glVertex2f( (float)vertex->info.x, (float)vertex->info.y );
}

void APIENTRY ocpnDCerrorCallback( GLenum errorCode )
{
   const GLubyte *estring;
   estring = gluErrorString(errorCode);
   wxLogMessage( _T("OpenGL Tessellation Error: %s"), estring );
}

void APIENTRY ocpnDCbeginCallback( GLenum type )
{
    glBegin( type );
}

void APIENTRY ocpnDCendCallback()
{
    glEnd();
}
#endif          //#ifdef ocpnUSE_GL

void ocpnDC::DrawPolygonTessellated( int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset )
{
    if( dc )
        dc->DrawPolygon( n, points, xoffset, yoffset );
#ifdef ocpnUSE_GL
    else {
# ifndef ocpnUSE_GLES  // tessalator in glues is broken
        if( n < 5 )
# endif
        {
            DrawPolygon( n, points, xoffset, yoffset );
            return;
        }

        glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_LINE_BIT | GL_HINT_BIT | GL_POLYGON_BIT ); //Save state
        SetGLAttrs( false );

        static GLUtesselator *tobj = NULL;
        if( ! tobj ) tobj = gluNewTess();

        gluTessCallback( tobj, GLU_TESS_VERTEX, (_GLUfuncptr) &ocpnDCvertexCallback );
        gluTessCallback( tobj, GLU_TESS_BEGIN, (_GLUfuncptr) &ocpnDCbeginCallback );
        gluTessCallback( tobj, GLU_TESS_END, (_GLUfuncptr) &ocpnDCendCallback );
        gluTessCallback( tobj, GLU_TESS_COMBINE, (_GLUfuncptr) &ocpnDCcombineCallback );
        gluTessCallback( tobj, GLU_TESS_ERROR, (_GLUfuncptr) &ocpnDCerrorCallback );

        gluTessNormal( tobj, 0, 0, 1);
        gluTessProperty( tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO );

        if( ConfigureBrush() ) {
            gluTessBeginPolygon( tobj, NULL );
            gluTessBeginContour( tobj );

            for( int i = 0; i < n; i++ ) {
                GLvertex* vertex = new GLvertex();
                gTesselatorVertices.Add( vertex );
                vertex->info.x = (GLdouble) points[i].x;
                vertex->info.y = (GLdouble) points[i].y;
                vertex->info.z = (GLdouble) 0.0;
                vertex->info.r = (GLdouble) 0.0;
                vertex->info.g = (GLdouble) 0.0;
                vertex->info.b = (GLdouble) 0.0;
                gluTessVertex( tobj, (GLdouble*)vertex, (GLdouble*)vertex );
            }
            gluTessEndContour( tobj );
            gluTessEndPolygon( tobj );
        }

        glPopAttrib();

        for( unsigned int i=0; i<gTesselatorVertices.Count(); i++ )
            delete (GLvertex*)gTesselatorVertices.Item(i);
        gTesselatorVertices.Clear();
    }
#endif    
}

