#include "03-PublishingTest.hh"
#include "ReturnCodes.hh"

#include <RTI/certiRTI1516.h>
#include <RTI/RTI1516fedTime.h>
#include <RTI/Enums.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

/***************************************
 * This file is part of the Unit testing done for the HLA1516
 * functionality in CERTI.
 *
 * This test requires that it's FOM file (with .xml extention)
 * is placed under $BUILD/share/federations/
 *
 * Before running this test, start the 'rtig' project.
 *
 * The test will pause during it's run to allow the tester to review
 * the test results. Press <Return> to continue when the test is paused.
*****************************************/

int main(int argc, char** argv)
{
	int retCode1 = 0;

	srand( time(0) );

	TestFedAmb *fedAmb = new TestFedAmb();

	retCode1 = fedAmb->execute(argc, argv);



	if ( retCode1 == certi::rti1516_test::RETURN_NO_ERRORS)
	{
		std::wcerr << L"Pass: Test completed" << std::endl;
	} else
	{
		std::wcerr << L"Fail: Test failed with error code: " << retCode1 << std::endl;

	}

	// Poor man's 'pause'
	std::wcerr << L"Press enter to stop connection to RTI." << std::endl;
	getchar();

	delete fedAmb;

	// Poor man's 'pause'
	getchar();

	return retCode1;
}

TestFedAmb::TestFedAmb() throw()
: rtiAmb(0)
, fedHandle()
, fedName()
, federationName(L"TestFederationName")
{
	int nameVal;

	nameVal = (rand() % 99) + 1;

	std::wstringstream stream;
	stream << L"TestFederate" << nameVal;

	fedName = stream.str();
	std::wcerr << L"Federate name:" << fedName << std::endl;

}

TestFedAmb::~TestFedAmb() throw()
{
	delete rtiAmb;
}

int TestFedAmb::execute(int argc, char** argv)
{
	std::wcerr << L"01T Test: Attempting to set up test environment (create and join federation)" << std::endl;
	std::auto_ptr< rti1516::RTIambassadorFactory > rtiAmbFact(new rti1516::RTIambassadorFactory());
	std::vector< std::wstring > constructorArgs;

	// valid handles (are initialized in setup phase)
	rti1516::ObjectClassHandle classHandle;
	rti1516::AttributeHandle attrHandle;
	rti1516::AttributeHandleSet classAttributes;
	rti1516::InteractionClassHandle msgHandle;

	// Invalid handles.
	rti1516::ObjectClassHandle xClassHandle;
	rti1516::AttributeHandle xAttrHandle;
	rti1516::AttributeHandleSet xClassAttributes;
	xClassAttributes.insert(xAttrHandle);
	rti1516::InteractionClassHandle xMsgHandle;

	std::wstring testId(L"00");

	testId = L"01";

	try {
		//////////////////////////////////////////////
		// Set up
		std::auto_ptr< rti1516::RTIambassador > rtiAmbP(rtiAmbFact->createRTIambassador(constructorArgs));
		rtiAmb = rtiAmbP.release();

		try {
			rtiAmb->createFederationExecution(federationName, L"Certi-Test-02.xml");
			// expected
		} catch (rti1516::FederationExecutionAlreadyExists &)
		{
			// expected
		} catch (rti1516::Exception &)
		{
			throw;
		}

		fedHandle = rtiAmb->joinFederationExecution(fedName, federationName, *this);

		classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
		classAttributes.insert(attrHandle);
		msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");

	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_CREATING_AMBASSADOR;
	}

	std::wcerr << testId << L"E Pass: Federation joined. Press enter to continue with test." << std::endl;
	getchar();

	testId = L"02";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to Query GALT" << std::endl;
		RTI1516fedTime fedTime(2.0);
		std::wcerr << testId << L"time before: " << fedTime.getFedTime() << std::endl;
		bool retVal = rtiAmb->queryGALT(fedTime); 
		std::wcerr << testId << L"time after: " << fedTime.getFedTime() << std::endl;
		////////////////////////////////////////////////
		if (retVal) {
			if (fedTime.getFedTime() == 2.0) {
				std::wcerr << testId << L"N Fail: time did not change" << std::endl;
				return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
			} else {
				std::wcerr << testId << L"E Pass: Could Query GALT" << std::endl;
			}
		} else {
			std::wcerr << testId << L"N Fail: Query GALT returned false" << std::endl;
			return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"03";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to Query LITS" << std::endl;
		RTI1516fedTime fedTime(2.0);
		std::wcerr << testId << L"time before: " << fedTime.getFedTime() << std::endl;
		bool retVal = rtiAmb->queryLITS(fedTime); 
		std::wcerr << testId << L"time after: " << fedTime.getFedTime() << std::endl;
		////////////////////////////////////////////////
		if (retVal) {
			if (fedTime.getFedTime() == 2.0) {
				std::wcerr << testId << L"N Fail: time did not change" << std::endl;
				return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
			} else {
				std::wcerr << testId << L"E Pass: Could Query LITS" << std::endl;
			}
		} else {
			std::wcerr << testId << L"N Fail: Query LITS returned false" << std::endl;
			return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
	std::wcerr << L"Test operations performed. Press enter to continue with shutdown." << std::endl;
	getchar();

	try {
		////////////////////////////////////////////////
		std::wcerr << L"Attempting to resign from Federation" << std::endl;
		rtiAmb->resignFederationExecution(rti1516::CANCEL_THEN_DELETE_THEN_DIVEST);
		std::wcerr << L"Resigned from Federation" << std::endl;
		std::wcerr << L"Attempting to destroy Federation" << std::endl;
		rtiAmb->destroyFederationExecution(L"TestFederationName");
		std::wcerr << L"Destroyed Federation" << std::endl;
		////////////////////////////////////////////////
		// expected
		std::wcerr << L"Pass" << std::endl;
	} catch (rti1516::FederatesCurrentlyJoined &)
	{
		// expected
		std::wcerr << L"Pass: Could not destroy federation while another federate was still connected. This is expected behaviour." << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_DESTROYING_FEDERATION;
	}

	return certi::rti1516_test::RETURN_NO_ERRORS;
}
