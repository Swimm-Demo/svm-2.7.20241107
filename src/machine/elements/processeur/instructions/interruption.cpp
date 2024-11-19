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

#include <src/machine/elements/processeur/instructions/interruption.h>
using namespace SVM::Machine::Element::Processeur::Instruction;

void Interruption::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_interruption->decoration(decorateur);	
	if(static_cast<bool>(_cible))
	{
		_cible->decoration(decorateur);
	}
	sans_position(decorateur);
}

void Interruption::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Valeur::Interruption interruption = _interruption->evaluation_interruption(noyau);
	if(static_cast<bool>(_cible))
	{
		SVM_Valeur::AdresseInstruction cible = _cible->evaluation(noyau);
		Interruption::execution(noyau,_globale,_cascade,interruption,cible);
	}
	else
	{
		Interruption::execution(noyau,_globale,_cascade,interruption);
	}
}

void Interruption::execution(const SVM_Noyau::NoyauSP& noyau, const bool globale, const bool cascade, const SVM_Valeur::Interruption& interruption, const SVM_Valeur::AdresseInstruction& adresse)
{
	if(globale)
	{
		noyau->_processeur->ajoute_interruption_gestionnaire_global(interruption,adresse,noyau->gestionnaire_interruptions_materielles());
	}
	else
	{
		noyau->_processeur->ajoute_interruption_gestionnaire_local(interruption,adresse,cascade);
	}
}

void Interruption::execution(const SVM_Noyau::NoyauSP& noyau, const bool globale, const bool cascade, const SVM_Valeur::Interruption& interruption)
{
	if(globale)
	{
		noyau->_processeur->enleve_interruption_gestionnaire_global(interruption,noyau->gestionnaire_interruptions_materielles());
	}
	else
	{
		noyau->_processeur->enleve_interruption_gestionnaire_local(interruption,cascade);
	}
}

void Interruption::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream irq;
	_interruption->explique(noyau,irq);
	explications.push_back(irq.str());
	if(not _cible) return;
	_cible->explication(noyau,explications);
}
