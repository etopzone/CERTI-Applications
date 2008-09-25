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
# $Id: __init__.py,v 1.1 2008/09/25 17:17:42 gotthardp Exp $

import xml.sax.handler

from hla._omt import *
from array import HLAfixedArray, HLAvariableArray
from record import HLAfixedRecord, HLAvariantRecord

class TypeParser(xml.sax.handler.ContentHandler):
    def __init__(self):
        self.inRecord = False

    def startElement(self, name, attributes):

        if name == "simpleData":
            # name = HLAencoding('HLAinteger32LE')
            globals()[attributes["name"]] = HLAencoding(attributes["representation"])

        elif name == "enumeratedData":
            pass

        elif name == "arrayData":
            if(attributes["cardinality"].lower() == "dynamic"):
                cardinality = None
            else:
                cardinality = attributes["cardinality"]

            # name = HLAfixedArray(HLAinteger32LE, 3)
            globals()[attributes["name"]] = HLAencoding(
                attributes["encoding"], (HLAencoding(attributes["dataType"]), cardinality))

        elif name == "fixedRecordData":
            self.inRecord = True
    	    self.recordName = attributes["name"]
    	    self.recordFields = []

        elif name == "field":
            if not self.inRecord:
                return
    	    self.recordFields += [(attributes["name"], HLAencoding(attributes["dataType"]))]

        elif name == "variantRecordData":
            pass

        elif name == "alternative":
            pass

    def endElement(self, name):

        if name == "fixedRecordData":
            if not self.inRecord:
                return

            # name = HLAfixedRecord([("X",HLAinteger32LE), ("Y",HLAinteger32LE)])
            globals()[self.recordName] = HLAfixedRecord(self.recordFields)
            self.inRecord = False

class HLAencoding:
    def __init__(self, typeName, typeParameters = None):
        self.typeName = typeName
        self.typeParameters = typeParameters
        self._encoding = None

    @property
    def encoding(self):
        if(self._encoding == None):
            if(self.typeParameters == None):
                self._encoding = globals()[self.typeName]
            else:
                self._encoding = globals()[self.typeName](self.typeParameters)
        return self._encoding;

    @property
    def octetBoundary(self):
        return self.encoding.octetBoundary

    def pack(self, value):
        return self.encoding.pack(value)

    def unpack(self, buffer, offset = 0):
        return self.encoding.unpack(buffer, offset)

def HLAuse(filename):
    handler = TypeParser()

    parser = xml.sax.make_parser()
    parser.setContentHandler(handler)
    parser.parse(filename)

# $Id: __init__.py,v 1.1 2008/09/25 17:17:42 gotthardp Exp $
