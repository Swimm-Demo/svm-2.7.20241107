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

#include <src/machine/elements/valeur/instruction.h>
#include <src/machine/elements/programme/valeur_programme.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Programme = SVM::Machine::Element::Programme;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Processeur
{
namespace Instruction
{
	struct Drapeau : public SVM_Valeur::Instruction
	{
		Drapeau(const SVM_Valeur::PositionInstruction& position, const SVM_Programme::ValeurSP& drapeau, const bool cascade=false)
		:SVM_Valeur::Instruction(position), _drapeau(drapeau), _cascade(cascade) {}
		virtual ~Drapeau() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		static void execution(const SVM_Noyau::NoyauSP& noyau, const std::string& drapeau, const bool cascade);
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":flag " << (_cascade?"CASCADE ":"") << *_drapeau;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"instruction\">:flag</span> " << (_cascade?"<span class=\"keyword\">CASCADE</span> ":"");
			_drapeau->html(os);
		}
		private:
			SVM_Programme::ValeurSP _drapeau;
			bool _cascade;
	};
}
}
}
}
}
