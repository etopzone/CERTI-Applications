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
 * $Id: federateambassador.cpp,v 1.1 2008/09/25 17:17:35 gotthardp Exp $
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

    self->ob_type->tp_free((PyObject*)self);
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
    PyObject_CallMethod((PyObject *)m_federate,
        "synchronizationPointRegistrationSucceeded", "s", label);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::synchronizationPointRegistrationFailed(const char *label)
throw (RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "synchronizationPointRegistrationFailed", "s", label);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::announceSynchronizationPoint(const char *label, const char *tag)
throw (RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "announceSynchronizationPoint", "ss", label, tag);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationSynchronized(const char *label)
throw (RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "federationSynchronized", "s", label);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::initiateFederateSave(const char *label)
throw (RTI::UnableToPerformSave, RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "initiateFederateSave", "s", label);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, UnableToPerformSave)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationSaved()
throw (RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "federationSaved", NULL);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationNotSaved()
throw (RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "federationNotSaved", NULL);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::requestFederationRestoreSucceeded(const char *label)
throw (RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "requestFederationRestoreSucceeded", "s", label);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::requestFederationRestoreFailed(const char *label, const char *reason)
throw (RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "requestFederationRestoreFailed", "ss", label, reason);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationRestoreBegun()
throw (RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "federationRestoreBegun", NULL);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::initiateFederateRestore(const char *label, RTI::FederateHandle handle)
throw (RTI::SpecifiedSaveLabelDoesNotExist, RTI::CouldNotRestore, RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "initiateFederateRestore", "sO&",
        label,
        RtiFederateHandle_ToPython, &handle);

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
    PyObject_CallMethod((PyObject *)m_federate,
        "federationRestored", NULL);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::federationNotRestored()
throw (RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "federationNotRestored", NULL);

    if(PyObject *exception = PyErr_Occurred()) {
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

// Declaration Management

void
Py_FederateAmbassador::startRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
throw (RTI::ObjectClassNotPublished, RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "startRegistrationForObjectClass", "O&",
        RtiObjectClassHandle_ToPython, &theClass);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectClassNotPublished)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::stopRegistrationForObjectClass(RTI::ObjectClassHandle theClass)
throw (RTI::ObjectClassNotPublished, RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "stopRegistrationForObjectClass", "O&",
        RtiObjectClassHandle_ToPython, &theClass);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectClassNotPublished)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::turnInteractionsOn(RTI::InteractionClassHandle theHandle)
throw (RTI::InteractionClassNotPublished, RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "turnInteractionsOn", "O&",
        RtiInteractionClassHandle_ToPython, &theHandle);

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, InteractionClassNotPublished)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::turnInteractionsOff(RTI::InteractionClassHandle theHandle)
throw (RTI::InteractionClassNotPublished, RTI::FederateInternalError)
{
    PyObject_CallMethod((PyObject *)m_federate,
        "turnInteractionsOff", "O&",
        RtiInteractionClassHandle_ToPython, &theHandle);

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
    PyObject_CallMethod((PyObject *)m_federate,
        "discoverObjectInstance", "O&O&s",
        RtiObjectHandle_ToPython, &theObject,
        RtiObjectClassHandle_ToPython, &theObjectClass,
        theObjectName);

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
    // FIXME: theAttributes.getRegion() is not handled
    // FIXME: EventRetractionHandle is not handled

    PyObject_CallMethod((PyObject *)m_federate,
        "reflectAttributeValues", "O&O&OOs",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleValuePairSet_ToPython, &theAttributes,
        RtiULongHandle_FromULong(&RtiOrderTypeType, theAttributes.getOrderType(0)),
        RtiULongHandle_FromULong(&RtiTransportTypeType, theAttributes.getTransportType(0)),
        theTag);

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
    PyObject_CallMethod((PyObject *)m_federate,
        "reflectAttributeValues", "O&O&sOO",
        RtiObjectHandle_ToPython, &theObject,
        AttributeHandleValuePairSet_ToPython, &theAttributes,
        theTag,
        RtiULongHandle_FromULong(&RtiOrderTypeType, theAttributes.getOrderType(0)),
        RtiULongHandle_FromULong(&RtiTransportTypeType, theAttributes.getTransportType(0)));

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, AttributeNotKnown)
        CATCH_PYTHON_EXCEPTION(exception, FederateOwnsAttributes)
        CATCH_PYTHON_EXCEPTION(exception, InvalidFederationTime)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::receiveInteraction(RTI::InteractionClassHandle, const RTI::ParameterHandleValuePairSet &, 
    const RTI::FedTime &, const char *, RTI::EventRetractionHandle) 
throw (RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown, RTI::InvalidFederationTime,
    RTI::FederateInternalError) { }

void
Py_FederateAmbassador::receiveInteraction(RTI::InteractionClassHandle, const RTI::ParameterHandleValuePairSet &, 
    const char *) 
throw (RTI::InteractionClassNotKnown, RTI::InteractionParameterNotKnown, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::removeObjectInstance(RTI::ObjectHandle theObject, const RTI::FedTime &theTime,
    const char *theTag, RTI::EventRetractionHandle theHandle)
throw (RTI::ObjectNotKnown, RTI::InvalidFederationTime, RTI::FederateInternalError)
{
    const RTIfedTime& time = theTime;

    PyObject_CallMethod((PyObject *)m_federate,
        "removeObjectInstance", "O&sd",
        RtiObjectHandle_ToPython, &theObject,
        theTag,
        time.getTime());

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
    PyObject_CallMethod((PyObject *)m_federate,
        "removeObjectInstance", "O&sO",
        RtiObjectHandle_ToPython, &theObject,
        theTag,
        Py_None); // no RTI::FedTime provided

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, ObjectNotKnown)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::attributesInScope(RTI::ObjectHandle, const RTI::AttributeHandleSet &) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::attributesOutOfScope(RTI::ObjectHandle, const RTI::AttributeHandleSet &) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::provideAttributeValueUpdate(RTI::ObjectHandle, const RTI::AttributeHandleSet &) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeNotOwned,
    RTI::FederateInternalError) { }

void
Py_FederateAmbassador::turnUpdatesOnForObjectInstance(RTI::ObjectHandle, const RTI::AttributeHandleSet &) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotOwned, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::turnUpdatesOffForObjectInstance(RTI::ObjectHandle, const RTI::AttributeHandleSet &) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotOwned, RTI::FederateInternalError) { }

// Ownership Management

void
Py_FederateAmbassador::requestAttributeOwnershipAssumption(RTI::ObjectHandle, const RTI::AttributeHandleSet &, 
    const char *) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeAlreadyOwned,
    RTI::AttributeNotPublished, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::attributeOwnershipDivestitureNotification(RTI::ObjectHandle, const RTI::AttributeHandleSet &) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeNotOwned,
    RTI::AttributeDivestitureWasNotRequested, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::attributeOwnershipAcquisitionNotification(RTI::ObjectHandle, const RTI::AttributeHandleSet &) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeAcquisitionWasNotRequested,
    RTI::AttributeAlreadyOwned, RTI::AttributeNotPublished, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::attributeOwnershipUnavailable(RTI::ObjectHandle, const RTI::AttributeHandleSet &) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeAlreadyOwned,
    RTI::AttributeAcquisitionWasNotRequested, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::requestAttributeOwnershipRelease(RTI::ObjectHandle, const RTI::AttributeHandleSet &, 
    const char *) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeNotOwned, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::confirmAttributeOwnershipAcquisitionCancellation(RTI::ObjectHandle,
    const RTI::AttributeHandleSet &) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::AttributeAlreadyOwned,
    RTI::AttributeAcquisitionWasNotCanceled, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::informAttributeOwnership(RTI::ObjectHandle, RTI::AttributeHandle, RTI::FederateHandle) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::attributeIsNotOwned(RTI::ObjectHandle, RTI::AttributeHandle) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateInternalError) { }

void
Py_FederateAmbassador::attributeOwnedByRTI(RTI::ObjectHandle, RTI::AttributeHandle) 
throw (RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateInternalError) { }

// Time Management

void
Py_FederateAmbassador::timeRegulationEnabled(const RTI::FedTime& theFederateTime)
throw (RTI::InvalidFederationTime, RTI::EnableTimeRegulationWasNotPending, RTI::FederateInternalError)
{
    const RTIfedTime& time = theFederateTime;

    PyObject_CallMethod((PyObject *)m_federate,
        "timeRegulationEnabled", "d", time.getTime());

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

    PyObject_CallMethod((PyObject *)m_federate,
        "timeConstrainedEnabled", "d", time.getTime());

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

    PyObject_CallMethod((PyObject *)m_federate,
        "timeAdvanceGrant", "d", time.getTime());

    if(PyObject *exception = PyErr_Occurred()) {
        CATCH_PYTHON_EXCEPTION(exception, InvalidFederationTime)
        CATCH_PYTHON_EXCEPTION(exception, TimeAdvanceWasNotInProgress)
        throw SetFromPyException<RTI::FederateInternalError>(exception);
    }
}

void
Py_FederateAmbassador::requestRetraction(RTI::EventRetractionHandle) 
throw (RTI::EventNotKnown, RTI::FederateInternalError) { }

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
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".FederateAmbassador", /* tp_name */
    sizeof(FederateAmbassadorObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)federate_dealloc, /* tp_dealloc */
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

// $Id: federateambassador.cpp,v 1.1 2008/09/25 17:17:35 gotthardp Exp $
