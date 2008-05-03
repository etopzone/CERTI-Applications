
INCLUDE(MacroTools.cmake)

MACRO(HLATestsSuite_ADD_TEST)
    PARSE_ARGUMENTS(HLATSTools "FED_FILE;SOURCE_FILES" "VERBOSE" ${ARGN})          
    CAR(TESTNAME ${HLATSTools_DEFAULT_ARGS})       
    MESSAGE(STATUS "Configuring HLA Test: ${TESTNAME}")
    IF (HLATSTools_VERBOSE)     
       MESSAGE(STATUS "       FED_FILE = ${HLATSTools_FED_FILE}")
       MESSAGE(STATUS "   SOURCE_FILES = ${HLATSTools_SOURCE_FILES}")
    ENDIF(HLATSTools_VERBOSE)
    ADD_EXECUTABLE(${TESTNAME} ${HLATSTools_SOURCE_FILES})    
    TARGET_LINK_LIBRARIES (${TESTNAME} ${CERTI_LIBRARIES})    
    INSTALL(TARGETS ${TESTNAME} 
            RUNTIME DESTINATION bin)
    INSTALL(FILES ${HLATSTools_FED_FILE}
            DESTINATION share/federations)
ENDMACRO(HLATestsSuite_ADD_TEST)
