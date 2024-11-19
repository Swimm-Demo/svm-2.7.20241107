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
	struct Local : public SVM_Valeur::Instruction
	{
		Local(const SVM_Valeur::PositionInstruction& position, const bool cascade, const SVM_Programme::ValeurSP& pointeur)
		:SVM_Valeur::Instruction(position), _cascade(cascade), _pointeur(pointeur) {}
		virtual ~Local() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		static void execution(const SVM_Noyau::NoyauSP& noyau, const bool cascade, const SVM_Valeur::Pointeur& pointeur);
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":local";
			if(_cascade)
			{
				os << " CASCADE";
			}
			os << " " << *_pointeur;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"instruction\">:local</span>";
			if(_cascade)
			{
				os << " <span class=\"keyword\">CASCADE</span>";
			}
			os << " ";
			_pointeur->html(os);
		}
		private:
			bool _cascade;
			SVM_Programme::ValeurSP _pointeur;
	};

}
}
}
}
}
