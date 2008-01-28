function except = attrInScope(theObject, theAttributes)
%attrInScope    Attributes In Scope (rti initiated).
%
%Synopsis:
%	attrInScope(theObject, theAttributes)
%
%Purpose:
%   To advice a federate that a set of instance-attributes of a specified 
%   object instance is associated with update regions intersectiong the 
%   relevant  subscription regions of the federate.

%	throw (
%       ObjectNotKnown,
%       AttributeNotKnown,
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

cmd='';
 
while ~strcmp(cmd,'return')
        cmd = input('(NULL) attrInScope>> ','s');
        eval(cmd,'error_handler')
end
 
return
 
 
function error_handler()
        disp(lasterr)
return