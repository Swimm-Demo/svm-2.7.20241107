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
#include <src/machine/elements/valeur/parametre.h>

namespace SVM_Memoire = SVM::Machine::Element::Memoire;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Programme
{
	DECL_SHARED_PTR(Parametre);
	
	struct Parametre
	{
		virtual ~Parametre() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) {}
		virtual SVM_Valeur::ParametreSP evaluation(const SVM_Noyau::NoyauSP& noyau) const = 0;
		virtual void format(std::ostream& os) const = 0;
		virtual void html(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Parametre& parametre)
		{
			parametre.format(f);
			return f;
		}
	};

	struct ParametreValeur : public Parametre
	{
		ParametreValeur(const ValeurSP& valeur)
		:_valeur(valeur) {}
		virtual ~ParametreValeur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override
		{
			_valeur->decoration(decorateur);
		}
		virtual SVM_Valeur::ParametreSP evaluation(const SVM_Noyau::NoyauSP& noyau) const override
		{
			return std::make_shared<SVM_Valeur::ParametreValeur>(_valeur->evaluation(noyau));
		}
		void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
		{
			std::ostringstream val;
			_valeur->explique(noyau,val);
			explications.push_back(val.str());
		}
		virtual void format(std::ostream& os) const override
		{
			os << *_valeur;
		}
		virtual void html(std::ostream& os) const override
		{
			_valeur->html(os);
		}
		private:
			ValeurSP _valeur; 
	};

	struct ParametreMarqueur : public Parametre
	{
		ParametreMarqueur(const SVM_Valeur::ParametreMarqueur::Marqueur& marqueur)
		:_marqueur(marqueur) {}
		virtual ~ParametreMarqueur() {}
		bool operator<(const ParametreMarqueur& marqueur) const
		{
			return _marqueur<marqueur._marqueur;
		}
		bool operator==(const ParametreMarqueur& marqueur) const
		{
			return _marqueur==marqueur._marqueur;
		}
		virtual SVM_Valeur::ParametreSP evaluation(const SVM_Noyau::NoyauSP& noyau) const override
		{
			return std::make_shared<SVM_Valeur::ParametreMarqueur>(_marqueur);
		}
		virtual void format(std::ostream& os) const override
		{
			os << SVM_Valeur::ParametreMarqueur::texte(_marqueur);
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"operator\">" << SVM::Global::Chaine::echappe_html(SVM_Valeur::ParametreMarqueur::texte(_marqueur)) << "</span>";
		}
		private:
			SVM_Valeur::ParametreMarqueur::Marqueur _marqueur;
	};

	struct ParametreMotClef : public Parametre
	{
		ParametreMotClef(const std::string& mot_clef)
		:_mot_clef(mot_clef) {}
		virtual ~ParametreMotClef() {}
		virtual SVM_Valeur::ParametreSP evaluation(const SVM_Noyau::NoyauSP& noyau) const override
		{
			return std::make_shared<SVM_Valeur::ParametreMotClef>(_mot_clef);
		}
		virtual void format(std::ostream& os) const override
		{
			os << _mot_clef;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"keyword\">" << _mot_clef << "</span>";
		}
		private:
			std::string _mot_clef;
	};

	struct ParametreCalcule : public Parametre
	{
		ParametreCalcule(const SVM_Valeur::ParametreSP& p)
		:_parametre(p) {}
		virtual ~ParametreCalcule() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override
		{
		}
		virtual SVM_Valeur::ParametreSP evaluation(const SVM_Noyau::NoyauSP& noyau) const override
		{
			return _parametre;
		}
		virtual void format(std::ostream& os) const override
		{
			os << *_parametre;
		}
		virtual void html(std::ostream& os) const override
		{
			os << *_parametre;
		}
		private:
			SVM_Valeur::ParametreSP _parametre; 
	};
}
}
}
}
