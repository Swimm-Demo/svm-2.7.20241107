---
title: Building the Interruption Component with Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/machine/interruption/](src/machine/interruption/)</SwmPath> directory to build the interruption component of the Simple Virtual Machine (SVM) project.

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="1">

---

# <SwmPath>[Makefile.in](Makefile.in)</SwmPath> Header

The <SwmPath>[Makefile.in](Makefile.in)</SwmPath> file is generated by automake and includes licensing information and a disclaimer of warranty.

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
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="15">

---

# Setting Make

The <SwmToken path="src/machine/interruption/Makefile.in" pos="15:0:2" line-data="@SET_MAKE@">`@SET_MAKE@`</SwmToken> macro is used to ensure that the correct version of <SwmToken path="src/machine/interruption/Makefile.in" pos="496:11:11" line-data="# into them and run &#39;make&#39; without going through this Makefile.">`make`</SwmToken> is used.

```in
@SET_MAKE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="36">

---

# Directory Variables

The <SwmToken path="src/machine/interruption/Makefile.in" pos="36:0:0" line-data="VPATH = @srcdir@">`VPATH`</SwmToken> variable is set to <SwmToken path="src/machine/interruption/Makefile.in" pos="36:4:6" line-data="VPATH = @srcdir@">`@srcdir@`</SwmToken>, which specifies the directory where the source files are located.

```in
VPATH = @srcdir@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="92">

---

# Package Directories

Variables like <SwmToken path="src/machine/interruption/Makefile.in" pos="92:0:0" line-data="pkgdatadir = $(datadir)/@PACKAGE@">`pkgdatadir`</SwmToken>, <SwmToken path="src/machine/interruption/Makefile.in" pos="93:0:0" line-data="pkgincludedir = $(includedir)/@PACKAGE@">`pkgincludedir`</SwmToken>, <SwmToken path="src/machine/interruption/Makefile.in" pos="94:0:0" line-data="pkglibdir = $(libdir)/@PACKAGE@">`pkglibdir`</SwmToken>, and <SwmToken path="src/machine/interruption/Makefile.in" pos="95:0:0" line-data="pkglibexecdir = $(libexecdir)/@PACKAGE@">`pkglibexecdir`</SwmToken> define the directories for package data, include files, libraries, and executable libraries respectively.

```in
pkgdatadir = $(datadir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkglibexecdir = $(libexecdir)/@PACKAGE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="108">

---

# Build Triplets

The <SwmToken path="src/machine/interruption/Makefile.in" pos="108:0:0" line-data="build_triplet = @build@">`build_triplet`</SwmToken>, <SwmToken path="src/machine/interruption/Makefile.in" pos="109:0:0" line-data="host_triplet = @host@">`host_triplet`</SwmToken>, and <SwmToken path="src/machine/interruption/Makefile.in" pos="110:0:0" line-data="target_triplet = @target@">`target_triplet`</SwmToken> variables define the system triplets for building, hosting, and targeting the build.

```in
build_triplet = @build@
host_triplet = @host@
target_triplet = @target@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="111">

---

# Subdirectory

The <SwmToken path="src/machine/interruption/Makefile.in" pos="111:0:0" line-data="subdir = src/machine/interruption">`subdir`</SwmToken> variable is set to <SwmPath>[src/machine/interruption/](src/machine/interruption/)</SwmPath>, indicating the subdirectory for this Makefile.

```in
subdir = src/machine/interruption
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="119">

---

# Common Distribution Files

The <SwmToken path="src/machine/interruption/Makefile.in" pos="119:0:0" line-data="DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)">`DIST_COMMON`</SwmToken> variable includes common distribution files such as <SwmPath>[Makefile.am](Makefile.am)</SwmPath> and other dependencies.

```in
DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="124">

---

# Library Definitions

The <SwmToken path="src/machine/interruption/Makefile.in" pos="124:0:0" line-data="LTLIBRARIES = $(noinst_LTLIBRARIES)">`LTLIBRARIES`</SwmToken> variable defines the libraries to be built, and <SwmToken path="src/machine/interruption/Makefile.in" pos="127:0:0" line-data="libmachineinterruption_la_OBJECTS =  \">`libmachineinterruption_la_OBJECTS`</SwmToken> lists the object files for the <SwmToken path="src/machine/interruption/Makefile.am" pos="24:2:2" line-data="noinst_LTLIBRARIES=libmachineinterruption.la">`libmachineinterruption`</SwmToken> library.

```in
LTLIBRARIES = $(noinst_LTLIBRARIES)
libmachineinterruption_la_DEPENDENCIES =
am_libmachineinterruption_la_OBJECTS = interruption.lo
libmachineinterruption_la_OBJECTS =  \
	$(am_libmachineinterruption_la_OBJECTS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="133">

---

# Linking Libraries

The <SwmToken path="src/machine/interruption/Makefile.in" pos="133:0:0" line-data="libmachineinterruption_la_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX \">`libmachineinterruption_la_LINK`</SwmToken> variable defines the linking command for the <SwmToken path="src/machine/interruption/Makefile.am" pos="24:2:2" line-data="noinst_LTLIBRARIES=libmachineinterruption.la">`libmachineinterruption`</SwmToken> library using <SwmToken path="src/machine/interruption/Makefile.in" pos="113:11:11" line-data="am__aclocal_m4_deps = $(top_srcdir)/m4/libtool.m4 \">`libtool`</SwmToken>.

```in
libmachineinterruption_la_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX \
	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link $(CXXLD) \
	$(AM_CXXFLAGS) $(CXXFLAGS) \
	$(libmachineinterruption_la_LDFLAGS) $(LDFLAGS) -o $@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="153">

---

# Compilation Rules

The <SwmToken path="src/machine/interruption/Makefile.in" pos="153:0:0" line-data="CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \">`CXXCOMPILE`</SwmToken> and <SwmToken path="src/machine/interruption/Makefile.in" pos="155:0:0" line-data="LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`LTCXXCOMPILE`</SwmToken> variables define the compilation commands for C++ source files and libtool C++ source files respectively.

```in
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CXXFLAGS) $(CXXFLAGS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="414">

---

# Suffix Rules

The <SwmToken path="src/machine/interruption/Makefile.in" pos="414:0:1" line-data=".SUFFIXES:">`.SUFFIXES`</SwmToken> rule defines the file suffixes that <SwmToken path="src/machine/interruption/Makefile.in" pos="496:11:11" line-data="# into them and run &#39;make&#39; without going through this Makefile.">`make`</SwmToken> will recognize, including <SwmToken path="src/machine/interruption/Makefile.in" pos="415:4:5" line-data=".SUFFIXES: .cpp .lo .o .obj">`.cpp`</SwmToken>, <SwmToken path="src/machine/interruption/Makefile.in" pos="415:7:8" line-data=".SUFFIXES: .cpp .lo .o .obj">`.lo`</SwmToken>, <SwmToken path="src/machine/interruption/Makefile.in" pos="415:10:11" line-data=".SUFFIXES: .cpp .lo .o .obj">`.o`</SwmToken>, and <SwmToken path="src/machine/interruption/Makefile.in" pos="415:13:14" line-data=".SUFFIXES: .cpp .lo .o .obj">`.obj`</SwmToken>.

```in
.SUFFIXES:
.SUFFIXES: .cpp .lo .o .obj
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="416">

---

# Makefile Generation

The rule for generating <SwmToken path="src/machine/interruption/Makefile.in" pos="416:5:5" line-data="$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)">`Makefile`</SwmToken> from <SwmPath>[Makefile.in](Makefile.in)</SwmPath> and <SwmToken path="src/machine/interruption/Makefile.in" pos="428:17:19" line-data="Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status">`config.status`</SwmToken> ensures that the Makefile is up-to-date with the configuration.

```in
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)
	@for dep in $?; do \
	  case '$(am__configure_deps)' in \
	    *$$dep*) \
	      ( cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh ) \
	        && { if test -f $@; then exit 0; else break; fi; }; \
	      exit 1;; \
	  esac; \
	done; \
	echo ' cd $(top_srcdir) && $(AUTOMAKE) --gnu src/machine/interruption/Makefile'; \
	$(am__cd) $(top_srcdir) && \
	  $(AUTOMAKE) --gnu src/machine/interruption/Makefile
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="446">

---

# Cleaning Rules

The <SwmToken path="src/machine/interruption/Makefile.in" pos="446:0:2" line-data="clean-noinstLTLIBRARIES:">`clean-noinstLTLIBRARIES`</SwmToken> rule removes the non-installed libraries during the clean process.

```in
clean-noinstLTLIBRARIES:
	-test -z "$(noinst_LTLIBRARIES)" || rm -f $(noinst_LTLIBRARIES)
	@list='$(noinst_LTLIBRARIES)'; \
	locs=`for p in $$list; do echo $$p; done | \
	      sed 's|^[^/]*$$|.|; s|/[^/]*$$||; s|$$|/so_locations|' | \
	      sort -u`; \
	test -z "$$locs" || { \
	  echo rm -f $${locs}; \
	  rm -f $${locs}; \
	}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="460">

---

# Compilation Cleanup

The <SwmToken path="src/machine/interruption/Makefile.in" pos="460:0:2" line-data="mostlyclean-compile:">`mostlyclean-compile`</SwmToken> and <SwmToken path="src/machine/interruption/Makefile.in" pos="463:0:2" line-data="distclean-compile:">`distclean-compile`</SwmToken> rules remove compiled object files and other generated files.

```in
mostlyclean-compile:
	-rm -f *.$(OBJEXT)

distclean-compile:
	-rm -f *.tab.c
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="529">

---

# Tagging and Distribution

The <SwmToken path="src/machine/interruption/Makefile.in" pos="531:0:0" line-data="tags: tags-recursive">`tags`</SwmToken>, <SwmToken path="src/machine/interruption/Makefile.in" pos="532:0:0" line-data="TAGS: tags">`TAGS`</SwmToken>, <SwmToken path="src/machine/interruption/Makefile.in" pos="192:1:1" line-data="	ctags-recursive dvi-recursive html-recursive info-recursive \">`ctags`</SwmToken>, and <SwmToken path="src/machine/interruption/Makefile.in" pos="210:14:14" line-data="AM_RECURSIVE_TARGETS = $(am__recursive_targets:-recursive=) TAGS CTAGS \">`CTAGS`</SwmToken> rules generate tags for source code navigation, and the <SwmToken path="src/machine/interruption/Makefile.in" pos="211:1:1" line-data="	distdir">`distdir`</SwmToken> rule prepares the distribution directory.

```in
ID: $(am__tagged_files)
	$(am__define_uniq_tagged_files); mkid -fID $$unique
tags: tags-recursive
TAGS: tags

tags-am: $(TAGS_DEPENDENCIES) $(am__tagged_files)
	set x; \
	here=`pwd`; \
	if ($(ETAGS) --etags-include --version) >/dev/null 2>&1; then \
	  include_option=--etags-include; \
	  empty_fix=.; \
	else \
	  include_option=--include; \
	  empty_fix=; \
	fi; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  if test "$$subdir" = .; then :; else \
	    test ! -f $$subdir/TAGS || \
	      set "$$@" "$$include_option=$$here/$$subdir/TAGS"; \
	  fi; \
	done; \
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.in" line="654">

---

# Installation Rules

The <SwmToken path="src/machine/interruption/Makefile.in" pos="654:0:0" line-data="install: install-recursive">`install`</SwmToken> and <SwmToken path="src/machine/interruption/Makefile.in" pos="659:0:2" line-data="install-am: all-am">`install-am`</SwmToken> rules handle the installation of the built components.

```in
install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.am" line="1">

---

# <SwmPath>[Makefile.am](Makefile.am)</SwmPath> Header

The <SwmPath>[Makefile.am](Makefile.am)</SwmPath> file includes licensing information and a disclaimer of warranty.

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

<SwmSnippet path="/src/machine/interruption/Makefile.am" line="20">

---

# Compiler Flags

The <SwmToken path="src/machine/interruption/Makefile.am" pos="20:0:0" line-data="AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}">`AM_CPPFLAGS`</SwmToken> variable sets the preprocessor flags for the compiler, including the top build and source directories.

```am
AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.am" line="22">

---

# Subdirectories

The <SwmToken path="src/machine/interruption/Makefile.am" pos="22:0:0" line-data="SUBDIRS= ">`SUBDIRS`</SwmToken> variable is empty, indicating that there are no subdirectories to process.

```am
SUBDIRS= 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/interruption/Makefile.am" line="24">

---

# Library Definitions

The <SwmToken path="src/machine/interruption/Makefile.am" pos="24:0:0" line-data="noinst_LTLIBRARIES=libmachineinterruption.la">`noinst_LTLIBRARIES`</SwmToken> variable defines the non-installed libraries, and <SwmToken path="src/machine/interruption/Makefile.am" pos="26:0:0" line-data="libmachineinterruption_la_SOURCES=interruption.cpp interruption.h ">`libmachineinterruption_la_SOURCES`</SwmToken> lists the source files for the <SwmToken path="src/machine/interruption/Makefile.am" pos="24:2:2" line-data="noinst_LTLIBRARIES=libmachineinterruption.la">`libmachineinterruption`</SwmToken> library.

```am
noinst_LTLIBRARIES=libmachineinterruption.la

libmachineinterruption_la_SOURCES=interruption.cpp interruption.h 
libmachineinterruption_la_LIBADD=
libmachineinterruption_la_LDFLAGS=-no-undefined
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>