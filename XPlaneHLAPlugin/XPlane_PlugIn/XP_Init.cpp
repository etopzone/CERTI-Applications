#include <iostream>
#include <memory>
#include <string>

#include "XP_Certi.h"

using std::string;
using std::auto_ptr;

FILE		*fLog;						//Log File
Federe	*FedereXPlane ;			//Ambassador

//=============================================================================MAIN
int createFedere()
{//Load during the first call from XPlane
int		i;
int		nbcar= NBCAR;

fLog= fopen("FedXPlane.txt","wt");

try
	{//Ambassador creation (and then rtia will begin)
	FedereXPlane= new Federe();
	}
	
catch ( RTI::Exception &e) 
	{
	fprintf(fLog,"!!Error : Can't create Ambassador %s\n",e._reason) ;
	exit(-1);
	}
fprintf(fLog,"Ambassador have been created, so rtia is running\n"); fflush(fLog);

try{
//=============================================================================Read Descriptor
if (FedereXPlane->readFileInit())
	{//Read configFile : federation name, fed file, IP RTIg
	fprintf(fLog,"Error : File initCERTI.txt \n"); fflush(fLog);
	exit(-1);
	}

//=============================================================================Creating Federation
fprintf(fLog,"About creating federation execution\n"); fflush(fLog);

bool	is_created= FedereXPlane->createFederation();
if (is_created )	fprintf(fLog,"Creation of federation execution right. %s\n",	FedereXPlane->getFederationName());
else					fprintf(fLog,"Creation of federation execution failed. %s\n",	FedereXPlane->getFederationName());
fflush(fLog);

//=============================================================================Creating Federation
bool	is_joined= false ;
short	cpt= 0; do	
	{
	FedereXPlane->setFederateName("XPlane",cpt);										//Federate Name = XPlaneXXX
	is_joined= FedereXPlane->joinFederation();
	if (is_joined) 
		fprintf(fLog,"Federate %s has joined federation %s\n",FedereXPlane->getFederateName(),FedereXPlane->getFederationName());
	else if (cpt > 0x0FFF) 
		{//Federate no admit -> Exit
		fprintf(fLog,"!!Error : Creation of federate failed.\n");
		return(1);
		}
	cpt++;
	} while (is_joined == false);
 fflush(fLog);
 	
//=============================================================================Publish Objet/Attributs
FedereXPlane->registreObjAndpublishVal();
return(0);
}

catch ( RTI::Exception &e) 
	{
	fprintf(fLog,"!!Error : Can't launch Federate : %s\n",e._reason) ;
	exit(-1);
}	}
//-----------------------------------------------------------------------------MAIN

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Read Descriptor
char Federe::readFileInit()
{
FILE	*theFile= fopen("initCERTI.txt","r"); if (theFile == NULL) return(1);

fscanf(theFile,"%s\n",federationName);	//Federation name :-)
fgets(federationDesc,259,theFile);		//Fedfile (\installed\share\federations\xxxxx.fed)
fscanf(theFile,"%s\n",IP_RTIg);			//Server RTIG IP address

short	lng= strlen(federationDesc) - 1;	//Delete "line feed" if exists
if (federationDesc[lng] == 10) federationDesc[lng]= 0;
return(0);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Create Federation
bool Federe::createFederation()
// Create a federation execution name giving :
// federationName : federation execution name
// federationDesc : file Federation descriptor
// Return : true if federation execution created
//          false if creation failed
// Note 5 exceptions may raise : 
// CouldNotOpenFED,RTIinternalError,ConcurrentAccessAttempted,ErrorReadingFED,FederationExecutionAlreadyExists
{
bool is_created= false ;

	try {
	FedereXPlane->createFederationExecution(federationName,federationDesc);
	std::cout << "federation "<<federationName <<" created."<<std::endl;
	is_created= true ;
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
	FedereXPlane->joinFederationExecution(federateName,federationName,FedereXPlane);
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
FedereXPlane->deleteObjectInstance(FedereXPlane->HObject,FedereXPlane->getFederateName());
FedereXPlane->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
fprintf(fLog,"Destroy Federate. \n"); fflush(fLog);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++Destroy Federation
void Federe::destroyFederation()
{
try 
{
FedereXPlane->destroyFederationExecution(federationName);
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