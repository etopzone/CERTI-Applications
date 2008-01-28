function startRegForObjClass(theClass)
%startRegForObjClass	Start Registration for Object Class (rti initiated).
%
%Synopsis:
%	startRegForObjClass(theClass)
%
%Purpose:
%	To inform the federate that other federates in the federation
%	execution have subscribed to an object class that it has
%	published.

%	throw (
%		ObjectClassNotPublished,
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
 
 
ped_ts('startRegForObjClass', theClass)

return


%-------------------- default impl. -------------------------
cmd='';
 
while ~strcmp(cmd,'return')
        cmd = input('(NULL) startRegForObjClass>> ','s');
        eval(cmd,'error_handler')
end
 
return
 
 
function error_handler()
        disp(lasterr)
return
