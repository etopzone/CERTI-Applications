function reflectAttrVal(theObject,theAttributes,in3,in4,in5)
%reflectAttrVal		Reflect Attribute Values (rti initiated).
%
%Synopsis:
%	reflectAttrVal(theObject,theAttributes,theTime,theTag,theHandle)
%	reflectAttrVal(theObject,theAttributes,theTag)
%
%Purpose:
%	Receive updates from the RTI of attributes that belong to
%	object class instances.

%	throw (		first method
%		ObjectNotKnown,
%		AttributeNotKnown,
%		FederateOwnsAttributes,
%		InvalidFederationTime,
%		FederateInternalError)

%	throw (		second method
%		ObjectNotKnown,
%		AttributeNotKnown,
%		FederateOwnsAttributes,
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

global y
global newY
 
if (nargin == 6)
	theTime   = in3;
	theTag    = in4;
	theHandle = in5;
	clear in3 in4 in5
else
	theTag    = in3;
	clear in3
end

y = net2double( theAttributes{1,2} );
newY = true;

return

cmd='';
 
while ~strcmp(cmd,'return')
        cmd = input('(NULL) reflectAttrVal>> ','s');
        eval(cmd,'error_handler')
end
 
return
 
 
function error_handler()
        disp(lasterr)
return