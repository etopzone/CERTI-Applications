% control federate

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
federateName    = 'controlFed';
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

subscribeObjClassAttr( twoLevelContrID, yID);
publishObjClass(twoLevelContrID, uID);

objInstID_u = registerObjInst(twoLevelContrID, 'u')

global discoverY
discoverY = false

disp('Wait for y')
while ~discoverY
    tick;
    pause(0.5)
end
discoverY = false;
disp('Got y')

tEnd = 30;
tStart = 0;
h = 0.1;
w = 0.5;
K = 1;
T1 = 5;

tSim = tStart;
u = 1;

global newY
newY = false;

global y
y = 0;

while ( tSim-tEnd <= 0 )
    
    tick;
    pause(0.001);
    
    if (y < w-0.1 ), u=1; end
    if (y > w+0.1 ), u=0; end
   
    attrHdlSet = {uID, double2net(u)};
      
    updateAttrVal(objInstID_u, attrHdlSet, 'u')
    
    while ~newY
        tick;
        pause(0.001);
    end
    newY = false;
    
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

