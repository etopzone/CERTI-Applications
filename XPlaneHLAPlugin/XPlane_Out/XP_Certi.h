#ifndef	XP_CERTI_H
#define	XP_CERTI_H

#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#define	NB_IP 25									//IP Size
#define	NBCAR 300								//Federation, Federate : name[NBCAR] 

//-----------------------------------------------------------------------------XPluginStart

extern "C" 
{
int	createFedere();
void	destroyFederate();
void	upDateValues();
}

typedef struct structXPL
{//XPLane Part----------------------------------------------
char							XPName[NBCAR+1];			//Attribute Name
XPLMDataRef					xpHandle;					//Handle
XPLMDataTypeID				XPType;						//Attribute Type
char							format[5];					//Format
int							XPValueI;					//Value
float							XPValueF;
double						XPValueD;

int							XPLengthB;					//Size of Block
int							*XPBlockI;					//Block
float							*XPBlockF;
BYTE							*XPBlockB;
} sXPL;

#include	"XP_HLA.h"

//=============================================================================CLASS Federe
class Federe : public RTI::RTIambassador, public NullFederateAmbassador
{
public:
	Federe()	throw (RTI::RTIinternalError) {}
	~Federe() throw (RTI::FederateInternalError) {}

	void		destroyFederation();

	char		readFileInit		();					/*!< Description file				*/
	char		createFederation	();					/*!< Create Fedération				*/
	bool		joinFederation		();					/*!< Join fédération					*/
	void		registreObjAndpublishVal	();		/*!< Registre Object and Value	*/

	void		setFederateName(char *nameGen,short id)
											{ sprintf(federateName,"%s%03hx",nameGen,id);}

	char*		getFederationDesc()	{ return(federationDesc);}
	char*		getFederationName()	{ return(federationName);}	
	char*		getFederateName()		{ return(federateName);}
	
protected:		
	
	char		federationDesc[NBCAR+1];				//
	char		federationName[NBCAR+1];
	char		dictionaryName[NBCAR+1];				/*!< Dictionary XPlane<>FOM for activated attributes */
	char		federateName[NBCAR+1];
	char		IP_RTIg[NB_IP];
};

#endif 