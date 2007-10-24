/**
 * @file processFederate.cc
 *
 * @brief This federate simulates a low pass process of first order.
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
 * The process federate computes the output variable y(t) on basis of
 * its input variable. The input value consists of the regulating variable u(t)
 * provided by the control federate.
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

#include "DisplayServiceExplanation.hh"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::auto_ptr;

/**
 * The class ProcessFedAmb realizes the implementation of required
 * federate ambassador services. It holds the data of incoming callbacks too.
 * Flags are defined additionally. So the occurrence of special federate
 * services can be determined in main().
 */
class ProcessFedAmb : public NullFederateAmbassador {

private:

    /*
     * private variables:
     *
     * output variable u provided by the control federate
     * flag discovU to determine federate service discoverObjectInstance(...)
     * flag newU to determine federate service reflectAttributeValues(...)
     */

    double u;

    bool discovU;
    bool newU;

public:

    /* constructor initializes private variables. */
    ProcessFedAmb() : NullFederateAmbassador() {
        u          = 0;
        discovU    = false;
        newU       = false;
    }

    /**
     * @defgroup DataEncapsulation Data Encapsulation
     * Methods in this group set and get private variables.
     */

    /**
     * Get value of private variable u.
     * @ingroup DataEncapsulation
     * @return the u value
     */
    double
    getU() {
        return u;
    }

    /**
     * Get value of private variable discovU which serves as
     * flag for the occurrence of the federate service
     * discoverObjectInstance(...).
     * @ingroup DataEncapsulation
     * @return true if u was discovered false otherwise
     */
    bool
    getDiscoverU() {
        return discovU;
    }

    /**
     * Get value of private variable newU which serves as flag
     * for the occurrence of the federate service
     * reflectAttributeValues(...).
     * @ingroup DataEncapsulation
     * @return the newU value
     */
    bool
    getNewU() {
        return newU;
    }

    /**
     * Set value of private variable discovU which serves as
     * flag for the occurrence of the federate service
     * discoverObjectInstance(...).
     * @ingroup DataEncapsulation
     * @return true if u was discovered false otherwise
     */
    void
    setDiscoverU (bool val) {
        discovU = val;
    }

    /**
     * Set value of private variable newU which serves as
     * flag for the occurrence of the federate service
     * reflectAttributeValues(...).
     * @ingroup DataEncapsulation
     * @return true if new u arrived false otherwise
     */
    void
    setNewU (bool val) {
        newU = val;
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
    discoverObjectInstance ( RTI::ObjectHandle theObject,
                             RTI::ObjectClassHandle theObjectClass,
                             const char *theObjectName)
    throw ( RTI::CouldNotDiscover,
            RTI::ObjectClassNotKnown,
            RTI::FederateInternalError) {
        cout << "discoverObjInst " << theObjectName << endl;
        if (!strcmp(theObjectName,"u")) {
            discovU = true;
        }
    }

    /**
     * Reflect Attribute Values. This callback informs the federate of a state
     * update for a set of instance-attributes according to its current
     * subscription interests.
     * @ingroup RequiredFederateServices
     * @param[in] theObject,
     * @param[in] theAttributes,
     * @param[in]
     */
    void
    reflectAttributeValues( RTI::ObjectHandle theObject,
                            const RTI::AttributeHandleValuePairSet&
                            theAttributes,
                            const RTI::FedTime& theTime,
                            const char *theTag,
                            RTI::EventRetractionHandle theHandle)
    throw ( RTI::ObjectNotKnown,
            RTI::AttributeNotKnown,
            RTI::FederateOwnsAttributes,
            RTI::InvalidFederationTime,
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

            if (!strcmp(theTag,"u")) {
                u = *(reinterpret_cast<double*>(attrValue));
                newU = true;
            }

            delete[] attrValue;
        }
    }
};

/**
 * main of processFed.cc
 */
int main() {
    RTI::RTIambassador   rtiAmb;
    ProcessFedAmb        myFedAmb;

    string federationName = "TwoLevelController";
    string federateName   = "processFed";
    string fedFile        = "TwoLevelController.fed";

    /* Federation Management */

    cout << "Create Federation execution " << federationName << "." << endl;

    /* create federation execution */
    try {
        rtiAmb.createFederationExecution(federationName.c_str(), fedFile.c_str());
    } catch ( RTI::FederationExecutionAlreadyExists ) {
        cout << "Federation already created by another federate." << endl;
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    cout << "Federate " << federateName << " joins federation execution "
    << federationName << "." << endl;

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

    /* get handles */
    cout << "Get handles from fedFile " << fedFile.c_str() << "." << endl;

    RTI::ObjectClassHandle twoLevelContrID;
    RTI::AttributeHandle uID, yID;

    try {
        twoLevelContrID = rtiAmb.getObjectClassHandle("TwoLevelControllerClass");
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

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

    auto_ptr<RTI::AttributeHandleSet>
    attrU(RTI::AttributeHandleSetFactory::create(1));
    auto_ptr<RTI::AttributeHandleSet>
    attrY(RTI::AttributeHandleSetFactory::create(1));

    attrU->add(uID);
    attrY->add(yID);

    /* publish y */
    try {
        rtiAmb.publishObjectClass(twoLevelContrID, *attrY);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    /* subscribe to u */
    try {
        rtiAmb.subscribeObjectClassAttributes(twoLevelContrID, *attrU);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... )  {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    /* Object Management */

    /* register object */
    RTI::ObjectHandle objInstID_y;

    try {
        objInstID_y = rtiAmb.registerObjectInstance(twoLevelContrID, "y");
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    cout << "Wait for second federate." << endl;
    while (!myFedAmb.getDiscoverU()) {

        try {
            rtiAmb.tick();
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }

        sleep(1);
    }
    myFedAmb.setDiscoverU(false);

    /* simulation loop */

    const double tEnd     = 30;
    const double tStart   = 0;
    const double h        = 0.1;
    const double w        = 0.5;
    const double K        = 1;
    const double T1       = 5;

    double tSim      = tStart;
    double u         = 1;
    double y         = 0;
    double ydot      = 0;

    auto_ptr<RTI::AttributeHandleValuePairSet>
    ahvps(RTI::AttributeSetFactory::create(1));

    while ( tSim-tEnd <= 0 ) {

        while (!myFedAmb.getNewU()) {
            try {
                rtiAmb.tick();
            } catch ( RTI::Exception &e ) {
                cerr << "RTI exception: " << e._name << " ["
                << (e._reason ? e._reason : "undefined") << "]." << endl;
            } catch ( ... )  {
                cerr << "Error: unknown non-RTI exception." << endl;
            }
        }
        myFedAmb.setNewU(false);

        u = myFedAmb.getU();

        /* compute y by explicit Euler */
        ydot = ( -y + u*K ) / T1;
        y = y + h*ydot;

        /* update attribute */
        ahvps -> add(yID, (char*)&(y), sizeof(double));

        try {
            rtiAmb.updateAttributeValues(objInstID_y, *ahvps, "y");
        } catch ( RTI::Exception &e ) {
            cerr << "RTI exception: " << e._name << " ["
            << (e._reason ? e._reason : "undefined") << "]." << endl;
        } catch ( ... ) {
            cerr << "Error: unknown non-RTI exception." << endl;
        }

        ahvps -> empty ();

        cout << "tSim: " << tSim << ", u= " << u << ", y= " << y << endl;
        tSim += h;
    }

    /* Declaration Management */
    try {
        rtiAmb.unpublishObjectClass(yID);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    try {
        rtiAmb.unsubscribeObjectClass(uID);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    /* Federation Management */

    cout << "Federate " << federateName << " resigns federation execution "
    << federationName << "." << endl;

    /* resign federation exection */
    try {
        rtiAmb.resignFederationExecution(
            RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
    } catch ( RTI::Exception &e ) {
        cerr << "RTI exception: " << e._name << " ["
        << (e._reason ? e._reason : "undefined") << "]." << endl;
    } catch ( ... ) {
        cerr << "Error: unknown non-RTI exception." << endl;
    }

    cout << "Destroy Federation execution " << federationName << "." << endl;

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

    cout << "Successful termination." << endl;

    return 0;
}
