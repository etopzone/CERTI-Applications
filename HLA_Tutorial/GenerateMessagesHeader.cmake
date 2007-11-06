#########################################################################
# Here we will generate the DisplayServiceMessages.hh include file
# from DisplayServiceMessages.hh.in file.
#
# The resulting file contains all messages displayed by the 
# display service.
# This file is generated in order to ease it's update
# (or even later translation)
#
# There is a 2 steps: 
#   - first we run some scripts in order to extract some source line
#     of code
#   - then we generate the header file using CMake CONFIGURE_FILE 
#     which basically does var replacement.
#
# This generation currently works only on Unix 
# on other platform we just use the provided header file
# (which may not be up to date)
#########################################################################

# define a CMake MACRO whose purpose
# is to grep FOR_NB_LINES lines in
# the file SRC_FILE from the first line matching 
# FROM_STRING and put the result in VARNAME
MACRO(GREP_SOURCE SRC_FILE FROM_STRING FOR_NB_LINES VARNAME)
    # First we run some [not so ugly] scripts in order to get
    # some numbered lines of code
    EXECUTE_PROCESS(COMMAND grep -n -i -A ${FOR_NB_LINES} "${FROM_STRING}" ${SRC_FILE}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  
                    OUTPUT_VARIABLE TMP
                    ERROR_VARIABLE DS_MSG_ERROR)
    # replace end of line with proper escape sequence
    # '\n' becomes
    # '\n' for newline + '\' for continuation line
    # [note that extra '\' are here for CMake escape sequence :=( ]  
    STRING(REPLACE "\n" "\\n\\\n" TMP2 "${TMP}")
    # Add header line indicating source file name
    SET(TMP "Example from file: ${SRC_FILE}\\n${TMP2}")
    # now we should escape '"' for character string      
    STRING(REPLACE "\"" "\\\"" ${VARNAME} "${TMP}")    
ENDMACRO(GREP_SOURCE SRC_FILE FROM_STRING FOR_NB_LINES VARNAME)

IF(UNIX)
    # federation management
    GREP_SOURCE(controllerFederate.cc "create federation execution" 12 DS_MSG_CREATE_FEDERATION_EXECUTION_SRC)
    GREP_SOURCE(controllerFederate.cc "join federation execution" 10 DS_MSG_JOIN_FEDERATION_EXECUTION_SRC)
    GREP_SOURCE(controllerFederate.cc "resign federation execution" 9 DS_MSG_RESIGN_FEDERATION_EXECUTION_SRC)
    GREP_SOURCE(controllerFederate.cc "destroy federation execution" 10 DS_MSG_DESTROY_FEDERATION_EXECUTION_SRC)
    # declaration management
    # ancillary services
    GREP_SOURCE(controllerFederate.cc "get object class handle" 10 DS_MSG_GET_OBJECT_CLASS_HANDLE_SRC)
    GREP_SOURCE(controllerFederate.cc "get attribute handle" 9 DS_MSG_GET_ATTRIBUTE_HANDLE_SRC)
    # Then we generate the header using var value replacement 
    CONFIGURE_FILE(DisplayServiceMessages.hh.in ${CMAKE_CURRENT_SOURCE_DIR}/DisplayServiceMessages.hh)    
ENDIF(UNIX)
