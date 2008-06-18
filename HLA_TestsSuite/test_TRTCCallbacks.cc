#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <cstdlib>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::auto_ptr;

#define MAX_FEDERATES 10

class TRTCCallbacksFed : public NullFederateAmbassador {

private:
    string federationName;
    string fedFile;
    bool creator;
    bool TIMEREGENABLED, TIMECONENABLED;

public:
    TRTCCallbacksFed(void) {
        federationName = "TestTRTCCallbacks";
        fedFile = "test_TRTCCallbacks.fed";
        creator = false;
        TIMEREGENABLED = false;
        TIMECONENABLED = false;
    };

    virtual ~TRTCCallbacksFed() throw (RTI::FederateInternalError) {
    };

    int
    main(void) {
        this->create_and_join();
        if (creator) {
            cout << "Press ENTER to start execution ..." << endl;
            getchar();

            this->enableTimeReg();
            while (!TIMEREGENABLED) this->tickRTI();
	    TIMEREGENABLED=false;
            this->enableTimeCon();
            while (!TIMECONENABLED) this->tickRTI();
	    TIMECONENABLED=false;
        } else {
            this->enableTimeReg();
            while (!TIMEREGENABLED) this->tickRTI();
	    TIMEREGENABLED=false;
            this->enableTimeCon();
            while (!TIMECONENABLED) this->tickRTI();
	    TIMECONENABLED=false;
        }
        this->resign_and_destroy();
        return 0;
    };

private:

    void
    create_and_join(void) {
        try {
            rtiamb.createFederationExecution(federationName.c_str(),
                                             fedFile.c_str());
            creator = true;
        } catch ( RTI::FederationExecutionAlreadyExists& e) {
            cout << "--> Federation already created by another federate."
            << endl;
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: Unknown non-RTI exception." << endl;
        }

        string federateName = "TRTCCallbacks0";
        for (int i=1;i<=MAX_FEDERATES;i++) {
            cout << "Try to join joining federation: " << i << endl;
            try {
                cout << "Try to join federation as " << federateName << "." <<
                endl;
                rtiamb.joinFederationExecution(federateName.c_str(),
                                               federationName.c_str(),
                                               this);
                cout << "Joined federation as " << federateName << "." <<
                endl;
                break;
            } catch (RTI::FederateAlreadyExecutionMember& e) {
                cout << "Federate already execution member" << endl;
                federateName[13] = i+'0';
                if (i==MAX_FEDERATES) {
                    cout << "More than " << MAX_FEDERATES << " out there." <<
                    endl;
                    exit(-1);
                }
                continue;
            } catch ( RTI::Exception &e ) {
                cerr << "RTI exception: " << e._name << " ["
                << (e._reason ? e._reason : "undefined") << "]." << endl;
            } catch ( ... ) {
                cerr << "Error: Unknown non-RTI exception." << endl;
            }
        }

    }

    void
    resign_and_destroy(void) {
        try {
            rtiamb.resignFederationExecution(
                RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: Unknown non-RTI exception." << endl;
        }

        try {
            rtiamb.destroyFederationExecution(federationName.c_str());
        } catch (RTI::FederatesCurrentlyJoined) {
            cout << "Federates currently joined." << endl;
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: Unknown non-RTI exception." << endl;
        }
    }

    void
    enableTimeReg(void) {
        try {
            rtiamb.enableTimeRegulation(RTIfedTime(10), RTIfedTime(5));
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    void
    enableTimeCon(void) {
        try {
            rtiamb.enableTimeConstrained();
            rtiamb.enableAsynchronousDelivery();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    void
    tickRTI(void) {
        try {
            rtiamb.tick();
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
            rtiamb.tick(min, max);
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    /* Federate Ambassador services */
    void
    timeRegulationEnabled(const RTI::FedTime& theTime) 
    throw (RTI::InvalidFederationTime, 
	   RTI::EnableTimeRegulationWasNotPending, 
           RTI::FederateInternalError) {
        cout << "Time Regulation Enabled, press ENTER to continue..." << endl;
	cout << static_cast<RTIfedTime>(theTime).getTime() << endl;
	TIMEREGENABLED = true;
	getchar();
    }

    void
    timeConstrainedEnabled(const RTI::FedTime& theTime) 
    throw (RTI::InvalidFederationTime, 
	   RTI::EnableTimeConstrainedWasNotPending, 
           RTI::FederateInternalError) {
        cout << "Time Constrained Enabled, press ENTER to continue..." << endl;
	cout << static_cast<RTIfedTime>(theTime).getTime() << endl;
	TIMECONENABLED = true;
	getchar();
    }

protected:
    RTI::RTIambassador rtiamb;
};

int main(int argc, char **argv) {
    TRTCCallbacksFed testFed;
    return testFed.main();
}
