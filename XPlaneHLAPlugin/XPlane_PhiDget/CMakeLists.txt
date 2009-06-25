###################################
# Setup XPlane_PhiDget Federate
#   the executable name
#   the source list
###################################
SET(EXE_NAME XPlanePhiDget)
SET(SRC_LIST 	PhiD_Accelero.cpp 
				PhiD_Circular.cpp
				PhiD_Init.cpp
				PhiD_Multi.cpp
				XP_File.cpp
				XP_Init.cpp
				XP_Run.cpp
				XP_Cnt.h
				XP_Certi.h
				../XPlane_Common/XP_HLA.h)

###################################
# Generic CMake executable target
###################################
ADD_EXECUTABLE(${EXE_NAME}  ${SRC_LIST})
TARGET_LINK_LIBRARIES (${EXE_NAME} 
						${RTI_LIBRARIES}  
						${LibPhiDget_LIBRARY} )
INSTALL(TARGETS ${EXE_NAME} 
        RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib)
