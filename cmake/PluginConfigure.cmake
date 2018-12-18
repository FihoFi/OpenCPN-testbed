##---------------------------------------------------------------------------
## Author:      Pavel Kalian (Based on the work of Sean D'Epagnier)
## Copyright:   2014
## License:     GPLv3+
##---------------------------------------------------------------------------

SET(PLUGIN_SOURCE_DIR .)

# This should be 2.8.0 to have FindGTK2 module
IF (COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 OLD)
  CMAKE_POLICY(SET CMP0005 OLD)
  CMAKE_POLICY(SET CMP0011 OLD)
ENDIF (COMMAND cmake_policy)

MESSAGE (STATUS "*** Staging to build ${PACKAGE_NAME} ***")

configure_file(cmake/version.h.in ${PROJECT_SOURCE_DIR}/src/version.h)
SET(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )

#SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

INCLUDE_DIRECTORIES(
		${PROJECT_SOURCE_DIR}/src       # plugin source .h files
#		${PROJECT_SOURCE_DIR}/include   # symlinks to gdal root dir include files
#		${PROJ4_DIR}/src                # PROJ's include files
#		${GDAL_ROOT}/include            # GDAL's include files
		)

# SET(PROFILING 1)

#  IF NOT DEBUGGING CFLAGS="-O2 -march=native"
IF(NOT MSVC)
 IF(PROFILING)
  ADD_DEFINITIONS( "-Wall -g -fprofile-arcs -ftest-coverage -fexceptions" )
 ELSE(PROFILING)
#  ADD_DEFINITIONS( "-Wall -g -fexceptions" )
 ADD_DEFINITIONS( "-Wall -Wno-unused-result -g -O2 -fexceptions" )
 ENDIF(PROFILING)

 IF(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
 ELSE(NOT APPLE)
  SET(CMAKE_SHARED_LINKER_FLAGS "-Wl -undefined dynamic_lookup")
 ENDIF(NOT APPLE)

ENDIF(NOT MSVC)

# Add some definitions to satisfy MS
IF(MSVC)
    ADD_DEFINITIONS(-D__MSVC__)
    ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)
ENDIF(MSVC)

# Finding wxWidgets, see https://cmake.org/cmake/help/v3.11/module/FindwxWidgets.html
FIND_PACKAGE(wxWidgets REQUIRED)
SET(wxWidgets_USE_LIBS base core net xml html adv)
SET(BUILD_SHARED_LIBS TRUE)

IF(wxWidgets_FOUND)
    IF(MSYS)
        # this is just a hack. I think the bug is in FindwxWidgets.cmake
        STRING( REGEX REPLACE "/usr/local" "\\\\;C:/MinGW/msys/1.0/usr/local" wxWidgets_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS} )
    ENDIF(MSYS)

    INCLUDE(${wxWidgets_USE_FILE})   # for older cmake versions
    MESSAGE (STATUS "Found wxWidgets..." )
    MESSAGE (STATUS "    Lib: " ${wxWidgets_LIB_DIR})
   #MESSAGE (STATUS "    Use libs: " ${wxWidgets_USE_LIBS})
ELSE(wxWidgets_FOUND)
    MESSAGE (STATUS "wxWidgets not found..." )
ENDIF(wxWidgets_FOUND)

FIND_PACKAGE(OpenGL)
IF(OPENGL_GLU_FOUND)

    SET(wxWidgets_USE_LIBS ${wxWidgets_USE_LIBS} gl) # use gl in wxWidgets, too
   #MESSAGE (STATUS " wxWidgets use libs: " ${wxWidgets_USE_LIBS})
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

    MESSAGE (STATUS "Found OpenGL..." )
    MESSAGE (STATUS "    Lib: " ${OPENGL_LIBRARIES})
    MESSAGE (STATUS "    Include: " ${OPENGL_INCLUDE_DIR})
    ADD_DEFINITIONS(-DocpnUSE_GL)
ELSE(OPENGL_GLU_FOUND)
    MESSAGE (STATUS "OpenGL not found..." )
ENDIF(OPENGL_GLU_FOUND)

SET(BUILD_SHARED_LIBS TRUE)

FIND_PACKAGE(Gettext REQUIRED)

# Finding PROJ4, see https://cmake.org/cmake/help/v3.11/module/FindGDAL.html
FIND_PACKAGE(PROJ4 REQUIRED)
IF(PROJ4_FOUND)
    INCLUDE_DIRECTORIES(${PROJ4_INCLUDE_DIR})   # for older cmake versions

    MESSAGE (STATUS "Found PROJ4...")
    MESSAGE (STATUS "    Lib: " ${PROJ4_LIBRARIES})
    MESSAGE (STATUS "    Include: " ${PROJ4_INCLUDE_DIR})
ELSE(PROJ4_FOUND)
    MESSAGE (FATAL_ERROR "PROJ4 not found")
ENDIF(PROJ4_FOUND)

# Finding GDAL, see https://cmake.org/cmake/help/v3.11/module/FindGDAL.html
FIND_PACKAGE(GDAL REQUIRED)
IF(GDAL_FOUND)
    INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR})   # for older cmake versions

    MESSAGE (STATUS "Found GDAL...")
    MESSAGE (STATUS "    Lib: " ${GDAL_LIBRARIES})
    MESSAGE (STATUS "    Include: " ${GDAL_INCLUDE_DIR})
ELSE(GDAL_FOUND)
    MESSAGE (FATAL_ERROR "GDAL not found")
ENDIF(GDAL_FOUND)

