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

#include <src/machine/elements/memoire/valeur.h>
#include <src/machine/elements/memoire/element.h>

using namespace SVM::Machine::Element::Memoire;

void Valeur::ajoute_debugueur_element(const ElementWP& element) // pour debugueur
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto e = element.lock();
	auto it = std::find_if(_element.begin(),_element.end(),[&e](const ElementWP& m)
		{
			return e==m.lock();
		});
	if(it!=_element.end()) return;
	_element.push_back(element);
}

void Valeur::enleve_debugueur_element(const ElementWP& element) // pour debugueur
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto e = element.lock();
	auto it = std::find_if(_element.begin(),_element.end(),[&e](const ElementWP& m)
		{
			return e==m.lock();
		});
	if(it==_element.end()) return;
	_element.erase(it);
}

struct Notification
{
	Notification(Valeur& v)
	:_v(v)
	{
		_v._notification = true;
	}
	~Notification()
	{
		_v._notification = false;
	}
	Valeur& _v;
};

void Valeur::notification_debugueur_changement_memoire(const PiegeMemoireSP& piege) // pour debugueur
{
	// il faut mettre cette fonction idempotente, sinon dans des conditions precises, on a un deadlock :
	// Environnement -> Variable -> ValeurExtension -> contenu (contient une autre variable extension) -> affichage -> ~Environnement
	if(_notification) return;
	Notification n(*this);
	std::lock_guard<std::mutex> verrou(_protection);
	for(auto& e:_element)
	{
		auto ee = e.lock();
		if(not ee) continue;
		ee->notification_debugueur_changement_memoire(piege);
	}
}
