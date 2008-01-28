function turnInteractOff(theHandle)
%turnInteractOff	Turn Interactions Off (rti initiated).
%
%Synopsis:
%	turnInteractOff(theHandle)
%
%Purpose:
%	To inform the federate that no other federates in the federation
%	execution are subscribed to an interaction class that it has
%	published.

%      throw (
%              InteractionClassNotPublished,
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
 
 
trl_rt('turnInteractOff', theHandle)

return


%-------------------- default impl. -------------------------
cmd='';
 
while ~strcmp(cmd,'return')
        cmd = input('(NULL) turnInteractOff>> ','s');
        eval(cmd,'error_handler')
end
 
return
 
 
function error_handler()
        disp(lasterr)
return
 
