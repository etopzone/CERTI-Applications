#include "02-HandlesAndNamesTest.hh"
#include "ReturnCodes.hh"

#include <RTI/certiRTI1516.h>
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

	// Invalid handles.
	rti1516::ObjectClassHandle xClassHandle;
	rti1516::AttributeHandle xAttrHandle;
	rti1516::AttributeHandleSet xClassAttributes;
	rti1516::InteractionClassHandle xMsgHandle;
	rti1516::ParameterHandle xParamHandle;
	rti1516::ObjectInstanceHandle xObjHandle;

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

	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_CREATING_AMBASSADOR;
	}

	std::wcerr << testId << L"E Pass: Federation joined. Press enter to continue with test." << std::endl;
	getchar();

	testId = L"02";
	// CLASS HANDLE
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get handle for invalid class" << std::endl;
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"InvalidObjectClassName");
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get class handle for invalid object class name" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_HANDLE;
	} catch (rti1516::NameNotFound &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_HANDLE;
	}

	testId = L"03";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get handle for valid class name" << std::endl;
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		////////////////////////////////////////////////
		// expected
		if ( classHandle.isValid() )
		{
			std::wcerr << testId << L"E Pass: Got class handle for valid class name" << std::endl;
		} else
		{
			std::wcerr << testId << L"N Fail: Got invalid class handle for valid class name" << std::endl;
			return certi::rti1516_test::RETURN_ERROR_GET_CLASS_HANDLE;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_HANDLE;
	}

	// CLASS NAME
	testId = L"04";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get name for invalid class handle" << std::endl;
		rti1516::ObjectClassHandle classHandle;
		std::wstring className = rtiAmb->getObjectClassName(classHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get class name for invalid class handle. Name returned:" << className << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_NAME;
	} catch (rti1516::InvalidObjectClassHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_NAME;
	}

	testId = L"05";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get name for valid class handle" << std::endl;
		std::wstring className(L"TestObjectClass");
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(className);
		std::wstring retClassName = rtiAmb->getObjectClassName(classHandle);
		////////////////////////////////////////////////
		// expected
		if ( className != retClassName )
		{
			std::wcerr << testId << L"N Fail: Returned classname does not match expected classname. " << 
						  L"(Expected " << className << L", got " << retClassName << L".)" << std::endl;
			return certi::rti1516_test::RETURN_ERROR_GET_CLASS_NAME;
		} else 
		{
			std::wcerr << testId << L"E Pass: Got expected classname" << std::endl;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_NAME;
	}

	// ATTRIBUTE HANDLE
	testId = L"06";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get attribute handle for invalid class" << std::endl;
		rti1516::ObjectClassHandle classHandle;
		rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"InvalidAttributeName");
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get attribute handle for invalid class handle" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_HANDLE;
	} catch (rti1516::InvalidObjectClassHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_HANDLE;
	}

	testId = L"07";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get handle for invalid attribute name" << std::endl;
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"InvalidAttributeName");
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get attribute handle for invalid attribute name" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_HANDLE;
	} catch (rti1516::NameNotFound &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_HANDLE;
	}

	testId = L"08";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get handle for valid attribute class" << std::endl;
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
		////////////////////////////////////////////////
		// expected
		if ( classHandle.isValid() )
		{
			std::wcerr << testId << L"E Pass: Got attribute handle for valid attribute class & name" << std::endl;
		} else
		{
			std::wcerr << testId << L"N Fail: Got invalid class handle for valid class name" << std::endl;
			return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_HANDLE;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_HANDLE;
	}

	// ATTRIBUTE NAME
	testId = L"09";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get name for invalid attribute handle of invalid class" << std::endl;
		std::wstring attrName = rtiAmb->getAttributeName(xClassHandle, xAttrHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get name for invalid attribute handle of invalid class" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_NAME;
	} catch (rti1516::InvalidObjectClassHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::InvalidAttributeHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_NAME;
	}
	testId = L"10";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get name for invalid attribute handle of valid class" << std::endl;
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		std::wstring attrName = rtiAmb->getAttributeName(classHandle, xAttrHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get name for invalid attribute handle of valid class" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_NAME;
	} catch (rti1516::InvalidObjectClassHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::InvalidAttributeHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_NAME;
	}
	testId = L"11";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get name for valid attribute handle of invalid class" << std::endl;
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
		std::wstring attrName = rtiAmb->getAttributeName(xClassHandle, attrHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get name for invalid attribute handle of invalid class" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_NAME;
	} catch (rti1516::InvalidObjectClassHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::InvalidAttributeHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_NAME;
	}
	testId = L"12";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get name for valid attribute handle" << std::endl;
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		std::wstring orgAttrName(L"Data");
		rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, orgAttrName);
		std::wstring attrName = rtiAmb->getAttributeName(classHandle, attrHandle);
		////////////////////////////////////////////////
		if ( orgAttrName == attrName )
		{
			std::wcerr << testId << L"E Pass: Could get name for invalid attribute handle of valid class" << std::endl;
		} else
		{
			std::wcerr << testId << L"N Fail: Could get name for invalid attribute handle of valid class, but returned name didn't match" << std::endl;
			return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_NAME;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_ATTRIBUTE_NAME;
	}

	// INTERACTION HANDLE
	testId = L"13";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get handle for invalid interaction" << std::endl;
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"InvalidInteractionName");
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get handle for invalid interaction name." << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_INTERACTION_HANDLE;
	} catch (rti1516::NameNotFound &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_INTERACTION_HANDLE;
	}

	testId = L"14";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get handle for valid interaction name" << std::endl;
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
		////////////////////////////////////////////////
		// expected
        if (msgHandle.isValid() )
        {
			std::wcerr << testId << L"E Pass: Got interaction handle for valid interaction name" << std::endl;
		} else
		{
			std::wcerr << testId << L"N Fail: Got invalid interaction handle for valid interaction name" << std::endl;
			return certi::rti1516_test::RETURN_ERROR_GET_INTERACTION_HANDLE;
        }
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_INTERACTION_HANDLE;
	}

	// INTERACTION NAME
	testId = L"15";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get name for invalid interaction handle" << std::endl;
		std::wstring name = rtiAmb->getInteractionClassName(xMsgHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get name for invalid interaction handle: " << name << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_INTERACTION_NAME;
	} catch (rti1516::InvalidInteractionClassHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_INTERACTION_NAME;
	}

	testId = L"16";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get name for valid interaction handle" << std::endl;
		std::wstring orgMsgName(L"Msg");
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(orgMsgName);
		std::wstring msgName = rtiAmb->getInteractionClassName(msgHandle);
		////////////////////////////////////////////////
		// expected
        if ( orgMsgName == msgName )
        {
			std::wcerr << testId << L"E Pass: Got name for valid interaction handle" << std::endl;
		} else
		{
			std::wcerr << testId << L"N Fail: Got invalid name for valid interaction handle" << std::endl;
			return certi::rti1516_test::RETURN_ERROR_GET_INTERACTION_NAME;
        }
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_INTERACTION_NAME;
	}

	// PARAMETER HANDLE
	testId = L"17";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get parameter handle for invalid interaction and invalid parameter name" << std::endl;
		rti1516::ParameterHandle parHandle = rtiAmb->getParameterHandle(xMsgHandle, L"InvalidParameterName");
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get parameter handle for invalid interaction and invalid parameter name" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_HANDLE;
	} catch (rti1516::InvalidInteractionClassHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::NameNotFound &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_HANDLE;
	}

	testId = L"18";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get parameter handle for invalid interaction and valid parameter name" << std::endl;
		rti1516::ParameterHandle parHandle = rtiAmb->getParameterHandle(xMsgHandle, L"MsgData");
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get parameter handle for invalid interaction and valid parameter name" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_HANDLE;
	} catch (rti1516::InvalidInteractionClassHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::NameNotFound &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_HANDLE;
	}

	testId = L"19";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get parameter handle for valid interaction and invalid parameter name" << std::endl;
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
		rti1516::ParameterHandle parHandle = rtiAmb->getParameterHandle(msgHandle, L"InvalidParameterName");
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get parameter handle for valid interaction and invalid parameter name" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_HANDLE;
	} catch (rti1516::NameNotFound &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_HANDLE;
	}

	testId = L"20";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get parameter handle for valid interaction and valid parameter name" << std::endl;
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
		rti1516::ParameterHandle parHandle = rtiAmb->getParameterHandle(msgHandle, L"MsgData");
		////////////////////////////////////////////////
		// expected
		if ( parHandle.isValid() )
		{
			std::wcerr << testId << L"E Pass: Got parameter handle for valid interaction and valid parameter name" << std::endl;
		} else
		{
			std::wcerr << testId << L"N Fail: Got invalid parameter handle for valid interaction and valid parameter name" << std::endl;
			return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_HANDLE;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_HANDLE;
	}

	// PARAMETER NAME
	testId = L"21";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get parameter name with invalid interaction and invalid parameter handle" << std::endl;
		std::wstring paramName = rtiAmb->getParameterName(xMsgHandle, xParamHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get parameter name with invalid interaction and invalid parameter handle" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_NAME;
	} catch (rti1516::InvalidInteractionClassHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::InvalidParameterHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::InteractionParameterNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_NAME;
	}
	testId = L"22";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get parameter name with invalid interaction and valid parameter handle" << std::endl;
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
		rti1516::ParameterHandle parHandle = rtiAmb->getParameterHandle(msgHandle, L"MsgData");

		std::wstring paramName = rtiAmb->getParameterName(xMsgHandle, parHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get parameter name with invalid interaction and valid parameter handle" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_NAME;
	} catch (rti1516::InvalidInteractionClassHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_NAME;
	}
	testId = L"23";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get parameter name with valid interaction and invalid parameter handle" << std::endl;
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");

		std::wstring paramName = rtiAmb->getParameterName(msgHandle, xParamHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get parameter name with valid interaction and invalid parameter handle" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_NAME;
	} catch (rti1516::InvalidParameterHandle &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::InteractionParameterNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_NAME;
	}
	testId = L"24";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get parameter name with valid interaction and valid parameter handle" << std::endl;
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
		rti1516::ParameterHandle parHandle = rtiAmb->getParameterHandle(msgHandle, L"MsgData");
		std::wstring orgParamName(L"MsgData");

		std::wstring paramName = rtiAmb->getParameterName(msgHandle, parHandle);
		////////////////////////////////////////////////
		if ( orgParamName == paramName )
		{
			// expected
			std::wcerr << testId << L"E Pass: Could get parameter name with valid interaction and valid parameter handle" << std::endl;
		} else
		{
			std::wcerr << testId << L"N Fail: Could get parameter name with invalid interaction and valid parameter handle, but returned name was wrong" << std::endl;
			return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_NAME;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_PARAMETER_NAME;
	}

	// OBJECT HANDLE ?
	int nameVal;

	nameVal = (rand() % 99) + 1;

	std::wstringstream stream;
	stream << L"TestObject" << nameVal;

	std::wstring objName = stream.str();
	rti1516::ObjectInstanceHandle objHandle;
	bool objRegistered = false;
	try {
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
		rti1516::AttributeHandleSet attrSet;
		attrSet.insert(attrHandle);
		rtiAmb->publishObjectClassAttributes(classHandle, attrSet);
		objHandle = rtiAmb->registerObjectInstance(classHandle, objName);
		objRegistered = true;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"Warning: could not register object instance. Some tests may be skipped. Reason: " << e.what() << std::endl;
	}

	testId = L"25";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get object handle for invalid object instance name" << std::endl;
		rti1516::ObjectInstanceHandle retObjHandle = rtiAmb->getObjectInstanceHandle(L"InvalidObjectInstanceName");
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get object handle for invalid object instance name" << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_HANDLE;
	} catch (rti1516::ObjectInstanceNotKnown &e)
	{
		// expected
		std::wcerr << testId << L"E Pass:" << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_HANDLE;
	}

	testId = L"26";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get object handle for valid object instance name" << std::endl;
		if ( objRegistered )
		{
			rti1516::ObjectInstanceHandle retObjHandle = rtiAmb->getObjectInstanceHandle(objName);
			////////////////////////////////////////////////
			// expected
			if ( retObjHandle.isValid() )
			{
				std::wcerr << testId << L"E Pass: Got object handle for valid object instance name" << std::endl;
			} else
			{
				std::wcerr << testId << L"N Fail: Got invalid object handle for valid object instance name" << std::endl;
				return certi::rti1516_test::RETURN_ERROR_GET_CLASS_HANDLE;
			}
		} else
		{
			std::wcerr << testId << L"E Skip:" << std::endl;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_HANDLE;
	}

	// OBJECT NAME ?
	testId = L"27";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get name for invalid object instance handle" << std::endl;
		std::wstring retObjName = rtiAmb->getObjectInstanceName(xObjHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get name for invalid object instance handle. Name returned:" << retObjName << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_NAME;
	} catch (rti1516::ObjectInstanceNotKnown &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_NAME;
	}
	testId = L"28";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to get name for valid object instance handle" << std::endl;
		if ( objRegistered )
		{
			std::wstring retObjName = rtiAmb->getObjectInstanceName(objHandle);
			////////////////////////////////////////////////
			// expected
			if ( objName != retObjName )
			{
				std::wcerr << testId << L"N Fail: Returned object name does not match expected object name. " << 
							  L"(Expected " << objName << L", got " << retObjName << L".)" << std::endl;
				return certi::rti1516_test::RETURN_ERROR_GET_CLASS_NAME;
			} else 
			{
				std::wcerr << testId << L"E Pass: Got expected object name" << std::endl;
			}
		} else
		{
			std::wcerr << testId << L"E Skip:" << std::endl;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_GET_CLASS_NAME;
	}

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
