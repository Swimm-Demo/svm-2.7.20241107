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
#include <src/machine/extensions/reference_objet.h>
#include <src/machine/extensions/automate_deterministe.h>
#include <src/machine/elements/memoire/valeur.h>
#include <src/machine/elements/memoire/booleen.h>
#include <src/machine/interface/svm.h>
namespace SVM
{
namespace Machine
{
namespace Extension
{
	DECL_SHARED_PTR(PatronObjetExtension);

	struct PatronObjetExtension
	{
		PatronObjetExtension(const SVM_Valeur::PointEntreeExtension& nom)
		:_nom(nom) {}
		virtual ~PatronObjetExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const PatronObjetExtension& p)
		{
			p.format(f);
			return f;
		}
		virtual void format(std::ostream& os) const = 0;
		virtual void nom(std::ostream& os) const
		{
			format(os);
		}
		virtual void interruption(std::set<SVM::Machine::Element::Valeur::Interruption>& i) const {};
		virtual std::string debugueur() const
		{
			std::ostringstream oss;
			format(oss);
			return oss.str();
		}
		SVM_Valeur::PointEntreeExtension _nom;
	};

	DECL_SHARED_PTR(PatronTypeExtension);

	struct PatronTypeExtension : public PatronObjetExtension
	{
		typedef void (*Destructeur)(const void *svm, const void *objet);
		typedef void* (*Copie)(const void *svm, const void *objet);
		typedef unsigned char (*Comparaison)(const void *svm, const void *gauche, const void *droite);
		typedef void* (*Constante)(const void *svm, SVM_String valeur);
		typedef SVM_String (*Affichage)(const void *svm, const void* objet);
		PatronTypeExtension(const SVM_Valeur::PointEntreeExtension& nom, Destructeur destructeur, Copie copie, Comparaison comparaison, Constante constante, Affichage affichage)
		:PatronObjetExtension(nom),_destructeur(destructeur), _copie(copie), _comparaison(comparaison), _constante(constante), _affichage(affichage) {}
		PatronTypeExtension(const SVM_Valeur::PointEntreeExtension& nom)
		:PatronTypeExtension(nom,nullptr,nullptr,nullptr,nullptr,nullptr) {}
		virtual ~PatronTypeExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronTypeExtension *p=dynamic_cast<PatronTypeExtension*>(patron.get());
			if(not p)
				return false;
			return _nom==p->_nom;
		}
		virtual void format(std::ostream& os) const override
		{
			os << "TYPE " << _nom;
		}
		Destructeur _destructeur;
		Copie _copie;
		Comparaison _comparaison;
		Constante _constante;
		Affichage _affichage;
	};

	DECL_SHARED_PTR(PatronStructureExtension);

	struct PatronStructureExtension : public PatronObjetExtension
	{
		typedef void (*Destructeur)(const void *svm, const void *objet);
		typedef void* (*Copie)(const void *svm, const void *objet);
		PatronStructureExtension(const SVM_Valeur::PointEntreeExtension& nom, Destructeur destructeur, Copie copie)
		:PatronObjetExtension(nom),_destructeur(destructeur), _copie(copie) {}
		PatronStructureExtension(const SVM_Valeur::PointEntreeExtension& nom)
		:PatronStructureExtension(nom,nullptr,nullptr) {}
		virtual ~PatronStructureExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronStructureExtension *p=dynamic_cast<PatronStructureExtension*>(patron.get());
			if(not p)
				return false;
			return _nom==p->_nom;
		}
		virtual void format(std::ostream& os) const override
		{
			os << "STRUCT " << _nom;
		}
		Destructeur _destructeur;
		Copie _copie;
	};

	DECL_SHARED_PTR(PatronInstructionExtension);

	struct PatronInstructionExtension : public PatronObjetExtension
	{
		typedef void* (*Instruction)(const void *svm, size_t argc, const void **argv);
		PatronInstructionExtension(const SVM_Valeur::PointEntreeExtension& nom, const bool systeme, const bool attente, const bool remplacement, Instruction instruction, const AutomateDeterministeSP& parametres, const AutomateDeterministeSP& retour)
		:PatronObjetExtension(nom), _systeme(systeme), _attente(attente), _remplacement(remplacement), _instruction(instruction), _parametres(parametres), _retour(retour) {}
		PatronInstructionExtension(const SVM_Valeur::PointEntreeExtension& nom, const bool systeme, const bool attente, const bool remplacement, const AutomateDeterministeSP& parametres, const AutomateDeterministeSP& retour)
		:PatronInstructionExtension(nom,systeme,attente,remplacement,nullptr,parametres,retour) {}
		virtual ~PatronInstructionExtension() {}
		virtual void format(std::ostream& os) const override
		{
			if(_systeme)
			{
				os << "SYSTEM ";
			}
			if(_attente)
			{
				os << "WAITING ";
			}
			if(_remplacement)
			{
				os << "OVERRIDE ";
			}
			os << "INSTRUCTION " << _nom << (*_parametres);
			if(_retour)
			{
				os << " -> " << (*_retour);
			}
		}
		virtual void nom(std::ostream& os) const override
		{
			os << "INSTRUCTION " << _nom;
		}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronInstructionExtension *p=dynamic_cast<PatronInstructionExtension*>(patron.get());
			if(not p)
				return false;
			if(_systeme xor p->_systeme)
				return false;
			if(_attente xor p->_attente)
				return false;
			if(_remplacement xor p->_remplacement)
				return false;
			if(_nom!=p->_nom)
				return false;
			if(not p->_parametres->inclus_dans(_parametres))
				return false;
			if(static_cast<bool>(_retour) and not p->_retour)
				return true;
			if(not _retour)
				return not p->_retour;
			return _retour->inclus_dans(p->_retour);
		}
		void verifie_parametres(const std::vector<RegexParametresElementCSP>& parametres) const;
		void verifie_retour(const RegexParametresElementCSP& retour, const bool nulle = false) const;
		bool _systeme;
		bool _attente;
		bool _remplacement;
		Instruction _instruction;
		AutomateDeterministeSP _parametres;
		AutomateDeterministeSP _retour;
	};
	
	struct ParametresIncorrects : public SVM_Valeur::ExceptionExecution
	{
		ParametresIncorrects(const std::vector<RegexParametresElementCSP>& parametres, const PatronObjetExtension& instruction, const size_t indice)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			std::ostringstream p;
			size_t i = 0;
			for(auto& pp:parametres)
			{
				++i;
				p << " ";
				if(i==indice)
				{
					p << "---> ";
				}
				p << *pp;
				if(i==indice)
				{
					p << " <---";
				}
			}
			DETAILS_EXCEPTION("Invalid parameters" << p.str() << " within call to " << instruction);
		}
		virtual ~ParametresIncorrects() {}
	};

	struct RetourIncorrect : public SVM_Valeur::ExceptionExecution
	{
		RetourIncorrect(const RegexParametresElementCSP& retour, const PatronObjetExtension& instruction)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Invalid return " << *retour << " within call to " << instruction);
		}
		virtual ~RetourIncorrect() {}
	};

	struct RetourNulleIncorrect : public SVM_Valeur::ExceptionExecution
	{
		RetourNulleIncorrect(const RegexParametresElementCSP& retour, const PatronObjetExtension& instruction)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Invalid return null value of " << *retour << " within call to " << instruction);
		}
		virtual ~RetourNulleIncorrect() {}
	};

	struct RetourAttendu : public SVM_Valeur::ExceptionExecution
	{
		RetourAttendu(const AutomateDeterministeSP& retour, const PatronObjetExtension& instruction)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Return " << *retour << " expected within call to " << instruction);
		}
		virtual ~RetourAttendu() {}
	};

	struct RetourNonAttendu : public SVM_Valeur::ExceptionExecution
	{
		RetourNonAttendu(const PatronObjetExtension& instruction)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Return not expected within call to " << instruction);
		}
		virtual ~RetourNonAttendu() {}
	};

	DECL_SHARED_PTR(PatronFonctionExtension);

	struct PatronFonctionExtension : public PatronObjetExtension
	{
		typedef const void* (*Fonction)(const void *svm, size_t argc, const void **argv);
		PatronFonctionExtension(const SVM_Valeur::PointEntreeExtension& nom, Fonction fonction, const AutomateDeterministeSP& parametres, const AutomateDeterministeSP& retour)
		:PatronObjetExtension(nom),_fonction(fonction), _parametres(parametres), _retour(retour) {}
		PatronFonctionExtension(const SVM_Valeur::PointEntreeExtension& nom, const AutomateDeterministeSP& parametres, const AutomateDeterministeSP& retour)
		:PatronFonctionExtension(nom,nullptr,parametres,retour) {}
		virtual ~PatronFonctionExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronFonctionExtension *p=dynamic_cast<PatronFonctionExtension*>(patron.get());
			if(not p)
				return false;
			if(_nom!=p->_nom)
				return false;
			if(not p->_parametres->inclus_dans(_parametres))
				return false;
			if(static_cast<bool>(_retour) and not p->_retour)
				return true;
			if(not _retour)
				return not p->_retour;
			return _retour->inclus_dans(p->_retour);
		}
		virtual void format(std::ostream& os) const override
		{
			os << "FUNCTION " << _nom << (*_parametres);
			if(_retour)
			{
				os << " -> " << (*_retour);
			}
		}
		virtual void nom(std::ostream& os) const override
		{
			os << "FUNCTION " << _nom;
		}
		void verifie_parametres(const std::vector<RegexParametresElementCSP>& parametres) const;
		void verifie_retour(const RegexParametresElementCSP& retour, const bool nulle = false) const;
		public:
		Fonction _fonction;
		AutomateDeterministeSP _parametres;
		AutomateDeterministeSP _retour;
	};

	DECL_SHARED_PTR(PatronInterruptionExtension);

	struct PatronInterruptionExtension : public PatronObjetExtension
	{
		PatronInterruptionExtension(const SVM_Valeur::PointEntreeExtension& nom)
		:PatronObjetExtension(nom) {}
		virtual ~PatronInterruptionExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronInterruptionExtension *p=dynamic_cast<PatronInterruptionExtension*>(patron.get());
			if(not p)
				return false;
			return _nom==p->_nom;
		}
		virtual void format(std::ostream& os) const override
		{
			os << "INTERRUPTION " << _nom ;
		}
		virtual void interruption(std::set<SVM::Machine::Element::Valeur::Interruption>& i) const override
		{
			i.insert(SVM::Machine::Element::Valeur::Interruption(_nom,""));
		}
	};

	DECL_SHARED_PTR(PatronOrdonnanceurExtension);

	struct PatronOrdonnanceurExtension : public PatronObjetExtension
	{
		typedef void* (*Creation)(const void *svm);
		typedef void (*Destruction)(const void *svm, const void *ordonnanceur);
		typedef SVM_Boolean (*Attache)(const void *svm, const void *ordonnanceur, const void *processus, const size_t parametre);
		typedef SVM_Boolean (*Detache)(const void *svm, const void *ordonnanceur, const void *processus, const size_t parametre);
		typedef size_t (*Ordonnance)(const void *svm, const void *ordonnanceur, const void *processus, const SVM_Process_State etat);
		typedef size_t (*Notification)(const void *svm, const void *ordonnanceur, const SVM_Notification_Type, const size_t parametre);
		typedef SVM_String (*Affiche)(const void *svm, const void *ordonnanceur);
		PatronOrdonnanceurExtension(const SVM_Valeur::PointEntreeExtension& nom, Creation creation, Destruction destruction, Attache attache, Detache detache, Ordonnance ordonnance, Notification notification, Affiche affiche)
		:PatronObjetExtension(nom), _creation(creation), _destruction(destruction), _attache(attache), _detache(detache), _ordonnance(ordonnance), _notification(notification), _affiche(affiche) {}
		PatronOrdonnanceurExtension(const SVM_Valeur::PointEntreeExtension& nom)
		:PatronOrdonnanceurExtension(nom,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr) {}
		virtual ~PatronOrdonnanceurExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronOrdonnanceurExtension *p=dynamic_cast<PatronOrdonnanceurExtension*>(patron.get());
			if(not p)
				return false;
			return _nom==p->_nom;
		}
		virtual void format(std::ostream& os) const override
		{
			os << "SCHEDULER " << _nom ;
		}
		public:
		Creation _creation;
		Destruction _destruction;
		Attache _attache;
		Detache _detache;
		Ordonnance _ordonnance;
		Notification _notification;
		Affiche _affiche;
	};

	DECL_SHARED_PTR(PatronSequenceurExtension);

	struct PatronSequenceurExtension : public PatronObjetExtension
	{
		typedef void* (*Creation)(const void *svm);
		typedef void (*Destruction)(const void *svm, const void *sequenceur);
		typedef const void* (*Courant)(const void *svm, const void *sequenceur);
		typedef SVM_Boolean (*Attache)(const void *svm, const void *sequenceur, const void *kernel, size_t argc, const void **argv);
		typedef SVM_Boolean (*Detache)(const void *svm, const void *sequenceur, const void *noyau, size_t argc, const void **argv);
		typedef SVM_String (*Affiche)(const void *svm, const void *sequenceur);
		PatronSequenceurExtension(const SVM_Valeur::PointEntreeExtension& nom, Creation creation, Destruction destruction, Courant courant, Attache attache, Detache detache, Affiche affiche)
		:PatronObjetExtension(nom), _creation(creation), _destruction(destruction), _courant(courant), _attache(attache), _detache(detache), _affiche(affiche) {}
		PatronSequenceurExtension(const SVM_Valeur::PointEntreeExtension& nom)
		:PatronSequenceurExtension(nom,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr) {}
		virtual ~PatronSequenceurExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronSequenceurExtension *p=dynamic_cast<PatronSequenceurExtension*>(patron.get());
			if(not p)
				return false;
			return _nom==p->_nom;
		}
		virtual void format(std::ostream& os) const override
		{
			os << "SEQUENCER " << _nom ;
		}
		Creation _creation;
		Destruction _destruction;
		Courant _courant;
		Attache _attache;
		Detache _detache;
		Affiche _affiche;
	};

	struct PatronFonctionGeneriqueExtension : public PatronObjetExtension
	{
		explicit PatronFonctionGeneriqueExtension(const std::string& extension, const std::string& fonction)
		:PatronObjetExtension(SVM_Valeur::PointEntreeExtension("<plugin>","<entry>")), _extension(extension), _fonction(fonction) {}
		virtual ~PatronFonctionGeneriqueExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronFonctionGeneriqueExtension *p=dynamic_cast<PatronFonctionGeneriqueExtension*>(patron.get());
			if(not p)
				return false;
			if(_extension!=p->_extension)
				return false;
			return _fonction==p->_fonction;
		}
		virtual void format(std::ostream& os) const override
		{
			os << "void " << _fonction << "(const void *svm) of plugin " << _extension;
		}

		std::string _extension;
		std::string _fonction;
	};

	DECL_SHARED_PTR(PatronOptionExtension);

	struct PatronOptionExtension : public PatronObjetExtension
	{
		PatronOptionExtension(const SVM_Valeur::PointEntreeExtension& nom, const std::string& clef, SVM_Memoire::Type::TypeInterne type)
		:PatronObjetExtension(nom), _clef(clef), _type(type)
		{
			switch(_type)
			{
				case SVM_Memoire::Type::TypeInterne::BOOLEEN:
					_valeur = std::make_shared<SVM_Memoire::Booleen>(false);
					break;
				default:
					_valeur = std::make_shared<SVM_Memoire::ValeurNulle>(_type);
					break;
			}
			_valeur->constante();
		}
		virtual ~PatronOptionExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronOptionExtension *p=dynamic_cast<PatronOptionExtension*>(patron.get());
			if(not p)
				return false;
			return (_nom==p->_nom) and (_clef==p->_clef) and (_type==p->_type);
		}
		virtual void format(std::ostream& os) const override
		{
			os << "OPTION " << _nom << " -" << _clef << " " << SVM_Memoire::Type(_type);
		}
		virtual std::string debugueur() const
		{
			std::ostringstream oss;
			format(oss);
			oss << " (" << _valeur->serialise() << ")";
			return oss.str();
		}
		std::string _clef;
		SVM_Memoire::Type::TypeInterne _type;
		SVM_Memoire::ValeurSP _valeur;
	};

	DECL_SHARED_PTR(PatronArgumentExtension);

	struct PatronArgumentExtension : public PatronObjetExtension
	{
		PatronArgumentExtension(const SVM_Valeur::PointEntreeExtension& nom, const size_t indice, SVM_Memoire::Type::TypeInterne type)
		:PatronObjetExtension(nom), _indice(indice), _type(type)
		{
			_valeur = std::make_shared<SVM_Memoire::ValeurNulle>(_type);
			_valeur->constante();
		}
		virtual ~PatronArgumentExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronArgumentExtension *p=dynamic_cast<PatronArgumentExtension*>(patron.get());
			if(not p)
				return false;
			return (_nom==p->_nom) and (_type==p->_type);
		}
		virtual void format(std::ostream& os) const override
		{
			os << "ARGUMENT " << _nom << " " << SVM_Memoire::Type(_type);
		}
		virtual std::string debugueur() const
		{
			std::ostringstream oss;
			format(oss);
			oss << " (@" << _indice << " " << _valeur->serialise() << ")";
			return oss.str();
		}
		size_t _indice;
		SVM_Memoire::Type::TypeInterne _type;
		SVM_Memoire::ValeurSP _valeur;
	};

	DECL_SHARED_PTR(PatronArgumentsExtension);

	struct PatronArgumentsExtension : public PatronObjetExtension
	{
		PatronArgumentsExtension(const SVM_Valeur::PointEntreeExtension& nom)
		:PatronObjetExtension(nom) {}
		virtual ~PatronArgumentsExtension() {}
		virtual bool compatible(const PatronObjetExtensionSP& patron) const override
		{
			PatronArgumentsExtension *p=dynamic_cast<PatronArgumentsExtension*>(patron.get());
			if(not p)
				return false;
			return (_nom==p->_nom);
		}
		virtual void format(std::ostream& os) const override
		{
			os << "ARGUMENTS " << _nom;
		}
		virtual std::string debugueur() const
		{
			std::ostringstream oss;
			format(oss);
			oss << " ([";
			for(const auto &v:_valeurs)
			{
				oss << " " << v->serialise();
			}
			oss << " ])";
			return oss.str();
		}
		std::vector<SVM_Memoire::ValeurSP> _valeurs;
	};
}
}
}
