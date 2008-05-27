#include <iostream>
#include <memory>
#include <string>

#include "XP_Trace.h"

using std::string;
using std::auto_ptr;

#define	FHFREE	-1
FILE		*fLog;					//Log File Federate
Federe	*FedereViewer ;		//Ambassador

enum	{ leX,leZ,leY, Phi,The,Psi} XPLane;
//=============================================================================MAIN
int main(int argc, char **argv)
{
int		i;
int		nbcar= NBCAR;

fLog= fopen("FedXPlane.txt","wt");

try
	{//Ambassador creation (and then rtia will begin)
	FedereViewer= new Federe();
	}
catch ( RTI::Exception &e) 
	{
	fprintf(fLog,"!!Error : Can't create ambassador so : %s\n",e._reason); fflush(fLog);
	return(false);
	}
fprintf(fLog,"Ambassador have been created : RTIa is running\n"); fflush(fLog);

try{
//=============================================================================Read Descriptor
if (FedereViewer->readFileInit())
	{//Read configFile : federation name, fed file, IP RTIg
	fprintf(fLog,"!!Error : File initCERTI.txt \n"); fflush(fLog);
	return(false);
	}

//=============================================================================CREATING FEDERATION
bool	is_created= FedereViewer->createFederation();
if (is_created )	fprintf(fLog,"Creating Federation right. %s\n",	FedereViewer->getFederationName());
else					fprintf(fLog,"Creating Federation failed. %s\n",FedereViewer->getFederationName());
fflush(fLog);
 
//=============================================================================JOINING FEDERATION 
FedereViewer->setFederateName("XViewer");											//Federate Name = XViewer
if (!FedereViewer->joinFederation()) 
	{//Federate no admit -> Exit
	fprintf(fLog,"Creation of federateName failed.\n"); fflush(fLog);
	return(false);
	}
	
fprintf(fLog,"Federate %s has joined federation %s\n",FedereViewer->getFederateName(),FedereViewer->getFederationName());
fflush(fLog);
 
//=============================================================================SUBSCRIBE ATTRIBUTES
FedereViewer->subscribeAttVal();

while (1)
	{
	Sleep(900); 
	FedereViewer->tick();
	}

FedereViewer->destroyFederate();
}

catch ( RTI::Exception &e) 
	{
	fprintf(fLog,"!!Error : %s\n",e._reason); fflush(fLog);
	return(false);
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++READ DESCRIPTOR
char Federe::readFileInit()
{
FILE	*theFile= fopen("initCERTI.txt","r"); if (theFile == NULL) return(1);

fscanf(theFile,"%s\n",federationName);		//Federation name :-)
fgets(federationDesc,259,theFile);			//Fedfile (\installed\share\federations\xxxxx.fed)
fscanf(theFile,"%s\n",IP_RTIg);				//Server RTIG IP address

short	lng= strlen(federationDesc) - 1;		//Delete "line feed" if exists
if (federationDesc[lng] == 10) federationDesc[lng]= 0;
return(0);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++CREATE FEDERATION
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

try 
	{
	FedereViewer->createFederationExecution(federationName,federationDesc);
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

fflush(fLog);
return is_created ;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++JOIN FEDERATION
bool Federe::joinFederation()
// Create a federation execution name giving :
// federationName : federation execution name
// fileName : FED file name
// Return : true if federation execution created
//          false if creation failed
// Note 5 exceptions may raise : CouldNotOpenFED,RTIinternalError,
//   ConcurrentAccessAttempted,ErrorReadingFED,FederationExecutionAlreadyExists
{
try 
	{
	FedereViewer->joinFederationExecution(federateName,federationName,FedereViewer);
	fprintf(fLog,"Federation joined\n"); fflush(fLog);
	}
catch (RTI::Exception &e)
	{ 
	fprintf(fLog,"!!Error : %s ",e._name); fflush(fLog);
	return false ;
	}

return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++SUBSCRIBE ATTRIBUTE VALUES
void Federe::subscribeAttVal()
{
try
{
auto_ptr<RTI::AttributeHandleSet> attributes(RTI::AttributeHandleSetFactory::create(NBDATA));

IDClass= getObjectClassHandle("Aircraft");
ID_Att[leX]= getAttributeHandle("Att_Lat",IDClass); attributes->add(ID_Att[leX]);
ID_Att[leY]= getAttributeHandle("Att_Lon",IDClass); attributes->add(ID_Att[leY]);
ID_Att[leZ]= getAttributeHandle("Att_Hte",IDClass); attributes->add(ID_Att[leZ]);

ID_Att[Phi]= getAttributeHandle("Att_Phi",IDClass); attributes->add(ID_Att[Phi]);
ID_Att[The]= getAttributeHandle("Att_The",IDClass); attributes->add(ID_Att[The]);
ID_Att[Psi]= getAttributeHandle("Att_Psi",IDClass); attributes->add(ID_Att[Psi]);

fprintf(fLog,"Subscribe attributes of class %d : %d,%d,%d <> %d,%d,%d",IDClass,ID_Att[leX],ID_Att[leY],ID_Att[leZ],
																										 ID_Att[Phi],ID_Att[The],ID_Att[Psi]); fflush(fLog);
subscribeObjectClassAttributes(IDClass, *attributes, RTI::RTI_TRUE);
}

catch (RTI::Exception &e)
	{
	fprintf(fLog,"!!Error : %s ",e._name); fflush(fLog);
}	}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++DESTROY FEDERATE
void Federe::destroyFederate()
{//RESIGNING FEDERATION EXECUTION

FedereViewer->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
fprintf(fLog,"Destroy Federate. \n"); fflush(fLog);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++DESTROY FEDERATION
void Federe::destroyFederation()
{
try 
{
FedereViewer->destroyFederationExecution(getFederationName());
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