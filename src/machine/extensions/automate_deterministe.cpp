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

#include <src/machine/extensions/automate_deterministe.h>
using namespace SVM::Machine::Extension;

bool AutomateDeterministe::inclus_dans(const AutomateDeterministeSP& automate) const
{
	std::set<std::pair<size_t,size_t> > etats;
	std::list<std::pair<size_t,size_t> > a_traiter = { { _initial, automate->_initial} };
	while(not a_traiter.empty())
	{
		auto e = a_traiter.back();
		a_traiter.pop_back();
		if((_final.find(e.first)!=_final.end()) and not (automate->_final.find(e.second)!=automate->_final.end()))
		{
			return false;
		}
		for(auto& l:_transitions[e.first])
		{
			RegexParametresElementSP lc;
			for(lc = l.first->copie() ; static_cast<bool>(lc) ; lc = lc->joker())
			{
				auto it = automate->_transitions[e.second].find(lc);
				if(it!=automate->_transitions[e.second].end())
				{
					auto ne = std::make_pair(l.second,it->second);
					if(etats.insert(ne).second)
					{
						a_traiter.push_back(ne);
					}
					break;
				}
				if(not l.first->_modifiable)
					continue;
				lc->_modifiable = false;
				it = automate->_transitions[e.second].find(lc);
				if(it!=automate->_transitions[e.second].end())
				{
					auto ne = std::make_pair(l.second,it->second);
					if(etats.insert(ne).second)
					{
						a_traiter.push_back(ne);
					}
					break;
				}
				lc->_modifiable = true;
			}
			if(not lc)
			{
				return false;
			}
		}
	}
	return true;
}

std::pair<bool,size_t> AutomateDeterministe::conforme(const std::vector<RegexParametresElementCSP>& valeurs) const
{
	size_t etat_courant = _initial;
	size_t indice_valeur=0;
	for(auto& v: valeurs)
	{
		++indice_valeur;
		RegexParametresElementSP lettre;
		bool ok = false;
		for(lettre = v->copie() ; static_cast<bool>(lettre) ; lettre=lettre->joker())
		{
			auto it = _transitions[etat_courant].find(lettre);
			if(it!=_transitions[etat_courant].end())
			{
				ok = true;
				etat_courant = it->second;
				break;
			}
			if(not v->_modifiable)
				continue;
			lettre->_modifiable = false;	
			it = _transitions[etat_courant].find(lettre);
			if(it!=_transitions[etat_courant].end())
			{
				ok = true;
				etat_courant = it->second;
				break;
			}
			lettre->_modifiable = true;	
		}
		if(not ok)
		{
			return std::make_pair(false,indice_valeur);
		}
	}
	return std::make_pair((_final.find(etat_courant)!=_final.end()),indice_valeur+1);
}
