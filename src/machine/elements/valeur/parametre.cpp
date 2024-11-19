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

#include <src/machine/elements/valeur/parametre.h>
#include <src/machine/extensions/description.h>
#include <src/machine/interface/structure.h>

using namespace SVM::Machine::Element::Valeur;

::SVM_Extension::RegexParametresElementCSP ParametreValeur::type() const
{
	SVM_Memoire::Type type = *_valeur->_valeur;
	auto r = std::make_shared<::SVM_Extension::RegexParametresValeur>(type);
	if(not _valeur->_valeur->est_constante())
	{
		r->_modifiable = true;
	}
	return r;
}

::SVM_Extension::RegexParametresElementCSP ParametreMarqueur::type() const
{
	return std::make_shared<::SVM_Extension::RegexParametresMarqueur>(_marqueur);
}

::SVM_Extension::RegexParametresElementCSP ParametreMotClef::type() const
{
	return std::make_shared<::SVM_Extension::RegexParametresMotClef>(_mot_clef);
}

::SVM_Extension::RegexParametresElementCSP ParametreStructure::type() const
{
	SVM_Memoire::Type type = *_structure;
	auto r = std::make_shared<::SVM_Extension::RegexParametresStructure>(type);
	r->_modifiable = true;
	return r;
}

::SVM_Extension::RegexParametresElementCSP ParametreVariable::type() const
{
	auto r = std::make_shared<::SVM_Extension::RegexParametresJokerPartielVariable>();
	r->_modifiable = true;
	return r;
}

void ParametreStructure::format(std::ostream& os) const
{
	os << *_structure;
}
