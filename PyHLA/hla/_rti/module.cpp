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
 * $Id: module.cpp,v 1.2 2008/11/15 14:34:06 gotthardp Exp $
 */

// note: you must include Python.h before any standard headers are included
#include <Python.h>
#include <structmember.h>

#include "module.h"

typedef std::vector<RtiInitializer *> RtiInitializerList;

// this is to avoid the "static initialization order fiasco"
static RtiInitializerList& getRtiInitializers()
{
    static RtiInitializerList initializers;
    return initializers;
}

RtiInitializer::RtiInitializer()
{
    getRtiInitializers().push_back(this);
}

RtiInitializer::~RtiInitializer()
{
    for(RtiInitializerList::iterator pos = getRtiInitializers().begin();
        pos != getRtiInitializers().end(); pos++)
    {
        if(*pos == this)
        {
            getRtiInitializers().erase(pos);
            break;
        }
    }
}

void
RtiInitializer::init(PyObject *module)
{
    for(RtiInitializerList::iterator pos = getRtiInitializers().begin();
        pos != getRtiInitializers().end(); pos++)
    {
        (*pos)->on_init(module);
    }
}

static PyObject *
spam_system(PyObject *self, PyObject *args)
{
    return Py_BuildValue("i", 7);
}

static PyMethodDef rti_methods[] = {
    {"system", spam_system, METH_VARARGS,
        "Execute a shell command."},
    {NULL, NULL, 0, NULL} // sentinel
};

PyMODINIT_FUNC
init_rti(void)
{
    PyObject* module = Py_InitModule3("_rti", rti_methods,
        "Modeling and Simulation (M&S) High Level Architecture (HLA) -- Basic Federate Interface.");

    // call initializers
    RtiInitializer::init(module);
}

// $Id: module.cpp,v 1.2 2008/11/15 14:34:06 gotthardp Exp $
