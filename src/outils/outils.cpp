/*
 * Simple Virtual Machine - A versatile and robust architecture to
 * easily write applications.
 * Copyright (C) 2021  Julien BRUGUIER
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <src/outils/outils.h>
#include <src/machine/extensions/analyseur/analyseur.h>

using namespace SVM::Outils;

std::string Generation::source(const Langage& langage, const std::string& configuration, const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	oss	<< std::endl;

	if(not langage)
	{
		oss	<< "#include <stdlib.h>" << std::endl
			<< "#include <stddef.h>" << std::endl
			<< "#include <stdio.h>" << std::endl;
	}
	else
	{
		oss	<< "#include <string>" << std::endl
			<< "#include <sstream>" << std::endl;
	}
	oss	<< std::endl;
	extension->directive_localisation(oss,"inclusions");
	oss	<< std::endl
		<< "#include <src/plugin.h>" << std::endl
		<< std::endl;
	std::string prefixe = langage.prefixe();
	if(langage)
	{
		oss 	<< "extern \"C\"" << std::endl
			<< "{" << std::endl
			<< std::endl;
	}
	oss	<< "void plugin_configure(void *plugin)" << std::endl
		<< "{" << std::endl
		<< "\t" << prefixe << "svm_plugin_configure(plugin," << std::endl;
	std::istringstream iss;
	iss.str(extension->_description->chaine_configuration());
	for(std::string ligne ; std::getline(iss, ligne) ; )
	{
		oss << "\t\t\"" << ligne << " \\n\"" << std::endl;
	}
	oss	<< "\t\t,SVM_API_SIGNATURE,SVM_VERSION);" << std::endl
		<< "}" << std::endl
		<< std::endl;
	oss	<< Fichiers::macros(langage)
		<< std::endl;
	auto code = extension->directive("code");
	if(static_cast<bool>(code))
	{
		if(langage)
		{
			oss 	<< "}" << std::endl
				<< std::endl;
		}
		extension->directive_localisation(oss,"code");
		oss	<< std::endl;
		if(langage)
		{
			oss 	<< "extern \"C\"" << std::endl
				<< "{" << std::endl;
		}
	}
	for(auto& o: extension->_definition)
	{
		o->generation(oss,langage);
	}
	oss	<< std::endl
		<< "/* Generic handling functions */" << std::endl 
		<< std::endl;
	auto initialisation = extension->directive("initialisation");
	if(static_cast<bool>(initialisation))
	{
		oss	<< "void plugin_initialisation(const void *svm)" << std::endl
			<< "{" << std::endl;
		extension->directive_localisation(oss,"initialisation");
		oss	<< "}" << std::endl
			<< std::endl;
	}
	auto demarrage = extension->directive("startup");
	if(static_cast<bool>(demarrage))
	{
		oss	<< "void plugin_startup(const void *svm)" << std::endl
			<< "{" << std::endl;
		extension->directive_localisation(oss,"startup");
		oss	<< "}" << std::endl
			<< std::endl;
	}
	auto finalisation = extension->directive("finalisation");
	if(static_cast<bool>(finalisation))
	{
		oss	<< "void plugin_finalisation(const void *svm)" << std::endl
			<< "{" << std::endl;
		extension->directive_localisation(oss,"finalisation");
		oss	<< "}" << std::endl
			<< std::endl;
	}
	if(langage)
	{
		oss 	<< "}" << std::endl;
	}
	return oss.str();
}

std::string Generation::inclusion(const Langage& langage, const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	oss	<< std::endl
		<< "#pragma once" << std::endl
		<< std::endl
		<< "#include \"" << REP_INCLUDE << "/svm_compatibility.h\"" << std::endl
		<< "#include \"" << REP_INCLUDE << "/svm.h\"" << std::endl
		<< std::endl;
	return oss.str();
}

std::string Generation::documentation(const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	oss	<< ".TH \"Simple Virtual Machine - Plugin " << extension->_nom;
	auto titre = extension->directive("titre");
	if(static_cast<bool>(titre))
	{
		oss	<< ". " << (*titre);
	}
	oss	<< "\" 7 \"" << extension->directive_obligatoire("date") << "\"" << std::endl;
	auto synopsis = extension->directive("synopsis");
	if(static_cast<bool>(synopsis))
	{
		oss	<< ".SH SYNOPSIS" << std::endl
			<< ".B svm_plugin_" << extension->_nom << std::endl
			<< (*synopsis) << std::endl;
	}
	auto description = extension->directive("description");
	if(static_cast<bool>(description))
	{
		oss	<< ".SH DESCRIPTION" << std::endl
			<< (*description) << std::endl;
	}
	oss	<< ".SH INVOKATION" << std::endl
		<< "This plugin can be added to the Simple Virtual Machine using one of these directives:" << std::endl
		<< ".SS Local" << std::endl
		<< ".nf" << std::endl
		<< std::endl
		<< "LOCAL PLUGIN \"svmplugin" << extension->_nom << "/libsvm" << extension->_nom << ".so\"" << std::endl
		<< std::endl
		<< ".fi" << std::endl
		<< "when compiled/installed locally and should be accessible from the application directory." << std::endl
		<< ".SS Machine and System" << std::endl
		<< ".nf" << std::endl
		<< std::endl
		<< "PLUGIN \"===PLUGINLIB===\"" << std::endl
		<< std::endl
		<< ".fi" << std::endl
		<< "when installed with auto-tools and should be accessible from the Simple Virtual Machine installation directory or from system directories. (Recommended)" << std::endl
		<< ".SS Absolute path" << std::endl
		<< ".nf" << std::endl
		<< std::endl
		<< "PLUGIN \"===PLUGINLIBDIR===/===PLUGINLIB===\"" << std::endl
		<< std::endl
		<< ".fi" << std::endl
		<< "when installed with auto-tools and should be accessible from an absolute path." << std::endl;
	if(not extension->_dependance.empty())
	{
		oss	<< ".SH DEPENDENCIES" << std::endl;
		for(auto& o: extension->_dependance)
		{
			o->aide(oss);
		}
	}
	oss	<< ".SH CONTENT" << std::endl;
	for(auto& o: extension->_definition)
	{
		o->aide(oss);
	}
	if(not extension->_exemples.empty())
	{
		oss	<< ".SH EXAMPLE";
		if(extension->_exemples.size()>1)
		{
			oss	<< "S";
		}
		oss	<< std::endl;
		for(auto& e: extension->_exemples)
		{
			oss	<< ".SS " << e.first << std::endl
				<< e.second << std::endl;
		}
	}
	oss	<< ".SH SEE ALSO" << std::endl
		<< ".BR svm (1)" << std::endl
		<< "for help on the Simple Virtual Machine." << std::endl;
	auto voir_aussi = extension->directive("voir_aussi");
	if(static_cast<bool>(voir_aussi))
	{
		oss	<< (*voir_aussi) << std::endl;
	}
	oss	<< ".SH AUTHOR" << std::endl
		<< "This plugin has been written by " << extension->directive_obligatoire("auteur") << " and is maintained by " << extension->directive_obligatoire("mainteneur") << "." << std::endl;
	return oss.str();
}

std::string Generation::makefile(const Langage& langage, const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream liste_tests;
	for(const auto&f: extension->_tests)
	{
		liste_tests << " test/" << f.first << ".log";
	}
	std::ostringstream oss;
	oss	<< std::endl
		<< "include Makefile.inc" << std::endl
		<< std::endl
		<< "##### Configuration: default values" << std::endl
		<< std::endl
		<< "DEPENDENCIES=" << std::endl
		<< "GENERATED=" << std::endl
		<< "COMPILER=" << langage.compilateur() << std::endl
		<< std::endl
		<< "##### Configuration: to be overriden in file Makefile.config" << std::endl
		<< std::endl
		<< "-include Makefile.config" << std::endl
		<< std::endl
		<< "COPTIONS=" << extension->directive_facultative("options_compilation") << std::endl
		<< "LOPTIONS=" << extension->directive_facultative("options_lien") << std::endl
		<< std::endl
		<< ".PHONY: all check clean dist" << std::endl
		<< std::endl
		<< "all: libsvm" << extension->_nom << ".so" << std::endl
		<< "\t@mkdir -p .libs; ln -sf ../libsvm" << extension->_nom << ".so .libs" << std::endl
		<< "" << std::endl
		<< "check:";
	if(extension->_tests.empty())
	{
		oss	<< " all";
	}
	else
	{
		oss	<< liste_tests.str();
	}
	oss	<< std::endl
		<< "\t@echo; echo \"Test suite:\"; for t in" << liste_tests.str() << "; do echo -n \"  \"; tail -n 1 $$t; done; if [ " << extension->_tests.size() << " -eq 0 ] ; then echo \"No tests.\"; else ok=$$(tail -n 1" << liste_tests.str() << "| grep -c 'OK$$'); sk=$$(tail -n 1" << liste_tests.str() << "| grep -c 'SKIPPED$$'); echo \"Result: $$ok/$$(( " << extension->_tests.size() << " - $$sk)) passed.\" ; fi; echo;" << std::endl
		<< "" << std::endl
		<< "clean:" << std::endl
		<< "	rm -rf src/plugin.o $(DEPENDENCIES) $(GENERATED) libsvm" << extension->_nom << ".so" << liste_tests.str() << std::endl
		<< "" << std::endl
		<< "dist: all" << std::endl
		<< "	@echo \"Cleaning build system...\" ; rm -rf configure.ac Makefile.* */Makefile.* plugin_install update_patches src test plugin_configuration_string 2>/dev/null" << std::endl
		<< "" << std::endl
		<< "libsvm" << extension->_nom << ".so: $(DEPENDENCIES) src/plugin.o" << std::endl
		<< "\t$(COMPILER) -shared -o $@ -fPIC -DPIC -Wl,-L$(SVM) -lsvmplugininterface -Wl,-soname -Wl,libsvm" << extension->_nom << ".so $(COPTIONS) $^ $(LOPTIONS)" << std::endl
		<< "" << std::endl
		<< "." << langage.extension() << ".o:" << std::endl
		<< "\t$(COMPILER) -o $@ -c $< -fPIC -DPIC $(COPTIONS) -I." << std::endl
		<< std::endl
		<< "##### Extra rules: to be added in file Makefile.rules" << std::endl
		<< std::endl
		<< "-include Makefile.rules" << std::endl
		<< std::endl
		<< ".PRECIOUS: test/*.log" << std::endl
		<< std::endl
		<< "\%.log: \%.sh all" << std::endl
		<< "\t@s=\"$(subst test/,,$<)\" ; t=\"$(subst test/,,$@)\" ; cd test; echo \"Test case: $$s\" > $$t; ./$$s >> $$t 2>&1; r=$$?; case $$r in 0) rr=OK ;; 77) rr=SKIPPED ;; 99) rr=HARD_FAIL;; *) rr=\"FAIL (return code: $$r)\";; esac; echo \"Test $$s: $$rr\" >> $$t;" << std::endl
		<< std::endl;
	return oss.str();
}

std::string Generation::makefile_inc(const Langage& langage, const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	oss	<< std::endl
		<< "SVM=" << REP_INTERFACE << std::endl;
	return oss.str();
}

std::string Generation::makefile_am_doc(const Langage& langage, const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	oss	<< std::endl
		<< "SUBDIRS=" << std::endl
		<< std::endl
		<< "dist_man7_MANS=svm_plugin_" << extension->_nom << ".man" << std::endl
		<< "install-data-hook:" << std::endl
		<< "\tfor file in ${dist_man7_MANS} ; do \\" << std::endl
		<< "\t\tfile=$$(echo $$file | $(SED) 's/man$$/7/'); \\" << std::endl
		<< "\t\t$(SED) -i 's/===PLUGINBINDIR===/$(subst /,\\/,$(bindir))/' $(DESTDIR)$(man7dir)/$$file ; \\" << std::endl
		<< "\t\t$(SED) -i 's/===PLUGINDATADIR===/$(subst /,\\/,$(pkgdatadir))/' $(DESTDIR)$(man7dir)/$$file ; \\" << std::endl
		<< "\t\t$(SED) -i 's/===PLUGININCLUDEDIR===/$(subst /,\\/,$(pkgincludedir))/' $(DESTDIR)$(man7dir)/$$file ; \\" << std::endl
		<< "\t\t$(SED) -i 's/===PLUGINLIB===/libsvm" << extension->_nom << ".so/' $(DESTDIR)$(man7dir)/$$file ; \\" << std::endl
		<< "\t\t$(SED) -i 's/===PLUGINLIBDIR===/$(subst /,\\/,$(libdir))/' $(DESTDIR)$(man7dir)/$$file ; \\" << std::endl
		<< "\t\t$(SED) -i 's/===PLUGINCONTACT===/$(subst /,\\/,$(PACKAGE_BUGREPORT))/' $(DESTDIR)$(man7dir)/$$file ; \\" << std::endl
		<< "\t\t$(SED) -i 's/===SVMBIN===/$(subst /,\\/," << REP_BINAIRE << "/svm" << EXT_BINAIRE << ")/' $(DESTDIR)$(man7dir)/$$file ; \\" << std::endl
		<< "\t\t$(SED) -i 's/===SVMBINDIR===/$(subst /,\\/," << REP_BINAIRE << ")/' $(DESTDIR)$(man7dir)/$$file ; \\" << std::endl
		<< "\t\t$(SED) -i 's/===SVMINCLUDEDIR===/$(subst /,\\/," << REP_INCLUDE << ")/' $(DESTDIR)$(man7dir)/$$file ; \\" << std::endl
		<< "\t\t$(SED) -i 's/===SVMINTERFACEDIR===/$(subst /,\\/," << REP_INTERFACE << ")/' $(DESTDIR)$(man7dir)/$$file ; \\" << std::endl
		<< "\tdone" << std::endl;
	return oss.str();
}

std::string Generation::makefile_am_src(const Langage& langage, const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	oss	<< std::endl
		<< "AM_C" << langage.macro_am() << "FLAGS=-I${top_builddir} -I${top_srcdir}" << langage.option_compilateur() << " " << extension->directive_facultative("options_compilation") << std::endl
		<< std::endl
		<< "SUBDIRS=" << std::endl
		<< std::endl
		<< "noinst_LTLIBRARIES=libplugin.la" << std::endl
		<< std::endl
		<< "libplugin_la_SOURCES=plugin." << langage.extension() << " plugin.h" << std::endl
		<< "libplugin_la_LIBADD=" << std::endl
		<< "libplugin_la_LDFLAGS=-no-undefined" << std::endl;
	return oss.str();
}

std::string Generation::makefile_am_test(const Langage& langage, const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream f;
	std::ostringstream ff;
	bool debut = true;
	for(const auto& t: extension->_tests)
	{
		if(debut)
		{
			debut = false;
		}
		else
		{
			f << " ";
			ff << " ";
		}
		f << t.first << ".sh";
		ff << t.first << ".svm";
	}
	auto initialisation_test = extension->directive_facultative("initialisation_test");
	if(not initialisation_test.empty())
	{
		ff << " init.sh";
	}
	auto finalisation_test = extension->directive_facultative("finalisation_test");
	if(not finalisation_test.empty())
	{
		ff << " fini.sh";
	}
	std::ostringstream oss;
	oss	<< std::endl
		<< "AM_C" << langage.macro_am() << "FLAGS=-I${top_builddir} -I${top_srcdir}" << langage.option_compilateur() << " " << extension->directive_facultative("options_compilation") << std::endl
		<< std::endl
		<< "SUBDIRS=" << std::endl
		<< std::endl
		<< "EXTRA_DIST=" << f.str() << " " << ff.str() << std::endl
		<< std::endl
		<< "TESTS=" << f.str() << std::endl;
	return oss.str();
}

std::string Generation::makefile_am(const Langage& langage, const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	oss	<< std::endl
		<< "include Makefile.inc" << std::endl
		<< std::endl
		<< "AM_C" << langage.macro_am() << "FLAGS=-I${top_builddir} -I${top_srcdir}" << langage.option_compilateur() << " " << extension->directive_facultative("options_compilation") << std::endl
		<< std::endl
		<< "SUBDIRS=doc src test" << std::endl
		<< std::endl
		<< "lib_LTLIBRARIES=libsvm"<< extension->_nom << ".la" << std::endl
		<< std::endl
		<< "libsvm" << extension->_nom << "_la_SOURCES=" << std::endl
		<< "libsvm" << extension->_nom << "_la_LDFLAGS=-module -ldl -Wl,-rpath -Wl,${libdir} -Wl,-L$(SVM) " << extension->directive_facultative("options_lien") << " -release " << extension->directive_obligatoire("version") << std::endl
		<< "libsvm" << extension->_nom << "_la_LIBADD=src/libplugin.la" << std::endl
		<< "libsvm" << extension->_nom << "_la_LIBTOOLFLAGS=--tag=disable-static" << std::endl;
	return oss.str();
}

std::string Generation::configure_ac(const Langage& langage, const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	oss	<< std::endl
		<< "AC_PREREQ(2.59)" << std::endl
		<< "AC_INIT(svmplugin" << extension->_nom << "," << extension->directive_obligatoire("version") << "," << extension->directive_obligatoire("mainteneur") << ")" << std::endl
		<< std::endl
		<< "AC_CONFIG_MACRO_DIR([m4])" << std::endl
		<< "AC_CANONICAL_TARGET" << std::endl
		<< "LT_INIT" << std::endl
		<< "AM_INIT_AUTOMAKE" << std::endl
		<< "AC_CONFIG_SRCDIR([src/])" << std::endl
		<< std::endl
		<< "AM_CONFIG_HEADER([config.h])" << std::endl
		<< std::endl
		<< "AC_CHECK_PROG([DIFF],[diff],[diff -u])" << std::endl
		<< "AC_PROG_C" << (langage?"XX":"C") << std::endl;
	if(langage)
	{
		oss	<< "AC_LANG([C++])" << std::endl;
	}

	oss	<< "AM_PROG_LIBTOOL" << std::endl
		<< "AC_CHECK_TOOL([STRIP],[strip])" << std::endl
		<< std::endl;
	auto tests = extension->directive("tests");
	if(static_cast<bool>(tests))
	{
		oss	<< "# Specific plugin checks" << std::endl
			<< (*tests) << std::endl
			<< std::endl;
	}
	oss	<< "C" << langage.macro_ac() << "FLAGS_OLD=$C" << langage.macro_ac() << "FLAGS" << std::endl
		<< "C" << langage.macro_ac() << "FLAGS=\"$C" << langage.macro_ac() << "FLAGS $AM_C" << langage.macro_ac() << "FLAGS\"" << std::endl
		<< std::endl
		<< "AC_HEADER_ASSERT" << std::endl
		<< "AC_CHECK_HEADERS([" << REP_INCLUDE << "/svm.h " << REP_INCLUDE << "/svm_compatibility.h" << (langage?"":" stddef.h") << "],,[AC_MSG_ERROR([Header file required to compile not available !])])" << std::endl
		<< std::endl
		<< "C" << langage.macro_ac() << "FLAGS=$C" << langage.macro_ac() << "FLAGS_OLD" << std::endl
		<< std::endl
		<< "AC_TYPE_SIZE_T" << std::endl
		<< std::endl
		<< "AC_FUNC_STAT" << std::endl
		<< "AC_LTDL_DLLIB" << std::endl
		<< std::endl
		<< "AC_SUBST([AM_C" << langage.macro_ac() << "FLAGS])" << std::endl
		<< "AC_SUBST([AM_LDFLAGS])" << std::endl
		<< "AC_SUBST([LIBS])" << std::endl
		<< "AC_SUBST([DIFF])" << std::endl
		<< std::endl
		<< "AC_CONFIG_FILES([" << std::endl
		<< "\tMakefile" << std::endl
		<< "\tsrc/Makefile" << std::endl
		<< "\tdoc/Makefile" << std::endl
		<< "\ttest/Makefile" << std::endl
		<< "])" << std::endl
		<< "AC_OUTPUT" << std::endl;
	return oss.str();
}

std::string Generation::nouvelles(const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	auto corps = extension->directive("nouveautes");
	if(static_cast<bool>(corps))
	{
		oss << (*corps) << std::endl;
	}
	else
	{
		oss	<< "No news available." << std::endl
			<< "Please refer to the manpage svm_plugin_" << extension->_nom << "(7) for usage details." << std::endl;
	}
	return oss.str();
}

std::string Generation::lisez_moi(const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	auto corps = extension->directive("lisez_moi");
	if(static_cast<bool>(corps))
	{
		oss << (*corps) << std::endl;
	}
	else
	{
		oss << "Please refer to the manpage svm_plugin_" << extension->_nom << "(7) for more details." << std::endl;
	}
	return oss.str();
}

std::string Generation::auteur(const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	auto corps = extension->directive("auteur");
	if(static_cast<bool>(corps))
	{
		oss << "Author of this plugin: " << (*corps) << std::endl;
	}
	corps = extension->directive("mainteneur");
	if(static_cast<bool>(corps))
	{
		oss << "Maintainer of this plugin: " << (*corps) << std::endl;
	}
	return oss.str();
}

std::string Generation::historique(const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	auto corps = extension->directive("historique");
	if(static_cast<bool>(corps))
	{
		oss << (*corps) << std::endl;
	}
	else
	{
		oss	<< "No change log available." << std::endl
			<< "Please refer to the manpage svm_plugin_" << extension->_nom << "(7) for usage details." << std::endl;
	}
	return oss.str();
}

std::string Fichiers::script(const Langage& langage, const bool licence, const bool force, const bool patch, const std::string& shell, const std::string& nom, const std::string& repertoire, const Fichiers::Liste& fichiers, const std::map<std::string,std::string>& corrections)
{
	std::ostringstream oss;
	oss	<< "#!/bin/bash" << std::endl
		<< "echo \"Generating plugin " << nom << " in directory " << repertoire << "\"" << std::endl;
	if(force)
	{
		oss	<< "if [ -d " << repertoire << " ] ; then echo \"Directory already exists. Removal forced!\"; rm -rf " << repertoire << "; fi" << std::endl;
	}
	else
	{
		oss	<< "if [ -d " << repertoire << " ] ; then echo \"Directory already exists. Change directory with -d option, or force generation with -f option.\" >&2; exit 1; fi" << std::endl;
	}
	for(auto& f: fichiers)
	{
		std::string fichier = repertoire + "/" + f.first;
		oss	<< "mkdir -p $(dirname " << fichier << ")" << std::endl 
			<< "if [ $? -ne 0 ] ; then echo \"Unable to create plugin in directory " << repertoire << "\" >&2 ; exit 1; fi" << std::endl
			<< "cat > " << fichier <<  " << EOF_" << nom << std::endl
			<< f.second._valeur
			<< "EOF_" << nom << std::endl;
	}
	if(not licence)
	{
		std::string fichier = repertoire + "/COPYING";
		oss	<< "if [ -f " << fichier << " ] ; then" << std::endl
			<< "PS3='Your choice (Ctrl-C to abort)> '" << std::endl
			<< "select CHOICE in 'Remove license file' 'Keep license file' ;" << std::endl
			<< "do" << std::endl
			<< "case ${CHOICE} in" << std::endl
			<< "Keep*)" << std::endl
			<< "echo '" << fichier << " kept'" << std::endl
			<< "echo ''" << std::endl
			<< "break" << std::endl
			<< ";;" << std::endl
			<< "Remove*)" << std::endl
			<< "rm -rf " << fichier << std::endl
			<< "echo '" << fichier << " discarded'" << std::endl
			<< "echo ''" << std::endl
			<< "break" << std::endl
			<< ";;" << std::endl
			<< "esac" << std::endl
			<< "done" << std::endl
			<< "fi" << std::endl;
	}
	for(auto& a: corrections)
	{
		auto it = fichiers.find(a.first);
		if(it==fichiers.end())
		{
			std::ostringstream oss;
			oss << "Trying to patch non-generated file " << a.first;
			throw Erreur(oss.str());
		}
		if(patch)
		{
			std::string original = a.first+".orig";
			if(fichiers.find(original)!=fichiers.end())
			{
				std::ostringstream oss;
				oss << "Trying to save generated file into the generated file " << original;
				throw Erreur(oss.str());
			}
			oss	<< "cp " << repertoire << "/" << a.first << " " << repertoire << "/" << original << std::endl;
		}
		Fichier f(a.second);
		f.echappe_shell();
		oss	<< "{" << std::endl
			<< "cat << EOF_" << nom << std::endl
			<< f._valeur
			<< "EOF_" << nom << std::endl
			<< "}  | patch " << repertoire << "/" << a.first << std::endl;
	}
	oss	<< "chmod u+x " << repertoire << "/plugin_install" << std::endl;
	if(patch)
	{
		oss	<< "chmod u+x " << repertoire << "/update_patches" << std::endl;
	}
	if(not shell.empty())
	{
		oss	<< "echo \"Running custom shell commands:\"" << std::endl
			<< "echo" << std::endl
			<< "(" << std::endl
			<< "cd " << repertoire << std::endl
			<< "set -x" << std::endl
			<< shell << std::endl
			<< ")" << std::endl
			<< "echo" << std::endl;
	}
	oss	<< "chmod -f u+x " << repertoire << "/test/*.svm " << repertoire << "/test/*.sh " << repertoire << "/test/init.sh " << repertoire << "/test/fini.sh" << std::endl
		<< "" << std::endl
		<< "cat << EOF" << std::endl
		<< "" << std::endl
		<< "Generated files:" << std::endl;
	for(auto& f: fichiers)
	{
		oss	<< f.first << std::endl;
	}
	oss	<< "" << std::endl
		<< "For common installations of this plugin, you can use the provided install script." << std::endl
		<< "For more details, execute:" << std::endl
		<< std::endl
		<< "./" << repertoire << "/plugin_install -h" << std::endl
		<< std::endl
		<< "EOF" << std::endl;
	return oss.str();
}

Fichier Fichiers::fichier_vide()
{
	std::ostringstream oss;
	oss << "<TODO Replace me with proper information>" << std::endl;
	return Fichier(oss.str());
}

std::string Fichiers::macros(const Langage& langage)
{
	std::string prefix = langage.prefixe();
	std::ostringstream oss;
	oss	<< DebutMacro("ARGV_VALUE","index,type") << prefix << "svm_value_##type##_get(svm," << prefix << "svm_parameter_value_get(svm,argv[(index)]))" << FinMacro()
		<< DebutMacro("ARGV_PLUGIN","index,plugin,name");
	if(langage)
	{
		oss	<< "reinterpret_cast<type_##name*>";
	}
	else
	{
		oss	<< "(type_##name*)";
	}
	oss	<< "(" << prefix << "svm_value_plugin_get_internal(svm," << prefix << "svm_parameter_value_get(svm,argv[(index)])))" << FinMacro()
		<< DebutMacro("ARGV_MARKER","index");
	if(langage)
	{
		oss	<< "std::string(";
	}
	oss	<< prefix << "svm_parameter_marker_get(svm,argv[(index)])";
	if(langage)
	{
		oss	<< ".string)";
	}
	oss	<< FinMacro()
		<< DebutMacro("ARGV_KEYWORD","index");
	if(langage)
	{
		oss	<< "std::string(";
	}
	oss	<< prefix << "svm_parameter_keyword_get(svm,argv[(index)])";
	if(langage)
	{
		oss	<< ".string)";
	}
	oss	<< FinMacro()
		<< DebutMacro("ARGV_STRUCT","index,plugin,name");
	if(langage)
	{
		oss	<< "reinterpret_cast<struct_##name*>";
	}
	else
	{
		oss	<< "(struct_##name*)";
	}
	oss	<< "(" << prefix << "svm_structure_get_internal(svm," << prefix << "svm_value_pluginentrypoint_new__raw(svm,#plugin,#name)," << prefix << "svm_parameter_structure_get(svm,argv[(index)])))" << FinMacro()
		<< DebutMacro("ARGV_VARIABLE","index") << prefix << "svm_parameter_variable_get(svm,argv[(index)]);" << FinMacro()
		<< DebutMacro("NEW_VALUE","type,value") << prefix << "svm_value_##type##_new(svm,(value))" << FinMacro()
		<< DebutMacro("NEW_PLUGIN","plugin,name,value") << prefix << "svm_value_plugin_new(svm," << prefix << "svm_value_pluginentrypoint_new__raw(svm,#plugin,#name),(value))" << FinMacro()
		<< DebutMacro("NEW_STRUCT","plugin,name,value") << prefix << "svm_structure_new(svm," << prefix << "svm_value_pluginentrypoint_new__raw(svm,#plugin,#name),(value))" << FinMacro()
		<< DebutMacro("NEW_STRING","raw_string") << prefix << "svm_string_new";
	if(not langage)
	{
		oss	<< "__raw";
	}
	oss	<< "(svm,";
	if(langage)
	{
		oss	<< "raw_string.c_str(),raw_string.size()";
	}
	else
	{
		oss	<< "raw_string";
	}
	oss	<< ")" << FinMacro()
		<< DebutMacro("RAW_STRING","svm_string");
	if(langage)
	{
		oss	<< "std::string(svm_string.string,svm_string.size)";
	}
	else
	{
		oss	<< "svm_string.string";
	}
	oss	<< FinMacro()
		<< DebutMacro("NEW_NULL_VALUE","type") << prefix << "svm_value_##type##_new_null(svm)" << FinMacro()
		<< DebutMacro("NEW_NULL_PLUGIN","plugin,name") << prefix << "svm_value_plugin_new_null(svm," << prefix << "svm_value_pluginentrypoint_new__raw(svm,#plugin,#name))" << FinMacro()
		<< DebutMacro("NEW_NULL_STRUCT","plugin,name") << prefix << "svm_structure_new_null(svm," << prefix << "svm_value_pluginentrypoint_new__raw(svm,#plugin,#name))" << FinMacro()
		<< DebutMacro("ERROR_INTERNAL","irq,message") << prefix << "svm_processor_current_raise_error_internal__raw(svm,irq,message)" << FinMacro()
		<< DebutMacro("ERROR_EXTERNAL","plugin,name,message") << prefix << "svm_processor_current_raise_error_external__raw(svm," << prefix << "svm_value_pluginentrypoint_new__raw(svm,#plugin,#name),message)" << FinMacro()
		<< DebutMacro("CONST_PEP","plugin,name") << prefix << "svm_value_pluginentrypoint_new__raw(svm,#plugin,#name)" << FinMacro()
		<< DebutMacro("CURRENT","object") << prefix << "svm_##object##_get_current(svm)" << FinMacro()
		<< DebutMacro("RETURN") << "return " << langage.null() << FinMacro()
		<< DebutMacro("VARIABLE_GLOBAL","variable") << prefix << "svm_variable_scope_set_global(svm,(variable))" << FinMacro()
		<< DebutMacro("VARIABLE_LOCAL","variable") << prefix << "svm_variable_scope_set_local(svm,(variable))" << FinMacro()
		<< DebutMacro("VARIABLE_DELETE","variable") << prefix << "svm_variable_delete(svm,(variable))" << FinMacro();
	return oss.str();
}

void Fichier::ajoute_licence(const std::string& licence, const Commentaire& commentaire)
{
	std::ostringstream oss;
	if(not _interpreteur.empty())
	{
		oss	<< "#!" << _interpreteur << std::endl
			<< std::endl;
	}
	Fichiers::commente(licence,commentaire,oss);
	oss	<< std::endl
		<< _valeur;
	_valeur = oss.str();
}
void Fichiers::commente(const std::string& texte, const Commentaire& commentaire, std::ostream& os)
{
	os << commentaire._prefixe << std::endl;
	std::istringstream iss;
	iss.str(texte);
	for(std::string ligne ; std::getline(iss, ligne) ; )
	{
		os << commentaire._ligne << ligne << std::endl;
	}
	os << commentaire._suffixe << std::endl;
}

void Fichier::echappe_shell()
{
	std::string r;
	for(auto &c: _valeur)
	{
		if(c=='$')
		{
			r += "\\$";
			continue;
		}
		if(c=='\\')
		{
			r += "\\\\";
			continue;
		}
		r += c;
	}
	_valeur = r;
}

void Fichier::lignes_source(const std::string& nom)
{
	std::istringstream iss;
	std::ostringstream oss;
	iss.str(_valeur);
	size_t numero = 1;
	for(std::string ligne ; std::getline(iss, ligne) ; )
	{
		++numero;
		if(ligne=="#line")
		{
			oss << "#line " << numero << " \"" << nom << "\"" << std::endl;
		}
		else
		{
			oss << ligne << std::endl;
		}
	}
	_valeur = oss.str();
}

void Extension::verifie_et_complete(const std::string& fichier, SVM::Machine::Extension::DescriptionSP& description, std::string& configuration, Langage& langage, bool& licence, std::string& shell)
{
	_fichier = fichier;
	std::string date_courante;
	{
		char d[100];
		time_t t;
		t = ::time(NULL);
		::strftime(d, sizeof(d), "%Y-%m-%d", ::localtime(&t));
		date_courante = d;
	}
	{
		nouvelle_directive("langage","C++");
		nouvelle_directive("version","0.0");
		nouvelle_directive("auteur","Anonymous");
		nouvelle_directive("mainteneur","Anonymous <anonymous@example.com>");
		if(nouvelle_directive("date",date_courante))
		{
			auto date = directive_obligatoire("date");
			struct tm tm;
			if(not ::strptime(date.c_str(),"%Y-%m-%d",&tm))
			{
				throw Erreur("Invalid date or invalid date format (YYYY-MM-DD required)");
			}
		}
		nouveau_commentaire("source","/*"," * "," */");
		nouveau_commentaire("shell","#","# ","#");
		nouveau_commentaire("make","#","# ","#");
		nouveau_commentaire("conf","#","# ","#");
		nouveau_commentaire("svm","#","# ","#");
	}
	langage = Langage(directive_obligatoire("langage"));
	auto licence_longue = directive("licence_longue");
	auto licence_courte = directive("licence_courte");
	licence = static_cast<bool>(licence_longue);
	if(licence and not licence_courte)
	{
		throw Erreur("no usage specified while license specified");
	}
	if(not licence and not licence_courte)
	{
		std::ostringstream oss;
		oss	<< "Copyright (C) " << directive_obligatoire("date") << "  " << directive_obligatoire("auteur") << std::endl
			<< std::endl;
		auto titre = directive("titre");
		if(static_cast<bool>(titre))
		{
			oss	<< (*titre) << std::endl
				<< std::endl;
		}
		oss	
			<< "This program is free software: you can redistribute it and/or modify"  << std::endl
			<< "it under the terms of the GNU General Public License as published by" << std::endl
			<< "the Free Software Foundation, either version 3 of the License, or" << std::endl
			<< "(at your option) any later version." << std::endl
			<< std::endl
			<< "This program is distributed in the hope that it will be useful," << std::endl
			<< "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl
			<< "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl
			<< "GNU General Public License for more details." << std::endl
			<< std::endl
			<< "You should have received a copy of the GNU General Public License" << std::endl
			<< "along with this program.  If not, see <http://www.gnu.org/licenses/>." << std::endl;
		nouvelle_directive("licence_courte",oss.str());

	}
	shell = directive_facultative("shell");
	configuration = chaine_configuration();
	try
	{
		description = _description = SVM::Machine::Extension::Analyseur::analyse(_nom,configuration);
	}
	catch(...)
	{
		std::cerr	<< std::endl
				<< "Generated plugin configuration string:" << std::endl
				<< configuration << std::endl;
		throw;
	}
	{
		auto itu = description->_utilise.begin();
		for(auto& d:_dependance)
		{
			auto dobj = std::dynamic_pointer_cast<DefinitionObjet>(d);
			if(static_cast<bool>(dobj))
			{
				dobj->_objet = *itu;
				++itu;
			}
		}
		auto itd = description->_declare.begin();
		for(auto& d:_definition)
		{
			d->_fichier = _fichier;
			auto dobj = std::dynamic_pointer_cast<DefinitionObjet>(d);
			if(static_cast<bool>(dobj))
			{
				dobj->_objet = *itd;
				++itd;
			}
		}
	}
}

std::string Extension::chaine_configuration() const
{
	std::ostringstream oss;
	oss	<< "PLUGIN " << _nom << std::endl;
	if(not _dependance.empty())
	{
		oss	<< "USE" << std::endl;
		for(const auto& d: _dependance)
		{
			d->prototype(oss);
		}
	}
	oss	<< "DEFINE" << std::endl;
	for(const auto& d: _definition)
	{
		d->prototype(oss);
	}
	return oss.str();
}

Fichier Generation::test_source(const Test& t, const ExtensionSP& extension)
{
	std::ostringstream i;
	i << REP_BINAIRE << "/svm";
	std::ostringstream oss;
	oss	<< "LOG" << std::endl
		<< "PLUGIN \"../../.libs/libsvm" << extension->_nom << ".so\"";
	if(not t._commande.empty())
	{
		oss	<< " " << t._commande;
	}
	oss	<< std::endl
		<< t._contenu._valeur;
	return Fichier(t._contenu._type,i.str(),oss.str());
}

Fichier Generation::test_script(const std::string& nom, const Test& t, const bool initialisation, const bool finalisation, const SVM::Outils::ExtensionSP& extension)
{
	std::ostringstream oss;
	oss	<< "cd $(dirname $0)" << std::endl
		<< "DIR=$(pwd)" << std::endl
		<< "mkdir -p test_" << nom << std::endl
		<< "cd test_" << nom << std::endl;
	static std::vector<std::string> commandes= { "svm", "svm_plugin", "svm_config"};
	for(const auto& c: commandes)
	{
		oss	<< "export " << c << "=" << REP_BINAIRE << "/" << c << EXT_BINAIRE << std::endl;
	}
	if(initialisation)
	{
		oss	<< "$DIR/init.sh" << std::endl;
	}
	oss	<< "$SVM_TOOL $DIR/" << nom << ".svm $@" << std::endl
		<< "RES=$?" << std::endl;
	if(finalisation)
	{
		oss	<< "$DIR/fini.sh" << std::endl;
	}
	oss	<< "cd .." << std::endl
		<< "rm -rf test_" << nom << std::endl
		<< "exit $RES" << std::endl;
	return Fichier("shell","/bin/sh",oss.str());
}

std::string Generation::installation(const std::string& nom)
{
	std::ostringstream oss;
	oss	<< "cd $(dirname $0)" << std::endl
		<< "init_autotools() {" << std::endl
		<< "# default arguments" << std::endl
		<< std::endl
		<< "ACLOCAL=aclocal" << std::endl
		<< "AUTOCONF=autoconf" << std::endl
		<< "AUTOHEADER=autoheader" << std::endl
		<< "AUTOUPDATE=autoupdate" << std::endl
		<< "AUTOMAKE=automake" << std::endl
		<< "LIBTOOLIZE=libtoolize" << std::endl
		<< std::endl
		<< "ACLOCAL_ARGS=\"\"" << std::endl
		<< std::endl
		<< "if [ -f init_autotools.local ]; then" << std::endl
		<< "  chmod u+x init_autotools.local" << std::endl
		<< "  . ./init_autotools.local" << std::endl
		<< "fi" << std::endl
		<< std::endl
		<< "abort () {" << std::endl
		<< "    echo \"$1 not found or command failed. Aborting!\"" << std::endl
		<< "    exit 1" << std::endl
		<< "}" << std::endl
		<< std::endl
		<< "rm -rf autom4te.cache/" << std::endl
		<< "rm -f aclocal.m4 && touch aclocal.m4 && chmod +w aclocal.m4" << std::endl
		<< "$AUTOUPDATE || abort \"autoupdate\"" << std::endl
		<< "$LIBTOOLIZE --force --copy --automake || abort \"libtoolize\"" << std::endl
		<< "$ACLOCAL $ACLOCAL_ARGS || abort \"aclocal\"" << std::endl
		<< "$AUTOHEADER || abort \"autoheader\"" << std::endl
		<< "$AUTOMAKE --add-missing --copy --gnu || abort \"automake\"" << std::endl
		<< "$AUTOCONF || abort \"autoconf\"" << std::endl
		<< "}" << std::endl
		<< "getopts :lcmsh O" << std::endl
		<< "shift" << std::endl
		<< std::endl
		<< "case $O in" << std::endl
		<< "	l)" << std::endl
		<< "		make -f Makefile.local \"$@\"" << std::endl
		<< "	;;" << std::endl
		<< "	c)" << std::endl
		<< "		init_autotools" << std::endl
		<< "	;;" << std::endl
		<< "	m)" << std::endl
		<< "		init_autotools" << std::endl
		<< "		./configure --prefix='" << MACHINE_PLUGINS << "' \"$@\" && make -j && (make install || sudo make install || su -c \"make install\")" << std::endl
		<< "	;;" << std::endl
		<< "	s)" << std::endl
		<< "		init_autotools" << std::endl
		<< "		./configure --prefix='" << REP_INSTALL << "' \"$@\" && make -j && (make install || sudo make install || su -c \"make install\")" << std::endl
		<< "	;;" << std::endl
		<< "	*)" << std::endl
		<< "		cat << EOM" << std::endl
		<< "$0 OPTION [ARGUMENTS]" << std::endl
		<< std::endl
		<< "Perform a common installation of the plugin:" << std::endl
		<< "" << std::endl
		<< "-l: Local installation. Recommended for plugins specific to an application. Use the LOCAL PLUGIN directive to use it in applications." << std::endl
		<< "-c: Custom auto-tools installation. Use the PLUGIN directive to use it in applications." << std::endl
		<< "-m: Machine auto-tools installation. Recommended for generic plugins used with any Simple Virtual Machine installation. Use the MACHINE PLUGIN directive to use it in applications." << std::endl
		<< "-s: System auto-tools installation. Recommended for generic plugins used with a Simple Virtual Machine installed at system level. Use the PLUGIN with absolute path to use it in applications." << std::endl
		<< std::endl
		<< "With -l option, extra ARGUMENTS are passed to make." << std::endl
		<< "Otherwise, extra ARGUMENTS are passed to the configure script." << std::endl
		<< std::endl
		<< "-h: Display this help." << std::endl
		<< std::endl
		<< "EOM" << std::endl
		<< "	;;" << std::endl
		<< "esac" << std::endl;
	return oss.str();
}

std::string Generation::update_patches()
{
	std::ostringstream oss;
	oss	<< "cd $(dirname $0)" << std::endl
		<< "find * -name '*.orig' | while read ORIG ;" << std::endl
		<< "do" << std::endl
		<< "   FILE=${ORIG%.orig}" << std::endl
		<< "   if [ -f $FILE ] ; then" << std::endl
		<< "      echo \"patch: \\\"$FILE\\\"\"" << std::endl
		<< "      echo \"%{\"" << std::endl
		<< "      diff -u $ORIG $FILE" << std::endl
		<< "      echo \"%}\"" << std::endl
		<< "      echo" << std::endl
		<< "   fi" << std::endl
		<< "done" << std::endl
		<< std::endl;
	return oss.str();
}
