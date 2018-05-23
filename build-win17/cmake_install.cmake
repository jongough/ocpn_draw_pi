# Install script for directory: P:/working area/opencpn/odraw

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ocpn_draw_pi/../OpenCPN")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins" TYPE SHARED_LIBRARY FILES "P:/working area/opencpn/odraw/build-win17/Debug/ocpn_draw_pi.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins" TYPE SHARED_LIBRARY FILES "P:/working area/opencpn/odraw/build-win17/Release/ocpn_draw_pi.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins" TYPE SHARED_LIBRARY FILES "P:/working area/opencpn/odraw/build-win17/MinSizeRel/ocpn_draw_pi.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins" TYPE SHARED_LIBRARY FILES "P:/working area/opencpn/odraw/build-win17/RelWithDebInfo/ocpn_draw_pi.dll")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins\\ocpn_draw_pi" TYPE DIRECTORY FILES "P:/working area/opencpn/odraw/data")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/plugins\\ocpn_draw_pi" TYPE DIRECTORY FILES "P:/working area/opencpn/odraw/UserIcons")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/da_DK/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/da_DK.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/it_IT/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/it_IT.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/nb_NO/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/nb_NO.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/tr_TR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/tr_TR.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/pt_PT/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/pt_PT.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ca_ES/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/ca_ES.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/fil_PH/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/fil_PH.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/de_DE/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/de_DE.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/fr_FR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/fr_FR.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ro_RO/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/ro_RO.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ja_JP/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/ja_JP.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/es_ES/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/es_ES.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/vi_VN/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/vi_VN.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/et_EE/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/et_EE.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/nl_NL/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/nl_NL.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/sv_SE/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/sv_SE.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/pt_BR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/pt_BR.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/gl_ES/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/gl_ES.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/id_ID/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/id_ID.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/cs_CZ/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/cs_CZ.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ru_RU/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/ru_RU.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/hi_IN/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/hi_IN.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/pl_PL/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/pl_PL.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/el_GR/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/el_GR.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/ar_SA/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/ar_SA.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/fi_FI/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/fi_FI.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/hu_HU/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/hu_HU.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/mr_IN/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/mr_IN.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/th_TH/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/th_TH.mo")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/locale/zh_TW/LC_MESSAGES" TYPE FILE RENAME "opencpn-ocpn_draw_pi.mo" FILES "P:/working area/opencpn/odraw/build-win17/zh_TW.mo")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "P:/working area/opencpn/odraw/build-win17/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
