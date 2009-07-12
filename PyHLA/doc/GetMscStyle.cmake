# Simple cmake script which may be used
# to get the msc.sty file from http://satoss.uni.lu/mscpackage
# which is needed to build the PyHLA documentation
cmake_minimum_required(VERSION 2.6)
SET(LOCAL_FILE "./msc.sty")
SET(REMOTE_FILE "http://satoss.uni.lu/mscpackage/msc.sty")

message(STATUS "Checking for ${LOCAL_FILE} file...")
if (EXISTS ${LOCAL_FILE})
   message(STATUS "Already there: nothing to do")
else (EXISTS ${LOCAL_FILE})
   message(STATUS "Not there, trying to download...")
   file(DOWNLOAD ${REMOTE_FILE} ${LOCAL_FILE}
        TIMEOUT 10
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
