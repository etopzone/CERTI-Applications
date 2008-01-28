function receiveInteract(theInteraction,theParameters,in3,in4,in5)
%receiveInteract	Receive Interactions (rti initiated).
%
%Synopsis:
%	receiveInteract(theInteraction,theParameters,theTime,theTag,theHandle)
%	receiveInteract(theInteraction,theParameters,theTag)
%
%Purpose:
%	Receive interactions sent from remote federates.

%	throw (		first method
%		InteractionClassNotKnown,
%		InteractionParameterNotKnown,
%		InvalidFederationTime,
%		FederateInternalError)

%	throw (		second method
%		InteractionClassNotKnown,
%		InteractionParameterNotKnown,
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

if (nargin == 6)
	theTime	  = in3;
	theTag	  = in4;
	theHandle = in5;
	clear in3 in4 in5
else
	theTag	  = in3;
	clear in3
end

cmd='';
 
while ~strcmp(cmd,'return')
        cmd = input('(NULL) receiveInteract>> ','s');
        eval(cmd,'error_handler')
end
 
return
 
 
function error_handler()
        disp(lasterr)
return