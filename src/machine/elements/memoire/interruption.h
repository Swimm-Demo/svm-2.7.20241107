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
#include <src/machine/elements/valeur/interruption.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Interface
{
	struct Outils;
}
namespace Extension
{
	DECL_SHARED_PTR(PatronInterruptionExtension);
}
namespace Element
{
namespace Programme
{
	struct Valeur;
	struct InterruptionExterne;
}
namespace Memoire
{

	namespace SVM_Extension = SVM::Machine::Extension;
	DECL_SHARED_PTR(Interruption);

	struct Interruption : public Valeur
	{
		friend struct SVM::Machine::Interface::Outils;
		friend struct Programme::Valeur;
		friend struct Programme::InterruptionExterne;
		Interruption(const SVM_Valeur::Interruption& valeur)
		:_valeur(valeur) {};
		virtual ~Interruption() {};
		virtual ValeurSP copie() const override { return std::make_shared<Interruption>(_valeur); }
		virtual void format(std::ostream& os) const override
		{
			os << _valeur;
		}
		virtual void html(std::ostream& os) const
		{
			_valeur.html(os);
		}
		virtual operator Type () const override
		{
			return Type(Type::TypeInterne::INTERRUPTION);
		}
		virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const override
		{
			const Interruption *d=dynamic_cast<const Interruption*>(&droite);
			return SVM::Machine::Element::Valeur::Comparaison::comparaison_ordre_total(_valeur,d->_valeur,true);
		}
		operator const SVM_Valeur::Interruption& () const { return _valeur; }
		operator SVM_Valeur::Interruption& () { return _valeur; }
		private:
		SVM_Valeur::Interruption _valeur;
		SVM_Extension::PatronInterruptionExtensionSP _patron;
	};
}
}
}
}
