---
title: Building the Programme Directory with Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/machine/elements/programme/](src/machine/elements/programme/)</SwmPath> directory to build the project components.

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="36">

---

# Setting Up Makefile Variables

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="36:0:0" line-data="VPATH = @srcdir@">`VPATH`</SwmToken> variable is set to <SwmToken path="src/machine/elements/programme/Makefile.in" pos="36:4:6" line-data="VPATH = @srcdir@">`@srcdir@`</SwmToken>, which specifies the search path for source files.

```in
VPATH = @srcdir@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="92">

---

Variables like <SwmToken path="src/machine/elements/programme/Makefile.in" pos="92:0:0" line-data="pkgdatadir = $(datadir)/@PACKAGE@">`pkgdatadir`</SwmToken>, <SwmToken path="src/machine/elements/programme/Makefile.in" pos="93:0:0" line-data="pkgincludedir = $(includedir)/@PACKAGE@">`pkgincludedir`</SwmToken>, <SwmToken path="src/machine/elements/programme/Makefile.in" pos="94:0:0" line-data="pkglibdir = $(libdir)/@PACKAGE@">`pkglibdir`</SwmToken>, and <SwmToken path="src/machine/elements/programme/Makefile.in" pos="95:0:0" line-data="pkglibexecdir = $(libexecdir)/@PACKAGE@">`pkglibexecdir`</SwmToken> are defined to specify the installation directories for data, include files, libraries, and executable libraries respectively.

```in
pkgdatadir = $(datadir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkglibexecdir = $(libexecdir)/@PACKAGE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="108">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="108:0:0" line-data="build_triplet = @build@">`build_triplet`</SwmToken>, <SwmToken path="src/machine/elements/programme/Makefile.in" pos="109:0:0" line-data="host_triplet = @host@">`host_triplet`</SwmToken>, and <SwmToken path="src/machine/elements/programme/Makefile.in" pos="110:0:0" line-data="target_triplet = @target@">`target_triplet`</SwmToken> variables are defined to specify the build, host, and target system types.

```in
build_triplet = @build@
host_triplet = @host@
target_triplet = @target@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="111">

---

# Defining Subdirectories and Libraries

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="111:0:0" line-data="subdir = src/machine/elements/programme">`subdir`</SwmToken> variable is set to <SwmPath>[src/machine/elements/programme/](src/machine/elements/programme/)</SwmPath>, indicating the current subdirectory.

```in
subdir = src/machine/elements/programme
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="124">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="124:0:0" line-data="LTLIBRARIES = $(noinst_LTLIBRARIES)">`LTLIBRARIES`</SwmToken> variable is set to <SwmToken path="src/machine/elements/programme/Makefile.in" pos="124:4:7" line-data="LTLIBRARIES = $(noinst_LTLIBRARIES)">`$(noinst_LTLIBRARIES)`</SwmToken>, indicating that no libraries will be installed.

```in
LTLIBRARIES = $(noinst_LTLIBRARIES)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="125">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="125:0:0" line-data="libmachineelementsprogramme_la_DEPENDENCIES =">`libmachineelementsprogramme_la_DEPENDENCIES`</SwmToken> variable is defined, followed by the <SwmToken path="src/machine/elements/programme/Makefile.in" pos="126:0:0" line-data="am_libmachineelementsprogramme_la_OBJECTS = adresse_programme.lo \">`am_libmachineelementsprogramme_la_OBJECTS`</SwmToken> variable, which lists the object files to be compiled.

```in
libmachineelementsprogramme_la_DEPENDENCIES =
am_libmachineelementsprogramme_la_OBJECTS = adresse_programme.lo \
	cible.lo condition.lo parametre.lo type_programme.lo \
	valeur_programme.lo
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="135">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="135:0:0" line-data="libmachineelementsprogramme_la_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX \">`libmachineelementsprogramme_la_LINK`</SwmToken> variable is defined to specify the linking command for the library.

```in
libmachineelementsprogramme_la_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX \
	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link $(CXXLD) \
	$(AM_CXXFLAGS) $(CXXFLAGS) \
	$(libmachineelementsprogramme_la_LDFLAGS) $(LDFLAGS) -o $@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="155">

---

# Compilation and Linking

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="155:0:0" line-data="CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \">`CXXCOMPILE`</SwmToken> variable is defined to specify the C++ compilation command.

```in
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="157">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="157:0:0" line-data="LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`LTCXXCOMPILE`</SwmToken> variable is defined to specify the libtool C++ compilation command.

```in
LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CXXFLAGS) $(CXXFLAGS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="165">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="166:0:0" line-data="CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`CXXLINK`</SwmToken> variable is defined to specify the C++ linking command.

```in
CXXLD = $(CXX)
CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CXXLD) $(AM_CXXFLAGS) \
	$(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="448">

---

# Cleaning and Installation

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="448:0:2" line-data="clean-noinstLTLIBRARIES:">`clean-noinstLTLIBRARIES`</SwmToken> target is defined to clean up non-installed libraries.

```in
clean-noinstLTLIBRARIES:
	-test -z "$(noinst_LTLIBRARIES)" || rm -f $(noinst_LTLIBRARIES)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="462">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="462:0:2" line-data="mostlyclean-compile:">`mostlyclean-compile`</SwmToken> target is defined to remove object files.

```in
mostlyclean-compile:
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="496">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="496:0:2" line-data="mostlyclean-libtool:">`mostlyclean-libtool`</SwmToken> target is defined to remove libtool files.

```in
mostlyclean-libtool:
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="691">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="691:0:0" line-data="clean: clean-recursive">`clean`</SwmToken> target is defined to clean up the build directory.

```in
clean: clean-recursive
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="696">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="696:0:0" line-data="distclean: distclean-recursive">`distclean`</SwmToken> target is defined to remove all generated files.

```in
distclean: distclean-recursive
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/Makefile.in" line="742">

---

The <SwmToken path="src/machine/elements/programme/Makefile.in" pos="742:0:2" line-data="maintainer-clean: maintainer-clean-recursive">`maintainer-clean`</SwmToken> target is defined for maintainers to clean up the build directory.

```in
maintainer-clean: maintainer-clean-recursive
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
