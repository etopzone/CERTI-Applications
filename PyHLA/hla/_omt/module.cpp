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
 * $Id: module.cpp,v 1.3 2008/11/15 14:34:05 gotthardp Exp $
 */

// note: you must include Python.h before any standard headers are included
#include <Python.h>
#include <structmember.h>

#include "module.h"

typedef std::vector<OmtInitializer *> OmtInitializerList;

// this is to avoid the "static initialization order fiasco"
static OmtInitializerList& getOmtInitializers()
{
    static OmtInitializerList initializers;
    return initializers;
}

OmtInitializer::OmtInitializer()
{
    getOmtInitializers().push_back(this);
}

OmtInitializer::~OmtInitializer()
{
    for(OmtInitializerList::iterator pos = getOmtInitializers().begin();
        pos != getOmtInitializers().end(); pos++)
    {
        if(*pos == this)
        {
            getOmtInitializers().erase(pos);
            break;
        }
    }
}

void OmtInitializer::init(PyObject *module)
{
    for(OmtInitializerList::iterator pos = getOmtInitializers().begin();
        pos != getOmtInitializers().end(); pos++)
    {
        (*pos)->on_init(module);
    }
}

static PyMethodDef omt_methods[] = {
  {NULL, NULL, 0, NULL} // sentinel
};

PyMODINIT_FUNC
init_omt(void)
{
    PyObject* module = Py_InitModule3("_omt", omt_methods,
        "Modeling and Simulation (M&S) High Level Architecture (HLA) -- Object Model Template (OMT).");

    // call initializers
    OmtInitializer::init(module);
}

// $Id: module.cpp,v 1.3 2008/11/15 14:34:05 gotthardp Exp $
