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

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Valeur
{
	DECL_SHARED_PTR(ArbreSubstitution);
	struct ArbreSubstitution
	{
		template<typename V>
		explicit ArbreSubstitution(const V* source)
		:_resultat("..."),_reussi(false)
		{
			std::ostringstream oss;
			oss << (*source);
			_source = oss.str();
		}

		template<typename V>
		explicit ArbreSubstitution(const SHARED_PTR(V)& source)
		:_resultat("..."),_reussi(false)
		{
			std::ostringstream oss;
			oss << (*source);
			_source = oss.str();
		}

		void initialise(const std::string& prefixe, const std::string& suffixe)
		{
			_prefixe = prefixe;
			_suffixe = suffixe;
		}

		template<typename V>
		void resultat(const V& v)
		{
			_reussi = true;
			std::ostringstream oss;
			oss << v;
			_resultat = oss.str();
		}

		void resultat_booleen(const bool b)
		{
			_reussi = true;
			_resultat = b?"true":"false";
		}
		
		template<typename V>
		void resultat(const SHARED_PTR(V)& v)
		{
			_reussi = true;
			std::ostringstream oss;
			oss << v->serialise();
			_resultat = oss.str();
		}
		
		template<typename V>
		void ajout_enfant(const std::string& separateur, const V& source)
		{
			_enfants.push_back(std::make_pair(separateur,std::make_shared<ArbreSubstitution>(source)));
		}

		ArbreSubstitutionSP& enfant(const size_t indice)
		{
			return _enfants.at(indice).second;
		}

		template<typename Flux>
		bool explique(Flux& f, std::list<std::string>& prefixe, std::list<std::string>& suffixe)
		{
			prefixe.push_back(_prefixe);
			suffixe.push_front(_suffixe);
			for(auto it = _enfants.rbegin() ; it!=_enfants.rend() ; ++it)
			{
				suffixe.push_front(it->second->_source);
				suffixe.push_front(it->first);
			}
			for(auto& e: _enfants)
			{
				suffixe.pop_front();
				suffixe.pop_front();
				prefixe.push_back(e.first);
				if(not e.second->explique(f,prefixe,suffixe))
					return false;
				prefixe.push_back(e.second->_resultat);
			}
			for(size_t i=0 ; i<_enfants.size() ; ++i)
			{
				prefixe.pop_back();
				prefixe.pop_back();
			}
			prefixe.pop_back();
			suffixe.pop_front();
			for(auto& p: prefixe)
			{
				f << SVM::Global::Chaine::echappe_html(p);
			}
			f << " <b>";
			if(_enfants.empty())
			{
				f << SVM::Global::Chaine::echappe_html(_source);
			}
			else
			{
				f << _prefixe;
				for(auto& e: _enfants)
				{
					f << e.first << SVM::Global::Chaine::echappe_html(e.second->_resultat);
				}
				f << _suffixe;
			}
			f << " => " << SVM::Global::Chaine::echappe_html(_resultat) << "</b> ";
			for(auto& s: suffixe)
			{
				f << SVM::Global::Chaine::echappe_html(s);
			}
			f << "\n";
			return _reussi;

		}

		template<typename Flux>
		bool explique(Flux& f)
		{
			std::list<std::string> prefixe;
			std::list<std::string> suffixe;
			return explique(f,prefixe,suffixe);
		}

		std::string _source;
		std::string _resultat;
		std::string _prefixe;
		std::string _suffixe;
		bool _reussi;
		std::vector<std::pair<std::string,ArbreSubstitutionSP> > _enfants;
	};
}
}
}
}
