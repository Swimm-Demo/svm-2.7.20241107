---
title: Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/machine/elements/memoire/](src/machine/elements/memoire/)</SwmPath> directory to build the project components.

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="1">

---

# <SwmPath>[Makefile.in](Makefile.in)</SwmPath> Header

The <SwmPath>[Makefile.in](Makefile.in)</SwmPath> is generated by automake and includes licensing information and a disclaimer of warranty.

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

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="15">

---

# Setting Make Variables

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="15:0:2" line-data="@SET_MAKE@">`@SET_MAKE@`</SwmToken> macro is used to set up the make environment.

```in
@SET_MAKE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="36">

---

# Directory and File Paths

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="36:0:0" line-data="VPATH = @srcdir@">`VPATH`</SwmToken> variable is set to <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="36:4:6" line-data="VPATH = @srcdir@">`@srcdir@`</SwmToken> to specify the directory for source files.

```in
VPATH = @srcdir@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="92">

---

# Package Directories

Variables like <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="92:0:0" line-data="pkgdatadir = $(datadir)/@PACKAGE@">`pkgdatadir`</SwmToken>, <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="93:0:0" line-data="pkgincludedir = $(includedir)/@PACKAGE@">`pkgincludedir`</SwmToken>, <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="94:0:0" line-data="pkglibdir = $(libdir)/@PACKAGE@">`pkglibdir`</SwmToken>, and <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="95:0:0" line-data="pkglibexecdir = $(libexecdir)/@PACKAGE@">`pkglibexecdir`</SwmToken> are set to define the package directories.

```in
pkgdatadir = $(datadir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkglibexecdir = $(libexecdir)/@PACKAGE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="97">

---

# Installation Commands

Commands for installing data, programs, and scripts are defined using <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="97:0:0" line-data="install_sh_DATA = $(install_sh) -c -m 644">`install_sh_DATA`</SwmToken>, <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="98:0:0" line-data="install_sh_PROGRAM = $(install_sh) -c">`install_sh_PROGRAM`</SwmToken>, and <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="99:0:0" line-data="install_sh_SCRIPT = $(install_sh) -c">`install_sh_SCRIPT`</SwmToken>.

```in
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="108">

---

# Build Configuration

Variables <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="108:0:0" line-data="build_triplet = @build@">`build_triplet`</SwmToken>, <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="109:0:0" line-data="host_triplet = @host@">`host_triplet`</SwmToken>, and <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="110:0:0" line-data="target_triplet = @target@">`target_triplet`</SwmToken> are set to define the build, host, and target configurations.

```in
build_triplet = @build@
host_triplet = @host@
target_triplet = @target@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="111">

---

# Subdirectory and Dependencies

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="111:0:0" line-data="subdir = src/machine/elements/memoire">`subdir`</SwmToken> variable is set to <SwmPath>[src/machine/elements/memoire/](src/machine/elements/memoire/)</SwmPath>, and dependencies for <SwmPath>[aclocal.m4](aclocal.m4)</SwmPath> are listed.

```in
subdir = src/machine/elements/memoire
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

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="119">

---

# Common Distribution Files

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="119:0:0" line-data="DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)">`DIST_COMMON`</SwmToken> variable includes common distribution files like <SwmPath>[Makefile.am](Makefile.am)</SwmPath>.

```in
DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="124">

---

# Library Configuration

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="124:0:0" line-data="LTLIBRARIES = $(noinst_LTLIBRARIES)">`LTLIBRARIES`</SwmToken> variable is set to <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="124:4:7" line-data="LTLIBRARIES = $(noinst_LTLIBRARIES)">`$(noinst_LTLIBRARIES)`</SwmToken>, and object files for the library are listed.

```in
LTLIBRARIES = $(noinst_LTLIBRARIES)
libmachineelementsmemoire_la_DEPENDENCIES =
am_libmachineelementsmemoire_la_OBJECTS = bibliotheque.lo booleen.lo \
	chaine.lo descriptionbloc.lo element.lo entier.lo extension.lo \
	interruption.lo memoire.lo point_entree_extension.lo \
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="136">

---

# Linking Libraries

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="136:0:0" line-data="libmachineelementsmemoire_la_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX \">`libmachineelementsmemoire_la_LINK`</SwmToken> variable is used to link the library with specified flags.

```in
libmachineelementsmemoire_la_LINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX \
	$(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link $(CXXLD) \
	$(AM_CXXFLAGS) $(CXXFLAGS) \
	$(libmachineelementsmemoire_la_LDFLAGS) $(LDFLAGS) -o $@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="156">

---

# Compilation Commands

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="156:0:0" line-data="CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \">`CXXCOMPILE`</SwmToken> variable defines the command for compiling C++ source files.

```in
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="167">

---

# Linking Commands

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="167:0:0" line-data="CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`CXXLINK`</SwmToken> variable defines the command for linking C++ object files.

```in
CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \
	$(LIBTOOLFLAGS) --mode=link $(CXXLD) $(AM_CXXFLAGS) \
	$(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -o $@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="192">

---

# Source Files

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="192:0:0" line-data="SOURCES = $(libmachineelementsmemoire_la_SOURCES)">`SOURCES`</SwmToken> variable lists the source files for the library.

```in
SOURCES = $(libmachineelementsmemoire_la_SOURCES)
DIST_SOURCES = $(libmachineelementsmemoire_la_SOURCES)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="194">

---

# Recursive Targets

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="194:0:0" line-data="RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \">`RECURSIVE_TARGETS`</SwmToken> variable lists targets that should be built recursively.

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

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="207">

---

# Clean Targets

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="207:0:0" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`RECURSIVE_CLEAN_TARGETS`</SwmToken> variable lists targets for cleaning the build directory.

```in
RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\
  distclean-recursive maintainer-clean-recursive
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="469">

---

# Include Dependencies

Include dependency files for various source files to ensure proper dependency tracking.

```in
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/bibliotheque.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/booleen.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/chaine.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/descriptionbloc.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/element.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/entier.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/extension.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/interruption.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/memoire.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/point_entree_extension.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/pointeur.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/symbole.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/type.Plo@am__quote@
@AMDEP_TRUE@@am__include@ @am__quote@./$(DEPDIR)/valeur.Plo@am__quote@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="484">

---

# Compilation Rules

Rules for compiling <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="484:0:1" line-data=".cpp.o:">`.cpp`</SwmToken> files to <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="484:2:3" line-data=".cpp.o:">`.o`</SwmToken>, <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="491:2:3" line-data=".cpp.obj:">`.obj`</SwmToken>, and <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="126:5:6" line-data="am_libmachineelementsmemoire_la_OBJECTS = bibliotheque.lo booleen.lo \">`.lo`</SwmToken> files are defined.

```in
.cpp.o:
@am__fastdepCXX_TRUE@	$(AM_V_CXX)$(CXXCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ $<
@am__fastdepCXX_TRUE@	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	$(AM_V_CXX)source='$<' object='$@' libtool=no @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCXX_FALSE@	$(AM_V_CXX@am__nodep@)$(CXXCOMPILE) -c -o $@ $<

.cpp.obj:
@am__fastdepCXX_TRUE@	$(AM_V_CXX)$(CXXCOMPILE) -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c -o $@ `$(CYGPATH_W) '$<'`
@am__fastdepCXX_TRUE@	$(AM_V_at)$(am__mv) $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	$(AM_V_CXX)source='$<' object='$@' libtool=no @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCXX_FALSE@	$(AM_V_CXX@am__nodep@)$(CXXCOMPILE) -c -o $@ `$(CYGPATH_W) '$<'`
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="505">

---

# Clean Rules

Rules for cleaning up generated files, including <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="126:5:6" line-data="am_libmachineelementsmemoire_la_OBJECTS = bibliotheque.lo booleen.lo \">`.lo`</SwmToken> and <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="509:7:8" line-data="	-rm -rf .libs _libs">`.libs`</SwmToken> directories.

```in
mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="511">

---

# Recursive Make

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="517:0:3" line-data="$(am__recursive_targets):">`$(am__recursive_targets)`</SwmToken> rule allows for recursive make calls in subdirectories.

```in
# This directory's subdirectories are mostly independent; you can cd
# into them and run 'make' without going through this Makefile.
# To change the values of 'make' variables: instead of editing Makefiles,
# (1) if the variable is set in 'config.status', edit 'config.status'
#     (which will cause the Makefiles to be regenerated when you run 'make');
# (2) otherwise, pass the desired values on the 'make' command line.
$(am__recursive_targets):
	@fail=; \
	if $(am__make_keepgoing); then \
	  failcom='fail=yes'; \
	else \
	  failcom='exit 1'; \
	fi; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	case "$@" in \
	  distclean-* | maintainer-clean-*) list='$(DIST_SUBDIRS)' ;; \
	  *) list='$(SUBDIRS)' ;; \
	esac; \
	for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="545">

---

# Tagging Files

Rules for generating tags for source files using <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="553:10:10" line-data="	if ($(ETAGS) --etags-include --version) &gt;/dev/null 2&gt;&amp;1; then \">`etags`</SwmToken> and <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="195:1:1" line-data="	ctags-recursive dvi-recursive html-recursive info-recursive \">`ctags`</SwmToken>.

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

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="610">

---

# Distribution Rules

Rules for creating distribution archives of the source files.

```in
distdir: $(DISTFILES)
	@srcdirstrip=`echo "$(srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	topsrcdirstrip=`echo "$(top_srcdir)" | sed 's/[].[^$$\\*]/\\\\&/g'`; \
	list='$(DISTFILES)'; \
	  dist_files=`for file in $$list; do echo $$file; done | \
	  sed -e "s|^$$srcdirstrip/||;t" \
	      -e "s|^$$topsrcdirstrip/|$(top_builddir)/|;t"`; \
	case $$dist_files in \
	  */*) $(MKDIR_P) `echo "$$dist_files" | \
			   sed '/\//!d;s|^|$(distdir)/|;s,/[^/]*$$,,' | \
			   sort -u` ;; \
	esac; \
	for file in $$dist_files; do \
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="670">

---

# Installation Rules

Rules for installing the built files to the appropriate directories.

```in
install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="700">

---

# Clean Targets

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="700:0:0" line-data="clean: clean-recursive">`clean`</SwmToken> target depends on <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="700:3:5" line-data="clean: clean-recursive">`clean-recursive`</SwmToken> and other clean targets to ensure all generated files are removed.

```in
clean: clean-recursive

clean-am: clean-generic clean-libtool clean-noinstLTLIBRARIES \
	mostlyclean-am
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.in" line="773">

---

# Phony Targets

The <SwmToken path="src/machine/elements/memoire/Makefile.in" pos="773:0:1" line-data=".PHONY: $(am__recursive_targets) CTAGS GTAGS TAGS all all-am check \">`.PHONY`</SwmToken> target lists all phony targets to avoid conflicts with files of the same name.

```in
.PHONY: $(am__recursive_targets) CTAGS GTAGS TAGS all all-am check \
	check-am clean clean-generic clean-libtool \
	clean-noinstLTLIBRARIES cscopelist-am ctags ctags-am distclean \
	distclean-compile distclean-generic distclean-libtool \
	distclean-tags distdir dvi dvi-am html html-am info info-am \
	install install-am install-data install-data-am install-dvi \
	install-dvi-am install-exec install-exec-am install-html \
	install-html-am install-info install-info-am install-man \
	install-pdf install-pdf-am install-ps install-ps-am \
	install-strip installcheck installcheck-am installdirs \
	installdirs-am maintainer-clean maintainer-clean-generic \
	mostlyclean mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool pdf pdf-am ps ps-am tags tags-am uninstall \
	uninstall-am

.PRECIOUS: Makefile
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.am" line="1">

---

# <SwmPath>[Makefile.am](Makefile.am)</SwmPath> Header

The <SwmPath>[Makefile.am](Makefile.am)</SwmPath> includes licensing information and a disclaimer of warranty.

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

<SwmSnippet path="/src/machine/elements/memoire/Makefile.am" line="20">

---

# Compiler Flags

The <SwmToken path="src/machine/elements/memoire/Makefile.am" pos="20:0:0" line-data="AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}">`AM_CPPFLAGS`</SwmToken> variable is set to include the top build and source directories.

```am
AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/memoire/Makefile.am" line="22">

---

# Subdirectories

The <SwmToken path="src/machine/elements/memoire/Makefile.am" pos="22:0:0" line-data="SUBDIRS= ">`SUBDIRS`</SwmToken> variable is set to an empty value, indicating no subdirectories.

```am
SUBDIRS= 
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
