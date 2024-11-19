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

pkglib_LTLIBRARIES=svmint.la svmbool.la svmreal.la svmmath.la svmstr.la svmmem.la svmrun.la svmcom.la svmplugin.la svmtype.la

#svmXXX_la_SOURCES=XXX.cpp
#svmXXX_la_LIBADD=
#svmXXX_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
#svmXXX_la_LIBTOOLFLAGS= --tag=disable-static

# -release @PACKAGE_VERSION@ enleve, car ca genere plein de fichiers morts sur le systeme

svmint_la_SOURCES=int.cpp
svmint_la_LIBADD=
svmint_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
svmint_la_LIBTOOLFLAGS= --tag=disable-static

svmbool_la_SOURCES=bool.cpp
svmbool_la_LIBADD=
svmbool_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
svmbool_la_LIBTOOLFLAGS= --tag=disable-static

svmreal_la_SOURCES=real.cpp
svmreal_la_LIBADD=
svmreal_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
svmreal_la_LIBTOOLFLAGS= --tag=disable-static

svmmath_la_SOURCES=math.cpp
svmmath_la_LIBADD=
svmmath_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
svmmath_la_LIBTOOLFLAGS= --tag=disable-static

svmstr_la_SOURCES=str.cpp
svmstr_la_LIBADD=
svmstr_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
svmstr_la_LIBTOOLFLAGS= --tag=disable-static

svmmem_la_SOURCES=mem.cpp
svmmem_la_LIBADD=
svmmem_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
svmmem_la_LIBTOOLFLAGS= --tag=disable-static

svmrun_la_SOURCES=run.cpp
svmrun_la_LIBADD=
svmrun_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
svmrun_la_LIBTOOLFLAGS= --tag=disable-static

svmcom_la_SOURCES=com.cpp
svmcom_la_LIBADD=
svmcom_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
svmcom_la_LIBTOOLFLAGS= --tag=disable-static

svmplugin_la_SOURCES=plugin.cpp
svmplugin_la_LIBADD=
svmplugin_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
svmplugin_la_LIBTOOLFLAGS= --tag=disable-static

svmtype_la_SOURCES=type.cpp
svmtype_la_LIBADD=
svmtype_la_LDFLAGS=-no-undefined -module -ldl -Wl,-rpath -Wl,${pkglibdir} -Wl,-L${top_builddir}/src/machine/interface/.libs/ ${PLUGINSSTRICTCHECK}
svmtype_la_LIBTOOLFLAGS= --tag=disable-static

