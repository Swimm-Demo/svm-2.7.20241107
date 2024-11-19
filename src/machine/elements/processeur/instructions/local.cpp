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

#include <src/machine/elements/processeur/instructions/local.h>
using namespace SVM::Machine::Element::Processeur::Instruction;

void Local::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_pointeur->decoration(decorateur);
	sans_position(decorateur);
}

void Local::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Valeur::Pointeur pointeur = _pointeur->evaluation_pointeur(noyau);
	Local::execution(noyau,_cascade,pointeur);
}

void Local::execution(const SVM_Noyau::NoyauSP& noyau, const bool cascade, const SVM_Valeur::Pointeur& pointeur)
{
	std::set<SVM_Valeur::Pointeur> blocs;
	if(cascade)
	{
		blocs = noyau->_memoire->accessible(pointeur);
	}
	else
	{
		blocs = { pointeur };
	}
	std::set<std::string> alias = noyau->_memoire->alias_inclus(blocs);
	noyau->_processeur->memoire_allouee(blocs);
	noyau->_processeur->alias_definis(alias);
}

void Local::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream ptr;
	_pointeur->explique(noyau,ptr);
	explications.push_back(ptr.str());
}
