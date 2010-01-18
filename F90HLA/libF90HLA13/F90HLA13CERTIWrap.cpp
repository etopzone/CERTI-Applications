/*
    Copyright (C) 2008 Christian Stenzel

    This file is part of F90HLA13.

    F90HLA13 is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as 
    published by the Free Software Foundation, either version 3 of 
    the License, or (at your option) any later version.

    F90HLA13 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public 
    License along with F90HLA13.  
    
    If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef __INTEL_COMPILER
#pragma warning(disable : 383)
#pragma warning(disable : 592)
#pragma warning(disable : 810)
#pragma warning(disable : 869)
#pragma warning(disable : 981)
#pragma warning(disable : 1418)
#pragma warning(disable : 1419)
#pragma warning(disable : 1469)
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <netinet/in.h>
#include <stdlib.h>
#include <memory>

#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#if defined __INTEL_COMPILER

// symbols federate services (Intel)

// federation management

#define _F90syncPointRegSucceeded fedsrvmod_mp_syncpointregsucceeded_
#define _F90syncPointRegFailed fedsrvmod_mp_syncpointregfailed_
#define _F90announceSyncPoint fedsrvmod_mp_announcesyncpoint_
#define _F90fedSync fedsrvmod_mp_fedsync_
#define _F90initiateFedSave fedsrvmod_mp_initiatefedsave_
#define _F90fedSaved fedsrvmod_mp_fedsaved_
#define _F90fedNotSaved fedsrvmod_mp_fednotsaved_
#define _F90requestFedResSucceeded fedsrvmod_mp_requestfedressucceeded_
#define _F90requestFedResFailed fedsrvmod_mp_requestfedresfailed_
#define _F90fedResBegun fedsrvmod_mp_fedresbegun_
#define _F90initiateFedRestore fedsrvmod_mp_initiatefedrestore_
#define _F90fedRestored fedsrvmod_mp_fedrestored_
#define _F90fedNotRestored fedsrvmod_mp_fednotrestored_

// declaration management

#define _F90startRegForObjClass fedsrvmod_mp_startregforobjclass_
#define _F90stopRegforObjClass fedsrvmod_mp_stopregforobjclass_
#define _F90turnInteractOn fedsrvmod_mp_turninteracton_
#define _F90turnInteractOff fedsrvmod_mp_turninteractoff_

// object management

#define _F90discoverObjInst fedsrvmod_mp_discoverobjinst_
#define _F90reflectAttrVal fedsrvmod_mp_reflectattrval_
#define _F90receiveInteract fedsrvmod_mp_receiveinteract_
#define _F90removeObjInst fedsrvmod_mp_removeobjinst_
#define _F90attrInScope fedsrvmod_mp_attrinscope_
#define _F90attrOutOfScope fedsrvmod_mp_attroutofscope_
#define _F90provideAttrValUp fedsrvmod_mp_provideattrvalup_
#define _F90turnUpdatesOnForObjInst fedsrvmod_mp_turnUpdatesOnForObjInst_
#define _F90turnUpdatesOffForObjInst fedsrvmod_mp_turnUpdatesOffForObjInst_


// time management

#define _F90timeRegEnabled fedsrvmod_mp_timeregenabled_
#define _F90timeConsEnabled fedsrvmod_mp_timeconsenabled_
#define _F90timeAdvGrant fedsrvmod_mp_timeadvgrant_
#define _F90requestRetr fedsrvmod_mp_requestretr_

#elif defined __GNUG__

// symbols federate services (GNU)

// federation management

#define _F90syncPointRegSucceeded __fedsrvmod_MOD_syncpointregsucceeded
#define _F90syncPointRegFailed __fedsrvmod_MOD_syncpointregfailed
#define _F90announceSyncPoint __fedsrvmod_MOD_announcesyncpoint
#define _F90fedSync __fedsrvmod_MOD_fedsync
#define _F90initiateFedSave __fedsrvmod_MOD_initiatefedsave
#define _F90fedSaved __fedsrvmod_MOD_fedsaved
#define _F90fedNotSaved __fedsrvmod_MOD_fednotsaved
#define _F90requestFedResSucceeded __fedsrvmod_MOD_requestfedressucceeded
#define _F90requestFedResFailed __fedsrvmod_MOD_requestfedresfailed
#define _F90fedResBegun __fedsrvmod_MOD_fedresbegun
#define _F90initiateFedRestore __fedsrvmod_MOD_initiatefedrestore
#define _F90fedRestored __fedsrvmod_MOD_fedrestored
#define _F90fedNotRestored __fedsrvmod_MOD_fednotrestored

// declaration management

#define _F90startRegForObjClass __fedsrvmod_MOD_startregforobjclass
#define _F90stopRegforObjClass  __fedsrvmod_MOD_stopregforobjclass
#define _F90turnInteractOn __fedsrvmod_MOD_turninteracton
#define _F90turnInteractOff __fedsrvmod_MOD_turninteractoff

// object management

#define _F90objInstNameResSucceeded __fedsrvmod_MOD_objinstnameressucceeded
#define _F90objInstNameResFailed __fedsrvmod_MOD_objinstnameresfailed
#define _F90discoverObjInst __fedsrvmod_MOD_discoverobjinst
#define _F90reflectAttrVal __fedsrvmod_MOD_reflectattrval
#define _F90receiveInteract __fedsrvmod_MOD_receiveinteract
#define _F90removeObjInst __fedsrvmod_MOD_removeobjinst
#define _F90attrInScope __fedsrvmod_MOD_attrinscope
#define _F90attrOutOfScope __fedsrvmod_MOD_attroutofscope
#define _F90provideAttrValUp __fedsrvmod_MOD_provideattrvalup
#define _F90turnUpdatesOnForObjInst __fedsrvmod_MOD_turnUpdatesOnForObjInst
#define _F90turnUpdatesOffForObjInst __fedsrvmod_MOD_turnUpdatesOffForObjInst

// time management

#define _F90timeRegEnabled __fedsrvmod_MOD_timeregenabled
#define _F90timeConsEnabled __fedsrvmod_MOD_timeconsenabled
#define _F90timeAdvGrant __fedsrvmod_MOD_timeadvgrant
#define _F90requestRetr __fedsrvmod_MOD_requestretr

#endif

// Exceptions

#define __ArrayIndexOutOfBounds                      -1
#define __AsynchronousDeliveryAlreadyDisabled        -2
#define __AsynchronousDeliveryAlreadyEnabled         -3
#define __AttributeAcquisitionWasNotRequested        -4
#define __AttributeAcquisitionWasNotCanceled         -5
#define __AttributeAlreadyBeingAcquired              -6
#define __AttributeAlreadyBeingDivested              -7
#define __AttributeAlreadyOwned                      -8
#define __AttributeDivestitureWasNotRequested        -9
#define __AttributeNotDefined                       -10
#define __AttributeNotKnown                         -11
#define __AttributeNotOwned                         -12
#define __AttributeNotPublished                     -13
#define __ConcurrentAccessAttempted                 -14
#define __CouldNotDiscover                          -15
#define __CouldNotOpenFED                           -16
#define __CouldNotRestore                           -17
#define __DeletePrivilegeNotHeld                    -18
#define __DimensionNotDefined                       -19
#define __EnableTimeConstrainedPending              -20
#define __EnableTimeConstrainedWasNotPending        -21
#define __EnableTimeRegulationPending               -22
#define __EnableTimeRegulationWasNotPending         -23
#define __ErrorReadingFED                           -24
#define __EventNotKnown                             -25
#define __FederateAlreadyExecutionMember            -26
#define __FederateInternalError                     -27
#define __FederateLoggingServiceCalls               -28
#define __FederateNotExecutionMember                -29
#define __FederateOwnsAttributes                    -30
#define __FederateWasNotAskedToReleaseAttribute     -31
#define __FederatesCurrentlyJoined                  -32
#define __FederationExecutionAlreadyExists          -33
#define __FederationExecutionDoesNotExist           -34
#define __FederationTimeAlreadyPassed               -35
#define __HandleValuePairMaximumExceeded            -36
#define __InteractionClassNotDefined                -37
#define __InteractionClassNotKnown                  -38
#define __InteractionClassNotPublished              -39
#define __InteractionClassNotSubscribed             -40
#define __InteractionParameterNotDefined            -41
#define __InteractionParameterNotKnown              -42
#define __InvalidExtents                            -43
#define __InvalidFederationTime                     -44
#define __InvalidHandleValuePairSetContext          -45
#define __InvalidLookahead                          -46
#define __InvalidOrderingHandle                     -47
#define __InvalidRegionContext                      -48
#define __InvalidResignAction                       -49
#define __InvalidRetractionHandle                   -50
#define __InvalidTransportationHandle               -51
#define __MemoryExhausted                           -52
#define __NameNotFound                              -53
#define __ObjectClassNotDefined                     -54
#define __ObjectClassNotKnown                       -55
#define __ObjectClassNotPublished                   -56
#define __ObjectClassNotSubscribed                  -57
#define __ObjectNotKnown                            -58
#define __ObjectAlreadyRegistered                   -59
#define __OwnershipAcquisitionPending               -60
#define __RegionInUse                               -61
#define __RegionNotKnown                            -62
#define __RestoreInProgress                         -63
#define __RestoreNotRequested                       -64
#define __RTIinternalError                          -65
#define __SpaceNotDefined                           -66
#define __SaveInProgress                            -67
#define __SaveNotInitiated                          -68
#define __SpecifiedSaveLabelDoesNotExist            -69
#define __SynchronizationPointLabelWasNotAnnounced  -70
#define __TimeAdvanceAlreadyInProgress              -71
#define __TimeAdvanceWasNotInProgress               -72
#define __TimeConstrainedAlreadyEnabled             -73
#define __TimeConstrainedWasNotEnabled              -74
#define __TimeRegulationAlreadyEnabled              -75
#define __TimeRegulationWasNotEnabled               -76
#define __UnableToPerformSave                       -77
#define __ValueCountExceeded                        -78
#define __ValueLengthExceeded                       -79

extern "C" {

    void rtion1_(int*);     // error
    void rtioff1_(int*);    // error

    /////////////////////////////////////
    // RTI services                    //
    /////////////////////////////////////


    /////////////////////////////////////
    // federation management (4)       //
    /////////////////////////////////////

    // 4.2
    extern void
        createfedexec5_(const char*, const int*,   // fedExecName
                        const char*, const int*,   // path2FDD
                        int*);                     // error

    // 4.3
    extern void
        destroyfedexec3_(const char*, const int*,   // fedExecName
                         int*);                     // error

    // 4.4
    extern void
        joinfedexec6_(const char*, const int*,  // fedType
                      const char*, const int*,  // fedExecName
                      int*,                     // fedHdl (retVal)
                      int*);                    // error

    // 4.5
    extern void
        resignfedexec2_(const int*,    // resignAction
                        int*);         // error

    // 4.6
    extern void
        registerfedsyncpoint5_(const char*, const int*, // label
                               const char*, const int*, // theTag
                               int*);                   // error

    extern void
        registerfedsyncpoint7_(const char*, const int*, // label
                               const char*, const int*, // theTag
                               const int*, const int*,  // fedHdlSet
                               int*);                   // error

    // 4.9
    extern void
        syncpointachieved3_(const char*, const int*,  // label
                            int*);                    // error

    // 4.11
    extern void
        requestfedsave3_(const char*, const int*,  // label
                         int*);                    // error

    extern void
        requestfedsave4_(const char*, const int*,  // label
                         const double*,            // theTime
                         int*);                    // error

    // 4.13
    extern void
        fedsavebegun1_(int*);          // error

    // 4.14
    extern void
        fedsavecomplete1_(int*);       // error

    extern void
        fedsavenotcomplete1_(int*);    // error

    // 4.16
    extern void
        requestfedrestore3_(const char*, const int*,  // label
                            int*);                    // error

    // 4.20
    extern void
        fedrescomplete1_(int*);    // error

    extern void
        fedresnotcomplete1_(int*);  // error
    
    /////////////////////////////////////
    // declaration management (5)      //
    /////////////////////////////////////
    
    // 5.2
    extern void 
  	publishobjclass4_(const int*,                     // ObjClassHdl
                          const int*, const int*,         // AttrHdlSet
                          int*);                          // error

    // 5.6
    extern void
  	subscribeobjclassattr4_(const int*,                   // ObjClassHdl
                                const int*, const int*,       // AttrHdlSet
                                int*);                        // error
    
    /////////////////////////////////////
    // object management (6)           //
    /////////////////////////////////////
    
    // 6.2
    extern void
    	registerobjinst3_(const int*,			// theClass
			  int*,				// ObjHdl (retVal)
			  int*);			// error

    extern void
    	registerobjinst5_(const int*,			// theClass
			  const char*, const int*,	// theObject
			  int*,				// ObjHdl (retVal)
			  int*);			// error

    
    // 6.6
    extern void
    updateattrval7_(const int*, 				// ObjInstHdl 
		    const int*, const double*, const int*,	// AttrValMap 
		    const char*, const int*, 			// UserSuppliedTag
		    int*);					// error
  
    extern void
    updateattrval10_(const int*, 				// ObjInstHdl
		     const int*, const double*, const int*,	// AttrValMap
		     const char*, const int*, 			// UserSuppliedTag
		     const double*, 				// logTime
		     int*, 					// EventRetrHdl.Serial (retVal)
		     int*,					// EventRetrHdl.sendingFederate (retVal)
		     int*);					// error
    
    /////////////////////////////////////
    // time management (8)             //
    /////////////////////////////////////
    
    // 8.2
    extern void
    enabletimereg3_(const double*,	// theFederateTime
    		    const double*,	// theLookahead
		    int*);		// error

    // 8.5
    extern void
    enabletimecons1_(int*);		// error

    // 8.8
    extern void
    timeadvreq2_(const double*,		// theTime
		 int *);		// error

    // 8.14
    extern void
    enableasyncdeliv1_(int*);		// error


    /////////////////////////////////////
    // ancillary services              //
    /////////////////////////////////////

    extern void
    tick2_(bool*,                      // bool (retVal) 			
    	   int*);                      // error

    extern void
    tick4_(double*,                    // tickMinTime
           double*,                    // tickMaxTime
	   bool*,                      // bool (retVal)
	   int*);                      // error

    // 10.2
    extern void
    getobjclasshdl4_(const char*, const int*, 		// ObjClassName
  		     int*, 				// ObjClassHdl (retVal)
		     int*);				// error

    // 10.4
    extern void
    getattrhdl5_(const int*, 				// ObjClassHdl
	         const char*, const int*, 		// attrName
	         int*,					// attrHdl (retVal)
	         int*);					// error

    /////////////////////////////////////
    // federate services               //
    /////////////////////////////////////


    /////////////////////////////////////
    // federation management (4)       //
    /////////////////////////////////////

    // 4.7
    extern void
        _F90syncPointRegSucceeded(const char*, const int*, //label
                                  int*);                   //error
    extern void
        _F90syncPointRegFailed(const char*, const int*,    //label
                               int*);                      //error

    // 4.8
    extern void
        _F90announceSyncPoint(const char*, const int*, // label
                              const char*, const int*, // tag
                              int*);                   // error

    // 4.10
    extern void
        _F90fedSync(const char*, const int*, // label
                    int*);                   // error

    // 4.12
    extern void
        _F90initiateFedSave(const char*, const int*, // label
                            int*);                   // error

    // 4.15
    extern void
        _F90fedSaved(int*);       // error

    extern void
        _F90fedNotSaved(int*);    // error

    // 4.17
    extern void
        _F90requestFedResSucceeded(const char*, const int*,// label
                                   int*);                  // error
    extern void
        _F90requestFedResFailed(const char*, const int*,   // label
                                const char*, const int*,   // reason
                                int*);                     // error

    // 4.18
    extern void
        _F90fedResBegun(int*);   // error

    // 4.19
    extern void
        _F90initiateFedRestore(const char*, const int*, // label
                       	       const int*,              // fedHdl
                       	       int*);                   // error

    // 4.21
    extern void
        _F90fedRestored(int*);      // error

    extern void
        _F90fedNotRestored(int*);   // error
    
    
    /////////////////////////////////////
    // object management (6)           //
    /////////////////////////////////////
    
    // 6.3
    extern void
    _F90discoverObjInst(const int*, 			// ObjHdl
			const int*, 			// ObjClassHdl
			const char*, const int*, 	// ObjName
			int*);				// error

    
    // 6.7
    extern void
    _F90reflectAttrVal(const int*,				// ObjHdl
		       const int*, const double*, const int*,	// AttrHdlValPairSet
		       const double*,				// theTime
		       const char*, const int*,		// theTag
		       const int*,				// EventRetrHdl.Serial
		       const int*,				// EventRetrHdl.sendingFederate
		       int*);					// error
    
    /////////////////////////////////////
    // time management (8)             //
    /////////////////////////////////////

    // 8.13
    extern void
    _F90timeAdvGrant(const double*,	// theTime
    		     int*);		// error

}

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::auto_ptr;


/////////////////////////////////////
// conversions			   //
/////////////////////////////////////

void
charArray2String(string &dest, const char* src, unsigned int size) {
    dest.resize(size);
    for (string::size_type i = 0; i < size; i++) {
        dest[i] = static_cast<unsigned char>(src[i]);
    }
    dest[size] = '\0';
}

int
fedHdl2Int(RTI::FederateHandle fedHdl) {
    return static_cast<int>(fedHdl);
}

int
objClassHdl2Int(RTI::ObjectClassHandle objClassHdl) {
    return static_cast<int>(objClassHdl);
}

int
objHdl2Int(RTI::ObjectHandle objHdl) {
    return static_cast<int>(objHdl);
}

int
attrHdl2Int(RTI::AttributeHandle attrHdl) {
    return static_cast<int>(attrHdl);
}

void
eventRetrHdl2Int(RTI::EventRetractionHandle eventHdl, int *ret1, int *ret2) {
    *ret1 = static_cast<int>(eventHdl.theSerialNumber);
    *ret2 = static_cast<int>(eventHdl.sendingFederate);
}

/////////////////////////////////////
// net conversions		   //
/////////////////////////////////////

double
double2net (double dval) {
        double tmp;

        *((long*)&tmp) = htonl(*(((long*)&dval) +1 ));
        *(((long*)&tmp) + 1) = htonl(*((long*)&dval));
        return tmp;
}

double
net2double (double dval) {
        long tmp;

        tmp = ntohl(*(long*)&dval);
        *(long*)&dval = ntohl(*(((long*)&dval) + 1));
        *(((long*)&dval)+1) = tmp;
        return dval;
}

/////////////////////////////////////
// federate ambassador             //
/////////////////////////////////////

class F90FedAmbassador : public NullFederateAmbassador {

public:
    F90FedAmbassador() : NullFederateAmbassador() {}

    ~F90FedAmbassador() throw() {}

    /////////////////////////////////////
    // federation management (6)       //
    /////////////////////////////////////
    
    // 4.7
    void
    synchronizationPointRegistrationSucceeded(const char *label)
    throw (RTI::FederateInternalError) {

        int err = 0;

        string theLabel(label);
        int lenLabel = theLabel.size();

        _F90syncPointRegSucceeded(theLabel.c_str(), &lenLabel, &err);

        switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: syncPointRegSucceeded")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
        }
    }

    void
    synchronizationPointRegistrationFailed(const char *label)
    throw (RTI::FederateInternalError) {

        int err = 0;

        string theLabel(label);
        int lenLabel = theLabel.size();

        _F90syncPointRegFailed(theLabel.c_str(), &lenLabel, &err);

        switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: syncPointRegFailed")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
        }
    }

    // 4.8
    void
    announceSynchronizationPoint(const char *label,
                                 const char *tag)
    throw (RTI::FederateInternalError) {

        int err = 0;


        string theLabel(label), theTag(tag);

        int lenLabel = theLabel.size();
        int lenTag   = theTag.size();


        _F90announceSyncPoint(theLabel.c_str(), &lenLabel,
                              theTag.c_str(), &lenTag,
                              &err);

        switch (err) {
        case 0:
            break;
        case __FederateInternalError:
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: announceSynchronizationPoint")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
        }
    }

    // 4.10
    void
    federationSynchronized(const char *label)
    throw (RTI::FederateInternalError) {

        int err = 0;

        string theLabel(label);
        int lenLabel = theLabel.size();

        _F90fedSync(theLabel.c_str(), &lenLabel, &err);

        switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: federationSynchronized")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
        }
    }

    // 4.12
    void
    initiateFederateSave(const char *label)
    throw (RTI::FederateInternalError,
    	   RTI::UnableToPerformSave) {
        
	int err = 0;

        string theLabel(label);
        int lenLabel = theLabel.size();

        _F90initiateFedSave(theLabel.c_str(), &lenLabel, &err);
        
	switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: initiateFederateSave")
                  );
            break;
        case __UnableToPerformSave :
            throw (RTI::UnableToPerformSave(
                       __UnableToPerformSave,
                       "Federation Management: initiateFederateSave")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
        }
    }

    // 4.15
    void
    federationSaved()
    throw (RTI::FederateInternalError) {

        int err = 0;

	_F90fedSaved(&err);

        switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: federationSaved")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
	}
    }
    
    void
    federationNotSaved()
    throw (RTI::FederateInternalError) {

        int err = 0;

	_F90fedNotSaved(&err);

        switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: federationNotSaved")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
	}
    }

    // 4.17
    void
    requestFederationRestoreSucceeded(const char *label) 
    throw (RTI::FederateInternalError) {
        
	int err = 0;

        string theLabel(label);
        int lenLabel = theLabel.size();

        _F90requestFedResSucceeded(theLabel.c_str(), &lenLabel, &err);
        
	switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: requestFedRestoreSucceeded")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
	}
    }

    void
    requestFederationRestoreFailed(const char *label,
    				   const char *reason) 
    throw (RTI::FederateInternalError) {
        
	int err = 0;

        string theLabel(label), theReason(reason);
        int lenLabel = theLabel.size(), lenReason = theReason.size();

        _F90requestFedResFailed(theLabel.c_str(), &lenLabel, 
				theReason.c_str(), &lenReason,
				&err);
        
	switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: requestFedRestoreSucceeded")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
	}
    }

    // 4.18
    void
    federationRestoreBegun() 
    throw (RTI::FederateInternalError) {

        int err = 0;

	_F90fedResBegun(&err);

        switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: federationRestoreBegun")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
	}
    }

    // 4.19
    void
    initiateFederateRestore(const char *label,
    			    RTI::FederateHandle handle) 
    throw (RTI::SpecifiedSaveLabelDoesNotExist,
           RTI::CouldNotRestore,
           RTI::FederateInternalError) {

        int err = 0;
        string theLabel(label);
        int lenLabel = theLabel.size();
	int fedHdl = fedHdl2Int(handle);

	_F90initiateFedRestore(theLabel.c_str(), &lenLabel,
		               &fedHdl,
		               &err);

	switch (err) {
	case 0 :
	    break;
	case __CouldNotRestore :
	    throw (RTI::CouldNotRestore(
	                __CouldNotRestore,
			"Federation Management: initiateFederateRestore")
		  );
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                        __FederateInternalError,
		        "Federation Management: initiateFederateRestore")
                  );
	    break;
	case __SpecifiedSaveLabelDoesNotExist : 
	    throw (RTI::SpecifiedSaveLabelDoesNotExist(
	    		__SpecifiedSaveLabelDoesNotExist,
		        "Federation Management: initiateFederateRestore")
		  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
	}
    }

    // 4.21
    void
    federationRestored()
    throw (RTI::FederateInternalError) {

        int err = 0;

	_F90fedRestored(&err);

        switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: federationRestored")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
	}
    }

    void
    federationNotRestored()
    throw (RTI::FederateInternalError) {

        int err = 0;

	_F90fedNotRestored(&err);

        switch (err) {
        case 0 :
            break;
        case __FederateInternalError :
            throw (RTI::FederateInternalError(
                       __FederateInternalError,
                       "Federation Management: federationNotRestored")
                  );
            break;
        default:
            cerr << "Unknown Exception" << endl;
            break;
	}
    }
    
    /////////////////////////////////////
    // object management (6)           //
    /////////////////////////////////////
    
    // 6.3
    void
    discoverObjectInstance(
    	RTI::ObjectHandle theObject,
	RTI::ObjectClassHandle theObjectClass,
	const char* theObjectName)
    throw (
    	RTI::CouldNotDiscover,
	RTI::FederateInternalError,
	RTI::ObjectClassNotKnown
    ) {
	int err = 0;
	int objHdl;
	int objClassHdl;
	string objName(theObjectName);
	int lenObjName = objName.size();

	objHdl = objHdl2Int(theObject);
	objClassHdl = objClassHdl2Int(theObjectClass);

	cout << "DiscoverObjInst" << endl;

	_F90discoverObjInst(&objHdl, &objClassHdl,
			    objName.c_str(), &lenObjName,	
			    &err);

	switch (err) {
		case 0 : 
		  break;
		case __CouldNotDiscover :
		  throw (RTI::CouldNotDiscover(
				__CouldNotDiscover,
				"Object Management: CouldNotDiscover")
			);
		  break;
		case __ObjectClassNotKnown :
		  throw (RTI::ObjectClassNotKnown(
				__ObjectClassNotKnown,
				"ObjectClassNotKnown")
			);
		  break;
		case __FederateInternalError :
		  throw (RTI::FederateInternalError(
				__FederateInternalError,
				"FederateInternalError")
			);
		  break;
		default :
		  cout << "Unknown Exception" << endl;
		  break;
	}
    }

    // 6.5
    void
    reflectAttributeValues (
    	RTI::ObjectHandle theObject,
	const RTI::AttributeHandleValuePairSet& theAttributes,
	const RTI::FedTime& theTime,
	const char *theTag,
	RTI::EventRetractionHandle theHandle)
    throw (
    	RTI::AttributeNotKnown,
	RTI::FederateInternalError,
	RTI::FederateOwnsAttributes,
	RTI::InvalidFederationTime,
	RTI::ObjectNotKnown
    ) {
	
	int err = 0;
	int objHdl;
	int attrHdl;
	double *attrVals;
	RTI::ULong lenAttrVals;
	double logTime;
	string tag(theTag);
	int lenTag = tag.size();
	
	objHdl = objHdl2Int(theObject);
	
	for (int i=0; i<theAttributes.size(); i++) {

		RTI::AttributeHandle handle = theAttributes.getHandle(i);
		lenAttrVals = theAttributes.getValueLength(i);
		attrVals = new double[lenAttrVals];
		theAttributes.getValue(i, reinterpret_cast<char*>(attrVals), lenAttrVals);

		for (int j=0; j<lenAttrVals; j++) {
			attrVals[j] = net2double(attrVals[j]);
		}

		int lenAttrValues = lenAttrVals;
		_F90reflectAttrVal(&objHdl,
				   &attrHdl, attrVals, &lenAttrValues,
				   &logTime,
				   tag.c_str(), &lenTag,
				    NULL, 
				    NULL,
				    &err);
		
		delete attrVals;

		switch (err) {
			case 0 : 
		 	  break;
			case __AttributeNotKnown :
		  	  throw (RTI::AttributeNotKnown(
			  		__AttributeNotKnown,
					"AttributeNotKnown")
				);
			  	break;
			case __FederateInternalError :
		 	  throw (RTI::FederateInternalError(
			  		__FederateInternalError,
					"FederateInternalError")
				);
			 	break;
			case __FederateOwnsAttributes :
		 	  throw (RTI::FederateOwnsAttributes(
			  		__FederateOwnsAttributes,
					"FederateOwnsAttributes")
				);
			 	break;
			case __InvalidFederationTime :
		 	  throw (RTI::InvalidFederationTime(
			  		__InvalidFederationTime,
					"InvalidFederationTime")
				);
			 	break;
			case __ObjectNotKnown :
		  	  throw (RTI::ObjectNotKnown(
			  		__ObjectNotKnown,
					"ObjectNotKnown")
				);
			  	break;
			default :
			 	  cout << "Unknown Exception" << endl;
			  	  break;
			}

	}
    }
 
    void
    reflectAttributeValues (
    	RTI::ObjectHandle theObject,
	const RTI::AttributeHandleValuePairSet& theAttributes,
	const char *theTag)
    throw (
    	RTI::AttributeNotKnown,
	RTI::FederateInternalError,
	RTI::FederateOwnsAttributes,
	RTI::ObjectNotKnown
    ) {
    
	int err = 0;
	int objHdl;
	int attrHdl;
	double *attrVals;
	RTI::ULong lenAttrVals;
	string tag(theTag);
	int lenTag = tag.size();
	
	objHdl = objHdl2Int(theObject);

	for (int i=0; i<theAttributes.size(); i++) {

		RTI::AttributeHandle handle = theAttributes.getHandle(i);
		lenAttrVals = theAttributes.getValueLength(i);
		attrVals = new double[lenAttrVals];
		theAttributes.getValue(i, reinterpret_cast<char*>(attrVals), lenAttrVals);

		for (int j=0; j<lenAttrVals; j++) {
			attrVals[j] = net2double(attrVals[j]);
		}

		int lenAttrValues = lenAttrVals;
		_F90reflectAttrVal(&objHdl,
				   &attrHdl, attrVals, &lenAttrValues,
				   NULL,
				   tag.c_str(), &lenTag,
				   NULL, 
				   NULL,
				   &err);
		
		delete attrVals;

		switch (err) {
			case 0 : 
		 	  break;
			case __AttributeNotKnown :
		  	  throw (RTI::AttributeNotKnown(
			  		__AttributeNotKnown,
					"AttributeNotKnown")
				);
			  	break;
			case __FederateInternalError :
		 	  throw (RTI::FederateInternalError(
			  		__FederateInternalError,
					"FederateInternalError")
				);
			 	break;
			case __FederateOwnsAttributes :
		 	  throw (RTI::FederateOwnsAttributes(
			  		__FederateOwnsAttributes,
					"FederateOwnsAttributes")
				);
			 	break;
			case __ObjectNotKnown :
		  	  throw (RTI::ObjectNotKnown(
			  		__ObjectNotKnown,
					"ObjectNotKnown")
				);
			  	break;
			default :
			 	  cout << "Unknown Exception" << endl;
			  	  break;
			}

	}
    }
    
    /////////////////////////////////////
    // time management (6)             //
    /////////////////////////////////////
    
   
    // 8.13 
    void 
    timeAdvanceGrant(const RTI::FedTime& theTime) 
    throw (RTI::FederateInternalError,
    	   RTI::InvalidFederationTime,
	   RTI::TimeAdvanceWasNotInProgress) { 
	int err = 0;
	double logTime;
		
	logTime = RTIfedTime(theTime).getTime();
	
	_F90timeAdvGrant(&logTime, &err);

	switch (err) {
	case 0 : 
	  break;
	case __FederateInternalError :
	  throw (RTI::FederateInternalError(
		__FederateInternalError,
		"FederateInternalError")
		);
		break;
	case __InvalidFederationTime : 
	  throw (RTI::InvalidFederationTime(
	  	__InvalidFederationTime, 
		"InvalidLogicalTime")
		);
		break;
	case __TimeAdvanceWasNotInProgress : 
	  throw (RTI::TimeAdvanceWasNotInProgress(
	  	__TimeAdvanceWasNotInProgress, 
		"TimeAdvanceWasNotInProgress")
		);
		break;
	default : 
		cout << "Unknown Exception" << endl;
		break;
	}
    }
 
};

static F90FedAmbassador *FEDAMB;
static RTI::RTIambassador *RTIAMB;

void
rtion1_(int* err) {

    RTIAMB = new RTI::RTIambassador();
    FEDAMB = new F90FedAmbassador();

    *err = 0;
}

void
rtioff1_(int *err) {
    delete RTIAMB;
    delete FEDAMB;

    *err = 0;
}

/////////////////////////////////////
// rti ambassador                  //
/////////////////////////////////////

/////////////////////////////////////
// federation management (4)       //
/////////////////////////////////////

// 4.2
void
createfedexec5_(const char *fedExecName, const int *lenFedExecName,
                const char *FED, const int *lenFED,
                int *err) {

    string federationExecutionName;
    string FEDFile;

    charArray2String(federationExecutionName, fedExecName,
                     *lenFedExecName);
    charArray2String(FEDFile, FED,
                     *lenFED);

    try {
        RTIAMB -> createFederationExecution(
            federationExecutionName.c_str(),
            FEDFile.c_str());
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederationExecutionAlreadyExists) {
        *err = __FederationExecutionAlreadyExists;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

// 4.3
extern void
    destroyfedexec3_(const char *fedExecName, const int *lenFedExecName,
                     int *err) {

    string federationExecutionName;

    charArray2String(federationExecutionName, fedExecName,
                     *lenFedExecName);

    try {
        RTIAMB -> destroyFederationExecution(
            federationExecutionName.c_str());
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederatesCurrentlyJoined) {
        *err = __FederatesCurrentlyJoined;
    } catch (RTI::FederationExecutionDoesNotExist) {
        *err = __FederationExecutionDoesNotExist;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

// 4.4
extern void
    joinfedexec6_(const char *fedName, const int *lenFedName,
                  const char *fedExecName, const int *lenFedExecName,
                  int *ret, int *err) {

    string federateName;
    string federationExecutionName;
    RTI::FederateHandle federateHandle;

    charArray2String(federateName, fedName, *lenFedName);
    charArray2String(federationExecutionName, fedExecName,
                     *lenFedExecName);

    try {
        federateHandle = RTIAMB -> joinFederationExecution(
                             federateName.c_str(),
                             federationExecutionName.c_str(),
                             FEDAMB);

        *ret = fedHdl2Int(federateHandle);
        *err = 0;

    } catch (RTI::ConcurrentAccessAttempted) {
        ret = NULL;
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::CouldNotOpenFED) {
        ret = NULL;
        *err = __CouldNotOpenFED;
    } catch (RTI::ErrorReadingFED) {
        ret = NULL;
        *err = __ErrorReadingFED;
    } catch (RTI::FederateAlreadyExecutionMember) {
        ret = NULL;
        *err = __FederateAlreadyExecutionMember;
    } catch (RTI::FederationExecutionDoesNotExist) {
        ret = NULL;
        *err = __FederationExecutionDoesNotExist;
    } catch (RTI::RestoreInProgress) {
        ret = NULL;
        *err = __RestoreInProgress;
    } catch (RTI::RTIinternalError) {
        ret = NULL;
        *err = __RTIinternalError;
    } catch (RTI::SaveInProgress) {
        ret = NULL;
        *err = __SaveInProgress;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

// 4.5
extern void
    resignfedexec2_(const int *resignAction, int *err) {

    try {
        switch (*resignAction) {
        case 1 :
            RTIAMB -> resignFederationExecution(
                RTI::RELEASE_ATTRIBUTES);
            break;
        case 2 :
            RTIAMB -> resignFederationExecution(
                RTI::DELETE_OBJECTS);
            break;
        case 3 :
            RTIAMB -> resignFederationExecution(
                RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
            break;
        case 4 :
            RTIAMB -> resignFederationExecution(
                RTI::NO_ACTION);
            break;
        }
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::FederateOwnsAttributes) {
        *err = __FederateOwnsAttributes;
    } catch (RTI::InvalidResignAction) {
        *err = __InvalidResignAction;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

// 4.6
extern void
    registerfedsyncpoint5_(const char *label, const int *lenLabel,
                           const char *tag, const int *lenTag,
                           int *err) {

    string theLabel;
    string theTag;

    charArray2String(theLabel, label, *lenLabel);
    charArray2String(theTag, tag, *lenTag);

    try {
        RTIAMB -> registerFederationSynchronizationPoint(
            theLabel.c_str(), theTag.c_str());
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::RestoreInProgress) {
        *err = __RestoreInProgress;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (RTI::SaveInProgress) {
        *err = __SaveInProgress;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

extern void
    registerfedsyncpoint7_(const char *label, const int *lenLabel,
                           const char *tag, const int *lenTag,
                           const int *fedHdlSet, const int *numFedHdlSet,
                           int *err) {

    string theLabel;
    string theTag;
    RTI::FederateHandleSet *syncSet;

    charArray2String(theLabel, label, *lenLabel);
    charArray2String(theTag, tag, *lenTag);

    syncSet = RTI::FederateHandleSetFactory::create(*numFedHdlSet);

    try {
        for (int i=0; i<*numFedHdlSet; i++) {
            syncSet -> add(
                    static_cast<RTI::FederateHandle>(fedHdlSet[i]));
        }
    } catch (RTI::MemoryExhausted) {
        *err = __MemoryExhausted;
    } catch (RTI::ValueCountExceeded) {
        *err = __ValueCountExceeded;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }

    try {
        RTIAMB -> registerFederationSynchronizationPoint(
            theLabel.c_str(), theTag.c_str(), *syncSet);
        syncSet -> empty();
        delete syncSet;
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::RestoreInProgress) {
        *err = __RestoreInProgress;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (RTI::SaveInProgress) {
        *err = __SaveInProgress;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

// 4.9
extern void
    syncpointachieved3_(const char *label, const int *lenLabel, int *err) {

    string theLabel;

    charArray2String(theLabel, label, *lenLabel);

    try {
        RTIAMB -> synchronizationPointAchieved(theLabel.c_str());
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::RestoreInProgress) {
        *err = __RestoreInProgress;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (RTI::SaveInProgress) {
        *err = __SaveInProgress;
    } catch (RTI::SynchronizationPointLabelWasNotAnnounced) {
        *err = __SynchronizationPointLabelWasNotAnnounced;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

// 4.11
extern void
    requestfedsave3_(const char *label, const int *lenLabel,
                     int *err) {

    string theLabel;

    charArray2String(theLabel, label, *lenLabel);

    try {
        RTIAMB -> requestFederationSave(theLabel.c_str());
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::RestoreInProgress) {
        *err = __RestoreInProgress;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (RTI::SaveInProgress) {
        *err = __SaveInProgress;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

extern void
    requestfedsave4_(const char *label, const int *lenLabel,
                     const double *time,
                     int *err) {

    string theLabel;
    RTIfedTime theTime(*time);

    charArray2String(theLabel, label, *lenLabel);

    try {
        RTIAMB -> requestFederationSave(theLabel.c_str(),
                                        theTime);
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::FederationTimeAlreadyPassed) {
        *err = __FederationTimeAlreadyPassed;
    } catch (RTI::InvalidFederationTime) {
        *err = __InvalidFederationTime;
    } catch (RTI::RestoreInProgress) {
        *err = __RestoreInProgress;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (RTI::SaveInProgress) {
        *err = __SaveInProgress;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

// 4.13
extern void
    fedsavebegun1_(int *err) {

    try {
        RTIAMB -> federateSaveBegun();
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::RestoreInProgress) {
        *err = __RestoreInProgress;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (RTI::SaveNotInitiated) {
        *err = __SaveNotInitiated;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

// 4.14
extern void
    fedsavecomplete1_(int *err) {

    try {
        RTIAMB -> federateSaveComplete();
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::RestoreInProgress) {
        *err = __RestoreInProgress;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (RTI::SaveNotInitiated) {
        *err = __SaveNotInitiated;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

extern void
    fedsavenotcomplete1_(int *err) {

    try {
        RTIAMB -> federateSaveNotComplete();
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::RestoreInProgress) {
        *err = __RestoreInProgress;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (RTI::SaveNotInitiated) {
        *err = __SaveNotInitiated;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

// 4.16
extern void
    requestfedrestore3_(const char *label, const int *lenLabel,
                        int *err) {

    string theLabel;

    charArray2String(theLabel, label, *lenLabel);

    try {
        RTIAMB -> requestFederationRestore(theLabel.c_str());
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::RestoreInProgress) {
        *err = __RestoreInProgress;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (RTI::SaveInProgress) {
        *err = __SaveInProgress;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}


// 4.20
extern void
    fedrescomplete1_(int *err) {

    try {
        RTIAMB -> federateRestoreComplete();
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::RestoreNotRequested) {
        *err = __RestoreNotRequested;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

extern void
    fedresnotcomplete1_(int *err) {

    try {
        RTIAMB -> federateRestoreNotComplete();
        *err = 0;
    } catch (RTI::ConcurrentAccessAttempted) {
        *err = __ConcurrentAccessAttempted;
    } catch (RTI::FederateNotExecutionMember) {
        *err = __FederateNotExecutionMember;
    } catch (RTI::RestoreNotRequested) {
        *err = __RestoreNotRequested;
    } catch (RTI::RTIinternalError) {
        *err = __RTIinternalError;
    } catch (...) {
        cout << "Unknown Exception. Terminating" << endl;
        exit(EXIT_FAILURE);
    }
}

/////////////////////////////////////
// declaration management (5)      //
/////////////////////////////////////

// 5.2
extern void
publishobjclass4_(const int *objClassHdl, const int *attrList,
                  const int *numOfElementsInAttrList, int *err) {
       
        RTI::AttributeHandleSet *attributeList = 
		RTI::AttributeHandleSetFactory::create(*numOfElementsInAttrList);

        for (int i=0; i<*numOfElementsInAttrList; i++) {
                attributeList -> add(
                        static_cast<RTI::AttributeHandle>(*(attrList+i)));
        }

        try {
                RTIAMB -> publishObjectClass(
                        static_cast<RTI::ObjectClassHandle>(*objClassHdl),
                        *attributeList );
                attributeList -> empty();
		delete attributeList;
                *err = 0;
        }
        catch (RTI::AttributeNotDefined) {
                *err = __AttributeNotDefined;
        }
    	catch (RTI::ConcurrentAccessAttempted) {
        	*err = __ConcurrentAccessAttempted;
	}
        catch (RTI::FederateNotExecutionMember) {
                *err = __FederateNotExecutionMember;
        }
        catch (RTI::ObjectClassNotDefined) {
                *err = __ObjectClassNotDefined;
        }
        catch (RTI::RestoreInProgress) {
                *err = __RestoreInProgress;
        }
        catch (RTI::RTIinternalError) {
                *err = __RTIinternalError;
        }
        catch (RTI::SaveInProgress) {
                *err = __SaveInProgress;
        }
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}
}

// 5.6
extern void
subscribeobjclassattr4_(const int *objClassHdl, const int *attrList,
                        const int *numOfElementsInAttrList, int *err) {
        
	RTI::AttributeHandleSet *attributeList = 
		RTI::AttributeHandleSetFactory::create(*numOfElementsInAttrList);

        for (int i=0; i<*numOfElementsInAttrList; i++) {
                attributeList -> add(
                        static_cast<RTI::AttributeHandle>(*(attrList+i)));
        }

        try {
                RTIAMB -> subscribeObjectClassAttributes(
                        static_cast<RTI::ObjectClassHandle>(*objClassHdl),
                        *attributeList,
			RTI::RTI_TRUE);
                attributeList -> empty();
		delete attributeList;
                *err = 0;
        }
        catch (RTI::AttributeNotDefined) {
                *err = __AttributeNotDefined;
	}
    	catch (RTI::ConcurrentAccessAttempted) {
        	*err = __ConcurrentAccessAttempted;
	}
        catch (RTI::FederateNotExecutionMember) {
                *err = __FederateNotExecutionMember;
        }
        catch (RTI::ObjectClassNotDefined) {
                *err = __ObjectClassNotDefined;
        }
        catch (RTI::RestoreInProgress) {
                *err = __RestoreInProgress;
        }
        catch (RTI::RTIinternalError) {
                *err = __RTIinternalError;
        }
        catch (RTI::SaveInProgress) {
                *err = __SaveInProgress;
        }
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////
// object management (5)           //
/////////////////////////////////////

// 6.4
extern void
registerobjinst3_(const int *objClassHdl, 
		  int *ret, 
		  int *err) {

	RTI::ObjectHandle theObject;

	try {
		theObject = RTIAMB -> registerObjectInstance(
				static_cast<RTI::ObjectClassHandle>(*objClassHdl));
		*ret = objHdl2Int(theObject);
		*err = 0;
	}
    	catch (RTI::ConcurrentAccessAttempted) {
		ret = NULL;
        	*err = __ConcurrentAccessAttempted;
	}
        catch (RTI::FederateNotExecutionMember) {
		ret = NULL;
                *err = __FederateNotExecutionMember;
        }
        catch (RTI::ObjectClassNotDefined) {
		ret = NULL;
                *err = __ObjectClassNotDefined;
        }
        catch (RTI::ObjectClassNotPublished) {
		ret = NULL;
                *err = __ObjectClassNotPublished;
        }
        catch (RTI::RestoreInProgress) {
		ret = NULL;
                *err = __RestoreInProgress;
        }
        catch (RTI::RTIinternalError) {
		ret = NULL;
                *err = __RTIinternalError;
        }
        catch (RTI::SaveInProgress) {
		ret = NULL;
                *err = __SaveInProgress;
        }
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}
}

extern void
registerobjinst5_(const int *objClassHdl, 
		  const char *theObjInstName, const int *lenTheObjInstName, 
		  int *ret, 
		  int *err) {

	RTI::ObjectHandle theObject;
	string theObjectInstanceName;

 	charArray2String(theObjectInstanceName, theObjInstName, 
			 *lenTheObjInstName);	

	try {
		theObject = RTIAMB -> registerObjectInstance(
				static_cast<RTI::ObjectClassHandle>(*objClassHdl),
				theObjectInstanceName.c_str());
		*ret = objHdl2Int(theObject);
		*err = 0;
	}
    	catch (RTI::ConcurrentAccessAttempted) {
		ret = NULL;
        	*err = __ConcurrentAccessAttempted;
	}
        catch (RTI::FederateNotExecutionMember) {
		ret = NULL;
                *err = __FederateNotExecutionMember;
        }
        catch (RTI::ObjectAlreadyRegistered) {
		ret = NULL;
                *err = __ObjectAlreadyRegistered;
        }
        catch (RTI::ObjectClassNotDefined) {
		ret = NULL;
                *err = __ObjectClassNotDefined;
        }
        catch (RTI::ObjectClassNotPublished) {
		ret = NULL;
                *err = __ObjectClassNotPublished;
        }
        catch (RTI::RestoreInProgress) {
		ret = NULL;
                *err = __RestoreInProgress;
        }
        catch (RTI::RTIinternalError) {
		ret = NULL;
                *err = __RTIinternalError;
        }
        catch (RTI::SaveInProgress) {
		ret = NULL;
                *err = __SaveInProgress;
        }
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}
}

// 6.6
extern void
updateattrval7_(const int *objHdl, 
		const int *attrHdl, const double *attrVals, const int *lenAttrVals, 
                const char *tag, const int *lenTag,
		int *err) {

	RTI::AttributeHandleValuePairSet *ahvps =  
		RTI::AttributeSetFactory::create(*lenAttrVals);
	RTI::Handle h = *attrHdl;
	double tmp[*lenAttrVals];
	string theTag;
 	
	charArray2String(theTag, tag, *lenTag);	

	for (int i=0; i<*lenAttrVals; i++) tmp[i] = double2net(attrVals[i]);
	
	try {
		ahvps -> add (h, reinterpret_cast<char*>(tmp), *lenAttrVals*sizeof(double));
	} 
	catch (RTI::ValueLengthExceeded) {
		*err = __ValueLengthExceeded;
	}
	catch (RTI::ValueCountExceeded) {
		*err = __ValueCountExceeded;
	}
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}
		
	try {
		RTIAMB -> updateAttributeValues (
			(static_cast<RTI::ObjectHandle>(*objHdl)),
			*ahvps, 
			theTag.c_str());
		*err = 0;
	}
        catch (RTI::AttributeNotDefined) {
                *err = __AttributeNotDefined;
        }
        catch (RTI::AttributeNotOwned) {
                *err = __AttributeNotOwned;
        }
    	catch (RTI::ConcurrentAccessAttempted) {
        	*err = __ConcurrentAccessAttempted;
	}
        catch (RTI::FederateNotExecutionMember) {
                *err = __FederateNotExecutionMember;
        }
        catch (RTI::ObjectNotKnown) {
                *err = __ObjectNotKnown;
        }
        catch (RTI::RestoreInProgress) {
                *err = __RestoreInProgress;
        }
        catch (RTI::RTIinternalError) {
                *err = __RTIinternalError;
        }
        catch (RTI::SaveInProgress) {
                *err = __SaveInProgress;
        }
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}
}

extern void
updateattrval10_(const int *objHdl, const int *attrHdl, 
                 const double *attrVals, const int *lenAttrVals, 
                 const char *tag, const int *lenTag,
		 const double *time, 
		 int *ret1,
		 int *ret2,
		 int *err) {

	RTI::AttributeHandleValuePairSet *ahvps =  
		RTI::AttributeSetFactory::create(*lenAttrVals);
	RTI::Handle h = *attrHdl;
	RTI::EventRetractionHandle eventHdl;

	double tmp[*lenAttrVals];
	string theTag;
 	
	charArray2String(theTag, tag, *lenTag);	

	for (int i=0; i<*lenAttrVals; i++) tmp[i] = double2net(attrVals[i]);
	
	try {
		ahvps -> add (h, reinterpret_cast<char*>(tmp), *lenAttrVals*sizeof(double));
	} 
	catch (RTI::ValueLengthExceeded) {
		*err = __ValueLengthExceeded;
	}
	catch (RTI::ValueCountExceeded) {
		*err = __ValueCountExceeded;
	}
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}
	
	try {
		 eventHdl = RTIAMB -> updateAttributeValues (
				static_cast<RTI::ObjectHandle>(*objHdl), 
				*ahvps, 
				RTIfedTime(*time),
				theTag.c_str());
	
		int eventHdlSerial, eventHdlSendingFed;

		eventRetrHdl2Int(eventHdl, &eventHdlSerial, &eventHdlSendingFed);
		*ret1 = eventHdlSerial;
		*ret2 = eventHdlSendingFed;
		*err = 0;
	}
        catch (RTI::AttributeNotDefined) {
		ret1 = NULL;
		ret2 = NULL;
                *err = __AttributeNotDefined;
        }
        catch (RTI::AttributeNotOwned) {
		ret1 = NULL;
		ret2 = NULL;
                *err = __AttributeNotOwned;
        }
    	catch (RTI::ConcurrentAccessAttempted) {
		ret1 = NULL;
		ret2 = NULL;
        	*err = __ConcurrentAccessAttempted;
	}
        catch (RTI::FederateNotExecutionMember) {
		ret1 = NULL;
		ret2 = NULL;
                *err = __FederateNotExecutionMember;
        }
        catch (RTI::InvalidFederationTime) {
		ret1 = NULL;
		ret2 = NULL;
                *err = __InvalidFederationTime;
        }
        catch (RTI::ObjectNotKnown) {
		ret1 = NULL;
		ret2 = NULL;
                *err = __ObjectNotKnown;
        }
        catch (RTI::RestoreInProgress) {
		ret1 = NULL;
		ret2 = NULL;
                *err = __RestoreInProgress;
        }
        catch (RTI::RTIinternalError) {
		ret1 = NULL;
		ret2 = NULL;
                *err = __RTIinternalError;
        }
        catch (RTI::SaveInProgress) {
		ret1 = NULL;
		ret2 = NULL;
                *err = __SaveInProgress;
        }
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////
// time management (5)             //
/////////////////////////////////////

// 8.2
extern void
enabletimereg3_(const double* fedTime,
		const double* lh,
		int* err) {

	try {
		RTIAMB -> enableTimeRegulation(
				RTIfedTime(*fedTime),
				RTIfedTime(*lh));
		*err = 0;
	}
    	catch (RTI::ConcurrentAccessAttempted) {
        	*err = __ConcurrentAccessAttempted;
	}
        catch (RTI::EnableTimeRegulationPending) {
                *err = __EnableTimeRegulationPending;
	}
        catch (RTI::FederateNotExecutionMember) {
                *err = __FederateNotExecutionMember;
        }
        catch (RTI::InvalidFederationTime) {
                *err = __InvalidFederationTime;
        }
        catch (RTI::InvalidLookahead) {
                *err = __InvalidLookahead;
        }
        catch (RTI::RestoreInProgress) {
                *err = __RestoreInProgress;
        }
        catch (RTI::RTIinternalError) {
                *err = __RTIinternalError;
        }
        catch (RTI::SaveInProgress) {
                *err = __SaveInProgress;
        }
        catch (RTI::TimeAdvanceAlreadyInProgress) {
                *err = __TimeAdvanceAlreadyInProgress;
        }
        catch (RTI::TimeRegulationAlreadyEnabled) {
                *err = __TimeRegulationAlreadyEnabled;
        }
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}

}

// 8.5
extern void
enabletimecons1_(int* err) {

	try {
		RTIAMB -> enableTimeConstrained();
		*err = 0;
	}
    	catch (RTI::ConcurrentAccessAttempted) {
        	*err = __ConcurrentAccessAttempted;
	}
        catch (RTI::EnableTimeConstrainedPending) {
                *err = __EnableTimeConstrainedPending;
	}
        catch (RTI::FederateNotExecutionMember) {
                *err = __FederateNotExecutionMember;
        }
        catch (RTI::RestoreInProgress) {
                *err = __RestoreInProgress;
        }
        catch (RTI::RTIinternalError) {
                *err = __RTIinternalError;
        }
        catch (RTI::SaveInProgress) {
                *err = __SaveInProgress;
        }
        catch (RTI::TimeAdvanceAlreadyInProgress) {
                *err = __TimeAdvanceAlreadyInProgress;
        }
        catch (RTI::TimeConstrainedAlreadyEnabled) {
                *err = __TimeConstrainedAlreadyEnabled;
        }
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}

}

// 8.8
extern void
timeadvreq2_(const double* advTime,
	     int *err) {

	try {
		RTIAMB -> timeAdvanceRequest(
			RTIfedTime(*advTime));
		*err = 0;
	}
    	catch (RTI::ConcurrentAccessAttempted) {
        	*err = __ConcurrentAccessAttempted;
	}
        catch (RTI::EnableTimeConstrainedPending) {
                *err = __EnableTimeConstrainedPending;
	}
        catch (RTI::EnableTimeRegulationPending) {
                *err = __EnableTimeRegulationPending;
	}
        catch (RTI::FederateNotExecutionMember) {
                *err = __FederateNotExecutionMember;
        }
        catch (RTI::FederationTimeAlreadyPassed) {
                *err = __FederationTimeAlreadyPassed;
        }
        catch (RTI::InvalidFederationTime) {
                *err = __InvalidFederationTime;
        }
        catch (RTI::RestoreInProgress) {
                *err = __RestoreInProgress;
        }
        catch (RTI::RTIinternalError) {
                *err = __RTIinternalError;
        }
        catch (RTI::SaveInProgress) {
                *err = __SaveInProgress;
        }
        catch (RTI::TimeAdvanceAlreadyInProgress) {
                *err = __TimeAdvanceAlreadyInProgress;
        }
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	        exit(EXIT_FAILURE);
	}

}

// 8.14
extern void
enableasyncdeliv1_(int* err) {

	try {
		RTIAMB -> enableAsynchronousDelivery();
		*err = 0;
	}
    	catch (RTI::AsynchronousDeliveryAlreadyEnabled) {
        	*err = __AsynchronousDeliveryAlreadyEnabled;
	}
    	catch (RTI::ConcurrentAccessAttempted) {
        	*err = __ConcurrentAccessAttempted;
	}
        catch (RTI::FederateNotExecutionMember) {
                *err = __FederateNotExecutionMember;
        }
        catch (RTI::RestoreInProgress) {
                *err = __RestoreInProgress;
        }
        catch (RTI::RTIinternalError) {
                *err = __RTIinternalError;
        }
        catch (RTI::SaveInProgress) {
                *err = __SaveInProgress;
        }
    	catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
	}
}

/////////////////////////////////////
// ancillary services              //
/////////////////////////////////////

extern void
tick2_(bool *ret,	
       int *err) {

       RTI::Boolean boolVal;

	try {
		boolVal = RTIAMB -> tick();
		*ret = boolVal;
		*err = 0;
    	} catch (RTI::ConcurrentAccessAttempted) {
		ret = NULL;
        	*err = __ConcurrentAccessAttempted;
    	} catch (RTI::RTIinternalError) {
		ret = NULL;
        	*err = __RTIinternalError;
	} catch (RTI::SpecifiedSaveLabelDoesNotExist) {
		ret = NULL;
		*err = __SpecifiedSaveLabelDoesNotExist;
    	} catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
        	exit(EXIT_FAILURE);
	}
}

extern void
tick4_(double *tickMinTime,
       double *tickMaxTime,
       bool *ret, 
       int *err) {

       RTI::Boolean boolVal;

	try {
		boolVal = RTIAMB -> tick(*tickMinTime, *tickMaxTime);
		*ret = boolVal;
		*err = 0;
    	} catch (RTI::ConcurrentAccessAttempted) {
		ret = NULL;
        	*err = __ConcurrentAccessAttempted;
    	} catch (RTI::RTIinternalError) {
		ret = NULL;
        	*err = __RTIinternalError;
	} catch (RTI::SpecifiedSaveLabelDoesNotExist) {
		ret = NULL;
		*err = __SpecifiedSaveLabelDoesNotExist;
    	} catch (...) {
        	cout << "Unknown Exception. Terminating" << endl;
        	exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////
// RTI Support Services (10) 	   //
/////////////////////////////////////

// 10.2
extern void 
getobjclasshdl4_(const char *theName, const int *lenTheName, 
		 int *ret, int *err) {

	string objClassName;
	RTI::ObjectClassHandle objClassHdl;
	
	charArray2String(objClassName, theName, *lenTheName);

	try {
		objClassHdl = RTIAMB -> getObjectClassHandle(objClassName.c_str());
		*ret = objClassHdl2Int(objClassHdl);
		*err = 0;
	}
    	catch (RTI::ConcurrentAccessAttempted) {
		ret = NULL;
        	*err = __ConcurrentAccessAttempted;
	}
	catch (RTI::FederateNotExecutionMember) {
		ret = NULL;
                *err = __FederateNotExecutionMember;
        }
	catch (RTI::NameNotFound) {
		ret = NULL;
		*err = __NameNotFound;
	}
        catch (RTI::RTIinternalError) {
		ret = NULL;
                *err = __RTIinternalError;
        }
}

// 10.4
extern void
getattrhdl5_(const int *objClassHdl, 
	     const char *theName, const int *lenTheName, 
	     int *ret, int *err) {

	string theAttributeName;
	RTI::AttributeHandle attrHdl;

	charArray2String(theAttributeName, theName, *lenTheName);

	try {
		attrHdl = RTIAMB -> getAttributeHandle(
				theAttributeName.c_str(),
				static_cast<RTI::ObjectClassHandle>(*objClassHdl));
		*ret = attrHdl2Int(attrHdl);
		*err = 0;
	}
    	catch (RTI::ConcurrentAccessAttempted) {
		ret = NULL;
        	*err = __ConcurrentAccessAttempted;
	}
	catch (RTI::FederateNotExecutionMember) {
		ret = NULL;
                *err = __FederateNotExecutionMember;
        }
	catch (RTI::NameNotFound) {
		ret = NULL;
		*err = __NameNotFound;
	}
	catch (RTI::ObjectClassNotDefined) {
		ret = NULL;
		*err = __ObjectClassNotDefined;
	}
        catch (RTI::RTIinternalError) {
		ret = NULL;
                *err = __RTIinternalError;
        }
}

