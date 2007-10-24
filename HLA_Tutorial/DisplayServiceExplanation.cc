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
    system("clear");
    std::cout << "HLA Tutorial\n\nThis tutorial gives a brief introduction in the usage of the HLA API 1.3." << std::endl;
    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispFederationManagement(void) {
    system("clear");
    std::cout << "General infos about Federation Management" << std::endl;

    this->getKeyFromCmdLine();
}

void
DisplayServiceExplanation::dispCreateFederationExecution(void) {
    system("clear");
    std::cout << "Create Federation Executiontions. Further explanations ..." << std::endl;

    this->getKeyFromCmdLine();
}
