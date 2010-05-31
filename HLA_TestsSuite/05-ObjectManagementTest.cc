// 05-ObjectManagementTest.cc : Defines the entry point for the console application.
//

#include "05-ObjectManagementTest.hh"
#include "ReturnCodes.hh"

#include <RTI/certiRTI1516.h>
#include <RTI/Enums.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include <RTI1516fedTime.h>

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
, fedAmb2()
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

	// handles.
	rti1516::ObjectClassHandle classHandle;
	rti1516::AttributeHandle attrHandle;
	rti1516::AttributeHandleSet classAttributes;
	rti1516::AttributeHandleValueMap attributeValues;
	rti1516::AttributeHandle invalidAttrHandle;
	rti1516::AttributeHandleValueMap invalidAttributes;

	rti1516::VariableLengthData theUserSuppliedTag;
	std::wstring className(L"TestObjectClass");

	std::wstring testId(L"00");

	testId = L"01";

	try {
		//////////////////////////////////////////////
		// Set up
		std::auto_ptr< rti1516::RTIambassador > rtiAmbP(rtiAmbFact->createRTIambassador(constructorArgs));
		rtiAmb = rtiAmbP.release();

		try {
			rtiAmb->createFederationExecution(federationName, L"Certi-Test-05.xml");
			// expected
		} catch (rti1516::FederationExecutionAlreadyExists &)
		{
			// expected
		} catch (rti1516::Exception &)
		{
			throw;
		}

		fedHandle = rtiAmb->joinFederationExecution(fedName, federationName, *this);

		classHandle = rtiAmb->getObjectClassHandle(className);
		attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
		classAttributes.insert(attrHandle);
		std::string testString("test string");
		rti1516::VariableLengthData varData(testString.c_str(), testString.size());
		attributeValues.insert(std::pair<rti1516::AttributeHandle, rti1516::VariableLengthData>(attrHandle, varData));
		invalidAttributes.insert(std::pair<rti1516::AttributeHandle, rti1516::VariableLengthData>(invalidAttrHandle, varData)); 
		theUserSuppliedTag.setData(testString.c_str(), testString.size());

	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_ERROR_CREATING_AMBASSADOR;
	}

	std::wcerr << testId << L"E Pass: Federation joined. Press enter to continue with test." << std::endl;
	getchar();


	// REGISTER OBJECT INSTANCE
	testId = L"02";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to register an object class using an invalid class handle" << std::endl;
		rti1516::ObjectClassHandle invalidClassHandle;
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(invalidClassHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get ObjectInstanceHandle for invalid class handle. Return value:" << instance << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	} catch (rti1516::ObjectClassNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"03";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to register an object class using a class handle of an unpublished Object Class" << std::endl;
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could register an unpublished Object Class. Return value:" << instance << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	} catch (rti1516::ObjectClassNotPublished &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"04";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to register an object class using a class handle of an unpublished Object Class" << std::endl;

		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle, std::wstring(L"instance1"));
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could register an unpublished Object Class. Return value:" << instance << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	} catch (rti1516::ObjectClassNotPublished &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"05";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to register an object class using a valid and publised class handle" << std::endl;
		rtiAmb->publishObjectClassAttributes(classHandle, classAttributes);
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
		////////////////////////////////////////////////
		// expected
		if ( instance.isValid() )
		{
			std::wcerr << testId << L"E Pass: Got object instance handle for valid and published object class" << std::endl;
		} else
		{
			std::wcerr << testId << L"N Fail: Got invalid object instance handle for valid and publised object class" << std::endl;
			return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"06";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to register an object class using a valid and publised class handle" << std::endl;
		rtiAmb->publishObjectClassAttributes(classHandle, classAttributes);
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle, std::wstring(L"instance1"));
		////////////////////////////////////////////////
		// expected
		if ( instance.isValid() )
		{
			std::wcerr << testId << L"E Pass: Got object instance handle for valid and published object class" << std::endl;
		} else
		{
			std::wcerr << testId << L"N Fail: Got invalid object instance handle for valid and publised object class" << std::endl;
			return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"07";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to register another object class using a valid and publised class handle" << std::endl;
		rtiAmb->publishObjectClassAttributes(classHandle, classAttributes);
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle, std::wstring(L"instance2"));
		////////////////////////////////////////////////
		// expected
		if ( instance.isValid() )
		{
			std::wcerr << testId << L"E Pass: Got object instance handle for valid and published object class" << std::endl;
		} else
		{
			std::wcerr << testId << L"N Fail: Got invalid object instance handle for valid and publised object class" << std::endl;
			return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
		}
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"08";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to register an object class with an instance name that is already in use" << std::endl;
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle, std::wstring(L"instance2"));
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could get ObjectInstanceHandle for an instance name that is already in use. Return value:" << instance << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	} catch (rti1516::ObjectInstanceNameInUse &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}


	// UPDATE ATTRIBUTE VALUES
	testId = L"09";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to update an attribute on an undefined object instance" << std::endl;
		rti1516::ObjectInstanceHandle xObjHandle;
		rtiAmb->updateAttributeValues(xObjHandle, attributeValues, theUserSuppliedTag);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could update an attribute of an undefined object instance" << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	} catch (rti1516::ObjectInstanceNotKnown &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"10";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to update an undefined attribute" << std::endl;
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
		rtiAmb->updateAttributeValues(instance, invalidAttributes, theUserSuppliedTag);
		////////////////////////////////////////////////
		std::wcerr << testId << L"N Fail: Could update an undefined attribute" << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	} catch (rti1516::AttributeNotDefined &e)
	{
		// expected
		std::wcerr << testId << L"E Pass: " << e.what() << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"11";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to update a defined attribute on a known object instance" << std::endl;
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
		rtiAmb->updateAttributeValues(instance, attributeValues, theUserSuppliedTag);
		////////////////////////////////////////////////
		// expected
		std::wcerr << testId << L"E Pass: Attribute values updated" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"12";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to update a defined attribute on a known object instance with valid logical time" << std::endl;
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
		std::auto_ptr<rti1516::LogicalTimeFactory> factory = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"certi");
		std::auto_ptr<rti1516::LogicalTime> time = factory->makeLogicalTime();
		rtiAmb->updateAttributeValues(instance, attributeValues, theUserSuppliedTag, *time);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: Attribute values updated" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}


	testId = L"13";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to delete an object instance" << std::endl;
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
		std::auto_ptr<rti1516::LogicalTimeFactory> factory = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"certi");
		rtiAmb->deleteObjectInstance(instance, theUserSuppliedTag);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: object instance deleted" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"14";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to delete an object instance" << std::endl;
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
		std::auto_ptr<rti1516::LogicalTimeFactory> factory = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"certi");
		std::auto_ptr<rti1516::LogicalTime> time = factory->makeLogicalTime();
		rtiAmb->updateAttributeValues(instance, attributeValues, theUserSuppliedTag, *time);
		rtiAmb->deleteObjectInstance(instance, theUserSuppliedTag);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: object instance deleted" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	testId = L"15";
	try {
		////////////////////////////////////////////////
		std::wcerr << testId << L"T Test: Attempting to delete an object instance" << std::endl;
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
		std::auto_ptr<rti1516::LogicalTimeFactory> factory = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"certi");
		std::auto_ptr<rti1516::LogicalTime> time = factory->makeLogicalTime();
		rtiAmb->deleteObjectInstance(instance, theUserSuppliedTag, *time);
		////////////////////////////////////////////////
		std::wcerr << testId << L"E Pass: object instance deleted" << std::endl;
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail: " << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}



	std::wcerr << L"Test operations performed." << std::endl;
	std::wcerr << L"Start '05-ObjectManagermentCallbackTest' and press enter to continue with processing callbacks." << std::endl;
	getchar();
	
	try {
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
		//std::auto_ptr<rti1516::LogicalTimeFactory> factory = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"certi");
		//std::auto_ptr<rti1516::LogicalTime> time = factory->makeLogicalTime();
		rtiAmb->updateAttributeValues(instance, attributeValues, theUserSuppliedTag);
		rtiAmb->deleteObjectInstance(instance, theUserSuppliedTag);
	} catch (rti1516::Exception &e)
	{
		std::wcerr << testId << L"N Fail:" << e.what() << std::endl;
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	std::wcerr << L"Press enter in '05-ObjectManagementCallbackTest' to show the callbacks." << std::endl;
	std::wcerr << L"Expecting callbacks 'DiscoverObjectInstance' (6.5), 'ReflectAttributeValues' (6.7) and 'RemoveObjectInstance' (6.11)" << std::endl;
	
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
