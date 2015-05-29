Readme.doc
---------------

ocpn_draw_pi  
--------------------------------


Compiling for Windows
------------------------------  
1. Must be installed with the most current version of Opencpn V4.1 beta as it is being actively developed.
2. Must be compiled with the most recent version of opencpn.lib from V4.1 beta, placed into the "build" directory.
3. Must be compiled when on the origin/boundary branch which is the branch being worked upon currently.
====================
First from the command prompt change directory to your plugin compile directory, for me that is
cd C:\Users\Frederick\Documents\OpenCPN-Data\o-plugin

Then git clone Jon's  jongough repository named ocpn_draw_pi
git clone git://github.com/jongough/ocpn_draw_pi.git  ocpn_draw_pi

Then
cd ocpn_draw_pi
git fetch -all
git branch -va 

Then to set up the local branch using the remote

If you want the latest code:
	git checkout -b boundary origin/boundary
	Result:  Branch boundary set up to track remote branch boundary from origin.
	Switched to a new branch 'boundary'
If you want the more stable code:
	git checkout -b master origin/master
	Result:  Branch master set up to track remote master from origin.
	Switched to a new branch 'master'



Next copy the file "opencpn.lib" from the most recent Opencpn Version 4.1
to the 'build' directory.

Then
cmake -T v120_xp ..
cmake --build .
cmake --build . --config release

If you have NSIS installed
cpack

Then install the plugin as Debug or Release version (appropriately) into the current OpenCPN v4.1
----------
