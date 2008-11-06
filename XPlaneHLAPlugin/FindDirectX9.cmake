#########################################################################
#Look for DXUT package
#########################################################################

#########################################################################
# Go to subdirs DirectX9_LIBRARY
#########################################################################
SET(DirectX9_LIBRARIES "")
FIND_LIBRARY(DirectX9_D3D9_LIBRARY
	NAMES 	d3d9 
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX9 Library")
LIST(APPEND DirectX9_LIBRARIES ${DirectX9_D3D9_LIBRARY})
	
FIND_LIBRARY(DirectX9_D3DX9_LIBRARY
	NAMES 	d3dx9
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX9 Library")
LIST(APPEND DirectX9_LIBRARIES ${DirectX9_D3DX9_LIBRARY})
	
FIND_LIBRARY(DirectX9_D3DX9D_LIBRARY
	NAMES 	d3dx9d
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX9 Library")
	LIST(APPEND DirectX9_LIBRARIES ${DirectX9_D3DX9D_LIBRARY})
	
	FIND_LIBRARY(DirectX9_D3DXOF_LIBRARY
	NAMES 	d3dxof
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX9 Library")
	LIST(APPEND DirectX9_LIBRARIES ${DirectX9_D3DXOF_LIBRARY})
	
	FIND_LIBRARY(DirectX9_DDRAW_LIBRARY
	NAMES 	ddraw
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX9 Library")
	LIST(APPEND DirectX9_LIBRARIES ${DirectX9_DDRAW_LIBRARY})
	
	FIND_LIBRARY(DirectX9_DXERR9_LIBRARY
	NAMES DxErr9
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX9 Library")
	LIST(APPEND DirectX9_LIBRARIES ${DirectX9_DXERR9_LIBRARY})
	
	FIND_LIBRARY(DirectX9_DXERR_LIBRARY
	NAMES 	DxErr
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX9 Library")
	LIST(APPEND DirectX9_LIBRARIES ${DirectX9_DXERR_LIBRARY})
	
	FIND_LIBRARY(DirectX9_DXGI_LIBRARY
	NAMES 	dxgi dxtrans
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX9 Library")
	LIST(APPEND DirectX9_LIBRARIES ${DirectX9_DXGI_LIBRARY})
	
	FIND_LIBRARY(DirectX9_DXTRANS_LIBRARY
	NAMES 	dxtrans
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX9 Library")
	LIST(APPEND DirectX9_LIBRARIES ${DirectX9_DXTRANS_LIBRARY})
		
	FIND_LIBRARY(DirectX9_DXGUID_LIBRARY
	NAMES 	dxguid
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Lib\\X86"
	PATH_SUFFIXES lib
	DOC "The DirectX9 Library")
	LIST(APPEND DirectX9_LIBRARIES ${DirectX9_DXGUID_LIBRARY})
	
IF (DirectX9_D3D9_LIBRARY) 
  IF (NOT DirectX9_FIND_QUIETLY)
     MESSAGE_QUIETLY(DirectX9_FIND_QUIETLY STATUS "Looking for DirectX9 library... - found ${DirectX9_D3D9_LIBRARY}")
  ENDIF(NOT DirectX9_FIND_QUIETLY)
  SET(DirectX9_FOUND "YES")  
ELSE (DirectX9_LIBRARY)
  SET(DirectX9_FOUND "NO")
  IF (NOT DirectX9_FIND_QUIETLY)
      MESSAGE_QUIETLY(DirectX9_FIND_QUIETLY STATUS "Looking for DirectX9 library... - NOT FOUND")
  ENDIF(NOT DirectX9_FIND_QUIETLY)
ENDIF (DirectX9_D3D9_LIBRARY)

IF (DirectX9_D3D9_LIBRARY) 
  GET_FILENAME_COMPONENT(DirectX9_LIBRARY_DIRS ${DirectX9_D3D9_LIBRARY} PATH)
ENDIF (DirectX9_D3D9_LIBRARY)

#########################################################################
# Go to subdirs DirectX9_XPLM_INCLUDE
#########################################################################
FIND_FILE(DirectX9_INCLUDE
	NAMES 	d3d9.h d3d9caps.h d3d9types.h d3d.h d3dcaps.h d3dtypes.h d3dx9.h 
	PATHS "C:\\Program Files\\Microsoft DirectX SDK (August 2007)\\Include"
	PATH_SUFFIXES include
	DOC "The DirectX9 Include Files")

IF (DirectX9_INCLUDE) 
  GET_FILENAME_COMPONENT(DirectX9_INCLUDE_DIRS ${DirectX9_INCLUDE} PATH) 
   INCLUDE_DIRECTORIES(${DirectX9_INCLUDE_DIRS})
ENDIF (DirectX9_INCLUDE)
