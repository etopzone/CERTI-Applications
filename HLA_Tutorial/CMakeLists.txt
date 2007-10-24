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
# containing FindCERTI.cmake
SET(CMAKE_MODULE_PATH $ENV{CERTI_HOME}/share/scripts)

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
SET(PACKAGE_VERSION_PATCH "2")
SET(PACKAGE_VERSION   "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
SET(PACKAGE_BUGREPORT "christianstenzel@gmx.net")
MESSAGE(STATUS "Configuring ${PACKAGE_NAME} version ${PACKAGE_VERSION}")

#########################################################################
# Call i.e. INCLUDE the CERTI usage configuration
#########################################################################
INCLUDE(UseCERTI)

#########################################################################
# CPack specific Configuration
#########################################################################
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HLA Tutorial using CERTI")
SET(CPACK_PACKAGE_VENDOR "University Wismar, RG CEA, Christian Stenzel")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/HLA_Tutorial_DESCRIPTION.txt")
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
TARGET_LINK_LIBRARIES (${EXE_NAME} ${CERTI_LIBRARIES})
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
TARGET_LINK_LIBRARIES (${EXE_NAME} ${CERTI_LIBRARIES})
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin)

INSTALL_FILES(share/federations TwoLevelController.fed)

MESSAGE(STATUS "*** ${CMAKE_PROJECT_NAME} has been successfully configured ***")
