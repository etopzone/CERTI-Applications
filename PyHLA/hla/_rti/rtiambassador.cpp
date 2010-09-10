/*
 * Python Language HLA API
 * Copyright (C) 2008  Petr Gotthard <petr.gotthard@centrum.cz>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * $Id: rtiambassador.cpp,v 1.6 2010/09/10 21:04:29 gotthardp Exp $
 */

// note: you must include Python.h before any standard headers are included
#include <Python.h>
#include <structmember.h>

#include <RTI.hh>  
#include <fedtime.hh> 

#include "rtiambassador.h"
#include "federateambassador.h"
#include "handles.h"
#include "exceptions.h"

#define CATCH_RTI_EXCEPTION(EXCEPTION) \
    catch(RTI::EXCEPTION& exception) { PyErr_SetFromRTIException(rti_##EXCEPTION, &exception); }

#define CATCH_RTI_EXCEPTION2(RTI_EXCEPTION, PYTHON_EXCEPTION) \
    catch(RTI_EXCEPTION& exception) { PyErr_SetFromRTIException(PYTHON_EXCEPTION, &exception); }

static PyObject *
rtia_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    RTI::RTIambassador* rtia;

    try {
        // create the connection to HLA RTI
        rtia = new RTI::RTIambassador();

        RTIAmbassadorObject *self = (RTIAmbassadorObject *)type->tp_alloc(type, 0);
        self->ob_rtia = rtia;

        return (PyObject *)self;
    }
    catch(std::bad_alloc& exception) {
        PyErr_SetString(rti_MemoryExhausted, exception.what()); // new operator failed
    }
    CATCH_RTI_EXCEPTION(MemoryExhausted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    Py_INCREF(Py_None);
    return Py_None;
}

static void
rtia_dealloc(RTIAmbassadorObject *self)
{
    try {
        delete self->ob_rtia;
    }
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    self->ob_type->tp_free((PyObject *)self);
}

// Federation Management Services

// 4.2
static PyObject *
rtia_createFederationExecution(RTIAmbassadorObject *self, PyObject *args)
{
    const char* executionName;
    const char* FED;

    if(!PyArg_ParseTuple(args, "ss", &executionName, &FED))
        return NULL;

    try {
        self->ob_rtia->createFederationExecution(executionName, FED);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederationExecutionAlreadyExists)
    CATCH_RTI_EXCEPTION(CouldNotOpenFED)
    CATCH_RTI_EXCEPTION(ErrorReadingFED)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 4.3
static PyObject *
rtia_destroyFederationExecution(RTIAmbassadorObject *self, PyObject *args)
{
    const char *executionName;

    if(!PyArg_ParseTuple(args, "s", &executionName))
        return NULL;

    try {
        self->ob_rtia->destroyFederationExecution(executionName);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederatesCurrentlyJoined)
    CATCH_RTI_EXCEPTION(FederationExecutionDoesNotExist)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 4.4
static PyObject *
rtia_joinFederationExecution(RTIAmbassadorObject *self, PyObject *args)
{
    const char *yourName;
    const char *executionName;
    FederateAmbassadorObject *federateAmbassador;

    if(!PyArg_ParseTuple(args, "ssO!",
        &yourName,
        &executionName,
        &FederateAmbassadorObjectType, &federateAmbassador))
        return NULL;

    try {
        RTI::FederateHandle result = self->ob_rtia->joinFederationExecution(
            yourName, executionName, federateAmbassador->ob_federate);

        // keep the pointer that has been passed to RTIA
        self->ob_federate = (PyObject *)federateAmbassador;
        Py_INCREF(self->ob_federate);

        return RtiULongHandle_FromULong(&RtiFederateHandleType, result);
    }
    CATCH_RTI_EXCEPTION(FederateAlreadyExecutionMember)
    CATCH_RTI_EXCEPTION(FederationExecutionDoesNotExist)
    CATCH_RTI_EXCEPTION(CouldNotOpenFED)
    CATCH_RTI_EXCEPTION(ErrorReadingFED)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 4.5
static PyObject *
rtia_resignFederationExecution(RTIAmbassadorObject *self, PyObject *args)
{
    int theAction;

    if(!PyArg_ParseTuple(args, "i", &theAction))
        return NULL;

    try {
        self->ob_rtia->resignFederationExecution((RTI::ResignAction)theAction);

        // destroy the pointer stored in rtia_joinFederationExecution()
        Py_DECREF(self->ob_federate);
        self->ob_federate = NULL;

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateOwnsAttributes)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(InvalidResignAction)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 4.6
static PyObject *
rtia_registerFederationSynchronizationPoint(RTIAmbassadorObject *self, PyObject *args)
{
    const char *label;
    const char *theTag;
    RTI::FederateHandleSet *syncSet = NULL; // optional

    if(!PyArg_ParseTuple(args, "ss|O&",
        &label,
        &theTag,
        AttributeHandleSet_FromPython, &syncSet))
        return NULL;

    try {
        if(syncSet == NULL)
            self->ob_rtia->registerFederationSynchronizationPoint(label, theTag);
        else
            self->ob_rtia->registerFederationSynchronizationPoint(label, theTag, *syncSet);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 4.9
static PyObject *
rtia_synchronizationPointAchieved(RTIAmbassadorObject *self, PyObject *args)
{
    const char *label;

    if(!PyArg_ParseTuple(args, "s", &label))
        return NULL;

    try {
        self->ob_rtia->synchronizationPointAchieved(label);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(SynchronizationPointLabelWasNotAnnounced)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 4.11
static PyObject *
rtia_requestFederationSave(RTIAmbassadorObject *self, PyObject *args)
{
    const char *label;
    RTIfedTime theTime; // optional

    if(!PyArg_ParseTuple(args, "s|O&",
        &label,
        RTIfedTime_FromPython, &theTime))
        return NULL;

    try {
        if(PyTuple_Size(args) == 1)
            self->ob_rtia->requestFederationSave(label);
        else
            self->ob_rtia->requestFederationSave(label, theTime);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederationTimeAlreadyPassed)
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 4.13
static PyObject *
rtia_federateSaveBegun(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->federateSaveBegun();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(SaveNotInitiated)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 4.14
static PyObject *
rtia_federateSaveComplete(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->federateSaveComplete();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(SaveNotInitiated)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

static PyObject *
rtia_federateSaveNotComplete(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->federateSaveNotComplete();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(SaveNotInitiated)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 4.16
static PyObject *
rtia_requestFederationRestore(RTIAmbassadorObject *self, PyObject *args)
{
    const char *label;

    if(!PyArg_ParseTuple(args, "s", &label))
        return NULL;

    try {
        self->ob_rtia->requestFederationRestore(label);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 4.20
static PyObject *
rtia_federateRestoreComplete(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->federateRestoreComplete();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(RestoreNotRequested)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

static PyObject *
rtia_federateRestoreNotComplete(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->federateRestoreNotComplete();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(RestoreNotRequested)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// Declaration Management Services

// 5.2
static PyObject *
rtia_publishObjectClass(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theClass;
    RTI::AttributeHandleSet *attributeList;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectClassHandle_FromPython, &theClass,
        AttributeHandleSet_FromPython, &attributeList))
        return NULL;

    try {
        self->ob_rtia->publishObjectClass(theClass, *attributeList);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(OwnershipAcquisitionPending)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 5.3
static PyObject *
rtia_unpublishObjectClass(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theClass;

    if(!PyArg_ParseTuple(args, "O&",
        RtiObjectClassHandle_FromPython, &theClass))
        return NULL;

    try {
        self->ob_rtia->unpublishObjectClass(theClass);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(ObjectClassNotPublished)
    CATCH_RTI_EXCEPTION(OwnershipAcquisitionPending)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 5.4
static PyObject *
rtia_publishInteractionClass(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theInteraction;

    if(!PyArg_ParseTuple(args, "O&",
        RtiInteractionClassHandle_FromPython, &theInteraction))
        return NULL;

    try {
        self->ob_rtia->publishInteractionClass(theInteraction);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 5.5
static PyObject *
rtia_unpublishInteractionClass(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theInteraction;

    if(!PyArg_ParseTuple(args, "O&",
        RtiInteractionClassHandle_FromPython, &theInteraction))
        return NULL;

    try {
        self->ob_rtia->unpublishInteractionClass(theInteraction);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(InteractionClassNotPublished)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 5.6
static PyObject *
rtia_subscribeObjectClassAttributes(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theClass;
    RTI::AttributeHandleSet *attributeList;
    bool pyActive = true;

    if(!PyArg_ParseTuple(args, "O&O&|b",
        RtiObjectClassHandle_FromPython, &theClass,
        AttributeHandleSet_FromPython, &attributeList,
        &pyActive))
        return NULL;

    try {
        RTI::Boolean active = pyActive ? RTI::RTI_TRUE : RTI::RTI_FALSE;
        self->ob_rtia->subscribeObjectClassAttributes(theClass, *attributeList, active);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 5.7
static PyObject *
rtia_unsubscribeObjectClass(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theClass;

    if(!PyArg_ParseTuple(args, "O&",
        RtiObjectClassHandle_FromPython, &theClass))
        return NULL;

    try {
        self->ob_rtia->unsubscribeObjectClass(theClass);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(ObjectClassNotSubscribed)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 5.8
static PyObject *
rtia_subscribeInteractionClass(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theClass;
    bool pyActive = true;

    if(!PyArg_ParseTuple(args, "O&|b",
        RtiInteractionClassHandle_FromPython, &theClass,
        &pyActive))
        return NULL;

    try {
        RTI::Boolean active = pyActive ? RTI::RTI_TRUE : RTI::RTI_FALSE;
        self->ob_rtia->subscribeInteractionClass(theClass, active);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(FederateLoggingServiceCalls)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 5.9
static PyObject *
rtia_unsubscribeInteractionClass(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theClass;

    if(!PyArg_ParseTuple(args, "O&",
        RtiInteractionClassHandle_FromPython, &theClass))
        return NULL;

    try {
        self->ob_rtia->unsubscribeInteractionClass(theClass);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(InteractionClassNotSubscribed)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// Object Management Services

// 6.2
static PyObject *
rtia_registerObjectInstance(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theClass;
    const char *theObject = NULL; // optional

    if(!PyArg_ParseTuple(args, "O&|s",
        RtiObjectClassHandle_FromPython, &theClass,
        &theObject))
        return NULL;

    try {
        RTI::ObjectHandle result;

        if(theObject == NULL)
            result = self->ob_rtia->registerObjectInstance(theClass);
        else
            result = self->ob_rtia->registerObjectInstance(theClass, theObject);

        return RtiULongHandle_FromULong(&RtiObjectHandleType, result);
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(ObjectClassNotPublished)
    CATCH_RTI_EXCEPTION(ObjectAlreadyRegistered)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 6.4
static PyObject *
rtia_updateAttributeValues(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleValuePairSet *theAttributes;
    const char *theTag;
    RTIfedTime theTime; // optional

    if(!PyArg_ParseTuple(args, "O&O&s|O&",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleValuePairSet_FromPython, &theAttributes,
        &theTag,
        RTIfedTime_FromPython, &theTime))
        return NULL;

    try {
        if(PyTuple_Size(args) == 3) {
            self->ob_rtia->updateAttributeValues(theObject, *theAttributes, theTag);

            Py_INCREF(Py_None);
            return Py_None;
        }
        else {
            RTI::EventRetractionHandle result = self->ob_rtia->updateAttributeValues(theObject, *theAttributes, theTime, theTag);
            return EventRetractionHandle_ToPython(&result);
        }
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotOwned)
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 6.6
static PyObject *
rtia_sendInteraction(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theInteraction;
    RTI::ParameterHandleValuePairSet *theParameters;
    const char *theTag;
    RTIfedTime theTime; // optional

    if(!PyArg_ParseTuple(args, "O&O&s|d",
        RtiInteractionClassHandle_FromPython, &theInteraction,
        ParameterHandleValuePairSet_FromPython, &theParameters,
        &theTag,
        RTIfedTime_FromPython, &theTime))
        return NULL;

    try {
        if(PyTuple_Size(args) == 3) {
            self->ob_rtia->sendInteraction(theInteraction, *theParameters, theTag);

            Py_INCREF(Py_None);
            return Py_None;
        }
        else {
            RTI::EventRetractionHandle result = self->ob_rtia->sendInteraction(theInteraction, *theParameters, theTime, theTag);
            return EventRetractionHandle_ToPython(&result);
        }
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(InteractionClassNotPublished)
    CATCH_RTI_EXCEPTION(InteractionParameterNotDefined)
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 6.8
static PyObject *
rtia_deleteObjectInstance(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    const char *theTag;
    RTIfedTime theTime; // optional

    if(!PyArg_ParseTuple(args, "O&s|O&",
        RtiObjectHandle_FromPython, &theObject,
        &theTag,
        RTIfedTime_FromPython, &theTime))
        return NULL;

    try {
        if(PyTuple_Size(args) == 2) {
            self->ob_rtia->deleteObjectInstance(theObject, theTag);

            Py_INCREF(Py_None);
            return Py_None;
        }
        else {
            RTI::EventRetractionHandle result = self->ob_rtia->deleteObjectInstance(theObject, theTime, theTag);
            return EventRetractionHandle_ToPython(&result);
        }
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(DeletePrivilegeNotHeld)
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 6.10
static PyObject *
rtia_localDeleteObjectInstance(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;

    if(!PyArg_ParseTuple(args, "O&",
        RtiObjectHandle_FromPython, &theObject))
        return NULL;

    try {
        self->ob_rtia->localDeleteObjectInstance(theObject);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(FederateOwnsAttributes)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 6.11
static PyObject *
rtia_changeAttributeTransportationType(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *theAttributes;
    RTI::TransportationHandle theType;

    if(!PyArg_ParseTuple(args, "O&O&O&",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &theAttributes,
        RtiTransportationHandle_FromPython, &theType))
        return NULL;

    try {
        self->ob_rtia->changeAttributeTransportationType(theObject, *theAttributes, theType);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotOwned)
    CATCH_RTI_EXCEPTION(InvalidTransportationHandle)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 6.12
static PyObject *
rtia_changeInteractionTransportationType(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theClass;
    RTI::TransportationHandle theType;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiInteractionClassHandle_FromPython, &theClass,
        RtiTransportationHandle_FromPython, &theType))
        return NULL;

    try {
        self->ob_rtia->changeInteractionTransportationType(theClass, theType);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(InteractionClassNotPublished)
    CATCH_RTI_EXCEPTION(InvalidTransportationHandle)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 6.15
static PyObject *
rtia_requestObjectAttributeValueUpdate(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *theAttributes;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &theAttributes))
        return NULL;

    try {
        self->ob_rtia->requestObjectAttributeValueUpdate(theObject, *theAttributes);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

static PyObject *
rtia_requestClassAttributeValueUpdate(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theClass;
    RTI::AttributeHandleSet *theAttributes;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectClassHandle_FromPython, &theClass,
        AttributeHandleSet_FromPython, &theAttributes))
        return NULL;

    try {
        self->ob_rtia->requestClassAttributeValueUpdate(theClass, *theAttributes);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// Ownership Management Services

// 7.2
static PyObject *
rtia_unconditionalAttributeOwnershipDivestiture(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *theAttributes;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &theAttributes))
        return NULL;

    try {
        self->ob_rtia->unconditionalAttributeOwnershipDivestiture(theObject, *theAttributes);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotOwned)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 7.3
static PyObject *
rtia_negotiatedAttributeOwnershipDivestiture(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *theAttributes;
    const char *theTag;

    if(!PyArg_ParseTuple(args, "O&O&s",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &theAttributes,
        &theTag))
        return NULL;

    try {
        self->ob_rtia->negotiatedAttributeOwnershipDivestiture(theObject, *theAttributes, theTag);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotOwned)
    CATCH_RTI_EXCEPTION(AttributeAlreadyBeingDivested)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 7.7
static PyObject *
rtia_attributeOwnershipAcquisition(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *desiredAttributes;
    const char *theTag;

    if(!PyArg_ParseTuple(args, "O&O&s",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &desiredAttributes,
        &theTag))
        return NULL;

    try {
        self->ob_rtia->attributeOwnershipAcquisition(theObject, *desiredAttributes, theTag);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(ObjectClassNotPublished)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotPublished)
    CATCH_RTI_EXCEPTION(FederateOwnsAttributes)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 7.8
static PyObject *
rtia_attributeOwnershipAcquisitionIfAvailable(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *desiredAttributes;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &desiredAttributes))
        return NULL;

    try {
        self->ob_rtia->attributeOwnershipAcquisitionIfAvailable(theObject, *desiredAttributes);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(ObjectClassNotPublished)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotPublished)
    CATCH_RTI_EXCEPTION(FederateOwnsAttributes)
    CATCH_RTI_EXCEPTION(AttributeAlreadyBeingAcquired)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 7.11
static PyObject *
rtia_attributeOwnershipReleaseResponse(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *theAttributes;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &theAttributes))
        return NULL;

    try {
        RTI::AttributeHandleSet *result = self->ob_rtia->attributeOwnershipReleaseResponse(theObject, *theAttributes);

        return AttributeHandleSet_ToPython(result);
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotOwned)
    CATCH_RTI_EXCEPTION(FederateWasNotAskedToReleaseAttribute)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 7.12
static PyObject *
rtia_cancelNegotiatedAttributeOwnershipDivestiture(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *theAttributes;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &theAttributes))
        return NULL;

    try {
        self->ob_rtia->cancelNegotiatedAttributeOwnershipDivestiture(theObject, *theAttributes);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotOwned)
    CATCH_RTI_EXCEPTION(AttributeDivestitureWasNotRequested)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 7.13
static PyObject *
rtia_cancelAttributeOwnershipAcquisition(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *theAttributes;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &theAttributes))
        return NULL;

    try {
        self->ob_rtia->cancelAttributeOwnershipAcquisition(theObject, *theAttributes);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeAlreadyOwned)
    CATCH_RTI_EXCEPTION(AttributeAcquisitionWasNotRequested)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 7.15
static PyObject *
rtia_queryAttributeOwnership(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandle theAttribute;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectHandle_FromPython, &theObject,
        RtiAttributeHandle_FromPython, &theAttribute))
        return NULL;

    try {
        self->ob_rtia->queryAttributeOwnership(theObject, theAttribute);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 7.17
static PyObject *
rtia_isAttributeOwnedByFederate(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandle theAttribute;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectHandle_FromPython, &theObject,
        RtiAttributeHandle_FromPython, &theAttribute))
        return NULL;

    try {
        RTI::Boolean result = self->ob_rtia->isAttributeOwnedByFederate(theObject, theAttribute);

        return PyBool_FromLong(result);
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// Time Management Services

// 8.2
static PyObject *
rtia_enableTimeRegulation(RTIAmbassadorObject *self, PyObject *args)
{
    RTIfedTime theFederateTime;
    RTIfedTime theLookahead;

    if(!PyArg_ParseTuple(args, "O&O&",
        RTIfedTime_FromPython, &theFederateTime,
        RTIfedTime_FromPython, &theLookahead))
        return NULL;

    try {
        self->ob_rtia->enableTimeRegulation(theFederateTime, theLookahead);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(TimeRegulationAlreadyEnabled)
    CATCH_RTI_EXCEPTION(EnableTimeRegulationPending)
    CATCH_RTI_EXCEPTION(TimeAdvanceAlreadyInProgress)
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(InvalidLookahead)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.4
static PyObject *
rtia_disableTimeRegulation(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->disableTimeRegulation();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(TimeRegulationWasNotEnabled)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.5
static PyObject *
rtia_enableTimeConstrained(RTIAmbassadorObject *self, PyObject *args)
{
    try {
        self->ob_rtia->enableTimeConstrained();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(TimeConstrainedAlreadyEnabled)
    CATCH_RTI_EXCEPTION(EnableTimeConstrainedPending)
    CATCH_RTI_EXCEPTION(TimeAdvanceAlreadyInProgress)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.7
static PyObject *
rtia_disableTimeConstrained(RTIAmbassadorObject *self, PyObject *args)
{
    try {
        self->ob_rtia->disableTimeConstrained();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(TimeConstrainedWasNotEnabled)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.8
static PyObject *
rtia_timeAdvanceRequest(RTIAmbassadorObject *self, PyObject *args)
{
    RTIfedTime theTime;

    if(!PyArg_ParseTuple(args, "O&",
        RTIfedTime_FromPython, &theTime))
        return NULL;

    try {
        self->ob_rtia->timeAdvanceRequest(theTime);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(FederationTimeAlreadyPassed)
    CATCH_RTI_EXCEPTION(TimeAdvanceAlreadyInProgress)
    CATCH_RTI_EXCEPTION(EnableTimeRegulationPending)
    CATCH_RTI_EXCEPTION(EnableTimeConstrainedPending)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.9
static PyObject *
rtia_timeAdvanceRequestAvailable(RTIAmbassadorObject *self, PyObject *args)
{
    RTIfedTime theTime;

    if(!PyArg_ParseTuple(args, "O&",
        RTIfedTime_FromPython, &theTime))
        return NULL;

    try {
        self->ob_rtia->timeAdvanceRequestAvailable(theTime);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(FederationTimeAlreadyPassed)
    CATCH_RTI_EXCEPTION(TimeAdvanceAlreadyInProgress)
    CATCH_RTI_EXCEPTION(EnableTimeRegulationPending)
    CATCH_RTI_EXCEPTION(EnableTimeConstrainedPending)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.10
static PyObject *
rtia_nextEventRequest(RTIAmbassadorObject *self, PyObject *args)
{
    RTIfedTime theTime;

    if(!PyArg_ParseTuple(args, "O&",
        RTIfedTime_FromPython, &theTime))
        return NULL;

    try {
        self->ob_rtia->nextEventRequest(theTime);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(FederationTimeAlreadyPassed)
    CATCH_RTI_EXCEPTION(TimeAdvanceAlreadyInProgress)
    CATCH_RTI_EXCEPTION(EnableTimeRegulationPending)
    CATCH_RTI_EXCEPTION(EnableTimeConstrainedPending)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.11
static PyObject *
rtia_nextEventRequestAvailable(RTIAmbassadorObject *self, PyObject *args)
{
    RTIfedTime theTime;

    if(!PyArg_ParseTuple(args, "O&",
        RTIfedTime_FromPython, &theTime))
        return NULL;

    try {
        self->ob_rtia->nextEventRequestAvailable(theTime);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(FederationTimeAlreadyPassed)
    CATCH_RTI_EXCEPTION(TimeAdvanceAlreadyInProgress)
    CATCH_RTI_EXCEPTION(EnableTimeRegulationPending)
    CATCH_RTI_EXCEPTION(EnableTimeConstrainedPending)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.12
static PyObject *
rtia_flushQueueRequest(RTIAmbassadorObject *self, PyObject *args)
{
    RTIfedTime theTime;

    if(!PyArg_ParseTuple(args, "O&",
        RTIfedTime_FromPython, &theTime))
        return NULL;

    try {
        self->ob_rtia->flushQueueRequest(theTime);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(FederationTimeAlreadyPassed)
    CATCH_RTI_EXCEPTION(TimeAdvanceAlreadyInProgress)
    CATCH_RTI_EXCEPTION(EnableTimeRegulationPending)
    CATCH_RTI_EXCEPTION(EnableTimeConstrainedPending)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.14
static PyObject *
rtia_enableAsynchronousDelivery(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->enableAsynchronousDelivery();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(AsynchronousDeliveryAlreadyEnabled)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.15
static PyObject *
rtia_disableAsynchronousDelivery(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->disableAsynchronousDelivery();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(AsynchronousDeliveryAlreadyDisabled)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.16
static PyObject *
rtia_queryLBTS(RTIAmbassadorObject *self)
{
    try {
        RTIfedTime theTime;
        self->ob_rtia->queryLBTS(theTime);

        return PyFloat_FromDouble(theTime.getTime());
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.17
static PyObject *
rtia_queryFederateTime(RTIAmbassadorObject *self)
{
    try {
        RTIfedTime theTime;
        self->ob_rtia->queryFederateTime(theTime);

        return PyFloat_FromDouble(theTime.getTime());
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.18
static PyObject *
rtia_queryMinNextEventTime(RTIAmbassadorObject *self)
{
    try {
        RTIfedTime theTime;
        self->ob_rtia->queryMinNextEventTime(theTime);

        return PyFloat_FromDouble(theTime.getTime());
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.19
static PyObject *
rtia_modifyLookahead(RTIAmbassadorObject *self, PyObject *args)
{
    RTIfedTime theLookahead;

    if(!PyArg_ParseTuple(args, "O&",
        RTIfedTime_FromPython, &theLookahead))
        return NULL;

    try {
        self->ob_rtia->modifyLookahead(theLookahead);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InvalidLookahead)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.20
static PyObject *
rtia_queryLookahead(RTIAmbassadorObject *self)
{
    try {
        RTIfedTime theTime;
        self->ob_rtia->queryLookahead(theTime);

        return PyFloat_FromDouble(theTime.getTime());
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.21
static PyObject *
rtia_retract(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::EventRetractionHandle theHandle;

    if(!PyArg_ParseTuple(args, "O&",
        EventRetractionHandle_FromPython, &theHandle))
        return NULL;

    try {
        self->ob_rtia->retract(theHandle);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InvalidRetractionHandle)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.23
static PyObject *
rtia_changeAttributeOrderType(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *theAttributes;
    RTI::OrderingHandle theType;

    if(!PyArg_ParseTuple(args, "O&O&O&",
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &theAttributes,
        RtiOrderingHandle_FromPython, &theType))
        return NULL;

    try {
        self->ob_rtia->changeAttributeOrderType(theObject, *theAttributes, theType);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotOwned)
    CATCH_RTI_EXCEPTION(InvalidOrderingHandle)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 8.24
static PyObject *
rtia_changeInteractionOrderType(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theClass;
    RTI::OrderingHandle theType;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiInteractionClassHandle_FromPython, &theClass,
        RtiOrderingHandle_FromPython, &theType))
        return NULL;

    try {
        self->ob_rtia->changeInteractionOrderType(theClass, theType);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(InteractionClassNotPublished)
    CATCH_RTI_EXCEPTION(InvalidOrderingHandle)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// Data Distribution Management

// 9.2
static PyObject *
rtia_createRegion(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::SpaceHandle theSpace;
    PyObject *theDimensions;

    if(!PyArg_ParseTuple(args, "O&O",
        RtiSpaceHandle_FromPython, &theSpace,
        &theDimensions))
        return NULL;

    if(!PySequence_Check(theDimensions)) {
        PyErr_SetString(PyExc_TypeError,
            "sequence [DimensionHandle,(int,int)] required");
        return NULL;
    }

    try {
        RTI::ULong numberOfExtents = PySequence_Size(theDimensions);
        RTI::Region *regionData = self->ob_rtia->createRegion(theSpace, numberOfExtents);

        for (RTI::ULong pos = 0; pos < numberOfExtents; pos++) {
            RTI::DimensionHandle dimension;
            unsigned long lowerBound;
            unsigned long upperBound;

            auto_decref<PyObject> item = PySequence_GetItem(theDimensions, pos);

            if(!PyArg_ParseTuple(item, "O&(kk)",
                RtiDimensionHandle_FromPython, &dimension,
                &lowerBound,
                &upperBound))
                return 0; // failure

            regionData->setRangeLowerBound(pos, dimension, lowerBound);
            regionData->setRangeUpperBound(pos, dimension, upperBound);
        }

        RegionHandleObject *result =
            PyObject_New(RegionHandleObject, &RegionHandleType);
        if (result == NULL)
            return NULL;
        // keep also the handle
        // this is to facilitate hashing and forward compatibility with IEEE1516
        result->ob_handle = self->ob_rtia->getRegionToken(regionData);
        result->ob_value = regionData;

        return (PyObject *)result;
    }
    CATCH_RTI_EXCEPTION(SpaceNotDefined)
    CATCH_RTI_EXCEPTION(InvalidExtents)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.3
static PyObject *
rtia_notifyAboutRegionModification(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::Region *theRegion;

    if(!PyArg_ParseTuple(args, "O&",
        RegionHandle_FromPython, &theRegion))
        return NULL;

    try {
        self->ob_rtia->notifyAboutRegionModification(*theRegion);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(InvalidExtents)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.4
static PyObject *
rtia_deleteRegion(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::Region *theRegion;

    if(!PyArg_ParseTuple(args, "O&",
        RegionHandle_FromPython, &theRegion))
        return NULL;

    try {
        self->ob_rtia->deleteRegion(theRegion);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(RegionInUse)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.5
static PyObject *
rtia_registerObjectInstanceWithRegion(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theClass;
    PyObject *attributesAndRegions;
    const char *theObject = NULL; // optional

    if(!PyArg_ParseTuple(args, "O&O|s",
        RtiObjectClassHandle_FromPython, &theClass,
        &attributesAndRegions,
        &theObject))
        return NULL;

    try {
        RTI::ULong theNumberOfHandles = PySequence_Size(attributesAndRegions);

        // note, std::auto_ptr cannot handle arrays
        auto_arrayptr<RTI::AttributeHandle> theAttributes = new RTI::AttributeHandle[theNumberOfHandles];
        auto_arrayptr<RTI::Region *> theRegions = new RTI::Region *[theNumberOfHandles];

        for (RTI::ULong pos = 0; pos < theNumberOfHandles; pos++) {
            auto_decref<PyObject> item = PySequence_GetItem(attributesAndRegions, pos);

            if(!PyArg_ParseTuple(item, "O&O&",
                RtiAttributeHandle_FromPython, theAttributes+pos,
                RegionHandle_FromPython, theRegions+pos))
                return NULL;
        }

        RTI::ObjectHandle result;
        if (theObject == NULL)
            result = self->ob_rtia->registerObjectInstanceWithRegion(
                theClass, theAttributes, theRegions, theNumberOfHandles);
        else
            result = self->ob_rtia->registerObjectInstanceWithRegion(
                theClass, theObject, theAttributes, theRegions, theNumberOfHandles);

        return RtiULongHandle_FromULong(&RtiObjectHandleType, result);
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(ObjectClassNotPublished)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotPublished)
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(InvalidRegionContext)
    CATCH_RTI_EXCEPTION(ObjectAlreadyRegistered)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.6
static PyObject *
rtia_associateRegionForUpdates(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::Region *theRegion;
    RTI::ObjectHandle theObject;
    RTI::AttributeHandleSet *theAttributes;

    if(!PyArg_ParseTuple(args, "O&O&O&",
        RegionHandle_FromPython, &theRegion,
        RtiObjectHandle_FromPython, &theObject,
        AttributeHandleSet_FromPython, &theAttributes))
        return NULL;

    try {
        self->ob_rtia->associateRegionForUpdates(*theRegion, theObject, *theAttributes);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(InvalidRegionContext)
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.7
static PyObject *
rtia_unassociateRegionForUpdates(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::Region *theRegion;
    RTI::ObjectHandle theObject;

    if(!PyArg_ParseTuple(args, "O&O&",
        RegionHandle_FromPython, &theRegion,
        RtiObjectHandle_FromPython, &theObject))
        return NULL;

    try {
        self->ob_rtia->unassociateRegionForUpdates(*theRegion, theObject);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(InvalidRegionContext)
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.8
static PyObject *
rtia_subscribeObjectClassAttributesWithRegion(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theClass;
    RTI::Region *theRegion;
    RTI::AttributeHandleSet *attributeList;
    bool pyActive = true;

    if(!PyArg_ParseTuple(args, "O&O&O&|b",
        RtiObjectClassHandle_FromPython, &theClass,
        RegionHandle_FromPython, &theRegion,
        AttributeHandleSet_FromPython, &attributeList,
        &pyActive))
        return NULL;

    try {
        RTI::Boolean active = pyActive ? RTI::RTI_TRUE : RTI::RTI_FALSE;
        self->ob_rtia->subscribeObjectClassAttributesWithRegion(theClass, *theRegion, *attributeList, active);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(InvalidRegionContext)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.9
static PyObject *
rtia_unsubscribeObjectClassWithRegion(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theClass;
    RTI::Region *theRegion;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiObjectClassHandle_FromPython, &theClass,
        RegionHandle_FromPython, &theRegion))
        return NULL;

    try {
        self->ob_rtia->unsubscribeObjectClassWithRegion(theClass, *theRegion);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(ObjectClassNotSubscribed)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.10
static PyObject *
rtia_subscribeInteractionClassWithRegion(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theClass;
    RTI::Region *theRegion;
    bool pyActive = true;

    if(!PyArg_ParseTuple(args, "O&O&|b",
        RtiInteractionClassHandle_FromPython, &theClass,
        RegionHandle_FromPython, &theRegion,
        &pyActive))
        return NULL;

    try {
        RTI::Boolean active = pyActive ? RTI::RTI_TRUE : RTI::RTI_FALSE;
        self->ob_rtia->subscribeInteractionClassWithRegion(theClass, *theRegion, active);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(InvalidRegionContext)
    CATCH_RTI_EXCEPTION(FederateLoggingServiceCalls)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.11
static PyObject *
rtia_unsubscribeInteractionClassWithRegion(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theClass;
    RTI::Region *theRegion;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiInteractionClassHandle_FromPython, &theClass,
        RegionHandle_FromPython, &theRegion))
        return NULL;

    try {
        self->ob_rtia->unsubscribeInteractionClassWithRegion(theClass, *theRegion);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(InteractionClassNotSubscribed)
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.12
static PyObject *
rtia_sendInteractionWithRegion(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theInteraction;
    RTI::ParameterHandleValuePairSet *theParameters;
    RTI::Region *theRegion;
    const char *theTag;
    RTIfedTime theTime; // optional

    if(!PyArg_ParseTuple(args, "O&O&O&s|O&",
        RtiInteractionClassHandle_FromPython, &theInteraction,
        ParameterHandleValuePairSet_FromPython, &theParameters,
        RegionHandle_FromPython, &theRegion,
        &theTag,
        RTIfedTime_FromPython, &theTime))
        return NULL;

    try {
        if(PyTuple_Size(args) == 4) {
            self->ob_rtia->sendInteractionWithRegion(
                theInteraction, *theParameters, theTag, *theRegion);

            Py_INCREF(Py_None);
            return Py_None;
        }
        else {
            RTI::EventRetractionHandle result = self->ob_rtia->sendInteractionWithRegion(
                theInteraction, *theParameters, theTime, theTag, *theRegion);
            return EventRetractionHandle_ToPython(&result);
        }
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(InteractionClassNotPublished)
    CATCH_RTI_EXCEPTION(InteractionParameterNotDefined)
    CATCH_RTI_EXCEPTION(InvalidFederationTime)
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(InvalidRegionContext)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 9.13
static PyObject *
rtia_requestClassAttributeValueUpdateWithRegion(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theClass;
    RTI::AttributeHandleSet *theAttributes;
    RTI::Region *theRegion;

    if(!PyArg_ParseTuple(args, "O&O&O&",
        RtiObjectClassHandle_FromPython, &theClass,
        AttributeHandleSet_FromPython, &theAttributes,
        RegionHandle_FromPython, &theRegion))
        return NULL;

    try {
        self->ob_rtia->requestClassAttributeValueUpdateWithRegion(theClass, *theAttributes, *theRegion);

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(RegionNotKnown)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// RTI Support Services

// 10.2
static PyObject *
rtia_getObjectClassHandle(RTIAmbassadorObject *self, PyObject *args)
{
    const char *theName;

    if(!PyArg_ParseTuple(args, "s", &theName))
        return NULL;

    try {
        RTI::ObjectClassHandle result = self->ob_rtia->getObjectClassHandle(theName);

        return RtiULongHandle_FromULong(&RtiObjectClassHandleType, result);
    }
    CATCH_RTI_EXCEPTION(NameNotFound)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.3
static PyObject *
rtia_getObjectClassName(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectClassHandle theHandle;

    if(!PyArg_ParseTuple(args, "O&",
        RtiObjectClassHandle_FromPython, &theHandle))
        return NULL;

    try {
        char *theName = self->ob_rtia->getObjectClassName(theHandle);

        PyObject *result = PyString_FromString(theName);
        delete[] theName;

        return result;
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.4
static PyObject *
rtia_getAttributeHandle(RTIAmbassadorObject *self, PyObject *args)
{
    const char *theName;
    RTI::ObjectClassHandle whichClass;

    if(!PyArg_ParseTuple(args, "sO&",
        &theName,
        RtiObjectClassHandle_FromPython, &whichClass))
        return NULL;

    try {
        RTI::AttributeHandle result = self->ob_rtia->getAttributeHandle(theName, whichClass);

        return RtiULongHandle_FromULong(&RtiAttributeHandleType, result);
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(NameNotFound)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.5
static PyObject *
rtia_getAttributeName(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::AttributeHandle theHandle;
    RTI::ObjectClassHandle whichClass;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiAttributeHandle_FromPython, &theHandle,
        RtiObjectClassHandle_FromPython, &whichClass))
        return NULL;

    try {
        char *theName = self->ob_rtia->getAttributeName(theHandle, whichClass);

        PyObject *result = PyString_FromString(theName);
        delete[] theName;

        return result;
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.6
static PyObject *
rtia_getInteractionClassHandle(RTIAmbassadorObject *self, PyObject *args)
{
    const char *theName;

    if(!PyArg_ParseTuple(args, "s", &theName))
        return NULL;

    try {
        RTI::InteractionClassHandle result = self->ob_rtia->getInteractionClassHandle(theName);

        return RtiInteractionClassHandle_ToPython(&result);
    }
    CATCH_RTI_EXCEPTION(NameNotFound)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.7
static PyObject *
rtia_getInteractionClassName(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theHandle;

    if(!PyArg_ParseTuple(args, "O&",
        RtiInteractionClassHandle_FromPython, &theHandle))
        return NULL;

    try {
        char *theName = self->ob_rtia->getInteractionClassName(theHandle);

        PyObject *result = PyString_FromString(theName);
        delete[] theName;

        return result;
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.8
static PyObject *
rtia_getParameterHandle(RTIAmbassadorObject *self, PyObject *args)
{
    const char *theName;
    RTI::InteractionClassHandle whichClass;

    if(!PyArg_ParseTuple(args, "sO&",
        &theName,
        RtiInteractionClassHandle_FromPython, &whichClass))
        return NULL;

    try {
        RTI::ParameterHandle result = self->ob_rtia->getParameterHandle(theName, whichClass);

        return RtiParameterHandle_ToPython(&result);
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(NameNotFound)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.9
static PyObject *
rtia_getParameterName(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ParameterHandle theHandle;
    RTI::InteractionClassHandle whichClass;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiParameterHandle_FromPython, &theHandle,
        RtiInteractionClassHandle_FromPython, &whichClass))
        return NULL;

    try {
        char *theName = self->ob_rtia->getParameterName(theHandle, whichClass);

        PyObject *result = PyString_FromString(theName);
        delete[] theName;

        return result;
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(InteractionParameterNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.10
static PyObject *
rtia_getObjectInstanceHandle(RTIAmbassadorObject *self, PyObject *args)
{
    const char *theName;

    if(!PyArg_ParseTuple(args, "s", &theName))
        return NULL;

    try {
        RTI::ObjectHandle result = self->ob_rtia->getObjectInstanceHandle(theName);

        return RtiObjectHandle_ToPython(&result);
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.11
static PyObject *
rtia_getObjectInstanceName(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theHandle;

    if(!PyArg_ParseTuple(args, "O&",
        RtiObjectHandle_FromPython, &theHandle))
        return NULL;

    try {
        char *theName = self->ob_rtia->getObjectInstanceName(theHandle);

        PyObject *result = PyString_FromString(theName);
        delete[] theName;

        return result;
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.12
static PyObject *
rtia_getRoutingSpaceHandle(RTIAmbassadorObject *self, PyObject *args)
{
    const char *theName;

    if(!PyArg_ParseTuple(args, "s", &theName))
        return NULL;

    try {
        RTI::SpaceHandle result = self->ob_rtia->getRoutingSpaceHandle(theName);

        return RtiSpaceHandle_ToPython(&result);
    }
    CATCH_RTI_EXCEPTION(NameNotFound)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.13
static PyObject *
rtia_getRoutingSpaceName(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::SpaceHandle theHandle;

    if(!PyArg_ParseTuple(args, "O&",
        RtiSpaceHandle_FromPython, &theHandle))
        return NULL;

    try {
        char *theName = self->ob_rtia->getRoutingSpaceName(theHandle);

        PyObject *result = PyString_FromString(theName);
        delete[] theName;

        return result;
    }
    CATCH_RTI_EXCEPTION(SpaceNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.14
static PyObject *
rtia_getDimensionHandle(RTIAmbassadorObject *self, PyObject *args)
{
    const char *theName;
    RTI::SpaceHandle whichSpace;

    if(!PyArg_ParseTuple(args, "sO&",
        &theName,
        RtiSpaceHandle_FromPython, &whichSpace))
        return NULL;

    try {
        RTI::DimensionHandle result = self->ob_rtia->getDimensionHandle(theName, whichSpace);

        return RtiDimensionHandle_ToPython(&result);
    }
    CATCH_RTI_EXCEPTION(SpaceNotDefined)
    CATCH_RTI_EXCEPTION(NameNotFound)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.15
static PyObject *
rtia_getDimensionName(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::DimensionHandle theHandle;
    RTI::SpaceHandle whichSpace;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiDimensionHandle_FromPython, &theHandle,
        RtiSpaceHandle_FromPython, &whichSpace))
        return NULL;

    try {
        char *theName = self->ob_rtia->getDimensionName(theHandle, whichSpace);

        PyObject *result = PyString_FromString(theName);
        delete[] theName;

        return result;
    }
    CATCH_RTI_EXCEPTION(SpaceNotDefined)
    CATCH_RTI_EXCEPTION(DimensionNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.16
static PyObject *
rtia_getAttributeRoutingSpaceHandle(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::AttributeHandle theHandle;
    RTI::ObjectClassHandle whichClass;

    if(!PyArg_ParseTuple(args, "O&O&",
        RtiAttributeHandle_FromPython, &theHandle,
        RtiObjectClassHandle_FromPython, &whichClass))
        return NULL;

    try {
        RTI::SpaceHandle result = self->ob_rtia->getAttributeRoutingSpaceHandle(theHandle, whichClass);

        return RtiSpaceHandle_ToPython(&result);
    }
    CATCH_RTI_EXCEPTION(ObjectClassNotDefined)
    CATCH_RTI_EXCEPTION(AttributeNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.17
static PyObject *
rtia_getObjectClass(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::ObjectHandle theObject;

    if(!PyArg_ParseTuple(args, "O&",
        RtiObjectHandle_FromPython, &theObject))
        return NULL;

    try {
        RTI::ObjectClassHandle result = self->ob_rtia->getObjectClass(theObject);

        return RtiObjectClassHandle_ToPython(&result);
    }
    CATCH_RTI_EXCEPTION(ObjectNotKnown)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.18
static PyObject *
rtia_getInteractionRoutingSpaceHandle(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::InteractionClassHandle theHandle;

    if(!PyArg_ParseTuple(args, "O&",
        RtiInteractionClassHandle_FromPython, &theHandle))
        return NULL;

    try {
        RTI::SpaceHandle result = self->ob_rtia->getInteractionRoutingSpaceHandle(theHandle);

        return RtiSpaceHandle_ToPython(&result);
    }
    CATCH_RTI_EXCEPTION(InteractionClassNotDefined)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.19
static PyObject *
rtia_getTransportationHandle(RTIAmbassadorObject *self, PyObject *args)
{
    const char *theName;

    if(!PyArg_ParseTuple(args, "s", &theName))
        return NULL;

    try {
        RTI::TransportationHandle result = self->ob_rtia->getTransportationHandle(theName);

        return RtiTransportationHandle_ToPython(&result);
    }
    CATCH_RTI_EXCEPTION(NameNotFound)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.20
static PyObject *
rtia_getTransportationName(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::TransportationHandle theHandle;

    if(!PyArg_ParseTuple(args, "O&",
        RtiTransportationHandle_FromPython, &theHandle))
        return NULL;

    try {
        char *theName = self->ob_rtia->getTransportationName(theHandle);

        PyObject *result = PyString_FromString(theName);
        delete[] theName;

        return result;
    }
    CATCH_RTI_EXCEPTION(InvalidTransportationHandle)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.21
static PyObject *
rtia_getOrderingHandle(RTIAmbassadorObject *self, PyObject *args)
{
    const char *theName;

    if(!PyArg_ParseTuple(args, "s", &theName))
        return NULL;

    try {
        RTI::OrderingHandle result = self->ob_rtia->getOrderingHandle(theName);

        return RtiOrderingHandle_ToPython(&result);
    }
    CATCH_RTI_EXCEPTION(NameNotFound)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.22
static PyObject *
rtia_getOrderingName(RTIAmbassadorObject *self, PyObject *args)
{
    RTI::OrderingHandle theHandle;

    if(!PyArg_ParseTuple(args, "O&",
        RtiOrderingHandle_FromPython, &theHandle))
        return NULL;

    try {
        char *theName = self->ob_rtia->getOrderingName(theHandle);

        PyObject *result = PyString_FromString(theName);
        delete[] theName;

        return result;
    }
    CATCH_RTI_EXCEPTION(InvalidOrderingHandle)
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.23
static PyObject *
rtia_enableClassRelevanceAdvisorySwitch(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->enableClassRelevanceAdvisorySwitch();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.24
static PyObject *
rtia_disableClassRelevanceAdvisorySwitch(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->disableClassRelevanceAdvisorySwitch();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.25
static PyObject *
rtia_enableAttributeRelevanceAdvisorySwitch(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->enableAttributeRelevanceAdvisorySwitch();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.26
static PyObject *
rtia_disableAttributeRelevanceAdvisorySwitch(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->disableAttributeRelevanceAdvisorySwitch();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.27
static PyObject *
rtia_enableAttributeScopeAdvisorySwitch(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->enableAttributeScopeAdvisorySwitch();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.28
static PyObject *
rtia_disableAttributeScopeAdvisorySwitch(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->disableAttributeScopeAdvisorySwitch();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.29
static PyObject *
rtia_enableInteractionRelevanceAdvisorySwitch(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->enableInteractionRelevanceAdvisorySwitch();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

// 10.30
static PyObject *
rtia_disableInteractionRelevanceAdvisorySwitch(RTIAmbassadorObject *self)
{
    try {
        self->ob_rtia->disableInteractionRelevanceAdvisorySwitch();

        Py_INCREF(Py_None);
        return Py_None;
    }
    CATCH_RTI_EXCEPTION(FederateNotExecutionMember)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION(SaveInProgress)
    CATCH_RTI_EXCEPTION(RestoreInProgress)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

//
static PyObject *
__rtia_tick_1(RTIAmbassadorObject *self, PyObject *args)
{
    try {
        RTI::Boolean result = self->ob_rtia->tick();

        return PyBool_FromLong(result);
    }
    CATCH_RTI_EXCEPTION(SpecifiedSaveLabelDoesNotExist)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

static PyObject *
__rtia_tick_2(RTIAmbassadorObject *self, PyObject *args)
{
    double minimum = 0.0;
    double maximum = 0.0;

    if(!PyArg_ParseTuple(args, "dd", &minimum, &maximum))
        return NULL;

    try {
        RTI::Boolean result = self->ob_rtia->tick(minimum, maximum);

        return PyBool_FromLong(result);
    }
    CATCH_RTI_EXCEPTION(SpecifiedSaveLabelDoesNotExist)
    CATCH_RTI_EXCEPTION(ConcurrentAccessAttempted)
    CATCH_RTI_EXCEPTION2(RTI::Exception, rti_RTIInternalError)

    return NULL;
}

static PyObject *
rtia_tick(RTIAmbassadorObject *self, PyObject *args)
{
    if(PyTuple_Size(args) == 0)
        return __rtia_tick_1(self, args);
    else
        return __rtia_tick_2(self, args);
}

static PyMethodDef rtia_methods[] =
{
    {"createFederationExecution",
        (PyCFunction)rtia_createFederationExecution, METH_VARARGS,
        ""},
    {"destroyFederationExecution",
        (PyCFunction)rtia_destroyFederationExecution, METH_VARARGS,
        ""},
    {"joinFederationExecution",
        (PyCFunction)rtia_joinFederationExecution, METH_VARARGS,
        ""},
    {"resignFederationExecution",
        (PyCFunction)rtia_resignFederationExecution, METH_VARARGS,
        ""},
    {"registerFederationSynchronizationPoint",
        (PyCFunction)rtia_registerFederationSynchronizationPoint, METH_VARARGS,
        ""},       
    {"synchronizationPointAchieved",
        (PyCFunction)rtia_synchronizationPointAchieved, METH_VARARGS,
        ""},      
    {"requestFederationSave",
        (PyCFunction)rtia_requestFederationSave, METH_VARARGS,
        ""},    
    {"federateSaveBegun",
        (PyCFunction)rtia_federateSaveBegun, METH_NOARGS,
        ""},
    {"federateSaveComplete",
        (PyCFunction)rtia_federateSaveComplete, METH_NOARGS,
        ""},
    {"federateSaveNotComplete",
        (PyCFunction)rtia_federateSaveNotComplete, METH_NOARGS,
        ""},
    {"requestFederationRestore",
        (PyCFunction)rtia_requestFederationRestore, METH_VARARGS,
        ""},    
    {"federateRestoreComplete",
        (PyCFunction)rtia_federateRestoreComplete, METH_NOARGS,
        ""},
    {"federateRestoreNotComplete",
        (PyCFunction)rtia_federateRestoreNotComplete, METH_NOARGS,
        ""},
    {"publishObjectClass",
        (PyCFunction)rtia_publishObjectClass, METH_VARARGS,
        ""},
    {"unpublishObjectClass",
        (PyCFunction)rtia_unpublishObjectClass, METH_VARARGS,
        ""},
    {"publishInteractionClass",
        (PyCFunction)rtia_publishInteractionClass, METH_VARARGS,
        ""},
    {"unpublishInteractionClass",
        (PyCFunction)rtia_unpublishInteractionClass, METH_VARARGS,
        ""},
    {"subscribeObjectClassAttributes",
        (PyCFunction)rtia_subscribeObjectClassAttributes, METH_VARARGS,
        ""},
    {"unsubscribeObjectClass",
        (PyCFunction)rtia_unsubscribeObjectClass, METH_VARARGS,
        ""},
    {"subscribeInteractionClass",
        (PyCFunction)rtia_subscribeInteractionClass, METH_VARARGS,
        ""},
    {"unsubscribeInteractionClass",
        (PyCFunction)rtia_unsubscribeInteractionClass, METH_VARARGS,
        ""},
    {"registerObjectInstance",
        (PyCFunction)rtia_registerObjectInstance, METH_VARARGS,
        ""},
    {"updateAttributeValues",
        (PyCFunction)rtia_updateAttributeValues, METH_VARARGS,
        ""},
    {"updateAttributeValues",
        (PyCFunction)rtia_updateAttributeValues, METH_VARARGS,
        ""},
    {"sendInteraction",
        (PyCFunction)rtia_sendInteraction, METH_VARARGS,
        ""},
    {"deleteObjectInstance",
        (PyCFunction)rtia_deleteObjectInstance, METH_VARARGS,
        ""},
    {"localDeleteObjectInstance",
        (PyCFunction)rtia_localDeleteObjectInstance, METH_VARARGS,
        ""},
    {"changeAttributeTransportationType",
        (PyCFunction)rtia_changeAttributeTransportationType, METH_VARARGS,
        ""},
    {"changeInteractionTransportationType",
        (PyCFunction)rtia_changeInteractionTransportationType, METH_VARARGS,
        ""},
    {"requestObjectAttributeValueUpdate",
        (PyCFunction)rtia_requestObjectAttributeValueUpdate, METH_VARARGS,
        ""},
    {"requestClassAttributeValueUpdate",
        (PyCFunction)rtia_requestClassAttributeValueUpdate, METH_VARARGS,
        ""},
    {"unconditionalAttributeOwnershipDivestiture",
        (PyCFunction)rtia_unconditionalAttributeOwnershipDivestiture, METH_VARARGS,
        ""},
    {"negotiatedAttributeOwnershipDivestiture",
        (PyCFunction)rtia_negotiatedAttributeOwnershipDivestiture, METH_VARARGS,
        ""},
    {"attributeOwnershipAcquisition",
        (PyCFunction)rtia_attributeOwnershipAcquisition, METH_VARARGS,
        ""},
    {"attributeOwnershipAcquisitionIfAvailable",
        (PyCFunction)rtia_attributeOwnershipAcquisitionIfAvailable, METH_VARARGS,
        ""},
    {"attributeOwnershipReleaseResponse",
        (PyCFunction)rtia_attributeOwnershipReleaseResponse, METH_VARARGS,
        ""},
    {"cancelNegotiatedAttributeOwnershipDivestiture",
        (PyCFunction)rtia_cancelNegotiatedAttributeOwnershipDivestiture, METH_VARARGS,
        ""},
    {"cancelAttributeOwnershipAcquisition",
        (PyCFunction)rtia_cancelAttributeOwnershipAcquisition, METH_VARARGS,
        ""},
    {"queryAttributeOwnership",
        (PyCFunction)rtia_queryAttributeOwnership, METH_VARARGS,
        ""},
    {"isAttributeOwnedByFederate",
        (PyCFunction)rtia_isAttributeOwnedByFederate, METH_VARARGS,
        ""},
    {"enableTimeRegulation",
        (PyCFunction)rtia_enableTimeRegulation, METH_VARARGS,
        ""},
    {"disableTimeRegulation",
        (PyCFunction)rtia_disableTimeRegulation, METH_NOARGS,
        ""},
    {"enableTimeConstrained",
        (PyCFunction)rtia_enableTimeConstrained, METH_NOARGS,
        ""},
    {"disableTimeConstrained",
        (PyCFunction)rtia_disableTimeConstrained, METH_NOARGS,
        ""},
    {"timeAdvanceRequest",
        (PyCFunction)rtia_timeAdvanceRequest, METH_VARARGS,
        ""},
    {"timeAdvanceRequestAvailable",
        (PyCFunction)rtia_timeAdvanceRequestAvailable, METH_VARARGS,
        ""},
    {"nextEventRequest",
        (PyCFunction)rtia_nextEventRequest, METH_VARARGS,
        ""},
    {"nextEventRequestAvailable",
        (PyCFunction)rtia_nextEventRequestAvailable, METH_VARARGS,
        ""},
    {"flushQueueRequest",
        (PyCFunction)rtia_flushQueueRequest, METH_VARARGS,
        ""},
    {"enableAsynchronousDelivery",
        (PyCFunction)rtia_enableAsynchronousDelivery, METH_NOARGS,
        ""},
    {"disableAsynchronousDelivery",
        (PyCFunction)rtia_disableAsynchronousDelivery, METH_NOARGS,
        ""},
    {"queryLBTS",
        (PyCFunction)rtia_queryLBTS, METH_NOARGS,
        ""},
    {"queryFederateTime",
        (PyCFunction)rtia_queryFederateTime, METH_NOARGS,
        ""},
    {"queryMinNextEventTime",
        (PyCFunction)rtia_queryMinNextEventTime, METH_NOARGS,
        ""},
    {"modifyLookahead",
        (PyCFunction)rtia_modifyLookahead, METH_VARARGS,
        ""},
    {"queryLookahead",
        (PyCFunction)rtia_queryLookahead, METH_NOARGS,
        ""},
    {"retract",
        (PyCFunction)rtia_retract, METH_VARARGS,
        ""},
    {"changeAttributeOrderType",
        (PyCFunction)rtia_changeAttributeOrderType, METH_VARARGS,
        ""},
    {"changeInteractionOrderType",
        (PyCFunction)rtia_changeInteractionOrderType, METH_VARARGS,
        ""},
    {"createRegion",
        (PyCFunction)rtia_createRegion, METH_VARARGS,
        ""},
    {"notifyAboutRegionModification",
        (PyCFunction)rtia_notifyAboutRegionModification, METH_VARARGS,
        ""},
    {"deleteRegion",
        (PyCFunction)rtia_deleteRegion, METH_VARARGS,
        ""},
    {"registerObjectInstanceWithRegion",
        (PyCFunction)rtia_registerObjectInstanceWithRegion, METH_VARARGS,
        ""},
    {"registerObjectInstanceWithRegion",
        (PyCFunction)rtia_registerObjectInstanceWithRegion, METH_VARARGS,
        ""},
    {"associateRegionForUpdates",
        (PyCFunction)rtia_associateRegionForUpdates, METH_VARARGS,
        ""},
    {"unassociateRegionForUpdates",
        (PyCFunction)rtia_unassociateRegionForUpdates, METH_VARARGS,
        ""},
    {"subscribeObjectClassAttributesWithRegion",
        (PyCFunction)rtia_subscribeObjectClassAttributesWithRegion, METH_VARARGS,
        ""},
    {"unsubscribeObjectClassWithRegion",
        (PyCFunction)rtia_unsubscribeObjectClassWithRegion, METH_VARARGS,
        ""},
    {"subscribeInteractionClassWithRegion",
        (PyCFunction)rtia_subscribeInteractionClassWithRegion, METH_VARARGS,
        ""},
    {"unsubscribeInteractionClassWithRegion",
        (PyCFunction)rtia_unsubscribeInteractionClassWithRegion, METH_VARARGS,
        ""},
    {"sendInteractionWithRegion",
        (PyCFunction)rtia_sendInteractionWithRegion, METH_VARARGS,
        ""},
    {"requestClassAttributeValueUpdateWithRegion",
        (PyCFunction)rtia_requestClassAttributeValueUpdateWithRegion, METH_VARARGS,
        ""},
    {"getObjectClassHandle",
        (PyCFunction)rtia_getObjectClassHandle, METH_VARARGS,
        ""},
    {"getObjectClassName",
        (PyCFunction)rtia_getObjectClassName, METH_VARARGS,
        ""},
    {"getAttributeHandle",
        (PyCFunction)rtia_getAttributeHandle, METH_VARARGS,
        ""},
    {"getAttributeName",
        (PyCFunction)rtia_getAttributeName, METH_VARARGS,
        ""},
    {"getInteractionClassHandle",
        (PyCFunction)rtia_getInteractionClassHandle, METH_VARARGS,
        ""},
    {"getInteractionClassName",
        (PyCFunction)rtia_getInteractionClassName, METH_VARARGS,
        ""},
    {"getParameterHandle",
        (PyCFunction)rtia_getParameterHandle, METH_VARARGS,
        ""},
    {"getParameterName",
        (PyCFunction)rtia_getParameterName, METH_VARARGS,
        ""},
    {"getObjectInstanceHandle",
        (PyCFunction)rtia_getObjectInstanceHandle, METH_VARARGS,
        ""},
    {"getObjectInstanceName",
        (PyCFunction)rtia_getObjectInstanceName, METH_VARARGS,
        ""},
    {"getRoutingSpaceHandle",
        (PyCFunction)rtia_getRoutingSpaceHandle, METH_VARARGS,
        ""},
    {"getRoutingSpaceName",
        (PyCFunction)rtia_getRoutingSpaceName, METH_VARARGS,
        ""},
    {"getDimensionHandle",
        (PyCFunction)rtia_getDimensionHandle, METH_VARARGS,
        ""},
    {"getDimensionName",
        (PyCFunction)rtia_getDimensionName, METH_VARARGS,
        ""},
    {"getAttributeRoutingSpaceHandle",
        (PyCFunction)rtia_getAttributeRoutingSpaceHandle, METH_VARARGS,
        ""},
    {"getObjectClass",
        (PyCFunction)rtia_getObjectClass, METH_VARARGS,
        ""},
    {"getInteractionRoutingSpaceHandle",
        (PyCFunction)rtia_getInteractionRoutingSpaceHandle, METH_VARARGS,
        ""},
    {"getTransportationHandle",
        (PyCFunction)rtia_getTransportationHandle, METH_VARARGS,
        ""},
    {"getTransportationName",
        (PyCFunction)rtia_getTransportationName, METH_VARARGS,
        ""},
    {"getOrderingHandle",
        (PyCFunction)rtia_getOrderingHandle, METH_VARARGS,
        ""},
    {"getOrderingName",
        (PyCFunction)rtia_getOrderingName, METH_VARARGS,
        ""},
    {"enableClassRelevanceAdvisorySwitch",
        (PyCFunction)rtia_enableClassRelevanceAdvisorySwitch, METH_NOARGS,
        ""},
    {"disableClassRelevanceAdvisorySwitch",
        (PyCFunction)rtia_disableClassRelevanceAdvisorySwitch, METH_NOARGS,
        ""},
    {"enableAttributeRelevanceAdvisorySwitch",
        (PyCFunction)rtia_enableAttributeRelevanceAdvisorySwitch, METH_NOARGS,
        ""},
    {"disableAttributeRelevanceAdvisorySwitch",
        (PyCFunction)rtia_disableAttributeRelevanceAdvisorySwitch, METH_NOARGS,
        ""},
    {"enableAttributeScopeAdvisorySwitch",
        (PyCFunction)rtia_enableAttributeScopeAdvisorySwitch, METH_NOARGS,
        ""},
    {"disableAttributeScopeAdvisorySwitch",
        (PyCFunction)rtia_disableAttributeScopeAdvisorySwitch, METH_NOARGS,
        ""},
    {"enableInteractionRelevanceAdvisorySwitch",
        (PyCFunction)rtia_enableInteractionRelevanceAdvisorySwitch, METH_NOARGS,
        ""},
    {"disableInteractionRelevanceAdvisorySwitch",
        (PyCFunction)rtia_disableInteractionRelevanceAdvisorySwitch, METH_NOARGS,
        ""},
    {"tick",
        (PyCFunction)rtia_tick, METH_VARARGS,
        ""},
    {NULL} // sentinel
};

static PyMemberDef rtia_members[] =
{
    {NULL} // sentinel
};

static PyTypeObject RTIAmbassadorObjectType =
{
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".RTIAmbassador", /* tp_name */
    sizeof(RTIAmbassadorObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)rtia_dealloc,  /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "RTI Ambassador",          /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    rtia_methods,              /* tp_methods */
    rtia_members,              /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    rtia_new,                  /* tp_new */
};

class RTIAmbassadorInitializer : public RtiInitializer
{
public:
    RTIAmbassadorInitializer() : RtiInitializer() {}
    void on_init(PyObject* module);
};

static RTIAmbassadorInitializer g_initializer;

void
RTIAmbassadorInitializer::on_init(PyObject* module)
{
    if (PyType_Ready(&RTIAmbassadorObjectType) < 0)
        return;

    Py_INCREF(&RTIAmbassadorObjectType);
    PyModule_AddObject(module, "RTIAmbassador", (PyObject *)&RTIAmbassadorObjectType);
}

// $Id: rtiambassador.cpp,v 1.6 2010/09/10 21:04:29 gotthardp Exp $
