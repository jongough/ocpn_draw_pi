# Install script for directory: /home/jon/opencpn/odraw

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/opencpn/libocpn_draw_pi.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/opencpn/libocpn_draw_pi.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/opencpn/libocpn_draw_pi.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/opencpn/libocpn_draw_pi.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/opencpn" TYPE SHARED_LIBRARY FILES "/home/jon/opencpn/odraw/build-cb/libocpn_draw_pi.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/opencpn/libocpn_draw_pi.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/opencpn/libocpn_draw_pi.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/opencpn/libocpn_draw_pi.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/opencpn/plugins/ocpn_draw_pi" TYPE DIRECTORY FILES "/home/jon/opencpn/odraw/data")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/opencpn/plugins/ocpn_draw_pi" TYPE DIRECTORY FILES "/home/jon/opencpn/odraw/UserIcons")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/da_DK/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/da_DK.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/it_IT/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/it_IT.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/nb_NO/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/nb_NO.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/tr_TR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/tr_TR.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/pt_PT/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/pt_PT.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ca_ES/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/ca_ES.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/fil_PH/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/fil_PH.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/de_DE/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/de_DE.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/fr_FR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/fr_FR.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ro_RO/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/ro_RO.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ja_JP/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/ja_JP.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/es_ES/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/es_ES.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/vi_VN/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/vi_VN.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/et_EE/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/et_EE.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/nl_NL/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/nl_NL.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/sv_SE/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/sv_SE.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/pt_BR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/pt_BR.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/gl_ES/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/gl_ES.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/id_ID/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/id_ID.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/cs_CZ/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/cs_CZ.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ru_RU/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/ru_RU.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/hi_IN/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/hi_IN.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/pl_PL/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/pl_PL.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/el_GR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/el_GR.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ar_SA/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/ar_SA.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/fi_FI/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/fi_FI.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/hu_HU/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/hu_HU.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/mr_IN/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/mr_IN.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/th_TH/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/th_TH.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/zh_TW/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "/home/jon/opencpn/odraw/build-cb/zh_TW.mo")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/jon/opencpn/odraw/build-cb/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
