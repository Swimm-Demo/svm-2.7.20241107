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

ACLOCAL_AMFLAGS = -I m4

SUBDIRS=debug src doc

EXTRA_DIST=valgrind.supp

bin_PROGRAMS=svm svm_plugin svm_config

svm_SOURCES=
svm_LDFLAGS=-Wl,-rpath -Wl,${pkglibdir}:${MACHINE_PLUGINS}/lib:${libdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ -lsvmplugininterface -Wl,-ldl
svm_LDADD=  \
	src/demarrage/binaire/libdemarragebinaire.la \
	src/demarrage/lanceur/analyseur/libdemarragelanceuranalyseur.la \
	src/demarrage/lanceur/libdemarragelanceur.la \
	src/machine/machine/libmachinemachine.la \
	src/machine/performance/libmachineperformance.la \
	src/machine/debugueur/libmachinedebugueur.la \
	debug/styles/libdebugstyles.la \
	src/machine/debugueur/analyseur/libmachinedebugueuranalyseur.la \
	src/machine/elements/base/libmachineelementsbase.la \
	src/machine/elements/systeme/libmachineelementssysteme.la \
	src/machine/elements/ordonnanceur/libmachineelementsordonnanceur.la \
	src/machine/elements/processus/libmachineelementsprocessus.la \
	src/machine/elements/sequenceur/libmachineelementssequenceur.la \
	src/machine/elements/programme/analyseur/libmachineelementsprogrammeanalyseur.la \
	src/machine/elements/programme/libmachineelementsprogramme.la \
	src/machine/elements/processeur/instructions/libmachineelementsprocesseurinstructions.la \
	src/machine/elements/memoire/libmachineelementsmemoire.la \
	src/machine/elements/synchronisation/libmachineelementssynchronisation.la \
	src/machine/elements/noyau/libmachineelementsnoyau.la \
	src/machine/elements/processeur/libmachineelementsprocesseur.la \
	src/machine/elements/valeur/libmachineelementsvaleur.la \
	src/machine/interruption/libmachineinterruption.la \
	src/machine/extensions/gestionnaire/libmachineextensionsgestionnaire.la \
	src/machine/extensions/analyseur/libmachineextensionsanalyseur.la \
	src/machine/extensions/libmachineextensions.la \
	src/machine/interface/libsvmplugininterface.la \
	src/global/libglobal.la

svm_plugin_SOURCES=
svm_plugin_LDFLAGS=-Wl,-ldl
svm_plugin_LDADD=  \
	src/outils/libsvmplugin.la \
	src/outils/analyseur/liboutilssvmpluginanalyseur.la \
	src/machine/extensions/analyseur/libmachineextensionsanalyseur.la \
	src/machine/extensions/libmachineextensions.la \
	src/machine/elements/valeur/libmachineelementsvaleur.la \
	src/global/libglobal.la

svm_config_SOURCES=
svm_config_LDFLAGS=-Wl,-ldl
svm_config_LDADD=  \
	src/outils/libsvmconfig.la \
	src/global/libglobal.la

# hack avant de trouver la vraie raison
svm_LINK = $(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
	--mode=link $(CXX) $(AM_CFLAGS) $(CFLAGS) $(svm_LDFLAGS) \
	$(LDFLAGS) -Wl,--as-needed -o $@

svm_plugin_LINK = $(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
	--mode=link $(CXX) $(AM_CFLAGS) $(CFLAGS) $(svm_plugin_LDFLAGS) \
	$(LDFLAGS) -Wl,--as-needed -o $@

svm_config_LINK = $(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) \
	--mode=link $(CXX) $(AM_CFLAGS) $(CFLAGS) $(svm_plugin_LDFLAGS) \
	$(LDFLAGS) -Wl,--as-needed -o $@
