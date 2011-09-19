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
# $Id: array.py,v 1.5 2011/09/19 12:56:33 erk Exp $

from hla._omt import *
from .basic import *

# For example:
# +-------------+----------------+-------------+-----------------+-----------+
# | Name        | Element type   | Cardinality | Encoding        | Semantics |
# +-------------+----------------+-------------+-----------------+-----------+
# | Coordinates | HLAinteger32BE | 3           | HLAfixedArray   |           |
# +-------------+----------------+-------------+-----------------+-----------+
# | List        | HLAinteger32BE | Dynamic     | HLAvaribleArray |           |
# +-------------+----------------+-------------+-----------------+-----------+

class HLAfixedArray:
    def __init__(self, typeName, elementType, cardinality):
        self.elementType = elementType
        if(cardinality == None):
            raise TypeError("HLAfixedArray cannot have dynamic cardinality")
        self.cardinality = cardinality

    @property
    def octetBoundary(self):
        return self.elementType.octetBoundary

    def pack(self, value):
        buffer = bytes()
        if(len(value) != self.cardinality):
            raise TypeError("expecting %i elements" % (self.cardinality))

        for element in value:
            buffer += padding(len(buffer), self.elementType.octetBoundary) * bytes("\0", encoding='utf-8')
            buffer += self.elementType.pack(element)

        return buffer

    def unpack(self, buffer, offset = 0):
        value, size = (), 0

        for pos in range(self.cardinality):
            size += padding(size, self.elementType.octetBoundary)
            myvalue, mysize = self.elementType.unpack(buffer[offset+size:])
            value, size = value+(myvalue,), size+mysize

        return value, size

class HLAvariableArray:
    # if cardinality = None, any cardinality is accepted (Dynamic cardinality)
    # note: we support HLAvariableArray encoding with fixed cardinality
    def __init__(self, typeName, elementType, cardinality = None):
        self.elementType = elementType
        self.cardinality = cardinality

    @property
    def octetBoundary(self):
        return max(self.elementType.octetBoundary, HLAinteger32BE.octetBoundary)

    def pack(self, value):
        buffer = HLAinteger32BE.pack(len(value))
        if(self.cardinality != None and len(value) != self.cardinality):
            raise TypeError("expecting %i elements" % (self.cardinality))

        for element in value:
            buffer += padding(len(buffer), self.elementType.octetBoundary)* bytes("\0", encoding='utf-8')
            buffer += self.elementType.pack(element)

        return buffer

    def unpack(self, buffer, offset = 0):
        value = ()
        cardinality, size = HLAinteger32BE.unpack(buffer, offset)
        if(self.cardinality != None and cardinality != self.cardinality):
            raise TypeError("expecting %i elements" % (self.cardinality))

        for pos in range(cardinality):
            size += padding(size, self.elementType.octetBoundary)
            myvalue, mysize = self.elementType.unpack(buffer[offset+size:])
            value, size = value+(myvalue,), size+mysize

        return value, size

# $Id: array.py,v 1.5 2011/09/19 12:56:33 erk Exp $
