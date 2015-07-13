##---------------------------------------------------------------------------
# Author:      jongough aka. Jon Gough
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(ocpn_draw_pi)

SET(PACKAGE_NAME ocpn_draw_pi)
SET(VERBOSE_NAME ocpn_draw)
SET(TITLE_NAME OCPNDRAW)
SET(CPACK_PACKAGE_CONTACT "Jon Gough")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "4")
SET(VERSION_PATCH "0380")
SET(VERSION_DATE "13/07/2015")

SET(BUNDLE_DATA FALSE)

#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_BUILD_TYPE "RelWithDebInfo")
IF( NOT CMAKE_BUILD_TYPE )
  SET( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
       FORCE )
ENDIF( NOT CMAKE_BUILD_TYPE )

MESSAGE (STATUS "Build type: ${CMAKE_BUILD_TYPE}")

SET(CMAKE_VERBOSE_MAKEFILE ON)

#SET_DIRECTORY_PROPERTIES(dummy ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_SOURCE_DIR}/include/version.h)

INCLUDE("cmake/PluginConfigure.cmake")

ADD_DEFINITIONS(-DUSE_S57)

SET(SRCS
        src/ocpn_draw_pi.cpp
        src/Boundary.cpp
        src/BoundaryProp.cpp
        src/BoundaryPoint.cpp
        src/PointMan.cpp
        src/ODSelect.cpp
        src/PathMan.cpp
        src/pathmanagerdialog.cpp
        src/Path.cpp
        src/ODPoint.cpp
        src/ODConfig.cpp
        src/ODEventHandler.cpp
        src/ODNavObjectChanges.cpp
        src/ODPropertiesDialogDef.cpp
        src/ODPropertiesDialogImpl.cpp
        src/ODPathPropertiesDialogDef.cpp
        src/ODPathPropertiesDialogImpl.cpp
        src/ODPointPropertiesDialog.cpp
        src/ODPointPropertiesImpl.cpp
        src/ODPositionParser.cpp
        src/ODRolloverWin.cpp
        src/ODToolbarDef.cpp
        src/ODToolbarImpl.cpp
        src/ODUtils.cpp
        src/ODicons.cpp
        src/TextPoint.cpp
            src/jsonreader.cpp
            src/jsonval.cpp
            src/jsonwriter.cpp
     )

SET(HDRS
        include/ocpn_draw_pi.h
        include/Boundary.h
        include/BoundaryProp.h
        include/BoundaryPoint.h
        include/PointMan.h
        include/ODSelect.h
        include/PathMan.h
        include/pathmanagerdialog.h
        include/Path.h
        include/ODPoint.h
        include/ODConfig.h
        include/ODEventHandler.h
        include/ODNavObjectChanges.h
        include/ODPropertiesDialogDef.h
        include/ODPropertiesDialogImpl.h
        include/ODPathPropertiesDialogDef.h
        include/ODPathPropertiesDialogImpl.h
        include/ODPointPropertiesDialog.h
        include/ODPointPropertiesImpl.h
        include/ODPositionParser.h
        include/ODRolloverWin.h
        include/ODToolbarDef.h
        include/ODToolbarImpl.h
        include/ODUtils.h
        include/ODvector2D.h
        include/ODicons.h
        include/TextPoint.h
        include/version.h
#            include/wx/json_defs.h
            include/wx/jsonreader.h
            include/wx/jsonval.h
            include/wx/jsonwriter.h
)

SET(OCPNSRC 
        ocpnsrc/bbox.cpp
        ocpnsrc/cutil.cpp
        ocpnsrc/FontDesc.cpp
        ocpnsrc/FontMgr.cpp
        ocpnsrc/geodesic.cpp
        ocpnsrc/georef.cpp
        ocpnsrc/Hyperlink.cpp
        ocpnsrc/Layer.cpp
        ocpnsrc/ocpndc.cpp
        ocpnsrc/pugixml.cpp
        ocpnsrc/SelectItem.cpp
)
SET(OCPNHDRS
        ocpninclude/AISTargetAlertDialog.h
        ocpninclude/AISTargetQueryDialog.h
        ocpninclude/apb.hpp
        ocpninclude/bbox.h
        ocpninclude/canvasMenu.h
        ocpninclude/chart1.h
        ocpninclude/chartbarwin.h
        ocpninclude/chartbase.h
        ocpninclude/chartdbs.h
        ocpninclude/chcanv.h
        ocpninclude/CM93DSlide.h
        ocpninclude/ConnectionParams.h
        ocpninclude/cutil.h
        ocpninclude/datastream.h
        ocpninclude/dsPortType.h
        ocpninclude/dychart.h
        ocpninclude/emboss_data.h
        ocpninclude/FontDesc.h
        ocpninclude/FontMgr.h
        ocpninclude/geodesic.h
        ocpninclude/georef.h
        ocpninclude/gga.hpp
        ocpninclude/gll.hpp
        ocpninclude/GoToPositionDialog.h
        ocpninclude/GPwpl.hpp
        ocpninclude/gpxdocument.h
        ocpninclude/gsv.hpp
        ocpninclude/hdg.hpp
        ocpninclude/hdm.hpp
        ocpninclude/hdt.hpp
        ocpninclude/Hyperlink.h
        ocpninclude/IDX_entry.h
        ocpninclude/LatLong.hpp
        ocpninclude/Layer.h
        ocpninclude/LinkPropDlg.h
        ocpninclude/MarkIcon.h
        ocpninclude/navutil.h
        ocpninclude/nmea0183.h
        ocpninclude/nmea0183.hpp
        ocpninclude/ocpCursor.h
        ocpninclude/ocpndc.h
        ocpninclude/OCPNPlatform.h
        ocpninclude/OCPNRegion.h
        ocpninclude/ocpn_plugin.h
        ocpninclude/ocpn_types.h
        ocpninclude/pluginmanager.h
        ocpninclude/pugiconfig.hpp
        ocpninclude/pugixml.hpp
        ocpninclude/Quilt.h
        ocpninclude/Response.hpp
        ocpninclude/RMB.hpp
        ocpninclude/RMC.HPP
        ocpninclude/RolloverWin.h
        ocpninclude/Route.h
        ocpninclude/RoutePoint.h
        ocpninclude/routeprop.h
        ocpninclude/rte.hpp
        ocpninclude/s52s57.h
        ocpninclude/s57chart.h
        ocpninclude/S57ClassRegistrar.h
        ocpninclude/S57Light.h
        ocpninclude/S57QueryDialog.h
        ocpninclude/s57RegistrarMgr.h
        ocpninclude/S57Sector.h
        ocpninclude/scrollingdialog.h
        ocpninclude/SelectItem.h
        ocpninclude/Sentence.hpp
        ocpninclude/styles.h
        ocpninclude/TexFont.h
        ocpninclude/timers.h
        ocpninclude/undo.h
        ocpninclude/vector2D.h
        ocpninclude/viewport.h
        ocpninclude/vtg.hpp
        ocpninclude/wpl.hpp
        ocpninclude/xte.hpp
)

#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/mygdal ${CMAKE_SOURCE_DIR}/../../include ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src )
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/mygdal  )
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../../include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/ocpninclude)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/ocpnsrc/lz4)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/ocpnsrc/bitmaps)

IF(UNIX)
 INCLUDE("cmake/FindTinyXML.cmake")
 FIND_PACKAGE(TinyXML QUIET)
ENDIF(UNIX)

IF(TINYXML_FOUND)
 message (STATUS "Building with system tinyxml")
 INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC})
 TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${TINYXML_LIBRARIES})
ELSE(TINYXML_FOUND)
 message (STATUS "Building with embedded tinyxml")
 INCLUDE_DIRECTORIES(src/tinyxml/)
 SET(SRC_LTINYXML
            src/tinyxml/tinyxml.cpp
            src/tinyxml/tinyxmlparser.cpp
            src/tinyxml/tinyxmlerror.cpp
 )
 ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${SRC_LTINYXML})
ENDIF(TINYXML_FOUND)
ADD_DEFINITIONS(-DTIXML_USE_STL)


INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
