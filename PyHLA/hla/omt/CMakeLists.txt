SET(PYTHON_SOURCES
    __init__.py
    basic.py
    array.py
    record.py)

INSTALL(FILES ${PYTHON_SOURCES}
    DESTINATION ${SITE_PACKAGES}/data)

# $Id: CMakeLists.txt,v 1.1 2008/09/25 17:17:41 gotthardp Exp $
