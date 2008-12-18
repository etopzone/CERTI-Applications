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
 * $Id: basicdata.cpp,v 1.5 2008/12/18 19:41:56 gotthardp Exp $
 */

// note: you must include Python.h before any standard headers are included
#include <Python.h>
#include <structmember.h>

#include "basicdata.h"
#include "module.h"

#ifdef _MSC_VER
typedef unsigned __int64  uint64_t;
typedef __int64           int64_t;
typedef unsigned __int32  uint32_t;
typedef __int32           int32_t;
typedef unsigned __int16  uint16_t;
typedef __int16           int16_t;
typedef unsigned __int8   uint8_t;
typedef __int8            int8_t;
#else
#include <inttypes.h>
#endif

//! Swap <i> bytes of the <T> type
/*! Template specializations are defined for each possible <i>.
 */
template<class T, int i = sizeof(T)>
struct __swap;

//! Conversion to the Little Endian encoding
template<class T>
struct LittleEndian
{
    inline const T operator()(const T& x) const {
#if __BYTE_ORDER == __LITTLE_ENDIAN
        return x;
#elif __BYTE_ORDER == __BIG_ENDIAN
        return __swap<T>()( x );
#else
#error Undefined __BYTE_ORDER
#endif
    }
};

//! Conversion to the Big Endian encoding
template<class T>
struct BigEndian
{
    inline const T operator()(const T& x) const {
#if __BYTE_ORDER == __LITTLE_ENDIAN
        return __swap<T>()( x );
#elif __BYTE_ORDER == __BIG_ENDIAN
        return x;
#endif
    }
};

template<class T>
struct __swap<T,1>
{
    inline const T operator()(const T& x) const {
        return x;
    }
};

template<class T>
struct __swap<T,2>
{
    inline const T operator()(const T& x) const {
        union {
            uint16_t u16;
            T x;
        } result;
        result.u16 =
            (*(uint16_t*)&x<<8 | *(uint16_t*)&x>>8);
        return result.x;
    }
};

template<class T>
struct __swap<T,4>
{
    inline const T operator()(const T& x) const {
        union {
            uint32_t u32;
            T x;
        } result;
        result.u32 =
            (*(uint32_t*)&x<<24 | *(uint32_t*)&x>>24 |
            (*(uint32_t*)&x & 0x0000ff00UL)<<8 |
            (*(uint32_t*)&x & 0x00ff0000UL)>>8);
        return result.x;
    }
};

template<class T>
struct __swap<T,8>
{
    inline const T operator()(const T& x) const {
        union {
            uint64_t u64;
            T x;
        } result;
        result.u64 =
            (*(uint64_t*)&x<<56 | *(uint64_t*)&x>>56 |
            (*(uint64_t*)&x & 0x000000000000ff00ULL)<<40 |
            (*(uint64_t*)&x & 0x0000000000ff0000ULL)<<24 |
            (*(uint64_t*)&x & 0x00000000ff000000ULL)<< 8 |
            (*(uint64_t*)&x & 0x000000ff00000000ULL)>> 8 |
            (*(uint64_t*)&x & 0x0000ff0000000000ULL)>>24 |
            (*(uint64_t*)&x & 0x00ff000000000000ULL)>>40);
        return result.x;
    }
};

template<class T>
const T*
getUnpackBuffer(PyObject *args, size_t *length = NULL)
{
    const char *buffer;
    int size;
    long int offset = 0;
    if(!PyArg_ParseTuple(args, "s#|l", &buffer, &size, &offset))
        return NULL;
    if(size-offset < (int)sizeof(T)) {
        PyErr_Format(PyExc_TypeError, "need at least %d bytes", offset+sizeof(T));
        return NULL;
    }
    if(length != NULL)
        *length = size-offset;

    return (T*)(buffer+offset);
}

static PyObject *
createObjectSizeTuple(PyObject *object, long int size)
{
    return PyTuple_Pack(2,
        object,
        PyInt_FromLong(size));
}

/*
 *  HLAinteger16BE
 */
static PyObject *
HLAinteger16BE_pack(PyObject *self, PyObject *args)
{
    short int value;
    if(!PyArg_ParseTuple(args, "h", &value))
        return NULL;
    int16_t buffer = BigEndian<int16_t>()(value);
    return PyString_FromStringAndSize((const char *)&buffer, sizeof(int16_t));
}

static PyObject *
HLAinteger16BE_unpack(PyObject *self, PyObject *args)
{
    const int16_t *buffer = getUnpackBuffer<int16_t>(args);
    if(buffer == NULL)
        return NULL;
    short int value = BigEndian<int16_t>()(*buffer);
    return createObjectSizeTuple(PyLong_FromLong(value), sizeof(int16_t));
}

static PyMethodDef HLAinteger16BE_methods[] =
{
    {"pack", (PyCFunction)HLAinteger16BE_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAinteger16BE_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

/*
 *  HLAinteger32BE
 */
static PyObject *
HLAinteger32BE_pack(PyObject *self, PyObject *args)
{
    long int value;
    if(!PyArg_ParseTuple(args, "l", &value))
        return NULL;
    int32_t buffer = BigEndian<int32_t>()(value);
    return PyString_FromStringAndSize((const char *)&buffer, sizeof(int32_t));
}

static PyObject *
HLAinteger32BE_unpack(PyObject *self, PyObject *args)
{
    const int32_t *buffer = getUnpackBuffer<int32_t>(args);
    if(buffer == NULL)
        return NULL;
    long int value = BigEndian<int32_t>()(*buffer);
    return createObjectSizeTuple(PyLong_FromLong(value), sizeof(int32_t));
}

static PyMethodDef HLAinteger32BE_methods[] =
{
    {"pack", (PyCFunction)HLAinteger32BE_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAinteger32BE_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

#ifdef HAVE_LONG_LONG

/*
 *  HLAinteger64BE
 */
static PyObject *
HLAinteger64BE_pack(PyObject *self, PyObject *args)
{
    PY_LONG_LONG value;
    if(!PyArg_ParseTuple(args, "L", &value))
        return NULL;
    int64_t buffer = BigEndian<int64_t>()(value);
    return PyString_FromStringAndSize((const char *)&buffer, sizeof(int64_t));
}

static PyObject *
HLAinteger64BE_unpack(PyObject *self, PyObject *args)
{
    const int64_t *buffer = getUnpackBuffer<int64_t>(args);
    if(buffer == NULL)
        return NULL;
    PY_LONG_LONG value = BigEndian<int64_t>()(*buffer);
    return createObjectSizeTuple(PyLong_FromLongLong(value), sizeof(int64_t));
}

static PyMethodDef HLAinteger64BE_methods[] =
{
    {"pack", (PyCFunction)HLAinteger64BE_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAinteger64BE_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

#endif

/*
 *  HLAfloat32BE
 */
static PyObject *
HLAfloat32BE_pack(PyObject *self, PyObject *args)
{
    float value;
    if(!PyArg_ParseTuple(args, "f", &value))
        return NULL;
    float buffer = BigEndian<float>()(value);
    return PyString_FromStringAndSize((const char *)&buffer, sizeof(float));
}

static PyObject *
HLAfloat32BE_unpack(PyObject *self, PyObject *args)
{
    const float *buffer = getUnpackBuffer<float>(args);
    if(buffer == NULL)
        return NULL;
    float value = BigEndian<float>()(*buffer);
    return createObjectSizeTuple(PyFloat_FromDouble(value), sizeof(float));
}

static PyMethodDef HLAfloat32BE_methods[] =
{
    {"pack", (PyCFunction)HLAfloat32BE_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAfloat32BE_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

/*
 *  HLAfloat64BE
 */
static PyObject *
HLAfloat64BE_pack(PyObject *self, PyObject *args)
{
    double value;
    if(!PyArg_ParseTuple(args, "d", &value))
        return NULL;
    double buffer = BigEndian<double>()(value);
    return PyString_FromStringAndSize((const char *)&buffer, sizeof(double));
}

static PyObject *
HLAfloat64BE_unpack(PyObject *self, PyObject *args)
{
    const double *buffer = getUnpackBuffer<double>(args);
    if(buffer == NULL)
        return NULL;
    double value = BigEndian<double>()(*buffer);
    return createObjectSizeTuple(PyFloat_FromDouble(value), sizeof(double));
}

static PyMethodDef HLAfloat64BE_methods[] =
{
    {"pack", (PyCFunction)HLAfloat64BE_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAfloat64BE_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

/*
 *  HLAinteger16LE
 */
static PyObject *
HLAinteger16LE_pack(PyObject *self, PyObject *args)
{
    short int value;
    if(!PyArg_ParseTuple(args, "h", &value))
        return NULL;
    int16_t buffer = LittleEndian<int16_t>()(value);
    return PyString_FromStringAndSize((const char *)&buffer, sizeof(int16_t));
}

static PyObject *
HLAinteger16LE_unpack(PyObject *self, PyObject *args)
{
    const int16_t *buffer = getUnpackBuffer<int16_t>(args);
    if(buffer == NULL)
        return NULL;
    short int value = LittleEndian<int16_t>()(*buffer);
    return createObjectSizeTuple(PyLong_FromLong(value), sizeof(int16_t));
}

static PyMethodDef HLAinteger16LE_methods[] =
{
    {"pack", (PyCFunction)HLAinteger16LE_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAinteger16LE_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

/*
 *  HLAinteger32LE
 */
static PyObject *
HLAinteger32LE_pack(PyObject *self, PyObject *args)
{
    long int value;
    if(!PyArg_ParseTuple(args, "l", &value))
        return NULL;
    int32_t buffer = LittleEndian<int32_t>()(value);
    return PyString_FromStringAndSize((const char *)&buffer, sizeof(int32_t));
}

static PyObject *
HLAinteger32LE_unpack(PyObject *self, PyObject *args)
{
    const int32_t *buffer = getUnpackBuffer<int32_t>(args);
    if(buffer == NULL)
        return NULL;
    long int value = LittleEndian<int32_t>()(*buffer);
    return createObjectSizeTuple(PyLong_FromLong(value), sizeof(int32_t));
}

static PyMethodDef HLAinteger32LE_methods[] =
{
    {"pack", (PyCFunction)HLAinteger32LE_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAinteger32LE_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

#ifdef HAVE_LONG_LONG

/*
 *  HLAinteger64LE
 */
static PyObject *
HLAinteger64LE_pack(PyObject *self, PyObject *args)
{
    PY_LONG_LONG value;
    if(!PyArg_ParseTuple(args, "L", &value))
        return NULL;
    int64_t buffer = LittleEndian<int64_t>()(value);
    return PyString_FromStringAndSize((const char *)&buffer, sizeof(int64_t));
}

static PyObject *
HLAinteger64LE_unpack(PyObject *self, PyObject *args)
{
    const int64_t *buffer = getUnpackBuffer<int64_t>(args);
    if(buffer == NULL)
        return NULL;
    PY_LONG_LONG value = LittleEndian<int64_t>()(*buffer);
    return createObjectSizeTuple(PyLong_FromLongLong(value), sizeof(int64_t));
}

static PyMethodDef HLAinteger64LE_methods[] =
{
    {"pack", (PyCFunction)HLAinteger64LE_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAinteger64LE_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

#endif

/*
 *  HLAfloat32LE
 */
static PyObject *
HLAfloat32LE_pack(PyObject *self, PyObject *args)
{
    float value;
    if(!PyArg_ParseTuple(args, "f", &value))
        return NULL;
    float buffer = LittleEndian<float>()(value);
    return PyString_FromStringAndSize((const char *)&buffer, sizeof(float));
}

static PyObject *
HLAfloat32LE_unpack(PyObject *self, PyObject *args)
{
    const float *buffer = getUnpackBuffer<float>(args);
    if(buffer == NULL)
        return NULL;
    float value = LittleEndian<float>()(*buffer);
    return createObjectSizeTuple(PyFloat_FromDouble(value), sizeof(float));
}

static PyMethodDef HLAfloat32LE_methods[] =
{
    {"pack", (PyCFunction)HLAfloat32LE_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAfloat32LE_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

/*
 *  HLAfloat64LE
 */
static PyObject *
HLAfloat64LE_pack(PyObject *self, PyObject *args)
{
    double value;
    if(!PyArg_ParseTuple(args, "d", &value))
        return NULL;
    double buffer = LittleEndian<double>()(value);
    return PyString_FromStringAndSize((const char *)&buffer, sizeof(double));
}

static PyObject *
HLAfloat64LE_unpack(PyObject *self, PyObject *args)
{
    const double *buffer = getUnpackBuffer<double>(args);
    if(buffer == NULL)
        return NULL;
    double value = LittleEndian<double>()(*buffer);
    return createObjectSizeTuple(PyFloat_FromDouble(value), sizeof(double));
}

static PyMethodDef HLAfloat64LE_methods[] =
{
    {"pack", (PyCFunction)HLAfloat64LE_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAfloat64LE_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

/*
 *  HLAoctet
 */
static PyObject *
HLAoctet_pack(PyObject *self, PyObject *args)
{
    char buffer;
    if(!PyArg_ParseTuple(args, "c", &buffer))
        return NULL;
    return PyString_FromStringAndSize(&buffer, sizeof(char));
}

static PyObject *
HLAoctet_unpack(PyObject *self, PyObject *args)
{
    const char *buffer = getUnpackBuffer<char>(args);
    if(buffer == NULL)
        return NULL;
    return createObjectSizeTuple(PyString_FromStringAndSize(buffer, sizeof(char)), sizeof(char));
}

static PyMethodDef HLAoctet_methods[] =
{
    {"pack", (PyCFunction)HLAoctet_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAoctet_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

/*
 *  HLAASCIIstring
 */
static PyObject *
HLAASCIIstring_pack(PyObject *self, PyObject *args)
{
    const char* value;
    int length;
    if(!PyArg_ParseTuple(args, "s#", &value, &length))
        return NULL;

    char *buffer = (char *)malloc(sizeof(int32_t)+length);
    *(int32_t *)buffer = BigEndian<int32_t>()(length);
    memcpy(buffer+sizeof(int32_t), value, length);

    PyObject *result = PyString_FromStringAndSize(buffer, sizeof(int32_t)+length);
    free(buffer);

    return result;
}

static PyObject *
HLAASCIIstring_unpack(PyObject *self, PyObject *args)
{
    size_t size;
    const int32_t *buffer = getUnpackBuffer<int32_t>(args, &size);
    if(buffer == NULL)
        return NULL;

    int length = BigEndian<int32_t>()(*buffer);
    if(size < sizeof(int32_t)+length) {
        PyErr_Format(PyExc_TypeError, "need at least %d bytes", sizeof(int32_t)+length);
        return NULL;
    }

    return createObjectSizeTuple(
        PyString_FromStringAndSize((const char *)buffer+sizeof(int32_t), length),
        sizeof(int32_t)+length);
}

static PyMethodDef HLAASCIIstring_methods[] =
{
    {"pack", (PyCFunction)HLAASCIIstring_pack, METH_VARARGS, NULL},
    {"unpack", (PyCFunction)HLAASCIIstring_unpack, METH_VARARGS, NULL},
    {NULL} // sentinel
};

typedef struct {
  const char* co_name;
  long co_size;
  PyMethodDef *co_methods;
} PyCodingDef;

static PyCodingDef basic_coding[] =
{
    {"HLAinteger16BE", sizeof(int16_t), HLAinteger16BE_methods},
    {"HLAinteger32BE", sizeof(int32_t), HLAinteger32BE_methods},
#ifdef HAVE_LONG_LONG
    {"HLAinteger64BE", sizeof(int64_t), HLAinteger64BE_methods},
#endif
    {"HLAfloat32BE", sizeof(float), HLAfloat32BE_methods},
    {"HLAfloat64BE", sizeof(double), HLAfloat64BE_methods},
    {"HLAinteger16LE", sizeof(int16_t), HLAinteger16LE_methods},
    {"HLAinteger32LE", sizeof(int32_t), HLAinteger32LE_methods},
#ifdef HAVE_LONG_LONG
    {"HLAinteger64LE", sizeof(int64_t), HLAinteger64LE_methods},
#endif
    {"HLAfloat32LE", sizeof(float), HLAfloat32LE_methods},
    {"HLAfloat64LE", sizeof(double), HLAfloat64LE_methods},
    {"HLAoctet", sizeof(char), HLAoctet_methods},
    {"HLAASCIIstring", sizeof(char), HLAASCIIstring_methods},
    {NULL} // sentinel
};

PyTypeObject RtiCodingType =
{
  PyObject_HEAD_INIT(NULL)
  0,                         /* ob_size */
  MODULE_NAME ".Coding",     /* tp_name */
  sizeof(RtiCoding),         /* tp_basicsize */
  0,                         /* tp_itemsize */
  0,                         /* tp_dealloc */
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
  "Coding",                  /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  0,                         /* tp_methods */
  0,                         /* tp_members */
  0,                         /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  offsetof(RtiCoding, ob_dict), /* tp_dictoffset */
  0,                         /* tp_init */
  0,                         /* tp_alloc */
  0,                         /* tp_new */
};

static void
add_encoding(PyObject* dict, const char *name, long size, PyMethodDef *methods)
{
    PyMethodDef *pos;
    RtiCoding *result = (RtiCoding *)RtiCodingType.tp_alloc(&RtiCodingType, 0);

    result->ob_dict = PyDict_New();
    if(result->ob_dict == NULL)
        return; // error occurred

    PyObject* octetBoundary = PyInt_FromLong(size);
    if(octetBoundary == NULL)
        return; // error occurred
    PyDict_SetItemString(result->ob_dict, "octetBoundary", octetBoundary);
    // the PyDict_SetItemString has increased the counter
    Py_DECREF(octetBoundary);

    for(pos = methods; pos->ml_name != NULL; pos++)
    {
        PyObject *method = PyCFunction_New(pos, NULL);
        if(method == NULL)
            return; // error occurred
        PyDict_SetItemString(result->ob_dict, pos->ml_name, method);
        Py_DECREF(method);
    }

    PyDict_SetItemString(dict, name, (PyObject *)result);
}

class BasicDataInitializer : public OmtInitializer
{
public:
    BasicDataInitializer() : OmtInitializer() {}
    void on_init(PyObject* module);
};
    
static BasicDataInitializer g_initializer;

void
BasicDataInitializer::on_init(PyObject *module)
{
    PyCodingDef *pos;

    PyObject* dict = PyModule_GetDict(module);
    if(dict == NULL)
        return; // error occurred

    if (PyType_Ready(&RtiCodingType) < 0)
        return;

    for(pos = basic_coding; pos->co_name != NULL; pos++)
        add_encoding(dict, pos->co_name, pos->co_size, pos->co_methods);
}

// $Id: basicdata.cpp,v 1.5 2008/12/18 19:41:56 gotthardp Exp $
