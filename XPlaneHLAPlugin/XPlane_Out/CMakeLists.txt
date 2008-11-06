IF(WIN32)
ADD_DEFINITIONS(-DIBM=1 -DAPL=0)
ENDIF(WIN32)

###################################
# Setup XPlane_Cde Federate
#   the executable name
#   the source list
###################################
SET(EXE_NAME XPlaneOut)
SET(SRC_LIST XPlane_InitDll.cpp XPlane_RunDll.cpp XP_Init.cpp  XP_File.cpp  XP_Run.cpp  XP_Certi.h  ../XPlane_Common/XP_HLA.h)

###################################
# Generic CMake executable target
###################################
ADD_LIBRARY(${EXE_NAME}  ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} 
						${CERTI_LIBRARIES}  
						${XPLANE_LIBRARIES} )
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib)
	
add_custom_command(TARGET ${EXE_NAME} 
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} ARGS -E copy  ${CMAKE_BUILD_TYPE}/${EXE_NAME}.dll ${XPLANE_PLUGINS_DIRECTORY}/${EXE_NAME}.xpl                    
                     WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
                     COMMENT "Copy the DLL to XPlane install directory")