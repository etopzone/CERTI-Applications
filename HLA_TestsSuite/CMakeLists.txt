#########################################################################
# CMakeLists.txt for the HLA_TestsSuite project
# 
# HLA_TestsSuite is a set of [possibly] small program
# which may be used to HLA conformance testing.
# We are currently far away for full conformance testing
# but with the help of the community we may end-up with
# a full test suite which may be suitable for CERTI 
# conformance and non-regression tests and may be for 
# any RTI.
#########################################################################
PROJECT(HLA_TestsSuite CXX)

#########################################################################
# Setup reasonable default CMake behavior
#########################################################################

# CERTI requires CMake 2.4.x
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.4)

# Use CERTI specific CMake modules first
# You should set CERTI_HOME env var to appropriate value
# or put here the full path to the directory
# containing FindCERTI.cmake
SET(CERTI_HOME $ENV{CERTI_HOME} CACHE PATH "The CERTI install directory")
SET(CMAKE_MODULE_PATH ${CERTI_HOME}/share/scripts)

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
SET(PACKAGE_VERSION_PATCH "0")
SET(PACKAGE_VERSION   "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
SET(PACKAGE_BUGREPORT "certi-devel@nongnu.org")
MESSAGE(STATUS "Configuring ${PACKAGE_NAME} version ${PACKAGE_VERSION}")

#########################################################################
# Call i.e. INCLUDE the CERTI usage configuration
#########################################################################
INCLUDE(UseCERTI)

#########################################################################
# CPack specific Configuration
#########################################################################
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An HLA Tests Suite")
SET(CPACK_PACKAGE_VENDOR "CERTI Devel Team https://savannah.nongnu.org/projects/CERTI")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/HLA_TestsSuite_DESCRIPTION.txt")
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

#INCLUDE(HLATestSuiteTools)
#########################################################################
# ADD TEST ELEMENTARY PROGRAM
#########################################################################
#HLATestSuite_ADD_TEST(test_CreateJoinResignDestroy
#                      FEDFILE test_CreateJoinResignDestroy.fed
#                      SOURCE_FILES test_CreateJoinResignDestroy.cc)

SET(TESTNAME test_CreateJoinResignDestroy)
ADD_EXECUTABLE(${TESTNAME} ${TESTNAME}.cc)
TARGET_LINK_LIBRARIES(${TESTNAME} ${CERTI_LIBRARIES})
INSTALL(TARGETS ${TESTNAME} RUNTIME DESTINATION bin)
INSTALL(FILES ${TESTNAME}.fed DESTINATION share/federations)

#########################################################################
# ADD TEST CREATE-JOIN-RESIGN-SAVE-RESTORE-PUBLISH.... PROGRAM
#########################################################################
#HLATestSuite_ADD_TEST(create_destroy
#                      FEDFILE create-destroy.fed
#                      SOURCE_FILES create_destroy.cc create_destroy.hh)

SET(TESTNAME create_destroy)
ADD_EXECUTABLE(${TESTNAME} ${TESTNAME}.cc)
TARGET_LINK_LIBRARIES(${TESTNAME} ${CERTI_LIBRARIES})
INSTALL(TARGETS ${TESTNAME} RUNTIME DESTINATION bin)
INSTALL(FILES ${TESTNAME}.fed DESTINATION share/federations)

#########################################################################
# ADD TEST HUGE ATTRIBUTE AND PARAMETER VALUES.... PROGRAM
#########################################################################
#HLATestSuite_ADD_TEST(test_HugeAVPV
#                      FEDFILE test_HugeAVPV.fed
#                      SOURCE_FILES test_HugeAVPV.cc) 

SET(TESTNAME test_HugeAVPV)
ADD_EXECUTABLE(${TESTNAME} ${TESTNAME}.cc)
TARGET_LINK_LIBRARIES(${TESTNAME} ${CERTI_LIBRARIES})
INSTALL(TARGETS ${TESTNAME} RUNTIME DESTINATION bin)
INSTALL(FILES ${TESTNAME}.fed DESTINATION share/federations)

MESSAGE(STATUS "*** ${CMAKE_PROJECT_NAME} has been successfully configured ***")
