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
#include <sstream>
using namespace std;

namespace rti1516
{
	class RTIambassador;
}

// VARIABLES GLOBALES

//temps
std::auto_ptr<rti1516::LogicalTimeFactory> temps = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"CERTI");
std::auto_ptr< rti1516::LogicalTime > tps(temps->makeLogicalTime());

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

		void callGetObjClassHandle();
		void callGetObjClassName();
		void callGetAttrHandle();
		void callGetAttrHandleName();
		void callGetIntClassHandle();
		void callGetIntClassName();
		void callGetParaHandle();
		void callGetParaHandleName();
		void callGetObjInstanceHandle();
		void callGetObjInstanceHandleName();
		void callambF();
		void callCFE();
		void callDFE();
		void callJFE(char *name);
		void callRFE();
		void callPOCA();
		void callUOC();
		void callUOCA();
		void callPIC();
		void callUIC();
		void callSOCA();
		void callUsubOC();
		void callUsubOCA();
		void callSIC();
		void callUsubIC();
		void callROI();
		void callUAV();
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
      else if (commande=="getoch") myInteractifFederate->callGetObjClassHandle();
      else if (commande=="getocn") myInteractifFederate->callGetObjClassName();
      else if (commande=="getah") myInteractifFederate->callGetAttrHandle();
      else if (commande=="getahn") myInteractifFederate->callGetAttrHandleName();
      else if (commande=="getich") myInteractifFederate->callGetIntClassHandle();
      else if (commande=="geticn") myInteractifFederate->callGetIntClassName();
      else if (commande=="getph") myInteractifFederate->callGetParaHandle();
      else if (commande=="getphn") myInteractifFederate->callGetParaHandleName();
      else if (commande=="getoih") myInteractifFederate->callGetObjInstanceHandle();
      else if (commande=="getoihn") myInteractifFederate->callGetObjInstanceHandleName();
      else if (commande=="amb") myInteractifFederate->callambF();
      else if (commande=="cfe") myInteractifFederate->callCFE();
      else if (commande=="dfe") myInteractifFederate->callDFE();
      else if (commande=="jfe") myInteractifFederate->callJFE(argv[1]);
      else if (commande=="rfe") myInteractifFederate->callRFE();
      else if (commande=="poca") myInteractifFederate->callPOCA();
      else if (commande=="uoc") myInteractifFederate->callUOC();
      else if (commande=="uoca") myInteractifFederate->callUOCA();
      else if (commande=="pic") myInteractifFederate->callPIC();
      else if (commande=="uic") myInteractifFederate->callUIC();
      else if (commande=="soca") myInteractifFederate->callSOCA();
      else if (commande=="usuboc") myInteractifFederate->callUsubOC(); //*
      else if (commande=="usuboca") myInteractifFederate->callUsubOCA(); //*
      else if (commande=="sic") myInteractifFederate->callSIC();
      else if (commande=="usubic") myInteractifFederate->callUsubIC(); //*
      else if (commande=="roi") myInteractifFederate->callROI(); //*
      else if (commande=="uav") myInteractifFederate->callUAV(); //*
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
	bool menu = true;
	while (menu)
	{
		int choix;
		cout <<"\t0- Quitter"<< endl;
		cout <<"\t1- Commandes utilitaires"<< endl;
		cout <<"\t2- Federation Management"<< endl;
		cout <<"\t3- Declaration Management"<< endl;
		cout <<"\t4- Object Management"<< endl;
		cout <<"\t5- Time Management\n"<< endl;
		cout <<"votre choix : ";
		cin >> choix;
		cout <<"\n\n-------------------------------------\n";

		switch (choix)
		{
			case 0 :
				menu = false;
			break;

			case 1 :
				cout <<"\t1- Commandes utilitaires\n"<< endl;
				cout <<"t       => tick"<< endl;
				cout <<"getoch  => Get Object Class Handle"<< endl;
				cout <<"getocn  => Get Object Class Name"<< endl;
				cout <<"getah   => Get Attribute Handle"<< endl;
				cout <<"getahn  => Get Attribute Handle Name"<< endl;
				cout <<"getich  => Get Interaction Class Handle"<< endl;
				cout <<"geticn  => Get Interaction Class Name"<< endl;
				cout <<"getph   => Get Parameter Handle"<< endl;
				cout <<"getphn  => Get Parameter Handle Name"<< endl;
				cout <<"getoih  => Get Object Instance Handle"<< endl;
				cout <<"getoihn => Get Object Instance Handle Name\n\n"<< endl;
			break;

			case 2 :
				cout <<"\t2- Federation Management\n"<< endl;
				cout <<"amb => create ambassador"<< endl;
				cout <<"cfe => create federation execution"<< endl;
				cout <<"dfe => destroy federation execution"<< endl;
				cout <<"jfe => join federation execution"<< endl;
				cout <<"rfe => resign federation execution\n\n"<< endl;
			break;

			case 3 :
				cout <<"\t3- Declaration Management\n"<< endl;
				cout <<"poca    => Publish Object Class Attributes"<< endl;
				cout <<"uoc     => Unpublish Object Class"<< endl;
				cout <<"uoca    => Unpublish Object Class Attributes"<< endl;
				cout <<"pic     => Publish Interaction Class"<< endl;
				cout <<"usuboc  => Unsubscribe Object Class"<< endl;
				cout <<"usuboca => Unsubscribe Object Class Attributes"<< endl;
				cout <<"usubic  => Unsubscribe Interaction Class"<< endl;
				cout <<"sic     => Suscribe Interaction Class\n\n"<< endl;
			break;

			case 4 :
				cout <<"\t4- Object Management\n"<< endl;

				cout <<"si  => Send Interaction"<< endl;
				cout <<"roi => Register Object Instance"<< endl;
				cout <<"uav => Update Attribute Values\n\n"<< endl;
			break;

			case 5 :
				cout <<"\t5- Time Management\n"<< endl;
				cout <<"etr   => tick"<< endl;
				cout <<"etc   => Get Object Class Handle"<< endl;
				cout <<"tar   => Get Object Class Name"<< endl;
				cout <<"tara  => Get Attribute Handle"<< endl;
				cout <<"ner   => Get Attribute Handle Name"<< endl;
				cout <<"nera  => Get Interaction Class Handle"<< endl;
				cout <<"ead   => Get Interaction Class Name"<< endl;
				cout <<"dad   => Get Parameter Handle"<< endl;
				cout <<"glbts => Get Parameter Handle Name"<< endl;
				cout <<"gmnet => Get Object Instance Handle"<< endl;
				cout <<"ml    => Get Object Instance Handle Name\n\n"<< endl;
				cout <<"gl    => Get Parameter Handle"<< endl;
				cout <<"ciot  => Get Parameter Handle Name"<< endl;
			break;

		default :
			cout <<"\tEntrée invalide\n"<< endl;


		}//fin switch
	}//fin while
}//fin print_menu

/*************************************************************************************************
********************************** COMMANDES UTILITAIRE *******************************************
*************************************************************************************************/

/*void callTICK (void)
{
    cout << endl << "t=" << temps->getTime() << " : -----> tick" << endl;
    myFedere_Interactif->tick(0.1, 0.2);
}*/

//getObjectClassHandle
void Federe_Interactif::callGetObjClassHandle()
{
	cout <<"=>Get Object Class Handle"<< endl;

	try
	{
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");

	    if (classHandle.isValid())
	       std::cout << "* Got class handle\n" << std::endl;
	    else
	       std::cout << "* Fail: Got invalid class handle\n" << std::endl;
	}

	catch (rti1516::Exception &e)
	{
		std::cout << "* Erreur" << std::endl;
	}
}

//getObjectClassName
void Federe_Interactif::callGetObjClassName()
{
	cout <<"=>Get Object Class Name"<< endl;

	try
	{
		std::wstring getClassName(L"TestObjectClass");
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(getClassName);
		cout << "\t-> getObjectClassHandle\n" << endl;

		std::wstring returnedClassName = rtiAmb->getObjectClassName(classHandle);


		if ( getClassName != returnedClassName )
		{
			std::wcerr << L"* Fail: Returned classname does not match expected classname" << std::endl;
		}
		else
		{
			std::wcerr << L"* Got expected classname : ";
			std::wcerr << returnedClassName << endl;
		}
	}

		catch (rti1516::Exception &e)
		{
			std::cout << "* Erreur" << std::endl;
		}
}

//getAttributeHandle
void Federe_Interactif::callGetAttrHandle()
{
	cout <<"=>getAttributeHandle"<< endl;
	try
	{
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		cout << "\t-> getObjectClassHandle\n" << endl;

		rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");

		if ( classHandle.isValid() )
		{
			cout << "* Got attribute handle" << endl;
		}

		else
		{
			cout << "* Fail: Got invalid class handle" << endl;
		}
	}

	catch (rti1516::Exception &e)
	{
		std::cout << "* Erreur" << std::endl;
	}
}
//getAttributeHandleName
void Federe_Interactif::callGetAttrHandleName()
{
	cout <<"=>getAttributeHandleName"<< endl;
	try
	{
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		std::wstring orgAttrName(L"Data");
		rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, orgAttrName);
		std::wstring getattrName = rtiAmb->getAttributeName(classHandle, attrHandle);

		cout << "\t-> getObjectClassHandle" << endl;
		cout << "\t-> getAttributeHandle" << endl;
		cout << "\t-> getAttributeName\n" << endl;

		if ( orgAttrName == getattrName )
		{
			cout << "* Got attribute handle name : " ;
			std::wcerr << getattrName << endl;
		}

		else
		{
			cout << "* Got invalib attribute handle name" << endl;
		}

	}

	catch (rti1516::Exception &e)
	{
		std::cout << "Erreur" << std::endl;
	}
}

//getInteractionClassHandle
void Federe_Interactif::callGetIntClassHandle()
{
	cout <<"=>getInteractionClassHandle"<< endl;
	try
	{
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");

		if (msgHandle.isValid())
		{
			cout << "* Got interaction class handle" << endl;
		}

		else
		{
			cout << "* Got invalid interaction class handle" << endl;
		}

	}
	catch (rti1516::Exception &e)
	{
		std::cout << "Erreur" << std::endl;
	}
}

//getInteractionClassName
void Federe_Interactif::callGetIntClassName()
{
	cout <<"=>getInteractionClassName"<< endl;
	try
	{
		std::wstring orgMsgName(L"Msg");
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(orgMsgName);
		cout << "\t-> getInteractionClassHandle\n" << endl;

		std::wstring getMsgName = rtiAmb->getInteractionClassName(msgHandle);

        if ( orgMsgName == getMsgName )
        {
			cout << "* Got interaction class name : ";
			std::wcerr << getMsgName << endl;
		}

        else
		{
        	cout << "* Got invalid interaction class name" << endl;
        }

	}

	catch (rti1516::Exception &e)
	{
		std::cout << "* Erreur" << std::endl;
	}
}

//getParameterHandle
void Federe_Interactif::callGetParaHandle()
{
	cout <<"=>getParameterHandle"<< endl;
	try
	{
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
		cout << "\t-> getInteractionClassHandle\n" << endl;

		rti1516::ParameterHandle paraHandle = rtiAmb->getParameterHandle(msgHandle, L"MsgData");

		if ( paraHandle.isValid() )
		{
			cout << "* Got parameter handle"<< endl;
		}

		else
		{
			cout << "* Got invalid parameter handle"<< endl;
		}

	}
	catch (rti1516::Exception &e)
	{
		std::cout << "* Erreur" << std::endl;
	}
}
//getParameterHandleName
void Federe_Interactif::callGetParaHandleName()
{
	cout <<"=>getParameterHandleName"<< endl;
	try
	{
		rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
		cout << "\t-> getInteractionClassHandle" << endl;
		rti1516::ParameterHandle parHandle = rtiAmb->getParameterHandle(msgHandle, L"MsgData");
		cout << "\t-> getParameterHandle\n" << endl;
		std::wstring orgParamName(L"MsgData");

		std::wstring getParamName = rtiAmb->getParameterName(msgHandle, parHandle);

		if ( orgParamName == getParamName )
		{
			cout << "* Got parameter handle name : ";
			std::wcerr << getParamName << endl;
		}

		else
		{
			cout << "* Got invalid parameter handle name "<< endl;

		}
	}
	catch (rti1516::Exception &e)
	{
		std::cout << "* Erreur" << std::endl;
	}
}

//getObjectInstanceHandle
void Federe_Interactif::callGetObjInstanceHandle()
{
	cout <<"=>getObjectInstanceHandle"<< endl;
	int nameVal;
	nameVal = (rand() % 99) + 1;
	std::wstringstream stream;
	stream << L"TestObject" << nameVal;
	std::wstring objName = stream.str();
	rti1516::ObjectInstanceHandle objHandle;
	bool objRegistered = false;



	try
	{
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		cout << "\t-> getObjectClassHandle" << endl;
		rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
		cout << "\t-> getAttributeHandle" << endl;
		rti1516::AttributeHandleSet attrSet;
		attrSet.insert(attrHandle);
		rtiAmb->publishObjectClassAttributes(classHandle, attrSet);
		cout << "\t-> publishObjectClassAttributes" << endl;
		objHandle = rtiAmb->registerObjectInstance(classHandle, objName);
		cout << "\t-> registerObjectInstance\n" << endl;
		objRegistered = true;
	}
	catch (rti1516::Exception &e)
	{
		cout << "* Could not register object instance "<< endl;
	}

	try
	{
		if ( objRegistered )
		{
			rti1516::ObjectInstanceHandle returnedObjHandle = rtiAmb->getObjectInstanceHandle(objName);

			if ( returnedObjHandle.isValid() )
			{
				cout << "* Got object handle "<< endl;
			}
			else
			{
				cout << "* Got invalid object handle "<< endl;
			}
		}
		else
		{
			cout << "* Object not registered "<< endl;
		}
	}

	catch (rti1516::Exception &e)
	{
		std::cout << "* Erreur" << std::endl;
	}
}

//getObjectInstanceName
void Federe_Interactif::callGetObjInstanceHandleName()
{
	cout <<"=>getObjectInstanceName"<< endl;
	int nameVal;
	nameVal = (rand() % 99) + 1;
	std::wstringstream stream;
	stream << L"TestObject" << nameVal;
	std::wstring objName = stream.str();
	rti1516::ObjectInstanceHandle objHandle;
	bool objRegistered = false;

	try
	{
		rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
		cout << "\t-> getObjectClassHandle" << endl;
		rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
		cout << "\t-> getAttributeHandle" << endl;
		rti1516::AttributeHandleSet attrSet;
		attrSet.insert(attrHandle);
		rtiAmb->publishObjectClassAttributes(classHandle, attrSet);
		cout << "\t-> publishObjectClassAttributes" << endl;
		objHandle = rtiAmb->registerObjectInstance(classHandle, objName);
		cout << "\t-> registerObjectInstance\n" << endl;
		objRegistered = true;
	}
	catch (rti1516::Exception &e)
	{
		cout << "* Could not register object instance "<< endl;
	}

	try
	{
			if ( objRegistered )
			{
				std::wstring returnedObjName = rtiAmb->getObjectInstanceName(objHandle);

				if ( objName == returnedObjName )
				{
					cout << "* Got object handle name : ";
					std::wcerr << returnedObjName << endl;
				}
				else
				{
					cout << "* Got invalid object handle name "<< endl;
				}
			}
			else
			{
				cout << "* Object not registered "<< endl;
			}
		}
	catch (rti1516::Exception &e)
	{
		std::cout << "* Erreur" << std::endl;
	}
}
/*************************************************************************************************
********************************* FEDERARION MANAGEMENT ******************************************
*************************************************************************************************/

//createAmbassador
void Federe_Interactif::callambF()
{
	bool test = true;



    std::auto_ptr< rti1516::RTIambassadorFactory > rtiAmbFact(new rti1516::RTIambassadorFactory());
    std::vector< std::wstring > constructorArgs;

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
	    {
	    	cout <<"\t->createAmbassador"<< endl;
	    }
	    else
	    {
	    	cout <<"\t->createAmbassador"<< endl;
			std::cout << "* Error creating ambassador" << std::endl;
	    }
}

//createFederationExecution
void Federe_Interactif::callCFE()
{
	cout <<"=>createFederationExecution"<< endl;
	//création de l'ambassador
	myInteractifFederate->callambF();
	bool test = true;

    try
    {
    	rtiAmb->createFederationExecution(L"Federation_Interactive", L"Certi-Test-02.xml");
	}

    catch (rti1516::Exception &e)
    {
    	test = false;
    	std::wcerr << L"* Error : "<<  e.what() << std::endl;
    }

    if (test)
       std::cout << "* Federation créée\n" << std::endl;
    else
       std::cout << "* Federation non créée\n" << std::endl;
}

//deleteFederationExecution
void Federe_Interactif::callDFE()
{
	cout <<"=>deleteFederationExecution"<< endl;
	int test = 0;
	try
	{
			rtiAmb->destroyFederationExecution(L"Federation_Interactive");
	}
	catch (rti1516::FederationExecutionDoesNotExist &e)
	{
			test =1;
	}
	catch (rti1516::Exception &e)
	{
			test = 2;
	}


	if (test==0)
		cout << "* Fédération détruite\n" <<endl;
	else if (test==1)
		cout << "* No federation to destroy\n" <<endl;
	else
		cout << "* Error destroying federation\n" <<endl;
}

// joinFederationExecution
void Federe_Interactif::callJFE(char *name)
{
	cout <<"=>joinFederationExecution"<< endl;
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
       std::cout << "* Federation rejointe\n" << std::endl;
    else
       std::cout << "* Federation non rejointe\n" << std::endl;

}

// resignFederationExecution
void Federe_Interactif::callRFE()
{
	cout <<"=>resignFederationExecution"<< endl;
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
       std::cout << "* federation quittee\n" << std::endl;
    else
       std::cout << "* federation non quittee\n" << std::endl;

}

/*************************************************************************************************
********************************* DECLARATION MANAGEMENT *****************************************
*************************************************************************************************/

// publishObjectClassAttributes
void Federe_Interactif::callPOCA ()
{
	cout <<"=>publishObjectClassAttributes"<< endl;
	bool test = true;

	rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
	cout << "\t-> getObjectClassHandle" << endl;
	rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
	cout << "\t-> getAttributeHandle\n" << endl;
	rti1516::AttributeHandleSet classAttributes;
	classAttributes.insert(attrHandle);

	try
	{
		rtiAmb->publishObjectClassAttributes(classHandle, classAttributes);
	}

	catch (rti1516::Exception &e)
	{
		test = false;
	}

    if (test)
       std::cout << "* OK\n" << std::endl;
    else
       std::cout << "* Pas OK\n" << std::endl;
}

//unpublishObjectClass
void Federe_Interactif::callUOC ()
{
	cout <<"=>unpublishObjectClass"<< endl;
	bool test = true;

	rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
	cout << "\t-> getObjectClassHandle\n" << endl;

	try
	{
		rtiAmb->unpublishObjectClass(classHandle);
	}
	catch (rti1516::Exception &e)
	{
		test = false;
	}

    if (test)
       std::cout << "* OK\n" << std::endl;
    else
       std::cout << "* Pas OK\n" << std::endl;
}

//unpublishObjectClassAttributes
//PB A REVOIR !!!
void Federe_Interactif::callUOCA ()
{
	cout <<"=>unpublishObjectClassAttributes"<< endl;
	bool test = true;

	rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
	cout << "\t-> getObjectClassHandle" << endl;
	rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
	cout << "\t-> getAttributeHandle\n" << endl;
	rti1516::AttributeHandleSet classAttributes;
	classAttributes.insert(attrHandle);

	try
	{
		rtiAmb->unpublishObjectClassAttributes(classHandle, classAttributes);
	}
	catch (rti1516::Exception &e)
	{
		test = false;
	}

    if (test)
       std::cout << "* OK\n" << std::endl;
    else
       std::cout << "* Pas OK\n" << std::endl;
}

// publishInteractionClass
void Federe_Interactif::callPIC ()
{
	cout <<"=>publishInteractionClass"<< endl;
	bool test = true;
    try
    {
    	rtiAmb->publishInteractionClass(rtiAmb->getInteractionClassHandle(L"Msg"));
    }

    catch (rti1516::Exception &e)
    {
    	test = false;
    }

    if (test)
       std::cout << "* interaction publiee\n" << std::endl;
    else
       std::cout << "* interaction non publiee\n" << std::endl;
}

// unpublishInteractionClass
void Federe_Interactif::callUIC ()
{
	cout <<"=>unpublishInteractionClass"<< endl;
	bool test = true;
	rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
	cout << "\t-> getInteractionClassHandle\n" << endl;

	try
	{
		rtiAmb->unpublishInteractionClass(msgHandle);
	}
	catch (rti1516::Exception &e)
	{
		test = false;
	}

	if (test)
		std::cout << "* OK" << std::endl;
	else
		std::cout << "* PAS OK" << std::endl;
}
// subscribeObjectClassAttributes
void Federe_Interactif::callSOCA ()
{
	cout <<"=>subscribeObjectClassAttributes"<< endl;
	bool test = true;

	rti1516::AttributeHandleSet classAttributes;
	rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
	cout << "\t-> getObjectClassHandle" << endl;
	rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
	cout << "\t-> getAttributeHandle\n" << endl;
	classAttributes.insert(attrHandle);


	try
	{
		rtiAmb->subscribeObjectClassAttributes(classHandle, classAttributes);
	}
	catch (rti1516::Exception &e)
	{
		test = false;
	}

	if (test)
		std::cout << "* OK\n" << std::endl;
	else
		std::cout << "* PAS OK\n" << std::endl;
}

// unsubscribeObjectClass
void Federe_Interactif::callUsubOC ()
{
	cout <<"=>unsubscribeObjectClass"<< endl;
	bool test = true;

	rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
	cout << "\t-> getObjectClassHandle\n" << endl;

	try
	{
		rtiAmb->unsubscribeObjectClass(classHandle);
	}
	catch (rti1516::Exception &e)
	{
		test = false;
	}

if (test)
	std::cout << "* OK\n" << std::endl;
else
	std::cout << "* PAS OK\n" << std::endl;
}

// unsubscribeObjectClassAttributes
void Federe_Interactif::callUsubOCA ()
{
	cout <<"=>unsubscribeObjectClassAttributes"<< endl;
	bool test = true;

	rti1516::AttributeHandleSet classAttributes;
	rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
	cout << "\t-> getObjectClassHandle" << endl;
	rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
	cout << "\t-> getObjectClassHandle\n" << endl;
	classAttributes.insert(attrHandle);

	try
	{
	rtiAmb->unsubscribeObjectClassAttributes(classHandle, classAttributes);
	}
	catch (rti1516::Exception &e)
	{
		test = false;
	}

	if (test)
			std::cout << "* OK\n" << std::endl;
	else
		std::cout << "* PAS OK\n" << std::endl;
}

// subscribeInteractionClass
void Federe_Interactif::callSIC ()
{
	cout <<"=>subscribeInteractionClass"<< endl;
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
       std::cout << "* interaction souscrite\n" << std::endl;
    else
       std::cout << "* interaction non souscrite\n" << std::endl;
}
// unsubscribeInteractionClass
void Federe_Interactif::callUsubIC ()
{
	cout <<"=>unsubscribeInteractionClass"<< endl;
	bool test = true;

	rti1516::InteractionClassHandle msgHandle = rtiAmb->getInteractionClassHandle(L"Msg");
	cout << "\t-> getInteractionClassHandle\n" << endl;

	try
	{
		rtiAmb->unsubscribeInteractionClass(msgHandle);
	}
	catch (rti1516::Exception &e)
	{
		test = false;
	}

	if (test)
		std::cout << "* OK\n" << std::endl;
	else
		std::cout << "* PAS OK\n" << std::endl;
}

/*************************************************************************************************
************************************ OBJECT MANAGEMENT *******************************************
*************************************************************************************************/

// registerObjectInstance
void Federe_Interactif::callROI ()
{
	cout <<"=>registerObjectInstance"<< endl;
	rti1516::AttributeHandleSet classAttributes;
	rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(L"TestObjectClass");
	cout << "\t-> getObjectClassHandle" << endl;
	rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
	cout << "\t-> getAttributeHandle" << endl;
	classAttributes.insert(attrHandle);

	try
	{
		rtiAmb->publishObjectClassAttributes(classHandle, classAttributes);
		cout << "\t-> publishObjectClassAttributes\n" << endl;
		rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);

		if ( instance.isValid() )
		{
			std::cout << "* OK\n" << std::endl;
		}
		else
		{
			std::cout << "* PAS OK\n" << std::endl;
		}
	}
	catch (rti1516::Exception &e)
	{
		std::cout << "* error\n" << std::endl;
	}
}

// updateAttributeValues
void Federe_Interactif::callUAV ()
{
	cout <<"=>updateAttributeValues"<< endl;
	char choix;


	rti1516::AttributeHandleValueMap attributeValues;
	rti1516::AttributeHandle invalidAttrHandle;
	rti1516::AttributeHandleValueMap invalidAttributes;
	rti1516::VariableLengthData theUserSuppliedTag;
	std::wstring className(L"TestObjectClass");

	rti1516::ObjectClassHandle classHandle = rtiAmb->getObjectClassHandle(className);
	cout << "\t-> getObjectClassHandle" << endl;
	rti1516::AttributeHandle attrHandle = rtiAmb->getAttributeHandle(classHandle, L"Data");
	cout << "\t-> getAttributeHandle" << endl;
	rti1516::AttributeHandleSet classAttributes;
	classAttributes.insert(attrHandle);
	std::string testString("test string");
	rti1516::VariableLengthData varData(testString.c_str(), testString.size());
	attributeValues.insert(std::pair<rti1516::AttributeHandle, rti1516::VariableLengthData>(attrHandle, varData));
	invalidAttributes.insert(std::pair<rti1516::AttributeHandle, rti1516::VariableLengthData>(invalidAttrHandle, varData));
	theUserSuppliedTag.setData(testString.c_str(), testString.size());

	while(1)
	{
	    cout <<"Avec gestion du temps ?[y/n]" << endl;
	    cin >> choix;
	    if (choix=='y' || choix=='n') break;
	    cout <<"Vous devez entrer 'y' ou 'n'" << endl;
	}


	if (choix=='y')
	{

		try
		{
			rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
			cout << "\t-> registerObjectInstance\n" << endl;
			std::auto_ptr<rti1516::LogicalTimeFactory> factory = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"certi");
			std::auto_ptr<rti1516::LogicalTime> time = factory->makeLogicalTime();
			rtiAmb->updateAttributeValues(instance, attributeValues, theUserSuppliedTag, *time);
		    cout <<"* Attribute values updated (with time)" << endl;

		}
		catch (rti1516::Exception &e)
		{
			std::cout << "* error\n" << std::endl;
		}
	}

	else
	{
		try
		{
			rti1516::ObjectInstanceHandle instance = rtiAmb->registerObjectInstance(classHandle);
			cout << "\t-> registerObjectInstance\n" << endl;
			rtiAmb->updateAttributeValues(instance, attributeValues, theUserSuppliedTag);
			cout <<"* Attribute values updated\n" << endl;
		}
		catch (rti1516::Exception &e)
		{
			std::cout << "* error\n" << std::endl;
		}
	}

}


// sendInteraction
void Federe_Interactif::callSI ()
{
	cout <<"=>sendInteraction"<< endl;

	double d;
    bool test = true;
    char choix;
    rti1516::ParameterHandleValueMap phvm;
    rti1516::InteractionClassHandle  iclassHandle;
    rti1516::VariableLengthData      paramValue;
    rti1516::ParameterHandle         paramHandle;
    rti1516::VariableLengthData      tag;

	iclassHandle = rtiAmb->getInteractionClassHandle(L"Msg");
	cout << "\t-> getInteractionClassHandle" << endl;
    paramHandle = rtiAmb->getParameterHandle(iclassHandle,L"MsgData");
    cout << "\t-> getParameterHandle" << endl;
    //paramValue = ... A CONSTRUIRE

	phvm[paramHandle] = paramValue;

    while(1)
    {
    cout <<"Avec gestion du temps ?[y/n]" << endl;
    cin >> choix;
    if (choix=='y' || choix=='n') break;
    cout <<"Vous devez entrer 'y' ou 'n'" << endl;
    }

    if (choix=='y') //SI avec estampille temporelle
    {
        wcout << endl << "t=" << tps->toString() << " : Donner la valeur de l'estampille voulue : ";
        cin >> d;

        std::auto_ptr<rti1516::LogicalTimeFactory> SItemps = rti1516::LogicalTimeFactoryFactory::makeLogicalTimeFactory(L"CERTI");
        std::auto_ptr< rti1516::LogicalTime > SItps(SItemps->makeLogicalTime());

    	try
    	{
    	   // rtiAmb->sendInteraction(iclassHandle, phvm, tag, *SItps);
    	}

    	catch (rti1516::Exception &e)
    	{
    	    test = false;
    	}

    	if (test)
    	   cout << "* interaction envoyee (estampillee " << d << ")\n" << endl;
    	else
    	   cout << "* interaction non envoyee\n" << endl;

    }

    else //SI sans estampille temporelle
    {
    	try
    	{
    	    rtiAmb->sendInteraction(iclassHandle, phvm, tag);
		}

    	catch (rti1516::Exception &e)
    	{
    		test = false;
    	}

    	if (test)
    		cout << "* interaction envoyee\n "<< endl;
    	else
    		cout << "* interaction non envoyee\n" << endl;
    }

}

/*************************************************************************************************
************************************* TIME MANAGEMENT ********************************************
*************************************************************************************************/

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















