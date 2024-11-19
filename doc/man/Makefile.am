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

AM_CPPFLAGS=-I${top_builddir} -I${top_srcdir}

dist_man1_MANS=svm.man svm_plugin.man

dist_man7_MANS=svm_plugin_bool.man svm_plugin_com.man svm_plugin_int.man svm_plugin_math.man svm_plugin_mem.man svm_plugin_plugin.man svm_plugin_real.man svm_plugin_run.man svm_plugin_str.man svm_plugin_type.man

install-data-hook: ${top_srcdir}/debug/styles/liste_styles
	CAT=$${CAT:-cat}; SED=$${SED:-sed}; TR=$${TR:-tr}; DEBUG_STYLES=$$($$CAT $< | $$SED 's/^/"/; s/$$/"/'| $$TR "\n" " ") ;\
	for file in ${dist_man1_MANS} ; do \
		file=$$(echo $$file | sed 's/man$$/1/'); \
		sed -i 's/===PKGBINDIR===/$(subst /,\/,$(bindir))/' $(DESTDIR)$(man1dir)/$$file ; \
		sed -i 's/===PKGDATADIR===/$(subst /,\/,$(pkgdatadir))/' $(DESTDIR)$(man1dir)/$$file ; \
		sed -i 's/===PKGINCLUDEDIR===/$(subst /,\/,$(pkgincludedir))/' $(DESTDIR)$(man1dir)/$$file ; \
		sed -i 's/===PKGLIBDIR===/$(subst /,\/,$(pkglibdir))/' $(DESTDIR)$(man1dir)/$$file ; \
		sed -i 's/===EMAILCONTACT===/$(subst /,\/,$(PACKAGE_BUGREPORT))/' $(DESTDIR)$(man1dir)/$$file ; \
		sed -i 's/===DEBUG_DEFAULT_STYLE===/$(subst /,\/,$(DEFAULT_STYLE))/' $(DESTDIR)$(man1dir)/$$file ; \
		sed -i "s/===DEBUG_STYLES===/$(subst /,\/,$${DEBUG_STYLES})/" $(DESTDIR)$(man1dir)/$$file ; \
	done ; \
	for file in ${dist_man7_MANS} ; do \
		file=$$(echo $$file | sed 's/man$$/7/'); \
		sed -i 's/===PKGBINDIR===/$(subst /,\/,$(bindir))/' $(DESTDIR)$(man7dir)/$$file ; \
		sed -i 's/===PKGDATADIR===/$(subst /,\/,$(pkgdatadir))/' $(DESTDIR)$(man7dir)/$$file ; \
		sed -i 's/===PKGINCLUDEDIR===/$(subst /,\/,$(pkgincludedir))/' $(DESTDIR)$(man7dir)/$$file ; \
		sed -i 's/===PKGLIBDIR===/$(subst /,\/,$(pkglibdir))/' $(DESTDIR)$(man7dir)/$$file ; \
		sed -i 's/===EMAILCONTACT===/$(subst /,\/,$(PACKAGE_BUGREPORT))/' $(DESTDIR)$(man7dir)/$$file ; \
		sed -i 's/===DEBUG_DEFAULT_STYLE===/$(subst /,\/,$(DEFAULT_STYLE))/' $(DESTDIR)$(man7dir)/$$file ; \
		sed -i "s/===DEBUG_STYLES===/$(subst /,\/,$${DEBUG_STYLES})/" $(DESTDIR)$(man7dir)/$$file ; \
	done
