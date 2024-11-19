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
#include <src/machine/elements/programme/valeur_programme.h>
#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/valeur/decorateur.h>
#include <src/machine/elements/valeur/arbre_substitution.h>

namespace SVM_Programme = SVM::Machine::Element::Programme;
namespace SVM_Noyau = SVM::Machine::Element::Noyau;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Valeur
{
	DECL_SHARED_PTR(ExpressionCondition);
	struct ExpressionCondition
	{
		virtual ~ExpressionCondition() {};
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) = 0;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& ce) const = 0;
		virtual bool explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const = 0;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const = 0;
		virtual void format(std::ostream& os) const = 0;
		virtual void html(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const ExpressionCondition& ec)
		{
			ec.format(f);
			return f;
		}
	};
}
}
}
}
