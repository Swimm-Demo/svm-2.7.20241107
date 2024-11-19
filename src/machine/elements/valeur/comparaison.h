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
	struct Comparaison
	{
		bool _egal = false;
		bool _inferieur = false;
		bool _superieur = false;
		bool _ordre = false;
		bool _total = false;
		bool _type = false;
		bool _nul = false;
		bool _faible = false;

		template<typename T>
		static Comparaison comparaison_ordre_total(const T& gauche, const T& droite, const bool faible = false)
		{
			Comparaison c;
			c._egal = gauche==droite;
			c._inferieur = gauche<droite;
			c._superieur = gauche>droite;
			c._ordre = true;
			c._total = true;
			c._faible = faible;
			return c;
		}

		template<typename T>
		static Comparaison comparaison_equivalence(const T& gauche, const T& droite, const bool faible = false)
		{
			Comparaison c;
			c._egal = gauche==droite;
			c._faible = faible;
			return c;
		}
	};

}
}
}
}

