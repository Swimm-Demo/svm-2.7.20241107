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
	DECL_SHARED_PTR(PointEntreeExtension);
	struct PointEntreeExtension
	{
		PointEntreeExtension() {}
		PointEntreeExtension(const std::string& nom_extension, const std::string& nom_entree)
			:_nom_extension(nom_extension), _nom_entree(nom_entree)
		{
			ASSERT(not nom_extension.empty(),"PEP - nom extension vide");
			ASSERT(not nom_entree.empty(),"PEP - nom entree vide");
		}
		bool operator==(const PointEntreeExtension& pep) const
		{
			return (_nom_extension==pep._nom_extension) and (_nom_entree==pep._nom_entree);
		}
		bool operator!=(const PointEntreeExtension& pep) const
		{
			return not ((*this)==pep);
		}
		bool operator<(const PointEntreeExtension& pep) const
		{
			if(_nom_extension!=pep._nom_extension)
			{
				return _nom_extension<pep._nom_extension;
			}
			return _nom_entree<pep._nom_entree;
		}
		bool operator>(const PointEntreeExtension& pep) const
		{
			if(_nom_extension!=pep._nom_extension)
			{
				return _nom_extension>pep._nom_extension;
			}
			return _nom_entree>pep._nom_entree;
		}
		const std::string& nom_extension() const
		{
			return _nom_extension;
		}
		const std::string& nom_entree_pur() const
		{
			return _nom_entree;
		}
		std::string nom_entree(const std::string& prefixe="") const
		{
			if(prefixe.empty())
			{
				return _nom_entree;
			}
			std::ostringstream oss;
			oss << prefixe << "_" << _nom_entree;
			return oss.str();
		}
		operator std::string () const
		{
			std::ostringstream oss;
			oss << _nom_extension << "." << _nom_entree;
			return oss.str();
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const PointEntreeExtension& point_entree_extension)
		{
			f << point_entree_extension._nom_extension << "." << point_entree_extension._nom_entree;
			return f;
		}
		void html(std::ostream& os) const
		{
			os << _nom_extension << "<span class=\"operator\">.</span>" << _nom_entree;
		}
		private:
		std::string _nom_extension;
		std::string _nom_entree;
	};
}
}
}
}
