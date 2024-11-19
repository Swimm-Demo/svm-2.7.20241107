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

BUILT_SOURCES=svm_compatibility.h svm_plugin.vim
CLEANFILES=${BUILT_SOURCES}

EXTRA_DIST=${BUILT_SOURCES} COPYING signature_api database_api vim_syntax Doxyfile

pkglib_LTLIBRARIES=libsvmplugininterface.la
pkginclude_HEADERS=svm.h svm_compatibility.h COPYING Doxyfile

libsvmplugininterface_la_SOURCES=environnement.cpp environnement.h fonction.cpp fonction.h structure.cpp structure.h svm.cpp svm.h variable.cpp variable.h 
libsvmplugininterface_la_LIBADD=
libsvmplugininterface_la_LDFLAGS=-no-undefined -version-info @PLUGIN_INTERFACE_VERSION@
libsvmplugininterface_la_LIBTOOLFLAGS=--tag=disable-static

${builddir}/svm_compatibility.h: ${srcdir}/svm.h ${top_builddir}/configure ${srcdir}/signature_api
	echo "/* " > $@
	echo " *  Simple Virtual Machine - A versatile and robust architecture to" >> $@
	echo " *  easily write applications." >> $@
	echo " *  Copyright (C) 2021  Julien BRUGUIER" >> $@
	echo " * " >> $@
	echo " *  This program is free software; you can redistribute it and/or modify" >> $@
	echo " *  it under the terms of the Lesser GNU General Public License as published by" >> $@
	echo " *  the Free Software Foundation; either version 3 of the License, or" >> $@
	echo " *  (at your option) any later version." >> $@
	echo " * " >> $@
	echo " *  This program is distributed in the hope that it will be useful," >> $@
	echo " *  but WITHOUT ANY WARRANTY; without even the implied warranty of" >> $@
	echo " *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" >> $@
	echo " *  GNU General Public License for more details." >> $@
	echo " * " >> $@
	echo " *  You should have received a copy of the Lesser GNU General Public License" >> $@
	echo " *  along with this program.  If not, see <http://www.gnu.org/licenses/>." >> $@
	echo " */ " >> $@
	echo "" >> $@
	echo "#ifndef SVM_COMPATIBILITY" >> $@
	echo "#define SVM_COMPATIBILITY" >> $@
	GREP="$(GREP)" SED="$(SED)" $(SHELL) ${srcdir}/signature_api $< $@
	echo "#define SVM_VERSION \"$(PACKAGE_VERSION)\"" >> $@
	GREP="$(GREP)" SED="$(SED)" $(SHELL) ${srcdir}/database_api $< $@
	echo "#endif" >> $@

${builddir}/svm_plugin.vim: ${srcdir}/svm.h ${top_builddir}/configure ${srcdir}/vim_syntax
	echo "" > $@
	GREP="$(GREP)" SED="$(SED)" $(SHELL) ${srcdir}/vim_syntax $< $@
