Federation HLA - XPlane

XP:HLA is an free plugin for X-Plane simulator allowing to build an HLA(*) federation.

XP:HLA has been tested with X-Plane 8.61 and over on Windows XP.

This plug-in should be usable with all HLA RTI even if it has only been thoroughly
tested with the CERTI Open Source RTI https://savannah.nongnu.org/projects/certi/).

XPlane_Plugin allows to send Datarefs XPlane to the federation. 
List of these datas are defined within the source (XPluginStart) 

XPlane_Trace is a short example for receiving the datas.

IMPORTANT : For CERTI, you have to define the "CERTI_HOME" env var pointing the path to CERTI install folder.

a)	Unzip files within XXX folder
b)	Run CMake 			//Source code : XXX folder - Binaries : YYY folder
	You have to find the xPlane folder if CMake can't do it.

//-------------------------------------------------------------------------------CMake Process
Source (input)		XXX/Misc
		XXX/XPlane_Plugin
		XXX/XPlane_Trace 
		XXX/CMakeLists.txt
		XXX/others files...
Build (output)	YYY/XPlane_PlugIn
		YYY/XPlane_Trace
		YYY/CMakeFiles
		YYY/XPlaneHLAPlugIn.	>>	IDE Project (ie .sln for Visual C 2005)
		YYY/other files

//-------------------------------------------------------------------------------Build Process
c)	Run your IDE + Build the project
Build		YYY/XPlane_PlugIn/Release/XPlaneFederate.dll
		YYY/XPlane_Trace/Release/XPlaneTrace.exe


d)  	Move the file XPlaneFederate.dll into folder "XPLANE/Resources/plugins"
    	XPlaneFederate.dll must be renamed XPlaneFederate.xpl (normal XPlane extension)

e)   	Copy file initCERTI.txt from /Misc to YYY/XPlane_PlugIn/Release/
   	Copy file initCERTI.txt from /Misc to XPLANE

//-------------------------------------------------------------------------------
Have a look at https://savannah.nongnu.org/projects/certi/
//-------------------------------------------------------------------------------

(*) HLA (High Level Architecture) is a standard for distributed simulation.

PS : Take a look at "http://download.savannah.gnu.org/releases/certi/contrib/" for a "FlightGear-HLA plug-in"
