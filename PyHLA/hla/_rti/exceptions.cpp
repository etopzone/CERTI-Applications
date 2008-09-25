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
 * $Id: exceptions.cpp,v 1.1 2008/09/25 17:17:34 gotthardp Exp $
 */

// note: you must include Python.h before any standard headers are included
#include <Python.h>

#include "exceptions.h"

PyObject *rti_Exception;
PyObject *rti_ArrayIndexOutOfBounds;
PyObject *rti_AsynchronousDeliveryAlreadyDisabled;
PyObject *rti_AsynchronousDeliveryAlreadyEnabled;
PyObject *rti_AttributeAcquisitionWasNotRequested;
PyObject *rti_AttributeAcquisitionWasNotCanceled;
PyObject *rti_AttributeAlreadyBeingAcquired;
PyObject *rti_AttributeAlreadyBeingDivested;
PyObject *rti_AttributeAlreadyOwned;
PyObject *rti_AttributeDivestitureWasNotRequested;
PyObject *rti_AttributeNotDefined;
PyObject *rti_AttributeNotKnown;
PyObject *rti_AttributeNotOwned;
PyObject *rti_AttributeNotPublished;
PyObject *rti_ConcurrentAccessAttempted;
PyObject *rti_CouldNotDiscover;
PyObject *rti_CouldNotOpenFED;
PyObject *rti_CouldNotRestore;
PyObject *rti_DeletePrivilegeNotHeld;
PyObject *rti_DimensionNotDefined;
PyObject *rti_EnableTimeConstrainedPending;
PyObject *rti_EnableTimeConstrainedWasNotPending;
PyObject *rti_EnableTimeRegulationPending;
PyObject *rti_EnableTimeRegulationWasNotPending;
PyObject *rti_ErrorReadingFED;
PyObject *rti_EventNotKnown;
PyObject *rti_FederateAlreadyExecutionMember;
PyObject *rti_FederateInternalError;
PyObject *rti_FederateLoggingServiceCalls;
PyObject *rti_FederateNotExecutionMember;
PyObject *rti_FederateOwnsAttributes;
PyObject *rti_FederateWasNotAskedToReleaseAttribute;
PyObject *rti_FederatesCurrentlyJoined;
PyObject *rti_FederationExecutionAlreadyExists;
PyObject *rti_FederationExecutionDoesNotExist;
PyObject *rti_FederationTimeAlreadyPassed;
PyObject *rti_HandleValuePairMaximumExceeded;
PyObject *rti_InteractionClassNotDefined;
PyObject *rti_InteractionClassNotKnown;
PyObject *rti_InteractionClassNotPublished;
PyObject *rti_InteractionClassNotSubscribed;
PyObject *rti_InteractionParameterNotDefined;
PyObject *rti_InteractionParameterNotKnown;
PyObject *rti_InvalidExtents;
PyObject *rti_InvalidFederationTime;
PyObject *rti_InvalidHandleValuePairSetContext;
PyObject *rti_InvalidLookahead;
PyObject *rti_InvalidOrderingHandle;
PyObject *rti_InvalidRegionContext;
PyObject *rti_InvalidResignAction;
PyObject *rti_InvalidRetractionHandle;
PyObject *rti_InvalidTransportationHandle;
PyObject *rti_MemoryExhausted;
PyObject *rti_NameNotFound;
PyObject *rti_ObjectClassNotDefined;
PyObject *rti_ObjectClassNotKnown;
PyObject *rti_ObjectClassNotPublished;
PyObject *rti_ObjectClassNotSubscribed;
PyObject *rti_ObjectNotKnown;
PyObject *rti_ObjectAlreadyRegistered;
PyObject *rti_OwnershipAcquisitionPending;
PyObject *rti_RegionInUse;
PyObject *rti_RegionNotKnown;
PyObject *rti_RestoreInProgress;
PyObject *rti_RestoreNotRequested;
PyObject *rti_RTIInternalError;
PyObject *rti_SpaceNotDefined;
PyObject *rti_SaveInProgress;
PyObject *rti_SaveNotInitiated;
PyObject *rti_SpecifiedSaveLabelDoesNotExist;
PyObject *rti_SynchronizationPointLabelWasNotAnnounced;
PyObject *rti_TimeAdvanceAlreadyInProgress;
PyObject *rti_TimeAdvanceWasNotInProgress;
PyObject *rti_TimeConstrainedAlreadyEnabled;
PyObject *rti_TimeConstrainedWasNotEnabled;
PyObject *rti_TimeRegulationAlreadyEnabled;
PyObject *rti_TimeRegulationWasNotEnabled;
PyObject *rti_UnableToPerformSave;
PyObject *rti_ValueCountExceeded;
PyObject *rti_ValueLengthExceeded;

void
PyErr_SetFromRTIException(PyObject *type, const RTI::Exception* exception)
{
    const char* reason = exception->_reason;
    PyErr_SetString(type, reason);
}

class ExceptionsInitializer : public RtiInitializer
{
public:
    ExceptionsInitializer() : RtiInitializer() {}
    void on_init(PyObject* module);
};

static ExceptionsInitializer g_initializer;

void
ExceptionsInitializer::on_init(PyObject* module)
{
    PyObject* dict = PyModule_GetDict(module);

    rti_Exception = PyErr_NewException(MODULE_NAME ".Exception", NULL, NULL);
    PyDict_SetItemString(dict, "Exception", rti_Exception);

    rti_ArrayIndexOutOfBounds = PyErr_NewException(MODULE_NAME ".ArrayIndexOutOfBounds", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ArrayIndexOutOfBounds", rti_ArrayIndexOutOfBounds);
    rti_AsynchronousDeliveryAlreadyDisabled = PyErr_NewException(MODULE_NAME ".AsynchronousDeliveryAlreadyDisabled", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AsynchronousDeliveryAlreadyDisabled", rti_AsynchronousDeliveryAlreadyDisabled);
    rti_AsynchronousDeliveryAlreadyEnabled = PyErr_NewException(MODULE_NAME ".AsynchronousDeliveryAlreadyEnabled", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AsynchronousDeliveryAlreadyEnabled", rti_AsynchronousDeliveryAlreadyEnabled);
    rti_AttributeAcquisitionWasNotRequested = PyErr_NewException(MODULE_NAME ".AttributeAcquisitionWasNotRequested", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AttributeAcquisitionWasNotRequested", rti_AttributeAcquisitionWasNotRequested);
    rti_AttributeAcquisitionWasNotCanceled = PyErr_NewException(MODULE_NAME ".AttributeAcquisitionWasNotCanceled", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AttributeAcquisitionWasNotCanceled", rti_AttributeAcquisitionWasNotCanceled);
    rti_AttributeAlreadyBeingAcquired = PyErr_NewException(MODULE_NAME ".AttributeAlreadyBeingAcquired", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AttributeAlreadyBeingAcquired", rti_AttributeAlreadyBeingAcquired);
    rti_AttributeAlreadyBeingDivested = PyErr_NewException(MODULE_NAME ".AttributeAlreadyBeingDivested", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AttributeAlreadyBeingDivested", rti_AttributeAlreadyBeingDivested);
    rti_AttributeAlreadyOwned = PyErr_NewException(MODULE_NAME ".AttributeAlreadyOwned", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AttributeAlreadyOwned", rti_AttributeAlreadyOwned);
    rti_AttributeDivestitureWasNotRequested = PyErr_NewException(MODULE_NAME ".AttributeDivestitureWasNotRequested", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AttributeDivestitureWasNotRequested", rti_AttributeDivestitureWasNotRequested);
    rti_AttributeNotDefined = PyErr_NewException(MODULE_NAME ".AttributeNotDefined", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AttributeNotDefined", rti_AttributeNotDefined);
    rti_AttributeNotKnown = PyErr_NewException(MODULE_NAME ".AttributeNotKnown", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AttributeNotKnown", rti_AttributeNotKnown);
    rti_AttributeNotOwned = PyErr_NewException(MODULE_NAME ".AttributeNotOwned", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AttributeNotOwned", rti_AttributeNotOwned);
    rti_AttributeNotPublished = PyErr_NewException(MODULE_NAME ".AttributeNotPublished", rti_Exception, NULL);
    PyDict_SetItemString(dict, "AttributeNotPublished", rti_AttributeNotPublished);
    rti_ConcurrentAccessAttempted = PyErr_NewException(MODULE_NAME ".ConcurrentAccessAttempted", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ConcurrentAccessAttempted", rti_ConcurrentAccessAttempted);
    rti_CouldNotDiscover = PyErr_NewException(MODULE_NAME ".CouldNotDiscover", rti_Exception, NULL);
    PyDict_SetItemString(dict, "CouldNotDiscover", rti_CouldNotDiscover);
    rti_CouldNotOpenFED = PyErr_NewException(MODULE_NAME ".CouldNotOpenFED", rti_Exception, NULL);
    PyDict_SetItemString(dict, "CouldNotOpenFED", rti_CouldNotOpenFED);
    rti_CouldNotRestore = PyErr_NewException(MODULE_NAME ".CouldNotRestore", rti_Exception, NULL);
    PyDict_SetItemString(dict, "CouldNotRestore", rti_CouldNotRestore);
    rti_DeletePrivilegeNotHeld = PyErr_NewException(MODULE_NAME ".DeletePrivilegeNotHeld", rti_Exception, NULL);
    PyDict_SetItemString(dict, "DeletePrivilegeNotHeld", rti_DeletePrivilegeNotHeld);
    rti_DimensionNotDefined = PyErr_NewException(MODULE_NAME ".DimensionNotDefined", rti_Exception, NULL);
    PyDict_SetItemString(dict, "DimensionNotDefined", rti_DimensionNotDefined);
    rti_EnableTimeConstrainedPending = PyErr_NewException(MODULE_NAME ".EnableTimeConstrainedPending", rti_Exception, NULL);
    PyDict_SetItemString(dict, "EnableTimeConstrainedPending", rti_EnableTimeConstrainedPending);
    rti_EnableTimeConstrainedWasNotPending = PyErr_NewException(MODULE_NAME ".EnableTimeConstrainedWasNotPending", rti_Exception, NULL);
    PyDict_SetItemString(dict, "EnableTimeConstrainedWasNotPending", rti_EnableTimeConstrainedWasNotPending);
    rti_EnableTimeRegulationPending = PyErr_NewException(MODULE_NAME ".EnableTimeRegulationPending", rti_Exception, NULL);
    PyDict_SetItemString(dict, "EnableTimeRegulationPending", rti_EnableTimeRegulationPending);
    rti_EnableTimeRegulationWasNotPending = PyErr_NewException(MODULE_NAME ".EnableTimeRegulationWasNotPending", rti_Exception, NULL);
    PyDict_SetItemString(dict, "EnableTimeRegulationWasNotPending", rti_EnableTimeRegulationWasNotPending);
    rti_ErrorReadingFED = PyErr_NewException(MODULE_NAME ".ErrorReadingFED", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ErrorReadingFED", rti_ErrorReadingFED);
    rti_EventNotKnown = PyErr_NewException(MODULE_NAME ".EventNotKnown", rti_Exception, NULL);
    PyDict_SetItemString(dict, "EventNotKnown", rti_EventNotKnown);
    rti_FederateAlreadyExecutionMember = PyErr_NewException(MODULE_NAME ".FederateAlreadyExecutionMember", rti_Exception, NULL);
    PyDict_SetItemString(dict, "FederateAlreadyExecutionMember", rti_FederateAlreadyExecutionMember);
    rti_FederateInternalError = PyErr_NewException(MODULE_NAME ".FederateInternalError", rti_Exception, NULL);
    PyDict_SetItemString(dict, "FederateInternalError", rti_FederateInternalError);
    rti_FederateLoggingServiceCalls = PyErr_NewException(MODULE_NAME ".FederateLoggingServiceCalls", rti_Exception, NULL);
    PyDict_SetItemString(dict, "FederateLoggingServiceCalls", rti_FederateLoggingServiceCalls);
    rti_FederateNotExecutionMember = PyErr_NewException(MODULE_NAME ".FederateNotExecutionMember", rti_Exception, NULL);
    PyDict_SetItemString(dict, "FederateNotExecutionMember", rti_FederateNotExecutionMember);
    rti_FederateOwnsAttributes = PyErr_NewException(MODULE_NAME ".FederateOwnsAttributes", rti_Exception, NULL);
    PyDict_SetItemString(dict, "FederateOwnsAttributes", rti_FederateOwnsAttributes);
    rti_FederateWasNotAskedToReleaseAttribute = PyErr_NewException(MODULE_NAME ".FederateWasNotAskedToReleaseAttribute", rti_Exception, NULL);
    PyDict_SetItemString(dict, "FederateWasNotAskedToReleaseAttribute", rti_FederateWasNotAskedToReleaseAttribute);
    rti_FederatesCurrentlyJoined = PyErr_NewException(MODULE_NAME ".FederatesCurrentlyJoined", rti_Exception, NULL);
    PyDict_SetItemString(dict, "FederatesCurrentlyJoined", rti_FederatesCurrentlyJoined);
    rti_FederationExecutionAlreadyExists = PyErr_NewException(MODULE_NAME ".FederationExecutionAlreadyExists", rti_Exception, NULL);
    PyDict_SetItemString(dict, "FederationExecutionAlreadyExists", rti_FederationExecutionAlreadyExists);
    rti_FederationExecutionDoesNotExist = PyErr_NewException(MODULE_NAME ".FederationExecutionDoesNotExist", rti_Exception, NULL);
    PyDict_SetItemString(dict, "FederationExecutionDoesNotExist", rti_FederationExecutionDoesNotExist);
    rti_FederationTimeAlreadyPassed = PyErr_NewException(MODULE_NAME ".FederationTimeAlreadyPassed", rti_Exception, NULL);
    PyDict_SetItemString(dict, "FederationTimeAlreadyPassed", rti_FederationTimeAlreadyPassed);
    rti_HandleValuePairMaximumExceeded = PyErr_NewException(MODULE_NAME ".HandleValuePairMaximumExceeded", rti_Exception, NULL);
    PyDict_SetItemString(dict, "HandleValuePairMaximumExceeded", rti_HandleValuePairMaximumExceeded);
    rti_InteractionClassNotDefined = PyErr_NewException(MODULE_NAME ".InteractionClassNotDefined", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InteractionClassNotDefined", rti_InteractionClassNotDefined);
    rti_InteractionClassNotKnown = PyErr_NewException(MODULE_NAME ".InteractionClassNotKnown", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InteractionClassNotKnown", rti_InteractionClassNotKnown);
    rti_InteractionClassNotPublished = PyErr_NewException(MODULE_NAME ".InteractionClassNotPublished", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InteractionClassNotPublished", rti_InteractionClassNotPublished);
    rti_InteractionClassNotSubscribed = PyErr_NewException(MODULE_NAME ".InteractionClassNotSubscribed", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InteractionClassNotSubscribed", rti_InteractionClassNotSubscribed);
    rti_InteractionParameterNotDefined = PyErr_NewException(MODULE_NAME ".InteractionParameterNotDefined", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InteractionParameterNotDefined", rti_InteractionParameterNotDefined);
    rti_InteractionParameterNotKnown = PyErr_NewException(MODULE_NAME ".InteractionParameterNotKnown", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InteractionParameterNotKnown", rti_InteractionParameterNotKnown);
    rti_InvalidExtents = PyErr_NewException(MODULE_NAME ".InvalidExtents", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InvalidExtents", rti_InvalidExtents);
    rti_InvalidFederationTime = PyErr_NewException(MODULE_NAME ".InvalidFederationTime", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InvalidFederationTime", rti_InvalidFederationTime);
    rti_InvalidHandleValuePairSetContext = PyErr_NewException(MODULE_NAME ".InvalidHandleValuePairSetContext", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InvalidHandleValuePairSetContext", rti_InvalidHandleValuePairSetContext);
    rti_InvalidLookahead = PyErr_NewException(MODULE_NAME ".InvalidLookahead", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InvalidLookahead", rti_InvalidLookahead);
    rti_InvalidOrderingHandle = PyErr_NewException(MODULE_NAME ".InvalidOrderingHandle", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InvalidOrderingHandle", rti_InvalidOrderingHandle);
    rti_InvalidRegionContext = PyErr_NewException(MODULE_NAME ".InvalidRegionContext", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InvalidRegionContext", rti_InvalidRegionContext);
    rti_InvalidResignAction = PyErr_NewException(MODULE_NAME ".InvalidResignAction", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InvalidResignAction", rti_InvalidResignAction);
    rti_InvalidRetractionHandle = PyErr_NewException(MODULE_NAME ".InvalidRetractionHandle", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InvalidRetractionHandle", rti_InvalidRetractionHandle);
    rti_InvalidTransportationHandle = PyErr_NewException(MODULE_NAME ".InvalidTransportationHandle", rti_Exception, NULL);
    PyDict_SetItemString(dict, "InvalidTransportationHandle", rti_InvalidTransportationHandle);
    rti_MemoryExhausted = PyErr_NewException(MODULE_NAME ".MemoryExhausted", rti_Exception, NULL);
    PyDict_SetItemString(dict, "MemoryExhausted", rti_MemoryExhausted);
    rti_NameNotFound = PyErr_NewException(MODULE_NAME ".NameNotFound", rti_Exception, NULL);
    PyDict_SetItemString(dict, "NameNotFound", rti_NameNotFound);
    rti_ObjectClassNotDefined = PyErr_NewException(MODULE_NAME ".ObjectClassNotDefined", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ObjectClassNotDefined", rti_ObjectClassNotDefined);
    rti_ObjectClassNotKnown = PyErr_NewException(MODULE_NAME ".ObjectClassNotKnown", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ObjectClassNotKnown", rti_ObjectClassNotKnown);
    rti_ObjectClassNotPublished = PyErr_NewException(MODULE_NAME ".ObjectClassNotPublished", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ObjectClassNotPublished", rti_ObjectClassNotPublished);
    rti_ObjectClassNotSubscribed = PyErr_NewException(MODULE_NAME ".ObjectClassNotSubscribed", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ObjectClassNotSubscribed", rti_ObjectClassNotSubscribed);
    rti_ObjectNotKnown = PyErr_NewException(MODULE_NAME ".ObjectNotKnown", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ObjectNotKnown", rti_ObjectNotKnown);
    rti_ObjectAlreadyRegistered = PyErr_NewException(MODULE_NAME ".ObjectAlreadyRegistered", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ObjectAlreadyRegistered", rti_ObjectAlreadyRegistered);
    rti_OwnershipAcquisitionPending = PyErr_NewException(MODULE_NAME ".OwnershipAcquisitionPending", rti_Exception, NULL);
    PyDict_SetItemString(dict, "OwnershipAcquisitionPending", rti_OwnershipAcquisitionPending);
    rti_RegionInUse = PyErr_NewException(MODULE_NAME ".RegionInUse", rti_Exception, NULL);
    PyDict_SetItemString(dict, "RegionInUse", rti_RegionInUse);
    rti_RegionNotKnown = PyErr_NewException(MODULE_NAME ".RegionNotKnown", rti_Exception, NULL);
    PyDict_SetItemString(dict, "RegionNotKnown", rti_RegionNotKnown);
    rti_RestoreInProgress = PyErr_NewException(MODULE_NAME ".RestoreInProgress", rti_Exception, NULL);
    PyDict_SetItemString(dict, "RestoreInProgress", rti_RestoreInProgress);
    rti_RestoreNotRequested = PyErr_NewException(MODULE_NAME ".RestoreNotRequested", rti_Exception, NULL);
    PyDict_SetItemString(dict, "RestoreNotRequested", rti_RestoreNotRequested);
    rti_RTIInternalError = PyErr_NewException(MODULE_NAME ".RTIInternalError", rti_Exception, NULL);
    PyDict_SetItemString(dict, "RTIInternalError", rti_RTIInternalError);
    rti_SpaceNotDefined = PyErr_NewException(MODULE_NAME ".SpaceNotDefined", rti_Exception, NULL);
    PyDict_SetItemString(dict, "SpaceNotDefined", rti_SpaceNotDefined);
    rti_SaveInProgress = PyErr_NewException(MODULE_NAME ".SaveInProgress", rti_Exception, NULL);
    PyDict_SetItemString(dict, "SaveInProgress", rti_SaveInProgress);
    rti_SaveNotInitiated = PyErr_NewException(MODULE_NAME ".SaveNotInitiated", rti_Exception, NULL);
    PyDict_SetItemString(dict, "SaveNotInitiated", rti_SaveNotInitiated);
    rti_SpecifiedSaveLabelDoesNotExist = PyErr_NewException(MODULE_NAME ".SpecifiedSaveLabelDoesNotExist", rti_Exception, NULL);
    PyDict_SetItemString(dict, "SpecifiedSaveLabelDoesNotExist", rti_SpecifiedSaveLabelDoesNotExist);
    rti_SynchronizationPointLabelWasNotAnnounced = PyErr_NewException(MODULE_NAME ".SynchronizationPointLabelWasNotAnnounced", rti_Exception, NULL);
    PyDict_SetItemString(dict, "SynchronizationPointLabelWasNotAnnounced", rti_SynchronizationPointLabelWasNotAnnounced);
    rti_TimeAdvanceAlreadyInProgress = PyErr_NewException(MODULE_NAME ".TimeAdvanceAlreadyInProgress", rti_Exception, NULL);
    PyDict_SetItemString(dict, "TimeAdvanceAlreadyInProgress", rti_TimeAdvanceAlreadyInProgress);
    rti_TimeAdvanceWasNotInProgress = PyErr_NewException(MODULE_NAME ".TimeAdvanceWasNotInProgress", rti_Exception, NULL);
    PyDict_SetItemString(dict, "TimeAdvanceWasNotInProgress", rti_TimeAdvanceWasNotInProgress);
    rti_TimeConstrainedAlreadyEnabled = PyErr_NewException(MODULE_NAME ".TimeConstrainedAlreadyEnabled", rti_Exception, NULL);
    PyDict_SetItemString(dict, "TimeConstrainedAlreadyEnabled", rti_TimeConstrainedAlreadyEnabled);
    rti_TimeConstrainedWasNotEnabled = PyErr_NewException(MODULE_NAME ".TimeConstrainedWasNotEnabled", rti_Exception, NULL);
    PyDict_SetItemString(dict, "TimeConstrainedWasNotEnabled", rti_TimeConstrainedWasNotEnabled);
    rti_TimeRegulationAlreadyEnabled = PyErr_NewException(MODULE_NAME ".TimeRegulationAlreadyEnabled", rti_Exception, NULL);
    PyDict_SetItemString(dict, "TimeRegulationAlreadyEnabled", rti_TimeRegulationAlreadyEnabled);
    rti_TimeRegulationWasNotEnabled = PyErr_NewException(MODULE_NAME ".TimeRegulationWasNotEnabled", rti_Exception, NULL);
    PyDict_SetItemString(dict, "TimeRegulationWasNotEnabled", rti_TimeRegulationWasNotEnabled);
    rti_UnableToPerformSave = PyErr_NewException(MODULE_NAME ".UnableToPerformSave", rti_Exception, NULL);
    PyDict_SetItemString(dict, "UnableToPerformSave", rti_UnableToPerformSave);
    rti_ValueCountExceeded = PyErr_NewException(MODULE_NAME ".ValueCountExceeded", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ValueCountExceeded", rti_ValueCountExceeded);
    rti_ValueLengthExceeded = PyErr_NewException(MODULE_NAME ".ValueLengthExceeded", rti_Exception, NULL);
    PyDict_SetItemString(dict, "ValueLengthExceeded", rti_ValueLengthExceeded);
}

// $Id: exceptions.cpp,v 1.1 2008/09/25 17:17:34 gotthardp Exp $
