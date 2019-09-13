##---------------------------------------------------------------------------
## Author:      Pavel Kalian (Based on the work of Sean D'Epagnier)
## Copyright:   2014
## License:     GPLv3+
##---------------------------------------------------------------------------

# build a CPack driven installer package
#include (InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_NAME "${PACKAGE_NAME}")
SET(CPACK_PACKAGE_VENDOR "opencpn.org")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CPACK_PACKAGE_NAME} ${PACKAGE_VERSION})
SET(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
SET(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};${PACKAGE_NAME};ALL;/")
SET(CPACK_PACKAGE_EXECUTABLES OpenCPN ${PACKAGE_NAME})

IF(WIN32)
# to protect against confusable windows users, let us _not_ generate zip packages
#  SET(CPACK_GENERATOR "NSIS;ZIP")

  # override install directory to put package files in the opencpn directory
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "OpenCPN")
  SET(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}-${OCPN_MIN_VERSION}")

# CPACK_NSIS_DIR ??
# CPACK_BUILDWIN_DIR ??
# CPACK_PACKAGE_ICON ??

  SET(CPACK_NSIS_PACKAGE_NAME "${PACKAGE_NAME}")

  # Let cmake find NSIS.template.in
  SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/buildwin")

#  These lines set the name of the Windows Start Menu shortcut and the icon that goes with it
#  SET(CPACK_NSIS_INSTALLED_ICON_NAME "${PACKAGE_NAME}")
  SET(CPACK_NSIS_DISPLAY_NAME "OpenCPN ${PACKAGE_NAME}")

  SET(CPACK_PACKAGE_FILE_NAME "${PACKAGE_FILE_NAME}_${CPACK_PACKAGE_VERSION}_setup" )

  SET(CPACK_NSIS_DIR "${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode")  #Gunther
  SET(CPACK_BUILDWIN_DIR "${PROJECT_SOURCE_DIR}/buildwin")  #Gunther

ELSE(WIN32)
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${PACKAGE_NAME})
ENDIF(WIN32)

SET(CPACK_STRIP_FILES "${PACKAGE_NAME}")

SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gpl.txt")

IF (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/README")
#    MESSAGE(STATUS "Using generic cpack package description file.")
    SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
    SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
ENDIF ()

#SET(CPACK_SOURCE_GENERATOR "TGZ")

# The following components are regex's to match anywhere (unless anchored)
# in absolute path + filename to find files or directories to be excluded
# from source tarball.
set(CPACK_SOURCE_IGNORE_FILES
"^${CMAKE_CURRENT_SOURCE_DIR}/.git/*"
"^${CMAKE_CURRENT_SOURCE_DIR}/build*"
"^${CPACK_PACKAGE_INSTALL_DIRECTORY}/*"
)

SET(PACKAGE_RELEASE ${OCPN_MIN_VERSION})

IF(UNIX AND NOT APPLE)
#    INCLUDE(UseRPMTools)
#    IF(RPMTools_FOUND)
#        RPMTools_ADD_RPM_TARGETS(packagename ${PROJECT_SOURCE_DIR}/package.spec)
#    ENDIF(RPMTools_FOUND)

# need apt-get install rpm, for rpmbuild
    SET(PACKAGE_DEPS "opencpn, bzip2, gzip")


  IF (CMAKE_SYSTEM_PROCESSOR MATCHES "arm*")
    SET (ARCH "armhf")
    # don't bother with rpm on armhf
    SET(CPACK_GENERATOR "DEB;TBZ2")
  ELSE ()
    SET(CPACK_GENERATOR "DEB;RPM;TBZ2")

    IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
      SET (ARCH "amd64")
      SET(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
    ELSE (CMAKE_SIZEOF_VOID_P MATCHES "8")
      SET (ARCH "i386")
      # note: in a chroot must use "setarch i686 make package"
      SET(CPACK_RPM_PACKAGE_ARCHITECTURE "i686")
    ENDIF (CMAKE_SIZEOF_VOID_P MATCHES "8")
  ENDIF ()
  
  SET(CPACK_DEBIAN_PACKAGE_DEPENDS ${PACKAGE_DEPS})
  SET(CPACK_DEBIAN_PACKAGE_RECOMMENDS ${PACKAGE_RECS})
  SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${ARCH})
  SET(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")
  SET(CPACK_DEBIAN_PACKAGE_SECTION "misc")
  SET(CPACK_DEBIAN_COMPRESSION_TYPE "xz") # requires my patches to cmake

  SET(CPACK_RPM_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")
  SET(CPACK_RPM_PACKAGE_REQUIRES  ${PACKAGE_DEPS})
#    SET(CPACK_RPM_PACKAGE_GROUP "Applications/Engineering")
  SET(CPACK_RPM_PACKAGE_LICENSE "gplv3+")

  SET(CPACK_RPM_COMPRESSION_TYPE "xz")
#    SET(CPACK_RPM_USER_BINARY_SPECFILE "${PROJECT_SOURCE_DIR}/opencpn.spec.in")

  SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PACKAGE_NAME} PlugIn for OpenCPN")
  SET(CPACK_PACKAGE_DESCRIPTION "${PACKAGE_NAME} PlugIn for OpenCPN")
  SET(CPACK_SET_DESTDIR ON)

  SET(CPACK_PACKAGE_FILE_NAME "${PACKAGE_FILE_NAME}_${PACKAGE_VERSION}-${PACKAGE_RELEASE}_${ARCH}" )

ENDIF(UNIX AND NOT APPLE)

IF(TWIN32 AND NOT UNIX)
# configure_file(${PROJECT_SOURCE_DIR}/src/opencpn.rc.in ${PROJECT_SOURCE_DIR}/src/opencpn.rc)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_GERMAN.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_GERMAN.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_FRENCH.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_FRENCH.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_CZECH.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_CZECH.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_DANISH.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_DANISH.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_SPANISH.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_SPANISH.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_ITALIAN.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_ITALIAN.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_DUTCH.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_DUTCH.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_POLISH.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_POLISH.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_PORTUGUESEBR.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_PORTUGUESEBR.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_PORTUGUESE.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_PORTUGUESE.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_RUSSIAN.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_RUSSIAN.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_SWEDISH.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_SWEDISH.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_FINNISH.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_FINNISH.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_NORWEGIAN.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_NORWEGIAN.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_CHINESETW.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_CHINESETW.nsh" @ONLY)
 configure_file("${PROJECT_SOURCE_DIR}/buildwin/NSIS_Unicode/Language files/Langstrings_TURKISH.nsh.in" "${PROJECT_SOURCE_DIR}//buildwin/NSIS_Unicode/Include/Langstrings_TURKISH.nsh" @ONLY)
ENDIF(TWIN32 AND NOT UNIX)


# this dummy target is necessary to make sure the ADDITIONAL_MAKE_CLEAN_FILES directive is executed.
# apparently, the base CMakeLists.txt file must have "some" target to activate all the clean steps.
#ADD_CUSTOM_TARGET(dummy COMMENT "dummy: Done." DEPENDS ${PACKAGE_NAME})

INCLUDE(CPack)

option(OCPN_CI_BUILD "Use CI build versioning rules" OFF)
if (OCPN_CI_BUILD)
  include("cmake/Utils.cmake")
  today(DATE)
  commit_id(COMMIT)
  set(VERSION_TAIL "+${COMMIT}")
  set(VERSION_DATE "${DATE}")
endif (OCPN_CI_BUILD)

IF(APPLE)
  MESSAGE (STATUS "*** Staging to build PlugIn OSX Package the new way ***")
# -- Run the BundleUtilities cmake code
  set(CPACK_BUNDLE_PLIST "${CMAKE_SOURCE_DIR}/buildosx/Info.plist.in")
  
  SET(CPACK_PACKAGE_FILE_NAME "${PACKAGE_FILE_NAME}_${PACKAGE_VERSION}-${PACKAGE_RELEASE}" )


#  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/OpenCPN.app")
  set(APPS "")
  set(DIRS "")

  # INSTALL(DIRECTORY DESTINATION "bin/OpenCPN.app/Contents/PlugIns")
  install(
    FILES ${PREFIX_PARENTLIB}/libocpn_draw_pi.dylib
    DESTINATION "bin/OpenCPN.app/Contents/PlugIns"
  )
  set(
    LIBS
    "\${CMAKE_INSTALL_PREFIX}/bin/OpenCPN.app/Contents/PlugIns/libocpn_draw_pi.dylib"
  )
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CPACK_PACKAGE_FILE_NAME}.dmg
    COMMAND chmod +x ${CMAKE_SOURCE_DIR}/buildosx/create-dmg
    COMMAND
      ${CMAKE_SOURCE_DIR}/buildosx/create-dmg 
      --volname "ocpn_draw_pi Installer" 
      --background ${CMAKE_SOURCE_DIR}/buildosx/background.png
        ${CMAKE_CURRENT_BINARY_DIR}/${CPACK_PACKAGE_FILE_NAME}.dmg
        ${CMAKE_INSTALL_PREFIX}/bin/
    DEPENDS ${CMAKE_INSTALL_PREFIX}/bin/OpenCPN.app/Contents/PlugIns/libocpn_draw_pi.dylib
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "create-dmg [${PACKAGE_NAME}]: Generated dmg file."
  )
  add_custom_target(
    create-dmg
    COMMENT "create-dmg: Done."
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${CPACK_PACKAGE_FILE_NAME}.dmg
  )

  MESSAGE (STATUS "*** Staging to build PlugIn OSX Package the old way ***")
  configure_file(${PROJECT_SOURCE_DIR}/cmake/gpl.txt
            ${CMAKE_CURRENT_BINARY_DIR}/license.txt COPYONLY)
	    
  configure_file(${PROJECT_SOURCE_DIR}/buildosx/InstallOSX/pkg_background.jpg
            ${CMAKE_CURRENT_BINARY_DIR}/pkg_background.jpg COPYONLY)

  configure_file(${PROJECT_SOURCE_DIR}/buildosx/InstallOSX/${PACKAGE_NAME}.pkgproj.in
            ${CMAKE_CURRENT_BINARY_DIR}/${VERBOSE_NAME}.pkgproj)

  ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${VERBOSE_NAME}-Plugin_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}_${OCPN_MIN_VERSION}.pkg
   COMMAND /usr/local/bin/packagesbuild -F ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${VERBOSE_NAME}.pkgproj
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   DEPENDS ${PACKAGE_NAME}
   COMMENT "create-pkg [${PACKAGE_NAME}]: Generating pkg file."
  )

  ADD_CUSTOM_TARGET(create-pkg COMMENT "create-pkg: Done."
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${VERBOSE_NAME}-Plugin_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}_${OCPN_MIN_VERSION}.pkg )

ENDIF(APPLE)
