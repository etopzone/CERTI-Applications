#!/usr/bin/env python
# Requires PyHLA, see http://www.nongnu.org/certi/PyHLA
import hla.rti
import hla.omt as fom

import struct

class MyAmbassador(hla.rti.FederateAmbassador):
    def initialize(self):
        self.classHandle = rtia.getObjectClassHandle("SampleClass")

        self.textAttributeHandle = rtia.getAttributeHandle("TextAttribute", self.classHandle)
        self.structAttributeHandle = rtia.getAttributeHandle("StructAttribute", self.classHandle)
        self.fomAttributeHandle = rtia.getAttributeHandle("FOMAttribute", self.classHandle)

        rtia.subscribeObjectClassAttributes(self.classHandle,
            [self.textAttributeHandle, self.structAttributeHandle, self.fomAttributeHandle])

    # RTI callbacks
    def discoverObjectInstance(self, object, objectclass, name):
        print "DISCOVER", name
        rtia.requestObjectAttributeValueUpdate(object,
            [self.textAttributeHandle, self.structAttributeHandle, self.fomAttributeHandle])

    def reflectAttributeValues(self, object, attributes, tag, order, transport, time=None, retraction=None):
        if self.textAttributeHandle in attributes:
            print "REFLECT", attributes[self.textAttributeHandle]

        if self.structAttributeHandle in attributes:
            structValue = struct.unpack('hhl', attributes[self.structAttributeHandle])
            print "REFLECT", structValue

        if self.fomAttributeHandle in attributes:
            fomValue, size = fom.HLAfloat32BE.unpack(attributes[self.fomAttributeHandle])
            print "REFLECT", fomValue

print "Create ambassador"
rtia = hla.rti.RTIAmbassador()

try:
    rtia.createFederationExecution("uav", "uav.fed")
    print "Federation created."
except hla.rti.FederationExecutionAlreadyExists:
    print "Federation already exists."

mya = MyAmbassador()
rtia.joinFederationExecution("uav-receive", "uav", mya)

mya.initialize()

try:
    while(1):
        rtia.tick(1.0, 1.0)
except KeyboardInterrupt:
    pass

rtia.resignFederationExecution(hla.rti.ResignAction.DeleteObjectsAndReleaseAttributes)

print "Done."
