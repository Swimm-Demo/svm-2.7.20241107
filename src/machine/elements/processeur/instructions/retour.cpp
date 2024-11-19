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

#include <src/machine/elements/processeur/instructions/retour.h>
using namespace SVM::Machine::Element::Processeur::Instruction;

void Retour::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_condition->decoration(decorateur);
	sans_position(decorateur);
}

void Retour::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	if(_nombre_retours>0)
	{
		bool condition = _condition->evaluation(noyau);
		Retour::execution(noyau,condition,_nombre_retours);
	}
	else
	{ // cas special :return 0
		for( ; ; )
		{
			bool condition = _condition->evaluation(noyau);
			if(not condition)
				return;
			noyau->_processeur->retour(*(noyau->_memoire),noyau->_etat._retour_est_arret);
		}
	}
}

void Retour::execution(const SVM_Noyau::NoyauSP& noyau, const bool condition, const size_t nombre_retours)
{
	if(condition)
	{
		for(size_t i=0 ; i<nombre_retours ; ++i)
		{
			noyau->_processeur->retour(*(noyau->_memoire),noyau->_etat._retour_est_arret);
		}
	}
}

void Retour::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	_condition->explication(noyau,explications);
}

