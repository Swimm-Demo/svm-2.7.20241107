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
#include <src/machine/elements/valeur/parametre.h>
#include <src/machine/elements/noyau/noyau.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Noyau = SVM::Machine::Element::Noyau;

namespace SVM
{
namespace Machine
{
	DECL_SHARED_PTR(Machine);
namespace Extension
{
	DECL_SHARED_PTR(PatronFonctionExtension);
}
namespace Interface
{
	struct RetourFonction
	{
		RetourFonction()
		:_variable(nullptr) {}
		SVM_Memoire::ValeurInterfaceSP _valeur;
		SVM_Interface::StructureSP _structure;
		const void *_variable;
	};
	DECL_SHARED_PTR(Fonction);
	struct Fonction
	{
		Fonction(const SVM::Machine::MachineSP& machine, const SVM_Valeur::PointEntreeExtension& nom_fonction, const std::vector<SVM_Valeur::ParametreSP>& parametres);
		RetourFonction appel(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& appellant) const;
		public:
		SVM_Valeur::PointEntreeExtension _nom;
		SVM::Machine::Extension::PatronFonctionExtensionSP _patron;
		std::vector<SVM_Valeur::ParametreSP> _parametres;
		SVM::Machine::MachineSP _machine;
	};

	struct FonctionInexistante : public SVM_Valeur::ExceptionExecution
	{
		FonctionInexistante(const SVM_Valeur::PointEntreeExtension& nom)
		:ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Function " << nom << " does not exist.");
		}
	};
}
}
}
