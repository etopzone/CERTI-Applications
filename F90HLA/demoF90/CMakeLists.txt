#    Copyright (C) 2008 Christian Stenzel
#
#    This file is part of F90HLA13.
#
#    F90HLA13 is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as 
#    published by the Free Software Foundation, either version 3 of 
#    the License, or (at your option) any later version.
#
#    F90HLA13 is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public 
#    License along with F90HLA13.  
#    
#    If not, see <http://www.gnu.org/licenses/>.
#
#########################################################################
# CMakeLists.txt for the fortran demo programs 
#########################################################################
PROJECT(Demo_Fortran_Wrapper Fortran)

# CERTI requires CMake 2.4.x
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.4)

EXEC_PROGRAM(rm ARGS constantsMod.mod.proxy
		     fedSrvMod.mod.proxy
		     rtiSrvMod.mod.proxy)

EXEC_PROGRAM(touch ARGS constantsMod.mod.proxy
			fedSrvMod.mod.proxy
			rtiSrvMod.mod.proxy)

FIND_PATH(INC modF90)
INCLUDE_DIRECTORIES(${INC}/modF90)
FIND_LIBRARY(F90_LIB NAMES F90HLA13Modules)

###################################
# Setup Control Federate
###################################
SET(EXE_NAME F90controllerFederate)
SET(SRC_LIST TwoLevelController/controllerFederate/fedSrvMod.f90
    TwoLevelController/controllerFederate/controllerFederate_woTime.f90)

###################################
# Generic CMake library target
###################################
ADD_EXECUTABLE(${EXE_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${F90_LIB})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)

###################################
# Setup Control Federate with time
###################################
SET(EXE_NAME F90TMcontrollerFederate)
SET(SRC_LIST TwoLevelController/controllerFederate/fedSrvMod.f90
    TwoLevelController/controllerFederate/controllerFederate_wTime.f90)

###################################
# Generic CMake library target
###################################
ADD_EXECUTABLE(${EXE_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${F90_LIB})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)


###################################
# Setup Process Federate
###################################
SET(EXE_NAME F90processFederate)
SET(SRC_LIST TwoLevelController/processFederate/fedSrvMod.f90 
    TwoLevelController/processFederate/processFederate_woTime.f90)

###################################
# Generic CMake library target
###################################
ADD_EXECUTABLE(${EXE_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${F90_LIB})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)

###################################
# Setup Process Federate with time
###################################
SET(EXE_NAME F90TMprocessFederate)
SET(SRC_LIST TwoLevelController/processFederate/fedSrvMod.f90 
    TwoLevelController/processFederate/processFederate_wTime.f90)

###################################
# Generic CMake library target
###################################
ADD_EXECUTABLE(${EXE_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${F90_LIB})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)

INSTALL(FILES  
  TwoLevelController/F90TwoLevelController.fed
  DESTINATION share/federations)


MESSAGE(STATUS "*** ${CMAKE_PROJECT_NAME} has been successfully configured ***")
