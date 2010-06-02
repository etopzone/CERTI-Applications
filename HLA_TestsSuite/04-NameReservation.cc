#include "04-NameReservation.hh"
#include "ReturnCodes.hh"

#include <RTI/RTI1516.h>
#include <RTI/Enums.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>

#include <RTI/RTI1516fedTime.h>
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

// TODO: Test does not test Name reservation yet.

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


void
TestFedAmb::objectInstanceNameReservationSucceeded(std::wstring const &
												   theObjectInstanceName)
												   throw (rti1516::UnknownName,
												   rti1516::FederateInternalError)
{
	std::wcerr << L"Name reservation for object '" << theObjectInstanceName << L"' succeeded." << std::endl;
}

void
TestFedAmb::objectInstanceNameReservationFailed(std::wstring const &
												theObjectInstanceName)
												throw (rti1516::UnknownName,
												rti1516::FederateInternalError)
{
	std::wcerr << L"Name reservation for object '" << theObjectInstanceName << L"' failed." << std::endl;
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
	rti1516::ObjectInstanceHandle objHandle;

	// Invalid handles.
	rti1516::ObjectClassHandle xClassHandle;
	rti1516::AttributeHandle xAttrHandle;
	rti1516::AttributeHandleSet xClassAttributes;
	xClassAttributes.insert(xAttrHandle);
	rti1516::InteractionClassHandle xMsgHandle;

	std::wstring testId(L"00");
	std::wstring testAction(L"Nothing?");

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

		rtiAmb->publishObjectClassAttributes(classHandle, classAttributes);


	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_CREATING_AMBASSADOR;
	}

	std::wcerr << testId << L"E Pass: Federation joined. Press enter to continue with test." << std::endl;
	getchar();

	// Name reservation
	testId = L"02";
	testAction = L"reserve invalid (empty) name";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		std::wstring invalidName(L"");
		////////////////////////////////////////////////
		rtiAmb->reserveObjectInstanceName(invalidName);
		rtiAmb->evokeCallback(0.1);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could " << testAction << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_INTERACTION;
	} catch (rti1516::IllegalName &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}

	testId = L"03";
	testAction = L"reserve invalid name (begins with 'HLA')";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		std::wstring invalidName(L"HLA_INVALID_NAME");
		////////////////////////////////////////////////
		rtiAmb->reserveObjectInstanceName(invalidName);
		rtiAmb->evokeCallback(0.1);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could " << testAction << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_INTERACTION;
	} catch (rti1516::IllegalName &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}

	testId = L"04";
	testAction = L"reserve valid name";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		std::wstring validName(L"testObject");
		////////////////////////////////////////////////
		rtiAmb->reserveObjectInstanceName(validName);
		rtiAmb->evokeCallback(0.1);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could " << testAction << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}

	testId = L"05";
	testAction = L"reserve invalid (duplicate) name";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		std::wstring invalidName(L"testObject");
		////////////////////////////////////////////////
		rtiAmb->reserveObjectInstanceName(invalidName);
		rtiAmb->evokeCallback(0.1);
		////////////////////////////////////////////////
		// expected
		std::wcerr << testId << L"E Pass:" << testAction << std::endl;
	} catch (rti1516::IllegalName &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}

	testId = L"06";
	testAction = L"create valid object instance with reserved name";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		std::wstring validName(L"testObject");
		////////////////////////////////////////////////
		objHandle = rtiAmb->registerObjectInstance(classHandle, validName);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass:" << testAction << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}

	testId = L"07";
	testAction = L"create invalid object instance with (duplicate) instantiated name";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		std::wstring validName(L"testObject");
		////////////////////////////////////////////////
		rtiAmb->registerObjectInstance(classHandle, validName);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could " << testAction << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_INTERACTION;
	} catch (rti1516::Exception &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	}
	
	testId = L"08";
	testAction = L"reserve invalid (instantiated) name";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		std::wstring invalidName(L"testObject");
		////////////////////////////////////////////////
		rtiAmb->reserveObjectInstanceName(invalidName);
		rtiAmb->evokeCallback(0.1);
		////////////////////////////////////////////////
		// expected
		std::wcerr << testId << L"E Pass:" << testAction << std::endl;
	} catch (rti1516::IllegalName &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}

	testId = L"09";
	testAction = L"delete valid object instance";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		////////////////////////////////////////////////
		const char *c_tag = "tag";
		rti1516::VariableLengthData tag(c_tag, strlen(c_tag));
		rtiAmb->deleteObjectInstance(objHandle, tag);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass:" << testAction << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}

	testId = L"10";
	testAction = L"reserve valid (released) name";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		std::wstring validName(L"testObject");
		////////////////////////////////////////////////
		rtiAmb->reserveObjectInstanceName(validName);
		rtiAmb->evokeCallback(0.1);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass:" << testAction << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}

	testId = L"11";
	testAction = L"create valid object instance with reserved (reused) name";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		std::wstring validName(L"testObject");
		////////////////////////////////////////////////
		objHandle = rtiAmb->registerObjectInstance(classHandle, validName);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass:" << testAction << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}


	std::wcerr << L"Test operations performed. Press enter to continue with processing callbacks." << std::endl;
	getchar();

	testId = L"12";
	testAction = L"process callbacks";
	std::wcerr << testId << L"T Test: Attempting to " << testAction << std::endl;
	try {
		////////////////////////////////////////////////
		std::auto_ptr<rti1516::LogicalTimeFactory> factory = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"certi");
		std::auto_ptr<rti1516::LogicalTime> logicalTime = factory->makeLogicalTime();

		RTI1516fedTime fedTime(1.0);
		
		//rtiAmb->timeAdvanceRequest(*logicalTime);
		rtiAmb->nextMessageRequestAvailable(fedTime);
		rtiAmb->evokeMultipleCallbacks(0.0, 1.0);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass:" << testAction << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}


	std::wcerr << L"Press enter to continue with shutdown." << std::endl;
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
	} catch (rti1516::FederatesCurrentlyJoined &)
	{
		// expected
		std::wcerr << L"Could not destroy federation while another federate was still connected. This is expected behaviour." << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"17N " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_DESTROYING_FEDERATION;
	}

	return certi::rti1516_test::RETURN_NO_ERRORS;
}
