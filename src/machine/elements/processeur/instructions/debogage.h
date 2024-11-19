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

#include <src/machine/elements/valeur/instruction.h>
#include <src/machine/elements/programme/adresse_programme.h>
#include <src/machine/elements/programme/valeur_programme.h>
#include <src/machine/elements/noyau/noyau.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Programme = SVM::Machine::Element::Programme;
namespace SVM_Noyau = SVM::Machine::Element::Noyau;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Processeur
{
namespace Instruction
{
	struct DebogageArretProcesseur : public SVM_Valeur::Instruction
	{
		explicit DebogageArretProcesseur(const SVM_Valeur::PositionInstruction& position, const SVM_Programme::ValeurSP& message = SVM_Programme::ValeurSP())
		:SVM_Valeur::Instruction(position), _message(message) {}
		virtual ~DebogageArretProcesseur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":debug BREAK";
			if(static_cast<bool>(_message))
			{
				os << " " << (*_message);
			}
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"comment\">";
			format(os);
			os << "</span>";
		}
		private:
			SVM_Programme::ValeurSP _message;
	};

	struct DebogageArretMemoire : public SVM_Valeur::Instruction
	{
		enum class Type { LECTURE, ECRITURE, ACCES, SUPPRESSION };
		DebogageArretMemoire(const SVM_Valeur::PositionInstruction& position, const bool ajout, const Type type, const SVM_Programme::AdresseSP& adresse)
		:SVM_Valeur::Instruction(position), _ajout(ajout), _type(type), _adresse(adresse) {}
		virtual ~DebogageArretMemoire() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":debug " << (_ajout?"ADD":"REMOVE") << " ";
			switch(_type)
			{
				case Type::LECTURE:
					os << "READ";
					break;
				case Type::ECRITURE:
					os << "WRITE";
					break;
				case Type::ACCES:
					os << "ACCESS";
					break;
				case Type::SUPPRESSION:
					os << "FREE";
					break;
			}
			os << " " << (*_adresse) ;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"comment\">";
			format(os);
			os << "</span>";
		}
		private:
			bool _ajout;
			Type _type;
			SVM_Programme::AdresseSP _adresse;
	};

	struct DebogageExplicationAdresse : public SVM_Valeur::Instruction
	{
		DebogageExplicationAdresse(const SVM_Valeur::PositionInstruction& position, const SVM_Programme::AdresseSP& adresse)
		:SVM_Valeur::Instruction(position), _adresse(adresse) {}
		virtual ~DebogageExplicationAdresse() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":debug EXPLAIN " << (*_adresse) ;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"comment\">";
			format(os);
			os << "</span>";
		}
		private:
			SVM_Programme::AdresseSP _adresse;
	};

	struct DebogageExplicationValeur : public SVM_Valeur::Instruction
	{
		DebogageExplicationValeur(const SVM_Valeur::PositionInstruction& position, const SVM_Programme::ValeurSP& valeur)
		:SVM_Valeur::Instruction(position), _valeur(valeur) {}
		virtual ~DebogageExplicationValeur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":debug EXPLAIN " << (*_valeur) ;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"comment\">";
			format(os);
			os << "</span>";
		}
		private:
			SVM_Programme::ValeurSP _valeur;
	};

	struct DebogageArretInterruption : public SVM_Valeur::Instruction
	{
		DebogageArretInterruption(const SVM_Valeur::PositionInstruction& position, const bool ajout, const SVM_Programme::ValeurSP& interruption)
		:SVM_Valeur::Instruction(position), _ajout(ajout), _interruption(interruption) {}
		virtual ~DebogageArretInterruption() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual void execution(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << ":debug " << (_ajout?"ADD":"REMOVE") << " " << (*_interruption) ;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"comment\">";
			format(os);
			os << "</span>";
		}
		private:
			bool _ajout;
			SVM_Programme::ValeurSP _interruption;
	};
}
}
}
}
}
