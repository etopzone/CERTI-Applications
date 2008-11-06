#########################################################################
#Look for DXUT package
#########################################################################

#########################################################################
# Go to subdirs LibDXUT_LIBRARY
#########################################################################
FIND_LIBRARY(LibDXUT_DXUTOpt_LIBRARY
	NAMES 	DXUTOpt
	PATHS "D:\\Lib DXUT\\Debug"
	PATH_SUFFIXES lib
	DOC "The LibDXUT Library")

FIND_LIBRARY(LibDXUT_DXUT_LIBRARY
	NAMES DXUT
	PATHS "D:\\Lib DXUT\\Debug"
	PATH_SUFFIXES lib
	DOC "The LibDXUT Library")
	
IF (LibDXUT_DXUT_LIBRARY) 
  IF (NOT LibDXUT_FIND_QUIETLY)
     MESSAGE_QUIETLY(LibDXUT_FIND_QUIETLY STATUS "Looking for LibDXUT library... - found ${LibDXUT_LIBRARY}")
  ENDIF(NOT LibDXUT_FIND_QUIETLY)
  SET(LibDXUT_FOUND "YES")
  LIST(APPEND  LibDXUT_LIBRARIES ${LibDXUT_DXUT_LIBRARY} ${LibDXUT_DXUTOpt_LIBRARY})
ELSE (LibDXUT_DXUT_LIBRARY)
  SET(LibDXUT_FOUND "NO")
  IF (NOT LibDXUT_FIND_QUIETLY)
      MESSAGE_QUIETLY(LibDXUT_FIND_QUIETLY STATUS "Looking for LibDXUT library... - NOT FOUND")
  ENDIF(NOT LibDXUT_FIND_QUIETLY)
ENDIF (LibDXUT_DXUT_LIBRARY)

IF (LibDXUT_DXUT_LIBRARY) 
  GET_FILENAME_COMPONENT(LibDXUT_LIBRARY_DIRS ${LibDXUT_DXUT_LIBRARY} PATH)
ENDIF (LibDXUT_DXUT_LIBRARY)

#########################################################################
# Go to subdirs LibDXUT_XPLM_INCLUDE
#########################################################################
FIND_FILE(LibDXUT_INCLUDE
	NAMES 	DXUT.h camera.h DXUTsettingsdlg.h SDKmesh.h SDKmisc.h
	PATHS "D:\\Lib DXUT\\Include"
	PATH_SUFFIXES include
	DOC "The LibDXUT Include Files")

IF (LibDXUT_INCLUDE) 
  GET_FILENAME_COMPONENT(LibDXUT_INCLUDE_DIRS ${LibDXUT_INCLUDE} PATH) 
   INCLUDE_DIRECTORIES(${LibDXUT_INCLUDE_DIRS})
ENDIF (LibDXUT_INCLUDE)
