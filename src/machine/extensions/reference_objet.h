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
#include <src/machine/elements/valeur/point_entree_extension.h>
namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Extension
{
	struct ReferenceObjet
	{
		enum class Type
		{
			TYPE,
			STRUCTURE,
			INSTRUCTION,
			FONCTION,
			INTERRUPTION,
			ORDONNANCEUR,
			SEQUENCEUR,
			OPTION,
			ARGUMENT,
			ARGUMENTS
		};
		ReferenceObjet(const Type& type, const SVM_Valeur::PointEntreeExtension& nom, const size_t ligne = 0)
		:_type(type), _nom(nom), _ligne(ligne) {}

		bool operator<(const ReferenceObjet& o) const
		{
			if(_type!=o._type)
			{
				return _type<o._type;
			}
			return _nom<o._nom;
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const ReferenceObjet& r)
		{
			f << "(line " << r._ligne << ") " << ReferenceObjet::texte(r._type) << " " << r._nom;
			return f;
		}
		static std::string texte(const Type type)
		{
			switch(type)
			{
				case Type::TYPE: return "TYPE"; break;
				case Type::STRUCTURE: return "STRUCT"; break;
				case Type::INSTRUCTION: return "INSTRUCTION"; break;
				case Type::FONCTION: return "FUNCTION"; break;
				case Type::INTERRUPTION: return "INTERRUPTION"; break;
				case Type::ORDONNANCEUR: return "SCHEDULER"; break;
				case Type::SEQUENCEUR: return "SEQUENCER"; break;
				case Type::OPTION: return "OPTION"; break;
				case Type::ARGUMENT: return "ARGUMENT"; break;
				case Type::ARGUMENTS: return "ARGUMENTS"; break;
			}
			return "<unknown>";
		}
		Type _type;
		SVM_Valeur::PointEntreeExtension _nom;
		size_t _ligne;
	};
}
}
}
