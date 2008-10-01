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
 * $Id: handles.cpp,v 1.2 2008/10/01 21:08:58 gotthardp Exp $
 */

// note: you must include Python.h before any standard headers are included
#include <Python.h>
#include <structmember.h>

#include <RTI.hh>  
#include <fedtime.hh>

#include "handles.h"

/* Numeric handles are represented by encapsulated integers.
 * These objects can be assigned and compared, but cannot be directly initialized
 * and do not support arithmetic operations.
 */

PyObject *
RtiULongHandle_FromULong(PyTypeObject *type, unsigned long val)
{
    RtiULongHandleObject *self = PyObject_New(RtiULongHandleObject, type);
    if (self == NULL)
        return NULL;
    self->ob_ival = val;

    return (PyObject *)self;
}

int RtiObjectClassHandle_FromPython(RtiULongHandleObject *value, RTI::ObjectClassHandle *addr)
{
    if(value == NULL || !PyObject_TypeCheck(value, &RtiObjectClassHandleType)) {
        PyErr_SetString(PyExc_TypeError,
            "ObjectClassHandle object required");
        return 0; // failure
    }

    *addr = value->ob_ival;
    return 1; // success
}

PyObject *RtiObjectClassHandle_ToPython(RTI::ObjectClassHandle *value)
{
    return RtiULongHandle_FromULong(&RtiObjectClassHandleType, *value);
}

PyObject *RtiInteractionClassHandle_ToPython(RTI::InteractionClassHandle *value)
{
    return RtiULongHandle_FromULong(&RtiInteractionClassHandleType, *value);
}

int RtiAttributeHandle_FromPython(RtiULongHandleObject *value, RTI::AttributeHandle *result)
{
    if(result == NULL || !PyObject_TypeCheck(value, &RtiAttributeHandleType)) {
        PyErr_SetString(PyExc_TypeError,
            "AttributeHandle object required");
        return 0; // failure
    }

    *result = value->ob_ival;
    return 1; // success
}

PyObject *RtiAttributeHandle_ToPython(RTI::AttributeHandle *value)
{
    return RtiULongHandle_FromULong(&RtiAttributeHandleType, *value);
}

int RtiParameterHandle_FromPython(RtiULongHandleObject *value, RTI::ParameterHandle *result)
{
    if(result == NULL || !PyObject_TypeCheck(value, &RtiParameterHandleType)) {
        PyErr_SetString(PyExc_TypeError,
            "ParameterHandle object required");
        return 0; // failure
    }

    *result = value->ob_ival;
    return 1; // success
}

int RtiObjectHandle_FromPython(RtiULongHandleObject *value, RTI::ObjectHandle *result)
{
    if(result == NULL || !PyObject_TypeCheck(value, &RtiObjectHandleType)) {
        PyErr_SetString(PyExc_TypeError,
            "ObjectHandle object required");
        return 0; // failure
    }

    *result = value->ob_ival;
    return 1; // success
}

PyObject *RtiObjectHandle_ToPython(RTI::ObjectHandle *value)
{
    return RtiULongHandle_FromULong(&RtiObjectHandleType, *value);
}

PyObject *RtiFederateHandle_ToPython(RTI::FederateHandle *value)
{
    return RtiULongHandle_FromULong(&RtiFederateHandleType, *value);
}

int
AttributeHandleSet_FromPython(PyObject *value, RTI::AttributeHandleSet **result)
{
    if(value == NULL || !PySequence_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
            "sequence [AttributeHandle] required");
        return 0; // failure
    }

    PyObject *iter = PyObject_GetIter(value);
    if(iter == NULL)
        return 0; // failure

    *result = RTI::AttributeHandleSetFactory::create(PySequence_Size(value));

    PyObject *item;
    while((item = PyIter_Next(iter)) != NULL) {

        if(!PyObject_TypeCheck(item, &RtiAttributeHandleType))
        {
            delete *result;

            PyErr_SetString(PyExc_TypeError,
                "sequence [AttributeHandle] required");
            return 0; // failure
        }

        (*result)->add(((RtiULongHandleObject *)item)->ob_ival);
    }

    Py_DECREF(iter);

    return 1; // success
}

PyObject *
AttributeHandleSet_ToPython(RTI::AttributeHandleSet *value)
{
    PyObject *result = PyTuple_New(value->size());
    for(RTI::ULong i = 0; i < value->size(); i++)
        PyTuple_SetItem(result, i, RtiULongHandle_FromULong(&RtiAttributeHandleType, value->getHandle(i)));

    return result;
}

int
AttributeHandleValuePairSet_FromPython(PyObject *value, RTI::AttributeHandleValuePairSet **result)
{
    if(value == NULL || !PyMapping_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
            "mapping {AttributeHandle:string} required");
        return 0; // failure
    }

    PyObject *items = PyMapping_Items(value);
    if(items == NULL)
        return 0; // failure

    PyObject *iter = PyObject_GetIter(items);
    if(iter == NULL) {
        Py_DECREF(items);
        return 0; // failure
    }

    *result = RTI::AttributeSetFactory::create(PySequence_Size(items));

    PyObject *item;
    while((item = PyIter_Next(iter)) != NULL) {
        RTI::AttributeHandle theHandle;
        const char *valueData;
        int valueSize;

        if(!PyArg_ParseTuple(item, "O&s#",
            RtiAttributeHandle_FromPython, &theHandle,
            &valueData, &valueSize))
            return 0; // failure

        (*result)->add(theHandle, valueData, valueSize);
    }

    Py_DECREF(iter);
    Py_DECREF(items);

    return 1; // success
}

PyObject *
AttributeHandleValuePairSet_ToPython(RTI::AttributeHandleValuePairSet *value)
{
    PyObject *result = PyDict_New();
    for(RTI::ULong i = 0; i < value->size(); i++)
    {
        PyObject *atHandle = RtiULongHandle_FromULong(&RtiAttributeHandleType, value->getHandle(i));

        RTI::ULong length;
        char* data = value->getValuePointer(i, length);
        PyObject *atData = PyString_FromStringAndSize(data, length);

        PyDict_SetItem(result, atHandle, atData);

        Py_DECREF(atHandle);
        Py_DECREF(atData);
    }

    return result;
}

int
ParameterHandleValuePairSet_FromPython(PyObject *value, RTI::ParameterHandleValuePairSet **result)
{
    if(value == NULL || !PyMapping_Check(value)) {
        PyErr_SetString(PyExc_TypeError,
            "mapping {ParameterHandle:string} required");
        return 0; // failure
    }

    PyObject *items = PyMapping_Items(value);
    if(items == NULL)
        return 0; // failure

    PyObject *iter = PyObject_GetIter(items);
    if(iter == NULL) {
        Py_DECREF(items);
        return 0; // failure
    }

    *result = RTI::ParameterSetFactory::create(PySequence_Size(items));

    PyObject *item;
    while((item = PyIter_Next(iter)) != NULL) {
        RTI::ParameterHandle theHandle;
        const char *valueData;
        int valueSize;

        if(!PyArg_ParseTuple(item, "O&s#",
            RtiParameterHandle_FromPython, &theHandle,
            &valueData, &valueSize))
            return 0; // failure

        (*result)->add(theHandle, valueData, valueSize);
    }

    Py_DECREF(iter);
    Py_DECREF(items);

    return 1; // success
}

PyObject *
ParameterHandleValuePairSet_ToPython(RTI::ParameterHandleValuePairSet *value)
{
    PyObject *result = PyDict_New();
    for(RTI::ULong i = 0; i < value->size(); i++)
    {
        PyObject *atHandle = RtiULongHandle_FromULong(&RtiParameterHandleType, value->getHandle(i));

        RTI::ULong length;
        char* data = value->getValuePointer(i, length);
        PyObject *atData = PyString_FromStringAndSize(data, length);

        PyDict_SetItem(result, atHandle, atData);

        Py_DECREF(atHandle);
        Py_DECREF(atData);
    }

    return result;
}

static int
ulonghandle_print(RtiULongHandleObject *v, FILE *fp, int flags)
{
    fprintf(fp, "<%s=%lu>", v->ob_type->tp_name, v->ob_ival);
    return 0;
}

static int
ulonghandle_compare(RtiULongHandleObject *v, RtiULongHandleObject *w)
{
    return (v->ob_ival < w->ob_ival) ? -1 :
        (v->ob_ival > w->ob_ival) ? 1 : 0;
}

static PyObject *
ulonghandle_repr(RtiULongHandleObject *v)
{
    return PyString_FromFormat("<%s=%lu>", v->ob_type->tp_name, v->ob_ival);
}

static long
ulonghandle_hash(RtiULongHandleObject *v)
{
    // create a "signed long" hash from an "unsigned long" number
    static const int __shift = 15; // see SHIFT in longintrepr.h
    return (v->ob_ival >> __shift) | (v->ob_ival & ~0UL << __shift);
}

PyTypeObject RtiObjectClassHandleType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".ObjectClassHandle", /* tp_name */
    sizeof(RtiULongHandleObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    (printfunc)ulonghandle_print, /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    (cmpfunc)ulonghandle_compare, /* tp_compare */
    (reprfunc)ulonghandle_repr, /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    (hashfunc)ulonghandle_hash, /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "ObjectClassHandle",       /* tp_doc */
};

PyTypeObject RtiInteractionClassHandleType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".InteractionClassHandle", /* tp_name */
    sizeof(RtiULongHandleObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    (printfunc)ulonghandle_print, /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    (cmpfunc)ulonghandle_compare, /* tp_compare */
    (reprfunc)ulonghandle_repr, /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    (hashfunc)ulonghandle_hash, /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "InteractionClassHandle",  /* tp_doc */
};

PyTypeObject RtiAttributeHandleType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".AttributeHandle", /* tp_name */
    sizeof(RtiULongHandleObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    (printfunc)ulonghandle_print, /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    (cmpfunc)ulonghandle_compare, /* tp_compare */
    (reprfunc)ulonghandle_repr, /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    (hashfunc)ulonghandle_hash, /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "AttributeHandle",         /* tp_doc */
};

PyTypeObject RtiParameterHandleType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".ParameterHandle", /* tp_name */
    sizeof(RtiULongHandleObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    (printfunc)ulonghandle_print, /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    (cmpfunc)ulonghandle_compare, /* tp_compare */
    (reprfunc)ulonghandle_repr, /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    (hashfunc)ulonghandle_hash, /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "ParameterHandle",         /* tp_doc */
};

PyTypeObject RtiObjectHandleType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".ObjectHandle", /* tp_name */
    sizeof(RtiULongHandleObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    (printfunc)ulonghandle_print, /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    (cmpfunc)ulonghandle_compare, /* tp_compare */
    (reprfunc)ulonghandle_repr, /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    (hashfunc)ulonghandle_hash, /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "ClassHandle",             /* tp_doc */
};

PyTypeObject RtiFederateHandleType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".FederateHandle", /* tp_name */
    sizeof(RtiULongHandleObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    (printfunc)ulonghandle_print, /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    (cmpfunc)ulonghandle_compare, /* tp_compare */
    (reprfunc)ulonghandle_repr, /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    (hashfunc)ulonghandle_hash, /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "FederateHandle",          /* tp_doc */
};

PyTypeObject RtiOrderTypeType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".OrderType", /* tp_name */
    sizeof(RtiULongHandleObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    (printfunc)ulonghandle_print, /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    (cmpfunc)ulonghandle_compare, /* tp_compare */
    (reprfunc)ulonghandle_repr, /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    (hashfunc)ulonghandle_hash, /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "OrderType",               /* tp_doc */
};

PyTypeObject RtiTransportTypeType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".TransportType", /* tp_name */
    sizeof(RtiULongHandleObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    (printfunc)ulonghandle_print, /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    (cmpfunc)ulonghandle_compare, /* tp_compare */
    (reprfunc)ulonghandle_repr, /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    (hashfunc)ulonghandle_hash, /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "TransportType",           /* tp_doc */
};

static int
EventRetractionHandle_print(EventRetractionHandleObject *v, FILE *fp, int flags)
{
    fprintf(fp, "<%s=%lu:%lu>", v->ob_type->tp_name,
        v->ob_value.sendingFederate, v->ob_value.theSerialNumber);
    return 0;
}

static int
EventRetractionHandle_compare(EventRetractionHandleObject *v, EventRetractionHandleObject *w)
{
    return v->ob_value.sendingFederate < w->ob_value.sendingFederate ? -1 :
        v->ob_value.sendingFederate > w->ob_value.sendingFederate ? 1 :
        v->ob_value.theSerialNumber < w->ob_value.theSerialNumber ? -1 :
        v->ob_value.theSerialNumber > w->ob_value.theSerialNumber ? 1 : 0;
}

static PyObject *
EventRetractionHandle_repr(EventRetractionHandleObject *v)
{
    return PyString_FromFormat("<%s=%lu:%lu>", v->ob_type->tp_name,
        v->ob_value.sendingFederate, v->ob_value.theSerialNumber);
}

static long
EventRetractionHandle_hash(EventRetractionHandleObject *v)
{
    // create a "signed long" hash from an "unsigned long" number
    static const int __shift = 15; // see SHIFT in longintrepr.h
    return (v->ob_value.sendingFederate >> __shift) | (v->ob_value.sendingFederate & ~0UL << __shift)
         | (v->ob_value.theSerialNumber >> __shift) | (v->ob_value.theSerialNumber & ~0UL << __shift);
}

PyTypeObject EventRetractionHandleType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    MODULE_NAME ".EventRetractionHandle", /* tp_name */
    sizeof(RtiULongHandleObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    (printfunc)EventRetractionHandle_print, /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    (cmpfunc)EventRetractionHandle_compare, /* tp_compare */
    (reprfunc)EventRetractionHandle_repr, /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    (hashfunc)EventRetractionHandle_hash, /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "EventRetractionHandle",   /* tp_doc */
};

int
EventRetractionHandle_FromPython(EventRetractionHandleObject *value, RTI::EventRetractionHandle *result)
{
    if(value == NULL || !PyObject_TypeCheck(value, &EventRetractionHandleType)) {
        PyErr_SetString(PyExc_TypeError,
            "EventRetractionHandle object required");
        return 0; // failure
    }

    *result = value->ob_value;
    return 1; // success
}

PyObject *
EventRetractionHandle_ToPython(RTI::EventRetractionHandle *value)
{
    EventRetractionHandleObject *self =
        PyObject_New(EventRetractionHandleObject, &EventRetractionHandleType);
    if (self == NULL)
        return NULL;
    self->ob_value = *value;

    return (PyObject *)self;
}

int
RTIfedTime_FromPython(PyObject *value, RTIfedTime *result)
{
    PyObject *number = PyNumber_Float(value);
    if(number == NULL)
        return 0; // failure

    *result = PyFloat_AsDouble(number);

    Py_DECREF(number);
    return 1; // success
}

class HandlesInitializer : public RtiInitializer
{
public:
    HandlesInitializer() : RtiInitializer() {}
    void on_init(PyObject* module);
};

static HandlesInitializer g_initializer;

void
HandlesInitializer::on_init(PyObject* module)
{
    if (PyType_Ready(&RtiObjectClassHandleType) < 0)
        return;
    Py_INCREF(&RtiObjectClassHandleType);
    PyModule_AddObject(module, "ObjectClassHandle", (PyObject *)&RtiObjectClassHandleType);

    if (PyType_Ready(&RtiInteractionClassHandleType) < 0)
        return;
    Py_INCREF(&RtiInteractionClassHandleType);
    PyModule_AddObject(module, "InteractionClassHandle", (PyObject *)&RtiInteractionClassHandleType);

    if (PyType_Ready(&RtiAttributeHandleType) < 0)
        return;
    Py_INCREF(&RtiAttributeHandleType);
    PyModule_AddObject(module, "AttributeHandle", (PyObject *)&RtiAttributeHandleType);

    if (PyType_Ready(&RtiParameterHandleType) < 0)
        return;
    Py_INCREF(&RtiParameterHandleType);
    PyModule_AddObject(module, "ParameterHandle", (PyObject *)&RtiParameterHandleType);

    if (PyType_Ready(&RtiObjectHandleType) < 0)
        return;
    Py_INCREF(&RtiObjectHandleType);
    PyModule_AddObject(module, "ObjectHandle", (PyObject *)&RtiObjectHandleType);

    if (PyType_Ready(&RtiFederateHandleType) < 0)
        return;
    Py_INCREF(&RtiFederateHandleType);
    PyModule_AddObject(module, "FederateHandle", (PyObject *)&RtiFederateHandleType);

    if (PyType_Ready(&RtiOrderTypeType) < 0)
        return;
    Py_INCREF(&RtiOrderTypeType);
    PyModule_AddObject(module, "OrderType", (PyObject *)&RtiOrderTypeType);

    if (PyType_Ready(&RtiTransportTypeType) < 0)
        return;
    Py_INCREF(&RtiTransportTypeType);
    PyModule_AddObject(module, "TransportType", (PyObject *)&RtiTransportTypeType);

    if (PyType_Ready(&EventRetractionHandleType) < 0)
        return;
    Py_INCREF(&EventRetractionHandleType);
    PyModule_AddObject(module, "EventRetractionHandle", (PyObject *)&EventRetractionHandleType);
}

// $Id: handles.cpp,v 1.2 2008/10/01 21:08:58 gotthardp Exp $
