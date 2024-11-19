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
#include <src/machine/elements/base/base.h>
#include <src/machine/elements/systeme/systeme.h>
#include <src/machine/elements/memoire/memoire.h>
#include <src/machine/elements/valeur/point_entree_extension.h>
#include <src/machine/extensions/gestionnaire/gestionnaire.h>
#include <src/machine/machine/historique.h>
#include <src/machine/machine/demarrage.h>
#include <src/machine/debugueur/debugueur.h>

namespace SVM_Base = SVM::Machine::Element::Base;
namespace SVM_Systeme = SVM::Machine::Element::Systeme;
namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Memoire = SVM::Machine::Element::Memoire;
namespace SVM_Debugueur = SVM::Machine::Debugueur;

namespace SVM
{
namespace Machine
{
namespace Extension
{
	DECL_SHARED_PTR(Gestionnaire);
}

DECL_SHARED_PTR(PiegeMachine);
struct PiegeMachine : public SVM_Debugueur::Piege
{
	enum class Etat { DEMARRAGE, ENCOURS, TERMINEE, TUEE, ERREUR };
	PiegeMachine()
		:Piege("machine"), _etat(Etat::DEMARRAGE) { }
	virtual ~PiegeMachine() {}
	virtual SVM_Debugueur::JSON_Interne::ObjetSP mise_a_jour() override
	{
		return SVM_Debugueur::JSON::objet({ { "state" , SVM_Debugueur::JSON::chaine(chaine_etat()) } , { "version" , SVM_Debugueur::JSON::chaine(PACKAGE_VERSION)} });
	}
	virtual void evenement(const SVM_Debugueur::JSON_Interne::ObjetSP& evenement) override
	{
		auto debug = debugueur();
		auto mort = SVM_Debugueur::JSON::booleen((*evenement)["kill"]);
		if(SVM_Debugueur::JSON::est(mort))
		{
			tuee();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			::kill(::getpid(),SIGKILL);
		}
	}
	void demarree()
	{
		_etat = Etat::ENCOURS;
		notification_changement_objet();
	}
	void terminee()
	{
		_etat = Etat::TERMINEE;
		notification_changement_objet();
		notification_fin();
	}
	void tuee()
	{
		_etat = Etat::TUEE;
		notification_changement_objet();
		notification_fin();
	}
	void erreur()
	{
		_etat = Etat::ERREUR;
		notification_changement_objet();
		notification_fin();
	}
	private:
	std::string chaine_etat() const
	{
		switch(_etat)
		{
			case Etat::DEMARRAGE: return "START";
			case Etat::ENCOURS: return "EXECUTION";
			case Etat::TERMINEE: return "TERMINATED";
			case Etat::TUEE: return "KILLED";
			case Etat::ERREUR: return "ERROR";
			default: return "UNKNOWN";
		}
	}
	bool fin() const
	{
		switch(_etat)
		{
			case Etat::DEMARRAGE: 
			case Etat::ENCOURS:
				return false; 
			default:
				break;
		}
		return true;
	}
	Etat _etat;
};

DECL_WEAK_PTR(Machine);
DECL_SHARED_PTR(Machine);

struct Machine : public std::enable_shared_from_this<Machine>
{
	Machine(const SHARED_PTR(std::string)& description, const HistoriqueSP& historique, const std::vector<DeclarationExtension>& extensions, const std::vector<Processus>& processus, const DebugSP& debugueur, const PerfSP& performance);
	int execution();
	SHARED_PTR(std::string) _description;
	HistoriqueSP _historique;
	std::vector<DeclarationExtension> _extensions;
	std::vector<Processus> _processus;
	SVM_Systeme::SystemeSP _systeme;
	SVM_Base::BaseSP _base;
	PiegeMachineSP _piege;
	DebugSP _debugueur;
	PerfSP _performance;
};
}
}
