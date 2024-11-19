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

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Valeur
{
DECL_SHARED_PTR(PositionInstruction);

	struct PositionInstruction
	{
		PositionInstruction(const std::string& nom_code, const size_t ligne_debut, const size_t ligne_fin, const PositionInstructionSP& precedente = PositionInstructionSP())
		:_nom_code(nom_code), _ligne_debut(ligne_debut), _ligne_fin(ligne_fin), _precedente(precedente)
		{
		}
		friend std::ostream& operator<<(std::ostream& f, const PositionInstruction& p)
		{
			f << "@(" << p._nom_code << ", ";
			if(p._ligne_debut==p._ligne_fin)
			{
				f << "line " << p._ligne_debut;
			}
			else
			{
				f << "lines " << p._ligne_debut << "-" << p._ligne_fin;
			}
			if(static_cast<bool>(p._precedente))
			{
				f << ", from " << *(p._precedente);
			}
			f << ")";
			return f;
		}
		private:
			std::string _nom_code;
			size_t _ligne_debut;
			size_t _ligne_fin;
			PositionInstructionSP _precedente;
	};
}
}
}
}
