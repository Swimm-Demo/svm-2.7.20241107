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

#include <src/machine/extensions/patron_objet_extension.h>
#include <src/machine/extensions/description.h>

using namespace SVM::Machine::Extension;


void PatronInstructionExtension::verifie_parametres(const std::vector<RegexParametresElementCSP>& parametres) const
{
	auto c = _parametres->conforme(parametres);
	if(not c.first)
	{
		throw ParametresIncorrects(parametres,*this,c.second);
	}
}

void PatronInstructionExtension::verifie_retour(const RegexParametresElementCSP& retour, const bool nulle) const
{
	if(static_cast<bool>(_retour) and static_cast<bool>(retour))
	{
		if(not _retour->conforme({retour}).first)
		{
			throw RetourIncorrect(retour,*this);
		}
		if(nulle and not _retour->conforme({}).first)
		{
			throw RetourNulleIncorrect(retour,*this);
		}
	}
	else
	{
		if(_retour and not retour)
		{
			throw RetourAttendu(_retour,*this);
		}
		if(not _retour and retour)
		{
			throw RetourNonAttendu(*this);
		}
	}
}

void PatronFonctionExtension::verifie_parametres(const std::vector<RegexParametresElementCSP>& parametres) const
{
	auto c = _parametres->conforme(parametres);
	if(not c.first)
	{
		throw ParametresIncorrects(parametres,*this,c.second);
	}
}

void PatronFonctionExtension::verifie_retour(const RegexParametresElementCSP& retour, const bool nulle) const
{
	if(static_cast<bool>(_retour) and static_cast<bool>(retour))
	{
		if(not _retour->conforme({retour}).first)
		{
			throw RetourIncorrect(retour,*this);
		}
		if(nulle and not _retour->conforme({}).first)
		{
			throw RetourNulleIncorrect(retour,*this);
		}
	}
	else
	{
		if(_retour and not retour)
		{
			throw RetourAttendu(_retour,*this);
		}
		if(not _retour and retour)
		{
			throw RetourNonAttendu(*this);
		}
	}
}
