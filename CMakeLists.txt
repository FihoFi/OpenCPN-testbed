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
SET(TITLE_NAME LIVI_Depth_model)
SET(CPACK_PACKAGE_CONTACT "LIVI - Liikennevirasto")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "01")
SET(VERSION_DATE "2018-07-10")

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
        src/LIVI_Depth_model_pi_UI_impl.cpp
        src/LIVI_Depth_model_pi_UI_impl.h
        src/NavFunc.h
        src/NavFunc.cpp
        src/ocpn_plugin.h
        src/tinyxml.cpp
        src/tinyxmlerror.cpp
        src/tinyxmlparser.cpp
        src/tinyxml.h
    )


ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_LIVI_Depth_model})

ADD_DEFINITIONS(-DTIXML_USE_STL)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
