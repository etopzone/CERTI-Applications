// 05-ObjectManagementCallbackTest.cc : Defines the entry point for the console application.
//

#include "05-ObjectManagementCallbackTest.hh"
#include "ReturnCodes.hh"

#include <RTI/certiRTI1516.h>
#include <RTI/Enums.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include <RTI/RTI1516fedTime.h>

int main(int argc, char** argv)
{
	int retCode1 = 0;

	srand( time(0) );

	TestFedAmbCallbacks *fedAmb = new TestFedAmbCallbacks();

	retCode1 = fedAmb->execute();



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


TestFedAmbCallbacks::TestFedAmbCallbacks() throw()
: rtiAmb(0)
, fedHandle()
, fedName()
, federationName(L"TestFederationName")
{
	int nameVal;

	nameVal = (rand() % 99) + 1;

	std::wstringstream stream;
	stream << L"TestFederateCallbacks" << nameVal;

	fedName = stream.str();
	std::wcerr << L"Federate name:" << fedName << std::endl;

}

TestFedAmbCallbacks::~TestFedAmbCallbacks() throw()
{
	delete rtiAmb;
}

int TestFedAmbCallbacks::execute()
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

		//subscribe
		rtiAmb->subscribeObjectClassAttributes(classHandle, classAttributes);

	} catch (rti1516::Exception &e)
	{
		std::wcerr << L"N Setup failed:" << e.what() << std::endl;
		std::wcerr << L"Press enter to exit" << std::endl;
		getchar();
		return certi::rti1516_test::RETURN_UNDEFINED_ERROR;
	}

	while (true) {
		std::wcerr << L"Press enter to evoke callbacks" << std::endl;
		getchar();
		rtiAmb->evokeMultipleCallbacks(0.0, 1.0);
	}

	return certi::rti1516_test::RETURN_NO_ERRORS;

}

// 6.5
    void
    TestFedAmbCallbacks::discoverObjectInstance(rti1516::ObjectInstanceHandle theObject,
                           rti1516::ObjectClassHandle theObjectClass,
                           std::wstring const & theObjectInstanceName)
      throw (rti1516::CouldNotDiscover,
             rti1516::ObjectClassNotKnown,
             rti1516::FederateInternalError) 
	{
		std::wcerr << L"Object Instance '" << theObjectInstanceName << L"' discoverd. (callback 6.5)" << std::endl;
	}

    // 6.7 a
    void
    TestFedAmbCallbacks::reflectAttributeValues
    (rti1516::ObjectInstanceHandle theObject,
     rti1516::AttributeHandleValueMap const & theAttributeValues,
     rti1516::VariableLengthData const & theUserSuppliedTag,
     rti1516::OrderType sentOrder,
     rti1516::TransportationType theType)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::AttributeNotRecognized,
             rti1516::AttributeNotSubscribed,
             rti1516::FederateInternalError) 
	{
		std::wcerr << L"Attribute Values Reflected (callback 6.7 a)" << std::endl;
	}

	// 6.7 b
    void
    TestFedAmbCallbacks::reflectAttributeValues
    (rti1516::ObjectInstanceHandle theObject,
     rti1516::AttributeHandleValueMap const & theAttributeValues,
     rti1516::VariableLengthData const & theUserSuppliedTag,
     rti1516::OrderType sentOrder,
     rti1516::TransportationType theType,
     rti1516::RegionHandleSet const & theSentRegionHandleSet)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::AttributeNotRecognized,
             rti1516::AttributeNotSubscribed,
             rti1516::FederateInternalError) 
	{
		std::wcerr << L"Attribute Values Reflected (callback 6.7 b)" << std::endl;
	}

	// 6.7 c
    void
    TestFedAmbCallbacks::reflectAttributeValues
    (rti1516::ObjectInstanceHandle theObject,
     rti1516::AttributeHandleValueMap const & theAttributeValues,
     rti1516::VariableLengthData const & theUserSuppliedTag,
     rti1516::OrderType sentOrder,
     rti1516::TransportationType theType,
     rti1516::LogicalTime const & theTime,
     rti1516::OrderType receivedOrder)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::AttributeNotRecognized,
             rti1516::AttributeNotSubscribed,
             rti1516::FederateInternalError) 
	{
		std::wcerr << L"Attribute Values Reflected (callback 6.7 c)" << std::endl;
	}
  
	// 6.7 d
    void
    TestFedAmbCallbacks::reflectAttributeValues
    (rti1516::ObjectInstanceHandle theObject,
     rti1516::AttributeHandleValueMap const & theAttributeValues,
     rti1516::VariableLengthData const & theUserSuppliedTag,
     rti1516::OrderType sentOrder,
     rti1516::TransportationType theType,
     rti1516::LogicalTime const & theTime,
     rti1516::OrderType receivedOrder,
     rti1516::RegionHandleSet const & theSentRegionHandleSet)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::AttributeNotRecognized,
             rti1516::AttributeNotSubscribed,
             rti1516::FederateInternalError) 
	{
		std::wcerr << L"Attribute Values Reflected (callback 6.7 d)" << std::endl;
	}
  
	// 6.7 e
    void
    TestFedAmbCallbacks::reflectAttributeValues
    (rti1516::ObjectInstanceHandle theObject,
     rti1516::AttributeHandleValueMap const & theAttributeValues,
     rti1516::VariableLengthData const & theUserSuppliedTag,
     rti1516::OrderType sentOrder,
     rti1516::TransportationType theType,
     rti1516::LogicalTime const & theTime,
     rti1516::OrderType receivedOrder,
     rti1516::MessageRetractionHandle theHandle)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::AttributeNotRecognized,
             rti1516::AttributeNotSubscribed,
             rti1516::InvalidLogicalTime,
             rti1516::FederateInternalError) 
	{
		std::wcerr << L"Attribute Values Reflected (callback 6.7 e)" << std::endl;
	}

	// 6.7 f
    void
    TestFedAmbCallbacks::reflectAttributeValues
    (rti1516::ObjectInstanceHandle theObject,
     rti1516::AttributeHandleValueMap const & theAttributeValues,
     rti1516::VariableLengthData const & theUserSuppliedTag,
     rti1516::OrderType sentOrder,
     rti1516::TransportationType theType,
     rti1516::LogicalTime const & theTime,
     rti1516::OrderType receivedOrder,
     rti1516::MessageRetractionHandle theHandle,
     rti1516::RegionHandleSet const & theSentRegionHandleSet)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::AttributeNotRecognized,
             rti1516::AttributeNotSubscribed,
             rti1516::InvalidLogicalTime,
             rti1516::FederateInternalError) 
	{
		std::wcerr << L"Attribute Values Reflected (callback 6.7 f)" << std::endl;
	}

	// 6.11 a
    void
    TestFedAmbCallbacks::removeObjectInstance(rti1516::ObjectInstanceHandle theObject,
                         rti1516::VariableLengthData const & theUserSuppliedTag,
                         rti1516::OrderType sentOrder)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::FederateInternalError) 
	{
		std::wcerr << L"Object instance removed (callback 6.11 a)" << std::endl;
	}

	// 6.11 b
    void
    TestFedAmbCallbacks::removeObjectInstance(rti1516::ObjectInstanceHandle theObject,
                         rti1516::VariableLengthData const & theUserSuppliedTag,
                         rti1516::OrderType sentOrder,
                         rti1516::LogicalTime const & theTime,
                         rti1516::OrderType receivedOrder)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::FederateInternalError) 
	{
		std::wcerr << L"Object instance removed (callback 6.11 b)" << std::endl;
	}

	// 6.11 c
    void
    TestFedAmbCallbacks::removeObjectInstance(rti1516::ObjectInstanceHandle theObject,
                         rti1516::VariableLengthData const & theUserSuppliedTag,
                         rti1516::OrderType sentOrder,
                         rti1516::LogicalTime const & theTime,
                         rti1516::OrderType receivedOrder,
                         rti1516::MessageRetractionHandle theHandle)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::InvalidLogicalTime,
             rti1516::FederateInternalError) 
	{
		std::wcerr << L"Object instance removed (callback 6.11 c)" << std::endl;
	}
