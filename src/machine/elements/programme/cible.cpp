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

#include <src/machine/elements/programme/cible.h>

using namespace SVM::Machine::Element::Programme;

void CibleStatique::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_adresse = SVM_Valeur::AdresseInstruction(decorateur._code->resolution_etiquette(_etiquette));
}

SVM_Valeur::AdresseInstruction CibleStatique::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	return _adresse;
}

SVM_Valeur::AdresseInstruction CibleStatique::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->resultat(_adresse);
	return _adresse;
}

void CibleDynamique::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_cible->decoration(decorateur);
}

SVM_Valeur::AdresseInstruction CibleDynamique::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Memoire::ValeurSP valeur = _cible->evaluation(noyau);
	if(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::CHAINE) == (*valeur))
	{
		std::string etiquette = _cible->evaluation_chaine(valeur);
		size_t adresse = noyau->_processeur->code_courant()->resolution_etiquette(etiquette);
		return SVM_Valeur::AdresseInstruction(adresse);
	}
	else
	{
		return _cible->evaluation_symbole(valeur);
	}
}

SVM_Valeur::AdresseInstruction CibleDynamique::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->ajout_enfant("",_cible);
	SVM_Memoire::ValeurSP valeur = _cible->explique_calcul(noyau,arbre->enfant(0));
	if(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::CHAINE) == (*valeur))
	{
		std::string etiquette = _cible->evaluation_chaine(valeur);
		size_t adresse = noyau->_processeur->code_courant()->resolution_etiquette(etiquette);
		auto r = SVM_Valeur::AdresseInstruction(adresse);
		arbre->resultat(r);
		return r;
	}
	else
	{
		auto r = _cible->evaluation_symbole(valeur);
		arbre->resultat(r);
		return r;
	}
}

void CibleDynamique::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream cib;
	_cible->explique(noyau,cib);
	explications.push_back(cib.str());
}
