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

#include <src/machine/interface/fonction.h>
#include <src/machine/interface/structure.h>
#include <src/machine/machine/machine.h>
#include <src/machine/elements/ordonnanceur/ordonnanceur.h>
#include <src/machine/elements/processus/processus.h>
using namespace SVM::Machine::Interface;

Fonction::Fonction(const SVM::Machine::MachineSP& machine, const SVM_Valeur::PointEntreeExtension& nom_fonction, const std::vector<SVM_Valeur::ParametreSP>& parametres)
:_nom(nom_fonction), _patron(machine->_base->_gestionnaire_extensions->fonction(nom_fonction)), _parametres(parametres), _machine(machine)
{
}

struct MemoireFonction
{
	MemoireFonction(const void **parametres)
	:_parametres(parametres) {}
	~MemoireFonction()
	{
		if(_parametres!=nullptr)
		{
			delete [] _parametres;
		}
	}
	const void **_parametres;
};

RetourFonction Fonction::appel(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& appellant) const
{
	if(not _patron)
	{
		throw FonctionInexistante(_nom);
	}
	std::vector<SVM_Extension::RegexParametresElementCSP> types;
	for(auto& p:_parametres)
	{
		types.push_back(p->type());
	}
	_patron->verifie_parametres(types);
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,_machine,_machine->_base->_gestionnaire_extensions->variables_extension(_nom.nom_extension()),appellant->_attente,appellant->_systeme);
	environnement->_variable_noyau = nullptr;
	environnement->_variable_processus = nullptr;
	environnement->_variable_ordonnanceur = nullptr;
	if(static_cast<bool>(noyau))
	{
		environnement->_variable_noyau = environnement->ajout(noyau);
		auto p = noyau->_processus.lock();
		if(static_cast<bool>(p))
		{
			environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<VariableProcessus>(p));
			auto o = p->_ordonnanceur.lock();
			if(static_cast<bool>(o))
			{
				environnement->_variable_ordonnanceur = environnement->ajout(o);
			}
		}
	}
	environnement->_position = appellant->_position;
	::MemoireFonction memoire(new const void*[_parametres.size()]);
	size_t indice = 0;
	for(auto& p: _parametres)
	{
		memoire._parametres[indice++] = environnement->ajout<SVM_Valeur::Parametre>(p);
		auto pv = std::dynamic_pointer_cast<SVM_Valeur::ParametreVariable>(p);
		if(static_cast<bool>(pv))
		{
			appellant->exporte(pv->_variable,environnement->_appellant);
		}
	}
	const void* retour_brut = _patron->_fonction(&environnement,_parametres.size(),memoire._parametres);
	RetourFonction retour;
	if(retour_brut!=nullptr)
	{
		retour._valeur = environnement->valeur<SVM_Memoire::ValeurInterface>(retour_brut);
		retour._structure = environnement->valeur<SVM_Interface::Structure>(retour_brut);
		if(environnement->existe(retour_brut))
		{
			retour._variable = retour_brut;
		}
	}
	if(static_cast<bool>(retour._valeur))
	{
		SVM_Memoire::Type type_retour = *retour._valeur->_valeur;
		_patron->verifie_retour(std::make_shared<const SVM_Extension::RegexParametresValeur>(type_retour),retour._valeur->_valeur->nulle());
	}
	else if(static_cast<bool>(retour._structure))
	{
		SVM_Memoire::Type type_retour = *retour._structure;
		_patron->verifie_retour(std::make_shared<const SVM_Extension::RegexParametresStructure>(type_retour),retour._structure->nulle());
	}
	else if(retour._variable)
	{
		_patron->verifie_retour(std::make_shared<const SVM_Extension::RegexParametresJokerPartielVariable>());
		environnement->exporte(retour._variable,appellant->_appellant);
	}
	else
	{
		_patron->verifie_retour(SVM_Extension::RegexParametresValeurSP());
	}
	if(static_cast<bool>(_machine->_base->_debugueur))
	{
		SVM_Memoire::PiegeMemoireSP piege;
		if(static_cast<bool>(noyau))
		{
			piege = noyau->_memoire->_piege;
		}
		environnement->notification_debugueur_changement_memoire(piege);
	}
	return retour;
}
