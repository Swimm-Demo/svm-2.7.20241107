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

#include <src/machine/elements/processeur/instructions/affectation.h>
using namespace SVM::Machine::Element::Processeur::Instruction;

void AffectationValeur::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_valeur->decoration(decorateur);
	_adresse->decoration(decorateur);
	sans_position(decorateur);
}

void AffectationValeur::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Memoire::ValeurSP valeur = _valeur->evaluation(noyau);
	SVM_Valeur::AdresseMemoire adresse = _adresse->evaluation(noyau);
	AffectationValeur::execution(noyau,valeur,adresse);
}

void AffectationValeur::execution(const SVM_Noyau::NoyauSP& noyau, const SVM_Memoire::ValeurSP& valeur, const SVM_Valeur::AdresseMemoire& adresse)
{
	noyau->_memoire->ecriture(adresse,valeur);
}

void AffectationValeur::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream val;
	_valeur->explique(noyau,val);
	explications.push_back(val.str());
	std::ostringstream adr;
	_adresse->explique(noyau,adr);
	explications.push_back(adr.str());
}

void AffectationBloc::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	for(auto& v:_valeurs)
	{
		if(static_cast<bool>(v))
		{
			v->decoration(decorateur);
		}
	}
	_pointeur->decoration(decorateur);
	sans_position(decorateur);
}

void AffectationBloc::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	std::vector<SVM_Memoire::ValeurSP> valeurs;
	for(auto& v:_valeurs)
	{
		if(static_cast<bool>(v))
		{
			valeurs.push_back(v->evaluation(noyau));
		}
		else
		{
			valeurs.push_back(std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::AUTOMATIQUE)));
		}
	}
	SVM_Valeur::Pointeur pointeur = _pointeur->evaluation_pointeur(noyau);
	AffectationBloc::execution(noyau,valeurs,pointeur);
}

void AffectationBloc::execution(const SVM_Noyau::NoyauSP& noyau, const std::vector<SVM_Memoire::ValeurSP>& valeurs, const SVM_Valeur::Pointeur& pointeur)
{
	noyau->_memoire->ecriture(pointeur,valeurs);
}

void AffectationBloc::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	for(auto& v:_valeurs)
	{
		if(not v) continue;
		std::ostringstream val;
		v->explique(noyau,val);
		explications.push_back(val.str());
	}
	std::ostringstream poi;
	_pointeur->explique(noyau,poi);
	explications.push_back(poi.str());
}

void AffectationInstruction::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_instruction->decoration(decorateur);
	_systeme = _instruction->_systeme;
	_attente = _instruction->_attente;
	_adresse->decoration(decorateur);
	sans_position(decorateur);
}

void AffectationInstruction::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Memoire::ValeurSP valeur = _instruction->execution_avec_resultat(noyau);
	if(not valeur)
	{
		throw SVM_Valeur::InstructionSansRetour(_instruction.get());
	}
	SVM_Valeur::AdresseMemoire adresse = _adresse->evaluation(noyau);
	AffectationValeur::execution(noyau,valeur,adresse);
}

void AffectationInstruction::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	_instruction->explication(noyau,explications);
	std::ostringstream adr;
	_adresse->explique(noyau,adr);
	explications.push_back(adr.str());
}
