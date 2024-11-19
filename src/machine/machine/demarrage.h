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
#include <src/machine/elements/memoire/memoire.h>
#include <src/machine/elements/valeur/point_entree_extension.h>
#include <src/machine/machine/historique.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Memoire = SVM::Machine::Element::Memoire;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Systeme
{
	struct Systeme;
}
}
namespace Extension
{
	DECL_SHARED_PTR(Gestionnaire);
}
namespace Debugueur
{
	DECL_SHARED_PTR(Debugueur);
}
namespace Performance
{
	DECL_SHARED_PTR(Performance);
}
	struct Machine;

	DECL_SHARED_PTR(Debug);
	struct Debug
	{
		friend struct SVM::Machine::Debugueur::Debugueur;
		Debug(const std::string& titre, const std::string& ip, const std::string& port, const std::string& style, const size_t clients, const size_t securite, const HistoriqueSP& historique)
		:_titre(titre), _ip(ip), _port(port), _style(style), _clients(clients), _securite(securite), _historique(historique)
		{ }
		private:
			std::string _titre;
			std::string _ip;
			std::string _port;
			std::string _style;
			size_t _clients;
			size_t _securite;
			HistoriqueSP _historique;
	};

	DECL_SHARED_PTR(Perf);
	struct Perf
	{
		friend struct SVM::Machine::Performance::Performance;
		Perf(const bool brut, const size_t details, const HistoriqueSP& historique)
		:_brut(brut), _details(details), _historique(historique) {}
		private:
			bool _brut;
			size_t _details;
			HistoriqueSP _historique;
	};

	DECL_SHARED_PTR(ParametreExtension);
	struct ParametreExtension
	{
		virtual ~ParametreExtension() {}
	};

	struct ParametreExtensionEntier : public ParametreExtension
	{
		explicit ParametreExtensionEntier(const long int& entier)
		:_entier(entier) {}
		virtual ~ParametreExtensionEntier() {}
		long int _entier;
	};

	struct ParametreExtensionChaine : public ParametreExtension
	{
		explicit ParametreExtensionChaine(const std::string& chaine)
		:_chaine(chaine) {}
		virtual ~ParametreExtensionChaine() {}
		std::string _chaine;
	};

	struct ParametreExtensionOption
	{
		explicit ParametreExtensionOption(const std::string& nom)
		:_nom(nom) {}
		ParametreExtensionOption(const std::string& nom, const ParametreExtensionSP& valeur)
		:_nom(nom), _valeur(valeur) {}
		std::string _nom;
		ParametreExtensionSP _valeur;
	};

	struct DeclarationExtension
	{
		DeclarationExtension(const std::string& nom, const std::vector<ParametreExtensionOption>& options, const std::vector<ParametreExtensionSP>& arguments)
		:_nom(nom), _options(options), _arguments(arguments) {}
		std::string _nom;
		std::vector<ParametreExtensionOption> _options;
		std::vector<ParametreExtensionSP> _arguments;
	};

	DECL_SHARED_PTR(Code);
	struct Code
	{
		virtual ~Code() {}
		void nom(const std::string& nom) { _nom = nom; }
		virtual SVM_Valeur::CodeSP analyse(const SVM::Machine::MachineSP& machine) const = 0;
		protected:
		Code(const std::string& source)
		:_source(source) {}
		std::string _nom;
		std::string _source;
	};

	struct Fichier : public Code
	{
		Fichier(const std::string& fichier)
		:Code(fichier) {}
		virtual SVM_Valeur::CodeSP analyse(const SVM::Machine::MachineSP& machine) const override;
		virtual ~Fichier() {}
	};

	struct Texte : public Code
	{
		Texte(const std::string& texte)
		:Code(texte) {}
		virtual ~Texte() {}
		virtual SVM_Valeur::CodeSP analyse(const SVM::Machine::MachineSP& machine) const override;
	};

	struct Memoire
	{
		typedef std::pair< SVM_Memoire::Type,std::vector<SVM_Memoire::ValeurSP> > DescriptionPointeur;
		void ajoute_unique(const std::string& alias, SVM_Memoire::Type::TypeInterne type, const SVM_Memoire::ValeurSP& valeur)
		{
			std::vector<SVM_Memoire::ValeurSP> p;
			p.push_back(valeur);
			_memoire.insert(std::make_pair(alias,std::make_pair(SVM_Memoire::Type(type),p)));
		}
		void ajoute_multiple(const std::string& alias, SVM_Memoire::Type::TypeInterne type, const std::vector<SVM_Memoire::ValeurSP>& valeurs)
		{
			std::vector<SVM_Memoire::ValeurSP> p;
			for(const auto& v: valeurs)
			{
				p.push_back(v);
			}
			_memoire.insert(std::make_pair(alias,std::make_pair(SVM_Memoire::Type(type),p)));
		}
		void initialise(SVM_Memoire::MemoireSP& memoire) const;
		private:
			std::map<std::string, Memoire::DescriptionPointeur > _memoire;
	};

	struct Processus
	{
		friend struct Machine;
		friend struct SVM::Machine::Element::Systeme::Systeme;
		Processus(const std::string& nom, const CodeSP& source, const Memoire& memoire, const SVM_Valeur::PointEntreeExtensionSP& ordonnanceur, const SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique)
		:_nom(nom),_source(source),_memoire(memoire),_ordonnanceur(ordonnanceur),_sequenceur(sequenceur),_terminaison_automatique(terminaison_automatique) {}
		void compile(const SVM::Machine::MachineSP& machine);
		void initialise(SVM_Memoire::MemoireSP& memoire) const { _memoire.initialise(memoire); }
		private:
			std::string _nom;
			CodeSP _source;
			SVM_Valeur::CodeSP _code;
			Memoire _memoire;
			SVM_Valeur::PointEntreeExtensionSP _ordonnanceur;
			SVM_Valeur::PointEntreeExtensionSP _sequenceur;
			bool _terminaison_automatique;
	};

	struct ProcessusCodeInvalide : public SVM_Valeur::ExceptionDemarrage
	{
		ProcessusCodeInvalide(const std::string& nom, const SVM_Valeur::Interruption& interruption)
		{
			std::ostringstream details_oss;
			interruption.erreur_demarrage(details_oss);
			DETAILS_EXCEPTION("Process " << nom << " has invalid code: " << details_oss.str());
		}
		virtual ~ProcessusCodeInvalide() {}
	};

	struct ProcessusObjetInvalide : public SVM_Valeur::ExceptionDemarrage
	{
		ProcessusObjetInvalide(const std::string& nom, const std::string& type_objet, const SVM_Valeur::PointEntreeExtensionSP& nom_objet)
		{
			DETAILS_EXCEPTION("Process " << nom << " references unknown " << type_objet << " " << *nom_objet);
		}
		virtual ~ProcessusObjetInvalide() {}
	};
}
}
