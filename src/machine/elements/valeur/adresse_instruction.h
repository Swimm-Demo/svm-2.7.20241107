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
#include <src/machine/elements/valeur/code.h>
#include <src/machine/debugueur/json.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Performance
{
	struct PerformanceRapport;
	struct PerformanceBrute;
}
namespace Interface
{
	struct Outils;
}
namespace Element
{
namespace Processeur
{
	struct Etat;
}
namespace Valeur
{
	DECL_SHARED_PTR(AdresseInstruction);
	struct AdresseInstruction
	{
		friend struct SVM::Machine::Interface::Outils;
		friend struct SVM::Machine::Performance::PerformanceRapport;
		friend struct SVM::Machine::Performance::PerformanceBrute;
		friend struct Processeur::Etat;
		explicit AdresseInstruction(const size_t locale) // adresse instruction locale
		:_locale(locale) {}
		AdresseInstruction(const SVM_Valeur::CodeSP& code, const size_t locale) // adresse instruction globale
		:_code(code),_locale(locale)
		{
			ASSERT(static_cast<bool>(_code),"Adresse instruction globale sans code");
		}
		bool locale() const { return not _code; }
		AdresseInstruction globale(const SVM_Valeur::CodeSP& code) const
		{
			if(static_cast<bool>(_code))
			{
				return *this;
			}
			else
			{
				return AdresseInstruction(code,_locale);
			}
		}
		void operator=(const AdresseInstruction& adresse_instruction) // saut
		{
			if(static_cast<bool>(adresse_instruction._code))
			{
				_code = adresse_instruction._code;
			}
			_locale = adresse_instruction._locale;
		}
		AdresseInstruction operator++(int)
		{
			AdresseInstruction c = *this;
			++_locale;
			return c;
		}
		SVM_Valeur::InstructionSP instruction() const
		{
			ASSERT(static_cast<bool>(_code),"Instruction ne peut pas etre trouvee via une adresse locale");
			return _code->instruction(_locale);
		}
		bool operator==(const AdresseInstruction& a) const
		{
			return (_code==a._code) and (_locale==a._locale); // ici aussi; on compare "physiquement" les codes
		}
		bool operator!=(const AdresseInstruction& a) const
		{
			return not ((*this)==a);
		}
		bool operator<(const AdresseInstruction& a) const
		{
			if(_code!=a._code)
			{
				return _code<a._code;
			}
			return _locale<a._locale;
		}
		bool operator>(const AdresseInstruction& a) const
		{
			if(_code!=a._code)
			{
				return _code>a._code;
			}
			return _locale>a._locale;
		}
		std::string nom_noyau() const
		{
			if(static_cast<bool>(_code))
			{
				return _code->_nom;
			}
			return "";
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const AdresseInstruction& adresse)
		{
			f << "<";
			if(static_cast<bool>(adresse._code))
			{
				f << adresse._code->nom() << "/";
			}
			f << adresse._locale << ">";
			return f;
		}
		template<typename Flux>
		void html(Flux& f)
		{
			f << "&lt;";
			if(static_cast<bool>(_code))
			{
				f << _code->nom() << "/";
			}
			f << _locale << "&gt;";
		}
		template<typename Flux>
		void liste_code(Flux& flux) const
		{
			_code->trace(flux,std::make_shared<size_t>(_locale));
		}
		SVM::Machine::Debugueur::JSON_Interne::ObjetSP debugueur() const
		{
			auto a = SVM::Machine::Debugueur::JSON::objet();
			if(static_cast<bool>(_code))
			{
				a->ajoute("code",SVM::Machine::Debugueur::JSON::entier(_code->_piege->identifiant()));
				a->ajoute("code_name",SVM::Machine::Debugueur::JSON::chaine(_code->nom()));
			}
			a->ajoute("local",SVM::Machine::Debugueur::JSON::entier(_locale));
			return a;
		}
		private:
		SVM_Valeur::CodeSP _code;
		size_t _locale;
	};
}
}
}
}
