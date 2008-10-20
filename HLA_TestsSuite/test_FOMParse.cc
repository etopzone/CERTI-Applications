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

    int
    main(void) {
        this->create();
        this->destroy();
        return 0;
    };

private:

    void
    create() {
        try {
            myRTIamb.createFederationExecution(federationName.c_str(),
                                               federationFile.c_str());
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
    destroy(void) {
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

    void
    tickRTI(void) {
        try {
            myRTIamb.tick();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

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
    }

protected:
    RTI::RTIambassador myRTIamb;
};

int main(int argc, char **argv) {

	// Command line
	gengetopt_args_info args ;
	if (cmdline_parser(argc, argv, &args))
		    exit(EXIT_FAILURE);

    ParseFederate parseFederate(std::string(args.fedfile_arg),
								std::string(args.fedname_arg),args.verbose_arg);
    return parseFederate.main();
}
