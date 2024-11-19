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

#include <src/machine/elements/ordonnanceur/ordonnanceur_defaut.h>
#include <src/machine/elements/systeme/systeme.h>
#include <src/machine/machine/machine.h>
using namespace SVM::Machine::Element::Ordonnanceur;

OrdonnanceurSP OrdonnanceurDefaut::creation_ordonnanceur(const SVM::Machine::Element::Systeme::SystemeSP& systeme)
{
	auto ordonnanceur = std::make_shared<OrdonnanceurDefaut>(systeme);
	ordonnanceur->initialisation();
	auto machine = systeme->_machine.lock();
	if(static_cast<bool>(machine))
	{
		ordonnanceur->_piege = SVM_Debugueur::Debugueur::ajoute_piege<PiegeOrdonnanceur>(machine->_base->_debugueur,ordonnanceur);
	}
	return ordonnanceur;
}
