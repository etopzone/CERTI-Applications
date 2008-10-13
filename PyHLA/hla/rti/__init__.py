#
# Python Language HLA API
# Copyright (C) 2008  Petr Gotthard <petr.gotthard@centrum.cz>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License version 2.1, as published by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# $Id: __init__.py,v 1.2 2008/10/13 13:40:05 gotthardp Exp $

from hla._rti import *

class ResignAction:
    ReleaseAttributes = 1
    DeleteObjects = 2
    DeleteObjectsAndReleaseAttributes = 3
    NoAction = 4

# $Id: __init__.py,v 1.2 2008/10/13 13:40:05 gotthardp Exp $
