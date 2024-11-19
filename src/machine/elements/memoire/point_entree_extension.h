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
#include <src/machine/elements/valeur/point_entree_extension.h>

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
	struct InterruptionExterne;
	struct TypeExtension;
	struct Valeur;
}
namespace Processeur
{
namespace Instruction
{
	struct Extension;
}
}
namespace Memoire
{
	namespace SVM_Instruction = SVM::Machine::Element::Processeur::Instruction;
DECL_SHARED_PTR(PointEntreeExtension);
	struct PointEntreeExtension : public Valeur
	{
		friend struct SVM::Machine::Interface::Outils;
		friend struct Programme::InterruptionExterne;
		friend struct Programme::TypeExtension;
		friend struct Programme::Valeur;
		friend struct SVM_Instruction::Extension;
		PointEntreeExtension(const SVM_Valeur::PointEntreeExtension& valeur)
		:_valeur(valeur) {};
		virtual ~PointEntreeExtension() {};
		virtual ValeurSP copie() const override { return std::make_shared<PointEntreeExtension>(_valeur); }
		virtual void format(std::ostream& os) const override
		{
			os << _valeur;
		}
		void html(std::ostream& os) const
		{
			_valeur.html(os);
		}
		virtual operator Type () const override
		{
			return Type(Type::TypeInterne::POINT_ENTREE_EXTENSION);
		}
		virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const override
		{
			const PointEntreeExtension *d=dynamic_cast<const PointEntreeExtension*>(&droite);
			return SVM::Machine::Element::Valeur::Comparaison::comparaison_ordre_total(_valeur,d->_valeur,true);
		}       
		operator const SVM_Valeur::PointEntreeExtension& () const { return _valeur; }
		operator SVM_Valeur::PointEntreeExtension& () { return _valeur; }
		private:
		SVM_Valeur::PointEntreeExtension _valeur;
	};
}
}
}
}
