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
#include <src/machine/elements/memoire/type.h>
#include <src/machine/elements/programme/parametre.h>
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/elements/valeur/parametre.h>
#include <src/machine/extensions/reference_objet.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Memoire = SVM::Machine::Element::Memoire;
namespace SVM_Programme = SVM::Machine::Element::Programme;

namespace SVM
{
namespace Outils
{
	struct DefinitionObjet;
	struct Langage;
}
namespace Machine
{
namespace Extension
{
	DECL_SHARED_PTR(PatronObjetExtension);
	DECL_SHARED_PTR(AutomateNonDeterministe);

	struct MauvaisNomExtension : public SVM_Valeur::ExceptionDemarrage
	{
		MauvaisNomExtension(const std::string& extension, const std::string& nom, const size_t ligne)
		{
			DETAILS_EXCEPTION("Wrong plugin name " << nom << " for object in plugin " << extension << " configuration, line " << ligne);
		}
		virtual ~MauvaisNomExtension() {}
	};

	struct ExtensionObjectDuplique : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionObjectDuplique(const std::string& extension, const ReferenceObjet::Type type, const SVM_Valeur::PointEntreeExtension& nom, const size_t ligne_origine, const size_t ligne_duplicat)
		{
			DETAILS_EXCEPTION("Duplicated object " << ReferenceObjet::texte(type) << " " << nom << " in plugin " << extension << " configuration, line " << ligne_duplicat << ". First occurence is line " << ligne_origine << ".");
		}
		virtual ~ExtensionObjectDuplique() {}
	};

	struct ExtensionFonctionNonDefinie : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionFonctionNonDefinie(const std::string& nom, const std::string& retour_fonction, const std::string& fonction, const std::string& parametres_fonction)
		{
			DETAILS_EXCEPTION("Plugin file " << nom << " does not define C function " << retour_fonction << " " << fonction << "(" << parametres_fonction << ").");
		}
		virtual ~ExtensionFonctionNonDefinie() {}
	};

	struct ExtensionArgumentsDuplique : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionArgumentsDuplique(const std::string& nom, const SVM_Valeur::PointEntreeExtension& duplicat, const size_t ligne_duplicat)
		{
			DETAILS_EXCEPTION("Duplicated object ARGUMENTS " << duplicat << ", line " << ligne_duplicat << " in plugin " << nom << " configuration.");
		}
		virtual ~ExtensionArgumentsDuplique() {}
	};

	struct ExtensionOptionDuplique : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionOptionDuplique(const std::string& nom, const SVM_Valeur::PointEntreeExtension& duplicat, const std::string& clef, const size_t ligne_duplicat)
		{
			DETAILS_EXCEPTION("Object OPTION " << duplicat << ", line " << ligne_duplicat << " in plugin " << nom << " redefines option -" << clef << ". ");
		}
		virtual ~ExtensionOptionDuplique() {}
	};

	DECL_SHARED_PTR(RegexParametres);
	struct RegexParametres
	{
		virtual ~RegexParametres() {}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const RegexParametres& regex_parametres)
		{
			regex_parametres.format(f);
			if(not regex_parametres._nom.empty())
			{
				f << " : " << regex_parametres._nom;
			}
			return f;
		}
		virtual void format(std::ostream& os) const = 0;
		virtual operator AutomateNonDeterministeSP () const = 0;
		std::string _nom;
	};

	DECL_SHARED_PTR(RegexParametresSequence);
	struct RegexParametresSequence : public RegexParametres
	{
		virtual ~RegexParametresSequence() {}
		virtual void format(std::ostream& os) const override
		{
			for(auto& e: _elements)
			{
				os << " " << *e;
			}
		}
		virtual operator AutomateNonDeterministeSP () const override;
		std::vector<RegexParametresSP> _elements;
	};

	DECL_SHARED_PTR(RegexParametresElement);
	struct RegexParametresElement : public RegexParametres, public std::enable_shared_from_this<RegexParametresElement>
	{
		RegexParametresElement()
		:_modifiable(false) {}
		virtual ~RegexParametresElement() {}
		virtual RegexParametresElementSP joker() const = 0;
		virtual bool operator<(const RegexParametresElement& element) const = 0;
		virtual bool operator==(const RegexParametresElement& element) const = 0;
		virtual RegexParametresElementSP copie() const = 0;
		virtual bool autre() const { return false; }
		bool _modifiable;
	};

	struct RegexParametresJokerTotal : public RegexParametresElement
	{
		virtual ~RegexParametresJokerTotal() {}
		virtual void format(std::ostream& os) const override
		{
			os << ".";
		}
		virtual RegexParametresElementSP joker() const override
		{
			return RegexParametresElementSP();
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresJokerTotal>();
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			return false;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			return (typeid(*this).hash_code()==typeid(element).hash_code());
		}
	};
/*	Abandonne, car impossible avec les jokers
	struct RegexParametresAutre : public RegexParametresElement
	{
		typedef std::set<RegexParametresElementSP,Global::SPinferieurSP<RegexParametresElement> > EnsembleExclus;
		RegexParametresAutre(const EnsembleExclus& exclus)
		:_exclus(exclus) {}
		virtual ~RegexParametresAutre() {}
		virtual void format(std::ostream& os) const override
		{
			os << "!.[";
			for(auto& e:_exclus)
			{
				os << " " << *e;
			}
			os << " ]";
		}
		virtual RegexParametresElementSP joker() const override
		{
			return std::make_shared<RegexParametresJokerTotal>();
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresAutre>(_exclus);
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			return false;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			return (typeid(*this).hash_code()==typeid(element).hash_code());
		}
		virtual bool autre() const override { return true; }
		EnsembleExclus _exclus;
	};
*/
	struct RegexParametresJokerPartielValeur : public RegexParametresElement
	{
		virtual ~RegexParametresJokerPartielValeur() {}
		virtual void format(std::ostream& os) const override
		{
			if(_modifiable)
			{
				os << "MUTABLE ";
			}
			os << "VALUE";
		}
		virtual RegexParametresElementSP joker() const override
		{
			return std::make_shared<RegexParametresJokerTotal>();
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresJokerPartielValeur>();
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			if(_modifiable xor element._modifiable)
			{
				return element._modifiable;
			}
			return false;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return false;
			}
			return (_modifiable == element._modifiable);
		}
	};

	DECL_SHARED_PTR(RegexParametresValeur);
	struct RegexParametresValeur : public RegexParametresElement
	{
		RegexParametresValeur(const SVM_Memoire::Type& type)
		:_type(type) {}
		virtual ~RegexParametresValeur() {}
		virtual void format(std::ostream& os) const override
		{
			if(_modifiable)
			{
				os << "MUTABLE ";
			}
			os << _type;
		}
		virtual RegexParametresElementSP joker() const override
		{
			auto j = std::make_shared<RegexParametresJokerPartielValeur>();
			j->_modifiable=_modifiable;
			return j;
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresValeur>(_type);
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			if(_modifiable xor element._modifiable)
			{
				return element._modifiable;
			}
			auto e = dynamic_cast<const RegexParametresValeur&>(element);
			return _type<e._type;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return false;
			}
			if(_modifiable xor element._modifiable)
			{
				return false;
			}
			auto e = dynamic_cast<const RegexParametresValeur&>(element);
			return _type==e._type;
		}
		SVM_Memoire::Type _type;
	};

	struct RegexParametresJokerPartielMarqueur : public RegexParametresElement
	{
		virtual ~RegexParametresJokerPartielMarqueur() {}
		virtual void format(std::ostream& os) const override
		{
			os << "MARKER";
		}
		virtual RegexParametresElementSP joker() const override
		{
			return std::make_shared<RegexParametresJokerTotal>();
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresJokerPartielMarqueur>();
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			return false;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			return (typeid(*this).hash_code()==typeid(element).hash_code());
		}
	};

	struct RegexParametresMarqueur : public RegexParametresElement
	{
		RegexParametresMarqueur(const SVM_Valeur::ParametreMarqueur::Marqueur& marqueur)
		:_marqueur(marqueur) {}
		RegexParametresMarqueur(const SVM_Programme::ParametreMarqueur& marqueur)
		:_marqueur(marqueur) {}
		virtual ~RegexParametresMarqueur() {}
		virtual void format(std::ostream& os) const override
		{
			os << _marqueur;
		}
		virtual RegexParametresElementSP joker() const override
		{
			return std::make_shared<RegexParametresJokerPartielMarqueur>();
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresMarqueur>(_marqueur);
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			auto e = dynamic_cast<const RegexParametresMarqueur&>(element);
			return _marqueur<e._marqueur;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return false;
			}
			auto e = dynamic_cast<const RegexParametresMarqueur&>(element);
			return _marqueur==e._marqueur;
		}
		SVM_Programme::ParametreMarqueur _marqueur;
	};

	struct RegexParametresJokerPartielMotClef : public RegexParametresElement
	{
		virtual ~RegexParametresJokerPartielMotClef() {}
		virtual void format(std::ostream& os) const override
		{
			os << "KEYWORD";
		}
		virtual RegexParametresElementSP joker() const override
		{
			return std::make_shared<RegexParametresJokerTotal>();
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresJokerPartielMotClef>();
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			return false;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			return (typeid(*this).hash_code()==typeid(element).hash_code());
		}
	};

	struct RegexParametresMotClef : public RegexParametresElement
	{
		RegexParametresMotClef(const std::string& mot_clef)
		:_mot_clef(mot_clef) {}
		virtual ~RegexParametresMotClef() {}
		virtual void format(std::ostream& os) const override
		{
			os << "'" << _mot_clef << "'";
		}
		virtual RegexParametresElementSP joker() const override
		{
			return std::make_shared<RegexParametresJokerPartielMotClef>();
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresMotClef>(_mot_clef);
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			auto e = dynamic_cast<const RegexParametresMotClef&>(element);
			return _mot_clef<e._mot_clef;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return false;
			}
			auto e = dynamic_cast<const RegexParametresMotClef&>(element);
			return _mot_clef==e._mot_clef;
		}
		std::string _mot_clef;
	};

	struct RegexParametresJokerPartielStructure : public RegexParametresElement
	{
		virtual ~RegexParametresJokerPartielStructure() {}
		virtual void format(std::ostream& os) const override
		{
			if(_modifiable)
			{
				os << "MUTABLE ";
			}
			os << "STRUCT";
		}
		virtual RegexParametresElementSP joker() const override
		{
			return std::make_shared<RegexParametresJokerTotal>();
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresJokerPartielStructure>();
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			if(_modifiable xor element._modifiable)
			{
				return element._modifiable;
			}
			return false;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return false;
			}
			return (_modifiable == element._modifiable);
		}
	};

	struct RegexParametresJokerPartielVariable : public RegexParametresElement
	{
		virtual ~RegexParametresJokerPartielVariable() {}
		virtual void format(std::ostream& os) const override
		{
			os << "VARIABLE";
		}
		virtual RegexParametresElementSP joker() const override
		{
			return std::make_shared<RegexParametresJokerTotal>();
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresJokerPartielVariable>();
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			return false;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			return (typeid(*this).hash_code()==typeid(element).hash_code());
		}
	};

	DECL_SHARED_PTR(RegexParametresStructure);
	struct RegexParametresStructure : public RegexParametresElement
	{
		RegexParametresStructure(const SVM_Memoire::Type& type)
		:_type(type) {}
		virtual ~RegexParametresStructure() {}
		virtual void format(std::ostream& os) const override
		{
			if(_modifiable)
			{
				os << "MUTABLE ";
			}
			os << "$" << _type;
		}
		virtual RegexParametresElementSP joker() const override
		{
			auto j = std::make_shared<RegexParametresJokerPartielStructure>();
			j->_modifiable=_modifiable;
			return j;
		}
		virtual operator AutomateNonDeterministeSP () const override;
		virtual RegexParametresElementSP copie() const override
		{
			auto c = std::make_shared<RegexParametresStructure>(_type);
			c->_modifiable = _modifiable;
			return c;
		}
		virtual bool operator<(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return typeid(*this).hash_code()<typeid(element).hash_code();
			}
			if(_modifiable xor element._modifiable)
			{
				return element._modifiable;
			}
			auto e = dynamic_cast<const RegexParametresStructure&>(element);
			return _type<e._type;
		}
		virtual bool operator==(const RegexParametresElement& element) const override
		{
			if(typeid(*this).hash_code()!=typeid(element).hash_code())
			{
				return false;
			}
			if(_modifiable xor element._modifiable)
			{
				return false;
			}
			auto e = dynamic_cast<const RegexParametresStructure&>(element);
			return _type==e._type;
		}
		SVM_Memoire::Type _type;
	};
	DECL_SHARED_PTR(RegexParametresAlternativeSimple);
	
	struct RegexParametresAlternativeSimple : public RegexParametres
	{
		RegexParametresAlternativeSimple()
		:_inverse(false) {}
		virtual ~RegexParametresAlternativeSimple() {}
		virtual void format(std::ostream& os) const override
		{
			os << "[";
			if(_inverse)
			{
				os << " ^";
			}
			for(auto& e:_elements)
			{
				os << " " << *e;
			}
			os << " ]";
		}
		virtual operator AutomateNonDeterministeSP () const override;
		std::vector<RegexParametresElementSP> _elements;
		bool _inverse;
	};

	DECL_SHARED_PTR(RegexParametresAlternative);

	struct RegexParametresAlternative : public RegexParametres
	{
		virtual ~RegexParametresAlternative() {}
		virtual void format(std::ostream& os) const override
		{
			os << "(";
			bool sep=false;
			for(auto& a:_alternatives)
			{
				if(sep)
				{
					os << " |";
				}
				sep = true;
				os << *a;
			}
			os << " )";
		}
		virtual operator AutomateNonDeterministeSP () const override;
		std::vector<RegexParametresSequenceSP> _alternatives;
	};
	
	struct RegexParametresOptionnel : public RegexParametres
	{
		RegexParametresOptionnel(const RegexParametresSP& regex)
		:_regex(regex) {}
		virtual ~RegexParametresOptionnel() {}
		virtual void format(std::ostream& os) const override
		{
			os << *_regex << " ?";
		}
		virtual operator AutomateNonDeterministeSP () const override;
		RegexParametresSP _regex;
	};
	
	struct RegexParametresRepetitionOptionnel : public RegexParametres
	{
		RegexParametresRepetitionOptionnel(const RegexParametresSP& regex)
		:_regex(regex) {}
		virtual ~RegexParametresRepetitionOptionnel() {}
		virtual void format(std::ostream& os) const override
		{
			os << *_regex << " *";
		}
		virtual operator AutomateNonDeterministeSP () const override;
		RegexParametresSP _regex;
	};

	struct RegexParametresRepetitionObligatoire : public RegexParametres
	{
		RegexParametresRepetitionObligatoire(const RegexParametresSP& regex)
		:_regex(regex) {}
		virtual ~RegexParametresRepetitionObligatoire() {}
		virtual void format(std::ostream& os) const override
		{
			os << *_regex << " +";
		}
		virtual operator AutomateNonDeterministeSP () const override;
		RegexParametresSP _regex;
	};

	struct RegexParametresRepetitionFixe : public RegexParametres
	{
		RegexParametresRepetitionFixe(const RegexParametresSP& regex, const size_t repetitions)
		:_regex(regex),_repetitions(repetitions) {}
		virtual ~RegexParametresRepetitionFixe() {}
		virtual void format(std::ostream& os) const override
		{
			os << *_regex << " " << _repetitions;
		}
		virtual operator AutomateNonDeterministeSP () const override;
		RegexParametresSP _regex;
		size_t _repetitions;
	};

	DECL_SHARED_PTR(Objet);
	struct Objet
	{
		explicit Objet(const size_t ligne)
		:_ligne(ligne) {}
		virtual ~Objet() {}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Objet& objet)
		{
			objet.format(f);
			return f;
		}
		virtual void format(std::ostream& os) const = 0;
		virtual operator ReferenceObjet () const = 0;
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const = 0;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const = 0;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const = 0;
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const = 0;
		virtual bool ignore_aide() const { return false; }
		size_t _ligne;
	};

	struct Type : public Objet
	{
		Type(const size_t ligne, const SVM_Valeur::PointEntreeExtension& type)
		:Objet(ligne), _type(type) {}
		virtual ~Type() {}
		virtual void format(std::ostream& os) const override
		{
			os << "TYPE " << _type;
		}
		virtual operator ReferenceObjet () const override
		{
			return ReferenceObjet(ReferenceObjet::Type::TYPE,_type,_ligne);
		}
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const override;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const override;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const override
		{
			if(not verif(_type.nom_extension(),extension))
			{
				throw MauvaisNomExtension(extension,_type.nom_extension(),_ligne);
			}
		}
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const override;
		SVM_Valeur::PointEntreeExtension _type;
	};

	struct Structure : public Objet
	{
		Structure(const size_t ligne, const SVM_Valeur::PointEntreeExtension& structure)
		:Objet(ligne), _structure(structure) {}
		virtual ~Structure() {}
		virtual void format(std::ostream& os) const override
		{
			os << "STRUCT " << _structure;
		}
		virtual operator ReferenceObjet () const override
		{
			return ReferenceObjet(ReferenceObjet::Type::STRUCTURE,_structure,_ligne);
		}
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const override;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const override;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const override
		{
			if(not verif(extension,_structure.nom_extension()))
			{
				throw MauvaisNomExtension(extension,_structure.nom_extension(),_ligne);
			}
		}
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const override;
		SVM_Valeur::PointEntreeExtension _structure;
	};

	struct Instruction : public Objet
	{
		Instruction(const size_t ligne, const SVM_Valeur::PointEntreeExtension& instruction, const RegexParametresSequenceSP& parametres, const RegexParametresSP& retour, const bool systeme, const bool attente, const bool remplacement)
		:Objet(ligne), _instruction(instruction), _parametres(parametres), _retour(retour), _systeme(systeme), _attente(attente),_remplacement(remplacement) {}
		virtual ~Instruction() {}
		virtual void format(std::ostream& os) const override
		{
			if(_systeme)
			{
				os << "SYSTEM ";
			}
			if(_attente)
			{
				os << "WAITING ";
			}
			if(_remplacement)
			{
				os << "OVERRIDE ";
			}
			os << "INSTRUCTION " << _instruction << *_parametres;
			if(static_cast<bool>(_retour))
			{
				os << " -> " << *_retour;
			}
		}
		virtual operator ReferenceObjet () const override
		{
			return ReferenceObjet(ReferenceObjet::Type::INSTRUCTION,_instruction,_ligne);
		}
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const override;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const override;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const override
		{
			if(not verif(extension,_instruction.nom_extension()))
			{
				throw MauvaisNomExtension(extension,_instruction.nom_extension(),_ligne);
			}
		}
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const override;
		virtual bool ignore_aide() const override { return _remplacement; }

		SVM_Valeur::PointEntreeExtension _instruction;
		RegexParametresSequenceSP _parametres;
		RegexParametresSP _retour;
		bool _systeme;
		bool _attente;
		bool _remplacement;
	};

	struct Fonction : public Objet
	{
		Fonction(const size_t ligne, const SVM_Valeur::PointEntreeExtension& fonction, const RegexParametresSequenceSP& parametres, const RegexParametresSP& retour)
		:Objet(ligne), _fonction(fonction), _parametres(parametres), _retour(retour) {}
		virtual ~Fonction() {}
		virtual void format(std::ostream& os) const override
		{
			os << "FUNCTION " << _fonction << *_parametres;
			if(static_cast<bool>(_retour))
			{
				os << " -> " << *_retour;
			}
		}
		virtual operator ReferenceObjet () const override
		{
			return ReferenceObjet(ReferenceObjet::Type::FONCTION,_fonction,_ligne);
		}
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const override;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const override;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const override
		{
			if(not verif(extension,_fonction.nom_extension()))
			{
				throw MauvaisNomExtension(extension,_fonction.nom_extension(),_ligne);
			}
		}
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const override;
		SVM_Valeur::PointEntreeExtension _fonction;
		RegexParametresSequenceSP _parametres;
		RegexParametresSP _retour;
	};

	struct Interruption : public Objet
	{
		Interruption(const size_t ligne, const SVM_Valeur::PointEntreeExtension& interruption)
		:Objet(ligne), _interruption(interruption) {}
		virtual ~Interruption() {}
		virtual void format(std::ostream& os) const override
		{
			os << "INTERRUPTION " << _interruption;
		}
		virtual operator ReferenceObjet () const override
		{
			return ReferenceObjet(ReferenceObjet::Type::INTERRUPTION,_interruption,_ligne);
		}
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const override;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const override;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const override
		{
			if(not verif(extension,_interruption.nom_extension()))
			{
				throw MauvaisNomExtension(extension,_interruption.nom_extension(),_ligne);
			}
		}
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const override;
		SVM_Valeur::PointEntreeExtension _interruption;
	};

	struct Ordonnanceur : public Objet
	{
		Ordonnanceur(const size_t ligne, const SVM_Valeur::PointEntreeExtension& ordonnanceur)
		:Objet(ligne), _ordonnanceur(ordonnanceur) {}
		virtual ~Ordonnanceur() {}
		virtual void format(std::ostream& os) const override
		{
			os << "SCHEDULER " << _ordonnanceur;
		}
		virtual operator ReferenceObjet () const override
		{
			return ReferenceObjet(ReferenceObjet::Type::ORDONNANCEUR,_ordonnanceur,_ligne);
		}
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const override;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const override;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const override
		{
			if(not verif(extension,_ordonnanceur.nom_extension()))
			{
				throw MauvaisNomExtension(extension,_ordonnanceur.nom_extension(),_ligne);
			}
		}
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const override;
		SVM_Valeur::PointEntreeExtension _ordonnanceur;
	};

	struct Sequenceur : public Objet
	{
		Sequenceur(const size_t ligne, const SVM_Valeur::PointEntreeExtension& sequenceur)
		:Objet(ligne), _sequenceur(sequenceur) {}
		virtual ~Sequenceur() {}
		virtual void format(std::ostream& os) const override
		{
			os << "SEQUENCER " << _sequenceur;
		}
		virtual operator ReferenceObjet () const override
		{
			return ReferenceObjet(ReferenceObjet::Type::SEQUENCEUR,_sequenceur,_ligne);
		}
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const override;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const override;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const override
		{
			if(not verif(extension,_sequenceur.nom_extension()))
			{
				throw MauvaisNomExtension(extension,_sequenceur.nom_extension(),_ligne);
			}
		}
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const override;
		SVM_Valeur::PointEntreeExtension _sequenceur;
	};

	struct Option : public Objet
	{
		Option(const size_t ligne, const SVM_Valeur::PointEntreeExtension& option, const std::string& clef, const SVM_Memoire::Type::TypeInterne type)
		:Objet(ligne), _option(option), _clef(clef), _type(type) {}
		virtual ~Option() {}
		virtual void format(std::ostream& os) const override
		{
			os << "OPTION " << _option << " -" << _clef << " " << SVM_Memoire::Type(_type);
		}
		virtual operator ReferenceObjet () const override
		{
			return ReferenceObjet(ReferenceObjet::Type::OPTION,_option,_ligne);
		}
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const override;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const override;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const override
		{
			if(not verif(extension,_option.nom_extension()))
			{
				throw MauvaisNomExtension(extension,_option.nom_extension(),_ligne);
			}
		}
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const override;
		SVM_Valeur::PointEntreeExtension _option;
		std::string _clef;
		SVM_Memoire::Type::TypeInterne _type;
	};

	struct Argument : public Objet
	{
		Argument(const size_t ligne, const SVM_Valeur::PointEntreeExtension& argument, const size_t indice, const SVM_Memoire::Type::TypeInterne type)
		:Objet(ligne), _argument(argument), _indice(indice), _type(type) {}
		virtual ~Argument() {}
		virtual void format(std::ostream& os) const override
		{
			os << "ARGUMENT " << _argument << " " << SVM_Memoire::Type(_type);
		}
		virtual operator ReferenceObjet () const override
		{
			return ReferenceObjet(ReferenceObjet::Type::ARGUMENT,_argument,_ligne);
		}
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const override;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const override;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const override
		{
			if(not verif(extension,_argument.nom_extension()))
			{
				throw MauvaisNomExtension(extension,_argument.nom_extension(),_ligne);
			}
		}
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const override;
		SVM_Valeur::PointEntreeExtension _argument;
		size_t _indice;
		SVM_Memoire::Type::TypeInterne _type;
	};

	struct Arguments : public Objet
	{
		Arguments(const size_t ligne, const SVM_Valeur::PointEntreeExtension& arguments)
		:Objet(ligne), _arguments(arguments) {}
		virtual ~Arguments() {}
		virtual void format(std::ostream& os) const override
		{
			os << "ARGUMENTS " << _arguments;
		}
		virtual operator ReferenceObjet () const override
		{
			return ReferenceObjet(ReferenceObjet::Type::ARGUMENTS,_arguments,_ligne);
		}
		virtual PatronObjetExtensionSP patron(const std::string& nom, void *fichier) const override;
		virtual PatronObjetExtensionSP dependance(const std::string& nom) const override;
		virtual void verification(const std::string& extension, const std::function<bool(const std::string&, const std::string&)>& verif) const override
		{
			if(not verif(extension,_arguments.nom_extension()))
			{
				throw MauvaisNomExtension(extension,_arguments.nom_extension(),_ligne);
			}
		}
		virtual void generation(std::ostream& os, const SVM::Outils::Langage& langage, const SVM::Outils::DefinitionObjet* definition) const override;
		SVM_Valeur::PointEntreeExtension _arguments;
	};

	DECL_SHARED_PTR(Description);
	struct Description
	{
		Description(const std::string& nom, const std::string& source)
		:_nom(nom),_source(source), _erreur(false), _ligne(0), _indice_argument(0) {}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Description& description)
		{
			f	<< "Plugin " << description._nom << std::endl
				<< description._source << std::endl
				<< "Error: " << description._erreur << " at line " << description._ligne << std::endl
				<< description._message << std::endl
				<< "Uses:" << std::endl;
			for(auto& o:description._utilise)
			{
				f << "  " << *o << std::endl;
			}
			f	<< "Declares:" << std::endl;
			for(auto& o:description._declare)
			{
				f << "  " << *o << std::endl;
			}
			
			return f;
		}
		void verification() const
		{
			std::set<ReferenceObjet> utilisations;
			for(auto& u:_utilise)
			{
				u->verification(_nom,[] (const std::string& p, const std::string& nom) -> bool { return p!=nom;});
				auto it = utilisations.insert(*u); 
				if(not it.second)
				{
					throw ExtensionObjectDuplique(_nom,it.first->_type,it.first->_nom,it.first->_ligne,u->_ligne);
				}
			}
			std::set<ReferenceObjet> declarations;
			bool arguments = false;
			std::set<std::string> clefs_option;
			for(auto& d:_declare)
			{
				d->verification(_nom,[] (const std::string& p, const std::string& nom) -> bool { return p==nom;});
				ReferenceObjet r = *d;
				auto it = declarations.insert(r); 
				if(not it.second)
				{
					throw ExtensionObjectDuplique(_nom,it.first->_type,it.first->_nom,it.first->_ligne,d->_ligne);
				}
				if(r._type==ReferenceObjet::Type::ARGUMENTS)
				{
					if(arguments)
					{
						throw ExtensionArgumentsDuplique(_nom,r._nom,r._ligne);
					}
					else
					{
						arguments = true;
					}
				}
				if(r._type==ReferenceObjet::Type::OPTION)
				{
					auto o = std::dynamic_pointer_cast<Option>(d);
					if(not clefs_option.insert(o->_clef).second)
					{
						throw ExtensionOptionDuplique(_nom,r._nom,o->_clef,r._ligne);
					}
				}
			}
		}
		std::string chaine_configuration() const
		{
			std::ostringstream oss;
			oss	<< "PLUGIN " << _nom << std::endl;
			if(not _utilise.empty())
			{
				oss	<< "USE" << std::endl;
				for(const auto& u: _utilise)
				{
					oss << "\t" << (*u) << std::endl;
				}
			}
			oss	<< "DEFINE" << std::endl;
			for(const auto& d: _declare)
			{
				oss << "\t" << (*d) << std::endl;
			}
			return oss.str();
		}
		std::string _nom;
		std::string _source;
		bool _erreur;
		size_t _ligne;
		std::string _message;
		std::vector<ObjetSP> _utilise;
		std::vector<ObjetSP> _declare;
		size_t _indice_argument;
	};
}
}
}
namespace SVM_Extension = SVM::Machine::Extension;
