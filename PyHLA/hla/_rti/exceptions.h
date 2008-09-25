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
 * $Id: exceptions.h,v 1.1 2008/09/25 17:17:34 gotthardp Exp $
 */

#ifndef RTI_EXCEPTIONS_H
#define RTI_EXCEPTIONS_H

#include <RTI.hh>  

#include "module.h"

extern PyObject *rti_Exception;
extern PyObject *rti_ArrayIndexOutOfBounds;
extern PyObject *rti_AsynchronousDeliveryAlreadyDisabled;
extern PyObject *rti_AsynchronousDeliveryAlreadyEnabled;
extern PyObject *rti_AttributeAcquisitionWasNotRequested;
extern PyObject *rti_AttributeAcquisitionWasNotCanceled;
extern PyObject *rti_AttributeAlreadyBeingAcquired;
extern PyObject *rti_AttributeAlreadyBeingDivested;
extern PyObject *rti_AttributeAlreadyOwned;
extern PyObject *rti_AttributeDivestitureWasNotRequested;
extern PyObject *rti_AttributeNotDefined;
extern PyObject *rti_AttributeNotKnown;
extern PyObject *rti_AttributeNotOwned;
extern PyObject *rti_AttributeNotPublished;
extern PyObject *rti_ConcurrentAccessAttempted;
extern PyObject *rti_CouldNotDiscover;
extern PyObject *rti_CouldNotOpenFED;
extern PyObject *rti_CouldNotRestore;
extern PyObject *rti_DeletePrivilegeNotHeld;
extern PyObject *rti_DimensionNotDefined;
extern PyObject *rti_EnableTimeConstrainedPending;
extern PyObject *rti_EnableTimeConstrainedWasNotPending;
extern PyObject *rti_EnableTimeRegulationPending;
extern PyObject *rti_EnableTimeRegulationWasNotPending;
extern PyObject *rti_ErrorReadingFED;
extern PyObject *rti_EventNotKnown;
extern PyObject *rti_FederateAlreadyExecutionMember;
extern PyObject *rti_FederateInternalError;
extern PyObject *rti_FederateLoggingServiceCalls;
extern PyObject *rti_FederateNotExecutionMember;
extern PyObject *rti_FederateOwnsAttributes;
extern PyObject *rti_FederateWasNotAskedToReleaseAttribute;
extern PyObject *rti_FederatesCurrentlyJoined;
extern PyObject *rti_FederationExecutionAlreadyExists;
extern PyObject *rti_FederationExecutionDoesNotExist;
extern PyObject *rti_FederationTimeAlreadyPassed;
extern PyObject *rti_HandleValuePairMaximumExceeded;
extern PyObject *rti_InteractionClassNotDefined;
extern PyObject *rti_InteractionClassNotKnown;
extern PyObject *rti_InteractionClassNotPublished;
extern PyObject *rti_InteractionClassNotSubscribed;
extern PyObject *rti_InteractionParameterNotDefined;
extern PyObject *rti_InteractionParameterNotKnown;
extern PyObject *rti_InvalidExtents;
extern PyObject *rti_InvalidFederationTime;
extern PyObject *rti_InvalidHandleValuePairSetContext;
extern PyObject *rti_InvalidLookahead;
extern PyObject *rti_InvalidOrderingHandle;
extern PyObject *rti_InvalidRegionContext;
extern PyObject *rti_InvalidResignAction;
extern PyObject *rti_InvalidRetractionHandle;
extern PyObject *rti_InvalidTransportationHandle;
extern PyObject *rti_MemoryExhausted;
extern PyObject *rti_NameNotFound;
extern PyObject *rti_ObjectClassNotDefined;
extern PyObject *rti_ObjectClassNotKnown;
extern PyObject *rti_ObjectClassNotPublished;
extern PyObject *rti_ObjectClassNotSubscribed;
extern PyObject *rti_ObjectNotKnown;
extern PyObject *rti_ObjectAlreadyRegistered;
extern PyObject *rti_OwnershipAcquisitionPending;
extern PyObject *rti_RegionInUse;
extern PyObject *rti_RegionNotKnown;
extern PyObject *rti_RestoreInProgress;
extern PyObject *rti_RestoreNotRequested;
extern PyObject *rti_RTIInternalError;
extern PyObject *rti_SpaceNotDefined;
extern PyObject *rti_SaveInProgress;
extern PyObject *rti_SaveNotInitiated;
extern PyObject *rti_SpecifiedSaveLabelDoesNotExist;
extern PyObject *rti_SynchronizationPointLabelWasNotAnnounced;
extern PyObject *rti_TimeAdvanceAlreadyInProgress;
extern PyObject *rti_TimeAdvanceWasNotInProgress;
extern PyObject *rti_TimeConstrainedAlreadyEnabled;
extern PyObject *rti_TimeConstrainedWasNotEnabled;
extern PyObject *rti_TimeRegulationAlreadyEnabled;
extern PyObject *rti_TimeRegulationWasNotEnabled;
extern PyObject *rti_UnableToPerformSave;
extern PyObject *rti_ValueCountExceeded;
extern PyObject *rti_ValueLengthExceeded;

void
PyErr_SetFromRTIException(PyObject *type, const RTI::Exception* exception);

template<class EXC>
EXC SetFromPyException(PyObject *type)
{
    auto_decref<PyObject> strexception = PyObject_Str(PyErr_Occurred());
    return EXC(PyString_AsString(strexception));
}

#endif // RTI_EXCEPTIONS_H

// $Id: exceptions.h,v 1.1 2008/09/25 17:17:34 gotthardp Exp $
