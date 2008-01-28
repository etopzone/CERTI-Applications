% process federate

%    Copyright (C) 2008 Christian Stenzel, Sven Pawletta

%    This file is part of MatlabHLA13 demo applications.
%
%    This demo is free software: you can redistribute it and/or
%    modify it under the terms of the GNU General Public 
%    License as published by the Free Software Foundation, either 
%    version 3 of the License, or (at your option) any later version.
%
%    This demo is distributed in the hope that it will be useful,
%    but WITHOUT ANY WARRANTY; without even the implied warranty of
%    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%    GNU General Public License for more details.
%
%    You should have received a copy of the GNU General Public 
%    License along with this demo.  
%    
%    If not, see <http://www.gnu.org/licenses/>.

rtiOn;

federationName  = 'TwoLevelController';
federateName    = 'processFed';
fedFile         = 'TwoLevelController.fed'

disp('Create federation execution ....')

e = createFedExec(federationName, fedFile)

switch e
	case ''
		disp('created.');
	case 'FederationExecutionAlreadyExists'
		disp('already created.');
	otherwise
		error(['createFedExec failed: ' e])
end

disp('Join federation execution ...')

[federateId, e] = joinFedExec( federateName, federationName);

if isempty(e)
    disp('joined.')
else
    error(['joinFedExec failed: ' e])
end


% get handles

twoLevelContrID = getObjClassHdl('TwoLevelControllerClass')
uID = getAttrHdl('U', twoLevelContrID)
yID = getAttrHdl('Y', twoLevelContrID)

subscribeObjClassAttr( twoLevelContrID, uID)
publishObjClass(twoLevelContrID, yID)

objInstID_y = registerObjInst(twoLevelContrID, 'y')

tEnd = 30;
tStart = 0;
h = 0.1;
w = 0.5;
K = 1;
T1 = 5;

tSim = tStart;
y = 0;
ydot = 0;

global newU
newU = false;

global u
u = 1;

global discoverU
discoverU = false

disp('Wait for u')
while ~discoverU
    tick;
    pause(0.5);
end
discoverU = false;
disp('Got u')

while ( tSim-tEnd <= 0 )
    
    while ~newU
        tick;
        pause(0.001);
    end
    newU = false;
    
    tick;
    pause(0.001);
    
    ydot = ( -y + u*K ) / T1;
    y = y + h*ydot;
    
    attrHdlSet = {yID, double2net(y)};
    
    updateAttrVal(objInstID_y, attrHdlSet, 'y');
    
    y
    u
    
    tSim = tSim + h
end
    
disp('Resign Federation execution ... ')

e = resignFedExec('dr');

if isempty(e)
	disp('resigned.');
else
	error(['resignFedExec failed: ' e])
end


disp('Destroy federation execution ... ');

disp('Hit key to resume in creator of federation')
pause

e = destroyFedExec(federationName)

switch e
	case ''
		disp('destroyed.');
	case 'FederatesCurrentlyJoined'
		disp('other federates still joined.');
    case 'FederationExecutionDoesNotExist'
        disp('Federation already destroyed.')
    otherwise
        rtiOff
		error(['destroyFedExec: ' e])
end

rtiOff
