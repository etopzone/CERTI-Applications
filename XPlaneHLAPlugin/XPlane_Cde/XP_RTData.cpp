#include <iostream>
#include <memory>
#include <string>

#define	IBM		1
#define	APL		0

#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

#include "XP_Trace.h"

using std::string;
using std::auto_ptr;

extern	FILE		*fLog;				//Log File
extern	Federe	*theFederate ;	//Ambassador

extern	int		gNbClasse;			//# classes
extern	int		gNbAttrib;			//# classes
extern	sCLA		*CLA;					//HHLA Class core
extern	sHLA		*HLA;					//HLA Attribute core
extern	sPHI		*PHI;					//PhiDJet

#define	NB_DATA		1024

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++REFELECT ATTRIBUTE VALUE
void tick()
					{ theFederate->tick();}		//Fire the possible RTI reflectAttributeValues CallBack

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++REFELECT ATTRIBUTE VALUE
void Federe::reflectAttributeValues(RTI::ObjectHandle									oHandle,				//Instance
												const RTI::AttributeHandleValuePairSet&	theAttributes,		//Attributs list
												const char											*theTag)				//Tag Name Federate
												throw (	RTI::ObjectNotKnown,					RTI::AttributeNotKnown,
															RTI::FederateOwnsAttributes,
															RTI::FederateInternalError)
{
RTI::ULong		nbVPS= theAttributes.size();
char				bVal[NB_DATA];
float				fVal;
double			dVal;
int				iVal;

try
{
for (RTI::ULong idVPS= 0; idVPS < nbVPS; idVPS++)
	{//
	RTI::AttributeHandle		theH= theAttributes.getHandle(idVPS);
	RTI::ULong					theS= theAttributes.getValueLength(idVPS);

	if (theS < NB_DATA)
		{
		for (short lVAL= 0; lVAL < gNbAttrib; lVAL++) if (theH == HLA[lVAL].aHandle)
			{
			XPLMDataRef	xpHandle= (XPLMDataRef)PHI[lVAL].xpHandle;

			if (HLA[lVAL].format[3] == '1')
				switch (HLA[lVAL].format[0])
					{
					case 'I': theAttributes.getValue(idVPS, (char*)&iVal, theS); XPLMSetDatai(xpHandle,iVal); break;
					case 'F': theAttributes.getValue(idVPS, (char*)&fVal, theS); XPLMSetDataf(xpHandle,fVal); break;
					case 'D': theAttributes.getValue(idVPS, (char*)&dVal, theS); XPLMSetDatad(xpHandle,dVal); break;
					}
			else
				switch (HLA[lVAL].format[0])
					{
					case 'B': theAttributes.getValue(idVPS, (char*)&bVal, theS); XPLMSetDatab( xpHandle,bVal,0,theS); break;
					case 'I': theAttributes.getValue(idVPS, (char*)&iVal, theS); XPLMSetDatavi(xpHandle,&iVal,0,1); break;
					case 'F': theAttributes.getValue(idVPS, (char*)&fVal, theS); XPLMSetDatavf(xpHandle,&fVal,0,1); break;
					}

			break;
	}	}	}

printf("\n");
}
catch (RTI::ObjectNotKnown)
	{
	fprintf(fLog,"!!Error (reflectAttributeValues): ObjectNotKnown\n");
	}
catch (RTI::AttributeNotKnown)
	{
	fprintf(fLog,"!!Error (reflectAttributeValues): AttributeNotKnown\n");
	}
catch (RTI::FederateOwnsAttributes)
	{
	fprintf(fLog,"!!Error (reflectAttributeValues): FederateOwnsAttributes\n");
	}
catch (RTI::InvalidFederationTime)
	{
	fprintf(fLog,"!!Error (reflectAttributeValues): InvalidFederationTime\n");
	}
catch (RTI::FederateInternalError)
	{
	fprintf(fLog,"!!Error (reflectAttributeValues): FederateInternalError\n");
	}

fflush(fLog);
}
