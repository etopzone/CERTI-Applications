#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

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
#else
    #include <unistd.h>
#endif

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::auto_ptr;

#define MAX_FEDERATES 10
#define LH 5

class HugeAVPVFed : public NullFederateAmbassador {

private:
    string federationName;
    string fedFile;
    unsigned int payloadSize;
    unsigned int rav_ri_counter;
    bool creator;
    struct hdls {
        struct objclass {
            RTI::ObjectClassHandle och;
            RTI::ObjectHandle oh;
            RTI::AttributeHandle a0;
            RTI::AttributeHandle a1;
            RTI::AttributeHandle a2;
            RTI::AttributeHandle a3;
        } TestAttribute;
        struct intClass {
            RTI::InteractionClassHandle ich;
            RTI::ParameterHandle p;
        } TestParameter0, TestParameter1, TestParameter2, TestParameter3;
    } myhdls;
    bool SYNCPOINTREGSUCC, SYNCPOINTREGFAIL, ANNOUNCESYNCPOINT, FEDSYNC,
	 TIMEADVANCEGRANT;
    string SYNCPOINTLABEL, SYNCPOINTTAG;

public:
    HugeAVPVFed(unsigned int size) {
        federationName = "TestHugeAVPV";
        fedFile = "test_HugeAVPV.fed";
        creator = false;
        SYNCPOINTREGSUCC = false;
        SYNCPOINTREGFAIL = false;
        ANNOUNCESYNCPOINT = false;
        FEDSYNC = false;
        TIMEADVANCEGRANT = false;
        payloadSize = size;
        rav_ri_counter = 0;
    };

    virtual ~HugeAVPVFed() throw (RTI::FederateInternalError) {
    };

    int
    main(void) {
        this->create_and_join();
        this->getHandles();
        if (creator) {
            cout << "--------------------------------------------------" << endl;
            cout << "Press ENTER to start execution ..." << endl;
            getchar();
  
            cout << "1) BEGIN Initialization" << endl;
            this->synchronizeFederation("sync1", "test");
	    cout << "  Sync (1) done" << endl;

            this->publishAttributesAndParameters();
            this->registerObjectInst();

            cout << "1) END Initialization" << endl;
            cout << "--------------------------------------------------" << endl;

            // not time regulating
	    // all receivers have to get messages wo timestamps
            cout << "2) BEGIN Updates, Federate State: Nor TR/TC" << endl;

            this->synchronizeFederation("sync2", "test");
	    cout << "Sync (2) done" << endl;

	    cout << "(1) Send Attributes and Parameters" << endl;
            this->updateAttributeVals(false);
            this->sendInteractions(false);
            this->updateAttributeVals(true);
            this->sendInteractions(true);

	    cout << "(1) Done" << endl;
            this->synchronizeFederation("sync3", "test");
	    cout << "Sync (3) done" << endl;

            cout << "2) END Updates, Federate State: Nor TR/TC" << endl;
            cout << "--------------------------------------------------" << endl;
	   
            // time regulating, receivers are not time constrained
	    // all receivers have to get messages wo timestamps
            cout << "3) BEGIN Updates, Federate State: TR" << endl;
	    cout << "Enabling Time Regulation. Press ENTER ..." << endl;
            getchar();
            this->enableTimeReg();
	    cout << "Time Regulation Enabled. Press ENTER ..." << endl;
            getchar();
            this->synchronizeFederation("sync4", "test");
	    cout << "Sync (4) done" << endl;

	    cout << "(2) Send Attributes and Parameters" << endl;
            this->updateAttributeVals(false);
            this->sendInteractions(false);
            this->updateAttributeVals(true);
            this->sendInteractions(true);
	    cout << "(2) Done." << endl;
            cout << "3) END Updates, Federate State: TR" << endl;
            cout << "--------------------------------------------------" << endl;

            // time regulating, receivers are time constrained
	    // all receivers have to get messages w timestamps !
            cout << "4) BEGIN Updates, Federate State: TR, Receivers TC" << endl;
            this->synchronizeFederation("sync5", "test");
	    cout << "Sync (5) done" << endl;

	    cout << "(3) Send Attributes and Parameters" << endl;
            this->updateAttributeVals(false);
            this->sendInteractions(false);
            this->updateAttributeVals(true);
            this->sendInteractions(true);
	    cout << "(3) Done." << endl;

	    cout << "All ts updates done. Press ENTER ..." << endl;
	    getchar();

            this->synchronizeFederation("sync6", "test");
	    cout << "Sync (6) done" << endl;

	    cout << "Advancing time. Press ENTER ..." << endl;
	    getchar();
	    this->advanceTime(10.0);
	    cout << "Time advanced. Press ENTER ..." << endl;
	    getchar();

            cout << "4) END Updates, Federate State: TR, Receivers TC" << endl;
            cout << "--------------------------------------------------" << endl;
            
            // stay active until all federates reach end of execution
            this->synchronizeFederation("sync7", "test");
	    cout << "Sync (7) done" << endl;
        } else {
            this->subscribeAttributesAndParameters();
            // initial sync point
            this->waitForSyncPoint();
	    cout << "First sync point reached." << endl;

            // second sync point -> afterwards have to get rav and ri wo ts
            this->waitForSyncPoint();
	    cout << "Second sync point reached." << endl;

            while (rav_ri_counter!=6) this->tickRTI();
            rav_ri_counter = 0;

	    // after next sync point sender is time regulating, receivers
	    // are still timeconstrained nor time regulating
	    // => all received messages have no timestamps
            this->waitForSyncPoint();
            cout << "Third sync point reached." << endl;

            // third sync point -> afterwards have to get rav and ri wo ts
            this->waitForSyncPoint();
            cout << "Fourth sync point reached." << endl;

            while (rav_ri_counter!=6) this->tickRTI();
            rav_ri_counter = 0;

            // fourth sync point -> afterwards have to get rav and ri wo/w ts
	    cout << "Enabling Time Constrained. Press ENTER ..." << endl;
            getchar();
            this->enableTimeCon();
	    cout << "Time Constrained Enabled. Press ENTER ..." << endl;
            getchar();

            this->waitForSyncPoint();
            cout << "Fifth sync point reached." << endl;

            this->waitForSyncPoint();
            cout << "Sixth sync point reached." << endl;

	    cout << "Advancing time. Press ENTER ..." << endl;
	    getchar();
	    this->advanceTime(10.0);
	    cout << "Time advanced. Press ENTER ..." << endl;
	    getchar();

            while (rav_ri_counter!=6) this->tickRTI();
            rav_ri_counter = 0;

            // final sync point
            this->waitForSyncPoint();
            cout << "Final sync point reached." << endl;
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

        string federateName = "hugeAVPVFed0";
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
                federateName[11] = i+'0';
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
            myhdls.TestAttribute.a0 =
                rtiamb.getAttributeHandle("Payload_A_0",
                                          myhdls.TestAttribute.och);
            myhdls.TestAttribute.a1 =
                rtiamb.getAttributeHandle("Payload_A_1",
                                          myhdls.TestAttribute.och);
            myhdls.TestAttribute.a2 =
                rtiamb.getAttributeHandle("Payload_A_2",
                                          myhdls.TestAttribute.och);
            myhdls.TestAttribute.a3 =
                rtiamb.getAttributeHandle("Payload_A_3",
                                          myhdls.TestAttribute.och);
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: Unknown non-RTI exception." << endl;
        }

        // get interaction class handle
        try {
            myhdls.TestParameter0.ich =
                rtiamb.getInteractionClassHandle("TestParameter_P_0");
            myhdls.TestParameter1.ich =
                rtiamb.getInteractionClassHandle("TestParameter_P_1");
            myhdls.TestParameter2.ich =
                rtiamb.getInteractionClassHandle("TestParameter_P_2");
            myhdls.TestParameter3.ich =
                rtiamb.getInteractionClassHandle("TestParameter_P_3");
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: Unknown non-RTI exception." << endl;
        }

        // get parameter handles
        try {
            myhdls.TestParameter0.p =
                rtiamb.getParameterHandle("Payload", myhdls.TestParameter0.ich);
            myhdls.TestParameter1.p =
                rtiamb.getParameterHandle("Payload", myhdls.TestParameter1.ich);
            myhdls.TestParameter2.p =
                rtiamb.getParameterHandle("Payload", myhdls.TestParameter2.ich);
            myhdls.TestParameter3.p =
                rtiamb.getParameterHandle("Payload", myhdls.TestParameter3.ich);
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: Unknown non-RTI exception." << endl;
        }

    }

    void
    publishAttributesAndParameters(void) {

        auto_ptr<RTI::AttributeHandleSet>
        attrs(RTI::AttributeHandleSetFactory::create(4));

        attrs->add(myhdls.TestAttribute.a0);
        attrs->add(myhdls.TestAttribute.a1);
        attrs->add(myhdls.TestAttribute.a2);
        attrs->add(myhdls.TestAttribute.a3);

        try {
            rtiamb.publishObjectClass(myhdls.TestAttribute.och, *attrs);
            rtiamb.publishInteractionClass(myhdls.TestParameter0.ich);
            rtiamb.publishInteractionClass(myhdls.TestParameter1.ich);
            rtiamb.publishInteractionClass(myhdls.TestParameter2.ich);
            rtiamb.publishInteractionClass(myhdls.TestParameter3.ich);
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }

        attrs->empty();
    }

    void
    subscribeAttributesAndParameters(void) {

        auto_ptr<RTI::AttributeHandleSet>
        attrs(RTI::AttributeHandleSetFactory::create(4));

        attrs->add(myhdls.TestAttribute.a0);
        attrs->add(myhdls.TestAttribute.a1);
        attrs->add(myhdls.TestAttribute.a2);
        attrs->add(myhdls.TestAttribute.a3);

        try {
            rtiamb.subscribeObjectClassAttributes(myhdls.TestAttribute.och,
                                                  *attrs);
            rtiamb.subscribeInteractionClass(myhdls.TestParameter0.ich);
            rtiamb.subscribeInteractionClass(myhdls.TestParameter1.ich);
            rtiamb.subscribeInteractionClass(myhdls.TestParameter2.ich);
            rtiamb.subscribeInteractionClass(myhdls.TestParameter3.ich);
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
    advanceTime(double time) {
	try {
	    rtiamb.timeAdvanceRequest(RTIfedTime(time));
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }
	while (!TIMEADVANCEGRANT) this->tickRTI();
	TIMEADVANCEGRANT = false;
    }

    void
    updateAttributeVals(bool ts) {

        auto_ptr<RTI::AttributeHandleValuePairSet>
        ahvps(RTI::AttributeSetFactory::create(4));

        char *payload = new char[payloadSize];

        memset(payload, 0, payloadSize);

        ahvps->add(myhdls.TestAttribute.a0, payload, payloadSize);
        ahvps->add(myhdls.TestAttribute.a1, payload, payloadSize);
        ahvps->add(myhdls.TestAttribute.a2, payload, payloadSize);
        ahvps->add(myhdls.TestAttribute.a3, payload, payloadSize);

        try {
            if (ts) {
                rtiamb.updateAttributeValues(myhdls.TestAttribute.oh, *ahvps,
                                             RTIfedTime(LH), "a");
            }
            else {
                rtiamb.updateAttributeValues(myhdls.TestAttribute.oh,
                                             *ahvps, "a");
            }
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }

        ahvps->empty();
        delete [] payload;

    }

    void
    sendInteractions(bool ts) {

        auto_ptr<RTI::ParameterHandleValuePairSet>
        phvps0(RTI::ParameterSetFactory::create(1));
        auto_ptr<RTI::ParameterHandleValuePairSet>
        phvps1(RTI::ParameterSetFactory::create(1));
        auto_ptr<RTI::ParameterHandleValuePairSet>
        phvps2(RTI::ParameterSetFactory::create(1));
        auto_ptr<RTI::ParameterHandleValuePairSet>
        phvps3(RTI::ParameterSetFactory::create(1));

        char *payload = new char[payloadSize];

        memset(payload, 0, payloadSize);

        phvps0->add(myhdls.TestParameter0.p, payload, payloadSize);
        phvps1->add(myhdls.TestParameter1.p, payload, payloadSize);
        phvps2->add(myhdls.TestParameter2.p, payload, payloadSize);
        phvps3->add(myhdls.TestParameter3.p, payload, payloadSize);

        try {
            if (ts) {
                rtiamb.sendInteraction(myhdls.TestParameter0.ich, *phvps0,
                                       RTIfedTime(LH), "p0");
                rtiamb.sendInteraction(myhdls.TestParameter1.ich, *phvps1,
                                       RTIfedTime(LH), "p1");
                rtiamb.sendInteraction(myhdls.TestParameter2.ich, *phvps2,
                                       RTIfedTime(LH), "p2");
                rtiamb.sendInteraction(myhdls.TestParameter3.ich, *phvps3,
                                       RTIfedTime(LH), "p3");
            } else {
                rtiamb.sendInteraction(myhdls.TestParameter0.ich, *phvps0, "p");
                rtiamb.sendInteraction(myhdls.TestParameter1.ich, *phvps1, "p");
                rtiamb.sendInteraction(myhdls.TestParameter2.ich, *phvps2, "p");
                rtiamb.sendInteraction(myhdls.TestParameter3.ich, *phvps3, "p");
            }
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }

        phvps0->empty();
        phvps1->empty();
        phvps2->empty();
        phvps3->empty();
        delete [] payload;

    }

    void
    enableTimeReg(void) {
        try {
            rtiamb.enableTimeRegulation(RTIfedTime(0), RTIfedTime(LH));
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

    void
    discoverObjectInstance (RTI::ObjectHandle theObject,
                            RTI::ObjectClassHandle theObjectClass,
                            const char *theObjectName)
    throw ( RTI::CouldNotDiscover,
            RTI::ObjectClassNotKnown,
            RTI::FederateInternalError) {
        cout << "discoverObjInst " << theObjectName << endl;
    }

    void
    reflectAttributeValues (RTI::ObjectHandle theObject,
                            const RTI::AttributeHandleValuePairSet& 
				  theAttributes,
                            const char *theTag)
    throw ( RTI::ObjectNotKnown,
            RTI::AttributeNotKnown,
            RTI::FederateOwnsAttributes,
            RTI::FederateInternalError) {
        rav_ri_counter += 1;
        cout << "rav wo ts" << endl;
    }

    void
    reflectAttributeValues (RTI::ObjectHandle theObject,
                            const RTI::AttributeHandleValuePairSet& 
				  theAttributes,
                            const RTI::FedTime& theTime,
                            const char *theTag,
                            RTI::EventRetractionHandle erhdl)
    throw ( RTI::ObjectNotKnown,
            RTI::AttributeNotKnown,
            RTI::FederateOwnsAttributes,
            RTI::InvalidFederationTime,
            RTI::FederateInternalError) {
        rav_ri_counter += 1;
        cout << "rav w ts " << (static_cast<RTIfedTime>(theTime)).getTime() << endl;
    }

    void
    receiveInteraction (RTI::InteractionClassHandle theHandle,
                        const RTI::ParameterHandleValuePairSet
                        &theParameters,
                        const char *theTag)
    throw ( RTI::InteractionClassNotKnown,
            RTI::InteractionParameterNotKnown,
            RTI::FederateInternalError ) {
        rav_ri_counter += 1;
        cout << "ri wo ts" << endl;
    }

    void
    receiveInteraction (RTI::InteractionClassHandle theHandle,
                        const RTI::ParameterHandleValuePairSet
                        &theParameters,
                        const RTI::FedTime& theTime,
                        const char *theTag,
                        RTI::EventRetractionHandle erhdl)
    throw ( RTI::InteractionClassNotKnown,
            RTI::InteractionParameterNotKnown,
            RTI::InvalidFederationTime,
            RTI::FederateInternalError ) {
        rav_ri_counter += 1;
        cout << "ri w ts " << (static_cast<RTIfedTime>(theTime)).getTime() << endl;
    }

    void
    timeRegulationEnabled(const RTI::FedTime& theTime) 
    throw (RTI::InvalidFederationTime, 
	   RTI::EnableTimeRegulationWasNotPending, 
           RTI::FederateInternalError) {
	cout << "Time Regulation enabled at " << (static_cast<RTIfedTime>(theTime)).getTime() << endl;
    }

    void
    timeConstrainedEnabled(const RTI::FedTime& theTime) 
    throw (RTI::InvalidFederationTime, 
	   RTI::EnableTimeConstrainedWasNotPending, 
           RTI::FederateInternalError) {
	cout << "Time Constrained enabled at " << (static_cast<RTIfedTime>(theTime)).getTime() << endl;
    }

    void 
    timeAdvanceGrant(const RTI::FedTime &)
    throw (RTI::InvalidFederationTime, 
           RTI::TimeAdvanceWasNotInProgress, 
           RTI::FederateInternalError) { 
	TIMEADVANCEGRANT = true;
	cout << "Time Advance Grant, press ENTER to continue..." << endl;
	getchar();
    }
	

protected:
    RTI::RTIambassador rtiamb;
};

int main(int argc, char **argv) {

    if (argc<2) {
        cout << "Usage: " << argv[0] << " payload size [bytes]" << endl;
        return -1;
    }

    HugeAVPVFed testFed(atoi(argv[1]));
    return testFed.main();
}
