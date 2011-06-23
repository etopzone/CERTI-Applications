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
 * $Id: module.h,v 1.4 2011/06/23 00:10:08 gotthardp Exp $
 */

#ifndef RTI_MODULE_H
#define RTI_MODULE_H

#include <vector>

#define MODULE_NAME "rti"

#if PY_VERSION_HEX < 0x02060000
// Python 2.5 compatibility macros
#define Py_TYPE(o) (((PyObject*)(o))->ob_type)
#define PyBytes_AsString PyString_AsString
#define PyBytes_FromStringAndSize PyString_FromStringAndSize
#define PyUnicode_AsString PyString_AsString
#define PyUnicode_FromFormat PyString_FromFormat
#define PyUnicode_FromString PyString_FromString
#endif

template<typename T>
class auto_arrayptr
{
public:
    auto_arrayptr(T *ptr = NULL) { m_ptr = ptr; }
    ~auto_arrayptr() { delete[] m_ptr; }

    operator T*() const { return m_ptr; }
    T* get() const { return m_ptr; }

private:
    T* m_ptr;
};

template<typename T>
class auto_decref
{
public:
    auto_decref(T *ptr = NULL) { m_ptr = ptr; }
    ~auto_decref() { Py_DECREF(m_ptr); }

    operator T*() const { return m_ptr; }
    T* get() const { return m_ptr; }

private:
    T* m_ptr;
};

class RtiInitializer
{
public:
    RtiInitializer();
    virtual ~RtiInitializer();

    static void init(PyObject *module);
    virtual void on_init(PyObject *module) = 0;
};

#endif // RTI_MODULE_H

// $Id: module.h,v 1.4 2011/06/23 00:10:08 gotthardp Exp $
