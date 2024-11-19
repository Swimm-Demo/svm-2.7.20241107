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
#include <src/machine/elements/programme/parametre.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Programme = SVM::Machine::Element::Programme;

namespace SVM
{
namespace Machine
{
namespace Extension
{
	DECL_SHARED_PTR(PatronInstructionExtension);
}
namespace Element
{
namespace Processeur
{
namespace Instruction
{
	namespace SVM_Extension = SVM::Machine::Extension;
	struct Extension : public SVM_Valeur::Instruction
	{
		Extension(const SVM_Valeur::PositionInstruction& position, const SVM_Programme::ValeurSP& nom_instruction, const std::vector<SVM_Programme::ParametreSP>& parametres)
		:SVM_Valeur::Instruction(position), _nom_instruction(nom_instruction), _parametres(parametres) {}
		virtual ~Extension() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau) const override;
		static SVM_Memoire::ValeurSP execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau, const SVM_Valeur::PointEntreeExtension& nom_instruction, const SVM_Extension::PatronInstructionExtensionSP instruction, const std::vector<SVM_Valeur::ParametreSP>& parametres, const SVM_Valeur::PositionInstructionSP& position, const bool attente, const bool systeme);
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":" << *_nom_instruction;
			for(auto& p:_parametres)
			{
				os << " " << *p;
			}
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"instruction\">:";
			_nom_instruction->html(os);
			os << "</span>";
			for(auto& p:_parametres)
			{
				os << " ";
				p->html(os);
			}
		}
		private:
			SVM_Programme::ValeurSP _nom_instruction;
			std::vector<SVM_Programme::ParametreSP> _parametres;
			SVM_Extension::PatronInstructionExtensionSP _patron;
	};

	struct InstructionInexistante : public SVM_Valeur::ExceptionExecution
	{
		InstructionInexistante(const SVM_Valeur::PointEntreeExtension& nom)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR)
		{
			DETAILS_EXCEPTION("Instruction " << nom << " is not defined.");
		}
	};

	struct InstructionRemplacement : public SVM_Valeur::ExceptionExecution
	{
		InstructionRemplacement(const SVM_Valeur::PointEntreeExtension& nom)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR)
		{
			DETAILS_EXCEPTION("Instruction " << nom << " can be used only in an override.");
		}
	};
}
}
}
}
}
