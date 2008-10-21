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

	void
	create() {
		try {
			myRTIamb.createFederationExecution(federationName.c_str(),
					federationFile.c_str());
			if (verbose) {
				cout << "createFederationExecution SUCCESS." << endl;
			}
		} catch ( RTI::FederationExecutionAlreadyExists& e) {
			cout << "--> Federation already created by another federate." << endl;
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
			<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: Unknown non-RTI exception." << endl;
		}
	} /* end of create */

	void
	destroy() {
		try {
			myRTIamb.destroyFederationExecution(federationName.c_str());
		} catch (RTI::FederatesCurrentlyJoined) {
			cout << "Federates currently joined." << endl;
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
			<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: Unknown non-RTI exception." << endl;
		}
	} /* end of destroy */

	void join(std::string federateName) {
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
		} catch (RTI::FederateAlreadyExecutionMember& e) {
			cerr << "Federate already execution member" << endl;
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
			<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: Unknown non-RTI exception." << endl;
		}
	} /* end of join */

	void resign() {

		try {
			if (verbose) {
				cout << "Try to resign from federation...";
			}
			myRTIamb.resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
			if (verbose) {
				cout << "SUCCESS." << endl;
			}
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
			<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: Unknown non-RTI exception." << endl;
		}
	} /* end of resign */

	void publish() {

	} /* end of publish */

	void subscribe(std::string className, std::string attributeName) {
		RTI::ObjectClassHandle classId;
		RTI::AttributeHandle   attId;
		if (verbose) {
			cout << "Getting handles..." <<endl;
		}
		try {
			classId = myRTIamb.getObjectClassHandle(className.c_str());
			cout << "   " << className << " ID = " << classId << endl;
		} catch (RTI::Exception& e) {
			cerr << "   Unable to obtain class handle for class <"<< className<<">" <<endl;
		}
		try {
			attId = myRTIamb.getAttributeHandle(attributeName.c_str(),classId);
			cout << "   " << attributeName << " ID = " << attId << endl;
		} catch (RTI::Exception& e){
			cerr << "   Unable to obtain attribute handle for attribute <"<< attributeName<<">" <<endl;
		}

		// Add attribute handle to the attribute set
		// Before, we create the Set with one attribute
		auto_ptr<RTI::AttributeHandleSet> attributes(RTI::AttributeHandleSetFactory::create(1));
		attributes->add(attId);
		try {
			myRTIamb.subscribeObjectClassAttributes(classId,*attributes);
		} catch (RTI::Exception& e) {
			cerr << "Unable to subscribe" << endl;
		}
	} /* end of subscribe */

	void unsubscribe(std::string className) {
		RTI::ObjectClassHandle classId;

		try {
			classId = myRTIamb.getObjectClassHandle(className.c_str());
		} catch (RTI::Exception& e) {
			cerr << "Unable to obtain class handle for class <"<< className<<">" <<endl;
		}
		try {
			myRTIamb.unsubscribeObjectClass(classId);
		} catch (RTI::Exception& e) {
			cerr << "Unable to unsubscribe" << endl;
		}
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

int main(int argc, char **argv) {

	// Command line arguments handling (generated with gengetopt)
	gengetopt_args_info args ;
	if (cmdline_parser(argc, argv, &args))
		exit(EXIT_FAILURE);

	ParseFederate parseFederate(std::string(args.fedfile_arg),
			std::string(args.fedname_arg),args.verbose_flag);

	parseFederate.create();
	if (args.joinname_given) {
		parseFederate.join(std::string(args.joinname_arg));
	}

	if (args.classname_given && args.attname_given) {
		parseFederate.subscribe(std::string(args.classname_arg),
				std::string(args.attname_arg));
	}

	parseFederate.tickRTI(1,2);

	if (args.classname_given && args.attname_given) {
		parseFederate.unsubscribe(std::string(args.classname_arg));
	}

	if (args.joinname_given) {
		parseFederate.resign();
	}

	parseFederate.destroy();

	return 0;
}
