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

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Programme
{
	struct DescriptionBlocAlias;
	struct DescriptionBlocAtome;
	struct DescriptionBlocCompose;
}
namespace Memoire
{

namespace SVM_Programme = SVM::Machine::Element::Programme;

	struct SousBloc
	{
		SousBloc(const size_t debut,const size_t taille)
		:_debut(debut), _taille(taille) {}
		size_t _debut;
		size_t _taille;
	};

	struct Memoire;
	struct PiegeMemoire;
	
	DECL_SHARED_PTR(DescriptionBloc);
	struct DescriptionBloc // construit par evaluation de la description de bloc du programme
	{
		friend struct Memoire;
		friend struct PiegeMemoire;
		friend struct SVM_Programme::DescriptionBlocAlias;
		friend struct SVM_Programme::DescriptionBlocAtome;
		friend struct SVM_Programme::DescriptionBlocCompose;
		DescriptionBloc() {}
		std::set<std::string> alias() const
		{
			std::set<std::string> alias;
			for(auto& a:_alias)
			{
				alias.insert(alias.end(),a.first);
			}
			return alias;
		}
		DescriptionBloc(const Type type, const size_t taille, const std::string& alias)
		:_elements(taille,type)
		{
			_alias.insert(std::make_pair(alias,SousBloc(0,taille)));
		}
		void ajout(const Type type, const size_t taille)
		{
			std::vector<Type> a(taille,type);
			_elements.insert(_elements.end(),a.begin(),a.end());
		}
		void ajout(const DescriptionBloc& ajout)
		{
			_elements.insert(_elements.end(),ajout._elements.begin(),ajout._elements.end());
		}
		void insere(const size_t indice, const DescriptionBloc& insert)
		{
			_elements.insert(_elements.begin()+indice,insert._elements.begin(),insert._elements.end());
		}
		void enleve(const size_t indice, const size_t taille)
		{
			_elements.erase(_elements.begin()+indice,_elements.begin()+indice+taille);
		}
		void remplace(const size_t indice, const DescriptionBloc& remplace)
		{
			for(size_t i=0 ; i<remplace._elements.size() ; ++i)
			{
				_elements[indice+i]=remplace._elements[i];
			}
		}
		const Type& element(const size_t indice) const
		{
			return _elements[indice];
		}
		size_t taille() const
		{
			return _elements.size(); 
		}
		size_t cherche(const size_t debut, const DescriptionBloc& motif) const
		{
			for(size_t indice_debut=debut ; indice_debut<=(_elements.size()-motif._elements.size()) ; ++indice_debut)
			{
				bool trouve=true;
				for(size_t indice=0 ; indice<motif._elements.size() ; ++indice)
				{
					if(_elements[indice_debut+indice]!=motif._elements[indice])
					{
						trouve=false;
						break;
					}
				}
				if(trouve)
				{
					return indice_debut;
				}
			}
			return _elements.size();
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const DescriptionBloc& d)
		{
			f << "[";
			for(auto& t: d._elements)
			{
				f << " " << t;
			}
			f << " ]";
			return f;
		}
		private:
		std::vector<Type> _elements;
		std::map<std::string,SousBloc> _alias;
	};
}
}
}
}
