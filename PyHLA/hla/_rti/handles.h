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
 * $Id: handles.h,v 1.4 2008/10/09 16:50:58 gotthardp Exp $
 */

#ifndef RTI_HANDLES_H
#define RTI_HANDLES_H

#include "module.h"

typedef struct {
    PyObject_HEAD
    RTI::ULong ob_ival;
} RtiULongHandleObject;

extern PyTypeObject RtiObjectClassHandleType;
extern PyTypeObject RtiInteractionClassHandleType;
extern PyTypeObject RtiAttributeHandleType;
extern PyTypeObject RtiParameterHandleType;
extern PyTypeObject RtiObjectHandleType;
extern PyTypeObject RtiFederateHandleType;

extern PyTypeObject RtiOrderingHandleType;
extern PyTypeObject RtiTransportationHandleType;

PyObject* RtiULongHandle_FromULong(PyTypeObject *type, unsigned long val);

int RtiObjectClassHandle_FromPython(RtiULongHandleObject *value, RTI::ObjectClassHandle *addr);
PyObject *RtiObjectClassHandle_ToPython(RTI::ObjectClassHandle *value);

int RtiInteractionClassHandle_FromPython(RtiULongHandleObject *value, RTI::InteractionClassHandle *addr);
PyObject *RtiInteractionClassHandle_ToPython(RTI::InteractionClassHandle *value);

int RtiAttributeHandle_FromPython(RtiULongHandleObject *value, RTI::AttributeHandle *result);
PyObject *RtiAttributeHandle_ToPython(RTI::AttributeHandle *value);

int RtiParameterHandle_FromPython(RtiULongHandleObject *value, RTI::ParameterHandle *result);
PyObject *RtiParameterHandle_ToPython(RTI::ParameterHandle *value);

int RtiObjectHandle_FromPython(RtiULongHandleObject *value, RTI::ObjectHandle *result);
PyObject *RtiObjectHandle_ToPython(RTI::ObjectHandle *value);

PyObject *RtiFederateHandle_ToPython(RTI::FederateHandle *value);

int RtiOrderingHandle_FromPython(RtiULongHandleObject *value, RTI::OrderingHandle *result);
PyObject *RtiOrderingHandle_ToPython(RTI::OrderingHandle *value);

int RtiTransportationHandle_FromPython(RtiULongHandleObject *value, RTI::TransportationHandle *result);
PyObject *RtiTransportationHandle_ToPython(RTI::TransportationHandle *value);

int AttributeHandleSet_FromPython(PyObject *value, RTI::AttributeHandleSet **result);
PyObject *AttributeHandleSet_ToPython(RTI::AttributeHandleSet *value);

int AttributeHandleValuePairSet_FromPython(PyObject *value, RTI::AttributeHandleValuePairSet **result);
PyObject *AttributeHandleValuePairSet_ToPython(RTI::AttributeHandleValuePairSet *value);

int ParameterHandleValuePairSet_FromPython(PyObject *value, RTI::ParameterHandleValuePairSet **result);
PyObject *ParameterHandleValuePairSet_ToPython(RTI::ParameterHandleValuePairSet *value);

typedef struct {
    PyObject_HEAD
    RTI::EventRetractionHandle ob_value;
} EventRetractionHandleObject;

extern PyTypeObject EventRetractionHandleType;

int EventRetractionHandle_FromPython(EventRetractionHandleObject *value, RTI::EventRetractionHandle *result);
PyObject *EventRetractionHandle_ToPython(RTI::EventRetractionHandle *value);

int RTIfedTime_FromPython(PyObject *value, RTIfedTime *result);

typedef struct {
    PyObject_HEAD
    RTI::RegionToken ob_handle;
    RTI::Region *ob_value;
} RegionHandleObject;

extern PyTypeObject RegionHandleType;

int RegionHandle_FromPython(RegionHandleObject *value, RTI::Region **result);

typedef struct {
    PyObject_HEAD
    RTI::Long ob_ival;
} RtiLongHandleObject;

extern PyTypeObject RtiSpaceHandleType;
extern PyTypeObject RtiDimensionHandleType;

int RtiSpaceHandle_FromPython(RtiLongHandleObject *value, RTI::SpaceHandle *result);
PyObject *RtiSpaceHandle_ToPython(RTI::SpaceHandle *value);

int RtiDimensionHandle_FromPython(RtiULongHandleObject *value, RTI::DimensionHandle *result);
PyObject *RtiDimensionHandle_ToPython(RTI::DimensionHandle *value);

#endif // RTI_HANDLES_H

// $Id: handles.h,v 1.4 2008/10/09 16:50:58 gotthardp Exp $
