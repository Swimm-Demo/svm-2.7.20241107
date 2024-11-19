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

#include <src/machine/debugueur/piege.h>
#include <src/machine/debugueur/debugueur.h>

using namespace SVM::Machine::Debugueur;

Piege::~Piege()
{
	auto debugueur = _debugueur.lock();
	if(not debugueur) return;
	debugueur->enleve_piege(_identifiant,_adresse);
	debugueur->notification_changement_index(_type);
	debugueur->notification_suppression_objet(_identifiant);
}

void Piege::notification_changement_objet()
{
	try
	{
		auto o = objet();
		o->ajoute("command",JSON::chaine("object"));

		std::lock_guard<std::mutex> verrou(_protection);
		_description = JSON::serialise(o);
		auto debugueur = _debugueur.lock();
		debugueur->notification_changement_objet(this->shared_from_this());
		cascade();
	}
	catch(PasDeMiseAJour e)
	{
	}
}

void Piege::notification_evenement_objet(const Evenement::Categorie categorie, JSON_Interne::ObjetSP& evenement)
{
	try
	{
		auto o = objet();
		o->ajoute("command",JSON::chaine("object"));

		std::lock_guard<std::mutex> verrou(_protection);
		_description = JSON::serialise(o);
		auto debugueur = _debugueur.lock();
		auto p = this->shared_from_this();

		debugueur->notification_evenement_objet(p,categorie,evenement);
		cascade();
	}
	catch(PasDeMiseAJour e)
	{
	}
}

void Piege::notification_fin()
{
	auto d = _debugueur.lock();
	if(static_cast<bool>(d)) { d->notification_fin(); }
}
