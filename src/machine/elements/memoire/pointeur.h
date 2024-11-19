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
#include <src/machine/elements/valeur/pointeur.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Programme
{
	struct Valeur;
}
namespace Memoire
{
	struct Memoire;
	namespace SVM_Programme = SVM::Machine::Element::Programme;
	DECL_SHARED_PTR(Pointeur);

	struct Pointeur : public Valeur
	{
		friend struct SVM_Programme::Valeur;
		friend struct Memoire;
		Pointeur(const SVM_Valeur::Pointeur& valeur)
		:_valeur(valeur) {}
		Pointeur(const size_t adresse, const size_t taille)
		:_valeur(SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(adresse),taille)) { }
		virtual ValeurSP copie() const override { return std::make_shared<Pointeur>(_valeur); }
		virtual ~Pointeur() {}
		virtual void format(std::ostream& os) const override
		{
			os << _valeur;
		}
		virtual operator Type () const override
		{
			return Type(Type::TypeInterne::POINTEUR);
		}
		virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const override
		{
			const Pointeur *d=dynamic_cast<const Pointeur*>(&droite);
			return SVM::Machine::Element::Valeur::Comparaison::comparaison_ordre_total(_valeur,d->_valeur,true);
		}       
		operator const SVM_Valeur::Pointeur& () const { return _valeur; }
		operator SVM_Valeur::Pointeur& () { return _valeur; }
		void decalage(const PointeurSP& initial, const SVM_Valeur::Pointeur& origine, const SVM_Valeur::Pointeur& destination)
		{
			if(origine.inclus(initial->_valeur))
			{
				_valeur.decalage(destination.adresse()-origine.adresse());
			}
		}
		private:
		SVM_Valeur::Pointeur _valeur;
	};
}
}
}
}
