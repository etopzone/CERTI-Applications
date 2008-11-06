#########################################################################
#Look for DXUT package
#########################################################################

#########################################################################
# Go to subdirs DirectX10_LIBRARY
#########################################################################
SET(DirectX10_LIBRARIES "")
FIND_LIBRARY(DirectX10_D3D9_LIBRARY
	NAMES 	d3d9 
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX10 Library")
LIST(APPEND DirectX10_LIBRARIES ${DirectX10_D3D9_LIBRARY})
	
FIND_LIBRARY(DirectX10_D3DX10_LIBRARY
	NAMES 	d3dX10
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX10 Library")
	
#LIST(APPEND DirectX10_LIBRARIES ${DirectX10_D3DX10_LIBRARY})
	
FIND_LIBRARY(DirectX10_D3DX10D_LIBRARY
	NAMES 	d3dX10d
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX10 Library")
	LIST(APPEND DirectX10_LIBRARIES ${DirectX10_D3DX10D_LIBRARY})
	
	FIND_LIBRARY(DirectX10_D3DXOF_LIBRARY
	NAMES 	d3dxof
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX10 Library")
	LIST(APPEND DirectX10_LIBRARIES ${DirectX10_D3DXOF_LIBRARY})
	
	FIND_LIBRARY(DirectX10_DDRAW_LIBRARY
	NAMES 	ddraw
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX10 Library")
	LIST(APPEND DirectX10_LIBRARIES ${DirectX10_DDRAW_LIBRARY})
	
	FIND_LIBRARY(DirectX10_DXERR9_LIBRARY
	NAMES DxErr9
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX10 Library")
	LIST(APPEND DirectX10_LIBRARIES ${DirectX10_DXERR9_LIBRARY})
	
	FIND_LIBRARY(DirectX10_DXERR_LIBRARY
	NAMES 	DxErr
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX10 Library")
	LIST(APPEND DirectX10_LIBRARIES ${DirectX10_DXERR_LIBRARY})
	
	FIND_LIBRARY(DirectX10_DXGI_LIBRARY
	NAMES 	dxgi dxtrans
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX10 Library")
	LIST(APPEND DirectX10_LIBRARIES ${DirectX10_DXGI_LIBRARY})
	
	FIND_LIBRARY(DirectX10_DXTRANS_LIBRARY
	NAMES 	dxtrans
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX10 Library")
	LIST(APPEND DirectX10_LIBRARIES ${DirectX10_DXTRANS_LIBRARY})
	
IF (DirectX10_D3D9_LIBRARY) 
  IF (NOT DirectX10_FIND_QUIETLY)
     MESSAGE_QUIETLY(DirectX10_FIND_QUIETLY STATUS "Looking for DirectX10 library... - found ${DirectX10_D3D9_LIBRARY}")
  ENDIF(NOT DirectX10_FIND_QUIETLY)
  SET(DirectX10_FOUND "YES")  
ELSE (DirectX10_LIBRARY)
  SET(DirectX10_FOUND "NO")
  IF (NOT DirectX10_FIND_QUIETLY)
      MESSAGE_QUIETLY(DirectX10_FIND_QUIETLY STATUS "Looking for DirectX10 library... - NOT FOUND")
  ENDIF(NOT DirectX10_FIND_QUIETLY)
ENDIF (DirectX10_D3D9_LIBRARY)

IF (DirectX10_D3D9_LIBRARY) 
  GET_FILENAME_COMPONENT(DirectX10_LIBRARY_DIRS ${DirectX10_D3D9_LIBRARY} PATH)
ENDIF (DirectX10_D3D9_LIBRARY)
LINK_DIRECTORIES(${DirectX10_D3D9_LIBRARY})

#########################################################################
# Go to subdirs DirectX10_XPLM_INCLUDE
#########################################################################
FIND_FILE(DirectX10_INCLUDE
	NAMES 	d3d9.h d3d9caps.h d3d9types.h d3d.h d3dcaps.h d3dtypes.h d3dX10.h 
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Include"
	PATH_SUFFIXES include
	DOC "The DirectX10 Include Files")

IF (DirectX10_INCLUDE) 
  GET_FILENAME_COMPONENT(DirectX10_INCLUDE_DIRS ${DirectX10_INCLUDE} PATH) 
   INCLUDE_DIRECTORIES(${DirectX10_INCLUDE_DIRS})
ENDIF (DirectX10_INCLUDE)
