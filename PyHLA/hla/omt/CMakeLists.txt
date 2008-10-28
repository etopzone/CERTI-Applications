SET(PYTHON_SOURCES
    __init__.py
    basic.py
    enumerated.py
    array.py
    record.py)

INSTALL(FILES ${PYTHON_SOURCES}
    DESTINATION hla/omt)

# $Id: CMakeLists.txt,v 1.3 2008/10/28 17:00:45 gotthardp Exp $
