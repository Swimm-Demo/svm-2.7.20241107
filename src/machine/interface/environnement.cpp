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

#include <src/machine/interface/environnement.h>
#include <src/machine/elements/processus/processus.h>
#include <src/machine/elements/ordonnanceur/ordonnanceur.h>
#include <src/machine/extensions/patron_objet_extension.h>
#include <src/machine/interface/variable.h>
#include <src/machine/machine/machine.h>

using namespace SVM::Machine::Interface;

std::string Environnement::nom()
{
	std::ostringstream oss;
	if(_variable_processus)
	{
		VariableProcessusSP vp = this->valeur<VariableProcessus>(_variable_processus);
		vp->_processus->nom_flux(oss);
	}
	else if(_variable_ordonnanceur)
	{
		SVM::Machine::Element::Ordonnanceur::OrdonnanceurSP o = this->valeur<SVM::Machine::Element::Ordonnanceur::Ordonnanceur>(_variable_ordonnanceur);
		o->nom_flux(oss);
	}
	else
	{
		oss << "SYSTEM";
	}
	oss << " : ";
	_patron->nom(oss);
	return oss.str();
}

void Environnement::notification_debugueur_changement_memoire(const SVM::Machine::Element::Memoire::PiegeMemoireSP& piege)
{
	if(_notification_debugueur) return;
	_notification_debugueur = true;
	if(not _machine->_base->_debugueur) return;
	auto f = [piege] (VariableSP v)
		{
			SVM_Memoire::ValeurSP valeur;
			auto vvi = std::dynamic_pointer_cast<VariableValeur<SVM_Memoire::ValeurInterface> >(v);
			if(static_cast<bool>(vvi))
			{
				if(static_cast<bool>(vvi->_valeur))
				{
					if(static_cast<bool>(std::dynamic_pointer_cast<SVM_Memoire::Extension>(vvi->_valeur->_valeur)))
					{
						valeur = vvi->_valeur->_valeur;
					}
				}
			}
			auto vv = std::dynamic_pointer_cast<VariableValeur<SVM_Memoire::Extension> >(v);
			if(static_cast<bool>(vv) and not valeur)
			{
				valeur = vv->_valeur;
			}
			if(not valeur) return;
			valeur->notification_debugueur_changement_memoire(piege);
		};
	_appellant->applique(f);
	_extension->applique(f);
}

VariableProcessus::~VariableProcessus()
{
	if(_pur) return; // sert a eviter de tuer le processus lorsque l'avatar est inclus dans l'environnement d'appel d'une fonction extension, car celui-ci peut etre non lie a un ordonnanceur a ce moment la
	if(not _processus->termine() and _processus->_ordonnanceur.expired())
	{
		_processus->terminaison_demandee(_processus->_adresse);
	}
}
