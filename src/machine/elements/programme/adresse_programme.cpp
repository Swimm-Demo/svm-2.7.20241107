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

#include <src/machine/elements/programme/adresse_programme.h>
#include <src/machine/elements/programme/valeur_programme.h>
#include <src/machine/elements/memoire/valeur.h>

using namespace SVM::Machine::Element::Programme;

void Adresse::explique(const SVM_Noyau::NoyauSP& noyau, std::ostream& os) const
{
	os << "Address ";
	format(os);
	os << "\n";
	SVM_Valeur::ArbreSubstitutionSP arbre = std::make_shared<SVM_Valeur::ArbreSubstitution>(this);
	std::ostringstream erreur;
	try
	{
		explique_calcul(noyau,arbre);
	}
	catch(SVM_Valeur::Interruption& i)
	{
		erreur << i;
	}
	catch(SVM_Valeur::ExceptionExecution& e)
	{
		erreur << e;
	}
	if(arbre->explique(os))
	{
		os << SVM::Global::Chaine::echappe_html(arbre->_resultat);
	}
	else
	{
		os << SVM::Global::Chaine::echappe_html(erreur.str());
	}
}

void AdresseDepuisPointeur::format(std::ostream& os) const
{
	os << "&" << *_pointeur;
}

void AdresseDepuisPointeur::html(std::ostream& os) const
{
	os << "<span class=\"operator\">&amp;</span>";
	_pointeur->html(os);
}

void AdresseDepuisIndicePointeur::format(std::ostream& os) const
{
	os << "(" << *_pointeur << "/" << *_indice << ")";
}

void AdresseDepuisIndicePointeur::html(std::ostream& os) const
{
	os << "(";
	_pointeur->html(os);
	os << "/";
	_indice->html(os);
	os << ")";
}

void AdresseDecalage::format(std::ostream& os) const
{
	os << "(" << *_adresse << ((_sens>0)?"+":"-") << *_indice << ")";
}

void AdresseDecalage::html(std::ostream& os) const
{
	os << "(";
	_adresse->html(os);
	os << ((_sens>0)?"+":"-");
	_indice->html(os);
	os << ")";
}

void AdresseConstante::decoration(SVM_Valeur::Decorateur& decorateur)
{
}

SVM_Valeur::AdresseMemoire AdresseConstante::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	return _adresse;
}

SVM_Valeur::AdresseMemoire AdresseConstante::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->resultat(_adresse);
	return _adresse;
}

void AdresseDepuisPointeur::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_pointeur->decoration(decorateur);
}

SVM_Valeur::AdresseMemoire AdresseDepuisPointeur::evaluation(const SVM_Noyau::NoyauSP& ce) const 
{
	SVM_Valeur::Pointeur p = _pointeur->evaluation_pointeur(ce);
	return p.adresse();
}

SVM_Valeur::AdresseMemoire AdresseDepuisPointeur::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise("&","");
	arbre->ajout_enfant("",_pointeur);
	auto a = _pointeur->explique_calcul_pointeur(noyau,arbre->enfant(0));
	auto r = a.adresse();
	arbre->resultat(r);
	return r;
}

void AdresseDepuisIndicePointeur::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_pointeur->decoration(decorateur);
	_indice->decoration(decorateur);
}

SVM_Valeur::AdresseMemoire AdresseDepuisIndicePointeur::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	SVM_Valeur::Pointeur p = _pointeur->evaluation_pointeur(ce);
	long int indice = _indice->evaluation_entier(ce);
	return p.element(static_cast<ssize_t>(indice));
}

SVM_Valeur::AdresseMemoire AdresseDepuisIndicePointeur::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise("(",")");
	arbre->ajout_enfant("",_pointeur);
	arbre->ajout_enfant("/",_indice);
	SVM_Valeur::Pointeur p = _pointeur->explique_calcul_pointeur(noyau,arbre->enfant(0));
	long int indice = _indice->explique_calcul_entier(noyau,arbre->enfant(1));
	auto r = p.element(static_cast<ssize_t>(indice));
	arbre->resultat(r);
	return r;
}

void AdresseDecalage::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_adresse->decoration(decorateur);
	_indice->decoration(decorateur);
}

SVM_Valeur::AdresseMemoire AdresseDecalage::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	SVM_Valeur::AdresseMemoire a = _adresse->evaluation(ce);
	long int indice = _indice->evaluation_entier(ce);
	return a+_sens*indice;
}

SVM_Valeur::AdresseMemoire AdresseDecalage::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise("(",")");
	arbre->ajout_enfant("",_adresse);
	arbre->ajout_enfant((_sens>0)?"+":"-",_indice);
	SVM_Valeur::AdresseMemoire a = _adresse->explique_calcul(noyau,arbre->enfant(0));
	long int indice = _indice->explique_calcul_entier(noyau,arbre->enfant(1));
	auto r = a+_sens*indice;
	arbre->resultat(r);
	return r;
}
