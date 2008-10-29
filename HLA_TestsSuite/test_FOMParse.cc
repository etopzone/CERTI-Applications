#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#include "test_FOMParse_cmdline.h"

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <stdlib.h>

#if defined(_WIN32)
#include <windows.h>
#include <algorithm>
#include <process.h>
#define	sleep(a)	      Sleep(a * 1000)
#endif

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::auto_ptr;
/**
 * The ParseFederate inherit from the classical
 * NullFederateAmbassador.
 */
class ParseFederate : public NullFederateAmbassador {

private:
	string federationName;
	string federationFile;

	RTI::ObjectClassHandle        objectClassId;
	RTI::AttributeHandle          attributeId;
	RTI::InteractionClassHandle   interactionClassId;
	RTI::ParameterHandle          parameterId;
	bool   verbose;

public:
	ParseFederate(std::string federationFile, std::string federationName, bool verbose=false) {
		this->verbose        = verbose;
		if (verbose) {
			cout << "federationFile = " << federationFile <<endl;
			cout << "federationName = " << federationName <<endl;
		}
		this->federationFile = federationFile;
		this->federationName = federationName;
	};

	virtual ~ParseFederate() throw (RTI::FederateInternalError) {
	};

	bool
	create() {
		bool retval = false;

		try {
			myRTIamb.createFederationExecution(federationName.c_str(),
					federationFile.c_str());
			if (verbose) {
				cout << "createFederationExecution SUCCESS." << endl;
			}
			retval = true;
		} catch ( RTI::FederationExecutionAlreadyExists& e) {
			cout << "--> Federation already created by another federate." << endl;
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
			<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: Unknown non-RTI exception." << endl;
		}

		return retval;
	} /* end of create */

	bool
	destroy() {
		bool retval = false;
		try {
			myRTIamb.destroyFederationExecution(federationName.c_str());
			retval = true;
		} catch (RTI::FederatesCurrentlyJoined) {
			cout << "Federates currently joined." << endl;
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
			<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: Unknown non-RTI exception." << endl;
		}
		return retval;
	} /* end of destroy */

	bool
	join(std::string federateName) {
		bool retval = false;
		try {
			if (verbose) {
				cout << "Try to join federation as <" << federateName << ">...";
			}
			myRTIamb.joinFederationExecution(federateName.c_str(),
					federationName.c_str(),
					this);
			if (verbose) {
				cout << "SUCCESS." << endl;
			}
			retval = true;
		} catch (RTI::FederateAlreadyExecutionMember& e) {
			cerr << "Federate already execution member" << endl;
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
			<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: Unknown non-RTI exception." << endl;
		}
		return retval;
	} /* end of join */

	bool
	resign() {
		bool retval = false;
		try {
			if (verbose) {
				cout << "Try to resign from federation...";
			}
			myRTIamb.resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
			if (verbose) {
				cout << "SUCCESS." << endl;
			}
			retval = true;
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
			<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: Unknown non-RTI exception." << endl;
		}
		return retval;
	} /* end of resign */

	bool
	getObjectHandles(const std::string& className, const std::string& attributeName) {
		bool retval = true;

		if (verbose) {
			cout << "Getting object handles..." <<endl;
		}
		try {
			objectClassId = myRTIamb.getObjectClassHandle(className.c_str());
			cout << "   " << className << " ID = " << objectClassId << endl;
		} catch (RTI::Exception& e) {
			cerr << "   Unable to obtain class handle for object class <"<< className <<">" <<endl;
			retval = false;
		}

		if (attributeName.length()>0) {
			try {
				attributeId = myRTIamb.getAttributeHandle(attributeName.c_str(),objectClassId);
				cout << "   " << attributeName << " ID = " << attributeId << endl;
			} catch (RTI::Exception& e){
				cerr << "   Unable to obtain attribute handle for object class attribute <"<< attributeName<<">" <<endl;
				retval = false;
			}
		}
		return retval;
	} /* end of getClassHandles */

	bool
	getInteractionHandles(std::string className, std::string parameterName) {
		bool retval = false;

		if (verbose) {
			cout << "Getting interaction handles..." <<endl;
		}
		try {
			interactionClassId = myRTIamb.getInteractionClassHandle(className.c_str());
			cout << "   " << className << " ID = " << interactionClassId << endl;
		} catch (RTI::Exception& e) {
			cerr << "   Unable to obtain class handle for interaction class <"<< className <<">" <<endl;
			retval = false;
		}

		if (parameterName.length()>0) {
			try {
				parameterId = myRTIamb.getParameterHandle(parameterName.c_str(),interactionClassId);
				cout << "   " << parameterName << " ID = " << parameterId << endl;
			} catch (RTI::Exception& e){
				cerr << "   Unable to obtain attribute handle for interaction class parameter <"<< parameterName<<">" <<endl;
				retval = false;
			}
		}
		return retval;
	} /* end of getInteractionHandles */

	bool
	publish() {
		bool retval = false;
		return retval;
	} /* end of publish */

	bool
	subscribe() {
		bool retval = false;
		// Add attribute handle to the attribute set
		// Before, we create the Set with one attribute
		auto_ptr<RTI::AttributeHandleSet> attributes(RTI::AttributeHandleSetFactory::create(1));
		attributes->add(attributeId);
		try {
			myRTIamb.subscribeObjectClassAttributes(objectClassId,*attributes);
			retval = true;
		} catch (RTI::Exception& e) {
			cerr << "Unable to subscribe" << endl;
		}
		return retval;
	} /* end of subscribe */

	bool
	unsubscribe(std::string className) {
		bool retval = true;
		RTI::ObjectClassHandle classId;

		try {
			classId = myRTIamb.getObjectClassHandle(className.c_str());
		} catch (RTI::Exception& e) {
			cerr << "Unable to obtain class handle for class <"<< className<<">" <<endl;
			retval = false;
		}
		try {
			myRTIamb.unsubscribeObjectClass(classId);
		} catch (RTI::Exception& e) {
			cerr << "Unable to unsubscribe" << endl;
			retval = false;
		}
		return retval;
	} /* end of unsubscribe */

	void
	tickRTI() {
		try {
			myRTIamb.tick();
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
			<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: unknown non-RTI exception." << endl;
		}
	} /* end of tickRTI() */

	void
	tickRTI(double min, double max) {
		try {
			myRTIamb.tick(min, max);
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
			<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: unknown non-RTI exception." << endl;
		}
	} /* end of tickRTI(double,double) */

protected:


	RTI::RTIambassador myRTIamb;
};

int
main(int argc, char **argv) {

	bool status = true;
	// Command line arguments handling (generated with gengetopt)
	gengetopt_args_info args ;
	if (cmdline_parser(argc, argv, &args))
		exit(EXIT_FAILURE);

	ParseFederate parseFederate(std::string(args.fedfile_arg),
			std::string(args.fedname_arg),args.verbose_flag);

	status &= parseFederate.create();

	if (args.joinname_given) {
		status &= parseFederate.join(std::string(args.joinname_arg));
	}

	if (args.joinname_given && args.objectclass_given) {
		if (args.attribute_given) {
			status &= parseFederate.getObjectHandles(std::string(args.objectclass_arg),
								std::string(args.attribute_arg));
		} else {
			status &= parseFederate.getObjectHandles(std::string(args.objectclass_arg),
								std::string(""));
		}
	}

	if (args.joinname_given && args.interaction_given) {
		if (args.parameter_given) {
			status &= parseFederate.getInteractionHandles(std::string(args.interaction_arg),
					std::string(args.parameter_arg));
		} else {
			status &= parseFederate.getInteractionHandles(std::string(args.interaction_arg),
								std::string(""));
		}
	}

	parseFederate.tickRTI(1,2);

	if (args.joinname_given) {
		status &= parseFederate.resign();
	}

	status &= parseFederate.destroy();
	if (status) {
		cout << "GLOBAL SUCCESS." <<endl;
	} else {
		cout << "GLOBAL ERROR." <<endl;
	}
	return status ? 0 : 1;
}
