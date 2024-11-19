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
#include <src/machine/elements/sequenceur/sequenceur.h>
#include <src/machine/interface/svm.h>

namespace SVM
{
namespace Machine
{
	DECL_SHARED_PTR(Machine);
	DECL_WEAK_PTR(Machine);
namespace Extension
{
	DECL_SHARED_PTR(PatronSequenceurExtension);
}
namespace Element
{
namespace Sequenceur
{
	struct SequenceurNonDefini : public SVM_Valeur::ExceptionExecution
	{
		SequenceurNonDefini(const SVM_Valeur::PointEntreeExtensionSP& nom)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Sequencer "<< *nom << " does not exist.");
		}
		virtual ~SequenceurNonDefini() {}
	};

	struct NoyauInvalide : public SVM_Valeur::ExceptionExecution
	{
		NoyauInvalide(const SVM_Valeur::PointEntreeExtensionSP& nom)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Sequencer " << *nom << " required execution of an invalid kernel.");
		}
		virtual ~NoyauInvalide() {}
	};

	struct SequenceurExtension : public Sequenceur
	{
		SequenceurExtension(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM_Processus::ProcessusSP& processus);
		virtual ~SequenceurExtension();
		virtual SVM_Noyau::NoyauSP noyau_courant() override;
		virtual bool attache_noyau(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& env_parent, const size_t argc, SVM_Parameter* argv) override;
		virtual bool detache_noyau(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& env_parent, const size_t argc, SVM_Parameter* argv) override;
		virtual void affiche(std::ostream& os) const override;
		virtual void* interne() override { return _sequenceur; }
		private:
		SVM::Machine::MachineWP _machine;
		SVM_Interface::VariablesDefiniesSP _variables;
		SVM_Extension::PatronSequenceurExtensionSP _patron;
		void *_sequenceur;
	};
}
}
}
}
