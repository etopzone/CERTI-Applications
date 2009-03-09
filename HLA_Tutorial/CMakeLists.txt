#########################################################################
# CMakeLists.txt for the HLA_Tutorial project
#########################################################################
PROJECT(HLA_Tutorial CXX)

#########################################################################
# Setup reasonable default CMake behavior
#########################################################################

# CERTI requires CMake 2.4.x
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.4)

# Use CERTI specific CMake modules first
# You should set CERTI_HOME env var to appropriate value
# or put here the full path to the directory
# containing FindRTI.cmake
# Use CERTI_HOME env var if it is defined
IF ("$ENV{CERTI_HOME}" STRGREATER "")
    FILE(TO_CMAKE_PATH "$ENV{CERTI_HOME}" CERTI_HOME_TEMP)
    SET(CERTI_HOME ${CERTI_HOME_TEMP} CACHE PATH "The CERTI install directory")
    LIST(APPEND CMAKE_MODULE_PATH "${CERTI_HOME}/share/scripts")
# Or try to use CERTI possible installed at the same INSTALL_PREFIX
ELSE ("$ENV{CERTI_HOME}" STRGREATER "")
    LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/share/scripts")    
ENDIF ("$ENV{CERTI_HOME}" STRGREATER "")
#SET(CMAKE_MODULE_PATH $ENV{CERTI_HOME}/share/scripts)

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
SET(PACKAGE_VERSION_MAJOR "1")
SET(PACKAGE_VERSION_MINOR "0")
SET(PACKAGE_VERSION_PATCH "1")
SET(PACKAGE_VERSION   "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
SET(PACKAGE_BUGREPORT "christianstenzel@gmx.net")
MESSAGE(STATUS "Configuring ${PACKAGE_NAME} version ${PACKAGE_VERSION}")

#########################################################################
# Call i.e. FIND RTI
#########################################################################
FIND_PACKAGE(RTI REQUIRED)
INCLUDE_DIRECTORIES(${RTI_INCLUDE_DIR})
ADD_DEFINITIONS(${RTI_DEFINITIONS})

#########################################################################
# CPack specific Configuration
#########################################################################
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HLA Tutorial using CERTI")
SET(CPACK_PACKAGE_VENDOR "University Wismar, RG CEA, Christian Stenzel")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/HLA_Tutorial_DESCRIPTION.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
# Avoid putting BUILDIR inside source package (simple case)
IF ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
     SET(CPACK_SOURCE_IGNORE_FILES "/CVS/;/\\\\.svn/;\\\\.swp$;\\\\.#;/#;.*~;cscope.*;\\\\.cproject;\\\\.project;autom4te.cache/*")
ELSE ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
     SET(CPACK_SOURCE_IGNORE_FILES "/CVS/;/\\\\.svn/;\\\\.swp$;\\\\.#;/#;.*~;cscope.*;\\\\.cproject;\\\\.project;autom4te.cache/*;${CMAKE_BINARY_DIR}/*")
ENDIF ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")

SET(CPACK_SOURCE_GENERATOR "ZIP;TGZ")
IF (WIN32)
  SET(CPACK_GENERATOR "ZIP;NSIS")
  ## Add a custom target callable from IDE (Mostly for Visual Studio)
  GET_FILENAME_COMPONENT(CPACK_COMMAND ${CMAKE_COMMAND} PATH)
  SET(CPACK_COMMAND ${CPACK_COMMAND}/cpack)
  ADD_CUSTOM_TARGET(PACKAGE_SOURCE_ZIP
              COMMAND ${CPACK_COMMAND} -G ZIP --config CPackSourceConfig.cmake
              COMMENTS "Build a ZIP file containing the source")              
  # NSIS installer specific part
  INCLUDE(InstallRequiredSystemLibraries)
ELSE (WIN32)
  SET(CPACK_GENERATOR "ZIP;TGZ")
ENDIF(WIN32)
SET(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
SET(CPACK_PACKAGE_VERSION_MAJOR ${PACKAGE_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${PACKAGE_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${PACKAGE_VERSION_PATCH})

INCLUDE(CPack)

#########################################################################
# Here we will generate the DisplayServiceMessages.hh include file
# from DisplayServiceMessages.hh.in file.
#########################################################################
INCLUDE(GenerateMessagesHeader.cmake)

#########################################################################
# Go to subdirs 
# (or process library and executable in the current directory) 
#########################################################################

###################################
# Setup Controller Federate
#   the executable name
#   the source list
###################################
SET(EXE_NAME controllerFederate)
SET(SRC_LIST controllerFederate.cc DisplayServiceExplanation.cc)

###################################
# Generic CMake executable target
###################################
ADD_EXECUTABLE(${EXE_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${RTI_LIBRARIES})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)

###################################
# Setup Process Federate
#   the executable name
#   the source list
###################################
SET(EXE_NAME processFederate)
SET(SRC_LIST processFederate.cc DisplayServiceExplanation.cc)

###################################
# Generic CMake executable target
###################################
ADD_EXECUTABLE(${EXE_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${RTI_LIBRARIES})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)

INSTALL(FILES 
	TwoLevelController.fed
        DESTINATION share/federations)

MESSAGE(STATUS "*** ${CMAKE_PROJECT_NAME} has been successfully configured ***")
