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

    SET(HOWTO_HTML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../manual")

    ADD_CUSTOM_TARGET(howto
        DEPENDS ${HOWTO_HTML_DIR}/PyHLA.html PyHLA.dvi PyHLA.pdf
    )

    # download msc.sty from http://satoss.uni.lu/mscpackage
    ADD_CUSTOM_COMMAND(
        OUTPUT msc.sty
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/GetMscStyle.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    SET(HOWTO_SOURCES
        PyHLA.tex
        omt/module.tex
        rti/module.tex
        rti/federateambassador.tex
        rti/rtiambassador.tex
        rti/services.tex
    )

    ADD_CUSTOM_COMMAND(
        OUTPUT ${HOWTO_HTML_DIR}/PyHLA.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}     
        COMMAND ${PYTHON_MKHOWTO} --html --dir ${HOWTO_HTML_DIR} PyHLA.tex
        DEPENDS ${HOWTO_SOURCES}
    )

    ADD_CUSTOM_COMMAND(
        OUTPUT PyHLA.dvi
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${PYTHON_MKHOWTO} --a4 --dvi PyHLA.tex
        DEPENDS ${HOWTO_SOURCES}
    )
    
    # This is currently not working
    #ADD_CUSTOM_COMMAND(
    #    OUTPUT PyHLA.pdf
    #    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}         
    #    COMMAND ${PYTHON_MKHOWTO} --a4 --pdf PyHLA.tex
    #    DEPENDS ${HOWTO_SOURCES}
    #)
    
    ADD_CUSTOM_COMMAND(
        OUTPUT PyHLA.pdf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}         
        COMMAND dvipdf PyHLA.dvi
        DEPENDS PyHLA.dvi
    )

ELSE(PYTHON_MKHOWTO)
    MESSAGE("Cannot find Python mkhowto rules for generating doc not added.")
ENDIF(PYTHON_MKHOWTO)

# $Id: CMakeLists.txt,v 1.5 2009/07/12 19:30:05 gotthardp Exp $
