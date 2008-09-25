FIND_PACKAGE(RTI REQUIRED)

ADD_DEFINITIONS(${RTI_DEFINITIONS})
INCLUDE_DIRECTORIES(${RTI_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

ADD_LIBRARY(_rti SHARED
    federateambassador.h
    federateambassador.cpp
    rtiambassador.h
    rtiambassador.cpp
    exceptions.h
    exceptions.cpp
    handles.h
    handles.cpp
    module.h
    module.cpp)

SET_TARGET_PROPERTIES(_rti PROPERTIES PREFIX "")
IF (WIN32)
SET_TARGET_PROPERTIES(_rti PROPERTIES SUFFIX ".pyd")
ENDIF (WIN32)

TARGET_LINK_LIBRARIES(_rti
    ${PYTHON_LIBRARY}
    ${RTI_LIBRARIES})

INSTALL(TARGETS _rti
    LIBRARY DESTINATION ${SITE_PACKAGES}
    ARCHIVE DESTINATION ${SITE_PACKAGES})

# $Id: CMakeLists.txt,v 1.1 2008/09/25 17:17:34 gotthardp Exp $
