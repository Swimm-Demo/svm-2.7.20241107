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

#include <src/machine/elements/processeur/instructions/memoire.h>
using namespace SVM::Machine::Element::Processeur::Instruction;

void Memoire::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	for(auto& db:_description_bloc)
	{
		db->decoration(decorateur);
	}
	sans_position(decorateur);
}

SVM_Memoire::ValeurSP Memoire::execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Memoire::DescriptionBloc description_bloc;
	for(auto& db: _description_bloc)
	{
		db->evaluation(noyau,description_bloc);
	}
	return Memoire::execution_avec_resultat(noyau,_garder,description_bloc);
}

SVM_Memoire::ValeurSP Memoire::execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau, const bool garder, const SVM_Memoire::DescriptionBloc& description_bloc)
{
	noyau->_memoire->verification_alias(description_bloc);
	SVM_Valeur::Pointeur bloc = noyau->_memoire->allocation(description_bloc);
	noyau->_memoire->definition_alias(description_bloc,bloc);
	if(garder)
	{
		noyau->_processeur->retire_memoire_allouee({bloc});
		noyau->_processeur->retire_alias_definis(description_bloc.alias());
	}
	else
	{
		noyau->_processeur->memoire_allouee({bloc});
		noyau->_processeur->alias_definis(description_bloc.alias());
	}
	return std::make_shared<SVM_Memoire::Pointeur>(bloc);
}

void Memoire::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	for(auto& db: _description_bloc)
	{
		db->explication(noyau,explications);
	}
}
