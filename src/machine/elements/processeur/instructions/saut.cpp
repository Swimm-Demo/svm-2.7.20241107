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

#include <src/machine/elements/processeur/instructions/saut.h>
using namespace SVM::Machine::Element::Processeur::Instruction;

void Saut::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_cible->decoration(decorateur);
	_condition->decoration(decorateur);
	sans_position(decorateur);
}

void Saut::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Valeur::AdresseInstruction cible = _cible->evaluation(noyau);
	bool condition = _condition->evaluation(noyau);
	Saut::execution(noyau,condition,cible);
}

void Saut::execution(const SVM_Noyau::NoyauSP& noyau, const bool condition, const SVM_Valeur::AdresseInstruction& cible)
{
	if(condition)
	{
		noyau->_processeur->saut(cible);
	}
}

void Saut::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	_cible->explication(noyau,explications);
	_condition->explication(noyau,explications);
}
