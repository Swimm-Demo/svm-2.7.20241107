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
#include <src/machine/extensions/description.h>

namespace SVM
{
namespace Outils
{

struct Langage
{
	Langage()
	:_cpp(true), _langage("c++14") {}
	explicit Langage(const std::string& l)
	:_cpp(l.find("++")!=std::string::npos)
	{
		if(l=="C")
		{
			return;
		}
		if(l=="C++")
		{
			_langage = "c++14";
			return;
		}
		for(const auto& c: l)
		{
			if((c>='A') and (c<='Z'))
			{
				_langage += (c-'A'+'a');
			}
			else
			{
				_langage += c;
			}
		}
	}

	std::string extension() const
	{
		return _cpp?"cpp":"c";
	}
	std::string prefixe() const
	{
		return _cpp?"::":"";
	}
	std::string compilateur() const
	{
		return std::string(_cpp?"g++":"gcc")+option_compilateur();
	}
	std::string option_compilateur() const
	{
		if(_langage.empty()) return "";
		return std::string(" -std=")+_langage;
	}
	std::string macro_am() const
	{
		return _cpp?"PP":"";
	}
	std::string macro_ac() const
	{
		return _cpp?"XX":"";
	}
	std::string null() const
	{
		if(not _cpp) return "NULL";
		if(_langage=="c++98") return "0";
		return "nullptr";
	}
	operator bool () const { return _cpp; }
	bool _cpp;
	std::string _langage;
};

struct Commentaire
{
	Commentaire() = default;
	Commentaire(const std::string& prefixe, const std::string& ligne, const std::string& suffixe)
	:_prefixe(prefixe), _ligne(ligne), _suffixe(suffixe) {}

	std::string _prefixe;
	std::string _ligne;
	std::string _suffixe;
};

struct Fichier
{
	explicit Fichier(const std::string& valeur)
	:_valeur(valeur) {}
	Fichier(const std::string& type, const std::string& valeur)
	:_type(type), _valeur(valeur) {}
	Fichier(const std::string& type, const std::string& interpreteur, const std::string& valeur)
	:_type(type), _interpreteur(interpreteur), _valeur(valeur) {}
	void echappe_shell();
	void lignes_source(const std::string& nom);
	void ajoute_licence(const std::string& licence, const Commentaire& commentaire);

	std::string _type;
	std::string _interpreteur;
	std::string _valeur;
};

DECL_SHARED_PTR(Extension);
struct Fichiers
{
	typedef std::map<std::string,Fichier> Liste;
	static Fichier fichier_vide();
	static std::string macros(const Langage& langage);
	static std::string script(const Langage& langage, const bool licence, const bool force, const bool patch, const std::string& shell, const std::string& nom, const std::string& repertoire, const Fichiers::Liste& fichiers, const std::map<std::string,std::string>& corrections);
	static void commente(const std::string& texte, const Commentaire& commentaire, std::ostream& os);
};

struct DebutMacro
{
	explicit DebutMacro(const std::string& nom)
	:_nom(nom) {}
	DebutMacro(const std::string& nom, const std::string& parametres)
	:_nom(nom),_parametres(parametres) {}

	template<typename oStream>
	friend oStream& operator<<(oStream& os, const DebutMacro& dm)
	{
		os	<< "#ifndef " << dm._nom << std::endl
			<< "#define " << dm._nom;
		if(not dm._parametres.empty())
		{
			os	<< "(" << dm._parametres << ")";
		}
		os	<< " ";
		return os;
	}
	std::string _nom;
	std::string _parametres;
};

struct FinMacro
{
	template<typename oStream>
	friend oStream& operator<<(oStream& os, const FinMacro& fm)
	{
		os	<< std::endl
			<< "#endif" << std::endl;
		return os;
	}
};

struct Test
{
	typedef std::map<std::string,Test> Liste;
	Test(const std::string& commande, const Fichier& contenu)
	:_commande(commande), _contenu(contenu) {}
	
	const std::string _commande;
	const Fichier _contenu;
};

struct Generation
{
	static std::string source(const Langage& langage, const std::string& configuration, const SVM::Outils::ExtensionSP& extension);
	static std::string inclusion(const Langage& langage, const SVM::Outils::ExtensionSP& extension);
	static std::string documentation(const SVM::Outils::ExtensionSP& extension);
	static std::string makefile(const Langage& langage, const SVM::Outils::ExtensionSP& extension);
	static std::string makefile_inc(const Langage& langage, const SVM::Outils::ExtensionSP& extension);
	static std::string makefile_am_doc(const Langage& langage, const SVM::Outils::ExtensionSP& extension);
	static std::string makefile_am_src(const Langage& langage, const SVM::Outils::ExtensionSP& extension);
	static std::string makefile_am_test(const Langage& langage, const SVM::Outils::ExtensionSP& extension);
	static std::string makefile_am(const Langage& langage, const SVM::Outils::ExtensionSP& extension);
	static std::string configure_ac(const Langage& langage, const SVM::Outils::ExtensionSP& extension);
	static std::string nouvelles(const SVM::Outils::ExtensionSP& extension);
	static std::string lisez_moi(const SVM::Outils::ExtensionSP& extension);
	static std::string auteur(const SVM::Outils::ExtensionSP& extension);
	static std::string historique(const SVM::Outils::ExtensionSP& extension);
	static std::string installation(const std::string& nom);
	static Fichier test_source(const Test& t, const SVM::Outils::ExtensionSP& extension);
	static Fichier test_script(const std::string& nom, const Test& t, const bool initialisation, const bool finalisation, const SVM::Outils::ExtensionSP& extension);
	static std::string update_patches();
};

DECL_SHARED_PTR(Erreur);
struct Erreur
{
	explicit Erreur(const std::string& message)
	:_message(message), _debut(0), _fin(0) {}
	void position(const size_t debut, const size_t fin)
	{
		_debut = debut;
		_fin = (fin>debut)?fin:debut;
	}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const Erreur& e)
	{
		f << "Error";
		if(e._debut>0)
		{
			if(e._debut==e._fin)
			{
				f << " at line " << e._debut;
			}
			else
			{
				f << " at lines " << e._debut << "-" << e._fin;
			}
		}
		f << ": " << e._message << std::endl;
		return f;
	}
	private:
		std::string _message;
		size_t _debut;
		size_t _fin;
};

struct ChaineLocalisee
{
	ChaineLocalisee(const std::string& chaine, const size_t debut, const size_t fin)
	:_chaine(chaine), _debut(debut), _fin(fin) {}
	std::string _chaine;
	size_t _debut;
	size_t _fin;
};

DECL_SHARED_PTR(Definition);
struct Extension
{
	typedef std::map<std::string,std::string> Corrections;
	std::string _fichier;
	std::string _nom;
	std::map<std::string,ChaineLocalisee> _directive;
	std::vector<std::pair<std::string,std::string> > _exemples;
	Fichiers::Liste _fichiers;
	Test::Liste _tests;
	Corrections _corrections;
	std::vector<DefinitionSP> _dependance;
	std::vector<DefinitionSP> _definition;
	std::map<std::string,Commentaire> _commentaires;
	SVM::Machine::Extension::DescriptionSP _description;
	void nom(const std::string& nom)
	{
		_nom = nom;
	}
	void nouvelle_dependance(const DefinitionSP& dependance)
	{
		_dependance.push_back(dependance);
	}
	bool nouvelle_directive(const std::string& nom, const std::string& valeur, const size_t debut=0, const size_t fin=0)
	{
		return not _directive.insert(std::make_pair(nom,ChaineLocalisee(valeur,debut+1,fin-1))).second;
	}
	void nouvel_exemple(const std::string& nom, const std::string& valeur)
	{
		_exemples.push_back(std::make_pair(nom,valeur));
	}
	bool nouveau_fichier(const std::string& nom, const std::string& valeur, const std::string& type="")
	{
		std::ostringstream oss;
		oss << valeur << std::endl;
		return _fichiers.insert(std::make_pair(nom,Fichier(type,oss.str()))).second;
	}
	bool nouveau_test(const std::string& nom, const std::string& commande, const std::string& valeur)
	{
		std::ostringstream oss;
		oss << valeur << std::endl;
		return _tests.insert(std::make_pair(nom,Test(commande,Fichier("svm",oss.str())))).second;
	}
	bool nouvelle_correction(const std::string& nom, const std::string& valeur)
	{
		std::ostringstream oss;
		oss << valeur << std::endl;
		return _corrections.insert(std::make_pair(nom,oss.str())).second;
	}
	void nouvelle_definition(const DefinitionSP& definition)
	{
		_definition.push_back(definition);
	}
	bool nouveau_commentaire(const std::string& nom, const std::string& prefixe, const std::string& ligne, const std::string& suffixe)
	{
		return not _commentaires.insert(std::make_pair(nom,Commentaire(prefixe,ligne,suffixe))).second;
	}
	std::string chaine_configuration() const;
	SHARED_PTR(std::string) directive(const std::string& clef)
	{
		auto it = _directive.find(clef);
		if(it==_directive.end())
		{
			return SHARED_PTR(std::string)();
		}
		return std::make_shared<std::string>(it->second._chaine);
	}
	std::string directive_obligatoire(const std::string& clef)
	{
		auto it = _directive.find(clef);
		if(it==_directive.end())
		{
			throw Erreur("Internal error");
		}
		return it->second._chaine;
	}
	std::string directive_facultative(const std::string& clef, const std::string& alternative = std::string())
	{
		auto it = _directive.find(clef);
		if(it==_directive.end())
		{
			return alternative;
		}
		return it->second._chaine;
	}
	void directive_localisation(std::ostream& os, const std::string& clef)
	{
		auto it = _directive.find(clef);
		if(it==_directive.end())
		{
			return;
		}
		os	<< "#line " << it->second._debut << " \"" << _fichier << "\"" << std::endl
			<< it->second._chaine << std::endl
			<< "#line" << std::endl;
	}
	void verifie_et_complete(const std::string& fichier, SVM::Machine::Extension::DescriptionSP& description, std::string& configuration, Langage& langage, bool& licence, std::string& shell);
};

DECL_SHARED_PTR(Fonction);
struct Fonction
{
	enum class Option { AUCUNE, DEFAUT, CHAINE, OBJET };
	Fonction(const Option option, const std::string& corps, const size_t debut, const size_t fin)
	:_option(option), _corps(corps), _debut(debut+1), _fin(fin-1) {}
	explicit Fonction(const std::string& corps, const size_t debut, const size_t fin)
	:_option(Option::AUCUNE), _corps(corps), _debut(debut+1), _fin(fin-1) {}
	void corps(std::ostream& os, const std::string& fichier) const
	{
		if(_option!=Option::AUCUNE)
		{
			os	<< "\t{" << std::endl;
		}
		os	<< "#line " << _debut << " \"" << fichier << "\"" << std::endl
			<< _corps << std::endl
			<< "#line" << std::endl;
		if(_option!=Option::AUCUNE)
		{
			os	<< "\t}" << std::endl;
		}
	}
	Fonction::Option _option;
	std::string _corps;
	size_t _debut;
	size_t _fin;
};

struct Definition
{
	virtual ~Definition()
	{}
	virtual void generation(std::ostream& oss, const Langage& langage) const = 0;
	virtual void prototype(std::ostream& oss) const {}
	virtual void aide(std::ostream& oss) const {}
	std::string _fichier;
};

struct DefinitionObjet : public Definition
{
	DefinitionObjet(const std::string& prototype, const std::string& corps, const size_t debut, const size_t fin)
	:_prototype(prototype), _corps(corps), _debut(debut+1), _fin(fin-1)
	{
	}
	virtual ~DefinitionObjet() {};
	virtual void generation(std::ostream& oss, const Langage& langage) const override
	{
		oss 	<< std::endl
			<< "/* " << (*_objet) << " */" << std::endl
			<< std::endl;
		_objet->generation(oss,langage,this);
	}
	virtual void prototype(std::ostream& oss) const
	{
		oss << "\t" << _prototype << std::endl;
	}
	virtual void aide(std::ostream& oss) const
	{
		if(_objet->ignore_aide()) return;
		oss	<< ".SS " << (*_objet) << std::endl
			<< aide() << std::endl;
	}
	void corps(std::ostream& os) const
	{
		os	<< "#line " << _debut << " \"" << _fichier << "\"" << std::endl
			<< _corps << std::endl
			<< "#line" << std::endl;
	}
	
	bool nouvelle_fonction(const std::string& nom, const FonctionSP& fonction)
	{
		if(not fonction)
			return false;
		return not _fonction.insert(std::make_pair(nom,fonction)).second;
	}

	void nouvelle_aide(const SHARED_PTR(std::string)& aide)
	{
		_aide = aide;
	}

	std::string aide() const
	{
		if(not _aide) return "No help available. Please contact the maintainer of this plugin for support.";
		return (*_aide);
	}
	FonctionSP fonction(const std::string& clef) const
	{
		auto it=_fonction.find(clef);
		if(it==_fonction.end())
		{
			return FonctionSP();
		}
		return it->second;
	}
	std::string _prototype;
	std::string _corps;
	SVM::Machine::Extension::ObjetSP _objet;
	std::map<std::string,FonctionSP> _fonction;
	SHARED_PTR(std::string) _aide;
	size_t _debut;
	size_t _fin;
};

struct DefinitionCode : public Definition
{
	virtual ~DefinitionCode() {}
	virtual void generation(std::ostream& oss, const Langage& langage) const override
	{
		if(langage)
		{
			oss 	<< "}" << std::endl;
		}
		oss 	<< std::endl
			<< "/* code */" << std::endl
			<< std::endl
			<< "#line " << _debut << " \"" << _fichier << "\"" << std::endl
			<< _corps << std::endl
			<< "#line" << std::endl;
		if(langage)
		{
			oss 	<< std::endl
				<< "extern \"C\"" << std::endl
				<< "{" << std::endl;
		}
	}
	DefinitionCode(const std::string& corps, const size_t debut, const size_t fin)
	:_corps(corps), _debut(debut+1), _fin(fin-1) {}
	std::string _corps;
	size_t _debut;
	size_t _fin;
};

}
}
