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

#include <src/machine/elements/memoire/extension.h>
#include <src/machine/machine/machine.h>
#include <src/machine/extensions/patron_objet_extension.h>
#include <src/machine/machine/historique.h>
using namespace SVM::Machine::Element::Memoire;

Extension::~Extension()
{
	destruction();
}

void Extension::destruction()
{
	auto machine = _machine.lock();
	auto gestionnaire_extensions = machine->_base->_gestionnaire_extensions;
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,gestionnaire_extensions->variables_extension(_type.nom_extension()),true,true,gestionnaire_extensions->_nettoyage);
	try
	{
		_patron->_destructeur(&environnement,_valeur);
	}
	catch(const SVM_Valeur::ExceptionExecution& e)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << e;
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << i;
	}
	catch(const std::exception& e)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: " << e.what();
	}
	catch(...)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object destruction: Unknown error" ;
	}
}

void Extension::format(std::ostream& os) const
{
	if(not _patron->_affichage)
	{
		os << "<plugin object " << _valeur << ">";
		//throw TypePasDAffichage(_type);
		return;
	}
	auto machine = _machine.lock();
	auto gestionnaire_extensions = machine->_base->_gestionnaire_extensions;
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,gestionnaire_extensions->variables_extension(_type.nom_extension()));
	try
	{
		SVM_String v = _patron->_affichage(&environnement,_valeur);
		std::string s(v.string,v.size);
		os << s;
	}
	catch(const SVM_Valeur::ExceptionExecution& e)
	{
		os << "<error: " << e << ">";
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		os << "<interruption: " << i << ">";
	}
	catch(const std::exception& e)
	{
		os << "<error: " << e.what() << ">";
	}
	catch(...)
	{
		os << "<unknown error>";
	}
}

ValeurSP Extension::copie() const
{
	void *v = copie_interne();
	auto machine = _machine.lock();
	return std::make_shared<Extension>(machine,_type,_patron,v);
}

void* Extension::copie_interne() const
{
	if(not _patron->_copie)
	{
		throw TypePasDeCopie(_type);
	}
	auto machine = _machine.lock();
	auto gestionnaire_extensions = machine->_base->_gestionnaire_extensions;
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,gestionnaire_extensions->variables_extension(_type.nom_extension()));
	try
	{
		void *v = _patron->_copie(&environnement,_valeur);
		return v;
	}
	catch(const SVM_Valeur::ExceptionExecution& e)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object copy: " << e;
		throw;
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object copy: " << i;
		throw;
	}
	catch(const std::exception& e)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object copy: " << e.what();
		throw;
	}
	catch(...)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object copy: Unknown error" ;
		throw;
	}
	return nullptr;
}

SVM::Machine::Element::Valeur::Comparaison Extension::compare(const Valeur& droite) const
{
	const Extension *d=dynamic_cast<const Extension*>(&droite);                                                                                                  
	if(not _patron->_comparaison)
	{
		return SVM::Machine::Element::Valeur::Comparaison::comparaison_ordre_total(_valeur,d->_valeur,true);
	}
	auto machine = _machine.lock();
	auto gestionnaire_extensions = machine->_base->_gestionnaire_extensions;
	SVM_Interface::EnvironnementSP environnement = std::make_shared<SVM_Interface::Environnement>(_patron,machine,gestionnaire_extensions->variables_extension(_type.nom_extension()));
	try
	{
		unsigned char comparaison = _patron->_comparaison(&environnement,_valeur,d->_valeur);
		SVM::Machine::Element::Valeur::Comparaison c;
		c._egal = (comparaison & 0x3) == 0x0;
		c._inferieur = (comparaison & 0x3) == 0x1;
		c._superieur = (comparaison & 0x3) == 0x2;
		c._ordre = (comparaison & 0x4) or c._inferieur or c._superieur;
		if(c._ordre)
		{
			c._total = not (comparaison & 0x8) and not ((comparaison & 0x3) == 0x3);
		}
		c._faible = comparaison & 0x10;
		return c;
	}
	catch(const SVM_Valeur::ExceptionExecution& e)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object compare: " << e;
		throw;
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object compare: " << i;
		throw;
	}
	catch(const std::exception& e)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object compare: " << e.what();
		throw;
	}
	catch(...)
	{
		std::ostringstream oss;
		oss << "TYPE " << _type;
		SVM::Machine::Trace(machine->_historique,oss.str()) << "Error in plugin object compare: Unknown error" ;
		throw;
	}
	return SVM::Machine::Element::Valeur::Comparaison();
}       
