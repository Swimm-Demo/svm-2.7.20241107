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

#include <src/machine/elements/sequenceur/sequenceur_extension.h>
#include <src/machine/machine/machine.h>
#include <src/machine/elements/processus/processus.h>
#include <src/machine/elements/systeme/systeme.h>
using namespace SVM::Machine::Element::Sequenceur;

SequenceurExtension::SequenceurExtension(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM_Processus::ProcessusSP& processus)
:Sequenceur(nom,processus)
{
	_machine = _processus.lock()->_systeme.lock()->_machine;
	auto machine = _machine.lock();
	_variables = _processus.lock()->_systeme.lock()->variables_extension(nom->nom_extension());
	_patron = machine->_base->_gestionnaire_extensions->sequenceur(*nom);
	if(not _patron)
	{
		throw SequenceurNonDefini(nom);
	}
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM_Interface::VariableProcessus>(_processus.lock()));
	try
	{
		_sequenceur = _patron->_creation(&environnement);
	}
	catch(const SVM_Valeur::ExceptionExecution& e)
	{
		std::ostringstream oss;
		oss << "SEQUENCER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object creation: " << e;
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		oss << "SEQUENCER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object creation: " << i;
	}
	catch(const std::exception& e)
	{
		std::ostringstream oss;
		oss << "SEQUENCER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object creation: " << e.what();
	}
	catch(...)
	{
		std::ostringstream oss;
		oss << "SEQUENCER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object creation: Unknown error" ;
	}
}

SequenceurExtension::~SequenceurExtension()
{
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables,true,true,machine->_base->_gestionnaire_extensions->_nettoyage);
	environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM_Interface::VariableProcessus>(_processus.lock()));
	try
	{
		_patron->_destruction(&environnement,_sequenceur);
	}
	catch(const SVM_Valeur::ExceptionExecution& e)
	{
		std::ostringstream oss;
		oss << "SEQUENCER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << e;
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		oss << "SEQUENCER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << i;
	}
	catch(const std::exception& e)
	{
		std::ostringstream oss;
		oss << "SEQUENCER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << e.what();
	}
	catch(...)
	{
		std::ostringstream oss;
		oss << "SEQUENCER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: Unknown error" ;
	}
}

SVM_Noyau::NoyauSP SequenceurExtension::noyau_courant()
{
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM_Interface::VariableProcessus>(_processus.lock()));
	const void *noyau = _patron->_courant(&environnement,_sequenceur);
	if(not noyau)
	{
		return SVM_Noyau::NoyauSP();
	}
	auto vrai_noyau = environnement->valeur<SVM_Noyau::Noyau>(noyau);
	if(noyau and not vrai_noyau)
	{
		throw NoyauInvalide(_nom);
	}
	return vrai_noyau;
}

bool SequenceurExtension::attache_noyau(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& env_parent, const size_t argc, SVM_Parameter *argv)
{
	if(not noyau)
		return false;
	if(not noyau->_sequenceur.expired())
	{
		throw NoyauDejaAttache(_nom);
	}
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM_Interface::VariableProcessus>(_processus.lock()));
	const void *n = environnement->ajout(noyau);
	if(argv and static_cast<bool>(env_parent))
	{
		for(size_t i=0 ; i<argc ; ++i)
		{
			env_parent->exporte(argv[i],environnement->_appellant);
		}
	}
	if(_patron->_attache(&environnement,_sequenceur,n,argc,argv)==SVM_Boolean::TRUE)
	{
		noyau->_sequenceur=this->shared_from_this();
		noyau->_processus=_processus;
		_processus.lock()->ajoute_noyau(noyau);
		if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		if(static_cast<bool>(noyau->_piege)) { noyau->_piege->notification_changement_objet(); }
		return true;
	}
	return false;
}

bool SequenceurExtension::detache_noyau(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& env_parent, const size_t argc, SVM_Parameter *argv)
{
	if(not noyau)
		return false;
	if(noyau->_sequenceur.expired())
	{
		throw NoyauDejaDetache(_nom);
	}
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM_Interface::VariableProcessus>(_processus.lock()));
	const void *n = environnement->ajout(noyau);
	if(argv and static_cast<bool>(env_parent))
	{
		for(size_t i=0 ; i<argc ; ++i)
		{
			env_parent->exporte(argv[i],environnement->_appellant);
		}
	}
	if(_patron->_detache(&environnement,_sequenceur,n,argc,argv)==SVM_Boolean::TRUE)
	{
		noyau->_sequenceur.reset();
		noyau->_processus.reset();
		_processus.lock()->enleve_noyau(noyau);
		if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		if(static_cast<bool>(noyau->_piege)) { noyau->_piege->notification_changement_objet(); }
		return true;
	}
	return false;
}

void SequenceurExtension::affiche(std::ostream& os) const
{
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM_Interface::VariableProcessus>(_processus.lock()));
	SVM_String s = _patron->_affiche(&environnement,_sequenceur);
	os << std::string(s.string,s.size);
}
