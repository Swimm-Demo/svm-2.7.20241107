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

#pragma once
#include <src/global/global.h>

#include <src/machine/debugueur/json.h>
#include <src/machine/elements/synchronisation/evenement.h>
#include <src/machine/debugueur/evenement.h>

namespace SVM
{
namespace Machine
{
namespace Debugueur
{
DECL_SHARED_PTR(Debugueur);
DECL_WEAK_PTR(Debugueur);

DECL_WEAK_PTR(Piege);
DECL_SHARED_PTR(Piege);

struct Piege : public std::enable_shared_from_this<Piege>
{
	friend struct Debugueur;
	virtual ~Piege();
	const std::string& type() const { return _type; }
	size_t& identifiant() { return _identifiant; }
	std::string description() const
	{
		std::lock_guard<std::mutex> verrou(_protection);
		return _description;
	}
	void notification_changement_objet();
	void notification_evenement_objet(const Evenement::Categorie categorie, JSON_Interne::ObjetSP& evenement);
	void notification_fin();
	virtual JSON_Interne::ObjetSP objet()
	{
		auto objet = mise_a_jour();
		objet->ajoute("type",JSON::chaine(_type));
		objet->ajoute("identifier",JSON::entier(_identifiant));
		return objet;
	}
	virtual JSON_Interne::ObjetSP mise_a_jour() = 0;
	virtual void cascade() {};
	virtual void evenement(const JSON_Interne::ObjetSP& evenement) {};
	void nom_flux(std::ostream& os) const
	{
		os << "TRAP " << _type << " " << _identifiant;
	}
	void nom_flux(SVM::Global::Print& os) const
	{
		os << "TRAP " << _type << " " << _identifiant;
	}
	protected:
		explicit Piege(const std::string& type)
			:_type(type) { }
		DebugueurSP debugueur()
		{
			return _debugueur.lock();
		}

		std::string _type;
		size_t _identifiant;
	private:
		SVM::Machine::Element::Synchronisation::AdresseSP _adresse;
		mutable std::mutex _protection;
		std::string _description;
		DebugueurWP _debugueur;
};
	struct PasDeMiseAJour
	{
	};
}
}
}
