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
# $Id: basic.py,v 1.2 2011/09/19 12:56:33 erk Exp $

def padding(size, boundary):
    return boundary - ((size-1)%boundary + 1)

# Test Python version and define the 'bytes' operator accordingly
from sys import version_info

if version_info < (3,0):
    def bytes(string='', encoding="utf-8"):
        return string

# $Id: basic.py,v 1.2 2011/09/19 12:56:33 erk Exp $

