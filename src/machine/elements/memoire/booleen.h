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
DECL_SHARED_PTR(Booleen);
	struct Booleen : public Valeur
	{
		friend struct Programme::Valeur;
		Booleen(const bool valeur)
		:_valeur(valeur) {};
		virtual ~Booleen() {};
		virtual ValeurSP copie() const override { return std::make_shared<Booleen>(_valeur); }
		virtual void format(std::ostream& os) const override
		{
			os << ( _valeur?"TRUE":"FALSE");
		}
		virtual operator Type () const override
		{
			return Type(Type::TypeInterne::BOOLEEN);
		}
		virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const override
		{
			const Booleen *d=dynamic_cast<const Booleen*>(&droite);                                                                                                  
			SVM::Machine::Element::Valeur::Comparaison c;
			if(_valeur==d->_valeur)
			{
				c._egal = true;
			}
			if(not _valeur and d->_valeur)
			{
				c._inferieur = true;
			}
			if(_valeur and not d->_valeur)
			{
				c._superieur = true;
			}
			c._ordre = true;
			c._total = true;
			c._faible = true;
			return c;
		}       
		operator bool () const { return _valeur; }
		operator bool& () { return _valeur; }
		private:
		bool _valeur;
	};
}
}
}
}
