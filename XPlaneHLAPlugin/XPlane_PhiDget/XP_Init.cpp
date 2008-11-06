#include <iostream>
#include <memory>
#include <string>
#include <map>

#include "XP_Certi.h"

using std::string;

FILE						*fLog;				//Log File
Federe					*theFederate;		//Ambassador

int						gNbClasse;			//# classes 
int						gNbAttrib;			//# classes 
sCLA						*CLA;					//HHLA Class core			
sHLA						*HLA;					//HLA Attribute core			
sPHI						*PHI;					//			


//=============================================================================MAIN
int createFedere()
{//Load during the first call from XPlane
int		nbcar= NBCAR;

fLog= fopen("LogPhyDJet.txt","wt");

try
	{//Ambassador creation (and then rtia will begin)
	theFederate= new Federe();
	}
	
catch ( RTI::Exception &e) 
	{
	fprintf(fLog,"!!Error : Can't create Ambassador %s\n",e._reason) ;
	exit(-1);
	}
fprintf(fLog,"Ambassador have been created : RTIa is running\n"); fflush(fLog);

try{
//=============================================================================Read Descriptor
if (theFederate->readFileInit())
	{//Read configFile : federation name, fed file, IP RTIg
	fprintf(fLog,"!!Error : File XPlanePhiDget.txt \n"); fflush(fLog);
	exit(-1);
	}

//=============================================================================CREATING FEDERATION
fprintf(fLog,"About creating federation execution\n"); fflush(fLog);

char	is_created= theFederate->createFederation();
if (is_created < 0) 
	{//Error : exit from plug-in
								 fprintf(fLog,"Creation of federation execution failed. %s\n",	theFederate->getFederationName());
	if (HLA) free(HLA); 
//	if (XPL) free(XPL);
	exit(0xFBAD);
	}
else if (is_created > 0) fprintf(fLog,"Creation of federation execution right. %s\n",	theFederate->getFederationName());
else							 fprintf(fLog,"Federation execution exists. %s\n",					theFederate->getFederationName());
fflush(fLog);

//=============================================================================Creating Federation
theFederate->setFederateName("PhiDJet");													//Federate Name = PhiDJet
if (!theFederate->joinFederation()) 
	{//Federate no admit -> Exit
	fprintf(fLog,"Creation of federateName failed.\n"); fflush(fLog);
	return(false);
	}
	
fprintf(fLog,"Federate %s has joined federation %s\n",theFederate->getFederateName(),theFederate->getFederationName());
fflush(fLog);
 	
//=============================================================================Publish Objet/Attributs
theFederate->registreObjAndpublishVal();
return(0);
}

catch ( RTI::Exception &e) 
	{
	fprintf(fLog,"!!Error : Can't launch Federate : %s\n",e._reason) ;
	exit(-1);
}	}
//-----------------------------------------------------------------------------MAIN

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Create Federation
char Federe::createFederation()
// Create a federation execution name giving :
// federationName : federation execution name
// federationDesc : file Federation descriptor
// Return : true if federation execution created
//          false if creation failed
// Note 5 exceptions may raise : 
// CouldNotOpenFED,RTIinternalError,ConcurrentAccessAttempted,ErrorReadingFED,FederationExecutionAlreadyExists
{
char is_created= -1;																														//Error : Exit																	

try {
	theFederate->createFederationExecution(federationName,federationDesc);
	std::cout << "federation "<<federationName <<" created."<<std::endl;
																														is_created= 0 ;	//Created : OK
	}
catch (RTI::Exception &e)
	{ 
	fprintf(fLog,"!!Error : %s ",e._name);
	
	if (strcmp(e._name,"CouldNotOpenFED")==0 || strcmp(e._name,"ErrorReadingFED")==0)
		{
		fprintf(fLog,"!!Error : file %s : %s\n",Federe::federationDesc,e._reason);
		}
	else if (strcmp(e._name,"FederationExecutionAlreadyExists")==0)
		{
		fprintf(fLog,"!!Error : Federation : %s\n",e._reason);
																														is_created= 1;		//Exit : OK
		}
	else
		fprintf(fLog,"!!Error : %s\n",e._reason);
	 fflush(fLog);
	}

return is_created ;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Join Federation
bool Federe::joinFederation()
// Create a federation execution name giving :
// federationName : federation execution name
// fileName : FED file name
// Return : true if federation execution created
//          false if creation failed
// Note 5 exceptions may raise : CouldNotOpenFED,RTIinternalError,
//   ConcurrentAccessAttempted,ErrorReadingFED,FederationExecutionAlreadyExists
{
bool success= false ;

	try {
	theFederate->joinFederationExecution(federateName,federationName,theFederate);
	fprintf(fLog,"Federation joined\n");
	}
catch (RTI::Exception &e)
	{ 
	fprintf(fLog,"!!Error : %s ",e._name);
	return false ;
	}

fflush(fLog);
return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Destroy Federate
void destroyFederate()
{//RESIGNING FEDERATION EXECUTION
for (short j= 0; j < gNbClasse; j++)
	theFederate->deleteObjectInstance(CLA[j].cHandle,theFederate->getFederateName());

theFederate->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
fprintf(fLog,"Destroy Federate. \n"); fflush(fLog);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Destroy Federation
void Federe::destroyFederation()
{
try 
{
theFederate->destroyFederationExecution(federationName);
fprintf(fLog,"Federation has been destroyed.\n");
}
	
catch (RTI::FederatesCurrentlyJoined)
	{ 
	fprintf(fLog,"!!Error (Destroy): federates currently joined\n");
	}
catch (RTI::FederationExecutionDoesNotExist)
	{ 
	fprintf(fLog,"!!Error (Destroy): federation does not exist (maybe has been yet destroyed)\n");
	}
catch (RTI::ConcurrentAccessAttempted)
	{ 
	fprintf(fLog,"!!Error (Destroy): Concurrent Access Attempted\n");
	}
catch (RTI::RTIinternalError)
	{ 
	fprintf(fLog,"!!Error (Destroy): RTI internal error\n");
	}

fflush(fLog);
}