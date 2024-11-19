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
	DECL_SHARED_PTR(Arret);
	struct Arret
	{
		Arret()
		:_machine(false), _code(0)
		{}
		explicit Arret(const size_t code)
		:_machine(true), _code(code)
		{}
		operator SHARED_PTR(size_t) () const
		{
			if(_machine)
			{
				return std::make_shared<size_t>(_code);
			}
			return SHARED_PTR(size_t)();
		}
		private:
			bool _machine;
			size_t _code;
	};
}
}
}
}

