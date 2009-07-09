ADD_CUSTOM_TARGET(PyHLA.dvi
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    # e.g. Python-2.5.1/Doc/tools/mkhowto
    COMMAND "mkhowto" --html --a4 --dvi --dir ${CMAKE_CURRENT_SOURCE_DIR}/../manual PyHLA.tex
)
