IF(WIN32)
ADD_DEFINITIONS(-DIBM=1 -DAPL=0)
ENDIF(WIN32)

###################################
# Setup XPlane PlugIn Federate
#   the executable name
#   the source list
###################################
SET(EXE_NAME XPlaneFederate)
SET(SRC_LIST XPlane_InitDll.cpp XPlane_RunDll.cpp XP_Init.cpp  XP_Run.cpp  XP_Certi.h)

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