##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3
##---------------------------------------------------------------------------

if (TARGET ocpn::plugingl)
    return ()
endif ()

set(SAVE_CMLOC_PLUGINGLL ${CMLOC})
set(CMLOC "CMakeLists_PLUGINGL: ")

message(STATUS "${CMLOC}Building PluginGL")
SET(SRC_PLUGINGL
          src/pi_shaders.cpp
          src/pi_TexFont.cpp
          )

if(UNIX)
    add_definitions("-fpic")
endif(UNIX)

TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} PRIVATE "-lz" )
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} PRIVATE "-lGLESv1_CM" )
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} PRIVATE "-lGLESv2" )
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} PRIVATE  ${Qt_Base}/${Qt_Build}/lib/libQt5Gui.so )
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} PRIVATE  ${Qt_Base}/${Qt_Build}/lib/libQt5Core.so )
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} PRIVATE  ${Qt_Base}/${Qt_Build}/lib/libQt5Widgets.so )
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} PRIVATE  ${Qt_Base}/${Qt_Build}/lib/libQt5AndroidExtras.so )
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} PRIVATE  ${CMAKE_CURRENT_BINARY_DIR}/../../libgorp.so )
SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-soname,lib${PACKAGE_NAME}.so ")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-s")


include_directories(BEFORE ${CMAKE_SOURCE_DIR}/libs/glshim/include/GLES)
ADD_LIBRARY(${PACKAGE_NAME}_LIB_PLUGINGL STATIC ${SRC_PLUGINGL})
target_include_directories(${PACKAGE_NAME}_LIB_PLUGINGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(ocpn::plugingl ALIAS ${PACKAGE_NAME}_LIB_PLUGINGL)

set(CMLOC ${SAVE_CMLOC_PLUGINGL})
