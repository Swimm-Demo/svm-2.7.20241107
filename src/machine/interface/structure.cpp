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

#include <src/machine/interface/structure.h>
#include <src/machine/machine/machine.h>
#include <src/machine/machine/historique.h>
using namespace SVM::Machine::Interface;

Structure::~Structure()
{
	auto machine = _machine.lock();
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,machine->_base->_gestionnaire_extensions->variables_extension(_nom.nom_extension()),true,true,machine->_base->_gestionnaire_extensions->_nettoyage);
	if(not _valeur)
		return;
	try
	{
		_patron->_destructeur(&environnement,_valeur);
	}
	catch(const SVM_Valeur::ExceptionExecution& e)
	{
		std::ostringstream oss;
		oss << "STRUCT " << _nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << e;
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		oss << "STRUCT " << _nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << i;
	}
	catch(const std::exception& e)
	{
		std::ostringstream oss;
		oss << "STRUCT " << _nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << e.what();
	}
	catch(...)
	{
		std::ostringstream oss;
		oss << "STRUCT " << _nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: Unknown error" ;
	}
}

StructureSP Structure::copie() const
{
	if(not _patron->_copie)
	{
		throw StructurePasDeCopie(_nom);
	}
	auto machine = _machine.lock();
	if(not _valeur)
	{
		return std::make_shared<Structure>(machine,_nom,_patron);
	}
	try
	{
		SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,machine->_base->_gestionnaire_extensions->variables_extension(_nom.nom_extension()));
		void *v = _patron->_copie(&environnement,_valeur);
		return std::make_shared<Structure>(machine,_nom,_patron,v);
	}
	catch(const SVM_Valeur::ExceptionExecution& e)
	{
		std::ostringstream oss;
		oss << "STRUCT " << _nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object copy: " << e;
		throw;
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		oss << "STRUCT " << _nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object copy: " << i;
		throw;
	}
	catch(const std::exception& e)
	{
		std::ostringstream oss;
		oss << "STRUCT " << _nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object copy: " << e.what();
		throw;
	}
	catch(...)
	{
		std::ostringstream oss;
		oss << "STRUCT " << _nom;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object copy: Unknown error" ;
		throw;
	}
	return StructureSP();
}
