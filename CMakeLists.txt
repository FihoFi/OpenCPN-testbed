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

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "beta-3a0fe4a")
SET(VERSION_DATE "2018-12-18")
# Version release notes:
# 1.00 beta ("0.12")
#   Addition of required external libraries, and files added to PACKACE project (DM-74)
#   Water level adjustments available with user's colour configuration files. Both "normal"
#       depth values, and percentage values are supported. (DM-71)
#   Simplification of water level console window. (DM-67)
#   Bugfix: Corrected ini values that made the newly fresh-installed plugin not shown at all.
#       Also corrected Depth console leaving into toolbar after disabling Depth model plugin.
#   Bugfix: Corrected Current depth value of depth console, that got summed up with
#       wrong direction  water level values wrong direction.
#   Bugfix: Plugin UI dialog max size grown so that all five-colour-option fields do shown.
#   Bugfix: initial values: black colour values changed to other colours (black is drawn as fully
#       transparent). Initial restricting depth values also got smarter starting levels (other
#       than all "-20"s.)
# 0.11
#   Plain image generation uses cubic resampling algorithm instead of max, to keep The
#       picture from changing too much. (DM-68)
#   Hillshade chart type parameters can now be changed from config (.ini) file. (DM-70)
#   Water level parameters are now shown in a small window. (DM-67)
# 0.10
#   Now shows depth values next to cursor on the map. Value is shown where the 
#       model has sensible values. Format is"current (systemcorrected + water level)".
#       (DM-65, DM-66)
#   Icon updated to comply more with the general OpenCPN icon style.
# 0.(0)9
#   Added water level variables Current water level, and Vertical reference system offset.
#       These are used with Colour relief chrats with five, of two depth ranges. (DM-60-63)
#   File generation tab has now info for Current, and To-be-generated options
#   Fixed bug, where five, and two colour range depths were loaded from config into the UI
#        as integers instead of doubles, thus getting the decimals truncated off at plugin startup.
#   Fixed "on the fly changing" of the chart type, where tweaking UI chartType field
#       caused the chart type to change unwantedly.
# Known problems
#   Colouring option Colour relief / Sliding colours is not implemented
# 0.(0)8
#   Fixed requirement of running OpenCPN as administrator, in order to use Depth model. (DM-64)
#       The changing files are now saved under wxStandardPaths::Get().GetUserDataDir(), 
#       which is platform-independent. In Win10 we end up with 
#       C:\Users\YOURUSERNAME\AppData\Roaming\opencpn\plugins\LIVI_Depth_model_pi\data
#   Fixed "yellow stripes". That is, corrected gdal usage, so that there will be no faulty 
#       intermediate colours outside of what the user has fed anymore.
#   Fixed Colour changes in UI kicking in only after second click of "Generate image".
#   Fixed failure/crash at generating a chart with Plain visualization type.
#   Second shot at fixing weird problem, where the plugin icon was randomly shown or not shown at OCPN toolbar.
#   (Leading zero removed from the minor version number, as the number was interpreted as octal, and thus there could be no 08.)
# 0.07
#    Fixed weird problem, where the plugin icon was randomly shown or not shown at OCPN toolbar.
#    Fixed colour config values not getting saved.
#    Fixed the chart image not updating at "Generate image" click.
#    Fixed colour configuration values not propagating further from the UI.
#    Fixed opaqueness of "two colours/too shallow". Now has half transparency insteaf of being fully opaque.
#    Fixed Too transparent "five colours/deep enough" colouring. Opaqueness value changed from 16 to 32.
#    Added ini file writing at "Generate Image" click.
#    Added "Clear image" button, to switch depth model chart drawing off.
#    Added *.png to chart file filters.
#    Added some logging.
#    Disabled Hillshade tab fields, as they currently are not taken to the backend.
# 0.06
#    Cropping, and stabilizing. (DM-54, DM-55, DM-59)
#    The drawn chart is now cropped nicely.
#    No more memory related crashings on zoom in, or crash on zoom out of the chart.
# 0.05
#
#
SET(GDAL_VERSION "2.3.1") # Just telling aloud the used GDAL version. No package-retrieval system here for now.
SET(PROJ_VERSION "5.2.0") # Just telling aloud the used PROJ version. No package-retrieval system here for now.
SET(HDF5_VERSION "1.10.3") # Just telling aloud the used HDF5 version. No package-retrieval system here for now.

# You may set these environment variables to your corresponding locations, to find wxWidgets.
#SET(wxWidgets_ROOT_DIR "C:/wxWidgets/wxWidgets-3.1.1")
#SET(wxWidgets_LIB_DIR "C:/wxWidgets-3.1.1/lib/vc_lib")
#IF(CMAKE_BUILD_TYPE MATCHES "Debug")
#  SET(wxWidgets_CONFIGURATION mswd)
#ELSEIF(CMAKE_BUILD_TYPE MATCHES "Release")
#  SET(wxWidgets_CONFIGURATION msw)
#ENDIF()#(CMAKE_BUILD_TYPE)

# Set Directories for proj, and gdal.
#These could be set as environment variables instead.
SET(PROJ4_DIR "C:/OSGeo4W")          # C:/proj-5.2.0
SET(GDAL_ROOT "C:/gdal-2.3.1/build") # C:/warmerda/---
# NOTE! set environment variable CMAKE_PREFIX_PATH  to point where ever your proj4 cmake configuration file is
SET(MY_PROJ4_DIR "C:/OSGeo4W")       # Ugh. Required for package installer, as PROJ4_DIR resolves to something under CMAKE_PREFIX_PATH, which is incorrect.

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
        src/LIVI_Depth_model_pi_UI.h    # generated by wxFormWidgets
        src/LIVI_Depth_model_pi_UI.cpp  # generated by wxFormWidgets
        src/LIVI_Depth_model_pi_UI_impl.h
        src/LIVI_Depth_model_pi_UI_impl.cpp
        src/dm_API.h
        src/dm_API.cpp
        src/dmColourfileHandler.h
        src/dmColourfileHandler.cpp
        src/dmConfigHandler.h
        src/dmConfigHandler.cpp
        src/dmDataset.h
        src/dmDataset.cpp
        src/NavFunc.h
        src/NavFunc.cpp
        src/dmDepthModelDrawer.h
        src/dmDepthModelDrawer.cpp
        src/dmDepthsViewerUI.h   # generated by wxFormWidgets
        src/dmDepthsViewerUI.cpp # generated by wxFormWidgets
        src/dmDepthsViewerUI_impl.h
        src/dmDepthsViewerUI_impl.cpp
        src/dmDrawingState.h
        src/dmDrawingState.cpp
        src/dmExtent.h
        src/dmExtent.cpp
        src/ocpn_plugin.h
        src/tinyxml.cpp
        src/tinyxmlerror.cpp
        src/tinyxmlparser.cpp
        src/tinyxml.h
    )

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_LIVI_Depth_model})

# Linking external libraries. For finding the libraries, see PluginConfigure.cmake.
target_link_libraries(${PACKAGE_NAME} ${wxWidgets_LIBRARIES})
target_link_libraries(${PACKAGE_NAME} ${PROJ4_LIBRARIES}) # proj_X_X[_d].lib
target_link_libraries(${PACKAGE_NAME} ${GDAL_LIBRARIES})

#target_link_libraries(${PACKAGE_NAME}
#    debug       ${PROJ4_DIR}/local/lib/proj_5_2_d.lib
#    optimized   ${PROJ4_DIR}/local/lib/proj_5_2.lib)

ADD_DEFINITIONS(-DTIXML_USE_STL)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
