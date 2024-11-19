---
title: Building the Analyseur Directory with Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/machine/elements/programme/analyseur/](src/machine/elements/programme/analyseur/)</SwmPath> directory to build the project components.

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="20">

---

# Compiler Flags

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="20:0:0" line-data="AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}">`AM_CPPFLAGS`</SwmToken> variable sets the include directories for the compiler. It includes the top build directory and the top source directory.

```am
AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="22">

---

# Subdirectories

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="22:0:0" line-data="SUBDIRS= ">`SUBDIRS`</SwmToken> variable is currently empty, indicating that there are no subdirectories to process.

```am
SUBDIRS= 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="24">

---

# Library Definition

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="24:0:0" line-data="noinst_LTLIBRARIES=libmachineelementsprogrammeanalyseur.la">`noinst_LTLIBRARIES`</SwmToken> variable defines a library that will not be installed. Here, it specifies <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="24:2:4" line-data="noinst_LTLIBRARIES=libmachineelementsprogrammeanalyseur.la">`libmachineelementsprogrammeanalyseur.la`</SwmToken>.

```am
noinst_LTLIBRARIES=libmachineelementsprogrammeanalyseur.la
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="26">

---

# Built Sources

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="26:0:0" line-data="BUILT_SOURCES=analyseur.lex.cpp analyseur.syn.cpp">`BUILT_SOURCES`</SwmToken> variable lists the sources that need to be built. It includes <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.lex.cpp](src/demarrage/lanceur/analyseur/analyseur.lex.cpp)</SwmPath> and <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.syn.cpp](src/demarrage/lanceur/analyseur/analyseur.syn.cpp)</SwmPath>.

```am
BUILT_SOURCES=analyseur.lex.cpp analyseur.syn.cpp
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="28">

---

# Extra Built Sources

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="28:0:0" line-data="EXTRABUILTSOURCES=analyseur.syn.h">`EXTRABUILTSOURCES`</SwmToken> variable includes additional sources that need to be built, such as <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="28:2:6" line-data="EXTRABUILTSOURCES=analyseur.syn.h">`analyseur.syn.h`</SwmToken>.

```am
EXTRABUILTSOURCES=analyseur.syn.h
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="30">

---

# Extra Distribution Files

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="30:0:0" line-data="EXTRA_DIST=analyseur.h ">`EXTRA_DIST`</SwmToken> variable lists files that should be included in the distribution tarball. Here, it includes <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.h](src/demarrage/lanceur/analyseur/analyseur.h)</SwmPath>.

```am
EXTRA_DIST=analyseur.h 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="31">

---

# Clean Files

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="31:0:0" line-data="CLEANFILES= analyseur.syn.output">`CLEANFILES`</SwmToken> variable specifies files to be removed during the `make clean` process. It includes <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="31:3:7" line-data="CLEANFILES= analyseur.syn.output">`analyseur.syn.output`</SwmToken>.

```am
CLEANFILES= analyseur.syn.output
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="33">

---

# Yacc and Lex Flags

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="33:0:0" line-data="AM_YFLAGS=-d -v --file-prefix=y --name-prefix=machineelementsprogrammeanalyseur -o y.tab.c">`AM_YFLAGS`</SwmToken> and <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="34:0:0" line-data="AM_LFLAGS=-P machineelementsprogrammeanalyseur -o lex.yy.c">`AM_LFLAGS`</SwmToken> variables set flags for Yacc and Lex, respectively. These flags control the output file names and prefixes.

```am
AM_YFLAGS=-d -v --file-prefix=y --name-prefix=machineelementsprogrammeanalyseur -o y.tab.c
AM_LFLAGS=-P machineelementsprogrammeanalyseur -o lex.yy.c
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="36">

---

# Compiler Flags for Library

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="36:0:0" line-data="libmachineelementsprogrammeanalyseur_la_CXXFLAGS=$(AM_CXXFLAGS) -Wno-error=sign-compare">`libmachineelementsprogrammeanalyseur_la_CXXFLAGS`</SwmToken> variable sets additional compiler flags for the library, including <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="36:6:13" line-data="libmachineelementsprogrammeanalyseur_la_CXXFLAGS=$(AM_CXXFLAGS) -Wno-error=sign-compare">`-Wno-error=sign-compare`</SwmToken>.

```am
libmachineelementsprogrammeanalyseur_la_CXXFLAGS=$(AM_CXXFLAGS) -Wno-error=sign-compare
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="38">

---

# Library Sources

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="38:0:0" line-data="libmachineelementsprogrammeanalyseur_la_SOURCES=analyseur.lex.lpp analyseur.syn.ypp analyseur.cpp analyseur.h includes.h">`libmachineelementsprogrammeanalyseur_la_SOURCES`</SwmToken> variable lists the source files for the library. It includes <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.lex.lpp](src/demarrage/lanceur/analyseur/analyseur.lex.lpp)</SwmPath>, <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.syn.ypp](src/demarrage/lanceur/analyseur/analyseur.syn.ypp)</SwmPath>, <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.cpp](src/demarrage/lanceur/analyseur/analyseur.cpp)</SwmPath>, <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.h](src/demarrage/lanceur/analyseur/analyseur.h)</SwmPath>, and <SwmPath>[src/demarrage/lanceur/analyseur/includes.h](src/demarrage/lanceur/analyseur/includes.h)</SwmPath>.

```am
libmachineelementsprogrammeanalyseur_la_SOURCES=analyseur.lex.lpp analyseur.syn.ypp analyseur.cpp analyseur.h includes.h
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="39">

---

# Library Dependencies

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="39:0:0" line-data="libmachineelementsprogrammeanalyseur_la_LIBADD=">`libmachineelementsprogrammeanalyseur_la_LIBADD`</SwmToken> variable is empty, indicating no additional libraries are linked.

```am
libmachineelementsprogrammeanalyseur_la_LIBADD=
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/programme/analyseur/Makefile.am" line="40">

---

# Linker Flags

The <SwmToken path="src/machine/elements/programme/analyseur/Makefile.am" pos="40:0:0" line-data="libmachineelementsprogrammeanalyseur_la_LDFLAGS=-no-undefined">`libmachineelementsprogrammeanalyseur_la_LDFLAGS`</SwmToken> variable sets linker flags, including `-no-undefined`.

```am
libmachineelementsprogrammeanalyseur_la_LDFLAGS=-no-undefined
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
