#include "DisplayServiceExplanation.hh"

/* constructor */
DisplayServiceExplanation::DisplayServiceExplanation(void) {
    system("clear");
}

/* destructor */
DisplayServiceExplanation::~DisplayServiceExplanation(void) {
}

/* private methods */
void
DisplayServiceExplanation::getKeyFromCmdLine(void) {
    std::cout << "\n\n>> Press any key to resume <<" << std::endl;
    char pressedKey;
    std::cin.get(pressedKey);
}

/* public methods */
void
DisplayServiceExplanation::dispTutorialIntroduction(void) {
    std::cout << "\
HLA Tutorial\n\n\
This tutorial gives a brief introduction in the usage of the High Level \
Architecture Interface Specification 1.3. HLA is a standard for distributed \
computer simulation systems. The middleware implementing HLA is called Runtime \
Infrastructure (RTI).\n\
The API is separated in so called RTI ambassador services and federate \
ambassador services. The RTI ambassador services are provided by an \
RTIambassador object and are handled like normal method invocations. In \
contrast, federate ambassador services have to be implemented within the \
federate and can be regarded as a callback mechanism.\n\
Furthermore the services are divided in different management areas. This \
tutorial shows and explains the correct usage of essential API calls of \
the management areas Federation Management, Declaration Management and \
Object Management.\
    " << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispFederationManagement(void) {
    system("clear");
    std::cout << "\
Federation Management\n\n\
Services of this management area coordinate federation-wide activities \
such as creating or destroying federations and joining or resigning a \
federation. Federation-wide save and restore activities and synchronization \
operations are also supported. \
    " << std::endl;

    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispCreateFederationExecution(void) {
    system("clear");
    std::cout << "\
Federation Management -> Create Federation Execution\n\n\
The RTI ambassador service createFederationExecution is usally invoked by the \
first federate. This service creates the federation specifed by a name and a \
file holding the federation object model (FOM). \
    " << std::endl;
    std::cout << std::endl;
    std::cout << "Example (controllerFederate.cc):" << std::endl;
    std::cout << std::endl;
    system("LOCATION=`find ~/ -name controllerFederate.cc 2>/dev/null`; \
    	    LN=`cat -n $LOCATION | grep -i 'create federation execution' \
	    | awk '{print $1}'`; \
	    LN=`echo ${LN}+12 | bc`; \
	    cat -n ${LOCATION} | head -n $LN | tail -n 12");
    system("echo $SRCLOC");
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispJoinFederationExecution(void) {
    system("clear");
    std::cout << "\
Federation Management -> Join Federation Execution\n\n\
The RTI ambassador service joinFederationExecution is used by a federate to \
join an existing federation. Input parameters are his own name, the federation \
name of the federation it wants to join and a pointer to its federate \
ambassador implementation. \
    " << std::endl;
    std::cout << std::endl;
    std::cout << "Example (controllerFederate.cc):" << std::endl;
    std::cout << std::endl;
    system("LOCATION=`find ~/ -name controllerFederate.cc 2>/dev/null`; \
    	    LN=`cat -n $LOCATION | grep -i 'join federation execution' \
	    | awk '{print $1}'`; \
	    LN=`echo ${LN}+10 | bc`; \
	    cat -n ${LOCATION} | head -n $LN | tail -n 10");
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispResignFederationExecution(void) {
    system("clear");
    std::cout << "\
Federation Management -> Resign Federation Execution\n\n\
The RTI ambassador service resignFederationExecution is used by a federate \
when leaving the federation. \
    " << std::endl;
    std::cout << std::endl;
    std::cout << "Example (controllerFederate.cc):" << std::endl;
    std::cout << std::endl;
    system("LOCATION=`find ~/ -name controllerFederate.cc 2>/dev/null`; \
    	    LN=`cat -n $LOCATION | grep -i 'resign federation execution' \
	    | awk '{print $1}'`; \
	    LN=`echo ${LN}+9 | bc`; \
	    cat -n ${LOCATION} | head -n $LN | tail -n 9 ");

    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispDestroyFederationExecution(void) {
    system("clear");
    std::cout << "\
Federation Management -> Destroy Federation Execution\n\n\
The RTI ambassador service destroyFederationExecution is usally invoked \
by the last federate. This service destroys the current federation \
execution. \
    " << std::endl;
    std::cout << std::endl;
    std::cout << "Example (controllerFederate.cc):" << std::endl;
    std::cout << std::endl;
    system("LOCATION=`find ~/ -name controllerFederate.cc 2>/dev/null`; \
    	    LN=`cat -n $LOCATION | grep -i 'destroy federation execution' \
	    | awk '{print $1}'`; \
	    LN=`echo ${LN}+10 | bc`; \
	    cat -n ${LOCATION} | head -n $LN | tail -n 10");

    this->getKeyFromCmdLine();
}
