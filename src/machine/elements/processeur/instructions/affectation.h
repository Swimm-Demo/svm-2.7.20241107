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
#include <src/machine/elements/programme/adresse_programme.h>
#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/memoire/valeur.h>

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
	struct AffectationValeur : public SVM_Valeur::Instruction
	{
		AffectationValeur(const SVM_Valeur::PositionInstruction& position, const SVM_Programme::ValeurSP& valeur, const SVM_Programme::AdresseSP& adresse)
		:SVM_Valeur::Instruction(position), _valeur(valeur), _adresse(adresse) {}
		virtual ~AffectationValeur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		static void execution(const SVM_Noyau::NoyauSP& noyau, const SVM_Memoire::ValeurSP& valeur, const SVM_Valeur::AdresseMemoire& adresse);
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_valeur << " -> " << *_adresse;
		}
		virtual void html(std::ostream& os) const override
		{
			_valeur->html(os);
			os << " -> ";
			_adresse->html(os);
		}
		private:
			SVM_Programme::ValeurSP _valeur;
			SVM_Programme::AdresseSP _adresse;
	};

	struct AffectationBloc : public SVM_Valeur::Instruction
	{
		AffectationBloc(const SVM_Valeur::PositionInstruction& position, const std::vector<SVM_Programme::ValeurSP>& valeurs, const SVM_Programme::ValeurSP& pointeur)
		:SVM_Valeur::Instruction(position), _valeurs(valeurs), _pointeur(pointeur) {}
		virtual ~AffectationBloc() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		static void execution(const SVM_Noyau::NoyauSP& noyau, const std::vector<SVM_Memoire::ValeurSP>& valeurs, const SVM_Valeur::Pointeur& pointeur);
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << "[";
			bool virgule=false;
			for(auto& v:_valeurs)
			{
				os << " ";
				if(virgule)
					os << ", ";
				virgule=true;
				if(static_cast<bool>(v))
				{
					os << *v;
				}
			}
			os << " ] -> " << *_pointeur;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "[";
			bool virgule=false;
			for(auto& v:_valeurs)
			{
				os << " ";
				if(virgule)
					os << ", ";
				virgule=true;
				if(static_cast<bool>(v))
				{
					v->html(os);
				}
			}
			os << " ] -> ";
			_pointeur->html(os);
		}
		private:
			std::vector<SVM_Programme::ValeurSP> _valeurs;
			SVM_Programme::ValeurSP _pointeur;
	};

	struct AffectationInstruction : public SVM_Valeur::Instruction
	{
		AffectationInstruction(const SVM_Valeur::PositionInstruction& position, const SVM_Valeur::InstructionSP& instruction, const SVM_Programme::AdresseSP& adresse)
		:SVM_Valeur::Instruction(position), _instruction(instruction), _adresse(adresse) {}
		virtual ~AffectationInstruction() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_instruction << " -> " << *_adresse;
		}
		virtual void html(std::ostream& os) const override
		{
			_instruction->html(os);
			os << " -> ";
			_adresse->html(os);
		}
		private:
			SVM_Valeur::InstructionSP _instruction;
			SVM_Programme::AdresseSP _adresse;
	};
}
}
}
}
}
