%    Copyright (C) 2008 Christian Stenzel, Sven Pawletta, Thorsten Pawletta
%
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
%
% HLA Toolbox - Matlab/RTI/Matlab-Interface
%
%
% Data Conversions.
%   double2net			- Convert double to network representation.
%   net2double			- Convert network representation to double.
%   char2net			- Convert string to network representation.
%   net2char			- Convert network representation to string.
%
% Matlab/RTI/Matlab - Interface.
%   rti				- MEX-implementation of a bi-directional 
%				  Matlab <-> RTI interface.
%				  Federate services (application provided) are
%				  invoked via callbacks (rti -> Matlab appl.).
%				  RTI services are invoked via wrapper funktions
%				  (Matlab appl. -> wrapper fcn. -> rti).
%				  Do not call rti directly.
%
%
%   Interface Initialization/Deinitialization.
%
%   rtiOn			- Lock interface and create ambassadors.
%   rtiOff			- Unlock interface and delete ambassadors.
%
%
%   Wrapper to RTI Services (see RTI Programmer's Guide Appendix A,
%			     names of services have been shortened).
%
%   (A.1) Federation Management.
%   createFedExec		            - Create Federation Execution.
%   destroyFedExec		            - Destroy Federate Execution.
%   federateResComplete             - Federate Restore Complete.
%   federateResNotComplete          - Federate Restore Not Complete.
%   federateSaveBegun               - Federate Save Begun.
%   federateSaveComplete            - Federate Save Complete.
%   federateSaveNotComplete         - Federate Save Not Complete.
%   joinFedExec			            - Join Federation Execution.
%   registerFedSyncPoint            - Register Federation Synchronization Point.
%   requestFedRestore               - Request Federation Restore.
%   requestFedSave                  - Request Federation Save.
%   resignFedExec   		        - Resign Federation Execution.
%   synchronizationPointAchieved    - Synchronization Point Achieved.
%   
%   (A.2) Declaration Management.
%   publishInteractClass	        - Publish Interaction Class.
%   publishObjClass		            - Publish Object Class.
%   subscribeInteractClass	        - Subscribe Interaction Class.
%   subscribeObjClassAttr	        - Subscribe Object Class Attributes.
%   unpublishInteractClass          - Unpublish Interaction Class.
%   unpublishObjClass               - Unpublish Object Class.
%   unsubscribeInteractClass        - Unsubscribe Interaction Class.
%   unsubscribeObjClass 	        - Unsubscribe Object Class.
%
%   (A.3) Object Management.
%   changeAttrTransType             - Change Attribute Transportation Type.
%   chaneInteractTransType          - Change Interaction Transportation Type.
%   deleteObjInst		            - Delete Object Instance.
%   localDelObjInst                 - Local Delete Object Instance.
%   registerObjInst		            - Register Object Instance.
%   requestClassAttrValUp           - Request Class Attribute Value Update.
%   requestObjAttrValUp             - Request Object Attribute Value Update.
%   sendInteract		            - Send Interaction.
%   updateAttrVal		            - Update Attribute Values.
%
%   (A.5) Time Management.
%   changeAttrOrderType             - Change Attrbute Order Type.
%   changeInteractOrderType         - Change Interaction Order Type.
%   disableAsyncDeliv               - Disable Asynchronous Delivery.
%   disableTimeConstr               - Disable Time Constrained.
%   disableTimeReg                  - Disable Time Regulation.
%   enableAsyncDeliv		        - Enable Asynchronous Delivery.
%   enableTimeConstr		        - Enable Time Constrained.
%   enableTimeReg		            - Enable Time Regulation.
%   flushQueueReq                   - Flush Queue Request.
%   modifyLookahead                 - Modify Lookahead.
%   nextEventReq                    - Next Event Request.
%   nextEventReqAvail               - Next Event Request Available.
%   queryFedTime                    - Query Federate Time.
%   queryLBTS                       - Query LBTS.
%   queryLookahead		            - Query Lookahead.
%   queryMinMaxEventTime            - Query Min Max Event Time.
%   retract                         - Retract
%   timeAdvReq			            - Request advance in logical time of federate.
%   timeAdvReqAvail                 - Time Advance Request Availbale.
%
%   (A.7) Ancillary Services.
%   enableAttrRelAdvisSwitch	    - Enable Attr. Relev. Advisories.
%   getAttrHdl			            - Get Attribute Handle.
%   getInteractClassHdl		        - Get Interaction Class Handle.
%   getObjClassHdl		            - Get Object Class Handle.
%   getParamHdl			            - Get Parameter Handle.
%   tick			                - Turn Control Over to RTI.
%   getOrderingHdl                  - Get Ordering Handle.
%   getTransHdl                     - Get Transportation Handle.
%
%   NULL Federate Services (see RTI Programmer's Guide Appendix B,
%			    names of services have been shortened).
%
%   (B.1) Federation Management.
%   announceSyncPoint               - Announce Synchronization Point.
%   fedNotRestored                  - Federation Not Restored.
%   fedNotSaved                     - Federation Not Saved.
%   fedResBegun                     - Federation Restore Begun.
%   fedRestored                     - Federation Restored.
%   fedSaved                        - Federation Saved.
%   fedSync                         - Federation Synchronized.
%   initiateFedRestore              - Initiate Federate Restore.
%   initiateFedSave                 - Initiate Federate Save.
%   requestFedResFailed             - Request Federation Restore Failed.
%   requestFedResSuccess            - Request Federation Restore Succeeded.
%   syncPointRegFailed              - Synchronization Point Registration Failed.
%   syncPointRegSuccess             - Synchronization Point Registration Succeeded.
%
%   (B.2) Declaration Management.
%   startRegForObjClass		        - Start Registration For Object Class.
%   stopRegForObjClass		        - Stop Registration For Object Class.
%   turnInteractOn		            - Turn Interactions On.
%   turnInteractOff		            - Turn Interactions Off.
%
%   (B.3) Object Management.
%   attrInScope                     - Attributes In Scope.
%   attrOutOfScope                  - Attributes Out Of Scope.
%   discoverObjInst		            - Discover Object Instance.
%   receiveInteract		            - Receive Interactions.
%   reflectAttrVal		            - Reflect Attribute Values.
%   removeObjInst		            - Remove Object Instance.
%   provideAttrValUpdate	        - Provide Attribute Value Update.
%   turnUpdatesOffForObjInst        - Turn Updates Off For Object Instance.
%   turnUpdatesOnForObjInst	        - Turn Updates On For Object Instance.
%
%   (B.5) Time Management.
%   requestRetract                  - Request Retraction.
%   timeAdvGrant		            - Time Advance Grant.
%   timeConstrEnabled		        - Time Constrained Enabled.
%   timeRegEnabled		            - Time Regulation Enabled.
%
%   Wrapper to RTI Classes and Supporting Types (see RTI Prog. Guide App. C).
%
%   (C.1.5) FedTime.
%   getEpsilon			- Get default value for federate lookahead.

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


