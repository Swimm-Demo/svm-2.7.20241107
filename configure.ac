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

m4_define([v_fonctionalite],[2])
m4_define([v_interface],[7])
m4_define([v_date],[20241107])

AC_PREREQ([2.69])
AC_INIT([svm],[v_fonctionalite.v_interface.v_date],[projet.svm@pappy.tf])

PLUGIN_INTERFACE_VERSION="0:0:0"
AC_SUBST([PLUGIN_INTERFACE_VERSION])

AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_TARGET

LT_INIT
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/])

AC_CONFIG_HEADERS([config.h])

#Arguments
AC_ARG_ENABLE(strict-plugins,
[AS_HELP_STRING([--enable-strict-plugins],[enable strict plugin link checking])],[
AC_MSG_CHECKING([whether strict plugin link is activated])
AS_IF([test x$enable_strict_plugins = xno],[
PLUGINSSTRICTCHECK=""
AC_MSG_RESULT([no])
],[
PLUGINSSTRICTCHECK="-Wl,--no-undefined"
AC_MSG_RESULT([yes])
])
],[
AC_MSG_CHECKING([whether strict plugin link is activated])
PLUGINSSTRICTCHECK=""
AC_MSG_RESULT([no])
])

AC_ARG_ENABLE(developer-mode,
[AS_HELP_STRING([--enable-developer-mode],[enable developer mode (assertions and traces)])],[
AC_MSG_CHECKING([whether developer mode is activated])
AS_IF([test x$enable_developer_mode = xno],[
AC_DEFINE([NDEBUG],[1],[Production mode])
AC_MSG_RESULT([no])
],[
AC_MSG_RESULT([yes])
])
],[
AC_MSG_CHECKING([whether developer mode is activated])
AC_DEFINE([NDEBUG],[1],[Production mode])
AC_MSG_RESULT([no])
])

AC_ARG_ENABLE(linux-extensions,
[AS_HELP_STRING([--enable-linux-extensions],[enable Linux extensions])],[
AC_MSG_CHECKING([whether Linux extensions are activated])
AS_IF([test x$enable_linux_extensions = xno],[
AC_MSG_RESULT([no])
],[
AC_MSG_RESULT([yes])
AC_DEFINE([LINUX_EXTENSIONS],[1],[Linux extensions])
])
],[
AC_MSG_CHECKING([whether Linux extensions are activated])
AC_DEFINE([LINUX_EXTENSIONS],[1],[Linux extensions])
enable_linux_extensions=yes
AC_MSG_RESULT([yes])
])

AC_ARG_VAR([REACTIVE_SIGNAL],[signal used to interrupt processes on notifications (requires Linux extensions, SIGURG by default)])
AS_IF([test x$enable_linux_extensions = xyes ],[
	AC_MSG_CHECKING([which signal is used for reactive behavior])
	AS_IF([test x$REACTIVE_SIGNAL = x],[
		REACTIVE_SIGNAL=SIGURG
	])
	AC_DEFINE_UNQUOTED([REACTIVE_SIGNAL],[$REACTIVE_SIGNAL],[Signal used for reactive behavior on notifications])
	AC_MSG_RESULT([$REACTIVE_SIGNAL])
])

AC_ARG_VAR([DEFAULT_STYLE],[default style for debugger (default by default)])
AC_MSG_CHECKING([which default style is used for debugger])
AS_IF([test x$DEFAULT_STYLE = x],[
	DEFAULT_STYLE=default
])
AC_DEFINE_UNQUOTED([DEFAULT_STYLE],["$DEFAULT_STYLE"],[Default style for debugger])
AC_MSG_RESULT([$DEFAULT_STYLE])

AC_ARG_VAR([MACHINE_PLUGINS],[machine plugins root directory (EPREFIX by default)])
AC_MSG_CHECKING([which directory is used to store machine plugins])
AS_IF([test x$MACHINE_PLUGINS = x],[
	MACHINE_PLUGINS='${exec_prefix}'
])
AC_DEFINE_UNQUOTED([MACHINE_PLUGINS],["$MACHINE_PLUGINS"],[Machine plugins root directory])
AC_MSG_RESULT([$MACHINE_PLUGINS])

# Checks for programs.
AC_CHECK_PROG([DIFF],[diff],[diff -u])
AC_PROG_CXX
AC_LANG([C++])
AC_CXXFLAGS_GXX_OPTION_MANDATORY(-std=c++14)
AC_PARSER_STRICT_CHECK
LEX=flex
AM_PROG_LEX_ABLE
YACC=bison
AC_PROG_YACC_ABLE
AC_CHECK_TOOL([STRIP],[strip])

# Checks for libraries.
AC_CHECK_LIB(dl,dlopen)
AC_CHECK_LIB(pthread,pthread_create)
AS_IF([test "x$enable_linux_extensions" = xyes],[
	AC_MSG_CHECKING([whether Linux extensions can be enabled])
	AS_IF([test "x$ac_cv_lib_pthread_pthread_create" = xno],[
		AC_MSG_ERROR([pthread required])
	])
	AC_MSG_RESULT([yes])
])

# Checks for header files.
CXXFLAGS_OLD=$CXXFLAGS
CXXFLAGS="$CXXFLAGS $AM_CXXFLAGS"

AC_HEADER_ASSERT
AC_CHECK_HEADERS([algorithm atomic cassert chrono condition_variable cstring dlfcn.h fcntl.h fstream functional iomanip iostream iterator list map memory mutex netdb.h netinet/tcp.h random set signal.h sstream stdint.h stdio.h stdlib.h string string.h sys/socket.h sys/stat.h sys/types.h sys/wait.h thread time.h typeinfo unistd.h unordered_map vector],,[AC_MSG_ERROR([Header file required to compile not available!])])
AC_CHECK_HEADERS([pthread_np.h],,)

CXXFLAGS=$CXXFLAGS_OLD

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_STAT
AC_CHECK_FUNCS([accept access acos asin atan atoi atol bind chmod close connect cos dlclose dlerror dlopen dlsym exp fclose fcntl fopen freeaddrinfo gai_strerror getaddrinfo getnameinfo getopt getpid gmtime kill listen localtime log lseek malloc memcpy memset open rand read recv select send setsockopt shutdown sigaction sigaddset sigdelset sigemptyset sigfillset sigprocmask sigwait sin snprintf socket srand stat strcmp strerror strftime strptime strtol tan tcgetattr tcsetattr time waitpid write],,[AC_MSG_ERROR([Function required to compile not available!])])

AS_IF([test "x$ac_cv_lib_pthread_pthread_create" = xyes],[
	AC_CHECK_FUNCS([pthread_exit pthread_kill pthread_setname_np pthread_sigmask],,[AC_MSG_ERROR([Function required to compile not available!])])
])

svm_old_prefix=$prefix
test x$prefix == xNONE && prefix=$ac_default_prefix
svm_old_exec_prefix=$exec_prefix
test x$exec_prefix == xNONE && exec_prefix=$prefix

svm_expand_var()
{
	p=$1
	v=$(eval echo $p)
	while test x$v != x$p ;
	do
		p=$v;
		v=$(eval echo $v)
	done
	echo -n $v
}

DEBUGGER_SITE=$(svm_expand_var ${datadir})/${PACKAGE}/debug
REP_BINAIRE=$(svm_expand_var ${bindir})
EXT_BINAIRE=$(svm_expand_var ${EXEEXT})
REP_INCLUDE=$(svm_expand_var ${includedir})/${PACKAGE}
REP_INTERFACE=$(svm_expand_var ${libdir})/${PACKAGE}
REP_INSTALL=$(svm_expand_var ${prefix})
REP_EXECINSTALL=$(svm_expand_var ${exec_prefix})
REP_DONNEES=$(svm_expand_var ${datadir})/${PACKAGE}
REP_DOCUMENTATION=$(svm_expand_var ${mandir})
MACHINE_PLUGINS=$(svm_expand_var ${MACHINE_PLUGINS})
SYSTEM_PLUGINS=$(svm_expand_var ${libdir})
exec_prefix=$svm_old_exec_prefix
unset svm_old_exec_prefix
prefix=$svm_old_prefix
unset svm_old_prefix
AC_DEFINE_UNQUOTED([REP_BINAIRE],["$REP_BINAIRE"],[Binary installation directory])
AC_DEFINE_UNQUOTED([EXT_BINAIRE],["$EXT_BINAIRE"],[Binary installation suffix])
AC_DEFINE_UNQUOTED([REP_INCLUDE],["$REP_INCLUDE"],[Include directory])
AC_DEFINE_UNQUOTED([REP_INTERFACE],["$REP_INTERFACE"],[Interface library directory])
AC_DEFINE_UNQUOTED([REP_INSTALL],["$REP_INSTALL"],[Installation root directory])
AC_DEFINE_UNQUOTED([REP_EXECINSTALL],["$REP_EXECINSTALL"],[Installation execution root directory])
AC_DEFINE_UNQUOTED([REP_DONNEES],["$REP_DONNEES"],[Installation data directory])
AC_DEFINE_UNQUOTED([REP_DOCUMENTATION],["$REP_DOCUMENTATION"],[Installation documentation directory])
AC_DEFINE_UNQUOTED([MACHINE_PLUGINS],["$MACHINE_PLUGINS"],[Machine plugins root directory])
AC_DEFINE_UNQUOTED([SYSTEM_PLUGINS],["$SYSTEM_PLUGINS"],[System plugins directory])

AC_MSG_CHECKING([where will be the debug website])
AC_DEFINE_UNQUOTED([DEBUGGER_SITE],["$DEBUGGER_SITE"],[Root directory of debug website])
AC_MSG_RESULT([$DEBUGGER_SITE])

AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])
AC_SUBST([LIBS])
AC_SUBST([DIFF])
AC_SUBST([TESTS_ENVIRONMENT])
AC_SUBST([PLUGINSSTRICTCHECK])
AC_SUBST([REACTIVE_SIGNAL])
AC_SUBST([DEFAULT_STYLE])
AC_SUBST([REP_BINAIRE])
AC_SUBST([EXT_BINAIRE])
AC_SUBST([REP_INCLUDE])
AC_SUBST([REP_INTERFACE])
AC_SUBST([REP_INSTALL])
AC_SUBST([REP_EXECINSTALL])
AC_SUBST([REP_DONNEES])
AC_SUBST([REP_DOCUMENTATION])
AC_SUBST([MACHINE_PLUGINS])
AC_SUBST([SYSTEM_PLUGINS])

AC_CONFIG_FILES([
doc/editeurs/Makefile
doc/editeurs/vim/Makefile
doc/Makefile
doc/man/Makefile
debug/Makefile
debug/styles/Makefile
debug/styles/default/Makefile
debug/styles/rounded/Makefile
Makefile
src/demarrage/binaire/Makefile
src/demarrage/lanceur/analyseur/Makefile
src/demarrage/lanceur/Makefile
src/demarrage/Makefile
src/extensions/Makefile
src/global/Makefile
src/machine/performance/Makefile
src/machine/debugueur/analyseur/Makefile
src/machine/debugueur/Makefile
src/machine/elements/base/Makefile
src/machine/elements/Makefile
src/machine/elements/memoire/Makefile
src/machine/elements/noyau/Makefile
src/machine/elements/ordonnanceur/Makefile
src/machine/elements/processeur/instructions/Makefile
src/machine/elements/processeur/Makefile
src/machine/elements/processus/Makefile
src/machine/elements/programme/analyseur/Makefile
src/machine/elements/programme/Makefile
src/machine/elements/sequenceur/Makefile
src/machine/elements/synchronisation/Makefile
src/machine/elements/systeme/Makefile
src/machine/elements/valeur/Makefile
src/machine/extensions/analyseur/Makefile
src/machine/extensions/gestionnaire/Makefile
src/machine/extensions/Makefile
src/machine/interface/Makefile
src/machine/interruption/Makefile
src/machine/machine/Makefile
src/machine/Makefile
src/outils/analyseur/Makefile
src/outils/Makefile
src/Makefile
])

AC_OUTPUT

