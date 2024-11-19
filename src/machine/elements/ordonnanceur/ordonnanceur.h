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
#include <src/machine/elements/processus/processus.h>
#include <src/machine/elements/systeme/evenement.h>
#include <src/machine/machine/historique.h>
#include <src/machine/debugueur/piege.h>

namespace SVM_Processus = SVM::Machine::Element::Processus;
namespace SVM_Machine = SVM::Machine;
namespace SVM_Debugueur = SVM::Machine::Debugueur;

namespace SVM
{
namespace Machine
{
namespace Interface
{
	struct Outils;
	struct VerrouProcessus;
}
namespace Element
{
namespace Systeme
{
	DECL_SHARED_PTR(Systeme);
	DECL_WEAK_PTR(Systeme);
}
namespace Ordonnanceur
{

	struct OrdonnanceurNonLance : public SVM_Valeur::ExceptionExecution
	{
		OrdonnanceurNonLance(const SVM_Valeur::PointEntreeExtensionSP& nom, const std::string& details)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			if(static_cast<bool>(nom))
			{
				DETAILS_EXCEPTION("Scheduler " << nom << " not started: " << details);
			}
			else
			{
				DETAILS_EXCEPTION("Default scheduler not started: " << details);
			}
		}
		virtual ~OrdonnanceurNonLance() {}
	};

DECL_SHARED_PTR(Ordonnanceur);
DECL_WEAK_PTR(Ordonnanceur);

DECL_SHARED_PTR(PiegeOrdonnanceur);
struct PiegeOrdonnanceur : public SVM_Debugueur::Piege
{
	PiegeOrdonnanceur(const OrdonnanceurSP& ordonnanceur)
	:Piege("scheduler"), _ordonnanceur(ordonnanceur) {}
	virtual ~PiegeOrdonnanceur() {}
	virtual SVM_Debugueur::JSON_Interne::ObjetSP mise_a_jour() override;

	OrdonnanceurWP _ordonnanceur;
};

struct Ordonnanceur : public std::enable_shared_from_this<Ordonnanceur>
{
	friend struct SVM::Machine::Interface::Outils;
	friend struct SVM::Machine::Interface::VerrouProcessus;
	Ordonnanceur(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme);
	Ordonnanceur(const Ordonnanceur&) = delete;
	virtual ~Ordonnanceur();

	bool operator<(const Ordonnanceur& o) const
	{
		if(not _nom)
			return true;
		if(not o._nom)
			return false;
		return _nom<o._nom;
	}
	void attache(const SVM_Synchronisation::AdresseSP& source, const SVM_Processus::ProcessusSP& processus, const size_t parametre);
	void detache(const SVM_Synchronisation::AdresseSP& source, const SVM_Processus::ProcessusSP& processus, const size_t parametre);
	void notifie(const SVM_Synchronisation::AdresseSP& source, const size_t parametre);
	void terminaison_demandee(const SVM_Synchronisation::AdresseSP& source);

	void initialisation();
	void execution();
	void notifie_etat(const SVM_Synchronisation::AdresseSP source, const SVM_Processus::Etat::Execution etat)
	{
		_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::ETAT,etat));
	}
	void pret(const SVM_Processus::ProcessusSP& processus)
	{
		_livraison->poste(_adresse,processus->_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::ETAT,SVM_Processus::Etat::Execution::SUSPENDU));
	}
	SVM_Systeme::Evenement::Reponse attente_verrouillage(const SVM_Synchronisation::AdresseSP& verrouillage);
	virtual void *interne() = 0;
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const Ordonnanceur& o)
	{
		o.nom_flux(f);
		f << ":" << std::endl;
		o.affiche(f);
		return f;
	}
	template<typename Flux>
	void nom_flux(Flux& f) const
	{
		f << "SCHEDULER";
		if(static_cast<bool>(_nom))
		{
			f << " " << *_nom;
		}
	}
#ifdef LINUX_EXTENSIONS
	template<typename Flux>
	void nom_flux_court(Flux& f) const
	{
		f << "S";
		if(static_cast<bool>(_nom))
		{
			f << " " << *_nom;
		}
	}
#endif
	template<typename E>
	void erreur(const E& e) const
	{
		SVM_Machine::HistoriqueSP h = historique();
		if(not h) return;
		std::ostringstream oss;
		nom_flux(oss);
		SVM_Machine::Trace trace(h,oss.str());
		
		trace << "Scheduler error in ";
		nom_flux(trace);
		trace << ": " << e;
	}
	SVM_Machine::HistoriqueSP historique() const;
	const SVM_Valeur::PointEntreeExtensionSP& nom() const { return _nom; }
	PiegeOrdonnanceurSP _piege;
	protected:
	SVM_Valeur::PointEntreeExtensionSP _nom;
	SVM::Machine::Element::Systeme::SystemeWP _systeme;
	SVM_Systeme::LivraisonSP _livraison;
	SVM_Synchronisation::AdresseSP _adresse;
	private:
	const SVM_Synchronisation::AdresseSP& adresse() const { return _adresse; }
	SVM_Systeme::SystemeSP systeme() const { return _systeme.lock(); }
	std::thread _effecteur;
	virtual size_t ordonnancement(const SVM_Processus::ProcessusSP& processus, const SVM_Processus::Etat::Execution etat_processus) = 0;
	virtual size_t notification(const SVM_Synchronisation::AdresseSP& source, const SVM_Notification_Type type, const size_t parametre) = 0;
	virtual bool attache_processus(const SVM_Processus::ProcessusSP& processus, const size_t parametre) = 0;
	virtual bool detache_processus(const SVM_Processus::ProcessusSP& processus, const size_t parametre) = 0;
	virtual void affiche(std::ostream& os) const = 0;
};

	struct OperationNonSupportee : public SVM_Valeur::ExceptionExecution
	{
		OperationNonSupportee(const SVM_Valeur::PointEntreeExtensionSP& nom, const std::string& op)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			if(static_cast<bool>(nom))
			{
				DETAILS_EXCEPTION("Scheduler " << *nom << " has no " << op << " operation.");
			}
			else
			{
				DETAILS_EXCEPTION("Default scheduler has no " << op << " operation.");
			}
		}
		virtual ~OperationNonSupportee() {}
	};

	struct ProcessusDejaAttache : public SVM_Valeur::ExceptionExecution
	{
		ProcessusDejaAttache(const SVM_Valeur::PointEntreeExtensionSP& nom, const std::string& processus)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			if(static_cast<bool>(nom))
			{
				DETAILS_EXCEPTION("Cannot attach already attached process " << processus << " to " << (*nom) << " scheduler.");
			}
			else
			{
				DETAILS_EXCEPTION("Cannot attach already attached process " << processus << " to default scheduler.");
			}
		}
		virtual ~ProcessusDejaAttache() {}
	};

	struct ProcessusAttacheParAutreOrdonnanceur : public SVM_Valeur::ExceptionExecution
	{
		ProcessusAttacheParAutreOrdonnanceur(const SVM_Valeur::PointEntreeExtensionSP& nom, const std::string& processus)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			if(static_cast<bool>(nom))
			{
				DETAILS_EXCEPTION("Cannot detach attached process " << processus << " on another scheduler to " << (*nom) << " scheduler.");
			}
			else
			{
				DETAILS_EXCEPTION("Cannot detach attached process " << processus << " on another scheduler to default scheduler.");
			}
		}
		virtual ~ProcessusAttacheParAutreOrdonnanceur() {}
	};

	struct ProcessusDejaDetache : public SVM_Valeur::ExceptionExecution
	{
		ProcessusDejaDetache(const SVM_Valeur::PointEntreeExtensionSP& nom, const std::string& processus)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			if(static_cast<bool>(nom))
			{
				DETAILS_EXCEPTION("Cannot detach already detached process " << processus << " to " << (*nom) << " scheduler.");
			}
			else
			{
				DETAILS_EXCEPTION("Cannot detach already detached process " << processus << " to default scheduler.");
			}
		}
		virtual ~ProcessusDejaDetache() {}
	};

}
}
}
}
