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
#include <src/machine/elements/valeur/point_entree_extension.h>
#include <src/machine/elements/memoire/type.h>
#include <src/machine/extensions/patron_objet_extension.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Memoire = SVM::Machine::Element::Memoire;
namespace SVM_Extension = SVM::Machine::Extension;

namespace SVM
{
namespace Machine
{
	DECL_SHARED_PTR(Machine);
	DECL_WEAK_PTR(Machine);
namespace Interface
{
	DECL_SHARED_PTR(Structure);
	struct Structure
	{
		Structure(const SVM::Machine::MachineSP& machine, const SVM_Valeur::PointEntreeExtension& nom, const SVM_Extension::PatronStructureExtensionSP& patron)
		:_nom(nom), _patron(patron), _valeur(nullptr),_machine(machine) {}
		Structure(const SVM::Machine::MachineSP& machine, const SVM_Valeur::PointEntreeExtension& nom, const SVM_Extension::PatronStructureExtensionSP& patron, void *valeur)
		:_nom(nom), _patron(patron), _valeur(valeur),_machine(machine) {}
		~Structure();
		StructureSP copie() const;
		bool nulle() const { return _valeur==nullptr; }
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Structure& structure)
		{
			f << "<STRUCT " << structure._nom << ">";
			return f;
		}
		operator SVM_Memoire::Type () const
		{
			return SVM_Memoire::Type(_nom);
		}
		std::string uml() const
		{
			std::ostringstream oss;
			oss << "STRUCT_" << this;
			return oss.str();
		}
		public:
		SVM_Valeur::PointEntreeExtension _nom;
		SVM_Extension::PatronStructureExtensionSP _patron;
		void *_valeur;
		SVM::Machine::MachineWP _machine;
	};

	struct StructurePasDeCopie : public SVM_Valeur::ExceptionExecution
	{
		StructurePasDeCopie(const SVM_Valeur::PointEntreeExtension& type)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Structure " << type << " does not support copy.");
		}
		virtual ~StructurePasDeCopie() {}
	};
}
}
}
