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

    void
    dispTutorialIntroduction(void);

    /* Federation Management - RTI services */

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

    /* Declaration Management - RTI services */

    void
    dispDeclarationManagement(void);

    /* Ancillary Services */

    void
    dispGetObjectClassHandle(void);

    void
    dispGetAttributeHandle(void);

};

#endif /* DISPLAY_SERVICE_EXPLANATION_H */
