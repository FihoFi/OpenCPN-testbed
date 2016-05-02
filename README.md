30 Oct 2015

Now compiles and builds for Win 8.1/OpenCPN 4.1.925.

I needed to use CMake-GUI to force the use of WxWidgets 3.0.2.


Description of Operations
---------------------------
As it stands the DR plugin is editing gpx files that have been saved on your pc. 
The route name is the name of the route within the GPX file.

1. Use Route Manager to export the route GPX.
2. Open DR and set the parameters.
3. Using DR_pi (Generate) select the GPX you exported.
4. With the second file dialogue save the GPX+DR as a different GPX file.
5. Use Route Manager to import the route + DR positions.

Previous discussion centred on building this capacity into route manager. With the enhancements made to the API recently it may be possible to avoid all the file read/save business.

The logic behind creating DR positions over a multi-leg route may be a useful start for someone working on an OpenCPN-Draw DR solution.

Mike 
