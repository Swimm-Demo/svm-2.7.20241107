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

#include <src/machine/interruption/interruption.h>
#include <src/machine/debugueur/debugueur.h>
#include <src/machine/performance/performance.h>

namespace SVM
{
namespace Machine
{
namespace Extension
{
	DECL_SHARED_PTR(Gestionnaire);
}
namespace Element
{
namespace Noyau
{
	struct Noyau;
}
namespace Sequenceur
{
	struct SequenceurExtension;
}
namespace Base
{
	DECL_SHARED_PTR(Base);
	DECL_WEAK_PTR(Base);
	struct Base
	{
		friend struct SVM::Machine::Element::Noyau::Noyau;
		friend struct SVM::Machine::Element::Sequenceur::SequenceurExtension;
		Base(const SVM::Machine::Extension::GestionnaireSP& gestionnaire_extensions, const SVM::Machine::Debugueur::DebugueurSP& debugueur, const SVM::Machine::Performance::PerformanceSP& performance)
		:_gestionnaire_extensions(gestionnaire_extensions), _debugueur(debugueur), _performance(performance)
		{
			ASSERT(static_cast<bool>(gestionnaire_extensions),"Gestionnaire interruptions vide");
			_gestionnaire_interruptions_materielles = SVM::Machine::Interruption::Interruption::creation_interruptions();
		}
		Base(const Base&) = delete;
		public:
		SVM::Machine::Extension::GestionnaireSP _gestionnaire_extensions;
		SVM::Machine::Interruption::InterruptionSP _gestionnaire_interruptions_materielles;
		SVM::Machine::Debugueur::DebugueurSP _debugueur;
		SVM::Machine::Performance::PerformanceSP _performance;
	};
}
}
}
}
