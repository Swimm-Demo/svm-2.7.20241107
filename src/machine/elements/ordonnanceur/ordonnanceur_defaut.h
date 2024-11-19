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
#include <src/machine/elements/systeme/systeme.h>

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Ordonnanceur
{
	struct UnSeulProcessusAdmis : public SVM_Valeur::ExceptionExecution
	{
		UnSeulProcessusAdmis()
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Default scheduler can not handle more than one process.");
		}
		virtual ~UnSeulProcessusAdmis() {}
	};

	DECL_SHARED_PTR(OrdonnanceurDefaut);
	struct OrdonnanceurDefaut : public Ordonnanceur
	{
		static OrdonnanceurSP creation_ordonnanceur(const SVM::Machine::Element::Systeme::SystemeSP& systeme);
		OrdonnanceurDefaut(const SVM::Machine::Element::Systeme::SystemeSP& systeme)
		:Ordonnanceur(SVM_Valeur::PointEntreeExtensionSP(),systeme) {}
		virtual ~OrdonnanceurDefaut() {}
		virtual size_t ordonnancement(const SVM_Processus::ProcessusSP& processus, const SVM_Processus::Etat::Execution etat_processus) override
		{
			if(etat_processus==SVM_Processus::Etat::Execution::SUSPENDU)
			{
				processus->execution_demandee(_adresse);
			}
			return 0;
		}
		virtual size_t notification(const SVM_Synchronisation::AdresseSP& source, const SVM_Notification_Type type, const size_t parametre) override
		{
			return 0;
		}
		virtual bool attache_processus(const SVM_Processus::ProcessusSP& processus, const size_t parametre) override
		{
			if(not processus)
				return false;
			if(not _processus.expired())
			{
				return false;
			}
			_processus = processus;
			return true;
		}
		virtual bool detache_processus(const SVM_Processus::ProcessusSP& processus, const size_t parametre) override
		{
			if(not processus)
				return false;
			auto p = _processus.lock();
			if(p==processus)
			{
				_processus.reset();
				return true;
			}
			return false;
		}
		virtual void affiche(std::ostream& os) const override
		{
			auto p = _processus.lock();
			if(static_cast<bool>(p))
			{
				p->nom_flux(os);
				os << ": " << p->_etat.etat() << std::endl;
			}
			else
			{
				os << "No process." << std::endl;
			}
		}
		virtual void *interne() override { return nullptr; }
		private:
			SVM_Processus::ProcessusWP _processus;
	};
}
}
}
}
