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
#include <src/machine/elements/valeur/exception.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Interface
{
	struct Outils;
}
namespace Extension
{
	DECL_SHARED_PTR(PatronTypeExtension);
}
namespace Element
{
namespace Memoire
{
	DECL_SHARED_PTR(Type);
	namespace SVM_Extension = SVM::Machine::Extension;
	struct Type
	{
		friend struct SVM::Machine::Interface::Outils;
		enum class TypeInterne
		{
			AUTOMATIQUE,
			ENTIER,
			CHAINE,
			BOOLEEN,
			POINTEUR,
			SYMBOLE,
			BIBLIOTHEQUE,
			INTERRUPTION,
			POINT_ENTREE_EXTENSION,
			TYPE_EXTENSION

		};
		Type(const TypeInterne& interne)
		:_type_interne(interne)
		{
			ASSERT((interne!=TypeInterne::TYPE_EXTENSION),"Un type interne ne peut pas etre de type TYPE_EXTENSION");
		}
		Type(const SVM_Valeur::PointEntreeExtension& externe, const SVM_Extension::PatronTypeExtensionSP& patron = SVM_Extension::PatronTypeExtensionSP())
		:_type_interne(TypeInterne::TYPE_EXTENSION),_point_entree_extension(externe), _patron(patron)
		{ }
		bool operator==(const TypeInterne& type) const
		{
			//ASSERT(type!=TypeInterne::TYPE_EXTENSION,"Ne peut pas comparer partiellement a un type extension");
			return _type_interne == type;
		}
		bool operator==(const Type& type) const
		{
			return (_type_interne == type._type_interne) and (_point_entree_extension==type._point_entree_extension);
		}
		bool operator!=(const Type& type) const
		{
			return not ((*this)==type);
		}
		bool operator<(const Type& type) const
		{
			if(_type_interne!=type._type_interne)
			{
				return _type_interne<type._type_interne;
			}
			return _point_entree_extension<type._point_entree_extension;
		}
		bool operator>(const Type& type) const
		{
			if(_type_interne!=type._type_interne)
			{
				return _type_interne>type._type_interne;
			}
			return _point_entree_extension>type._point_entree_extension;
		}
		bool copiable() const;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Type& type)
		{
			switch(type._type_interne)
			{
				case TypeInterne::AUTOMATIQUE: f << "AUTO"; break;
				case TypeInterne::ENTIER: f << "INT"; break;
				case TypeInterne::CHAINE: f << "STR"; break;
				case TypeInterne::BOOLEEN: f << "BLN"; break;
				case TypeInterne::POINTEUR: f << "PTR"; break;
				case TypeInterne::SYMBOLE: f << "SYM"; break;
				case TypeInterne::BIBLIOTHEQUE: f << "LIB"; break;
				case TypeInterne::INTERRUPTION: f << "IRQ"; break;
				case TypeInterne::POINT_ENTREE_EXTENSION: f << "PEP"; break;
				case TypeInterne::TYPE_EXTENSION: f << type._point_entree_extension; break;
			}
			return f;
		}
		private:
		TypeInterne _type_interne;
		SVM_Valeur::PointEntreeExtension _point_entree_extension;
		SVM_Extension::PatronTypeExtensionSP _patron;
	};

	struct TypeInexistant : public SVM_Valeur::ExceptionExecution
	{
		TypeInexistant(const SVM_Valeur::PointEntreeExtension& type)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Type " << type << " is not defined.");
		}
		virtual ~TypeInexistant() {}
	};

	struct TypePasDeConstante : public SVM_Valeur::ExceptionExecution
	{
		TypePasDeConstante(const SVM_Valeur::PointEntreeExtension& type)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Type " << type << " does not support creation from constant.");
		}
		virtual ~TypePasDeConstante() {}
	};

	struct TypePasDeCopie : public SVM_Valeur::ExceptionExecution
	{
		TypePasDeCopie(const SVM_Valeur::PointEntreeExtension& type)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Type " << type << " does not support copy.");
		}
		virtual ~TypePasDeCopie() {}
	};

	struct TypePasDAffichage : public SVM_Valeur::ExceptionExecution
	{
		TypePasDAffichage(const SVM_Valeur::PointEntreeExtension& type)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Type " << type << " does not support print.");
		}
		virtual ~TypePasDAffichage() {}
	};
}
}
}
}
