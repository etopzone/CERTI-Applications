function varargout = ped_ts(varargin)
%ped_ts		Time-Step Simulation of Pedestrian Traffic / HLA

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
				% by ped_ts('terminate_Callback')
        T.Continue      = 1;    % continuation flag; set to
				% 0 by ped_ts('stop_Callback')
				% 1 by ped_ts('cont_Callback')
				% -1 by ped_ts('step_Callback')
 
 
	ped_ts('init')		% model init

	ped_ts('hla_join')	% join hla

	ped_ts('loop')		% simulation loop

	ped_ts('hla_leave')	% leave hla

	ped_ts('hla_down')	% terminate hla
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
%Model initialization

global Peds
        Peds.NextLhdl   = 1;    % next local handle
        Peds.Lhdl       = [];   % local handles
        Peds.Pos        = [];   % positions
        Peds.Ghdl       = [];   % animation handles
        Peds.Hhdl       = [];   % HLA instance handles
        Peds.Barrier    = 0;    % barrier on path on/off and position

	ped_ts('init_anim')
return


%------------------------------------------------------------------
function init_anim
%Animation initialization

        clf reset
        cax = newplot;
 
        set(get(cax,'Parent'), ...              % figure properties
            'Name','ped_ts', ...
            'NumberTitle','off', ...
            'DoubleBuffer','on', ...            % ???, sonst Flimmern
            'BackingStore','off', ...
            'Colormap',[]);
 
        set(cax, ...                            % axes properties
            'Visible','off', ...
            'Drawmode','Fast',...
            'NextPlot','add');
        axis([0 20 0 20]);                      % 'Position'
  
        plot([ 9,  9],[0 20],'k')
        plot([11, 11],[0 20],'k')

global Ghdl
        Ghdl.TCurrent = text(1,18,'t = ');
        Ghdl.Barrier  = plot([9 11],[9 9],'r','Visible','off');

	uicontrol('String','Barrier',...
		  'Callback','ped_ts(''barrier_Callback'')',...
                  'Position',[200 190 60 20]);
 
	uicontrol('String','Quit',...
		  'Callback','ped_ts(''terminate_Callback'')');

	uicontrol('String','Stop', ...
		  'Callback','ped_ts(''stop_Callback'')', ...
                  'Position',[130 20 60 20]);
 
        uicontrol('String','Step', ...
		  'Callback','ped_ts(''step_Callback'')', ...
                  'Position',[230 20 60 20]);
 
        uicontrol('String','Cont', ...
		  'Callback','ped_ts(''cont_Callback'')', ...
                  'Position',[330 20 60 20]);

	drawnow
return


%------------------------------------------------------------------
function barrier_Callback(action)
%Toggles Peds.Barrier

global Peds
 
	if nargin < 1

		if Peds.Barrier
			Peds.Barrier = 0;
		else
			Peds.Barrier = 9;
		end

	else

		if action == 'p'
			Peds.Barrier = 0;
		else
			Peds.Barrier = 9;
		end
	end

        ped_ts('barrier_anim', Peds.Barrier)
return
 
 
%---------------------------------------------------------------
function barrier_anim(barr)
%Animates Peds.Barrier

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
        HLA.RttPed      = [];   % HLA run-time-type for ped objects
        HLA.RttPedPos   = [];   % HLA run-time-type for ped pos. attributes
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
		[federateId, e] = joinFedExec('ped_ts','Traffic-Distr1');
		pause(1)
	end
	if ~isempty(e)
		error(['joinFedExec failed: ' e])
	end


	% RTT identification
        HLA.RttPed    = getObjClassHdl('Ped');
        HLA.RttPedPos = getAttrHdl(    'Pos', HLA.RttPed);

        HLA.RttTrl      = getInteractClassHdl('Trl');
        HLA.RttTrlEvent = getParamHdl(        'Event', HLA.RttTrl);


	% Publish Ped
	publishObjClass(HLA.RttPed, HLA.RttPedPos);
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
return


%---------------------------------------------------------------
function startRegForObjClass(theClass)
%Invoked by RTI to inform the federate that other federates have subscribed
%to a published object class.
 
disp('(ped_ts) startRegForObjClass>> ... no operation');

return
 
 
%---------------------------------------------------------------
function stopRegForObjClass(theClass)
%Invoked by RTI to inform the federate that no longer any other federate
%have subscribed a published object class.

disp('(ped_ts) stopRegForObjClass>> ... no operation');

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
        	cmd = input('(ped_ts) receiveInteract(5 arg var)>> ','s');
        	eval(cmd,'error_handler')
	end
 
else
	% receiveInteract(2 input param) is used and implemented here
	theTag	  = in3;
	clear in3

	global HLA

	%DEBUG
	disp(['receive ' theTag])
	global LOG
	fprintf(LOG,'%s\n',['receive ' theTag]);

        if theInteraction == HLA.RttTrl
                event = net2char(theParameters{2});
		ped_ts('barrier_Callback', event)
        else
                error('receiveInteract failed: unknown InteractClass')
        end

end

return
 

%---------------------------------------------------------------
function loop
%ped_ts loop

global T

	while ~T.Terminate			% simulation loop

		[trash,e] = tick(0.3,0.3);  if ~isempty(e), error(e), end

		ped_ts('generate')

		ped_ts('move')

		ped_ts('destroy')

		ped_ts('time_step')

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
%Creates new peds

global Peds

	full = 0;
	if ~isempty(Peds.Lhdl)
		if Peds.Pos(1) == 0
			full = 1;
		end
	end

	if ~full  &  rand > 0.5
                lhdl = Peds.NextLhdl;
                pos  = 0;
		ghdl = ped_ts('generate_anim', lhdl);
		hhdl = ped_ts('hla_register', lhdl);
 
                Peds.Lhdl     = [lhdl Peds.Lhdl];
                Peds.Pos      = [pos  Peds.Pos ];
                Peds.Ghdl     = [ghdl Peds.Ghdl];
		Peds.Hhdl     = [hhdl Peds.Hhdl];

                Peds.NextLhdl = Peds.NextLhdl + 1;
	end
return


%-----------------------------------------------------------
function ghdl = generate_anim(lhdl)
%Animates creation of new peds

        x  = 10;  y  = 0;
        xl = 0.2; yl = 0.4;
 
        ghdl = patch([x-xl,x-xl,x+xl,x+xl],...
                     [y-yl,y+yl,y+yl,y-yl],...
                     'g');
 
        text_ghdl = text(x,y,int2str(lhdl),'HorizontalAlignment','center');
 
        set(ghdl,'UserData',text_ghdl)

	drawnow
return


%-----------------------------------------------------------
function hhdl = hla_register(lhdl)
%hla_register	HLA: neuen Fussgaenger registrieren

global HLA

	%DEBUG
	disp(['register ped' int2str(lhdl) ])
	global LOG
	fprintf(LOG,'%s\n',['register ped' int2str(lhdl) ]);
	
	hhdl = registerObjInst(HLA.RttPed, int2str(lhdl) );

return


%--------------------------------------------------------------------
function move
%Moves peds

global Peds

	idx = find( Peds.Pos(:) >= Peds.Barrier );   % peds upon or behind barr.
	if ~isempty(idx)
		Peds.Pos(idx) = Peds.Pos(idx) + 1;

		ped_ts('move_anim', Peds.Ghdl(idx));

		ped_ts('hla_update', Peds.Hhdl(idx), ...
			    	     Peds.Pos(idx), ...
			    	     Peds.Lhdl(idx) );
	end

	idx = find( Peds.Pos(:) < Peds.Barrier );    % peds in front of barrier
	if ~isempty(idx)
		if Peds.Pos(idx(end)) < Peds.Barrier-1	% ped next to barrier
			Peds.Pos(idx(end)) = Peds.Pos(idx(end)) + 1;

			ped_ts('move_anim', Peds.Ghdl(idx(end)));

			ped_ts('hla_update', Peds.Hhdl(idx(end)), ...
				    	     Peds.Pos(idx(end)), ...
				    	     Peds.Lhdl(idx(end)) );
		end
	end
	for i=1:length(idx)-1			% rest of peds
		if Peds.Pos(i) < Peds.Pos(i+1)-1
			Peds.Pos(i) = Peds.Pos(i) + 1;

			ped_ts('move_anim', Peds.Ghdl(i));

			ped_ts('hla_update', Peds.Hhdl(idx(end)), ...
				    	     Peds.Pos(idx(end)), ...
				    	     Peds.Lhdl(idx(end)) );
		end
	end
return


%--------------------------------------------------------------------
function move_anim(ghdl)
%Animates moving peds
%
%	move_anim(ghdl)	ghdl may be a vector

	for i=ghdl
		set(i,'YDATA', ...
	            get(i,'YDATA') ...
	            + 1 )

                text_ghdl = get(i,'UserData');
                pos       = get(text_ghdl,'Position');
                pos(2)    = pos(2) + 1;
                set(text_ghdl,'Position',pos);
	end

	drawnow
return


%--------------------------------------------------------------------
function hla_update(hhdl,pos,lhdl)
%hla_update

global HLA

	for i=1:length(hhdl)

		tag = ['ped' int2str(lhdl(i))];

		%DEBUG
		disp(['update ' tag])
		global LOG
		fprintf(LOG,'%s\n',['update ' tag]);

		updateAttrVal( hhdl(i), ...
		              {HLA.RttPedPos, double2net(pos(i))}, ...
		               tag);
	end
return


%--------------------------------------------------------------------
function destroy
%Destroys peds

global Peds

	if ~isempty(Peds.Lhdl)
		if Peds.Pos(end) > 20
			ped_ts('hla_delete', Peds.Hhdl(end), Peds.Lhdl(end));

                        ped_ts('destroy_anim', Peds.Ghdl(end) )
 
                        Peds.Lhdl = Peds.Lhdl(1:end-1);
                        Peds.Pos  = Peds.Pos(1:end-1);
                        Peds.Ghdl = Peds.Ghdl(1:end-1);
                        Peds.Hhdl = Peds.Hhdl(1:end-1);
		end
	end
return


%----------------------------------------------------------------
function destroy_anim(ghdl)
%Animates destroying peds

        text_ghdl = get(ghdl,'UserData');
 
        delete(text_ghdl)
 
	delete(ghdl);

	drawnow
return


%-------------------------------------------------------------
function hla_delete(hhdl,lhdl)
%hla_delete	HLA: delete object instance

	tag = ['ped' int2str(lhdl)];

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

%	fprintf('t = %d\n',T.Current)

	ped_ts('time_step_anim', T.Current);
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
	% Aufruf mit Argument: DELETE OBJECTS, weil die Fussgaenger, die sich 
	% bei Simulationsabbruch auf der Strasse befinden nicht durch 
	% destroy_ped->hla_delete->deleteObjInst geloescht werden.
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


