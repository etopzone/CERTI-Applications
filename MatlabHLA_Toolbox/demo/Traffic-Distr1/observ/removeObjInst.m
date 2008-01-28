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

%    Copyright (c) 1998-2000 University of Rostock, Germany,
%    Institute of Automatic Control. All rights reserved.
%    Author: S. Pawletta

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
 
 
if (nargin == 5)
	theTime	  = in2;
	theTag	  = in3;
	theHandle = in4;
	clear in2 in3 in4
	observ('removeObjInst', ...
		theObject, theTime, theTag, theHandle)
else
	theTag	  = in2;
	clear in2
	observ('removeObjInst', ...
		theObject, theTag)
end

return


%--------------------------- default impl. ----------------------
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


