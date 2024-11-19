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

#include <src/global/global.h>
#include <src/outils/outils.h>
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/extensions/analyseur/analyseur.h>
#include <src/outils/analyseur/analyseur.h>

void aide(std::ostream& os, const std::string& binaire)
{
	os	<< binaire << " [options]" << std::endl
		<< std::endl
		<< "This command produces a shell script generating a SVM plugin source directory." << std::endl 
		<< "The generated script can be used in plugin creation automation." << std::endl
		<< std::endl
		<< "This generator customises the SVM plugin source from a SVM plugin configuration or description string." << std::endl
		<< "The generated code is an adapted canevas and can be produced in:" << std::endl
		<< "  - C," << std::endl
		<< "  - C++14." << std::endl
		<< "The generated plugin provides:" << std::endl
		<< "  - a simple Makefile for local build and local testing," << std::endl
		<< "  - a full Autotools suite for wide build and distribution." << std::endl
		<< std::endl
		<< "Options:" << std::endl
		<< "\t-i <name>: input file name (stdin by default)" << std::endl
		<< "\t-o <name>: output file name (stdout by default, can be piped to bash)" << std::endl
		<< "\t-l       : Autotools license (no license by default)" << std::endl
		<< "\t-p       : save generated files before patch and generate patches update script (nothing by default)" << std::endl
		<< "\t-d <name>: plugin directory name (svm_plugin_<plugin name> by default)" << std::endl
		<< "\t-f       : force generation" << std::endl
		<< "\t-q       : quiet mode" << std::endl
		<< "\t-h       : display this help" << std::endl
		<< "\t-v       : display svm_plugin version and license" << std::endl
		<< std::endl;
}

void version(std::ostream& os, const std::string& binaire)
{
	os	<< binaire << " version " << VERSION << "  Copyright (C) 2021  Julien BRUGUIER" << std::endl
		<< std::endl
		<< "This program comes with ABSOLUTELY NO WARRANTY." << std::endl
		<< "This is free software, and you are welcome to redistribute it under certain conditions." << std::endl
		<< "See GNU GPLv3 terms for details." << std::endl
		<< std::endl;
}

int main(int argc, char *argv[])
{
	bool licence = true;
	bool verbeux = true;
	SVM::Outils::Langage langage;
	bool force = false;
	bool patch = false;
	std::string repertoire;
	std::string entree;
	std::string sortie;
	::opterr = 0;
	int option_trouvee;
	std::string shell;
	for( ; ; )
	{
		option_trouvee = ::getopt(argc,argv,"cd:i:o:qfplhv");
		if(option_trouvee==EOF)
			break;
		switch(option_trouvee)
		{
			case 'h':
				std::cerr << std::endl;
				::aide(std::cerr,argv[0]);
				return 0;
				break;
			case 'v':
				std::cerr << std::endl;
				::version(std::cerr,argv[0]);
				return 0;
				break;
			case 'd':
				repertoire = std::string(optarg);
				break;
			case 'i':
				entree = std::string(optarg);
				break;
			case 'o':
				sortie = std::string(optarg);
				break;
			case 'l':
				licence = false;
				break;
			case 'p':
				patch = true;
				break;
			case 'q':
				verbeux = false;
				break;
			case 'f':
				force = true;
				break;
			case '?':
			default:
				std::cerr << "invalid option -" << std::string(1,optopt) << std::endl << std::endl;
				::aide(std::cerr,argv[0]);
				return 2;
				break;
		}
	}

	if(verbeux)
		std::cerr	<< "Plugin generator for Simple Virtual Machine " << VERSION << std::endl
				<< "Use -h option for help" << std::endl;
	std::string configuration;
	if(entree.empty())
	{
		if(verbeux)
			std::cerr << "Reading plugin definition from stdin...";
		for(std::string ligne ; std::getline(std::cin, ligne) ; )
		{
			configuration += ligne;
			configuration += '\n';
		}
		entree = "<stdin>";
	}
	else
	{
		if(verbeux)
			std::cerr << "Reading plugin definition from file " << entree << "...";
		try
		{
			std::ifstream e(entree);
			if(not e.good())
			{
				throw std::ifstream::failure("Invalid input file.");
			}
			for(std::string ligne ; std::getline(e, ligne) ; )
			{
				configuration += ligne;
				configuration += '\n';
			}
		}
		catch(std::ifstream::failure& e)
		{
			if(verbeux)
				std::cerr << " ERROR" << std::endl;
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	if(verbeux)
		std::cerr << " OK" << std::endl;
	try
	{
		if(verbeux)
			std::cerr << "Parsing plugin definition..."; 
		SVM::Machine::Extension::DescriptionSP description;
		SVM::Outils::ExtensionSP extension;
		extension = SVM::Outils::Analyseur::analyse_chaine(configuration);
		if(verbeux)
			std::cerr << " OK" << std::endl << "Generating and parsing plugin configuration string...";
		extension->verifie_et_complete(entree,description,configuration,langage,licence,shell);
		if(verbeux)
			std::cerr << " OK" << std::endl;
		if(repertoire.empty())
		{
			repertoire = "svmplugin";
			repertoire += description->_nom;
		}
		if(verbeux)
			std::cerr << "Generating plugin files...";
		std::ostringstream source;
		source << "src/plugin." << langage.extension();
		std::ostringstream documentation;
		documentation << "doc/svm_plugin_" << description->_nom << ".man";
		SVM::Outils::Extension::Corrections corrections;
		SVM::Outils::Fichiers::Liste fichiers = {
			{ source.str() , SVM::Outils::Fichier("source",SVM::Outils::Generation::source(langage,configuration,extension)) },
			{ "src/plugin.h" , SVM::Outils::Fichier("source",SVM::Outils::Generation::inclusion(langage,extension)) },
			{ documentation.str() , SVM::Outils::Fichier(SVM::Outils::Generation::documentation(extension)) },
			{ "Makefile.local" , SVM::Outils::Fichier("make",SVM::Outils::Generation::makefile(langage,extension)) },
			{ "Makefile.inc" , SVM::Outils::Fichier("make",SVM::Outils::Generation::makefile_inc(langage,extension)) },
			{ "src/Makefile.am" , SVM::Outils::Fichier("make",SVM::Outils::Generation::makefile_am_src(langage,extension)) },
			{ "doc/Makefile.am" , SVM::Outils::Fichier("make",SVM::Outils::Generation::makefile_am_doc(langage,extension)) },
			{ "test/Makefile.am" , SVM::Outils::Fichier("make",SVM::Outils::Generation::makefile_am_test(langage,extension)) },
			{ "Makefile.am" , SVM::Outils::Fichier("make",SVM::Outils::Generation::makefile_am(langage,extension)) },
			{ "configure.ac" , SVM::Outils::Fichier("conf",SVM::Outils::Generation::configure_ac(langage,extension)) },
			{ "NEWS", SVM::Outils::Fichier(SVM::Outils::Generation::nouvelles(extension)) },
			{ "README", SVM::Outils::Fichier(SVM::Outils::Generation::lisez_moi(extension)) },
			{ "AUTHORS", SVM::Outils::Fichier(SVM::Outils::Generation::auteur(extension)) },
			{ "ChangeLog", SVM::Outils::Fichier(SVM::Outils::Generation::historique(extension)) },
			{ "plugin_configuration_string", SVM::Outils::Fichier(extension->_description->chaine_configuration()) },
			{ "plugin_install", SVM::Outils::Fichier("shell","/bin/sh",SVM::Outils::Generation::installation(extension->_nom)) }
		};
		auto initialisation_test = extension->directive_facultative("initialisation_test");
		if(not initialisation_test.empty())
		{
			std::ostringstream fi;
			fi << initialisation_test << std::endl;
			fichiers.insert(std::make_pair("test/init.sh",SVM::Outils::Fichier("shell","/bin/sh",fi.str())));
		}
		auto finalisation_test = extension->directive_facultative("finalisation_test");
		if(not finalisation_test.empty())
		{
			std::ostringstream ff;
			ff << finalisation_test << std::endl;
			fichiers.insert(std::make_pair("test/fini.sh",SVM::Outils::Fichier("shell","/bin/sh",ff.str())));
		}
		for(const auto& t:extension->_tests)
		{
			std::ostringstream oss;
			oss << "test/" << t.first << ".svm";
			fichiers.insert(std::make_pair(oss.str(),SVM::Outils::Generation::test_source(t.second,extension)));
			std::ostringstream oss2;
			oss2 << "test/" << t.first << ".sh";
			fichiers.insert(std::make_pair(oss2.str(),SVM::Outils::Generation::test_script(t.first,t.second,not initialisation_test.empty(),not finalisation_test.empty(),extension)));
		}
		for(const auto& f:extension->_fichiers)
		{
			if(not fichiers.insert(f).second)
			{
				std::ostringstream oss;
				oss << "Trying to replace file " << f.first;
				throw SVM::Outils::Erreur(oss.str());
			}
		}
		if(patch)
		{
			fichiers.insert(std::make_pair("update_patches",SVM::Outils::Fichier("shell","/bin/sh",SVM::Outils::Generation::update_patches())));
		}
		std::string licence_courte = extension->directive_obligatoire("licence_courte");
		for(auto& f: fichiers)
		{
			if(not f.second._type.empty())
			{
				auto it=extension->_commentaires.find(f.second._type);
				if(it==extension->_commentaires.end())
				{
					std::ostringstream oss;
					oss << "Invalid file type " << f.second._type;
					throw SVM::Outils::Erreur(oss.str());
				}
				f.second.ajoute_licence(licence_courte,it->second);
			}
		}
		fichiers.find(source.str())->second.lignes_source(source.str());
		corrections = extension->_corrections;
		if(licence)
		{
			std::ostringstream oss;
			oss << *extension->directive("licence_longue") << std::endl;
			fichiers.insert(std::make_pair("COPYING", SVM::Outils::Fichier(oss.str())));
		}
		if(verbeux)
			std::cerr << " OK" << std::endl;
		for(auto& f: fichiers)
		{
			f.second.echappe_shell();
		}
		std::string script = SVM::Outils::Fichiers::script(langage,licence,force,patch,shell,description->_nom,repertoire,fichiers,corrections);
		if(sortie.empty())
		{
			if(verbeux)
				std::cerr << "Generating plugin creation shell script to stdout...";
			std::cout << script;
		}
		else
		{
			if(verbeux)
				std::cerr << "Generating plugin creation shell script to file " << sortie << "...";
			try
			{
				std::ofstream s(sortie);
				if(not s.good())
				{
					throw std::ifstream::failure("Invalid output file.");
				}
				s << script;
			}
			catch(std::ifstream::failure& e)
			{
				if(verbeux)
					std::cerr << " ERROR" << std::endl;
				std::cerr << e.what() << std::endl;
				return 1;
			}
			struct stat etat;
			if(::stat(sortie.c_str(),&etat))
			{
				if(verbeux)
					std::cerr << " ERROR" << std::endl;
				std::cerr << ::strerror(errno) << std::endl;
				return 1;
			}
			if(::chmod(sortie.c_str(),etat.st_mode | S_IXUSR))
			{
				if(verbeux)
					std::cerr << " ERROR" << std::endl;
				std::cerr << ::strerror(errno) << std::endl;
				return 1;
			}
		}
		if(verbeux)
			std::cerr << " OK" << std::endl;
	}
	catch(SVM::Outils::Erreur& e)
	{
		if(verbeux)
			std::cerr << " ERROR" << std::endl;
		std::cerr << e << std::endl;
		return 1;
	}
	catch(SVM::Machine::Element::Valeur::ExceptionDemarrage& e)
	{
		if(verbeux)
			std::cerr << " ERROR" << std::endl;
		std::cerr << e << std::endl;
		return 1;
	}
	return 0;
}
