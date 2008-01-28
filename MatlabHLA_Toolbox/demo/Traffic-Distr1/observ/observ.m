function varargout = observ(varargin)
%observ		Observer for Distributed Traffic Simulations / HLA

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
	T.Terminate	= 0;	% termination flag; set to 1
				% by observ('terminate_Callback')


	observ('init')		% observer initialization

	observ('hla_join')	% join hla
    
    keyboard

	observ('loop')		% observer loop

	observ('hla_leave')	% leave hla

	observ('hla_down')	% terminate hla
return


%--------------------------------------------------------------------
function terminate_Callback
%Set termination flag to 1

global T

	T.Terminate = 1;
return


%--------------------------------------------------------------------
function init
%Observer initialization

global Cars
        Cars.Lhdl       = [];   % local handles
        Cars.Pos        = [];   % positions
        Cars.Ghdl       = [];   % animation handles
        Cars.Hhdl       = [];   % HLA instance handles

global Peds
        Peds.Lhdl       = [];   % local handles
        Peds.Pos        = [];   % positions
        Peds.Ghdl       = [];   % animation handles
        Peds.Hhdl       = [];   % HLA instance handles

	observ('init_anim')
return


%---------------------------------------------------------------
function init_anim
%Animation initialization

        clf reset
        cax = newplot;
 
        set(get(cax,'Parent'), ...              % figure properties
            'Name','observ', ...
            'NumberTitle','off', ...
            'DoubleBuffer','on', ...            % ???, sonst Flimmern
            'BackingStore','off', ...
            'Colormap',[]);
 
        set(cax, ...                            % axes properties
            'Visible','off', ...
            'Drawmode','Fast',...
            'NextPlot','add');
        axis([0 20 0 20]);                      % 'Position'
 
        plot([0 20],[11, 11],'k')	% street for cars
        plot([0 20],[ 9,  9],'k')
        plot(9,9.3,'ko')                % traffic light for cars
 
        plot([ 9,  9],[0 20],'k')	% path for peds
        plot([11, 11],[0 20],'k')
        plot(10.7,9,'ko')               % traffic light for peds
 
	uicontrol('String','Quit', ...
		  'Callback','observ(''terminate_Callback'')');


        drawnow
return


%---------------------------------------------------------------
function hla_join
%Join HLA

global HLA
	HLA.RttCar	= [];	% HLA run-time-type for car objects 
	HLA.RttCarPos	= [];	% HLA run-time-type for car pos. attributes 
	HLA.RttPed	= [];	% HLA run-time-type for ped objects 
	HLA.RttPedPos	= [];	% HLA run-time-type for ped pos. attributes 
        HLA.RttTrl      = [];	% HLA run-time-type for trl event class
        HLA.RttTrlEvent = [];	% HLA run-time-type for trl event parameter

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
		[federateId, e] = joinFedExec('observ','Traffic-Distr1');
		pause(1)
	end
	if ~isempty(e)
		error(['joinFedExec failed: ' e])
	end

	% RTT identification
	HLA.RttCar      = getObjClassHdl(     'Car');
	HLA.RttCarPos   = getAttrHdl(         'Pos', HLA.RttCar);

	HLA.RttPed      = getObjClassHdl(     'Ped');
	HLA.RttPedPos   = getAttrHdl(         'Pos', HLA.RttPed);

        HLA.RttTrl      = getInteractClassHdl('Trl');
        HLA.RttTrlEvent = getParamHdl(        'Event', HLA.RttTrl);

	% Subscribe Car, Ped and Trl
	subscribeObjClassAttr(HLA.RttCar, HLA.RttCarPos);
	subscribeObjClassAttr(HLA.RttPed, HLA.RttPedPos);
	% --> Callback discoverObjInst()
	% --> Callback reflectAttrVal()
	% --> Callback removeObjInst()
	subscribeInteractClass(HLA.RttTrl);
	% --> receiveInteract()
return


%---------------------------------------------------------------
function discoverObjInst(theObject, theObjectClass, theObjectName)
%Invoked by RTI to tell observer new object instances of other federates
 
global HLA

	switch theObjectClass

		case HLA.RttCar

			%DEBUG
			disp(['discover car' theObjectName])
			global LOG
			fprintf(LOG,'%s\n',['discover car' theObjectName]);

			observ('car_generate', theObject, theObjectName)

		case HLA.RttPed

			%DEBUG
			disp(['discover ped' theObjectName])
			global LOG
			fprintf(LOG,'%s\n',['discover ped' theObjectName]);
			
			observ('ped_generate', theObject, theObjectName)

		otherwise

			%DEBUG
			disp(['discover UNKNOWN' theObjectName])
			global LOG
			fprintf(LOG,'%s\n',['discover UNKNOWN' theObjectName]);

			error('discoverObjInst failed: unknown ObjClass')
	end
return


%---------------------------------------------------------------
function car_generate(hhdl,lhdl)
%Creates new cars

global Cars

	lhdl = str2num(lhdl);
	pos  = 0;
	ghdl = observ('car_generate_anim', lhdl);
	
	Cars.Lhdl = [lhdl Cars.Lhdl];;
	Cars.Pos  = [pos  Cars.Pos ];
	Cars.Ghdl = [ghdl Cars.Ghdl];
	Cars.Hhdl = [hhdl Cars.Hhdl];
return


%---------------------------------------------------------------
function ghdl = car_generate_anim(lhdl)
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
 
 
%---------------------------------------------------------------
function ped_generate(hhdl,lhdl)
%Creates new peds

global Peds

	lhdl = str2num(lhdl);
	pos  = 0;
	ghdl = observ('ped_generate_anim', lhdl);
	
	Peds.Lhdl = [lhdl Peds.Lhdl];;
	Peds.Pos  = [pos  Peds.Pos ];
	Peds.Ghdl = [ghdl Peds.Ghdl];
	Peds.Hhdl = [hhdl Peds.Hhdl];
return


%---------------------------------------------------------------
function ghdl = ped_generate_anim(lhdl)
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
 
 
%---------------------------------------------------------------
function reflectAttrVal(theObject,theAttributes,in3,in4,in5)
%Invoked by RTI to tell observer new updates of subscribed object instances 
 
if (nargin == 6)
	% treatment of reflectAttrVal(6 input param) is not needed;
	% instead default implementation
	theTime   = in3;
	theTag    = in4;
	theHandle = in5;
	clear in3 in4 in5

	cmd='';
 
	while ~strcmp(cmd,'return')
        	cmd = input('(observ) reflectAttrVal(5 arg var)>> ','s');
        	eval(cmd,'error_handler')
	end
 
else
	% reflectAttrVal(3 input param) is used and implemented here
	theTag    = in3;
	clear in3

	global Cars Peds

	%DEBUG
	disp(['reflect ' theTag])
	global LOG
	fprintf(LOG,'%s\n',['reflect ' theTag]);
	
	idxc = find( Cars.Hhdl(:) == theObject );
	idxp = find( Peds.Hhdl(:) == theObject );
	if     ~isempty(idxc)
		new_pos = net2double(theAttributes{2});
		observ('car_move', idxc, new_pos)
	elseif ~isempty(idxp)
		new_pos = net2double(theAttributes{2});
		observ('ped_move', idxp, new_pos)
	else
		error('reflectAttrVal failed: unknown Object')
	end
end

return
 
 
%---------------------------------------------------------------
function car_move(idx,new_pos)
%Moves cars
 
global Cars
 
	delta_x       = new_pos - Cars.Pos(idx);
	Cars.Pos(idx) = Cars.Pos(idx) + delta_x;

	observ('car_move_anim', Cars.Ghdl(idx), delta_x )
return


%---------------------------------------------------------------
function car_move_anim(ghdl,delta_x)
%Animates moving cars
 
	set(ghdl,'XDATA', ...
	    get(ghdl,'XDATA') ...
	    + delta_x )
 
	text_ghdl = get(ghdl,'UserData');
	pos       = get(text_ghdl,'Position');
	pos(1)    = pos(1) + delta_x;
	set(text_ghdl,'Position',pos);

	drawnow
return
 
 
%---------------------------------------------------------------
function ped_move(idx,new_pos)
%Moves peds
 
global Peds
 
	delta_y       = new_pos - Peds.Pos(idx);
	Peds.Pos(idx) = Peds.Pos(idx) + delta_y;

	observ('ped_move_anim', Peds.Ghdl(idx), delta_y )
return


%---------------------------------------------------------------
function ped_move_anim(ghdl,delta_y)
%Animates moving peds
 
	set(ghdl,'YDATA', ...
	    get(ghdl,'YDATA') ...
	    + delta_y )
 
	text_ghdl = get(ghdl,'UserData');
	pos       = get(text_ghdl,'Position');
	pos(2)    = pos(2) + delta_y;
	set(text_ghdl,'Position',pos);

	drawnow
return
 
 
%---------------------------------------------------------------
function removeObjInst(theObject,in2,in3,in4)
%Invoked by RTI to tell observer that subscribed obj. inst. not longer exist
 
if (nargin == 5)
	% treatment of removeObjInst(5 input param) is not needed;
	% instead default implementation
	theTime	  = in2;
	theTag	  = in3;
	theHandle = in4;
	clear in2 in3 in4

	cmd='';
 
	while ~strcmp(cmd,'return')
        	cmd = input('(observ) removeObjInst(4 arg var)>> ','s');
        	eval(cmd,'error_handler')
	end

else
	% removeObjInst(2 input param) is used and implemented here
	theTag	  = in2;
	clear in2

	global Cars Peds

	%DEBUG
	disp(['remove ' theTag]);
	global LOG
	fprintf(LOG,'%s\n',['remove ' theTag]);
	
	idxc = find( Cars.Hhdl(:) == theObject);
	idxp = find( Peds.Hhdl(:) == theObject);
	if     ~isempty(idxc)
		observ('car_destroy', idxc)
	elseif ~isempty(idxp)
		observ('ped_destroy', idxp)
	else
		error('removeObjInst failed: unknown Object')
	end
end

return
 
 
%---------------------------------------------------------------
function car_destroy(idx)
%Destroys cars
 
global Cars
 
	observ('car_destroy_anim', Cars.Ghdl(idx))
 
	Cars.Lhdl = Cars.Lhdl([1:idx-1,idx+1:end]);
	Cars.Pos  = Cars.Pos([1:idx-1,idx+1:end]);
	Cars.Ghdl = Cars.Ghdl([1:idx-1,idx+1:end]);
	Cars.Hhdl = Cars.Hhdl([1:idx-1,idx+1:end]);
return


%---------------------------------------------------------------
function car_destroy_anim(ghdl)
%Animates destroying cars
 
        text_ghdl = get(ghdl,'UserData');
 
        delete(text_ghdl)
 
        delete(ghdl)

	drawnow
return
 
 
%---------------------------------------------------------------
function ped_destroy(idx)
%Destroys peds
 
global Peds

	observ('ped_destroy_anim', Peds.Ghdl(idx))
 
	Peds.Lhdl = Peds.Lhdl([1:idx-1,idx+1:end]);
	Peds.Pos  = Peds.Pos([1:idx-1,idx+1:end]);
	Peds.Ghdl = Peds.Ghdl([1:idx-1,idx+1:end]);
	Peds.Hhdl = Peds.Hhdl([1:idx-1,idx+1:end]);
return


%---------------------------------------------------------------
function ped_destroy_anim(ghdl)
%Animates destroying peds
 
        text_ghdl = get(ghdl,'UserData');
 
        delete(text_ghdl)
 
        delete(ghdl)

	drawnow
return
 
 
%---------------------------------------------------------------
function receiveInteract(theInteraction,theParameters,in3,in4,in5)
%Invoked by RTI to tell the federate interactions sent from remote federates.
 
if (nargin == 5)
	% treatment of receiveInteract(5 input args) method;
	theTime	  = in3;
	theTag	  = in4;
	theHandle = in5;
	clear in3 in4 in5

	error('receiveInteract(5 input args) not implemented')
 
else
	% treatment of receiveInteract(3 input args) method;
	theTag	  = in3;
	clear in3

	global HLA

	%DEBUG
	disp(['receive ' theTag])
	global LOG
	fprintf(LOG,'%s\n',['receive ' theTag]);

        if theInteraction == HLA.RttTrl
                event = net2char(theParameters{2});
		switch event
			case abs('c'), observ('trl_car_green')
			case abs('p'), observ('trl_ped_green')
			otherwise,     error('Unknown event.')
        	end


        else
                error('receiveInteract failed: unknown InteractClass')
        end

end

return
 
 
%------------------------------------------------------------------
function trl_car_green
%Switch traffic ligth to car green

	observ('trl_car_green_anim')
return


%-------------------------------------------------------------
function trl_car_green_anim
%Animates switching traffic ligth to car green

	plot( 9.0,9.3,'g*')		% green for cars
	plot(10.7,9.0,'r*')		% red for peds

        drawnow
return


%------------------------------------------------------------------
function trl_ped_green
%Switch traffic ligth to ped green

	observ('trl_ped_green_anim')
return


%---------------------------------------------------------------
function trl_ped_green_anim
%Animates switching traffic ligth to ped green

	plot( 9.0,9.3,'r*')		% red for cars
	plot(10.7,9.0,'g*')		% green for peds

	drawnow
return


%---------------------------------------------------------------
function loop
%Observer loop

global T

	while ~T.Terminate		% loop

		tick(0.3,0.3);		% take over control to RTI

		drawnow			% process anim. callbacks
	end
return


%---------------------------------------------------------------
function hla_leave
%Leave HLA

	% Resign federation
	e = resignFedExec;
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


