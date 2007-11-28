#include "DisplayServiceExplanation.hh"
#include "DisplayServiceMessages.hh"

/* constructor */
DisplayServiceExplanation::DisplayServiceExplanation(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
}

/* destructor */
DisplayServiceExplanation::~DisplayServiceExplanation(void) {
}

/* private methods */
void
DisplayServiceExplanation::getKeyFromCmdLine(void) {
    std::cout << "\n>> Press any key to resume <<" << std::endl;
    char pressedKey;
    std::cin.get(pressedKey);
}

/* public methods */
void
DisplayServiceExplanation::dispTutorialIntroduction(void) {
    std::cout << DS_MSG_TUTORIAL_INTRODUCTION << std::endl;
    this->getKeyFromCmdLine();
}

/* Federation Management - RTI services */

void
DisplayServiceExplanation::dispFederationManagement(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_FEDERATION_MANAGEMENT << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispCreateFederationExecution(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_CREATE_FEDERATION_EXECUTION << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_CREATE_FEDERATION_EXECUTION_SRC << std::endl;   
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispJoinFederationExecution(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_JOIN_FEDERATION_EXECUTION << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_JOIN_FEDERATION_EXECUTION_SRC << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispResignFederationExecution(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_RESIGN_FEDERATION_EXECUTION<< std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_RESIGN_FEDERATION_EXECUTION_SRC<< std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispDestroyFederationExecution(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_DESTROY_FEDERATION_EXECUTION << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_DESTROY_FEDERATION_EXECUTION_SRC<< std::endl;
    this->getKeyFromCmdLine();
}

/* Declaration Management - RTI services */

void
DisplayServiceExplanation::dispDeclarationManagement(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_DECLARATION_MANAGEMENT << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispPublishObjectClass(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_PUBLISH_OBJECT_CLASS << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_PUBLISH_OBJECT_CLASS_SRC << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispSubscribeToObjectClass(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_SUBSCRIBE_TO_OBJECT_CLASS << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_SUBSCRIBE_TO_OBJECT_CLASS_SRC << std::endl;
    this->getKeyFromCmdLine();
}

/* Object Management - RTI services */

void
DisplayServiceExplanation::dispObjectManagement(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_OBJECT_MANAGEMENT << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispRegisterObjectInstance(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_REGISTER_OBJECT_INSTANCE << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_REGISTER_OBJECT_INSTANCE_SRC << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispUpdateAttributeValues(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_UPDATE_ATTRIBUTE_VALUES << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_UPDATE_ATTRIBUTE_VALUES_SRC << std::endl;
    this->getKeyFromCmdLine();
}

/* Object Management - Federate services */

void
DisplayServiceExplanation::dispDiscoverObjectInstance(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_DISCOVER_OBJECT_INSTANCE << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_DISCOVER_OBJECT_INSTANCE_SRC << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispReflectAttributeValues(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_REFLECT_ATTRIBUTE_VALUES << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_REFLECT_ATTRIBUTE_VALUES_SRC << std::endl;
    this->getKeyFromCmdLine();
}

/* Ancillary Services */

void 
DisplayServiceExplanation::dispGetObjectClassHandle(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_GET_OBJECT_CLASS_HANDLE << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_GET_OBJECT_CLASS_HANDLE_SRC << std::endl;
    this->getKeyFromCmdLine();
}

void 
DisplayServiceExplanation::dispGetAttributeHandle(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_GET_ATTRIBUTE_HANDLE << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_GET_ATTRIBUTE_HANDLE_SRC << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispAddToAttributeHandleSet(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_ADD_TO_ATTRIBUTE_HANDLE_SET << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_ADD_TO_ATTRIBUTE_HANDLE_SET_SRC << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispTick(void) {
    system(DS_CLEAR_CONSOLE_COMMAND);
    std::cout << DS_MSG_TICK << std::endl;
    std::cout << std::endl;
    std::cout << DS_MSG_TICK_SRC << std::endl;
    this->getKeyFromCmdLine();
}
