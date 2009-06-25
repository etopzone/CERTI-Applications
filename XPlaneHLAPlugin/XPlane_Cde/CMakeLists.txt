IF(WIN32)
ADD_DEFINITIONS(-DIBM=1 -DAPL=0)
ENDIF(WIN32)

###################################
# Setup XPlane_Trace
#   the executable name
#   the source list
###################################
SET(EXE_NAME XPlaneCde)
SET(SRC_LIST 	XP_File.cpp
				XP_Init.cpp
				XP_RTData.cpp
				XPlane_InitDll.cpp
				XPlane_RunDll.cpp
				../XPlane_Common/XP_HLA.h
				XP_Trace.h)

###################################
# Generic CMake executable target
###################################
ADD_LIBRARY(${EXE_NAME}   ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${RTI_LIBRARIES} ${XPLANE_XPLM_LIBRARY})
INSTALL(TARGETS ${EXE_NAME} RUNTIME DESTINATION bin)

add_custom_command(TARGET ${EXE_NAME} 
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} ARGS -E copy  ${CMAKE_BUILD_TYPE}/${EXE_NAME}.dll ${XPLANE_PLUGINS_DIRECTORY}/${EXE_NAME}.xpl 
		    WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                     COMMENT "Copy the DLL to XPlane install directory")
