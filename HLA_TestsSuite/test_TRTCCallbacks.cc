#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <cstdlib>
#include <vector>
#include <HLAtypesIEEE1516.hh>

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
	string federateName;
	bool creator;
	bool TIME_REGULATION_ENABLED, TIME_CONSTRAINED_ENABLED;
	bool TIME_ADVANCE_GRANT;
	int nbTARstepsToDo;
	typedef struct UpdateMeH {
		RTI::ObjectClassHandle och_updateMe;
		RTI::AttributeHandle   ah_pingMe;
		RTI::AttributeHandle   ah_counter;
	} UpdateMeH_t;

	UpdateMeH_t umoh;
	typedef struct UpdateMeObject {
		std::string            name;
		RTI::ObjectHandle      oh_updateMe;
		libhla::HLAdata<libhla::HLAinteger64LE> pingMe;
		libhla::HLAdata<libhla::HLAinteger64LE> counter;
	} UpdateMeObject_t;

	UpdateMeObject_t myUmo;
	std::map<RTI::ObjectHandle,UpdateMeObject_t*> otherUmo;
	RTIfedTime mytime;
	RTIfedTime myLookahead;

public:
	TRTCCallbacksFed(void) {
		federationName = "TestTRTCCallbacks";
		fedFile = "test_TRTCCallbacks.fed";
		creator = false;
		TIME_REGULATION_ENABLED = false;
		TIME_CONSTRAINED_ENABLED = false;
		TIME_ADVANCE_GRANT = false;
		nbTARstepsToDo = 0;
		myUmo.name ="unknown";
		*(myUmo.pingMe) = 0;
		*(myUmo.counter) = 0;
		// initial simulation time is 0
		mytime.setZero();
		myLookahead.setZero();
		// initial [default lookahead is 1.0]
		myLookahead = myLookahead + RTIfedTime(1.0);
	};

	virtual ~TRTCCallbacksFed() throw (RTI::FederateInternalError) {
		for (std::map<RTI::ObjectHandle,UpdateMeObject_t*>::iterator it = otherUmo.begin(); it != otherUmo.end(); it++) {
			delete (it->second);
		}
		otherUmo.clear();
	};

	void setNbTARStepToDo(int nbsteps) {
		this->nbTARstepsToDo = nbsteps;
	}

	int
	main(void) {
		this->create_and_join();
		this->getHandles();
		if (creator) {
			cout << "Press ENTER to start execution ..." << endl;
			getchar();
		}
		this->publishAndRegister();

		this->enableTimeReg();
		while (!TIME_REGULATION_ENABLED) this->tickRTI();
		TIME_REGULATION_ENABLED=false;
		this->enableTimeCon();
		while (!TIME_CONSTRAINED_ENABLED) this->tickRTI();
		TIME_CONSTRAINED_ENABLED=false;

		// advance time loop
		while (nbTARstepsToDo>0) {
			this->advanceTime(mytime+myLookahead);
			nbTARstepsToDo--;
		}

		this->resign_and_destroy();
		return 0;
	};

	void
	discoverObjectInstance (RTI::ObjectHandle theObject,
			RTI::ObjectClassHandle theObjectClass,
			const char *theObjectName)
	throw ( RTI::CouldNotDiscover,
			RTI::ObjectClassNotKnown,
			RTI::FederateInternalError) {

		UpdateMeObject_t*  newObject = new UpdateMeObject_t();

		newObject->name = std::string(theObjectName);
		newObject->oh_updateMe = theObject;
		otherUmo[newObject->oh_updateMe] = newObject;
		cout << "Discovered " << newObject->name
				<< " [handle =" << newObject->oh_updateMe <<"]"<< endl;
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

		RTI::ULong rul;
		char* value;

		if (theAttributes.getHandle(0)==umoh.ah_counter) {
			value = theAttributes.getValuePointer(0,rul);
			//libhla::HLAdata<libhla::HLAinteger64LE> newval(value,rul);
			(*(otherUmo[theObject]->counter)).copy(value);
		}

		if (theAttributes.getHandle(0)==umoh.ah_pingMe) {
			value = theAttributes.getValuePointer(0,rul);
			//libhla::HLAdata<libhla::HLAinteger64LE> newval(value,rul);
			(*(otherUmo[theObject]->pingMe)).copy(value);
		}

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

		cout << "rav w ts " << (static_cast<RTIfedTime>(theTime)).getTime() << endl;
	}

	void
	timeAdvanceGrant(const RTI::FedTime &newTime)
	throw (RTI::InvalidFederationTime,
			RTI::TimeAdvanceWasNotInProgress,
			RTI::FederateInternalError) {
		TIME_ADVANCE_GRANT = true;
		mytime = newTime;
		cout << "TIME_ADVANCE_GRANT" << endl;
	}

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

		federateName = "TRTCCallbacks0";
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

	void getHandles() {
		// get object class handle
		try {
			umoh.och_updateMe =
					rtiamb.getObjectClassHandle("UpdateMe");
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
					<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: Unknown non-RTI exception." << endl;
		}

		// get attribute handles
		try {
			umoh.ah_pingMe =
					rtiamb.getAttributeHandle("PingMe",umoh.och_updateMe);
			umoh.ah_counter =
					rtiamb.getAttributeHandle("Counter",umoh.och_updateMe);
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
					<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: Unknown non-RTI exception." << endl;
		}
	}

	void publishAndRegister() {
		auto_ptr<RTI::AttributeHandleSet>
		attributes(RTI::AttributeHandleSetFactory::create(2));

		attributes->add(umoh.ah_pingMe);
		attributes->add(umoh.ah_counter);

		try {
			rtiamb.publishObjectClass(umoh.och_updateMe, *attributes);

		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
					<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: unknown non-RTI exception." << endl;
		}
		attributes->empty();

		// Register My Object instance
		try {
			myUmo.name = "UMO-"+federateName;
			myUmo.oh_updateMe = rtiamb.registerObjectInstance(umoh.och_updateMe);
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
					<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: unknown non-RTI exception." << endl;
		}
	} /* end of publishAndRegister */

	/**
	 * Send update attribute values.
	 * @param ts true if the UAV is timestamped false otherwise
	 */
	void UAV(bool ts) {
		auto_ptr<RTI::AttributeHandleValuePairSet>

		ahvps(RTI::AttributeSetFactory::create(2));
		ahvps->add(umoh.ah_counter, myUmo.counter.data(), myUmo.counter.size());
		ahvps->add(umoh.ah_pingMe, myUmo.pingMe.data(), myUmo.pingMe.size());

		try {
			if (ts) {
				rtiamb.updateAttributeValues(myUmo.oh_updateMe, *ahvps,mytime+myLookahead, "upd");
			}
			else {
				rtiamb.updateAttributeValues(myUmo.oh_updateMe, *ahvps,"upd");
			}
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
					<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: unknown non-RTI exception." << endl;
		}

		ahvps->empty();
	}  /* end of UAV */

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
	advanceTime(const RTIfedTime& time) {
		try {
			rtiamb.timeAdvanceRequest(time);
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
					<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: unknown non-RTI exception." << endl;
		}
		while (!TIME_ADVANCE_GRANT) this->tickRTI();
		TIME_ADVANCE_GRANT = false;
		getchar();
	}

	/* Federate Ambassador services */
	void
	timeRegulationEnabled(const RTI::FedTime& theTime)
	throw (RTI::InvalidFederationTime,
			RTI::EnableTimeRegulationWasNotPending,
			RTI::FederateInternalError) {
		cout << "Time Regulation Enabled, press ENTER to continue..." << endl;
		cout << static_cast<RTIfedTime>(theTime).getTime() << endl;
		TIME_REGULATION_ENABLED = true;
		getchar();
	}

	void
	timeConstrainedEnabled(const RTI::FedTime& theTime)
	throw (RTI::InvalidFederationTime,
			RTI::EnableTimeConstrainedWasNotPending,
			RTI::FederateInternalError) {
		cout << "Time Constrained Enabled, press ENTER to continue..." << endl;
		cout << static_cast<RTIfedTime>(theTime).getTime() << endl;
		TIME_CONSTRAINED_ENABLED = true;
		getchar();
	}

protected:
	RTI::RTIambassador rtiamb;
};

int main(int argc, char **argv) {
	TRTCCallbacksFed testFed;
	if (argc>1) {
		testFed.setNbTARStepToDo(atoi(argv[1]));
	}

	return testFed.main();
}
