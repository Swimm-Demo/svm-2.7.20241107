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

#include <debug/styles/styles.h>

using namespace SVM::Debug::Styles;

std::string Styles::html(const std::string& style)
{
	auto vrai_style = style;
	auto s = style.find(':');
	if(s==std::string::npos)
	{
		vrai_style+=":default";
	}
	static const std::set<std::string> styles_valides = 
#include <debug/styles/styles_valides.cpp>
	;
	auto it = styles_valides.find(vrai_style);
	if(it==styles_valides.end())
	{
		vrai_style="default:default";
	}
	std::ostringstream oss;
	for(const auto& st: styles_valides)
	{
		s = st.find(':');
		oss << "<link title=\"" << st << "\" rel=\"";
		if(st!=vrai_style)
		{
			oss << "alternate ";
		}
		oss	<< "stylesheet\" href=\"/styles/" << st.substr(0,s) << "/" << st.substr(s+1) << ".css\" type=\"text/css\"/>";
		oss << "<link title=\"" << st << "\" rel=\"";
		if(st!=vrai_style)
		{
			oss << "alternate ";
		}
		oss	<< "stylesheet\" href=\"/styles/" << st.substr(0,s) << "/style.css\" type=\"text/css\"/>";
	}
	return oss.str();
}
