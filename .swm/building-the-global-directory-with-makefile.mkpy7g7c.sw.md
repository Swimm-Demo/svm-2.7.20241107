---
title: Building the Global Directory with Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/global/](src/global/)</SwmPath> directory to build the project components.

<SwmSnippet path="/src/global/Makefile.in" line="1">

---

# <SwmPath>[Makefile.in](Makefile.in)</SwmPath> Header

The <SwmPath>[Makefile.in](Makefile.in)</SwmPath> is generated by automake and includes licensing information and a notice about the Free Software Foundation.

```in
# Makefile.in generated by automake 1.15 from Makefile.am.
# @configure_input@

# Copyright (C) 1994-2014 Free Software Foundation, Inc.

# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

@SET_MAKE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.in" line="36">

---

# Setting Make Variables

Defines the <SwmToken path="src/global/Makefile.in" pos="36:0:0" line-data="VPATH = @srcdir@">`VPATH`</SwmToken> variable and checks if GNU make is being used.

```in
VPATH = @srcdir@
am__is_gnu_make = { \
  if test -z '$(MAKELEVEL)'; then \
    false; \
  elif test -n '$(MAKE_HOST)'; then \
    true; \
  elif test -n '$(MAKE_VERSION)' && test -n '$(CURDIR)'; then \
    true; \
  else \
    false; \
  fi; \
}
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.in" line="92">

---

# Package Directories

Defines directories for package data, include files, libraries, and executable files.

```in
pkgdatadir = $(datadir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkglibexecdir = $(libexecdir)/@PACKAGE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.in" line="97">

---

# Installation Commands

Defines commands for installing data, programs, and scripts.

```in
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.in" line="108">

---

# Build Triplets

Defines the build, host, and target triplets for cross-compilation.

```in
build_triplet = @build@
host_triplet = @host@
target_triplet = @target@
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.in" line="111">

---

# Subdirectory and Dependencies

Specifies the subdirectory and dependencies for the <SwmToken path="src/global/Makefile.in" pos="1:2:2" line-data="# Makefile.in generated by automake 1.15 from Makefile.am.">`Makefile`</SwmToken>.

```in
subdir = src/global
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
am__aclocal_m4_deps = $(top_srcdir)/m4/libtool.m4 \
	$(top_srcdir)/m4/ltoptions.m4 $(top_srcdir)/m4/ltsugar.m4 \
	$(top_srcdir)/m4/ltversion.m4 $(top_srcdir)/m4/lt~obsolete.m4 \
	$(top_srcdir)/acinclude.m4 $(top_srcdir)/configure.ac
am__configure_deps = $(am__aclocal_m4_deps) $(CONFIGURE_DEPENDENCIES) \
	$(ACLOCAL_M4)
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.in" line="124">

---

# Library Configuration

Defines the library to be built (<SwmToken path="src/global/Makefile.am" pos="24:2:4" line-data="noinst_LTLIBRARIES=libglobal.la">`libglobal.la`</SwmToken>) and its source files.

```in
LTLIBRARIES = $(noinst_LTLIBRARIES)
libglobal_la_DEPENDENCIES =
am_libglobal_la_OBJECTS = global.lo installation.lo systeme.lo
libglobal_la_OBJECTS = $(am_libglobal_la_OBJECTS)
AM_V_lt = $(am__v_lt_@AM_V@)
am__v_lt_ = $(am__v_lt_@AM_DEFAULT_V@)
am__v_lt_0 = --silent
am__v_lt_1 = 
libglobal_la_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CXXLD) $(AM_CXXFLAGS) \
	$(CXXFLAGS) $(libglobal_la_LDFLAGS) $(LDFLAGS) -o $@
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.in" line="151">

---

# Compilation and Linking

Specifies the commands for compiling and linking C++ source files.

```in
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CXXFLAGS) $(CXXFLAGS)
AM_V_CXX = $(am__v_CXX_@AM_V@)
am__v_CXX_ = $(am__v_CXX_@AM_DEFAULT_V@)
am__v_CXX_0 = @echo "  CXX     " $@;
am__v_CXX_1 = 
CXXLD = $(CXX)
CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CXXLD) $(AM_CXXFLAGS) \
	$(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.in" line="189">

---

# Recursive Targets

Defines recursive targets for building, checking, and installing the project.

```in
RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \
	ctags-recursive dvi-recursive html-recursive info-recursive \
	install-data-recursive install-dvi-recursive \
	install-exec-recursive install-html-recursive \
	install-info-recursive install-pdf-recursive \
	install-ps-recursive install-recursive installcheck-recursive \
	installdirs-recursive pdf-recursive ps-recursive \
	tags-recursive uninstall-recursive
am__can_run_installinfo = \
  case $$AM_UPDATE_INFO_DIR in \
    n|no|NO) false;; \
    *) (install-info --version) >/dev/null 2>&1;; \
  esac
RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\
  distclean-recursive maintainer-clean-recursive
am__recursive_targets = \
  $(RECURSIVE_TARGETS) \
  $(RECURSIVE_CLEAN_TARGETS) \
  $(am__extra_recursive_targets)
AM_RECURSIVE_TARGETS = $(am__recursive_targets:-recursive=) TAGS CTAGS \
	distdir
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.in" line="202">

---

# Clean Targets

Specifies targets for cleaning up build artifacts.

```in
RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\
  distclean-recursive maintainer-clean-recursive
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.in" line="228">

---

# Tagging and Distribution

Defines targets for generating tags and preparing distribution files.

```in
CTAGS = ctags
DIST_SUBDIRS = $(SUBDIRS)
am__DIST_COMMON = $(srcdir)/Makefile.in $(top_srcdir)/depcomp
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.am" line="1">

---

# <SwmPath>[Makefile.am](Makefile.am)</SwmPath> Header

The <SwmPath>[Makefile.am](Makefile.am)</SwmPath> includes licensing information and a notice about the Free Software Foundation.

```am
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
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.am" line="20">

---

# Compiler Flags

Sets the compiler flags for the project.

```am
AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.am" line="22">

---

# Subdirectories

Specifies the subdirectories to be included in the build.

```am
SUBDIRS= 
```

---

</SwmSnippet>

<SwmSnippet path="/src/global/Makefile.am" line="24">

---

# Library Configuration

Defines the library to be built (<SwmToken path="src/global/Makefile.am" pos="24:2:4" line-data="noinst_LTLIBRARIES=libglobal.la">`libglobal.la`</SwmToken>) and its source files.

```am
noinst_LTLIBRARIES=libglobal.la

libglobal_la_SOURCES=global.cpp global.h installation.cpp installation.h systeme.cpp systeme.h
libglobal_la_LIBADD=
libglobal_la_LDFLAGS=-no-undefined
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>