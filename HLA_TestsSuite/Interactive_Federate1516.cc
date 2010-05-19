/*
 alias ; certi_run ; clear ;

 */



//#include <RTI/certiRTI1516.h>
#include <RTI/RTI1516.h>
#include <RTI/Enums.h>
#include <RTI/NullFederateAmbassador.h>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
using namespace std;

namespace rti1516
{
	class RTIambassador;
}

// VARIABLES GLOBALES



//RTIfedTime* temps = new RTIfedTime(0.0);
//RTIfedTime* lookahead = new RTIfedTime(1.0);
//RTIfedTime ft;

//RTIfedTime* temps = new RTIfedTime(0.0);


bool timeRegulation = false;
bool timeConstrained = false;


//Class Federe_Interactif
class Federe_Interactif : public rti1516::NullFederateAmbassador
{
	public:
		Federe_Interactif()
			throw ();
		~Federe_Interactif()
			throw ();

		void callambF();
		void callCFE();
		void callDFE();
		void callJFE(char *name);
		void callRFE();
		void callPIC();
		void callSIC();
		void callSI();

		void callCIOT();

	private:
		rti1516::RTIambassador *rtiAmb;
		rti1516::FederateHandle fedHandle;

	};


//méthode
Federe_Interactif::Federe_Interactif() throw()
		: rtiAmb(0)
		, fedHandle()
{

}

//méthode
Federe_Interactif::~Federe_Interactif() throw()
{
	delete rtiAmb;
}


//Prototypes
void print_menu (void);


//VARIABLE
Federe_Interactif* myInteractifFederate = new Federe_Interactif();


int main(int argc, char **argv)
{

  int i =0;//compteur
  float d;
  char c; //entrée clavier (préambule automatique)
  string commande; //comande entrée pour le test

  if (argc!=2)
  {
    cout <<"Vous devez donner en parametre le nom du federe" << endl;
    exit(1);
  }

  while(1)
  {
    cout <<"Voulez-vous un preambule automatique [y/n]" << endl;
    cin >> c;
    if (c=='y' || c=='n') break;
    cout <<"Vous devez entrer 'y' ou 'n'" << endl;
  }

  if (c=='y')
  {
	//Ambassador
	//myInteractifFederate->callambF();

    //CREER
    //callCFE ();
	myInteractifFederate->callCFE();

    //REJOINDRE
	myInteractifFederate->callJFE(argv[1]);

    //PUBLIER INTERACTION
	myInteractifFederate->callPIC();

    //SOUSCRIRE INTERACTION
	myInteractifFederate->callSIC();

    //DECLARER REGULATEUR ET CONTRAINT
    //callETR ();
    //callETC ();
  }

  //MENU PRINCIPAL

  while(1)
  {
	  i++;
      cout << endl <<"Choisissez une action : " << endl;
      cin >> commande;

      if (commande=="q" || commande=="quit") break;
      else if (commande=="h" || commande=="help") print_menu();
      //else if (commande=="t") callTICK();
      else if (commande=="amb") myInteractifFederate->callambF();
      else if (commande=="cfe") myInteractifFederate->callCFE();
      else if (commande=="dfe") myInteractifFederate->callDFE();
      else if (commande=="jfe") myInteractifFederate->callJFE(argv[1]);
      else if (commande=="rfe") myInteractifFederate->callRFE();
      else if (commande=="pic") myInteractifFederate->callPIC();
      else if (commande=="sic") myInteractifFederate->callSIC();
      else if (commande=="si") myInteractifFederate->callSI();
      //else if (commande=="etr") callETR();
      //else if (commande=="etc") callETC();
      //else if (commande=="tar") callTAR();
      //else if (commande=="tara") callTARA();
      //else if (commande=="ner") callNER();
      //else if (commande=="nera") callNERA();
      //else if (commande=="ead") callEAD();
      //else if (commande=="dad") callDAD();
      //else if (commande=="qlbts") callQLBTS();
      //else if (commande=="qmnet") callQMNET();
      //else if (commande=="ml") callML();
      //else if (commande=="ql") callQL();
      //else if (commande=="ciot") myInteractifFederate->callCIOT();
      else cout << " commande inconnue " << endl;
  }

  exit(1);
}

void print_menu (void)
{
   cout << "**********************************************\n           liste de commandes :\n**********************************************" << endl;
   cout << "*                                            *" << endl;
   cout << "*liste de commandes utilitaires :            *" << endl;
   cout << "*   t                                        *" << endl;
   cout << "*liste de commandes Federation Management :  *" << endl;
   cout << "*   amb ; cfe ; dfe ; jfe ; rfe              *" << endl;
   cout << "*liste de commandes Declaration Management : *" << endl;
   cout << "*   pic ; sic                                *" << endl;
   cout << "*liste de commandes Object Management :      *" << endl;
   cout << "*   si                                       *" << endl;
   cout << "*liste de commandes Time Management :        *" << endl;
   cout << "*   etr ; etc ; tar ; tara ; ner ; nera      *" << endl;
   cout << "* ead ; dad ; qlbts ; qmnet ; ml ; ql ; ciot *" << endl;
   cout << "**********************************************" << endl;
}

//commande utilitaire
/*void callTICK (void)
{
    cout << endl << "t=" << temps->getTime() << " : -----> tick" << endl;
    myFedere_Interactif->tick(0.1, 0.2);
}*/


// FEDERARION MANAGEMENT
//createAmbassador
void Federe_Interactif::callambF()
{
	bool test = true;



    std::auto_ptr< rti1516::RTIambassadorFactory > rtiAmbFact(new rti1516::RTIambassadorFactory());
    std::vector< std::wstring > constructorArgs;
/*
//**********************



    std::auto_ptr<rti1516::LogicalTimeFactory> time = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"CERTI");
    time->makeLogicalTime();






//**************************
*/
	    try
	    {
			std::auto_ptr< rti1516::RTIambassador > rtiAmbP(rtiAmbFact->createRTIambassador(constructorArgs));
			rtiAmb = rtiAmbP.release();
		}
	    catch (rti1516::RTIinternalError &e)
	    {
		    test = false;
		}
	    catch (rti1516::Exception &e)
	    {
	    	test = false;
	    }

	    if (test)
	       std::cout << "Ambassador create" << std::endl;
	    else
	       std::cout << "Error creating ambassador" << std::endl;
}

//createFederationExecution
void Federe_Interactif::callCFE()
{
	myInteractifFederate->callambF();
	bool test = true;

    try
    {
    	rtiAmb->createFederationExecution(L"Federation_Interactive", L"Certi-Test-02.xml");
	}

    catch (rti1516::Exception &e)
    {
    	test = false;
    	std::wcerr << L"Error : "<<  e.what() << std::endl;
    	//std::cout << "catch " << e._name << " reason " << e._reason <<std::endl;
    }

    if (test)
       std::cout << "Federation créée" << std::endl;
    else
       std::cout << "Federation non créée" << std::endl;
}

//deleteFederationExecution
void Federe_Interactif::callDFE()
{
	int test = 0;
	try
	{
			rtiAmb->destroyFederationExecution(L"Federation_Interactive");
	}
	catch (rti1516::FederationExecutionDoesNotExist &e)
	{
			test =1;
			//cout << "No federation to destroy" <<endl;
	}
	catch (rti1516::Exception &e)
	{
			test = 2;
			//cout << "Error destroying federation" <<endl;
	}


	if (test==0)
		cout << "Fédération détruite" <<endl;
	else if (test==1)
		cout << "No federation to destroy" <<endl;
	else
		cout << "Error destroying federation" <<endl;
}

// joinFederationExecution
void Federe_Interactif::callJFE(char *name)
{
	bool test = true;

	// char => wstring
	std::wstring wname(name,name+strlen(name));


	try
	{
		rtiAmb->joinFederationExecution(wname, L"Federation_Interactive", *myInteractifFederate);
	}
	catch (rti1516::Exception &e)
	{
		test = false;
	}

    if (test)
       std::cout << "Federation rejointe" << std::endl;
    else
       std::cout << "Federation non rejointe" << std::endl;

}

// resignFederationExecution
void Federe_Interactif::callRFE()
{
	bool test = true;
	try
	{
		rtiAmb->resignFederationExecution(rti1516::CANCEL_THEN_DELETE_THEN_DIVEST);
	}
	catch (rti1516::Exception &e)
	{
		test = false;
	}

    if (test)
       std::cout << "federation quittee" << std::endl;
    else
       std::cout << "federation non quittee" << std::endl;

}

// DECLARATION MANAGEMENT
// publishInteractionClass
void Federe_Interactif::callPIC ()
{

	//rti1516::InteractionClassHandle msgHandle;
	//msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
	bool test = true;
    try
    {
    	rtiAmb->publishInteractionClass(rtiAmb->getInteractionClassHandle(L"Msg"));
       //rtiAmb->publishInteractionClass(msgHandle);
    }

    catch (rti1516::Exception &e)
    {
    	test = false;
    }

    if (test)
       std::cout << "interaction publiee" << std::endl;
    else
       std::cout << "interaction non publiee" << std::endl;
}


// subscribeInteractionClass
void Federe_Interactif::callSIC ()
{
	//rti1516::InteractionClassHandle msgHandle;
	//msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
	bool test = true;
    try
    {
    	rtiAmb->subscribeInteractionClass(rtiAmb->getInteractionClassHandle(L"Msg"));
    }

    catch (rti1516::Exception &e)
    {
    	test = false;
    }

    if (test)
       std::cout << "interaction souscrite" << std::endl;
    else
       std::cout << "interaction non souscrite" << std::endl;
}

// OBJECT MANAGEMENT
// sendInteraction
void Federe_Interactif::callSI ()
{
    float d;
    bool test = true;


  //  std::auto_ptr<rti1516::LogicalTimeFactory> temps = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"CERTI");
 //   temps->makeLogicalTime();



   // cout << "t=" << temps->toString() << endl;

    /*
    cout << endl << "t=" << time->getTime() << " : Donner la valeur de l'estampille voulue : ";
    cin >> d;

    try {
    	rtiAmb->sendInteraction(rtiAmb->getInteractionClassHandle(L"Msg"),
                                             *(rti1516::ParameterSetFactory::create(0)),
                                             *(new RTIfedTime(d)),
                                             "msg");
    }

    catch (rti1516::Exception &e)
    {
        test = false;
    }

    if (test)
       cout << "interaction envoyee (estampillee " << d << ")" << endl;
    else
       cout << "interaction non envoyee" << endl;
*/

}

// TIME MANAGEMENT
/*// enableTimeRegulation
void callETR (void)
{
    try {
       myFedere_Interactif->enableTimeRegulation(*temps,*lookahead);
    }
    catch (rti1516::Exception& e) {
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    while (!timeRegulation)
    {
        myFedere_Interactif->tick(0.1, 0.2);
    }
    cout << "federe regulateur" << endl;
}*/

/*// enableTimeConstrained
void callETC (void)
{
    try {
       myFedere_Interactif->enableTimeConstrained();
    }
    catch (rti1516::Exception& e) {
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    while (!timeConstrained)
    {
        myFedere_Interactif->tick(0.1, 0.2);

    }
    cout << "federe contraint" << endl;
}*/

/*// timeAdvanceRequest
void callTAR (void) {
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
    cin >> d;
    try {
       myFedere_Interactif->timeAdvanceRequest(*(new RTIfedTime(d)));
    }
    catch (rti1516::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "t=" << temps->getTime() << " timeAdvanceRequest(" << d << ")" << endl;
    else
       cout << "timeAdvanceRequest a echoue" << endl;
}*/

/*// timeAdvanceRequestAvailable
void callTARA (void) {
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
    cin >> d;
    try {
       myFedere_Interactif->timeAdvanceRequestAvailable(*(new RTIfedTime(d)));
    }
    catch (rti1516::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "t=" << temps->getTime() << " timeAdvanceRequestAvailable(" << d << ")" << endl;
    else
       cout << "timeAdvanceRequestAvailable a echoue" << endl;
}*/

/*// nextEventRequest
void callNER (void)
{
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
    cin >> d;
    try {
       myFedere_Interactif->nextEventRequest(*(new RTIfedTime(d)));
    }
    catch (rti1516::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "t=" << temps->getTime() << " nextEventRequest(" << d << ")" << endl;
    else
       cout << "nextEventRequest a echoue" << endl;
}*/

/*// nextEventRequestAvailable
void callNERA (void)
{
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la date a laquelle vous souhaitez avancer : ";
    cin >> d;
    try {
       myFedere_Interactif->nextEventRequestAvailable(*(new RTIfedTime(d)));
    }
    catch (rti1516::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "t=" << temps->getTime() << " nextEventRequestAvailable(" << d << ")" << endl;
    else
       cout << "nextEventRequestAvailable a echoue" << endl;
}*/

/*// enableAsynchronousDelivery
void callEAD (void)
{
    int test = 1;
    try {
       myFedere_Interactif->enableAsynchronousDelivery();
    }
    catch (rti1516::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << "asynchronous delivery enabled" << endl;
    else
       cout << "enableAsynchronousDelivery has failed" << endl;
}*/

/*// disableAsynchronousDelivery
void callDAD (void)
{
    int test = 1;
    try {
       myFedere_Interactif->disableAsynchronousDelivery();
    }
    catch (rti1516::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << "asynchronous delivery disabled" << endl;
    else
       cout << "disableAsynchronousDelivery has failed" << endl;
}*/

/*// queryLBTS
void callQLBTS (void)
{
    int test = 1;
    try {
       myFedere_Interactif->queryLBTS(ft);
    }
    catch (rti1516::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "LBTS=" << ft.getTime() << endl;
    else
       cout << "queryLBTS has failed" << endl;
}*/

/*// queryMinNextEventTime
void callQMNET (void)
{
    int test = 1;
    try {
       myFedere_Interactif->queryMinNextEventTime(ft);
    }
    catch (rti1516::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "MinNextEventTime=" << ft.getTime() << endl;
    else
       cout << "queryMinNextEventTime has failed" << endl;
}*/

/*// modifyLookahead
void callML (void)
{
    float d;
    int test = 1;
    cout << endl << "t=" << temps->getTime() << " : Donner la valeur du lookahead voulu : ";
    cin >> d;
    try {
       myFedere_Interactif->modifyLookahead( *(new RTIfedTime(d)) );
    }
    catch (rti1516::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "t=" << temps->getTime() << " modifyLookahead(" << d << " )" << endl;
    else
       cout << "modifyLookahead has failed" << endl;
}*/

/*// queryLookahead
void callQL (void)
{
    int test = 1;
    try {
       myFedere_Interactif->queryLookahead(ft);
    }
    catch (rti1516::Exception& e) {
        test = 0;
        cout << "catch " << e._name << " reason " << e._reason << endl;
    }
    if (test)
       cout << endl << "Lookahead=" << ft.getTime() << endl;
    else
       cout << "queryLookahead has failed" << endl;
}*/

/*// changeInteractionOrderType
void Federe_Interactif::callCIOT ()
{
    int test = 1;
    int type ;
    cout << "ordre RECEIVE (1) ou TIMESTAMP (2) ?" << endl ;
    cin >> type ;

    try
    {
    	rtiAmb->changeInteractionOrderType(rtiAmb->getInteractionClassHandle(L"Msg"), type);
    }

    catch (rti1516::Exception& e)
    {
        test = 0;
    }

    if (test)
       cout << endl << "Nouveau type d'ordre pour l'interaction = " << type << endl;
    else
       cout << "changeInteractionOrderType" << endl;
}*/















