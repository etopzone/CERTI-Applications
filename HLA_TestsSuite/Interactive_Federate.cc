#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#include <iostream>

RTIfedTime* temps = new RTIfedTime(0.0);
RTIfedTime* lookahead = new RTIfedTime(1.0);
RTIfedTime ft;

class Federe_Interactif : public RTI::RTIambassador, public NullFederateAmbassador
{
public:
  Federe_Interactif() {}
  virtual ~Federe_Interactif() throw (RTI::FederateInternalError) {}
  
private:
  void timeAdvanceGrant (const RTI::FedTime& theTime)
  throw (
    RTI::InvalidFederationTime,
    RTI::TimeAdvanceWasNotInProgress,
    RTI::FederateInternalError)
  {
    *(temps) = theTime;
    std::cout << std::endl << "t=" << ((RTIfedTime&)theTime).getTime() << " : <----- timeAdvanceGrant(" << ((RTIfedTime&)theTime).getTime() << ")"<< std::endl << std::endl;
  }

  void receiveInteraction (
          RTI::InteractionClassHandle       theInteraction, // supplied C1
    const RTI::ParameterHandleValuePairSet& theParameters,  // supplied C4
    const RTI::FedTime&                     theTime,        // supplied C4
    const char                             *theTag,         // supplied C4
          RTI::EventRetractionHandle        theHandle)      // supplied C1
  throw (
    RTI::InteractionClassNotKnown,
    RTI::InteractionParameterNotKnown,
    RTI::InvalidFederationTime,
    RTI::FederateInternalError)
  {
    std::cout << std::endl << "t=" << ((RTIfedTime&)theTime).getTime() << " : <----- receiveInteraction(" << ((RTIfedTime&)theTime).getTime() << ")" << std::endl;
  }

  void receiveInteraction (
          RTI::InteractionClassHandle       theInteraction, // supplied C1
    const RTI::ParameterHandleValuePairSet& theParameters,  // supplied C4
    const char                             *theTag)         // supplied C4
  throw (
    RTI::InteractionClassNotKnown,
    RTI::InteractionParameterNotKnown,
    RTI::FederateInternalError)
  {
    std::cout << std::endl << "     <----- receiveInteraction" << std::endl;
  }
};


//VARIABLE
Federe_Interactif* myFedere_Interactif = new Federe_Interactif();

int main(int argc, char **argv)
{

  int i;
  float d;
  char c, entree;

  if (argc!=2)
  {
    std::cout <<"Vous devez donner en parametre le nom du federe" << std::endl;
    exit(1);
  }

  while(1)
  {
    std::cout <<"Voulez-vous creer la federation ? [y/n]" << std::endl;
    scanf("%c", &c);
    scanf("%c", &entree);
    if (c=='y' || c=='n') break;
    std::cout <<"Vous devez entrer 'y' ou 'n'" << std::endl;
  }

//CREER
  if (c=='y')
  {
    myFedere_Interactif->createFederationExecution("Interactive_Federation","Interactive_Federation.fed");
    std::cout << "federation creee" << std::endl;
  }

//REJOINDRE
  myFedere_Interactif->joinFederationExecution(argv[1], "Interactive_Federation", myFedere_Interactif);
  std::cout << "federation rejointe" << std::endl;

//PUBLIER
  myFedere_Interactif->publishInteractionClass(myFedere_Interactif->getInteractionClassHandle("msg"));
  std::cout << "interaction publiee" << std::endl;

//SOUSCRIRE
  myFedere_Interactif->subscribeInteractionClass(myFedere_Interactif->getInteractionClassHandle("msg"), RTI::RTI_TRUE);
  std::cout << "interaction souscrite" << std::endl;

//DECLARER REGULATEUR ET CONTRAINT
#define WITH_CERTI
#ifdef WITH_CERTI
  myFedere_Interactif->queryFederateTime(*temps);
  myFedere_Interactif->enableTimeRegulation(*temps, *lookahead);
  myFedere_Interactif->enableTimeConstrained();
  // myFedere_Interactif->modifyLookahead(*(new RTIfedTime(1.0)));
#else
  bool timeRegulation = false;
  while (!timeRegulation)
  {
    try
      { myFedere_Interactif->enableTimeRegulation(*temps,*lookahead); }
    catch (RTI::EnableTimeRegulationPending&)
      { myFedere_Interactif->tick(0.1, 0.2); }
    catch (RTI::TimeRegulationAlreadyEnabled&)
      { timeRegulation = true; }
  }
  bool timeConstrained = false;
  while (!timeConstrained)
  {
  try
    { myFedere_Interactif->enableTimeConstrained(); }
  catch (RTI::EnableTimeConstrainedPending&)
    { myFedere_Interactif->tick(0.1, 0.2); }
  catch (RTI::TimeConstrainedAlreadyEnabled&)
    { timeConstrained = true; }
  }
#endif
  std::cout << "federe regulateur et contraint" << std::endl;

//SYNCHRONISER
  std::cout << std::endl << "Synchro manuel, tapez ENTREE continuer";
  getchar();
  std::cout << "federation synchronisee" << std::endl << std::endl;
  
  while(1)
  {
    while(1)
    {
      std::cout << std::endl <<"Choisissez une action : " << std::endl;
      std::cout <<"  1 : timeAdvanceRequest" << std::endl;
      std::cout <<"  2 : nextEventRequest" << std::endl;
      std::cout <<"  3 : sendInteraction" << std::endl;
      std::cout <<"  5 : tick()" << std::endl;
      std::cout <<"  6 : queryLBTS" << std::endl;
      std::cout <<"  7 : queryMinNextEventTime" << std::endl;
      std::cout <<"  8 : queryLookahead" << std::endl;
      std::cout <<"  9 : timeAdvanceRequestAvailable" << std::endl;
      std::cout <<"  10 : nextEventRequestAvailable" << std::endl;
      std::cout <<"  20 : Quitter la federation" << std::endl;
      if (c=='y')
        std::cout <<"  21 : Detruire la federation" << std::endl;
      scanf("%i", &i);
      scanf("%c", &entree);
      if (i==1 || i==2 || i==3 || i==5 || i==6 || i==7 || i==8 || i==9 || i==10 || i==20) break;
      if (c=='y')
        { if (i==21) break; }
      if (c=='n')
        std::cout <<"Vous devez entrer 1, ..., 10 ou 20" << std::endl << std::endl;
      if (c=='y')
        std::cout <<"Vous devez entrer 1, ..., 10 ou 21" << std::endl << std::endl;
    }

//TIME_ADVANCE_REQUEST
    if (i==1)
    {
      std::cout << std::endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
      scanf("%f", &d);
      scanf("%c", &entree);
      myFedere_Interactif->timeAdvanceRequest(*(new RTIfedTime(d)));
      std::cout << std::endl << "t=" << temps->getTime() << " : -----> timeAdvanceRequest(" << d << ")" << std::endl;
    }

//NEXT_EVENT_REQUEST
    else if (i==2)
    {
      std::cout << std::endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
      scanf("%f", &d);
      scanf("%c", &entree);
      myFedere_Interactif->nextEventRequest(*(new RTIfedTime(d)));
      std::cout << std::endl << "t=" << temps->getTime() << " : -----> nextEventRequest(" << d << ")" << std::endl;
    }

//SEND_INTERACTION
    else if (i==3)
    {
      std::cout << std::endl << "t=" << temps->getTime() << " : Donner la valeur de l'estampille voulue : ";
      scanf("%f", &d);
      scanf("%c", &entree);
      myFedere_Interactif->sendInteraction(myFedere_Interactif->getInteractionClassHandle("msg"), *(RTI::ParameterSetFactory::create(0)), *(new RTIfedTime(d)), "msg");
      std::cout << std::endl << "t=" << temps->getTime() << " : -----> sendInteraction(getInteractionClassHandle(-, -, " << d << ", -)" << std::endl;
    }

//TICK
    else if (i==5)
    {
      myFedere_Interactif->tick(0.1, 0.2);
      std::cout << std::endl << "t=" << temps->getTime() << " : -----> tick" << std::endl;
    }

//REQUEST_LBTS
    else if (i==6)
    {
      myFedere_Interactif->queryLBTS(ft);
      std::cout << std::endl << "LBTS=" << ft.getTime() << std::endl;
    }

//REQUEST_MIN_NEXT_EVENT_TIME
    else if (i==7)
    {
      myFedere_Interactif->queryMinNextEventTime(ft);
      std::cout << std::endl << "MinNextEventTime=" << ft.getTime() << std::endl;
    }

//REQUEST_LOOKAHEAD
    else if (i==8)
    {
      myFedere_Interactif->queryLookahead(ft);
      std::cout << std::endl << "Lookahead=" << ft.getTime() << std::endl;
    }

//TIME_ADVANCE_REQUEST_AVAILABLE
    else if (i==9)
    {
      std::cout << std::endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
      scanf("%f", &d);
      scanf("%c", &entree);
      myFedere_Interactif->timeAdvanceRequestAvailable(*(new RTIfedTime(d)));
      std::cout << std::endl << "t=" << temps->getTime() << " : -----> timeAdvanceRequestAvailable(" << d << ")" << std::endl;
    }

//NEXT_EVENT_REQUEST_AVAILABLE
    else if (i==10)
    {
      std::cout << std::endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
      scanf("%f", &d);
      scanf("%c", &entree);
      myFedere_Interactif->nextEventRequestAvailable(*(new RTIfedTime(d)));
      std::cout << std::endl << "t=" << temps->getTime() << " : -----> nextEventRequestAvailable(" << d << ")" << std::endl;
    }
//QUITTER
    else if (i==20)
    {
      myFedere_Interactif->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
      std::cout << std::endl << "federation quittee" << std::endl;
      if (c=='n')
        break;
    }

//DETRUIRE
    else if (i==21)
    {
      myFedere_Interactif->destroyFederationExecution("Federation_Interactive");
      std::cout << std::endl << "federation detruite" << std::endl;
      break;
    }
  }

  exit(0);
}
