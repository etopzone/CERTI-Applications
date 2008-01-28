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
program processFed 
!
use constantsMod
use rtiSrvMod
use fedSrvMod
!
implicit none
!
character (len=*), parameter :: fedExecName = "TwoLevelController" 
character (len=*), parameter :: FED    = "F90TwoLevelController.fed" 
character (len=*), parameter :: fedName = "ProcessFed" 
!
integer :: err = 0
integer :: fedHdl = 0
integer :: twoLevelContrID = 0
integer :: ObjInstHdl = 0
integer :: uID = 0, yID = 0
integer :: objID_u = 0
!
real (kind=8), parameter :: t0 = 0, tend = 30 
real (kind=8), parameter :: h = 0.01, T1 = 5, k = 1
real (kind=8) :: t, u, y, ydot
logical :: bool = .false.
!
!
! init 
!
call rtiOn();
call createFedExec(fedExecName, FED, err);
call joinFedExec(fedName, fedExecName, ret=fedHdl);
write(*,*) "joinFedExec"
!
call getObjClassHdl("TwoLevelControllerClass", twoLevelContrID);
call getAttrHdl(twoLevelContrID, "U", ret=uID)
call getAttrHdl(twoLevelContrID, "Y", ret=yID)
call subscribeObjClassAttr(twoLevelContrID,(/uID/))
call publishObjClass(twoLevelContrID, (/yID/))
!
call registerObjInst(twoLevelContrID, ret=objID_u);
!
write(*,*) "Wait for u ..."
do
  if (DISOBJINST_.eqv..true.) then
    DISOBJINST_ = .false.
    exit
  end if
  call tick_1(bool)
end do
!
! simulation loop
t = t0
y = 0 
u = 0
do
  if ( t-tend.gt.0 ) then 
    exit
  end if

  do
    if (REFLATTRVAL_.eqv..true.) then
      REFLATTRVAL_ = .false.
      u = Uglobal
      exit
    end if
    call tick_1(bool) 
  end do

  ydot = (-y + u*k) / T1
  y = y + h*ydot 

  call updateAttrVals(objID_u, yID, (/ y /), "y")

  t = t + h
  write (*, fmt = "(A,F8.3)") "t= ", t
  write (*, fmt = "(A,F8.3)") "y= ", y
  write (*, fmt = "(A,F8.3)") "u= ", u
end do
!
! deinit 
!
call resignFedExec(DELETE_OBJECTS_AND_RELEASE_ATTRIBUTES)
call destroyFedExec(fedExecName);
!
end program processFed 
