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
 * $Id: module.h,v 1.2 2008/11/15 14:34:05 gotthardp Exp $
 */

#ifndef OMT_MODULE_H
#define OMT_MODULE_H

#include <vector>

#define MODULE_NAME "omt"

class OmtInitializer
{
public:
    OmtInitializer();
    virtual ~OmtInitializer();

    static void init(PyObject *module);
    virtual void on_init(PyObject *module) = 0;
};

#endif // OMT_MODULE_H

// $Id: module.h,v 1.2 2008/11/15 14:34:05 gotthardp Exp $
