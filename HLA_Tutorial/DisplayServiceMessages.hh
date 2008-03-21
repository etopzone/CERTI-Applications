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
    
#define DS_MSG_CREATE_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n243:    /* create federation execution */\n\
244-    try {\n\
245-        rtiAmb.createFederationExecution(federationName.c_str(), \n\
246-					 fedFile.c_str());\n\
247-    } catch ( RTI::FederationExecutionAlreadyExists ) {\n\
248-        cout << \"Federation already created by another federate.\" \n\
249-	<< endl;\n\
250-    } catch ( RTI::Exception &e ) {\n\
251-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
252-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
253-    } catch ( ... ) {\n\
254-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
255-    }\n\
"

#define DS_MSG_JOIN_FEDERATION_EXECUTION "\
Federation Management -> Join Federation Execution\n\n\
The RTI ambassador service joinFederationExecution is used by a federate to \
join an existing federation. Input parameters are its own name, the federation \
name of the federation it wants to join and a pointer to its federate \
ambassador implementation. \
    "

#define DS_MSG_JOIN_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n259:    /* join federation execution */\n\
260-    try {\n\
261-        rtiAmb.joinFederationExecution(federateName.c_str(),\n\
262-                                       federationName.c_str(),\n\
263-                                       &myFedAmb);\n\
264-    } catch ( RTI::Exception &e ) {\n\
265-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
266-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
267-    } catch ( ... ) {\n\
268-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
269-    }\n\
"

#define DS_MSG_RESIGN_FEDERATION_EXECUTION "\
Federation Management -> Resign Federation Execution\n\n\
The RTI ambassador service resignFederationExecution is used by a federate \
when leaving the federation. The required ResignAction parameter resolves \
ownership of any attributes owned by the federate. Allowed values are: \
RELEASE_ATTRIBUTES, DELETE_OBJECT, \
DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES (common practice), NO_ACTION. \
    " 

#define DS_MSG_RESIGN_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n474:    /* resign federation execution */\n\
475-    try {\n\
476-        rtiAmb.resignFederationExecution(\n\
477-            RTI::DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES);\n\
478-    } catch ( RTI::Exception &e ) {\n\
479-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
480-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
481-    } catch ( ... ) {\n\
482-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
483-    }\n\
"

#define DS_MSG_DESTROY_FEDERATION_EXECUTION "\
Federation Management -> Destroy Federation Execution\n\n\
The RTI ambassador service destroyFederationExecution is usally invoked \
by the last federate. This service destroys the current federation \
execution. \
    " 

#define DS_MSG_DESTROY_FEDERATION_EXECUTION_SRC "Example from file: controllerFederate.cc\n487:    /* destroy federation execution */\n\
488-    try {\n\
489-        rtiAmb.destroyFederationExecution(federationName.c_str());\n\
490-    } catch (RTI::FederatesCurrentlyJoined) {\n\
491-        cout << \"Federates currently joined.\" << endl;\n\
492-    } catch ( RTI::Exception &e ) {\n\
493-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
494-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
495-    } catch ( ... ) {\n\
496-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
497-    }\n\
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

#define DS_MSG_PUBLISH_OBJECT_CLASS_SRC "Example from file: controllerFederate.cc\n326:    /* publish u */\n\
327-    try {\n\
328-        rtiAmb.publishObjectClass(twoLevelContrID, *attrU);\n\
329-    } catch ( RTI::Exception &e ) {\n\
330-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
331-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
332-    } catch ( ... ) {\n\
333-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
334-    }\n\
"

#define DS_MSG_SUBSCRIBE_TO_OBJECT_CLASS "\
Declaration Management -> Subscribe To Object Class\n\n \
When invoking this service the federate announces that it is interested in \
receiving updates for the specified attributes within the AttributeHandleSet \
class in its future execution. \
    "

#define DS_MSG_SUBSCRIBE_TO_OBJECT_CLASS_SRC "Example from file: controllerFederate.cc\n338:    /* subscribe to y */\n\
339-    try {\n\
340-        rtiAmb.subscribeObjectClassAttributes(twoLevelContrID, *attrY);\n\
341-    } catch ( RTI::Exception &e ) {\n\
342-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
343-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
344-    } catch ( ... ) {\n\
345-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
346-    }\n\
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

#define DS_MSG_REGISTER_OBJECT_INSTANCE_SRC "Example from file: controllerFederate.cc\n354:    /* register object */\n\
355-    RTI::ObjectHandle objInstID_u;\n\
356-\n\
357-    try {\n\
358-        objInstID_u = rtiAmb.registerObjectInstance(twoLevelContrID, \"u\");\n\
359-    } catch ( RTI::Exception &e ) {\n\
360-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
361-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
362-    } catch ( ... ) {\n\
363-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
364-    }\n\
"

#define DS_MSG_UPDATE_ATTRIBUTE_VALUES "\
Object Management -> Update Attribute Values\n\n \
The federate informs interested federates about object updates. Interested \
federates will receive the updates by a reflectAttributeValues callback. \
    "

#define DS_MSG_UPDATE_ATTRIBUTE_VALUES_SRC "Example from file: controllerFederate.cc\n411:        /* update attribute */\n\
412-        ahvps -> add(uID, (char*)&(u), sizeof(double));\n\
413-\n\
414-        try {\n\
415-            rtiAmb.updateAttributeValues(objInstID_u, *ahvps, \"u\");\n\
416-        } catch ( RTI::Exception &e ) {\n\
417-            cerr << \"RTI exception: \" << e._name << \" [\"\n\
418-            << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
419-        } catch ( ... ) {\n\
420-            cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
421-        }\n\
"

/* Object Management - Federate services */

#define DS_MSG_DISCOVER_OBJECT_INSTANCE "\
Object Management -> Discover Object Instance\n\n \
This federate service informs the federate of a new object instance. Federates \
are only informed about new objects if they have specified their interests in \
a former subscription invocation. In this case, we are informed about a new \
instance of the output variable y. \
    "

#define DS_MSG_DISCOVER_OBJECT_INSTANCE_SRC "Example from file: controllerFederate.cc\n169:    discoverObjectInstance (RTI::ObjectHandle theObject,\n\
170-                            RTI::ObjectClassHandle theObjectClass,\n\
171-                            const char *theObjectName)\n\
172-    throw ( RTI::CouldNotDiscover,\n\
173-            RTI::ObjectClassNotKnown,\n\
174-            RTI::FederateInternalError) {\n\
175-        cout << \"discoverObjInst \" << theObjectName << endl;\n\
176-        if (!strcmp(theObjectName,\"y\")) {\n\
177-            discovY = true;\n\
178-        }\n\
179-    }\n\
"

#define DS_MSG_REFLECT_ATTRIBUTE_VALUES "\
Object Management -> Reflect Attribute Values\n\n \
This federate service informs the federate of new state updates for attributes \
it is currently interesting in. \
    "

#define DS_MSG_REFLECT_ATTRIBUTE_VALUES_SRC "Example from file: controllerFederate.cc\n191:    reflectAttributeValues (RTI::ObjectHandle theObject,\n\
192-                            const RTI::AttributeHandleValuePairSet&\n\
193-                            theAttributes,\n\
194-                            const char *theTag)\n\
195-    throw ( RTI::ObjectNotKnown,\n\
196-            RTI::AttributeNotKnown,\n\
197-            RTI::FederateOwnsAttributes,\n\
198-            RTI::FederateInternalError) {\n\
199-        RTI::ULong length;\n\
200-        char *attrValue;\n\
201-\n\
202-        /*\n\
203-        * common implementation scheme is to iterate through the\n\
204-        * AttributeHandleValuePairSet.\n\
205-        */\n\
206-        for (unsigned int i=0; i<theAttributes.size(); i++) {\n\
207-\n\
208-            RTI::AttributeHandle handle = theAttributes.getHandle(i);\n\
209-            length = theAttributes.getValueLength(i);\n\
210-            attrValue = new char[length];\n\
211-            theAttributes.getValue(i, attrValue, length);\n\
212-\n\
213-            if (!strcmp(theTag,\"y\")) {\n\
214-                y = *(reinterpret_cast<double*>(attrValue));\n\
215-                newY = true;\n\
216-            }\n\
217-\n\
218-            delete[] attrValue;\n\
219-        }\n\
220-    }\n\
"

/* Ancillary Services */

#define DS_MSG_GET_OBJECT_CLASS_HANDLE "\
Ancillary Service -> Get Object Class Handle\n\n \
This service returns a numeric handle for the named object class \
according to the FOM resp. SOM. \
    "

#define DS_MSG_GET_OBJECT_CLASS_HANDLE_SRC "Example from file: controllerFederate.cc\n279:    /* get object class handle */\n\
280-    RTI::ObjectClassHandle twoLevelContrID;\n\
281-\n\
282-    try {\n\
283-        twoLevelContrID = rtiAmb.getObjectClassHandle(\"TwoLevelControllerClass\");\n\
284-    } catch ( RTI::Exception &e ) {\n\
285-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
286-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
287-    } catch ( ... ) {\n\
288-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
289-    }\n\
"

#define DS_MSG_GET_ATTRIBUTE_HANDLE "\
Ancillary Service -> Get Attribute Handle\n\n \
This service returns a numeric handle for the named attribute according to \
the FOM resp. SOM. The numeric handle of the corresponding object class have to \
be specified as second input parameter. \
    "

#define DS_MSG_GET_ATTRIBUTE_HANDLE_SRC "Example from file: controllerFederate.cc\n293:    /* get attribute handle */\n\
294-    RTI::AttributeHandle uID, yID;\n\
295-    try {\n\
296-        uID = rtiAmb.getAttributeHandle(\"U\", twoLevelContrID);\n\
297-    } catch ( RTI::Exception &e ) {\n\
298-        cerr << \"RTI exception: \" << e._name << \" [\"\n\
299-        << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
300-    } catch ( ... ) {\n\
301-        cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
302-    }\n\
"

#define DS_MSG_ADD_TO_ATTRIBUTE_HANDLE_SET "\
Ancillary Service -> Add attribute handles to attribute handle set\n\n \
The code below shows how to add attributes to the AttributeHandleSet class. \
    "

#define DS_MSG_ADD_TO_ATTRIBUTE_HANDLE_SET_SRC "Example from file: controllerFederate.cc\n315:    /* add attribute handle to AttributeHandleSet */\n\
316-    auto_ptr<RTI::AttributeHandleSet>\n\
317-    attrU(RTI::AttributeHandleSetFactory::create(1));\n\
318-    auto_ptr<RTI::AttributeHandleSet>\n\
319-    attrY(RTI::AttributeHandleSetFactory::create(1));\n\
320-\n\
321-    attrU->add(uID);\n\
322-    attrY->add(yID);\n\
"

#define DS_MSG_TICK "\
Ancillary Service -> Tick\n\n \
Tick is used to provide computation time to the LRC. Here we tick until we \
get the discoverObjectInstance callback. \
    "

#define DS_MSG_TICK_SRC "Example from file: controllerFederate.cc\n369:    /* discover y */\n\
370-    cout << \"Wait for second federate.\" << endl;\n\
371-    while (!myFedAmb.getDiscoverY()) {\n\
372-\n\
373-        try {\n\
374-            rtiAmb.tick();\n\
375-        } catch ( RTI::Exception &e ) {\n\
376-            cerr << \"RTI exception: \" << e._name << \" [\"\n\
377-            << (e._reason ? e._reason : \"undefined\") << \"].\" << endl;\n\
378-        } catch ( ... ) {\n\
379-            cerr << \"Error: unknown non-RTI exception.\" << endl;\n\
380-        }\n\
381-\n\
382-#ifdef _WIN32\n\
383-        Sleep(1000);\n\
384-#else\n\
"

#ifdef _WIN32
#define DS_CLEAR_CONSOLE_COMMAND "cls"
#else
#define DS_CLEAR_CONSOLE_COMMAND "clear"
#endif
#endif
