SET(PYTHON_SOURCES
    __init__.py
    basic.py
    array.py
    record.py)

INSTALL(FILES ${PYTHON_SOURCES}
    DESTINATION hla/omt)

# $Id: CMakeLists.txt,v 1.2 2008/10/03 15:02:23 gotthardp Exp $
