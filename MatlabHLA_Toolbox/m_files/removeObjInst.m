function removeObjInst(theObject,in2,in3,in4)
%removeObjInst		Remove Object Instance (rti initiated).
%
%Synopsis:
%	removeObjInst(theObject, theTime, theTag, theHandle)
%	removeObjInst(theObject, theTag)
%Purpose:
%	Either, a remote federate has deleted one of its object instances
%	or it no longer meets my subscription criteria so the RTI is
%	telling me that I can remove the object Id from my database.

%	throw (		first method
%		ObjectNotKnown,
%		InvalidFederationTime,
%		FederateInternalError)
 
%	throw (		second method
%		ObjectNotKnown,
%		FederateInternalError)

%    Copyright (C) 2008 Christian Stenzel, Sven Pawletta, Thorsten Pawletta

%    This file is part of MatlabHLA13.
%
%    MatlabHLA13 is free software: you can redistribute it and/or
%    modify it under the terms of the GNU Lesser General Public 
%    License as published by the Free Software Foundation, either 
%    version 3 of the License, or (at your option) any later version.
%
%    MatlabHLA13 is distributed in the hope that it will be useful,
%    but WITHOUT ANY WARRANTY; without even the implied warranty of
%    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%    GNU Lesser General Public License for more details.
%
%    You should have received a copy of the GNU Lesser General Public 
%    License along with MatlabHLA13.  
%    
%    If not, see <http://www.gnu.org/licenses/>.
 
 
if (nargin == 5)
	theTime	  = in2;
	theTag	  = in3;
	theHandle = in4;
	clear in2 in3 in4
else
	theTag	  = in2;
	clear in2
end

cmd='';
 
while ~strcmp(cmd,'return')
        cmd = input('(NULL) removeObjInst>> ','s');
        eval(cmd,'error_handler')
end
 
return
 
 
function error_handler()
        disp(lasterr)
return