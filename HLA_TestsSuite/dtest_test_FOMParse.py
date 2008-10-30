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
federate_param = dtest.Utils.getUserHostPath("test_FOMParse")
federate_param['fom']="TestFed.fed"
    
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

firstFederate = dtest.DTester("test_FOMParse-TestFed",
                               session=dtest.SSHSessionHandler(federate_param['user'],host=federate_param['host']))

FOMFed2 = dtest.DTester("test_FOMParse-RPR-FOM-flat",
                        session=dtest.SSHSessionHandler(federate_param['user'],host=federate_param['host']))

FOMFed3 = dtest.DTester("test_FOMParse-RPR-FOM-hierarchical",
                        session=dtest.SSHSessionHandler(federate_param['user'],host=federate_param['host']))

FOMFed4 = dtest.DTester("test_FOMParse-aviationsimnetV3.1.xml-flat",
                        session=dtest.SSHSessionHandler(federate_param['user'],host=federate_param['host']))

FOMFed5 = dtest.DTester("test_FOMParse-aviationsimnetV3.1.xml-hierarchical",
                        session=dtest.SSHSessionHandler(federate_param['user'],host=federate_param['host']))

FOMFed6 = dtest.DTester("test_FOMParse-BuggyFed",
                        session=dtest.SSHSessionHandler(federate_param['user'],host=federate_param['host']))


# you may change the default time out value
rtig.timeout = 40
# you add want to save the output of your dtester to a file.
rtig.stdout    = file(rtig.name + ".out",'w+')
rtig.stdin     = file(rtig.name + ".in",'w+')
rtig.stderr    = file(rtig.name + ".err",'w+')

# describe RTIG run steps
rtig.addRunStep("ok",True,"HLA test test_FOMParse Starts.")
dtest.ReusableSequences.addConditionalRunShellScript(rtig,c_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.csh "+rtig_param['host'],
                               bourne_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
rtig.addRunStep("runCommand",command=rtig_param['path']+" -v 1")
rtig.addRunStep("expectFromCommand",pattern="CERTI RTIG up and running",timeout=5)
rtig.addRunStep("barrier","RTIG started")
#rtig.addRunStep("expectFromCommand",pattern="200.",timeout=20,silent=True)
#rtig.addRunStep("expectFromCommand",pattern="200.",timeout=20,silent=True)
rtig.addRunStep("barrier","All Federate(s) ended")
rtig.addRunStep("terminateCommand")
rtig.addRunStep("waitCommandTermination")
rtig.addRunStep("ok",True,"HLA test test_FOMParse Ends.")

#dtest.DTester.logger.setLevel(level=logging.DEBUG)

nbfederate = 1

# Describe a generic successful FOM sequence federate run steps
def addFOMFedSequence(shouldBeSuccessfull,federate, specificLaunchParameters,barrierStart,barrierStop):
    federate.timeout = globals()['nbfederate']*20
    globals()['nbfederate']  += 1
    federate.stdout  = file(federate.name + ".out",'w+')
    federate.stdin   = file(federate.name + ".in",'w+')
    federate.stderr  = file(federate.name + ".err",'w+')
    #federate.addRunStep("barrier","RTIG started")
    federate.addRunStep("barrier",barrierStart)
    dtest.ReusableSequences.addConditionalRunShellScript(federate,c_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.csh "+rtig_param['host'],
                                                         bourne_shell_cmd="source "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
   
    federate.addRunStep("runCommand",command=federate_param['path']+specificLaunchParameters)
    if shouldBeSuccessfull:
        federate.addRunStep("expectFromCommand",pattern="GLOBAL SUCCESS.*")
    else:
        federate.addRunStep("expectFromCommand",pattern="GLOBAL ERROR.*")
        
    federate.addRunStep("ok",federate.getFutureLastStepStatus,federate.name)    
    federate.addRunStep("terminateCommand")
    federate.addRunStep("barrier",barrierStop)
    #federate.addRunStep("barrier","All Federate(s) ended",timeout=60)

addFOMFedSequence(True,firstFederate," -v -f TestFed.fed -n TestFed -j FOMParse1" , "RTIG started", "FOM1")
addFOMFedSequence(True,FOMFed2, " -v -f RPR-FOM.fed -n FederationName -j FOMFed2 -o PhysicalEntity -a DamageState","FOM1","FOM2")
addFOMFedSequence(True,FOMFed3, " -v -f RPR-FOM.fed -n FederationName -j FOMFed2 -o BaseEntity.PhysicalEntity -a DamageState","FOM2","FOM3")
addFOMFedSequence(True,FOMFed4, " -v -f aviationsimnetV3.1.xml -n AviationSimNet -j FOMFed4 -o Federate -a FederateType","FOM3","FOM4")
addFOMFedSequence(True,FOMFed5, " -v -f aviationsimnetV3.1.xml -n AviationSimNet -j FOMFed4 -o Manager.Federate -a FederateType","FOM4","FOM5")
addFOMFedSequence(False,FOMFed6, " -v -f BuggyFed -n BuggyFed","FOM5","All Federate(s) ended")

def goTest():
    myDTestMaster = dtest.DTestMaster("HLA test test_FOMParse Starts","Launch RTIG + several test_FOMParse federates for testing several FOM file parsing.")
    myDTestMaster.timeout = globals()['nbfederate']*20+30
    rtig.timeout = globals()['nbfederate']*20
    myDTestMaster.register(rtig)
    myDTestMaster.register(firstFederate)
    myDTestMaster.register(FOMFed2)
    myDTestMaster.register(FOMFed3)
    myDTestMaster.register(FOMFed4)
    myDTestMaster.register(FOMFed5)    
    myDTestMaster.register(FOMFed6)
    myDTestMaster.startTestSequence()
    myDTestMaster.waitTestSequenceEnd()
    
goTest()

