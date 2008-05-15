#include <RTI.hh>
#include <fedtime.hh>
#include <NullFederateAmbassador.hh>

#include <iostream>

#define NBCAR 300

char say_Y_N(char *question,int pas=0);
bool joinFedExec(char *federate,char *federationName);
void changingNames(char *federationname,char *fedfilename);
bool creeFedExec(char *federationName, char *fileName);


using std::auto_ptr ;


