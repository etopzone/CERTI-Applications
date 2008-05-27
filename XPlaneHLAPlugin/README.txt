Federation HLA - XPlane

HLA (High Level Architecture) is a standard (IEE1516/HLA 1.3)  for distributed simulation.

This plug-in should be usable with all HLA RTI even if it has only been thoroughly
tested with the CERTI Open Source RTI https://savannah.nongnu.org/projects/certi/).

For CERTI, you have to define the "CERTI_HOME" env var pointing the path to CERTI install folder.

The source tree has the following layout:

X/Misc Folder-------------------------------------------------------------------
initCERTI.txt 
	{
	XPlane
	D:\DVP Cert\CertiSav\installed\share\federations\XPlane.fed		//where is FOM?
	192.000.000.001								//
	}
XPlane.fed	(file HLA Federation Object Model : FOM) 


A/ XPlane_Plugin Folder: XPlane Federate ---------------------------------------
	{
	XP_Certi.h
	XPlane_InitDll.cpp 	//Xplane interface
	XPlane_RunDll.cpp 
	XP_Init.cpp  		//CERTI interface
	XP_Run.cpp
	CMakeLists.txt 		//for building "Federate XPlane"
	}

B/ XPlane_Trace Folder: Trace Federate -----------------------------------------
	{
	XP_Trace.h
	XP_Trace.cpp 
	XP_RTData.cpp
	CMakeLists.txt 		//for building "Federate Trace "
	}

Run CMake.... (see www.cmake.org)
Build XPlaneFederate.dll
Build XplaneTrace.exe

Move the file XPlaneFederate.dll into folder "XXXX/Resources/plugins"
XPlaneFederate.dll must be renamed XPlaneFederate.xpl (normal XPlane extension)
Copy file initCERTI.txt from /Misc to /XXXX 
!!! /XXXX is the XPlane root folder !!!

Copy file initCERTI.txt from /Misc to /YYYY
!!! /YYYY is the Trace build folder !!!

//-------------------------------------------------------------------------------
Have a look at https://savannah.nongnu.org/projects/certi/
//-------------------------------------------------------------------------------

