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

SUBDIRS= instructions 

noinst_LTLIBRARIES=libmachineelementsprocesseur.la

libmachineelementsprocesseur_la_SOURCES=etat.cpp etat.h interruptions.cpp interruptions.h processeur.cpp processeur.h 
libmachineelementsprocesseur_la_LIBADD=
libmachineelementsprocesseur_la_LDFLAGS=-no-undefined
