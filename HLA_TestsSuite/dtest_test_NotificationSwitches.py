#!/usr/bin/env python

##-----------------------------------------------------------------------
##
## HLA Tests Suite
##
## Copyright (c) 2006-2008 Eric NOULARD, Jean-Yves ROUSSELOT 
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
##
##-----------------------------------------------------------------------

import logging
import os
import time
import threading
import getopt, sys
import dtest  

def usage():
    print "Usage:\n %s [--help] [--certi_home=<path>] --rtig=[[<user>@]<host>]:<rtig_path> --federate=[[<user>@]<host>]:<federate_path>" % sys.argv[0]

try:
    opts, args = getopt.getopt(sys.argv[1:], "hr:f:c:", ["help","rtig=", "federate=","certi_home="])
except getopt.GetoptError, err:
    print >> sys.stderr, "opt = %s, msg = %s" % (err.opt,err.msg)
    usage()
    sys.exit(2)

## default values
certi_home_defined=False
rtig_param = dtest.Utils.getUserHostPath("rtig")
federate_param = dtest.Utils.getUserHostPath("test_NotificationSwitches")
federate_param['fom']="test_NotificationSwitches.fed"
    
for o, a in opts:
    if o in ("--help"):
            usage()
            sys.exit(2)
    if o in ("-r", "--rtig"):
        rtig_param   = dtest.Utils.getUserHostPath(a)
    if o in ("-f", "--federate"):
        federate_param = dtest.Utils.getUserHostPath(a)
    if o in ("-c", "--certi_home"):
        certi_home = a
        certi_home_defined=True
        
if not certi_home_defined:
    if os.environ.has_key("CERTI_HOME"):
        certi_home=os.environ["CERTI_HOME"]
    else: 
        print "You must define CERTI_HOME environment variable"
        sys.exit(2)

rtig = dtest.DTester("RTIG",
                     session=dtest.SSHSessionHandler(rtig_param['user'],host=rtig_param['host']))

firstFederate = dtest.DTester("test_NotificationSwitches_First",
                               session=dtest.SSHSessionHandler(federate_param['user'],host=federate_param['host']))

otherFederate = dtest.DTester("test_NotificationSwitches_Other1",
                               session=dtest.SSHSessionHandler(federate_param['user'],host=federate_param['host']))

# you may change the default time out value
rtig.timeout = 40
# you add want to save the output of your dtester to a file.
rtig.stdout    = file(rtig.name + ".out",'w+')
rtig.stdin     = file(rtig.name + ".in",'w+')
rtig.stderr    = file(rtig.name + ".err",'w+')

# describe RTIG run steps
rtig.addRunStep("ok",True,"HLA test test_NotificationSwitches Starts.")
dtest.ReusableSequences.addConditionalRunShellScript(rtig,c_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.csh "+rtig_param['host'],
                               bourne_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
rtig.addRunStep("runCommand",command=rtig_param['path'])
rtig.addRunStep("expectFromCommand",pattern="CERTI RTIG up and running",timeout=5)
rtig.addRunStep("barrier","RTIG started")
rtig.addRunStep("barrier","All Federate(s) ended")
rtig.addRunStep("terminateCommand")
rtig.addRunStep("waitCommandTermination")
rtig.addRunStep("ok",True,"HLA test test_NotificationSwitches Ends.")

#dtest.DTester.logger.setLevel(level=logging.DEBUG)

# describe first federate run steps
firstFederate.timeout = 20
firstFederate.stdout  = file(firstFederate.name + ".out",'w+')
firstFederate.stdin   = file(firstFederate.name + ".in",'w+')
firstFederate.stderr  = file(firstFederate.name + ".err",'w+')
firstFederate.addRunStep("barrier","RTIG started")
dtest.ReusableSequences.addConditionalRunShellScript(firstFederate,c_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.csh "+rtig_param['host'],
                               bourne_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
firstFederate.addRunStep("runCommand",command=federate_param['path']+" 1")
firstFederate.addRunStep("expectFromCommand",pattern="Joined federation.*")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"First Federate started and has joined federation")
firstFederate.addRunStep("expectFromCommand",pattern="Press ENTER to start execution")
firstFederate.addRunStep("barrier","First Federate waiting other(s) before going on")
firstFederate.addRunStep("barrier","Other Federate started")
firstFederate.addRunStep("sendToCommand",string="\n")

firstFederate.addRunStep("barrier","Test Notification Switches starts...")

firstFederate.addRunStep("expectFromCommand",pattern="enableClassRelevanceAdvisorySwitch")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"set ClassRelevanceAdvisorySwitch - "+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="\n")
firstFederate.addRunStep("expectFromCommand",pattern="disableClassRelevanceAdvisorySwitch")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"clear ClassRelevanceAdvisorySwitch - "+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="\n")
firstFederate.addRunStep("expectFromCommand",pattern="enableInteractionRelevanceAdvisorySwitch")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"set InteractionClassRelevanceAdvisorySwitch - "+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="\n")
firstFederate.addRunStep("expectFromCommand",pattern="disableInteractionRelevanceAdvisorySwitch")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"clear InteractionClassRelevanceAdvisorySwitch - "+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="\n")
firstFederate.addRunStep("expectFromCommand",pattern="enableAttributeRelevanceAdvisorySwitch")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"set AttributeRelevanceAdvisorySwitch - "+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="\n")
firstFederate.addRunStep("expectFromCommand",pattern="disableAttributeRelevanceAdvisorySwitch")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"clear AttributeRelevanceAdvisorySwitch - "+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="\n")
firstFederate.addRunStep("expectFromCommand",pattern="enableAttributeScopeAdvisorySwitch")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"set AttributeScopeAdvisorySwitch - "+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="\n")
firstFederate.addRunStep("expectFromCommand",pattern="disableAttributeScopeAdvisorySwitch")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"clear AttributeScopeAdvisorySwitch - "+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="\n")

firstFederate.addRunStep("barrier","Test Notfication Switches ends.")

firstFederate.addRunStep("barrier","Test Notification Switches Callbacks starts...")

# test startRegistrationForObjectClass
firstFederate.addRunStep("expectFromCommand",pattern="enableClassRelevanceAdvisorySwitch")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"re-enable ClassRelevanceAdvisorySwitch - "+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="\n")

# after subscription within other federate we should get startRegForObjClass callback
firstFederate.addRunStep("expectFromCommand",pattern="startRegForObjClass")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"startRegistrationForObjectClass received - "+firstFederate.name)

# after re-subscription within other federate we should get second startRegForObjClass callback
firstFederate.addRunStep("expectFromCommand",pattern="startRegForObjClass")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"startRegistrationForObjectClass received - "+firstFederate.name)

# test stopRegistrationForObjectClass
# ... to be continued

firstFederate.addRunStep("barrier","Test Notification Switches Callbacks ends...")

firstFederate.addRunStep("expectFromCommand",pattern="fedSync")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"deinitialize federation - "+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="\n")

firstFederate.addRunStep("terminateCommand")
firstFederate.addRunStep("barrier","All Federate(s) ended")

# other federate
otherFederate.timeout = 20
otherFederate.stdout  = file(otherFederate.name + ".out",'w+')
otherFederate.stdin   = file(otherFederate.name + ".in",'w+')
otherFederate.stderr  = file(otherFederate.name + ".err",'w+')
otherFederate.addRunStep("barrier","RTIG started")
otherFederate.addRunStep("barrier","First Federate waiting other(s) before going on")
dtest.ReusableSequences.addConditionalRunShellScript(otherFederate,c_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.csh "+rtig_param['host'],
                               bourne_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
otherFederate.addRunStep("runCommand",command=federate_param['path']+" 1")
otherFederate.addRunStep("expectFromCommand",pattern="Joined federation.*")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"Other Federate started and has joined federation")
otherFederate.addRunStep("barrier","Other Federate started")

# first sync point
otherFederate.addRunStep("expectFromCommand",pattern="Synchronization Point sync reached")
otherFederate.addRunStep("sendToCommand",string="\n")

otherFederate.addRunStep("barrier","Test Notification Switches starts...")

otherFederate.addRunStep("expectFromCommand",pattern="enableClassRelevanceAdvisorySwitch")
otherFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"set ClassRelevanceAdvisorySwitch - "+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="\n")
otherFederate.addRunStep("expectFromCommand",pattern="disableClassRelevanceAdvisorySwitch")
otherFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"clear ClassRelevanceAdvisorySwitch - "+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="\n")
otherFederate.addRunStep("expectFromCommand",pattern="enableInteractionRelevanceAdvisorySwitch")
otherFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"set InteractionClassRelevanceAdvisorySwitch - "+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="\n")
otherFederate.addRunStep("expectFromCommand",pattern="disableInteractionRelevanceAdvisorySwitch")
otherFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"clear InteractionClassRelevanceAdvisorySwitch - "+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="\n")
otherFederate.addRunStep("expectFromCommand",pattern="enableAttributeRelevanceAdvisorySwitch")
otherFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"set AttributeRelevanceAdvisorySwitch - "+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="\n")
otherFederate.addRunStep("expectFromCommand",pattern="disableAttributeRelevanceAdvisorySwitch")
otherFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"clear AttributeRelevanceAdvisorySwitch - "+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="\n")
otherFederate.addRunStep("expectFromCommand",pattern="enableAttributeScopeAdvisorySwitch")
otherFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"set AttributeScopeAdvisorySwitch - "+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="\n")
otherFederate.addRunStep("expectFromCommand",pattern="disableAttributeScopeAdvisorySwitch")
otherFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"clear AttributeScopeAdvisorySwitch - "+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="\n")

otherFederate.addRunStep("barrier","Test Notification Switches ends.")

otherFederate.addRunStep("barrier","Test Notification Switches Callbacks starts...")

otherFederate.addRunStep("expectFromCommand",pattern="Synchronization Point sync reached")
otherFederate.addRunStep("sendToCommand",string="\n")

otherFederate.addRunStep("expectFromCommand",pattern="Synchronization Point sync reached")
otherFederate.addRunStep("sendToCommand",string="\n")

# subscription and discovering
otherFederate.addRunStep("expectFromCommand",pattern="subscribe object class")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"subscribe object class - "+otherFederate.name )
otherFederate.addRunStep("expectFromCommand",pattern="Discovering object")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"discover object instance - "+otherFederate.name )

otherFederate.addRunStep("expectFromCommand",pattern="Synchronization Point sync reached")
otherFederate.addRunStep("sendToCommand",string="\n")

otherFederate.addRunStep("expectFromCommand",pattern="unsubscribe object class")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"unsubscribe object class - "+otherFederate.name )

otherFederate.addRunStep("expectFromCommand",pattern="Synchronization Point sync reached")
otherFederate.addRunStep("sendToCommand",string="\n")

# re-subscription, re-discovering
otherFederate.addRunStep("expectFromCommand",pattern="resubscribe object class")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"resubscribe object class - "+otherFederate.name )
otherFederate.addRunStep("expectFromCommand",pattern="Discovering object")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"discover object instance - "+otherFederate.name )

otherFederate.addRunStep("expectFromCommand",pattern="Synchronization Point sync reached")
otherFederate.addRunStep("sendToCommand",string="\n")

otherFederate.addRunStep("barrier","Test Notification Switches Callbacks ends...")

# test stopRegistrationForObjectClass
# ... to be continued

otherFederate.addRunStep("expectFromCommand",pattern="fedSync")
otherFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"deinitialize federation - "+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="\n")

otherFederate.addRunStep("terminateCommand")
otherFederate.addRunStep("barrier","All Federate(s) ended")

def goTest():
    myDTestMaster = dtest.DTestMaster("HLA test test_NotificationSwitches Starts","Launch RTIG + two test_NotficationSwitches federate for testing notification switches and callbacks,...")
    myDTestMaster.timeout = 40
    myDTestMaster.register(rtig)
    myDTestMaster.register(firstFederate)
    myDTestMaster.register(otherFederate)
    myDTestMaster.startTestSequence()
    myDTestMaster.waitTestSequenceEnd()
    
goTest()

