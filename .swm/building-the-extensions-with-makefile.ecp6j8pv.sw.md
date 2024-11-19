---
title: Building the Extensions with Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/machine/extensions/](src/machine/extensions/)</SwmPath> directory to build the project components.

<SwmSnippet path="/src/machine/extensions/Makefile.am" line="20">

---

# Setting Up Compilation Flags

The <SwmToken path="src/machine/extensions/Makefile.am" pos="20:0:0" line-data="AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}">`AM_CPPFLAGS`</SwmToken> variable is used to specify the include directories for the compiler. It includes the top-level build and source directories.

```am
AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.am" line="22">

---

# Subdirectories

The <SwmToken path="src/machine/extensions/Makefile.am" pos="22:0:0" line-data="SUBDIRS= analyseur gestionnaire ">`SUBDIRS`</SwmToken> variable lists the subdirectories that should be built. In this case, it includes <SwmToken path="src/machine/extensions/Makefile.am" pos="22:3:3" line-data="SUBDIRS= analyseur gestionnaire ">`analyseur`</SwmToken> and <SwmToken path="src/machine/extensions/Makefile.am" pos="22:5:5" line-data="SUBDIRS= analyseur gestionnaire ">`gestionnaire`</SwmToken>.

```am
SUBDIRS= analyseur gestionnaire 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.am" line="24">

---

# Library Definitions

The <SwmToken path="src/machine/extensions/Makefile.am" pos="24:0:0" line-data="noinst_LTLIBRARIES=libmachineextensions.la libmachineextensionsgestionnaire.la">`noinst_LTLIBRARIES`</SwmToken> variable defines the libraries that will be built but not installed. Here, it includes <SwmToken path="src/machine/extensions/Makefile.am" pos="24:2:4" line-data="noinst_LTLIBRARIES=libmachineextensions.la libmachineextensionsgestionnaire.la">`libmachineextensions.la`</SwmToken> and <SwmToken path="src/machine/extensions/Makefile.am" pos="24:6:8" line-data="noinst_LTLIBRARIES=libmachineextensions.la libmachineextensionsgestionnaire.la">`libmachineextensionsgestionnaire.la`</SwmToken>.

```am
noinst_LTLIBRARIES=libmachineextensions.la libmachineextensionsgestionnaire.la
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.am" line="26">

---

# Source Files for libmachineextensions

The <SwmToken path="src/machine/extensions/Makefile.am" pos="26:0:0" line-data="libmachineextensions_la_SOURCES=automate_deterministe.cpp automate_deterministe.h automate_non_deterministe.cpp automate_non_deterministe.h description.cpp description.h patron_objet_extension.cpp patron_objet_extension.h reference_objet.cpp reference_objet.h ">`libmachineextensions_la_SOURCES`</SwmToken> variable lists the source files for the <SwmToken path="src/machine/extensions/Makefile.am" pos="24:2:2" line-data="noinst_LTLIBRARIES=libmachineextensions.la libmachineextensionsgestionnaire.la">`libmachineextensions`</SwmToken> library.

```am
libmachineextensions_la_SOURCES=automate_deterministe.cpp automate_deterministe.h automate_non_deterministe.cpp automate_non_deterministe.h description.cpp description.h patron_objet_extension.cpp patron_objet_extension.h reference_objet.cpp reference_objet.h 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.am" line="28">

---

# Linker Flags for libmachineextensions

The <SwmToken path="src/machine/extensions/Makefile.am" pos="28:0:0" line-data="libmachineextensions_la_LDFLAGS=-no-undefined">`libmachineextensions_la_LDFLAGS`</SwmToken> variable specifies linker flags for the <SwmToken path="src/machine/extensions/Makefile.am" pos="24:2:2" line-data="noinst_LTLIBRARIES=libmachineextensions.la libmachineextensionsgestionnaire.la">`libmachineextensions`</SwmToken> library. The <SwmToken path="src/machine/extensions/Makefile.in" pos="426:4:7" line-data="libmachineextensions_la_LDFLAGS = -no-undefined">`-no-undefined`</SwmToken> flag ensures that all symbols are defined.

```am
libmachineextensions_la_LDFLAGS=-no-undefined
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.am" line="30">

---

# Source Files for libmachineextensionsgestionnaire

The <SwmToken path="src/machine/extensions/Makefile.am" pos="30:0:0" line-data="libmachineextensionsgestionnaire_la_SOURCES=automate_deterministe.cpp automate_deterministe.h automate_non_deterministe.cpp automate_non_deterministe.h description.cpp description.h patron_objet_extension.cpp patron_objet_extension.h reference_objet.cpp reference_objet.h ">`libmachineextensionsgestionnaire_la_SOURCES`</SwmToken> variable lists the source files for the <SwmToken path="src/machine/extensions/Makefile.am" pos="24:6:6" line-data="noinst_LTLIBRARIES=libmachineextensions.la libmachineextensionsgestionnaire.la">`libmachineextensionsgestionnaire`</SwmToken> library.

```am
libmachineextensionsgestionnaire_la_SOURCES=automate_deterministe.cpp automate_deterministe.h automate_non_deterministe.cpp automate_non_deterministe.h description.cpp description.h patron_objet_extension.cpp patron_objet_extension.h reference_objet.cpp reference_objet.h 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.am" line="32">

---

# Linker Flags for libmachineextensionsgestionnaire

The <SwmToken path="src/machine/extensions/Makefile.am" pos="32:0:0" line-data="libmachineextensionsgestionnaire_la_LDFLAGS=-no-undefined">`libmachineextensionsgestionnaire_la_LDFLAGS`</SwmToken> variable specifies linker flags for the <SwmToken path="src/machine/extensions/Makefile.am" pos="24:6:6" line-data="noinst_LTLIBRARIES=libmachineextensions.la libmachineextensionsgestionnaire.la">`libmachineextensionsgestionnaire`</SwmToken> library. The <SwmToken path="src/machine/extensions/Makefile.in" pos="426:4:7" line-data="libmachineextensions_la_LDFLAGS = -no-undefined">`-no-undefined`</SwmToken> flag ensures that all symbols are defined.

```am
libmachineextensionsgestionnaire_la_LDFLAGS=-no-undefined
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.in" line="1">

---

# <SwmPath>[Makefile.in](Makefile.in)</SwmPath> Configuration

The <SwmPath>[Makefile.in](Makefile.in)</SwmPath> file is generated by automake and includes various configuration settings and copyright information.

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

<SwmSnippet path="/src/machine/extensions/Makefile.in" line="36">

---

# Directory and File Paths

The <SwmToken path="src/machine/extensions/Makefile.in" pos="36:0:0" line-data="VPATH = @srcdir@">`VPATH`</SwmToken> variable specifies the search path for source files.

```in
VPATH = @srcdir@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.in" line="92">

---

# Package Directories

Variables like <SwmToken path="src/machine/extensions/Makefile.in" pos="92:0:0" line-data="pkgdatadir = $(datadir)/@PACKAGE@">`pkgdatadir`</SwmToken>, <SwmToken path="src/machine/extensions/Makefile.in" pos="93:0:0" line-data="pkgincludedir = $(includedir)/@PACKAGE@">`pkgincludedir`</SwmToken>, <SwmToken path="src/machine/extensions/Makefile.in" pos="94:0:0" line-data="pkglibdir = $(libdir)/@PACKAGE@">`pkglibdir`</SwmToken>, and <SwmToken path="src/machine/extensions/Makefile.in" pos="95:0:0" line-data="pkglibexecdir = $(libexecdir)/@PACKAGE@">`pkglibexecdir`</SwmToken> define the directories for package data, include files, libraries, and executable libraries, respectively.

```in
pkgdatadir = $(datadir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkglibexecdir = $(libexecdir)/@PACKAGE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.in" line="166">

---

# Compilation and Linking

Variables like <SwmToken path="src/machine/extensions/Makefile.in" pos="166:0:0" line-data="CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \">`CXXCOMPILE`</SwmToken>, <SwmToken path="src/machine/extensions/Makefile.in" pos="168:0:0" line-data="LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`LTCXXCOMPILE`</SwmToken>, <SwmToken path="src/machine/extensions/Makefile.in" pos="176:0:0" line-data="CXXLD = $(CXX)">`CXXLD`</SwmToken>, and <SwmToken path="src/machine/extensions/Makefile.in" pos="177:0:0" line-data="CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`CXXLINK`</SwmToken> define the commands for compiling and linking C++ source files.

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

<SwmSnippet path="/src/machine/extensions/Makefile.in" line="124">

---

# Object Files and Libraries

The <SwmToken path="src/machine/extensions/Makefile.in" pos="124:0:0" line-data="LTLIBRARIES = $(noinst_LTLIBRARIES)">`LTLIBRARIES`</SwmToken> variable lists the libraries to be built, and <SwmToken path="src/machine/extensions/Makefile.in" pos="129:0:0" line-data="libmachineextensions_la_OBJECTS =  \">`libmachineextensions_la_OBJECTS`</SwmToken> specifies the object files for the <SwmToken path="src/machine/extensions/Makefile.am" pos="24:2:2" line-data="noinst_LTLIBRARIES=libmachineextensions.la libmachineextensionsgestionnaire.la">`libmachineextensions`</SwmToken> library.

```in
LTLIBRARIES = $(noinst_LTLIBRARIES)
libmachineextensions_la_DEPENDENCIES =
am_libmachineextensions_la_OBJECTS = automate_deterministe.lo \
	automate_non_deterministe.lo description.lo \
	patron_objet_extension.lo reference_objet.lo
libmachineextensions_la_OBJECTS =  \
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.in" line="206">

---

# Recursive Targets

The <SwmToken path="src/machine/extensions/Makefile.in" pos="206:0:0" line-data="RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \">`RECURSIVE_TARGETS`</SwmToken> variable lists the targets that should be built recursively, such as <SwmToken path="src/machine/extensions/Makefile.in" pos="206:4:6" line-data="RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \">`all-recursive`</SwmToken>, <SwmToken path="src/machine/extensions/Makefile.in" pos="206:8:10" line-data="RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \">`check-recursive`</SwmToken>, and <SwmToken path="src/machine/extensions/Makefile.in" pos="211:7:9" line-data="	install-ps-recursive install-recursive installcheck-recursive \">`install-recursive`</SwmToken>.

```in
RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \
	ctags-recursive dvi-recursive html-recursive info-recursive \
	install-data-recursive install-dvi-recursive \
	install-exec-recursive install-html-recursive \
	install-info-recursive install-pdf-recursive \
	install-ps-recursive install-recursive installcheck-recursive \
	installdirs-recursive pdf-recursive ps-recursive \
	tags-recursive uninstall-recursive
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.in" line="219">

---

# Clean Targets

The <SwmToken path="src/machine/extensions/Makefile.in" pos="219:0:0" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`RECURSIVE_CLEAN_TARGETS`</SwmToken> variable lists the targets for cleaning up files, such as <SwmToken path="src/machine/extensions/Makefile.in" pos="219:4:6" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`mostlyclean-recursive`</SwmToken>, <SwmToken path="src/machine/extensions/Makefile.in" pos="219:8:10" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`clean-recursive`</SwmToken>, and <SwmToken path="src/machine/extensions/Makefile.in" pos="220:1:3" line-data="  distclean-recursive maintainer-clean-recursive">`distclean-recursive`</SwmToken>.

```in
RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\
  distclean-recursive maintainer-clean-recursive
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/Makefile.in" line="679">

---

# Installation Targets

The <SwmToken path="src/machine/extensions/Makefile.in" pos="679:0:0" line-data="install: install-recursive">`install`</SwmToken> target is defined to install the built files recursively.

```in
install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
