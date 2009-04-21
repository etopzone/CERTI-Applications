#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#include <iostream>
#include <cstdlib>
using namespace std;


// VARIABLES GLOBALES


RTIfedTime* temps = new RTIfedTime(0.0);
RTIfedTime* lookahead = new RTIfedTime(1.0);
RTIfedTime ft;

bool timeRegulation = false;
bool timeConstrained = false;

class Federe_Interactif : public RTI::RTIambassador, public NullFederateAmbassador
{
public:
  Federe_Interactif() {}
  virtual ~Federe_Interactif() throw (RTI::FederateInternalError) {}

private:

  void timeRegulationEnabled(const RTI::FedTime& theTime)
    throw (
      RTI::InvalidFederationTime,
      RTI::EnableTimeRegulationWasNotPending,
      RTI::FederateInternalError)
  {
     std::cout << std::endl << "callback timeRegulationEnabled pour l'instant " <<
               ((RTIfedTime&)theTime).getTime() << std::endl;
     timeRegulation = true;
  }

  void timeConstrainedEnabled(const RTI::FedTime& theTime)
    throw (
      RTI::InvalidFederationTime,
      RTI::EnableTimeConstrainedWasNotPending,
      RTI::FederateInternalError)
  {
     std::cout << std::endl << "callback timeRegulationConstrained pour l'instant " <<
               ((RTIfedTime&)theTime).getTime() << std::endl;
     timeConstrained = true;
  }

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
          RTI::InteractionClassHandle       theInteraction,
    const RTI::ParameterHandleValuePairSet& theParameters,
    const RTI::FedTime&                     theTime,
    const char                             *theTag,
          RTI::EventRetractionHandle        theHandle)
  throw (
    RTI::InteractionClassNotKnown,
    RTI::InteractionParameterNotKnown,
    RTI::InvalidFederationTime,
    RTI::FederateInternalError)
  {
    std::cout << std::endl << "t=" << ((RTIfedTime&)theTime).getTime() << " : <----- receiveInteraction(" << ((RTIfedTime&)theTime).getTime() << ")" << theTag << std::endl;
  }

  void receiveInteraction (
          RTI::InteractionClassHandle       theInteraction,
    const RTI::ParameterHandleValuePairSet& theParameters,
    const char                             *theTag)
  throw (
    RTI::InteractionClassNotKnown,
    RTI::InteractionParameterNotKnown,
    RTI::FederateInternalError)
  {
    std::cout << std::endl << "     <----- receiveInteraction" << std::endl;
  }
};

void print_menu (void);
void callTICK (void);
void callCFE (void);
void callJFE (char * name);
void callRFE (void);
void callDFE (void);
void callPIC (void);
void callSIC (void);
void callSI (void);
void callETR(void);
void callETC(void);
void callTAR(void);
void callTARA(void);
void callNER(void);
void callNERA(void);
void callEAD(void);
void callDAD(void);
void callQLBTS(void);
void callQMNET(void);
void callML(void);
void callQL(void);
void callCIOT(void);

//VARIABLE
Federe_Interactif* myFedere_Interactif = new Federe_Interactif();

int main(int argc, char **argv)
{

  int i;
  float d;
  char c, entree;
  string commande;

  if (argc!=2)
  {
    cout <<"Vous devez donner en parametre le nom du federe" << endl;
    exit(1);
  }

  while(1)
  {
    cout <<"Voulez-vous un preambule automatique [y/n]" << endl;
    scanf("%c", &c);
    scanf("%c", &entree);
    if (c=='y' || c=='n') break;
    cout <<"Vous devez entrer 'y' ou 'n'" << endl;
  }


  if (c=='y')
  {
    //CREER
    callCFE ();

    //REJOINDRE
    callJFE (argv[1]);

    //PUBLIER INTERACTION
    callPIC ();

    //SOUSCRIRE INTERACTION
    callSIC ();

    //DECLARER REGULATEUR ET CONTRAINT
    callETR ();
    callETC ();
  }

  //MENU PRINCIPAL

  while(1)
  {
      cout << endl <<"Choisissez une action : " << endl;
      cin >> commande;

      if (commande=="q" || commande=="quit") break;
      else if (commande=="h" || commande=="help") print_menu();
      else if (commande=="t") callTICK();
      else if (commande=="cfe") callCFE();
      else if (commande=="dfe") callDFE();
      else if (commande=="jfe") callJFE(argv[1]);
      else if (commande=="rfe") callRFE();
      else if (commande=="pic") callPIC();
      else if (commande=="sic") callSIC();
      else if (commande=="si") callSI();
      else if (commande=="etr") callETR();
      else if (commande=="etc") callETC();
      else if (commande=="tar") callTAR();
      else if (commande=="tara") callTARA();
      else if (commande=="ner") callNER();
      else if (commande=="nera") callNERA();
      else if (commande=="ead") callEAD();
      else if (commande=="dad") callDAD();
      else if (commande=="qlbts") callQLBTS();
      else if (commande=="qmnet") callQMNET();
      else if (commande=="ml") callML();
      else if (commande=="ql") callQL();
      else if (commande=="ciot") callCIOT();
      else cout << " commande inconnue " << endl;
  }

  exit(1);
}

void print_menu (void) {
   cout << "liste de commandes utilitaires :" << endl;
   cout << "t" << endl;
   cout << "liste de commandes Federation Management :" << endl;
   cout << "cfe dfe jfe rfe" << endl;
   cout << "liste de commandes Declaration Management :" << endl;
   cout << "pic sic" << endl;
   cout << "liste de commandes Object Management :" << endl;
   cout << "si" << endl;
   cout << "liste de commandes Time Management :" << endl;
   cout << "etr etc tar tara ner nera ead dad qlbts qmnet ml ql ciot" << endl;

}

void callTICK (void) {
    cout << endl << "t=" << temps->getTime() << " : -----> tick" << endl;
    myFedere_Interactif->tick(0.1, 0.2);
}

// FEDERARION MANAGEMENT
// createFederationExecution
void callCFE (void) {
    int test = 1;
    try {
       myFedere_Interactif->createFederationExecution("Federation_Interactive","Interactive_Federation.fed");
    }
    catch (RTI::Exception& e) {
        test = 0;
        std::cout << "catch " << e._name << " reason " << e._reason <<std::endl;
    }
    if (test)
       std::cout << "federation creee" << std::endl;
    else
       std::cout << "federation non creee" << std::endl;
}
// deleteFederationExecution
void callDFE (void) {
    int test = 1;
    try {
       myFedere_Interactif->destroyFederationExecution("Federation_Interactive");
    }
    catch (RTI::Exception& e) {
        test = 0;
        std::cout << "catch " << e._name << " reason " << e._reason <<std::endl;
    }
    if (test)
       std::cout << "federation detruite" << std::endl;
    else
       std::cout << "federation non detruite" << std::endl;
}
// joinFederationExecution
void callJFE (char *name) {
    int test = 1;
    try {
       myFedere_Interactif->joinFederationExecution(name, "Federation_Interactive", myFedere_Interactif);
    }
    catch (RTI::Exception& e) {
        test = 0;
        std::cout << "catch " << e._name << " reason " << e._reason <<std::endl;
    }
    if (test)
       std::cout << "federation rejointe" << std::endl;
    else
       std::cout << "federation non rejointe" << std::endl;
}
// resignFederationExecution
void callRFE (void) {
    int test = 1;
    try {
       myFedere_Interactif->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
    }
    catch (RTI::Exception& e) {
        test = 0;
        std::cout << "catch " << e._name << " reason " << e._reason <<std::endl;
    }
    if (test)
       std::cout << "federation quittee" << std::endl;
    else
       std::cout << "federation non quittee" << std::endl;
}
// DECLARATION MANAGEMENT
// publishInteractionClass
void callPIC (void) {
    int test = 1;
    try {
       myFedere_Interactif->publishInteractionClass(myFedere_Interactif->getInteractionClassHandle("msg"));
    }
    catch (RTI::Exception& e) {
        test = 0;
        std::cout << "catch " << e._name << " reason " << e._reason <<std::endl;
    }
    if (test)
       std::cout << "interaction publiee" << std::endl;
    else
       std::cout << "interaction non publiee" << std::endl;
}
// subscribeInteractionClass
void callSIC (void) {
    int test = 1;
    try {
       myFedere_Interactif->subscribeInteractionClass(myFedere_Interactif->getInteractionClassHandle("msg"), RTI::RTI_TRUE);
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << "interaction souscrite" << endl;
    else
       cout << "interaction non souscrite" << endl;
}
// OBJECT MANAGEMENT
// sendInteraction
void callSI (void) {
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la valeur de l'estampille voulue : ";
    cin >> d;
    try {
        myFedere_Interactif->sendInteraction(myFedere_Interactif->getInteractionClassHandle("msg"),
                                             *(RTI::ParameterSetFactory::create(0)),
                                             *(new RTIfedTime(d)),
                                             "msg");
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << "interaction envoyee (estampillee " << d << ")" << endl;
    else
       cout << "interaction non envoyee" << endl;
}

// TIME MANAGEMENT
// enableTimeRegulation
void callETR (void) {
    try {
       myFedere_Interactif->enableTimeRegulation(*temps,*lookahead);
    }
    catch (RTI::Exception& e) {
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    while (!timeRegulation)
    {
        myFedere_Interactif->tick(0.1, 0.2);
    }
    cout << "federe regulateur" << endl;
}
// enableTimeConstrained
void callETC (void) {
    try {
       myFedere_Interactif->enableTimeConstrained();
    }
    catch (RTI::Exception& e) {
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    while (!timeConstrained)
    {
        myFedere_Interactif->tick(0.1, 0.2);

    }
    cout << "federe contraint" << endl;
}
// timeAdvanceRequest
void callTAR (void) {
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
    cin >> d;
    try {
       myFedere_Interactif->timeAdvanceRequest(*(new RTIfedTime(d)));
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "t=" << temps->getTime() << " timeAdvanceRequest(" << d << ")" << endl;
    else
       cout << "timeAdvanceRequest a echoue" << endl;
}
// timeAdvanceRequestAvailable
void callTARA (void) {
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
    cin >> d;
    try {
       myFedere_Interactif->timeAdvanceRequestAvailable(*(new RTIfedTime(d)));
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "t=" << temps->getTime() << " timeAdvanceRequestAvailable(" << d << ")" << endl;
    else
       cout << "timeAdvanceRequestAvailable a echoue" << endl;
}
// nextEventRequest
void callNER (void) {
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
    cin >> d;
    try {
       myFedere_Interactif->nextEventRequest(*(new RTIfedTime(d)));
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "t=" << temps->getTime() << " nextEventRequest(" << d << ")" << endl;
    else
       cout << "nextEventRequest a echoue" << endl;
}
// nextEventRequestAvailable
void callNERA (void) {
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
    cin >> d;
    try {
       myFedere_Interactif->nextEventRequestAvailable(*(new RTIfedTime(d)));
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "t=" << temps->getTime() << " nextEventRequestAvailable(" << d << ")" << endl;
    else
       cout << "nextEventRequestAvailable a echoue" << endl;
}
// enableAsynchronousDelivery
void callEAD (void) {
    int test = 1;
    try {
       myFedere_Interactif->enableAsynchronousDelivery();
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << "asynchronous delivery enabled" << endl;
    else
       cout << "enableAsynchronousDelivery has failed" << endl;
}
// disableAsynchronousDelivery
void callDAD (void) {
    int test = 1;
    try {
       myFedere_Interactif->disableAsynchronousDelivery();
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << "asynchronous delivery disabled" << endl;
    else
       cout << "disableAsynchronousDelivery has failed" << endl;
}
// queryLBTS
void callQLBTS (void) {
    int test = 1;
    try {
       myFedere_Interactif->queryLBTS(ft);
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "LBTS=" << ft.getTime() << endl;
    else
       cout << "queryLBTS has failed" << endl;
}
// queryMinNextEventTime
void callQMNET (void) {
    int test = 1;
    try {
       myFedere_Interactif->queryMinNextEventTime(ft);
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "MinNextEventTime=" << ft.getTime() << endl;
    else
       cout << "queryMinNextEventTime has failed" << endl;
}
// modifyLookahead
void callML (void) {
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la valeur du lookahead voulu : ";
    cin >> d;
    try {
       myFedere_Interactif->modifyLookahead( *(new RTIfedTime(d)) );
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "t=" << temps->getTime() << " modifyLookahead(" << d << " )" << endl;
    else
       cout << "modifyLookahead has failed" << endl;
}
// queryLookahead
void callQL (void) {
    int test = 1;
    try {
       myFedere_Interactif->queryLookahead(ft);
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "Lookahead=" << ft.getTime() << endl;
    else
       cout << "queryLookahead has failed" << endl;
}
// changeInteractionOrderType
void callCIOT (void) {
    int test = 1;
    int type ;
    cout << "ordre RECEIVE (1) ou TIMESTAMP (2) ?" << endl ;
    cin >> type ;
    try {
       myFedere_Interactif->changeInteractionOrderType(myFedere_Interactif->getInteractionClassHandle("msg"),
                                                       type);
    }
    catch (RTI::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "Nouveau type d'ordre pour l'interaction = " << type << endl;
    else
       cout << "changeInteractionOrderType" << endl;
}

