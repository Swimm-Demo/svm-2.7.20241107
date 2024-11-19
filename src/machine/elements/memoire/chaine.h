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
DECL_SHARED_PTR(Chaine);
	struct Chaine : public Valeur
	{
		friend struct Programme::Valeur;
		Chaine(const std::string& valeur)
		:_valeur(valeur) {};
		virtual ~Chaine() {};
		virtual ValeurSP copie() const override { return std::make_shared<Chaine>(_valeur); }
		virtual std::string serialise() const override
		{
			std::ostringstream oss;
			oss << "\"" << SVM::Global::Chaine::echappe(_valeur) << "\"";
			return oss.str();
		}
		virtual void format(std::ostream& os) const override
		{
			os << _valeur;
		}
		virtual void html(std::ostream& os) const
		{
			os << "\"" << SVM::Global::Chaine::echappe_html(SVM::Global::Chaine::echappe(_valeur)) << "\"";
		}
		virtual operator const std::string& () const
		{
			return _valeur;
		}
		virtual operator std::string& ()
		{
			return _valeur;
		}
		virtual operator Type () const override
		{
			return Type(Type::TypeInterne::CHAINE);
		}
		virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const override
		{
			const Chaine *d=dynamic_cast<const Chaine*>(&droite);                                                                                                  
			return SVM::Machine::Element::Valeur::Comparaison::comparaison_ordre_total(_valeur,d->_valeur);
		}       
		private:
		std::string _valeur;
	};
}
}
}
}
