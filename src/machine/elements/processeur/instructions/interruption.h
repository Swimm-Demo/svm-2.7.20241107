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
#include <src/machine/elements/programme/cible.h>
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
	struct Interruption : public SVM_Valeur::Instruction
	{
		Interruption(const SVM_Valeur::PositionInstruction& position, const bool globale, const bool cascade, const SVM_Programme::ValeurSP& interruption, const SVM_Programme::CibleSP& cible)
		:SVM_Valeur::Instruction(position), _globale(globale), _cascade(cascade), _interruption(interruption), _cible(cible) {}
		Interruption(const SVM_Valeur::PositionInstruction& position, const bool globale, const bool cascade, const SVM_Programme::ValeurSP& interruption)
		:SVM_Valeur::Instruction(position), _globale(globale), _cascade(cascade), _interruption(interruption) {}
		virtual ~Interruption() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		static void execution(const SVM_Noyau::NoyauSP& noyau, const bool globale, const bool cascade, const SVM_Valeur::Interruption& interruption, const SVM_Valeur::AdresseInstruction& adresse);
		static void execution(const SVM_Noyau::NoyauSP& noyau, const bool globale, const bool cascade, const SVM_Valeur::Interruption& interruption);
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":interruption ";
			if(_globale)
			{
				os << "GLOBAL ";
			}
			if(_cascade)
			{
				os << "CASCADE ";
			}
			os << *_interruption;
			if(_cible)
			{
				os << " " << *_cible;
			}
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"instruction\">:interruption</span> ";
			if(_globale)
			{
				os << "<span class=\"keyword\">GLOBAL</span> ";
			}
			if(_cascade)
			{
				os << "<span class=\"keyword\">CASCADE</span> ";
			}
			_interruption->html(os);
			if(_cible)
			{
				os << " ";
				_cible->html(os);
			}
		}
		private:
			bool _globale;
			bool _cascade;
			SVM_Programme::ValeurSP _interruption;
			SVM_Programme::CibleSP _cible;
	};
}
}
}
}
}
