---
title: Building the Analyseur Extension with Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/machine/extensions/analyseur/](src/machine/extensions/analyseur/)</SwmPath> directory to build the project. It will go through the configuration steps in the <SwmPath>[Makefile.am](Makefile.am)</SwmPath> and <SwmPath>[Makefile.in](Makefile.in)</SwmPath> files.

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="20">

---

# Compiler and Include Flags

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="20:0:0" line-data="AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}">`AM_CPPFLAGS`</SwmToken> variable sets the include paths for the compiler. It includes the top build directory and the top source directory. <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="20:0:13" line-data="AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}">`AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}`</SwmToken>

```am
AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="22">

---

# Subdirectories

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="22:0:0" line-data="SUBDIRS= ">`SUBDIRS`</SwmToken> variable is used to specify subdirectories that should be built. In this case, it is empty. <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="22:0:1" line-data="SUBDIRS= ">`SUBDIRS=`</SwmToken>

```am
SUBDIRS= 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="24">

---

# Library Configuration

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="24:0:0" line-data="noinst_LTLIBRARIES=libmachineextensionsanalyseur.la">`noinst_LTLIBRARIES`</SwmToken> variable specifies that the library <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="24:2:4" line-data="noinst_LTLIBRARIES=libmachineextensionsanalyseur.la">`libmachineextensionsanalyseur.la`</SwmToken> should be built but not installed. <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="24:0:4" line-data="noinst_LTLIBRARIES=libmachineextensionsanalyseur.la">`noinst_LTLIBRARIES=libmachineextensionsanalyseur.la`</SwmToken>

```am
noinst_LTLIBRARIES=libmachineextensionsanalyseur.la
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="26">

---

# Built Sources

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="26:0:0" line-data="BUILT_SOURCES=analyseur.lex.cpp analyseur.syn.cpp">`BUILT_SOURCES`</SwmToken> variable lists the source files that need to be built. Here, it includes <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.lex.cpp](src/demarrage/lanceur/analyseur/analyseur.lex.cpp)</SwmPath> and <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.syn.cpp](src/demarrage/lanceur/analyseur/analyseur.syn.cpp)</SwmPath>. <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="26:0:12" line-data="BUILT_SOURCES=analyseur.lex.cpp analyseur.syn.cpp">`BUILT_SOURCES=analyseur.lex.cpp analyseur.syn.cpp`</SwmToken>

```am
BUILT_SOURCES=analyseur.lex.cpp analyseur.syn.cpp
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="28">

---

# Extra Built Sources

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="28:0:0" line-data="EXTRABUILTSOURCES=analyseur.syn.h">`EXTRABUILTSOURCES`</SwmToken> variable includes additional source files that need to be built, such as <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="28:2:6" line-data="EXTRABUILTSOURCES=analyseur.syn.h">`analyseur.syn.h`</SwmToken>. <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="28:0:6" line-data="EXTRABUILTSOURCES=analyseur.syn.h">`EXTRABUILTSOURCES=analyseur.syn.h`</SwmToken>

```am
EXTRABUILTSOURCES=analyseur.syn.h
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="30">

---

# Extra Distribution Files

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="30:0:0" line-data="EXTRA_DIST=analyseur.h ">`EXTRA_DIST`</SwmToken> variable lists files that should be included in the distribution tarball. Here, it includes <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.h](src/demarrage/lanceur/analyseur/analyseur.h)</SwmPath>. <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="30:0:4" line-data="EXTRA_DIST=analyseur.h ">`EXTRA_DIST=analyseur.h`</SwmToken>

```am
EXTRA_DIST=analyseur.h 
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="31">

---

# Clean Files

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="31:0:0" line-data="CLEANFILES= analyseur.syn.output">`CLEANFILES`</SwmToken> variable specifies files that should be removed when the <SwmToken path="src/machine/extensions/analyseur/Makefile.in" pos="559:11:11" line-data="# into them and run &#39;make&#39; without going through this Makefile.">`make`</SwmToken>` `<SwmToken path="src/machine/extensions/analyseur/Makefile.in" pos="225:8:8" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`clean`</SwmToken> command is run. Here, it includes <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="31:3:7" line-data="CLEANFILES= analyseur.syn.output">`analyseur.syn.output`</SwmToken>. <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="31:0:7" line-data="CLEANFILES= analyseur.syn.output">`CLEANFILES= analyseur.syn.output`</SwmToken>

```am
CLEANFILES= analyseur.syn.output
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="33">

---

# Yacc and Lex Flags

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="33:0:0" line-data="AM_YFLAGS=-d -v --file-prefix=y --name-prefix=machineextensionsanalyseur -o y.tab.c">`AM_YFLAGS`</SwmToken> and <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="34:0:0" line-data="AM_LFLAGS=-P machineextensionsanalyseur -o lex.yy.c">`AM_LFLAGS`</SwmToken> variables set the flags for Yacc and Lex, respectively. These flags are used to generate the parser and lexer.

````AM_YFLAGS=-d -v --file-prefix=y --name-prefix=machineextensionsanalyseur -o y.tab.c
AM_LFLAGS=-P machineextensionsanalyseur -o lex.yy.c```
````

```am
AM_YFLAGS=-d -v --file-prefix=y --name-prefix=machineextensionsanalyseur -o y.tab.c
AM_LFLAGS=-P machineextensionsanalyseur -o lex.yy.c
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="36">

---

# Compiler Flags

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="36:0:0" line-data="libmachineextensionsanalyseur_la_CXXFLAGS=$(AM_CXXFLAGS) -Wno-error=sign-compare">`libmachineextensionsanalyseur_la_CXXFLAGS`</SwmToken> variable sets the compiler flags for the library. It includes the <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="36:3:3" line-data="libmachineextensionsanalyseur_la_CXXFLAGS=$(AM_CXXFLAGS) -Wno-error=sign-compare">`AM_CXXFLAGS`</SwmToken> and disables the <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="36:11:13" line-data="libmachineextensionsanalyseur_la_CXXFLAGS=$(AM_CXXFLAGS) -Wno-error=sign-compare">`sign-compare`</SwmToken> warning. <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="36:0:13" line-data="libmachineextensionsanalyseur_la_CXXFLAGS=$(AM_CXXFLAGS) -Wno-error=sign-compare">`libmachineextensionsanalyseur_la_CXXFLAGS=$(AM_CXXFLAGS) -Wno-error=sign-compare`</SwmToken>

```am
libmachineextensionsanalyseur_la_CXXFLAGS=$(AM_CXXFLAGS) -Wno-error=sign-compare
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="38">

---

# Source Files

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="38:0:0" line-data="libmachineextensionsanalyseur_la_SOURCES=analyseur.lex.lpp analyseur.syn.ypp analyseur.cpp analyseur.h includes.h">`libmachineextensionsanalyseur_la_SOURCES`</SwmToken> variable lists the source files for the library. It includes <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.lex.lpp](src/demarrage/lanceur/analyseur/analyseur.lex.lpp)</SwmPath>, <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.syn.ypp](src/demarrage/lanceur/analyseur/analyseur.syn.ypp)</SwmPath>, <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.cpp](src/demarrage/lanceur/analyseur/analyseur.cpp)</SwmPath>, <SwmPath>[src/demarrage/lanceur/analyseur/analyseur.h](src/demarrage/lanceur/analyseur/analyseur.h)</SwmPath>, and <SwmPath>[src/demarrage/lanceur/analyseur/includes.h](src/demarrage/lanceur/analyseur/includes.h)</SwmPath>. <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="38:0:24" line-data="libmachineextensionsanalyseur_la_SOURCES=analyseur.lex.lpp analyseur.syn.ypp analyseur.cpp analyseur.h includes.h">`libmachineextensionsanalyseur_la_SOURCES=analyseur.lex.lpp analyseur.syn.ypp analyseur.cpp analyseur.h includes.h`</SwmToken>

```am
libmachineextensionsanalyseur_la_SOURCES=analyseur.lex.lpp analyseur.syn.ypp analyseur.cpp analyseur.h includes.h
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.am" line="39">

---

# Library Additions and Flags

The <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="39:0:0" line-data="libmachineextensionsanalyseur_la_LIBADD=">`libmachineextensionsanalyseur_la_LIBADD`</SwmToken> and <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="40:0:0" line-data="libmachineextensionsanalyseur_la_LDFLAGS=-no-undefined">`libmachineextensionsanalyseur_la_LDFLAGS`</SwmToken> variables specify additional libraries and linker flags for the library. Here, <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="39:0:0" line-data="libmachineextensionsanalyseur_la_LIBADD=">`libmachineextensionsanalyseur_la_LIBADD`</SwmToken> is empty, and <SwmToken path="src/machine/extensions/analyseur/Makefile.am" pos="40:0:0" line-data="libmachineextensionsanalyseur_la_LDFLAGS=-no-undefined">`libmachineextensionsanalyseur_la_LDFLAGS`</SwmToken> includes the <SwmToken path="src/machine/extensions/analyseur/Makefile.in" pos="441:4:7" line-data="libmachineextensionsanalyseur_la_LDFLAGS = -no-undefined">`-no-undefined`</SwmToken> flag.

````libmachineextensionsanalyseur_la_LIBADD=
libmachineextensionsanalyseur_la_LDFLAGS=-no-undefined```
````

```am
libmachineextensionsanalyseur_la_LIBADD=
libmachineextensionsanalyseur_la_LDFLAGS=-no-undefined
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.in" line="1">

---

# Makefile Generation

The <SwmPath>[Makefile.in](Makefile.in)</SwmPath> file is generated by automake from <SwmPath>[Makefile.am](Makefile.am)</SwmPath>. This is indicated by the comment at the top of the file. <SwmToken path="src/machine/extensions/analyseur/Makefile.in" pos="1:0:21" line-data="# Makefile.in generated by automake 1.15 from Makefile.am.">`# Makefile.in generated by automake 1.15 from Makefile.am.`</SwmToken>

```in
# Makefile.in generated by automake 1.15 from Makefile.am.
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.in" line="4">

---

# Copyright and License

The <SwmPath>[Makefile.in](Makefile.in)</SwmPath> includes a copyright notice and license information from the Free Software Foundation.

````# Copyright (C) 1994-2014 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
gives unlimited permission to copy and/or distribute it,
with or without modifications, as long as this notice is preserved.```
````

```in
# Copyright (C) 1994-2014 Free Software Foundation, Inc.

# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.in" line="119">

---

# Distribution Common Files

The <SwmToken path="src/machine/extensions/analyseur/Makefile.in" pos="119:0:0" line-data="DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)">`DIST_COMMON`</SwmToken> variable lists common files that should be included in the distribution. It includes <SwmPath>[Makefile.am](Makefile.am)</SwmPath> and other common files. <SwmToken path="src/machine/extensions/analyseur/Makefile.in" pos="119:0:16" line-data="DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)">`DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)`</SwmToken>

```in
DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.in" line="447">

---

# Makefile Dependencies

The <SwmPath>[Makefile.in](Makefile.in)</SwmPath> depends on <SwmPath>[Makefile.am](Makefile.am)</SwmPath> and other configuration dependencies. This ensures that changes in <SwmPath>[Makefile.am](Makefile.am)</SwmPath> trigger a regeneration of <SwmPath>[Makefile.in](Makefile.in)</SwmPath>. <SwmPath>[src/machine/extensions/analyseur/Makefile.in](src/machine/extensions/analyseur/Makefile.in)</SwmPath>`: `<SwmPath>[src/machine/extensions/analyseur/Makefile.am](src/machine/extensions/analyseur/Makefile.am)</SwmPath>` `<SwmToken path="src/machine/extensions/analyseur/Makefile.in" pos="447:19:21" line-data="$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)">`$(am__configure_deps`</SwmToken>`)`

```in
$(srcdir)/Makefile.in:  $(srcdir)/Makefile.am  $(am__configure_deps)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/extensions/analyseur/Makefile.in" line="111">

---

# Subdirectory

The <SwmToken path="src/machine/extensions/analyseur/Makefile.in" pos="111:0:0" line-data="subdir = src/machine/extensions/analyseur">`subdir`</SwmToken> variable specifies the subdirectory for the build. In this case, it is <SwmPath>[src/machine/extensions/analyseur/](src/machine/extensions/analyseur/)</SwmPath>. <SwmPath>[src/demarrage/lanceur/analyseur/](src/demarrage/lanceur/analyseur/)</SwmPath>

```in
subdir = src/machine/extensions/analyseur
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
