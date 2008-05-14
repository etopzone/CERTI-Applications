#include "create_destroy.hh"

class Create_Destroy : public RTI::RTIambassador, public NullFederateAmbassador
{
public:
  Create_Destroy() throw (RTI::RTIinternalError) {}
  virtual ~Create_Destroy() throw (RTI::FederateInternalError) {}
  virtual bool publishAndSubscribe(char *classe,char *attribut,char *interact,char *param);



private:
//========================= CALLBACKS =========================================
//------------------------- initiateFederateSave ------------------------------
 void initiateFederateSave (const char *name) 
	throw (RTI::UnableToPerformSave, RTI::FederateInternalError)
 { 
    std::cout << std::endl << "     <----- initiateFederateSaved ( " << name <<")" <<std::endl; 
 } 
//------------------------- federationSaved ------------------------------
 void federationSaved () 
	throw (RTI::FederateInternalError) 
 { 
    std::cout << std::endl << "     <----- federationSaved ()" <<std::endl; 
 }
//------------------------- initiateFederateRestore ------------------------------
 void initiateFederateRestore (const char *name,RTI::FederateHandle handle) 
	throw (RTI::FederateInternalError, RTI::SpecifiedSaveLabelDoesNotExist, RTI::CouldNotRestore)
 { 
    std::cout << std::endl << "     <----- initiateFederateRestored ( " << name <<")" <<std::endl; 
 }
//------------------------- federationRestored ------------------------------
 void federationRestored () 
	throw (RTI::FederateInternalError) 
 { 
    std::cout << std::endl << "     <----- federationRestored ()" <<std::endl; 
 }
//------------------------- federationRestoreBegun ------------------------------
 void federationRestoreBegun () 
	throw (RTI::FederateInternalError) 
 { 
    std::cout << std::endl << "     <----- federationRestoreBegun ()" <<std::endl; 
 }
//------------------------- requestFederationRestoreSucceeded ------------------------------
void requestFederationRestoreSucceeded(const char *name) 
	throw (RTI::FederateInternalError)
 {
    std::cout << std::endl << "    <----- requestFederationRestoreSucceeded("<<name<<")"<<std::endl; 
 }
//------------------------- discoverObjectInstance ----------------------------
void
discoverObjectInstance(RTI::ObjectHandle theObject,
				RTI::ObjectClassHandle theObjectClass,
				const char *theObjectName)
    throw (RTI::CouldNotDiscover, RTI::ObjectClassNotKnown, 
	   RTI::FederateInternalError)
{

std::cout << std::endl << "    <----- discoverObjectInstance"<<std::endl;
printf("Object name %s with handle = %d and from Object Class Handle = %d ",theObjectName,theObject, theObjectClass);
printf(" has been discovered\n");
Object_discovered = theObject;

}
//------------------------- reflectAttributeValues ----------------------------
void
reflectAttributeValues(
    RTI::ObjectHandle theObject,
    const RTI::AttributeHandleValuePairSet& theAttributes,
    const char *theTag)
    throw (RTI::ObjectNotKnown,
           RTI::AttributeNotKnown,
           RTI::FederateInternalError)
{
RTI::ULong valueLength ;
char *attrValue ;

std::cout << std::endl << "    <----- reflectAttributeValues"<<std::endl;
printf("I received object handle = %d",theObject);
printf(" and :\n");
    for (unsigned int j=0 ; j<theAttributes.size(); j++)
        {
        RTI::AttributeHandle handle = theAttributes.getHandle(j);
        valueLength = theAttributes.getValueLength(j);
        attrValue = new char[valueLength] ;
        theAttributes.getValue(j, attrValue, valueLength);
        if ( handle == AttributeID )
           {
           int d_x ;
           memcpy(&d_x,attrValue,valueLength) ;
           printf("handle = %d , value = %d\n",handle,d_x);
           }
        else
           {
           printf("ERROR : handle %d unknown\n",handle);
           }
        }
}
//------------------------- provideAttributeValueUpdate ------------------------------
void provideAttributeValueUpdate(RTI::ObjectHandle the_object, const RTI::AttributeHandleSet &theAttributes) 
	throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeNotOwned,
	       RTI::FederateInternalError)
 { 
    std::cout << std::endl << "     <----- provideAttributeValueUpdate ()" <<std::endl; 
    printf("I have received  a provide for Object handle : %d with %d attributes\n",
            the_object,theAttributes.size());
    printf("the attributes handles are :\n");
    for (unsigned int j=0 ; j<theAttributes.size(); j++)
        {  
        printf("attribute %d has handle %d\n",j,theAttributes.getHandle(j));
        }  
 }
protected:
    virtual bool getHandles(char *classe,char *atrribut,char *interact,char *param);

    RTI::ObjectClassHandle ClassID ;
    RTI::AttributeHandle AttributeID ;
    RTI::InteractionClassHandle InteractClassID ;
    RTI::ParameterHandle ParameterID ;
  RTI::ObjectHandle Object_discovered;
};

//Ambassador
Create_Destroy* myCreate_Destroy ;

char federate[NBCAR+1];

//=============================================================================
//-------------------------------- main ---------------------------------------
//=============================================================================
int main(int argc, char **argv)
{

  int i;
  int nbcar = NBCAR ;
  char Federation_Name[NBCAR+1];
  char FED_file[NBCAR+1] ;
  char rep[10],answer ;
  bool try_again = true ;
  bool is_created,is_joined;
  //char federate[NBCAR+1];

  printf("================================================================\n");
  printf("|     TEST CREATE AND DESTROY FEDERATION EXECUTION             |\n");
  printf("================================================================\n");
  if (argc!=2)
  {
    std::cout <<"Vous have to give a federate name (as 1,2.. or a,b..)" << std::endl;
    exit(1);
  }
//=============================== step 0 ======================================
//Ambassador creation (and then rtia will begin)
//=============================================================================
  printf("================================================================\n");
  printf("|     STEP 0 : Ambassador creation (rtia process)              |\n");
  printf("================================================================\n");
try
  {
  myCreate_Destroy = new Create_Destroy();
  }
  catch ( RTI::Exception &e) {printf("Can't create ambassador so I stop.\n") ;
                                  exit(-1);}
printf("================================================================\n");
printf("|                      STEP 0 COMPLETED                        |\n");
printf("|     ambassador have been created, so rtia is running         |\n");
printf("================================================================\n");

//================================ step 1 =====================================
// About federation and fed file names
// User may change federation or fed file names
//=============================================================================
printf("================================================================\n");
printf("|     STEP 1 : Dance with federation and FED file names        |\n");
printf("================================================================\n");

changingNames(Federation_Name,FED_file) ;

printf("================================================================\n");
printf("|                      STEP 1 COMPLETED                        |\n");
printf("|--------------------- Situation ------------------------------|\n");
printf("| Federation Name = %20s  (not yet created)    |\n",
        Federation_Name);
printf("| FED file Name   = %20s                       |\n",
        FED_file);
printf("================================================================\n");
//=============================== step 2 ======================================
//CREATING FEDERATION EXECUTION
//=============================================================================
printf("================================================================\n");
printf("|     STEP 2 : Play with create federation(s)                  |\n");
printf("================================================================\n");

answer = say_Y_N("Do you want to make one or more creation federation execution ? : ",2);
if ( answer == 'y' )
  {
  // If creation fails we ask user to change (or not) the name of the federation
  while ( try_again)
    {
    is_created = creeFedExec(Federation_Name, FED_file);

    if ( !is_created )
        {
        // CREATION FAILED !
        std::cout<<"Creation of federation execution failed."<<std::endl;
        answer = say_Y_N("Do you want to try another FED file name (and path) ? [y/n]",3);
        if ( answer == 'y' )
            {
            std::cout<<"Enter FED file name (and path) (no more "<<NBCAR<<" characters !) : ";
            scanf("%s",FED_file) ;
            std::cout<<"FED file (and path) is now "<<FED_file << std::endl;        
            try_again = true ;
            }
        else
            {
            // 2 possibilities : stop or join
            printf("You can stop or join the federation %s \n",Federation_Name);
            answer = say_Y_N("Do you want to stop ? [y/n] :",31);
            if ( answer == 'y' )
                {
                std::cout<<"Bye !"<<std::endl;
                delete myCreate_Destroy;
                exit(-1);
                }
            else
                {
                // OK for join ( next step)
                try_again = false ;
                }
            }
        }
    else
        // CREATION GOOD !
        try_again = false ;
    }

  answer = say_Y_N("Do you want to try another federation execution creation ? [y/n]",3);
        if ( answer == 'y' )
           {
           char First_Federation_Name[NBCAR+1];
           char First_FED_file[NBCAR+1] ;
           strcpy(First_Federation_Name,Federation_Name);
           strcpy(First_FED_file,FED_file);
           changingNames(Federation_Name,FED_file) ;     
           is_created = creeFedExec(Federation_Name, FED_file);
           printf("NOTE : we work now on the federation %s with fed file %s\n",Federation_Name,FED_file);
           printf("---------------------------------------------------------------------------------\n");
           printf("================================================================\n");
           printf("|                      STEP 2 COMPLETED                        |\n");
           printf("|--------------------- Situation ------------------------------|\n");
           printf("| Federation Name = %20s (created)             |\n",
                  First_Federation_Name);
           printf("| FED file Name   = %20s  (used)               |\n",
                  First_FED_file);
           printf("| Federation Name = %20s (created)             |\n",
                  Federation_Name);
           printf("| FED file Name   = %20s  (used)               |\n",
                  FED_file);
           }
        else
           {
           printf("================================================================\n");
           printf("|                      STEP 2 COMPLETED                        |\n");
           printf("|--------------------- Situation ------------------------------|\n");
           printf("| Federation Name = %20s (created)             |\n",
                     Federation_Name);
           printf("| FED file Name   = %20s  (used)               |\n",
                     FED_file);
           }
  }

printf("================================================================\n");

//=============================== 3d step  ====================================
//JOINING FEDERATION EXECUTION
//=============================================================================
printf("----------------------------------------------------------------\n");
printf("|     STEP 3 : Joining a federation                            |\n");
printf("----------------------------------------------------------------\n");

  is_joined = false ;
  strcpy(federate,argv[1]);
  answer = say_Y_N("Do you want to join federation with federate ? [y/n]",4);
  if ( answer == 'y' )
     {
     is_joined = joinFedExec(federate,Federation_Name);

     printf("============================================================================\n");
     printf("|                      STEP 3 COMPLETED                                    |\n");
     printf("|--------------------- Situation ------------------------------------------|\n");
     printf("| Federation Name = %20s  (created)                        |\n",
        Federation_Name);
     printf("| FED file Name   = %20s                                   |\n",
        FED_file);
     if ( is_joined )
        {
        printf("| Federate %20s has joined federation %20s |\n",
                federate,Federation_Name);
        }
     else
        {
        printf("| No Federate has joined federation %20s |\n",
                Federation_Name);
        }
     printf("============================================================================\n");
     }

//=============================== 4d step  ====================================
// SAVE FEDERATION
//=============================================================================
printf("----------------------------------------------------------------\n");
printf("|     STEP 4 : Dance with save and restore a federation        |\n");
printf("----------------------------------------------------------------\n");
//BEGINNING FEDERATION SAVE
answer = say_Y_N("Do you want to do save federation step ? [y/n]",5);
if ( answer == 'y' )
  {
  answer = say_Y_N("Do you want to save federation ? [y/n]",51);
  if ( answer == 'y' )
     {
     myCreate_Destroy->requestFederationSave("TEST");
     std::cout<<"Federation "<<Federation_Name<<" has requested a save under label TEST"<<std::endl;
     }
// TICK TO GET INITIATE
  answer = say_Y_N("Do you want to do a tick (needed here) ? [y/n]",52);
  if ( answer == 'y' )
     {
     myCreate_Destroy->tick();
     //std::cout << std::endl << "t=" << temps->getTime() << " : -----> tick" << std::endl;
     }
  else
     {
     std::cout << "Bad answer, continuation is impredictible !"<< std::endl;
     }
// FEDERATE SAVE BEGUN
  answer = say_Y_N("Do you want to begin save federate (needed here) ? [y/n]",53);
  if ( answer == 'y' )
     {
     myCreate_Destroy->federateSaveBegun();
     std::cout<<"Federate "<<federate<<" begun a save"<<std::endl;
     std::cout<<"Federate is saving something........"<<std::endl;
     for ( int i = 1 ; i<=80 ; i++)
         printf(".");
     printf("\n") ;
     std::cout<<"Federate has completed saving successfull"<<std::endl;     
// FEDERATE SAVE COMPLETE
     myCreate_Destroy->federateSaveComplete();
     std::cout<<"Federate "<<federate<<" completed the save"<<std::endl;     
     }
// TICK
  answer = say_Y_N("Do you want to do a tick (needed here) ? [y/n]",54);
  if ( answer == 'y' )
     {
     myCreate_Destroy->tick();
     //std::cout << std::endl << "t=" << temps->getTime() << " : -----> tick" << std::endl;
     }
  else
     {
     std::cout << "Bad answer, continuation is impredictible !"<< std::endl;
     }
  }

//=============================== 5d step  ====================================
//BEGINNING FEDERATION RESTORE
printf("----------------------------------------------------------------\n");
printf("|     STEP 5 :            restore a federation                 |\n");
printf("----------------------------------------------------------------\n");
answer = say_Y_N("Do you want to do restore federation step ? [y/n]",6);
if ( answer == 'y' )
  {
  answer = say_Y_N("Do you want to restore federation ? [y/n]",61);
  if ( answer == 'y' )
     {
     myCreate_Destroy->requestFederationRestore("TEST");
     std::cout<<"Federation "<<Federation_Name<<" has requested a restore under label TEST"<<std::endl;
     }
// TICK TO GET REQUEST FEDERATION RESTORE SUCCEEDED
  answer = say_Y_N("Do you want to do a tick (needed here to get request federation restore succeeded) ? [y/n]",62);
  if ( answer == 'y' )
     {
     myCreate_Destroy->tick();
     }
  else
     {
     std::cout << "Bad answer, continuation is impredictible !"<< std::endl;
     }
printf("============================================================\n");
std::cout<<"Request federation has succeeded ......................."<<std::endl;     
// TICK TO GET FEDERATION RESTORE BEGUN
  answer = say_Y_N("Do you want to do a tick (needed here to get federation restore begun) ? [y/n]",63);
  if ( answer == 'y' )
     {
     myCreate_Destroy->tick();
     }
  else
     {
     std::cout << "Bad answer, continuation is impredictible !"<< std::endl;
     }
std::cout<<"Now federation restore begun................"<<std::endl;
// TICK TO GET INITIATE FEDERATE RESTORE
  answer = say_Y_N("Do you want to do a tick (needed here to get initiate federate restore) ? [y/n]",64);
  if ( answer == 'y' )
     {
     myCreate_Destroy->tick();
     }
  else
     {
     std::cout << "Bad answer, continuation is impredictible !"<< std::endl;
     }
std::cout<<"Now we are allowed to initiate federate restore..............."<<std::endl;
std::cout<<"Federate "<<federate<<" begun a restore"<<std::endl;
std::cout<<"Federate is restoring something........"<<std::endl;
for ( int i = 1 ; i<=80 ; i++)
     printf(".");
printf("\n") ;
std::cout<<"Federate has completed restoring successfull"<<std::endl;    
// FEDERATE RESTORE COMPLETE
  answer = say_Y_N("Do you want to do a federateRestoreComplete ? [y/n]",65);
  if ( answer == 'y' )
     {
     myCreate_Destroy->federateRestoreComplete();
     }
  else
     {
     std::cout << "Bad answer, continuation is impredictible !"<< std::endl;
     }
  std::cout<<"Federate "<<federate<<" has completed the restore"<<std::endl; 
// TICK TO GET FEDERATION RESTORED
  answer = say_Y_N("Do you want to do a tick (needed here to get federation restored) ? [y/n]",66);
  if ( answer == 'y' )
     {
     myCreate_Destroy->tick();
     }
  else
     {
     std::cout << "Bad answer, continuation is impredictible !"<< std::endl;
     }
std::cout<<"FEDERATION RESTORE STEP FINISHED"<<std::endl;
}
//=============================== 6d step  ====================================
//UPDATE ATTRIBUTE VALUES
printf("----------------------------------------------------------------\n");
printf("|     STEP 6 : Dance with objects                              |\n");
printf("----------------------------------------------------------------\n");
 answer = say_Y_N("Do you want to work on objects ? [y/n]",7);
  if ( answer == 'y' )
     {
     char class_name[NBCAR],attribute_name[NBCAR],interaction_name[NBCAR],parameter_name[NBCAR];
     printf("Name of the class ? ");
     scanf("%s",class_name);
     printf("Name of the atribute ? ");
     scanf("%s",attribute_name); 
     //printf("Name of the interaction ? ");
     //scanf("%s",interaction_name);
     //printf("Name of the parameter of the interaction ? ");
     //scanf("%s",parameter_name); 
     strcpy(interaction_name,"Bing");
     strcpy( parameter_name,"DX");  
     if ( ! myCreate_Destroy->publishAndSubscribe(class_name,attribute_name,interaction_name,parameter_name))
         printf("ERROR during working on objects.\n");
     else
         printf("publish and subscribe made\n");;
     }

//=============================== 6d step  ====================================
//RESIGNING FEDERATION EXECUTION
  answer = say_Y_N("Do you want to resign federation ? [y/n]",8);
  if ( answer == 'y' )
     {
     try {
         myCreate_Destroy->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
         std::cout << std::endl << "federation left" << std::endl;
         }
     catch (RTI::Exception &e)
          { printf("ERROR : Resign not done %s (%s)\n",e._name,e._reason); }
     }

//=============================== 7d step  ====================================
//DESTROYING FEDERATION EXECUTION
  answer = say_Y_N("Do you want to destroy federation ? [y/n]",9);
  if ( answer == 'y' )
     {
      try {
          myCreate_Destroy->destroyFederationExecution(Federation_Name);
          // We sleep a little (RTIA ending messages)
          sleep(3);
          std::cout << std::endl << "federation destroyed" << std::endl;
          }
      catch (RTI::Exception &e)
          { 
          if (strcmp(e._name,"FederatesCurrentlyJoined")==0)
             {
              printf("Destroy not allowed (%s): federates currently joined\n",e._reason);
              printf("You have to resign before !\n");
              answer = say_Y_N("Do you want to resign federation ? [y/n]");
              if ( answer == 'y' )
                 {
                 myCreate_Destroy->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
                 std::cout << std::endl << "federation left" << std::endl;
                 printf("NOW we can destroy federation %s\n",Federation_Name);
                 myCreate_Destroy->destroyFederationExecution(Federation_Name);
                 std::cout << std::endl << "federation destroyed" << std::endl;
                 }
             }
          else if (strcmp(e._name,"FederationExecutionDoesNotExist")==0)
             {    
             printf("Destroy impossible : federation does not exist (maybe has been yet destroyed)\n");
             }
          else if (strcmp(e._name,"ConcurrentAccessAttempted")==0)
             { 
             printf("ERROR during Destroy : Concurrent Access Attempted\n");
             }
          else if (strcmp(e._name,"RTIinternalError")==0)
             {
             printf("ERROR during Destroy : RTI internal error, reason is %s\n",e._reason);
             }
          else
             printf("%s with %s\n",e._name,e._reason);
          }
     }
     //delete myCreate_Destroy ;
//=============================== 7d step  ====================================
//CREATE -JOIN - RESIGN -DESTROY - CREATE - JOIN - RESIGN - DESTROY

  answer = say_Y_N("Do you want to do loop create-join-resign-destroy 2 times ? [y/n]",10);
  if ( answer == 'y' )
     {
     printf("************************************** LOOP n°1 *************\n");
     myCreate_Destroy = new Create_Destroy();
     is_created = creeFedExec(Federation_Name, FED_file);
     if ( ! is_created ) printf("Creation 1 aborted\n"); else printf("Creation 1 succeeded\n");
     is_joined = joinFedExec(federate,Federation_Name);
     if ( ! is_joined ) printf("Join 1 aborted\n"); else printf("Join 1 succeeded\n");
     myCreate_Destroy->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
     myCreate_Destroy->destroyFederationExecution(Federation_Name);
     printf("************************************** LOOP n°2 *************\n");
     delete myCreate_Destroy ;
     myCreate_Destroy = new Create_Destroy();
     is_created = creeFedExec(Federation_Name, FED_file);
     if ( ! is_created ) printf("Creation 2 aborted\n");else printf("Creation 2 succeeded\n");
     is_joined = joinFedExec(federate,Federation_Name);

     myCreate_Destroy->resignFederationExecution(RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);
     myCreate_Destroy->destroyFederationExecution(Federation_Name);
     printf("************************************* Done...\n");
     delete myCreate_Destroy ;
     }

//============================= THE END =======================================
//  answer = say_Y_N("Do you want to finish ? [y/n]");
//  if ( answer == 'y' )
 //    {
 //    delete myCreate_Destroy ;
     exit(0);
//     }
  
}
//========================================= MAIN END ==========================
//----------------------------------------- creeFedExec -----------------------
bool creeFedExec(char *federationName, char *fileName)
// Create a federation execution name giving :
// federationName : federation execution name
// fileName : FED file name
// Return : true if federation execution created
//          false if creation failed
// Note 5 exceptions may raise : CouldNotOpenFED,RTIinternalError,
//   ConcurrentAccessAttempted,ErrorReadingFED,FederationExecutionAlreadyExists
{
  bool is_created = false ;

  try {
      myCreate_Destroy->createFederationExecution(federationName,fileName);
      std::cout << "federation "<<federationName <<" created."<<std::endl;
      is_created = true ;
      }
  catch (RTI::Exception &e)
      { printf("ERROR : %s ",e._name);
        if (strcmp(e._name,"CouldNotOpenFED")==0 || strcmp(e._name,"ErrorReadingFED")==0)
          {
          printf("(file %s) ",fileName);
          printf(" Reason is %s\n",e._reason);
          }
        else if (strcmp(e._name,"FederationExecutionAlreadyExists")==0)
          {
          printf("(Federation %s)\n",e._reason);
          }
        else
          printf(" Reason is %s\n",e._reason);
      }

  return is_created ;
}

//----------------------------------------- joinFedExec -----------------------
bool joinFedExec(char *federate,char *federationName)
// Create a federation execution name giving :
// federationName : federation execution name
// fileName : FED file name
// Return : true if federation execution created
//          false if creation failed
// Note 5 exceptions may raise : FederateAlreadyExecutionMember,CouldNotOpenFED,
//                               RTIinternalError,
//   ConcurrentAccessAttempted,ErrorReadingFED,FederationExecutionAlreadyExists
// Attempt to join
// if success return true
// if fail ask to user to change federate name in case of FederateAlreadyExecutionMember
// if user doesn't agree return false
// if user agree then give the federate name and loop to join attempt
{
  bool success = false ;      // true means join succeeded
  bool interaction = true ;   // true means attempt to join
  char answer ;               // y or n

while(interaction)
  {
  interaction = false ;
  try {
      myCreate_Destroy->joinFederationExecution(federate,federationName,myCreate_Destroy);
      printf("Federation joined\n");
      }
  catch (RTI::Exception &e)
      { printf("Create_Destroy ERROR : %s reason is %s\n",e._name,e._reason);
        if (strcmp(e._name,"FederateAlreadyExecutionMember")==0)
          {
          printf("(federate %s) ",federate);
          printf(" Reason is %s\n",e._reason);
          answer = say_Y_N("Do you want to change federate name ? [y/n]");
          if ( answer == 'y' )
             {
             printf("federate name : ");
             scanf("%s",federate);
             interaction = true ;
             }
          else
             return false ;            
          }
        else
            return false ;
        }
    }

  return true;
}

//---------------------------- say_Y_N ----------------------------------------
char say_Y_N(char *question,int pas)
// Write question to user
// User has to answer y or n which is returned as one char
{
char rep[NBCAR];
  while(true)
  {
    printf("(%d) %s ",pas,question);
    scanf("%s",rep);
    if (rep[0] =='y' || rep[0] =='n') break;
    std::cout <<"Please enter 'y' or 'n'" << std::endl;
  }
  return rep[0];
}

//======================================== changing ===========================
void changingNames(char *Federation_Name, char *FED_file)
{
char answer ;

strcpy(Federation_Name,"Create_Destroy");
strcpy(FED_file,"create_destroy.fed");
printf("We use Federation name %s and fed file %s by default.\n",Federation_Name,FED_file);
printf("------------------------------------------------------------------------------\n");
answer = say_Y_N("Do you want to change Federation name or fed file (or twice) ? [y/n]",1);
if ( answer == 'y' )
    { 
    //------------------ Federation Name ------------------------------------------
    // default federation name may be changed by user (why not ?)
    answer = say_Y_N("Do you want to change Federation name ? [y/n]",11);
    if ( answer == 'y' )
      {
      // Changing federation name
      std::cout<<"Enter Federation Name : ";
      scanf("%s",Federation_Name) ;
      }

    //------------------ FED file Name --------------------------------------------
    // default FED file name may be changed by user
    answer = say_Y_N("Do you want to change FED file name (and path) ? [y/n]",12);
    if ( answer == 'y' )
      {
      // Changing FED file name
      std::cout<<"Enter FED file name (and path) : ";
      scanf("%s",FED_file) ;
      }
    printf("NOW we use Federation name %s and fed file %s .\n",Federation_Name,FED_file);
    printf("------------------------------------------------------------------------\n");
    }
}
// ----------------------------------------------------------------------------
/** get handles of objet/interaction classes
 */
bool
Create_Destroy::getHandles(char *class_name,char *attribute_name,char *interaction_name,char *parameter_name)
{
    // First get the handle of class_name
    try
      {
      ClassID = myCreate_Destroy->getObjectClassHandle(class_name);
      printf("Class %s has handle = %d\n",class_name,ClassID);
      }
    catch (RTI::NameNotFound) { printf("Class %s not found\n",class_name);
                                    return false;}
    catch (RTI::FederateNotExecutionMember) { printf("Federate is not member of the federation\n");
                                    return false;}
    catch (RTI::ConcurrentAccessAttempted) { printf("ConcurrentAccessAttempted\n");
                                    return false;}
    catch (RTI::RTIinternalError) { printf("RTIinternalError\n");
                                    return false;}

    // Attributs des classes d'Objets
    try
      {
      AttributeID = myCreate_Destroy->getAttributeHandle(attribute_name, ClassID);
      printf("Attribute %s from Object Class handle %d has handle = %d\n",attribute_name,ClassID,AttributeID);
      }
    catch (RTI::NameNotFound) { printf("Attribute %s not found\n",attribute_name);
                                    return false;}

    // Interactions
    //InteractClassID = myCreate_Destroy->getInteractionClassHandle(interaction_name);
    //printf("Interaction %s has handle %d\n",interaction_name,InteractClassID);

    //ParameterID = myCreate_Destroy->getParameterHandle(parameter_name, InteractClassID);
    //printf("Parameter %s has handle %d\n",parameter_name,ParameterID);
    return true;
}
// ----------------------------------------------------------------------------
/** Carry out publications and subscriptions
 */
bool
Create_Destroy::publishAndSubscribe(char *class_name,char *attribute_name,char *interaction_name,char *parameter_name)
{
    char buf[NBCAR],answer ;
    RTI::ParameterHandleValuePairSet *parameterSet=NULL ;
    RTI::AttributeHandleValuePairSet *attributeSet =NULL ;
    RTI::ObjectHandle object_handle ;
    int dx ,x;

    // Get all class and attributes handles
    // For class name (like Bille)
    // For attribute name (like PositionX)
    // For interaction name (like Bing)
    // For parameter name (like DX)
    if ( ! getHandles(class_name,attribute_name,interaction_name,parameter_name))
        {
        printf("ERROR : at least one handle not found\n");
        return false ;
        }
    else
        {
        printf("OK I have all handles.\n");
        printf("======================\n");
        }
    // At this point :
    // ClassID = Class handle
    // AttributeID      = Attribute handle
    // InteractClassID  = Interaction handle
    // ParameterID    = Parameter handle

    // Add attribute handle to the attribute set 
    // Before, we create the Set with one attribute
    auto_ptr<RTI::AttributeHandleSet> attributes(RTI::AttributeHandleSetFactory::create(1));
    attributes->add(AttributeID);

    answer = say_Y_N("Do you want to publish the object class ? [y/n]",71);
    if ( answer == 'y' )
       {
       // Publish Object class with attribute set.
       printf("Publish object of class %d ",ClassID);
       try {
           myCreate_Destroy->publishObjectClass(ClassID, *attributes);
           printf("Made\n");
           }
       catch (RTI::ObjectClassNotDefined &e)
           { printf("ERROR : Object Class %s not defined",e._name);}
       catch (RTI::AttributeNotDefined &e)
           { printf("ERROR : Attribute %s not defined",e._name);}      
       // Publish to the interaction
       // printf("Publish to the interaction %s ",interaction_name);
       // myCreate_Destroy->publishInteractionClass(InteractClassID);
       // printf("Made\n");
       }

    answer = say_Y_N("Do you want to subscribe to the object class ? [y/n]",72);
    if ( answer == 'y' )
       {
       // Subscribe to object class named class_name, which has handle=ClassID and
       // with attribute named attribute-name, which has handle=AttributeID
       printf("Subscribe classe %d with attribute %d ",ClassID,AttributeID);
       myCreate_Destroy->subscribeObjectClassAttributes(ClassID, *attributes, RTI::RTI_TRUE);
       printf("Made\n");
       // Subscribe to the interaction
       // printf("Subscribe to the interaction %s ",interaction_name);
       // myCreate_Destroy->subscribeInteractionClass(InteractClassID, RTI::RTI_TRUE);
       // printf("Made\n");
       // tick needed here 
       answer = say_Y_N("Do you want to do a tick to get discover (needed here) ? [y/n]",73);
       if ( answer == 'y' )
           {
           myCreate_Destroy->tick();
           }
       }

    answer = say_Y_N("Do you want to register an instance of the object class ? [y/n]",74);
    if ( answer == 'y' )
       {
       // Register an instance of the object class ClassID, we give his name from the federate name (why not ?)
       // Handle of this created object is stored in object_handle
       object_handle = myCreate_Destroy->registerObjectInstance(ClassID,federate); 
       printf("Instance of object from class %d has been registered with handle %d and name %s \n",ClassID,object_handle,federate);
       printf("I confirm : The class of the newly created object is %d and his name is %s\n",
              myCreate_Destroy->getObjectInstanceHandle(federate),
              myCreate_Destroy->getObjectInstanceName(object_handle));
       }



    // UPDATE
    answer = say_Y_N("Do you want to do an updateAttributeValues ? [y/n]",75);
    if ( answer == 'y' )
       {
       // 1 attribute AttributeID stored in attributeSet
       attributeSet = RTI::AttributeSetFactory::create(1);
       printf("%s value ? ",attribute_name);
       scanf("%d",&x);
       memcpy(buf,&x,sizeof(x));
       attributeSet->add(AttributeID, buf,sizeof(x));    
       myCreate_Destroy->updateAttributeValues(object_handle,*attributeSet,"MAJ");
       std::cout << std::endl << "update attribute values made" << std::endl;
       } 

    answer = say_Y_N("Do you want to do a tick to get reflect attribute values ? [y/n]",76);
    if ( answer == 'y' )
       {  
       myCreate_Destroy->tick(); 
       }
 
//=================== REQUEST OBJECT ATTRIBUTE TEST ===========================
     answer = say_Y_N("Do you want to do a request object attribute ? [y/n]",77);
     if ( answer == 'y' )
       {
        printf("Now, I want the values of this attribute.\n");
        auto_ptr<RTI::AttributeHandleSet> attributesbis(RTI::AttributeHandleSetFactory::create(1));
        attributesbis->add(AttributeID);
        myCreate_Destroy->requestObjectAttributeValueUpdate(Object_discovered,*attributes);
        } 

     answer = say_Y_N("Do you want to do a tick to get provide attribute values ? [y/n]",78);
     if ( answer == 'y' )
         {  
          myCreate_Destroy->tick(); 
          }
    // UPDATE
    answer = say_Y_N("Do you want to do an updateAttributeValues ? [y/n]",79);
    if ( answer == 'y' )
       {
       // 1 attribute AttributeID stored in attributeSet
       attributeSet = RTI::AttributeSetFactory::create(1);
       printf("%s value ? ",attribute_name);
       scanf("%d",&x);
       memcpy(buf,&x,sizeof(x));
       attributeSet->add(AttributeID, buf,sizeof(x));    
       myCreate_Destroy->updateAttributeValues(object_handle,*attributeSet,"MAJ");
       std::cout << std::endl << "update attribute values made" << std::endl;
       } 

    answer = say_Y_N("Do you want to do a tick to get reflect attribute values ? [y/n]",700);
    if ( answer == 'y' )
       {  
       myCreate_Destroy->tick(); 
       }

    
    // SEND INTERACTION
    // 1 parameter : PositionX value stored in ParameterSet
    answer = say_Y_N("Do you want to do a sendInteraction ? [y/n]",701);
    if ( answer == 'y' )
       {
       parameterSet = RTI::ParameterSetFactory::create(1);
       printf("%s value ? ",parameter_name);
       scanf("%d",&dx);
       memcpy(buf,&dx,sizeof(dx)); 
       parameterSet->add(ParameterID, buf, sizeof(dx));
       printf("Sending interaction class %d ",InteractClassID);
       myCreate_Destroy->sendInteraction(InteractClassID, *parameterSet, "GO");
       printf("Made\n");
       }
return true ;
}

