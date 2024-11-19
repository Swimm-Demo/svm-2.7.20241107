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

#include <src/machine/elements/programme/valeur_programme.h>
#include <src/machine/machine/machine.h>
#include <src/machine/elements/valeur/expression_condition.h>

using namespace SVM::Machine::Element::Programme;

void Valeur::explique(const SVM_Noyau::NoyauSP& noyau, std::ostream& os) const
{
	os << "Value ";
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

long long int Valeur::evaluation_entier(const SVM_Memoire::ValeurSP& valeur) const
{
	SVM_Memoire::EntierSP vv = std::dynamic_pointer_cast<SVM_Memoire::Entier>(valeur);
	ASSERT(static_cast<bool>(vv),"Evaluation entiere sur une valeur non entiere");
	return vv->_valeur;
}

long long int Valeur::explique_calcul_entier(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre)
{
	SVM_Memoire::EntierSP vv = std::dynamic_pointer_cast<SVM_Memoire::Entier>(explique_calcul(noyau,arbre));
	ASSERT(static_cast<bool>(vv),"Evaluation entiere sur une valeur non entiere");
	return vv->_valeur;
}

std::string Valeur::evaluation_chaine(const SVM_Memoire::ValeurSP& valeur) const
{
	SVM_Memoire::ChaineSP vv = std::dynamic_pointer_cast<SVM_Memoire::Chaine>(valeur);
	ASSERT(static_cast<bool>(vv),"Evaluation chaine sur une valeur non chaine");
	return vv->_valeur;
}

std::string Valeur::explique_calcul_chaine(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre)
{
	SVM_Memoire::ChaineSP vv = std::dynamic_pointer_cast<SVM_Memoire::Chaine>(explique_calcul(noyau,arbre));
	ASSERT(static_cast<bool>(vv),"Evaluation chaine sur une valeur non chaine");
	return vv->_valeur;
}

bool Valeur::evaluation_booleen(const SVM_Memoire::ValeurSP& valeur) const
{
	SVM_Memoire::BooleenSP vv = std::dynamic_pointer_cast<SVM_Memoire::Booleen>(valeur);
	ASSERT(static_cast<bool>(vv),"Evaluation booleene sur une valeur non booleene");
	return vv->_valeur;
}

bool Valeur::explique_calcul_booleen(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre)
{
	SVM_Memoire::BooleenSP vv = std::dynamic_pointer_cast<SVM_Memoire::Booleen>(explique_calcul(noyau,arbre));
	ASSERT(static_cast<bool>(vv),"Evaluation booleene sur une valeur non booleene");
	return vv->_valeur;
}

SVM_Valeur::Pointeur Valeur::evaluation_pointeur(const SVM_Memoire::ValeurSP& valeur) const
{
	SVM_Memoire::PointeurSP vv = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(valeur);
	ASSERT(static_cast<bool>(vv),"Evaluation pointeur sur une valeur non pointeur");
	return vv->_valeur;
}

SVM_Valeur::Pointeur Valeur::explique_calcul_pointeur(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre)
{
	SVM_Memoire::PointeurSP vv = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(explique_calcul(noyau,arbre));
	ASSERT(static_cast<bool>(vv),"Evaluation pointeur sur une valeur non pointeur");
	return vv->_valeur;
}

SVM_Valeur::CodeSP Valeur::evaluation_bibliotheque(const SVM_Memoire::ValeurSP& valeur) const
{
	SVM_Memoire::BibliothequeSP vv = std::dynamic_pointer_cast<SVM_Memoire::Bibliotheque>(valeur);
	ASSERT(static_cast<bool>(vv),"Evaluation bibliotheque sur une valeur non bibliotheque");
	return vv->_valeur;
}

SVM_Valeur::CodeSP Valeur::explique_calcul_bibliotheque(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre)
{
	SVM_Memoire::BibliothequeSP vv = std::dynamic_pointer_cast<SVM_Memoire::Bibliotheque>(explique_calcul(noyau,arbre));
	ASSERT(static_cast<bool>(vv),"Evaluation bibliotheque sur une valeur non bibliotheque");
	return vv->_valeur;
}

SVM_Valeur::AdresseInstruction Valeur::evaluation_symbole(const SVM_Memoire::ValeurSP& valeur) const
{
	SVM_Memoire::SymboleSP vv = std::dynamic_pointer_cast<SVM_Memoire::Symbole>(valeur);
	ASSERT(static_cast<bool>(vv),"Evaluation symbole sur une valeur non symbole");
	return vv->_valeur;
}

SVM_Valeur::PointEntreeExtension Valeur::evaluation_point_entree_extension(const SVM_Memoire::ValeurSP& valeur) const
{
	SVM_Memoire::PointEntreeExtensionSP vv = std::dynamic_pointer_cast<SVM_Memoire::PointEntreeExtension>(valeur);
	ASSERT(static_cast<bool>(vv),"Evaluation point entree extension sur une valeur non point entree extension");
	return vv->_valeur;
}

SVM_Valeur::PointEntreeExtension Valeur::explique_calcul_point_entree_extension(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre)
{
	SVM_Memoire::PointEntreeExtensionSP vv = std::dynamic_pointer_cast<SVM_Memoire::PointEntreeExtension>(explique_calcul(noyau,arbre));
	ASSERT(static_cast<bool>(vv),"Evaluation point entree extension sur une valeur non point entree extension");
	return vv->_valeur;
}

SVM_Valeur::Interruption Valeur::evaluation_interruption(const SVM_Memoire::ValeurSP& valeur) const
{
	SVM_Memoire::InterruptionSP vv = std::dynamic_pointer_cast<SVM_Memoire::Interruption>(valeur);
	ASSERT(static_cast<bool>(vv),"Evaluation interruption sur une valeur non interruption");
	return vv->_valeur;
}

SVM_Memoire::ValeurSP LectureMemoire::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	SVM_Valeur::AdresseMemoire adresse = _adresse->evaluation(ce);
	return ce->_memoire->lecture(adresse,_validateur,_types_attendus);
}

SVM_Memoire::ValeurSP LectureMemoire::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise("@","");
	arbre->ajout_enfant("",_adresse);
	SVM_Valeur::AdresseMemoire adresse = _adresse->explique_calcul(noyau,arbre->enfant(0));
	auto r = noyau->_memoire->lecture(adresse,_validateur,_types_attendus,false/* == pas de point d'arret*/); 
	arbre->resultat(r);
	return r;
}

void LectureMemoire::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_adresse->decoration(decorateur);
}

void Entier::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_valeur->constante();
}

SVM_Memoire::ValeurSP Entier::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	return _valeur;
}

SVM_Memoire::ValeurSP Entier::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->resultat(_valeur);
	return _valeur;
}

void TaillePointeur::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_pointeur->decoration(decorateur);
}

SVM_Memoire::ValeurSP TaillePointeur::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	SVM_Valeur::Pointeur p = _pointeur->evaluation_pointeur(ce);
	return std::make_shared<SVM_Memoire::Entier>(p.taille());
}

SVM_Memoire::ValeurSP TaillePointeur::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise("SIZE ","");
	arbre->ajout_enfant("",_pointeur);
	SVM_Valeur::Pointeur p = _pointeur->explique_calcul_pointeur(noyau,arbre->enfant(0));
	auto r = std::make_shared<SVM_Memoire::Entier>(p.taille());
	arbre->resultat(r);
	return r;
}

void Chaine::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_valeur->constante();
}

SVM_Memoire::ValeurSP Chaine::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	return _valeur;
}

SVM_Memoire::ValeurSP Chaine::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->resultat(_valeur);
	return _valeur;
}

void Booleen::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_valeur->constante();
}

SVM_Memoire::ValeurSP Booleen::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	return _valeur;
}

SVM_Memoire::ValeurSP Booleen::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->resultat(_valeur);
	return _valeur;
}

void BooleenCondition::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_valeur->decoration(decorateur);
}

SVM_Memoire::ValeurSP BooleenCondition::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	return std::make_shared<SVM_Memoire::Booleen>(_valeur->evaluation(ce));
}

SVM_Memoire::ValeurSP BooleenCondition::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise("? ","");
	arbre->ajout_enfant("",_valeur);
	auto r = std::make_shared<SVM_Memoire::Booleen>(_valeur->explique_calcul(noyau,arbre->enfant(0)));
	arbre->resultat(r);
	return r;
}

void BooleenCondition::format(std::ostream& os) const
{
	os << "? " << *_valeur;
}

void BooleenCondition::html(std::ostream& os) const
{
	os << "? ";
	_valeur->html(os);
}

void Pointeur::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_valeur->constante();
}

SVM_Memoire::ValeurSP Pointeur::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	return _valeur;
}

SVM_Memoire::ValeurSP Pointeur::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->resultat(_valeur);
	return _valeur;
}

void PointeurConstruit::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_adresse->decoration(decorateur);
	_taille->decoration(decorateur);
}

SVM_Memoire::ValeurSP PointeurConstruit::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	SVM_Valeur::AdresseMemoire adresse = _adresse->evaluation(ce);
	long int taille = _taille->evaluation_entier(ce);
	if(taille<0)
	{
		throw TaillePointeurInvalide(taille);
	}
	auto p = std::make_shared<SVM_Memoire::Pointeur>(SVM_Valeur::Pointeur(adresse,taille));
	p->constante();
	return p;
}

SVM_Memoire::ValeurSP PointeurConstruit::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->ajout_enfant("",_adresse);
	arbre->ajout_enfant("*",_taille);
	SVM_Valeur::AdresseMemoire adresse = _adresse->explique_calcul(noyau,arbre->enfant(0));
	long int taille = _taille->explique_calcul_entier(noyau,arbre->enfant(1));
	if(taille<0)
	{
		throw TaillePointeurInvalide(taille);
	}
	auto r = std::make_shared<SVM_Memoire::Pointeur>(SVM_Valeur::Pointeur(adresse,taille));
	arbre->resultat(r);
	return r;
}

void PointeurAlias::decoration(SVM_Valeur::Decorateur& decorateur)
{
}

SVM_Memoire::ValeurSP PointeurAlias::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	auto p = std::make_shared<SVM_Memoire::Pointeur>(ce->_memoire->resolution_alias(_alias));
	p->constante();
	return p;
}

SVM_Memoire::ValeurSP PointeurAlias::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	auto r = std::make_shared<SVM_Memoire::Pointeur>(noyau->_memoire->resolution_alias(_alias));
	arbre->resultat(r);
	return r;
}

void PointeurCourant::decoration(SVM_Valeur::Decorateur& decorateur)
{
}

SVM_Memoire::ValeurSP PointeurCourant::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	auto p = std::make_shared<SVM_Memoire::Pointeur>(ce->_processeur->memoire_courante());
	p->constante();
	return p;
}

SVM_Memoire::ValeurSP PointeurCourant::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	auto r = std::make_shared<SVM_Memoire::Pointeur>(noyau->_processeur->memoire_courante());
	arbre->resultat(r);
	return r;
}

void SymboleLocal::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_symbole->decoration(decorateur);
}

SVM_Memoire::ValeurSP SymboleLocal::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	SVM_Valeur::CodeSP code_courant = ce->_processeur->code_courant();
	std::string symbole = _symbole->evaluation_chaine(ce);
	size_t locale = code_courant->resolution_symbole(symbole);
	return std::make_shared<SVM_Memoire::Symbole>(SVM_Valeur::AdresseInstruction(code_courant,locale));
}

SVM_Memoire::ValeurSP SymboleLocal::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise("$","");
	arbre->ajout_enfant("",_symbole);
	SVM_Valeur::CodeSP code_courant = noyau->_processeur->code_courant();
	std::string symbole = _symbole->explique_calcul_chaine(noyau,arbre->enfant(0));
	size_t locale = code_courant->resolution_symbole(symbole);
	auto r = std::make_shared<SVM_Memoire::Symbole>(SVM_Valeur::AdresseInstruction(code_courant,locale));
	arbre->resultat(r);
	return r;
}

void SymboleDebut::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_bibliotheque->decoration(decorateur);
}

SVM_Memoire::ValeurSP SymboleDebut::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	SVM_Valeur::CodeSP code = _bibliotheque->evaluation_bibliotheque(ce);
	return std::make_shared<SVM_Memoire::Symbole>(SVM_Valeur::AdresseInstruction(code,0));
}

SVM_Memoire::ValeurSP SymboleDebut::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise("$(","/)");
	arbre->ajout_enfant("",_bibliotheque);
	SVM_Valeur::CodeSP code = _bibliotheque->explique_calcul_bibliotheque(noyau,arbre->enfant(0));
	auto r = std::make_shared<SVM_Memoire::Symbole>(SVM_Valeur::AdresseInstruction(code,0));
	arbre->resultat(r);
	return r;
}

void SymboleComplet::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_bibliotheque->decoration(decorateur);
	_symbole->decoration(decorateur);
}

SVM_Memoire::ValeurSP SymboleComplet::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	SVM_Valeur::CodeSP code = _bibliotheque->evaluation_bibliotheque(ce);
	std::string symbole = _symbole->evaluation_chaine(ce);
	size_t locale = code->resolution_symbole(symbole);
	return std::make_shared<SVM_Memoire::Symbole>(SVM_Valeur::AdresseInstruction(code,locale));
}

SVM_Memoire::ValeurSP SymboleComplet::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->initialise("$(",")");
	arbre->ajout_enfant("",_bibliotheque);
	arbre->ajout_enfant("/",_symbole);
	SVM_Valeur::CodeSP code = _bibliotheque->explique_calcul_bibliotheque(noyau,arbre->enfant(0));
	std::string symbole = _symbole->explique_calcul_chaine(noyau,arbre->enfant(1));
	size_t locale = code->resolution_symbole(symbole);
	auto r = std::make_shared<SVM_Memoire::Symbole>(SVM_Valeur::AdresseInstruction(code,locale));
	arbre->resultat(r);
	return r;
}

void PointEntreeExtension::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_pep->constante();
}

SVM_Memoire::ValeurSP PointEntreeExtension::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	return _pep;
}

SVM_Memoire::ValeurSP PointEntreeExtension::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->resultat(_pep);
	return _pep;
}

void InterruptionInterne::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_interruption->constante();
}

SVM_Memoire::ValeurSP InterruptionInterne::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	return _interruption;
}

SVM_Memoire::ValeurSP InterruptionInterne::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->resultat(_interruption);
	return _interruption;
}

void InterruptionExterne::decoration(SVM_Valeur::Decorateur& decorateur)
{
	ASSERT(_interruption->_valeur._interruption_interne==SVM_Valeur::Interruption::InterruptionInterne::INTERRUPTION_EXTENSION,"interruption interne dans interruption externe");
	_interruption->_patron = decorateur._machine->_base->_gestionnaire_extensions->interruption(_interruption->_valeur._point_entree_extension);
	if(not _interruption->_patron)
	{
		throw InterruptionInexistante(_interruption->_valeur._point_entree_extension);
	}
	_interruption->constante();
}

SVM_Memoire::ValeurSP InterruptionExterne::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	return _interruption;
}

SVM_Memoire::ValeurSP InterruptionExterne::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->resultat(_interruption);
	return _interruption;
}

void ConstanteExtension::decoration(SVM_Valeur::Decorateur& decorateur)
{
	auto t = decorateur._machine->_base->_gestionnaire_extensions->type(_type);
	if(not t)
	{
		throw SVM_Memoire::TypeInexistant(_type);
	}
	if(not t->_constante)
	{
		throw SVM_Memoire::TypePasDeConstante(_type);
	}
	std::string s = *_texte->_valeur;
	SVM_String sv;
	sv.string = s.c_str();
	sv.size = s.size();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(t,decorateur._machine,decorateur._machine->_base->_gestionnaire_extensions->variables_extension(_type.nom_extension()));
	try
	{
		void *v=t->_constante(&environnement,sv);
		_valeur.reset(new SVM_Memoire::Extension(decorateur._machine,_type,t,v));
		_valeur->constante();
	}
	catch(SVM_Valeur::Interruption& i)
	{
		i.position(_position);
		throw;
	}
}

SVM_Memoire::ValeurSP ConstanteExtension::evaluation(const SVM_Noyau::NoyauSP& ce) const
{
	return _valeur;
}

SVM_Memoire::ValeurSP ConstanteExtension::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->resultat(_valeur);
	return _valeur;
}

