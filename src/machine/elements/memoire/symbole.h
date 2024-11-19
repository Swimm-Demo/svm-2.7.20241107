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
#include <src/machine/elements/valeur/adresse_instruction.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Interface
{
	struct Outils;
}
namespace Element
{
namespace Programme
{
	struct Valeur;
}
namespace Memoire
{
DECL_SHARED_PTR(Symbole);
	struct Symbole : public Valeur // nom reel : cible globale
	{
		friend struct SVM::Machine::Interface::Outils;
		friend struct Programme::Valeur;
		Symbole(const SVM_Valeur::AdresseInstruction& valeur)
		:_valeur(valeur) {};
		virtual ~Symbole() {};
		virtual ValeurSP copie() const override { return std::make_shared<Symbole>(_valeur); }
		virtual void format(std::ostream& os) const override
		{
			os << _valeur;
		}
		virtual operator Type () const override
		{
			return Type(Type::TypeInterne::SYMBOLE);
		}
		virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const override
		{
			const Symbole *d=dynamic_cast<const Symbole*>(&droite);
			return SVM::Machine::Element::Valeur::Comparaison::comparaison_ordre_total(_valeur,d->_valeur,true);
		}       
		operator const SVM_Valeur::AdresseInstruction& () const { return _valeur; }
		operator SVM_Valeur::AdresseInstruction& () { return _valeur; }
		private:
		SVM_Valeur::AdresseInstruction _valeur;
	};
}
}
}
}
