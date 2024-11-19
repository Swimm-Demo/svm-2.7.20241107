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
#include <src/global/installation.h>
#include <src/machine/interface/svm_compatibility.h>

struct ElementDatabase
{
	ElementDatabase() = default;
	ElementDatabase(const std::string& s)
	{
		if(s.empty()) return;
		if(s[0]=='(')
		{
			auto f = s.find(')');
			if(f==std::string::npos) throw 1;
			_nom = s.substr(1,f-1);
			_valeur = s.substr(f+1);
		}
		else
		{
			_nom = s;
			_valeur = s;
		}
	}
	std::string _nom;
	std::string _valeur;
	operator const std::string& () const { return _valeur; }
	template<typename oStream>
	friend oStream& operator<<(oStream& os, const ElementDatabase& e)
	{
		os << e._valeur;
		return os;
	}
	bool operator<(const ElementDatabase& e) const { return _nom<e._nom; }
};

struct Modification
{
	static Modification ajout(const ElementDatabase& a)
	{
		return { Operation::AJOUT , a , ElementDatabase() };
	}
	static Modification enleve(const ElementDatabase& e)
	{
		return { Operation::ENLEVE , e , ElementDatabase() };
	}
	static Modification change(const ElementDatabase& avant, const ElementDatabase& apres)
	{
		return { Operation::CHANGE , avant , apres };
	}
	enum class Operation { AJOUT, ENLEVE, CHANGE };
	Operation _operation;
	ElementDatabase _primaire;
	ElementDatabase _secondaire;
	bool operator<(const Modification& m) const
	{
		if(_primaire<m._primaire) return true;
		if(m._primaire<_primaire) return false;
		if(_operation<m._operation) return true;
		if(m._operation<_operation) return false;
		if(_secondaire<m._secondaire) return true;
		if(m._secondaire<_secondaire) return false;
		return false;
	}
	template<typename oStream>
	friend oStream& operator<<(oStream& os, const Modification& m)
	{
		switch(m._operation)
		{
			case Operation::AJOUT:
			{
				os << "Added  : " << m._primaire << std::endl;
			}
			break;
			case Operation::ENLEVE:
			{
				os << "Removed: " << m._primaire << std::endl;
			}
			break;
			case Operation::CHANGE:
			{
				os << "Changed: " << std::endl
				   << "         From: " << m._primaire << std::endl
				   << "         To  : " << m._secondaire << std::endl;
			}
			break;
		}
		return os;
	}
	private:
		Modification() = default;
};

typedef std::map<Modification,size_t> Changements;

void ajout_changement(Changements& changements, const Modification& m)
{
	auto it = changements.insert(std::make_pair(m,1));
	if(not it.second)
	{
		++(it.first->second);
	}
}

std::map<std::string,std::string> tokens(const std::string& chaine, const char separateur, const char sep)
{
	std::map<std::string,std::string> elements;
	std::stringstream ss;
	ss.str(chaine);
	for(std::string item ; std::getline(ss, item, separateur) ; )
	{
		if(item.empty()) continue;
		auto s = item.find(sep);
		if(s==std::string::npos) throw 1;
		elements.insert(std::make_pair(item.substr(0,s),item.substr(s+1)));
	}
	return elements;
}

ElementDatabase valeur(const std::map<std::string,ElementDatabase>& database, const std::string& clef)
{
	auto it = database.find(clef);
	if(it==database.end()) return ElementDatabase("<unknown>");
	return it->second;
}

void affiche(const size_t mesure, const std::string& texte)
{
	std::string p = ((mesure==1)?"":"s");
	std::cout << "  " << mesure << " C/C++ type" << p << " or function" << p << " " << texte << "." << std::endl;
}

void aide(std::ostream& os, const std::string& binaire)
{
	os	<< binaire << " [options]" << std::endl
		<< std::endl
		<< "This command produces elements of the SVM installation." << std::endl 
		<< std::endl
		<< "Options:" << std::endl
		<< "\t-c <name>: display value associated to one configuration element of this SVM" << std::endl
		<< "\t-l       : display all configuration elements of this SVM" << std::endl
		<< "\t-s       : display the API signature of this SVM" << std::endl
		<< "\t-B       : check the API signature (given on stdin) backward compatibility against API signature of this SVM" << std::endl
		<< "\t           (Returns 0 when compatible, 1 when incompatible, 2 when API signature is invalid)" << std::endl
		<< "\t-F       : check the API signature (given on stdin) forward compatibility against API signature of this SVM" << std::endl
		<< "\t           (Returns 0 when compatible, 1 when incompatible, 2 when API signature is invalid)" << std::endl
		<< "\t-h       : display this help" << std::endl
		<< "\t-v       : display svm_config version and license" << std::endl
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
	::opterr = 0;
	int option_trouvee;
	for( ; ; )
	{
		option_trouvee = ::getopt(argc,argv,"lc:sBFhv");
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
			case 'c':
				{
					try
					{
						std::cout << SVM::Global::Installation::valeur(optarg) << std::endl;
					}
					catch(SVM::Global::ClefInvalide& e)
					{
						std::cerr << "Invalid key " << optarg << "." << std::endl;
						return 1;
					}
					return 0;
				}
				break;
			case 'l':
				{
					size_t t = 0;
					for(const auto& c : SVM::Global::Installation::clefs())
					{
						auto ct = c.size();
						if(ct>t)
						{
							t=ct;
						}
					}
					for(const auto& c : SVM::Global::Installation::variables())
					{
						std::cout << c.first << std::string(t-c.first.size(),' ') << ": " << c.second << std::endl;
					}
					return 0;
				}
				break;
			case 's':
				{
					std::cout << SVM_API_SIGNATURE << "@@@" << SVM_API_DATABASE << "@@@" << SVM_VERSION << std::endl;
					return 0;
				}
				break;
			case 'B':
			case 'F':
				{
					try
					{
						std::string signature_api_machine(SVM_API_SIGNATURE);
						std::string database_api_machine(SVM_API_DATABASE);
						std::string version_api_machine(SVM_VERSION);
						std::string api_parametre;
						std::getline(std::cin,api_parametre);
						std::string signature_api_parametre;
						std::string database_api_parametre;
						size_t dbidx = api_parametre.find("@@@");
						if(dbidx==std::string::npos)
						{
							signature_api_parametre = api_parametre;
						}
						else
						{
							signature_api_parametre = api_parametre.substr(0,dbidx);
							api_parametre = database_api_parametre = api_parametre.substr(dbidx+3);
						}
						std::string version_api_parametre = "<unknown>";
						size_t vidx = api_parametre.find("@@@");
						if(vidx!=std::string::npos)
						{
							database_api_parametre = database_api_parametre.substr(0,vidx);
							version_api_parametre = api_parametre.substr(vidx+3);
						}
						auto signature_api_machine_set = tokens(signature_api_machine,'_',':');
						auto signature_api_parametre_set = tokens(signature_api_parametre,'_',':');
						auto db = tokens(database_api_machine,':','_');
						std::map<std::string,ElementDatabase> database_api_machine_set = { db.begin(),db.end() };
						decltype(database_api_machine_set) database_api_parametre_set;
						if(not database_api_parametre.empty())
						{
							db = tokens(database_api_parametre,':','_');
							database_api_parametre_set = { db.begin(),db.end() };
						}
						if(option_trouvee=='B')
						{
							std::swap(signature_api_parametre_set,signature_api_machine_set);
							std::swap(database_api_parametre_set,database_api_machine_set);
							std::swap(version_api_parametre,version_api_machine);
						}
						auto itm = signature_api_machine_set.begin();
						auto itp = signature_api_parametre_set.begin();
						size_t supprimes = 0;
						size_t changes = 0;
						size_t ajoutes = 0;
						size_t im = 0;
						size_t ip = 0;
						Changements changements;
						for( ; ; )
						{
							if(itm==signature_api_machine_set.end())
							{
								ajoutes += signature_api_parametre_set.size()-ip;
								for( ; itp!=signature_api_parametre_set.end() ; ++itp)
								{
									ajout_changement(changements,Modification::ajout(valeur(database_api_parametre_set,itp->first)));
								}
								break;
							}
							if(itp==signature_api_parametre_set.end())
							{
								supprimes += signature_api_machine_set.size()-im;
								for( ; itm!=signature_api_machine_set.end() ; ++itm)
								{
									ajout_changement(changements,Modification::enleve(valeur(database_api_machine_set,itm->first)));
								}
								break;
							}
							if(itm->first==itp->first)
							{
								if(itm->second!=itp->second)
								{
									ajout_changement(changements,Modification::change(valeur(database_api_machine_set,itm->first),valeur(database_api_parametre_set,itp->first)));
									++changes;
								}
								++itm; ++im;
								++itp; ++ip;
								continue;
							}
							if(itm->first<itp->first)
							{
								ajout_changement(changements,Modification::enleve(valeur(database_api_machine_set,itm->first)));
								++supprimes;
								++itm; ++im;
							}
							else
							{
								ajout_changement(changements,Modification::ajout(valeur(database_api_parametre_set,itp->first)));
								++ajoutes;
								++itp; ++ip;
							}
						}
						if(not changements.empty())
						{
							std::cout << "##### Details #####" << std::endl;
							for(const auto& c: changements)
							{
								std::cout << c.first;
								if(c.second>1)
								{
									std::cout << "         Found: " << c.second << " times." << std::endl;
								}
							}
							std::cout << std::endl;
						}
						std::cout << "##### Summary #####" << std::endl;
						affiche(supprimes,"removed");
						affiche(changes,"changed");
						affiche(ajoutes,"added");
						std::cout << std::endl;
						bool compatible = (supprimes==0) and (changes==0);

						std::cout << "Compatibility: " << (compatible?"YES":"NO") << std::endl
							<< std::endl
 							<< "  From old SVM API version " << version_api_machine << std::endl
							<< "  To   new SVM API version " << version_api_parametre << std::endl;
						return compatible?0:1;
					}
					catch(...)
					{
						std::cerr << "Invalid SVM API signature or database!" << std::endl;
						return 2;
					}
					return 0;
				}
				break;
			case '?':
			default:
				std::cerr << "invalid option -" << std::string(1,optopt) << std::endl << std::endl;
				::aide(std::cerr,argv[0]);
				return 2;
				break;
		}
	}
	std::cerr << std::endl;
	::aide(std::cerr,argv[0]);
	return 1;
}
