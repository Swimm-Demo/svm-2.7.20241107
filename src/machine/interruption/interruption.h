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

#include <src/machine/elements/valeur/interruption.h>
#include <src/machine/elements/valeur/exception.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Systeme
{
	DECL_SHARED_PTR(Systeme);
	DECL_WEAK_PTR(Systeme);
}
}
namespace Interruption
{
	struct InterruptionNonLance : public SVM_Valeur::ExceptionExecution
	{
		InterruptionNonLance(const std::string& details)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Hardware interruption not started: " << details);
		}
		virtual ~InterruptionNonLance() {}
	};

	DECL_SHARED_PTR(CompteurInterruption);

	struct CompteurInterruption
	{
		CompteurInterruption()
		:_compteur(0) {}
		void ecriture() { ++_compteur; }
		bool lecture()
		{
			if(_compteur.load(std::memory_order_relaxed)==0)
				return false;
			--_compteur;
			return true;
		}
		size_t compteur() const
		{
			return _compteur.load(std::memory_order_relaxed);
		}
		private:
			std::atomic<size_t> _compteur;
	};

	DECL_SHARED_PTR(Interruption);
	DECL_WEAK_PTR(Interruption);

	struct Interruption : public std::enable_shared_from_this<Interruption>
	{
		friend struct SVM::Machine::Element::Systeme::Systeme;
		~Interruption();
		static void bloque_signaux();
		static InterruptionSP creation_interruptions();
		void ajoute(const SVM_Valeur::Interruption::InterruptionInterne interruption, const CompteurInterruptionSP& compteur);
		void enleve(const SVM_Valeur::Interruption::InterruptionInterne interruption, const CompteurInterruptionSP& compteur);
		void interruptions_materielles(const SVM::Machine::Element::Systeme::SystemeSP& systeme,int signal_recu);
		private:
			mutable std::mutex _protection;
			std::thread _effecteur;
			std::map<SVM_Valeur::Interruption::InterruptionInterne, std::set< CompteurInterruptionSP > > _interruptions;
			SVM::Machine::Element::Systeme::SystemeWP _systeme;
	};
}
}
}
