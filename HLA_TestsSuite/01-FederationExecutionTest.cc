#include "01-FederationExecutionTest.hh"
#include "ReturnCodes.hh"

#include <RTI/certiRTI1516.h>
#include <RTI/Enums.h>
#include <iostream>
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
	TestFedAmb *fedAmb = new TestFedAmb();

	retCode1 = fedAmb->execute(argc, argv);

	delete fedAmb;

	if ( retCode1 == certi::rti1516_test::RETURN_NO_ERRORS)
	{
		std::wcerr << L"Pass: Test completed" << std::endl;
	} else
	{
		std::wcerr << L"Fail: Test failed with error code: " << retCode1 << std::endl;

	}

	// Poor man's 'pause'
	getchar();

	return retCode1;
}

TestFedAmb::TestFedAmb() throw()
: rtiAmb(0)
, fedHandle()
{
}

TestFedAmb::~TestFedAmb() throw()
{
	delete rtiAmb;
}

int TestFedAmb::execute(int argc, char** argv)
{
	std::auto_ptr< rti1516::RTIambassadorFactory > rtiAmbFact(new rti1516::RTIambassadorFactory());
	std::vector< std::wstring > constructorArgs;
	try {
		//////////////////////////////////////////////
		std::auto_ptr< rti1516::RTIambassador > rtiAmbP(rtiAmbFact->createRTIambassador(constructorArgs));
		rtiAmb = rtiAmbP.release();
		//////////////////////////////////////////////
	} catch (rti1516::RTIinternalError &e)
	{
		std::wcerr << L"01N " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_CREATING_AMBASSADOR;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"02N " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_CREATING_AMBASSADOR;
	}

	getchar();

	try {
		////////////////////////////////////////////////
		std::wcerr << L"Attempting to destroy Federation" << std::endl;
		rtiAmb->destroyFederationExecution(L"TestFederationName");
		////////////////////////////////////////////////
		std::wcerr << L"03E " << L"Pass: Destroyed federation before starting rest of test" << std::endl;
	} catch (rti1516::FederationExecutionDoesNotExist &e)
	{
		// expected
		std::wcerr << L"04E " << e.what() << L"(Pass: No federation to destroy)" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"05N " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_DESTROYING_FEDERATION;
	}

	getchar();

	try {
		///////////////////////////////////////////////
		std::wcerr << L"Attempting to join non-existant Federation" << std::endl;
		fedHandle = rtiAmb->joinFederationExecution(L"TestFederateName", L"TestFederationName", *this);
		///////////////////////////////////////////////
		std::wcerr << L"06N " << L"Fail: Could join federation that should not exist yet." << std::endl;
		return certi::rti1516_test::RETURN_ERROR_JOINING_INVALID_FEDERATION;
	} catch (rti1516::FederationExecutionDoesNotExist &e)
	{
		// expected
		std::wcerr << L"07E " << e.what() << L"(Pass: Could not join non-existant federation)" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"08N " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_JOINING_INVALID_FEDERATION;
	} 

	getchar();

	try {
		////////////////////////////////////////////////
		std::wcerr << L"Attempting to create Federation" << std::endl;
		rtiAmb->createFederationExecution(L"TestFederationName", L"01.xml");
		////////////////////////////////////////////////
		// expected
		std::wcerr << L"09E Pass: Created federation" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"10N " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_CREATING_FEDERATION;
	}

	try {
		////////////////////////////////////////////////
		std::wcerr << L"Attempting to join Federation" << std::endl;
		rtiAmb->joinFederationExecution(L"TestFederateName", L"TestFederationName", *this);
		////////////////////////////////////////////////
		// expected
		std::wcerr << L"11E Pass: Joined federation" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"12N " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_CREATING_FEDERATION;
	}

	try {
		////////////////////////////////////////////////
		std::wcerr << L"Attempting to destroy Federation while joined" << std::endl;
		rtiAmb->destroyFederationExecution(L"TestFederationName");
		////////////////////////////////////////////////
		std::wcerr << L"13N " << L"Could destroy federation before resigning!" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_DESTROYING_FEDERATION;
	} catch (rti1516::FederatesCurrentlyJoined &e)
	{
		// expected
		std::wcerr << L"14E " << e.what() << L"(Pass: Could not destroy federation while still joined)" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"15N " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_DESTROYING_FEDERATION;
	}

	try {
		////////////////////////////////////////////////
		std::wcerr << L"Attempting to resign from Federation" << std::endl;
		rtiAmb->resignFederationExecution(rti1516::CANCEL_THEN_DELETE_THEN_DIVEST);
		////////////////////////////////////////////////
		// expected
		std::wcerr << L"16E " << L"Pass: Resigned from federation" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"17N " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_DESTROYING_FEDERATION;
	}

	try {
		////////////////////////////////////////////////
		std::wcerr << L"Attempting to destroy Federation" << std::endl;
		rtiAmb->destroyFederationExecution(L"TestFederationName");
		////////////////////////////////////////////////
		// expected
		std::wcerr << L"18E " << L"Pass: Destroyed federation." << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"19N " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_DESTROYING_FEDERATION;
	}

	return certi::rti1516_test::RETURN_NO_ERRORS;
}
