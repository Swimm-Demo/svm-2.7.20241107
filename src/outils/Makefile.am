# 
# Simple Virtual Machine - A versatile and robust architecture to
# easily write applications.
# Copyright (C) 2021  Julien BRUGUIER
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}

SUBDIRS=analyseur

noinst_LTLIBRARIES=libsvmplugin.la libsvmconfig.la

libsvmplugin_la_SOURCES=svm_plugin.cpp outils.cpp outils.h
libsvmplugin_la_LIBADD=
libsvmplugin_la_LDFLAGS=-no-undefined

libsvmconfig_la_SOURCES=svm_config.cpp
libsvmconfig_la_LIBADD=
libsvmconfig_la_LDFLAGS=-no-undefined

