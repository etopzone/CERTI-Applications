#define	IBM		1
#define	APL		0

#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XP_Trace.h"

extern	int		gNbClasse;			//# classes 
extern	int		gNbAttrib;			//# attribut 
extern	sCLA		*CLA;					//HHLA Class core			
extern	sHLA		*HLA;					//HLA Attribute core			
extern	sPHI		*PHI;					//			

void		tick();
float		getPlaneDatas(float, float, int, void*);

//-----------------------------------------------------------------------------XPluginStart
PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
createFedere();

strcpy(outName,"HLA");
strcpy(outSig, "HLA-CERTI");
strcpy(outDesc,"PHIDJET PlugIn");

for (short i= 0; i < gNbClasse; i++) 
	if (!strcmp(CLA[i].cName,"PhiDjet"))
		{
		short k= CLA[i].ptAttribute;
		for (short j= 0; j < CLA[i].nbAttribute; j++, k++)
			PHI[j].xpHandle= (int)XPLMFindDataRef(PHI[j].Name);
		}

XPLMRegisterFlightLoopCallback(	getPlaneDatas,		//Callback
											-1,					//Interval (next frame)
											NULL);				//Refcon not used

return 1;
}

//-----------------------------------------------------------------------------XPluginStop
PLUGIN_API void XPluginStop(void)
{
destroyFederate();
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

float	getPlaneDatas(	float	inElapsedSinceLastCall,    
							float	inElapsedTimeSinceLastFlightLoop,    
							int	inCounter,    
							void*	inRefcon)
{
tick();										//Fire the possible RTI CallBack
return -1.0;								//Return -1.0 to indicate that we want to be called for the next trame
}