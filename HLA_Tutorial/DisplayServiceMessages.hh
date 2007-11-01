#ifndef DISPLAY_SERVICE_MESSAGES_H
#define DISPLAY_SERVICE_MESSAGES_H

#define DS_MSG_TUTORIAL_INTRODUCTION "\
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
    "
    
#define DS_MSG_FEDERATION_MANAGEMENT "\
Federation Management\n\n\
Services of this management area coordinate federation-wide activities \
such as creating or destroying federations and joining or resigning a \
federation. Federation-wide save and restore activities and synchronization \
operations are also supported. \
    "

#define DS_MSG_CREATE_FEDERATION_EXECUTION "\
Federation Management -> Create Federation Execution\n\n\
The RTI ambassador service createFederationExecution is usally invoked by the \
first federate. This service creates the federation specifed by a name and a \
file holding the federation object model (FOM). \
    "
    
#define DS_MSG_CREATE_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n244:    /* create federation execution */\n\
245-    try {\n\
246-        rtiAmb.createFederationExecution(federationName.c_str(), \n\
247-					 fedFile.c_str());\n\
248-    } catch ( RTI::FederationExecutionAlreadyExists ) {\n\
249-        cout << \"Federation already created by another federate.\" \n\
250-	<< endl;\n\
251-    } catch ( RTI::Exception &e ) {\n\
252-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
253-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
254-    } catch ( ... ) {\n\
255-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
256-    }\n\
"

#define DS_MSG_JOIN_FEDERATION_EXECUTION "\
Federation Management -> Join Federation Execution\n\n\
The RTI ambassador service joinFederationExecution is used by a federate to \
join an existing federation. Input parameters are his own name, the federation \
name of the federation it wants to join and a pointer to its federate \
ambassador implementation. \
    "

#define DS_MSG_JOIN_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n260:    /* join federation execution */\n\
261-    try {\n\
262-        rtiAmb.joinFederationExecution(federateName.c_str(),\n\
263-                                       federationName.c_str(),\n\
264-                                       &myFedAmb);\n\
265-    } catch ( RTI::Exception &e ) {\n\
266-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
267-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
268-    } catch ( ... ) {\n\
269-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
270-    }\n\
"

#define DS_MSG_RESIGN_FEDERATION_EXECUTION "\
Federation Management -> Resign Federation Execution\n\n\
The RTI ambassador service resignFederationExecution is used by a federate \
when leaving the federation. \
    " 

#define DS_MSG_RESIGN_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n441:    /* resign federation execution */\n\
442-    try {\n\
443-        rtiAmb.resignFederationExecution(\n\
444-            RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);\n\
445-    } catch ( RTI::Exception &e ) {\n\
446-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
447-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
448-    } catch ( ... ) {\n\
449-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
450-    }\n\
451-\n\
"

#define DS_MSG_DESTROY_FEDERATION_EXECUTION "\
Federation Management -> Destroy Federation Execution\n\n\
The RTI ambassador service destroyFederationExecution is usally invoked \
by the last federate. This service destroys the current federation \
execution. \
    " 

#define DS_MSG_DESTROY_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n454:    /* destroy federation execution */\n\
455-    try {\n\
456-        rtiAmb.destroyFederationExecution(federationName.c_str());\n\
457-    } catch (RTI::FederatesCurrentlyJoined) {\n\
458-        cout << \"Federates currently joined.\" << endl;\n\
459-    } catch ( RTI::Exception &e ) {\n\
460-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
461-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
462-    } catch ( ... ) {\n\
463-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
464-    }\n\
"

#ifdef _WIN32
#define DS_CLEAR_CONSOLE_COMMAND "cls"
#else
#define DS_CLEAR_CONSOLE_COMMAND "clear"
#endif
#endif
