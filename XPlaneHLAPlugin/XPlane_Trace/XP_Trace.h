#ifndef	XP_CERTI_H
#define	XP_CERTI_H

#define	NBDATA	6

#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#define	NB_IP 25
#define	NBCAR 300

//enum		{ leX,leY,leZ,Phi,The,Psi} dataPlane;

//-----------------------------------------------------------------------------XPluginStart

//=============================================================================CLASS Federe
class Federe : public RTI::RTIambassador, public NullFederateAmbassador
{
public:
	Federe() throw (RTI::RTIinternalError) {}
	~Federe() throw (RTI::FederateInternalError) {}
	
	void		destroyFederation	();
	void		destroyFederate	();			//
	
	RTI::ObjectClassHandle	IDClass;	
	RTI::ObjectHandle			HObject;
	RTI::AttributeHandle		ID_Att[NBDATA];
	
	char		readFileInit		();			//Description file
	bool		createFederation	();			//Create Fedération
	bool		joinFederation		();			//Join fédération
	void		subscribeAttVal	();			//Subscribe Attributes Value

	void		reflectAttributeValues(RTI::ObjectHandle									theObject, 
												const RTI::AttributeHandleValuePairSet&	theAttributes,
												const char											*theTag);
	
	void		setFederateName(char *nameGen) { strcpy(federateName,nameGen);}
											
	char*		getFederationDesc()				{ return(federationDesc);}
	char*		getFederationName()				{ return(federationName);}
	char*		getFederateName()					{ return(federateName);}

private:
//=============================================================================CALLBACKS

protected:	
	char		federationDesc[NBCAR+1] ;
	char		federationName[NBCAR+1];
	char		federateName[NBCAR+1];
	char		IP_RTIg[NB_IP];
};
#endif 