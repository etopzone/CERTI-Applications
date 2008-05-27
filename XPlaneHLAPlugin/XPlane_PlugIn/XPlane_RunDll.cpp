#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XP_Certi.h"

#define				NBDATA	6
XPLMDataRef			hPlane[NBDATA];

float		getPlaneDatas(float, float, int, void*);
enum		{ leX,leY,leZ,Phi,The,Psi} dataPlane;											//TODO : datas

//-----------------------------------------------------------------------------XPluginStart
PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
createFedere();

strcpy(outName,"HLA");
strcpy(outSig, "HLA-CERTI");
strcpy(outDesc,"CERTI PlugIn");

hPlane[leX]= XPLMFindDataRef("sim/flightmodel/position/local_x");					//TODO : XPlane Parameters
hPlane[leY]= XPLMFindDataRef("sim/flightmodel/position/local_y");
hPlane[leZ]= XPLMFindDataRef("sim/flightmodel/position/local_z");

hPlane[Phi]= XPLMFindDataRef("sim/flightmodel/position/phi");
hPlane[The]= XPLMFindDataRef("sim/flightmodel/position/theta");
hPlane[Psi]= XPLMFindDataRef("sim/flightmodel/position/psi");

XPLMRegisterFlightLoopCallback(	getPlaneDatas,		//Callback
											1.0,					//Interval
											NULL);				//Refcon not used
return 1;
}

//-----------------------------------------------------------------------------XPluginStop
PLUGIN_API void	XPluginStop(void)
{
destroyFederate();
XPLMUnregisterFlightLoopCallback(getPlaneDatas, NULL);
}

//-----------------------------------------------------------------------------XPluginDisable
PLUGIN_API void XPluginDisable(void)
{
}

//-----------------------------------------------------------------------------XPluginEnable
PLUGIN_API int XPluginEnable(void)
{
return 1;
}

//-----------------------------------------------------------------------------XPluginReceive
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID	inFromWho, long inMessage, void *inParam)
{
}


//-----------------------------------------------------------------------------getPlaneDatas
float	getPlaneDatas(	float	inElapsedSinceLastCall,    
							float	inElapsedTimeSinceLastFlightLoop,    
							int	inCounter,    
							void*	inRefcon)
{
float	fData[NBDATA], elapsed = XPLMGetElapsedTime();		//read the sim's time and the data

fData[leX]= XPLMGetDataf(hPlane[0]);														//TODO : XPlane Parameters
fData[leY]= XPLMGetDataf(hPlane[1]);
fData[leZ]= XPLMGetDataf(hPlane[2]);
fData[Phi]= XPLMGetDataf(hPlane[3]);
fData[The]= XPLMGetDataf(hPlane[4]);
fData[Psi]= XPLMGetDataf(hPlane[5]);

upDateValues(fData,NBDATA);
return 1.0;																//Return 1.0 to indicate that we want to be called again in 1 second
}