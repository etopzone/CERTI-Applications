#########################################################################
# CMakeLists.txt for the XPlane project
#########################################################################
PROJECT(XPlaneHLAPlugin CXX)

#########################################################################
# Setup reasonable default CMake behavior
#########################################################################

# CERTI requires CMake 2.4.x
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.4)

# Use CERTI specific CMake modules first
# You should set CERTI_HOME env var to appropriate value or put here the full path to the directory containing FindCERTI.cmake
FILE(TO_CMAKE_PATH "$ENV{CERTI_HOME}" CERTI_PATH_HOME)
LIST(APPEND CMAKE_MODULE_PATH ${CERTI_PATH_HOME}/share/scripts)

# You should uncomment the following line if you want VERBOSE build
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# The preferred (default) behaviour is to build library as shared on all platform
OPTION(BUILD_SHARED
  "Build libraries as shared library" ON)
IF (BUILD_SHARED)
  SET(BUILD_SHARED_LIBS ON)
ELSE (BUILD_SHARED)
  SET(BUILD_SHARED_LIBS OFF)
ENDIF (BUILD_SHARED)

# Change the build type to release when you are ready to release 
SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")

#########################################################################
# Define your project packaging basics
#########################################################################
SET(PACKAGE_NAME      ${CMAKE_PROJECT_NAME})
SET(PACKAGE_VERSION_MAJOR "0")
SET(PACKAGE_VERSION_MINOR "1")
SET(PACKAGE_VERSION_PATCH "0")
SET(PACKAGE_VERSION   "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
SET(PACKAGE_BUGREPORT "jean-michel.mathe@onecert.fr")
MESSAGE(STATUS "Configuring ${PACKAGE_NAME} version ${PACKAGE_VERSION}")

#########################################################################
# Call i.e. INCLUDE the CERTI usage configuration
#########################################################################
INCLUDE(UseCERTI)

#########################################################################
# CPack specific Configuration
#########################################################################
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "XPlane Federation using CERTI")
SET(CPACK_PACKAGE_VENDOR "ONERA, CT/TIS, JMMathé")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/XPlane_DESCRIPTION.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
SET(CPACK_SOURCE_IGNORE_FILES "/CVS/;/\\\\.svn/;\\\\.swp$;\\\\.#;/#;.*~;cscope.*")
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

 LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
 FIND_PACKAGE(XPlane)

ADD_SUBDIRECTORY(XPlane_PlugIn)
ADD_SUBDIRECTORY(XPlane_Trace)

INSTALL(FILES 
	XPlane_CERTI.fed
        DESTINATION share/federations)

MESSAGE(STATUS "*** ${CMAKE_PROJECT_NAME} has been successfully configured ***")