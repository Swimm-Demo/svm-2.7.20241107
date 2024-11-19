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

#include <src/machine/elements/ordonnanceur/ordonnanceur_extension.h>
#include <src/machine/extensions/gestionnaire/gestionnaire.h>
#include <src/machine/elements/systeme/systeme.h>
#include <src/machine/machine/machine.h>
using namespace SVM::Machine::Element::Ordonnanceur;

OrdonnanceurSP OrdonnanceurExtension::creation_ordonnanceur(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme)
{
	auto ordonnanceur = std::make_shared<OrdonnanceurExtension>(nom,systeme);
	ordonnanceur->initialisation();
	auto machine = systeme->_machine.lock();
	if(static_cast<bool>(machine))
	{
		ordonnanceur->_piege = SVM_Debugueur::Debugueur::ajoute_piege<PiegeOrdonnanceur>(machine->_base->_debugueur,ordonnanceur);
	}
	return ordonnanceur;
}

OrdonnanceurExtension::OrdonnanceurExtension(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme)
:Ordonnanceur(nom,systeme)
{
	_machine = systeme->_machine;
	auto machine = _machine.lock();
	_variables = systeme->variables_extension(nom->nom_extension());
	_patron = machine->_base->_gestionnaire_extensions->ordonnanceur(*nom);
	if(not _patron)
	{
		throw OrdonnanceurNonDefini(nom);
	}
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	try
	{
		_ordonnanceur = _patron->_creation(&environnement);
	}
	catch(const SVM_Valeur::ExceptionExecution& e)
	{
		std::ostringstream oss;
		oss << "SCHEDULER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object creation: " << e;
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		oss << "SCHEDULER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object creation: " << i;
	}
	catch(const std::exception& e)
	{
		std::ostringstream oss;
		oss << "SCHEDULER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object creation: " << e.what();
	}
	catch(...)
	{
		std::ostringstream oss;
		oss << "SCHEDULER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object creation: Unknown error" ;
	}
}

OrdonnanceurExtension::~OrdonnanceurExtension()
{
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables,true,true,machine->_base->_gestionnaire_extensions->_nettoyage);
	std::lock_guard<std::mutex> verrou(_protection);
	try
	{
		_patron->_destruction(&environnement,_ordonnanceur);
	}
	catch(const SVM_Valeur::ExceptionExecution& e)
	{
		std::ostringstream oss;
		oss << "SCHEDULER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << e;
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		oss << "SCHEDULER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << i;
	}
	catch(const std::exception& e)
	{
		std::ostringstream oss;
		oss << "SCHEDULER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << e.what();
	}
	catch(...)
	{
		std::ostringstream oss;
		oss << "SCHEDULER " << *_nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: Unknown error" ;
	}
}

size_t OrdonnanceurExtension::ordonnancement(const SVM_Processus::ProcessusSP& processus, const SVM_Processus::Etat::Execution etat_processus)
{
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	const void *p = environnement->ajout_avatar(std::make_shared<SVM_Interface::VariableProcessus>(processus));
	environnement->_variable_ordonnanceur = environnement->ajout(this->shared_from_this());
	SVM_Process_State e;
	switch(etat_processus)
	{
		case SVM_Processus::Etat::Execution::EXECUTION: e = RUNNING; break;
		case SVM_Processus::Etat::Execution::SUSPENDU: e = SUSPENDED; break;
		case SVM_Processus::Etat::Execution::ATTENTE: e = WAITING; break;
		case SVM_Processus::Etat::Execution::BLOQUE: e = LOCKED; break;
		case SVM_Processus::Etat::Execution::INVESTIGATION: e = DEBUG; break;
		case SVM_Processus::Etat::Execution::ZOMBIE: e = ZOMBIE; break;
		case SVM_Processus::Etat::Execution::INTERROMPU: e = INTERRUPTED; break;
		case SVM_Processus::Etat::Execution::ERREUR:
		default:
			e = ERROR;
			break;
	}
	auto r = _patron->_ordonnance(&environnement,_ordonnanceur,p,e);
	return r;
}

size_t OrdonnanceurExtension::notification(const SVM_Synchronisation::AdresseSP& source, const SVM_Notification_Type type, const size_t parametre)
{
	if(not _patron->_notification)
		return 0;
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	environnement->_variable_ordonnanceur = environnement->ajout(this->shared_from_this());
	auto r = _patron->_notification(&environnement,_ordonnanceur,type,parametre);
	return r;
}

bool OrdonnanceurExtension::attache_processus(const SVM_Processus::ProcessusSP& processus, const size_t parametre)
{
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	const void *p = environnement->ajout_avatar(std::make_shared<SVM_Interface::VariableProcessus>(processus));
	std::lock_guard<std::mutex> verrou(_protection);
	environnement->_variable_ordonnanceur = environnement->ajout(this->shared_from_this());
	auto r = _patron->_attache(&environnement,_ordonnanceur,p,parametre)==SVM_Boolean::TRUE;
	return r;
}

bool OrdonnanceurExtension::detache_processus(const SVM_Processus::ProcessusSP& processus, const size_t parametre)
{
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	const void *p = environnement->ajout_avatar(std::make_shared<SVM_Interface::VariableProcessus>(processus));
	environnement->_variable_ordonnanceur = environnement->ajout(this->shared_from_this());
	std::lock_guard<std::mutex> verrou(_protection);
	auto r = _patron->_detache(&environnement,_ordonnanceur,p,parametre)==SVM_Boolean::TRUE;
	return r;
}

void OrdonnanceurExtension::affiche(std::ostream& os) const
{
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,_variables);
	std::lock_guard<std::mutex> verrou(_protection);
	SVM_String s = _patron->_affiche(&environnement,_ordonnanceur);
	os << std::string(s.string,s.size);
}


