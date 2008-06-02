#include <iostream>
#include <memory>
#include <string>

#include "XP_Trace.h"

using std::string;
using std::auto_ptr;

#define	FHFREE			-1
extern	FILE				*fLog;								//Log File Federate
extern	Federe			*FedereViewer;						//Ambassador

extern	enum	{ leX,leZ,leY, Phi,The,Psi} XPLane;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++REFELECT ATTRIBUTE VALUE
void Federe::reflectAttributeValues(RTI::ObjectHandle									oHandle,				//Instance
												const RTI::AttributeHandleValuePairSet&	theAttributes,		//Attributs list
												const char											*theTag)				//Tag Name Federate
								throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes,
										 RTI::InvalidFederationTime, RTI::FederateInternalError)

{
float					value[NBDATA];
RTI::ULong			valueLength;

try 
{
for (long idT= 0; idT < 6; idT++)
	{
	long		idH;											
	int		leH= theAttributes.getHandle(idT);
	
	if (leH == ID_Att[leX])			idH= leX;
	else if (leH == ID_Att[leY])	idH= leZ;
	else if (leH == ID_Att[leZ])	idH= leY;
	else if (leH == ID_Att[Phi])	idH= Phi;
	else if (leH == ID_Att[The])	idH= The;
	else if (leH == ID_Att[Psi])	idH= Psi;
	
	theAttributes.getValue(idT, (char*)&value[idH], valueLength);
	
	printf("V%02d %10.3f ", idH,value[idH]);
	}
	
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