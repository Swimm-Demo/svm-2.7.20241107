---
title: Building the Elements Directory with Makefile
---
# Intro

This document explains how to use the Makefile in the <SwmPath>[src/machine/elements/](src/machine/elements/)</SwmPath> directory to build the project components.

<SwmSnippet path="/src/machine/elements/Makefile.in" line="35">

---

# Setting Up Variables

The <SwmToken path="src/machine/elements/Makefile.in" pos="35:0:0" line-data="VPATH = @srcdir@">`VPATH`</SwmToken> variable is set to the source directory, allowing the Makefile to locate source files.

```in
VPATH = @srcdir@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="91">

---

Variables like <SwmToken path="src/machine/elements/Makefile.in" pos="91:0:0" line-data="pkgdatadir = $(datadir)/@PACKAGE@">`pkgdatadir`</SwmToken>, <SwmToken path="src/machine/elements/Makefile.in" pos="92:0:0" line-data="pkgincludedir = $(includedir)/@PACKAGE@">`pkgincludedir`</SwmToken>, <SwmToken path="src/machine/elements/Makefile.in" pos="93:0:0" line-data="pkglibdir = $(libdir)/@PACKAGE@">`pkglibdir`</SwmToken>, and <SwmToken path="src/machine/elements/Makefile.in" pos="94:0:0" line-data="pkglibexecdir = $(libexecdir)/@PACKAGE@">`pkglibexecdir`</SwmToken> are defined to specify the installation directories for package data, headers, libraries, and executables.

```in
pkgdatadir = $(datadir)/@PACKAGE@
pkgincludedir = $(includedir)/@PACKAGE@
pkglibdir = $(libdir)/@PACKAGE@
pkglibexecdir = $(libexecdir)/@PACKAGE@
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="110">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="110:0:0" line-data="subdir = src/machine/elements">`subdir`</SwmToken> variable is set to <SwmPath>[src/machine/elements/](src/machine/elements/)</SwmPath>, indicating the current subdirectory.

```in
subdir = src/machine/elements
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="137">

---

# Recursive Targets

The <SwmToken path="src/machine/elements/Makefile.in" pos="137:0:0" line-data="RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \">`RECURSIVE_TARGETS`</SwmToken> variable lists targets that should be built recursively, such as <SwmToken path="src/machine/elements/Makefile.in" pos="137:4:6" line-data="RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \">`all-recursive`</SwmToken>, <SwmToken path="src/machine/elements/Makefile.in" pos="137:8:10" line-data="RECURSIVE_TARGETS = all-recursive check-recursive cscopelist-recursive \">`check-recursive`</SwmToken>, and <SwmToken path="src/machine/elements/Makefile.in" pos="142:7:9" line-data="	install-ps-recursive install-recursive installcheck-recursive \">`install-recursive`</SwmToken>.

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

<SwmSnippet path="/src/machine/elements/Makefile.in" line="150">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="150:0:0" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`RECURSIVE_CLEAN_TARGETS`</SwmToken> variable lists clean targets that should be executed recursively, such as <SwmToken path="src/machine/elements/Makefile.in" pos="150:4:6" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`mostlyclean-recursive`</SwmToken> and <SwmToken path="src/machine/elements/Makefile.in" pos="150:8:10" line-data="RECURSIVE_CLEAN_TARGETS = mostlyclean-recursive clean-recursive	\">`clean-recursive`</SwmToken>.

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

<SwmSnippet path="/src/machine/elements/Makefile.in" line="156">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="156:0:0" line-data="AM_RECURSIVE_TARGETS = $(am__recursive_targets:-recursive=) TAGS CTAGS \">`AM_RECURSIVE_TARGETS`</SwmToken> variable combines all recursive targets into a single list.

```in
AM_RECURSIVE_TARGETS = $(am__recursive_targets:-recursive=) TAGS CTAGS \
	distdir
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="354">

---

# Building and Cleaning

The <SwmToken path="src/machine/elements/Makefile.in" pos="354:0:0" line-data="all: all-recursive">`all`</SwmToken> target is defined to build all components by invoking the <SwmToken path="src/machine/elements/Makefile.in" pos="354:3:5" line-data="all: all-recursive">`all-recursive`</SwmToken> target.

```in
all: all-recursive
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="369">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="369:0:0" line-data="Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status">`Makefile`</SwmToken> target ensures that the Makefile is up to date by checking dependencies and running the <SwmToken path="src/machine/elements/Makefile.in" pos="369:17:19" line-data="Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status">`config.status`</SwmToken> script if necessary.

```in
Makefile: $(srcdir)/Makefile.in $(top_builddir)/config.status
	@case '$?' in \
	  *config.status*) \
	    cd $(top_builddir) && $(MAKE) $(AM_MAKEFLAGS) am--refresh;; \
	  *) \
	    echo ' cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe)'; \
	    cd $(top_builddir) && $(SHELL) ./config.status $(subdir)/$@ $(am__depfiles_maybe);; \
	esac;
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="387">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="387:0:2" line-data="mostlyclean-libtool:">`mostlyclean-libtool`</SwmToken> target removes intermediate files generated by libtool.

```in
mostlyclean-libtool:
	-rm -f *.lo
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="390">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="390:0:2" line-data="clean-libtool:">`clean-libtool`</SwmToken> target removes libtool-generated directories.

```in
clean-libtool:
	-rm -rf .libs _libs
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="582">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="582:0:0" line-data="clean: clean-recursive">`clean`</SwmToken> target invokes the <SwmToken path="src/machine/elements/Makefile.in" pos="582:3:5" line-data="clean: clean-recursive">`clean-recursive`</SwmToken> target to clean all components.

```in
clean: clean-recursive

```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="586">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="586:0:0" line-data="distclean: distclean-recursive">`distclean`</SwmToken> target removes all generated files, including the Makefile itself.

```in
distclean: distclean-recursive
	-rm -f Makefile
distclean-am: clean-am distclean-generic distclean-tags
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="630">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="630:0:2" line-data="maintainer-clean: maintainer-clean-recursive">`maintainer-clean`</SwmToken> target is intended for maintainers to remove files that require special tools to rebuild.

```in
maintainer-clean: maintainer-clean-recursive
	-rm -f Makefile
maintainer-clean-am: distclean-am maintainer-clean-generic
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="634">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="634:0:0" line-data="mostlyclean: mostlyclean-recursive">`mostlyclean`</SwmToken> target invokes the <SwmToken path="src/machine/elements/Makefile.in" pos="634:3:5" line-data="mostlyclean: mostlyclean-recursive">`mostlyclean-recursive`</SwmToken> target to remove most intermediate files.

```in
mostlyclean: mostlyclean-recursive

mostlyclean-am: mostlyclean-generic mostlyclean-libtool
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="550">

---

# Installation

The <SwmToken path="src/machine/elements/Makefile.in" pos="552:0:0" line-data="install: install-recursive">`install`</SwmToken> target invokes the <SwmToken path="src/machine/elements/Makefile.in" pos="552:3:5" line-data="install: install-recursive">`install-recursive`</SwmToken> target to install all components.

```in
installdirs: installdirs-recursive
installdirs-am:
install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="557">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="557:0:2" line-data="install-am: all-am">`install-am`</SwmToken> target ensures that all components are built before installation by invoking the <SwmToken path="src/machine/elements/Makefile.in" pos="557:5:7" line-data="install-am: all-am">`all-am`</SwmToken> target.

```in
install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/elements/Makefile.in" line="561">

---

The <SwmToken path="src/machine/elements/Makefile.in" pos="561:0:2" line-data="install-strip:">`install-strip`</SwmToken> target installs the components with stripped symbols for reduced size.

```in
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

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
