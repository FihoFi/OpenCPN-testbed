##---------------------------------------------------------------------------
## Author:      Rasbats aka. Mike Rossiter
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(LIVI_Depth_model_pi)

SET(PACKAGE_NAME LIVI_Depth_model_pi)
SET(VERBOSE_NAME LIVI_Depth_model)
SET(TITLE_NAME Depth_model)
SET(CPACK_PACKAGE_CONTACT "LIVI - Liikennevirasto")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "05")
SET(VERSION_PATCH "55e1a8f")
SET(VERSION_DATE "2018-10-22")
SET(GDAL_VERSION "2.3.1") # Just telling aloud the used GDAL version. No package-retrieval system here for now.
SET(PROJ_VERSION "5.1.0") # Just telling aloud the used PROJ version. No package-retrieval system here for now.
SET(HDF5_VERSION "1.10.3") # Just telling aloud the used HDF5 version. No package-retrieval system here for now.

#SET(CMAKE_BUILD_TYPE Debug)
SET( wxWidgets_USE_DEBUG OFF)
SET( wxWidgets_USE_UNICODE ON)
SET( wxWidgets_USE_UNIVERSAL OFF)
SET( wxWidgets_USE_STATIC OFF)

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_LIVI_Depth_model
        src/LIVI_Depth_model_pi.h
        src/LIVI_Depth_model_pi.cpp
        src/icons.h
        src/icons.cpp
        src/LIVI_Depth_model_pi_UI.h
        src/LIVI_Depth_model_pi_UI.cpp
        src/LIVI_Depth_model_pi_UI_impl.h
        src/LIVI_Depth_model_pi_UI_impl.cpp
        src/dm_API.h
        src/dm_API.cpp
        src/dmConfigHandler.h
        src/dmConfigHandler.cpp
        src/dmDataset.h
        src/dmDataset.cpp
        src/NavFunc.h
        src/NavFunc.cpp
        src/dmDepthModelDrawer.h
        src/dmDepthModelDrawer.cpp
        src/dmDrawingState.h
        src/dmDrawingState.cpp
        src/ocpn_plugin.h
        src/tinyxml.cpp
        src/tinyxmlerror.cpp
        src/tinyxmlparser.cpp
        src/tinyxml.h
    )


ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_LIVI_Depth_model})

# GDAL related headers and libraries
target_include_directories(${PACKAGE_NAME} PRIVATE C:/warmerda/bld/include/)
target_link_libraries(${PACKAGE_NAME} 
        debug       C:/proj-5.1.0/build/lib/Debug/proj_5_0_d.lib
        optimized   C:/proj-5.1.0/build/lib/Release/proj_5_0.lib)
target_link_libraries(${PACKAGE_NAME} C:/gdal-2.3.1/gdal_i.lib)
ADD_DEFINITIONS(-DTIXML_USE_STL)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
