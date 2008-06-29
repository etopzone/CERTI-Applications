/**
 * @file controllerFederate.cc
 *
 * @brief This federate simulates a two level controller.
 *
 * The federate serves as a HLA RunTime Infrastructure API 1.3 tutorial based
 * on CERTI.
 *
 * The HLA API is seperated in so called RTI ambassador services and federate
 * ambassador services. The RTI ambassador services are provided by an
 * RTIambassador object and are handled like normal method invocations. In
 * contrast, federate ambassador services have to be implemented within the
 * federate and can be regarded as a callback mechanism.
 *
 * In this federate the usage of nessessary RTI ambassador method invocations
 * and their correct calling order are shown for the management areas
 * Federation Management, Declaration Management and Object Management. The
 * needed federate ambassador services are implemented in a separate class.
 *
 * The controller federate computes the regulating variable u(t) on basis of
 * its input variables. The input values consist of a static reference
 * variable w(t) and the output variable y(t) provided by the process
 * federate. The regulating variable u(t) serves as input for the process
 * federate.
 */

/*
 * <RTI.hh>
 * This header is the necessary include for using HLA RunTime Infrastructure
 * API.
 */
#include <RTI.hh>

/*
 * <NullFederateAmbassador.hh>
 * The header provides virtual methods of the abstract basis class
 * FederateAmbassador included by <RTI.hh>. When using the
 * NullFederateAmbassador only required federate services have to be
 * implemented.
 */
#include <NullFederateAmbassador.hh>

#include <iostream>
#include <memory>
#include <string>

#include <string.h>
#include "DisplayServiceExplanation.hh"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::auto_ptr;

/**
 * The class ControlFedAmb realizes the implementation of required
 * federate ambassador services. It holds the data of incoming callbacks too.
 * Flags are defined additionally. So the occurrence of special federate
 * services can be determined in main().
 */
class ControlFedAmb : public NullFederateAmbassador {

private:

    /*
     * private variables:
     *
     * output variable y provided by the process federate
     * flag discovY to determine federate service discoverObjectInstance(...)
     * flag newY to determine federate service reflectAttributeValues(...)
     */

    double y;

    bool discovY;
    bool newY;

public:

    /* constructor initializes private variables. */
    ControlFedAmb() : NullFederateAmbassador() {
        y          = 0;
        discovY    = false;
        newY       = false;
    }

    /**
     * @defgroup DataEncapsulation Data Encapsulation
     * Methods in this group set and get private variables.
     */

    /**
     * Get value of private variable y.
     * @ingroup DataEncapsulation
     * @return the y value
     */
    double
    getY() {
        return y;
    }

    /**
     * Get value of private variable discovY which serves as
     * flag for the occurrence of the federate service
     * discoverObjectInstance(...).
     * @ingroup DataEncapsulation
     * @return true if y was discovered false otherwise
     */
    bool
    getDiscoverY() {
        return discovY;
    }

    /**
     * Get value of private variable newY which serves as flag
     * for the occurrence of the federate service
     * reflectAttributeValues(...).
     * @ingroup DataEncapsulation
     * @return the newY value
     */
    bool
    getNewY() {
        return newY;
    }

    /**
     * Set value of private variable discovY which serves as
     * flag for the occurrence of the federate service
     * discoverObjectInstance(...).
     * @ingroup DataEncapsulation
     * @return true if y was discovered false otherwise
     */
    void
    setDiscoverY (bool val) {
        discovY = val;
    }

    /**
     * Set value of private variable newY which serves as
     * flag for the occurrence of the federate service
     * reflectAttributeValues(...).
     * @ingroup DataEncapsulation
     * @return true if new y arrived false otherwise
     */
    void
    setNewY (bool val) {
        newY = val;
    }

    /**
     * @defgroup RequiredFederateServices Required Federate Services
     * The method of this group MUST be implemented by the federate
     * and will be invoked by the RTI.
     */

    /**
     * Discover object instance. This callback informs the federate of the
     * existence of an object instance in the federation according 
     * to the federate's current subscription interests.
     * @ingroup RequiredFederateServices
     * @param[in] theObject,
     * @param[in] theObjectClass,
     * @param[in] theObjectName, the object name to discover
     * @exception RTI::CouldNotDiscover could not discover the specified object
     * @exception RTI::ObjectClassNotKnown the specified object class is unknown
     * @exception RTI::FederateInternalError internal error
     */
    void
    discoverObjectInstance (RTI::ObjectHandle theObject,
                            RTI::ObjectClassHandle theObjectClass,
                            const char *theObjectName)
    throw ( RTI::CouldNotDiscover,
            RTI::ObjectClassNotKnown,
            RTI::FederateInternalError) {
        cout << "discoverObjInst " << theObjectName << endl;
        if (!strcmp(theObjectName,"y")) {
            discovY = true;
        }
    }

    /**
     * Reflect Attribute Values. This callback informs the federate of a state
     * update for a set of instance-attributes according to its current
     * subscription interests.
     * @ingroup RequiredFederateServices
     * @param[in] theObject,
     * @param[in] theAttributes,
     * @param[in] theTag
     */
    void
    reflectAttributeValues (RTI::ObjectHandle theObject,
                            const RTI::AttributeHandleValuePairSet&
                            theAttributes,
                            const char *theTag)
    throw ( RTI::ObjectNotKnown,
            RTI::AttributeNotKnown,
            RTI::FederateOwnsAttributes,
            RTI::FederateInternalError) {
        RTI::ULong length;
        char *attrValue;

        /*
        * common implementation scheme is to iterate through the
        * AttributeHandleValuePairSet.
        */
        for (unsigned int i=0; i<theAttributes.size(); i++) {

            RTI::AttributeHandle handle = theAttributes.getHandle(i);
            length = theAttributes.getValueLength(i);
            attrValue = new char[length];
            theAttributes.getValue(i, attrValue, length);

            if (!strcmp(theTag,"y")) {
                y = *(reinterpret_cast<double*>(attrValue));
                newY = true;
            }

            delete[] attrValue;
        }
    }
};

/**
 * main of controlFed.cc
 */
int main() {
    RTI::RTIambassador   rtiAmb;
    ControlFedAmb        myFedAmb;

    DisplayServiceExplanation dispSE;

    string federationName = "TwoLevelController";
    string federateName   = "controlFed";
    string fedFile        = "TwoLevelController.fed";

    dispSE.dispTutorialIntroduction();

    /* Federation Management */
    dispSE.dispFederationManagement();

    dispSE.dispCreateFederationExecution();

    /* create federation execution */
    try {
        rtiAmb.createFederationExecution(federationName.c_str(), 
					 fedFile.c_str());
    } catch ( RTI::FederationExecutionAlreadyExists ) {
        cout << "Federation already created by another federate." 
	<< endl;
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    dispSE.dispJoinFederationExecution();

    /* join federation execution */
    try {
        rtiAmb.joinFederationExecution(federateName.c_str(),
                                       federationName.c_str(),
                                       &myFedAmb);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    /* Declaration Management */

    dispSE.dispDeclarationManagement();

    /* get handles */

    dispSE.dispGetObjectClassHandle();

    /* get object class handle */
    RTI::ObjectClassHandle twoLevelContrID;

    try {
        twoLevelContrID = rtiAmb.getObjectClassHandle("TwoLevelControllerClass");
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    dispSE.dispGetAttributeHandle();

    /* get attribute handle */
    RTI::AttributeHandle uID, yID;
    try {
        uID = rtiAmb.getAttributeHandle("U", twoLevelContrID);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    try {
        yID = rtiAmb.getAttributeHandle("Y", twoLevelContrID);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    dispSE.dispAddToAttributeHandleSet();

    /* add attribute handle to AttributeHandleSet */
    auto_ptr<RTI::AttributeHandleSet>
    attrU(RTI::AttributeHandleSetFactory::create(1));
    auto_ptr<RTI::AttributeHandleSet>
    attrY(RTI::AttributeHandleSetFactory::create(1));

    attrU->add(uID);
    attrY->add(yID);

    dispSE.dispPublishObjectClass();

    /* publish u */
    try {
        rtiAmb.publishObjectClass(twoLevelContrID, *attrU);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    dispSE.dispSubscribeToObjectClass();

    /* subscribe to y */
    try {
        rtiAmb.subscribeObjectClassAttributes(twoLevelContrID, *attrY);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    /* Object Management */

    dispSE.dispObjectManagement();

    dispSE.dispRegisterObjectInstance();

    /* register object */
    RTI::ObjectHandle objInstID_u;

    try {
        objInstID_u = rtiAmb.registerObjectInstance(twoLevelContrID, "u");
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    dispSE.dispTick();
    dispSE.dispDiscoverObjectInstance();

    /* discover y */
    cout << "Wait for second federate." << endl;
    while (!myFedAmb.getDiscoverY()) {

        try {
            rtiAmb.tick();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }

#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }
    myFedAmb.setDiscoverY(false);

    /* simulation loop */

    const double tEnd     = 30;
    const double tStart   = 0;
    const double h        = 0.1;
    const double w        = 0.5;

    double tSim      = tStart;
    double u         = 1;
    double y         = 0;

    auto_ptr<RTI::AttributeHandleValuePairSet>
    ahvps(RTI::AttributeSetFactory::create(1));

    while ( tSim-tEnd <= 0 ) {

        y = myFedAmb.getY();

        if (y < w-0.1 ) u=1;
        if (y > w+0.1 ) u=0;

        /* update attribute */
        ahvps -> add(uID, (char*)&(u), sizeof(double));

        try {
            rtiAmb.updateAttributeValues(objInstID_u, *ahvps, "u");
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }

        ahvps -> empty ();

        /* tick */
        while (!myFedAmb.getNewY()) {

            try {
                rtiAmb.tick();
            } catch ( RTI::Exception &e ) {
                cerr << "RTI exception: " << e._name << " ["
                << (e._reason ? e._reason : "undefined") << "]." << endl;
            } catch ( ... ) {
                cerr << "Error: unknown non-RTI exception." << endl;
            }

        }
        myFedAmb.setNewY(false);

	if (tSim==tStart) {
	    cout << "gaskfgkjh" << endl;
	    dispSE.dispUpdateAttributeValues();
	    dispSE.dispReflectAttributeValues();
	}

        cout << "tSim: " << tSim << ", u= " << u << ", y= " << y << endl;
        tSim += h;
    }

    /* Declaration Management */

    try {
        rtiAmb.unpublishObjectClass(uID);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    try {
        rtiAmb.unsubscribeObjectClass(yID);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    /* Federation Management */

    dispSE.dispResignFederationExecution();

    /* resign federation execution */
    try {
        rtiAmb.resignFederationExecution(
            RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    dispSE.dispDestroyFederationExecution();

    /* destroy federation execution */
    try {
        rtiAmb.destroyFederationExecution(federationName.c_str());
    } catch (RTI::FederatesCurrentlyJoined) {
        cout << "Federates currently joined." << endl;
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    return 0;
}
