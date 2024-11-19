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
#include <src/machine/elements/memoire/valeur.h>
#include <src/machine/elements/synchronisation/acces.h>

namespace SVM_Synchronisation = SVM::Machine::Element::Synchronisation;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Memoire
{
	DECL_WEAK_PTR(Memoire);
	DECL_SHARED_PTR(PiegeMemoire);

	DECL_SHARED_PTR(Element);
	struct Memoire;
	struct PiegeMemoire;
	struct Element
	{
		friend struct Memoire;
		friend struct PiegeMemoire;
		Element(const Type& type)
		:_type(type){}
		bool initialise() const { return static_cast<bool>(_valeur); }
		bool type(const Type& type) const { return type==_type; }
		std::string serialise() const
		{
			std::ostringstream oss;
			oss << _type;
			if(static_cast<bool>(_acces))
			{
				oss << " (sync)";
			}
			if(static_cast<bool>(_valeur))
			{
				oss << " " << _valeur->serialise();
			}
			return oss.str();
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Element& element)
		{
			f << element._type;
			if(static_cast<bool>(element._acces))
			{
				f << " (sync)";
			}
			if(static_cast<bool>(element._valeur))
			{
				f << " " << (*element._valeur);
			}
			return f;
		}
	void ajoute_reference_memoire(const MemoireWP& memoire, const size_t adresse);
	void enleve_reference_memoire(const MemoireWP& memoire, const size_t adresse);
	void notification_debugueur_changement_memoire(const PiegeMemoireSP& piege);
	void notification_debugueur_changement_memoire(const PiegeMemoireSP& piege, const MemoireWP& memoire, const size_t adresse);
	public:
		mutable std::mutex _protection;
		std::list<std::pair<MemoireWP,size_t> > _memoire;
	private:
		Type _type;
		ValeurSP _valeur;
		SVM_Synchronisation::AccesSP _acces;
	};
}
}
}
}
