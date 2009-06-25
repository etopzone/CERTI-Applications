#include <iostream>
#include <memory>
#include <string>

#include "XP_Trace.h"

#include "XP_Trace.h"

using std::string;
using std::auto_ptr;

extern	FILE		*fLog;				//Log File
extern	Federe	*theFederate ;	//Ambassador

extern	int		gNbClasse;			//# classes
extern	int		gNbAttrib;			//# classes
extern	sCLA		*CLA;					//HHLA Class core
extern	sHLA		*HLA;					//HLA Attribute core
//extern	sXPL		*XPL;					//Xplane core

#define	NB_DATA		1024
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++REFELECT ATTRIBUTE VALUE
void Federe::reflectAttributeValues(RTI::ObjectHandle									oHandle,				//Instance
												const RTI::AttributeHandleValuePairSet&	theAttributes,		//Attributs list
												const char											*theTag)				//Tag Name Federate
												throw (	RTI::ObjectNotKnown,					RTI::AttributeNotKnown,
															RTI::FederateOwnsAttributes,
															RTI::FederateInternalError)
{
RTI::ULong		nbVPS= theAttributes.size();
char				bVal[NB_DATA],theString[50];
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
			switch (HLA[lVAL].format[0])
				{
				case 'B': theAttributes.getValue(idVPS, (char*)&bVal, theS); printf("%s V=%s\n",		HLA[lVAL].aName,bVal); break;
				case 'I': theAttributes.getValue(idVPS, (char*)&iVal, theS); printf("%s V=%10d\n",	HLA[lVAL].aName,iVal); break;
				case 'F': theAttributes.getValue(idVPS, (char*)&fVal, theS); printf("%s V=%10.3f\n",HLA[lVAL].aName,fVal); break;
				case 'D': theAttributes.getValue(idVPS, (char*)&dVal, theS); printf("%s V=%10.3f\n",HLA[lVAL].aName,dVal); break;
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
