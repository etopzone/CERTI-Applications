#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#include <iostream>
#include <sstream>
#include <memory>
#include <string>

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::auto_ptr;

#define MAX_FEDERATES 10
#define SYNCITERATIONS 5 

class SyncFed : public NullFederateAmbassador {

private:
    string federationName;
    string fedFile;
    unsigned int numOfSyncs;
    bool creator;
    bool SYNCPOINTREGSUCC, SYNCPOINTREGFAIL, ANNOUNCESYNCPOINT, FEDSYNC;
    string SYNCPOINTLABEL, SYNCPOINTTAG;

public:
    SyncFed(unsigned int num) {
        federationName = "TestSync";
        fedFile = "test_Sync.fed";
        creator = false;
        SYNCPOINTREGSUCC = false;
        SYNCPOINTREGFAIL = false;
        ANNOUNCESYNCPOINT = false;
        FEDSYNC = false;
        numOfSyncs = num;
    };

    virtual ~SyncFed() throw (RTI::FederateInternalError) {
    };

    int
    main(void) {
        this->create_and_join();
        if (creator) {
	    std::ostringstream osstr;
	    string syncpoint;

            cout << "Press ENTER to start execution ..." << endl;
            getchar();

	    for (unsigned int i=0; i<numOfSyncs; i++) {
	      syncpoint="sync";
	      osstr  << i;
	      syncpoint.append(osstr.str());
	      osstr.str("");
	      
              // same sync point for SYNCITERATIONS times
	      for (unsigned j=0; j<SYNCITERATIONS; j++)
	      	this->synchronizeFederation(syncpoint, "test");

	      cout << "Sync " << syncpoint << "  done." << endl;
	    }

        } else {

	    for (unsigned int i=0; i<numOfSyncs*SYNCITERATIONS; i++) {
              this->waitForSyncPoint();
	      cout << "Reached Sync Point " << SYNCPOINTLABEL << endl;
	    }

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

        string federateName = "syncFed0";
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
                federateName[7] = i+'0';
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
    synchronizeFederation(string label, string tag) {

        for (;;) {
            this->registerFedSyncPoint(label, tag);

            while (!(SYNCPOINTREGSUCC || SYNCPOINTREGFAIL)) this->tickRTI();

            if (SYNCPOINTREGFAIL) {
                SYNCPOINTREGFAIL = false;
		sleep(1);
                continue;
            } else {
                SYNCPOINTREGSUCC = false;
                break;
            }
        }

        while (!ANNOUNCESYNCPOINT) this->tickRTI();
        ANNOUNCESYNCPOINT = false;

        this->indicateSynchronization(label);
    }

    void
    indicateSynchronization(string label) {
        this->syncPointAchieved(label);
        while (!FEDSYNC) this->tickRTI(0,1);
        FEDSYNC=false;
    }

    void
    waitForSyncPoint(void) {
        while (!ANNOUNCESYNCPOINT) this->tickRTI();
        ANNOUNCESYNCPOINT = false;
        cout << "Synchronization Point " << SYNCPOINTLABEL <<
        " reached, press ENTER ..." << endl;
        getchar();
        this->indicateSynchronization(SYNCPOINTLABEL);
    }

    void
    registerFedSyncPoint(string label, string tag) {
        try {
            rtiamb.registerFederationSynchronizationPoint(label.c_str(),
                    tag.c_str());
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
    }

    void
    syncPointAchieved(string label) {
        try {
            rtiamb.synchronizationPointAchieved(label.c_str());
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
    synchronizationPointRegistrationSucceeded(const char* label)
    throw ( RTI::FederateInternalError) {
        cout << "syncPointRegSucc: " << label << endl;
        SYNCPOINTREGSUCC = true;
    }

    void
    synchronizationPointRegistrationFailed(const char* label)
    throw ( RTI::FederateInternalError) {
        cout << "syncPointRegSucc: " << label << endl;
        SYNCPOINTREGFAIL = true;
    }

    void
    announceSynchronizationPoint(const char* label, const char* tag)
    throw ( RTI::FederateInternalError) {
        cout << "announceSyncPoint: " << label << ", " << tag << endl;
        SYNCPOINTLABEL = label;
        SYNCPOINTTAG = tag;
        ANNOUNCESYNCPOINT = true;
    }

    void
    federationSynchronized(const char* label)
    throw (RTI::FederateInternalError) {
        cout << "fedSync: " << label << endl;
        FEDSYNC = true;
    }

protected:
    RTI::RTIambassador rtiamb;
};

int main(int argc, char **argv) {

    if (argc<2) {
        cout << "Usage: " << argv[0] << " number of sync point" << endl;
        return -1;
    }

    SyncFed testFed(atoi(argv[1]));
    return testFed.main();
}
