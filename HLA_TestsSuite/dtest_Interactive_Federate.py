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
federate_param = dtest.Utils.getUserHostPath("Interactive_Federate")
federate_param['fom']="Interactive_Federation.fed"
    
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

firstFederate = dtest.DTester("Interactive_First",
                               session=dtest.SSHSessionHandler(federate_param['user'],host=federate_param['host']))

otherFederate = dtest.DTester("Interactive_Second",
                               session=dtest.SSHSessionHandler(federate_param['user'],host=federate_param['host']))

# you may change the default time out value
rtig.timeout = 40
# you add want to save the output of your dtester to a file.
rtig.stdout    = file(rtig.name + ".out",'w+')
rtig.stdin     = file(rtig.name + ".in",'w+')
rtig.stderr    = file(rtig.name + ".err",'w+')

# describe RTIG run steps
rtig.addRunStep("ok",True,"HLA test Zero Lookahead (InteractiveFederate) Starts.")
dtest.ReusableSequences.addConditionalRunShellScript(rtig,c_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.csh "+rtig_param['host'],
                               bourne_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
rtig.addRunStep("runCommand",command=rtig_param['path'])
rtig.addRunStep("expectFromCommand",pattern="CERTI RTIG up and running",timeout=5)
rtig.addRunStep("barrier","RTIG started")
rtig.addRunStep("barrier","All Federate(s) ended")
rtig.addRunStep("terminateCommand")
rtig.addRunStep("waitCommandTermination")
rtig.addRunStep("ok",True,"HLA test Zero Lookahead (InteractiveFederate) Ends.")

#dtest.DTester.logger.setLevel(level=logging.DEBUG)

# describe first federate run steps
firstFederate.timeout = 20
firstFederate.stdout  = file(firstFederate.name + ".out",'w+')
firstFederate.stdin   = file(firstFederate.name + ".in",'w+')
firstFederate.stderr  = file(firstFederate.name + ".err",'w+')
firstFederate.addRunStep("barrier","RTIG started")
dtest.ReusableSequences.addConditionalRunShellScript(firstFederate,c_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.csh "+rtig_param['host'],
                               bourne_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
firstFederate.addRunStep("runCommand",command=federate_param['path']+" "+firstFederate.name)
firstFederate.addRunStep("expectFromCommand",pattern="Voulez-vous un preambule automatique.*")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"First Federate started and has joined federation")
firstFederate.addRunStep("sendToCommand",string="y\n")
firstFederate.addRunStep("barrier","Wait others...")

firstFederate.addRunStep("expectFromCommand",pattern="Choisissez une action :")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"ACTION"+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="ml\n")
firstFederate.addRunStep("sendToCommand",string="0\n")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,firstFederate.name+" has zero lookahead")

firstFederate.addRunStep("expectFromCommand",pattern="Choisissez une action :")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"ACTION"+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="tara\n")
firstFederate.addRunStep("sendToCommand",string="1\n")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,firstFederate.name+" has requested advance request of 1 as available")
firstFederate.addRunStep("barrier","After TAR and TARA")

firstFederate.addRunStep("sendToCommand",string="qlbts\n")
firstFederate.addRunStep("expectFromCommand",pattern="LBTS=1.0001")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,firstFederate.name+" has LBTS > 1")
firstFederate.addRunStep("sendToCommand",string="t\n")
firstFederate.addRunStep("expectFromCommand",pattern="timeAdvanceGrant")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,firstFederate.name+" time advance granted to 1")
firstFederate.addRunStep("barrier","Wait before an interaction exchange")

firstFederate.addRunStep("sendToCommand",string="si\n")
firstFederate.addRunStep("sendToCommand",string="1\n")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,firstFederate.name+" can send an interaction with timestamp 1")
firstFederate.addRunStep("barrier","Wait after an interaction exchange")

firstFederate.addRunStep("sendToCommand",string="tar\n")
firstFederate.addRunStep("sendToCommand",string="2\n")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,firstFederate.name+" has requested advance request of 2")

firstFederate.addRunStep("expectFromCommand",pattern="Choisissez une action :")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,"ACTION"+firstFederate.name)
firstFederate.addRunStep("sendToCommand",string="rfe\n")
firstFederate.addRunStep("expectFromCommand",pattern="federation quittee")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,firstFederate.name+" has quit federation")

firstFederate.addRunStep("barrier","Wait before DFE")

firstFederate.addRunStep("expectFromCommand",pattern="Choisissez une action :")
firstFederate.addRunStep("sendToCommand",string="dfe\n")
firstFederate.addRunStep("expectFromCommand",pattern="federation detruite")
firstFederate.addRunStep("ok",firstFederate.getFutureLastStepStatus,firstFederate.name+" has quit federation")

firstFederate.addRunStep("sendToCommand",string="q\n")

firstFederate.addRunStep("terminateCommand")
firstFederate.addRunStep("barrier","All Federate(s) ended")

# other federate
otherFederate.timeout = 20
otherFederate.stdout  = file(otherFederate.name + ".out",'w+')
otherFederate.stdin   = file(otherFederate.name + ".in",'w+')
otherFederate.stderr  = file(otherFederate.name + ".err",'w+')
otherFederate.addRunStep("barrier","RTIG started")

dtest.ReusableSequences.addConditionalRunShellScript(otherFederate,c_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.csh "+rtig_param['host'],
                               bourne_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
otherFederate.addRunStep("runCommand",command=federate_param['path']+" "+otherFederate.name)
otherFederate.addRunStep("expectFromCommand",pattern="Voulez-vous un preambule automatique.*")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"Second Federate started and has joined federation")
otherFederate.addRunStep("sendToCommand",string="y\n")
otherFederate.addRunStep("barrier","Wait others...")

otherFederate.addRunStep("expectFromCommand",pattern="Choisissez une action :")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"ACTION"+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="ml\n")
otherFederate.addRunStep("sendToCommand",string="0\n")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,otherFederate.name+" has zero lookahead")

otherFederate.addRunStep("expectFromCommand",pattern="Choisissez une action :")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"ACTION"+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="tar\n")
otherFederate.addRunStep("sendToCommand",string="1\n")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,otherFederate.name+" has requested advance request of 1")
otherFederate.addRunStep("barrier","After TAR and TARA")

otherFederate.addRunStep("sendToCommand",string="ql\n")
otherFederate.addRunStep("expectFromCommand",pattern="Lookahead=0.0001")
#otherFederate.addRunStep("expectFromCommand",pattern="Lookahead=0.00\r")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,otherFederate.name+" has Lookahead > 0")
otherFederate.addRunStep("barrier","Wait before an interaction exchange")

otherFederate.addRunStep("expectFromCommand",pattern="Choisissez une action :")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"ACTION"+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="t\n")
otherFederate.addRunStep("expectFromCommand",pattern="receiveInteraction")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,otherFederate.name+" has received an interaction")
otherFederate.addRunStep("barrier","Wait after an interaction exchange")

otherFederate.addRunStep("sendToCommand",string="t\n")
otherFederate.addRunStep("expectFromCommand",pattern="timeAdvanceGrant")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,otherFederate.name+" time advance granted")
otherFederate.addRunStep("sendToCommand",string="ql\n")
otherFederate.addRunStep("expectFromCommand",pattern="Lookahead=0\r")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,otherFederate.name+" lookahead back to 0")

otherFederate.addRunStep("expectFromCommand",pattern="Choisissez une action :")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,"ACTION"+otherFederate.name)
otherFederate.addRunStep("sendToCommand",string="rfe\n")
otherFederate.addRunStep("expectFromCommand",pattern="federation quittee")
otherFederate.addRunStep("ok",otherFederate.getFutureLastStepStatus,otherFederate.name+" has quit federation")

otherFederate.addRunStep("barrier","Wait before DFE")

otherFederate.addRunStep("sendToCommand",string="q\n")

otherFederate.addRunStep("terminateCommand")
otherFederate.addRunStep("barrier","All Federate(s) ended")

def goTest():
    myDTestMaster = dtest.DTestMaster("HLA Test Zero Lookahead","Launch RTIG + two interactive federates for testing zero lookahead")
    myDTestMaster.timeout = 40
    myDTestMaster.register(rtig)
    myDTestMaster.register(firstFederate)
    myDTestMaster.register(otherFederate)
    myDTestMaster.startTestSequence()
    myDTestMaster.waitTestSequenceEnd()
    
goTest()

