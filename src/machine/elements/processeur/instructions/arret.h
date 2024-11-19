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
#include <src/machine/elements/programme/condition.h>
#include <src/machine/elements/noyau/noyau.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Programme = SVM::Machine::Element::Programme;
namespace SVM_Noyau = SVM::Machine::Element::Noyau;

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
	
	struct ArretProcesseur : public SVM_Valeur::Instruction
	{
		ArretProcesseur(const SVM_Valeur::PositionInstruction& position, const SVM_Programme::ConditionSP& condition)
		:SVM_Valeur::Instruction(position), _condition(condition) {}
		virtual ~ArretProcesseur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		static void execution(const SVM_Noyau::NoyauSP& noyau, const bool condition);
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":shutdown" << *_condition;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"instruction\">:shutdown</span>";
			_condition->html(os);
		}
		private:
			SVM_Programme::ConditionSP _condition;
	};

	struct CodeRetourInvalide : public SVM_Valeur::ExceptionExecution
	{
		CodeRetourInvalide(const long int code)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR)
		{
			DETAILS_EXCEPTION(code << " is not a valid shutdown code.");
		}
		virtual ~CodeRetourInvalide() {}
	};

	struct ArretMachine : public SVM_Valeur::Instruction
	{
		ArretMachine(const SVM_Valeur::PositionInstruction& position, const SVM_Programme::ValeurSP& code_retour, const SVM_Programme::ConditionSP& condition)
		:SVM_Valeur::Instruction(position,false,true/*systeme*/), _code_retour(code_retour), _condition(condition) {}
		virtual ~ArretMachine() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		static void execution(const SVM_Noyau::NoyauSP& noyau, const bool condition, const size_t code_retour);
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":shutdown " << *_code_retour << *_condition;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"instruction\">:shutdown</span> ";
			_code_retour->html(os);
			_condition->html(os);
		}
		private:
			SVM_Programme::ValeurSP _code_retour;
			SVM_Programme::ConditionSP _condition;
	};
}
}
}
}
}
