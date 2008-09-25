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
 * $Id: basicdata.h,v 1.1 2008/09/25 17:17:32 gotthardp Exp $
 */

#ifndef RTI_BASICDATA_H
#define RTI_BASICDATA_H

typedef struct {
    PyObject_HEAD
    PyObject *ob_dict;
} RtiCoding;

#endif // RTI_BASICDATA_H

// $Id: basicdata.h,v 1.1 2008/09/25 17:17:32 gotthardp Exp $
