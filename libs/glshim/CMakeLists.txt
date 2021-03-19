cmake_minimum_required(VERSION 2.6)

if (TARGET ocpn::gdal)
    return ()
endif ()

project(glesgl)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# Raspberry PI
if(BCMHOST)
    include_directories(/opt/vc/include /opt/vc/include/interface/vcos/pthreads /opt/vc/include/interface/vmcs_host/linux)
    link_directories(/opt/vc/lib)
    add_definitions(-DBCMHOST)
endif()

link_directories(${CMAKE_BINARY_DIR}/lib)
add_definitions(-g -std=c99 -funwind-tables -O2)

include_directories(include)
add_subdirectory(src)

add_library(GLSHIM STATIC ${SRC})
add_library(ocpn::glshim ALIAS GLSHIM)

