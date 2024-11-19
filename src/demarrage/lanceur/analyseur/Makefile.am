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

SUBDIRS= 

noinst_LTLIBRARIES=libdemarragelanceuranalyseur.la

BUILT_SOURCES=analyseur.lex.cpp analyseur.syn.cpp

EXTRABUILTSOURCES=analyseur.syn.h

EXTRA_DIST=analyseur.h 
CLEANFILES= analyseur.syn.output

AM_YFLAGS=-d -v --file-prefix=y --name-prefix=demarragelanceuranalyseur -o y.tab.c
AM_LFLAGS=-P demarragelanceuranalyseur -o lex.yy.c

libdemarragelanceuranalyseur_la_CXXFLAGS=$(AM_CXXFLAGS) -Wno-error=sign-compare

libdemarragelanceuranalyseur_la_SOURCES=analyseur.lex.lpp analyseur.syn.ypp analyseur.cpp analyseur.h includes.h
libdemarragelanceuranalyseur_la_LIBADD=
libdemarragelanceuranalyseur_la_LDFLAGS=-no-undefined

