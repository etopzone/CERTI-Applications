function discoverObjInst(theObject, theObjectClass, theObjectName)
%discoverObjInst    Discover Object Instance (rti initiated).
%
%Synopsis:
%	discoverObjInst(theObject, theObjectClass, theObjectName)
%
%Purpose:
%	Allows a federate to discover other objects that exist
%	in remote federates. 

%      throw (
%		CouldNotDiscover,
%		ObjectClassNotKnown,
%		FederateInternalError)

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

global discoverU

discoverU = true;


return

cmd='';
 
while ~strcmp(cmd,'return')
        cmd = input('(NULL) discoverObjInst>> ','s');
        eval(cmd,'error_handler')
end
 
return
 
 
function error_handler()
        disp(lasterr)
return