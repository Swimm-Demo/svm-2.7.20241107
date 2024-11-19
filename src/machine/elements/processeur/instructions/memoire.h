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
#include <src/machine/elements/programme/type_programme.h>
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

	struct Memoire : public SVM_Valeur::Instruction
	{
		Memoire(const SVM_Valeur::PositionInstruction& position, const bool garder, const std::vector<SVM_Programme::DescriptionBlocAliasSP>& description_bloc)
		:SVM_Valeur::Instruction(position), _garder(garder),_description_bloc(description_bloc) {}
		virtual ~Memoire() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau) const override;
		static SVM_Memoire::ValeurSP execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau, const bool garder, const SVM_Memoire::DescriptionBloc& description_bloc);
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":memory" << (_garder?" GLOBAL":"");
			bool virgule=false;
			for(auto& d:_description_bloc)
			{
				if(virgule)
					os << " ,";
				virgule=true;
				os << " " << *d;
			}
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"instruction\">:memory</span>" << (_garder?" <span class=\"keyword\">GLOBAL</span>":"");
			bool virgule=false;
			for(auto& d:_description_bloc)
			{
				if(virgule)
					os << " ,";
				virgule=true;
				os << " ";
				d->html(os);
			}
		}
		private:
		bool _garder;
		std::vector<SVM_Programme::DescriptionBlocAliasSP> _description_bloc;
	};

}
}
}
}
}
