# ocpn_draw_pi
OpenCPN general drawing plug in

Requires latest OCPN beta as it is using version 1.13 of the plugin API

Files are stored in share/opencpn/OCPNDraw/data & share/opencpn/OCPNDraw/images. You will need to copy the images from 
the /images folder to the share/opencpn/OCPNDraw/images folder.

Currently you will need to create the images folder before trying to run OCPNDraw. 

If you are running under windows the 'PACKAGE' process works and will create an installable exe (you just have to run it). 
There is no package for Linux at the moment, although the Linux version is likely to be more stable as that is the 
platform used to develop this plugin.

From OpenCPN.org Developers Manual - Compiling for Windows
after you copy the most current opencpn.lib, expat.lib files from the most recently fetched & compiled Opencpn to the build directory, from the "build" directory issue:

cmake -T v120_xp ..   <---sets up cmakelists.txt
cmake --build .    <---compiles debug, Note the "." period
cmake --build . --config release   <---compiles release
cpack                             <---builds the NSIS Release


