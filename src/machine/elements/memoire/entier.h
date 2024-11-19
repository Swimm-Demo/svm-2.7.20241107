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
DECL_SHARED_PTR(Entier);
	struct Entier : public Valeur
	{
		friend struct Programme::Valeur;
		Entier(const long long int valeur)
		:_valeur(valeur) {};
		virtual ~Entier() {};
		virtual ValeurSP copie() const override { return std::make_shared<Entier>(_valeur); }
		virtual	ValeurSP operator+(const long long int increment) override
		{
			return std::make_shared<Entier>(_valeur+increment);
		}
		virtual void operator+=(const long long int increment) override
		{
			_valeur += increment;
		}
		virtual void format(std::ostream& os) const override
		{
			os << _valeur;
		}
		virtual operator Type () const override
		{
			return Type(Type::TypeInterne::ENTIER);
		}
		virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const override
		{
			const Entier *d=dynamic_cast<const Entier*>(&droite);
			return SVM::Machine::Element::Valeur::Comparaison::comparaison_ordre_total(_valeur,d->_valeur);
		}       
		operator long long int () const { return _valeur; }
		operator long long int& () { return _valeur; }
		private:
		long long int _valeur;
	};
}
}
}
}
