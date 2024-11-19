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
#include <src/machine/elements/programme/adresse_programme.h>
#include <src/machine/elements/memoire/entier.h>
#include <src/machine/elements/memoire/chaine.h>
#include <src/machine/elements/memoire/booleen.h>
#include <src/machine/elements/memoire/pointeur.h>
#include <src/machine/elements/memoire/point_entree_extension.h>
#include <src/machine/elements/memoire/interruption.h>
#include <src/machine/elements/memoire/bibliotheque.h>
#include <src/machine/elements/memoire/symbole.h>
#include <src/machine/elements/memoire/extension.h>
#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/valeur/arbre_substitution.h>

namespace SVM_Memoire = SVM::Machine::Element::Memoire;
namespace SVM_Noyau = SVM::Machine::Element::Noyau;

namespace SVM
{
namespace Machine
{
namespace Extension
{
	DECL_SHARED_PTR(PatronInterruptionExtension);
}
namespace Element
{
namespace Processeur
{
namespace Instruction
{
	struct Extension;
}
}
namespace Valeur
{
	DECL_SHARED_PTR(ExpressionCondition);
}
namespace Programme
{
	namespace SVM_Instruction = SVM::Machine::Element::Processeur::Instruction;
	namespace SVM_Extension = SVM::Machine::Extension;

	DECL_SHARED_PTR(Valeur);
	
	struct Valeur
	{
		virtual ~Valeur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) = 0;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const = 0;
		long long int evaluation_entier(const SVM_Memoire::ValeurSP& valeur) const;
		std::string evaluation_chaine(const SVM_Memoire::ValeurSP& valeur) const;
		bool evaluation_booleen(const SVM_Memoire::ValeurSP& valeur) const;
		SVM_Valeur::Pointeur evaluation_pointeur(const SVM_Memoire::ValeurSP& valeur) const;
		SVM_Valeur::CodeSP evaluation_bibliotheque(const SVM_Memoire::ValeurSP& valeur) const;
		SVM_Valeur::AdresseInstruction evaluation_symbole(const SVM_Memoire::ValeurSP& valeur) const;
		SVM_Valeur::PointEntreeExtension evaluation_point_entree_extension(const SVM_Memoire::ValeurSP& valeur) const;
		SVM_Valeur::Interruption evaluation_interruption(const SVM_Memoire::ValeurSP& valeur) const;
		long long int evaluation_entier(const SVM_Noyau::NoyauSP& noyau) const { return evaluation_entier(evaluation(noyau)); }
		std::string evaluation_chaine(const SVM_Noyau::NoyauSP& noyau) const { return evaluation_chaine(evaluation(noyau)); }
		bool evaluation_booleen(const SVM_Noyau::NoyauSP& noyau) const { return evaluation_booleen(evaluation(noyau)); }
		SVM_Valeur::Pointeur evaluation_pointeur(const SVM_Noyau::NoyauSP& noyau) const { return evaluation_pointeur(evaluation(noyau)); }
		SVM_Valeur::CodeSP evaluation_bibliotheque(const SVM_Noyau::NoyauSP& noyau) const { return evaluation_bibliotheque(evaluation(noyau)); }
		SVM_Valeur::AdresseInstruction evaluation_symbole(const SVM_Noyau::NoyauSP& noyau) const { return evaluation_symbole(evaluation(noyau)); }
		SVM_Valeur::PointEntreeExtension evaluation_point_entree_extension(const SVM_Noyau::NoyauSP& noyau) const { return evaluation_point_entree_extension(evaluation(noyau)); }
		SVM_Valeur::Interruption evaluation_interruption(const SVM_Noyau::NoyauSP& noyau) const { return evaluation_interruption(evaluation(noyau)); }
		void explique(const SVM_Noyau::NoyauSP& noyau, std::ostream& os) const;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const = 0;
		long long int explique_calcul_entier(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre);
		std::string explique_calcul_chaine(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre);
		bool explique_calcul_booleen(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre);
		SVM_Valeur::Pointeur explique_calcul_pointeur(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre);
		SVM_Valeur::PointEntreeExtension explique_calcul_point_entree_extension(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre);
		SVM_Valeur::CodeSP explique_calcul_bibliotheque(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre);
		virtual void format(std::ostream& os) const = 0;
		virtual void html(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Valeur& valeur)
		{
			valeur.format(f);
			return f;
		}
	};

	DECL_SHARED_PTR(LectureMemoire);

	struct LectureMemoire : public Valeur
	{
		LectureMemoire(const AdresseSP& adresse)
		:_adresse(adresse) { }
		virtual ~LectureMemoire() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		void type_voulu(std::function<bool(const SVM_Memoire::Type& type)> validateur, const std::string& types_attendus)
		{
			_validateur = validateur;
			_types_attendus = types_attendus;
		}
		virtual void format(std::ostream& os) const override
		{
			os << "@" << (*_adresse);
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"operator\">@</span>";
			_adresse->html(os);
		}
		private:
		AdresseSP _adresse;
		std::function<bool(const SVM_Memoire::Type& type)> _validateur;
		std::string _types_attendus;
	};

	struct Entier : public Valeur
	{
		explicit Entier(const long int valeur)
		:_valeur(std::make_shared<SVM_Memoire::Entier>(valeur)) { }
		virtual ~Entier() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_valeur;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"constant\">" << *_valeur << "</span>";
		}

		operator long long int () const
		{
			return *_valeur;
		}

		private:
			SVM_Memoire::EntierSP _valeur;
	};
	
	struct TaillePointeur : public Valeur
	{
		explicit TaillePointeur(const ValeurSP& pointeur)
		:_pointeur(pointeur) { }
		virtual ~TaillePointeur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << "SIZE " << *_pointeur; 
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"keyword\">SIZE</span> ";
			_pointeur->html(os);
		}
		private:
			ValeurSP _pointeur;
	};
	
	DECL_SHARED_PTR(Chaine);
	struct ConstanteExtension;
	struct Chaine : public Valeur
	{
		friend struct ConstanteExtension;
		Chaine(const std::string& valeur, const bool multi)
		:_valeur(std::make_shared<SVM_Memoire::Chaine>(valeur)),_multi(multi)
		{}
		virtual ~Chaine() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			if(_multi)
			{
				os << "\"\"\"" << (*_valeur) << "\"\"\"";
			}
			else
			{
				os << _valeur->serialise();
			}
		}
		virtual void html(std::ostream& os) const override
		{
			if(_multi)
			{
				os << "<span class=\"constant\">\"\"\"" << SVM::Global::Chaine::echappe_html(*_valeur) << "\"\"\"</span>";
			}
			else
			{
				os << "<span class=\"constant\">";
				_valeur->html(os);
				os << "</span>";
			}
		}
		private:
			SVM_Memoire::ChaineSP _valeur;
			bool _multi;
	};
	
	struct Booleen : public Valeur
	{
		explicit Booleen(const long int valeur)
		:_valeur(std::make_shared<SVM_Memoire::Booleen>(valeur))
		{
		}
		virtual ~Booleen() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_valeur;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"constant\">" << *_valeur << "</span>";
		}
		private:
			SVM_Memoire::BooleenSP _valeur;
	};

	struct BooleenCondition : public Valeur
	{
		explicit BooleenCondition(const SVM_Valeur::ExpressionConditionSP& condition)
		:_valeur(condition) {}
		virtual ~BooleenCondition() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override;
		virtual void html(std::ostream& os) const override;
		private:
			SVM_Valeur::ExpressionConditionSP _valeur;
	};

	struct Pointeur : public Valeur
	{
		Pointeur(const size_t& adresse, const size_t& taille)
		:_valeur(std::make_shared<SVM_Memoire::Pointeur>(SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(adresse),taille))) {}
		virtual ~Pointeur() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_valeur; 
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"constant\">" << *_valeur << "</span>";
		}
		private:
			SVM_Memoire::PointeurSP _valeur;
	};

	struct TaillePointeurInvalide : public SVM_Valeur::ExceptionExecution
	{
		TaillePointeurInvalide(const ssize_t taille)
		:ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE)
		{
			DETAILS_EXCEPTION("Pointer size " << taille << " invalid.");
		}
		virtual ~TaillePointeurInvalide() {}
	};

	struct PointeurConstruit : public Valeur
	{
		PointeurConstruit(const AdresseSP& adresse, const ValeurSP& taille)
		:_adresse(adresse), _taille(taille) {}
		virtual ~PointeurConstruit() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_adresse << "*" << *_taille;
		}
		virtual void html(std::ostream& os) const override
		{
			_adresse->html(os);
			os << "*";
			_taille->html(os);
		}
		private:
			AdresseSP _adresse;
			ValeurSP _taille;
	};
	
	struct PointeurAlias : public Valeur
	{
		PointeurAlias(const std::string& alias)
		:_alias(alias) {}
		virtual ~PointeurAlias() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << _alias;
		}
		virtual void html(std::ostream& os) const override
		{
			os << _alias;
		}
		private:
			std::string _alias;
	};

	struct PointeurCourant : public Valeur
	{
		PointeurCourant()
		{}
		virtual ~PointeurCourant() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << "P";
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"keyword\">P</span>";
		}
		private:
	};

	struct SymboleLocal : public Valeur
	{
		SymboleLocal(const ValeurSP& symbole)
		:_symbole(symbole) {}
		virtual ~SymboleLocal() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << "$" << *_symbole;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "$";
			_symbole->html(os);
		}
		private:
			ValeurSP _symbole;
	};

	struct SymboleDebut : public Valeur
	{
		SymboleDebut(const ValeurSP& bibliotheque)
		:_bibliotheque(bibliotheque) {}
		virtual ~SymboleDebut() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << "$(" << *_bibliotheque << "/)";
		}
		virtual void html(std::ostream& os) const override
		{
			os << "$(";
			_bibliotheque->html(os);
			os << "/)";
		}
		private:
			ValeurSP _bibliotheque;
	};

	struct SymboleComplet : public Valeur
	{
		SymboleComplet(const ValeurSP& bibliotheque, const ValeurSP& symbole)
		:_bibliotheque(bibliotheque), _symbole(symbole) {}
		virtual ~SymboleComplet() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << "$(" << *_bibliotheque << "/" << *_symbole << ")";
		}
		virtual void html(std::ostream& os) const override
		{
			os << "$(";
			_bibliotheque->html(os);
			os << "/";
			_symbole->html(os);
			os << ")";
		}
		private:
			ValeurSP _bibliotheque;
			ValeurSP _symbole;
	};

	struct InterruptionExterne;
	struct TypeExtension;

	DECL_SHARED_PTR(PointEntreeExtension);

	struct PointEntreeExtension : public Valeur
	{
		friend struct InterruptionExterne;
		friend struct SVM_Instruction::Extension;
		friend struct TypeExtension;
		PointEntreeExtension(const SVM_Valeur::PointEntreeExtension& pep)
		:_pep(std::make_shared<SVM_Memoire::PointEntreeExtension>(pep)) {}
		virtual ~PointEntreeExtension() {};
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_pep;
		}
		virtual void html(std::ostream& os) const override
		{
			_pep->html(os);
		}
		private:
			SVM_Memoire::PointEntreeExtensionSP _pep;
	};

	struct InterruptionInterne : public Valeur
	{
		InterruptionInterne(const SVM_Valeur::Interruption::InterruptionInterne& interruption)
		:_interruption(std::make_shared<SVM_Memoire::Interruption>(SVM_Valeur::Interruption(interruption,""))) {}
		virtual ~InterruptionInterne() {};
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_interruption;
		}
		virtual void html(std::ostream& os) const override
		{
			_interruption->html(os);
		}
		private:
			SVM_Memoire::InterruptionSP _interruption;
	};

	struct InterruptionInexistante : public SVM_Valeur::ExceptionExecution
	{
		InterruptionInexistante(const PointEntreeExtension& nom)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR)
		{
			DETAILS_EXCEPTION("Interruption " << nom << " is not defined.");
		}
		virtual ~InterruptionInexistante() {}
	};

	struct InterruptionExterne : public Valeur
	{
		InterruptionExterne(const ValeurSP& pep)
		:_interruption(std::make_shared<SVM_Memoire::Interruption>(SVM_Valeur::Interruption(std::dynamic_pointer_cast<PointEntreeExtension>(pep)->_pep->_valeur,""))) {}
		virtual ~InterruptionExterne() {};
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << *_interruption;
		}
		virtual void html(std::ostream& os) const override
		{
			_interruption->html(os);
		}
		private:
			SVM_Memoire::InterruptionSP _interruption;
	};

	struct ConstanteExtension : public Valeur
	{
		ConstanteExtension(const SVM_Valeur::PointEntreeExtension& type, const ChaineSP& texte, const SVM_Valeur::PositionInstruction& position)
		:_type(type), _texte(texte), _position(position) {}
		virtual ~ConstanteExtension() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::ValeurSP evaluation(const SVM_Noyau::NoyauSP& ce) const override;
		virtual SVM_Memoire::ValeurSP explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void format(std::ostream& os) const override
		{
			os << "CONST " << _type << " " << *_texte;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"keyword\">CONST</span> " << _type << " ";
			_texte->html(os);
		}
		private:
		SVM_Valeur::PointEntreeExtension _type;
		ChaineSP _texte;
		SVM_Memoire::ExtensionSP _valeur;
		SVM_Valeur::PositionInstruction _position;
	};
}
}
}
}
