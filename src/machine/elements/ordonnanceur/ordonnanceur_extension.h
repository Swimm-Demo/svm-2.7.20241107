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
#include <src/machine/elements/ordonnanceur/ordonnanceur.h>
#include <src/machine/interface/svm.h>

namespace SVM
{
namespace Machine
{
	DECL_SHARED_PTR(Machine);
	DECL_WEAK_PTR(Machine);
namespace Extension
{
	DECL_SHARED_PTR(PatronOrdonnanceurExtension);
}
namespace Element
{
namespace Ordonnanceur
{
	struct OrdonnanceurNonDefini : public SVM_Valeur::ExceptionExecution
	{
		OrdonnanceurNonDefini(const SVM_Valeur::PointEntreeExtensionSP& nom)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Scheduler "<< *nom << " does not exist.");
		}
		virtual ~OrdonnanceurNonDefini() {}
	};

	struct OrdonnanceurExtension : public Ordonnanceur
	{
		static OrdonnanceurSP creation_ordonnanceur(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme);
		OrdonnanceurExtension(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme);
		virtual ~OrdonnanceurExtension();
		virtual size_t ordonnancement(const SVM_Processus::ProcessusSP& processus, const SVM_Processus::Etat::Execution etat_processus) override;
		virtual size_t notification(const SVM_Synchronisation::AdresseSP& source, const SVM_Notification_Type type, const size_t parametre) override;
		virtual bool attache_processus(const SVM_Processus::ProcessusSP& processus, const size_t parametre) override;
		virtual bool detache_processus(const SVM_Processus::ProcessusSP& processus, const size_t parametre) override;
		virtual void affiche(std::ostream& os) const override;
		virtual void *interne() override { return _ordonnanceur; }
		private:
		SVM_Interface::VariablesDefiniesSP _variables;
		SVM::Machine::MachineWP _machine;
		SVM_Extension::PatronOrdonnanceurExtensionSP _patron;
		mutable std::mutex _protection;
		void *_ordonnanceur;
	};
}
}
}
}
