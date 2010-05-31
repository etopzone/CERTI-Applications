#ifndef OBJECT_MANAGEMENT_TEST_H
#define OBJECT_MANAGEMENT_TEST_H

#include <RTI/NullFederateAmbassador.h>

namespace rti1516 {
	class RTIambassador;
}

class TestFedAmbCallbacks : public rti1516::NullFederateAmbassador
{
public:
	TestFedAmbCallbacks()
		throw ();
	~TestFedAmbCallbacks()
		throw ();

	void execute();


	// 6.5
	virtual void
    discoverObjectInstance(rti1516::ObjectInstanceHandle theObject,
                           rti1516::ObjectClassHandle theObjectClass,
                           std::wstring const & theObjectInstanceName)
      throw (rti1516::CouldNotDiscover,
             rti1516::ObjectClassNotKnown,
             rti1516::FederateInternalError); 

    // 6.7 a
    virtual void
    reflectAttributeValues
    (rti1516::ObjectInstanceHandle theObject,
     rti1516::AttributeHandleValueMap const & theAttributeValues,
     rti1516::VariableLengthData const & theUserSuppliedTag,
     rti1516::OrderType sentOrder,
     rti1516::TransportationType theType)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::AttributeNotRecognized,
             rti1516::AttributeNotSubscribed,
             rti1516::FederateInternalError);

	// 6.7 b
    virtual void
    reflectAttributeValues
    (rti1516::ObjectInstanceHandle theObject,
     rti1516::AttributeHandleValueMap const & theAttributeValues,
     rti1516::VariableLengthData const & theUserSuppliedTag,
     rti1516::OrderType sentOrder,
     rti1516::TransportationType theType,
     rti1516::RegionHandleSet const & theSentRegionHandleSet)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::AttributeNotRecognized,
             rti1516::AttributeNotSubscribed,
             rti1516::FederateInternalError);

	// 6.7 c
    virtual void
    reflectAttributeValues
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
             rti1516::FederateInternalError);
  
	// 6.7 d
    virtual void
    reflectAttributeValues
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
             rti1516::FederateInternalError);
  
	// 6.7 e
    virtual void
    reflectAttributeValues
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
             rti1516::FederateInternalError);

	// 6.7 f
    virtual void
    reflectAttributeValues
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
             rti1516::FederateInternalError);

	// 6.11 a
    virtual void
    removeObjectInstance(rti1516::ObjectInstanceHandle theObject,
                         rti1516::VariableLengthData const & theUserSuppliedTag,
                         rti1516::OrderType sentOrder)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::FederateInternalError);

	// 6.11 b
    virtual void
    removeObjectInstance(rti1516::ObjectInstanceHandle theObject,
                         rti1516::VariableLengthData const & theUserSuppliedTag,
                         rti1516::OrderType sentOrder,
                         rti1516::LogicalTime const & theTime,
                         rti1516::OrderType receivedOrder)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::FederateInternalError);

	// 6.11 c
    virtual void
    removeObjectInstance(rti1516::ObjectInstanceHandle theObject,
                         rti1516::VariableLengthData const & theUserSuppliedTag,
                         rti1516::OrderType sentOrder,
                         rti1516::LogicalTime const & theTime,
                         rti1516::OrderType receivedOrder,
                         rti1516::MessageRetractionHandle theHandle)
      throw (rti1516::ObjectInstanceNotKnown,
             rti1516::InvalidLogicalTime,
             rti1516::FederateInternalError);


private:
	rti1516::RTIambassador *rtiAmb;
	rti1516::FederateHandle fedHandle;
	std::wstring fedName;
	std::wstring federationName;

};

class TestFedAmb : public rti1516::NullFederateAmbassador
{
public:
	TestFedAmb()
		throw ();
	~TestFedAmb()
		throw ();

	int execute(int argc, char **argv);

private:
	rti1516::RTIambassador *rtiAmb;
	rti1516::FederateHandle fedHandle;
	std::wstring fedName;
	std::wstring federationName;
	TestFedAmbCallbacks *fedAmb2;

};


	

#endif // OBJECT_MANAGEMENT_TEST_H
