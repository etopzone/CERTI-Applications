#include <iostream>
#include <NullFederateAmbassador.hh>

class TestAmbassador : public NullFederateAmbassador
{
public:
  TestAmbassador()
   : mRtiAmbassador()
  {}
  virtual ~TestAmbassador()
    throw (RTI::FederateInternalError)
  {}

  int joinAndLeave(const char* federation, const char* model, const char* federate);
  int destroy(const char* federation);

protected:
  RTI::RTIambassador mRtiAmbassador;
};

int TestAmbassador::joinAndLeave(
  const char* federation, const char* model, const char* federate)
{
  try
  {
    mRtiAmbassador.createFederationExecution(federation, model);
  }
  catch (RTI::FederationExecutionAlreadyExists& e)
  { }

  mRtiAmbassador.joinFederationExecution(federate, federation, this);

  mRtiAmbassador.resignFederationExecution(RTI::NO_ACTION);
    //RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);

  return 0;
}

int 
TestAmbassador::destroy(const char* federation)
{
  try
  {
    mRtiAmbassador.destroyFederationExecution(federation);
  }
  catch (RTI::FederationExecutionDoesNotExist& e)
  { }

  return 0;
}


int main(void)
{
  TestAmbassador test;
  //test.joinAndLeave("Test01", "Test01.fed", "name");
  std::cout << "OK" << std::endl;
  test.joinAndLeave("test_CreateJoinResignDestroy", "test_CreateJoinResignDestroy.fed", "");
  //test.destroy("Test01");
  std::cout << "OK" << std::endl;

  return 0;
}
