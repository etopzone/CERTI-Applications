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
module fedSrvMod
!
use constantsMod
!
implicit none
!
! placeholder for shared variables
!
! federate service indication flags
!
logical :: SYNCPOINTREGSUCC_ = .false.  ! 4.7
logical :: SYNCPOINTREGFAIL_ = .false.  ! 4.7
logical :: ANNOUNCESYNCPOINT_ = .false. ! 4.8
logical :: FEDSYNC_ = .false.           ! 4.10
logical :: INITFEDSAVE_ = .false.       ! 4.12
logical :: FEDSAVED_ = .false.          ! 4.15
logical :: FEDNOTSAVED_ = .false.       ! 4.15
logical :: REQFEDRESSUCC_ = .false.     ! 4.17
logical :: REQFEDRESFAIL_ = .false.     ! 4.17
logical :: FEDRESBEGUN_ = .false.       ! 4.18
logical :: INITFEDRES_ = .false.        ! 4.19
logical :: FEDRESTORED_ = .false.       ! 4.21
logical :: FEDNOTRESTORED_ = .false.    ! 4.21
logical :: DISOBJINST_ = .false.        ! 6.5
logical :: REFLATTRVAL_ = .false.       ! 6.7
logical :: REMOBJINST_ = .false.        ! 6.11
logical :: PROATTRVALUP_ = .false.      ! 6.18
logical :: ENTIMEREG_ = .false.         ! 8.3
logical :: ENTIMECONS_ = .false.        ! 8.6
logical :: TIMEADVGRA_ = .false.        ! 8.13
!
contains
!
! federation management
!
! 4.7
  subroutine syncPointRegSucceeded(label, lenLabel, err)
  
    implicit none

    integer, intent(in) :: lenLabel
    character (len=lenLabel), intent(in) :: label
    integer, intent(inout) :: err

    write(*,*) "syncPointRegSucceeded(label, err) not implemented"
    write(*,*) label
    
    err = 0

  end subroutine syncPointRegSucceeded 
!
  subroutine syncPointRegFailed(label, lenLabel, err)
!  
    implicit none
!
    integer, intent(in) :: lenLabel
    character (len=lenLabel), intent(in) :: label
    integer, intent(inout) :: err
!
    write(*,*) "syncPointRegFailed not implemented"
    write(*,*) label
!    
    err = 0

  end subroutine syncPointRegFailed
!
! 4.8
  subroutine announceSyncPoint(label, lenLabel, tag, lenTag, err)

    implicit none

    integer, intent(in) :: lenLabel 
    integer, intent(in) :: lenTag
    character (len=lenLabel), intent(in) :: label
    character (len=lenTag), intent(in) :: tag
    integer, intent(inout) :: err

    write(*,*) "announceSyncPoint not implemented"
    write(*,*) label
    write(*,*) tag 

    err = 0
  
  end subroutine announceSyncPoint
!
! 4.10
  subroutine fedSync(label, lenLabel, err)

    implicit none

    integer, intent(in) :: lenLabel
    character (len=lenLabel), intent(in) :: label
    integer, intent(inout) :: err

    write(*,*) "fedSync not implemented"
    write(*,*) label

    err = 0
  end subroutine fedSync
!
! 4.12
  subroutine initiateFedSave(label, lenLabel, err)

    implicit none

    integer, intent(in) :: lenLabel
    character (len=lenLabel), intent(in) :: label
    integer, intent(inout) :: err

    write(*,*) "initiateFedSave not implemented"
    write(*,*) label

    err = 0
  end subroutine initiateFedSave
!
! 4.15
  subroutine fedSaved(err)

    implicit none

    integer, intent(inout) :: err

    write(*,*) "fedSaved not implemented"

    err = 0
  end subroutine fedSaved
!
! 4.15
  subroutine fedNotSaved(err)

    implicit none

    integer, intent(inout) :: err

    write(*,*) "fedNotSaved not implemented"

    err = 0
  end subroutine fedNotSaved
!
! 4.17
  subroutine requestFedResSucceeded(label, lenLabel, err)

    implicit none

    integer, intent(in) :: lenLabel
    character (len=lenLabel), intent(in) :: label
    integer, intent(inout) :: err

    write(*,*) "requestFedResSucceeded not implemented"
    write(*,*) label

    err = 0
  end subroutine requestFedResSucceeded
!
! 4.17
  subroutine requestFedResFailed(label, lenLabel, &
                                 reason, lenReason, &
                                 err)
    implicit none

    integer, intent(in) :: lenLabel
    integer, intent(in) :: lenReason
    character (len=lenLabel), intent(in) :: label
    character (len=lenReason), intent(in) :: reason
    integer, intent(inout) :: err

    write(*,*) "requestFedRestoreFailed not implemented"
    write(*,*) label
    write(*,*) reason 

    err = 0
  end subroutine requestFedResFailed
!
! 4.18
  subroutine fedResBegun(err)

    implicit none

    integer, intent(inout) :: err

    write(*,*) "fedRestoreBegun not implemented"

    err = 0
  end subroutine fedResBegun
!
! 4.19
  subroutine initiateFedRestore(label, lenLabel, &
                                 fedHdl, &
                                 err)

    implicit none

    integer, intent(in) :: lenLabel
    character (len=lenLabel), intent(in) :: label
    integer, intent(in) :: fedHdl 
    integer, intent(inout) :: err

    write(*,*) "initiateFedRestore not implemented"
    write(*,*) label

    err = 0
  end subroutine initiateFedRestore
!
! 4.21
  subroutine fedRestored(err)

    implicit none

    integer, intent(inout) :: err

    write(*,*) "fedRestored not implemented"

    err = 0
  end subroutine fedRestored
!
! 4.21
  subroutine fedNotRestored(err)

    implicit none

    integer, intent(inout) :: err

    write(*,*) "fedNotRestored not implemented"

    err = 0
  end subroutine fedNotRestored
!
! object management
!
! 6.5
  subroutine discoverObjInst(objHdl, objClassHdl, &
                             objName, lenObjName, err)

    implicit none

    integer, intent(in) :: objHdl
    integer, intent(in) :: objClassHdl
    integer, intent(in) :: lenObjName
    character (len=lenObjName), intent(in) :: objName
    integer, intent(inout) :: err

    write(*,*) "discoverObjInst not implemented"
    write(*,*) objHdl
    write(*,*) objClassHdl
    err = 0

  end subroutine discoverObjInst
!
! 6.7
  subroutine reflectAttrVal(objHdl, &
                            attrHdl, attrVals, lenAttrVals, &
                            logTime, &
                            tag, lenTag, &
                            eventRetrHdlSerial, &
                            eventRetrHdlSendingFed, &
                            err)

    implicit none

    integer, intent(in) :: objHdl
    integer, intent(in) :: attrHdl
    integer, intent(in) :: lenAttrVals
    real (kind=8), dimension(lenAttrVals), intent(in) :: attrVals
    integer, intent(in) :: lenTag
    character (len=lenTag), intent(in) :: tag
    real (kind=8), intent(in), optional :: logTime
    integer, intent(in), optional :: eventRetrHdlSerial
    integer, intent(in), optional :: eventRetrHdlSendingFed
    integer, intent(inout) :: err

    write(*,*) "reflectAttrVal not implemented"
    write(*,*) objHdl
    write(*,*) attrHdl
    write(*,*) attrVals(:)
    write(*,*) tag

    err = 0

  end subroutine reflectAttrVal
!
! time management
!
! 8.13
  subroutine timeAdvGrant(logTime, &
                          err)

    implicit none

    real (kind=8), intent(in) :: logTime 
    integer, intent(inout) :: err

    write(*,*) "timeAdvGrant not implemented"
    write(*,*) logTime

    err = 0
  
  end subroutine timeAdvGrant
!
end module fedSrvMod
