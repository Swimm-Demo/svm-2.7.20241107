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
#include <src/machine/elements/memoire/valeur.h>
#include <src/machine/elements/synchronisation/acces.h>

namespace SVM_Memoire = SVM::Machine::Element::Memoire;
namespace SVM_Synchronisation = SVM::Machine::Element::Synchronisation;

namespace SVM
{
namespace Machine
{
namespace Interface
{
	struct Environnement;
	struct VariablesDefinies;

	DECL_WEAK_PTR(Variable);
	DECL_SHARED_PTR(Variable);

	struct Variable
	{
		friend struct VariablesDefinies;
		virtual ~Variable() { }
		bool operator<(const Variable& v) const
		{
			return _variable<v._variable;
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Variable& v)
		{
			v.format(f);
			return f;
		}
		virtual void format(std::ostream& os) const 
		{
			os << _variable;
			if(_partagee)
			{
				os << "[" << _compteur << "]";
			}
		}
		static inline const void* hachage(const void *adresse)
		{
			return adresse;
		}
		protected:
		explicit Variable(const void *variable)
		:_variable(variable), _partagee(false), _compteur(0) {}
		private:
		const void *_variable;
		bool _partagee;
		size_t _compteur;
	};

	struct VariableClef : public Variable
	{
		VariableClef(const void *v)
		:Variable(v)
		{ }
		virtual ~VariableClef() {}
	};

	template<typename T>
	struct VariableValeur : public Variable
	{
		friend struct Environnement;
		friend struct VariablesDefinies;
		VariableValeur(const void *v, const SHARED_PTR(T)& valeur)
		:Variable(v), _valeur(valeur) 
		{ }
		VariableValeur(const SHARED_PTR(T)& valeur)
		:Variable(Variable::hachage(valeur.get())), _valeur(valeur) 
		{ }
		virtual ~VariableValeur() {}
		virtual void format(std::ostream& os) const override
		{
			Variable::format(os);
			os << " => ";
			if(static_cast<bool>(_valeur))
			{
				os << (*_valeur);
			}
		}
		private:
		SHARED_PTR(T) _valeur;
	};

	DECL_SHARED_PTR(VariablesDefinies);
	struct VariablesDefinies
	{
		VariablesDefinies(const SVM_Synchronisation::AccesSP& protection = std::make_shared<SVM_Synchronisation::Acces>(false))
		:_protection(protection) {}
		virtual ~VariablesDefinies() { }
		template<typename T>
		const void* ajout(const SHARED_PTR(T)& valeur)
		{
			auto verrou = _protection->ecrivain();
			SHARED_PTR(VariableValeur<T>) v = std::make_shared<VariableValeur<T> >(valeur);
			_variables.insert(v);
			return v->_variable;
		}
		template<typename T>
		const void* ajout_avatar(const SHARED_PTR(T)& valeur)
		{
			auto verrou = _protection->ecrivain();
			SHARED_PTR(VariableValeur<T>) v = std::make_shared<VariableValeur<T> >(Variable::hachage(valeur->avatar()),valeur);
			_variables.insert(v);
			return v->_variable;
		}
		template<typename T>
		void remplace(const void *v, const SHARED_PTR(T)& valeur)
		{
			auto verrou = _protection->ecrivain();
			auto it=_variables.find(std::make_shared<VariableClef>(v));
			if(it!=_variables.end())
			{
				_variables.erase(it);
				SHARED_PTR(VariableValeur<T>) nv = std::make_shared<VariableValeur<T> >(v,valeur);
				_variables.insert(nv);
			}
		}
		void supprime(const void *v)
		{
			auto verrou = _protection->ecrivain();
			auto it=_variables.find(std::make_shared<VariableClef>(v));
			if(it!=_variables.end())
			{
				_variables.erase(it);
			}
		}
		bool teste(const void* v)
		{
			auto verrou = _protection->lecteur();
			auto it=_variables.find(std::make_shared<VariableClef>(v));
			return (it!=_variables.end());
		}
		VariableSP variable(const void *v)
		{
			auto verrou = _protection->lecteur();
			auto it=_variables.find(std::make_shared<VariableClef>(v));
			if(it!=_variables.end())
			{
				return *it;
			}
			return VariableSP();
			
		}
		template<typename T>
		SHARED_PTR(T) valeur(const void *v)
		{
			auto verrou = _protection->lecteur();
			VariableSP var = variable(v);
			if(static_cast<bool>(var))
			{
				SHARED_PTR(VariableValeur<T>) vv = std::dynamic_pointer_cast<VariableValeur<T> >(var);
				if(static_cast<bool>(vv))
				{
					return vv->_valeur;
				}
			}
			return SHARED_PTR(T)();
		}
		template<typename T>
		SHARED_PTR(T)& reference(const void *v)
		{
			auto verrou = _protection->lecteur();
			VariableSP var = variable(v);
			if(static_cast<bool>(var))
			{
				SHARED_PTR(VariableValeur<T>) vv = std::dynamic_pointer_cast<VariableValeur<T> >(var);
				if(static_cast<bool>(vv))
				{
					return vv->_valeur;
				}
			}
			static SHARED_PTR(T) vide;
			return vide;
		}
		void partagee(const void *variable)
		{
			auto verrou = _protection->ecrivain();
			auto it = _variables.find(std::make_shared<VariableClef>(variable));
			if(it==_variables.end()) return;
			(*it)->_partagee = true;
		}
		void reguliere(const void *variable)
		{
			auto verrou = _protection->ecrivain();
			auto it = _variables.find(std::make_shared<VariableClef>(variable));
			if(it==_variables.end()) return;
			(*it)->_partagee = false;
			(*it)->_compteur = 1;
		}
		bool est_partagee(const void *variable) const
		{
			auto verrou = _protection->lecteur();
			auto it = _variables.find(std::make_shared<VariableClef>(variable));
			if(it==_variables.end()) return false;
			return (*it)->_partagee;
		}
		void globale(const void *variable, VariablesDefiniesSP& destination)
		{
			auto verrou = _protection->ecrivain();
			auto verrou_destination = destination->_protection->ecrivain();
			auto itdest = destination->_variables.find(std::make_shared<VariableClef>(variable));
			if(itdest!=destination->_variables.end())
			{
				if(not (*itdest)->_partagee) return;
				++(*itdest)->_compteur;
				return;
			}
			auto it=_variables.find(std::make_shared<VariableClef>(variable));
			if(it==_variables.end())
				return;
			destination->_variables.insert(*it);
			(*it)->_compteur = 1;
			_variables.erase(it);
		}
		void locale(const void *variable, VariablesDefiniesSP& origine)
		{
			auto verrou = _protection->ecrivain();
			auto verrou_origine = origine->_protection->ecrivain();
			auto it=origine->_variables.find(std::make_shared<VariableClef>(variable));
			if(it==origine->_variables.end()) return;
			--(*it)->_compteur;
			if((*it)->_compteur>0) return;
			_variables.insert(*it);
			origine->_variables.erase(it);
		}
		void libere(const void *variable)
		{
			auto verrou = _protection->ecrivain();
			auto it=_variables.find(std::make_shared<VariableClef>(variable));
			if(it==_variables.end())
				return;
			_variables.erase(it);
		}
		void exporte(const void *variable, VariablesDefiniesSP& destination)
		{
			auto verrou = _protection->ecrivain();
			auto verrou_destination = destination->_protection->ecrivain();
			auto it=_variables.find(std::make_shared<VariableClef>(variable));
			if(it==_variables.end())
				return;
			destination->_variables.insert(*it);
		}
		void applique(std::function<void (VariableSP)> fonction)
		{
			auto verrou = _protection->lecteur();
			for(auto& v: _variables)
			{
				fonction(v);
			}
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const VariablesDefinies& v)
		{
			auto verrou = v._protection->lecteur();
			for(auto& vv:v._variables)
			{
				f << vv << std::endl;
			}
			return f;
		}
		private:
			std::set<VariableSP,SVM::Global::SPinferieurSP<Variable> > _variables;
			SVM_Synchronisation::AccesSP _protection;
	};
}
}
}
