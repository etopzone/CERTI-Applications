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
    bool SYNCPOINTREGSUCC, SYNCPOINTREGFAIL, ANNOUNCESYNCPOINT, FEDSYNC,
         DISCOVER, STARTREG;
    string SYNCPOINTLABEL, SYNCPOINTTAG;
    string syncpoint;
    struct hdls {
        struct objclass {
            RTI::ObjectClassHandle och;
            RTI::ObjectHandle oh;
            RTI::AttributeHandle attrA;
        } TestAttribute;
    } myhdls;

public:
    NotificationSwitchesFed(void) {
        federationName = "TestNotificationSwitches";
        fedFile = "test_NotificationSwitches.fed";
        creator = false;
        SYNCPOINTREGSUCC = false;
        SYNCPOINTREGFAIL = false;
        ANNOUNCESYNCPOINT = false;
        FEDSYNC = false;
	DISCOVER = false;
        STARTREG = false;
    };

    virtual ~NotificationSwitchesFed() throw (RTI::FederateInternalError) {
    };

    int
    main(void) {
        this->create_and_join();
	this->getHandles();
        if (creator) {
            cout << "Press ENTER to start execution ..." << endl;
            getchar();


	    // test switches	
            this->enableClassRelevanceAdvisorySwitch();
            this->disableClassRelevanceAdvisorySwitch();
            this->enableInteractionRelevanceAdvisorySwitch();
            this->disableInteractionRelevanceAdvisorySwitch();
            this->enableAttributeRelevanceAdvisorySwitch();
            this->disableAttributeRelevanceAdvisorySwitch();
            this->enableAttributeScopeAdvisorySwitch();
            this->disableAttributeScopeAdvisorySwitch();

            // publish phase            
            this->enableClassRelevanceAdvisorySwitch();
	    this->synchronizeFederation("sync", "test1");
		
            cout << "Publish and register objects" << endl;
            this->publishAttributes();
            this->registerObjectInst();

	    this->synchronizeFederation("sync", "test2");

	    this->synchronizeFederation("sync", "test3");
	    this->synchronizeFederation("sync", "test4");
	    this->synchronizeFederation("sync", "test5");


        } else {
            // test switches
            this->enableClassRelevanceAdvisorySwitch();
            this->disableClassRelevanceAdvisorySwitch();
            this->enableInteractionRelevanceAdvisorySwitch();
            this->disableInteractionRelevanceAdvisorySwitch();
            this->enableAttributeRelevanceAdvisorySwitch();
            this->disableAttributeRelevanceAdvisorySwitch();
            this->enableAttributeScopeAdvisorySwitch();
            this->disableAttributeScopeAdvisorySwitch();

            
            this->waitForSyncPoint(); // test1

	    // creator publish and register objects

            this->waitForSyncPoint();  // test2

            this->subscribeAttributes();

            this->waitForDiscover(); 
            this->waitForSyncPoint();  // test3

	    rtiamb.unsubscribeObjectClass(myhdls.TestAttribute.och);

            this->waitForSyncPoint();  // test4

	    /* Resubscription causes error
             * RTIA.cc:170: void certi::rtia::RTIA::execute(): Assertion `false' failed.
             *
             * ???
             */
 	
            //this->subscribeAttributes();
	    //this->waitForDiscover();	 
            this->waitForSyncPoint(); // test5

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
                federateName[20] = i+'0';
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
        while (!ANNOUNCESYNCPOINT) this->tickRTI(0,1);
        ANNOUNCESYNCPOINT = false;
        cout << "Synchronization Point " << SYNCPOINTLABEL <<
        " reached, press ENTER ..." << endl;
        getchar();
        this->indicateSynchronization(SYNCPOINTLABEL);
    }

    void
    waitForDiscover(void) {
        while (!DISCOVER) this->tickRTI(0,1);
        DISCOVER = false;
        cout << "Discovering object" << endl;
    }

    void
    waitStartReg(void) {
        while (!STARTREG) this->tickRTI(0,1);
        STARTREG = false;
        cout << "StartRegForObjClass" << endl;
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
    getHandles(void) {

        // get object class handle
        try {
            myhdls.TestAttribute.och =
                rtiamb.getObjectClassHandle("TestAttribute");
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: Unknown non-RTI exception." << endl;
        }

        // get attribute handles
        try {
            myhdls.TestAttribute.attrA =
                rtiamb.getAttributeHandle("attrA",
                                          myhdls.TestAttribute.och);
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: Unknown non-RTI exception." << endl;
        }

    }

    void
    publishAttributes(void) {

        auto_ptr<RTI::AttributeHandleSet>
        attrs(RTI::AttributeHandleSetFactory::create(4));

        attrs->add(myhdls.TestAttribute.attrA);

        try {
            rtiamb.publishObjectClass(myhdls.TestAttribute.och, *attrs);
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }

        attrs->empty();
    }

    void
    subscribeAttributes(void) {

        auto_ptr<RTI::AttributeHandleSet>
        attrs(RTI::AttributeHandleSetFactory::create(4));

        attrs->add(myhdls.TestAttribute.attrA);

        try {
            rtiamb.subscribeObjectClassAttributes(myhdls.TestAttribute.och,
                                                  *attrs);
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }

        attrs->empty();
    }

    void
    registerObjectInst(void) {

        try {
            myhdls.TestAttribute.oh = rtiamb.registerObjectInstance
                                      (myhdls.TestAttribute.och);
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
    throw ( RTI::FederateInternalError ) {
        cout << "fedSync: " << label << endl;
        FEDSYNC = true;
    }

    void
    startRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
    throw ( RTI::FederateInternalError,
	    RTI::ObjectClassNotPublished ) {
	cout << "startRegForObjClass: " << theClass << " och: "  
             << myhdls.TestAttribute.och  << endl;
	STARTREG=true;
    }

    void 
    discoverObjectInstance(RTI::ObjectHandle oh, RTI::ObjectClassHandle och,  
                           const char * tag) 
    throw ( RTI::CouldNotDiscover, 
            RTI::ObjectClassNotKnown, 
            RTI::FederateInternalError) { 
	cout << "discoverObjIns: " << oh << " och: " << och << " tag: " 
             << tag << endl;
        DISCOVER=true;
    }

protected:
    RTI::RTIambassador rtiamb;
};

int main(int argc, char **argv) {
    NotificationSwitchesFed testFed;
    return testFed.main();
}
