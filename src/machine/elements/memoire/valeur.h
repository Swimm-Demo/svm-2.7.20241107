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
#include <src/machine/elements/memoire/type.h>
#include <src/machine/elements/valeur/comparaison.h>

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Memoire
{
DECL_SHARED_PTR(PiegeMemoire);
DECL_WEAK_PTR(Element);

struct Memoire;

DECL_SHARED_PTR(Valeur);
struct Valeur : public std::enable_shared_from_this<Valeur>
{
	friend struct Memoire;
	Valeur()
	:_constante(false), _deplacable(false), _notification(false) {};
	virtual ~Valeur() {};
	virtual ValeurSP evaluation()
	{
		if(_constante)
		{
			return this->copie();
		}
		return this->shared_from_this();
	};
	virtual ValeurSP copie() const = 0;
	virtual	ValeurSP operator+(const long long int increment) { throw; }
	virtual	void operator+=(const long long int increment) { throw; }
	static SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& gauche, const Valeur& droite)
	{
		SVM::Machine::Element::Valeur::Comparaison c = SVM::Machine::Element::Valeur::Comparaison::comparaison_ordre_total(static_cast<Type>(gauche),static_cast<Type>(droite),true);
		if(not c._egal)
		{
			c._type = true;
			c._nul = gauche.nulle() or droite.nulle();
			return c;
		}
		c._nul = true;
		if(gauche.nulle() and droite.nulle())
		{
			return c;
		}
		c._egal = false;
		if(gauche.nulle())
		{
			c._inferieur = true;
			return c;
		}
		if(droite.nulle())
		{
			c._superieur = true;
			return c;
		}
		return gauche.compare(droite);
	}
	virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const = 0;
	bool operator==(const Valeur& droite) const
	{
		return compare(droite)._egal;
	}
	virtual std::string serialise() const
	{
		std::ostringstream oss;
		oss << (*this);
		return oss.str();
	}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const Valeur& v)
	{
		v.format(f);
		return f;
	}
	virtual void format(std::ostream& os) const = 0;
	virtual operator Type () const = 0;
	void constante() { _constante = true; }
	bool est_constante() const { return _constante; }
	void deplacable() { if(_constante or nulle()) return; _deplacable = true; }
	bool est_deplacable() const { return _deplacable; }
	virtual bool nulle() const { return false; }
	void ajoute_debugueur_element(const ElementWP& element);
	void enleve_debugueur_element(const ElementWP& element);
	void notification_debugueur_changement_memoire(const PiegeMemoireSP& piege);
	protected:
		bool _constante; // modifie depuis analyseur programme
	private:
		bool _deplacable;
	public:
		mutable std::mutex _protection;
		std::list<ElementWP> _element;
		bool _notification; // pour debugueur
};

struct ValeurNulle : public Valeur
{
	ValeurNulle(const Type& type)
	:_type(type) {}
	virtual ~ValeurNulle() {}
	virtual ValeurSP copie() const override { return std::make_shared<ValeurNulle>(_type); }
	virtual SVM::Machine::Element::Valeur::Comparaison compare(const Valeur& droite) const override
	{
		throw;
	}
	virtual operator Type () const override { return _type; }
	virtual bool nulle() const override { return true; }
	virtual void format(std::ostream& os) const override {}
	private:
		Type _type;
};

DECL_SHARED_PTR(ValeurInterface);
struct ValeurInterface
{
	ValeurInterface(const ValeurSP& valeur)
	:_valeur(valeur) {}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const ValeurInterface& v)
	{
		f << v._valeur->serialise();
		return f;
	}
	ValeurSP _valeur;
};

}
}
}
}
