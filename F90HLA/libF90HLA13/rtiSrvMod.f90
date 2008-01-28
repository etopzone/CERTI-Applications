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
module rtiSrvMod
!
use constantsMod
!
implicit none
!
contains
!
!
! library specific function 
!
  subroutine rtiOn(err)

    implicit none
    
    integer, intent(out), optional :: err

    integer :: wrapErr = 0
    
    write(*,*) "Call rtion()"
    call rtion1(wrapErr)
    write(*,*) "rtion() called"
    
    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine rtiOn
!
!
  subroutine rtiOff(err)

    implicit none

    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call rtioff1(wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error ocured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine rtiOff
!
!
! 4. federation management
!
! 4.2
  subroutine createFedExec(fedExecName, FED, err)
    
    implicit none
    
    character (len=*), intent(in) :: fedExecName, FED
    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call createFedExec5(fedExecName, len(fedExecName), &
                        FED, len(FED), wrapErr)

    if (present(err)) then
      err = wrapErr
      err = 0
    else
      if (wrapErr.eq.FederationExecutionAlreadyExists) then
        write(*,*) "FederationExecutionAlreadyExists"
      elseif (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine createFedExec
!
! 4.3
  subroutine destroyFedExec(fedExecName, err)
    
    implicit none
    
    character (len=*), intent(in) :: fedExecName
    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call destroyFedExec3(fedExecName, len(fedExecName), wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.eq.FederationExecutionDoesNotExist) then
        write(*,*) "FederationExecutionDoesNotExist"
      elseif (wrapErr.eq.FederatesCurrentlyJoined) then
        write(*,*) "FederatesCurrentlyJoined"
      elseif (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine destroyFedExec
!
! 4.4
  subroutine joinFedExec(fedName, fedExecName, ret, err)

    implicit none

    character (len=*), intent(in) :: fedName, fedExecName
    integer, intent(out) :: ret 
    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call joinFedExec6(fedName, len(fedName), fedExecName, len(fedExecName), &
                      ret, wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

    end subroutine joinFedExec
!
! 4.5
  subroutine resignFedExec(resignAction, err)

    implicit none
    
    integer, intent(in) :: resignAction
    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call resignFedExec2(resignAction, wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine resignFedExec 
!
! 4.6
  subroutine registerFedSyncPoint(label, tag, fedHdlSet, err)

    implicit none 

    character (len=*), intent(in) :: label, tag
    integer, dimension(:), intent(in), optional :: fedHdlSet
    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    if (present(fedHdlSet)) then
      call registerFedSyncPoint7(label, len(label), tag, len(tag), &
                                 fedHdlSet, size(fedHdlSet), wrapErr)
    else
      call registerFedSyncPoint5(label, len(label), tag, len(tag), &
                                 wrapErr)
    end if

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine registerFedSyncPoint
!
! 4.9 
  subroutine syncPointAchieved(label, err)

    implicit none

    character (len=*), intent(in) :: label
    integer, intent(out), optional :: err
    
    integer :: wrapErr = 0
   
    call syncPointAchieved3(label, len(label), wrapErr) 

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine syncPointAchieved
!
!
! 4.11
  subroutine requestFedSave(label, time, err)

    implicit none

    character (len=*), intent(in) :: label
    real (kind=8), intent(in), optional :: time
    integer, intent(out), optional :: err
    
    integer :: wrapErr = 0

    if (present(time)) then
      call requestFedSave4(label, len(label), time, wrapErr)
    else
      call requestFedSave3(label, len(label), wrapErr)
    end if

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine requestFedSave
!
!
! 4.13
  subroutine fedSaveBegun(err)
    
    implicit none

    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call fedSaveBegun1(wrapErr)
    
    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine fedSaveBegun
!
!
! 4.14
  subroutine fedSaveComplete(err)
    implicit none

    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call fedSaveComplete1(wrapErr)
    
    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine fedSaveComplete
!
!
! 4.14
  subroutine fedSaveNotComplete(err)
    implicit none

    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call fedSaveNotComplete1(wrapErr)
    
    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine fedSaveNotComplete
!
!
! 4.16
  subroutine requestFedRestore(label, err)

    implicit none

    character (len=*), intent(in) :: label
    integer, intent(out), optional :: err
    
    integer :: wrapErr = 0
   
    call requestFedRestore3(label, len(label), wrapErr) 

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine requestFedRestore 
!
!
! 4.20
  subroutine fedRestoreComplete(err)
    implicit none

    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call fedRestoreComplete1(wrapErr)
    
    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine fedRestoreComplete
!
!
! 4.20
  subroutine fedRestoreNotComplete(err)

    implicit none

    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call fedRestoreNotComplete1(wrapErr)
    
    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine fedRestoreNotComplete
!
!
! 5. Declaration Management
!
!
! 5.2
  subroutine publishObjClass(objClassHdl, attrList, err)

    implicit none

    integer, intent(in) :: objClassHdl
    integer, dimension(:), intent(in) :: attrList
    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call publishObjClass4(objClassHdl, attrList, size(attrList), wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine publishObjClass
!
! 5.6
  subroutine subscribeObjClassAttr(objClassHdl, attrList, err)

    implicit none

    integer, intent(in) :: objClassHdl
    integer, dimension(:), intent(in) :: attrList
    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call subscribeObjClassAttr4(objClassHdl, attrList, size(attrList), wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine subscribeObjClassAttr
!
!
! 6. Object Management
!
!
! 6.4
  subroutine registerObjInst(objClassHdl, theObjInstName, ret, err)
    
    implicit none
    
    integer, intent(in) :: objClassHdl    
    character (len=*), intent(in), optional :: theObjInstName
    integer, intent(out) :: ret 
    integer, intent(out), optional :: err

    integer :: wrapErr = 0
    
    if (present(theObjInstName)) then
      call registerObjInst5(objClassHdl, theObjInstName, len(TheObjInstName), &
                            ret, wrapErr)
    else
      call registerObjInst3(objClassHdl, ret, wrapErr)
    end if

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if
  
  end subroutine registerObjInst
!
!
! 6.6
  subroutine updateAttrVals(objHdl, attrHdl, attrVals, tag, &
                            logTime, ret1, ret2, err) 
    
    implicit none

    integer, intent(in) :: objHdl
    integer, intent(in) :: attrHdl
    real (kind=8), dimension(:), intent(in) :: attrVals
    character (len=*), intent(in) :: tag
    real (kind=8), intent(in), optional :: logTime 
    integer, intent(out), optional :: ret1
    integer, intent(out), optional :: ret2
    integer, intent(out), optional :: err
    
    integer :: wrapErr = 0
    integer :: wrapRet1 = 0
    integer :: wrapRet2 = 0
   
!    write(*,*) objInstHdl
!    write(*,*) attrHdl
!    write(*,*) attrVals 
!    write(*,*) userSuppliedTag
!    write(*,*) logTime 

    if (present(logTime)) then
      call updateAttrVals10(objHdl, attrHdl, attrVals, size(attrVals), &
                            tag, len(tag), logTime, &
                            wrapRet1, wrapRet2, wrapErr) 
    else
      call updateAttrVals7(objHdl, attrHdl, attrVals, size(attrVals), &
                           tag, len(tag), wrapErr) 
    end if

    if (present(ret1)) then
      ret1 = wrapRet1;
    end if

    if (present(ret2)) then
      ret1 = wrapRet2;
    end if

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine updateAttrVals
!
!
! 8. Time Management
!
!
! 8.2
  subroutine enableTimeReg(fedTime, lookAhead, err) 
    
    implicit none

    real (kind=8), intent(in) :: fedTime 
    real (kind=8), intent(in) :: lookAhead
    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call enableTimeReg3(fedTime, lookAhead, wrapErr)
    
    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine enableTimeReg
!
! 8.5
  subroutine enableTimeCons(err) 
    
    implicit none

    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call enableTimeCons1(wrapErr)
    
    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine enableTimeCons
!
! 8.8
  subroutine timeAdvReq(time, err) 

    implicit none

    real (kind=8), intent(in) :: time
    integer, intent(out), optional :: err 
    
    integer :: wrapErr = 0
    
    call timeAdvReq2(time, wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine timeAdvReq
!
!
! 8.14
  subroutine enableAsyncDeliv(err)
    
    implicit none

    integer, intent(out), optional :: err 
    
    integer :: wrapErr = 0
    
    call enableAsyncDeliv1(wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine enableAsyncDeliv
    
!
! 10. Ancillary Services
!
! 
  subroutine tick_1(ret, err)
    
    implicit none
    
    logical, intent(out) :: ret 
    integer, intent(out), optional :: err

    integer :: wrapErr = 0
        
    call tick2(ret, wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine tick_1

  subroutine tick_2(minTick, maxTick, ret, err)

    implicit none
    
    real (kind=8), intent(in) :: minTick
    real (kind=8), intent(in) :: maxTick
    
    logical, intent(out) :: ret 
    integer, intent(out), optional :: err

    integer :: wrapErr = 0
        
    call tick4(minTick, maxTick, ret, wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

  end subroutine tick_2
!
!
! 10.2
  subroutine getObjClassHdl(theName, ret, err)
    
    implicit none

    character (len=*), intent(in) :: theName
    integer, intent(out) :: ret
    integer, intent(out), optional :: err

    integer :: wrapErr = 0

    call getObjClassHdl4(theName, len(theName), ret, wrapErr)

    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

    end subroutine getObjClassHdl
!
!
!
! 10.4
  subroutine getAttrHdl(objClassHdl, theAttrName, ret, err)
    
    implicit none

    integer, intent(in) :: objClassHdl 
    character (len=*), intent(in) :: theAttrName
    integer, intent(out) :: ret
    integer, intent(out), optional :: err
    
    integer :: wrapErr = 0

    call getAttrHdl5(objClassHdl, theAttrName, len(theAttrName), ret, wrapErr)
   
    if (present(err)) then
      err = wrapErr
    else
      if (wrapErr.lt.0) then
        write(*,*) "Error occured. Errorcode = ", wrapErr
        stop "Terminating."
      end if
    end if

    end subroutine getAttrHdl
!
end module rtiSrvMod
