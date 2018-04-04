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

# You should uncomment the following line
# if you want VERBOSE build
#SET(CMAKE_VERBOSE_MAKEFILE OFF)

#EXEC_PROGRAM(rm ARGS constantsMod.mod.proxy
#		     fedSrvMod.mod.proxy
#		     rtiSrvMod.mod.proxy)

#EXEC_PROGRAM(touch ARGS constantsMod.mod.proxy
#			fedSrvMod.mod.proxy
#			rtiSrvMod.mod.proxy)

# Here you have to make sure that the paths with libF90HLA13 are corrects
# for me it is: /home/j.chaudron/Dev/CERTI_APPS/applications/F90HLA/libF90HLA13_install/include
# then before execution 
# export LD_LIBRARY_PATH=/home/j.chaudron/Dev/CERTI_APPS/applications/F90HLA/libF90HLA13_install/lib:${LD_LIBRARY_PATH}
#IF (!F90HLA_DIR)
SET(F90HLA_DIR /home/j.chaudron/Dev/CERTI_APPS/applications/F90HLA/libF90HLA13_install/)
#ENDIF (!F90HLA_DIR)
INCLUDE_DIRECTORIES(${F90HLA_DIR}/include/modF90)
INCLUDE_DIRECTORIES(${F90HLA_DIR}/lib)
# Here you have to make sure that the paths with libF90HLA13 are corrects
FIND_LIBRARY(F90HLA_LIB libF90HLA13Modules.so ${F90HLA_DIR}/lib)
MESSAGE(STATUS "F90HLA_LIB : ${F90HLA_LIB}")
FIND_LIBRARY(F90CERTIWRAP_LIB libF90HLA13CERTIWrap.so ${F90HLA_DIR}/lib)
MESSAGE(STATUS "F90CERTIWRAP_LIB : ${F90CERTIWRAP_LIB}")

###################################
# Setup Control Federate
###################################
SET(EXE_NAME F90controllerFederate)
SET(SRC_LIST ${CMAKE_SOURCE_DIR}/TwoLevelController/controllerFederate/fedSrvMod.f90
    ${CMAKE_SOURCE_DIR}/TwoLevelController/controllerFederate/controllerFederate_woTime.f90)

###################################
# Generic CMake library target
###################################
ADD_EXECUTABLE(${EXE_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${F90HLA_LIB} ${F90CERTIWRAP_LIB})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)

###################################
# Setup Control Federate with time
###################################
SET(EXE_NAME F90TMcontrollerFederate)
SET(SRC_LIST ${CMAKE_SOURCE_DIR}/TwoLevelController/controllerFederate/fedSrvMod.f90
    ${CMAKE_SOURCE_DIR}/TwoLevelController/controllerFederate/controllerFederate_wTime.f90)

###################################
# Generic CMake library target
###################################
ADD_EXECUTABLE(${EXE_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${F90HLA_LIB} ${F90CERTIWRAP_LIB})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)


###################################
# Setup Process Federate
###################################
SET(EXE_NAME F90processFederate)
SET(SRC_LIST ${CMAKE_SOURCE_DIR}/TwoLevelController/processFederate/fedSrvMod.f90 
    ${CMAKE_SOURCE_DIR}/TwoLevelController/processFederate/processFederate_woTime.f90)

###################################
# Generic CMake library target
###################################
ADD_EXECUTABLE(${EXE_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${F90HLA_LIB} ${F90CERTIWRAP_LIB})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)

###################################
# Setup Process Federate with time
###################################
SET(EXE_NAME F90TMprocessFederate)
SET(SRC_LIST ${CMAKE_SOURCE_DIR}/TwoLevelController/processFederate/fedSrvMod.f90 
    ${CMAKE_SOURCE_DIR}/TwoLevelController/processFederate/processFederate_wTime.f90)

###################################
# Generic CMake library target
###################################
ADD_EXECUTABLE(${EXE_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${F90HLA_LIB} ${F90CERTIWRAP_LIB})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)

INSTALL(FILES  
  ${CMAKE_SOURCE_DIR}/TwoLevelController/F90TwoLevelController.fed
  DESTINATION share/federations)


MESSAGE(STATUS "*** ${CMAKE_PROJECT_NAME} has been successfully configured ***")
