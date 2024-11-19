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
#include <src/machine/elements/valeur/code.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Debugueur
{
	DECL_SHARED_PTR(Debugueur);
}
namespace Element
{
namespace Programme
{
	struct Analyseur
	{
		static SVM_Valeur::CodeSP analyse_chaine(const std::string& nom, const std::string& source, const SVM::Machine::MachineSP& machine, const SVM_Valeur::PositionInstructionSP& position = SVM_Valeur::PositionInstructionSP());
		static SVM_Valeur::CodeSP analyse_fichier(const std::string& nom, const std::string& source, const SVM::Machine::MachineSP& machine, const SVM_Valeur::PositionInstructionSP& position = SVM_Valeur::PositionInstructionSP());
	};
}
}
}
}
