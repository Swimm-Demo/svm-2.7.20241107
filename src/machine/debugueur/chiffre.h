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

#include <src/machine/debugueur/securite.h>

namespace SVM
{
namespace Machine
{
namespace Debugueur
{
DECL_SHARED_PTR(Chiffre);

struct Chiffre
{
	typedef std::vector<std::pair<std::string, std::string> > Challenges;
	Chiffre(const std::string& clef_primaire, const Challenges& challenges);
	std::string chiffre(const std::string& source);
	std::string dechiffre(const std::string& source);
	private:
		static void transpose(unsigned char *matrice, size_t taille);
		static void masquage(unsigned char matrice[32], unsigned char masque[8], const std::string& clef);
		std::vector<std::string> _clefs_secondaires;
		template<typename T>
		static T decalage_circulaire(const char decalage, const T entree)
		{
			return (entree << decalage) bitor (entree >> (8*sizeof(T)-decalage));
		}
};

}
}
}
