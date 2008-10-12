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
 * $Id: module.cpp,v 1.2 2008/10/12 13:31:13 gotthardp Exp $
 */

// note: you must include Python.h before any standard headers are included
#include <Python.h>
#include <structmember.h>

#include "module.h"

DataInitializer::DataInitializerList DataInitializer::m_initializers;

DataInitializer::DataInitializer()
{
    m_initializers.push_back(this);
}

DataInitializer::~DataInitializer()
{
    for(DataInitializerList::iterator pos = m_initializers.begin();
        pos != m_initializers.end(); pos++)
    {
        if(*pos == this)
        {
            m_initializers.erase(pos);
            break;
        }
    }
}

void DataInitializer::init(PyObject *module)
{
    for(DataInitializerList::iterator pos = m_initializers.begin();
        pos != m_initializers.end(); pos++)
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
    DataInitializer::init(module);
}

// $Id: module.cpp,v 1.2 2008/10/12 13:31:13 gotthardp Exp $
