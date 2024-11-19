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
#include <src/machine/elements/valeur/position_instruction.h>


namespace SVM
{
namespace Machine
{
	DECL_SHARED_PTR(Machine);
namespace Element
{
namespace Valeur
{
DECL_SHARED_PTR(Code);
namespace SVM_Extension = SVM::Machine::Extension;

	struct Decorateur
	{
		Decorateur(const CodeSP& code, const SVM::Machine::MachineSP& machine, const bool remplacement=false)
		:_code(code), _machine(machine),_remplacement(remplacement) {}
		const CodeSP _code;
		const SVM::Machine::MachineSP _machine;
		const bool _remplacement;
		SVM_Valeur::PositionInstructionSP _position;
	};
}
}
}
}
