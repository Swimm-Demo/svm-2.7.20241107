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
#include <src/machine/elements/programme/cible.h>
#include <src/machine/elements/programme/type_programme.h>
#include <src/machine/elements/programme/valeur_programme.h>
#include <src/machine/elements/programme/adresse_programme.h>
#include <src/machine/elements/valeur/expression_condition.h>
#include <src/machine/elements/noyau/etat_noyau.h>
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
	struct ConditionBooleen : public SVM_Valeur::ExpressionCondition
	{
		ConditionBooleen(const SVM_Programme::ValeurSP& booleen)
		:_booleen(booleen) {}
		virtual ~ConditionBooleen() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual bool explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_booleen << " TRUE";
		}
		virtual void html(std::ostream& os) const override
		{
			_booleen->html(os);
			os << " <span class=\"keyword\">TRUE</span>";
		}
		private:
			SVM_Programme::ValeurSP _booleen;
	};

	struct ConditionDrapeau : public SVM_Valeur::ExpressionCondition
	{
		ConditionDrapeau(const SVM_Programme::ValeurSP& drapeau)
		:_drapeau(drapeau) {}
		virtual ~ConditionDrapeau() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual bool explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_drapeau << " RAISED";
		}
		virtual void html(std::ostream& os) const override
		{
			_drapeau->html(os);
			os << " <span class=\"keyword\">RAISED</span>";
		}
		private:
			SVM_Programme::ValeurSP _drapeau;
	};

	struct ConditionAdresseDefinie : public SVM_Valeur::ExpressionCondition
	{
		ConditionAdresseDefinie(const SVM_Programme::AdresseSP& adresse)
		:_adresse(adresse) {}
		virtual ~ConditionAdresseDefinie() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual bool explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_adresse << " DEFINED";
		}
		virtual void html(std::ostream& os) const override
		{
			_adresse->html(os);
			os << " <span class=\"keyword\">DEFINED</span>";
		}
		private:
			SVM_Programme::AdresseSP _adresse;
	};

	struct ConditionAdresseInitialisee : public SVM_Valeur::ExpressionCondition
	{
		ConditionAdresseInitialisee(const SVM_Programme::AdresseSP& adresse)
		:_adresse(adresse) {}
		virtual ~ConditionAdresseInitialisee() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual bool explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_adresse << " INITIALISED";
		}
		virtual void html(std::ostream& os) const override
		{
			_adresse->html(os);
			os << " <span class=\"keyword\">INITIALISED</span>";
		}
		private:
			SVM_Programme::AdresseSP _adresse;
	};

	struct ConditionIndiceDansPointeur : public SVM_Valeur::ExpressionCondition
	{
		ConditionIndiceDansPointeur(const SVM_Programme::ValeurSP& indice, const SVM_Programme::ValeurSP& pointeur)
		:_indice(indice), _pointeur(pointeur) {}
		virtual ~ConditionIndiceDansPointeur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual bool explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_indice << " IN " << *_pointeur;
		}
		virtual void html(std::ostream& os) const override
		{
			_indice->html(os);
			os << " <span class=\"keyword\">IN</span> ";
			_pointeur->html(os);
		}
		private:
			SVM_Programme::ValeurSP _indice;
			SVM_Programme::ValeurSP _pointeur;
	};

	struct ConditionAdresseDansPointeur : public SVM_Valeur::ExpressionCondition
	{
		ConditionAdresseDansPointeur(const SVM_Programme::AdresseSP& adresse, const SVM_Programme::ValeurSP& pointeur)
		:_adresse(adresse), _pointeur(pointeur) {}
		virtual ~ConditionAdresseDansPointeur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual bool explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_adresse << " IN " << *_pointeur;
		}
		virtual void html(std::ostream& os) const override
		{
			_adresse->html(os);
			os << " <span class=\"keyword\">IN</span> ";
			_pointeur->html(os);
		}
		private:
			SVM_Programme::AdresseSP _adresse;
			SVM_Programme::ValeurSP _pointeur;
	};

	struct ConditionAdresseEstType : public SVM_Valeur::ExpressionCondition
	{
		ConditionAdresseEstType(const SVM_Programme::AdresseSP& adresse, const SVM_Programme::TypeSP& type)
		:_adresse(adresse), _type(type) {}
		virtual ~ConditionAdresseEstType() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual bool explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_adresse << " IS " << *_type;
		}
		virtual void html(std::ostream& os) const override
		{
			_adresse->html(os);
			os << " <span class=\"keyword\">IS</span> ";
			_type->html(os);
		}
		private:
			SVM_Programme::AdresseSP _adresse;
			SVM_Programme::TypeSP _type;
	};

	struct ConditionCibleAppelable : public SVM_Valeur::ExpressionCondition
	{
		ConditionCibleAppelable(const SVM_Programme::CibleSP& cible)
		:_cible(cible) {}
		virtual ~ConditionCibleAppelable() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual bool explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_cible << " CALLABLE";
		}
		virtual void html(std::ostream& os) const override
		{
			_cible->html(os);
			os << " <span class=\"keyword\">CALLABLE</span>";
		}
		private:
			SVM_Programme::CibleSP _cible;
	};

	struct ConditionDebugueur : public SVM_Valeur::ExpressionCondition
	{
		ConditionDebugueur() {}
		virtual ~ConditionDebugueur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual bool explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override { }
		virtual void format(std::ostream& os) const override
		{
			os << "DEBUG";
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"keyword\">DEBUG</span>";
		}
	};

}
}
}
}
}
