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

#include <src/demarrage/lanceur/lanceur.h>
#include <config.h>

using namespace SVM::Demarrage::Lanceur;

void Lanceur::decoration()
{
	std::set<std::string> alias_possibles;
	std::set<std::string> options_possibles;
	for(auto &o: _options)
	{
		if(o.reservee())
		{
			throw Erreur() << "option -" << o._option << " not allowed";
		}
		if(not options_possibles.insert(o._option).second)
		{
			throw Erreur()  << "ambiguous option definition -" << o._option;
		}
		if(not alias_possibles.insert(o._alias).second)
		{
			throw Erreur() << "ambiguous alias definition " << o._alias;
		}
	}
	for(auto &a: _arguments)
	{
		if(not alias_possibles.insert(a._alias).second)
		{
			throw Erreur() << "ambiguous alias definition" << a._alias;
		}
	}
	if(static_cast<bool>(_arguments_supplementaires))
	{
		if(not alias_possibles.insert(*_arguments_supplementaires).second)
		{
			throw Erreur() << "ambiguous alias definition " << *_arguments_supplementaires;
		}
	}
	for(auto& p: _processus)
	{
		for(const auto& s: p._memoire)
		{
			s->decoration(alias_possibles);
		}
	}
}

void Lanceur::extension(const std::string& extension, const TypeExtension type, const std::vector<ParametreExtensionOption>& options, const std::vector<ParametreExtensionSP>& arguments)
{
	std::string f;
	switch(type)
	{
		case TypeExtension::LOCAL:
			f = _local+extension;
			break;
		case TypeExtension::SYSTEME:
			f = extension;
			break;
	}
	_extensions.emplace_back(f,options,arguments);
}

SVM_Machine::MachineSP Lanceur::machine_virtuelle(int argc, char *argv[])
{
	std::string options_possibles;
	std::map<int,Option*> options;
	{
		std::ostringstream chaine_option;
		chaine_option << Option::reservees();
		for(auto& o: _options)
		{
			chaine_option << o._option << (o.argument()?":":"");
			options.insert(std::make_pair(o._option.c_str()[0],&o));
		}
		options_possibles = chaine_option.str();
	}

	::opterr = 0;
	int option_trouvee;
	for( ; ; )
	{
		option_trouvee = ::getopt(argc,argv,options_possibles.c_str());
		if(option_trouvee==EOF)
			break;
		switch(option_trouvee)
		{
			case '?':
				throw Erreur() << "invalid option -" << std::string(1,optopt);
				break;
			case 'h':
				aide(std::cerr);
				return SVM_Machine::MachineSP();
				break;
			case 'v':
				version(std::cerr, argv[0]);
				return SVM_Machine::MachineSP();
				break;
			case 'd':
				{
					if(static_cast<bool>(_port_debugueur))
					{
						throw Erreur() << "Debug port defined twice";
					}
					std::string valeur(optarg);
					_port_debugueur = std::make_shared<std::string>(valeur);
				}
				break;
			case 'p':
				{
					if(static_cast<bool>(_option_performance))
					{
						throw Erreur() << "Profiler details defined twice";
					}
					std::string valeur(optarg);
					bool brut = false;
					size_t details = 0;
					if(valeur=="raw")
					{
						brut = true;
					}
					else
					{
						char *fin;
						long int entier = ::strtol(valeur.c_str(),&fin,0);
						if((static_cast<size_t>(fin-valeur.c_str())!=valeur.size()))
						{
							throw Erreur() << "option -p with invalid integer value " << valeur;
						}
						if(entier<=0)
						{
							throw Erreur() << "option -p with invalid integer value " << valeur;
						}
						details = static_cast<size_t>(entier);
					}
					_option_performance = std::make_shared<OptionPerformance>(brut,details);
				}
				break;
			default:
				{
					Option*& option_selectionnee = options.find(option_trouvee)->second;
					option_selectionnee->miseajour(optarg?optarg:"");
				}
				break;
		}
	}
	++optind;
	for(auto& a:_arguments)
	{
		if(optind>=argc)
		{
			throw Erreur() << "not enough arguments";
		}
		a.miseajour(argv[optind]);
		++optind;
	}
	std::vector<std::string> arguments_supplementaires;
	if(static_cast<bool>(_arguments_supplementaires))
	{
		arguments_supplementaires.push_back(_source);
		for( ; optind<argc ; ++optind)
		{
			arguments_supplementaires.push_back(argv[optind]);
		}
	}
	// construction machine virtuelle
	SVM_Machine::HistoriqueSP historique = std::make_shared<SVM_Machine::Historique>();
	if(static_cast<bool>(_historique))
	{
		historique = *_historique;
	}
	bool tous_termines_automatiquement = true;
	std::vector<SVM_Machine::Processus> processus;
	for(auto& p: _processus)
	{
		SVM_Machine::Memoire memoire;
		if(not p._memoire.empty())
		{
			for(const auto& option: _options)
			{
				for(const auto& selecteur: p._memoire)
				{
					if(selecteur->selectionne(option._alias))
					{
						option.enregistre(memoire);
						break;
					}
				}
			}
			for(const auto& argument: _arguments)
			{
				for(const auto& selecteur: p._memoire)
				{
					if(selecteur->selectionne(argument._alias))
					{
						argument.enregistre(memoire);
						break;
					}
				}
			}
			if(static_cast<bool>(_arguments_supplementaires))
			{
				for(const auto& selecteur: p._memoire)
				{
					if(selecteur->selectionne(*_arguments_supplementaires))
					{
						Argument::enregistre(*_arguments_supplementaires,arguments_supplementaires,memoire);
						break;
					}
				}
			}
		}
		tous_termines_automatiquement &= p._terminaison_automatique;
		processus.push_back(SVM_Machine::Processus(p._nom,p._code,memoire,p._ordonanceur,p._sequenceur,p._terminaison_automatique));
	}
	if(tous_termines_automatiquement)
	{
		throw TousTerminesAutomatiquement();
	}
	SVM::Machine::DebugSP debugueur;
	if(static_cast<bool>(_port_debugueur))
	{
		SVM_Machine::HistoriqueSP historique_debug = *_debugueur->_historique;
		debugueur = std::make_shared<SVM::Machine::Debug>(_debugueur->_titre,_debugueur->_ip,*_port_debugueur,_debugueur->_style,_debugueur->_clients,_debugueur->_securite,historique_debug);
	}
	SVM::Machine::PerfSP performance;
	if(static_cast<bool>(_option_performance))
	{
		SVM_Machine::HistoriqueSP historique_performance;
		if(static_cast<bool>(_performance->_historique))
		{
			historique_performance = *_performance->_historique;
		}
		performance = std::make_shared<SVM::Machine::Perf>(_option_performance->_brut,_option_performance->_details,historique_performance);
	}
	std::vector<SVM::Machine::DeclarationExtension> extensions(_extensions.begin(), _extensions.end());
	return std::make_shared<SVM_Machine::Machine>(_description,historique,extensions,processus,debugueur,performance);
}

void Lanceur::aide(std::ostream& os, const std::string& binaire) const
{
	os	<< std::endl
		<< "This command launches the Simple Virtual Machine." << std::endl
		<< std::endl
		<< binaire << " [filename] [options] [arguments]" << std::endl
		<< "  Invoke the Simple Virtual Machine with an application described in the file provided as parameter." << std::endl
		<< "  Each application uses specific options and arguments. Please use the command '" << binaire << " [filename] -h' for more details." << std::endl
		<< std::endl
		<< binaire << " -h" << std::endl
		<< "  Display this help." << std::endl
		<< std::endl
		<< binaire << " -v" << std::endl
		<< "  Display the Simple Virtual Machine version and license." << std::endl
		<< std::endl;
}

void Lanceur::aide(std::ostream& os) const
{
	bool arguments = (not _arguments.empty()) or static_cast<bool>(_arguments_supplementaires);
	os << _source << " [options]";
	if(arguments)
	{
		os << " [arguments]";
	}
	os << std::endl
		<< std::endl;
	if(static_cast<bool>(_description))
	{
		os << (*_description) << std::endl;
	}
	os << "Options:" << std::endl;
	bool entiers = false;
	for(auto& o:_options)
	{
		entiers |= o.aide(os);
	}
	os	<< "\t-h        : Display this help" << std::endl
		<< "\t-v        : Display Simple Virtual Machine version and license" << std::endl
		<< "\t-d port   : Launch in debug mode on this port" << std::endl
		<< "\t-p detail : Launch in performance mode (profiling and code coverage) with this level of details" << std::endl
		<< "\t--        : Separator between options and arguments" << std::endl
		<< std::endl;
	
	if(arguments)
	{
		os << "Arguments:" << std::endl;
		for(auto& a:_arguments)
		{
			entiers |= a.aide(os);
		}
		if(static_cast<bool>(_arguments_supplementaires))
		{
			os << "\tlist of strings: Optional arguments" << std::endl;
		}
		os << std::endl;
	}
	if(entiers)
	{
		os	<< "Integer format: <sign><base><value>" << std::endl
			<< "\tSign: - for negative integers, + or void for positive integers" << std::endl
			<< "\tBase: 0 for octal, 0x for hexadecimal, void for decimal" << std::endl
			<< "\tValue composed of digits depending on base: 0 to 7 for octal, 0 to 9 for decimal, 0 to 9 and A to F (lower case also accepted) for hexadecimal" << std::endl 
			<< std::endl;

	}
}

void Lanceur::version(std::ostream& os, const std::string& binaire) const
{
	os	<< std::endl
		<< binaire << " version " << VERSION << "  Copyright (C) 2021  Julien BRUGUIER" << std::endl
		<< std::endl
		<< "This program comes with ABSOLUTELY NO WARRANTY." << std::endl
		<< "This is free software, and you are welcome to redistribute it under certain conditions." << std::endl
		<< "See GNU GPLv3 terms for details." << std::endl
		<< std::endl;
}
