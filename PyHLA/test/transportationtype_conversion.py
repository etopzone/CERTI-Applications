#!/usr/bin/env python
import hla.rti as rti
import hla.omt as fom

def ASSERT(condition):
    if(not condition):
        raise Exception("unexpected result")

def RAISE_NAMENOTFOUND(function):
    try:
        function()
        raise Exception("unexpected result")
    except rti.NameNotFound:
        pass

rtia = rti.RTIAmbassador()

# --- transportationHandle
reliable = rtia.getTransportationHandle("HLAreliable")
reliable2 = rtia.getTransportationName(reliable)
print reliable, reliable2
ASSERT(reliable2 == "HLAreliable")

bestEffort = rtia.getTransportationHandle("HLAbestEffort")
print bestEffort
ASSERT(bestEffort == rtia.getTransportationHandle("HLAbestEffort"))
ASSERT(bestEffort != reliable)

RAISE_NAMENOTFOUND(lambda: rtia.getTransportationHandle("Receive")) # ordering
RAISE_NAMENOTFOUND(lambda: rtia.getTransportationHandle("UNKNOWN"))

# --- orderingHandle
receive = rtia.getOrderingHandle("Receive")
receive2 = rtia.getOrderingName(receive)
print receive, receive2
ASSERT(receive2 == "Receive")

timestamp = rtia.getOrderingHandle("Timestamp")
print timestamp
ASSERT(timestamp == rtia.getOrderingHandle("Timestamp"))
ASSERT(timestamp != receive)

RAISE_NAMENOTFOUND(lambda: rtia.getOrderingHandle("HLAreliable")) # transportation
RAISE_NAMENOTFOUND(lambda: rtia.getOrderingHandle("UNKNOWN"))

# $Id: transportationtype_conversion.py,v 1.2 2009/06/07 15:12:48 gotthardp Exp $
