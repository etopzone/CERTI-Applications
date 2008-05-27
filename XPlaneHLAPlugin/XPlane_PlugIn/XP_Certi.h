#ifndef	XP_CERTI_H
#define	XP_CERTI_H

#define	NBDATA	6										//TODO : Number of parameters
//enum		{ leX,leY,leZ,Phi,The,Psi} dataPlane;

#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#define	NB_IP 25											//IP Size
#define	NBCAR 300										//Federation, Federate : name[NBCAR] 

//-----------------------------------------------------------------------------XPluginStart

extern "C" 
{
int	createFedere();
void	destroyFederate();
void upDateValues(float *fData, short nbE);
}

//=============================================================================CLASS Federe
class Federe : public RTI::RTIambassador, public NullFederateAmbassador
{
public:
	Federe()	throw (RTI::RTIinternalError) { pass= false;}
	~Federe() throw (RTI::FederateInternalError) {}
	void		destroyFederation();

	bool							pass;						//for xyz initial values
	RTI::ObjectClassHandle	IDClass;	
	RTI::ObjectHandle			HObject;
	
	static	const	int		nbData= NBDATA;
	RTI::AttributeHandle		ID_Att[nbData];
	
	char		readFileInit		();					//Description file
	bool		createFederation	();					//Create Fedération
	bool		joinFederation		();					//Join fédération
	void		registreObjAndpublishVal();			//Registre Object and Value

	void		setFederateName(char *nameGen,short id)
											{ sprintf(federateName,"%s%03hx",nameGen,id);}

	char*		getFederationDesc()	{ return(federationDesc);}
	char*		getFederationName()	{ return(federationName);}	
	char*		getFederateName()		{ return(federateName);}
	
	void		setMemData(float val, short id)	{ data[id]= val;}
	float		getMemData(short id)	{ return(data[id]);}

protected:		
	float		data[nbData];								//Xplane Datas
	
	char		federationDesc[NBCAR+1] ;
	char		federationName[NBCAR+1];
	char		federateName[NBCAR+1];
	char		IP_RTIg[NB_IP];
};

#endif 