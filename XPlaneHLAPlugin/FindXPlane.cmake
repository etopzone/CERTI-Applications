#########################################################################
#Look for Xplane.exe
#########################################################################
FIND_PROGRAM(XPLANE_EXECUTABLE
	NAMES XPlane.exe
	PATHS "C:\\Program Files\\X-Plane 9.00 Beta-19" "D:\\mathe\\X-Plane 9.00 Beta-19"
	PATH_SUFFIXES 
	DOC "XPlane Software")
IF (XPLANE_EXECUTABLE)
  IF (NOT XPLANE_FIND_QUIETLY) 
      MESSAGE_QUIETLY(XPLANE_FIND_QUIETLY STATUS "Looking for XPlane... - XPlane is ${XPLANE_EXECUTABLE}")
  ENDIF(NOT XPLANE_FIND_QUIETLY) 
  SET(XPLANE_EXECUTABLE_FOUND "YES")
  GET_FILENAME_COMPONENT(XPLANE_BIN_DIRS ${XPLANE_EXECUTABLE} PATH)
ELSE (XPLANE_EXECUTABLE) 
  SET(XPLANE_EXECUTABLE_FOUND "NO")
  IF (NOT XPLANE_FIND_QUIETLY) 
      MESSAGE_QUIETLY(XPLANE_FIND_QUIETLY STATUS "Looking for XPlane... - XPlane NOT FOUND")
  ENDIF(NOT XPLANE_FIND_QUIETLY)
ENDIF (XPLANE_EXECUTABLE) 

#########################################################################
# Go to subdirs XPLANE_LIBRARY
#########################################################################
FIND_LIBRARY(XPLANE_XPLM_LIBRARY
			NAMES 	XPLM
			PATHS "D:\\mathe\\X-Plane PlugIn\\XPSDK200b1\\Libraries"
			PATH_SUFFIXES lib
			DOC "The XPLANE Library")

FIND_LIBRARY(XPLANE_XPWidgets_LIBRARY
			NAMES 	XPWidgets
			PATHS "D:\\mathe\\X-Plane PlugIn\\XPSDK200b1\\Libraries"
			PATH_SUFFIXES lib
			DOC "The XPLANE Library")

IF (XPLANE_XPLM_LIBRARY) 
  IF (NOT XPLANE_FIND_QUIETLY)
     MESSAGE_QUIETLY(XPLANE_FIND_QUIETLY STATUS "Looking for XPlane library... - found ${XPLANE_LIBRARY}")
  ENDIF(NOT XPLANE_FIND_QUIETLY)
  SET(XPLANE_FOUND "YES")
  LIST(APPEND XPLANE_LIBRARIES ${XPLANE_XPLM_LIBRARY} ${XPLANE_XPWidgets_LIBRARY})
ELSE (XPLANE_XPLM_LIBRARY)
  SET(XPLANE_FOUND "NO")
  IF (NOT XPLANE_FIND_QUIETLY)
      MESSAGE_QUIETLY(XPLANE_FIND_QUIETLY STATUS "Looking for XPlane library... - NOT FOUND")
  ENDIF(NOT XPLANE_FIND_QUIETLY)
ENDIF (XPLANE_XPLM_LIBRARY)

IF (XPLANE_XPLM_LIBRARY) 
  GET_FILENAME_COMPONENT(XPLANE_LIBRARY_DIRS ${XPLANE_XPLM_LIBRARY} PATH)
ENDIF (XPLANE_XPLM_LIBRARY)

#########################################################################
# Go to subdirs XPLANE_XPLM_INCLUDE
#########################################################################
FIND_FILE(XPLANE_XPLM_INCLUDE
	NAMES XPLMPlugin.h
	PATHS "D:\\mathe\\X-Plane PlugIn\\XPSDK200b1\\CHeaders\\XPLM"
	PATH_SUFFIXES include
	DOC "The XPLANE_XPLM Include Files")

IF (XPLANE_XPLM_INCLUDE) 
  GET_FILENAME_COMPONENT(XPLANE_XPLM_INCLUDE_DIRS ${XPLANE_XPLM_INCLUDE} PATH) 
   INCLUDE_DIRECTORIES(${XPLANE_XPLM_INCLUDE_DIRS})
ENDIF (XPLANE_XPLM_INCLUDE)

#########################################################################
# Go to subdirs XPLANE_XPW_INCLUDE
#########################################################################
FIND_FILE(XPLANE_XPW_INCLUDE
	NAMES XPWidgets.h
	PATHS "D:\\mathe\\X-Plane PlugIn\\XPSDK200b1\\CHeaders\\Widgets"
	PATH_SUFFIXES include
	DOC "The XPLANE_XPW Include Files")

IF (XPLANE_XPW_INCLUDE) 
  GET_FILENAME_COMPONENT(XPLANE_XPW_INCLUDE_DIRS ${XPLANE_XPW_INCLUDE} PATH) 
   INCLUDE_DIRECTORIES(${XPLANE_XPW_INCLUDE_DIRS})
ENDIF (XPLANE_XPW_INCLUDE)
