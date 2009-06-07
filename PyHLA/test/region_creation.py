#!/usr/bin/env python
import hla.rti as rti

class MyAmbassador(rti.FederateAmbassador):
    pass

rtia = rti.RTIAmbassador()

try:
    rtia.createFederationExecution("VirtualAir", "AviationSimNet-v3.1.fed")
except rti.FederationExecutionAlreadyExists:
    pass

mya = MyAmbassador()
rtia.joinFederationExecution("python-01", "VirtualAir", mya)

spaceHandle = rtia.getRoutingSpaceHandle("Geo")
dimensionHandle = rtia.getDimensionHandle("X", spaceHandle)

region = rtia.createRegion(spaceHandle,[(dimensionHandle,(1,5))])

rtia.notifyAboutRegionModification(region)

rtia.deleteRegion(region)

# $Id: region_creation.py,v 1.1 2009/06/07 15:12:48 gotthardp Exp $
