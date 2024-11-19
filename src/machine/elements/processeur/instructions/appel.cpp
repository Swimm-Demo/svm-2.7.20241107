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

#include <src/machine/elements/processeur/instructions/appel.h>

using namespace SVM::Machine::Element::Processeur::Instruction;

void Appel::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_cible->decoration(decorateur);
	_parametres->decoration(decorateur);
	_condition->decoration(decorateur);
	sans_position(decorateur);
}

void Appel::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Valeur::AdresseInstruction cible = _cible->evaluation(noyau);
	SVM_Valeur::Pointeur parametres = _parametres->evaluation_pointeur(noyau);
	bool condition = _condition->evaluation(noyau);
	Appel::execution(noyau, condition, cible, parametres);
}

void Appel::execution(const SVM_Noyau::NoyauSP& noyau, const bool condition, const SVM_Valeur::AdresseInstruction& cible, const SVM_Valeur::Pointeur& parametres)
{
	if(condition)
	{
		noyau->_processeur->appel(cible,parametres);
	}
}

void Appel::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	_cible->explication(noyau,explications);
	std::ostringstream par;
	_parametres->explique(noyau,par);
	explications.push_back(par.str());
	_condition->explication(noyau,explications);
}
