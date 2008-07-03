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
    
/* Federation Management - RTI services */

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
first federate. This service creates the federation specified by a name and a \
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
join an existing federation. Input parameters are its own name, the federation \
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
when leaving the federation. The required ResignAction parameter resolves \
ownership of any attributes owned by the federate. Allowed values are: \
RELEASE_ATTRIBUTES, DELETE_OBJECT, \
DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES (common practice), NO_ACTION. \
    " 

#define DS_MSG_RESIGN_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n475:    /* resign federation execution */\n\
476-    try {\n\
477-        rtiAmb.resignFederationExecution(\n\
478-            RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);\n\
479-    } catch ( RTI::Exception &e ) {\n\
480-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
481-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
482-    } catch ( ... ) {\n\
483-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
484-    }\n\
"

#define DS_MSG_DESTROY_FEDERATION_EXECUTION "\
Federation Management -> Destroy Federation Execution\n\n\
The RTI ambassador service destroyFederationExecution is usally invoked \
by the last federate. This service destroys the current federation \
execution. \
    " 

#define DS_MSG_DESTROY_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n488:    /* destroy federation execution */\n\
489-    try {\n\
490-        rtiAmb.destroyFederationExecution(federationName.c_str());\n\
491-    } catch (RTI::FederatesCurrentlyJoined) {\n\
492-        cout << \"Federates currently joined.\" << endl;\n\
493-    } catch ( RTI::Exception &e ) {\n\
494-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
495-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
496-    } catch ( ... ) {\n\
497-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
498-    }\n\
"

/* Declaration Management - RTI services */

#define DS_MSG_DECLARATION_MANAGEMENT "\
Declaration Management\n\n \
HLA communication relies on an implicit data exchange mechanism. That's why \
federates have to declare their intention to generate or receive information \
in their initialization phase according to their FOM resp. their SOM. \
Declaration Management can also be regarded as interest management. \
    "

#define DS_MSG_PUBLISH_OBJECT_CLASS "\
Declaration Management -> Publish Object Class\n\n \
When invoking this service the federate announces that it can produce \
information for the specified attributes within the AttributeHandleSet class \
in its future execution. \
    "

#define DS_MSG_PUBLISH_OBJECT_CLASS_SRC "Example from file: controllerFederate.cc\n327:    /* publish u */\n\
328-    try {\n\
329-        rtiAmb.publishObjectClass(twoLevelContrID, *attrU);\n\
330-    } catch ( RTI::Exception &e ) {\n\
331-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
332-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
333-    } catch ( ... ) {\n\
334-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
335-    }\n\
"

#define DS_MSG_SUBSCRIBE_TO_OBJECT_CLASS "\
Declaration Management -> Subscribe To Object Class\n\n \
When invoking this service the federate announces that it is interested in \
receiving updates for the specified attributes within the AttributeHandleSet \
class in its future execution. \
    "

#define DS_MSG_SUBSCRIBE_TO_OBJECT_CLASS_SRC "Example from file: controllerFederate.cc\n339:    /* subscribe to y */\n\
340-    try {\n\
341-        rtiAmb.subscribeObjectClassAttributes(twoLevelContrID, *attrY);\n\
342-    } catch ( RTI::Exception &e ) {\n\
343-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
344-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
345-    } catch ( ... ) {\n\
346-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
347-    }\n\
"

/* Object Management - RTI services */

#define DS_MSG_OBJECT_MANAGEMENT "\
Object Management\n\n \
This part of the interface specification deals with deletion, instantiation, \
modification and registration of object instances. The necessary data exchange \
mechanisms, especially updating and reflecting attributes and sending and \
receiving interactions are part of this management area. \
    "

#define DS_MSG_REGISTER_OBJECT_INSTANCE "\
Object Management -> Register Object Instance\n\n \
This service creates a new object instance. Other federates are informed \
through the federate service discoverObjectInstance. \
    "

#define DS_MSG_REGISTER_OBJECT_INSTANCE_SRC "Example from file: controllerFederate.cc\n355:    /* register object */\n\
356-    RTI::ObjectHandle objInstID_u;\n\
357-\n\
358-    try {\n\
359-        objInstID_u = rtiAmb.registerObjectInstance(twoLevelContrID, \"u\");\n\
360-    } catch ( RTI::Exception &e ) {\n\
361-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
362-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
363-    } catch ( ... ) {\n\
364-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
365-    }\n\
"

#define DS_MSG_UPDATE_ATTRIBUTE_VALUES "\
Object Management -> Update Attribute Values\n\n \
The federate informs interested federates about object updates. Interested \
federates will receive the updates by a reflectAttributeValues callback. \
    "

#define DS_MSG_UPDATE_ATTRIBUTE_VALUES_SRC "Example from file: controllerFederate.cc\n412:        /* update attribute */\n\
413-        ahvps -> add(uID, (char*)&(u), sizeof(double));\n\
414-\n\
415-        try {\n\
416-            rtiAmb.updateAttributeValues(objInstID_u, *ahvps, \"u\");\n\
417-        } catch ( RTI::Exception &e ) {\n\
418-            cerr << \"RTI exception: \" << e._name << \" [\"\n\
419-            << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
420-        } catch ( ... ) {\n\
421-            cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
422-        }\n\
"

/* Object Management - Federate services */

#define DS_MSG_DISCOVER_OBJECT_INSTANCE "\
Object Management -> Discover Object Instance\n\n \
This federate service informs the federate of a new object instance. Federates \
are only informed about new objects if they have specified their interests in \
a former subscription invocation. In this case, we are informed about a new \
instance of the output variable y. \
    "

#define DS_MSG_DISCOVER_OBJECT_INSTANCE_SRC "Example from file: controllerFederate.cc\n170:    discoverObjectInstance (RTI::ObjectHandle theObject,\n\
171-                            RTI::ObjectClassHandle theObjectClass,\n\
172-                            const char *theObjectName)\n\
173-    throw ( RTI::CouldNotDiscover,\n\
174-            RTI::ObjectClassNotKnown,\n\
175-            RTI::FederateInternalError) {\n\
176-        cout << \"discoverObjInst \" << theObjectName << endl;\n\
177-        if (!strcmp(theObjectName,\"y\")) {\n\
178-            discovY = true;\n\
179-        }\n\
180-    }\n\
"

#define DS_MSG_REFLECT_ATTRIBUTE_VALUES "\
Object Management -> Reflect Attribute Values\n\n \
This federate service informs the federate of new state updates for attributes \
it is currently interesting in. \
    "

#define DS_MSG_REFLECT_ATTRIBUTE_VALUES_SRC "Example from file: controllerFederate.cc\n192:    reflectAttributeValues (RTI::ObjectHandle theObject,\n\
193-                            const RTI::AttributeHandleValuePairSet&\n\
194-                            theAttributes,\n\
195-                            const char *theTag)\n\
196-    throw ( RTI::ObjectNotKnown,\n\
197-            RTI::AttributeNotKnown,\n\
198-            RTI::FederateOwnsAttributes,\n\
199-            RTI::FederateInternalError) {\n\
200-        RTI::ULong length;\n\
201-        char *attrValue;\n\
202-\n\
203-        /*\n\
204-        * common implementation scheme is to iterate through the\n\
205-        * AttributeHandleValuePairSet.\n\
206-        */\n\
207-        for (unsigned int i=0; i<theAttributes.size(); i++) {\n\
208-\n\
209-            RTI::AttributeHandle handle = theAttributes.getHandle(i);\n\
210-            length = theAttributes.getValueLength(i);\n\
211-            attrValue = new char[length];\n\
212-            theAttributes.getValue(i, attrValue, length);\n\
213-\n\
214-            if (!strcmp(theTag,\"y\")) {\n\
215-                y = *(reinterpret_cast<double*>(attrValue));\n\
216-                newY = true;\n\
217-            }\n\
218-\n\
219-            delete[] attrValue;\n\
220-        }\n\
221-    }\n\
"

/* Ancillary Services */

#define DS_MSG_GET_OBJECT_CLASS_HANDLE "\
Ancillary Service -> Get Object Class Handle\n\n \
This service returns a numeric handle for the named object class \
according to the FOM resp. SOM. \
    "

#define DS_MSG_GET_OBJECT_CLASS_HANDLE_SRC "Example from file: controllerFederate.cc\n280:    /* get object class handle */\n\
281-    RTI::ObjectClassHandle twoLevelContrID;\n\
282-\n\
283-    try {\n\
284-        twoLevelContrID = rtiAmb.getObjectClassHandle(\"TwoLevelControllerClass\");\n\
285-    } catch ( RTI::Exception &e ) {\n\
286-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
287-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
288-    } catch ( ... ) {\n\
289-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
290-    }\n\
"

#define DS_MSG_GET_ATTRIBUTE_HANDLE "\
Ancillary Service -> Get Attribute Handle\n\n \
This service returns a numeric handle for the named attribute according to \
the FOM resp. SOM. The numeric handle of the corresponding object class have to \
be specified as second input parameter. \
    "

#define DS_MSG_GET_ATTRIBUTE_HANDLE_SRC "Example from file: controllerFederate.cc\n294:    /* get attribute handle */\n\
295-    RTI::AttributeHandle uID, yID;\n\
296-    try {\n\
297-        uID = rtiAmb.getAttributeHandle(\"U\", twoLevelContrID);\n\
298-    } catch ( RTI::Exception &e ) {\n\
299-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
300-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
301-    } catch ( ... ) {\n\
302-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
303-    }\n\
"

#define DS_MSG_ADD_TO_ATTRIBUTE_HANDLE_SET "\
Ancillary Service -> Add attribute handles to attribute handle set\n\n \
The code below shows how to add attributes to the AttributeHandleSet class. \
    "

#define DS_MSG_ADD_TO_ATTRIBUTE_HANDLE_SET_SRC "Example from file: controllerFederate.cc\n316:    /* add attribute handle to AttributeHandleSet */\n\
317-    auto_ptr<RTI::AttributeHandleSet>\n\
318-    attrU(RTI::AttributeHandleSetFactory::create(1));\n\
319-    auto_ptr<RTI::AttributeHandleSet>\n\
320-    attrY(RTI::AttributeHandleSetFactory::create(1));\n\
321-\n\
322-    attrU->add(uID);\n\
323-    attrY->add(yID);\n\
"

#define DS_MSG_TICK "\
Ancillary Service -> Tick\n\n \
Tick is used to provide computation time to the LRC. Here we tick until we \
get the discoverObjectInstance callback. \
    "

#define DS_MSG_TICK_SRC "Example from file: controllerFederate.cc\n370:    /* discover y */\n\
371-    cout << \"Wait for second federate.\" << endl;\n\
372-    while (!myFedAmb.getDiscoverY()) {\n\
373-\n\
374-        try {\n\
375-            rtiAmb.tick();\n\
376-        } catch ( RTI::Exception &e ) {\n\
377-            cerr << \"RTI exception: \" << e._name << \" [\"\n\
378-            << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
379-        } catch ( ... ) {\n\
380-            cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
381-        }\n\
382-\n\
383-#ifdef _WIN32\n\
384-        Sleep(1000);\n\
385-#else\n\
"

#ifdef _WIN32
#define DS_CLEAR_CONSOLE_COMMAND "cls"
#else
#define DS_CLEAR_CONSOLE_COMMAND "clear"
#endif
#endif
