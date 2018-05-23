# Install script for directory: /mnt/eclipse/working area/opencpn/odraw

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/lib/opencpn/libocpn_draw_pi.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib/opencpn/libocpn_draw_pi.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/lib/opencpn/libocpn_draw_pi.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/lib/opencpn/libocpn_draw_pi.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/lib/opencpn" TYPE SHARED_LIBRARY FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/libocpn_draw_pi.so")
  if(EXISTS "$ENV{DESTDIR}/usr/lib/opencpn/libocpn_draw_pi.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib/opencpn/libocpn_draw_pi.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/lib/opencpn/libocpn_draw_pi.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/opencpn/plugins/ocpn_draw_pi" TYPE DIRECTORY FILES "/mnt/eclipse/working area/opencpn/odraw/data")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/opencpn/plugins/ocpn_draw_pi" TYPE DIRECTORY FILES "/mnt/eclipse/working area/opencpn/odraw/UserIcons")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ar_SA/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/ar_SA.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ca_ES/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/ca_ES.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/cs_CZ/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/cs_CZ.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/da_DK/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/da_DK.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/de_DE/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/de_DE.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/el_GR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/el_GR.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/es_ES/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/es_ES.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/et_EE/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/et_EE.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/fi_FI/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/fi_FI.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/fr_FR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/fr_FR.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/gl_ES/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/gl_ES.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/hu_HU/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/hu_HU.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/it_IT/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/it_IT.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ja_JP/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/ja_JP.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/nb_NO/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/nb_NO.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/nl_NL/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/nl_NL.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/pl_PL/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/pl_PL.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/pt_BR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/pt_BR.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/pt_PT/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/pt_PT.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ru_RU/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/ru_RU.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/sv_SE/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/sv_SE.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/tr_TR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/tr_TR.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/vi_VN/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/vi_VN.mo")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/zh_TW/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/mnt/eclipse/working area/opencpn/odraw/build-pi/zh_TW.mo")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/mnt/eclipse/working area/opencpn/odraw/build-pi/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
