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

#include <src/machine/elements/processeur/instructions/conditions.h>

using namespace SVM::Machine::Element::Processeur::Instruction;

void ConditionBooleen::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_booleen->decoration(decorateur);
}

bool ConditionBooleen::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	return _booleen->evaluation_booleen(noyau);
}

bool ConditionBooleen::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise(""," TRUE");
	arbre->ajout_enfant("",_booleen);
	auto r = _booleen->explique_calcul_booleen(noyau,arbre->enfant(0));
	arbre->resultat_booleen(r);
	return r;
}

void ConditionBooleen::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream boo;
	_booleen->explique(noyau,boo);
	explications.push_back(boo.str());
}

void ConditionDrapeau::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_drapeau->decoration(decorateur);
}

bool ConditionDrapeau::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	std::string drapeau = _drapeau->evaluation_chaine(noyau);
	return noyau->_processeur->teste_drapeau(drapeau);
}

bool ConditionDrapeau::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise(""," RAISED");
	arbre->ajout_enfant("",_drapeau);
	auto drapeau = _drapeau->explique_calcul_chaine(noyau,arbre->enfant(0));
	auto r = noyau->_processeur->teste_drapeau(drapeau);
	arbre->resultat_booleen(r);
	return r;
}

void ConditionDrapeau::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream dra;
	_drapeau->explique(noyau,dra);
	explications.push_back(dra.str());
}

void ConditionAdresseDefinie::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_adresse->decoration(decorateur);
}

bool ConditionAdresseDefinie::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Valeur::AdresseMemoire adresse = _adresse->evaluation(noyau);
	return noyau->_memoire->adresse_definie(adresse);
}

bool ConditionAdresseDefinie::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise(""," DEFINED");
	arbre->ajout_enfant("",_adresse);
	auto adresse = _adresse->explique_calcul(noyau,arbre->enfant(0));
	auto r = noyau->_memoire->adresse_definie(adresse);
	arbre->resultat_booleen(r);
	return r;
}

void ConditionAdresseDefinie::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream adr;
	_adresse->explique(noyau,adr);
	explications.push_back(adr.str());
}

void ConditionAdresseInitialisee::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_adresse->decoration(decorateur);
}

bool ConditionAdresseInitialisee::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Valeur::AdresseMemoire adresse = _adresse->evaluation(noyau);
	return noyau->_memoire->adresse_initialisee(adresse);
}

bool ConditionAdresseInitialisee::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise(""," INITIALISED");
	arbre->ajout_enfant("",_adresse);
	auto adresse = _adresse->explique_calcul(noyau,arbre->enfant(0));
	auto r = noyau->_memoire->adresse_initialisee(adresse);
	arbre->resultat_booleen(r);
	return r;
}

void ConditionAdresseInitialisee::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream adr;
	_adresse->explique(noyau,adr);
	explications.push_back(adr.str());
}

void ConditionIndiceDansPointeur::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_indice->decoration(decorateur);
	_pointeur->decoration(decorateur);
}

bool ConditionIndiceDansPointeur::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	auto indice = _indice->evaluation_entier(noyau);
	SVM_Valeur::Pointeur pointeur = _pointeur->evaluation_pointeur(noyau);
	return pointeur.interieur(indice);
}

bool ConditionIndiceDansPointeur::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->ajout_enfant("",_indice);
	arbre->ajout_enfant(" IN ",_pointeur);
	auto indice = _indice->explique_calcul_entier(noyau,arbre->enfant(0));
	auto pointeur = _pointeur->explique_calcul_pointeur(noyau,arbre->enfant(1));
	auto r = pointeur.interieur(indice);
	arbre->resultat_booleen(r);
	return r;
}

void ConditionIndiceDansPointeur::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream ind;
	_indice->explique(noyau,ind);
	explications.push_back(ind.str());
	std::ostringstream ptr;
	_pointeur->explique(noyau,ptr);
	explications.push_back(ptr.str());
}

void ConditionAdresseDansPointeur::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_adresse->decoration(decorateur);
	_pointeur->decoration(decorateur);
}

bool ConditionAdresseDansPointeur::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Valeur::AdresseMemoire adresse = _adresse->evaluation(noyau);
	SVM_Valeur::Pointeur pointeur = _pointeur->evaluation_pointeur(noyau);
	return pointeur.interieur(adresse-pointeur.adresse());
}

bool ConditionAdresseDansPointeur::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->ajout_enfant("",_adresse);
	arbre->ajout_enfant(" IN ",_pointeur);
	SVM_Valeur::AdresseMemoire adresse = _adresse->explique_calcul(noyau,arbre->enfant(0));
	auto pointeur = _pointeur->explique_calcul_pointeur(noyau,arbre->enfant(1));
	auto r = pointeur.interieur(adresse-pointeur.adresse());
	arbre->resultat_booleen(r);
	return r;
}

void ConditionAdresseDansPointeur::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream adr;
	_adresse->explique(noyau,adr);
	explications.push_back(adr.str());
	std::ostringstream ptr;
	_pointeur->explique(noyau,ptr);
	explications.push_back(ptr.str());
}

void ConditionAdresseEstType::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_adresse->decoration(decorateur);
	_type->decoration(decorateur);
}

bool ConditionAdresseEstType::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Valeur::AdresseMemoire adresse = _adresse->evaluation(noyau);
	SVM_Memoire::Type type = _type->evaluation(noyau);
	return noyau->_memoire->adresse_type(adresse,type);
}

bool ConditionAdresseEstType::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->ajout_enfant("",_adresse);
	arbre->ajout_enfant(" IS ",_type);
	auto adresse = _adresse->explique_calcul(noyau,arbre->enfant(0));
	auto type = _type->explique_calcul(noyau,arbre->enfant(1));
	auto r = noyau->_memoire->adresse_type(adresse,type);
	arbre->resultat_booleen(r);
	return r;
}

void ConditionAdresseEstType::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream adr;
	_adresse->explique(noyau,adr);
	explications.push_back(adr.str());
}

void ConditionCibleAppelable::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_cible->decoration(decorateur);
}

bool ConditionCibleAppelable::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	try
	{
		SVM_Valeur::AdresseInstruction cible = _cible->evaluation(noyau);
	}
	catch(SVM_Valeur::EtiquetteNonDefinie& e)
	{
		return false;
	}
	catch(SVM_Valeur::SymboleNonDefini& e)
	{
		return false;
	}
	return true;
}

bool ConditionCibleAppelable::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise(""," CALLABLE");
	arbre->ajout_enfant("",_cible);
	try
	{
		SVM_Valeur::AdresseInstruction cible = _cible->explique_calcul(noyau,arbre->enfant(0));
	}
	catch(SVM_Valeur::EtiquetteNonDefinie& e)
	{
		arbre->resultat_booleen(false);
		return false;
	}
	catch(SVM_Valeur::SymboleNonDefini& e)
	{
		arbre->resultat_booleen(false);
		return false;
	}
	arbre->resultat_booleen(true);
	return true;
}

void ConditionCibleAppelable::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	_cible->explication(noyau,explications);
}

void ConditionDebugueur::decoration(SVM_Valeur::Decorateur& decorateur)
{
}

bool ConditionDebugueur::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	return static_cast<bool>(noyau->debugueur());
}

bool ConditionDebugueur::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	auto r = static_cast<bool>(noyau->debugueur());
	arbre->resultat_booleen(r);
	return r;
}
