#
# Python Language HLA API
# Copyright (C) 2008  Petr Gotthard <petr.gotthard@centrum.cz>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License version 2.1, as published by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# $Id: enumerated.py,v 1.2 2009/06/11 17:15:11 gotthardp Exp $

# For example:
# +------------+----------------+------------+--------+-----------+
# | Name       | Representation | Enumerator | Values | Semantics |
# +------------+----------------+------------+--------+-----------+
# |            |                | HLAfalse   | 0      |           |
# | HLAboolean | HLAinteger32BE +------------+--------+-----------+
# |            |                | HLAfalse   | 1      |           |
# +------------+----------------+------------+--------+-----------+

class HLAenumerator(int):
    def __new__(cls, typeName, name, value):
        return super(HLAenumerator, cls).__new__(cls, value)

    def __init__(self, typeName, name, value):
        self.__typeName = typeName
        self.__name = name

    def __repr__(self):
        return "<%s.%s=%d>" % (self.__typeName, self.__name, self)

    def __str__(self):
        return "%s.%s" % (self.__typeName, self.__name)

class HLAenumerated:
    def __init__(self, typeName, representation, enumerators):
        self.representation = representation
        self.enumerators = {}
        # initialize enumerators
        for key in enumerators.keys():
            self.enumerators[key] = HLAenumerator(typeName, key, enumerators[key])

    def __getattr__(self, name):
        return self.enumerators[name]

    @property
    def octetBoundary(self):
        return self.representation.octetBoundary

    def pack(self, value):
        return self.representation.pack(value)

    def unpack(self, buffer, offset = 0):
        value, size = self.representation.unpack(buffer, offset)
        # find a corresponding enumerator
        for name, enum in self.enumerators.items():
            if enum == value:
                return enum, size
        # if not found
        return value, size

# $Id: enumerated.py,v 1.2 2009/06/11 17:15:11 gotthardp Exp $
