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

#include <src/machine/elements/processeur/instructions/decalage.h>
using namespace SVM::Machine::Element::Processeur::Instruction;

void Decalage::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	if(static_cast<bool>(_increment))
	{
		_increment->decoration(decorateur);
	}
	_adresse->decoration(decorateur);
	sans_position(decorateur);
}

void Decalage::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	
	long int increment = 1;
	if(static_cast<bool>(_increment))
	{
		increment = _increment->evaluation_entier(noyau);
	}
	SVM_Valeur::AdresseMemoire adresse = _adresse->evaluation(noyau);
	Decalage::execution(noyau,increment,adresse);
}

void Decalage::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	if(static_cast<bool>(_increment))
	{
		std::ostringstream inc;
		_increment->explique(noyau,inc);
		explications.push_back(inc.str());
	}
	std::ostringstream adr;
	_adresse->explique(noyau,adr);
	explications.push_back(adr.str());
}

void Decalage::execution(const SVM_Noyau::NoyauSP& noyau, const long int increment, const SVM_Valeur::AdresseMemoire& adresse)
{
	noyau->_memoire->decalage(adresse,increment);
}
