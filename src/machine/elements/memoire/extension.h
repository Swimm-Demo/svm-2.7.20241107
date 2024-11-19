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
#include <src/machine/elements/memoire/valeur.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
	DECL_SHARED_PTR(Machine);
	DECL_WEAK_PTR(Machine);
namespace Interface
{
	struct Outils;
}
namespace Extension
{
	DECL_SHARED_PTR(PatronTypeExtension);
}
namespace Element
{
namespace Memoire
{
	namespace SVM_Extension = SVM::Machine::Extension;
	DECL_SHARED_PTR(Extension);
	struct Extension : public Valeur
	{
		friend struct SVM::Machine::Interface::Outils;
		Extension(const SVM::Machine::MachineSP& machine, const SVM_Valeur::PointEntreeExtension& type, const SVM_Extension::PatronTypeExtensionSP& patron, void * valeur)
		:_type(type), _patron(patron), _valeur(valeur), _machine(machine) {};
		virtual ~Extension();
		void destruction();
		virtual ValeurSP copie() const override;
		void* copie_interne() const;
		virtual std::string serialise() const override
		{
			std::ostringstream oss;
			format(oss);
			return SVM::Global::Chaine::echappe(oss.str());
		}
		virtual void format(std::ostream& os) const override;
		virtual operator Type () const override
		{
			return Type(_type);
		}
		virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const override;
		private:
		SVM_Valeur::PointEntreeExtension _type;
		SVM_Extension::PatronTypeExtensionSP _patron;	
		void *_valeur;
		SVM::Machine::MachineWP _machine;
	};
}
}
}
}
