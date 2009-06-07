ENABLE_TESTING()

ADD_TEST(transportationtype_conversion ${PYTHON_EXECUTABLE} transportationtype_conversion.py)
ADD_TEST(region_creation ${PYTHON_EXECUTABLE} region_creation.py)

# $Id: CMakeLists.txt,v 1.2 2009/06/07 15:12:48 gotthardp Exp $
