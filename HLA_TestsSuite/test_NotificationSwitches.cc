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

class NotificationSwitchesFed : public NullFederateAmbassador {

private:
    string federationName;
    string fedFile;
    bool creator;

public:
    NotificationSwitchesFed(void) {
        federationName = "TestNotificationSwitches";
        fedFile = "test_NotificationSwitches.fed";
        creator = false;
    };

    virtual ~NotificationSwitchesFed() throw (RTI::FederateInternalError) {
    };

    int
    main(void) {
        this->create_and_join();
        if (creator) {
            cout << "Press ENTER to start execution ..." << endl;
            getchar();

            this->enableClassRelevanceAdvisorySwitch();
            this->disableClassRelevanceAdvisorySwitch();
            this->enableInteractionRelevanceAdvisorySwitch();
            this->disableInteractionRelevanceAdvisorySwitch();
            this->enableAttributeRelevanceAdvisorySwitch();
            this->disableAttributeRelevanceAdvisorySwitch();
            this->enableAttributeScopeAdvisorySwitch();
            this->disableAttributeScopeAdvisorySwitch();

        } else {
            this->enableClassRelevanceAdvisorySwitch();
            this->disableClassRelevanceAdvisorySwitch();
            this->enableInteractionRelevanceAdvisorySwitch();
            this->disableInteractionRelevanceAdvisorySwitch();
            this->enableAttributeRelevanceAdvisorySwitch();
            this->disableAttributeRelevanceAdvisorySwitch();
            this->enableAttributeScopeAdvisorySwitch();
            this->disableAttributeScopeAdvisorySwitch();
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

        string federateName = "NotificationSwitches0";
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
                federateName[10] = i+'0';
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
    enableClassRelevanceAdvisorySwitch(void) {
       try {
           rtiamb.enableClassRelevanceAdvisorySwitch();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    void
    disableClassRelevanceAdvisorySwitch(void) {
       try {
           rtiamb.disableClassRelevanceAdvisorySwitch();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    void
    enableInteractionRelevanceAdvisorySwitch(void) {
       try {
           rtiamb.enableInteractionRelevanceAdvisorySwitch();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    void
    disableInteractionRelevanceAdvisorySwitch(void) {
       try {
           rtiamb.disableInteractionRelevanceAdvisorySwitch();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    void
    enableAttributeRelevanceAdvisorySwitch(void) {
       try {
           rtiamb.enableAttributeRelevanceAdvisorySwitch();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    void
    disableAttributeRelevanceAdvisorySwitch(void) {
       try {
           rtiamb.disableAttributeRelevanceAdvisorySwitch();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    void
    enableAttributeScopeAdvisorySwitch(void) {
       try {
           rtiamb.enableAttributeScopeAdvisorySwitch();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    void
    disableAttributeScopeAdvisorySwitch(void) {
       try {
           rtiamb.disableAttributeScopeAdvisorySwitch();
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

protected:
    RTI::RTIambassador rtiamb;
};

int main(int argc, char **argv) {
    NotificationSwitchesFed testFed;
    return testFed.main();
}
