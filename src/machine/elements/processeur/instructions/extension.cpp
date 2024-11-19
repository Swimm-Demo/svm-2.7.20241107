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

#include <src/machine/elements/processeur/instructions/extension.h>
#include <src/machine/elements/programme/valeur_programme.h>
#include <src/machine/machine/machine.h>
#include <src/machine/elements/ordonnanceur/ordonnanceur.h>
#include <src/machine/elements/processus/processus.h>

using namespace SVM::Machine::Element::Processeur::Instruction;
namespace SVM_Programme = SVM::Machine::Element::Programme;

void Extension::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_nom_instruction->decoration(decorateur);
	for(auto& p:_parametres)
	{
		p->decoration(decorateur);
	}
	SVM_Programme::PointEntreeExtensionSP nom = std::dynamic_pointer_cast<SVM_Programme::PointEntreeExtension>(_nom_instruction);
	if(static_cast<bool>(nom))
	{
		_patron = decorateur._machine->_base->_gestionnaire_extensions->instruction(nom->_pep->_valeur);
		if(not _patron)
		{
			throw InstructionInexistante(nom->_pep->_valeur);
		}
		if(not decorateur._remplacement and _patron->_remplacement)
		{
			throw InstructionRemplacement(nom->_pep->_valeur);
		}
		_systeme = _patron->_systeme;
		_attente = _patron->_attente;
	}
	else
	{
		// le plus restrictif en cas d'instruction dynamique (aka interdit en mode protege)
		_systeme = true;
		_attente = true;
	}
	sans_position(decorateur);
}

SVM_Memoire::ValeurSP Extension::execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Valeur::PointEntreeExtension nom_instruction = _nom_instruction->evaluation_point_entree_extension(noyau);
	SVM_Extension::PatronInstructionExtensionSP patron = _patron;
	if(not patron)
	{
		patron = noyau->machine()->_base->_gestionnaire_extensions->instruction(nom_instruction);
		if(not patron)
		{
			throw InstructionInexistante(nom_instruction);
		}
		if(patron->_remplacement)
		{
			throw InstructionRemplacement(nom_instruction);
		}
	}
	std::vector<SVM_Valeur::ParametreSP> parametres;
	for(auto& p: _parametres)
	{
		parametres.push_back(p->evaluation(noyau));
	}
	SVM_Memoire::ValeurSP retour = execution_avec_resultat(noyau,nom_instruction,patron,parametres,std::make_shared<SVM_Valeur::PositionInstruction>(_position),_attente,_systeme);
	return retour;
}

struct MemoireExtension
{
	MemoireExtension(const void **parametres)
	:_parametres(parametres) {}
	~MemoireExtension()
	{
		if(_parametres!=nullptr)
		{
			delete [] _parametres;
		}
	}
	const void **_parametres;
};

SVM_Memoire::ValeurSP Extension::execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau, const SVM_Valeur::PointEntreeExtension& nom_instruction, const SVM_Extension::PatronInstructionExtensionSP instruction, const std::vector<SVM_Valeur::ParametreSP>& parametres, const SVM_Valeur::PositionInstructionSP& position, const bool attente, const bool systeme)
{
	std::vector<SVM_Extension::RegexParametresElementCSP> types;
	for(auto& p: parametres)
	{
		types.push_back(p->type());
	}
	instruction->verifie_parametres(types);
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(instruction,noyau->machine(),noyau->variables_extension(nom_instruction.nom_extension()),attente,systeme);
	environnement->_variable_noyau = environnement->ajout(noyau);
	environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM::Machine::Interface::VariableProcessus>(noyau->_processus.lock()));
	environnement->_variable_ordonnanceur = environnement->ajout(noyau->_processus.lock()->_ordonnanceur.lock());
	environnement->_position = position;
	::MemoireExtension memoire(new const void*[parametres.size()]);
	size_t indice = 0;
	for(auto& p: parametres)
	{
		memoire._parametres[indice++] = environnement->ajout<SVM_Valeur::Parametre>(p);
	}
	void* retour_brut = instruction->_instruction(&environnement,parametres.size(),memoire._parametres);
	SVM_Memoire::ValeurSP retour;
	if(retour_brut!=nullptr)
	{
		SVM_Memoire::ValeurInterfaceSP valeur_interface = environnement->valeur<SVM_Memoire::ValeurInterface>(retour_brut);
		if(static_cast<bool>(valeur_interface))
		{
			retour = valeur_interface->_valeur;
			if(retour.use_count()==2)
			{	// optimisation + utile pour valeur extensions sans copie
				retour->deplacable();
			}
		}
	}
	if(static_cast<bool>(retour))
	{
		SVM_Memoire::Type type_retour = *retour;
		instruction->verifie_retour(std::make_shared<const SVM_Extension::RegexParametresValeur>(type_retour),retour->nulle());
	}
	else
	{
		instruction->verifie_retour(SVM_Extension::RegexParametresValeurSP());
	}
	if(static_cast<bool>(noyau->debugueur()))
	{
		environnement->notification_debugueur_changement_memoire(noyau->_memoire->_piege);
	}
	return retour;
}

void Extension::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream nom;
	_nom_instruction->explique(noyau,nom);
	explications.push_back(nom.str());
	for(auto& p: _parametres)
	{
		auto v = std::dynamic_pointer_cast<SVM_Programme::ParametreValeur>(p);
		if(not v) continue;
		v->explication(noyau,explications);
	}
}
