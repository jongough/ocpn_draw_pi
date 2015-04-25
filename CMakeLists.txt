##---------------------------------------------------------------------------
## Author:      jongough aka. Jon Gough
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
SET(VERSION_MINOR "2")

SET(BUNDLE_DATA FALSE)

SET_DIRECTORY_PROPERTIES(dummy ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_SOURCE_DIR}/include/version.h)

SET(CMAKE_BUILD_TYPE Debug)

INCLUDE("cmake/PluginConfigure.cmake")

ADD_DEFINITIONS(-DUSE_S57)

SET(SRCS
		src/ocpn_draw_pi.cpp
 		src/Boundary.cpp
		src/BoundaryProp.cpp
 		src/PointMan.cpp
 		src/OCPNSelect.cpp
 		src/PathMan.cpp
 		src/pathmanagerdialog.cpp
 		src/PathProp.cpp
 		src/Path.cpp
 		src/OCPNPoint.cpp
 		src/OCPNDrawConfig.cpp
 		src/OCPNDrawEventHandler.cpp
 		src/OCPNDrawNavObjectChanges.cpp
# 		src/OCPNDrawCanvasMenuHandler.cpp
# 		src/OCPNDrawPointInfoImpl.cpp
        src/OCPNDrawPropertiesDialog.cpp
        src/OCPNDrawPropertiesImpl.cpp
#        src/ODHelper.cpp
#        src/ODIcons.cpp
 		src/ODPointPropertiesDialog.cpp
 		src/ODPointPropertiesImpl.cpp
 		src/ODPositionParser.cpp
 		src/ODRolloverWin.cpp
 		src/ODstyles.cpp
# 		src/ODUndo.cpp
        src/ODUtils.cpp
 		src/OCPNDrawicons.cpp
        	src/jsonreader.cpp
        	src/jsonval.cpp
        	src/jsonwriter.cpp
 	)

SET(HDRS
		include/ocpn_draw_pi.h
 		include/Boundary.h
 		include/BoundaryProp.h
 		include/PointMan.h
 		include/OCPNSelect.h
 		include/PathMan.h
 		include/pathmanagerdialog.h
 		include/PathProp.h
 		include/Path.h
 		include/OCPNPoint.h
 		include/OCPNDrawConfig.h
 		include/OCPNDrawEventHandler.h
 		include/OCPNDrawNavObjectChanges.h
 		include/OCPNDrawPropertiesDialog.h
 		include/OCPNDrawPropertiesImpl.h
 		include/ODPointPropertiesDialog.h
 		include/ODPointPropertiesImpl.h
 		include/ODPositionParser.h
 		include/ODRolloverWin.h
# 		include/ODUndo.h
 		include/ODUtils.h
 		include/ODvector2D.h
 		include/OCPNDrawicons.h
		include/version.h
        	include/wx/json_defs.h
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
#SET(OCPNHDRS
#		ocpninclude/RoutePoint.h
#		ocpninclude/Route.h
#		ocpninclude/routeman.h
#		ocpninclude/routemanagerdialog.h
#)

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

