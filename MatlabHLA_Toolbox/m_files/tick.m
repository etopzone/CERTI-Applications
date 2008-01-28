function [bool,except] = tick(minimum, maximum)
%tick	Turn Control Over to RTI (federate initiated).
%
%Synopsis
%	[bool,except] = tick()
%   tick('rep')                             - starts tick timer with default   
%                                             period of 0.1 secs
%   tick('rep',value)                       - starts tick timer with the
%                                             period specified by value
%   tick('off')                             - stops the tick timer
%
%   [bool,except] = tick(minimum, maximum)

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

persistent to;              %TimerObject

switch nargin

	case 0
		[bool,except] = rti(10310);
        
    case 1
       
        switch minimum

            case 'rep'
                
                if isempty(to)
                    to    = timer;
                    value = 0.1;
                    set(to,'BusyMode','queue','ExecutionMode','fixedRate', ...
                           'Period',value,'TimerFcn','[bool,except]=rti(10310);');
                    start(to);
                else
                    disp('TimerObject already started');
                end
                
            case 'off'
                
                if ~isempty(to)
                    stop(to);
                    clear to;
                else
                    disp('No TimerObject started');
                end
        end
                
    case 2
		
        switch minimum
            
            case 'rep'
                if isempty(to) & isnumeric(maximum)
                    to    = timer;
                    value = maximum;
                    set(to,'BusyMode','queue','ExecutionMode','fixedRate', ...
                           'Period',value,'TimerFcn','[bool,except]=rti(10310);');
                    start(to);
                else
                    if ~isnumeric(maximum)
                        disp('Value for period must be numeric!');
                    else
                        disp('TimerObject already started');
                    end
                end
                
            otherwise
                [bool,except] = rti(10310, minimum, maximum);
        end

	otherwise
		error(['tick() with ', ...
		       int2str(nargin), ...
		       ' input args is undefined!'])
end


function error_handler()
        disp(lasterr);
return;