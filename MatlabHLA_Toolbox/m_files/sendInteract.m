function [eventRetractHdl,except] = sendInteract(iact,paramHdlValPairSet,i3,i4)
%sendInteract	Send Interaction (federate initiated).
%
%Synopsis
%	[eventRetractHdl,except] = sendInteract(theInteraction,theParameters,...
%						theTime,       theTag)
%	except = sendInteract(theInteraction, theParameters,...
%			      theTag)
%
%	Two overloaded methods.
%
%	theParameters is a Parameter Handle Value Pair Set represented by
%	an m x 2 cell array. The first column contains the Parameter Handles
%	as double values. The second column contains the Parameter Values
%	in a network transparent representation (see double2net and char2net).
%
%	Attention: eventRetractHdl is in Matlab a 2-element vector.

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

if nargin < 4
	except = rti(6060, iact, paramHdlValPairSet, i3);
else
	[eventRetractHdl,except] = rti(6060, iact, paramHdlValPairSet, i3, i4);
end