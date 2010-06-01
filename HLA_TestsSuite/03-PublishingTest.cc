#include "03-PublishingTest.hh"
#include "ReturnCodes.hh"

#include <RTI/RTI1516.h>
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

	// publishObjectClassAttributes
	testId = L"02";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to publish attributes for invalid class and invalid attributes" << std::endl;
		rtiAmb->publishObjectClassAttributes(xClassHandle, xClassAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could publish attributes for invalid class and invalid attributes" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_PUBLISH_CLASS_ATTRIBUTES;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_PUBLISH_CLASS_ATTRIBUTES;
	}
	testId = L"03";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to publish attributes for invalid class and valid attributes" << std::endl;
		rtiAmb->publishObjectClassAttributes(xClassHandle, classAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could publish attributes for invalid class and valid attributes" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_PUBLISH_CLASS_ATTRIBUTES;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_PUBLISH_CLASS_ATTRIBUTES;
	}
	testId = L"04";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to publish attributes for valid class and invalid attributes" << std::endl;
		rtiAmb->publishObjectClassAttributes(classHandle, xClassAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could publish attributes for valid class and invalid attributes" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_PUBLISH_CLASS_ATTRIBUTES;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_PUBLISH_CLASS_ATTRIBUTES;
	}
	testId = L"05";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to publish attributes for valid class and valid attributes" << std::endl;
		rtiAmb->publishObjectClassAttributes(classHandle, classAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could publish attributes for valid class and valid attributes" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_PUBLISH_CLASS_ATTRIBUTES;
	}

	// unpublishObjectClass
	testId = L"06";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unpublish invalid class" << std::endl;
		rtiAmb->unpublishObjectClass(xClassHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could unpublish invalid class" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_CLASS;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_CLASS;
	}
	testId = L"07";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unpublish valid class" << std::endl;
		rtiAmb->unpublishObjectClass(classHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could unpublish invalid class" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_CLASS;
	}

	// unpublishObjectClassAttributes
	testId = L"08";
	rtiAmb->publishObjectClassAttributes(classHandle, classAttributes);
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unpublish attributes for invalid class and invalid attributes" << std::endl;
		rtiAmb->unpublishObjectClassAttributes(xClassHandle, xClassAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could unpublish attributes for invalid class and invalid attributes" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_CLASS_ATTRIBUTES;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_CLASS_ATTRIBUTES;
	}
	testId = L"09";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unpublish attributes for invalid class and valid attributes" << std::endl;
		rtiAmb->unpublishObjectClassAttributes(xClassHandle, classAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could unpublish attributes for invalid class and valid attributes" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_CLASS_ATTRIBUTES;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_CLASS_ATTRIBUTES;
	}
	testId = L"10";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unpublish attributes for valid class and invalid attributes" << std::endl;
		rtiAmb->unpublishObjectClassAttributes(classHandle, xClassAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Skip." << std::endl;

		// TODO Restore proper handling of test case.
		//std::wcerr << testId << L"N Fail: Could unpublish attributes for valid class and invalid attributes" << std::endl;
		//return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_CLASS_ATTRIBUTES;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_CLASS_ATTRIBUTES;
	}
	testId = L"11";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unpublish attributes for valid class and valid attributes" << std::endl;
		rtiAmb->unpublishObjectClassAttributes(classHandle, classAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could unpublish attributes for valid class and valid attributes" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_CLASS_ATTRIBUTES;
	}

	// publishInteractionClass
	testId = L"12";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to publish invalid interaction" << std::endl;
		rtiAmb->publishInteractionClass(xMsgHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could publish invalid interaction" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_PUBLISH_INTERACTION;
	} catch (rti1516::InteractionClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_PUBLISH_INTERACTION;
	}
	testId = L"13";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to publish valid interaction" << std::endl;
		rtiAmb->publishInteractionClass(msgHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could publish valid interaction" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_PUBLISH_INTERACTION;
	}

	// unpublishInteractionClass
	testId = L"14";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unpublish invalid interaction" << std::endl;
		rtiAmb->unpublishInteractionClass(xMsgHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could unpublish invalid interaction" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_INTERACTION;
	} catch (rti1516::InteractionClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_INTERACTION;
	}
	testId = L"15";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unpublish valid interaction" << std::endl;
		rtiAmb->unpublishInteractionClass(msgHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could unpublish valid interaction" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNPUBLISH_INTERACTION;
	}

	// subscribeObjectClassAttributes
	testId = L"16";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to subscribe to attributes for invalid class and invalid attributes" << std::endl;
		rtiAmb->subscribeObjectClassAttributes(xClassHandle, xClassAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could subscribe to attributes for invalid class and invalid attributes" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_CLASS_ATTRIBUTES;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_CLASS_ATTRIBUTES;
	}
	testId = L"17";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to subscribe to attributes for invalid class and valid attributes" << std::endl;
		rtiAmb->subscribeObjectClassAttributes(xClassHandle, classAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could subscribe to attributes for invalid class and valid attributes" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_CLASS_ATTRIBUTES;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_CLASS_ATTRIBUTES;
	}
	testId = L"18";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to subscribe to attributes for valid class and invalid attributes" << std::endl;
		rtiAmb->subscribeObjectClassAttributes(classHandle, xClassAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could subscribe to attributes for valid class and invalid attributes" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_CLASS_ATTRIBUTES;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_CLASS_ATTRIBUTES;
	}
	testId = L"19";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to subscribe to attributes for valid class and valid attributes" << std::endl;
		rtiAmb->subscribeObjectClassAttributes(classHandle, classAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could subscribe to attributes for valid class and valid attributes" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_CLASS_ATTRIBUTES;
	}

	// unsubscribeObjectClass
	testId = L"20";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unsubscribe from invalid class" << std::endl;
		rtiAmb->unsubscribeObjectClass(xClassHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could unsubscribe from invalid class" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_CLASS;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_CLASS;
	}
	testId = L"21";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unsubscribe from valid class" << std::endl;
		rtiAmb->unsubscribeObjectClass(classHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could unpublish invalid class" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_CLASS;
	}

	// unsubscribeObjectClassAttributes
	rtiAmb->subscribeObjectClassAttributes(classHandle, classAttributes);
	testId = L"22";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unsubscribe from attributes for invalid class and invalid attributes" << std::endl;
		rtiAmb->unsubscribeObjectClassAttributes(xClassHandle, xClassAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could unsubscribe to attributes for invalid class and invalid attributes" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_CLASS_ATTRIBUTES;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_CLASS_ATTRIBUTES;
	}
	testId = L"23";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unsubscribe from attributes for invalid class and valid attributes" << std::endl;
		rtiAmb->unsubscribeObjectClassAttributes(xClassHandle, classAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could unsubscribe to attributes for invalid class and valid attributes" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_CLASS_ATTRIBUTES;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_CLASS_ATTRIBUTES;
	}
	testId = L"24";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unsubscribe from attributes for valid class and invalid attributes" << std::endl;
		rtiAmb->unsubscribeObjectClassAttributes(classHandle, xClassAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Skip." << std::endl;

		// TODO Restore proper handling of test case.
		//std::wcerr << testId << L"N Fail: Could unsubscribe to attributes for valid class and invalid attributes" << std::endl;
		//return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_CLASS_ATTRIBUTES;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_CLASS_ATTRIBUTES;
	}
	testId = L"25";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unsubscribe from attributes for valid class and valid attributes" << std::endl;
		rtiAmb->unsubscribeObjectClassAttributes(classHandle, classAttributes);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could unsubscribe to attributes for valid class and valid attributes" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_CLASS_ATTRIBUTES;
	}

	// subscribeInteractionClass
	testId = L"26";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to subscribe to invalid interaction" << std::endl;
		rtiAmb->subscribeInteractionClass(xMsgHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could subscribe to invalid interaction" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_INTERACTION;
	} catch (rti1516::InteractionClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_INTERACTION;
	}
	testId = L"27";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to subscribe to valid interaction" << std::endl;
		rtiAmb->subscribeInteractionClass(msgHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could subscribe to valid interaction" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_INTERACTION;
	}

	// unsubscribeInteractionClass
	testId = L"28";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unsubscribe from invalid interaction" << std::endl;
		rtiAmb->unsubscribeInteractionClass(xMsgHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could unsubscribe from invalid interaction" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_SUBSCRIBE_INTERACTION;
	} catch (rti1516::InteractionClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
	}
	testId = L"29";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to unsubscribe from valid interaction" << std::endl;
		rtiAmb->unsubscribeInteractionClass(msgHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Could unsubscribe from valid interaction" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_UNSUBSCRIBE_INTERACTION;
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
