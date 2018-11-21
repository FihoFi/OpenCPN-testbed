
Description of Operations
---------------------------
The LIVI_Depth_model plugin is for viewing additional depth information retrieved from depth model files.

Usage instructions
------------------
Usage instructions are still TODO.

Compilation instructions
------------------------
This package/plugin uses CMake meta compilation tool for generating the project files - 
see CMakeLists.txt to see (and alter, when developing) the contents of the package.

We have Visual Studio 2017 on Windows 10 to compile stuff, other environments have not been tested.
0. (You may need to set cmake to your path environment variable or something, but get back to this only if required)
00. Compile or get the required external libraries.
        This plugin requires PROJ (old name: PROJ4), which we compiled ourselves, and
        GDAL version supporting BAG format, which we had to compile ourselves, as the standard package does not have BAG support.
        GDAL 2.3.1 requires HDF5 library, which was also compiled by us.

        FOR THE USED VERSIONS OF THE EXTERNALS, see CMakeLists.txt.

1. Get the branch from git, and go to the root of the branch on your computer.
2. mk build (make a folder named "build")
3. cd build (go to the newly created folder)
3. cmake -vT141_xp .. (that is, tell
        cmake to generate you project file versions compilable by
        "T"=toolset, "141"="Visual Studio 2017", and "_xp"=XP-compatible (as the OpenCPN
	    we have as the base program, is also supporting the old 32-bit Windows XP),
        and stating the main CMakeLists.txt file will be found one folder up.
4. Wait for the compilation to end (hopefully without errors)
5. Now you may start developing by opening the Visual Studio solution (.sln) file generated to the build folder.
6. When adding files to the project, add them to CMakeLists.txt, instead of adding them directly to the VS solution, and/or projects.


2018-09-27 Marjukka Kokkonen, Sitowise
