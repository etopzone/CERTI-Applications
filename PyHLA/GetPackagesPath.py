# Script used by CMakeLists.txt to determine PYTHON_PACKAGES_PATH
from distutils.sysconfig import get_python_lib
print get_python_lib().replace('\\','/')
