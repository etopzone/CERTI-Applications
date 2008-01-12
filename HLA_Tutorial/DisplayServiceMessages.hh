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
join an existing federation. Input parameters are its own name, the federation \
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
when leaving the federation. The required ResignAction parameter resolves \
ownership of any attributes owned by the federate. Allowed values are: \
RELEASE_ATTRIBUTES, DELETE_OBJECT, \
DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES (common practice), NO_ACTION. \
    " 

#define DS_MSG_RESIGN_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n468:    /* resign federation execution */\n\
469-    try {\n\
470-        rtiAmb.resignFederationExecution(\n\
471-            RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);\n\
472-    } catch ( RTI::Exception &e ) {\n\
473-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
474-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
475-    } catch ( ... ) {\n\
476-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
477-    }\n\
"

#define DS_MSG_DESTROY_FEDERATION_EXECUTION "\
Federation Management -> Destroy Federation Execution\n\n\
The RTI ambassador service destroyFederationExecution is usally invoked \
by the last federate. This service destroys the current federation \
execution. \
    " 

#define DS_MSG_DESTROY_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n481:    /* destroy federation execution */\n\
482-    try {\n\
483-        rtiAmb.destroyFederationExecution(federationName.c_str());\n\
484-    } catch (RTI::FederatesCurrentlyJoined) {\n\
485-        cout << \"Federates currently joined.\" << endl;\n\
486-    } catch ( RTI::Exception &e ) {\n\
487-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
488-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
489-    } catch ( ... ) {\n\
490-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
491-    }\n\
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

#define DS_MSG_PUBLISH_OBJECT_CLASS_SRC "Example from file: controllerFederate.cc\n324:    /* publish u */\n\
325-    try {\n\
326-        rtiAmb.publishObjectClass(twoLevelContrID, *attrU);\n\
327-    } catch ( RTI::Exception &e ) {\n\
328-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
329-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
330-    } catch ( ... ) {\n\
331-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
332-    }\n\
"

#define DS_MSG_SUBSCRIBE_TO_OBJECT_CLASS "\
Declaration Management -> Subscribe To Object Class\n\n \
When invoking this service the federate announces that it is interested in \
receiving updates for the specified attributes within the AttributeHandleSet \
class in its future execution. \
    "

#define DS_MSG_SUBSCRIBE_TO_OBJECT_CLASS_SRC "Example from file: controllerFederate.cc\n336:    /* subscribe to y */\n\
337-    try {\n\
338-        rtiAmb.subscribeObjectClassAttributes(twoLevelContrID, *attrY);\n\
339-    } catch ( RTI::Exception &e ) {\n\
340-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
341-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
342-    } catch ( ... ) {\n\
343-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
344-    }\n\
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

#define DS_MSG_REGISTER_OBJECT_INSTANCE_SRC "Example from file: controllerFederate.cc\n352:    /* register object */\n\
353-    RTI::ObjectHandle objInstID_u;\n\
354-\n\
355-    try {\n\
356-        objInstID_u = rtiAmb.registerObjectInstance(twoLevelContrID, \"u\");\n\
357-    } catch ( RTI::Exception &e ) {\n\
358-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
359-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
360-    } catch ( ... ) {\n\
361-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
362-    }\n\
"

#define DS_MSG_UPDATE_ATTRIBUTE_VALUES "\
Object Management -> Update Attribute Values\n\n \
The federate informs interested federates about object updates. Interested \
federates will receive the updates by a reflectAttributeValues callback. \
    "

#define DS_MSG_UPDATE_ATTRIBUTE_VALUES_SRC "Example from file: controllerFederate.cc\n405:        /* update attribute */\n\
406-        ahvps -> add(uID, (char*)&(u), sizeof(double));\n\
407-\n\
408-        try {\n\
409-            rtiAmb.updateAttributeValues(objInstID_u, *ahvps, \"u\");\n\
410-        } catch ( RTI::Exception &e ) {\n\
411-            cerr << \"RTI exception: \" << e._name << \" [\"\n\
412-            << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
413-        } catch ( ... ) {\n\
414-            cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
415-        }\n\
"

/* Object Management - Federate services */

#define DS_MSG_DISCOVER_OBJECT_INSTANCE "\
Object Management -> Discover Object Instance\n\n \
This federate service informs the federate of a new object instance. Federates \
are only informed about new objects if they have specified their interests in \
a former subscription invocation. In this case, we are informed about a new \
instance of the output variable y. \
    "

#define DS_MSG_DISCOVER_OBJECT_INSTANCE_SRC "Example from file: controllerFederate.cc\n167:    discoverObjectInstance (RTI::ObjectHandle theObject,\n\
168-                            RTI::ObjectClassHandle theObjectClass,\n\
169-                            const char *theObjectName)\n\
170-    throw ( RTI::CouldNotDiscover,\n\
171-            RTI::ObjectClassNotKnown,\n\
172-            RTI::FederateInternalError) {\n\
173-        cout << \"discoverObjInst \" << theObjectName << endl;\n\
174-        if (!strcmp(theObjectName,\"y\")) {\n\
175-            discovY = true;\n\
176-        }\n\
177-    }\n\
"

#define DS_MSG_REFLECT_ATTRIBUTE_VALUES "\
Object Management -> Reflect Attribute Values\n\n \
This federate service informs the federate of new state updates for attributes \
it is currently interesting in. \
    "

#define DS_MSG_REFLECT_ATTRIBUTE_VALUES_SRC "Example from file: controllerFederate.cc\n189:    reflectAttributeValues (RTI::ObjectHandle theObject,\n\
190-                            const RTI::AttributeHandleValuePairSet&\n\
191-                            theAttributes,\n\
192-                            const char *theTag)\n\
193-    throw ( RTI::ObjectNotKnown,\n\
194-            RTI::AttributeNotKnown,\n\
195-            RTI::FederateOwnsAttributes,\n\
196-            RTI::FederateInternalError) {\n\
197-        RTI::ULong length;\n\
198-        char *attrValue;\n\
199-\n\
200-        /*\n\
201-        * common implementation scheme is to iterate through the\n\
202-        * AttributeHandleValuePairSet.\n\
203-        */\n\
204-        for (unsigned int i=0; i<theAttributes.size(); i++) {\n\
205-\n\
206-            RTI::AttributeHandle handle = theAttributes.getHandle(i);\n\
207-            length = theAttributes.getValueLength(i);\n\
208-            attrValue = new char[length];\n\
209-            theAttributes.getValue(i, attrValue, length);\n\
210-\n\
211-            if (!strcmp(theTag,\"y\")) {\n\
212-                y = *(reinterpret_cast<double*>(attrValue));\n\
213-                newY = true;\n\
214-            }\n\
215-\n\
216-            delete[] attrValue;\n\
217-        }\n\
218-    }\n\
"

/* Ancillary Services */

#define DS_MSG_GET_OBJECT_CLASS_HANDLE "\
Ancillary Service -> Get Object Class Handle\n\n \
This service returns a numeric handle for the named object class \
according to the FOM resp. SOM. \
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
This service returns a numeric handle for the named attribute according to \
the FOM resp. SOM. The numeric handle of the corresponding object class have to\
be specified as second input parameter. \
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

#define DS_MSG_ADD_TO_ATTRIBUTE_HANDLE_SET "\
Ancillary Service -> Add attribute handles to attribute handle set\n\n \
The code below shows how to add attributes to the AttributeHandleSet class. \
    "

#define DS_MSG_ADD_TO_ATTRIBUTE_HANDLE_SET_SRC "Example from file: controllerFederate.cc\n313:    /* add attribute handle to AttributeHandleSet */\n\
314-    auto_ptr<RTI::AttributeHandleSet>\n\
315-    attrU(RTI::AttributeHandleSetFactory::create(1));\n\
316-    auto_ptr<RTI::AttributeHandleSet>\n\
317-    attrY(RTI::AttributeHandleSetFactory::create(1));\n\
318-\n\
319-    attrU->add(uID);\n\
320-    attrY->add(yID);\n\
"

#define DS_MSG_TICK "\
Ancillary Service -> Tick\n\n \
Tick is used to provide computation time to the LRC. Here we tick until we \
get the discoverObjectInstance callback. \
    "

#define DS_MSG_TICK_SRC "Example from file: controllerFederate.cc\n367:    /* discover y */\n\
368-    cout << \"Wait for second federate.\" << endl;\n\
369-    while (!myFedAmb.getDiscoverY()) {\n\
370-\n\
371-        try {\n\
372-            rtiAmb.tick();\n\
373-        } catch ( RTI::Exception &e ) {\n\
374-            cerr << \"RTI exception: \" << e._name << \" [\"\n\
375-            << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
376-        } catch ( ... ) {\n\
377-            cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
378-        }\n\
379-\n\
380-        sleep(1);\n\
381-    }\n\
382-    myFedAmb.setDiscoverY(false);\n\
"

#ifdef _WIN32
#define DS_CLEAR_CONSOLE_COMMAND "cls"
#else
#define DS_CLEAR_CONSOLE_COMMAND "clear"
#endif
#endif
