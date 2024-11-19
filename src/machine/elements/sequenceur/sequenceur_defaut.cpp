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

#include <src/machine/elements/sequenceur/sequenceur_defaut.h>
#include <src/machine/elements/processus/processus.h>

using namespace SVM::Machine::Element::Sequenceur;

bool SequenceurDefaut::attache_noyau(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& env_parent, const size_t argc, SVM_Parameter *argv)
{
	if(not _noyau)
	{
		_noyau = noyau;
		_noyau->_sequenceur = this->shared_from_this();
		_noyau->_processus = _processus;
		_processus.lock()->ajoute_noyau(noyau);
		if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		if(static_cast<bool>(noyau->_piege)) { noyau->_piege->notification_changement_objet(); }
		return true;
	}
	return false;
}

bool SequenceurDefaut::detache_noyau(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& env_parent, const size_t argc, SVM_Parameter *argv)
{
	if(static_cast<bool>(_noyau) and (noyau==_noyau))
	{
		auto processus = _processus.lock();
		processus->enleve_noyau(noyau);
		_noyau->_processus.reset();
		_noyau->_sequenceur.reset();
		_noyau.reset();
		if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		if(static_cast<bool>(noyau->_piege)) { noyau->_piege->notification_changement_objet(); }
		return true;
	}
	return false;
}
