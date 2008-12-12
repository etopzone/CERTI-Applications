#    Copyright (C) 2008 Christian Stenzel, Sven Pawletta, Thorsten Pawletta
#
#    This file is part of MatlabHLA13.
#
#    MatlabHLA13 is free software: you can redistribute it and/or
#    modify it under the terms of the GNU Lesser General Public 
#    License as published by the Free Software Foundation, either 
#    version 3 of the License, or (at your option) any later version.
#
#    MatlabHLA13 is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public 
#    License along with MatlabHLA13.  
#    
#    If not, see <http://www.gnu.org/licenses/>.

# debug output enabled
FLAGS = -v -DRTI_USES_STD_FSTREAM -D_GNU_SOURCE

VERSION   = 0.5
MEXSUFFIX = mexglx
TARGETDIR = MatlabHLA13_v${VERSION}

BINS = rti.${MEXSUFFIX}

all: ${BINS}

rti.${MEXSUFFIX}: rti/rti.cpp
	mex ${FLAGS} -o $@ $^ -lRTI-NG

install:
	mkdir ./${TARGETDIR}; \
	cp rti.${MEXSUFFIX} ${TARGETDIR}
	cp m_files/* ${TARGETDIR}

clean:
	rm rti.${MEXSUFFIX}
