
get_filename_component(PYTHON_PREFIX "${PYTHON_PACKAGES_PATH}" PATH)

# e.g. Python-2.5.1/Doc/tools/mkhowto
find_program(PYTHON_MKHOWTO mkhowto
             PATHS ${PYTHON_PREFIX}/doc/tools
             DOC "The Python mkhowto utility") 
             
IF (PYTHON_MKHOWTO)

    add_custom_target(PyHLA.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}     
        COMMAND ${PYTHON_MKHOWTO} --html --dir ${CMAKE_CURRENT_SOURCE_DIR}/../manual PyHLA.tex
    )

    ADD_CUSTOM_TARGET(PyHLA.dvi
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${PYTHON_MKHOWTO} --a4 --dvi PyHLA.tex
    )
    
    # This is currently not working
    #add_custom_target(PyHLA.pdf
    #    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}         
    #    COMMAND ${PYTHON_MKHOWTO} --a4 --pdf PyHLA.tex
    #)
    
    add_custom_target(PyHLA.pdf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}         
        COMMAND dvipdf PyHLA.dvi
    )
    
    add_dependencies(PyHLA.pdf PyHLA.dvi)
                   
ELSE(PYTHON_MKHOWTO)
    MESSAGE("Cannot find Python mkhowto rules for generating doc not added.")
ENDIF(PYTHON_MKHOWTO)
