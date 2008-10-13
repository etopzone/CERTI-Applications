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
# $Id: record.py,v 1.3 2008/10/13 17:15:41 gotthardp Exp $

from hla._omt import *
from basic import *

# For example:
# +-------------+------------------------------------+----------------+-----------+
# |             | Field                              |                |           |
# | Record name +---------+--------------+-----------+ Encoding       | Semantics |
# |             | Name    | Type         | Semantics |                |           |
# +-------------+---------+--------------+-----------+----------------+-----------+
# |             | FIELD_X | HLAfloat32LE |           |                |           |
# |             +---------+--------------+-----------+                |           |
# | Coordinates | FIELD_Y | HLAfloat32LE |           | HLAfixedRecord |           |
# |             +---------+--------------+-----------+                |           |
# |             | FIELD_Z | HLAfloat32LE |           |                |           |
# +-------------+---------+--------------+-----------+----------------+-----------+

class HLAfixedRecord:
    def __init__(self, typeName, fields):
        self.fields = fields
        # to enable late binding, the octetBoundary cannot be evaluated now
        self._octetBoundary = None

    @property
    def octetBoundary(self):
        if(self._octetBoundary == None):
            self._octetBoundary = 0
            for fieldName, fieldType in self.fields:
                self._octetBoundary = max(self._octetBoundary, fieldType.octetBoundary)
        return self._octetBoundary

    def pack(self, value):
        buffer = ""
        for fieldName, fieldType in self.fields:
            buffer += padding(len(buffer), fieldType.octetBoundary)*'\0'
            buffer += fieldType.pack(value[fieldName])
        return buffer

    def unpack(self, buffer, offset = 0):
        value, size = {}, 0
        for fieldName, fieldType in self.fields:
            size += padding(size, fieldType.octetBoundary)
            myvalue, mysize = fieldType.unpack(buffer[offset+size:])
            value[fieldName] = myvalue
            size += mysize
        return value, size

# For example:
# +-------------+-------------------------------+------------------------------------+------------------+-----------+
# |             | Discriminant                  | Alternative                        |                  |           |
# | Record name +------+-----------+------------+---------+--------------+-----------+ Encoding         | Semantics |
# |             | Name | Type      | Enumerator | Name    | Type         | Semantics |                  |           |
# +-------------+------+-----------+------------+---------+--------------+-----------+------------------+-----------+
# |             |      |           | AXIS_X     | FIELD_X | HLAfloat32LE |           |                  |           |
# | Coordinates | TYPE | TypesEnum +------------+---------+--------------+-----------+ HLAvariantRecord |           |
# |             |      |           | AXIS_Y     | FIELD_Y | HLAfloat32LE |           |                  |           |
# +-------------+------+-----------+------------+---------+--------------+-----------+------------------+-----------+

# Enumerator = None ... HLAother
class HLAvariantRecord:
    def __init__(self, typeName, discriminant, alternatives):
        self.discriminantName, self.discriminantType = discriminant
        self.alternatives = alternatives
        # to enable late binding, the octetBoundary cannot be evaluated now
        self._octetBoundary = None

    @property
    def octetBoundary(self):
        if(self._octetBoundary == None):
            self._octetBoundary = 0
            for fieldEnumerator, fieldName, fieldType in self.alternatives:
                self._octetBoundary = max(self._octetBoundary, fieldType.octetBoundary)
        return self._octetBoundary

    def getAlternative(self, discriminantValue):
        # step 1: search the enumerators
        for fieldEnumerator, fieldName, fieldType in self.alternatives:
            if(discriminantValue in fieldEnumerator):
                return fieldName, fieldType
        # step 2: look for the 'HLAother' symbol
        for fieldEnumerator, fieldName, fieldType in self.alternatives:
            if(None in fieldEnumerator):
                return fieldName, fieldType
        # not found
        return None, None

    def pack(self, value):
        buffer = self.discriminantType.pack(value[self.discriminantName])
        fieldName, fieldType = self.getAlternative(value[self.discriminantName])
        if(fieldName != None):
            buffer += padding(len(buffer), fieldType.octetBoundary)*'\0'
            buffer += fieldType.pack(value[fieldName])
        else:
            buffer += padding(len(buffer), self.octetBoundary)*'\0'
        return buffer

    def unpack(self, buffer, offset = 0):
        myvalue, size = self.discriminantType.unpack(buffer[offset:])
        value = {self.discriminantName:myvalue}
        fieldName, fieldType = self.getAlternative(value[self.discriminantName])
        if(fieldName != None):
            size += padding(size, fieldType.octetBoundary)
            myvalue, mysize = fieldType.unpack(buffer[offset+size:])
            value[fieldName] = myvalue
            size += mysize
        else:
            buffer += padding(len(buffer), self.octetBoundary)*'\0'
        return value, size

# $Id: record.py,v 1.3 2008/10/13 17:15:41 gotthardp Exp $
