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
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/debugueur/json.h>

namespace SVM
{
namespace Machine
{
namespace Interface
{
	struct Outils;
}
namespace Element
{
namespace Memoire
{
	struct Memoire;
	struct PiegeMemoire;
}
namespace Programme
{
	struct AdresseConstante;
}
namespace Valeur
{

struct Pointeur;

struct AdresseHorsMemoire : public ExceptionExecution
{
	AdresseHorsMemoire(const ssize_t adresse)
	:ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
	{
		DETAILS_EXCEPTION("Address &" << adresse << " is outside memory.");
	}
	virtual ~AdresseHorsMemoire() {};
};

struct AdresseMemoire
{
	friend struct SVM::Machine::Element::Programme::AdresseConstante;
	friend struct SVM::Machine::Element::Memoire::Memoire;
	friend struct SVM::Machine::Element::Memoire::PiegeMemoire;
	friend struct SVM::Machine::Interface::Outils;
	friend struct Pointeur;
	explicit AdresseMemoire(const size_t adresse)
	:_adresse(adresse) {}
	AdresseMemoire operator+(const ssize_t decalage) const
	{
		if((static_cast<ssize_t>(_adresse)+decalage)<0)
		{
			throw AdresseHorsMemoire(static_cast<ssize_t>(_adresse)+decalage);
		}
		return AdresseMemoire(_adresse+decalage);
	}
	void operator++()
	{
		++_adresse;
	}
	ssize_t operator-(const AdresseMemoire& adresse) const
	{
		return _adresse-adresse._adresse;
	}
	bool operator==(const AdresseMemoire& a) const
	{
		return _adresse==a._adresse;
	}
	bool operator<(const AdresseMemoire& a) const
	{
		return _adresse<a._adresse;
	}
	bool operator>(const AdresseMemoire& a) const
	{
		return _adresse>a._adresse;
	}
	bool operator>=(const AdresseMemoire& a) const
	{
		return _adresse>=a._adresse;
	}
	bool operator>(const size_t& a) const
	{
		return _adresse>a;
	}
	bool operator>=(const size_t& a) const
	{
		return _adresse>=a;
	}
	AdresseMemoire& operator+=(const ssize_t decalage)
	{
		_adresse += decalage;
		return *this;
	}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const AdresseMemoire& adresse)
	{
		f << "&" << adresse._adresse;
		return f;
	}
	private:
	size_t _adresse;
};

struct AdresseHorsPointeur : public ExceptionExecution
{
	AdresseHorsPointeur(const ssize_t indice, const AdresseMemoire& adresse, const size_t taille)
	:ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
	{
		DETAILS_EXCEPTION("Index " << indice << " is outside pointer " << adresse << "*" << taille << "." );
	}
	virtual ~AdresseHorsPointeur() {};
};

DECL_SHARED_PTR(Pointeur);

struct Pointeur
{
	friend struct SVM::Machine::Element::Memoire::Memoire;
	Pointeur(const AdresseMemoire& adresse, const size_t taille)
	:_adresse(adresse), _taille(taille) {}

	const AdresseMemoire& adresse() const { return _adresse; }
	size_t taille() const { return _taille; }
	AdresseMemoire apres() const { return _adresse+_taille; }
	AdresseMemoire element(const ssize_t indice) const
	{
		if(not interieur(indice))
		{
			throw AdresseHorsPointeur(indice,_adresse,_taille);
		}
		return _adresse+indice;
	}
	bool interieur(const ssize_t indice) const
	{
		return (indice>=0) and (indice<static_cast<ssize_t>(_taille));
	}
	bool interieur(const AdresseMemoire& adresse) const
	{
		return (adresse>=_adresse) and (adresse<(_adresse+_taille));
	}
	bool operator<(const Pointeur& p) const
	{
		if(not (_adresse==p._adresse))
		{
			return _adresse<p._adresse;
		}
		return _taille<p._taille;
	}
	bool operator>(const Pointeur& p) const
	{
		if(not (_adresse==p._adresse))
		{
			return _adresse>p._adresse;
		}
		return _taille>p._taille;
	}
	bool colle(const SVM_Valeur::Pointeur& bloc)
	{
		if(_adresse+_taille==bloc._adresse)
		{
			_taille+=bloc._taille;
			return true;
		}
		return false;
	}
	bool inclus(const Pointeur& p) const
	{
		return (p._adresse._adresse>=_adresse._adresse) and ((p._adresse._adresse+p._taille)<=(_adresse._adresse+_taille));
	}
	bool intersecte(const Pointeur& p) const
	{
		return not ((p._adresse._adresse+p._taille<=_adresse._adresse) or (_adresse._adresse+_taille<=p._adresse._adresse));
	}
	std::pair<Pointeur,Pointeur> evide(const Pointeur& p) const
	{
		if((p._adresse._adresse+p._taille<=_adresse._adresse) or (_adresse._adresse+_taille<=p._adresse._adresse))
			return std::make_pair(Pointeur(*this),Pointeur(AdresseMemoire(0),0));
		if((p._adresse._adresse<=_adresse._adresse) and (p._adresse._adresse+p._taille<=_adresse._adresse+_taille))
			return std::make_pair(Pointeur(p._adresse+p._taille,_adresse._adresse+_taille-(p._adresse._adresse+p._taille)),Pointeur(AdresseMemoire(0),0));
		if((p._adresse._adresse<=_adresse._adresse+_taille) and (p._adresse._adresse+p._taille>=_adresse._adresse+_taille))
			return std::make_pair(Pointeur(_adresse,p._adresse._adresse-_adresse._adresse),Pointeur(AdresseMemoire(0),0));
		return std::make_pair(Pointeur(_adresse,p._adresse._adresse-_adresse._adresse),Pointeur(p._adresse+p._taille,_adresse._adresse+_taille-(p._adresse._adresse+p._taille)));
	}
	bool vide() const { return _taille==0; }
	bool operator==(const Pointeur& p) const
	{
		return (_adresse==p._adresse) and (_taille==p._taille);
	}
	bool operator!=(const Pointeur& p) const
	{
		return not ((*this)==p);
	}
	void decalage(const ssize_t decalage)
	{
		_adresse += decalage;
	}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const Pointeur& pointeur)
	{
		f << pointeur._adresse << "*" << pointeur._taille;
		return f;
	}
	SVM::Machine::Debugueur::JSON_Interne::ObjetSP debugueur() const
	{
		auto p = SVM::Machine::Debugueur::JSON::objet();
		p->ajoute("address",SVM::Machine::Debugueur::JSON::entier(_adresse._adresse));
		p->ajoute("size",SVM::Machine::Debugueur::JSON::entier(_taille));
		return p;
	}
	private:
	AdresseMemoire _adresse;
	size_t _taille;
};
}
}
}
}
