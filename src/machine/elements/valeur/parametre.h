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
#include <src/machine/elements/memoire/valeur.h>

namespace SVM_Memoire = SVM::Machine::Element::Memoire;

namespace SVM
{
namespace Machine
{
namespace Extension
{
	DECL_SHARED_PTR(RegexParametresElement);
}
namespace Interface
{
	DECL_SHARED_PTR(Structure);
}
namespace Element
{
namespace Valeur
{
	namespace SVM_Extension = SVM::Machine::Extension;
	DECL_SHARED_PTR(Parametre);

	struct Parametre
	{
		virtual ~Parametre() {}
		virtual SVM_Extension::RegexParametresElementCSP type() const = 0;
		virtual void format(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Parametre& parametre)
		{
			parametre.format(f);
			return f;
		}
	};

	struct ParametreValeur : public Parametre
	{
		ParametreValeur(const SVM_Memoire::ValeurSP& valeur)
		:_valeur(std::make_shared<SVM_Memoire::ValeurInterface>(valeur)) {}
		ParametreValeur(const SVM_Memoire::ValeurInterfaceSP& valeur)
		:_valeur(valeur) {}
		virtual ~ParametreValeur() {}
		virtual SVM_Extension::RegexParametresElementCSP type() const override;
		virtual void format(std::ostream& os) const override
		{
			os << (*_valeur);
		}
		public:
		SVM_Memoire::ValeurInterfaceSP _valeur;
	};

	struct MarqueurInvalide : public ExceptionExecution
	{
		MarqueurInvalide(const std::string& marqueur)
		:ExceptionExecution(Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Invalid marker " << marqueur << ".");
		}
		virtual ~MarqueurInvalide() {}
	};

	struct ParametreMarqueur : public Parametre
	{
		enum class Marqueur
		{
			INFERIEUR,
			SUPERIEUR,
			INFERIEUR_INFERIEUR,
			SUPERIEUR_SUPERIEUR,
			INFERIEUR_SUPERIEUR,
			EGAL,
			INFERIEUR_OU_EGAL,
			SUPERIEUR_OU_EGAL,
			DEBUT_ACCOLADE,
			VIRGULE,
			POINT_VIRGULE,
			FIN_ACCOLADE
		};
		ParametreMarqueur(const Marqueur marqueur)
		:_marqueur(marqueur) {}
		virtual SVM_Extension::RegexParametresElementCSP type() const override;
		virtual ~ParametreMarqueur() {}
		virtual void format(std::ostream& os) const override
		{
			os << ParametreMarqueur::texte(_marqueur);
		}
		static std::string texte(const Marqueur marqueur)
		{
			switch(marqueur)
			{
				case Marqueur::INFERIEUR:
					return "<"; break;
				case Marqueur::SUPERIEUR:
					return ">"; break;
				case Marqueur::INFERIEUR_INFERIEUR:
					return "<<"; break;
				case Marqueur::SUPERIEUR_SUPERIEUR:
					return ">>"; break;
				case Marqueur::INFERIEUR_SUPERIEUR:
					return "<>"; break;
				case Marqueur::EGAL:
					return "="; break;
				case Marqueur::INFERIEUR_OU_EGAL:
					return "<="; break;
				case Marqueur::SUPERIEUR_OU_EGAL:
					return "=>"; break;
				case Marqueur::DEBUT_ACCOLADE:
					return "{"; break;
				case Marqueur::VIRGULE:
					return ","; break;
				case Marqueur::POINT_VIRGULE:
					return ";"; break;
				case Marqueur::FIN_ACCOLADE:
					return "}"; break;
			}
			return "";
		}
		static Marqueur marqueur(const std::string& marqueur)
		{
			if(marqueur=="<")
				return Marqueur::INFERIEUR;
			if(marqueur==">")
				return Marqueur::SUPERIEUR;
			if(marqueur=="<<")
				return Marqueur::INFERIEUR_INFERIEUR;
			if(marqueur==">>")
				return Marqueur::SUPERIEUR_SUPERIEUR;
			if(marqueur=="<>")
				return Marqueur::INFERIEUR_SUPERIEUR;
			if(marqueur=="=")
				return Marqueur::EGAL;
			if(marqueur=="<=")
				return Marqueur::INFERIEUR_OU_EGAL;
			if(marqueur=="=>")
				return Marqueur::SUPERIEUR_OU_EGAL;
			if(marqueur=="{")
				return Marqueur::DEBUT_ACCOLADE;
			if(marqueur==",")
				return Marqueur::VIRGULE;
			if(marqueur==";")
				return Marqueur::POINT_VIRGULE;
			if(marqueur=="}")
				return Marqueur::FIN_ACCOLADE;
			throw MarqueurInvalide(marqueur);
		}
		static bool separateur(const Marqueur marqueur)
		{
			switch(marqueur)
			{
				case Marqueur::INFERIEUR_SUPERIEUR:
				case Marqueur::EGAL:
				case Marqueur::VIRGULE:
				case Marqueur::POINT_VIRGULE:
					return true; break;
				default:
					break;
			}
			return false;
		}
		static bool ouverture(const Marqueur marqueur)
		{
			switch(marqueur)
			{
				case Marqueur::DEBUT_ACCOLADE:
				case Marqueur::INFERIEUR:
				case Marqueur::INFERIEUR_OU_EGAL:
				case Marqueur::INFERIEUR_INFERIEUR:
					return true; break;
				default:
					break;
			}
			return false;
		}
		static bool fermeture(const Marqueur marqueur, const Marqueur ouverture)
		{
			if((marqueur==Marqueur::FIN_ACCOLADE) and (ouverture==Marqueur::DEBUT_ACCOLADE)) return true;
			if((marqueur==Marqueur::SUPERIEUR) and (ouverture==Marqueur::INFERIEUR)) return true;
			if((marqueur==Marqueur::SUPERIEUR_OU_EGAL) and (ouverture==Marqueur::INFERIEUR_OU_EGAL)) return true;
			if((marqueur==Marqueur::SUPERIEUR_SUPERIEUR) and (ouverture==Marqueur::INFERIEUR_INFERIEUR)) return true;
			return false;
		}
		public:
		Marqueur _marqueur;
	};

	struct ParametreMotClef : public Parametre
	{
		ParametreMotClef(const std::string& mot_clef)
		:_mot_clef(mot_clef) {}
		virtual ~ParametreMotClef() {}
		virtual SVM_Extension::RegexParametresElementCSP type() const override;
		virtual void format(std::ostream& os) const override
		{
			os << _mot_clef;
		}
		public:
		std::string _mot_clef;
	};

	struct ParametreStructure : public Parametre
	{
		ParametreStructure(const SVM::Machine::Interface::StructureSP& structure)
		:_structure(structure) {}
		virtual ~ParametreStructure() {}
		virtual SVM_Extension::RegexParametresElementCSP type() const override;
		virtual void format(std::ostream& os) const override;
		public:
		SVM::Machine::Interface::StructureSP _structure;
	};

	struct ParametreVariable : public Parametre
	{
		ParametreVariable(const void *variable)
		:_variable(variable) {}
		virtual ~ParametreVariable() {}
		virtual SVM_Extension::RegexParametresElementCSP type() const override;
		virtual void format(std::ostream& os) const override
		{
			os << _variable;
		}
		public:
		const void *_variable;
	};
}
}
}
}
