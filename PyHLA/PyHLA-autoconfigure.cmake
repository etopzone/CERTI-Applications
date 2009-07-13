# Simple cmake script which may be used to autoconfigure the PyHLA from 
# automatically downloaded source
#
#   cd tmp/
#   cmake -P PyHLA-autoconfigure.cmake
# you should end up with a
#   tmp/PyHLA-1.0.0-Source
# configure and compiled tree, using the tarball found on Savannah. 

cmake_minimum_required(VERSION 2.6)
set(PYHLA_VERSION "1.0.0")
set(PYHLA_FILE_PREFIX "PyHLA-${PYHLA_VERSION}-Source")
set(PYHLA_REMOTE_PREFIX "http://download.savannah.gnu.org/releases-noredirect/certi/contrib/PyHLA/")
set(PYHLA_FILE_SUFFIX ".tar.gz")

set(LOCAL_FILE "./${PYHLA_FILE_PREFIX}${PYHLA_FILE_SUFFIX}")
set(REMOTE_FILE "${PYHLA_REMOTE_PREFIX}${PYHLA_FILE_PREFIX}${PYHLA_FILE_SUFFIX}")

message(STATUS "Trying to autoconfigure PyHLA version ${VERSION} using ${REMOTE_FILE} file...")

message(STATUS "Downloading...")
if (EXISTS ${LOCAL_FILE})
   message(STATUS "Already there: nothing to do")
else (EXISTS ${LOCAL_FILE})
   message(STATUS "Not there, trying to download...")
   file(DOWNLOAD ${REMOTE_FILE} ${LOCAL_FILE}
        TIMEOUT 120
        STATUS DL_STATUS
        LOG DL_LOG)  
   list(GET DL_STATUS 0 DL_NOK)
   if ("${DL_LOG}" MATCHES "404 Not Found")
      set(DL_NOK 1)
   endif ("${DL_LOG}" MATCHES "404 Not Found")
   if (DL_NOK)
      # we shall remove the file because it is created
      # with an inappropriate content
      file(REMOVE ${LOCAL_FILE})
      message(SEND_ERROR "Download failed: ${DL_LOG}")           
   else (DL_NOK)
      message(STATUS "Download successful.")
   endif (DL_NOK)      
endif (EXISTS ${LOCAL_FILE})

message(STATUS "Unarchiving the file")
execute_process(COMMAND ${CMAKE_COMMAND} -E tar zxvf ${LOCAL_FILE}
                RESULT_VARIABLE UNTAR_RES
                OUTPUT_VARIABLE UNTAR_OUT
                ERROR_VARIABLE UNTAR_ERR
                ) 
message(STATUS "PyHLA version ${PYHLA_VERSION} has been unarchived in ${CMAKE_CURRENT_SOURCE_DIR}/${PYHLA_FILE_PREFIX}.")             
                
message(STATUS "Configuring with CMake...")
execute_process(COMMAND ${CMAKE_COMMAND} . 
                WORKING_DIRECTORY ${PYHLA_FILE_PREFIX}          
                RESULT_VARIABLE CONFIG_RES
                OUTPUT_VARIABLE CONFIG_OUT
                ERROR_VARIABLE CONFIG_ERR
                TIMEOUT 200
                )

# CMAKE_BUILD_TOOL is not defined in scripting (-P) mode
# so we have to build a sample project for knowing it
file(MAKE_DIRECTORY "TestProj")                              
file(WRITE TestProj/CMakeLists.txt 
    "
PROJECT(GETBT C CXX)
MESSAGE(STATUS \"BT=\${CMAKE_BUILD_TOOL}==\")
    "
     )     
execute_process(COMMAND  ${CMAKE_COMMAND} .
                WORKING_DIRECTORY  ./TestProj      
                OUTPUT_VARIABLE BT
                TIMEOUT 100
                ERROR_QUIET)
file(REMOVE_RECURSE  "TestProj")                           
string(REGEX MATCH "^-- .*=(.*)==.*" TEMP ${BT})
set(BUILD_TOOL ${CMAKE_MATCH_1})           
           
message(STATUS "Building with ${BUILD_TOOL}...")
execute_process(COMMAND ${BUILD_TOOL}        
                WORKING_DIRECTORY ${PYHLA_FILE_PREFIX}
                RESULT_VARIABLE CONFIG_RES
                OUTPUT_VARIABLE CONFIG_OUT
                ERROR_VARIABLE CONFIG_ERR
                ) 
                                
message(STATUS "PyHLA version ${PYHLA_VERSION} has been builtin in ${CMAKE_CURRENT_SOURCE_DIR}/${PYHLA_FILE_PREFIX}.")
message(STATUS "You either use it directly if you do update you PYTHONPATH")
message(STATUS "    or invoke install target of your build tool")