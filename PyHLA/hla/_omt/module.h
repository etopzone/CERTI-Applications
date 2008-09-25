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
 * $Id: module.h,v 1.1 2008/09/25 17:17:32 gotthardp Exp $
 */

#ifndef DATA_MODULE_H
#define DATA_MODULE_H

#include <vector>

#define MODULE_NAME "data"

class DataInitializer
{
public:
    DataInitializer();
    virtual ~DataInitializer();

    static void init(PyObject *module);
    virtual void on_init(PyObject *module) = 0;

private:
    typedef std::vector<DataInitializer *> DataInitializerList;
    static DataInitializerList m_initializers;
};

#endif // DATA_MODULE_H

// $Id: module.h,v 1.1 2008/09/25 17:17:32 gotthardp Exp $
