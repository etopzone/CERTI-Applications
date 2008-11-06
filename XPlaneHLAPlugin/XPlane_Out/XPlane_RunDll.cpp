#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XP_Certi.h"

extern	int		gNbClasse;			//# classes 
extern	int		gNbAttrib;			//# attribut 
extern	sCLA		*CLA;					//HHLA Class core			
extern	sHLA		*HLA;					//HLA Attribute core			
extern	sXPL		*XPL;					//Xplane core			

float		getPlaneDatas(float, float, int, void*);

//-----------------------------------------------------------------------------XPluginStart
PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
createFedere();

strcpy(outName,"HLA");
strcpy(outSig, "HLA-CERTI");
strcpy(outDesc,"CERTI PlugIn");

for (short i= 0; i < gNbAttrib; i++)
	{
	char	format;
	
	XPL[i].xpHandle=	XPLMFindDataRef(XPL[i].XPName); if (!XPL[i].xpHandle) return 1;
	XPL[i].XPType=		XPLMGetDataRefTypes(XPL[i].xpHandle);			//Test

	sscanf(XPL[i].format,"%c%03d",&format,&XPL[i].XPLengthB);
	switch (format)
		{
		case 'D': if(XPL[i].XPLengthB == 1)	//------------------------Double
				XPL[i].XPType= xplmType_Double;
			else																					return 1;
			break;
					
		case 'F': if(XPL[i].XPLengthB == 1)	//------------------------Float
				XPL[i].XPType= xplmType_Float;
			else
				{
				XPL[i].XPType= xplmType_FloatArray;
				XPL[i].XPBlockF= (float*)malloc(sizeof(float) * XPL[i].XPLengthB);
				}
			break;
					
		case 'I': if(XPL[i].XPLengthB == 1)	//------------------------Integer
				XPL[i].XPType= xplmType_Int;
			else
				{
				XPL[i].XPType= xplmType_IntArray;
				XPL[i].XPBlockI= (int*)malloc(sizeof(int) * XPL[i].XPLengthB);
				}
			break;
					
		case 'B':if(XPL[i].XPLengthB)			//------------------------Block
				{
				XPL[i].XPType= xplmType_Data;
				XPL[i].XPBlockB= (BYTE*)malloc(sizeof(BYTE) * XPL[i].XPLengthB);
				}
			else																					return 1;
			break;			
		}
	}

XPLMRegisterFlightLoopCallback(	getPlaneDatas,		//Callback
											1.0,					//Interval
											NULL);				//Refcon not used
return 1;
}

//-----------------------------------------------------------------------------getPlaneDatas
/* A single 4-byte integer, native endian.                                     */
/* A single 4-byte float, native endian.                                       */
/* A single 8-byte double, native endian.                                      */
/* An array of 4-byte floats, native endian.                                   */
/* An array of 4-byte integers, native endian.                                 */
/* A variable block of data.                                                   */

float	getPlaneDatas(	float	inElapsedSinceLastCall,    
							float	inElapsedTimeSinceLastFlightLoop,    
							int	inCounter,    
							void*	inRefcon)
{
float	elapsed = XPLMGetElapsedTime();		//read the sim's time and the data

for (short i= 0; i < gNbAttrib; i++) 
	{
	switch (XPL[i].XPType)
		{
		case xplmType_Int:			XPL[i].XPValueI= XPLMGetDatai(XPL[i].xpHandle);break;
		case xplmType_Float:			XPL[i].XPValueF= XPLMGetDataf(XPL[i].xpHandle);break;
		case xplmType_Double:		XPL[i].XPValueD= XPLMGetDatad(XPL[i].xpHandle);break;
		
		case xplmType_IntArray:		XPLMGetDatavi(	XPL[i].xpHandle,XPL[i].XPBlockI,0,XPL[i].XPLengthB);break;
		case xplmType_FloatArray:	XPLMGetDatavf(	XPL[i].xpHandle,XPL[i].XPBlockF,0,XPL[i].XPLengthB);break;
		case xplmType_Data:			XPLMGetDatab(	XPL[i].xpHandle,XPL[i].XPBlockB,0,XPL[i].XPLengthB);break;
		}
	}
upDateValues();
return 1.0;								//Return 1.0 to indicate that we want to be called again in 1 second
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
