function [eventRetractHdl,except] = deleteObjInst(theObject, in1, in2)
%deleteObjInst	Delete Object Instance (federate initiated).
%
%Synopsis
%	[eventRetractHdl,except] = deleteObjInst(theObject, theTime, theTag)
%	except = deleteObjInst(theObject, theTag)
%
%	Two overloaded methods.

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

if nargin < 3
	except = rti(6080, theObject, in1);
else
	[eventRetractHdl,except] = rti(6080, theObject, in1, in2);
end