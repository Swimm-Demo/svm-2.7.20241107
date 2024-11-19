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

#include <src/machine/machine/demarrage.h>
#include <src/machine/elements/programme/analyseur/analyseur.h>
#include <src/machine/extensions/gestionnaire/gestionnaire.h>

using namespace SVM::Machine;

SVM_Valeur::CodeSP Fichier::analyse(const SVM::Machine::MachineSP& machine) const
{
	return SVM::Machine::Element::Programme::Analyseur::analyse_fichier(_nom,_source,machine);
}

SVM_Valeur::CodeSP Texte::analyse(const SVM::Machine::MachineSP& machine) const
{
	return SVM::Machine::Element::Programme::Analyseur::analyse_chaine(_nom,_source,machine);
}

void Processus::compile(const SVM::Machine::MachineSP& machine)
{
	_code = _source->analyse(machine);
}

void Memoire::initialise(SVM_Memoire::MemoireSP& memoire) const
{
	for(const auto& a: _memoire)
	{
		SVM_Memoire::DescriptionBloc bloc(a.second.first,a.second.second.size(),a.first);
		auto pointeur = memoire->allocation(bloc);
		memoire->definition_alias(bloc,pointeur);
		auto adresse = pointeur.adresse();
		for(const auto& v: a.second.second)
		{
			if(static_cast<bool>(v))
			{
				memoire->ecriture(adresse,v);
			}
			++adresse;
		}
	}
}
