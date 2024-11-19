---
title: Building the Processeur Directory with Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/machine/elements/processeur/](src/machine/elements/processeur/)</SwmPath> directory. It will cover the configuration and build steps defined in the Makefile.

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="36">

---

# Setting VPATH

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="36:0:0" line-data="VPATH = @srcdir@">`VPATH`</SwmToken> variable is set to <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="36:4:6" line-data="VPATH = @srcdir@">`@srcdir@`</SwmToken>, which specifies the directory where the source files are located.

```in
VPATH = @srcdir@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="37">

---

# Checking for GNU Make

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="37:0:0" line-data="am__is_gnu_make = { \">`am__is_gnu_make`</SwmToken> variable checks if the current make tool is GNU Make.

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

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="48">

---

# Handling Make Options

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="48:0:0" line-data="am__make_running_with_option = \">`am__make_running_with_option`</SwmToken> variable and related code handle different make options such as dry run and keep going.

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

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="92">

---

# Package Directories

Variables like <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="92:0:0" line-data="pkgdatadir = $(datadir)/@PACKAGE@">`pkgdatadir`</SwmToken>, <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="93:0:0" line-data="pkgincludedir = $(includedir)/@PACKAGE@">`pkgincludedir`</SwmToken>, <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="94:0:0" line-data="pkglibdir = $(libdir)/@PACKAGE@">`pkglibdir`</SwmToken>, and <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="95:0:0" line-data="pkglibexecdir = $(libexecdir)/@PACKAGE@">`pkglibexecdir`</SwmToken> define the directories for package data, include files, libraries, and executable libraries.

```in
pkgdatadir = $(datadir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkglibexecdir = $(libexecdir)/@PACKAGE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="97">

---

# Installation Commands

Variables like <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="97:0:0" line-data="install_sh_DATA = $(install_sh) -c -m 644">`install_sh_DATA`</SwmToken>, <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="98:0:0" line-data="install_sh_PROGRAM = $(install_sh) -c">`install_sh_PROGRAM`</SwmToken>, and <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="99:0:0" line-data="install_sh_SCRIPT = $(install_sh) -c">`install_sh_SCRIPT`</SwmToken> define the commands for installing data, programs, and scripts.

```in
install_sh_DATA = $(install_sh) -c -m 644
install_sh_PROGRAM = $(install_sh) -c
install_sh_SCRIPT = $(install_sh) -c
INSTALL_HEADER = $(INSTALL_DATA)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="108">

---

# Build Triplets

Variables <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="108:0:0" line-data="build_triplet = @build@">`build_triplet`</SwmToken>, <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="109:0:0" line-data="host_triplet = @host@">`host_triplet`</SwmToken>, and <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="110:0:0" line-data="target_triplet = @target@">`target_triplet`</SwmToken> define the build, host, and target system triplets.

```in
build_triplet = @build@
host_triplet = @host@
target_triplet = @target@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="111">

---

# Subdirectory and Dependencies

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="111:0:0" line-data="subdir = src/machine/elements/processeur">`subdir`</SwmToken> variable specifies the current subdirectory, and <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="113:0:0" line-data="am__aclocal_m4_deps = $(top_srcdir)/m4/libtool.m4 \">`am__aclocal_m4_deps`</SwmToken> lists the dependencies for <SwmPath>[aclocal.m4](aclocal.m4)</SwmPath>.

```in
subdir = src/machine/elements/processeur
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

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="119">

---

# Common Distribution Files

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="119:0:0" line-data="DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)">`DIST_COMMON`</SwmToken> variable lists common files for distribution.

```in
DIST_COMMON = $(srcdir)/Makefile.am $(am__DIST_COMMON)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="124">

---

# Library and Object Files

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="124:0:0" line-data="LTLIBRARIES = $(noinst_LTLIBRARIES)">`LTLIBRARIES`</SwmToken> variable lists the libraries to be built, and <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="128:0:0" line-data="libmachineelementsprocesseur_la_OBJECTS =  \">`libmachineelementsprocesseur_la_OBJECTS`</SwmToken> lists the object files for the <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="409:4:6" line-data="noinst_LTLIBRARIES = libmachineelementsprocesseur.la">`libmachineelementsprocesseur.la`</SwmToken> library.

```in
LTLIBRARIES = $(noinst_LTLIBRARIES)
libmachineelementsprocesseur_la_DEPENDENCIES =
am_libmachineelementsprocesseur_la_OBJECTS = etat.lo interruptions.lo \
	processeur.lo
libmachineelementsprocesseur_la_OBJECTS =  \
	$(am_libmachineelementsprocesseur_la_OBJECTS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="154">

---

# Compilation and Linking

Variables like <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="154:0:0" line-data="CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \">`CXXCOMPILE`</SwmToken>, <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="156:0:0" line-data="LTCXXCOMPILE = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`LTCXXCOMPILE`</SwmToken>, <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="164:0:0" line-data="CXXLD = $(CXX)">`CXXLD`</SwmToken>, and <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="165:0:0" line-data="CXXLINK = $(LIBTOOL) $(AM_V_lt) --tag=CXX $(AM_LIBTOOLFLAGS) \">`CXXLINK`</SwmToken> define the commands for compiling and linking C++ source files.

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
AM_V_CXXLD = $(am__v_CXXLD_@AM_V@)
am__v_CXXLD_ = $(am__v_CXXLD_@AM_DEFAULT_V@)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="192">

---

# Recursive Targets

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="192:0:0" line-data="RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \">`RECURSIVE_TARGETS`</SwmToken> variable lists the targets that should be built recursively.

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

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="205">

---

# Clean Targets

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="205:0:0" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`RECURSIVE_CLEAN_TARGETS`</SwmToken> variable lists the targets for cleaning up files.

```in
RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\
  distclean-recursive maintainer-clean-recursive
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="213">

---

# Tagging Files

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="213:0:0" line-data="am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)">`am__tagged_files`</SwmToken> variable lists the files to be tagged, and <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="217:0:0" line-data="am__uniquify_input = $(AWK) &#39;\">`am__uniquify_input`</SwmToken> ensures the list of sources is unique.

```in
am__tagged_files = $(HEADERS) $(SOURCES) $(TAGS_FILES) $(LISP)
# Read a list of newline-separated strings from the standard input,
# and print each of them once, without duplicates.  Input order is
# *not* preserved.
am__uniquify_input = $(AWK) '\
  BEGIN { nonempty = 0; } \
  { items[$$0] = 1; nonempty = 1; } \
  END { if (nonempty) { for (i in items) print i; }; } \
'
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="234">

---

# Distribution Files

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="234:0:0" line-data="DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)">`DISTFILES`</SwmToken> variable lists the files to be included in the distribution.

```in
DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="232">

---

# Subdirectories

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="232:0:0" line-data="DIST_SUBDIRS = $(SUBDIRS)">`DIST_SUBDIRS`</SwmToken> variable lists the subdirectories to be included in the distribution.

```in
DIST_SUBDIRS = $(SUBDIRS)
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="471">

---

# Compilation Rules

Rules for compiling <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="471:0:1" line-data=".cpp.o:">`.cpp`</SwmToken> files to <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="471:2:3" line-data=".cpp.o:">`.o`</SwmToken>, <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="478:2:3" line-data=".cpp.obj:">`.obj`</SwmToken>, and <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="126:5:6" line-data="am_libmachineelementsprocesseur_la_OBJECTS = etat.lo interruptions.lo \">`.lo`</SwmToken> object files.

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

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="492">

---

# Clean Rules

Rules for cleaning up generated files, including <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="126:5:6" line-data="am_libmachineelementsprocesseur_la_OBJECTS = etat.lo interruptions.lo \">`.lo`</SwmToken> files and the <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="496:7:8" line-data="	-rm -rf .libs _libs">`.libs`</SwmToken> directory.

```in
mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="498">

---

# Recursive Make

Rules for running <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="499:11:11" line-data="# into them and run &#39;make&#39; without going through this Makefile.">`make`</SwmToken> recursively in subdirectories.

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

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="532">

---

# Tagging Targets

Rules for generating tags for source files.

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

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="657">

---

# Installation Targets

Rules for installing the built files, including <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="658:0:2" line-data="install-exec: install-exec-recursive">`install-exec`</SwmToken> and <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="659:0:2" line-data="install-data: install-data-recursive">`install-data`</SwmToken> targets.

```in
install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-recursive
install-strip:
	if test -z '$(STRIP)'; then \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	      install; \
	else \
	  $(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	    install_sh_PROGRAM="$(INSTALL_STRIP_PROGRAM)" INSTALL_STRIP_FLAG=-s \
	    "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'" install; \
	fi
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="687">

---

# Clean Targets

Rules for cleaning up files, including <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="687:0:0" line-data="clean: clean-recursive">`clean`</SwmToken>, <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="206:1:1" line-data="  distclean-recursive maintainer-clean-recursive">`distclean`</SwmToken>, and <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="206:5:7" line-data="  distclean-recursive maintainer-clean-recursive">`maintainer-clean`</SwmToken> targets.

```in
clean: clean-recursive

clean-am: clean-generic clean-libtool clean-noinstLTLIBRARIES \
	mostlyclean-am
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/processeur/Makefile.in" line="758">

---

# Phony Targets

The <SwmToken path="src/machine/elements/processeur/Makefile.in" pos="760:0:1" line-data=".PHONY: $(am__recursive_targets) CTAGS GTAGS TAGS all all-am check \">`.PHONY`</SwmToken> target lists phony targets that should not be treated as files.

```in
.MAKE: $(am__recursive_targets) install-am install-strip

.PHONY: $(am__recursive_targets) CTAGS GTAGS TAGS all all-am check \
	check-am clean clean-generic clean-libtool \
	clean-noinstLTLIBRARIES cscopelist-am ctags ctags-am distclean \
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
