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
 * $Id: federateambassador.cpp,v 1.7 2011/06/23 00:10:07 gotthardp Exp $
 */

// note: you must include Python.h before any standard headers are included
#include <Python.h>
#include <structmember.h>

#include <RTI.hh> 
#include <fedtime.hh>

#include "federateambassador.h"
#include "handles.h"
#include "exceptions.h"

#define CATCH_PYTHON_EXCEPTION(STATE, EXCEPTION) \
    if(PyErr_GivenExceptionMatches(STATE, rti_##EXCEPTION)) throw SetFromPyException<RTI::EXCEPTION>(exception);

static PyObject *
PyObject_GetCallback(FederateAmbassadorObject *federate, const char *name)
{
    PyObject *func = PyObject_GetAttrString((PyObject *)federate, name);
    if(func == NULL)
        PyErr_Clear();

    return func;
}

static PyObject *
federate_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    FederateAmbassadorObject *self = (FederateAmbassadorObject *)type->tp_alloc(type, 0);
    self->ob_federate = new Py_FederateAmbassador(self);

    return (PyObject *)self;
}

static void
federate_dealloc(FederateAmbassadorObject* self)
{
    delete self->ob_federate;

    Py_TYPE(self)->tp_free((PyObject*)self);
}

Py_FederateAmbassador::Py_FederateAmbassador(FederateAmbassadorObject *federate)
{
    m_federate = federate;
    // note: no Py_INCREF(m_federate) to avoid circular references
}

Py_FederateAmbassador::~Py_FederateAmbassador()
  throw (RTI::FederateInternalError)
{
}

// Federation Management

void
Py_FederateAmbassador::synchronizationPointRegistrationSucceeded(const char *label)
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "synchronizationPointRegistrationSucceeded");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "s",
        label);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::synchronizationPointRegistrationFailed(const char *label)
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "synchronizationPointRegistrationFailed");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "s", label);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::announceSynchronizationPoint(const char *label, const char *tag)
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "announceSynchronizationPoint");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "ss", label, tag);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationSynchronized(const char *label)
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "federationSynchronized");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "s", label);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::initiateFederateSave(const char *label)
throw (RTI::UnableToPerformSave, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "initiateFederateSave");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "s", label);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, UnableToPerformSave)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationSaved()
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "federationSaved");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, NULL);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationNotSaved()
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "federationNotSaved");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, NULL);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::requestFederationRestoreSucceeded(const char *label)
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "requestFederationRestoreSucceeded");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "s", label);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::requestFederationRestoreFailed(const char *label, const char *reason)
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "requestFederationRestoreFailed");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "ss", label, reason);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationRestoreBegun()
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "federationRestoreBegun");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, NULL);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::initiateFederateRestore(const char *label, RTI::FederateHandle handle)
throw (RTI::SpecifiedSaveLabelDoesNotExist, RTI::CouldNotRestore, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "initiateFederateRestore");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "sO&",
        label,
        RtiFederateHandle_ToPython, &handle);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, SpecifiedSaveLabelDoesNotExist)
        CATCH_PYTHON_EXCEPTION(exception, CouldNotRestore)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationRestored()
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "federationRestored");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, NULL);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationNotRestored()
throw (RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "federationNotRestored");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, NULL);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

// Declaration Management

void
Py_FederateAmbassador::startRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
throw (RTI::ObjectClassNotPublished, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "startRegistrationForObjectClass");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&",
        RtiObjectClassHandle_ToPython, &theClass);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectClassNotPublished)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::stopRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
throw (RTI::ObjectClassNotPublished, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "stopRegistrationForObjectClass");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&",
        RtiObjectClassHandle_ToPython, &theClass);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectClassNotPublished)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::turnInteractionsOn(RTI::InteractionClassHandle theHandle)
throw (RTI::InteractionClassNotPublished, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "turnInteractionsOn");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&",
        RtiInteractionClassHandle_ToPython, &theHandle);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, InteractionClassNotPublished)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::turnInteractionsOff(RTI::InteractionClassHandle theHandle)
throw (RTI::InteractionClassNotPublished, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "turnInteractionsOff");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&",
        RtiInteractionClassHandle_ToPython, &theHandle);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, InteractionClassNotPublished)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

// Object Management

void
Py_FederateAmbassador::discoverObjectInstance(RTI::ObjectHandle theObject,
  RTI::ObjectClassHandle theObjectClass, const char *theObjectName)
throw (RTI::CouldNotDiscover, RTI::ObjectClassNotKnown, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "discoverObjectInstance");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&s",
        RtiObjectHandle_ToPython, &theObject,
        RtiObjectClassHandle_ToPython, &theObjectClass,
        theObjectName);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, CouldNotDiscover)
        CATCH_PYTHON_EXCEPTION(exception, ObjectClassNotKnown)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::reflectAttributeValues(RTI::ObjectHandle theObject,
    const RTI::AttributeHandleValuePairSet& theAttributes, 
    const RTI::FedTime& theTime, const char *theTag, RTI::EventRetractionHandle theHandle)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes,
    RTI::InvalidFederationTime, RTI::FederateInternalError)
{
    const RTIfedTime& time = theTime;
    // FIXME: theAttributes.getRegion() is not handled

    PyObject *func = PyObject_GetCallback(m_federate,
        "reflectAttributeValues");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&sOOdO&",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleValuePairSet_ToPython, &theAttributes,
        theTag,
        RtiULongHandle_FromULong(&RtiOrderingHandleType, theAttributes.getOrderType(0)),
        RtiULongHandle_FromULong(&RtiTransportationHandleType, theAttributes.getTransportType(0)),
        time.getTime(),
        EventRetractionHandle_ToPython, &theHandle);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, FederateOwnsAttributes)
        CATCH_PYTHON_EXCEPTION(exception, InvalidFederationTime)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::reflectAttributeValues(RTI::ObjectHandle theObject,
    const RTI::AttributeHandleValuePairSet& theAttributes, const char *theTag)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes,
    RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "reflectAttributeValues");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&sOO",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleValuePairSet_ToPython, &theAttributes,
        theTag,
        RtiULongHandle_FromULong(&RtiOrderingHandleType, theAttributes.getOrderType(0)),
        RtiULongHandle_FromULong(&RtiTransportationHandleType, theAttributes.getTransportType(0)));
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, FederateOwnsAttributes)
        CATCH_PYTHON_EXCEPTION(exception, InvalidFederationTime)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::receiveInteraction(RTI::InteractionClassHandle theInteraction,
    const RTI::ParameterHandleValuePairSet& theParameters,
    const RTI::FedTime& theTime, const char *theTag, RTI::EventRetractionHandle theHandle)
throw (RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown,
    RTI::InvalidFederationTime, RTI::FederateInternalError)
{
    const RTIfedTime& time = theTime;
    // FIXME: theParameters.getRegion() is not handled

    PyObject *func = PyObject_GetCallback(m_federate,
        "receiveInteraction");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&sOOdO&",
        RtiInteractionClassHandle_ToPython, &theInteraction,
        ParameterHandleValuePairSet_ToPython, &theParameters,
        theTag,
        RtiULongHandle_FromULong(&RtiOrderingHandleType, theParameters.getOrderType()),
        RtiULongHandle_FromULong(&RtiTransportationHandleType, theParameters.getTransportType()),
        time.getTime(),
        EventRetractionHandle_ToPython, &theHandle);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, InteractionClassNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, InteractionParameterNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, InvalidFederationTime)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::receiveInteraction(RTI::InteractionClassHandle theInteraction,
    const RTI::ParameterHandleValuePairSet& theParameters, const char *theTag)
throw (RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown,
    RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "receiveInteraction");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&sOO",
        RtiInteractionClassHandle_ToPython, &theInteraction,
        ParameterHandleValuePairSet_ToPython, &theParameters,
        theTag,
        RtiULongHandle_FromULong(&RtiOrderingHandleType, theParameters.getOrderType()),
        RtiULongHandle_FromULong(&RtiTransportationHandleType, theParameters.getTransportType()));
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, InteractionClassNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, InteractionParameterNotKnown)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::removeObjectInstance(RTI::ObjectHandle theObject, const RTI::FedTime &theTime,
    const char *theTag, RTI::EventRetractionHandle theHandle)
throw (RTI::ObjectNotKnown, RTI::InvalidFederationTime, RTI::FederateInternalError)
{
    const RTIfedTime& time = theTime;

    PyObject *func = PyObject_GetCallback(m_federate,
        "removeObjectInstance");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&sdO&",
        RtiObjectHandle_ToPython, &theObject,
        theTag,
        time.getTime(),
        EventRetractionHandle_ToPython, &theHandle);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, InvalidFederationTime)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::removeObjectInstance(RTI::ObjectHandle theObject, const char *theTag)
throw (RTI::ObjectNotKnown, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "removeObjectInstance");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&s",
        RtiObjectHandle_ToPython, &theObject,
        theTag);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::attributesInScope(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "attributesInScope");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &theAttributes);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::attributesOutOfScope(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "attributesOutOfScope");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &theAttributes);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::provideAttributeValueUpdate(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeNotOwned,
    RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "provideAttributeValueUpdate");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &theAttributes);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotOwned)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::turnUpdatesOnForObjectInstance(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
throw (RTI::ObjectNotKnown, RTI::AttributeNotOwned, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "turnUpdatesOnForObjectInstance");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &theAttributes);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotOwned)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::turnUpdatesOffForObjectInstance(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
throw (RTI::ObjectNotKnown, RTI::AttributeNotOwned, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "turnUpdatesOffForObjectInstance");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &theAttributes);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotOwned)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

// Ownership Management

void
Py_FederateAmbassador::requestAttributeOwnershipAssumption(RTI::ObjectHandle theObject,
    const RTI::AttributeHandleSet& offeredAttributes, const char *theTag)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeAlreadyOwned,
    RTI::AttributeNotPublished, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "requestAttributeOwnershipAssumption");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&s",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &offeredAttributes,
        theTag);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeAlreadyOwned)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotPublished)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::attributeOwnershipDivestitureNotification(RTI::ObjectHandle theObject,
    const RTI::AttributeHandleSet& releasedAttributes)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeNotOwned,
    RTI::AttributeDivestitureWasNotRequested, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "attributeOwnershipDivestitureNotification");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &releasedAttributes);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotOwned)
        CATCH_PYTHON_EXCEPTION(exception, AttributeDivestitureWasNotRequested)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::attributeOwnershipAcquisitionNotification(RTI::ObjectHandle theObject,
    const RTI::AttributeHandleSet& securedAttributes)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeAcquisitionWasNotRequested,
    RTI::AttributeAlreadyOwned, RTI::AttributeNotPublished, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "attributeOwnershipAcquisitionNotification");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &securedAttributes);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeAcquisitionWasNotRequested)
        CATCH_PYTHON_EXCEPTION(exception, AttributeAlreadyOwned)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotPublished)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::attributeOwnershipUnavailable(RTI::ObjectHandle theObject, const RTI::AttributeHandleSet& theAttributes)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeAlreadyOwned,
    RTI::AttributeAcquisitionWasNotRequested, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "attributeOwnershipUnavailable");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &theAttributes);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeAlreadyOwned)
        CATCH_PYTHON_EXCEPTION(exception, AttributeAcquisitionWasNotRequested)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::requestAttributeOwnershipRelease(RTI::ObjectHandle theObject,
    const RTI::AttributeHandleSet& candidateAttribute, const char *theTag)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeNotOwned,
    RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "requestAttributeOwnershipRelease");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&s",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &candidateAttribute,
        theTag);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotOwned)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::confirmAttributeOwnershipAcquisitionCancellation(RTI::ObjectHandle theObject,
    const RTI::AttributeHandleSet& theAttributes)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeAlreadyOwned,
    RTI::AttributeAcquisitionWasNotCanceled, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "confirmAttributeOwnershipAcquisitionCancellation");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleSet_ToPython, &theAttributes);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeAlreadyOwned)
        CATCH_PYTHON_EXCEPTION(exception, AttributeAcquisitionWasNotCanceled)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::informAttributeOwnership(RTI::ObjectHandle theObject,
    RTI::AttributeHandle theAttribute, RTI::FederateHandle theOwner)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "informAttributeOwnership");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&O&",
        RtiObjectHandle_ToPython, &theObject,
        RtiAttributeHandle_ToPython, &theAttribute,
        RtiFederateHandle_ToPython, &theOwner);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::attributeIsNotOwned(RTI::ObjectHandle theObject, RTI::AttributeHandle theAttribute)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "attributeIsNotOwned");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        RtiAttributeHandle_ToPython, &theAttribute);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::attributeOwnedByRTI(RTI::ObjectHandle theObject, RTI::AttributeHandle theAttribute)
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "attributeOwnedByRTI");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&O&",
        RtiObjectHandle_ToPython, &theObject,
        RtiAttributeHandle_ToPython, &theAttribute);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

// Time Management

void
Py_FederateAmbassador::timeRegulationEnabled(const RTI::FedTime& theFederateTime)
throw (RTI::InvalidFederationTime, RTI::EnableTimeRegulationWasNotPending, RTI::FederateInternalError)
{
    const RTIfedTime& time = theFederateTime;

    PyObject *func = PyObject_GetCallback(m_federate,
        "timeRegulationEnabled");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "d",
        time.getTime());
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, InvalidFederationTime)
        CATCH_PYTHON_EXCEPTION(exception, EnableTimeRegulationWasNotPending)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::timeConstrainedEnabled(const RTI::FedTime& theFederateTime)
throw (RTI::InvalidFederationTime, RTI::EnableTimeConstrainedWasNotPending, RTI::FederateInternalError)
{
    const RTIfedTime& time = theFederateTime;

    PyObject *func = PyObject_GetCallback(m_federate,
        "timeConstrainedEnabled");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "d",
        time.getTime());
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, InvalidFederationTime)
        CATCH_PYTHON_EXCEPTION(exception, EnableTimeRegulationWasNotPending)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::timeAdvanceGrant(const RTI::FedTime& theFederateTime)
throw (RTI::InvalidFederationTime, RTI::TimeAdvanceWasNotInProgress, RTI::FederateInternalError)
{
    const RTIfedTime& time = theFederateTime;

    PyObject *func = PyObject_GetCallback(m_federate,
        "timeAdvanceGrant");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "d",
        time.getTime());
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, InvalidFederationTime)
        CATCH_PYTHON_EXCEPTION(exception, TimeAdvanceWasNotInProgress)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::requestRetraction(RTI::EventRetractionHandle theHandle)
throw (RTI::EventNotKnown, RTI::FederateInternalError)
{
    PyObject *func = PyObject_GetCallback(m_federate,
        "requestRetraction");
    if(func == NULL)
        return; // callback not defined

    PyObject *res = PyObject_CallFunction(func, "O&",
        EventRetractionHandle_ToPython, &theHandle);
    if(res != NULL)
      Py_XDECREF(res); // ignore the result

    Py_XDECREF(func);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, EventNotKnown)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

static PyMethodDef federate_methods[] =
{
    {NULL} // sentinel
};

static PyMemberDef federate_members[] =
{
    {"rtia", T_OBJECT, offsetof(FederateAmbassadorObject, ob_rtia), READONLY|RESTRICTED,
     "the RTI ambassador to which a federate is connected; None for unconnected federates"},
    {NULL} // sentinel
};

PyTypeObject FederateAmbassadorObjectType =
{
#if PY_MAJOR_VERSION >= 3
    PyVarObject_HEAD_INIT(NULL, 0)
#else
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
#endif
    MODULE_NAME ".FederateAmbassador", /* tp_name */
    sizeof(FederateAmbassadorObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)federate_dealloc, /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
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
    "Empty federate ambassador",  /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    federate_methods,          /* tp_methods */
    federate_members,          /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    federate_new,              /* tp_new */
};

class FederateAmbassadorInitializer : public RtiInitializer
{
public:
    FederateAmbassadorInitializer() : RtiInitializer() {}
    void on_init(PyObject* module);
};

static FederateAmbassadorInitializer g_initializer;

void
FederateAmbassadorInitializer::on_init(PyObject* module)
{
    if (PyType_Ready(&FederateAmbassadorObjectType) < 0)
        return;

    Py_INCREF(&FederateAmbassadorObjectType);
    PyModule_AddObject(module, "FederateAmbassador", (PyObject *)&FederateAmbassadorObjectType);
}

// $Id: federateambassador.cpp,v 1.7 2011/06/23 00:10:07 gotthardp Exp $
