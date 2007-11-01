#ifndef DISPLAY_SERVICE_EXPLANATION_H
#define DISPLAY_SERVICE_EXPLANATION_H

#include <iostream>

class DisplayServiceExplanation {

private:

    void
    getKeyFromCmdLine(void);

public:

    /* constructor */
    DisplayServiceExplanation(void);

    /* destructor */
    ~DisplayServiceExplanation(void);

    /* Federation Management - RTI services */

    void
    dispTutorialIntroduction(void);

    void
    dispFederationManagement(void);

    void
    dispCreateFederationExecution(void);

    void
    dispJoinFederationExecution(void);

    void
    dispResignFederationExecution(void);

    void
    dispDestroyFederationExecution(void);
};

#endif /* DISPLAY_SERVICE_EXPLANATION_H */
