/*
 * Python Language HLA API
 * Copyright(C) 2008  Petr Gotthard <petr.gotthard@centrum.cz>
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
 * $Id: federateambassador.h,v 1.1 2008/09/25 17:17:35 gotthardp Exp $
 */

#ifndef RTI_FEDERATEAMBASSADOR_H
#define RTI_FEDERATEAMBASSADOR_H

#include <RTI.hh>

class Py_FederateAmbassador;

extern PyTypeObject FederateAmbassadorObjectType;

typedef struct
{
    PyObject_HEAD
    Py_FederateAmbassador *ob_federate;
    PyObject *ob_rtia;
} FederateAmbassadorObject;

// note: we have to implement all methods, so we don't need NullFederateAmbassador
class Py_FederateAmbassador : public RTI::FederateAmbassador
{
public:
    Py_FederateAmbassador(FederateAmbassadorObject *federate);

    virtual ~Py_FederateAmbassador()
    throw (
      RTI::FederateInternalError);

    // Federation Management Services

    virtual void synchronizationPointRegistrationSucceeded(
      const char *label)
    throw (
      RTI::FederateInternalError);

    virtual void synchronizationPointRegistrationFailed(
      const char *label)
    throw (
      RTI::FederateInternalError);

    virtual void announceSynchronizationPoint(
      const char *label,
      const char *tag)
    throw (
      RTI::FederateInternalError);

    virtual void federationSynchronized(
      const char *label)
    throw (
      RTI::FederateInternalError);

    virtual void initiateFederateSave(
      const char *label)
    throw (
      RTI::UnableToPerformSave,
      RTI::FederateInternalError);

    virtual void federationSaved()
    throw (
      RTI::FederateInternalError);

    virtual void federationNotSaved()
    throw (
      RTI::FederateInternalError);

    virtual void requestFederationRestoreSucceeded(
      const char *label)
    throw (
      RTI::FederateInternalError);

    virtual void requestFederationRestoreFailed(
      const char *label,
      const char *reason)
    throw (
      RTI::FederateInternalError);

    virtual void federationRestoreBegun()
    throw (
      RTI::FederateInternalError);

    virtual void initiateFederateRestore(
      const char               *label,
            RTI::FederateHandle handle)
    throw (
      RTI::SpecifiedSaveLabelDoesNotExist,
      RTI::CouldNotRestore,
      RTI::FederateInternalError);

    virtual void federationRestored()
    throw (
      RTI::FederateInternalError);

    virtual void federationNotRestored()
    throw (
      RTI::FederateInternalError);

    // Declaration Management Services

    virtual void startRegistrationForObjectClass(
            RTI::ObjectClassHandle   theClass)
    throw (
      RTI::ObjectClassNotPublished,
      RTI::FederateInternalError);

    virtual void stopRegistrationForObjectClass(
            RTI::ObjectClassHandle   theClass)
    throw (
      RTI::ObjectClassNotPublished,
      RTI::FederateInternalError);

    virtual void turnInteractionsOn(
      RTI::InteractionClassHandle theHandle)
    throw (
      RTI::InteractionClassNotPublished,
      RTI::FederateInternalError);

    virtual void turnInteractionsOff(
      RTI::InteractionClassHandle theHandle)
    throw (
      RTI::InteractionClassNotPublished,
      RTI::FederateInternalError);

    // Object Management Services

    virtual void discoverObjectInstance(
            RTI::ObjectHandle          theObject,
            RTI::ObjectClassHandle     theObjectClass,
      const char*                      theObjectName)
    throw (
      RTI::CouldNotDiscover,
      RTI::ObjectClassNotKnown,
      RTI::FederateInternalError);

    virtual void reflectAttributeValues(
            RTI::ObjectHandle                 theObject,
      const RTI::AttributeHandleValuePairSet& theAttributes,
      const RTI::FedTime&                     theTime,
      const char                             *theTag,
            RTI::EventRetractionHandle        theHandle)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::FederateOwnsAttributes,
      RTI::InvalidFederationTime,
      RTI::FederateInternalError);

    virtual void reflectAttributeValues(
            RTI::ObjectHandle                 theObject,
      const RTI::AttributeHandleValuePairSet& theAttributes,
      const char                             *theTag)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::FederateOwnsAttributes,
      RTI::FederateInternalError);

    // 4.6
    virtual void receiveInteraction(
            RTI::InteractionClassHandle       theInteraction,
      const RTI::ParameterHandleValuePairSet& theParameters,
      const RTI::FedTime&                     theTime,
      const char                             *theTag,
            RTI::EventRetractionHandle        theHandle)
    throw (
      RTI::InteractionClassNotKnown,
      RTI::InteractionParameterNotKnown,
      RTI::InvalidFederationTime,
      RTI::FederateInternalError);

    virtual void receiveInteraction(
            RTI::InteractionClassHandle       theInteraction,
      const RTI::ParameterHandleValuePairSet& theParameters,
      const char                             *theTag)
    throw (
      RTI::InteractionClassNotKnown,
      RTI::InteractionParameterNotKnown,
      RTI::FederateInternalError);

    virtual void removeObjectInstance(
            RTI::ObjectHandle          theObject,
      const RTI::FedTime&              theTime,
      const char                      *theTag,
            RTI::EventRetractionHandle theHandle)
    throw (
      RTI::ObjectNotKnown,
      RTI::InvalidFederationTime,
      RTI::FederateInternalError);

    virtual void removeObjectInstance(
            RTI::ObjectHandle          theObject,
      const char                      *theTag)
    throw (
      RTI::ObjectNotKnown,
      RTI::FederateInternalError);

    virtual void attributesInScope(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& theAttributes)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::FederateInternalError);

    virtual void attributesOutOfScope(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& theAttributes)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::FederateInternalError);

    virtual void provideAttributeValueUpdate(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& theAttributes)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::AttributeNotOwned,
      RTI::FederateInternalError);

    virtual void turnUpdatesOnForObjectInstance(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& theAttributes)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotOwned,
      RTI::FederateInternalError);

    virtual void turnUpdatesOffForObjectInstance(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& theAttributes)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotOwned,
      RTI::FederateInternalError);

    // Ownership Management Services

    virtual void requestAttributeOwnershipAssumption(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& offeredAttributes,
      const char                    *theTag)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::AttributeAlreadyOwned,
      RTI::AttributeNotPublished,
      RTI::FederateInternalError);

    virtual void attributeOwnershipDivestitureNotification(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& releasedAttributes)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::AttributeNotOwned,
      RTI::AttributeDivestitureWasNotRequested,
      RTI::FederateInternalError);

    virtual void attributeOwnershipAcquisitionNotification(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& securedAttributes)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::AttributeAcquisitionWasNotRequested,
      RTI::AttributeAlreadyOwned,
      RTI::AttributeNotPublished,
      RTI::FederateInternalError);

    virtual void attributeOwnershipUnavailable(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& theAttributes)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::AttributeAlreadyOwned,
      RTI::AttributeAcquisitionWasNotRequested,
      RTI::FederateInternalError);

    virtual void requestAttributeOwnershipRelease(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& candidateAttributes,
      const char                    *theTag)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::AttributeNotOwned,
      RTI::FederateInternalError);

    virtual void confirmAttributeOwnershipAcquisitionCancellation(
            RTI::ObjectHandle        theObject,
      const RTI::AttributeHandleSet& theAttributes)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::AttributeAlreadyOwned,
      RTI::AttributeAcquisitionWasNotCanceled,
      RTI::FederateInternalError);

    virtual void informAttributeOwnership(
      RTI::ObjectHandle    theObject,
      RTI::AttributeHandle theAttribute,
      RTI::FederateHandle  theOwner)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::FederateInternalError);

    virtual void attributeIsNotOwned(
      RTI::ObjectHandle    theObject,
      RTI::AttributeHandle theAttribute)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::FederateInternalError);

    virtual void attributeOwnedByRTI(
      RTI::ObjectHandle    theObject,
      RTI::AttributeHandle theAttribute)
    throw (
      RTI::ObjectNotKnown,
      RTI::AttributeNotKnown,
      RTI::FederateInternalError);

    // Time Management Services

    virtual void timeRegulationEnabled(
     const  RTI::FedTime& theFederateTime)
    throw (
      RTI::InvalidFederationTime,
      RTI::EnableTimeRegulationWasNotPending,
      RTI::FederateInternalError);

    virtual void timeConstrainedEnabled(
      const RTI::FedTime& theFederateTime)
    throw (
      RTI::InvalidFederationTime,
      RTI::EnableTimeConstrainedWasNotPending,
      RTI::FederateInternalError);

    virtual void timeAdvanceGrant(
      const RTI::FedTime& theTime)
    throw (
      RTI::InvalidFederationTime,
      RTI::TimeAdvanceWasNotInProgress,
      RTI::FederateInternalError);

    virtual void requestRetraction(
      RTI::EventRetractionHandle theHandle)
    throw (
      RTI::EventNotKnown,
      RTI::FederateInternalError);

private:
    //! implementation of the federate ambassador
    FederateAmbassadorObject* m_federate;
};

#endif // RTI_FEDERATEAMBASSADOR_H

// $Id: federateambassador.h,v 1.1 2008/09/25 17:17:35 gotthardp Exp $
