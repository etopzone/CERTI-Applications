#!/usr/bin/env python
import hla.omt as fom

fom.HLAuse('rpr2-d18.xml')

def dump(string):
    for c in string:
        print '%02x' % ord(c),
    print

Enum = fom.HLAenumerated("Enum", fom.HLAinteger32BE,
    {"Little":0, "Big":1, "Yellow":64})

print Enum.Little

Rec = fom.HLAvariantRecord("Rec",("T", Enum),
    [([Enum.Little], "A", fom.HLAinteger16LE),
    ([Enum.Big, None], "B", fom.HLAinteger16BE)])

haf = Rec.pack({"T":Enum.Yellow, "B":42})
dump(haf)

val, size = Rec.unpack(haf)
print val

cc = fom.HLAASCIIstring.pack("UFO");
dump(cc)

cc2, size = fom.HLAASCIIstring.unpack(cc)
print cc2
