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

#pragma once

#include <src/global/global.h>
#include <src/machine/machine/machine.h>
#include <src/machine/elements/memoire/booleen.h>
#include <src/machine/elements/memoire/entier.h>
#include <src/machine/elements/memoire/chaine.h>
#include <src/machine/elements/valeur/exception.h>

namespace SVM_Machine = SVM::Machine;

namespace SVM
{
namespace Demarrage
{
namespace Lanceur
{
	DECL_SHARED_PTR(Lanceur);
	struct Lanceur;
	struct Erreur
	{
		friend struct Lanceur;
		Erreur()
		:_ligne(0) {}
		Erreur(const size_t ligne, const std::string& message)
		:_ligne(ligne), _message(message) {}
		Erreur(const std::string& source, const std::string& message)
		:_source(source), _ligne(0), _message(message) {}
		template<typename oStream>
		friend oStream& operator<<(oStream& os, const Erreur& e)
		{
			os << "Boot error:";
			if(not e._source.empty())
			{
				os << " In file " << e._source;
				if(e._ligne>0)
				{
					os << ", line " << e._ligne;
				}
				os << ":";
			}
			os << " " << e._message;
			return os;
		}
		Erreur& operator<<(const std::string& texte)
		{
			_message+=texte;
			return *this;
		}
		Erreur& operator<<(const long int& entier)
		{
			std::ostringstream oss;
			oss << entier;
			_message+=oss.str();
			return *this;
		}
		private:
			std::string _source;
			size_t _ligne;
			std::string _message;
	};

	enum class TypeOptions
	{
		DRAPEAU,
		CHAINE,
		ENTIER
	};
	
	struct Option
	{
		friend struct Lanceur;
		Option(const std::string& option, const bool multiple, const TypeOptions type, const std::string& alias, const SHARED_PTR(std::string)& aide)
		:_option(option), _multiple(multiple), _type(type), _alias(alias), _aide(aide), _compteur(0) { }
		bool reservee() const
		{
			return (_option=="h") or (_option=="v") or (_option=="d") or (_option=="p");
		}
		static std::string reservees()
		{
			return "hvd:p:";
		}
		bool argument() const
		{
			switch(_type)
			{
				case TypeOptions::CHAINE:
				case TypeOptions::ENTIER:
					return true;
				default:
					break;
			}
			return false;
		}
		void miseajour(const std::string& valeur)
		{
			switch(_type)
			{
				case TypeOptions::DRAPEAU:
					++_compteur;
					break;
				case TypeOptions::ENTIER:
					{
					char *fin;
					long int entier = ::strtol(valeur.c_str(),&fin,0);
					if((static_cast<size_t>(fin-valeur.c_str())!=valeur.size()))
					{
						throw Erreur() << "option -" << _option << " with invalid integer value " << valeur;
					}
					_entier.push_back(entier);
					}
					break;
				case TypeOptions::CHAINE:
					_chaine.push_back(valeur);
					break;
			}
		}
		void enregistre(SVM_Machine::Memoire& memoire) const
		{
			switch(_type)
			{
				case TypeOptions::DRAPEAU:
					if(_multiple)
					{	
						memoire.ajoute_unique(_alias,SVM_Memoire::Type::TypeInterne::ENTIER,std::make_shared<SVM_Memoire::Entier>(_compteur));
					}
					else
					{
						memoire.ajoute_unique(_alias,SVM_Memoire::Type::TypeInterne::BOOLEEN,std::make_shared<SVM_Memoire::Booleen>(_compteur>0));
					}
					break;
				case TypeOptions::ENTIER:
					if(_multiple)
					{
						std::vector<SVM_Memoire::ValeurSP> v;
						for(auto& e:_entier)
						{
							v.push_back(std::make_shared<SVM_Memoire::Entier>(e));
						}
						memoire.ajoute_multiple(_alias,SVM_Memoire::Type::TypeInterne::ENTIER,v);
					}
					else
					{
						SVM_Memoire::ValeurSP v;
						if(not _entier.empty())
						{
							v = std::make_shared<SVM_Memoire::Entier>(_entier.front());
						}
						memoire.ajoute_unique(_alias,SVM_Memoire::Type::TypeInterne::ENTIER,v);
					}
					break;
				case TypeOptions::CHAINE:
					if(_multiple)
					{
						std::vector<SVM_Memoire::ValeurSP> v;
						for(auto& c:_chaine)
						{
							v.push_back(std::make_shared<SVM_Memoire::Chaine>(c));
						}
						memoire.ajoute_multiple(_alias,SVM_Memoire::Type::TypeInterne::CHAINE,v);
					}
					else
					{
						SVM_Memoire::ValeurSP v;
						if(not _chaine.empty())
						{
							v = std::make_shared<SVM_Memoire::Chaine>(_chaine.front());
						}
						memoire.ajoute_unique(_alias,SVM_Memoire::Type::TypeInterne::CHAINE,v);
					}
					break;
			}
		}
		bool aide(std::ostream& os) const
		{
			os << "\t-" << _option;
			switch(_type)
			{
				case TypeOptions::CHAINE:
					os << " string ";
					break;
				case TypeOptions::ENTIER:
					os << " integer";
					break;
				default:
					os << "        ";
					break;
			}
			os << ": ";
			if(static_cast<bool>(_aide))
			{
				os << *(_aide);
			}
			else
			{
				os << _alias;
			}
			if(_multiple)
			{
				os << " (Repeatable option)";
			}
			os << std::endl;
			return _type == TypeOptions::ENTIER;
		}
		private:
			std::string _option;
			bool _multiple;
			TypeOptions _type;
			std::string _alias;
			SHARED_PTR(std::string) _aide;
			size_t _compteur;
			std::vector<long int> _entier;
			std::vector<std::string> _chaine;
	};

	struct Argument
	{
		friend struct Lanceur;
		Argument(const TypeOptions type, const std::string& alias, const SHARED_PTR(std::string)& aide)
		:_type(type), _alias(alias), _aide(aide) {}
		void miseajour(const std::string& valeur)
		{
			switch(_type)
			{
				case TypeOptions::ENTIER:
					{
					char *fin;
					long int entier = ::strtol(valeur.c_str(),&fin,0);
					if((static_cast<size_t>(fin-valeur.c_str())!=valeur.size()))
					{
						throw Erreur() << "argument " << _alias << " with invalid integer value " << valeur;
					}
					_entier = std::make_shared<long int>(entier);
					}
					break;
				case TypeOptions::CHAINE:
					_chaine = std::make_shared<std::string>(valeur);
					break;
				default:
					throw;
					break;

			}
		}
		void enregistre(SVM_Machine::Memoire& memoire) const
		{
			switch(_type)
			{
				case TypeOptions::ENTIER:
					{
						SVM_Memoire::ValeurSP v;
						if(static_cast<bool>(_entier))
						{
							v = std::make_shared<SVM_Memoire::Entier>(*_entier);
						}
						memoire.ajoute_unique(_alias,SVM_Memoire::Type::TypeInterne::ENTIER,v);
					}
					break;
				case TypeOptions::CHAINE:
					{
						SVM_Memoire::ValeurSP v;
						if(static_cast<bool>(_chaine))
						{
							v = std::make_shared<SVM_Memoire::Chaine>(*_chaine);
						}
						memoire.ajoute_unique(_alias,SVM_Memoire::Type::TypeInterne::CHAINE,v);
					}
					break;
				default:
					throw;
					break;
			}
		}
		static void enregistre(const std::string& alias, const std::vector<std::string>& arguments, SVM_Machine::Memoire& memoire)
		{
			std::vector<SVM_Memoire::ValeurSP> v;
			for(auto& c:arguments)
			{
				v.push_back(std::make_shared<SVM_Memoire::Chaine>(c));
			}
			memoire.ajoute_multiple(alias,SVM_Memoire::Type::TypeInterne::CHAINE,v);
		}
		bool aide(std::ostream& os) const
		{
			os << "\t";
			switch(_type)
			{
				case TypeOptions::CHAINE:
					os << "string ";
					break;
				case TypeOptions::ENTIER:
					os << "integer";
					break;
				default:
					break;
			}
			os << ": ";
			if(static_cast<bool>(_aide))
			{
				os << *(_aide);
			}
			else
			{
				os << _alias;
			}
			os << std::endl;
			return _type == TypeOptions::ENTIER;
		}
		private:
			TypeOptions _type;
			std::string _alias;
			SHARED_PTR(std::string) _aide;
			SHARED_PTR(long int) _entier;
			SHARED_PTR(std::string) _chaine;
	};

	DECL_SHARED_PTR(Selecteur);
	struct Selecteur
	{
		explicit Selecteur(const std::string& selecteur)
		:_selecteur(selecteur) {}
		virtual ~Selecteur() {}
		virtual void decoration(const std::set<std::string>& alias_possibles) = 0;
		virtual bool selectionne(const std::string& alias) const = 0;
		std::string _selecteur;
	};

	struct SelecteurAlias : public Selecteur
	{
		explicit SelecteurAlias(const std::string& selecteur)
		:Selecteur(selecteur) {}
		virtual ~SelecteurAlias() {}
		virtual void decoration(const std::set<std::string>& alias_possibles) override
		{
			if(alias_possibles.find(_selecteur)==alias_possibles.end())
			{
				throw Erreur() << "alias " << _selecteur << " does not match any option or argument for memory initialisation";
			}
		}
		virtual bool selectionne(const std::string& alias) const override
		{
			return alias==_selecteur;
		}

	};

	struct SelecteurGroupe : public Selecteur
	{
		explicit SelecteurGroupe(const std::string& selecteur)
		:Selecteur(selecteur) {}
		virtual ~SelecteurGroupe() {}
		virtual void decoration(const std::set<std::string>& alias_possibles) override
		{
			try
			{
				_expr = std::make_shared<std::regex>(_selecteur);
			}
			catch(std::regex_error& e)
			{
				throw Erreur() << "invalid regex " << _selecteur << " for memory initialisation";
			}
		}
		virtual bool selectionne(const std::string& alias) const override
		{
			std::smatch r;
			return std::regex_match(alias,r,*_expr);
		}
		std::shared_ptr<std::regex> _expr;
	};

	struct Processus
	{
		friend struct Lanceur;
		Processus(const std::string& nom, const SVM_Machine::CodeSP& code, const std::vector<SelecteurSP>& memoire, const SVM_Valeur::PointEntreeExtensionSP& ordonanceur, SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique)
		:_nom(nom), _code(code), _memoire(memoire), _ordonanceur(ordonanceur), _sequenceur(sequenceur), _terminaison_automatique(terminaison_automatique) {}
		private:
			std::string _nom;
			SVM_Machine::CodeSP _code;
			std::vector<SelecteurSP> _memoire;
			SVM_Valeur::PointEntreeExtensionSP _ordonanceur;
			SVM_Valeur::PointEntreeExtensionSP _sequenceur;
			bool _terminaison_automatique;
	};

	DECL_SHARED_PTR(Historique);
	struct Historique
	{
		explicit Historique(const bool calme)
		:_calme(calme) {}
		virtual ~Historique() {}
		virtual operator SVM_Machine::HistoriqueSP () const = 0;
		bool _calme;
	};

	struct HistoriqueTerminal : public Historique
	{
		explicit HistoriqueTerminal(const bool calme)
		:Historique(calme) {}
		virtual operator SVM_Machine::HistoriqueSP () const override
		{
			return std::make_shared<SVM_Machine::HistoriqueTerminal>(not _calme);
		}
	};

	struct HistoriqueFichier : public Historique
	{
		HistoriqueFichier(const std::string& fichier, const bool calme)
		:Historique(calme), _fichier(fichier) {}
		virtual ~HistoriqueFichier() {}
		virtual operator SVM_Machine::HistoriqueSP () const override
		{
			return std::make_shared<SVM_Machine::HistoriqueFichier>(_fichier,not _calme);
		}
		private:
			std::string _fichier;
	};

	struct HistoriqueServeur : public Historique
	{
		HistoriqueServeur(const std::string& ip, const std::string& port, const bool calme)
		:Historique(calme), _ip(ip), _port(port) {}
		virtual ~HistoriqueServeur() {}
		virtual operator SVM_Machine::HistoriqueSP () const override
		{
			return std::make_shared<SVM_Machine::HistoriqueServeur>(_ip,_port,not _calme);
		}
		private:
			std::string _ip;
			std::string _port;
	};

	DECL_SHARED_PTR(Debugueur);
	struct Debugueur
	{
		friend struct Lanceur;
		explicit Debugueur(const std::string& titre_defaut)
		:_titre(titre_defaut), _aip(false), _ip("localhost"), _astyle(false), _style(DEFAULT_STYLE), _aclients(false), _clients(100), _asecurite(false), _securite(0), _ahistorique(false), _historique(std::make_shared<SVM::Demarrage::Lanceur::HistoriqueTerminal>(false))
		{ }
		std::string _titre;
		bool _aip;
		std::string _ip;
		bool _astyle;
		std::string _style;
		bool _aclients;
		size_t _clients;
		bool _asecurite;
		size_t _securite;
		bool _ahistorique;
		HistoriqueSP _historique;
	};

	DECL_SHARED_PTR(OptionPerformance);
	struct OptionPerformance
	{
		friend struct Lanceur;
		OptionPerformance(const bool brut, const size_t details)
		:_brut(brut), _details(details)
		{ }
		bool _brut;
		size_t _details;
	};

	DECL_SHARED_PTR(Performance);
	struct Performance
	{
		friend struct Lanceur;
		Performance() = default;
		explicit Performance(const HistoriqueSP& historique)
		:_historique(historique) {}
		HistoriqueSP _historique;
	};

	DECL_SHARED_PTR(ParametreExtension);
	struct ParametreExtension
	{
		virtual ~ParametreExtension() {}
		virtual operator SVM::Machine::ParametreExtensionSP () const = 0;
	};

	struct ParametreExtensionEntier : public ParametreExtension
	{
		explicit ParametreExtensionEntier(const long int& entier)
		:_entier(entier) {}
		virtual ~ParametreExtensionEntier() {}
		virtual operator SVM::Machine::ParametreExtensionSP () const override
		{
			return std::make_shared<SVM::Machine::ParametreExtensionEntier>(_entier);
		}
		long int _entier;
	};

	struct ParametreExtensionChaine : public ParametreExtension
	{
		explicit ParametreExtensionChaine(const std::string& chaine)
		:_chaine(chaine) {}
		virtual ~ParametreExtensionChaine() {}
		virtual operator SVM::Machine::ParametreExtensionSP () const override
		{
			return std::make_shared<SVM::Machine::ParametreExtensionChaine>(_chaine);
		}
		std::string _chaine;
	};

	DECL_SHARED_PTR(ParametreExtensionOption);
	struct ParametreExtensionOption
	{
		explicit ParametreExtensionOption(const std::string& nom)
		:_nom(nom) {}
		ParametreExtensionOption(const std::string& nom, const ParametreExtensionSP& valeur)
		:_nom(nom), _valeur(valeur) {}
		operator SVM::Machine::ParametreExtensionOption () const
		{
			if(not _valeur)
				return SVM::Machine::ParametreExtensionOption(_nom);
			return SVM::Machine::ParametreExtensionOption(_nom,*_valeur);
		}
		std::string _nom;
		ParametreExtensionSP _valeur;
	};

	struct Extension
	{
		Extension(const std::string& nom, const std::vector<ParametreExtensionOption>& options, const std::vector<ParametreExtensionSP>& arguments)
		:_nom(nom), _options(options), _arguments(arguments) {}
		virtual operator SVM::Machine::DeclarationExtension () const
		{
			std::vector<SVM::Machine::ParametreExtensionOption> options(_options.begin(),_options.end());
			std::vector<SVM::Machine::ParametreExtensionSP> arguments;
			for(const auto& a:_arguments)
			{
				arguments.push_back(*a);
			}
			return SVM::Machine::DeclarationExtension(_nom,options,arguments);
		}
		std::string _nom;
		std::vector<ParametreExtensionOption> _options;
		std::vector<ParametreExtensionSP> _arguments;
	};

	struct Lanceur
	{
		enum class TypeExtension { LOCAL, SYSTEME };
		Lanceur(const std::string& source)
		:_source(source)
		{
			auto c = _source.find_last_of('/');
			if(c!=std::string::npos)
			{
				_local = _source.substr(0,c+1);
			}
		}

		void description(const std::string& description) { _description = std::make_shared<std::string>(description); }
		void debugueur(const DebugueurSP& debugueur) { _debugueur = debugueur; }
		void performance(const PerformanceSP& performance) { _performance = performance; }
		void historique(const HistoriqueSP& h) { _historique = h; }
		void extension(const std::string& extension, const TypeExtension type, const std::vector<ParametreExtensionOption>& options, const std::vector<ParametreExtensionSP>& arguments);
		void option(const Option& option) { _options.push_back(option); }
		void argument(const Argument& argument) { _arguments.push_back(argument); }
		void arguments_supplementaires(const std::string& arguments_supplementaires) { _arguments_supplementaires = std::make_shared<std::string>(arguments_supplementaires); }
		void processus(const Processus& processus) { _processus.push_back(processus); }
		void erreur(const Erreur& erreur) { _erreur = std::make_shared<Erreur>(erreur); _erreur->_source = _source; }
		const Erreur& erreur() const { return *_erreur; }
		void decoration();
		SVM_Machine::MachineSP machine_virtuelle(int argc, char *argv[]);
		void aide(std::ostream& os) const;
		void aide(std::ostream& os, const std::string& binaire) const;
		void version(std::ostream& os, const std::string& binaire) const;
		public:
			const std::string _source;
		private:
			std::string _local;
			SHARED_PTR(std::string) _description;
			DebugueurSP _debugueur;
			HistoriqueSP _historique;
			std::vector<std::string> _recherche;
			std::vector<Extension> _extensions;
			std::vector<Option> _options;
			std::vector<Argument> _arguments;
			SHARED_PTR(std::string) _arguments_supplementaires;
			std::vector<Processus> _processus;
			SHARED_PTR(Erreur) _erreur;
			SHARED_PTR(std::string) _port_debugueur;
			OptionPerformanceSP _option_performance;
			PerformanceSP _performance;
	};

	struct TousTerminesAutomatiquement : public SVM::Machine::Element::Valeur::ExceptionDemarrage
	{
		TousTerminesAutomatiquement()
		{
			DETAILS_EXCEPTION("All application processes are auto-terminated.");
		}
		virtual ~TousTerminesAutomatiquement()
		{
		}
	};
}
}
}
