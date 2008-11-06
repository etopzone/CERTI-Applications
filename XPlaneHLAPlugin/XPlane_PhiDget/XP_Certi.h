#ifndef	XP_CERTI_H
#define	XP_CERTI_H

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
void	upDateValues(char *name, int lng, float value);
}

typedef struct structPHI
{//Attribute Part------------------------------------------
char			Name[NBCAR+1];						//Name	
char			format[5];							//Format
int			xpHandle;							//Handle
int			valueI;								//Value
float			valueF;								//Value
double		valueD;								//Value
} sPHI;

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

	void		setFederateName(char *nameGen)
											{ strcpy(federateName,nameGen);}

	char*		getFederationDesc()	{ return(federationDesc);}
	char*		getFederationName()	{ return(federationName);}	
	char*		getFederateName()		{ return(federateName);}
	
protected:		
	
	char		federationDesc[NBCAR+1];				//
	char		federationName[NBCAR+1];
	char		dictionaryName[NBCAR+1];				//Dictionary XPlane<>FOM for activated attributes
	char		federateName[NBCAR+1];
	char		IP_RTIg[NB_IP];
};

#endif 