#!/usr/bin/env python

#here is an example to run this script :
#export PYTHONPATH=$HOME/DTest/dtest:$PYTHONPATH;
#export CERTI_HOME=$HOME/Certi;
# ~/dtest_HLA_Tutorial.py  --rtig=<user>@<host>:rtig --process=<user>@<host>:processFederate --controller=<user>@<host>:controllerFederate

import logging
import os
import time
import threading
import getopt, sys
import dtest

def usage():
    print "Usage : "
    print "%s [--help] [--certi_home=<path>]" % sys.argv[0]
    print "\t --rtig=<user>@[<host>]:<rtig_path>"
    print "\t --controller=<user>@[<host>]:<controllerFederate_path>"
    print "\t --process=<user>@[<host>]:<processFederate_path>"
    
def getUserHostPath(argument):
    if argument.find("@") != -1:
        (user,argument) = argument.split("@",1)
    else:
        user = os.environ["USER"]
    if argument.find(":") != -1:
        (host,path) = argument.split(":",1)
    else:
        host = "localhost"
        path = argument
    retval = dict()
    retval['user'] = user
    retval['host'] = host
    retval['path'] = path
    return retval

#------------------------------------------------------------- Getting parameters ----------------------------------    
try:
    opts, args = getopt.getopt(sys.argv[1:],"hc:r:p:C",["help","certi_home=","rtig=", "process=","controller="])
except getopt.GetoptError, err:
    print >> sys.stderr, "opt = %s, msg = %s" % (err.opt,err.msg)
    usage()
    sys.exit(2)
    
certi_home_defined=False
rtig_param   = getUserHostPath("rtig")
controller_param = getUserHostPath("controllerFederate")
process_param = getUserHostPath("processFederate")
    
for o, a in opts:
    if o in ("--help"):
        usage()
        sys.exit(2)
    if o in ("--rtig"):
        rtig_param   = getUserHostPath(a)
    if o in ("--controller"):
        controller_param = getUserHostPath(a)
    if o in ("--process"):
        process_param = getUserHostPath(a)
    if o in ("--certi_home"):
        certi_home = a
        certi_home_defined=True
        print "CERTI_HOME defined"
    
if not certi_home_defined:
    if os.environ.has_key("CERTI_HOME"):
        certi_home=os.environ["CERTI_HOME"]
    else: 
        print "You must define CERTI_HOME environment variable"
        sys.exit(2)                 
#---------------------------------------------------------- Describe rtig run steps --------------------------------
rtig = dtest.DTester("rtig",session=dtest.SSHSessionHandler(rtig_param['user'],host=rtig_param['host']))
# you may change the default time out value
rtig.timeout = 10
# you add want to save the output of your dtester to a file.
rtig.stdout    = file(rtig.name + ".out",'w+')
rtig.stdin     = file(rtig.name + ".in",'w+')
rtig.stderr    = file(rtig.name + ".err",'w+')
# describe RTIG run steps
rtig.addRunStep("ok",True,"HLA Tutorial functional test script version 0.3")
rtig.addRunStep("ok",True,"CERTI RTIG Starts")
#we source myCERTI_env.sh to set environment variables - the first parameter sets the rtig host
rtig.addRunStep("runCommand",command=". "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
rtig.addRunStep("runCommand",command=rtig_param['path'])
rtig.addRunStep("expectFromCommand",pattern="CERTI RTIG up and running",timeout=5)
rtig.addRunStep("barrier","RTIG started")
rtig.addRunStep("barrier","End of simulation",timeout=150)
rtig.addRunStep("terminateCommand")
rtig.addRunStep("waitCommandTermination")
rtig.addRunStep("ok",True,"RTIG ends")
#---------------------------------------------------------- Describe processFederate run steps ---------------------
processFederate = dtest.DTester("processFederate",session=dtest.SSHSessionHandler(process_param['user'],host=process_param['host']))
processFederate.timeout = 10
processFederate.stdout    = file(processFederate.name + ".out",'w+')
processFederate.stdin     = file(processFederate.name + ".in",'w+')
processFederate.stderr    = file(processFederate.name + ".err",'w+')
processFederate.addRunStep("barrier","RTIG started")
processFederate.addRunStep("runCommand",command=". "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
processFederate.addRunStep("runCommand",command="export DISPLAY=grasse:0.0")
processFederate.addRunStep("runCommand",command=process_param['path'])
processFederate.addRunStep("ok",True,"processFederate starts execution")
processFederate.addRunStep("barrier","waitForSecond",timeout=30)
processFederate.addRunStep("barrier","Simulation starts")
processFederate.addRunStep("ok",True,"Please be patient, the simulation can last 30 seconds")
processFederate.addRunStep("expectFromCommand",pattern="Successful termination",timeout=150)
processFederate.addRunStep("barrier","Successful termination")
processFederate.addRunStep("expectFromCommand",pattern="Federate destroyed")
processFederate.addRunStep("ok",True,"Federation destroyed")
processFederate.addRunStep("barrier","End of simulation")
processFederate.addRunStep("terminateCommand")
processFederate.addRunStep("waitCommandTermination")
#---------------------------------------------------------- Describe controllerFederate run steps ------------------
controllerFederate = dtest.DTester("controllerFederate",session=dtest.SSHSessionHandler(controller_param['user'],host=controller_param['host']))
controllerFederate.timeout = 10
controllerFederate.stdout    = file(controllerFederate.name + ".out",'w+')
controllerFederate.stdin     = file(controllerFederate.name + ".in",'w+')
controllerFederate.stderr    = file(controllerFederate.name + ".err",'w+')
controllerFederate.addRunStep("barrier","RTIG started")
controllerFederate.addRunStep("runCommand",command=". "+certi_home+"/share/scripts/myCERTI_env.sh "+rtig_param['host'])
controllerFederate.addRunStep("runCommand",command="export DISPLAY=grasse:0.0")
controllerFederate.addRunStep("runCommand",command=controller_param['path'])
controllerFederate.addRunStep("ok",True,"controllerFederate starts execution")

controllerFederate.addRunStep("expectFromCommand",pattern="HLA Tutorial")
controllerFederate.addRunStep("ok",True,"HLA Tutorial reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Federation Management")
controllerFederate.addRunStep("ok",True,"Federation Management reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Federation Management -> Create Federation Execution")
controllerFederate.addRunStep("ok",True,"Federation Management -> Create Federation Execution reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Federation Management -> Join Federation Execution")
controllerFederate.addRunStep("ok",True,"Federation Management -> Join Federation Execution reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Declaration Management")
controllerFederate.addRunStep("ok",True,"Declaration Management reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Ancillary Service -> Get Object Class Handle")
controllerFederate.addRunStep("ok",True,"Ancillary Service -> Get Object Class Handle reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Ancillary Service -> Get Attribute Handle")
controllerFederate.addRunStep("ok",True,"Ancillary Service -> Get Attribute Handle reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Ancillary Service -> Add attribute handles to attribute handle set")
controllerFederate.addRunStep("ok",True,"Ancillary Service -> Add attribute handle to attribute handles set reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Declaration Management -> Publish Object Class")
controllerFederate.addRunStep("ok",True,"Declaration Management -> Publish Object Class reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Declaration Management -> Subscribe To Object Class")
controllerFederate.addRunStep("ok",True,"Declaration Management -> Subscribe To Object Class reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Object Management")
controllerFederate.addRunStep("ok",True,"Object Management reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Object Management -> Register Object Instance")
controllerFederate.addRunStep("ok",True,"Object Management -> Register Object Instance reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Ancillary Service -> Tick")
controllerFederate.addRunStep("ok",True,"Ancillary Service -> Tick reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Object Management -> Discover Object Instance")
controllerFederate.addRunStep("ok",True,"Object Management -> Discover Object Instance reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Wait for second federate.")
controllerFederate.addRunStep("ok",True,"Wait for second federate.controller reached")

controllerFederate.addRunStep("barrier","waitForSecond")

controllerFederate.addRunStep("expectFromCommand",pattern="Object Management -> Update Attribute Values")
controllerFederate.addRunStep("ok",True,"Object Management -> Update Attributes Values reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Object Management -> Reflect Attribute Values")
controllerFederate.addRunStep("ok",True,"Object Management -> Reflect Attributes Values reached")

controllerFederate.addRunStep("barrier","Simulation starts")
controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("barrier","Successful termination",timeout=150)

controllerFederate.addRunStep("expectFromCommand",pattern="Federation Management -> Resign Federation Execution")
controllerFederate.addRunStep("ok",True,"Federation Management -> Resign Federation Execution reached")

controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("expectFromCommand",pattern="Federation Management -> Destroy Federation Execution")
controllerFederate.addRunStep("ok",True,"Federation Management -> Destroy Federation Execution reached")


controllerFederate.addRunStep("runCommand",command="")
controllerFederate.addRunStep("terminateCommand")
controllerFederate.addRunStep("waitCommandTermination")
controllerFederate.addRunStep("barrier","End of simulation")

#---------------------------------------------------------- Here begins the test -----------------------------------
dtest.DTestMaster.logger.setLevel(level=logging.WARNING)
dtest.DTester.logger.setLevel(level=logging.WARNING)
dtest.SSHSessionHandler.logger.setLevel(level=logging.WARNING)

def goTest():
    myDTestMaster = dtest.DTestMaster("CERTI HLA Tutorial autotest",description="1 RTIG + 1 process + 1 controller test")
    myDTestMaster.timeout = 200
    myDTestMaster.register(rtig)
    myDTestMaster.register(controllerFederate)
    myDTestMaster.register(processFederate)
    myDTestMaster.startTestSequence()
    myDTestMaster.waitTestSequenceEnd()
    
goTest()
