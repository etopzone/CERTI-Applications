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
    
#define DS_MSG_CREATE_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n241:    /* create federation execution */\n\
242-    try {\n\
243-        rtiAmb.createFederationExecution(federationName.c_str(), \n\
244-					 fedFile.c_str());\n\
245-    } catch ( RTI::FederationExecutionAlreadyExists ) {\n\
246-        cout << \"Federation already created by another federate.\" \n\
247-	<< endl;\n\
248-    } catch ( RTI::Exception &e ) {\n\
249-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
250-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
251-    } catch ( ... ) {\n\
252-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
253-    }\n\
"

#define DS_MSG_JOIN_FEDERATION_EXECUTION "\
Federation Management -> Join Federation Execution\n\n\
The RTI ambassador service joinFederationExecution is used by a federate to \
join an existing federation. Input parameters are his own name, the federation \
name of the federation it wants to join and a pointer to its federate \
ambassador implementation. \
    "

#define DS_MSG_JOIN_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n257:    /* join federation execution */\n\
258-    try {\n\
259-        rtiAmb.joinFederationExecution(federateName.c_str(),\n\
260-                                       federationName.c_str(),\n\
261-                                       &myFedAmb);\n\
262-    } catch ( RTI::Exception &e ) {\n\
263-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
264-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
265-    } catch ( ... ) {\n\
266-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
267-    }\n\
"

#define DS_MSG_RESIGN_FEDERATION_EXECUTION "\
Federation Management -> Resign Federation Execution\n\n\
The RTI ambassador service resignFederationExecution is used by a federate \
when leaving the federation. The required ResignAction parameter resolves ownership of \
any attributes owned by the federate. Allowed values are: RELEASE_ATTRIBUTES, \
DELETE_OBJECT, DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES (common practice), NO_ACTION. \
    " 

#define DS_MSG_RESIGN_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n447:    /* resign federation execution */\n\
448-    try {\n\
449-        rtiAmb.resignFederationExecution(\n\
450-            RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);\n\
451-    } catch ( RTI::Exception &e ) {\n\
452-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
453-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
454-    } catch ( ... ) {\n\
455-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
456-    }\n\
"

#define DS_MSG_DESTROY_FEDERATION_EXECUTION "\
Federation Management -> Destroy Federation Execution\n\n\
The RTI ambassador service destroyFederationExecution is usally invoked \
by the last federate. This service destroys the current federation \
execution. \
    " 

#define DS_MSG_DESTROY_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n460:    /* destroy federation execution */\n\
461-    try {\n\
462-        rtiAmb.destroyFederationExecution(federationName.c_str());\n\
463-    } catch (RTI::FederatesCurrentlyJoined) {\n\
464-        cout << \"Federates currently joined.\" << endl;\n\
465-    } catch ( RTI::Exception &e ) {\n\
466-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
467-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
468-    } catch ( ... ) {\n\
469-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
470-    }\n\
"

#define DS_MSG_DECLARATION_MANAGEMENT "\
Declaration Management\n\n \
    "

#define DS_MSG_GET_OBJECT_CLASS_HANDLE "\
Ancillary Service -> Get Object Class Handle\n\n \
    "

#define DS_MSG_GET_OBJECT_CLASS_HANDLE_SRC "Example from file: controllerFederate.cc\n277:    /* get object class handle */\n\
278-    RTI::ObjectClassHandle twoLevelContrID;\n\
279-\n\
280-    try {\n\
281-        twoLevelContrID = rtiAmb.getObjectClassHandle(\"TwoLevelControllerClass\");\n\
282-    } catch ( RTI::Exception &e ) {\n\
283-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
284-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
285-    } catch ( ... ) {\n\
286-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
287-    }\n\
"

#define DS_MSG_GET_ATTRIBUTE_HANDLE "\
Ancillary Service -> Get Attribute Handle\n\n \
    "

#define DS_MSG_GET_ATTRIBUTE_HANDLE_SRC "Example from file: controllerFederate.cc\n291:    /* get attribute handle */\n\
292-    RTI::AttributeHandle uID, yID;\n\
293-    try {\n\
294-        uID = rtiAmb.getAttributeHandle(\"U\", twoLevelContrID);\n\
295-    } catch ( RTI::Exception &e ) {\n\
296-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
297-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
298-    } catch ( ... ) {\n\
299-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
300-    }\n\
"


#ifdef _WIN32
#define DS_CLEAR_CONSOLE_COMMAND "cls"
#else
#define DS_CLEAR_CONSOLE_COMMAND "clear"
#endif
#endif
