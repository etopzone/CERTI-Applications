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
    DESTINATION ${SITE_PACKAGES})

# $Id: CMakeLists.txt,v 1.1 2008/09/25 17:17:30 gotthardp Exp $
