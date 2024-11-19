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
#include <src/machine/elements/valeur/code.h>

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
DECL_SHARED_PTR(Bibliotheque);
	struct Bibliotheque : public Valeur
	{
		friend struct SVM::Machine::Interface::Outils;
		friend struct Programme::Valeur;
		Bibliotheque(const SVM_Valeur::CodeSP& valeur)
		:_valeur(valeur) {};
		virtual ~Bibliotheque() {};
		virtual ValeurSP copie() const override { return std::make_shared<Bibliotheque>(_valeur); }
		virtual void format(std::ostream& os) const override
		{
			os << "<" << _valeur->nom() << ">";
		}
		virtual operator Type () const override
		{
			return Type(Type::TypeInterne::BIBLIOTHEQUE);
		}
		virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const override
		{
			const Bibliotheque *d=dynamic_cast<const Bibliotheque*>(&droite);
			return SVM::Machine::Element::Valeur::Comparaison::comparaison_ordre_total(_valeur,d->_valeur,true); // oui, ca doit etre le meme code "physiquement"
		}
		operator const SVM_Valeur::CodeSP& () const { return _valeur; }
		operator SVM_Valeur::CodeSP& () { return _valeur; }
		private:
		SVM_Valeur::CodeSP _valeur;
	};
}
}
}
}
