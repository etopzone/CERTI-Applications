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
# enabling C is needed if you want to compile C files.
ENABLE_LANGUAGE(C)

#########################################################################
# Setup reasonable default CMake behavior
#########################################################################

# CERTI requires CMake 2.4.x
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.4)
INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
MARK_AS_ADVANCED(TCL_TCLSH DART_ROOT)

# Use CERTI_HOME env var if it is defined
IF ("$ENV{CERTI_HOME}" STRGREATER "")
    FILE(TO_CMAKE_PATH "$ENV{CERTI_HOME}" CERTI_HOME_TEMP)
    SET(CERTI_HOME ${CERTI_HOME_TEMP} CACHE PATH "The CERTI install directory")
    LIST(APPEND CMAKE_MODULE_PATH "${CERTI_HOME}/share/scripts")
# Or try to use CERTI possible installed at the same INSTALL_PREFIX
ELSE ("$ENV{CERTI_HOME}" STRGREATER "")
    LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/share/scripts")    
ENDIF ("$ENV{CERTI_HOME}" STRGREATER "")

# Or you can append here the full path to the directory
# containing FindCERTI.cmake
# LIST(APPEND CMAKE_MODULE_PATH "/your/own/path")

# HLA_TestsSuite comes with its own set of CMake macros too
LIST(APPEND CMAKE_MODULE_PATH "${HLA_TestsSuite_SOURCE_DIR}")

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
SET(PACKAGE_VERSION_MINOR "4")
SET(PACKAGE_VERSION_PATCH "0cvs")
SET(PACKAGE_VERSION   "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
SET(PACKAGE_BUGREPORT "certi-devel@nongnu.org")
MESSAGE(STATUS "Configuring ${PACKAGE_NAME} version ${PACKAGE_VERSION}")

#########################################################################
# FIND RTI (CERTI, MAK, Pitch, ...)
#########################################################################
FIND_PACKAGE(RTI REQUIRED)
include_directories(${RTI_INCLUDE_DIR} ${RTI1516_INCLUDE_DIR} ${RTI_INCLUDE_DIR}/..)
ADD_DEFINITIONS(${RTI_DEFINITIONS})

find_library(LIBHLA
  NAMES HLA HLAd
  PATHS $ENV{CERTI_HOME}
  PATH_SUFFIXES lib Debug Release
  DOC "The libHLA Library")
  
find_path(LIBHLA_INCLUDE_DIR
   NAMES libhla.hh HHLAtypesIEEE1516.hh
   PATHS $ENV{CERTI_HOME}
   PATH_SUFFIXES libhla)
   
include_directories(${LIBHLA_INCLUDE_DIR})

#########################################################################
# CPack specific Configuration
#########################################################################
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An HLA Tests Suite")
SET(CPACK_PACKAGE_VENDOR "CERTI Devel Team https://savannah.nongnu.org/projects/CERTI")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/HLA_TestsSuite_DESCRIPTION.txt")
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
INCLUDE(HLATestsSuiteTools)

#########################################################################
# ADD TEST ELEMENTARY PROGRAM
#########################################################################
HLATestsSuite_ADD_TEST(test_CreateJoinResignDestroy
                       FED_FILE test_CreateJoinResignDestroy.fed
                       SOURCE_FILES test_CreateJoinResignDestroy.cc)

#########################################################################
# ADD TEST CREATE-JOIN-RESIGN-SAVE-RESTORE-PUBLISH
#########################################################################
HLATestsSuite_ADD_TEST(create_destroy
                       FED_FILE create_destroy.fed
                       SOURCE_FILES create_destroy.cc create_destroy.hh
                       DTEST_SCRIPT dtest_create_destroy.py
                       COMMENTS "A nice test from JYR")

#########################################################################
# ADD TEST FedTime PROGRAM
#########################################################################
HLATestsSuite_ADD_TEST(test_FedTime                      
                       SOURCE_FILES test_FedTime.cc)

#########################################################################
# ADD TEST HUGE ATTRIBUTE AND PARAMETER VALUES
#########################################################################
HLATestsSuite_ADD_TEST(test_HugeAVPV
                      FED_FILE test_HugeAVPV.fed
                      SOURCE_FILES test_HugeAVPV.cc) 

#########################################################################
# ADD TEST SYNCHRONIZATION POINTS
#########################################################################
HLATestsSuite_ADD_TEST(test_Sync
                      FED_FILE test_Sync.fed
                      SOURCE_FILES test_Sync.cc
                      DTEST_SCRIPT dtest_test_Sync.py
                      COMMENTS "Test HLA Synchronization points and Callbacks") 

#########################################################################
# ADD TEST TIME REGULATION TIME CONSTRAINED CALLBACKS
#########################################################################
HLATestsSuite_ADD_TEST(test_TRTCCallbacks
                      FED_FILE test_TRTCCallbacks.fed
                      SOURCE_FILES test_TRTCCallbacks.cc
                      MORELIBS ${LIBHLA}
                      DTEST_SCRIPT dtest_test_TRTCCallbacks.py dtest_test_TRTCCallbacks-withLateFederate.py
                      COMMENTS "Test HLA TimeRegulating and TimeConstrained Callbacks") 
                      
#########################################################################
# ADD TEST INTERACTIVE FEDERATE
#########################################################################
HLATestsSuite_ADD_TEST(Interactive_Federate 
                      FED_FILE Interactive_Federation.fed
                      SOURCE_FILES Interactive_Federate.cc
                      DTEST_SCRIPT dtest_Interactive_Federate.py
                      COMMENTS "A multi-purpose test interactive Federate")
                      
#########################################################################
# ADD TEST INTERACTIVE FEDERATE 1516
#########################################################################
HLATestsSuite_ADD_TEST(Interactive_Federate1516 
                      FED_FILE Interactive_Federation.fed 01.xml Certi-Test-02.xml
                      SOURCE_FILES Interactive_Federate1516.cc
                      DTEST_SCRIPT dtest_Interactive_Federate1516.py
                      COMMENTS "A multi-purpose test interactive Federate 1516"
                      HLA1516)

#########################################################################
# ADD TEST Notification SWITCH FEDERATE 
#########################################################################
HLATestsSuite_ADD_TEST(test_NotificationSwitches
                      FED_FILE test_NotificationSwitches.fed
                      SOURCE_FILES test_NotificationSwitches.cc
                      DTEST_SCRIPT dtest_test_NotificationSwitches.py
                      COMMENTS "A test for notification switches and services")

#########################################################################
# ADD TEST Inheritance 
#########################################################################
HLATestsSuite_ADD_TEST(test_Inheritance
                      FED_FILE InheritanceTest.fed
                      SOURCE_FILES test_Inheritance.cc test_Inheritance_cmdline.h test_Inheritance_cmdline.c
                      COMMENTS "An HLA inheritance test"
                      )

#########################################################################
# ADD TEST Parse FOM file 
#########################################################################
HLATestsSuite_ADD_TEST(test_FOMParse
                      FED_FILE TestFed.fed BuggyFed.fed Hierarchical.fed RPR-FOM.fed aviationsimnetV3.1.xml portico-testfom.fed LevelOne.fed
                      SOURCE_FILES test_FOMParse.cc test_FOMParse_cmdline.h test_FOMParse_cmdline.c
                      DTEST_SCRIPT dtest_test_FOMParse.py 
                      COMMENTS "A FOM parsing test"
                      )
#########################################################################
# IEEE-1516 unit and integration tests 
#########################################################################
HLATestsSuite_ADD_TEST(01-FederationExecutionTest 
                      FED_FILE 01.xml 01.fed
                      SOURCE_FILES 01-FederationExecutionTest.cc 01-FederationExecutionTest.hh ReturnCodes.hh
                      DTEST_SCRIPT dtest_1516_01-FederationExecutionTest.py
                      COMMENTS "01-FederationExecutionTest (1516)"
                      HLA1516)
                      
HLATestsSuite_ADD_TEST(02-HandlesAndNamesTest 
                      FED_FILE Certi-Test-02.xml
                      SOURCE_FILES 02-HandlesAndNamesTest.cc 02-HandlesAndNamesTest.hh ReturnCodes.hh
                      DTEST_SCRIPT dtest_1516_02-HandlesAndNamesTest.py
                      COMMENTS "02-HandlesAndNamesTest (1516)"
                      HLA1516)

HLATestsSuite_ADD_TEST(03-PublishingTest 
                      FED_FILE Certi-Test-03.xml
                      SOURCE_FILES  03-PublishingTest.cc 03-PublishingTest.hh ReturnCodes.hh
                      COMMENTS "03-PublishingTest (1516)"
                      HLA1516)
                      
HLATestsSuite_ADD_TEST(04-NameReservation
                      FED_FILE Certi-Test-04.xml
                      SOURCE_FILES  04-NameReservation.cc 04-NameReservation.hh ReturnCodes.hh
                      COMMENTS "04-NameReservation (1516)"
                      HLA1516)

HLATestsSuite_ADD_TEST(05-ObjectManagementTest
                      SOURCE_FILES  05-ObjectManagementTest.cc 05-ObjectManagementTest.hh ReturnCodes.hh
                      COMMENTS "05-ObjectManagementTest (1516)"
                      HLA1516)

HLATestsSuite_ADD_TEST(05-ObjectManagementCallbackTest
                      SOURCE_FILES  05-ObjectManagementCallbackTest.cc 05-ObjectManagementCallbackTest.hh ReturnCodes.hh
                      COMMENTS "05-ObjectManagementCallbackTest (1516)"
                      HLA1516)                      
                                           
HLATestsSuite_ADD_TEST(06-TimeEncodingTest
                      SOURCE_FILES  06-TimeEncodingTest.cc 06-TimeEncodingTest.hh ReturnCodes.hh
                      COMMENTS "06-TimeEncodingTest (1516)"
                      HLA1516)

MESSAGE(STATUS "*** ${CMAKE_PROJECT_NAME} has been successfully configured ***")
