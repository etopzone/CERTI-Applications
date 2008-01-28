!    Copyright (C) 2008 Christian Stenzel
!
!    This file is part of F90HLA13.
!
!    F90HLA13 is free software: you can redistribute it and/or modify
!    it under the terms of the GNU Lesser General Public License as 
!    published by the Free Software Foundation, either version 3 of 
!    the License, or (at your option) any later version.
!
!    F90HLA13 is distributed in the hope that it will be useful,
!    but WITHOUT ANY WARRANTY; without even the implied warranty of
!    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!    GNU Lesser General Public License for more details.
!
!    You should have received a copy of the GNU Lesser General Public 
!    License along with F90HLA13.  
!    
!    If not, see <http://www.gnu.org/licenses/>.
!
module constantsMod
!
implicit none
!
! Resign Actions
!
integer, parameter :: RELEASE_ATTRIBUTES                                = 1
integer, parameter :: DELETE_OBJECTS                                    = 2
integer, parameter :: DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES             = 3
integer, parameter :: NO_ACTION                                         = 4
!
!
! Exceptions
!
integer, parameter :: ArrayIndexOutOfBounds             		=  -1
integer, parameter :: AsynchronousDeliveryAlreadyDisabled		=  -2
integer, parameter :: AsynchronousDeliveryAlreadyEnabled		=  -3
integer, parameter :: AttributeAcquisitionWasNotRequested		=  -4
integer, parameter :: AttributeAcquisitionWasNotCanceled		=  -5
integer, parameter :: AttributeAlreadyBeingAcquired             	=  -6	
integer, parameter :: AttributeAlreadyBeingDivested             	=  -7	
integer, parameter :: AttributeAlreadyOwned                          	=  -8	
integer, parameter :: AttributeDivestitureWasNotRequested               =  -9	
integer, parameter :: AttributeNotDefined				= -10
integer, parameter :: AttributeNotKnown 				= -11 
integer, parameter :: AttributeNotOwned					= -12
integer, parameter :: AttributeNotPublished				= -13
integer, parameter :: ConcurrentAccessAttempted                         = -14
integer, parameter :: CouldNotDiscover					= -15
integer, parameter :: CouldNotOpenFED					= -16
integer, parameter :: CouldNotRestore     				= -17
integer, parameter :: DeletePrivilegeNotHeld				= -18
integer, parameter :: DimensionNotDefined				= -19
integer, parameter :: EnableTimeConstrainedPending                      = -20
integer, parameter :: EnableTimeConstrainedWasNotPending                = -21
integer, parameter :: EnableTimeRegulationPending                       = -22
integer, parameter :: EnableTimeRegulationWasNotPending                 = -23
integer, parameter :: ErrorReadingFED                                   = -24
integer, parameter :: EventNotKnown                                     = -25
integer, parameter :: FederateAlreadyExecutionMember			= -26
integer, parameter :: FederateInternalError				= -27
integer, parameter :: FederateLoggingServiceCalls                       = -28
integer, parameter :: FederateNotExecutionMember			= -29
integer, parameter :: FederateOwnsAttributes				= -30
integer, parameter :: FederateWasNotAskedToReleaseAttribute             = -31	
integer, parameter :: FederatesCurrentlyJoined				= -32
integer, parameter :: FederationExecutionAlreadyExists			= -33
integer, parameter :: FederationExecutionDoesNotExist			= -34
integer, parameter :: FederationTimeAlreadyPassed			= -35
integer, parameter :: HandleValuePairMaximumExceeded			= -36
integer, parameter :: InteractionClassNotDefined			= -37
integer, parameter :: InteractionClassNotKnown  			= -38
integer, parameter :: InteractionClassNotPublished			= -39
integer, parameter :: InteractionClassNotSubscribed  			= -40
integer, parameter :: InteractionParameterNotDefined			= -41
integer, parameter :: InteractionParameterNotKnown			= -42
integer, parameter :: InvalidExtents    				= -43
integer, parameter :: InvalidFederationTime				= -44
integer, parameter :: InvalidHandleValuePairSetContext          	= -45
integer, parameter :: InvalidLookahead					= -46
integer, parameter :: InvalidOrderingHandle				= -47
integer, parameter :: InvalidRegionContext				= -48
integer, parameter :: InvalidResignAction                               = -49
integer, parameter :: InvalidRetractionHandle				= -50
integer, parameter :: InvalidTransportationHandle			= -51
integer, parameter :: MemoryExhausted           			= -52
integer, parameter :: NameNotFound                     			= -53
integer, parameter :: ObjectClassNotDefined				= -54
integer, parameter :: ObjectClassNotKnown				= -55
integer, parameter :: ObjectClassNotPublished				= -56
integer, parameter :: ObjectClassNotSubscribed				= -57
integer, parameter :: ObjectNotKnown	        			= -58
integer, parameter :: ObjectAlreadyRegistered        			= -59
integer, parameter :: OwnershipAcquisitionPending			= -60
integer, parameter :: RegionInUse					= -61
integer, parameter :: RegionNotKnown					= -62
integer, parameter :: RestoreInProgress					= -63
integer, parameter :: RestoreNotRequested				= -64
integer, parameter :: RTIinternalError					= -65
integer, parameter :: SpaceNotDefined					= -66
integer, parameter :: SaveInProgress					= -67
integer, parameter :: SaveNotInitiated					= -68
integer, parameter :: SpecifiedSaveLabelDoesNotExist			= -69
integer, parameter :: SynchronizationPointLabelNotAnnounced		= -70
integer, parameter :: TimeAdvanceAlreadyInProgress              	= -71
integer, parameter :: TimeAdvanceWasNotInProgress			= -72
integer, parameter :: TimeConstrainedAlreadyEnabled			= -73
integer, parameter :: TimeConstrainedWasNotEnabled			= -74
integer, parameter :: TimeRegulationAlreadyEnabled			= -75
integer, parameter :: TimeRegulationWasNotEnabled			= -76
integer, parameter :: UnableToPerformSave				= -77
integer, parameter :: ValueCountExceeded				= -78
integer, parameter :: ValueLengthExceeded				= -79
!
end module constantsMod
