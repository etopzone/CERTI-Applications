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

        rtia.publishObjectClass(self.classHandle,
            [self.textAttributeHandle, self.structAttributeHandle, self.fomAttributeHandle])
        self.myObject = rtia.registerObjectInstance(self.classHandle, "HAF")

    def terminate(self):
        rtia.deleteObjectInstance(self.myObject, "HAF")

    # RTI callbacks
    def startRegistrationForObjectClass(*params):
        print "START", params

    def provideAttributeValueUpdate(*params):
        print "PROVIDE UAV", params

print "Create ambassador"
rtia = hla.rti.RTIAmbassador()
print rtia

try:
    rtia.createFederationExecution("uav", "uav.fed")
    print "Federation created."
except hla.rti.FederationExecutionAlreadyExists:
    print "Federation already exists."

mya = MyAmbassador()
rtia.joinFederationExecution("uav-send", "uav", mya)

mya.initialize()

try:
    while(1):
        rtia.updateAttributeValues(mya.myObject,
            {mya.textAttributeHandle:"text",
            mya.structAttributeHandle:struct.pack('hhl', 1, 2, 3),
            mya.fomAttributeHandle:fom.HLAfloat32BE.pack(3.14)},
            "update")
        rtia.tick(1.0, 1.0)
except KeyboardInterrupt:
    pass

mya.terminate()

rtia.resignFederationExecution(hla.rti.ResignAction.DeleteObjectsAndReleaseAttributes)

print "Done."
