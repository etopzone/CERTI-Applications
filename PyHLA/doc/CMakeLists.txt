GET_FILENAME_COMPONENT(PYTHON_PREFIX "${PYTHON_PACKAGES_PATH}" PATH)

# set the default PYTHON_SOURCE_DIR value
IF(NOT PYTHON_SOURCE_DIR)
    SET(PYTHON_SOURCE_DIR $ENV{PYTHON_SOURCE_DIR} CACHE PATH "The Python source directory.")
ENDIF(NOT PYTHON_SOURCE_DIR)

# e.g. Python-2.5.1/Doc/tools/mkhowto
FIND_PROGRAM(PYTHON_MKHOWTO
    NAMES mkhowto
    PATHS
        ${PYTHON_PREFIX}/doc/tools
        ${PYTHON_SOURCE_DIR}/Doc/tools
    DOC "The Python mkhowto utility")

IF (PYTHON_MKHOWTO)

    SET(MKHOWTO_SOURCES
        PyHLA.tex
        omt/module.tex
        rti/module.tex
        rti/federateambassador.tex
        rti/rtiambassador.tex
        rti/services.tex
    )

    ADD_CUSTOM_TARGET(PyHLA.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}     
        COMMAND ${PYTHON_MKHOWTO} --html --dir ${CMAKE_CURRENT_SOURCE_DIR}/../manual PyHLA.tex
        SOURCES ${MKHOWTO_SOURCES}
    )

    ADD_CUSTOM_TARGET(PyHLA.dvi
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${PYTHON_MKHOWTO} --a4 --dvi PyHLA.tex
        SOURCES ${MKHOWTO_SOURCES}
    )
    
    # This is currently not working
    #ADD_CUSTOM_TARGET(PyHLA.pdf
    #    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}         
    #    COMMAND ${PYTHON_MKHOWTO} --a4 --pdf PyHLA.tex
    #)
    
    ADD_CUSTOM_TARGET(PyHLA.pdf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}         
        COMMAND dvipdf PyHLA.dvi
    )

    ADD_DEPENDENCIES(PyHLA.pdf PyHLA.dvi)

ELSE(PYTHON_MKHOWTO)
    MESSAGE("Cannot find Python mkhowto rules for generating doc not added.")
ENDIF(PYTHON_MKHOWTO)

# $Id: CMakeLists.txt,v 1.3 2009/07/10 17:03:40 gotthardp Exp $
