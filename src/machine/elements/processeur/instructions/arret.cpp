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

#include <src/machine/elements/processeur/instructions/arret.h>
#include <src/machine/elements/valeur/arret.h>
using namespace SVM::Machine::Element::Processeur::Instruction;

void ArretProcesseur::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_condition->decoration(decorateur);
	sans_position(decorateur);
}

void ArretProcesseur::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	bool condition = _condition->evaluation(noyau);
	ArretProcesseur::execution(noyau,condition);
}

void ArretProcesseur::execution(const SVM_Noyau::NoyauSP& noyau, const bool condition)
{
	if(condition)
		throw SVM_Valeur::Arret();
}

void ArretProcesseur::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	_condition->explication(noyau,explications);
}

void ArretMachine::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_code_retour->decoration(decorateur);
	_condition->decoration(decorateur);
	sans_position(decorateur);
}

void ArretMachine::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	long int code_retour = _code_retour->evaluation_entier(noyau);
	bool condition = _condition->evaluation(noyau);
	if(code_retour<0)
	{
		throw CodeRetourInvalide(code_retour);
	}
	ArretMachine::execution(noyau,condition,static_cast<size_t>(code_retour));
}

void ArretMachine::execution(const SVM_Noyau::NoyauSP& noyau, const bool condition, const size_t code_retour)
{
	if(condition)
		throw SVM_Valeur::Arret(code_retour);
}

void ArretMachine::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream cod;
	_code_retour->explique(noyau,cod);
	explications.push_back(cod.str());
	_condition->explication(noyau,explications);
}

