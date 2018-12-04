cmake_minimum_required(VERSION 2.6)

project(glu)

SET(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

#add_definitions(-g -O2 )

IF(NOT WIN32 AND NOT APPLE )
  ADD_DEFINITIONS( "-Wall -Wno-unused -fexceptions -rdynamic" )
  ADD_DEFINITIONS( " -g -fno-strict-aliasing -O2")
ENDIF(NOT WIN32 AND NOT APPLE)

foreach(dir ${dirs})
  message(STATUS "dirtest1='${dir}'")
endforeach()
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/libtess)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/libutil)
INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/../../ocpninclude)
message(status "CMAKE_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dirtest2='${dir}'")
endforeach()

IF(WIN32)
    IF(MSVC)
        SET(CMAKE_C_FLAGS_DEBUG               "/MDd /Ob0 /Od  /D_DEBUG  /Zi /RTC1" )
        SET(CMAKE_C_FLAGS_MINSIZEREL          "/MD  /O1  /Ob1 /D NDEBUG")
        SET(CMAKE_C_FLAGS_RELEASE             "/MD  /O2  /Ob2 /D NDEBUG /Zi")
        SET(CMAKE_C_FLAGS_RELWITHDEBINFO      "/MD  /O2  /Ob1 /D NDEBUG /Zi")
        SET(CMAKE_CXX_FLAGS_DEBUG             "/MDd /Ob0 /Od  /D_DEBUG  /Zi /RTC1 /EHa")
        SET(CMAKE_CXX_FLAGS_MINSIZEREL        "/MD  /O1  /Ob1 /D NDEBUG /EHa")
        SET(CMAKE_CXX_FLAGS_RELEASE           "/MD  /O2  /Ob2 /D NDEBUG /Zi /EHa")
        SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO    "/MD  /O2  /Ob1 /D NDEBUG /Zi /EHa" )
        SET(CMAKE_EXE_LINKER_FLAGS /DEBUG)

    ENDIF(MSVC)
ENDIF(WIN32)

SET(GLU_SOURCES
    libutil/error.c
    libutil/glue.c
#    libutil/quad.c
#    libutil/mipmap.c
    libutil/project.c
    libutil/registry.c
    libtess/tess.c
    libtess/mesh.c
    libtess/priorityq.c
    libtess/sweep.c
    libtess/tessmono.c
    libtess/normal.c
    libtess/memalloc.c
    libtess/geom.c
    libtess/dict.c
    libtess/render.c
    )

add_library(GLU_static STATIC ${GLU_SOURCES})
set_target_properties(GLU_static PROPERTIES OUTPUT_NAME GLU)
