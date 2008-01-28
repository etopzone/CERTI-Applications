function varargout = trl_rt(varargin)
%trl_rt		Real-Time Simulation of Traffic-Light / HLA


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

% switchboard for local sub-functions
if nargin > 0
	[varargout{1:nargout}] = feval(varargin{:});
	return
end


% main function
%DEBUG
global LOG
        LOG=fopen('log.txt','w');

global T
	T.Current	= trl_rt('wallclock');	% start and current time
	T.NextEvent	= [];		% time of next event
	T.Terminate	= 0;		% termination flag; set to 1
					% by trl_rt('terminate_Callback')
        T.Continue      = 1;    	% continuation flag; set to
					% 0 by trl_rt('stop_Callback')
					% 1 by trl_rt('cont_Callback')

global EL                       	% event list
        EL.Time         = [];   	% event time
        EL.Event        = [];   	% event

 
	trl_rt('init')			% model init

	trl_rt('hla_join')		% join hla

	trl_rt('loop')			% simulation loop

	trl_rt('hla_leave')		% leave hla

	trl_rt('hla_down')		% terminate hla
return


%--------------------------------------------------------------------
function t = wallclock
%Returns wallclock time
 
        t = clock;
        t = t(4) * 3600 + t(5) * 60 + t(6);
return


%--------------------------------------------------------------------
function terminate_Callback
%Set termination flag to 1

global T

	T.Terminate = 1;
return


%--------------------------------------------------------------------
function stop_Callback
%Set continue flag to 0

global T

	T.Continue = 0;
return


%--------------------------------------------------------------------
function cont_Callback
%Set continue flag to 1

global T

	T.Continue = 1;
return


%--------------------------------------------------------------------
function init
%Model initialization

global T EL
 
global Trl
        Trl.NextLhdl            = 1;    % next local handle
        Trl.CarGreenTime        = 10;   % green duration for cars
        Trl.PedGreenTime        = 5;    % green duration for peds
 
        EL.Time  = [T.Current EL.Time]; % plan initial event
        EL.Event = ['c'       EL.Event];
 
        trl_rt('init_anim')
return


%---------------------------------------------------------------
function init_anim
%Animation initialization
 
        clf reset
        cax = newplot;
 
        set(get(cax,'Parent'), ...              % figure properties
            'Name','trl_rt', ...
            'NumberTitle','off', ...
            'DoubleBuffer','on', ...            % ???, sonst Flimmern
            'BackingStore','off', ...
            'Colormap',[]);
 
        set(cax, ...                            % axes properties
            'Visible','off', ...
            'Drawmode','Fast',...
            'NextPlot','add');
        axis([0 20 0 20]);                      % 'Position'
 
        plot([0 20],[11, 11],'k')       % street for cars
        plot([0 20],[ 9,  9],'k')
        plot(9,9.3,'ko')                % traffic light for cars
 
        plot([ 9,  9],[0 20],'k')       % path for peds
        plot([11, 11],[0 20],'k')
        plot(10.7,9,'ko')               % traffic light for peds
 
global Ghdl
        Ghdl.TCurrent   = text( 1,18,'tc = ');
        Ghdl.TNextEvent = text( 1,17,'tn = ');
        Ghdl.Event      = text(12,18,'event: ');
 
	uicontrol('String','Quit', ...
		  'Callback','trl_rt(''terminate_Callback'')');

        uicontrol('String','Stop', ...
		  'Callback','trl_rt(''stop_Callback'')', ...
                  'Position',[130 20 60 20]);
 
        uicontrol('String','Cont', ...
		  'Callback','trl_rt(''cont_Callback'')', ...
                  'Position',[330 20 60 20]);

        drawnow
return


%---------------------------------------------------------------
function hla_join
%Join HLA

global HLA
        HLA.RttTrl      = [];   % HLA run-time-type for trl event class
        HLA.RttTrlEvent = [];   % HLA run-time-type for trl event parameter


	% Connect to RTI
	rtiOn

	% Create federation
	e = createFedExec('Traffic-Distr1','TrafficDistr1.fed');
	if ~isempty(e) & ~strcmp(e,'FederationExecutionAlreadyExists')
		error(['createFedExec failed: ' e])
	end

	% Join federation
	e = 'FederationExecutionDoesNotExist';
	while strcmp(e,'FederationExecutionDoesNotExist')
		[federateId, e] = joinFedExec('trl_rt','Traffic-Distr1');
		pause(1)
	end
	if ~isempty(e)
		error(['joinFedExec failed: ' e])
	end

	% RTT identification
        HLA.RttTrl      = getInteractClassHdl('Trl');
        HLA.RttTrlEvent = getParamHdl(        'Event', HLA.RttTrl);

	% Publish Trl
	publishInteractClass(HLA.RttTrl);
	% Wenn durch ein anderes Federate subscribeInteractClass()
	% aufgerufen wird, erhaelt dieses Federate den turnInteractOn()
	% Callback.
	% Dieser Informationsmechanismus koennte hier mit 
	% disableInteractionRelevanceAdvisorySwitch() abgeschaltet werden.
	% Hier wurde aber nur der turnInteractOff() Callback als
	% Nop implementiert.
	% Analoges gilt fuer turnInteractOff()
return


%---------------------------------------------------------------
function turnInteractOn(theHandle)
%Invoked by RTI to inform the federate that other federates have subscribed
%to a published interaction class.
 
disp('(trl_rt) turnInteractOn>> ... no operation');

return


%---------------------------------------------------------------
function turnInteractOff(theHandle)
%Invoked by RTI to inform the federate that no longer any other federate
%has subscribed to a published interaction class.
%turnInteractOff	Turn Interactions Off (rti initiated).
 
disp('(trl_rt) turnInteractOff>> ... no operation');

return


%---------------------------------------------------------------
function loop
%trl_rt loop

global T


	while ~T.Terminate			% simulation loop

		[trash,e] = tick;  if ~isempty(e), error(e), end

		trl_rt('next_event_time')

		trl_rt('process_events')

		trl_rt('rt_sync')

                if     T.Continue == 0
                        while T.Continue == 0
                                drawnow         % stop
                        end
                end
	end
return


%------------------------------------------------------------------
function next_event_time
%Determines next event time

global T EL


	T.NextEvent = EL.Time(1);

	trl_rt('next_event_time_anim', T.NextEvent )
return


%---------------------------------------------------------------
function next_event_time_anim( t )
%Animates next event time
 
global Ghdl
 
        h = fix(t/3600); t = t - 3600 * h;
        m = fix(t/60);   t = t -   60 * m;
        s = fix(t);
        t_str = [int2str(h) ':'   ...
                 int2str(m) ' '   ...
                 int2str(s)          ];

%       fprintf('tn = %s\n',t_str)

        set(Ghdl.TNextEvent,'String',['tn = ' t_str])
 
        drawnow
return


%------------------------------------------------------------------
function process_events
%Process next events

global T EL

	if T.Current >= T.NextEvent

        	event = EL.Event(1);
 
        	EL.Time  = EL.Time(2:end);
        	EL.Event = EL.Event(2:end);
 
        	switch event
                	case abs('c'), trl_rt('car_green')
                	case abs('p'), trl_rt('ped_green')
                	otherwise,     error('Unknown event.')
        	end
	end
return


%------------------------------------------------------------------
function car_green
%Switch traffic ligth to car green

global T EL Trl

	lhdl         = Trl.NextLhdl;
	Trl.NextLhdl = Trl.NextLhdl + 1;

	trl_rt('car_green_anim', lhdl)

	trl_rt('hla_send', lhdl, 'c')

	EL.Time(1)  = [T.Current+Trl.CarGreenTime EL.Time];    % plan next event
	EL.Event(1) = ['p'                        EL.Event];
return


%-------------------------------------------------------------
function car_green_anim(lhdl)
%Animates switching traffic ligth to car green

global Ghdl

	set(Ghdl.Event,'String',['event: c ' int2str(lhdl)])
 
	plot( 9.0,9.3,'g*')		% green for cars
	plot(10.7,9.0,'r*')		% red for peds

        drawnow
return


%------------------------------------------------------------------
function ped_green
%Switch traffic ligth to ped green

global T EL Trl

	lhdl         = Trl.NextLhdl;
	Trl.NextLhdl = Trl.NextLhdl + 1;

	trl_rt('ped_green_anim', lhdl)

	trl_rt('hla_send', lhdl, 'p')

	EL.Time(1)  = [T.Current+Trl.PedGreenTime EL.Time];    % plan next event
	EL.Event(1) = ['c'                        EL.Event];
return


%---------------------------------------------------------------
function ped_green_anim(lhdl)
%Animates switching traffic ligth to ped green

global Ghdl

	set(Ghdl.Event,'String',['event: p ' int2str(lhdl)])
 
	plot( 9.0,9.3,'r*')		% red for cars
	plot(10.7,9.0,'g*')		% green for peds

	drawnow
return


%----------------------------------------------------------------
function hla_send(lhdl,event)
%hla_send

global HLA

	tag = ['trl ' event int2str(lhdl)];

	%DEBUG
	disp(['send ' tag])
	global LOG
	fprintf(LOG,'%s\n',['send ' tag]);

	sendInteract( HLA.RttTrl, ...
	              {HLA.RttTrlEvent, char2net(event)}, ...
	               tag);
return


%----------------------------------------------------------------
function rt_sync
%Synchronize with real-time

global T

	t         = wallclock;
	rt_diff   = T.Current - t;
	T.Current = t;

	trl_rt('rt_sync_anim', T.Current, rt_diff )
return


%-------------------------------------------------------------
function rt_sync_anim( t_current, rt_diff )
%Animates synchronization with real-time

global Ghdl

	t = t_current;                          % wallclock time
        h = fix(t/3600); t = t - 3600 * h;
        m = fix(t/60);   t = t -   60 * m;
        s = fix(t);
        t_str = [int2str(h) ':'   ...
                 int2str(m) ' '   ...
                 int2str(s)          ];
 
        diff_str = sprintf('%1.1f',rt_diff);    % delay against wallclock time
 
%       fprintf('t = %s   RTdiff = %s\n',t_str,diff_str);
 
       	set(Ghdl.TCurrent,'String',['tc = ' t_str '   RTdiff = ' diff_str])

        drawnow
return


%---------------------------------------------------------------
function hla_leave
%Leave HLA

	% Resign federation
	e = resignFedExec('');
	if ~isempty(e)
		error(['resignFedExec failed: ' e])
	end
return


%---------------------------------------------------------------
function hla_down
%Destroy federation Traffic-Distr1 and disconnect from RTI

	% Destroy federation
	e = destroyFedExec('Traffic-Distr1');
	if ~isempty(e) & ~strcmp(e,'FederatesCurrentlyJoined')
		error(['destroyFedExec failed: ' e])
	end

	% Disconnect from RTI
	rtiOff
return


