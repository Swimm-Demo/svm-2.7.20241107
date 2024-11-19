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
#include <src/machine/elements/valeur/pointeur.h>
#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/valeur/decorateur.h>
#include <src/machine/elements/valeur/arbre_substitution.h>

namespace SVM_Noyau = SVM::Machine::Element::Noyau;
namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Programme
{
	DECL_SHARED_PTR(Adresse);

	struct Adresse
	{
		Adresse() = default;
		virtual ~Adresse() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) = 0;
		virtual SVM_Valeur::AdresseMemoire evaluation(const SVM_Noyau::NoyauSP& ce) const = 0;
		void explique(const SVM_Noyau::NoyauSP& noyau, std::ostream& os) const;
		virtual SVM_Valeur::AdresseMemoire explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const = 0;
		virtual void format(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Adresse& adresse)
		{
			adresse.format(f);
			return f;
		}
		virtual void html(std::ostream& os) const = 0;

	};

	struct AdresseConstante : public Adresse
	{
		explicit AdresseConstante(const size_t adresse)
		:_adresse(SVM_Valeur::AdresseMemoire(adresse)) {}
		virtual ~AdresseConstante() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Valeur::AdresseMemoire evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Valeur::AdresseMemoire explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << _adresse;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"constant\">" << _adresse << "</span>";
		}
		operator size_t () const
		{
			return _adresse._adresse;
		}
		private:
		SVM_Valeur::AdresseMemoire _adresse;
	};

	DECL_SHARED_PTR(Valeur);
	struct AdresseDepuisPointeur : public Adresse
	{
		explicit AdresseDepuisPointeur(const ValeurSP& pointeur)
		:_pointeur(pointeur) {}
		virtual ~AdresseDepuisPointeur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Valeur::AdresseMemoire evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Valeur::AdresseMemoire explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override;
		virtual void html(std::ostream& os) const override;
		private:
			ValeurSP _pointeur;
	};

	struct AdresseDepuisIndicePointeur : public Adresse
	{
		explicit AdresseDepuisIndicePointeur(const ValeurSP& pointeur, const ValeurSP& indice)
		:_pointeur(pointeur), _indice(indice) {}
		virtual ~AdresseDepuisIndicePointeur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Valeur::AdresseMemoire evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Valeur::AdresseMemoire explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override;
		virtual void html(std::ostream& os) const override;
		private:
			ValeurSP _pointeur;
			ValeurSP _indice;
	};

	struct AdresseDecalage : public Adresse
	{
		explicit AdresseDecalage(const AdresseSP& adresse, const int sens, const ValeurSP& indice)
		:_adresse(adresse), _sens(sens), _indice(indice) {}
		virtual ~AdresseDecalage() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Valeur::AdresseMemoire evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Valeur::AdresseMemoire explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override;
		virtual void html(std::ostream& os) const override;
		private:
			AdresseSP _adresse;
			int _sens;
			ValeurSP _indice;
	};
}
}
}
}
