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
#include <src/machine/elements/programme/valeur_programme.h>
#include <src/machine/elements/valeur/adresse_instruction.h>
#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/valeur/decorateur.h>
namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Noyau = SVM::Machine::Element::Noyau;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Programme
{
	DECL_SHARED_PTR(Cible);
	struct Cible
	{
		Cible() = default;
		virtual ~Cible() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) = 0;
		virtual SVM_Valeur::AdresseInstruction evaluation(const SVM_Noyau::NoyauSP& noyau) const = 0;
		virtual SVM_Valeur::AdresseInstruction explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const = 0;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const = 0;
		virtual void format(std::ostream& os) const = 0;
		virtual void html(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Cible& c)
		{
			c.format(f);
			return f;
		}
	};

	struct CibleStatique : public Cible
	{
		CibleStatique(const std::string& etiquette)
		:_etiquette(etiquette),_adresse(-1) {}
		virtual ~CibleStatique() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Valeur::AdresseInstruction evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual SVM_Valeur::AdresseInstruction explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override { }
		virtual void format(std::ostream& os) const override
		{
			os << _etiquette;
		}
		virtual void html(std::ostream& os) const override
		{
			os << _etiquette;
		}
		private:
			std::string _etiquette;
			SVM_Valeur::AdresseInstruction _adresse;
	};

	struct CibleDynamique : public Cible
	{
		CibleDynamique(const ValeurSP& cible)
		:_cible(cible) {}
		virtual ~CibleDynamique() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Valeur::AdresseInstruction evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual SVM_Valeur::AdresseInstruction explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_cible;
		}
		virtual void html(std::ostream& os) const override
		{
			_cible->html(os);
		}
		private:
			ValeurSP _cible; // STR ou SYM
	};
}
}
}
}
