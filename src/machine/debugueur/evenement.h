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

namespace SVM
{
namespace Machine
{
namespace Debugueur
{
DECL_SHARED_PTR(Evenement);

struct Evenement
{
	enum class Categorie
	{
		FIN,
		STATUS_OBJET,
		CHANGEMENT_OBJET,
		STATUS_INDEX,
		POINT_ARRET,
		COMMANDE,
		RESULTAT
	};
	Evenement()
	:_categorie(Categorie::FIN), _identifiant(0) {}
	Evenement(const Categorie categorie, const std::string& texte)
	:_categorie(categorie), _texte(texte), _identifiant(0) {}
	Evenement(const Categorie categorie, const std::string& texte, const size_t identifiant)
	:_categorie(categorie), _texte(texte), _identifiant(identifiant) {}
	Evenement(const Categorie categorie, const std::string& texte, const std::string& type)
	:_categorie(categorie), _texte(texte), _identifiant(0), _type(type) {}
	Evenement(const Categorie categorie, const JSON_Interne::ObjetSP& objet)
	:_categorie(categorie), _objet(objet), _identifiant(0) {}
	Categorie _categorie;
	std::string _texte;
	JSON_Interne::ObjetSP _objet;
	size_t _identifiant;
	std::string _type;
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const Evenement& e)
	{
		switch(e._categorie)
		{
			case Categorie::FIN: f << "END"; break;
			case Categorie::STATUS_OBJET: f << "STATUS_OBJET"; break;
			case Categorie::CHANGEMENT_OBJET: f << "CHANGE_OBJET"; break;
			case Categorie::POINT_ARRET: f << "BREAK_POINT"; break;
			case Categorie::COMMANDE: f << "COMMAND"; break;
			case Categorie::RESULTAT: f << "RESULT"; break;
		}
		f << " ";
		if(static_cast<bool>(e._objet))
		{
			f << JSON::serialise(e._objet);
		}
		else
		{
			f << '"' << e._texte << '"';
		}
		if(e._identifiant>0)
		{
			f << " [I:" << e._identifiant << "]";
		}
		if(not e._type.empty())
		{
			f << " [T:" << e._type << "]";
		}
		return f;
	}
};

DECL_SHARED_PTR(Livraison);

struct Livraison : public SVM::Machine::Element::Synchronisation::Livraison<Evenement>
{
};

}
}
}
