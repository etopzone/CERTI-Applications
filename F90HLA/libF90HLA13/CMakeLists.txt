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
# CMakeLists.txt for library F90HLA13
#########################################################################
PROJECT(libF90HLA13 CXX Fortran)

#########################################################################
# Setup reasonable default CMake behavior
#########################################################################

# CERTI requires CMake 2.4.x
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.4)

# Use CERTI specific CMake modules first
# You should set CERTI_HOME env var to appropriate value
# or put here the full path to the directory
# containing FindCERTI.cmake
IF ("$ENV{CERTI_HOME}" STRGREATER "")
    FILE(TO_CMAKE_PATH "$ENV{CERTI_HOME}" CERTI_HOME_TEMP)
    SET(CERTI_HOME ${CERTI_HOME_TEMP} CACHE PATH "The CERTI install directory")
    LIST(APPEND CMAKE_MODULE_PATH "${CERTI_HOME}/share/scripts")

ELSE ("$ENV{CERTI_HOME}" STRGREATER "")
    LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/share/scripts")    
ENDIF ("$ENV{CERTI_HOME}" STRGREATER "")

# You should uncomment the following line
# if you want VERBOSE build
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# The preferred (default) behaviour is to
# build library as shared on all platform
OPTION(BUILD_SHARED
  "Build libraries as shared library" ON)
IF (BUILD_SHARED)
  SET(BUILD_SHARED_LIBS ON)
ELSE (BUILD_SHARED)
  SET(BUILD_SHARED_LIBS OFF)
ENDIF (BUILD_SHARED)

# Change the build type to release
# when you are ready to release 
SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")

#########################################################################
# Define your project packaging basics
#########################################################################
SET(PACKAGE_NAME      ${CMAKE_PROJECT_NAME})
SET(PACKAGE_VERSION_MAJOR "0")
SET(PACKAGE_VERSION_MINOR "1")
SET(PACKAGE_VERSION_PATCH "1")
SET(PACKAGE_VERSION   "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
SET(PACKAGE_BUGREPORT "christian.stenzel@hs-wismar.de")
MESSAGE(STATUS "Configuring ${PACKAGE_NAME} version ${PACKAGE_VERSION}")

#########################################################################
# FIND RTI (CERTI, MAK, Pitch, ...)
#########################################################################
FIND_PACKAGE(RTI REQUIRED)
INCLUDE_DIRECTORIES(${RTI_INCLUDE_DIR})
ADD_DEFINITIONS(${RTI_DEFINITIONS})

#########################################################################
# CPack specific Configuration
#########################################################################
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "F90-HLA1.3-CERTI-Wrapper")
SET(CPACK_PACKAGE_VENDOR "University Wismar, RG CEA, Christian Stenzel")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/F90_HLA13_CERTI_Wrapper_DESCRIPTION.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
SET(CPACK_SOURCE_IGNORE_FILES "/CVS/;/\\\\.svn/;\\\\.swp$;\\\\.#;/#;.*~;cscope.*")
SET(CPACK_SOURCE_GENERATOR "ZIP;TGZ")
SET(CPACK_GENERATOR "ZIP;TGZ")
SET(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
SET(CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION_PATCH})

INCLUDE(CPack)

#########################################################################
# Go to subdirs 
# (or process library and executable in the current directory) 
#########################################################################

###################################
# Setup F90-Certi-Wrapper
#   the library name
#   the source list
###################################
SET(LIB_NAME F90HLA13CERTIWrap)
SET(SRC_LIST F90HLA13CERTIWrap.cpp)

###################################
# Generic CMake library target
###################################
ADD_LIBRARY(${LIB_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${LIB_NAME} ${RTI_LIBRARIES})
INSTALL(TARGETS ${LIB_NAME} 
        LIBRARY DESTINATION lib)

###################################
# Setup Fortran libs
#   the library name
#   the source list
###################################

SET(LIB_NAME F90HLA13Modules)
SET(SRC_LIST constantsMod.f90 rtiSrvMod.f90 fedSrvMod.f90) 

ADD_LIBRARY(${LIB_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES(${LIB_NAME} F90HLA13CERTIWrap)
INSTALL(TARGETS ${LIB_NAME}
	LIBRARY DESTINATION lib)
INSTALL(FILES $ENV{PWD}/constantsmod.mod $ENV{PWD}/rtisrvmod.mod
        DESTINATION include/modF90)

MESSAGE(STATUS "*** ${CMAKE_PROJECT_NAME} has been successfully configured ***")
