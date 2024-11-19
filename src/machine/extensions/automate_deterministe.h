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
#include <src/machine/extensions/description.h>

namespace SVM
{
namespace Machine
{
namespace Extension
{

	DECL_SHARED_PTR(AutomateDeterministe);
	struct AutomateNonDeterministe;

	struct AutomateDeterministe
	{
		friend struct AutomateNonDeterministe;
		explicit AutomateDeterministe(const std::string& regex)
		:_regex(regex)
		{

		}
		bool inclus_dans(const AutomateDeterministeSP& automate) const;
		std::pair<bool,size_t> conforme(const std::vector<RegexParametresElementCSP>& valeurs) const;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const AutomateDeterministe& a)
		{
			f << a._regex;
			return f;
		}
		operator std::string () const
		{
			std::ostringstream f;
			f << "AutomateDeterministe: " << _regex << std::endl;
			f << "Initial : " << _initial << std::endl;
			f << "Finaux :";
			for(auto& i:_final)
			{
				f << " " << i;
			}
			f << std::endl;
			f << "Transitions:" << std::endl;
			size_t indice = 0;
			for(auto &t:_transitions)
			{
				for(auto& tt:t)
				{
					f << indice << " === " << *tt.first << " ===> " << tt.second << std::endl;
				}
				++indice;
			}
			f << "========================" << std::endl;
			return f.str(); 
		}
		private:
		std::string _regex;
		size_t _initial;
		std::set<size_t> _final;
		std::vector<std::map<RegexParametresElementCSP,size_t,SVM::Global::SPinferieurSP<RegexParametresElement> > > _transitions;
	};

}
}
}
