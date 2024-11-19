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

#include <src/machine/elements/programme/analyseur/analyseur.h>
#include <src/machine/elements/processeur/instructions/bibliotheque.h>
using namespace SVM::Machine::Element::Processeur::Instruction;

void Bibliotheque::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_nom->decoration(decorateur);
	_code->decoration(decorateur);
	sans_position(decorateur);
}

SVM_Memoire::ValeurSP Bibliotheque::execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau) const
{
	std::string nom = _nom->evaluation_chaine(noyau);
	std::string code = _code->evaluation_chaine(noyau);
	return std::make_shared<SVM_Memoire::Bibliotheque>(execution_avec_resultat(noyau,nom,code,_position));
}

SVM_Valeur::CodeSP Bibliotheque::execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau, const std::string& nom, const std::string& code, const SVM_Valeur::PositionInstruction& position)
{
	SVM_Valeur::PositionInstructionSP p = std::make_shared<SVM_Valeur::PositionInstruction>(position);
	return SVM_Programme::Analyseur::analyse_chaine(nom,code,noyau->machine(),p);
}

void Bibliotheque::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream nom;
	_nom->explique(noyau,nom);
	explications.push_back(nom.str());
	std::ostringstream cod;
	_code->explique(noyau,cod);
	explications.push_back(cod.str());
}
