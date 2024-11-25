---
title: Building the Processus Directory with Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/machine/elements/processus/](src/machine/elements/processus/)</SwmPath> directory to build the project components.

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="1">

---

# <SwmPath>[Makefile.in](Makefile.in)</SwmPath> Overview

The <SwmPath>[Makefile.in](Makefile.in)</SwmPath> is generated by automake and includes licensing information and general setup for the build process.

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

# 
# Simple Virtual Machine - A versatile and robust architecture to
# easily write applications.
# Copyright (C) 2021  Julien BRUGUIER
# 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="36">

---

# Setting VPATH

The <SwmToken path="src/machine/elements/processus/Makefile.in" pos="36:0:0" line-data="VPATH = @srcdir@">`VPATH`</SwmToken> variable is set to <SwmToken path="src/machine/elements/processus/Makefile.in" pos="36:4:6" line-data="VPATH = @srcdir@">`@srcdir@`</SwmToken>, which specifies the directory where the source files are located.

```in
VPATH = @srcdir@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="37">

---

# GNU Make Check

The <SwmToken path="src/machine/elements/processus/Makefile.in" pos="37:0:0" line-data="am__is_gnu_make = { \">`am__is_gnu_make`</SwmToken> variable checks if the current make tool is GNU make.

```in
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

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="48">

---

# Make Running with Option

The <SwmToken path="src/machine/elements/processus/Makefile.in" pos="48:0:0" line-data="am__make_running_with_option = \">`am__make_running_with_option`</SwmToken> variable handles different make options and flags.

```in
am__make_running_with_option = \
  case $${target_option-} in \
      ?) ;; \
      *) echo "am__make_running_with_option: internal error: invalid" \
              "target option '$${target_option-}' specified" >&2; \
         exit 1;; \
  esac; \
  has_opt=no; \
  sane_makeflags=$$MAKEFLAGS; \
  if $(am__is_gnu_make); then \
    sane_makeflags=$$MFLAGS; \
  else \
    case $$MAKEFLAGS in \
      *\\[\ \	]*) \
        bs=\\; \
        sane_makeflags=`printf '%s\n' "$$MAKEFLAGS" \
          | sed "s/$$bs$$bs[$$bs $$bs	]*//g"`;; \
    esac; \
  fi; \
  skip_next=no; \
  strip_trailopt () \
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="92">

---

# Package Directories

Variables like <SwmToken path="src/machine/elements/processus/Makefile.in" pos="92:0:0" line-data="pkgdatadir = $(datadir)/@PACKAGE@">`pkgdatadir`</SwmToken>, <SwmToken path="src/machine/elements/processus/Makefile.in" pos="93:0:0" line-data="pkgincludedir = $(includedir)/@PACKAGE@">`pkgincludedir`</SwmToken>, <SwmToken path="src/machine/elements/processus/Makefile.in" pos="94:0:0" line-data="pkglibdir = $(libdir)/@PACKAGE@">`pkglibdir`</SwmToken>, and <SwmToken path="src/machine/elements/processus/Makefile.in" pos="95:0:0" line-data="pkglibexecdir = $(libexecdir)/@PACKAGE@">`pkglibexecdir`</SwmToken> define the directories for package data, includes, libraries, and executables.

```in
pkgdatadir = $(datadir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkglibexecdir = $(libexecdir)/@PACKAGE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="97">

---

# Installation Commands

Commands like <SwmToken path="src/machine/elements/processus/Makefile.in" pos="97:0:0" line-data="install_sh_DATA = $(install_sh) -c -m 644">`install_sh_DATA`</SwmToken>, <SwmToken path="src/machine/elements/processus/Makefile.in" pos="98:0:0" line-data="install_sh_PROGRAM = $(install_sh) -c">`install_sh_PROGRAM`</SwmToken>, and <SwmToken path="src/machine/elements/processus/Makefile.in" pos="99:0:0" line-data="install_sh_SCRIPT = $(install_sh) -c">`install_sh_SCRIPT`</SwmToken> are used for installing data, programs, and scripts.

```in
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="108">

---

# Build Triplets

Variables <SwmToken path="src/machine/elements/processus/Makefile.in" pos="108:0:0" line-data="build_triplet = @build@">`build_triplet`</SwmToken>, <SwmToken path="src/machine/elements/processus/Makefile.in" pos="109:0:0" line-data="host_triplet = @host@">`host_triplet`</SwmToken>, and <SwmToken path="src/machine/elements/processus/Makefile.in" pos="110:0:0" line-data="target_triplet = @target@">`target_triplet`</SwmToken> define the build, host, and target system triplets.

```in
build_triplet = @build@
host_triplet = @host@
target_triplet = @target@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="111">

---

# Subdirectory and Dependencies

The <SwmToken path="src/machine/elements/processus/Makefile.in" pos="111:0:0" line-data="subdir = src/machine/elements/processus">`subdir`</SwmToken> variable specifies the current subdirectory, and <SwmToken path="src/machine/elements/processus/Makefile.in" pos="113:0:0" line-data="am__aclocal_m4_deps = $(top_srcdir)/m4/libtool.m4 \">`am__aclocal_m4_deps`</SwmToken> lists dependencies for <SwmPath>[aclocal.m4](aclocal.m4)</SwmPath>.

```in
subdir = src/machine/elements/processus
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

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="119">

---

# Common Distribution Files

The <SwmToken path="src/machine/elements/processus/Makefile.in" pos="119:0:0" line-data="DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)">`DIST_COMMON`</SwmToken> variable lists common files for distribution, including <SwmPath>[Makefile.am](Makefile.am)</SwmPath>.

```in
DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="124">

---

# Library Definitions

The <SwmToken path="src/machine/elements/processus/Makefile.in" pos="124:0:0" line-data="LTLIBRARIES = $(noinst_LTLIBRARIES)">`LTLIBRARIES`</SwmToken> variable defines libraries, and <SwmToken path="src/machine/elements/processus/Makefile.in" pos="128:0:0" line-data="libmachineelementsprocessus_la_OBJECTS =  \">`libmachineelementsprocessus_la_OBJECTS`</SwmToken> lists object files for the <SwmToken path="src/machine/elements/processus/Makefile.am" pos="24:2:4" line-data="noinst_LTLIBRARIES=libmachineelementsprocessus.la">`libmachineelementsprocessus.la`</SwmToken> library.

```in
LTLIBRARIES = $(noinst_LTLIBRARIES)
libmachineelementsprocessus_la_DEPENDENCIES =
am_libmachineelementsprocessus_la_OBJECTS = etat_processus.lo \
	processus.lo
libmachineelementsprocessus_la_OBJECTS =  \
	$(am_libmachineelementsprocessus_la_OBJECTS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="154">

---

# Compilation and Linking

Variables like <SwmToken path="src/machine/elements/processus/Makefile.in" pos="154:0:0" line-data="CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \">`CXXCOMPILE`</SwmToken>, <SwmToken path="src/machine/elements/processus/Makefile.in" pos="156:0:0" line-data="LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`LTCXXCOMPILE`</SwmToken>, <SwmToken path="src/machine/elements/processus/Makefile.in" pos="164:0:0" line-data="CXXLD = $(CXX)">`CXXLD`</SwmToken>, and <SwmToken path="src/machine/elements/processus/Makefile.in" pos="165:0:0" line-data="CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`CXXLINK`</SwmToken> define commands for compiling and linking C++ source files.

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

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="192">

---

# Recursive Targets

The <SwmToken path="src/machine/elements/processus/Makefile.in" pos="192:0:0" line-data="RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \">`RECURSIVE_TARGETS`</SwmToken> variable lists targets that should be processed recursively, such as <SwmToken path="src/machine/elements/processus/Makefile.in" pos="192:4:6" line-data="RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \">`all-recursive`</SwmToken> and <SwmToken path="src/machine/elements/processus/Makefile.in" pos="197:7:9" line-data="	install-ps-recursive install-recursive installcheck-recursive \">`install-recursive`</SwmToken>.

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

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="205">

---

# Clean Targets

The <SwmToken path="src/machine/elements/processus/Makefile.in" pos="205:0:0" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`RECURSIVE_CLEAN_TARGETS`</SwmToken> variable lists targets for cleaning up files, such as <SwmToken path="src/machine/elements/processus/Makefile.in" pos="205:8:10" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`clean-recursive`</SwmToken> and <SwmToken path="src/machine/elements/processus/Makefile.in" pos="206:1:3" line-data="  distclean-recursive maintainer-clean-recursive">`distclean-recursive`</SwmToken>.

```in
RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\
  distclean-recursive maintainer-clean-recursive
am__recursive_targets = \
  $(RECURSIVE_TARGETS) \
  $(RECURSIVE_CLEAN_TARGETS) \
  $(am__extra_recursive_targets)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.in" line="231">

---

# Tagging and Distribution

Variables like <SwmToken path="src/machine/elements/processus/Makefile.in" pos="231:0:0" line-data="CTAGS = ctags">`CTAGS`</SwmToken>, <SwmToken path="src/machine/elements/processus/Makefile.in" pos="232:0:0" line-data="DIST_SUBDIRS = $(SUBDIRS)">`DIST_SUBDIRS`</SwmToken>, and <SwmToken path="src/machine/elements/processus/Makefile.in" pos="234:0:0" line-data="DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)">`DISTFILES`</SwmToken> define commands for tagging files and listing files for distribution.

```in
CTAGS = ctags
DIST_SUBDIRS = $(SUBDIRS)
am__DIST_COMMON = $(srcdir)/Makefile.in $(top_srcdir)/depcomp
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.am" line="1">

---

# <SwmPath>[Makefile.am](Makefile.am)</SwmPath> Overview

The <SwmPath>[Makefile.am](Makefile.am)</SwmPath> includes licensing information and general setup for the build process.

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
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.am" line="20">

---

# Compiler Flags

The <SwmToken path="src/machine/elements/processus/Makefile.am" pos="20:0:0" line-data="AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}">`AM_CPPFLAGS`</SwmToken> variable sets the compiler flags for the C++ preprocessor.

```am
AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.am" line="22">

---

# Subdirectories

The <SwmToken path="src/machine/elements/processus/Makefile.am" pos="22:0:0" line-data="SUBDIRS= ">`SUBDIRS`</SwmToken> variable lists subdirectories to be processed.

```am
SUBDIRS= 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processus/Makefile.am" line="24">

---

# Library Definitions

The <SwmToken path="src/machine/elements/processus/Makefile.am" pos="24:0:0" line-data="noinst_LTLIBRARIES=libmachineelementsprocessus.la">`noinst_LTLIBRARIES`</SwmToken> variable defines libraries, and <SwmToken path="src/machine/elements/processus/Makefile.am" pos="26:0:0" line-data="libmachineelementsprocessus_la_SOURCES=etat_processus.cpp etat_processus.h processus.cpp processus.h ">`libmachineelementsprocessus_la_SOURCES`</SwmToken> lists source files for the <SwmToken path="src/machine/elements/processus/Makefile.am" pos="24:2:4" line-data="noinst_LTLIBRARIES=libmachineelementsprocessus.la">`libmachineelementsprocessus.la`</SwmToken> library.

```am
noinst_LTLIBRARIES=libmachineelementsprocessus.la

libmachineelementsprocessus_la_SOURCES=etat_processus.cpp etat_processus.h processus.cpp processus.h 
libmachineelementsprocessus_la_LIBADD=
libmachineelementsprocessus_la_LDFLAGS=-no-undefined
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
