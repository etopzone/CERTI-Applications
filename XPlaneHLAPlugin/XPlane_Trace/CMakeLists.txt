###################################
# Setup XPlane_Trace Federate
#   the executable name
#   the source list
###################################
SET(EXE_NAME XPlaneTrace)
SET(SRC_LIST XP_Init.cpp XP_RTData.cpp XP_File.cpp XP_Trace.h ../XPlane_Common/XP_HLA.h ../initFile/XPlaneCde.txt)

###################################
# Generic CMake executable target
###################################
ADD_EXECUTABLE(${EXE_NAME}   ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} ${CERTI_LIBRARIES})
INSTALL(TARGETS ${EXE_NAME} RUNTIME DESTINATION bin)