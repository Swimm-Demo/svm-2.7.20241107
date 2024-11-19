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

#include <src/machine/elements/memoire/element.h>
#include <src/machine/elements/memoire/memoire.h>

using namespace SVM::Machine::Element::Memoire;

void Element::ajoute_reference_memoire(const MemoireWP& memoire, const size_t adresse) // pour debugueur
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto m = memoire.lock();
	auto it = std::find_if(_memoire.begin(),_memoire.end(),[&m,adresse](const std::pair<MemoireWP,size_t>& e)
		{
			return (m==e.first.lock()) and (e.second==adresse);
		});
	if(it!=_memoire.end()) return;
	_memoire.push_back(std::make_pair(memoire,adresse));
}

void Element::enleve_reference_memoire(const MemoireWP& memoire, const size_t adresse) // pour debugueur
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto m = memoire.lock();
	auto it = std::find_if(_memoire.begin(),_memoire.end(),[&m,adresse](const std::pair<MemoireWP,size_t>& e)
		{
			return (m==e.first.lock()) and (e.second==adresse);
		});
	if(it==_memoire.end()) return;
	_memoire.erase(it);
}

void Element::notification_debugueur_changement_memoire(const PiegeMemoireSP& piege) // pour debugueur
{
	std::lock_guard<std::mutex> verrou(_protection);
	for(const auto& m: _memoire)
	{
		auto memoire = m.first.lock();
		if(not memoire) continue;
		memoire->_piege->point_arret_changement(SVM_Valeur::AdresseMemoire(m.second),_type,_valeur,piege);
		memoire->_piege->transformation(PiegeMemoireElement(m.second,std::make_shared<Type>(_type),_valeur));
	}
}

void Element::notification_debugueur_changement_memoire(const PiegeMemoireSP& piege, const MemoireWP& memoire, const size_t adresse) // pour debugueur
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto mm = memoire.lock();
	for(const auto& m: _memoire)
	{
		auto memoire = m.first.lock();
		if(not memoire) continue;
		if(not ((mm == memoire) and (m.second==adresse)))
		{
			memoire->_piege->point_arret_changement(SVM_Valeur::AdresseMemoire(m.second),_type,_valeur,piege);
		}
		memoire->_piege->transformation(PiegeMemoireElement(m.second,std::make_shared<Type>(_type),_valeur));
	}
}
