function varargout = car_ts(varargin)
%car_ts		Time-Step Simulation of Car Traffic / HLA

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
	T.Current       = 0;    % start and current time
	T.Step          = 1;    % time step
	T.Terminate	= 0;	% termination flag; set to 1
				% by car_ts('terminate_Callback')
        T.Continue      = 1;    % continuation flag; set to
				% 0 by car_ts('stop_Callback')
				% 1 by car_ts('cont_Callback')
				% -1 by car_ts('step_Callback')


	car_ts('init')		% model init

	car_ts('hla_join')	% join hla

	car_ts('loop')		% simulation loop

	car_ts('hla_leave')	% leave hla

	car_ts('hla_down')	% terminate hla
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
function step_Callback
%Set continue flag to -1

global T

	T.Continue = -1;
return


%--------------------------------------------------------------------
function init
%Observer initialization

global Cars
        Cars.NextLhdl   = 1;    % next local handle
        Cars.Lhdl       = [];   % local handles
        Cars.Pos        = [];   % positions
        Cars.Ghdl       = [];   % animation handles
        Cars.Hhdl       = [];   % HLA instance handles
        Cars.Barrier    = 0;    % barrier on street on/off and position

	car_ts('init_anim')
return


%---------------------------------------------------------------
function init_anim
%Animation initialization

        clf reset
        cax = newplot;
 
        set(get(cax,'Parent'), ...              % figure properties
            'Name','car_ts', ...
            'NumberTitle','off', ...
            'DoubleBuffer','on', ...            % ???, sonst Flimmern
            'BackingStore','off', ...
            'Colormap',[]);
 
        set(cax, ...                            % axes properties
            'Visible','off', ...
            'Drawmode','Fast',...
            'NextPlot','add');
        axis([0 20 0 20]);                      % 'Position'
 
        plot([0 20],[11, 11],'k')
        plot([0 20],[ 9,  9],'k')
 
global Ghdl
        Ghdl.TCurrent = text(1,18,'t = ');
        Ghdl.Barrier  = plot([9 9],[9 11],'r','Visible','off');
 
 
        uicontrol('String','Barrier', ...
		  'Callback','car_ts(''barrier_Callback'')', ...
                  'Position',[230 170 60 20]);
 
 	uicontrol('String','Quit', ...
		  'Callback','car_ts(''terminate_Callback'')');

	uicontrol('String','Stop', ...
		  'Callback','car_ts(''stop_Callback'')', ...
                  'Position',[130 20 60 20]);
 
        uicontrol('String','Step', ...
		  'Callback','car_ts(''step_Callback'')', ...
                  'Position',[230 20 60 20]);
 
        uicontrol('String','Cont', ...
		  'Callback','car_ts(''cont_Callback'')', ...
                  'Position',[330 20 60 20]);
 
        drawnow
return


%------------------------------------------------------------------
function barrier_Callback(action)
%Toggles Cars.Barrier

global Cars
 
	if nargin < 1

        	if Cars.Barrier
                	Cars.Barrier = 0;
        	else
                	Cars.Barrier = 9;
        	end

	else

		if action == 'c'
			Cars.Barrier = 0;
		else
			Cars.Barrier = 9;
		end
	end
 
        car_ts('barrier_anim', Cars.Barrier)
return
 
 
%---------------------------------------------------------------
function barrier_anim(barr)
%Animates Cars.Barrier

global Ghdl

        if barr
                set(Ghdl.Barrier,'Visible','on');
        else
                set(Ghdl.Barrier,'Visible','off');
        end
 
        drawnow
return


%---------------------------------------------------------------
function hla_join
%Join HLA

global HLA
        HLA.RttCar      = [];   % HLA run-time-type for car objects
        HLA.RttCarPos   = [];   % HLA run-time-type for car pos. attributes
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
		[federateId, e] = joinFedExec('car_ts','Traffic-Distr1');
		pause(1)
	end
	if ~isempty(e)
		error(['joinFedExec failed: ' e])
	end


	% RTT identification
        HLA.RttCar    = getObjClassHdl('Car');
        HLA.RttCarPos = getAttrHdl(    'Pos', HLA.RttCar);

        HLA.RttTrl      = getInteractClassHdl('Trl');
        HLA.RttTrlEvent = getParamHdl(        'Event', HLA.RttTrl);


	% Publish Car
	publishObjClass(HLA.RttCar, HLA.RttCarPos);
	% Wenn durch ein anderes Federate subscribeObjClassAttr()
	% aufgerufen wird, erhaelt dieses Federate den startRegForObjClass()
	% Callback.
	% Dieser Informationsmechanismus koennte hier mit 
	% disableClassRelevanceAdvisorySwitch() abgeschaltet werden.
	% Hier wurde aber nur der startRegForObjClass() Callback als
	% Nop implementiert.
	% Analoges gilt fuer stopRegForObjClass()

	% Subscribe Trl
	subscribeInteractClass(HLA.RttTrl);
	% --> receiveInteract()
return


%---------------------------------------------------------------
function startRegForObjClass(theClass)
%Invoked by RTI to inform the federate that other federates have subscribed
%to a published object class.
 
disp('(car_ts) startRegForObjClass>> ... no operation');

return
 
 
%---------------------------------------------------------------
function stopRegForObjClass(theClass)
%Invoked by RTI to inform the federate that no longer any other federate
%have subscribed a published object class.

disp('(car_ts) stopRegForObjClass>> ... no operation');

return


%---------------------------------------------------------------
function receiveInteract(theInteraction,theParameters,in3,in4,in5)
%Invoked by RTI to tell the federate interactions sent from remote federates.

if (nargin == 5)
	% treatment of receiveInteract(5 input param) is not needed;
	% instead default implementation
	theTime	  = in3;
	theTag	  = in4;
	theHandle = in5;
	clear in3 in4 in5

	cmd='';
 
	while ~strcmp(cmd,'return')
        	cmd = input('(car_ts) receiveInteract(5 arg var)>> ','s');
        	eval(cmd,'error_handler')
	end
 
else
	% receiveInteract(3 input param) is used and implemented here
	theTag	  = in3;
	clear in3

	global HLA

	%DEBUG
	disp(['receive ' theTag])
	global LOG
	fprintf(LOG,'%s\n',['receive ' theTag]);

        if theInteraction == HLA.RttTrl
                event = net2char(theParameters{2});
		car_ts('barrier_Callback', event)
        else
                error('receiveInteract failed: unknown InteractClass')
        end

end

return
 

%---------------------------------------------------------------
function loop
%car_ts loop

global T

	while ~T.Terminate			% simulation loop

		[trash,e] = tick(0.3,0.3);  if ~isempty(e), error(e), end

		car_ts('generate')

		car_ts('move')

		car_ts('destroy')

		car_ts('time_step')

		if     T.Continue == 0
			while T.Continue == 0
				drawnow		% stop
			end
		elseif T.Continue == -1
			T.Continue = 0;
			while T.Continue == 0
				drawnow		% step
			end
		end
	end
return


%------------------------------------------------------------------
function generate
%Creates new cars

global Cars
 
        full = 0;
        if ~isempty(Cars.Lhdl)
                if Cars.Pos(1) == 0
                        full = 1;
                end
        end
 
        if ~full  &  rand > 0.9
                lhdl = Cars.NextLhdl;
                pos  = 0;
                ghdl = car_ts('generate_anim', lhdl);
		hhdl = car_ts('hla_register', lhdl);
 
                Cars.Lhdl     = [lhdl Cars.Lhdl];
                Cars.Pos      = [pos  Cars.Pos ];
                Cars.Ghdl     = [ghdl Cars.Ghdl];
                Cars.Hhdl     = [hhdl Cars.Hhdl];
 
                Cars.NextLhdl = Cars.NextLhdl + 1;
        end
return


%-----------------------------------------------------------
function ghdl = generate_anim(lhdl)
%Animates creation of new cars

	x  = 0;   y  = 10;
	xl = 0.4; yl = 0.4;
 
	ghdl = patch([x-xl,x-xl,x+xl,x+xl],...
             	     [y-yl,y+yl,y+yl,y-yl],...
             	     'g');
 
	text_ghdl = text(x,y,int2str(lhdl),'HorizontalAlignment','center');
 
	set(ghdl,'UserData',text_ghdl)

	drawnow
return


%-----------------------------------------------------------
function hhdl = hla_register(lhdl)
%hla_register	HLA: neues Auto registrieren

global HLA

	%DEBUG
	disp(['register car' int2str(lhdl) ])
	global LOG
	fprintf(LOG,'%s\n',['register car' int2str(lhdl) ]);
	
	hhdl = registerObjInst(HLA.RttCar, int2str(lhdl) );

return


%-----------------------------------------------------------
function move
%Moves cars

global Cars
 
        idx = find( Cars.Pos(:) >= Cars.Barrier );   % cars upon or behind barr.
        if ~isempty(idx)
                Cars.Pos(idx) = Cars.Pos(idx) + 1;
 
                car_ts('move_anim', Cars.Ghdl(idx) );

		car_ts('hla_update', Cars.Hhdl(idx), ...
			    	     Cars.Pos(idx), ...
			    	     Cars.Lhdl(idx) );
        end
 
        idx = find( Cars.Pos(:) < Cars.Barrier );    % cars in front of barrier
        if ~isempty(idx)
                if Cars.Pos(idx(end)) < Cars.Barrier-1    % car next to barrier
                        Cars.Pos(idx(end)) = Cars.Pos(idx(end)) + 1;
 
                        car_ts('move_anim', Cars.Ghdl(idx(end)) );

			car_ts('hla_update', Cars.Hhdl(idx(end)), ...
				    	     Cars.Pos(idx(end)), ...
				    	     Cars.Lhdl(idx(end)) );
                end
        end
        for i=1:length(idx)-1                   % rest of cars
                if Cars.Pos(i) < Cars.Pos(i+1)-1
                        Cars.Pos(i) = Cars.Pos(i) + 1;
 
                        car_ts('move_anim', Cars.Ghdl(i) );

			car_ts('hla_update', Cars.Hhdl(i), ...
				    	     Cars.Pos(i), ...
				    	     Cars.Lhdl(i) );
                end
        end


%--------------------------------------------------------------------
function move_anim(ghdl)
%Animates moving cars
%
%	move_anim(ghdl,delta_x)	ghdl may be a vector

	for i=ghdl
		set(i,'XDATA', ...
	    	    get(i,'XDATA') ...
	    	    + 1 )
 
		text_ghdl = get(i,'UserData');
		pos       = get(text_ghdl,'Position');
		pos(1)    = pos(1) + 1;
		set(text_ghdl,'Position',pos);
	end

	drawnow
return


%--------------------------------------------------------------------
function hla_update(hhdl,pos,lhdl)
%hla_update

global HLA

	for i=1:length(hhdl)

		tag = ['car' int2str(lhdl(i))];

		%DEBUG
		disp(['update ' tag])
		global LOG
		fprintf(LOG,'%s\n',['update ' tag]);

		updateAttrVal( hhdl(i), ...
		              {HLA.RttCarPos, double2net(pos(i))}, ...
		               tag);
	end
return


%--------------------------------------------------------------------
function destroy
%Destroys cars

global Cars
 
        if ~isempty(Cars.Lhdl)
                if Cars.Pos(end) > 20
			car_ts('hla_delete', Cars.Hhdl(end), Cars.Lhdl(end));

                        car_ts('destroy_anim', Cars.Ghdl(end) )
 
                        Cars.Lhdl = Cars.Lhdl(1:end-1);
                        Cars.Pos  = Cars.Pos(1:end-1);
                        Cars.Ghdl = Cars.Ghdl(1:end-1);
                        Cars.Hhdl = Cars.Hhdl(1:end-1);
                end
        end
return


%----------------------------------------------------------------
function destroy_anim(ghdl)
%Animates destroying cars

	text_ghdl = get(ghdl,'UserData');
 
	delete(text_ghdl)
 
	delete(ghdl)

	drawnow
return


%-------------------------------------------------------------
function hla_delete(hhdl,lhdl)
%hla_delete	HLA: delete object instance

	tag = ['car' int2str(lhdl)];

	%DEBUG
	disp(['delete ' tag]);
	global LOG
	fprintf(LOG,'%s\n',['delete ' tag]);
	
	deleteObjInst(hhdl,tag);
return


%-------------------------------------------------------------
function time_step
%Increase simulation time
 
global T
 
 
        T.Current = T.Current + T.Step;
 
%       fprintf('t = %d\n',T.Current)
 
	car_ts('time_step_anim', T.Current);
return
 
 
%-------------------------------------------------------------
function time_step_anim( t_current )
%Animates increasing simulation time
 
global Ghdl
 
        set(Ghdl.TCurrent,'String',['t = ' int2str(t_current)])
 
        drawnow
return


%---------------------------------------------------------------
function hla_leave
%Leave HLA

	% Resign federation
	% Aufruf mit Argument: DELETE OBJECTS, weil die Autos, die sich bei
	% Simulationsabbruch auf der Strasse befinden nicht durch 
	% destroy_car->hla_delete->deleteObjInst geloescht werden.
	e = resignFedExec('d');
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


