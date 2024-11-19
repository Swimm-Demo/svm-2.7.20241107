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
#include <src/machine/elements/processus/etat_processus.h>
#include <src/machine/elements/base/base.h>
#include <src/machine/elements/valeur/point_entree_extension.h>
#include <src/machine/elements/valeur/code.h>
#include <src/machine/elements/memoire/memoire.h>
#include <src/machine/elements/sequenceur/sequenceur.h>
#include <src/machine/interface/variable.h>
#include <src/machine/elements/systeme/evenement.h>
#include <src/machine/debugueur/piege.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Memoire = SVM::Machine::Element::Memoire;
namespace SVM_Sequenceur = SVM::Machine::Element::Sequenceur;
namespace SVM_Base = SVM::Machine::Element::Base;
namespace SVM_Systeme = SVM::Machine::Element::Systeme;
namespace SVM_Debugueur = SVM::Machine::Debugueur;

namespace SVM
{
namespace Machine
{
namespace Interface
{
	struct Outils;
	struct VerrouProcessus;
	struct Fonction;
	struct VariableProcessus;
}
namespace Element
{
namespace Ordonnanceur
{
	DECL_WEAK_PTR(Ordonnanceur);
	struct OrdonnanceurDefaut;
	struct OrdonnanceurExtension;
}
namespace Systeme
{
	DECL_SHARED_PTR(Systeme);
	DECL_WEAK_PTR(Systeme);
}
namespace Processeur
{
namespace Instruction
{
	struct Extension;
}
}
namespace Processus
{
	struct ProcessusNonLance : public SVM_Valeur::ExceptionExecution
	{
		ProcessusNonLance(const std::string& nom, const std::string& details)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Process " << nom << " not started: " << details);
		}
		virtual ~ProcessusNonLance() {}
	};

	DECL_SHARED_PTR(Processus);
	DECL_WEAK_PTR(Processus);

	DECL_SHARED_PTR(PiegeProcessus);
	struct PiegeProcessus : public SVM_Debugueur::Piege
	{
		PiegeProcessus(const ProcessusSP& processus)
		:SVM_Debugueur::Piege("process"), _processus(processus) {}
		virtual ~PiegeProcessus() {}
		virtual SVM_Debugueur::JSON_Interne::ObjetSP mise_a_jour() override;
		ProcessusWP _processus;
	};

	struct Processus : public std::enable_shared_from_this<Processus>
	{
		friend struct PiegeProcessus;
		friend struct SVM::Machine::Interface::Outils;
		friend struct SVM::Machine::Interface::VerrouProcessus;
		friend struct SVM::Machine::Interface::Fonction;
		friend struct SVM::Machine::Interface::VariableProcessus;
		friend struct Processeur::Instruction::Extension;
		friend struct Ordonnanceur::Ordonnanceur;
		friend struct Ordonnanceur::OrdonnanceurDefaut;
		friend struct Ordonnanceur::OrdonnanceurExtension;
		friend struct Systeme::Systeme;
		static ProcessusSP creation_processus(const std::string& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique, SVM_Noyau::NoyauSP& noyau);
		static ProcessusSP creation_processus(const std::string& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique, SVM_Valeur::CodeSP& code, SVM_Memoire::MemoireSP& memoire, const bool transmet_interruption = true, const bool retour_est_arret = false, const bool mode_protege = false, const SVM_Noyau::ControleAccesSP& controle_acces = SVM_Noyau::ControleAccesSP());
		static ProcessusSP creation_processus(const std::string& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique, const SVM_Valeur::AdresseInstruction& code, SVM_Memoire::MemoireSP& memoire, const bool transmet_interruption = true, const bool retour_est_arret = false, const bool mode_protege = false, const SVM_Noyau::ControleAccesSP& controle_acces = SVM_Noyau::ControleAccesSP());
		Processus(const std::string& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const bool terminaison_automatique);
		Processus(const Processus&) = delete;
		~Processus();
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Processus& p)
		{
			p.nom_flux(f);
			f << ": " << p._etat << " (";
			p._sequenceur->affiche(f);
			f << ")" << std::endl;
			for(auto& n:p._noyaux)
			{
				f << *n << std::endl;
			}
			return f;
		}
		template<typename Flux>
		void nom_flux(Flux& f) const
		{
			f << "PROCESS " << _nom;
		}
		std::string nom_debug() const
		{
			std::ostringstream oss;
			oss << "Process " << _nom << ":" << _identifiant;
			return oss.str();
		}
		template<typename Flux>
		void status(Flux& f) const
		{
			nom_flux(f);
			f << ": " << _etat;
		}
#ifdef LINUX_EXTENSIONS
		template<typename Flux>
		void nom_flux_court(Flux& f) const
		{
			f << (_etat.terminaison_automatique()?"p":"P") << " " << _nom;
		}
#endif
		bool termine() { return _etat.termine(); }
		bool en_execution() {
			interactions();
			return _etat.en_execution();
		}
		bool vivant() { return _etat.vivant() and not _ordonnanceur.expired(); };
		void erreur(const std::string& message) { _etat.erreur(message); }
		SHARED_PTR(size_t) code_retour() const { return _etat.code_retour(); }
		SHARED_PTR(std::string) erreur() const { return _etat.erreur(); }
		SVM_Valeur::InterruptionSP interrompu() const { return _etat.interrompu(); }
		void execution_demandee(const SVM_Synchronisation::AdresseSP& source, const size_t delai = 0)
		{
			++_notification;
			_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::EXECUTION,delai));
		}
		void suspension_demandee(const SVM_Synchronisation::AdresseSP& source)
		{
			++_notification;
			_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::SUSPENSION));
			notification_interruption();
		}
		void terminaison_demandee(const SVM_Synchronisation::AdresseSP& source)
		{
			++_notification;
			_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::TERMINAISON));
			notification_interruption();
		}
		void attente_terminaison()
		{
			if(_travailleur.joinable()) _travailleur.join();
		}
		bool bloquage_demande(const SVM_Synchronisation::AdresseSP& source)
		{
			if(termine())
			{
				// On ne peut jamais bloquer un processus termine
				return false;
			}
			++_notification;
			_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::VERROUILLAGE));
			notification_interruption();
			return true;
		}
		bool tentative_bloquage_demande(const SVM_Synchronisation::AdresseSP& source)
		{
			if(termine())
			{
				// On ne peut jamais bloquer un processus termine
				return false;
			}
			++_notification;
			_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::VERROUILLAGE,SVM_Systeme::Evenement::Reponse::OUI));
			notification_interruption();
			return true;
		}
		void ajoute_noyau(const SVM_Noyau::NoyauSP& noyau)
		{
			_noyaux.insert(noyau);
			if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		}
		void enleve_noyau(const SVM_Noyau::NoyauSP& noyau)
		{
			auto it = _noyaux.find(noyau);
			if(it!=_noyaux.end())
			{
				_noyaux.erase(it);
				if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
			}
		}
		SVM_Systeme::Evenement::Reponse attente_verrouillage(const SVM_Synchronisation::AdresseSP& verrouillage);
		std::pair<SVM_Systeme::Evenement::Reponse,SVM_Synchronisation::AdresseSP> attente_verrouillage();
		bool verrouillage_mortel(const SVM_Synchronisation::AdresseSP& verrouillage) const
		{
			std::lock_guard<std::mutex> verrou(_verrouillage);
			if(not _verrouillage_en_cours)
				return false;
			if(_verrouillage_en_cours==verrouillage)
				return true;
			auto p = _livraison->objet<SVM_Processus::Processus>(_verrouillage_en_cours);
			if(not p)
				return false;
			return p->verrouillage_mortel(verrouillage);
		}
		bool debloquage_demande(const SVM_Synchronisation::AdresseSP& source)
		{
			if(termine())
			{
				// On ne peut jamais bloquer un processus termine
				return false;
			}
			++_notification;
			_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::DEVERROUILLAGE));
			return true;
		}
		void attente_deverrouillage(const SVM_Synchronisation::AdresseSP& verrouillage)
		{
			auto it = _verrous_en_cours.find(verrouillage);
			if(it!=_verrous_en_cours.end())
			{
				_verrous_en_cours.erase(it);
			}
		}
		SVM_Systeme::Evenement::Reponse attente_reponse(const SVM_Synchronisation::AdresseSP& origine);
		bool entree_mode_attente();
		bool sortie_mode_attente();
		void mode_debugueur(const bool mode);
		bool verrouille_par(const SVM_Synchronisation::AdresseSP& source) const { return _verrouille_par==source; }
		Etat::Execution etat_global() const { return _etat.etat_global(); }
		void interactions();
		void execution();
		void accepte_notification_interruption();
		void ignore_notification_interruption();
		void attend_notification_interruption();
		void notification_interruption();
		void force_deverrouillage(const SVM_Synchronisation::AdresseSP& adresse);
		const std::string _nom;
		private:
		Etat _etat;
		public:
		SVM_Noyau::NoyauSP _noyau_courant;
		private:
		SVM::Machine::Element::Ordonnanceur::OrdonnanceurWP _ordonnanceur;
		public:
		SVM::Machine::Element::Systeme::SystemeWP _systeme;
		SVM_Base::BaseWP _base;
		SVM_Sequenceur::SequenceurSP _sequenceur;
		std::set<SVM_Noyau::NoyauSP> _noyaux;
		private:
		mutable std::mutex _execution;
		std::thread _travailleur;
		SVM_Systeme::LivraisonSP _livraison;
		SVM_Synchronisation::AdresseSP _adresse;
		std::list<SVM_Synchronisation::AdresseSP> _verrouillages_en_attente;
		SVM_Synchronisation::AdresseSP _verrouille_par;
		size_t _nombre_verrouillage;
		std::set<SVM_Synchronisation::AdresseSP> _verrous_en_cours;
		SVM_Synchronisation::AdresseSP _verrouillage_en_cours;
		mutable std::mutex _verrouillage;
		std::atomic<ssize_t> _notification;
		std::atomic<bool> _accepte_notification_interruption;
		public:
		SVM_Synchronisation::NotificationSP _notification_recue;
		private:
		PiegeProcessusSP _piege;
		public:
		unsigned long long int _identifiant = 0;
	};
}
}
}
}
