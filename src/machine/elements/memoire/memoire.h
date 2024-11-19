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
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/elements/memoire/element.h>
#include <src/machine/elements/memoire/descriptionbloc.h>
#include <src/machine/elements/valeur/pointeur.h>
#include <src/machine/elements/noyau/controle_acces.h>
#include <src/machine/debugueur/piege.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Noyau = SVM::Machine::Element::Noyau;

namespace SVM
{
namespace Machine
{
namespace Debugueur
{
	DECL_SHARED_PTR(Debugueur);
}
namespace Element
{
namespace Noyau
{
	DECL_WEAK_PTR(Noyau);
}
namespace Processeur
{
	struct Processeur;
}
namespace Memoire
{
	DECL_SHARED_PTR(Memoire);
	DECL_WEAK_PTR(Memoire);
	
	struct AliasDejaDefini : public SVM_Valeur::ExceptionExecution
	{
		AliasDejaDefini(const std::string& alias)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Alias " << alias << " already linked to a pointer.");
		}
		virtual ~AliasDejaDefini() {}
	};

	struct AliasInexistant : public SVM_Valeur::ExceptionExecution
	{
		AliasInexistant(const std::string& alias)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Alias " << alias << " is not linked to a pointer.");
		}
		virtual ~AliasInexistant() {}
	};

	struct LectureAdresseNonDefinie : public SVM_Valeur::ExceptionExecution
	{
		LectureAdresseNonDefinie(const SVM_Valeur::AdresseMemoire& adresse)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Read to non defined address " << adresse << ".");
		}
		virtual ~LectureAdresseNonDefinie() {}
	};

	struct LectureAdresseNonInitialisee : public SVM_Valeur::ExceptionExecution
	{
		LectureAdresseNonInitialisee(const SVM_Valeur::AdresseMemoire& adresse)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Read to non initialised address " << adresse << ".");
		}
		virtual ~LectureAdresseNonInitialisee() {}
	};

	struct LectureAdresseTypeIncompatible : public SVM_Valeur::ExceptionExecution
	{
		LectureAdresseTypeIncompatible(const SVM_Valeur::AdresseMemoire& adresse, const Type& type, const std::string& types_attendus)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Read to initialised address " << adresse << " contains a value of type " << type << " instead of " << types_attendus << ".");
		}
		virtual ~LectureAdresseTypeIncompatible() {}
	};

	struct EcritureAdresseNonDefinie : public SVM_Valeur::ExceptionExecution
	{
		EcritureAdresseNonDefinie(const SVM_Valeur::AdresseMemoire& adresse)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Write to non defined address " << adresse << ".");
		}
		virtual ~EcritureAdresseNonDefinie() {}
	};

	struct SynchronisationAdresseNonDefinie : public SVM_Valeur::ExceptionExecution
	{
		SynchronisationAdresseNonDefinie(const SVM_Valeur::AdresseMemoire& adresse)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Synchronization of non defined address " << adresse << ".");
		}
		virtual ~SynchronisationAdresseNonDefinie() {}
	};

	struct DesynchronisationAdresseNonDefinie : public SVM_Valeur::ExceptionExecution
	{
		DesynchronisationAdresseNonDefinie(const SVM_Valeur::AdresseMemoire& adresse)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Desynchronization of non defined address " << adresse << ".");
		}
		virtual ~DesynchronisationAdresseNonDefinie() {}
	};

	struct EcritureAdresseTypeIncompatible : public SVM_Valeur::ExceptionExecution
	{
		EcritureAdresseTypeIncompatible(const SVM_Valeur::AdresseMemoire& adresse, const Type& type, const Type& type_attendu)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Write to defined address " << adresse << " with a value of type " << type << " instead of " << type_attendu << ".");
		}
		virtual ~EcritureAdresseTypeIncompatible() {}
	};

	struct TypesIncompatibles : public SVM_Valeur::ExceptionExecution
	{
		TypesIncompatibles(const SVM_Valeur::AdresseMemoire& origine, const Type& type_origine, const SVM_Valeur::AdresseMemoire& destination, const Type& type_destination)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Incompatible type from " << type_origine << " at " << origine << " to " << type_destination << " at " << destination << ".");
		}
		virtual ~TypesIncompatibles() {}
	};

	struct TailleBlocIncompatibles : public SVM_Valeur::ExceptionExecution
	{
		TailleBlocIncompatibles(const size_t taille1, const size_t taille2)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Memory block size mismatch: " << taille1 << " vs " << taille2 << ".");
		}
	};

	struct CopieNonDisponible : public SVM_Valeur::ExceptionExecution
	{
		CopieNonDisponible(const SVM_Valeur::AdresseMemoire& adresse, const Type& type)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Value at address " << adresse << " has type " << type << " without copy operation defined.");
		}
		virtual ~CopieNonDisponible() {}
	};

	struct PiegeMemoireElement
	{
		PiegeMemoireElement(const size_t adresse, const TypeSP& type, const ValeurSP& valeur)
		:_adresse(adresse), _type(type), _valeur(valeur) {}
		size_t _adresse;
		TypeSP _type;
		ValeurSP _valeur;
	};
	
	struct PiegeMemoireAlias
	{
		PiegeMemoireAlias(const std::string& alias, const SVM_Valeur::PointeurSP& pointeur)
		:_alias(alias), _pointeur(pointeur) {}

		std::string _alias;
		SVM_Valeur::PointeurSP _pointeur;
	};

	DECL_SHARED_PTR(PiegeMemoire);
	struct PiegeMemoire : public SVM::Machine::Debugueur::Piege
	{
		PiegeMemoire(const MemoireWP& memoire)
		:SVM::Machine::Debugueur::Piege("memory"), _memoire(memoire) {}
		virtual ~PiegeMemoire() {}

		virtual SVM::Machine::Debugueur::JSON_Interne::ObjetSP mise_a_jour() override;
		virtual void evenement(const SVM_Debugueur::JSON_Interne::ObjetSP& evenement) override;

		void allocation(const size_t adresse_debut, const DescriptionBloc& bloc);
		void quota(const SVM_Noyau::LimiteUtilisationSP& utilisation);
		void transformation(const PiegeMemoireElement& element);
		void transformation(const PiegeMemoireAlias& alias);
		void transformation(const std::vector<PiegeMemoireElement>& elements, const std::vector<PiegeMemoireAlias>& alias);
		void point_arret_lecture(const SVM_Valeur::AdresseMemoire& adresse, const bool ajout);
		void point_arret_ecriture(const SVM_Valeur::AdresseMemoire& adresse, const bool ajout);
		void point_arret_acces(const SVM_Valeur::AdresseMemoire& adresse, const bool ajout);
		void point_arret_suppression(const SVM_Valeur::AdresseMemoire& adresse, const bool ajout);
		void point_arret_lecture(const SVM_Valeur::AdresseMemoire& adresse, const Type& type, const SVM_Memoire::ValeurSP& valeur);
		void point_arret_ecriture(const SVM_Valeur::AdresseMemoire& adresse, const Type& type_avant, const SVM_Memoire::ValeurSP& valeur_avant, const Type& type_apres, const SVM_Memoire::ValeurSP& valeur_apres, const PiegeMemoireSP& piege = PiegeMemoireSP());
		void point_arret_changement(const SVM_Valeur::AdresseMemoire& adresse, const Type& type_apres, const SVM_Memoire::ValeurSP& valeur_apres, const PiegeMemoireSP& piege = PiegeMemoireSP());
		void point_arret_suppression(const SVM_Valeur::AdresseMemoire& adresse, const Type& type_avant, const SVM_Memoire::ValeurSP& valeur_avant);

		MemoireWP _memoire;
		std::set<SVM_Valeur::AdresseMemoire> _points_arret_lecture;
		std::set<SVM_Valeur::AdresseMemoire> _points_arret_ecriture;
		std::set<SVM_Valeur::AdresseMemoire> _points_arret_acces;
		std::set<SVM_Valeur::AdresseMemoire> _points_arret_suppression;
	};

	struct Memoire : public std::enable_shared_from_this<Memoire>
	{
		friend struct SVM::Machine::Element::Processeur::Processeur;
		friend struct PiegeMemoire;
		static MemoireSP creation_memoire(SVM::Machine::Debugueur::DebugueurSP& debugueur, const SVM_Noyau::ControleAccesSP& controle_acces);
		Memoire(const SVM_Noyau::ControleAccesSP& controle_acces)
		{
			if(static_cast<bool>(controle_acces)) { _utilisation = controle_acces->_memoire_allouee; }
			SVM_TRACE("Creation memoire " << this);
		}
		virtual ~Memoire()
		{
			SVM_TRACE("Destruction memoire " << this);
		}
		SVM_Valeur::Pointeur allocation(const DescriptionBloc& bloc);
		std::pair<bool,SVM_Valeur::Pointeur> allocation(const DescriptionBloc& bloc, const SVM_Valeur::AdresseMemoire& adresse);
		void liberation(const std::set<SVM_Valeur::Pointeur>& bloc);
		void verification_alias(const DescriptionBloc& bloc);
		void definition_alias(const DescriptionBloc& bloc, SVM_Valeur::Pointeur& emplacement);
		void suppression_alias(const std::set<std::string>& alias);
		void definition_alias(const std::string& alias, SVM_Valeur::Pointeur& emplacement);
		bool existence_alias(const std::string& alias);
		SVM_Valeur::Pointeur resolution_alias(const std::string& alias);
		template<bool strict=true>
		ValeurSP lecture(const SVM_Valeur::AdresseMemoire& adresse, const std::function<bool(const Type& type)>& validateur_type, const std::string& types_attendus /* pour message exception */, const bool point_arret = true);
		template<bool strict=true>
		std::vector<ValeurSP> lecture(const SVM_Valeur::Pointeur& pointeur, const std::function<bool(const Type& type)>& validateur_type, const std::string& types_attendus /* pour message exception */, const bool point_arret = true);
		template<bool strict=true>
		std::vector<ValeurSP> lecture(const SVM_Valeur::Pointeur& pointeur, const DescriptionBloc& zone, const bool point_arret = true);
		void ecriture(const SVM_Valeur::AdresseMemoire& adresse, const ValeurSP& valeur);
		void ecriture(const SVM_Valeur::Pointeur& pointeur, const std::vector<ValeurSP>& valeurs);
		bool inscriptible(const SVM_Valeur::AdresseMemoire& adresse, const ValeurSP& valeur) const
		{
			if(not adresse_definie(adresse)) return false;
			if(_elements[adresse._adresse]->_type==Type(Type::TypeInterne::AUTOMATIQUE)) return true;
			return _elements[adresse._adresse]->_type==(*valeur);
		}
		template<typename Exception = EcritureAdresseNonDefinie>
		Type type(const SVM_Valeur::AdresseMemoire& adresse) const
		{
			if(adresse._adresse>=_elements.size())
			{
				throw Exception(adresse);
			}
			ElementSP element = _elements[adresse._adresse];
			if(not element)
			{
				throw Exception(adresse);
			}
			SVM_Synchronisation::LecteurUP lecteur;
			if(static_cast<bool>(element->_acces))
			{
				lecteur = element->_acces->lecteur();
			}
			return element->_type;
		}
		void change_type(const SVM_Valeur::AdresseMemoire& adresse, const Type& type);
		void change_type(const SVM_Valeur::Pointeur& pointeur, const DescriptionBloc& zone);
		void active_synchronisation(const SVM_Valeur::Pointeur& pointeur);
		void desactive_synchronisation(const SVM_Valeur::Pointeur& pointeur);
		bool adresse_definie(const SVM_Valeur::AdresseMemoire& adresse) const
		{
			if(adresse>=_elements.size())
				return false;
			return static_cast<bool>(_elements[adresse._adresse]);
		}
		bool adresse_initialisee(const SVM_Valeur::AdresseMemoire& adresse) const
		{
			if(not adresse_definie(adresse))
				return false;
			return _elements[adresse._adresse]->initialise();
		}
		bool adresse_type(const SVM_Valeur::AdresseMemoire& adresse, const Type& type)
		{
			if(not adresse_definie(adresse))
				return false;
			return _elements[adresse._adresse]->type(type);
		}
		void decalage(const SVM_Valeur::AdresseMemoire& adresse, const long long int increment);
		std::set<SVM_Valeur::Pointeur> accessible(const SVM_Valeur::Pointeur& pointeur) const;
		template<bool strict=true>
		std::set<std::string> alias_inclus(const std::set<SVM_Valeur::Pointeur>& pointeurs) const;
		void compatible(const SVM_Valeur::Pointeur& origine, const SVM_Memoire::MemoireSP& distante, const SVM_Valeur::Pointeur& destination, std::vector<SVM_Synchronisation::LecteurUP>& lecteurs, std::vector<SVM_Synchronisation::EcrivainUP>& ecrivains) const ;
		void copie(const SVM_Valeur::Pointeur& origine, const SVM_Memoire::MemoireSP& distante, const SVM_Valeur::Pointeur& destination);
		void deplacement(const SVM_Valeur::Pointeur& origine, const SVM_Memoire::MemoireSP& distante, const SVM_Valeur::Pointeur& destination);
		void partage(const SVM_Valeur::Pointeur& origine, const SVM_Memoire::MemoireSP& distante, const SVM_Valeur::Pointeur& destination);
		void traduit(const SVM_Valeur::Pointeur& zone, const SVM_Valeur::Pointeur& origine, const SVM_Valeur::Pointeur& destination);
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Memoire& memoire)
		{
			bool vide=false;
			f << "Memory:" << std::endl;
			size_t adresse = 0;
			for(auto& element:memoire._elements)
			{
				if(static_cast<bool>(element))
				{
					f << "  &" << adresse << ": " << element->serialise() << std::endl;
					vide = false;
				}
				else
				{
					if(not vide)
					{
						f << "   ..." << std::endl;
					}
					vide = true;
				}
				++adresse;
			}
			f << " Aliases:" << std::endl;
			for(auto& a:memoire._alias)
			{
				f << "  " << a.first << " -> " << a.second << std::endl;
			}
			f << " Free space:" << std::endl;
			for(auto& ml:memoire._memoire_libre)
			{
				f << "  &" << ml.second << " ~ &" << (ml.first+ml.second-1) << " (" << ml.first << ")" << std::endl;
			}
			return f;
		}
		private:
			std::vector<ElementSP> _elements;
			std::map<std::string,SVM_Valeur::Pointeur> _alias;
			std::multimap<size_t,size_t> _memoire_libre; // clef = taille, valeur = adresse
			SVM_Noyau::LimiteUtilisationSP _utilisation;
		public:
			PiegeMemoireSP _piege;
			static std::mutex& protection()
			{
				static std::mutex protection;
				return protection;
			}
			SVM_Noyau::NoyauWP _noyau;
	};
}
}
}
}
