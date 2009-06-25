#ifndef	XP_CERTI_H
#define	XP_CERTI_H

#define	NBDATA	6

#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#define	NB_IP 25
#define	NBCAR 300

#include "XP_HLA.h"

//-----------------------------------------------------------------------------XPluginStart

//=============================================================================CLASS Federe
class Federe : public RTI::RTIambassador, public NullFederateAmbassador
{
public:
	Federe() throw (RTI::RTIinternalError) {}
	~Federe() throw (RTI::FederateInternalError) {}

	void		destroyFederation	();
	void		destroyFederate	();			//

	char		readFileInit		();					/*!< Description file				*/
	char		createFederation	();					/*!< Create Fedération				*/
	bool		joinFederation		();					/*!< Join fédération					*/
	void		subscribeAttVal	();					/*!< Subscribe Attributes Value	*/

	void		setFederateName(char *nameGen)
											{ strcpy(federateName,nameGen);}

	char*		getFederationDesc()	{ return(federationDesc);}
	char*		getFederationName()	{ return(federationName);}
	char*		getFederateName()		{ return(federateName);}

  /**
   * RAV normative API overload from FederateAmbassador.
   * Will be called by the RTI when an RAV message is received by the Federate.
   * @param[in] oHandle the object handle (the instance)
   * @param[in] theAttributes Attributs list
   * @param[in] theTag Tag Name Federate
   * @throw RTI::ObjectNotKnown the object is unknown
   */
	void		reflectAttributeValues(	RTI::ObjectHandle									oHandle,
												const RTI::AttributeHandleValuePairSet&	theAttributes,
												const char											*theTag)
												throw (	RTI::ObjectNotKnown,				RTI::AttributeNotKnown,
															RTI::FederateOwnsAttributes,
															RTI::FederateInternalError);

private:
//=============================================================================CALLBACKS

protected:

char		federationDesc[NBCAR+1];				//
char		federationName[NBCAR+1];
char		dictionaryName[NBCAR+1];				//Dictionary XPlane<>FOM for activated attributes
char		federateName[NBCAR+1];
char		IP_RTIg[NB_IP];
};
#endif
