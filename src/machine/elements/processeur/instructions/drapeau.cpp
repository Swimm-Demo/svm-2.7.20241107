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

#include <src/machine/elements/processeur/instructions/drapeau.h>
using namespace SVM::Machine::Element::Processeur::Instruction;

void Drapeau::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_drapeau->decoration(decorateur);
	sans_position(decorateur);
}

void Drapeau::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	std::string drapeau = _drapeau->evaluation_chaine(noyau);
	Drapeau::execution(noyau,drapeau,_cascade);
}

void Drapeau::execution(const SVM_Noyau::NoyauSP& noyau, const std::string& drapeau, const bool cascade)
{
	noyau->_processeur->ajoute_drapeau(drapeau,cascade);
}

void Drapeau::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream dra;
	_drapeau->explique(noyau,dra);
	explications.push_back(dra.str());
}
