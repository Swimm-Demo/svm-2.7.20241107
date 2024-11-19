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

#include <src/machine/elements/programme/type_programme.h>
#include <src/machine/machine/machine.h>
using namespace SVM::Machine::Element::Programme;

SVM_Memoire::Type TypeExtension::evaluation(const SVM_Noyau::NoyauSP& noyau) const
{
	SVM_Valeur::PointEntreeExtension pep = _pep->evaluation_point_entree_extension(noyau);
	if(not _patron)
	{
		auto patron = noyau->machine()->_base->_gestionnaire_extensions->type(pep);
		if(not patron)
		{
			throw SVM_Memoire::TypeInexistant(pep);
		}
		return SVM_Memoire::Type(pep,patron);
	}
	return SVM_Memoire::Type(pep,_patron);
}

SVM_Memoire::Type TypeExtension::explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const
{
	arbre->ajout_enfant("",_pep);
	SVM_Valeur::PointEntreeExtension pep = _pep->explique_calcul_point_entree_extension(noyau,arbre->enfant(0));
	if(not _patron)
	{
		auto patron = noyau->machine()->_base->_gestionnaire_extensions->type(pep);
		if(not patron)
		{
			throw SVM_Memoire::TypeInexistant(pep);
		}
		auto r = SVM_Memoire::Type(pep,patron);
		arbre->resultat(r);
		return r;
	}
	auto r = SVM_Memoire::Type(pep,_patron);
	arbre->resultat(r);
	return r;
}

void TypeExtension::decoration(SVM_Valeur::Decorateur& decorateur)
{
	_pep->decoration(decorateur);
	PointEntreeExtensionSP pep = std::dynamic_pointer_cast<PointEntreeExtension>(_pep);
	if(static_cast<bool>(pep))
	{
		_patron = decorateur._machine->_base->_gestionnaire_extensions->type(pep->_pep->_valeur);
		if(not _patron)
		{
			throw SVM_Memoire::TypeInexistant(pep->_pep->_valeur);
		}
	}
}
