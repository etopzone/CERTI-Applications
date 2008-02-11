/*
    Copyright (C) 2008 Christian Stenzel, Sven Pawletta, Thorsten Pawletta

    This file is part of MatlabHLA13.

    MatlabHLA13 is free software: you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public 
    License as published by the Free Software Foundation, either 
    version 3 of the License, or (at your option) any later version.

    MatlabHLA13 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public 
    License along with MatlabHLA13.  
    
    If not, see <http://www.gnu.org/licenses/>.
*/

/******************************************************************************/
/*
 * Matlab/RTI/Matlab - Interface
 *
 * Authors: C. Stenzel, S. Pawletta
 *					
 */

#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <iostream>
#include <exception>
#include <arpa/inet.h>

#include "mex.h"

#include "RTI.hh"
#include "NullFederateAmbassador.hh"
#include "fedtime.hh"

#ifndef _WIN32
#include <unistd.h>
#endif

extern "C" void SignalHandler(int Signal);

using std::string ;
using std::cout ;
using std::endl ;
using std::cerr ;

/*********************************************************
 * Conversions: Data Representations: Matlab <-> Network *
 *********************************************************/

static void 
double2net(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	double	dval;
	int	dims[2];

	dval = mxGetPr(prhs[0])[0];

#if defined(_X86_) || defined(i386)
	double tmp;
	*((long*)&tmp) = htonl(*(((long*)&dval) + 1));
	*(((long*)&tmp) + 1) = htonl(*((long*)&dval));
	dval = tmp;
#elif defined(__alpha)
	double tmp;
	cvt_ftof(&dval, CVT_BIG_ENDIAN_IEEE_T, &tmp, CVT_IEEE_T, 0);
	dval = tmp;
#endif

	dims[0] = sizeof(double);
	dims[1]	= 1;

	plhs[0] = mxCreateNumericArray(2, dims, mxINT8_CLASS, mxREAL);

	memcpy(mxGetPr(plhs[0]), &dval, dims[0]);

	return;
}


static void 
net2double(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	double	dval;

	memcpy(&dval, mxGetPr(prhs[0]), mxGetM(prhs[0]));

#if defined(_X86_) || defined(i386)
	long x = ntohl(*(long*)&dval);
	*(long*)&dval = ntohl(* (((long*)&dval) + 1));
	*(((long*)&dval)+1) = x;
#elif defined(__alpha)
	double x;
	cvt_ftof(&dval, CVT_IEEE_T, &x, CVT_BIG_ENDIAN_IEEE_T, 0);
	dval = x;
#endif
 
	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);

	mxGetPr(plhs[0])[0] = dval;

	return;
}


static void 
char2net(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	int	dims[2];

	dims[0] = (mxGetM(prhs[0]) * mxGetN(prhs[0])) + 1;
	dims[1]	= 1;

	plhs[0] = mxCreateNumericArray(2, dims, mxINT8_CLASS, mxREAL);

	mxGetString(prhs[0], (char*)mxGetData(plhs[0]), dims[0]);

	return;
}


static void 
net2char(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	const char	*strings[1];

	strings[0]  = (char*) mxGetData(prhs[0]);
	plhs[0]     = mxCreateCharMatrixFromStrings(1, strings);

	return;
}


/******************************************
 * Conversions: RTI Types: Matlab <-> C++ *
 ******************************************/

static char *		 				// returned
mstr2cstr(const mxArray *mstr) {			// supplied

	char	*cstr;

	int	strl;


	strl = (mxGetM(mstr) * mxGetN(mstr)) + 1;

	cstr = (char*) mxCalloc(strl, sizeof(char));

	mxGetString(mstr, cstr, strl);

	return cstr;
}


static void		 				
free_cstr(char *cstr) {					// supplied

	mxFree( (void*) cstr);

	return;
}


static mxArray*						// returned
cstr2mstr(const char *cstr) {				// supplied

	return mxCreateString(cstr);
}


static RTI::Boolean	 				// returned
mscalar2bool(const mxArray *mscalar) {			// supplied

	return ( (RTI::Boolean) (int) mxGetScalar(mscalar) );
}


static mxArray*
bool2mscalar(const RTI::Boolean boolVal) {				// supplied

	mxArray 			*mscalar;		// returned


	mscalar  	    = mxCreateDoubleMatrix(1,1,mxREAL);
	mxGetPr(mscalar)[0] = (double) boolVal;

	return mscalar;
}


static RTI::ResignAction 				// returned
mstr2resignAction(const mxArray *mstr) {		// supplied

	char	str[2];


	if ( mxIsEmpty(mstr) ) {
		return RTI::NO_ACTION;
	}

	if ( (mxGetM(mstr)*mxGetN(mstr)) > 1 ) {
		return RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES;
	}

	mxGetString(mstr, str, 2);

	if ( str[0] == 'd' ) {
		return RTI::DELETE_OBJECTS;
	}

	return RTI::RELEASE_ATTRIBUTES;
}



static RTI::ObjectClassHandle 					// returned
mscalar2objClassHdl(const mxArray *mscalar) {			// supplied

	return ( (RTI::ObjectClassHandle) mxGetScalar(mscalar) );
}


static mxArray*
objClassHdl2mscalar(const RTI::ObjectClassHandle objClassHdl) {	// supplied

	mxArray 			*mscalar;		// returned


	mscalar  	    = mxCreateDoubleMatrix(1,1,mxREAL);
	mxGetPr(mscalar)[0] = (double) objClassHdl;

	return mscalar;
}


static RTI::InteractionClassHandle 				// returned
mscalar2interactClassHdl(const mxArray *mscalar) {		// supplied

	return ( (RTI::InteractionClassHandle) mxGetScalar(mscalar) );
}


static mxArray*
interactClassHdl2mscalar(const RTI::InteractionClassHandle interactClassHdl) {

	mxArray 			*mscalar;		// returned


	mscalar  	    = mxCreateDoubleMatrix(1,1,mxREAL);
	mxGetPr(mscalar)[0] = (double) interactClassHdl;

	return mscalar;
}


static mxArray*
federateHdl2mscalar(const RTI::FederateHandle federateHdl) {	// supplied

	mxArray 			*mscalar;		// returned


	mscalar  	    = mxCreateDoubleMatrix(1,1,mxREAL);
	mxGetPr(mscalar)[0] = (double) federateHdl;

	return mscalar;
}


static mxArray*
attrHdl2mscalar(const RTI::AttributeHandle attrHdl) {		// supplied

	mxArray 			*mscalar;		// returned


	mscalar  	    = mxCreateDoubleMatrix(1,1,mxREAL);
	mxGetPr(mscalar)[0] = (double) attrHdl;

	return mscalar;
}

static mxArray*
transHdl2mscalar(const RTI::TransportationHandle transHdl) {		// supplied

	mxArray 			*mscalar;		// returned


	mscalar  	    = mxCreateDoubleMatrix(1,1,mxREAL);
	mxGetPr(mscalar)[0] = (double) transHdl;

	return mscalar;
}

static RTI::TransportationHandle 					// returned
mscalar2TransHdl(const mxArray *mscalar) {			// supplied

	return ( (RTI::TransportationHandle) mxGetScalar(mscalar) );
}

static mxArray*
orderHdl2mscalar(const RTI::OrderingHandle orderHdl) {		// supplied

	mxArray 			*mscalar;		// returned


	mscalar  	    = mxCreateDoubleMatrix(1,1,mxREAL);
	mxGetPr(mscalar)[0] = (double) orderHdl;

	return mscalar;
}

static RTI::OrderingHandle 					// returned
mscalar2orderHdl(const mxArray *mscalar) {			// supplied

	return ( (RTI::OrderingHandle) mxGetScalar(mscalar) );
}



static mxArray*
paramHdl2mscalar(const RTI::ParameterHandle paramHdl) {		// supplied

	mxArray 			*mscalar;		// returned


	mscalar  	    = mxCreateDoubleMatrix(1,1,mxREAL);
	mxGetPr(mscalar)[0] = (double) paramHdl;

	return mscalar;
}


static RTI::ObjectHandle 					// returned
mscalar2objHdl(const mxArray *mscalar) {			// supplied

	return ( (RTI::ObjectHandle) mxGetScalar(mscalar) );
}


static mxArray*
objHdl2mscalar(const RTI::ObjectHandle objHdl) {		// supplied

	mxArray 			*mscalar;		// returned


	mscalar  	    = mxCreateDoubleMatrix(1,1,mxREAL);
	mxGetPr(mscalar)[0] = (double) objHdl;

	return mscalar;
}



static RTI::TickTime 						// returned
mscalar2tickTime(const mxArray *mscalar) {			// supplied

	return ( (RTI::TickTime) mxGetScalar(mscalar) );
}



static RTI::FederateHandleSet* 
mvec2federateHdlSet(const mxArray *mvec) { 			// supplied

	RTI::FederateHandleSet		*federateHdlSet;	// returned

	int				count, i;
	RTI::FederateHandle		h;


	count = mxGetM(mvec) * mxGetN(mvec);

	try {
		federateHdlSet = RTI::FederateHandleSetFactory::create(count);
	}
	catch (RTI::Exception& e) {
		cout << "Exception: " << &e << endl;
		mexErrMsgTxt("mvec2attrHdlSet(): \
			      FederateHandleSetFactory::create() failed.");
	}
	catch (...) {
		printf("caught unknown exception");
	}


	for (i=0; i<count; i++) {
		h = (RTI::FederateHandle) (mxGetPr(mvec)[i]);
		try {
			federateHdlSet->add(h);
		}
		catch (RTI::Exception& e) {
			cout << "Exception: " << &e << endl;
			mexErrMsgTxt("mvec2federateHdlSet(): \
			              FederateHandleSetFactory::add() \
				      failed.");
		} 
		catch (...) {
			printf("caught unknown exception");
		}

	}

	return federateHdlSet;
}


static void
free_federateHdlSet(RTI::FederateHandleSet *federateHdlSet) {		// supplied

	federateHdlSet->empty();
	delete federateHdlSet;

	return;
}



static mxArray*
federateHdlSet2mvec(const RTI::FederateHandleSet &federateHdlSet) {	// supplied

	mxArray 			*mvec;			// returned

	int				count, i;
	RTI::FederateHandle		h;


	count = federateHdlSet.size();

	mvec  = mxCreateDoubleMatrix(count,1,mxREAL);

	for (i=0; i<count; i++) {
		h 		 = federateHdlSet.getHandle(i);
		mxGetPr(mvec)[i] = (double) h;
	}

	return mvec;
}


static RTI::AttributeHandleSet* 
mvec2attrHdlSet(const mxArray *mvec) { 			// supplied

	RTI::AttributeHandleSet		*attrHdlSet;	// returned

	int				count, i;
	RTI::AttributeHandle		h;


	count = mxGetM(mvec) * mxGetN(mvec);

	try {
		attrHdlSet = RTI::AttributeHandleSetFactory::create(count);
	}
	catch (RTI::Exception& e) {
		cout << "Exception: " << &e << endl;
		mexErrMsgTxt("mvec2attrHdlSet(): \
			      AttributeHandleSetFactory::create() failed.");
	}
	catch (...) {
		printf("caught unknown exception");
	}


	for (i=0; i<count; i++) {
		h = (RTI::AttributeHandle) (mxGetPr(mvec)[i]);
		try {
			attrHdlSet->add(h);
		}
		catch (RTI::Exception& e) {
			cout << "Exception: " << &e << endl;
			mexErrMsgTxt("mvec2attrHdlSet(): \
			              AttributeHandleSetFactory::add() \
				      failed.");
		} 
		catch (...) {
			printf("caught unknown exception");
		}

	}

	return attrHdlSet;
}


static void
free_attrHdlSet(RTI::AttributeHandleSet *attrHdlSet) {		// supplied

	attrHdlSet->empty();
	delete attrHdlSet;

	return;
}


static mxArray*
attrHdlSet2mvec(const RTI::AttributeHandleSet &attrHdlSet) {	// supplied

	mxArray 			*mvec;			// returned

	int				count, i;
	RTI::AttributeHandle		h;


	count = attrHdlSet.size();

	mvec  = mxCreateDoubleMatrix(count,1,mxREAL);

	for (i=0; i<count; i++) {
		h 		 = attrHdlSet.getHandle(i);
		mxGetPr(mvec)[i] = (double) h;
	}

	return mvec;
}


static RTI::AttributeHandleValuePairSet* 
mcell2attrHdlValPairSet(const mxArray *mcell) {			// supplied

	RTI::AttributeHandleValuePairSet *ahvps;		// returned

	int				count, i;
	RTI::AttributeHandle		h;
	mxArray 			*hdl, *val;


	count = mxGetM(mcell);

	try {
		ahvps = RTI::AttributeSetFactory::create(count);
	}
	catch (RTI::Exception& e) {
		cout << "Exception: " << &e << endl;
		mexErrMsgTxt("mcell2attrHdlValPairSet(): \
			      AttributeSetFactory::create() failed.");
	}
	catch (...) {
		printf("caught unknown exception");
	}


	for (i=0; i<count; i++) {

		hdl = mxGetCell(mcell,i);	// column-ordering; 1st col
		if (!hdl) {
			// no handle cell contained
			continue;
		}
		if (mxIsEmpty(hdl)) {
			// empty handle cell
			continue;
		}
		h   = (RTI::AttributeHandle) (mxGetPr(hdl)[0]);

		val = mxGetCell(mcell,count+i);	// column-ordering; 2ed col
		try {
			ahvps->add(h,
				   (char*)mxGetData(val),
				   mxGetNumberOfElements(val));
		}
		catch (RTI::Exception& e) {
			cout << "Exception: " << &e << endl;
			mexErrMsgTxt("mcell2attrHdlValPairSet(): \
			              AttributeHandleValuePairSet::add() \
				      failed.");
		} 
		catch (...) {
			printf("caught unknown exception");
		}

	}

	return ahvps;
}


static void
free_attrHdlValPairSet(RTI::AttributeHandleValuePairSet *ahvps)	{ // supplied

	ahvps->empty();
	delete ahvps;

	return;
}


static mxArray*
attrHdlValPairSet2mcell(const RTI::AttributeHandleValuePairSet &ahvps) { // sup.

	mxArray 			*mcell;			// returned

	int				count, i;
	mxArray				*hdl,*val;
	int				dims[2];
	RTI::ULong			trash;


	count = ahvps.size();

	mcell = mxCreateCellMatrix(count, 2);

	for (i=0; i<count; i++) {

		hdl = mxCreateDoubleMatrix(1, 1, mxREAL);
		mxGetPr(hdl)[0] = ahvps.getHandle(i);
		mxSetCell(mcell, i, hdl);	// column-ordering; 1st col.

		dims[0] = ahvps.getValueLength(i);
	 	dims[1] = 1;
		val = mxCreateNumericArray(2, dims, mxINT8_CLASS, mxREAL);
		ahvps.getValue(i, (char*)mxGetData(val), trash);
		mxSetCell(mcell, count+i, val);	// column-ordering; 2ed col.
	}

	return mcell;
}


static RTI::ParameterHandleValuePairSet* 
mcell2paramHdlValPairSet(const mxArray *mcell) {		// supplied

	RTI::ParameterHandleValuePairSet *phvps;		// returned

	int				count, i;
	RTI::ParameterHandle		h;
	mxArray 			*hdl, *val;


	count = mxGetM(mcell);

	try {
		phvps = RTI::ParameterSetFactory::create(count);
	}
	catch (RTI::Exception& e) {
		cout << "Exception: " << &e << endl;
		mexErrMsgTxt("mcell2paramHdlValPairSet(): \
			      ParameterSetFactory::create() failed.");

	}
	catch (...) {
		printf("caught unknown exception");
	}


	for (i=0; i<count; i++) {

		hdl = mxGetCell(mcell,i);	// column-ordering; 1st col
		if (!hdl) {
			// no handle cell contained
			continue;
		}
		if (mxIsEmpty(hdl)) {
			// empty handle cell
			continue;
		}
		h   = (RTI::ParameterHandle) (mxGetPr(hdl)[0]);

		val = mxGetCell(mcell,count+i);	// column-ordering; 2ed col
		try {
			phvps->add(h,
				   (char*)mxGetData(val),
				   mxGetNumberOfElements(val));
		}
		catch (RTI::Exception& e) {
			cout << "Exception: " << &e << endl;
			mexErrMsgTxt("mcell2paramHdlValPairSet(): \
			              ParameterHandleValuePairSet::add() \
				      failed.");
		}
		catch (...) {
			printf("caught unknown exception");
		}

		
	}

	return phvps;
}


static void
free_paramHdlValPairSet(RTI::ParameterHandleValuePairSet *phvps) { // supplied

	phvps->empty();
	delete phvps;

	return;
}


static mxArray*
paramHdlValPairSet2mcell(const RTI::ParameterHandleValuePairSet &phvps) {// sup.

	mxArray 			*mcell;			// returned

	int				count, i;
	mxArray				*hdl,*val;
	int				dims[2];
	RTI::ULong			trash;


	count = phvps.size();

	mcell = mxCreateCellMatrix(count, 2);

	for (i=0; i<count; i++) {

		hdl = mxCreateDoubleMatrix(1, 1, mxREAL);
		mxGetPr(hdl)[0] = phvps.getHandle(i);
		mxSetCell(mcell, i, hdl);	// column-ordering; 1st col.

		dims[0] = phvps.getValueLength(i);
	 	dims[1] = 1;
		val = mxCreateNumericArray(2, dims, mxINT8_CLASS, mxREAL);
		phvps.getValue(i, (char*)mxGetData(val), trash);
		mxSetCell(mcell, count+i, val);	// column-ordering; 2ed col.
	}

	return mcell;
}


static RTIfedTime* 
mscalar2fedTime(const mxArray *mscalar) {	// supplied

	RTIfedTime	*theTime;		// returned


	theTime = new RTIfedTime( mxGetScalar(mscalar) );

	return theTime;
}


static void
free_fedTime(RTIfedTime *theTime) {		// supplied

	delete theTime;

	return;
}


static mxArray*
fedTime2mscalar(const RTI::FedTime &theTime) {	// supplied

	mxArray		*mscalar;		// returned


        mscalar = mxCreateDoubleMatrix(1, 1, mxREAL);
        mxGetPr(mscalar)[0] = ((RTIfedTime&)theTime).getTime();

	return mscalar;
}


static RTI::EventRetractionHandle* 
mvec2eventRetractHdl(const mxArray *mvec) { 			// supplied
	
	RTI::EventRetractionHandle *eventRetHdl=NULL;

	
	eventRetHdl = new (RTI::EventRetractionHandle);

	eventRetHdl->theSerialNumber = (long unsigned int)mxGetPr(mvec)[0];
	eventRetHdl->sendingFederate = (long unsigned int)mxGetPr(mvec)[1];

	return eventRetHdl;

}

static void
free_eventRetHdl(RTI::EventRetractionHandle *eventRetHdl) {

	delete eventRetHdl;

	return;

}




static mxArray*
eventRetractHdl2mvec(const RTI::EventRetractionHandle theHandle) {// suppl.

	mxArray		*mvec;					// returned


        mvec             = mxCreateDoubleMatrix(1, 2, mxREAL);
        mxGetPr(mvec)[0] = (double) theHandle.theSerialNumber;
        mxGetPr(mvec)[1] = (double) theHandle.sendingFederate;

	return mvec;
}


/************************
 * Forward Declarations *
 * and Static Objects   *
 ************************/

static RTI::RTIambassador *RTIAMB = NULL;
class  MATLABambassador;
static MATLABambassador	  *FEDAMB = NULL;



/***********************
 **                   **
 ** (A) RTIambassador **
 **                   **
 ***********************/

/**********************************
 * Federation Management Services *
 **********************************/

static void 
m2createFedExec(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	char	*executionName;		// supplied C4
	char	*FED;			// supplied C4


	executionName = mstr2cstr(prhs[0]);
	FED           = mstr2cstr(prhs[1]);
	
	try {
		RTIAMB->createFederationExecution(executionName, FED);
		plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception& e) {
		plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception, probably no ambassadors, try rtiOn");
	}


	free_cstr(executionName);
	free_cstr(FED);

	return;
}


static void 
m2destroyFedExec(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	char	*executionName;		// supplied C4


	executionName = mstr2cstr(prhs[0]);
	

	try {
		RTIAMB->destroyFederationExecution(executionName);
		plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception& e) {
		plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception, probably no ambassadors, try rtiOn");
	}

	free_cstr(executionName);

	return;
}


static void 
m2joinFedExec(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTI::FederateHandle		federateHandle;		// returned C3 //0 indicates error

	char				*yourName;		// supplied C4
	char				*executionName;		// supplied C4

	// the following in parameter is actually not supplied but set
	// internally to FEDAMB
	RTI::FederateAmbassadorPtr	federateAmbassadorPtr;	// supplied C1


	yourName              = mstr2cstr(prhs[0]);
	executionName         = mstr2cstr(prhs[1]);
	federateAmbassadorPtr = (RTI::FederateAmbassador*)FEDAMB;
	
 	try {
		federateHandle = 
			RTIAMB->joinFederationExecution(yourName,
						        executionName,
						        federateAmbassadorPtr);
                plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[1] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[1]=cstr2mstr("caught unknown exception, probably no ambassadors, try rtiOn");
	}

	plhs[0] = federateHdl2mscalar(federateHandle);
	free_cstr(yourName);
	free_cstr(executionName);

	return;
}


static void 
m2resignFedExec(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTI::ResignAction	theAction;	// supplied C1


	theAction = mstr2resignAction(prhs[0]);

	try {
		RTIAMB->resignFederationExecution(theAction);
		plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception, probably no ambassadors, try rtiOn");
	}

	return;
}

static void				
m2registerFedSyncPoint(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	RTI::FederateHandleSet	*fedHdlSet;		// supplied C4
	char					*theTag;		// supplied C4
	char					*label;			// supplied C4

	label		=			mstr2cstr				(prhs[0]);
	theTag		=			mstr2cstr				(prhs[1]);							
	
	
	if (nrhs > 2) {
		try {
			fedHdlSet		=	mvec2federateHdlSet		(prhs[2]);
			RTIAMB->registerFederationSynchronizationPoint(label,theTag,*fedHdlSet);
			plhs[0]=cstr2mstr("");
		}
		catch (RTI::Exception &e) {
			plhs[0]=cstr2mstr(e._name);
		}
		catch (...) {
			plhs[0]=cstr2mstr("caught unknown exception");
		}
		free(fedHdlSet);
	}
	else {
		try {
			RTIAMB->registerFederationSynchronizationPoint(label,theTag);
			plhs[0]=cstr2mstr("");
		}
		catch (RTI::Exception &e) {
			plhs[0]=cstr2mstr(e._name);
		}
		catch (...) {
			plhs[0]=cstr2mstr("caught unknown exception");
		}
	}
	
	free_cstr(theTag);
	free_cstr(label);

}

static void				
m2syncPointAchieved(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	char		*label;		// supplied C4
	
	label=		mstr2cstr	(prhs[0]);

	try {
		RTIAMB->synchronizationPointAchieved(label);
		plhs[0]=cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[0]=cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	free_cstr(label);

}

static void			
m2requestFedSave(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	char		*label;			// supplied C4
	RTIfedTime	*theTime;		// supplied C4
	
	label = mstr2cstr		(prhs[0]);
	switch (nrhs) {
	
		case 1: try {
						RTIAMB->requestFederationSave(label);
                		plhs[0] = cstr2mstr("");
					}
				catch (RTI::Exception &e) {
					plhs[0] = cstr2mstr(e._name);
				}
				catch (...) {
					plhs[0]=cstr2mstr("caught unknown exception");
				}
				break;

		case 2:	theTime = mscalar2fedTime(prhs[1]);
				try {
						RTIAMB->requestFederationSave(label,*theTime);
                		plhs[0] = cstr2mstr("");
					}
				catch (RTI::Exception &e) {
					plhs[0] = cstr2mstr(e._name);
				}
				catch (...) {
					plhs[0]=cstr2mstr("caught unknown exception");
				}
				free_fedTime(theTime);
				break;
	}
	free_cstr(label);
	return;
}

static void			
m2fedSaveBegun(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	try {
			RTIAMB->federateSaveBegun();
			plhs[0] = cstr2mstr("");
		}
	catch (RTI::Exception &e) {
			plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
			plhs[0]=cstr2mstr("caught unknown exception");
	}
	return;	
}

static void				
m2fedSaveComplete(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	try {
			RTIAMB->federateSaveComplete();
			plhs[0] = cstr2mstr("");
		}
	catch (RTI::Exception &e) {
			plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
			plhs[0]=cstr2mstr("caught unknown exception");
	}
	return;

}

static void				
m2fedSaveNotComplete(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	try {
			RTIAMB->federateSaveNotComplete();
			plhs[0] = cstr2mstr("");
		}
	catch (RTI::Exception &e) {
			plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
			plhs[0]=cstr2mstr("caught unknown exception");
	}
	return;

}

static void			
m2requestFedRestore(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	char		*label;		// supplied C4
	
	label=		mstr2cstr	(prhs[0]);

	try {
		RTIAMB->requestFederationRestore(label);
		plhs[0]=cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[0]=cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	free_cstr(label);

}

static void			
m2fedRestoreComplete(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	try {
			RTIAMB->federateRestoreComplete();
			plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
			plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
			plhs[0]=cstr2mstr("caught unknown exception");
	}
	return;
}

static void			
m2fedRestoreNotComplete(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	try {
			RTIAMB->federateRestoreNotComplete();
			plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
			plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
			plhs[0]=cstr2mstr("caught unknown exception");
	}
	return;
}





/***********************************
 * Declaration Management Services *
 ***********************************/

static void 
m2publishObjClass(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTI::ObjectClassHandle	theClass;	// supplied C1
	RTI::AttributeHandleSet	*attrList;	// supplied C4


	theClass = mscalar2objClassHdl(prhs[0]);
	attrList =    mvec2attrHdlSet (prhs[1]);


 	try {
		RTIAMB->publishObjectClass(theClass, *attrList);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	free_attrHdlSet(attrList);

	return;
}

static void  
m2unpublishObjClass(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTI::ObjectClassHandle	theClass;	// supplied C1


	theClass = mscalar2objClassHdl(prhs[0]);


 	try {
		RTIAMB->unpublishObjectClass(theClass);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	return;
}

static void 
m2publishInteractClass(int nlhs, mxArray *plhs[], 
		       int nrhs, const mxArray *prhs[]) {

	RTI::InteractionClassHandle	theInteraction;	// supplied C1


	theInteraction = mscalar2interactClassHdl(prhs[0]);

 	try {
		RTIAMB->publishInteractionClass(theInteraction);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	return;
}

static void 
m2unpublishInteractClass(int nlhs, mxArray *plhs[], 
		       int nrhs, const mxArray *prhs[]) {

	RTI::InteractionClassHandle	theInteraction;	// supplied C1


	theInteraction = mscalar2interactClassHdl(prhs[0]);


 	try {
		RTIAMB->unpublishInteractionClass(theInteraction);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	return;
}

static void 
m2subscribeObjClassAttr(int nlhs, mxArray *plhs[], 
		        int nrhs, const mxArray *prhs[]) {

	RTI::ObjectClassHandle	theClass;	// supplied C1
	RTI::AttributeHandleSet	*attrList;	// supplied C4
	RTI::Boolean			active;		// supplied C1
						// (default: active = RTI_TRUE)


	theClass =       mscalar2objClassHdl(prhs[0]);
	attrList =       mvec2attrHdlSet (prhs[1]);
	if (nrhs > 2) {
		active = mscalar2bool       (prhs[2]);
	}

	try {
		if (nrhs > 2) {
			RTIAMB->subscribeObjectClassAttributes(theClass, 
							       *attrList,
							       active);
		}
		else {
			RTIAMB->subscribeObjectClassAttributes(theClass, 
							       *attrList);
		}
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	free_attrHdlSet(attrList);

	return;
}

static void 
m2unsubscribeObjClass(int nlhs, mxArray *plhs[], 
		        int nrhs, const mxArray *prhs[]) {

	RTI::ObjectClassHandle	theClass;	// supplied C1


	theClass =       mscalar2objClassHdl(prhs[0]);

 	try {
			RTIAMB->unsubscribeObjectClass(theClass);
	        plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	return;
}

static void 
m2subscribeInteractClass(int nlhs, mxArray *plhs[], 
		         int nrhs, const mxArray *prhs[]) {

	RTI::InteractionClassHandle	theClass;	// supplied C1
	RTI::Boolean			active;		// supplied C1
						// (default: active = RTI_TRUE)


	theClass =       mscalar2interactClassHdl(prhs[0]);
	if (nrhs > 1) {
		active = mscalar2bool            (prhs[1]);
	}


 	try {
		if (nrhs > 1) {
			RTIAMB->subscribeInteractionClass(theClass,active);
		}
		else {
			RTIAMB->subscribeInteractionClass(theClass);
		}
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	return;
}

static void 
m2unsubscribeInteractClass(int nlhs, mxArray *plhs[], 
		         int nrhs, const mxArray *prhs[]) {

	RTI::InteractionClassHandle	theClass;	// supplied C1


	theClass =       mscalar2interactClassHdl(prhs[0]);


 	try {

			RTIAMB->unsubscribeInteractionClass(theClass);
            plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	return;
}


/******************************
 * Object Management Services *
 ******************************/

static void 
m2registerObjInst(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTI::ObjectClassHandle	theClass;	// supplied C1
	char			*theObject;	// supplied C4

	RTI::ObjectHandle		objHdl;		// returned C3
						// two overloaded methods


	theClass =       mscalar2objClassHdl(prhs[0]);
	if (nrhs > 1) {
		theObject = mstr2cstr       (prhs[1]);
	}


 	try {
		if (nrhs > 1) {
			objHdl = RTIAMB->registerObjectInstance(theClass,
								theObject);
		}
		else {
			objHdl = RTIAMB->registerObjectInstance(theClass);
		}
                plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[1] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[1]=cstr2mstr("caught unknown exception");
	}

	plhs[0] = objHdl2mscalar(objHdl);

	return;
}


static void 
m2updateAttrVal(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTI::ObjectHandle			theObject;	// supplied C1
	RTI::AttributeHandleValuePairSet	*theAttr;	// supplied C4
	RTIfedTime			*theTime;	// supplied C4
	char				*theTag;	// supplied C4

	RTI::EventRetractionHandle		eventRetractHdl;// returned C3
						// two overloaded methods


	theObject =        mscalar2objHdl           (prhs[0]);
	theAttr   =          mcell2attrHdlValPairSet(prhs[1]);
	if (nrhs < 4) {
		theTag   =    mstr2cstr             (prhs[2]);
	}
	else {
		theTime  = mscalar2fedTime          (prhs[2]);
		theTag   =    mstr2cstr             (prhs[3]);
	}


	if (nrhs < 4) {
		try {
			RTIAMB->updateAttributeValues(theObject,
						      *theAttr,
						      theTag);
                	plhs[0] = cstr2mstr("");
		}
		catch (RTI::Exception &e) {
			plhs[0] = cstr2mstr(e._name);
		}
		catch (...) {
			plhs[0]=cstr2mstr("caught unknown exception");
		}
	}
	else {
		try {
			eventRetractHdl = RTIAMB->updateAttributeValues(
						      theObject,
						      *theAttr,
						      *theTime,
						      theTag);
                	plhs[1] = cstr2mstr("");
		}
		catch (RTI::Exception &e) {
			plhs[1] = cstr2mstr(e._name);
		}
		catch (...) {
			plhs[1]=cstr2mstr("caught unknown exception");
		}
		plhs[0] = eventRetractHdl2mvec(eventRetractHdl);

		free_fedTime(theTime);
	}

	free_attrHdlValPairSet(theAttr);
	free_cstr(theTag);

	return;
}


static void 
m2sendInteract(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTI::InteractionClassHandle		theInteraction;	// supplied C1
	RTI::ParameterHandleValuePairSet	*theParameters;	// supplied C4
	RTIfedTime			*theTime;	// supplied C4
	char				*theTag;	// supplied C4

	RTI::EventRetractionHandle		eventRetractHdl;// returned C3
						// two overloaded methods


	theInteraction =   mscalar2interactClassHdl  (prhs[0]);
	theParameters  =     mcell2paramHdlValPairSet(prhs[1]);
	if (nrhs < 4) {
		theTag   =    mstr2cstr		     (prhs[2]);
	}
	else {
		theTime  = mscalar2fedTime	     (prhs[2]);
		theTag   =    mstr2cstr              (prhs[3]);
	}


	if (nrhs < 4) {
		try {
			RTIAMB->sendInteraction(theInteraction,
						 *theParameters,
						 theTag);
                	plhs[0] = cstr2mstr("");
		}
		catch (RTI::Exception &e) {
			plhs[0] = cstr2mstr(e._name);
		}
		catch (...) {
			plhs[0]=cstr2mstr("caught unknown exception");
		}

	}
	else {
		try {
			eventRetractHdl = RTIAMB->sendInteraction(
						      theInteraction,
						      *theParameters,
						      *theTime,
						      theTag);
                	plhs[1] = cstr2mstr("");
		}
		catch (RTI::Exception &e) {
			plhs[1] = cstr2mstr(e._name);
		}
		catch (...) {
			plhs[1]=cstr2mstr("caught unknown exception");
		}

		plhs[0] = eventRetractHdl2mvec(eventRetractHdl);

		free_fedTime(theTime);
	}

	free_paramHdlValPairSet(theParameters);
	free_cstr(theTag);

	return;
}


static void 
m2deleteObjInst(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTI::ObjectHandle		theObject;	// supplied C1
	RTIfedTime		*theTime;	// supplied C4
	char			*theTag;	// supplied C4

	RTI::EventRetractionHandle	eventRetractHdl;// returned C3
						// two overloaded methods


	theObject =        mscalar2objHdl (prhs[0]);
	if (nrhs < 3) {
		theTag   =    mstr2cstr   (prhs[1]);
	}
	else {
		theTime  = mscalar2fedTime(prhs[1]);
		theTag   =    mstr2cstr   (prhs[2]);
	}
	

	if (nrhs < 3) {
		try {
			RTIAMB->deleteObjectInstance(theObject,theTag);
                	plhs[0] = cstr2mstr("");
		}
		catch (RTI::Exception &e) {
			plhs[0] = cstr2mstr(e._name);
		}
		catch (...) {
			plhs[0]=cstr2mstr("caught unknown exception");
		}

	}
	else {
		try {
			eventRetractHdl = RTIAMB->deleteObjectInstance(
						     theObject,*theTime,theTag);
                	plhs[1] = cstr2mstr("");
		}
		catch (RTI::Exception &e) {
			plhs[1] = cstr2mstr(e._name);
		}
		catch (...) {
			plhs[1]=cstr2mstr("caught unknown exception");
		}

		plhs[0] = eventRetractHdl2mvec(eventRetractHdl);

		free_fedTime(theTime);
	}

	free_cstr(theTag);

	return;
}

static void  
m2changeAttrTransType(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	RTI::ObjectHandle		theObject;			// supplied C1
	RTI::AttributeHandleSet *theAttributes;		// supplied C4
	RTI::TransportationHandle theType;			// supplied C1

	theObject		=		mscalar2objHdl		(prhs[0]);
	theAttributes	=		mvec2attrHdlSet		(prhs[1]);
	theType			=		mscalar2TransHdl	(prhs[2]);


	try {
		RTIAMB->changeAttributeTransportationType(theObject,*theAttributes,theType);
		plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	free_attrHdlSet(theAttributes);
}

static void  
m2changeInteractTransType(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	RTI::InteractionClassHandle		theClass;	// supplied C1
	RTI::TransportationHandle theType;			// supplied C1

	theClass		=		mscalar2interactClassHdl	(prhs[0]);
	theType			=		mscalar2TransHdl			(prhs[1]);


	try {
		RTIAMB->changeInteractionTransportationType(theClass,theType);
		plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}
}

static void 
m2localDelObjInst(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	RTI::ObjectHandle    theObject;       // supplied C1
	
	theObject		=		mscalar2objHdl		(prhs[0]);

	try {
		RTIAMB->localDeleteObjectInstance(theObject);
		plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}
}

static void 
m2requestClassAttrValUp(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	RTI::ObjectClassHandle theClass;			// supplied C1
	RTI::AttributeHandleSet *theAttributes;		// supplied C4

	theClass		=		mscalar2objClassHdl	(prhs[0]);
	theAttributes	=		mvec2attrHdlSet		(prhs[1]);

	try {
		RTIAMB->requestClassAttributeValueUpdate(theClass, *theAttributes);
		plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	free_attrHdlSet(theAttributes);
}

static void 
m2requestObjAttrValUp(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	RTI::ObjectHandle theObject;				// supplied C1
	RTI::AttributeHandleSet *theAttributes;		// supplied C4

	theObject		=		mscalar2objHdl		(prhs[0]);
	theAttributes	=		mvec2attrHdlSet		(prhs[1]);

	try {
		RTIAMB->requestObjectAttributeValueUpdate(theObject, *theAttributes);
		plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	free_attrHdlSet(theAttributes);
}

/****************************
 * Time Management Services *
 ****************************/

static void 
m2enableTimeReg(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime		*theFederateTime;	// supplied C4
	RTIfedTime		*theLookahead;		// supplied C4


	theFederateTime = mscalar2fedTime(prhs[0]);
	theLookahead    = mscalar2fedTime(prhs[1]);
	
	try {
		RTIAMB->enableTimeRegulation(*theFederateTime,*theLookahead);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	free_fedTime(theFederateTime);
	free_fedTime(theLookahead);

	return;
}


static void 
m2enableTimeConstr(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

 	try {
		RTIAMB->enableTimeConstrained();
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	return;
}


static void 
m2timeAdvReq(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime		*theTime;	// supplied C4


	theTime = mscalar2fedTime(prhs[0]);


 	try {
		RTIAMB->timeAdvanceRequest(*theTime);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	free_fedTime(theTime);

	return;
}



static void 
m2changeAttrOrderType(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	
	RTI::ObjectHandle theObject;				// supplied C1
	RTI::AttributeHandleSet	*theAttributes;		// supplied C4
	RTI::OrderingHandle	theType;				// supplied C1

	
	theObject		=		mscalar2objHdl		(prhs[0]);
	theAttributes	=		mvec2attrHdlSet		(prhs[1]);
	theType			=		mscalar2orderHdl	(prhs[2]);
	
 	try {
		RTIAMB->changeAttributeOrderType(theObject, *theAttributes, theType);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	free_attrHdlSet(theAttributes);
}

static void 
m2changeInteractOrderType(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	
	RTI::InteractionClassHandle		theClass;		// supplied C1
	RTI::OrderingHandle	theType;					// supplied C1

	
	theClass		=		mscalar2interactClassHdl	(prhs[0]);
	theType			=		mscalar2orderHdl			(prhs[1]);
	
 	try {
		RTIAMB->changeInteractionOrderType(theClass, theType);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

}

static void 
m2disableAsyncDeliv(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	
	
 	try {
		RTIAMB->disableAsynchronousDelivery();
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

}

static void 
m2disableTimeConstr(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	
	
 	try {
		RTIAMB->disableTimeConstrained();
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

}

static void 
m2disableTimeReg(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

 	try {
		RTIAMB->disableTimeRegulation();
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

}

static void  
m2flushQueueReq(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime		*theTime;	// supplied C4


	theTime = mscalar2fedTime(prhs[0]);
	

 	try {
		RTIAMB->flushQueueRequest(*theTime);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	free_fedTime(theTime);

	return;
}

static void 
m2modifyLookahead(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime		*theLookahead;		// supplied C4


	theLookahead = mscalar2fedTime(prhs[0]);
	

 	try {
		RTIAMB->modifyLookahead(*theLookahead);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	free_fedTime(theLookahead);

	return;
}

static void 
m2nextEventReq(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime		*theTime;	// supplied C4


	theTime = mscalar2fedTime(prhs[0]);
	

 	try {
		RTIAMB->nextEventRequest(*theTime);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	free_fedTime(theTime);

	return;
}

static void 
m2nextEventReqAvail(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime		*theTime;	// supplied C4


	theTime = mscalar2fedTime(prhs[0]);
	

 	try {
		RTIAMB->nextEventRequestAvailable(*theTime);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	free_fedTime(theTime);

	return;
}

static void 
m2queryFedTime(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime		theTime;	// supplied C4

 	try {
		RTIAMB->queryFederateTime(theTime);
		plhs[0] = fedTime2mscalar(theTime);
                plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	return;
}

static void 
m2queryLBTS(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime		theTime;	// supplied C4

 	try {
		RTIAMB->queryLBTS(theTime);
		plhs[0] = fedTime2mscalar(theTime);
                plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	return;
}

static void 
m2queryLookahead(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime			theTime;	// supplied C4

 	try {
		RTIAMB->queryLookahead(theTime);
		plhs[0] = fedTime2mscalar(theTime);
                plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	return;
}

static void 
m2queryMinNextEventTime(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime		theTime;	// supplied C4


 	try {
		RTIAMB->queryMinNextEventTime(theTime);
		plhs[0] = fedTime2mscalar(theTime);
                plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	return;
}

static void 
m2retract(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTI::EventRetractionHandle		*theHandle;	// supplied C1

	theHandle = mvec2eventRetractHdl(prhs[0]);
	
 	try {
		RTIAMB->retract(*theHandle);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}

	free_eventRetHdl(theHandle);

	return;
}


static void 
m2timeAdvReqAvail(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTIfedTime		*theTime;	// supplied C4


	theTime = mscalar2fedTime(prhs[0]);
	

 	try {
		RTIAMB->timeAdvanceRequestAvailable(*theTime);
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	free_fedTime(theTime);

	return;
}


/**********************
 * Ancillary Services *
 **********************/

static void 
m2getObjClassHdl(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	char			*theName;	// supplied C4

	RTI::ObjectClassHandle	objClassHdl;	// returned C3


	theName = mstr2cstr(prhs[0]);
	
 	try {
		objClassHdl = RTIAMB->getObjectClassHandle(theName);
                plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[1] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[1]=cstr2mstr("caught unknown exception");
	}


	plhs[0] = objClassHdl2mscalar(objClassHdl);

	free_cstr(theName);

	return;
}


static void 
m2getAttrHdl(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	char			*theName;	// supplied C4
	RTI::ObjectClassHandle	whichClass;	// supplied C1

	RTI::AttributeHandle		attrHdl;	// returned C3


	theName    =    mstr2cstr       (prhs[0]);
	whichClass = mscalar2objClassHdl(prhs[1]);

 	try {
		attrHdl = RTIAMB->getAttributeHandle(theName, whichClass);
                plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[1] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[1]=cstr2mstr("caught unknown exception");
	}


	plhs[0] = attrHdl2mscalar(attrHdl);

	free_cstr(theName);

	return;
}



static void 
m2getInteractClassHdl(int nlhs, mxArray *plhs[], 
		      int nrhs, const mxArray *prhs[]) {

	char			*theName;		// supplied C4

	RTI::InteractionClassHandle	interactClassHdl;	// returned C3


	theName = mstr2cstr(prhs[0]);
	

	try {
		interactClassHdl = RTIAMB->getInteractionClassHandle(theName);
		plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[1] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[1]=cstr2mstr("caught unknown exception");
	}


	plhs[0] = interactClassHdl2mscalar(interactClassHdl);

	free_cstr(theName);

	return;
}


static void 
m2getParamHdl(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	char			*theName;	// supplied C4
	RTI::InteractionClassHandle	whichClass;	// supplied C1

	RTI::ParameterHandle		paramHdl;	// returned C3


	theName    =    mstr2cstr            (prhs[0]);
	whichClass = mscalar2interactClassHdl(prhs[1]);


	try {
		paramHdl = RTIAMB->getParameterHandle(theName, whichClass);
		plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[1] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[1]=cstr2mstr("caught unknown exception");
	}


	plhs[0] = paramHdl2mscalar(paramHdl);

	free_cstr(theName);

	return;
}

static void 
m2getTransHdl(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	char			*theName;	// supplied C4
	RTI::TransportationHandle	transHdl;	// returned 

	theName    =    mstr2cstr            (prhs[0]);
	
	try {
		transHdl = RTIAMB->getTransportationHandle(theName);
		plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[1] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[1]=cstr2mstr("caught unknown exception");
	}


	plhs[0] = transHdl2mscalar(transHdl);

	free_cstr(theName);

	return;
}

static void 
m2getOrderingHdl(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	char				*theName;	// supplied C4
	RTI::OrderingHandle	orderHdl;	// returned 

	theName    =    mstr2cstr            (prhs[0]);
	
	try {
		orderHdl = RTIAMB->getOrderingHandle(theName);
		plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
		plhs[1] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[1]=cstr2mstr("caught unknown exception");
	}


	plhs[0] = orderHdl2mscalar(orderHdl);

	free_cstr(theName);

	return;
}

static void 
m2enableAsyncDeliv(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

 	try {
		RTIAMB->enableAsynchronousDelivery();
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	return;
}


static void 
m2enableAttrRelAdvisSwitch(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[]) {

 	try {
		RTIAMB->enableAttributeRelevanceAdvisorySwitch();
                plhs[0] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[0] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[0]=cstr2mstr("caught unknown exception");
	}


	return;
}


static void 
m2tick(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	RTI::TickTime	minimum;	// supplied C1
	RTI::TickTime	maximum;	// supplied C1

	RTI::Boolean 	boolVal;	// returned C3
					// two overloaded methods


	if (nrhs > 0) {
		minimum = mscalar2tickTime(prhs[0]);
		maximum = mscalar2tickTime(prhs[1]);
	}


 	try {
		if (nrhs > 0) {
			boolVal = RTIAMB->tick(minimum, maximum);
		}
		else {
			boolVal = RTIAMB->tick();
		}
                plhs[1] = cstr2mstr("");
	}
	catch (RTI::Exception &e) {
                plhs[1] = cstr2mstr(e._name);
	}
	catch (...) {
		plhs[1]=cstr2mstr("caught unknown exception");
	}


	plhs[0] = bool2mscalar(boolVal);

	return;
}







/****************************
 **                        **
 ** (B) FederateAmbassador **
 **                        **
 ****************************/

/* Class MATLABAmbassador - The General Matlab Federate Ambassador *
 * A derived class of the abstract FederateAmbassador class.       */

class MATLABambassador : public RTI::FederateAmbassador
{
public:
 

MATLABambassador()
{
};

 
virtual ~MATLABambassador()
throw(RTI::FederateInternalError)
{
};


////////////////////////////////////
// Federation Management Services //
////////////////////////////////////


virtual void synchronizationPointRegistrationSucceeded (
  const char *label) // supplied C4)
throw (
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = cstr2mstr(label);

	mexCallMATLAB(0, NULL, 1, prhs, "syncPointRegSucceeded");

	mxDestroyArray(prhs[0]);

	return;
};



virtual void synchronizationPointRegistrationFailed (
  const char *label) // supplied C4)
throw (
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = cstr2mstr(label);

	mexCallMATLAB(0, NULL, 1, prhs, "syncPointRegFailed");

	mxDestroyArray(prhs[0]);

	return;
};

virtual void announceSynchronizationPoint (
  const char *label, // supplied C4
  const char *tag)   // supplied C4
throw (
  RTI::FederateInternalError)
{
	mxArray	*prhs[2];

	prhs[0] = cstr2mstr(label);
	prhs[1] = cstr2mstr(tag);

	mexCallMATLAB(0, NULL, 2, prhs, "announceSyncPoint");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);

	return;
};



virtual void federationSynchronized (
  const char *label) // supplied C4)
throw (
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = cstr2mstr(label);

	mexCallMATLAB(0, NULL, 1, prhs, "fedSync");

	mxDestroyArray(prhs[0]);

	return;
};


virtual void initiateFederateSave (
  const char *label) // supplied C4
throw (
  RTI::UnableToPerformSave,
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = cstr2mstr(label);

	mexCallMATLAB(0, NULL, 1, prhs, "initiateFedSave");

	mxDestroyArray(prhs[0]);

	return;
};

virtual void federationSaved ()
throw (
  RTI::FederateInternalError)
{
	mexCallMATLAB(0, NULL, 0, NULL, "fedSaved");

	return;
};


virtual void federationNotSaved ()
throw (
  RTI::FederateInternalError)
{
	mexCallMATLAB(0, NULL, 0, NULL, "fedNotSaved");

	return;
};


virtual void requestFederationRestoreSucceeded (
  const char *label) // supplied C4
throw (
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = cstr2mstr(label);

	mexCallMATLAB(0, NULL, 1, prhs, "requestFedResSucceeded");

	mxDestroyArray(prhs[0]);

	return;
};


virtual void requestFederationRestoreFailed (
  const char *label,
  const char *reason) // supplied C4
throw (
  RTI::FederateInternalError)
{
	mxArray	*prhs[2];

	prhs[0] = cstr2mstr(label);
	prhs[1] = cstr2mstr(reason);

	mexCallMATLAB(0, NULL, 2, prhs, "requestFedResFailed");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);

	return;
};


virtual void federationRestoreBegun ()
throw (
  RTI::FederateInternalError)
{
	mexCallMATLAB(0, NULL, 0, NULL, "fedResBegun");

	return;
};


virtual void initiateFederateRestore (
  const char               *label,   // supplied C4
        RTI::FederateHandle handle)  // supplied C1
throw (
  RTI::SpecifiedSaveLabelDoesNotExist,
  RTI::CouldNotRestore,
  RTI::FederateInternalError)
{
	mxArray	*prhs[2];

	prhs[0] = cstr2mstr(label);
	prhs[1] = federateHdl2mscalar(handle);

	mexCallMATLAB(0, NULL, 2, prhs, "initiateFedRestore");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);

	return;
};

virtual void federationRestored ()
throw (
  RTI::FederateInternalError)
{
	mexCallMATLAB(0, NULL, 0, NULL, "fedRestored");

	return;
};


virtual void federationNotRestored ()
throw (
  RTI::FederateInternalError)
{
	mexCallMATLAB(0, NULL, 0, NULL, "fedNotRestored");

	return;
};


/////////////////////////////////////
// Declaration Management Services //
/////////////////////////////////////

virtual void startRegistrationForObjectClass (
        RTI::ObjectClassHandle   theClass)      // supplied C1
throw (
  RTI::ObjectClassNotPublished,
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = objClassHdl2mscalar(theClass);

	mexCallMATLAB(0, NULL, 1, prhs, "startRegForObjClass");

	mxDestroyArray(prhs[0]);

	return;
};


virtual void stopRegistrationForObjectClass (
        RTI::ObjectClassHandle   theClass)      // supplied C1
throw (
  RTI::ObjectClassNotPublished,
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = objClassHdl2mscalar(theClass);

	mexCallMATLAB(0, NULL, 1, prhs, "stopRegForObjClass");

	mxDestroyArray(prhs[0]);

	return;
};


virtual void turnInteractionsOn (
  RTI::InteractionClassHandle theHandle) // supplied C1
throw (
  RTI::InteractionClassNotPublished,
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = interactClassHdl2mscalar(theHandle);

	mexCallMATLAB(0, NULL, 1, prhs, "turnInteractOn");

	mxDestroyArray(prhs[0]);

	return;
};


virtual void turnInteractionsOff (
  RTI::InteractionClassHandle theHandle) // supplied C1
throw (
  RTI::InteractionClassNotPublished,
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = interactClassHdl2mscalar(theHandle);

	mexCallMATLAB(0, NULL, 1, prhs, "turnInteractOff");

	mxDestroyArray(prhs[0]);

	return;
};


////////////////////////////////
// Object Management Services //
////////////////////////////////

virtual void discoverObjectInstance (
  RTI::ObjectHandle          theObject,      // supplied C1
  RTI::ObjectClassHandle     theObjectClass, // supplied C1
  const char		    *theObjectName)  // supplied C4
throw (
  RTI::CouldNotDiscover,
  RTI::ObjectClassNotKnown,
  RTI::FederateInternalError)
{
	mxArray	*prhs[3];

	prhs[0] =      objHdl2mscalar(theObject);
	prhs[1] = objClassHdl2mscalar(theObjectClass);
	prhs[2] =        cstr2mstr   (theObjectName);

	mexCallMATLAB(0, NULL, 3, prhs, "discoverObjInst");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);
	mxDestroyArray(prhs[2]);

	return;
};


virtual void reflectAttributeValues (
        RTI::ObjectHandle                 theObject,     // supplied C1
  const RTI::AttributeHandleValuePairSet &theAttributes, // supplied C4
  const RTI::FedTime			 &theTime,       // supplied C1
  const char                             *theTag,        // supplied C4
        RTI::EventRetractionHandle        theHandle)     // supplied C1
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::FederateOwnsAttributes,
  RTI::InvalidFederationTime,
  RTI::FederateInternalError)
{
	mxArray	*prhs[5];

	prhs[0] =            objHdl2mscalar(theObject);
	prhs[1] = attrHdlValPairSet2mcell  (theAttributes);
	prhs[2] =           fedTime2mscalar(theTime);
	prhs[3] =              cstr2mstr   (theTag);
	prhs[4] =   eventRetractHdl2mvec   (theHandle);

	mexCallMATLAB(0, NULL, 5, prhs, "reflectAttrVal");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);
	mxDestroyArray(prhs[2]);
	mxDestroyArray(prhs[3]);
	mxDestroyArray(prhs[4]);

	return;
};


virtual void reflectAttributeValues (
        RTI::ObjectHandle                 theObject,     // supplied C1
  const RTI::AttributeHandleValuePairSet &theAttributes, // supplied C4
  const char                             *theTag)        // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::FederateOwnsAttributes,
  RTI::FederateInternalError)
{
	mxArray	*prhs[3];

	prhs[0] =            objHdl2mscalar(theObject);
	prhs[1] = attrHdlValPairSet2mcell  (theAttributes);
	prhs[2] =              cstr2mstr   (theTag);

	mexCallMATLAB(0, NULL, 3, prhs, "reflectAttrVal");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);
	mxDestroyArray(prhs[2]);

	return;
};


virtual void receiveInteraction (
        RTI::InteractionClassHandle       theInteraction, // supplied C1
  const RTI::ParameterHandleValuePairSet &theParameters,  // supplied C4
  const RTI::FedTime			 &theTime,        // supplied C4
  const char                             *theTag,         // supplied C4
        RTI::EventRetractionHandle        theHandle)      // supplied C1
throw (
  RTI::InteractionClassNotKnown,
  RTI::InteractionParameterNotKnown,
  RTI::InvalidFederationTime,
  RTI::FederateInternalError)
{
	mxArray	*prhs[5];

	prhs[0] =   interactClassHdl2mscalar(theInteraction);
	prhs[1] = paramHdlValPairSet2mcell  (theParameters);
	prhs[2] =            fedTime2mscalar(theTime);
	prhs[3] =               cstr2mstr   (theTag);
	prhs[4] =    eventRetractHdl2mvec   (theHandle);

	mexCallMATLAB(0, NULL, 5, prhs, "receiveInteract");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);
	mxDestroyArray(prhs[2]);
	mxDestroyArray(prhs[3]);
	mxDestroyArray(prhs[4]);

	return;
};


virtual void receiveInteraction (
        RTI::InteractionClassHandle       theInteraction, // supplied C1
  const RTI::ParameterHandleValuePairSet &theParameters,  // supplied C4
  const char                             *theTag)         // supplied C4
throw (
  RTI::InteractionClassNotKnown,
  RTI::InteractionParameterNotKnown,
  RTI::FederateInternalError)
{
	mxArray	*prhs[3];

	prhs[0] =   interactClassHdl2mscalar(theInteraction);
	prhs[1] = paramHdlValPairSet2mcell  (theParameters);
	prhs[2] =               cstr2mstr   (theTag);

	mexCallMATLAB(0, NULL, 3, prhs, "receiveInteract");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);
	mxDestroyArray(prhs[2]);

	return;
};


virtual void removeObjectInstance (
        RTI::ObjectHandle          theObject, // supplied C1
  const RTI::FedTime		  &theTime,   // supplied C4
  const char                      *theTag,    // supplied C4
        RTI::EventRetractionHandle theHandle) // supplied C1
throw (
  RTI::ObjectNotKnown,
  RTI::InvalidFederationTime,
  RTI::FederateInternalError)
{
	mxArray	*prhs[4];

	prhs[0] =          objHdl2mscalar(theObject);
	prhs[1] =         fedTime2mscalar(theTime);
	prhs[2] =            cstr2mstr   (theTag);
	prhs[3] = eventRetractHdl2mvec   (theHandle);

	mexCallMATLAB(0, NULL, 4, prhs, "removeObjInst");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);
	mxDestroyArray(prhs[2]);
	mxDestroyArray(prhs[3]);

	return;
};


virtual void removeObjectInstance (
        RTI::ObjectHandle          theObject, // supplied C1
  const char                      *theTag)    // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::FederateInternalError)
{
	mxArray	*prhs[2];

	prhs[0] = objHdl2mscalar(theObject);
	prhs[1] =   cstr2mstr   (theTag);

	mexCallMATLAB(0, NULL, 2, prhs, "removeObjInst");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);

	return;
};


virtual void attributesInScope (
        RTI::ObjectHandle        theObject,     // supplied C1
  const RTI::AttributeHandleSet &theAttributes) // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::FederateInternalError)
{
	mxArray	*prhs[2];

	prhs[0] = objHdl2mscalar(theObject);
	prhs[1] = attrHdlSet2mvec (theAttributes);

	mexCallMATLAB(0, NULL, 2, prhs, "attrInScope");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);

	return;
};


virtual void attributesOutOfScope (
        RTI::ObjectHandle        theObject,     // supplied C1
  const RTI::AttributeHandleSet &theAttributes) // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::FederateInternalError)
{
	mxArray	*prhs[2];

	prhs[0] = objHdl2mscalar(theObject);
	prhs[1] = attrHdlSet2mvec (theAttributes);

	mexCallMATLAB(0, NULL, 2, prhs, "attrOutOfScope");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);

	return;
};


virtual void provideAttributeValueUpdate (
        RTI::ObjectHandle        theObject,     // supplied C1
  const RTI::AttributeHandleSet &theAttributes) // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeNotOwned,
  RTI::FederateInternalError)
{
	mxArray	*prhs[2];

	prhs[0] = objHdl2mscalar(theObject);
	prhs[1] = attrHdlSet2mvec (theAttributes);

	mexCallMATLAB(0, NULL, 2, prhs, "provideAttrValUpdate");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);

	return;
};


virtual void turnUpdatesOnForObjectInstance (
        RTI::ObjectHandle        theObject,     // supplied C1
  const RTI::AttributeHandleSet	&theAttributes) // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotOwned,
  RTI::FederateInternalError)
{
	mxArray	*prhs[2];

	prhs[0] =     objHdl2mscalar(theObject);
	prhs[1] = attrHdlSet2mvec   (theAttributes);

	mexCallMATLAB(0, NULL, 2, prhs, "turnUpdatesOnForObjInst");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);

	return;
};


virtual void turnUpdatesOffForObjectInstance (
        RTI::ObjectHandle        theObject,      // supplied C1
  const RTI::AttributeHandleSet &theAttributes) // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotOwned,
  RTI::FederateInternalError)
{
	mxArray	*prhs[2];

	prhs[0] =     objHdl2mscalar(theObject);
	prhs[1] = attrHdlSet2mvec   (theAttributes);

	mexCallMATLAB(0, NULL, 2, prhs, "turnUpdatesOffForObjInst");

	mxDestroyArray(prhs[0]);
	mxDestroyArray(prhs[1]);

	return;
};


///////////////////////////////////
// Ownership Management Services //
///////////////////////////////////

virtual void requestAttributeOwnershipAssumption (
        RTI::ObjectHandle        theObject,         // supplied C1
  const RTI::AttributeHandleSet &offeredAttributes, // supplied C4
  const char                    *theTag)            // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeAlreadyOwned,
  RTI::AttributeNotPublished,
  RTI::FederateInternalError)
{
printf(">>>MATLABambassador::requestAttributeOwnershipAssumption()\n");
   cerr << "not supported" << endl;
printf("<<<MATLABambassador::requestAttributeOwnershipAssumption()\n");
};


virtual void attributeOwnershipDivestitureNotification (
        RTI::ObjectHandle        theObject,          // supplied C1
  const RTI::AttributeHandleSet &releasedAttributes) // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeNotOwned,
  RTI::AttributeDivestitureWasNotRequested,
  RTI::FederateInternalError)
{
printf(">>>MATLABambassador::attributeOwnershipDivestitureNotification()\n");
   cerr << "not supported" << endl;
printf("<<<MATLABambassador::attributeOwnershipDivestitureNotification()\n");
};


virtual void attributeOwnershipAcquisitionNotification (
        RTI::ObjectHandle        theObject,         // supplied C1
  const RTI::AttributeHandleSet &securedAttributes) // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeAcquisitionWasNotRequested,
  RTI::AttributeAlreadyOwned,
  RTI::AttributeNotPublished,
  RTI::FederateInternalError)
{
printf(">>>MATLABambassador::attributeOwnershipAcquisitionNotification()\n");
   cerr << "not supported" << endl;
printf("<<<MATLABambassador::attributeOwnershipAcquisitionNotification()\n");
};


virtual void attributeOwnershipUnavailable (
        RTI::ObjectHandle        theObject,         // supplied C1
  const RTI::AttributeHandleSet &theAttributes) // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeAlreadyOwned,
  RTI::AttributeAcquisitionWasNotRequested,
  RTI::FederateInternalError)
{
printf(">>>MATLABambassador::attributeOwnershipUnavailable()\n");
   cerr << "not supported" << endl;
printf("<<<MATLABambassador::attributeOwnershipUnavailable()\n");
};


virtual void requestAttributeOwnershipRelease (
        RTI::ObjectHandle        theObject,           // supplied C1
  const RTI::AttributeHandleSet &candidateAttributes, // supplied C4
  const char                    *theTag)              // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeNotOwned,
  RTI::FederateInternalError)
{
printf(">>>MATLABambassador::requestAttributeOwnershipRelease()\n");
   cerr << "not supported" << endl;
printf("<<<MATLABambassador::requestAttributeOwnershipRelease()\n");
};


virtual void confirmAttributeOwnershipAcquisitionCancellation (
        RTI::ObjectHandle        theObject,         // supplied C1
  const RTI::AttributeHandleSet &theAttributes) // supplied C4
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::AttributeAlreadyOwned,
  RTI::AttributeAcquisitionWasNotCanceled,
  RTI::FederateInternalError)
{
printf(">>>MATLABambassador::confirmAttributeOwnershipAcquisitionCancellation()\n");
   cerr << "not supported" << endl;
printf("<<<MATLABambassador::confirmAttributeOwnershipAcquisitionCancellation()\n");
};


virtual void informAttributeOwnership (
  RTI::ObjectHandle    theObject,    // supplied C1
  RTI::AttributeHandle theAttribute, // supplied C1
  RTI::FederateHandle  theOwner)     // supplied C1
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::FederateInternalError)
{
printf(">>>MATLABambassador::informAttributeOwnership()\n");
   cerr << "not supported" << endl;
printf("<<<MATLABambassador::informAttributeOwnership()\n");
};


virtual void attributeIsNotOwned (
  RTI::ObjectHandle    theObject,    // supplied C1
  RTI::AttributeHandle theAttribute) // supplied C1
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::FederateInternalError)
{
printf(">>>MATLABambassador::attributeIsNotOwned()\n");
   cerr << "not supported" << endl;
printf("<<<MATLABambassador::attributeIsNotOwned()\n");
};


virtual void attributeOwnedByRTI (
  RTI::ObjectHandle    theObject,    // supplied C1
  RTI::AttributeHandle theAttribute) // supplied C1
throw (
  RTI::ObjectNotKnown,
  RTI::AttributeNotKnown,
  RTI::FederateInternalError)
{
printf(">>>MATLABambassador::attributeOwnedByRTI()\n");
   cerr << "not supported" << endl;
printf("<<<MATLABambassador::attributeOwnedByRTI()\n");
};


//////////////////////////////
// Time Management Services //
//////////////////////////////

virtual void timeRegulationEnabled (
 const  RTI::FedTime &theFederateTime) // supplied C4
throw (
  RTI::InvalidFederationTime,
  RTI::EnableTimeRegulationWasNotPending,
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = fedTime2mscalar(theFederateTime);

	mexCallMATLAB(0, NULL, 1, prhs, "timeRegEnabled");

	mxDestroyArray(prhs[0]);

	return;
};


void timeConstrainedEnabled (
  const RTI::FedTime &theFederateTime) // supplied C4
throw (
  RTI::InvalidFederationTime,
  RTI::EnableTimeConstrainedWasNotPending,
  RTI::FederateInternalError)
{
    mxArray *prhs[1];
 
	prhs[0] = fedTime2mscalar(theFederateTime);
 
	mexCallMATLAB(0, NULL, 1, prhs, "timeConstrEnabled");
 
    mxDestroyArray(prhs[0]);

    return;
};


virtual void timeAdvanceGrant (
  const RTI::FedTime &theTime) // supplied C4
throw (
  RTI::InvalidFederationTime,
  RTI::TimeAdvanceWasNotInProgress,
  RTI::FederateInternalError)
{
	mxArray	*prhs[1];

	prhs[0] = fedTime2mscalar(theTime);

	mexCallMATLAB(0, NULL, 1, prhs, "timeAdvGrant");

	mxDestroyArray(prhs[0]);

	return;
};


virtual void requestRetraction (
  RTI::EventRetractionHandle theHandle) // supplied C1
throw (
  RTI::EventNotKnown,
  RTI::FederateInternalError)
{
	mxArray *prhs[1];

	prhs[0] = eventRetractHdl2mvec(theHandle);

	mexCallMATLAB(0, NULL, 1, prhs, "requestRetract");

	mxDestroyArray(prhs[0]);

	return;
};


}; /* End of class MATLABambassador definition */



/**************************************
 **				     **
 ** (C) Classes and Supporting Types **
 **				     **
 **************************************/

/***********
 * FedTime *
 ***********/

static void 
getEpsilon(int nlhs, mxArray *plhs[], 
	   int nrhs, const mxArray *prhs[]) {

	RTIfedTime	*timeObjPtr;


	timeObjPtr = new RTIfedTime();

	if (!timeObjPtr) {
		mexErrMsgTxt("rti.mex:getEpsilon(): new RTIfedTime() failed.");
	}

	timeObjPtr->setEpsilon();

	plhs[0] = fedTime2mscalar(*timeObjPtr);

	delete timeObjPtr;

	return;
}


/*********************************************
 * Interface Initialization/Deinitialization *
 *********************************************/

static void
rtiAtExitFailed() {
	mexWarnMsgTxt("rtiAtExit(): MEX file rti is cleared from Matlab\
		       in an unexpected way.");
	return;
}

static void
rtiAtExitSuccess() {
	printf("Deinitialization successfull\n");
	return;
}

static void 
newRtiAmb() {

	if (RTIAMB) {
		mexWarnMsgTxt("RTI ambassador already created.");
		return;
	}
	RTIAMB = new RTI::RTIambassador;
	if (!RTIAMB) {
		mexErrMsgTxt("Can't create RTI ambassador, new failed.");
		return;
	}

	return;
}


static void 
newFedAmb() {

	if (FEDAMB) {
		mexWarnMsgTxt("FED ambassador already created.");
		return;
	}

	FEDAMB = new MATLABambassador;
	if (!FEDAMB) {
		mexErrMsgTxt("Can't create FED ambassador, new failed.");
		return;
	}

	return;
}


static void
rtiOn() {
	if (!mexIsLocked()) {
		mexAtExit(rtiAtExitFailed);
		mexLock();
		newRtiAmb();
		newFedAmb();
	}
	else {
		mexWarnMsgTxt("Matlab/RTI/Matlab interface already init.");
	}
	return;
}


static void 
deleteRtiAmb() {

	if (!RTIAMB) {
		mexWarnMsgTxt("RTI amabassador already deleted.");
		return;
	}
	delete RTIAMB;

	return;
}


static void 
deleteFedAmb() {

	if (!FEDAMB) {
		mexWarnMsgTxt("FED amabassador already deleted.");
		return;
	}
	delete FEDAMB;

	return;
}


static void
rtiOff() {
	if (mexIsLocked()) {
		deleteRtiAmb();
		deleteFedAmb();
		mexAtExit(rtiAtExitSuccess);
		mexUnlock();
	}
	else {
		mexWarnMsgTxt("Matlab/RTI/Matlab interface already deinit.");
	}
	return;
}


extern void _main();	/* from a MathWorks C++ example; don't know *
			 * the meaning 				    */

/*******************
 * Gateway Routine *
 *******************/

void 
mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	int	opcode;		/* operation code */

	/* read operation code and drop it from input arg list */
	opcode	= (int) mxGetScalar(prhs[0]);
	prhs++;
	nrhs--;


	/* call wrapper routine */
   	switch (opcode) {


	/* Interface Initialization/Deinitialization */
	case     1: rtiOn			(); 		       break;
	case     2: rtiOff			(); 		       break;

	/* Data Conversions */
	case    50: double2net			(nlhs,plhs,nrhs,prhs); break;
	case    51: net2double			(nlhs,plhs,nrhs,prhs); break;
	case    52: char2net			(nlhs,plhs,nrhs,prhs); break;
	case    53: net2char			(nlhs,plhs,nrhs,prhs); break;

	/*********************
	 * (A) RTIambassador *
	 *********************/

	/* Federation Management Services */
	case  4020: m2createFedExec(nlhs,plhs,nrhs,prhs); break;
	case  4030: m2destroyFedExec(nlhs,plhs,nrhs,prhs); break;
	case  4040: m2joinFedExec(nlhs,plhs,nrhs,prhs); break;
	case  4050: m2resignFedExec(nlhs,plhs,nrhs,prhs); break;
	case  4060: m2registerFedSyncPoint(nlhs,plhs,nrhs,prhs); break;  
	case  4090: m2syncPointAchieved(nlhs,plhs,nrhs,prhs); break;  	
	case  4110: m2requestFedSave(nlhs,plhs,nrhs,prhs); break; 
	case  4130: m2fedSaveBegun(nlhs,plhs,nrhs,prhs); break; 
	case  4140: m2fedSaveComplete(nlhs,plhs,nrhs,prhs); break;  
	case  4141: m2fedSaveNotComplete(nlhs,plhs,nrhs,prhs); break;  
	case  4160: m2requestFedRestore(nlhs,plhs,nrhs,prhs); break;  
	case  4200: m2fedRestoreComplete(nlhs,plhs,nrhs,prhs); break;  
	case  4201: m2fedRestoreNotComplete(nlhs,plhs,nrhs,prhs); break;  


	/* Declaration Management Services */
	case  5020: m2publishObjClass(nlhs,plhs,nrhs,prhs); break;
	case  5030: m2unpublishObjClass(nlhs,plhs,nrhs,prhs); break;
	case  5040: m2publishInteractClass(nlhs,plhs,nrhs,prhs); break;
	case  5050: m2unpublishInteractClass(nlhs,plhs,nrhs,prhs); break;
	case  5060: m2subscribeObjClassAttr(nlhs,plhs,nrhs,prhs); break;
	case  5070: m2unsubscribeObjClass(nlhs,plhs,nrhs,prhs); break;
	case  5080: m2subscribeInteractClass(nlhs,plhs,nrhs,prhs); break;
	case  5090: m2unsubscribeInteractClass(nlhs,plhs,nrhs,prhs); break;

	/* Object Management Services */
	case  6020: m2registerObjInst(nlhs,plhs,nrhs,prhs); break;
	case  6040: m2updateAttrVal(nlhs,plhs,nrhs,prhs); break;
	case  6060: m2sendInteract(nlhs,plhs,nrhs,prhs); break;
	case  6080: m2deleteObjInst(nlhs,plhs,nrhs,prhs); break;
	case  6100: m2localDelObjInst(nlhs,plhs,nrhs,prhs); break;
	case  6110: m2changeAttrTransType(nlhs,plhs,nrhs,prhs); break;
	case  6120: m2changeInteractTransType(nlhs,plhs,nrhs,prhs); break;
	case  6150: m2requestClassAttrValUp(nlhs,plhs,nrhs,prhs); break;
	case  6151: m2requestObjAttrValUp(nlhs,plhs,nrhs,prhs); break;

	/* Time Management Services */
	case  8020: m2enableTimeReg(nlhs,plhs,nrhs,prhs); break;
	case  8040: m2disableTimeReg(nlhs,plhs,nrhs,prhs); break;
	case  8050: m2enableTimeConstr(nlhs,plhs,nrhs,prhs); break;
	case  8070: m2disableTimeConstr(nlhs,plhs,nrhs,prhs); break;
	case  8080: m2timeAdvReq(nlhs,plhs,nrhs,prhs); break;
	case  8090: m2timeAdvReqAvail(nlhs,plhs,nrhs,prhs); break;
	case  8100: m2nextEventReq(nlhs,plhs,nrhs,prhs); break;
	case  8110: m2nextEventReqAvail(nlhs,plhs,nrhs,prhs); break;
	case  8120: m2flushQueueReq(nlhs,plhs,nrhs,prhs); break;
	case  8140: m2enableAsyncDeliv(nlhs,plhs,nrhs,prhs); break;
	case  8150: m2disableAsyncDeliv(nlhs,plhs,nrhs,prhs); break;
	case  8160: m2queryLBTS(nlhs,plhs,nrhs,prhs); break;
	case  8170: m2queryFedTime(nlhs,plhs,nrhs,prhs); break;
	case  8180: m2queryMinNextEventTime(nlhs,plhs,nrhs,prhs); break;
	case  8190: m2modifyLookahead(nlhs,plhs,nrhs,prhs); break;
	case  8200: m2queryLookahead(nlhs,plhs,nrhs,prhs); break;
	case  8210: m2retract(nlhs,plhs,nrhs,prhs); break;
	case  8230: m2changeAttrOrderType(nlhs,plhs,nrhs,prhs); break;
	case  8240: m2changeInteractOrderType(nlhs,plhs,nrhs,prhs); break;

	/* Ancillary Services */
	case 10020: m2getObjClassHdl(nlhs,plhs,nrhs,prhs); break;
	case 10040: m2getAttrHdl(nlhs,plhs,nrhs,prhs); break;
	case 10060: m2getInteractClassHdl(nlhs,plhs,nrhs,prhs); break;
	case 10080: m2getParamHdl(nlhs,plhs,nrhs,prhs); break;
	case 10190: m2getTransHdl(nlhs,plhs,nrhs,prhs); break;
	case 10210: m2getOrderingHdl(nlhs,plhs,nrhs,prhs); break;
	case 10250: m2enableAttrRelAdvisSwitch(nlhs,plhs,nrhs,prhs); break;
	case 10310: m2tick(nlhs,plhs,nrhs,prhs); break;
	

	/************************************
	 * (C) Classes and Supporting Types *
	 ************************************/

	/* FedTime */
	case 100110: getEpsilon(nlhs,plhs,nrhs,prhs); break;

	
	default: mexErrMsgTxt("Function not implemented on this "
			      "platform or unknown operation code.\n");
	};

	return;
}

