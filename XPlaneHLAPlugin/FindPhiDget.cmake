#########################################################################
#Look for PhiDget package
#########################################################################

#########################################################################
# Go to subdirs PhiDget
#########################################################################
FILE(TO_CMAKE_PATH "D:\\PhiDget\\VCpp" PHIDGET_PATH) 
FIND_LIBRARY(LibPhiDget_LIBRARY
	NAMES phidget21
	PATHS ${PHIDGET_PATH}
	PATH_SUFFIXES lib
	DOC "The LibPhiDget Library")
	
MESSAGE(STATUS "LibPhiDget_LIBRARY =${LibPhiDget_LIBRARY}")
	
IF (LibPhiDget_LIBRARY) 
  IF (NOT LibPhiDget_FIND_QUIETLY)
     MESSAGE_QUIETLY(LibPhiDget_FIND_QUIETLY STATUS "Looking for LibPhiDget library... - found ${LibPhiDget_LIBRARY}")
  ENDIF(NOT LibPhiDget_FIND_QUIETLY)
  SET(LibPhiDget_FOUND "YES")
  LIST(APPEND  LibPhiDget_LIBRARIES ${LibPhiDget_LIBRARY}})
ELSE (LibPhiDget_LIBRARY)
  SET(LibPhiDget_FOUND "NO")
  IF (NOT LibPhiDget_FIND_QUIETLY)
      MESSAGE_QUIETLY(LibPhiDget_FIND_QUIETLY STATUS "Looking for LibPhiDget library... - NOT FOUND")
  ENDIF(NOT LibPhiDget_FIND_QUIETLY)
ENDIF (LibPhiDget_LIBRARY)

IF (LibPhiDget_LIBRARY) 
  GET_FILENAME_COMPONENT(LibPhiDget_LIBRARY_DIRS ${LibPhiDget_LIBRARY} PATH)
ENDIF (LibPhiDget_LIBRARY)

#########################################################################
# Go to subdirs LibPhiDget_INCLUDE
#########################################################################
FIND_FILE(LibPhiDget_INCLUDE
	NAMES 	phidget21.h
	PATHS ${PHIDGET_PATH}
	DOC "The LibPhiDget Include Files")
	
MESSAGE(STATUS "LibPhiDget_INCLUDE =${LibPhiDget_INCLUDE}")

IF (LibPhiDget_INCLUDE) 
  GET_FILENAME_COMPONENT(LibPhiDget_INCLUDE_DIRS ${LibPhiDget_INCLUDE} PATH) 
   INCLUDE_DIRECTORIES(${LibPhiDget_INCLUDE_DIRS})
ENDIF (LibPhiDget_INCLUDE)
