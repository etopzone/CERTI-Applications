!    Copyright (C) 2008 Christian Stenzel
!
!    This file is part of F90HLA13.
!
!    F90HLA13 is free software: you can redistribute it and/or modify
!    it under the terms of the GNU General Public License as published 
!    by the Free Software Foundation, either version 3 of the License, 
!    or (at your option) any later version.
!
!    F90HLA13 is distributed in the hope that it will be useful,
!    but WITHOUT ANY WARRANTY; without even the implied warranty of
!    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!    GNU General Public License for more details.
!
!    You should have received a copy of the GNU General Public 
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
real (kind=8) :: Yglobal=0
!
! federate service indication flags
!
logical :: DISOBJINST_ = .false.        ! 6.5
logical :: REFLATTRVAL_ = .false.       ! 6.7
logical :: TIMEADVGRA_ = .false.        ! 8.13
!
contains
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

    ! user implementation

    write(*,*) "discoverObjInst; objHdl = ", objHdl
    DISOBJINST_ = .true.

    err = 0

  end subroutine discoverObjInst
!
! 6.7
  subroutine reflectAttrVals(objHdl, &
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

    ! user implementation
    REFLATTRVAL_ = .true.
    Yglobal = attrVals(1)

    err = 0

  end subroutine reflectAttrVals
!
! time management
!
! 8.13
  subroutine timeAdvGrant(logTime, &
                          err)

    implicit none

    real (kind=8), intent(in) :: logTime 
    integer, intent(inout) :: err

    ! user implementation
    TIMEADVGRA_ = .true.

    err = 0
  
  end subroutine timeAdvGrant
!
end module fedSrvMod
