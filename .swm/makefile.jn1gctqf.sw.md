---
title: Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/machine/elements/synchronisation/](src/machine/elements/synchronisation/)</SwmPath> directory. It will go through the configuration steps in the <SwmPath>[Makefile.am](Makefile.am)</SwmPath> and <SwmPath>[Makefile.in](Makefile.in)</SwmPath> files.

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.am" line="1">

---

# <SwmPath>[Makefile.am](Makefile.am)</SwmPath> Configuration

The <SwmPath>[Makefile.am](Makefile.am)</SwmPath> file starts with the copyright and license information for the Simple Virtual Machine project.

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

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.am" line="20">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.am" pos="20:0:0" line-data="AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}">`AM_CPPFLAGS`</SwmToken> variable is set to include the top build and source directories.

```am
AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.am" line="22">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.am" pos="22:0:0" line-data="SUBDIRS= ">`SUBDIRS`</SwmToken> variable is set to an empty value, indicating there are no subdirectories to process.

```am
SUBDIRS= 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.am" line="24">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.am" pos="24:0:0" line-data="noinst_LTLIBRARIES=libmachineelementssynchronisation.la">`noinst_LTLIBRARIES`</SwmToken> variable specifies that the library <SwmToken path="src/machine/elements/synchronisation/Makefile.am" pos="24:2:4" line-data="noinst_LTLIBRARIES=libmachineelementssynchronisation.la">`libmachineelementssynchronisation.la`</SwmToken> is not to be installed.

```am
noinst_LTLIBRARIES=libmachineelementssynchronisation.la
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.am" line="26">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.am" pos="26:0:0" line-data="libmachineelementssynchronisation_la_SOURCES=acces.cpp acces.h evenement.cpp evenement.h ">`libmachineelementssynchronisation_la_SOURCES`</SwmToken> variable lists the source files for the library: <SwmPath>[src/machine/elements/synchronisation/acces.cpp](src/machine/elements/synchronisation/acces.cpp)</SwmPath>, <SwmPath>[src/machine/elements/synchronisation/acces.h](src/machine/elements/synchronisation/acces.h)</SwmPath>, <SwmPath>[src/machine/elements/synchronisation/evenement.cpp](src/machine/elements/synchronisation/evenement.cpp)</SwmPath>, and <SwmPath>[src/machine/debugueur/evenement.h](src/machine/debugueur/evenement.h)</SwmPath>.

```am
libmachineelementssynchronisation_la_SOURCES=acces.cpp acces.h evenement.cpp evenement.h 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.am" line="27">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.am" pos="27:0:0" line-data="libmachineelementssynchronisation_la_LIBADD=">`libmachineelementssynchronisation_la_LIBADD`</SwmToken> variable is empty, indicating no additional libraries are linked.

```am
libmachineelementssynchronisation_la_LIBADD=
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.am" line="28">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.am" pos="28:0:0" line-data="libmachineelementssynchronisation_la_LDFLAGS=-no-undefined">`libmachineelementssynchronisation_la_LDFLAGS`</SwmToken> variable is set to <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="413:4:7" line-data="libmachineelementssynchronisation_la_LDFLAGS = -no-undefined">`-no-undefined`</SwmToken>, ensuring that all symbols are defined.

```am
libmachineelementssynchronisation_la_LDFLAGS=-no-undefined
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="1">

---

# <SwmPath>[Makefile.in](Makefile.in)</SwmPath> Configuration

The <SwmPath>[Makefile.in](Makefile.in)</SwmPath> file starts with the copyright and license information, similar to <SwmPath>[Makefile.am](Makefile.am)</SwmPath>. It also includes a notice that it was generated by automake <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="1:12:14" line-data="# Makefile.in generated by automake 1.15 from Makefile.am.">`1.15`</SwmToken> from <SwmPath>[Makefile.am](Makefile.am)</SwmPath>.

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

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="36">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="36:0:0" line-data="VPATH = @srcdir@">`VPATH`</SwmToken> variable is set to <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="36:4:6" line-data="VPATH = @srcdir@">`@srcdir@`</SwmToken>, which will be replaced by the source directory path during configuration.

```in
VPATH = @srcdir@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="92">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="92:0:0" line-data="pkgdatadir = $(datadir)/@PACKAGE@">`pkgdatadir`</SwmToken>, <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="93:0:0" line-data="pkgincludedir = $(includedir)/@PACKAGE@">`pkgincludedir`</SwmToken>, <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="94:0:0" line-data="pkglibdir = $(libdir)/@PACKAGE@">`pkglibdir`</SwmToken>, and <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="95:0:0" line-data="pkglibexecdir = $(libexecdir)/@PACKAGE@">`pkglibexecdir`</SwmToken> variables define the installation directories for package data, headers, libraries, and executables, respectively.

```in
pkgdatadir = $(datadir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkglibexecdir = $(libexecdir)/@PACKAGE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="111">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="111:0:0" line-data="subdir = src/machine/elements/synchronisation">`subdir`</SwmToken> variable is set to <SwmPath>[src/machine/elements/synchronisation/](src/machine/elements/synchronisation/)</SwmPath>, indicating the current subdirectory.

```in
subdir = src/machine/elements/synchronisation
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="119">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="119:0:0" line-data="DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)">`DIST_COMMON`</SwmToken> variable lists common files for distribution, including <SwmPath>[Makefile.am](Makefile.am)</SwmPath> and other common files.

```in
DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="124">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="124:0:0" line-data="LTLIBRARIES = $(noinst_LTLIBRARIES)">`LTLIBRARIES`</SwmToken> variable specifies the libraries to be built, in this case, <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="124:6:6" line-data="LTLIBRARIES = $(noinst_LTLIBRARIES)">`noinst_LTLIBRARIES`</SwmToken>.

```in
LTLIBRARIES = $(noinst_LTLIBRARIES)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="126">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="126:0:0" line-data="am_libmachineelementssynchronisation_la_OBJECTS = acces.lo \">`am_libmachineelementssynchronisation_la_OBJECTS`</SwmToken> variable lists the object files for the library: <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="126:4:6" line-data="am_libmachineelementssynchronisation_la_OBJECTS = acces.lo \">`acces.lo`</SwmToken> and <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="127:1:3" line-data="	evenement.lo">`evenement.lo`</SwmToken>.

```in
am_libmachineelementssynchronisation_la_OBJECTS = acces.lo \
	evenement.lo
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="128">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="128:0:0" line-data="libmachineelementssynchronisation_la_OBJECTS =  \">`libmachineelementssynchronisation_la_OBJECTS`</SwmToken> variable is set to the value of <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="129:3:3" line-data="	$(am_libmachineelementssynchronisation_la_OBJECTS)">`am_libmachineelementssynchronisation_la_OBJECTS`</SwmToken>.

```in
libmachineelementssynchronisation_la_OBJECTS =  \
	$(am_libmachineelementssynchronisation_la_OBJECTS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="134">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="134:0:0" line-data="libmachineelementssynchronisation_la_LINK = $(LIBTOOL) $(AM_V_lt) \">`libmachineelementssynchronisation_la_LINK`</SwmToken> variable defines the command to link the library using <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="113:11:11" line-data="am__aclocal_m4_deps = $(top_srcdir)/m4/libtool.m4 \">`libtool`</SwmToken> and the specified flags.

```in
libmachineelementssynchronisation_la_LINK = $(LIBTOOL) $(AM_V_lt) \
	--tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link \
	$(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) \
	$(libmachineelementssynchronisation_la_LDFLAGS) $(LDFLAGS) -o \
	$@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="151">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="151:0:0" line-data="DEFAULT_INCLUDES = -I.@am__isrc@ -I$(top_builddir)">`DEFAULT_INCLUDES`</SwmToken> variable includes the current directory and the top build directory.

```in
DEFAULT_INCLUDES = -I.@am__isrc@ -I$(top_builddir)
depcomp = $(SHELL) $(top_srcdir)/depcomp
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="153">

---

The <SwmPath>[depcomp](depcomp)</SwmPath> variable is set to the <SwmPath>[depcomp](depcomp)</SwmPath> script in the top source directory, used for dependency tracking.

```in
am__depfiles_maybe = depfiles
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="155">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="155:0:0" line-data="CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \">`CXXCOMPILE`</SwmToken> variable defines the command to <SwmPath>[compile](compile)</SwmPath> C++ source files using the specified flags.

```in
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="157">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="157:0:0" line-data="LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`LTCXXCOMPILE`</SwmToken> variable defines the command to <SwmPath>[compile](compile)</SwmPath> C++ source files using <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="113:11:11" line-data="am__aclocal_m4_deps = $(top_srcdir)/m4/libtool.m4 \">`libtool`</SwmToken> and the specified flags.

```in
LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CXXFLAGS) $(CXXFLAGS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="165">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="166:0:0" line-data="CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`CXXLINK`</SwmToken> variable defines the command to link C++ object files using <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="113:11:11" line-data="am__aclocal_m4_deps = $(top_srcdir)/m4/libtool.m4 \">`libtool`</SwmToken> and the specified flags.

```in
CXXLD = $(CXX)
CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CXXLD) $(AM_CXXFLAGS) \
	$(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="173">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="173:0:0" line-data="COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \">`COMPILE`</SwmToken> variable defines the command to <SwmPath>[compile](compile)</SwmPath> C source files using the specified flags.

```in
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="175">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="175:0:0" line-data="LTCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \">`LTCOMPILE`</SwmToken> variable defines the command to <SwmPath>[compile](compile)</SwmPath> C source files using <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="113:11:11" line-data="am__aclocal_m4_deps = $(top_srcdir)/m4/libtool.m4 \">`libtool`</SwmToken> and the specified flags.

```in
LTCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CC $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CC) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CFLAGS) $(CFLAGS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="191">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="191:0:0" line-data="SOURCES = $(libmachineelementssynchronisation_la_SOURCES)">`SOURCES`</SwmToken> variable lists the source files for the library: <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="191:6:6" line-data="SOURCES = $(libmachineelementssynchronisation_la_SOURCES)">`libmachineelementssynchronisation_la_SOURCES`</SwmToken>.

```in
SOURCES = $(libmachineelementssynchronisation_la_SOURCES)
DIST_SOURCES = $(libmachineelementssynchronisation_la_SOURCES)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="193">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="192:0:0" line-data="DIST_SOURCES = $(libmachineelementssynchronisation_la_SOURCES)">`DIST_SOURCES`</SwmToken> variable is set to the value of <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="191:0:0" line-data="SOURCES = $(libmachineelementssynchronisation_la_SOURCES)">`SOURCES`</SwmToken> for distribution.

```in
RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="214">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="212:0:0" line-data="AM_RECURSIVE_TARGETS = $(am__recursive_targets:-recursive=) TAGS CTAGS \">`AM_RECURSIVE_TARGETS`</SwmToken> variable lists the recursive targets for the Makefile.

```in
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/synchronisation/Makefile.in" line="218">

---

The <SwmToken path="src/machine/elements/synchronisation/Makefile.in" pos="218:0:0" line-data="am__uniquify_input = $(AWK) &#39;\">`am__uniquify_input`</SwmToken> variable defines an AWK script to ensure the list of sources is unique.

```in
am__uniquify_input = $(AWK) '\
  BEGIN { nonempty = 0; } \
  { items[$$0] = 1; nonempty = 1; } \
  END { if (nonempty) { for (i in items) print i; }; } \
'
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
