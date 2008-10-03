IF (NOT WIN32)
# note: this makes the 'hla' module direcly executable, without installation
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF (NOT WIN32)

ADD_SUBDIRECTORY(_omt)
ADD_SUBDIRECTORY(_rti)
ADD_SUBDIRECTORY(omt)
ADD_SUBDIRECTORY(rti)

SET(PYTHON_SOURCES
    __init__.py)

INSTALL(FILES ${PYTHON_SOURCES}
    DESTINATION hla)

# $Id: CMakeLists.txt,v 1.2 2008/10/03 15:02:22 gotthardp Exp $
