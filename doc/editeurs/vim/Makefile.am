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

pkgvimdir=$(pkgdatadir)/vim
pkgvimsyntaxdir=$(pkgvimdir)/syntax

dist_pkgvim_DATA=filetype.vim
dist_pkgvimsyntax_DATA=svm.vim svm_plugin.vim svm_log.vim

install-data-hook:
	for file in ${dist_pkgvim_DATA} ; do \
	$(SED) -i 's/===VIMSYNTAXDIR===/$(subst /,\/,$(pkgvimsyntaxdir))/' $(DESTDIR)$(pkgvimdir)/$$file ; \
	$(SED) -i 's/===INCLUDEFILE===/$(subst /,\/,$(pkgincludedir)/svm.h)/' $(DESTDIR)$(pkgvimdir)/$$file ; \
	done ; \
	if [ -f $(top_builddir)/src/machine/interface/svm_plugin.vim ] ; then extra=$(top_builddir)/src/machine/interface/svm_plugin.vim; fi ; \
	if [ -f $(top_srcdir)/src/machine/interface/svm_plugin.vim ] ; then extra=$(top_srcdir)/src/machine/interface/svm_plugin.vim; fi ; \
	if [ -f $$extra ] ; then cat $$extra >> $(DESTDIR)$(pkgvimsyntaxdir)/svm_plugin.vim; fi ;
