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
# $Id: __init__.py,v 1.2 2008/10/13 17:15:39 gotthardp Exp $

import xml.sax.handler

from hla._omt import *
from enumerated import HLAenumerated
from array import HLAfixedArray, HLAvariableArray
from record import HLAfixedRecord, HLAvariantRecord

class TypeParser(xml.sax.handler.ContentHandler):
    def __init__(self):
        self.inEnumerated = False
        self.inFixedRecord = False
        self.inVariantRecord = False

    def startElement(self, name, attributes):

        if name == "simpleData":
            # name = HLAencoding('HLAinteger32LE')
            globals()[attributes["name"]] = HLAencoding(attributes["representation"])

        elif name == "enumeratedData":
            self.inEnumerated = True
            self.enumName = attributes["name"]
            self.enumRepresentation = HLAencoding(attributes["representation"])
            self.enumEnumerators = []

        elif name == "enumerator":
            if not self.inEnumerated:
                return
    	    self.enumEnumerators += [(attributes["name"],int(attributes["values"]))]

        elif name == "arrayData":
            if(attributes["cardinality"].lower() == "dynamic"):
                cardinality = None
            else:
                cardinality = attributes["cardinality"]

            # name = HLAfixedArray("name", HLAinteger32LE, 3)
            globals()[attributes["name"]] = HLAencoding(attributes["encoding"],
                (attributes["name"], HLAencoding(attributes["dataType"]), cardinality))

        elif name == "fixedRecordData":
            self.inFixedRecord = True
    	    self.recordName = attributes["name"]
    	    self.recordFields = []

        elif name == "field":
            if not self.inFixedRecord:
                return
    	    self.recordFields += [(attributes["name"], HLAencoding(attributes["dataType"]))]

        elif name == "variantRecordData":
            self.inVariantRecord = True
            self.recordName = attributes["name"]
            self.recordEncoding = attributes["encoding"]
            self.recordDiscriminant = (attributes["discriminant"], attributes["dataType"])
            self.recordFields = []

        elif name == "alternative":
            if not self.inVariantRecord:
                return
            self.recordFields += [(attributes["enumerator"].split(','),
                attributes["name"], HLAencoding(attributes["dataType"]))]

    def endElement(self, name):

        if name == "enumeratedData":
            if not self.inEnumerated:
                return

            # name = HLAenumerated("name",
            #     HLAinteger32BE, {"HLAfalse":0, "HLAtrue":1})
            globals()[self.enumName] = HLAenumerated(
                self.enumName, self.enumRepresentation, dict(self.enumEnumerators))
            self.inEnumerated = False

        elif name == "fixedRecordData":
            if not self.inFixedRecord:
                return

            # name = HLAfixedRecord("name", [("X",HLAinteger32LE), ("Y",HLAinteger32LE)])
            globals()[self.recordName] = HLAfixedRecord(self.recordName, self.recordFields)
            self.inFixedRecord = False

        elif name == "variantRecordData":
            if not self.inVariantRecord:
                return

            # name = HLAvariantRecord("name", ("Axis",AxisEnum),
            #     [(["TX"],"X",HLAinteger32LE), (["TY"],"Y",HLAinteger32LE)])
            globals()[self.recordName] = HLAencoding(self.recordEncoding,
                (self.recordName, self.recordDiscriminant, self.recordFields))
            self.inVariantRecord = False

class HLAencoding:
    def __init__(self, representation, typeParameters = None):
        self.representation = representation
        self.typeParameters = typeParameters
        self._encoding = None

    @property
    def encoding(self):
        if(self._encoding == None):
            if(self.typeParameters == None):
                self._encoding = globals()[self.representation]
            else:
                self._encoding = globals()[self.representation](self.typeParameters)
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

# $Id: __init__.py,v 1.2 2008/10/13 17:15:39 gotthardp Exp $
