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
	bool creator;
	bool TIME_REGULATION_ENABLED, TIME_CONSTRAINED_ENABLED;
	bool TIME_ADVANCE_GRANT;
	typedef struct UpdateMeH {
		RTI::ObjectClassHandle och_updateMe;
		RTI::AttributeHandle   ah_pingMe;
		RTI::AttributeHandle   ah_counter;
	} UpdateMeH_t;

	UpdateMeH_t umoh;
	typedef struct UpdateMeObject {
		RTI::ObjectHandle      oh_updateMe;
		libhla::HLAdata<libhla::HLAinteger64LE> pingMe;
		libhla::HLAdata<libhla::HLAinteger64LE> counter;
	} UpdateMeObject_t;

	std::vector<UpdateMeObject_t> umo;
	RTIfedTime mytime;
	RTIfedTime myLookahead;

public:
	TRTCCallbacksFed(void) {
		UpdateMeObject_t initVal;
		federationName = "TestTRTCCallbacks";
		fedFile = "test_TRTCCallbacks.fed";
		creator = false;
		TIME_REGULATION_ENABLED = false;
		TIME_CONSTRAINED_ENABLED = false;
		umo.reserve(MAX_FEDERATES);
		*(initVal.pingMe) = 0;
		*(initVal.counter) = 0;
		umo[0] = initVal;
		// initial simulation time is 0
		mytime.setZero();
		myLookahead.setZero();
		// initial [default lookahead is 1.0]
		myLookahead = myLookahead + RTIfedTime(1.0);
	};

	virtual ~TRTCCallbacksFed() throw (RTI::FederateInternalError) {
	};

	int
	main(void) {
		this->create_and_join();
		this->getHandles();
		if (creator) {
			cout << "Press ENTER to start execution ..." << endl;
			getchar();

			this->enableTimeReg();
			while (!TIME_REGULATION_ENABLED) this->tickRTI();
			TIME_REGULATION_ENABLED=false;
			this->enableTimeCon();
			while (!TIME_CONSTRAINED_ENABLED) this->tickRTI();
			TIME_CONSTRAINED_ENABLED=false;
		} else {
			this->enableTimeReg();
			while (!TIME_REGULATION_ENABLED) this->tickRTI();
			TIME_REGULATION_ENABLED=false;
			this->enableTimeCon();
			while (!TIME_CONSTRAINED_ENABLED) this->tickRTI();
			TIME_CONSTRAINED_ENABLED=false;
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
			umo[0].oh_updateMe = rtiamb.registerObjectInstance(umoh.och_updateMe);
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
		ahvps->add(umoh.ah_counter, umo[0].counter.data(), umo[0].counter.size());
		ahvps->add(umoh.ah_pingMe, umo[0].pingMe.data(), umo[0].pingMe.size());

		try {
			if (ts) {
				rtiamb.updateAttributeValues(umo[0].oh_updateMe, *ahvps,mytime+myLookahead, "upd");
			}
			else {
				rtiamb.updateAttributeValues(umo[0].oh_updateMe, *ahvps,"upd");
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
	advanceTime(double time) {
		try {
			rtiamb.timeAdvanceRequest(RTIfedTime(time));
		} catch ( RTI::Exception &e ) {
			cerr << "RTI exception: " << e._name << " ["
					<< (e._reason ? e._reason : "undefined") << "]." << endl;
		} catch ( ... ) {
			cerr << "Error: unknown non-RTI exception." << endl;
		}
		while (!TIME_ADVANCE_GRANT) this->tickRTI();
		TIME_ADVANCE_GRANT = false;
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
	return testFed.main();
}
