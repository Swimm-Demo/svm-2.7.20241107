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
#include <src/machine/elements/synchronisation/evenement.h>
#include <src/machine/elements/processus/etat_processus.h>
#include <src/machine/debugueur/piege.h>

namespace SVM_Processus = SVM::Machine::Element::Processus;
namespace SVM_Debugueur = SVM::Machine::Debugueur;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Processus
{
	DECL_SHARED_PTR(Processus);
}
namespace Systeme
{
	DECL_SHARED_PTR(Evenement);
	struct Evenement
	{
		enum class Type
		{
			ETAT,
			EXECUTION,
			SUSPENSION,
			TERMINAISON,
			VERROUILLAGE,
			DEVERROUILLAGE,
			REPONSE,
			ATTACHE,
			DETACHE,
			NOTIFICATION
		};
		enum class Reponse
		{
			ERREUR,
			NON,
			OUI,
			AUTRE_VERROUILLE,
			DEJA_VERROUILLE,
			VERROU_MORTEL
		};
		Evenement(const Type& type)
		:Evenement(type,SVM_Processus::Etat::Execution::ERREUR,Reponse::NON,0,SVM_Processus::ProcessusSP()) {}
		Evenement(const Type& type, const SVM_Processus::Etat::Execution etat)
		:Evenement(type,etat,Reponse::NON,0,SVM_Processus::ProcessusSP()) {}
		Evenement(const Type& type, const Reponse reponse)
		:Evenement(type,SVM_Processus::Etat::Execution::ERREUR,reponse,0,SVM_Processus::ProcessusSP()) {}
		Evenement(const Type& type, const Reponse reponse, const SVM_Processus::ProcessusSP& processus)
		:Evenement(type,SVM_Processus::Etat::Execution::ERREUR,reponse,0,processus) {}
		Evenement(const Type& type, const size_t entier)
		:Evenement(type,SVM_Processus::Etat::Execution::ERREUR,Reponse::NON,entier,SVM_Processus::ProcessusSP()) {}
		Evenement(const Type& type, const SVM_Processus::ProcessusSP& processus)
		:Evenement(type,SVM_Processus::Etat::Execution::ERREUR,Reponse::NON,0,processus) {}
		Evenement(const Type& type, const SVM_Processus::Etat::Execution etat, const size_t entier)
		:Evenement(type,etat,Reponse::NON,entier,SVM_Processus::ProcessusSP()) {}
		Evenement(const Type& type, const SVM_Processus::ProcessusSP& processus, const size_t entier)
		:Evenement(type,SVM_Processus::Etat::Execution::ERREUR,Reponse::NON,entier,processus) {}
		Evenement(const Type& type, const SVM_Processus::ProcessusSP& processus, const size_t entier, const bool reponse)
		:Evenement(type,SVM_Processus::Etat::Execution::ERREUR,reponse?Reponse::OUI:Reponse::NON,entier,processus) {}

		Type _type;
		SVM_Processus::Etat::Execution _etat;
		Reponse _reponse;
		size_t _entier;
		SVM_Processus::ProcessusSP _processus;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const EvenementSP& e)
		{
			if(static_cast<bool>(e))
			{
				f << "[";
				switch(e->_type)
				{
					case Type::ETAT: f << "ETAT"; break;
					case Type::EXECUTION: f << "EXEC"; break;
					case Type::SUSPENSION: f << "SUSP"; break;
					case Type::TERMINAISON: f << "TERM"; break;
					case Type::VERROUILLAGE: f << "VERR"; break;
					case Type::DEVERROUILLAGE: f << "DEVR"; break;
					case Type::REPONSE: f << "REPS"; break;
					case Type::ATTACHE: f << "ATCH"; break;
					case Type::DETACHE: f << "DTCH"; break;
					case Type::NOTIFICATION: f << "NOTF"; break;
				}
				f << " " << SVM_Processus::Etat::texte(e->_etat) << " ";
				switch(e->_reponse)
				{
					case Reponse::ERREUR: f << "ERR"; break;
					case Reponse::NON: f << "NON"; break;
					case Reponse::OUI: f << "OUI"; break;
					case Reponse::AUTRE_VERROUILLE: f << "AVR"; break;
					case Reponse::DEJA_VERROUILLE: f << "DJV"; break;
					case Reponse::VERROU_MORTEL: f << "VRM"; break;
				}
				f << " " << e->_entier << " " << e->nom_processus() << "]";
			}
			return f;
		}
		std::string uml() const
		{
			std::ostringstream oss;
			switch(_type)
			{
				case Type::ETAT: oss << "state " << SVM_Processus::Etat::texte(_etat); break;
				case Type::EXECUTION:
					{
						oss << "execute";
						if(_entier>0)
						{
							oss << " with delay " << _entier << "ms";
						}
						break;
					}
				case Type::SUSPENSION: oss << "suspend"; break;
				case Type::TERMINAISON: oss << "terminate"; break;
				case Type::VERROUILLAGE:
					{
						oss << "lock";
						if(_reponse==Reponse::OUI)
						{
							oss << " trial";
						}
						break;
					}
				case Type::DEVERROUILLAGE: oss << "unlock"; break;
				case Type::REPONSE:
					{
						oss << "response ";
						switch(_reponse)
						{
							case Reponse::ERREUR: oss << "error"; break;
							case Reponse::NON: oss << "no"; break;
							case Reponse::OUI: oss << "yes"; break;
							case Reponse::DEJA_VERROUILLE: oss << "already locked"; break;
							case Reponse::AUTRE_VERROUILLE: oss << "locked by other"; break;
							case Reponse::VERROU_MORTEL: oss << "dead lock"; break;
						}
						break;
					}
				case Type::ATTACHE: oss << "attach " << nom_processus() << " " << _entier; break;
				case Type::DETACHE: oss << "detach " << nom_processus() << " " << _entier; break;
				case Type::NOTIFICATION: oss << "notification " << _entier; break;
			}
			return oss.str();
		}
		private:
		Evenement(const Type& type, const SVM_Processus::Etat::Execution etat, const Reponse reponse, const size_t entier, const SVM_Processus::ProcessusSP& processus)
		:_type(type), _etat(etat),_reponse(reponse), _entier(entier), _processus(processus) {}
		std::string nom_processus() const;
	};

	DECL_SHARED_PTR(PiegeEvenements);
	struct PiegeEvenements : public SVM_Debugueur::Piege
	{
		PiegeEvenements()
		:SVM_Debugueur::Piege("events"),_evenements(SVM_Debugueur::JSON::tableau()) { }
		virtual ~PiegeEvenements() {}
		virtual SVM_Debugueur::JSON_Interne::ObjetSP mise_a_jour() override
		{
			return SVM_Debugueur::JSON::objet({ { "events", _evenements} });
		}
		void poste(const SVM::Machine::Element::Synchronisation::AdresseSP& source, const SVM::Machine::Element::Synchronisation::AdresseSP& destination, const EvenementSP& evenement)
		{
			std::lock_guard<std::mutex> verrou(_protection);
			auto e = SVM_Debugueur::JSON::objet({ { "source" , PiegeEvenements::adresse(source) } , { "target" , PiegeEvenements::adresse(destination) } , { "type" , SVM_Debugueur::JSON::chaine("post") } , { "event" , SVM_Debugueur::JSON::chaine(evenement->uml()) } });
			auto indice = _evenements->taille();
			e->ajoute("id",SVM_Debugueur::JSON::entier(indice));
			_evenements->ajoute(e);
			_index.insert(std::make_pair(evenement.get(),indice));
			notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
		}
		void reception(const SVM::Machine::Element::Synchronisation::AdresseSP& source, const SVM::Machine::Element::Synchronisation::AdresseSP& destination, const EvenementSP& evenement)
		{
			std::lock_guard<std::mutex> verrou(_protection);
			auto e = SVM_Debugueur::JSON::objet({ { "source" , PiegeEvenements::adresse(source) } , { "target" , PiegeEvenements::adresse(destination) } , { "type" , SVM_Debugueur::JSON::chaine("recv") } , { "event" , SVM_Debugueur::JSON::chaine(evenement->uml()) } });
			auto it = _index.find(evenement.get());
			if(it!=_index.end())
			{
				e->ajoute("ref",SVM_Debugueur::JSON::entier(it->second));
				_index.erase(it);
			}
			_evenements->ajoute(e);
			notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
		}

		static SVM_Debugueur::JSON_Interne::ObjetSP adresse(const SVM::Machine::Element::Synchronisation::AdresseSP& adresse)
		{
			std::ostringstream oss;
			oss << adresse.get();
			auto id = oss.str();
			oss.str("");
			adresse->format(oss);
			return SVM_Debugueur::JSON::objet({ { "id" , SVM_Debugueur::JSON::chaine(id)} , { "name" , SVM_Debugueur::JSON::chaine(oss.str()) } });
		}

		mutable std::mutex _protection;
		SVM_Debugueur::JSON_Interne::TableauSP _evenements;
		std::map<void*,size_t> _index;
	};

	DECL_SHARED_PTR(Livraison);
	
	struct Livraison : public SVM::Machine::Element::Synchronisation::Livraison<Evenement>
	{
		PiegeEvenementsSP _piege;
		virtual void piege_poste(const SVM::Machine::Element::Synchronisation::AdresseSP& source, const SVM::Machine::Element::Synchronisation::AdresseSP& destination, const EvenementSP& evenement) override
		{
			if(static_cast<bool>(_piege)) { _piege->poste(source,destination,evenement); }
		}
		virtual void piege_reception(const SVM::Machine::Element::Synchronisation::AdresseSP& source, const SVM::Machine::Element::Synchronisation::AdresseSP& destination, const EvenementSP& evenement) override
		{
			if(static_cast<bool>(_piege)) { _piege->reception(source,destination,evenement); }
		}
	};

}
}
}
}
