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

// ============================================
// Convention de nommage des fonctions de l'API
// ============================================
//
// Format : svm_<concept>{_<sousconcept>}...{_<sousconcept>}_<action>{_<complement>}{_<complement>}{__<variante>}
//	concept => categorie majeure de l'API
//	sousconcept => categorie mineure de l'API. Il peut y avoir autant de sous concepts que necessaire
//	action => operation effectuee sur le concept/sousconcept de l'API
//		Actions generiques :
//			new    :  cree un objet => implique un sousconcept (egal a l'objet) pour les fonctions associees a cet objet, sauf si egal au concept
//			delete :  detruit un objet => que pour svm_variable_delete
//			set    :  change la valeur attribut
//			reset  :  enleve la valeur attribut
//			get    :  recupere la valeur attribut
//			add    :  ajoute une valeur liste attribut
//			remove :  retire une valeur liste attribut
//			clear  :  retire toutes les valeurs liste attribut
//			list   :  donne toutes les valeurs liste attribut
//			is     :  teste attribut booleen
//			has    :  teste presence attribut ou liste attribut non vide
//		Actions specifiques :
//			Le verbe necessaire hors action generique.
//	complement => information complementaire a l'action.
//	variante => types parametres fonction C
//		raw    : type C
//		string : type SVM_String
//		buffer : type chaine C avec taille
//
// Regles :
// 	1) en cas de repetition d'un mot : un seul est indique, les autres sont omis
//		ex : svm_debug_form_new_form devient svm_debug_form_new
// 	2) quand il y a le choix : preferer sousconcept au complement pour autocompletion
// 		ex : svm_debug_form_new_form devient plutot svm_debug_form_new que svm_debug_new_form
//

#include <src/machine/interface/svm.h>
#include <src/global/global.h>
#include <svm_compatibility.h>
#include <src/machine/extensions/gestionnaire/gestionnaire.h>
#include <src/machine/machine/machine.h>
#include <src/machine/elements/memoire/entier.h>
#include <src/machine/elements/memoire/chaine.h>
#include <src/machine/elements/memoire/booleen.h>
#include <src/machine/elements/memoire/point_entree_extension.h>
#include <src/machine/elements/memoire/interruption.h>
#include <src/machine/elements/memoire/extension.h>
#include <src/machine/interface/structure.h>
#include <src/machine/interface/fonction.h>
#include <src/machine/elements/processus/processus.h>
#include <src/machine/elements/ordonnanceur/ordonnanceur.h>
#include <src/machine/elements/synchronisation/evenement.h>
#include <src/machine/elements/systeme/systeme.h>
#include <src/machine/elements/programme/analyseur/analyseur.h>
#include <src/machine/elements/programme/parametre.h>
#include <src/machine/elements/processeur/instructions/extension.h>
#include <src/global/installation.h>

namespace SVM_Extension = SVM::Machine::Extension;
namespace SVM_Processus = SVM::Machine::Element::Processus;
namespace SVM_Programme = SVM::Machine::Element::Programme;
namespace SVM_Ordonnanceur = SVM::Machine::Element::Ordonnanceur;
namespace SVM_Synchronisation = SVM::Machine::Element::Synchronisation;

struct MauvaisAppelFonction : public SVM_Valeur::ExceptionExecution
{
	MauvaisAppelFonction(const std::string& fonction, const std::string& erreur)
	:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
	{
		DETAILS_EXCEPTION("Invalid call to API function " << fonction << ": " << erreur << ".");
	}
	virtual ~MauvaisAppelFonction() {}	
};

struct MauvaisAppelFonctionSecurite : public SVM_Valeur::ExceptionExecution
{
	MauvaisAppelFonctionSecurite(const std::string& fonction, const std::string& erreur)
	:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::SECURITE)
	{
		DETAILS_EXCEPTION("Invalid call to API function " << fonction << ": " << erreur << ".");
	}
	virtual ~MauvaisAppelFonctionSecurite() {}	
};

namespace SVM
{
namespace Machine
{
namespace Interface
{

DECL_SHARED_PTR(TableauVariables);
struct TableauVariables
{
	TableauVariables(const std::string& nom, const size_t taille)
	:_nom(nom)
	{
		_variables = new SVM_Variable[taille];
	}
	~TableauVariables()
	{
		delete [] _variables;
	}
	const void* avatar() const { return _variables; }
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const TableauVariables& t)
	{
		f << "<" << t._nom << ">" ;
		return f;
	}
	std::string _nom;
	SVM_Variable* _variables;
};

DECL_SHARED_PTR(VerrouProcessus);
struct VerrouProcessus
{
	static SVM_Process_Lock verrouillage(const SVM_Interface::EnvironnementSP& environnement, const SVM_Processus::ProcessusSP& processus)
	{
		if(not processus)
		{
			return nullptr;
		}
		auto vp = environnement->valeur<VariableProcessus>(environnement->_variable_processus);
		if(static_cast<bool>(vp))
		{
			SVM_Processus::ProcessusSP processus_courant = vp->_processus;
			if(static_cast<bool>(processus_courant))
			{
				if(processus==processus_courant)
				{
					return nullptr;
				}
				if(not processus->bloquage_demande(processus_courant->_adresse))
					return nullptr;
				auto reponse = processus_courant->attente_verrouillage(processus->_adresse);
				if((reponse==SVM_Systeme::Evenement::Reponse::OUI) or (reponse==SVM_Systeme::Evenement::Reponse::DEJA_VERROUILLE))
				{
					return environnement->ajout(std::make_shared<VerrouProcessus>(processus,processus_courant->_adresse));
				}
				return nullptr;
			}
		}
		SVM_Ordonnanceur::OrdonnanceurSP ordonnanceur_courant = environnement->valeur<SVM_Ordonnanceur::Ordonnanceur>(environnement->_variable_ordonnanceur);
		if(static_cast<bool>(ordonnanceur_courant))
		{
			if(not processus->bloquage_demande(ordonnanceur_courant->adresse()))
				return nullptr;
			auto reponse = ordonnanceur_courant->attente_verrouillage(processus->_adresse);
			if((reponse==SVM_Systeme::Evenement::Reponse::OUI) or (reponse==SVM_Systeme::Evenement::Reponse::DEJA_VERROUILLE))
			{
				return environnement->ajout(std::make_shared<VerrouProcessus>(processus,ordonnanceur_courant->adresse()));
			}
			return nullptr;
		}
		if(not processus->bloquage_demande(environnement->_machine->_systeme->_adresse))
			return nullptr;
		auto reponse = environnement->_machine->_systeme->attente_verrouillage(processus->_adresse);
		if((reponse==SVM_Systeme::Evenement::Reponse::OUI) or (reponse==SVM_Systeme::Evenement::Reponse::DEJA_VERROUILLE))
		{
			return environnement->ajout(std::make_shared<VerrouProcessus>(processus,environnement->_machine->_systeme->_adresse));
		}
		return nullptr;
	}
	static SVM_Process_Lock verrouillage_total(const SVM_Interface::EnvironnementSP& environnement)
	{
		SVM_Synchronisation::AdresseSP source;
		SVM_Systeme::SystemeSP systeme;
		SVM_Processus::ProcessusSP processus_courant = environnement->valeur<VariableProcessus>(environnement->_variable_processus)->_processus;
		if(static_cast<bool>(processus_courant))
		{
			source = processus_courant->_adresse;
			systeme = processus_courant->_systeme.lock();
		}
		else
		{
			return nullptr;
		}
		auto liste_processus_brute = systeme->liste_processus();
		std::vector<SVM_Processus::ProcessusSP> liste_processus;
		for(auto& processus: liste_processus_brute)
		{
			if(processus->termine()) continue;
			if(processus==processus_courant) continue;
			liste_processus.push_back(processus);
		}
		std::vector<SVM_Processus::ProcessusSP> verrouilles;
		bool encore;
		do
		{
			encore = false;
			std::vector<SVM_Processus::ProcessusSP> retente;
			for(auto& processus: liste_processus)
			{
				if(processus->tentative_bloquage_demande(source))
				{
					retente.push_back(processus);
				}
			}
			liste_processus.swap(retente);
			retente.clear();
			while(not liste_processus.empty())
			{
				auto r = processus_courant->attente_verrouillage();
				if(not r.second)
				{
					return nullptr;
				}
				auto s = SVM_Systeme::Livraison::objet<SVM_Processus::Processus>(r.second);
				switch(r.first)
				{
					case SVM_Systeme::Evenement::Reponse::OUI:
					case SVM_Systeme::Evenement::Reponse::DEJA_VERROUILLE:
						{
							verrouilles.push_back(s);
							encore = true;
							break;
						}
					case SVM_Systeme::Evenement::Reponse::AUTRE_VERROUILLE:
						{
							retente.push_back(s);
							break;
						}
					default:
						return nullptr;
						break;
				}
				auto it = std::find(liste_processus.begin(),liste_processus.end(),s);
				if(it!=liste_processus.end())
				{
					liste_processus.erase(it);
				}
			}
			liste_processus.swap(retente);
		}
		while(encore);
		return environnement->ajout(std::make_shared<VerrouProcessus>(verrouilles,source));
	}
	VerrouProcessus(const SVM_Processus::ProcessusSP& processus, const SVM_Synchronisation::AdresseSP& source)
	:_source(source) { _processus.push_back(processus); /* construit quand verrouillage OK */ }
	VerrouProcessus(const std::vector<SVM_Processus::ProcessusSP>& processus, const SVM_Synchronisation::AdresseSP& source)
	:_processus(processus.begin(),processus.end()), _source(source) { /* construit quand verrouillage OK */ }
	~VerrouProcessus()
	{
		auto s = SVM_Systeme::Livraison::objet<SVM_Processus::Processus>(_source);
		for(auto& p: _processus)
		{
			auto processus = p.lock();
			if(not processus) continue;
			if(not processus->debloquage_demande(_source)) continue;
			if(static_cast<bool>(s))
			{
				s->attente_deverrouillage(processus->_adresse);
			}
		}
	}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const VerrouProcessus& v)
	{
		f << "<process lock";
		for(auto& p: v._processus)
		{
			auto pp = p.lock();
			if(static_cast<bool>(pp))
			{
				f << " " ;
				pp->nom_flux(f);
			}
		}
		if(static_cast<bool>(v._source))
		{
			f << " by " << (*v._source);
		}
		f << ">";
		return f;
	}
	operator bool () const
	{
		return static_cast<bool>(_source);
	}
	private:
		std::vector<SVM_Processus::ProcessusWP> _processus;
		SVM_Synchronisation::AdresseSP _source;
};

DECL_SHARED_PTR(VariableChaine);
struct VariableChaine
{
	VariableChaine(const SHARED_PTR(SVM_String)& chaine)
	:_chaine(chaine) {}
	~VariableChaine()
	{
		if(not _chaine) return;
		if(_chaine->string==nullptr) return;
		delete [] _chaine->string;
		_chaine->string = nullptr;
		_chaine->size = 0;
	}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const VariableChaine& v)
	{
		f << "SVM_String";
		if(static_cast<bool>(v._chaine))
		{
			f << " " << v._chaine->size;
			if(v._chaine->string)
			{
				f << "[" << v._chaine->string << "]";
			}
		}
		return f;
	}
	private:
		SHARED_PTR(SVM_String) _chaine;
};

DECL_SHARED_PTR(StructureAdresse);
struct StructureAdresse : public SVM_Synchronisation::Adresse
{
	template<typename Evenement>
		friend struct SVM_Synchronisation::Livraison;
	StructureAdresse() {}
	explicit StructureAdresse(const SVM_Interface::StructureSP& structure)
	:_structure(structure) {}
	virtual void format(std::ostream& f) const override
	{
		f << "STRUCT <" << _structure.get() << ">";
	}
	virtual void format(SVM::Global::Print& f) const override
	{
		f << "STRUCT <" << _structure.get() << ">";
	}
	virtual std::string uml() const override
	{
		std::ostringstream oss;
		oss << "STRUCT_" << this;
		return oss.str();
	}
	SVM_Interface::StructureSP _structure;
};

DECL_SHARED_PTR(QueueEvenements);
struct QueueEvenements : public SVM_Synchronisation::Livraison<SVM_Interface::Structure>
{
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const QueueEvenements& q)
	{
		f << "Event Queue";
		return f;
	}
};

struct Outils
{
	static SVM_Boolean booleen(const bool b)
	{
		return b?(SVM_Boolean::TRUE):(SVM_Boolean::FALSE);
	}

	static SVM_Interface::EnvironnementSP environnement(const void *svm, const std::string& fonction)
	{
		if(not svm)
		{
			throw MauvaisAppelFonction(fonction,"svm is void");
		}
		const SVM_Interface::EnvironnementSP e = *reinterpret_cast<const SVM_Interface::EnvironnementSP*>(svm);
		return e;
	}
	
	static SVM_Memoire::ValeurInterfaceSP valeur_interface(const SVM_Interface::EnvironnementSP& environnement, const void *valeur, const std::string& fonction, const std::string& parametre)
	{
		if(not valeur) { throw MauvaisAppelFonction(fonction,parametre+" is void"); }
		auto vi = environnement->valeur<SVM_Memoire::ValeurInterface>(valeur);
		if(not vi) { throw MauvaisAppelFonction(fonction,parametre+" is not a value"); }
		return vi;
	}

	static SVM_Memoire::ValeurSP valeur_valeur(const SVM_Interface::EnvironnementSP& environnement, const void *valeur, const std::string& fonction, const std::string& parametre)
	{
		return valeur_interface(environnement,valeur,fonction,parametre)->_valeur;
	}

	template<typename T>
	static SHARED_PTR(T) valeur_typee(const SVM_Interface::EnvironnementSP& environnement, const void *valeur, const std::string& fonction, const std::string& parametre)
	{
		if(not valeur) { throw MauvaisAppelFonction(fonction,parametre+" is void"); }
		SHARED_PTR(T) valeur_valeur = environnement->valeur<T>(valeur);
		if(not valeur_valeur) { throw MauvaisAppelFonction(fonction,parametre+" is invalid"); }
		return valeur_valeur;
	}

	template<typename T>
	static SHARED_PTR(T) valeur(const SVM_Interface::EnvironnementSP& environnement, const SVM_Memoire::ValeurInterfaceSP& valeur, const std::string& fonction, const std::string& parametre, const std::string& erreur_type)
	{
		auto valeur_type = std::dynamic_pointer_cast<T>(valeur->_valeur);
		if(not valeur_type) { throw MauvaisAppelFonction(fonction,parametre+" is not "+erreur_type); }
		return valeur_type;
	}

	template<typename T>
	static SHARED_PTR(T) valeur(const SVM_Interface::EnvironnementSP& environnement, const void *valeur, const std::string& fonction, const std::string& parametre, const std::string& erreur_type)
	{
		auto valeur_valeur = Outils::valeur_typee<SVM_Memoire::ValeurInterface>(environnement,valeur,fonction,parametre);
		return Outils::valeur<T>(environnement,valeur_valeur,fonction,parametre,erreur_type);
	}

	static void valeur_nulle(const SVM_Interface::EnvironnementSP& environnement, const SVM_Memoire::ValeurSP& valeur, const SVM_Memoire::Type& type, const std::string& fonction, const std::string& parametre, const std::string& erreur_type)
	{
		if(not (type==*valeur)) { throw MauvaisAppelFonction(fonction,parametre+" is not "+erreur_type); }
	}

	static SVM_Variable ajout(const SVM_Interface::EnvironnementSP& environnement, const SVM_Memoire::ValeurSP& valeur)
	{
		return environnement->ajout(std::make_shared<SVM_Memoire::ValeurInterface>(valeur));
	}

	template<typename T, typename V>
	static void change_valeur(SVM_Interface::EnvironnementSP& environnement, SVM_Value value, const V& v, const SVM_Memoire::Type& type, const std::string& fonction, const std::string& parametre, const std::string& erreur_type)
	{
		auto valeur_interface = Outils::valeur_interface(environnement,value,fonction,parametre);
		if(valeur_interface->_valeur->nulle())
		{
			Outils::valeur_nulle(environnement,valeur_interface->_valeur,type,fonction,parametre,erreur_type);
			valeur_interface->_valeur = std::make_shared<T>(v);
			return;
		}
		auto vv = Outils::valeur<T>(environnement,valeur_interface,fonction,parametre,erreur_type);
		if(vv->est_constante()) { throw MauvaisAppelFonction(fonction,parametre+" is a constant"); }
		static_cast<V&>(*vv) = v;
		if(not environnement->_machine->_base->_debugueur) return;
		SVM_Noyau::NoyauSP noyau_courant = environnement->valeur<SVM_Noyau::Noyau>(environnement->_variable_noyau);
		if(static_cast<bool>(noyau_courant))
		{
			vv->notification_debugueur_changement_memoire(noyau_courant->_memoire->_piege);
		}
		else
		{
			vv->notification_debugueur_changement_memoire(SVM_Memoire::PiegeMemoireSP());
		}
	}

	static SVM_Processus::ProcessusSP processus_libre(const SVM_Interface::EnvironnementSP& environnement, const void *processus, const std::string& fonction, const std::string& parametre = "process")
	{
		if(not processus) { throw MauvaisAppelFonction(fonction,parametre+" is void"); }
		SVM_Interface::VariableProcessusSP vp = environnement->valeur<VariableProcessus>(processus);
		if(not vp) { throw MauvaisAppelFonction(fonction,parametre+" is invalid"); }
		SVM_Processus::ProcessusSP p = vp->_processus;
		if(not p) { throw MauvaisAppelFonction(fonction,parametre+" is invalid"); }
		return p;
	}
	static SVM_Processus::ProcessusSP processus(const SVM_Interface::EnvironnementSP& environnement, const void *processus, const std::string& fonction, const std::string& parametre = "process")
	{
		auto p = Outils::processus_libre(environnement,processus,fonction,parametre);
		return Outils::processus_verrouille(environnement,p,fonction,parametre);
	}
	static SVM_Boolean verifie_verrou(const SVM_Interface::EnvironnementSP& environnement, const SVM_Processus::ProcessusSP& processus, const std::string& fonction, const std::string& parametre = "process")
	{
		auto vp = environnement->valeur<VariableProcessus>(environnement->_variable_processus);
		if(static_cast<bool>(vp))
		{
			SVM_Processus::ProcessusSP processus_courant = vp->_processus;
			if(static_cast<bool>(processus_courant))
			{
				return ((processus==processus_courant) or (processus->verrouille_par(processus_courant->_adresse)))?TRUE:FALSE;
			}
		}
		SVM_Ordonnanceur::OrdonnanceurSP ordonnanceur_courant = environnement->valeur<SVM_Ordonnanceur::Ordonnanceur>(environnement->_variable_ordonnanceur);
		if(static_cast<bool>(ordonnanceur_courant))
		{
			return (processus->verrouille_par(ordonnanceur_courant->_adresse))?TRUE:FALSE;
		}
		return FALSE;
	}
	static void erreur_verrou(const SVM_Processus::ProcessusSP& processus, const std::string& fonction, const std::string& parametre)
	{
		if(not processus->_verrouille_par)
		{
			throw MauvaisAppelFonction(fonction,parametre+" is not locked");
		}
		std::ostringstream oss;
		oss << parametre << " is locked by ";
		processus->_verrouille_par->format(oss);
		throw MauvaisAppelFonction(fonction,oss.str());
	}
	static SVM_Processus::ProcessusSP processus_verrouille(const SVM_Interface::EnvironnementSP& environnement, const SVM_Processus::ProcessusSP& processus, const std::string& fonction, const std::string& parametre = "process")
	{
		if(not processus)
			return processus;
		auto vp = environnement->valeur<VariableProcessus>(environnement->_variable_processus);
		if(static_cast<bool>(vp))
		{
			SVM_Processus::ProcessusSP processus_courant = vp->_processus;
			if(static_cast<bool>(processus_courant))
			{
				if(processus==processus_courant)
					return processus;
				if(processus->verrouille_par(processus_courant->_adresse))
					return processus;
				Outils::erreur_verrou(processus,fonction,parametre);
			}
		}
		SVM_Ordonnanceur::OrdonnanceurSP ordonnanceur_courant = environnement->valeur<SVM_Ordonnanceur::Ordonnanceur>(environnement->_variable_ordonnanceur);
		if(static_cast<bool>(ordonnanceur_courant))
		{
			if(processus->verrouille_par(ordonnanceur_courant->_adresse))
				return processus;
			Outils::erreur_verrou(processus,fonction,parametre);
		}
		if(processus->verrouille_par(environnement->_machine->_systeme->_adresse))
			return processus;
		Outils::erreur_verrou(processus,fonction,parametre);
		throw MauvaisAppelFonction(fonction,parametre+" lock status can not be verified");
	}
	static SVM_Noyau::NoyauSP noyau_libre(const SVM_Interface::EnvironnementSP& environnement, const void *noyau, const std::string& fonction, const std::string& parametre="kernel")
	{
		if(not noyau) { throw MauvaisAppelFonction(fonction,parametre+" is void"); }
		SVM_Noyau::NoyauSP n = environnement->valeur<SVM_Noyau::Noyau>(noyau);
		if(not n) { throw MauvaisAppelFonction(fonction,parametre+" is invalid"); }
		return n;
	}
	static SVM_Noyau::NoyauSP noyau(const SVM_Interface::EnvironnementSP& environnement, const void *noyau, const std::string& fonction, const std::string& parametre="kernel")
	{
		if(not noyau) { throw MauvaisAppelFonction(fonction,parametre+" is void"); }
		SVM_Noyau::NoyauSP n = environnement->valeur<SVM_Noyau::Noyau>(noyau);
		if(not n) { throw MauvaisAppelFonction(fonction,parametre+" is invalid"); }
		Outils::processus_verrouille(environnement,n->_processus.lock(),fonction,"process containing "+parametre);
		return n;
	}
	static SVM_Ordonnanceur::OrdonnanceurSP ordonnanceur(const SVM_Interface::EnvironnementSP& environnement, const void *ordonnanceur, const std::string& fonction, const std::string& parametre="scheduler")
	{
		auto o = environnement->valeur<SVM_Ordonnanceur::Ordonnanceur>(ordonnanceur);
		if(not o) { throw MauvaisAppelFonction(fonction,parametre+" is not a scheduler"); }
		return o;
	}
	static SVM_Ordonnanceur::OrdonnanceurSP ordonnanceur_courant(const SVM_Interface::EnvironnementSP& environnement, const std::string& fonction)
	{
		auto o = environnement->valeur<SVM_Ordonnanceur::Ordonnanceur>(environnement->_variable_ordonnanceur);
		if(not o) { throw MauvaisAppelFonction(fonction,"process run attempt outside a scheduler"); }
		return o;
	}
	static SVM_Ordonnanceur::OrdonnanceurSP ordonnanceur(const SVM_Processus::ProcessusSP& p)
	{
		return p->_ordonnanceur.lock();
	}
	static SVM_Synchronisation::AdresseSP adresse(const SVM_Ordonnanceur::OrdonnanceurSP& o)
	{
		return o->adresse();
	}
	static SVM_Synchronisation::AdresseSP adresse(const SVM_Interface::EnvironnementSP& environnement, const std::string& fonction)
	{
		auto vp = environnement->valeur<VariableProcessus>(environnement->_variable_processus);
		if(static_cast<bool>(vp))
		{
			SVM_Processus::ProcessusSP p = vp->_processus;
			if(static_cast<bool>(p))
			{
				return p->_adresse;
			}
		}
		auto o = environnement->valeur<SVM_Ordonnanceur::Ordonnanceur>(environnement->_variable_ordonnanceur);
		if(static_cast<bool>(o))
		{
			return o->adresse();
		}
		return environnement->_machine->_systeme->_adresse;
		throw MauvaisAppelFonction(fonction,"outside a process or scheduler context");
	}
	static std::string chaine(SVM_Interface::EnvironnementSP& environnement, const SVM_String chaine, const std::string& fonction, const std::string& parametre="string")
	{
		if(not chaine.string) { throw MauvaisAppelFonction(fonction,parametre+" is void"); }
		return std::string(chaine.string,chaine.size);
	}
	static SVM_Valeur::Interruption::InterruptionInterne interruption_interne(const SVM_Interface::EnvironnementSP& environnement, const SVM_Interruption_Internal interruption, const std::string& fonction, const std::string& parametre)
	{
		switch(interruption)
		{
			case CLONE: return SVM_Valeur::Interruption::InterruptionInterne::CLONE; break;
			case DEVICE: return SVM_Valeur::Interruption::InterruptionInterne::PERIPHERIQUE; break;
			case FAILURE: return SVM_Valeur::Interruption::InterruptionInterne::ECHEC; break;
			case FIRST: return SVM_Valeur::Interruption::InterruptionInterne::PREMIER; break;
			case MEMORY: return SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE; break;
			case NUMERIC: return SVM_Valeur::Interruption::InterruptionInterne::NUMERIQUE; break;
			case PROCESSOR: return SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR; break;
			case SECOND: return SVM_Valeur::Interruption::InterruptionInterne::SECOND; break;
			case SECURITY: return SVM_Valeur::Interruption::InterruptionInterne::SECURITE; break;
			case TERMINATE: return SVM_Valeur::Interruption::InterruptionInterne::TERMINAISON; break;
			case TERMINAL: return SVM_Valeur::Interruption::InterruptionInterne::TERMINAL; break;
			case GENERIC: return SVM_Valeur::Interruption::InterruptionInterne::GENERIQUE; break;
			default:
				break;
		}
		throw MauvaisAppelFonction(fonction,parametre+" is invalid");
	}
	static SVM_Interruption_Internal interruption_interne(const SVM_Interface::EnvironnementSP& environnement, const SVM_Valeur::Interruption& interruption, const std::string& fonction, const std::string& parametre)
	{
		switch(interruption._interruption_interne)
		{
			case SVM_Valeur::Interruption::InterruptionInterne::CLONE: return CLONE; break;
			case SVM_Valeur::Interruption::InterruptionInterne::PERIPHERIQUE: return DEVICE; break;
			case SVM_Valeur::Interruption::InterruptionInterne::ECHEC: return FAILURE; break;
			case SVM_Valeur::Interruption::InterruptionInterne::PREMIER: return FIRST; break;
			case SVM_Valeur::Interruption::InterruptionInterne::MEMOIRE: return MEMORY; break;
			case SVM_Valeur::Interruption::InterruptionInterne::NUMERIQUE: return NUMERIC; break;
			case SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR: return PROCESSOR; break;
			case SVM_Valeur::Interruption::InterruptionInterne::SECOND: return SECOND; break;
			case SVM_Valeur::Interruption::InterruptionInterne::SECURITE: return SECURITY; break;
			case SVM_Valeur::Interruption::InterruptionInterne::TERMINAISON: return TERMINATE; break;
			case SVM_Valeur::Interruption::InterruptionInterne::TERMINAL: return TERMINAL; break;
			case SVM_Valeur::Interruption::InterruptionInterne::GENERIQUE: return GENERIC; break;
			default:
				break;
		}
		throw MauvaisAppelFonction(fonction,parametre+" is invalid");
	}
	static SVM_Type_Internal type_interne(const SVM_Interface::EnvironnementSP& environnement, const SVM_Memoire::Type& type, const std::string& fonction, const std::string& parametre)
	{
		switch(type._type_interne)
		{
			case SVM_Memoire::Type::TypeInterne::AUTOMATIQUE: return AUTOMATIC; break;
			case SVM_Memoire::Type::TypeInterne::ENTIER: return INTEGER; break;
			case SVM_Memoire::Type::TypeInterne::CHAINE: return STRING; break;
			case SVM_Memoire::Type::TypeInterne::BOOLEEN: return BOOLEAN; break;
			case SVM_Memoire::Type::TypeInterne::POINTEUR: return POINTER; break;
			case SVM_Memoire::Type::TypeInterne::SYMBOLE: return SYMBOL; break;
			case SVM_Memoire::Type::TypeInterne::BIBLIOTHEQUE: return LIBRARY; break;
			case SVM_Memoire::Type::TypeInterne::INTERRUPTION: return INTERRUPTION; break;
			case SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION: return PLUGIN_ENTRY_POINT; break;
			default:
			     throw MauvaisAppelFonction(fonction,parametre+" has an invalid type");
			     break;
		}
	}
	static SVM_Memoire::Type::TypeInterne type_interne(const SVM_Interface::EnvironnementSP& environnement, const SVM_Type_Internal& type, const std::string& fonction, const std::string& parametre)
	{
		switch(type)
		{
			case AUTOMATIC: return SVM_Memoire::Type::TypeInterne::AUTOMATIQUE; break;
			case INTEGER: return SVM_Memoire::Type::TypeInterne::ENTIER; break;
			case STRING: return SVM_Memoire::Type::TypeInterne::CHAINE; break;
			case BOOLEAN: return SVM_Memoire::Type::TypeInterne::BOOLEEN; break;
			case POINTER: return SVM_Memoire::Type::TypeInterne::POINTEUR; break;
			case SYMBOL: return SVM_Memoire::Type::TypeInterne::SYMBOLE; break;
			case LIBRARY: return SVM_Memoire::Type::TypeInterne::BIBLIOTHEQUE; break;
			case INTERRUPTION: return SVM_Memoire::Type::TypeInterne::INTERRUPTION; break;
			case PLUGIN_ENTRY_POINT: return SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION; break;
			default:
			     throw MauvaisAppelFonction(fonction,parametre+" is invalid");
			     break;
		}
	}
	static SVM_Valeur::PointEntreeExtension type_externe(const SVM_Memoire::Type& type)
	{
		return type._point_entree_extension;
	}
	static size_t adresse(const SVM_Valeur::AdresseMemoire& adresse)
	{
		return adresse._adresse;
	}
	static SVM_Valeur::CodeSP code(const SVM_Valeur::AdresseInstruction& adresse)
	{
		return adresse._code;
	}
	static size_t locale(const SVM_Valeur::AdresseInstruction& adresse)
	{
		return adresse._locale;
	}
	static SVM_String chaine(SVM_Interface::EnvironnementSP& environnement, const std::string& chaine)
	{
		SHARED_PTR(SVM_String) s = std::make_shared<SVM_String>();
		s->size = chaine.size();
		s->string = new char[s->size+1];
		::memcpy(const_cast<char*>(s->string),chaine.c_str(),s->size+1);
		environnement->ajout(std::make_shared<VariableChaine>(s));
		return *s;
	}
	static const std::string& details(const SVM_Valeur::Interruption& i)
	{
		return i._details;
	}
	static bool nature(const SVM_Valeur::Interruption& i)
	{
		return i._materielle;
	}
	static const SVM_Valeur::PointEntreeExtension& pep(const SVM_Valeur::Interruption& i)
	{
		return i._point_entree_extension;
	}
	static const SVM_Valeur::PointEntreeExtension& type(const SVM_Memoire::Extension& e)
	{
		return e._type;
	}
	static void* valeur(const SVM_Memoire::Extension& e)
	{
		return e._valeur;
	}
	template<typename M, typename V>
	static void ecrase_valeur(SHARED_PTR(M)& m, const V& v)
	{
		m->_valeur = v;
	}
	static void ecrase_valeur(SVM_Memoire::ExtensionSP& valeur, const SVM_Valeur::PointEntreeExtension& type, void *interne)
	{
		auto machine = valeur->_machine.lock();
		auto patron = machine->_base->_gestionnaire_extensions->type(type);
		if(not patron)
		{
			throw SVM_Memoire::TypeInexistant(type);
		}
		valeur->_patron = patron;
		valeur->_type = type;
		valeur->_valeur = interne;
	}

	template<typename T>
	static SVM_Value_String* tableau_chaines(SVM_Interface::EnvironnementSP& environnement, const T& t)
	{
		SVM_Interface::TableauVariablesSP tableau = std::make_shared<SVM_Interface::TableauVariables>("strings",t.size()+1);
		auto it = tableau->_variables;
		for(const auto& a : t)
		{
			*it = Outils::ajout(environnement,std::make_shared<SVM_Memoire::Chaine>(a));
			++it;
		}
		*it=nullptr;
		environnement->ajout_avatar(tableau);
		return tableau->_variables;
	}
	
	static bool pause_processus(SVM_Interface::EnvironnementSP& environnement, const SVM_Boolean doux, SVM_Processus::ProcessusSP& p, const std::string& fonction)
	{
		if(not environnement->_variable_noyau)
			return false;
		auto noyau = Outils::noyau(environnement,environnement->_variable_noyau,fonction,"current kernel");
		p = noyau->_processus.lock();
		bool attente = false;
		if((doux==TRUE) or noyau->mode_protege())
		{
			if(not environnement->_attente) { throw MauvaisAppelFonction(fonction,"called outside a waiting instruction"); }
			if(not p) { throw MauvaisAppelFonction(fonction,"unable to access to process"); }

			attente = p->etat_global()!=SVM_Processus::Etat::Execution::ATTENTE;
			if(attente and not p->entree_mode_attente()) { throw MauvaisAppelFonction(fonction,"unable to change process state"); }
		}
		return attente;
	}

	static void reprise_processus(SVM_Interface::EnvironnementSP& environnement, const bool attente, const SVM_Processus::ProcessusSP& p, const std::string& fonction)
	{
		if(attente)
		{
			if(not p->sortie_mode_attente()) { throw MauvaisAppelFonction(fonction,"unable to change process state"); }
		}
	}

	static SVM_Valeur::PositionInstruction position(const SVM_Valeur::InstructionSP& instruction)
	{
		return instruction->_position;
	}
};

DECL_SHARED_PTR(ValeurFormulaire);
struct ValeurFormulaire
{
	virtual ~ValeurFormulaire() {}	
	virtual SVM_Value convertit(const SVM_Interface::EnvironnementSP& environnement) const = 0;
};

template<typename T>
struct TypeMemoireResultat
{
};

template<>
struct TypeMemoireResultat<bool>
{
	typedef SVM_Memoire::Booleen V;
};

template<>
struct TypeMemoireResultat<long long int>
{
	typedef SVM_Memoire::Entier V;
};

template<>
struct TypeMemoireResultat<std::string>
{
	typedef SVM_Memoire::Chaine V;
};

template<typename T>
struct ValeurSpecifiqueFormulaire : public ValeurFormulaire
{
	explicit ValeurSpecifiqueFormulaire(const T& valeur)
	:_valeur(valeur) {}
	virtual ~ValeurSpecifiqueFormulaire() {}
	virtual SVM_Value convertit(const SVM_Interface::EnvironnementSP& environnement) const override
	{
		SVM_Memoire::ValeurSP valeur = std::make_shared<typename TypeMemoireResultat<T>::V>(_valeur);
		return Outils::ajout(environnement,valeur);
	}
	T _valeur;
};

DECL_SHARED_PTR(ChampFormulaire);
struct ChampFormulaire
{
	explicit ChampFormulaire(const std::string& etiquette)
	:_etiquette(etiquette) {}
	virtual ~ChampFormulaire() {}
	virtual SVM_Debugueur::JSON_Interne::ObjetSP json() const = 0;
	virtual ValeurFormulaireSP valeur(const SVM_Debugueur::JSON_Interne::ValeurSP& valeur) = 0;
	std::string _etiquette;
};

struct CocheFormulaire : public ChampFormulaire
{
	CocheFormulaire(const std::string& etiquette, const bool defaut)
	:ChampFormulaire(etiquette), _defaut(defaut) {}
	virtual ~CocheFormulaire() {}
	virtual SVM_Debugueur::JSON_Interne::ObjetSP json() const override
	{
		return SVM_Debugueur::JSON::objet({ { "type" , SVM_Debugueur::JSON::chaine("checkbox") } , { "label" , SVM_Debugueur::JSON::chaine(_etiquette) } , { "value" , SVM_Debugueur::JSON::booleen(_defaut) } });
	}
	virtual ValeurFormulaireSP valeur(const SVM_Debugueur::JSON_Interne::ValeurSP& valeur) override
	{
		auto b = SVM_Debugueur::JSON::booleen(valeur);
		if(not SVM_Debugueur::JSON::est(b))
			return ValeurFormulaireSP();
		return std::make_shared<ValeurSpecifiqueFormulaire<bool> >(*b);
	}
	bool _defaut;
};

struct SelectionFormulaire : public ChampFormulaire
{
	SelectionFormulaire(const std::string& etiquette, const std::list<std::string>& valeurs)
	:ChampFormulaire(etiquette), _valeurs(valeurs) {}
	virtual ~SelectionFormulaire() {}
	virtual SVM_Debugueur::JSON_Interne::ObjetSP json() const override
	{
		auto o = SVM_Debugueur::JSON::objet({ { "type" , SVM_Debugueur::JSON::chaine("selection") } , { "label" , SVM_Debugueur::JSON::chaine(_etiquette) } });
		auto t = SVM_Debugueur::JSON::tableau();
		for(const auto& v: _valeurs)
		{
			t->ajoute(SVM_Debugueur::JSON::chaine(v));
		}
		o->ajoute("values",t);
		return o;
	}
	virtual ValeurFormulaireSP valeur(const SVM_Debugueur::JSON_Interne::ValeurSP& valeur) override
	{
		auto b = SVM_Debugueur::JSON::chaine(valeur);
		if(not SVM_Debugueur::JSON::est(b))
			return ValeurFormulaireSP();
		return std::make_shared<ValeurSpecifiqueFormulaire<std::string> >(*b);
	}
	std::list<std::string> _valeurs;
};

struct EntierFormulaire : public ChampFormulaire
{
	EntierFormulaire(const std::string& etiquette, const long long int defaut, const long long int min, const long long int max)
	:ChampFormulaire(etiquette), _defaut(defaut), _min(min), _max(max) {}
	virtual ~EntierFormulaire() {}
	virtual SVM_Debugueur::JSON_Interne::ObjetSP json() const override
	{
		auto o = SVM_Debugueur::JSON::objet({ { "type" , SVM_Debugueur::JSON::chaine("integer") } , { "label" , SVM_Debugueur::JSON::chaine(_etiquette) } , { "value" , SVM_Debugueur::JSON::entier(_defaut) } });
		if(_max>=_min)
		{
			o->ajoute("min", SVM_Debugueur::JSON::entier(_min));
			o->ajoute("max", SVM_Debugueur::JSON::entier(_max));
		}
		return o;
	}
	virtual ValeurFormulaireSP valeur(const SVM_Debugueur::JSON_Interne::ValeurSP& valeur) override
	{
		auto b = SVM_Debugueur::JSON::entier(valeur);
		if(not SVM_Debugueur::JSON::est(b))
			return ValeurFormulaireSP();
		return std::make_shared<ValeurSpecifiqueFormulaire<long long int> >(*b);
	}
	long long int _defaut;
	long long int _min;
	long long int _max;
};

struct ChaineFormulaire : public ChampFormulaire
{
	ChaineFormulaire(const std::string& etiquette, const std::string& defaut, const SVM_Size min, const SVM_Size max)
	:ChampFormulaire(etiquette), _defaut(defaut), _min(min), _max(max) {}
	virtual ~ChaineFormulaire() {}
	virtual SVM_Debugueur::JSON_Interne::ObjetSP json() const override
	{
		auto o = SVM_Debugueur::JSON::objet({ { "type" , SVM_Debugueur::JSON::chaine("string") } , { "label" , SVM_Debugueur::JSON::chaine(_etiquette) } , { "value" , SVM_Debugueur::JSON::chaine(_defaut) } });
		if(_max>=_min)
		{
			o->ajoute("min", SVM_Debugueur::JSON::entier(_min));
			o->ajoute("max", SVM_Debugueur::JSON::entier(_max));
		}
		return o;
	}
	virtual ValeurFormulaireSP valeur(const SVM_Debugueur::JSON_Interne::ValeurSP& valeur) override
	{
		auto b = SVM_Debugueur::JSON::chaine(valeur);
		if(not SVM_Debugueur::JSON::est(b))
			return ValeurFormulaireSP();
		return std::make_shared<ValeurSpecifiqueFormulaire<std::string> >(*b);
	}
	std::string _defaut;
	SVM_Size _min;
	SVM_Size _max;
};

struct TexteFormulaire : public ChampFormulaire
{
	TexteFormulaire(const std::string& etiquette, const SVM_Size largeur, const SVM_Size hauteur)
	:ChampFormulaire(etiquette), _largeur(largeur), _hauteur(hauteur) {}
	virtual ~TexteFormulaire() {}
	virtual SVM_Debugueur::JSON_Interne::ObjetSP json() const override
	{
		return SVM_Debugueur::JSON::objet({ { "type" , SVM_Debugueur::JSON::chaine("text") } , { "label" , SVM_Debugueur::JSON::chaine(_etiquette) } , { "width" , SVM_Debugueur::JSON::entier(_largeur) } , { "height" , SVM_Debugueur::JSON::entier(_hauteur) }});
	}
	virtual ValeurFormulaireSP valeur(const SVM_Debugueur::JSON_Interne::ValeurSP& valeur) override
	{
		auto b = SVM_Debugueur::JSON::chaine(valeur);
		if(not SVM_Debugueur::JSON::est(b))
			return ValeurFormulaireSP();
		return std::make_shared<ValeurSpecifiqueFormulaire<std::string> >(*b);
	}
	SVM_Size _largeur;
	SVM_Size _hauteur;
};

DECL_SHARED_PTR(Formulaire);
struct Formulaire
{
	explicit Formulaire(const std::string& titre)
	:_titre(titre) {}
	SVM_Debugueur::JSON_Interne::ObjetSP json() const
	{
		auto json = SVM_Debugueur::JSON::objet({ { "title" , SVM_Debugueur::JSON::chaine(_titre) } });
		auto t = SVM_Debugueur::JSON::tableau();
		for(const auto& c: _champs)
		{
			t->ajoute(c->json());
		}
		json->ajoute("fields",t);
		return json;
	}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const Formulaire& t)
	{
		f << "<debug form>";
		return f;
	}
	std::string _titre;
	std::vector<ChampFormulaireSP> _champs;
};

}
}
}
using namespace SVM::Machine::Interface;
extern "C"
{

const char* svm_api_get_signature()
{
	return SVM_API_SIGNATURE;
}
void svm_plugin_configure(void *plugin_handler, const char *plugin_configuration, const char api_signature[], const char svm_version[])
{
	SVM_Extension::ExtensionConfiguration *ec = reinterpret_cast<SVM_Extension::ExtensionConfiguration*>(plugin_handler);
	ec->_configuration = std::string(plugin_configuration);
	ec->_signature_api = std::string(api_signature);
	ec->_version_svm = std::string(svm_version);
}
 
SVM_String svm_string_new(const void *svm, const char *buffer, const unsigned long int size)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not buffer) { throw MauvaisAppelFonction(__func__,"buffer is void"); }
	std::string s(buffer,size);
	SHARED_PTR(SVM_String) sv = std::make_shared<SVM_String>();
	sv->size = s.size();
	sv->string = new char[sv->size];
	::memcpy(const_cast<char*>(sv->string),buffer,sv->size);
	environnement->ajout(std::make_shared<VariableChaine>(sv));
	return *sv;
}

SVM_String svm_string_new__raw(const void *svm, const char *string)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not string) { throw MauvaisAppelFonction(__func__,"string is void"); }
	std::string s(string);
	return Outils::chaine(environnement,s);
}

SVM_String svm_string_new__buffer(const void *svm, const unsigned long int size)
{
	auto environnement = Outils::environnement(svm,__func__);
	SHARED_PTR(SVM_String) sv = std::make_shared<SVM_String>();
	sv->size = size;
	sv->string = new char[sv->size];
	environnement->ajout(std::make_shared<VariableChaine>(sv));
	return *sv;
}

SVM_String svm_string_join(const void *svm, const SVM_String first, const SVM_String second)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not first.string) { throw MauvaisAppelFonction(__func__,"first is invalid"); }
	if(not second.string) { throw MauvaisAppelFonction(__func__,"second is invalid"); }
	SHARED_PTR(SVM_String) sv = std::make_shared<SVM_String>();
	sv->size = first.size+second.size+1;
	sv->string = new char[sv->size];
	::memcpy(const_cast<char*>(sv->string),first.string,first.size);
	::memcpy(const_cast<char*>(sv->string)+first.size,second.string,second.size);
	const_cast<char&>(sv->string[first.size+second.size]) = '\0';
	environnement->ajout(std::make_shared<VariableChaine>(sv));
	return *sv;
}

SVM_String svm_string_join__raw(const void *svm, const SVM_String first, const char *second)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not first.string) { throw MauvaisAppelFonction(__func__,"first is invalid"); }
	if(not second) { throw MauvaisAppelFonction(__func__,"second is invalid"); }
	SHARED_PTR(SVM_String) sv = std::make_shared<SVM_String>();
	auto size = ::strlen(second);
	sv->size = first.size+size+1;
	sv->string = new char[sv->size];
	::memcpy(const_cast<char*>(sv->string),first.string,first.size);
	::memcpy(const_cast<char*>(sv->string)+first.size,second,size);
	const_cast<char&>(sv->string[first.size+size]) = '\0';
	environnement->ajout(std::make_shared<VariableChaine>(sv));
	return *sv;
}

SVM_String svm_string_join__buffer(const void *svm, const SVM_String first, const char *second, const unsigned long int size)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not first.string) { throw MauvaisAppelFonction(__func__,"first is invalid"); }
	if(not second) { throw MauvaisAppelFonction(__func__,"second is invalid"); }
	SHARED_PTR(SVM_String) sv = std::make_shared<SVM_String>();
	sv->size = first.size+size+1;
	sv->string = new char[sv->size];
	::memcpy(const_cast<char*>(sv->string),first.string,first.size);
	::memcpy(const_cast<char*>(sv->string)+first.size,second,size);
	const_cast<char&>(sv->string[first.size+size]) = '\0';
	environnement->ajout(std::make_shared<VariableChaine>(sv));
	return *sv;
}

#if 0
/**
 * This function concatenates a SVM_String to another one.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] source The SVM_String to be extended by the extra one.
 * \param[in] extra The SVM_String to append to the source one.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_string_append(const void *svm, SVM_String *source, const SVM_String extra);

/**
 * This function concatenates a C string to a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] source The SVM_String to be extended by the extra one.
 * \param[in] extra The C string to append to the source one.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_string_append__raw(const void *svm, SVM_String *source, const char *extra);

/**
 * This function concatenates a C buffer to a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] source The SVM_String to be extended by the extra one.
 * \param[in] extra The buffer to append to the source one.
 * \param[in] size The buffer size.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_string_append__buffer(const void *svm, SVM_String *source, const char *extra, const unsigned long int size);

/**
 * This function removes the buffer of the SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] string The string to wipe.
 *
 * The size of the string is set to 0.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_string_clear(const void *svm, SVM_String *string);

void svm_string_append(const void *svm,  SVM_String *source, const SVM_String extra)
{
	if(not source) { throw MauvaisAppelFonction(__func__,"source is void"); }
	if(not source->string) { throw MauvaisAppelFonction(__func__,"source is invalid"); }
	if(not extra.string) { throw MauvaisAppelFonction(__func__,"extra is invalid"); }
	char *s = new char[source->size+extra.size+1];
	::memcpy(s,source->string,source->size);
	::memcpy(s+source->size,extra.string,extra.size);
	s[source->size+extra.size] = '\0';
	delete [] source->string;
	source->string = s;
	source->size += extra.size;
}

void svm_string_append__raw(const void *svm, SVM_String *source, const char *extra)
{
	if(not source) { throw MauvaisAppelFonction(__func__,"source is void"); }
	if(not source->string) { throw MauvaisAppelFonction(__func__,"source is invalid"); }
	if(not extra) { throw MauvaisAppelFonction(__func__,"extra is invalid"); }
	auto size = ::strlen(extra);
	char *s = new char[source->size+size+1];
	::memcpy(s,source->string,source->size);
	::memcpy(s+source->size,extra,size);
	s[source->size+size] = '\0';
	delete [] source->string;
	source->string = s;
	source->size += size;
}

void svm_string_append__buffer(const void *svm, SVM_String *source, const char *extra, const unsigned long int size)
{
	if(not source) { throw MauvaisAppelFonction(__func__,"source is void"); }
	if(not source->string) { throw MauvaisAppelFonction(__func__,"source is invalid"); }
	if(not extra) { throw MauvaisAppelFonction(__func__,"extra is invalid"); }
	char *s = new char[source->size+size+1];
	::memcpy(s,source->string,source->size);
	::memcpy(s+source->size,extra,size);
	s[source->size+size] = '\0';
	delete [] source->string;
	source->string = s;
	source->size += size;
}

void svm_string_clear(const void *svm,  SVM_String *string)
{
	if(not string) { throw MauvaisAppelFonction(__func__,"string is void"); }
	delete [] string->string;
	string->string = nullptr;
	string->size = 0;
}
#endif

SVM_Boolean svm_string_is_equal(const void *svm, const SVM_String left, const SVM_String right)
{
	if(not left.string)  { throw MauvaisAppelFonction(__func__,"left is invalid"); }
	if(not right.string)  { throw MauvaisAppelFonction(__func__,"right is invalid"); }
	if(left.size!=right.size) return Outils::booleen(false);
	return Outils::booleen(::strcmp(left.string,right.string)==0);
}

SVM_Boolean svm_string_is_equal__raw(const void *svm, const SVM_String left, const char *right)
{
	if(not left.string)  { throw MauvaisAppelFonction(__func__,"left is invalid"); }
	if(not right)  { throw MauvaisAppelFonction(__func__,"right is void"); }
	return Outils::booleen(::strcmp(left.string,right)==0);
}

SVM_Boolean svm_string_is_equal__buffer(const void *svm, const SVM_String left, const char *buffer, const unsigned long int size)
{
	if(not left.string)  { throw MauvaisAppelFonction(__func__,"left is invalid"); }
	if(not buffer)  { throw MauvaisAppelFonction(__func__,"right is void"); }
	if(left.size!=size) return Outils::booleen(false);
	return Outils::booleen(::strcmp(left.string,buffer)==0);
}

SVM_Boolean svm_parameter_type_is_value(const void *svm, const SVM_Parameter parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,parameter,__func__,"parameter");
	const SHARED_PTR(SVM_Valeur::ParametreValeur)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreValeur>(p);
	return Outils::booleen(static_cast<bool>(v));
}

SVM_Value svm_parameter_value_get(const void *svm, const SVM_Parameter parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,parameter,__func__,"parameter");
	const SHARED_PTR(SVM_Valeur::ParametreValeur)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreValeur>(p);
	if(not v) { throw MauvaisAppelFonction(__func__,"parameter does not contain a value"); }
	SVM_Memoire::ValeurInterfaceSP vv = v->_valeur;
	return environnement->ajout(vv);
}

SVM_Parameter svm_parameter_value_new(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_typee<SVM_Memoire::ValeurInterface>(environnement,value,__func__,"value");
	SVM_Valeur::ParametreSP p = std::make_shared<SVM_Valeur::ParametreValeur>(valeur);
	return environnement->ajout(p);
}

SVM_Boolean svm_parameter_type_is_marker(const void *svm, const SVM_Parameter parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,parameter,__func__,"parameter");
	const SHARED_PTR(SVM_Valeur::ParametreMarqueur)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreMarqueur>(p);
	return Outils::booleen(static_cast<bool>(v));
}

SVM_String svm_parameter_marker_get(const void *svm, const SVM_Parameter parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,parameter,__func__,"parameter");
	const SHARED_PTR(SVM_Valeur::ParametreMarqueur)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreMarqueur>(p);
	if(not v) { throw MauvaisAppelFonction(__func__,"parameter does not contain a marker"); }
	SHARED_PTR(std::string) m = std::make_shared<std::string>(SVM_Valeur::ParametreMarqueur::texte(v->_marqueur));	
	return svm_string_new__raw(svm,m->c_str());
}

SVM_Parameter svm_parameter_marker_new(const void *svm, const SVM_String marker)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not marker.string) { throw MauvaisAppelFonction(__func__,"marker is void"); }
	std::string m(marker.string,marker.size);
	SVM_Valeur::ParametreMarqueur::Marqueur marqueur = SVM_Valeur::ParametreMarqueur::marqueur(m);
	SVM_Valeur::ParametreSP p = std::make_shared<SVM_Valeur::ParametreMarqueur>(marqueur);
	return environnement->ajout(p);
}

SVM_Parameter svm_parameter_marker_new__raw(const void *svm, const char *marker)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not marker) { throw MauvaisAppelFonction(__func__,"marker is void"); }
	std::string m(marker);
	SVM_Valeur::ParametreMarqueur::Marqueur marqueur = SVM_Valeur::ParametreMarqueur::marqueur(m);
	SVM_Valeur::ParametreSP p = std::make_shared<SVM_Valeur::ParametreMarqueur>(marqueur);
	return environnement->ajout(p);
}

SVM_Size svm_parameter_marker_find(const void *svm, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Index start, const SVM_String marker)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(start>=argc) { throw MauvaisAppelFonction(__func__,"start is outside parameter array"); }
	if(not marker.string) { throw MauvaisAppelFonction(__func__,"marker is void"); }
	std::string ms(marker.string,marker.size);
	auto m = SVM_Valeur::ParametreMarqueur::marqueur(ms);
	for(auto i=start ; i<argc ; ++i)
	{
		auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,argv[i],__func__,"argv");
		const SHARED_PTR(SVM_Valeur::ParametreMarqueur)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreMarqueur>(p);
		if(static_cast<bool>(v))
		{
			if(v->_marqueur==m) return i;
		}
	}
	return argc;
}

SVM_Index svm_parameter_marker_find__raw(const void *svm, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Index start, const char *marker)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(start>=argc) { throw MauvaisAppelFonction(__func__,"start is outside parameter array"); }
	if(not marker) { throw MauvaisAppelFonction(__func__,"marker is void"); }
	std::string ms(marker);
	auto m = SVM_Valeur::ParametreMarqueur::marqueur(ms);
	for(auto i=start ; i<argc ; ++i)
	{
		auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,argv[i],__func__,"argv");
		const SHARED_PTR(SVM_Valeur::ParametreMarqueur)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreMarqueur>(p);
		if(static_cast<bool>(v))
		{
			if(v->_marqueur==m) return i;
		}
	}
	return argc;
}

SVM_Index svm_parameter_marker_find_separator(const void *svm, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Index start)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(start>=argc) { throw MauvaisAppelFonction(__func__,"start is outside parameter array"); }
	for(auto i=start ; i<argc ; ++i)
	{
		auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,argv[i],__func__,"argv");
		const SHARED_PTR(SVM_Valeur::ParametreMarqueur)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreMarqueur>(p);
		if(static_cast<bool>(v))
		{
			if(SVM_Valeur::ParametreMarqueur::separateur(v->_marqueur)) return i;
		}
	}
	return argc;
}

SVM_Index svm_parameter_marker_find_closing(const void *svm, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Index opening)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(opening>=argc) { throw MauvaisAppelFonction(__func__,"start is outside parameter array"); }
	auto ps = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,argv[opening],__func__,"argv");
	const SHARED_PTR(SVM_Valeur::ParametreMarqueur)& vs = std::dynamic_pointer_cast<SVM_Valeur::ParametreMarqueur>(ps);
	if(not vs) { throw MauvaisAppelFonction(__func__,"opening index is not a marker"); }
	if(not SVM_Valeur::ParametreMarqueur::ouverture(vs->_marqueur)) { throw MauvaisAppelFonction(__func__,"opening index is not an opening marker"); }
	size_t niveau = 0;
	for(auto i=opening ; i<argc ; ++i)
	{
		auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,argv[i],__func__,"argv");
		const SHARED_PTR(SVM_Valeur::ParametreMarqueur)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreMarqueur>(p);
		if(static_cast<bool>(v))
		{
			if(v->_marqueur==vs->_marqueur)
			{
				++niveau;
				continue;
			}
			if(SVM_Valeur::ParametreMarqueur::fermeture(v->_marqueur,vs->_marqueur))
			{
				--niveau;
				if(niveau==0) return i;
			}
		}
	}
	return argc;
}


SVM_Boolean svm_parameter_type_is_keyword(const void *svm, const SVM_Parameter parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,parameter,__func__,"parameter");
	const SHARED_PTR(SVM_Valeur::ParametreMotClef)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreMotClef>(p);
	return Outils::booleen(static_cast<bool>(v));
}

SVM_String svm_parameter_keyword_get(const void *svm, const SVM_Parameter parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,parameter,__func__,"parameter");
	const SHARED_PTR(SVM_Valeur::ParametreMotClef)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreMotClef>(p);
	if(not v) { throw MauvaisAppelFonction(__func__,"parameter does not contain a keyword"); }
	SHARED_PTR(std::string) m = std::make_shared<std::string>(v->_mot_clef);
	return svm_string_new__raw(svm,m->c_str());
}

SVM_Parameter svm_parameter_keyword_new(const void *svm, const SVM_String keyword)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not keyword.string) { throw MauvaisAppelFonction(__func__,"keyword is null"); }
	if(keyword.size==0) { throw MauvaisAppelFonction(__func__,"keyword is empty"); }
	std::string m(keyword.string,keyword.size);
	for(auto& c:m)
	{
		if((c<'A') or (c>'Z'))
		{
			throw MauvaisAppelFonction(__func__,"keyword contains invalid chars");
		}
	}
	SVM_Valeur::ParametreSP p = std::make_shared<SVM_Valeur::ParametreMotClef>(m);
	return environnement->ajout(p);
}

SVM_Parameter svm_parameter_keyword_new__raw(const void *svm, const char *keyword)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not keyword) { throw MauvaisAppelFonction(__func__,"keyword is void"); }
	std::string m(keyword);
	if(m.empty()) { throw MauvaisAppelFonction(__func__,"keyword is empty"); }
	for(auto& c:m)
	{
		if((c<'A') or (c>'Z'))
		{
			throw MauvaisAppelFonction(__func__,"keyword contains invalid chars");
		}
	}
	SVM_Valeur::ParametreSP p = std::make_shared<SVM_Valeur::ParametreMotClef>(m);
	return environnement->ajout(p);
}

SVM_Boolean svm_parameter_type_is_structure(const void *svm, const SVM_Parameter parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,parameter,__func__,"parameter");
	const SHARED_PTR(SVM_Valeur::ParametreStructure)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreStructure>(p);
	return Outils::booleen(static_cast<bool>(v));
}

SVM_Structure svm_parameter_structure_get(const void *svm, const SVM_Parameter parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,parameter,__func__,"parameter");
	const SHARED_PTR(SVM_Valeur::ParametreStructure)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreStructure>(p);
	if(not v) { throw MauvaisAppelFonction(__func__,"parameter does not contain a structure"); }
	SVM_Interface::StructureSP vv = v->_structure;
	return environnement->ajout(vv);
}

SVM_Parameter svm_parameter_structure_new(const void *svm, const SVM_Structure structure)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_typee<SVM_Interface::Structure>(environnement,structure,__func__,"structure");
	SVM_Valeur::ParametreSP p = std::make_shared<SVM_Valeur::ParametreStructure>(valeur);
	return environnement->ajout(p);
}

SVM_Boolean svm_parameter_type_is_variable(const void *svm, const SVM_Parameter parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,parameter,__func__,"parameter");
	const SHARED_PTR(SVM_Valeur::ParametreVariable)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreVariable>(p);
	return Outils::booleen(static_cast<bool>(v));
}

SVM_Variable svm_parameter_variable_get(const void *svm, const SVM_Parameter parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,parameter,__func__,"parameter");
	const SHARED_PTR(SVM_Valeur::ParametreVariable)& v = std::dynamic_pointer_cast<SVM_Valeur::ParametreVariable>(p);
	if(not v) { throw MauvaisAppelFonction(__func__,"parameter does not contain a variable"); }
	return v->_variable;
}

SVM_Parameter svm_parameter_variable_new(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Valeur::ParametreSP p = std::make_shared<SVM_Valeur::ParametreVariable>(variable);
	return environnement->ajout(p);
}

SVM_Type svm_type_new_internal(const void *svm, const SVM_Type_Internal internal)
{
	auto environnement = Outils::environnement(svm,__func__);
	std::shared_ptr<SVM_Memoire::Type> type = std::make_shared<SVM_Memoire::Type>(Outils::type_interne(environnement,internal,__func__,"internal"));
	return environnement->ajout(type);
}

SVM_Type svm_type_new_external(const void *svm, const SVM_Value_PluginEntryPoint external)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,external,__func__,"external","a plugin entry name");

	auto patron = environnement->_machine->_base->_gestionnaire_extensions->type(*pep);
	if(not patron)
	{
		throw SVM_Memoire::TypeInexistant(*pep);
	}
	std::shared_ptr<SVM_Memoire::Type> type = std::make_shared<SVM_Memoire::Type>(*pep,patron);
	return environnement->ajout(type);
}

SVM_Boolean svm_type_is_internal(const void *svm, const SVM_Type type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	return Outils::booleen(not ((*t)==SVM_Memoire::Type::TypeInterne::TYPE_EXTENSION));
}

SVM_Boolean svm_type_is_external(const void *svm, const SVM_Type type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	return Outils::booleen((*t)==SVM_Memoire::Type::TypeInterne::TYPE_EXTENSION);
}

SVM_Type_Internal svm_type_get_internal(const void *svm, const SVM_Type type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	if((*t)==SVM_Memoire::Type::TypeInterne::TYPE_EXTENSION)
	{
		throw MauvaisAppelFonction(__func__,"type is external");
	}
	return Outils::type_interne(environnement,*t,__func__,"type");
}

SVM_Value_PluginEntryPoint svm_type_get_external(const void *svm, const SVM_Type type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	if(not ((*t)==SVM_Memoire::Type::TypeInterne::TYPE_EXTENSION))
	{
		throw MauvaisAppelFonction(__func__,"type is internal");
	}
	auto pep = std::make_shared<SVM_Memoire::PointEntreeExtension>(Outils::type_externe(*t));
	return Outils::ajout(environnement,pep);
}

SVM_Type svm_type_copy(const void *svm, const SVM_Type type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	return environnement->ajout(std::make_shared<SVM_Memoire::Type>(*t));
}

SVM_String svm_type_print(const void *svm, const SVM_Type type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	std::ostringstream oss;
	oss << (*t);
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_Boolean svm_type_equal_internal(const void *svm, const SVM_Type type, const SVM_Type_Internal internal)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	auto ti = Outils::type_interne(environnement,internal,__func__,"internal");
	return Outils::booleen((*t)==ti);
}

SVM_Boolean svm_type_equal_external(const void *svm, const SVM_Type type, const SVM_Value_PluginEntryPoint external)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,external,__func__,"external","a plugin entry name");
	return Outils::booleen(((*t)==SVM_Memoire::Type::TypeInterne::TYPE_EXTENSION) and (Outils::type_externe(*t)==(*pep)));
}

SVM_Comparison_Result svm_type_compare(const void *svm, const SVM_Type left, const SVM_Type right)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto g = Outils::valeur_typee<SVM_Memoire::Type>(environnement,left,__func__,"left");
	auto d = Outils::valeur_typee<SVM_Memoire::Type>(environnement,right,__func__,"right");
	SVM_Comparison_Result c;
	bool e = (*g)==(*d);
	bool o1 = (*g)<(*d);
	bool o2 = (*d)<(*g);
	c.equal = Outils::booleen(e);
	c.different = Outils::booleen(not e);
	c.inferior = Outils::booleen(o1);
	c.inferior_or_equal = Outils::booleen(e or o1);
	c.superior = Outils::booleen(o2);
	c.superior_or_equal = Outils::booleen(e or o2);
	c.order = TRUE;
	c.total = TRUE;
	c.weak = TRUE;
	c.type = TRUE;
	c.null = FALSE;
	return c;
}

SVM_Value_Integer svm_value_integer_new(const void *svm, const long int integer)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Memoire::ValeurSP entier = std::make_shared<SVM_Memoire::Entier>(integer);
	return Outils::ajout(environnement,entier);
}

SVM_Value_String svm_value_string_new(const void *svm, const SVM_String string)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not string.string) { throw MauvaisAppelFonction(__func__,"string is void"); }
	std::string s(string.string,string.size);
	SVM_Memoire::ValeurSP chaine = std::make_shared<SVM_Memoire::Chaine>(s);
	return Outils::ajout(environnement,chaine);
}

SVM_Value_String svm_value_string_new__raw(const void *svm, const char *string)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not string) { throw MauvaisAppelFonction(__func__,"string is void"); }
	std::string s(string);
	SVM_Memoire::ValeurSP chaine = std::make_shared<SVM_Memoire::Chaine>(s);
	return Outils::ajout(environnement,chaine);
}

SVM_Value_String svm_value_string_new__buffer(const void *svm, const char *string, const unsigned long int size)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not string) { throw MauvaisAppelFonction(__func__,"string is void"); }
	std::string s(string,size);
	SVM_Memoire::ValeurSP chaine = std::make_shared<SVM_Memoire::Chaine>(s);
	return Outils::ajout(environnement,chaine);
}

SVM_Value_Boolean svm_value_boolean_new(const void *svm, const SVM_Boolean boolean)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Memoire::ValeurSP booleen = std::make_shared<SVM_Memoire::Booleen>(boolean==TRUE);
	return Outils::ajout(environnement,booleen);
}

SVM_Value_Boolean svm_value_boolean_new__raw(const void *svm, const int boolean)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Memoire::ValeurSP booleen = std::make_shared<SVM_Memoire::Booleen>(boolean!=0);
	return Outils::ajout(environnement,booleen);
}

SVM_Value_Pointer svm_value_pointer_new(const void *svm, const SVM_Value_Integer address, const SVM_Value_Integer size)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto adresse = Outils::valeur<SVM_Memoire::Entier>(environnement,address,__func__,"address","an integer");
	auto taille = Outils::valeur<SVM_Memoire::Entier>(environnement,size,__func__,"size","an integer");
	long long int a = *adresse;
	if(a<0) { throw MauvaisAppelFonction(__func__,"address is a negative integer"); }
	long long int t = *taille;
	if(t<0) { throw MauvaisAppelFonction(__func__,"size is a negative integer"); }
	SVM_Memoire::ValeurSP ptr = std::make_shared<SVM_Memoire::Pointeur>(static_cast<size_t>(a),static_cast<size_t>(t));
	return Outils::ajout(environnement,ptr);
}

SVM_Value_Pointer svm_value_pointer_new__raw(const void *svm, const SVM_Address address, const SVM_Size size)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Memoire::ValeurSP ptr = std::make_shared<SVM_Memoire::Pointeur>(address,size);
	return Outils::ajout(environnement,ptr);
}

SVM_Value_Library svm_value_library_new(const void *svm, const SVM_Code code)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Valeur::CodeSP c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	SVM_Memoire::BibliothequeSP b = std::make_shared<SVM_Memoire::Bibliotheque>(c);
	return Outils::ajout(environnement,b);
}

SVM_Value_Symbol svm_value_symbol_new(const void *svm, const SVM_Code code, const SVM_Address symbol)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Valeur::CodeSP c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	SVM_Memoire::ValeurSP sym = std::make_shared<SVM_Memoire::Symbole>(SVM_Valeur::AdresseInstruction(c,symbol));
	return Outils::ajout(environnement,sym);
}

SVM_Value_PluginEntryPoint svm_value_pluginentrypoint_new(const void *svm, const SVM_Value_String plugin_name, const SVM_Value_String entry_name)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto extension = Outils::valeur<SVM_Memoire::Chaine>(environnement,plugin_name,__func__,"plugin_name","a string");
	auto entree = Outils::valeur<SVM_Memoire::Chaine>(environnement,entry_name,__func__,"entry_name","a string");
	SVM_Memoire::ValeurSP pep = std::make_shared<SVM_Memoire::PointEntreeExtension>(SVM_Valeur::PointEntreeExtension(*extension,*entree));
	return Outils::ajout(environnement,pep);
}

SVM_Value_PluginEntryPoint svm_value_pluginentrypoint_new__string(const void *svm, const SVM_String plugin_name, const SVM_String entry_name)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not plugin_name.string) { throw MauvaisAppelFonction(__func__,"plugin_name is void"); }
	if(not entry_name.string) { throw MauvaisAppelFonction(__func__,"entry_name is void"); }
	std::string extension(plugin_name.string,plugin_name.size);
	std::string entree(entry_name.string,entry_name.size);
	SVM_Memoire::ValeurSP pep = std::make_shared<SVM_Memoire::PointEntreeExtension>(SVM_Valeur::PointEntreeExtension(extension,entree));
	return Outils::ajout(environnement,pep);
}

SVM_Value_PluginEntryPoint svm_value_pluginentrypoint_new__raw(const void *svm, const char *plugin_name, const char *entry_name)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not plugin_name) { throw MauvaisAppelFonction(__func__,"plugin_name is void"); }
	if(not entry_name) { throw MauvaisAppelFonction(__func__,"entry_name is void"); }
	std::string extension(plugin_name);
	std::string entree(entry_name);
	SVM_Memoire::ValeurSP pep = std::make_shared<SVM_Memoire::PointEntreeExtension>(SVM_Valeur::PointEntreeExtension(extension,entree));
	return Outils::ajout(environnement,pep);
}

SVM_Value_PluginEntryPoint svm_value_pluginentrypoint_new_prefix(const void *svm, const SVM_Value_PluginEntryPoint pep, const char *prefix)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,pep,__func__,"pep"," a plugin entry point");
	if(not prefix) { throw MauvaisAppelFonction(__func__,"prefix is void"); }
	SVM_Valeur::PointEntreeExtension ipep = *valeur;
	auto npep = std::make_shared<SVM_Memoire::PointEntreeExtension>(SVM_Valeur::PointEntreeExtension(ipep.nom_extension(),ipep.nom_entree(prefix)));
	return Outils::ajout(environnement,npep);
}

SVM_Value_Interruption svm_value_interruption_new_internal(const void *svm, const SVM_Interruption_Internal interruption, const SVM_Value_String message, const SVM_Interruption_Kind kind)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto mesg = Outils::valeur<SVM_Memoire::Chaine>(environnement,message,__func__,"message","a string");
	auto interruption_interne = Outils::interruption_interne(environnement,interruption,__func__,"interruption");
	SVM_Memoire::ValeurSP i = std::make_shared<SVM_Memoire::Interruption>(SVM_Valeur::Interruption(interruption_interne,*mesg,kind==HARDWARE));
	return Outils::ajout(environnement,i);
}

SVM_Value_Interruption svm_value_interruption_new_internal__string(const void *svm, const SVM_Interruption_Internal interruption, const SVM_String message, const SVM_Interruption_Kind kind)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not message.string) { throw MauvaisAppelFonction(__func__,"message is void"); }
	auto interruption_interne = Outils::interruption_interne(environnement,interruption,__func__,"interruption");
	SVM_Memoire::ValeurSP i = std::make_shared<SVM_Memoire::Interruption>(SVM_Valeur::Interruption(interruption_interne,std::string(message.string,message.size),kind==HARDWARE));
	return Outils::ajout(environnement,i);
}

SVM_Value_Interruption svm_value_interruption_new_internal__raw(const void *svm, const SVM_Interruption_Internal interruption, const char *message, const SVM_Interruption_Kind kind)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not message) { throw MauvaisAppelFonction(__func__,"message is void"); }
	auto interruption_interne = Outils::interruption_interne(environnement,interruption,__func__,"interruption");
	SVM_Memoire::ValeurSP i = std::make_shared<SVM_Memoire::Interruption>(SVM_Valeur::Interruption(interruption_interne,std::string(message),kind==HARDWARE));
	return Outils::ajout(environnement,i);
}

SVM_Value_Interruption svm_value_interruption_new_external(const void *svm, const SVM_Value_PluginEntryPoint name, const SVM_Value_String message)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto mesg = Outils::valeur<SVM_Memoire::Chaine>(environnement,message,__func__,"message","a string");
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,name,__func__,"name"," a plugin entry point");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->interruption(*pep);
	if(not patron) { throw MauvaisAppelFonction(__func__,"name is not a plugin interruption"); }
	SVM_Memoire::ValeurSP i = std::make_shared<SVM_Memoire::Interruption>(SVM_Valeur::Interruption(*pep,*mesg));
	return Outils::ajout(environnement,i);
}

SVM_Value_Interruption svm_value_interruption_new_external__string(const void *svm, const SVM_Value_PluginEntryPoint name, const SVM_String message)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not message.string) { throw MauvaisAppelFonction(__func__,"message is void"); }
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,name,__func__,"name"," a plugin entry point");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->interruption(*pep);
	if(not patron) { throw MauvaisAppelFonction(__func__,"name is not a plugin interruption"); }
	SVM_Memoire::ValeurSP i = std::make_shared<SVM_Memoire::Interruption>(SVM_Valeur::Interruption(*pep,std::string(message.string,message.size)));
	return Outils::ajout(environnement,i);
}

SVM_Value_Interruption svm_value_interruption_new_external__raw(const void *svm, const SVM_Value_PluginEntryPoint name, const char *message)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not message) { throw MauvaisAppelFonction(__func__,"message is void"); }
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,name,__func__,"name"," a plugin entry point");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->interruption(*pep);
	if(not patron) { throw MauvaisAppelFonction(__func__,"name is not a plugin interruption"); }
	SVM_Memoire::ValeurSP i = std::make_shared<SVM_Memoire::Interruption>(SVM_Valeur::Interruption(*pep,std::string(message)));
	return Outils::ajout(environnement,i);
}

SVM_Value_Plugin svm_value_plugin_new(const void *svm, const SVM_Value_PluginEntryPoint type, void *value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type"," a plugin entry point");
	auto t = environnement->_machine->_base->_gestionnaire_extensions->type(*pep);
	if(not t)
	{
		throw SVM_Memoire::TypeInexistant(*pep);
	}
	SVM_Memoire::ValeurSP p = std::make_shared<SVM_Memoire::Extension>(environnement->_machine,*pep,t,value);
	return Outils::ajout(environnement,p);
}

SVM_Value_Plugin svm_value_new_plugin_raw_interne(SVM_Interface::EnvironnementSP& environnement, const std::string& fonction, const SVM_Memoire::PointEntreeExtensionSP& type, const std::string& valeur)
{
	auto t = environnement->_machine->_base->_gestionnaire_extensions->type(*type);
	if(not t)
	{
		throw SVM_Memoire::TypeInexistant(*type);
	}
	if(not t->_constante)
	{
		throw SVM_Memoire::TypePasDeConstante(*type);
	}

	SVM_String sv;
	sv.string = valeur.c_str();
	sv.size = valeur.size();
	SVM_Interface::EnvironnementSP e = std::make_shared<SVM_Interface::Environnement>(t,environnement->_machine,environnement->_machine->_base->_gestionnaire_extensions->variables_extension(static_cast<SVM_Valeur::PointEntreeExtension>(*type).nom_extension()));
	void *v = nullptr;
	try
	{
		v=t->_constante(&e,sv);
	}
	catch(SVM_Valeur::Interruption& i)
	{
		i.position(*(environnement->_position));
		throw;
	}
	SVM_Memoire::ValeurSP p = std::make_shared<SVM_Memoire::Extension>(environnement->_machine,*type,t,v);
	return Outils::ajout(environnement,p);
}

SVM_Value_Plugin svm_value_plugin_new_const(const void *svm, const SVM_Value_PluginEntryPoint type, const SVM_Value_String value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type"," a plugin entry point");
	auto v = Outils::valeur<SVM_Memoire::Chaine>(environnement,value,__func__,"value","a string");
	return ::svm_value_new_plugin_raw_interne(environnement,"svm_value_new_plugin_raw",pep,*v);
}

SVM_Value_Plugin svm_value_plugin_new_const__string(const void *svm, const SVM_Value_PluginEntryPoint type, const SVM_String value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type"," a plugin entry point");
	if(not value.string) { throw MauvaisAppelFonction(__func__,"value is void"); }
	return ::svm_value_new_plugin_raw_interne(environnement,"svm_value_new_plugin_raw_string",pep,std::string(value.string,value.size));
}

SVM_Value_Plugin svm_value_plugin_new_const__raw(const void *svm, const SVM_Value_PluginEntryPoint type, const char *value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type"," a plugin entry point");
	if(not value) { throw MauvaisAppelFonction(__func__,"value is void"); }
	return ::svm_value_new_plugin_raw_interne(environnement,"svm_value_new_plugin_raw_const",pep,std::string(value));
}

SVM_Value_Integer svm_value_integer_new_null(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::ENTIER)));
}

SVM_Value_String svm_value_string_new_null(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::CHAINE)));
}

SVM_Value_Boolean svm_value_boolean_new_null(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::BOOLEEN)));
}

SVM_Value_Pointer svm_value_pointer_new_null(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINTEUR)));
}

SVM_Value_Library svm_value_library_new_null(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::BIBLIOTHEQUE)));
}

SVM_Value_Symbol svm_value_symbol_new_null(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::SYMBOLE)));
}

SVM_Value_PluginEntryPoint svm_value_pluginentrypoint_new_null(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION)));
}

SVM_Value_Interruption svm_value_interruption_new_null(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::INTERRUPTION)));
}

SVM_Value_Plugin svm_value_plugin_new_null(const void *svm, const SVM_Value_PluginEntryPoint type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type"," a plugin entry point");
	auto t = environnement->_machine->_base->_gestionnaire_extensions->type(*pep);
	if(not t)
	{
		throw SVM_Memoire::TypeInexistant(*pep);
	}
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(*pep,t)));
}

SVM_Value_Automatic svm_value_automatic_new_null(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::AUTOMATIQUE)));
}

SVM_Type svm_value_type_get(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	return environnement->ajout(std::make_shared<SVM_Memoire::Type>(*valeur));
}

SVM_String svm_value_type_print(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	std::ostringstream oss;
	oss << static_cast<SVM::Machine::Element::Memoire::Type>(*valeur);
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_Comparison_Result svm_value_compare(const void *svm, const SVM_Value left, const SVM_Value right)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto gauche = Outils::valeur_valeur(environnement,left,__func__,"left");
	auto droite = Outils::valeur_valeur(environnement,right,__func__,"right");
	auto comparaison = SVM::Machine::Element::Memoire::Valeur::compare(*gauche,*droite);
	SVM_Comparison_Result c;
	c.equal = Outils::booleen(comparaison._egal);
	c.different = Outils::booleen(not comparaison._egal);
	c.inferior = Outils::booleen(comparaison._ordre and comparaison._inferieur);
	c.inferior_or_equal = Outils::booleen(comparaison._ordre and (comparaison._egal or comparaison._inferieur));
	c.superior = Outils::booleen(comparaison._ordre and comparaison._superieur);
	c.superior_or_equal = Outils::booleen(comparaison._ordre and (comparaison._egal or comparaison._superieur));
	c.order = Outils::booleen(comparaison._ordre);
	c.total = Outils::booleen(comparaison._total);
	c.type = Outils::booleen(comparaison._type);
	c.null = Outils::booleen(comparaison._nul);
	c.weak = Outils::booleen(comparaison._faible);
	return c;
}

SVM_Value_Plugin_Comparison svm_value_plugin_compare_convert(const void *svm, const SVM_Comparison_Result compare)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Value_Plugin_Comparison r;
	if(compare.order)
	{
		if(compare.equal)
		{
			r = ORDER_EQUAL;
		}
		else if(compare.inferior and compare.superior)
		{
			r = ORDER_UNCOMPARABLE;
		}
		else if(compare.inferior)
		{
			r = ORDER_INFERIOR;
		}
		else if(compare.superior)
		{
			r = ORDER_SUPERIOR;
		}
		if(not compare.total)
		{
			r = static_cast<SVM_Value_Plugin_Comparison>(r bitor ORDER_PARTIAL);
		}
	}
	else
	{
		if(compare.equal)
		{
			r = EQUIVALENCE_EQUAL;
		}
		else
		{
			r = EQUIVALENCE_DIFFERENT;
		}
	}
	if(compare.weak)
	{
		r = static_cast<SVM_Value_Plugin_Comparison>(r bitor COMPARISON_WEAK);
	}
	return r;
}

SVM_Boolean svm_value_is_equal(const void *svm, const SVM_Value left, const SVM_Value right)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto gauche = Outils::valeur_valeur(environnement,left,__func__,"left");
	auto droite = Outils::valeur_valeur(environnement,right,__func__,"right");
	return Outils::booleen((*gauche)==(*droite));
}

SVM_Boolean svm_value_state_is_constant(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	return Outils::booleen(valeur->est_constante());
}

SVM_Boolean svm_value_state_is_movable(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	return Outils::booleen(valeur->est_deplacable());
}

SVM_Boolean svm_value_state_set_movable(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	if(valeur.use_count()==2)
	{
		valeur->deplacable();
	}
	return Outils::booleen(valeur->est_deplacable());
}

SVM_Value svm_value_copy(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	auto copie = valeur->copie();
	return Outils::ajout(environnement,copie);
}

SVM_String svm_value_print(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	std::ostringstream oss;
	oss << (*valeur);
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_Value* svm_value_array_new(const void *svm, SVM_Size size)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Interface::TableauVariablesSP tableau = std::make_shared<SVM_Interface::TableauVariables>("values",size);
	environnement->ajout_avatar(tableau);
	return tableau->_variables;
}

SVM_Boolean svm_value_state_is_null(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	return Outils::booleen(valeur->nulle());
}

SVM_Boolean svm_value_type_is_integer(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	auto specifique = std::dynamic_pointer_cast<SVM_Memoire::Entier>(valeur);
	return Outils::booleen(static_cast<bool>(specifique));
}

SVM_Boolean svm_value_type_is_string(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	auto specifique = std::dynamic_pointer_cast<SVM_Memoire::Chaine>(valeur);
	return Outils::booleen(static_cast<bool>(specifique));
}

SVM_Boolean svm_value_type_is_boolean(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	auto specifique = std::dynamic_pointer_cast<SVM_Memoire::Booleen>(valeur);
	return Outils::booleen(static_cast<bool>(specifique));
}

SVM_Boolean svm_value_type_is_pointer(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	auto specifique = std::dynamic_pointer_cast<SVM_Memoire::Pointeur>(valeur);
	return Outils::booleen(static_cast<bool>(specifique));
}

SVM_Boolean svm_value_type_is_library(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	auto specifique = std::dynamic_pointer_cast<SVM_Memoire::Bibliotheque>(valeur);
	return Outils::booleen(static_cast<bool>(specifique));
}

SVM_Boolean svm_value_type_is_symbol(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	auto specifique = std::dynamic_pointer_cast<SVM_Memoire::Symbole>(valeur);
	return Outils::booleen(static_cast<bool>(specifique));
}

SVM_Boolean svm_value_type_is_pluginentrypoint(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	auto specifique = std::dynamic_pointer_cast<SVM_Memoire::PointEntreeExtension>(valeur);
	return Outils::booleen(static_cast<bool>(specifique));
}

SVM_Boolean svm_value_type_is_interruption(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	auto specifique = std::dynamic_pointer_cast<SVM_Memoire::Interruption>(valeur);
	return Outils::booleen(static_cast<bool>(specifique));
}

SVM_Boolean svm_value_type_is_plugin(const void *svm, const SVM_Value value, const SVM_Value_PluginEntryPoint type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type"," a plugin entry point");
	auto specifique = std::dynamic_pointer_cast<SVM_Memoire::Extension>(valeur);
	if(not Outils::booleen(static_cast<bool>(specifique))) return FALSE;
	return Outils::booleen(Outils::type_externe(static_cast<SVM_Memoire::Type>(*specifique))==*pep);
}

long long int svm_value_integer_get(const void *svm, const SVM_Value_Integer integer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Entier>(environnement,integer,__func__,"integer","an integer");
	return *valeur;
}

SVM_String svm_value_string_get(const void *svm, const SVM_Value_String string)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Chaine>(environnement,string,__func__,"string","a string");
	return Outils::chaine(environnement,*valeur);
}

SVM_Boolean svm_value_boolean_get(const void *svm, const SVM_Value_Boolean boolean)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Booleen>(environnement,boolean,__func__,"boolean","a boolean");
	return Outils::booleen(*valeur);
}

SVM_Address svm_value_pointer_get_address(const void *svm, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *valeur;
	return Outils::adresse(p.adresse());
}

SVM_Size svm_value_pointer_get_size(const void *svm, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *valeur;
	return p.taille();
}

SVM_Code svm_value_library_get_code(const void *svm, const SVM_Value_Library library)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Bibliotheque>(environnement,library,__func__,"library","a library");
	return environnement->ajout(static_cast<SVM_Valeur::CodeSP>(*valeur));
}

SVM_Code svm_value_symbol_get_code(const void *svm, const SVM_Value_Symbol symbol)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	SVM_Valeur::AdresseInstruction adresse = *valeur;
	return environnement->ajout(Outils::code(adresse));
}

SVM_Address svm_value_symbol_get_address(const void *svm, const SVM_Value_Symbol symbol)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	SVM_Valeur::AdresseInstruction adresse = *valeur;
	return Outils::locale(adresse);
}

SVM_String svm_value_symbol_get_location(const void *svm, const SVM_Value_Symbol symbol)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	SVM_Valeur::AdresseInstruction adresse = *valeur;
	auto instruction = adresse.instruction();
	if(not instruction)
	{
		throw MauvaisAppelFonction(__func__,"symbol is not linked to an instruction");
	}
	SVM_Valeur::PositionInstruction position = Outils::position(instruction);
	std::ostringstream oss;
	oss << position;
	return Outils::chaine(environnement,oss.str());
}

SVM_String svm_value_pluginentrypoint_get_plugin(const void *svm, const SVM_Value_PluginEntryPoint pep)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,pep,__func__,"pep","a plugin entry point");
	SVM_Valeur::PointEntreeExtension ipep = *valeur;
	return Outils::chaine(environnement,ipep.nom_extension());
}

SVM_String svm_value_pluginentrypoint_get_entry(const void *svm, const SVM_Value_PluginEntryPoint pep)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,pep,__func__,"pep","a plugin entry point");
	SVM_Valeur::PointEntreeExtension ipep = *valeur;
	return Outils::chaine(environnement,ipep.nom_entree_pur());
}

SVM_Boolean svm_value_interruption_is_internal(const void *svm, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto irq = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	SVM_Valeur::Interruption i = *irq;
	return Outils::booleen(not (i==SVM_Valeur::Interruption::InterruptionInterne::INTERRUPTION_EXTENSION));
}

SVM_Boolean svm_value_interruption_is_external(const void *svm, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto irq = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	SVM_Valeur::Interruption i = *irq;
	return Outils::booleen(i==SVM_Valeur::Interruption::InterruptionInterne::INTERRUPTION_EXTENSION);
}

SVM_Interruption_Internal svm_value_interruption_get_internal(const void *svm, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	SVM_Valeur::Interruption i = *valeur;
	if(i==SVM_Valeur::Interruption::InterruptionInterne::INTERRUPTION_EXTENSION)
	{
		throw MauvaisAppelFonction(__func__,"interruption is external");
	}
	return Outils::interruption_interne(environnement,i,__func__,"interruption");
}

SVM_Value_PluginEntryPoint svm_value_interruption_get_external(const void *svm, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	SVM_Valeur::Interruption i = *valeur;
	if(not (i==SVM_Valeur::Interruption::InterruptionInterne::INTERRUPTION_EXTENSION))
	{
		throw MauvaisAppelFonction(__func__,"interruption is internal");
	}
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::PointEntreeExtension>(Outils::pep(i)));
}

SVM_String svm_value_interruption_get_message(const void *svm, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	return Outils::chaine(environnement,Outils::details(*valeur));
}

SVM_Interruption_Kind svm_value_interruption_get_kind(const void *svm, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	return Outils::nature(*valeur)?HARDWARE:SOFTWARE;
}

SVM_Value_PluginEntryPoint svm_value_plugin_get_type(const void *svm, const SVM_Value_Plugin plugin)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Extension>(environnement,plugin,__func__,"plugin","a plugin type");
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::PointEntreeExtension>(Outils::type(*valeur)));
}

void* svm_value_plugin_get_internal(const void *svm, const SVM_Value_Plugin plugin)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Extension>(environnement,plugin,__func__,"plugin","a plugin type");
	return Outils::valeur(*valeur);
}

void svm_value_integer_set(const void *svm,  SVM_Value_Integer value, const long long int integer)
{
	auto environnement = Outils::environnement(svm,__func__);
	Outils::change_valeur<SVM_Memoire::Entier>(environnement,value,integer,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::ENTIER),__func__,"value","an integer");
}

void svm_value_string_set(const void *svm,  SVM_Value_String value, const SVM_String string)
{
	auto environnement = Outils::environnement(svm,__func__);
	Outils::change_valeur<SVM_Memoire::Chaine>(environnement,value,std::string(string.string,string.size),SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::CHAINE),__func__,"value","a string");
}

void svm_value_string_set__raw(const void *svm,  SVM_Value_String value, const char *string)
{
	auto environnement = Outils::environnement(svm,__func__);
	Outils::change_valeur<SVM_Memoire::Chaine>(environnement,value,std::string(string),SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::CHAINE),__func__,"value","a string");
}

void svm_value_string_set__buffer(const void *svm,  SVM_Value_String value, const char *string, const unsigned long int size)
{
	auto environnement = Outils::environnement(svm,__func__);
	Outils::change_valeur<SVM_Memoire::Chaine>(environnement,value,std::string(string,size),SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::CHAINE),__func__,"value","a string");
}

void svm_value_boolean_set(const void *svm,  SVM_Value_Boolean value, const SVM_Boolean boolean)
{
	auto environnement = Outils::environnement(svm,__func__);
	Outils::change_valeur<SVM_Memoire::Booleen>(environnement,value,boolean==TRUE,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::BOOLEEN),__func__,"value","a boolean");
}

void svm_value_boolean_set__raw(const void *svm,  SVM_Value_Boolean value, const int boolean)
{
	auto environnement = Outils::environnement(svm,__func__);
	Outils::change_valeur<SVM_Memoire::Booleen>(environnement,value,boolean!=0,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::BOOLEEN),__func__,"value","a boolean");
}

void svm_value_pointer_set(const void *svm, SVM_Value_Pointer value, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	Outils::change_valeur<SVM_Memoire::Pointeur>(environnement,value,static_cast<SVM_Valeur::Pointeur>(*pointeur),SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINTEUR),__func__,"value","a pointer");
}

void svm_value_pointer_set_address(const void *svm,  SVM_Value_Pointer value, const SVM_Value_Integer address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,value,__func__,"value","a pointer");
	auto adresse = Outils::valeur<SVM_Memoire::Entier>(environnement,address,__func__,"address","an integer");
	if(adresse<0) { throw MauvaisAppelFonction(__func__,"address is a negative integer"); }
	SVM_Valeur::Pointeur p(SVM_Valeur::AdresseMemoire(*adresse),static_cast<SVM_Valeur::Pointeur>(*pointeur).taille());
	Outils::change_valeur<SVM_Memoire::Pointeur>(environnement,value,p,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINTEUR),__func__,"value","a pointer");
}

void svm_value_pointer_set_address__raw(const void *svm,  SVM_Value_Pointer value, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,value,__func__,"value","a pointer");
	SVM_Valeur::Pointeur p(SVM_Valeur::AdresseMemoire(address),static_cast<SVM_Valeur::Pointeur>(*pointeur).taille());
	Outils::change_valeur<SVM_Memoire::Pointeur>(environnement,value,p,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINTEUR),__func__,"value","a pointer");
}

void svm_value_pointer_set_size(const void *svm,  SVM_Value_Pointer value, const SVM_Value_Integer size)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,value,__func__,"value","a pointer");
	auto taille = Outils::valeur<SVM_Memoire::Entier>(environnement,size,__func__,"size","an integer");
	if(taille<0) { throw MauvaisAppelFonction(__func__,"size is a negative integer"); }
	SVM_Valeur::Pointeur p(static_cast<SVM_Valeur::Pointeur>(*pointeur).adresse(),*taille);
	Outils::change_valeur<SVM_Memoire::Pointeur>(environnement,value,p,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINTEUR),__func__,"value","a pointer");
}

void svm_value_pointer_set_size__raw(const void *svm,  SVM_Value_Pointer value, const SVM_Size size)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,value,__func__,"value","a pointer");
	SVM_Valeur::Pointeur p(static_cast<SVM_Valeur::Pointeur>(*pointeur).adresse(),size);
	Outils::change_valeur<SVM_Memoire::Pointeur>(environnement,value,p,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINTEUR),__func__,"value","a pointer");
}

void svm_value_pointer_set_addresssize(const void *svm,  SVM_Value_Pointer value, const SVM_Value_Integer address, const SVM_Value_Integer size)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto adresse = Outils::valeur<SVM_Memoire::Entier>(environnement,address,__func__,"address","an integer");
	auto taille = Outils::valeur<SVM_Memoire::Entier>(environnement,size,__func__,"size","an integer");
	if(adresse<0) { throw MauvaisAppelFonction(__func__,"address is a negative integer"); }
	if(taille<0) { throw MauvaisAppelFonction(__func__,"size is a negative integer"); }
	SVM_Valeur::Pointeur p(SVM_Valeur::AdresseMemoire(*adresse),*taille);
	Outils::change_valeur<SVM_Memoire::Pointeur>(environnement,value,p,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINTEUR),__func__,"value","a pointer");
}

void svm_value_pointer_set_addresssize__raw(const void *svm,  SVM_Value_Pointer value, const SVM_Address address, const SVM_Size size)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Valeur::Pointeur p(SVM_Valeur::AdresseMemoire(address),size);
	Outils::change_valeur<SVM_Memoire::Pointeur>(environnement,value,p,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINTEUR),__func__,"value","a pointer");
}

void svm_value_library_set(const void *svm, SVM_Value_Library value, const SVM_Value_Library library)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto bibliotheque = Outils::valeur<SVM_Memoire::Bibliotheque>(environnement,library,__func__,"library","a library");
	Outils::change_valeur<SVM_Memoire::Bibliotheque>(environnement,value,static_cast<SVM_Valeur::CodeSP>(*bibliotheque),SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::BIBLIOTHEQUE),__func__,"value","a library");
}

void svm_value_library_set_code(const void *svm,  SVM_Value_Library value, const SVM_Code code)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	Outils::change_valeur<SVM_Memoire::Bibliotheque>(environnement,value,c,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::BIBLIOTHEQUE),__func__,"value","a library");
}

void svm_value_symbol_set(const void *svm, SVM_Value_Symbol value, const SVM_Value_Symbol symbol)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto symbole = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	Outils::change_valeur<SVM_Memoire::Symbole>(environnement,value,static_cast<SVM_Valeur::AdresseInstruction>(*symbole),SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::SYMBOLE),__func__,"value","a symbol");
}

void svm_value_symbol_set_code(const void *svm,  SVM_Value_Symbol value, const SVM_Code code)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto symbole = Outils::valeur<SVM_Memoire::Symbole>(environnement,value,__func__,"value","a symbol");
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	SVM_Valeur::AdresseInstruction s(c,Outils::locale(*symbole));
	Outils::change_valeur<SVM_Memoire::Symbole>(environnement,value,s,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::SYMBOLE),__func__,"value","a symbol");
}

void svm_value_symbol_set_address(const void *svm,  SVM_Value_Symbol value, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto symbole = Outils::valeur<SVM_Memoire::Symbole>(environnement,value,__func__,"value","a symbol");
	SVM_Valeur::AdresseInstruction s(Outils::code(*symbole),address);
	Outils::change_valeur<SVM_Memoire::Symbole>(environnement,value,s,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::SYMBOLE),__func__,"value","a symbol");
}

void svm_value_symbol_set_codeaddress(const void *svm,  SVM_Value_Symbol value, const SVM_Code code, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	SVM_Valeur::AdresseInstruction s(c,address);
	Outils::change_valeur<SVM_Memoire::Symbole>(environnement,value,s,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::SYMBOLE),__func__,"value","a symbol");
}

void svm_value_pluginentrypoint_set(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_Value_PluginEntryPoint pluginentrypoint)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto point_entree_extension = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,pluginentrypoint,__func__,"pluginentrypoint","a plugin entry point");
	Outils::change_valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,static_cast<SVM_Valeur::PointEntreeExtension>(*point_entree_extension),SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION),__func__,"value","a plugin entry point");
}

void svm_value_pluginentrypoint_set_plugin(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_Value_String plugin_name)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointentreeextension = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,__func__,"value"," a plugin entry point");
	auto extension = Outils::valeur<SVM_Memoire::Chaine>(environnement,plugin_name,__func__,"plugin_name","a string");
	SVM_Valeur::PointEntreeExtension pep(*extension,static_cast<SVM_Valeur::PointEntreeExtension>(*pointentreeextension).nom_entree_pur());
	Outils::change_valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,pep,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION),__func__,"value","a plugin entry point");
}

void svm_value_pluginentrypoint_set_plugin__string(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_String plugin_name)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointentreeextension = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,__func__,"value"," a plugin entry point");
	auto extension = Outils::chaine(environnement,plugin_name,__func__,"plugin_name");
	SVM_Valeur::PointEntreeExtension pep(extension,static_cast<SVM_Valeur::PointEntreeExtension>(*pointentreeextension).nom_entree_pur());
	Outils::change_valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,pep,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION),__func__,"value","a plugin entry point");
}

void svm_value_pluginentrypoint_set_plugin__raw(const void *svm, SVM_Value_PluginEntryPoint value, const char *plugin_name)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointentreeextension = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,__func__,"value"," a plugin entry point");
	if(not plugin_name) { throw MauvaisAppelFonction(__func__,"plugin_name is void"); }
	SVM_Valeur::PointEntreeExtension pep(std::string(plugin_name),static_cast<SVM_Valeur::PointEntreeExtension>(*pointentreeextension).nom_entree_pur());
	Outils::change_valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,pep,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION),__func__,"value","a plugin entry point");
}

void svm_value_pluginentrypoint_set_entry(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_Value_String entry_name, const char *prefix)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointentreeextension = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,__func__,"value"," a plugin entry point");
	auto entree = Outils::valeur<SVM_Memoire::Chaine>(environnement,entry_name,__func__,"entry_name","a string");
	std::string prefixe;
	if(prefix) { prefixe = std::string(prefix)+"_"; }
	SVM_Valeur::PointEntreeExtension pep(static_cast<SVM_Valeur::PointEntreeExtension>(*pointentreeextension).nom_extension(),prefixe+static_cast<std::string>(*entree));
	Outils::change_valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,pep,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION),__func__,"value","a plugin entry point");
}

void svm_value_pluginentrypoint_set_entry__string(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_String entry_name, const char *prefix)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointentreeextension = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,__func__,"value"," a plugin entry point");
	auto entree = Outils::chaine(environnement,entry_name,__func__,"entry_name");
	std::string prefixe;
	if(prefix) { prefixe = std::string(prefix)+"_"; }
	SVM_Valeur::PointEntreeExtension pep(static_cast<SVM_Valeur::PointEntreeExtension>(*pointentreeextension).nom_extension(),prefixe+entree);
	Outils::change_valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,pep,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION),__func__,"value","a plugin entry point");
}

void svm_value_pluginentrypoint_set_entry__raw(const void *svm, SVM_Value_PluginEntryPoint value, const char *entry_name, const char *prefix)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pointentreeextension = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,__func__,"value"," a plugin entry point");
	if(not entry_name) { throw MauvaisAppelFonction(__func__,"entry_name is void"); }
	std::string prefixe;
	if(prefix) { prefixe = std::string(prefix)+"_"; }
	SVM_Valeur::PointEntreeExtension pep(static_cast<SVM_Valeur::PointEntreeExtension>(*pointentreeextension).nom_extension(),prefixe+std::string(entry_name));
	Outils::change_valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,pep,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION),__func__,"value","a plugin entry point");
}

void svm_value_pluginentrypoint_set_pluginentry(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_Value_String plugin_name, const SVM_Value_String entry_name, const char *prefix)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto extension = Outils::valeur<SVM_Memoire::Chaine>(environnement,plugin_name,__func__,"plugin_name","a string");
	auto entree = Outils::valeur<SVM_Memoire::Chaine>(environnement,entry_name,__func__,"entry_name","a string");
	std::string prefixe;
	if(prefix) { prefixe = std::string(prefix)+"_"; }
	SVM_Valeur::PointEntreeExtension pep(*extension,prefixe+static_cast<std::string>(*entree));
	Outils::change_valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,pep,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION),__func__,"value","a plugin entry point");
}

void svm_value_pluginentrypoint_set_pluginentry__string(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_String plugin_name, const SVM_String entry_name, const char *prefix)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto extension = Outils::chaine(environnement,plugin_name,__func__,"plugin_name");
	auto entree = Outils::chaine(environnement,entry_name,__func__,"entry_name");
	std::string prefixe;
	if(prefix) { prefixe = std::string(prefix)+"_"; }
	SVM_Valeur::PointEntreeExtension pep(extension,prefixe+entree);
	Outils::change_valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,pep,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION),__func__,"value","a plugin entry point");
}

void svm_value_pluginentrypoint_set_pluginentry__raw(const void *svm, SVM_Value_PluginEntryPoint value, const char *plugin_name, const char *entry_name, const char *prefix)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not plugin_name) { throw MauvaisAppelFonction(__func__,"plugin_name is void"); }
	if(not entry_name) { throw MauvaisAppelFonction(__func__,"entry_name is void"); }
	std::string prefixe;
	if(prefix) { prefixe = std::string(prefix)+"_"; }
	SVM_Valeur::PointEntreeExtension pep(std::string(plugin_name),prefixe+std::string(entry_name));
	Outils::change_valeur<SVM_Memoire::PointEntreeExtension>(environnement,value,pep,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINT_ENTREE_EXTENSION),__func__,"value","a plugin entry point");
}

void svm_value_interruption_set(const void *svm, SVM_Value_Interruption value, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto irq = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	Outils::change_valeur<SVM_Memoire::Interruption>(environnement,value,static_cast<SVM_Valeur::Interruption>(*irq),SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::INTERRUPTION),__func__,"value","an interruption");
}

void svm_value_interruption_set_internal(const void *svm,  SVM_Value_Interruption value, const SVM_Interruption_Internal type, const SVM_Value_String message, const SVM_Interruption_Kind kind)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto interne = Outils::interruption_interne(environnement,type,__func__,"type");
	auto mesg = Outils::valeur<SVM_Memoire::Chaine>(environnement,message,__func__,"message","a string");
	SVM_Valeur::Interruption interruption(interne,*mesg,kind==HARDWARE);
	Outils::change_valeur<SVM_Memoire::Interruption>(environnement,value,interruption,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::INTERRUPTION),__func__,"value","an interruption");
}

void svm_value_interruption_set_internal__string(const void *svm,  SVM_Value_Interruption value, const SVM_Interruption_Internal type, const SVM_String message, const SVM_Interruption_Kind kind)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto interne = Outils::interruption_interne(environnement,type,__func__,"type");
	auto mesg = Outils::chaine(environnement,message,__func__,"message");
	SVM_Valeur::Interruption interruption(interne,mesg,kind==HARDWARE);
	Outils::change_valeur<SVM_Memoire::Interruption>(environnement,value,interruption,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::INTERRUPTION),__func__,"value","an interruption");
}

void svm_value_interruption_set_internal__raw(const void *svm,  SVM_Value_Interruption value, const SVM_Interruption_Internal type, const char *message, const SVM_Interruption_Kind kind)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto interne = Outils::interruption_interne(environnement,type,__func__,"type");
	if(not message) { throw MauvaisAppelFonction(__func__,"message is void"); }
	SVM_Valeur::Interruption interruption(interne,std::string(message),kind==HARDWARE);
	Outils::change_valeur<SVM_Memoire::Interruption>(environnement,value,interruption,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::INTERRUPTION),__func__,"value","an interruption");
}

void svm_value_interruption_set_external(const void *svm,  SVM_Value_Interruption value, const SVM_Value_PluginEntryPoint name, const SVM_Value_String message)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,name,__func__,"name","a plugin entry name");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->interruption(*nom);
	if(not patron) { throw MauvaisAppelFonction(__func__,"name is not a plugin interruption"); }
	auto mesg = Outils::valeur<SVM_Memoire::Chaine>(environnement,message,__func__,"message","a string");
	SVM_Valeur::Interruption interruption(*nom,*mesg);
	Outils::change_valeur<SVM_Memoire::Interruption>(environnement,value,interruption,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::INTERRUPTION),__func__,"value","an interruption");
}

void svm_value_interruption_set_external__string(const void *svm,  SVM_Value_Interruption value, const SVM_Value_PluginEntryPoint name, const SVM_String message)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,name,__func__,"name","a plugin entry name");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->interruption(*nom);
	if(not patron) { throw MauvaisAppelFonction(__func__,"name is not a plugin interruption"); }
	auto mesg = Outils::chaine(environnement,message,__func__,"message");
	SVM_Valeur::Interruption interruption(*nom,mesg);
	Outils::change_valeur<SVM_Memoire::Interruption>(environnement,value,interruption,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::INTERRUPTION),__func__,"value","an interruption");
}

void svm_value_interruption_set_external__raw(const void *svm,  SVM_Value_Interruption value, const SVM_Value_PluginEntryPoint name, const char *message)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,name,__func__,"name","a plugin entry name");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->interruption(*nom);
	if(not patron) { throw MauvaisAppelFonction(__func__,"name is not a plugin interruption"); }
	if(not message) { throw MauvaisAppelFonction(__func__,"message is void"); }
	SVM_Valeur::Interruption interruption(*nom,std::string(message));
	Outils::change_valeur<SVM_Memoire::Interruption>(environnement,value,interruption,SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::INTERRUPTION),__func__,"value","an interruption");
}

void svm_value_plugin_set_interne(SVM_Interface::EnvironnementSP& environnement, const std::string& fonction, SVM_Value_Plugin value, const SVM_Valeur::PointEntreeExtension type, void *internal_value)
{
	auto t = environnement->_machine->_base->_gestionnaire_extensions->type(type);
	auto valeur_interface = Outils::valeur_interface(environnement,value,fonction,"value");
	SVM_Memoire::Type type_valeur = *valeur_interface->_valeur;
	if(not (type_valeur==SVM_Memoire::Type(type)))
	{
		throw MauvaisAppelFonction(fonction,"plugin type mismatch");
	}
	if(valeur_interface->_valeur->nulle())
	{
		Outils::valeur_nulle(environnement,valeur_interface->_valeur,SVM_Memoire::Type(type,t),fonction,"value","a plugin type");
		valeur_interface->_valeur = std::make_shared<SVM_Memoire::Extension>(environnement->_machine,type,t,internal_value);
		return;
	}
	auto vv = Outils::valeur<SVM_Memoire::Extension>(environnement,valeur_interface,fonction,"value","a plugin type");
	if(vv->est_constante()) { throw MauvaisAppelFonction(fonction,"value is a constant"); }
	vv->destruction();
	Outils::ecrase_valeur(vv,type,internal_value);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau_courant = environnement->valeur<SVM_Noyau::Noyau>(environnement->_variable_noyau);
	if(static_cast<bool>(noyau_courant))
	{
		vv->notification_debugueur_changement_memoire(noyau_courant->_memoire->_piege);
	}
	else
	{
		vv->notification_debugueur_changement_memoire(SVM_Memoire::PiegeMemoireSP());
	}
}

void svm_value_plugin_set(const void *svm, SVM_Value_Plugin value, const SVM_Value_Plugin plugin)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Extension>(environnement,value,__func__,"value","a plugin type");
	auto extension = Outils::valeur<SVM_Memoire::Extension>(environnement,plugin,__func__,"plugin","a plugin type");
	void *copie = nullptr;
	try
	{
		copie = extension->copie_interne();
	}
	catch(const SVM_Memoire::TypePasDeCopie& t)
	{
		throw MauvaisAppelFonction(__func__,"plugin value has no copy operation");
	}
	try
	{
		::svm_value_plugin_set_interne(environnement,__func__,value,Outils::type(*extension),copie);
	}
	catch(...)
	{
		throw MauvaisAppelFonction(__func__,"error during plugin value deletion");
	}
}

void svm_value_plugin_set_internal(const void *svm,  SVM_Value_Plugin value, const SVM_Value_PluginEntryPoint type, void *internal_value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur = Outils::valeur<SVM_Memoire::Extension>(environnement,value,__func__,"value","a plugin type");
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type","a plugin entry point");
	try
	{
		::svm_value_plugin_set_interne(environnement,__func__,value,*pep,internal_value);
	}
	catch(...)
	{
		throw MauvaisAppelFonction(__func__,"error during plugin value deletion");
	}
}

SVM_Variable* svm_variable_array_new(const void *svm, SVM_Size size)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Interface::TableauVariablesSP tableau = std::make_shared<SVM_Interface::TableauVariables>("variables",size);
	environnement->ajout_avatar(tableau);
	return tableau->_variables;
}

void svm_variable_delete(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_nettoyage) return;
	auto valeur = environnement->valeur<SVM_Memoire::Extension>(variable);
	environnement->libere(variable);
	if(not valeur) return;
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau_courant = environnement->valeur<SVM_Noyau::Noyau>(environnement->_variable_noyau);
	if(static_cast<bool>(noyau_courant))
	{
		valeur->notification_debugueur_changement_memoire(noyau_courant->_memoire->_piege);
	}
	else
	{
		valeur->notification_debugueur_changement_memoire(SVM_Memoire::PiegeMemoireSP());
	}
}

void svm_variable_scope_set_global(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_nettoyage) return;
	environnement->globale(variable);
}

void svm_variable_scope_set_local(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_nettoyage) return;
	environnement->locale(variable);
}

void svm_variable_scope_set_shared(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_nettoyage) return;
	environnement->partagee(variable);
}

void svm_variable_scope_reset_shared(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_nettoyage) return;
	environnement->reguliere(variable);
}

SVM_Boolean svm_variable_scope_is_shared(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_nettoyage) return Outils::booleen(false);
	return Outils::booleen(environnement->est_partagee(variable));
}

SVM_Boolean svm_variable_scope_is_local(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_nettoyage) return Outils::booleen(true);
	return Outils::booleen(environnement->ephemere(variable));
}

SVM_Boolean svm_variable_scope_is_global(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_nettoyage) return Outils::booleen(true);
	return Outils::booleen(environnement->durable(variable));
}

SVM_Boolean svm_variable_is_valid(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_nettoyage) return Outils::booleen(true);
	return Outils::booleen(environnement->existe(variable));
}

SVM_Boolean svm_variable_type_is_value(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Memoire::ValeurInterface>(variable)));
}

SVM_Boolean svm_variable_type_is_structure(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Interface::Structure>(variable)));
}

SVM_Boolean svm_variable_type_is_parameter(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Valeur::Parametre>(variable)));
}

SVM_Boolean svm_variable_type_is_accesscontrol(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Noyau::ControleAcces>(variable)));
}

SVM_Boolean svm_variable_type_is_memoryzone(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Memoire::DescriptionBloc>(variable)));
}

SVM_Boolean svm_variable_type_is_code(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Valeur::Code>(variable)));
}

SVM_Boolean svm_variable_type_is_kernel(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Noyau::Noyau>(variable)));
}

SVM_Boolean svm_variable_type_is_process(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<VariableProcessus>(variable)));
}

SVM_Boolean svm_variable_type_is_processlock(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Interface::VerrouProcessus>(variable)));
}

SVM_Boolean svm_variable_type_is_scheduler(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Ordonnanceur::Ordonnanceur>(variable)));
}

SVM_Boolean svm_variable_type_is_lock(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Synchronisation::Acces>(variable)));
}

SVM_Boolean svm_variable_type_is_lockguardread(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Synchronisation::LecteurUP>(variable)));
}

SVM_Boolean svm_variable_type_is_lockguardwrite(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Synchronisation::EcrivainUP>(variable)));
}

SVM_Boolean svm_variable_type_is_eventqueue(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Interface::QueueEvenements>(variable)));
}

SVM_Boolean svm_variable_type_is_eventqueueaddress(const void *svm, const SVM_Variable variable)
{
	auto environnement = Outils::environnement(svm,__func__);
	return Outils::booleen(static_cast<bool>(environnement->valeur<SVM_Synchronisation::Adresse>(variable)));
}

SVM_Structure svm_structure_new(const void *svm, const SVM_Value_PluginEntryPoint type, void *value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type","a plugin entry point");
	SVM_Extension::PatronStructureExtensionSP patron = environnement->_machine->_base->_gestionnaire_extensions->structure(*nom);
	if(not patron) { throw MauvaisAppelFonction(__func__,"name is not a valid struct name"); }
	if(not value) { throw MauvaisAppelFonction(__func__,"value is void"); }
	SVM_Interface::StructureSP structure = std::make_shared<SVM_Interface::Structure>(environnement->_machine,*nom,patron,value);
	return environnement->ajout<SVM_Interface::Structure>(structure);
}

SVM_Structure svm_structure_new_null(const void *svm, const SVM_Value_PluginEntryPoint type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type","a plugin entry point");
	SVM_Extension::PatronStructureExtensionSP patron = environnement->_machine->_base->_gestionnaire_extensions->structure(*nom);
	if(not patron) { throw MauvaisAppelFonction(__func__,"name is not a valid struct name"); }
	SVM_Interface::StructureSP structure = std::make_shared<SVM_Interface::Structure>(environnement->_machine,*nom,patron);
	return environnement->ajout<SVM_Interface::Structure>(structure);
}

SVM_Value_PluginEntryPoint svm_structure_get_type(const void *svm, const SVM_Structure structure)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur_structure = Outils::valeur_typee<SVM_Interface::Structure>(environnement,structure,__func__,"structure");
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::PointEntreeExtension>(valeur_structure->_nom));
}

void* svm_structure_get_internal(const void *svm, const SVM_Value_PluginEntryPoint type, const SVM_Structure structure)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type","a plugin entry point");
	SVM_Valeur::PointEntreeExtension n = *nom;
	SVM_Extension::PatronStructureExtensionSP patron = environnement->_machine->_base->_gestionnaire_extensions->structure(n);
	if(not patron) { throw MauvaisAppelFonction(__func__,"name is not a valid struct name"); }
	auto valeur_structure = Outils::valeur_typee<SVM_Interface::Structure>(environnement,structure,__func__,"structure");
	if(valeur_structure->_nom != n) { throw MauvaisAppelFonction(__func__,"structure has not the correct type"); }
	return valeur_structure->_valeur;
}

SVM_Structure svm_structure_copy(const void *svm, const SVM_Structure structure)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur_structure = Outils::valeur_typee<SVM_Interface::Structure>(environnement,structure,__func__,"structure");
	return environnement->ajout(valeur_structure->copie());
}

SVM_Parameter* svm_parameter_array_new(const void *svm, const SVM_Size size)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Interface::TableauVariablesSP tableau = std::make_shared<SVM_Interface::TableauVariables>("parameters",size);
	environnement->ajout_avatar(tableau);
	return tableau->_variables;
}

SVM_String svm_plugin_print(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	std::ostringstream oss;
	if(static_cast<bool>(environnement->_machine->_base->_gestionnaire_extensions))
	{
		oss << (*environnement->_machine->_base->_gestionnaire_extensions);
	}
	return ::svm_string_new__raw(svm,oss.str().c_str());
}

SVM_Boolean svm_plugin_has_instruction(const void *svm, const SVM_Value_PluginEntryPoint instruction, const SVM_Size argc, const SVM_Parameter argv[], SVM_Parameter result)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,instruction,__func__,"instruction","a plugin entry name");
	auto i = environnement->_machine->_base->_gestionnaire_extensions->instruction(*pep);
	auto r = environnement->valeur<SVM_Valeur::Parametre>(result);
	SHARED_PTR(SVM_Valeur::ParametreValeur) v;
	if(static_cast<bool>(r))
	{
		v = std::dynamic_pointer_cast<SVM_Valeur::ParametreValeur>(r);
		if(not v) { throw MauvaisAppelFonction(__func__,"result is not a value"); }
	}
	if(not i) return FALSE;
	std::vector<SVM_Extension::RegexParametresElementCSP> types;
	for(size_t i = 0 ; i<argc ; ++i)
	{
		SVM_Valeur::ParametreSP p = environnement->valeur<SVM_Valeur::Parametre>(argv[i]);
		if(not p) { throw MauvaisAppelFonction(__func__,"parameter is invalid"); }
		types.push_back(p->type());
	}
	try { i->verifie_parametres(types); } catch(...) { return FALSE; }
	if(static_cast<bool>(v))
	{
		try { i->verifie_retour(v->type(),v->_valeur->_valeur->nulle()); } catch(...) { return FALSE; }
	}
	return TRUE;
}

SVM_Boolean svm_plugin_has_type(const void *svm, const SVM_Value_PluginEntryPoint type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type","a plugin entry name");
	return Outils::booleen(static_cast<bool>(environnement->_machine->_base->_gestionnaire_extensions->type(*pep)));
}

SVM_Boolean svm_plugin_has_function(const void *svm, const SVM_Value_PluginEntryPoint function, const SVM_Size argc, const SVM_Parameter argv[], SVM_Parameter result)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,function,__func__,"function","a plugin entry name");
	auto f = environnement->_machine->_base->_gestionnaire_extensions->fonction(*pep);
	auto r = environnement->valeur<SVM_Valeur::Parametre>(result);
	SHARED_PTR(SVM_Valeur::ParametreValeur) v;
	SHARED_PTR(SVM_Valeur::ParametreStructure) s;
	if(static_cast<bool>(r))
	{
		v = std::dynamic_pointer_cast<SVM_Valeur::ParametreValeur>(r);
		s = std::dynamic_pointer_cast<SVM_Valeur::ParametreStructure>(r);
		if(not v and not s) { throw MauvaisAppelFonction(__func__,"result is not a value or a structure"); }
	}
	if(not f) return FALSE;
	std::vector<SVM_Extension::RegexParametresElementCSP> types;
	for(size_t i = 0 ; i<argc ; ++i)
	{
		SVM_Valeur::ParametreSP p = environnement->valeur<SVM_Valeur::Parametre>(argv[i]);
		if(not p) { throw MauvaisAppelFonction(__func__,"parameter is invalid"); }
		types.push_back(p->type());
	}
	try { f->verifie_parametres(types); } catch(...) { return FALSE; }
	if(static_cast<bool>(v))
	{
		try { f->verifie_retour(v->type(),v->_valeur->_valeur->nulle()); } catch(...) { return FALSE; }
	}
	if(static_cast<bool>(s))
	{
		try { f->verifie_retour(s->type()); } catch(...) { return FALSE; }
	}
	return TRUE;
}

SVM_Boolean svm_plugin_has_structure(const void *svm, const SVM_Value_PluginEntryPoint structure)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,structure,__func__,"structure","a plugin entry name");
	return Outils::booleen(static_cast<bool>(environnement->_machine->_base->_gestionnaire_extensions->structure(*pep)));
}

SVM_Boolean svm_plugin_has_interruption(const void *svm, const SVM_Value_PluginEntryPoint interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,interruption,__func__,"interruption","a plugin entry name");
	return Outils::booleen(static_cast<bool>(environnement->_machine->_base->_gestionnaire_extensions->interruption(*pep)));
}

SVM_Boolean svm_plugin_has_sequencer(const void *svm, const SVM_Value_PluginEntryPoint sequencer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,sequencer,__func__,"sequencer","a plugin entry name");
	return Outils::booleen(static_cast<bool>(environnement->_machine->_base->_gestionnaire_extensions->sequenceur(*pep)));
}

SVM_Boolean svm_plugin_has_scheduler(const void *svm, const SVM_Value_PluginEntryPoint scheduler)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,scheduler,__func__,"scheduler","a plugin entry name");
	return Outils::booleen(static_cast<bool>(environnement->_machine->_base->_gestionnaire_extensions->ordonnanceur(*pep)));
}

SVM_Boolean svm_plugin_has_option(const void *svm, const SVM_Value_PluginEntryPoint option)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,option,__func__,"option","a plugin entry name");
	return Outils::booleen(static_cast<bool>(environnement->_machine->_base->_gestionnaire_extensions->option(*pep)));
}

SVM_Boolean svm_plugin_has_argument(const void *svm, const SVM_Value_PluginEntryPoint argument)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,argument,__func__,"argument","a plugin entry name");
	return Outils::booleen(static_cast<bool>(environnement->_machine->_base->_gestionnaire_extensions->argument(*pep)));
}

SVM_Boolean svm_plugin_has_arguments(const void *svm, const SVM_Value_PluginEntryPoint arguments)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,arguments,__func__,"arguments","a plugin entry name");
	return Outils::booleen(static_cast<bool>(environnement->_machine->_base->_gestionnaire_extensions->arguments(*pep)));
}

SVM_Value svm_plugin_get_option(const void *svm, const SVM_Value_PluginEntryPoint option)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,option,__func__,"option","a plugin entry name");
	auto o = environnement->_machine->_base->_gestionnaire_extensions->option(*pep);
	if(not o) { throw MauvaisAppelFonction(__func__,"option does not exist"); }
	return Outils::ajout(environnement,o->_valeur);
}

SVM_Value svm_plugin_get_argument(const void *svm, const SVM_Value_PluginEntryPoint argument)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,argument,__func__,"argument","a plugin entry name");
	auto a = environnement->_machine->_base->_gestionnaire_extensions->argument(*pep);
	if(not a) { throw MauvaisAppelFonction(__func__,"argument does not exist"); }
	return Outils::ajout(environnement,a->_valeur);
}

SVM_Value* svm_plugin_get_arguments(const void *svm, const SVM_Value_PluginEntryPoint arguments)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,arguments,__func__,"arguments","a plugin entry name");
	auto a = environnement->_machine->_base->_gestionnaire_extensions->arguments(*pep);
	if(not a) { throw MauvaisAppelFonction(__func__,"arguments does not exist"); }
	SVM_Interface::TableauVariablesSP tableau = std::make_shared<SVM_Interface::TableauVariables>("values",a->_valeurs.size()+1);
	SVM_Value* it = tableau->_variables;
	for(const auto& v: a->_valeurs)
	{
		*it++=Outils::ajout(environnement,v);
	}
	*it=nullptr;
	environnement->ajout_avatar(tableau);
	return tableau->_variables;
}

SVM_Process svm_kernel_get_process(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau_libre(environnement,kernel,__func__,"kernel");
	auto p = noyau->_processus.lock();
	if(not p) return nullptr;
	return environnement->ajout(std::make_shared<VariableProcessus>(p));
}

SVM_Kernel svm_kernel_get_current(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return environnement->_variable_noyau;
}

SVM_Process svm_process_get_current(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return environnement->_variable_processus;
}

SVM_Scheduler svm_scheduler_get_current(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return environnement->_variable_ordonnanceur;
}

SVM_Value svm_function_call(const void *svm, const SVM_Value_PluginEntryPoint name, const SVM_Size argc,  SVM_Parameter argv[])
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,name,__func__,"name","a plugin entry point");
	SVM_Valeur::PointEntreeExtension n = *nom;
	if(not argv) { throw MauvaisAppelFonction(__func__,"argv is void"); }
	SVM_Noyau::NoyauSP noyau;
	if(environnement->_variable_noyau)
	{
		noyau = Outils::noyau(environnement,environnement->_variable_noyau,__func__,"current kernel");
	}
	std::vector<SVM_Valeur::ParametreSP> parametres;
	for(size_t i = 0 ; i<argc ; ++i)
	{
		SVM_Valeur::ParametreSP p = environnement->valeur<SVM_Valeur::Parametre>(argv[i]);
		if(not p) { throw MauvaisAppelFonction(__func__,"parameter is invalid"); }
		parametres.push_back(p);
	}
	SVM_Interface::Fonction fonction(environnement->_machine,n,parametres);
	SVM_Interface::RetourFonction retour = fonction.appel(noyau,environnement);
	if(retour._valeur) return environnement->ajout(retour._valeur);
	if(retour._structure) return environnement->ajout(retour._structure);
	if(retour._variable) return retour._variable;
	return nullptr;
}

SVM_Code svm_code_new(const void *svm, const SVM_Value_String name, const SVM_Value_String source)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::Chaine>(environnement,name,__func__,"name","a string");
	auto s = Outils::valeur<SVM_Memoire::Chaine>(environnement,source,__func__,"source","a string");
	SVM_Valeur::CodeSP code = SVM_Programme::Analyseur::analyse_chaine(*nom,*s,environnement->_machine,environnement->_position);
	return environnement->ajout(code);
}

SVM_Code svm_code_new__string(const void *svm, const SVM_String name, const SVM_String source)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::chaine(environnement,name,__func__,"name");
	auto s = Outils::chaine(environnement,source,__func__,"source");
	SVM_Valeur::CodeSP code = SVM_Programme::Analyseur::analyse_chaine(nom,s,environnement->_machine,environnement->_position);
	return environnement->ajout(code);
}

SVM_Code svm_code_new__raw(const void *svm, const char *name, const char *source)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not name) { throw MauvaisAppelFonction(__func__,"name is void"); }
	if(not source) { throw MauvaisAppelFonction(__func__,"source is void"); }
	std::string nom(name);
	std::string s(source);
	SVM_Valeur::CodeSP code = SVM_Programme::Analyseur::analyse_chaine(nom,s,environnement->_machine);
	return environnement->ajout(code);
}

SVM_Variable svm_code_compile(const void *svm, const SVM_Value_String name, const SVM_Value_String source)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::Chaine>(environnement,name,__func__,"name","a string");
	auto s = Outils::valeur<SVM_Memoire::Chaine>(environnement,source,__func__,"source","a string");
	try
	{
		SVM_Valeur::CodeSP code = SVM_Programme::Analyseur::analyse_chaine(*nom,*s,environnement->_machine,environnement->_position);
		return environnement->ajout(code);
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		i.rapport(oss);
		SVM_Memoire::ValeurSP chaine = std::make_shared<SVM_Memoire::Chaine>(oss.str());
		return Outils::ajout(environnement,chaine);
	}
}

SVM_Variable svm_code_compile__string(const void *svm, const SVM_String name, const SVM_String source)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::chaine(environnement,name,__func__,"name");
	auto s = Outils::chaine(environnement,source,__func__,"source");
	try
	{
		SVM_Valeur::CodeSP code = SVM_Programme::Analyseur::analyse_chaine(nom,s,environnement->_machine,environnement->_position);
		return environnement->ajout(code);
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		i.rapport(oss);
		SVM_Memoire::ValeurSP chaine = std::make_shared<SVM_Memoire::Chaine>(oss.str());
		return Outils::ajout(environnement,chaine);
	}
}

SVM_Variable svm_code_compile__raw(const void *svm, const char *name, const char *source)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not name) { throw MauvaisAppelFonction(__func__,"name is void"); }
	if(not source) { throw MauvaisAppelFonction(__func__,"source is void"); }
	std::string nom(name);
	std::string s(source);
	try
	{
		SVM_Valeur::CodeSP code = SVM_Programme::Analyseur::analyse_chaine(nom,s,environnement->_machine);
		return environnement->ajout(code);
	}
	catch(const SVM_Valeur::Interruption& i)
	{
		std::ostringstream oss;
		i.rapport(oss);
		SVM_Memoire::ValeurSP chaine = std::make_shared<SVM_Memoire::Chaine>(oss.str());
		return Outils::ajout(environnement,chaine);
	}
}

SVM_String svm_code_get_name(const void *svm, const SVM_Code code)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	return Outils::chaine(environnement,c->_nom);
}

SVM_String svm_code_get_source(const void *svm, const SVM_Code code)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	std::ostringstream oss;
	c->trace(oss,SHARED_PTR(size_t)(),SVM_Valeur::Code::Decorations::AUCUNE);
	return Outils::chaine(environnement,oss.str());
}

SVM_String svm_code_print(const void *svm, const SVM_Code code)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	std::ostringstream oss;
	c->trace(oss);
	return Outils::chaine(environnement,oss.str());
}

SVM_Boolean svm_code_label_has_address(const void *svm, const SVM_Code code, const SVM_Value_String label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto etiquette = Outils::valeur<SVM_Memoire::Chaine>(environnement,label,__func__,"label","a string");
	try { c->resolution_etiquette(*etiquette); } catch(const SVM_Valeur::EtiquetteNonDefinie& e) { return FALSE; }
	return TRUE;
}

SVM_Boolean svm_code_label_has_address__string(const void *svm, const SVM_Code code, const SVM_String label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto etiquette = Outils::chaine(environnement,label,__func__,"label");
	try { c->resolution_etiquette(etiquette); } catch(const SVM_Valeur::EtiquetteNonDefinie& e) { return FALSE; }
	return TRUE;
}

SVM_Boolean svm_code_label_has_address__raw(const void *svm, const SVM_Code code, const char *label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	try { c->resolution_etiquette(std::string(label)); } catch(const SVM_Valeur::EtiquetteNonDefinie& e) { return FALSE; }
	return TRUE;
}

SVM_Value_String* svm_code_label_list(const void *svm, const SVM_Code code)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto etiquettes = c->etiquettes();
	return Outils::tableau_chaines(environnement,etiquettes);
}

SVM_Boolean svm_code_symbol_has_address(const void *svm, const SVM_Code code, const SVM_Value_String label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto etiquette = Outils::valeur<SVM_Memoire::Chaine>(environnement,label,__func__,"label","a string");
	try { c->resolution_symbole(*etiquette); } catch(const SVM_Valeur::SymboleNonDefini& e) { return FALSE; }
	return TRUE;
}

SVM_Boolean svm_code_symbol_has_address__string(const void *svm, const SVM_Code code, const SVM_String label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto etiquette = Outils::chaine(environnement,label,__func__,"label");
	try { c->resolution_symbole(etiquette); } catch(const SVM_Valeur::SymboleNonDefini& e) { return FALSE; }
	return TRUE;
}

SVM_Boolean svm_code_symbol_has_address__raw(const void *svm, const SVM_Code code, const char *label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	try { c->resolution_symbole(std::string(label)); } catch(const SVM_Valeur::SymboleNonDefini& e) { return FALSE; }
	return TRUE;
}

SVM_Value_String* svm_code_symbol_list(const void *svm, const SVM_Code code)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto symboles = c->symboles();
	return Outils::tableau_chaines(environnement,symboles);
}

SVM_Address svm_code_label_get_address(const void *svm, const SVM_Code code, const SVM_Value_String label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto etiquette = Outils::valeur<SVM_Memoire::Chaine>(environnement,label,__func__,"label","a string");
	return c->resolution_etiquette(*etiquette);
}

SVM_Address svm_code_label_get_address__string(const void *svm, const SVM_Code code, const SVM_String label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto etiquette = Outils::chaine(environnement,label,__func__,"label");
	return c->resolution_etiquette(etiquette);
}

SVM_Address svm_code_label_get_address__raw(const void *svm, const SVM_Code code, const char *label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	return c->resolution_etiquette(std::string(label));
}

SVM_Value_Symbol svm_code_symbol_get_address(const void *svm, const SVM_Code code, const SVM_Value_String label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto etiquette = Outils::valeur<SVM_Memoire::Chaine>(environnement,label,__func__,"label","a string");
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Symbole>(SVM_Valeur::AdresseInstruction(c,c->resolution_symbole(*etiquette))));
}

SVM_Value_Symbol svm_code_symbol_get_address__string(const void *svm, const SVM_Code code, const SVM_String label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto etiquette = Outils::chaine(environnement,label,__func__,"label");
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Symbole>(SVM_Valeur::AdresseInstruction(c,c->resolution_symbole(etiquette))));
}

SVM_Value_Symbol svm_code_symbol_get_address__raw(const void *svm, const SVM_Code code, const char *label)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Symbole>(SVM_Valeur::AdresseInstruction(c,c->resolution_symbole(std::string(label)))));
}

SVM_Size svm_code_get_size(const void *svm, const SVM_Code code)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	return c->taille();
}

SVM_String svm_code_instruction_get_text(const void *svm, const SVM_Code code, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto instruction = c->instruction(address);
	if(not instruction) { throw MauvaisAppelFonction(__func__,"address is out of code valid addresses"); }
	std::ostringstream oss;
	oss << (*instruction);
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_Boolean svm_code_instruction_is_system(const void *svm, const SVM_Code code, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto instruction = c->instruction(address);
	if(not instruction) { throw MauvaisAppelFonction(__func__,"address is out of code valid addresses"); }
	return SVM_Interface::Outils::booleen(instruction->systeme());
}

SVM_Boolean svm_code_instruction_is_waiting(const void *svm, const SVM_Code code, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto instruction = c->instruction(address);
	if(not instruction) { throw MauvaisAppelFonction(__func__,"address is out of code valid addresses"); }
	return SVM_Interface::Outils::booleen(instruction->attente());
}

SVM_String svm_code_instruction_get_location(const void *svm, const SVM_Code code, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	auto instruction = c->instruction(address);
	if(not instruction) { throw MauvaisAppelFonction(__func__,"address is out of code valid addresses"); }
	std::ostringstream oss;
	oss << instruction->position();
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_AccessControl svm_accesscontrol_new(const void *svm, const unsigned long int *max_instructions, const unsigned long int *max_memory)
{
	auto environnement = Outils::environnement(svm,__func__);
	SHARED_PTR(size_t) instr;
	if(max_instructions)
	{
		instr = std::make_shared<size_t>(*max_instructions);
	}
	SHARED_PTR(size_t) mem;
	if(max_memory)
	{
		mem = std::make_shared<size_t>(*max_memory);
	}
	SVM_Noyau::ControleAccesSP ca = std::make_shared<SVM_Noyau::ControleAcces>(instr,mem);
	return environnement->ajout<SVM_Noyau::ControleAcces>(ca);
}

SVM_Boolean svm_accesscontrol_has_instructions(const void *svm, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,__func__,"access_control");
	return Outils::booleen(static_cast<bool>(controle_acces->_instructions_executees));
}

unsigned long int svm_accesscontrol_get_instructions(const void *svm, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,__func__,"access_control");
	if(not controle_acces->_instructions_executees)
	{
		throw MauvaisAppelFonction(__func__,"access_control has no instructions limit");
	}
	return controle_acces->_instructions_executees->restant();
}

void svm_accesscontrol_set_instructions(const void *svm, SVM_AccessControl access_control, const unsigned long int instructions)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,__func__,"access_control");
	controle_acces->change_instructions(instructions);
}

SVM_Boolean svm_accesscontrol_has_memory(const void *svm, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,__func__,"access_control");
	return Outils::booleen(static_cast<bool>(controle_acces->_memoire_allouee));
}

unsigned long int svm_accesscontrol_get_memory(const void *svm, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,__func__,"access_control");
	if(not controle_acces->_memoire_allouee)
	{
		throw MauvaisAppelFonction(__func__,"access_control has no memory limit");
	}
	return controle_acces->_memoire_allouee->restant();
}

void svm_accesscontrol_set_memory(const void *svm, SVM_AccessControl access_control, const unsigned long int memory)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,__func__,"access_control");
	controle_acces->change_memoire(memory);
}

SVM_Kernel svm_kernel_new_code(const void *svm, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control, const SVM_Code code)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto c = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,__func__,"code");
	SVM_Noyau::NoyauSP noyau_courant;
	if(environnement->_variable_noyau)
	{
		noyau_courant = Outils::noyau(environnement,environnement->_variable_noyau,__func__,"current kernel");
	}
	SVM_Noyau::ControleAccesSP controle_acces;
	if(access_control)
	{
		controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,__func__,"access_control");
	}
	bool mode_protege = false;
	if(static_cast<bool>(noyau_courant))
	{
		controle_acces = SVM_Noyau::ControleAcces::integre(noyau_courant->_controle_acces,controle_acces);
		mode_protege = noyau_courant->mode_protege();
	}
	else
	{
		if(not environnement->_systeme) { throw MauvaisAppelFonction(__func__,"called outside a system instruction"); }
	}
	SVM_Noyau::NoyauSP n = SVM_Noyau::Noyau::creation_noyau(environnement->_machine->_base->_debugueur,transmit_interruptions==SVM_Boolean::TRUE,last_return_is_shutdown==SVM_Boolean::TRUE,mode_protege or protected_mode==SVM_Boolean::TRUE,controle_acces,SVM_Valeur::AdresseInstruction(c,0));
	return environnement->ajout(n);
}

SVM_Kernel svm_kernel_new_symbol(const void *svm, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control, const SVM_Value_Symbol symbol)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto symbole = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	SVM_Noyau::NoyauSP noyau_courant;
	if(environnement->_variable_noyau)
	{
		noyau_courant = Outils::noyau(environnement,environnement->_variable_noyau,__func__,"current kernel");
	}
	SVM_Noyau::ControleAccesSP controle_acces;
	if(access_control)
	{
		controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,__func__,"access_control");
	}
	bool mode_protege = false;
	if(static_cast<bool>(noyau_courant))
	{
		controle_acces = SVM_Noyau::ControleAcces::integre(noyau_courant->_controle_acces,controle_acces);
		mode_protege = noyau_courant->mode_protege();
	}
	else
	{
		if(not environnement->_systeme) { throw MauvaisAppelFonction(__func__,"called outside a system instruction"); }
	}
	SVM_Noyau::NoyauSP n = SVM_Noyau::Noyau::creation_noyau(environnement->_machine->_base->_debugueur,transmit_interruptions==SVM_Boolean::TRUE,last_return_is_shutdown==SVM_Boolean::TRUE,mode_protege or protected_mode==SVM_Boolean::TRUE,controle_acces,*symbole);
	return environnement->ajout(n);
}

SVM_Boolean svm_kernel_is_runnable(const void* svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__);
	return Outils::booleen(not noyau->termine());
}

SVM_Boolean svm_kernel_is_terminated(const void* svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__);
	return Outils::booleen(noyau->termine());
}

SVM_Kernel_State svm_kernel_get_state(const void* svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__);
	switch(noyau->etat_global())
	{
		case SVM_Noyau::Etat::Execution::EXECUTION: return RUNNING; break;
		case SVM_Noyau::Etat::Execution::SUSPENDU: return SUSPENDED; break;
		case SVM_Noyau::Etat::Execution::ATTENTE: return WAITING; break;
		case SVM_Noyau::Etat::Execution::INVESTIGATION: return DEBUG; break;
		case SVM_Noyau::Etat::Execution::ZOMBIE: return ZOMBIE; break;
		case SVM_Noyau::Etat::Execution::INTERROMPU: return INTERRUPTED; break;
		default:
		case SVM_Noyau::Etat::Execution::ERREUR: return ERROR; break;
	}
}

SVM_Boolean svm_kernel_has_transmitinterruption(const void* svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__);
	return Outils::booleen(noyau->transmet_interruptions());
}

SVM_Boolean svm_kernel_has_lastreturnisshutdown(const void* svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__);
	return Outils::booleen(noyau->retour_est_arret());
}

SVM_Boolean svm_kernel_has_protectedmode(const void* svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__);
	return Outils::booleen(noyau->mode_protege());
}

SVM_AccessControl svm_kernel_get_accesscontrol(const void* svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__);
	SVM_Noyau::ControleAccesSP ca = noyau->controle_acces();
	if(not ca)
	{
		return nullptr;
	}
	return environnement->ajout<SVM_Noyau::ControleAcces>(ca);
}

void svm_kernel_set_accesscontrol(const void* svm, const SVM_Kernel kernel, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__);
	auto controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,__func__,"access_control");
	noyau->integre(controle_acces);
}

void svm_kernel_suspend(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau_libre(environnement,kernel,__func__,"kernel");
	noyau->suspension_demandee();
}

void svm_kernel_terminate(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau_libre(environnement,kernel,__func__,"kernel");
	noyau->terminaison_demandee();
}

void svm_kernel_swap_memory(const void *svm, SVM_Kernel kernel1, SVM_Kernel kernel2)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau1 = Outils::noyau(environnement,kernel1,__func__,"kernel1");
	SVM_Noyau::NoyauSP noyau2 = Outils::noyau(environnement,kernel2,__func__,"kernel2");
	noyau1->echange_memoire(noyau2);
}

SVM_Value_Interruption svm_kernel_get_interruption(const void* svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto interruption = noyau->interrompu();
	if(not interruption)
		return nullptr;
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Interruption>(*interruption));
}

SVM_Value_Interruption svm_process_get_interruption(const void* svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	auto interruption = processus->interrompu();
	if(not interruption)
		return nullptr;
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Interruption>(*interruption));
}

SVM_String svm_kernel_print(const void* svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	std::ostringstream oss;
	noyau->trace_etat(oss);
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_String svm_kernel_get_coredump(const void* svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	std::ostringstream oss;
	oss << (*noyau);
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_Code svm_processor_get_currentcode(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return environnement->ajout(noyau->_processeur->code_courant());
}

SVM_Process_Lock svm_process_ownership_lock(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_attente) { throw MauvaisAppelFonction(__func__,"called outside a waiting instruction"); }
	SVM_Processus::ProcessusSP processus = Outils::processus_libre(environnement,process,__func__);
	return VerrouProcessus::verrouillage(environnement,processus);
}

SVM_Process_Lock svm_process_ownership_lock_critical(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not (environnement->_systeme and environnement->_attente)) { throw MauvaisAppelFonction(__func__,"called outside a system waiting instruction"); }
	return VerrouProcessus::verrouillage_total(environnement);
}

SVM_Boolean svm_process_ownership_check(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	return Outils::verifie_verrou(environnement,processus,__func__,"process");
}

SVM_Boolean svm_process_ownership_get_local(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_attente) { throw MauvaisAppelFonction(__func__,"called outside a waiting instruction"); }
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	VerrouProcessus::verrouillage(environnement,processus);
	return Outils::verifie_verrou(environnement,processus,__func__,"process");
}

SVM_Boolean svm_process_kernel_attach(const void *svm,  SVM_Process process, const SVM_Kernel kernel, const SVM_Size argc,  SVM_Parameter argv[])
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not kernel) { throw MauvaisAppelFonction(__func__,"kernel is void"); }
	SVM_Noyau::NoyauSP n = Outils::noyau(environnement,kernel,__func__);
	if(not n) { throw MauvaisAppelFonction(__func__,"kernel is invalid"); }
	auto processus = Outils::processus(environnement,process,__func__);
	return processus->_sequenceur->attache_noyau(n,environnement,argc,argv)?TRUE:FALSE;
}

SVM_Boolean svm_process_kernel_detach(const void *svm,  SVM_Process process, const SVM_Kernel kernel, const SVM_Size argc,  SVM_Parameter argv[])
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not kernel) { throw MauvaisAppelFonction(__func__,"kernel is void"); }
	SVM_Noyau::NoyauSP n = Outils::noyau(environnement,kernel,__func__);
	if(not n) { throw MauvaisAppelFonction(__func__,"kernel is invalid"); }
	auto processus = Outils::processus(environnement,process,__func__);
	return processus->_sequenceur->detache_noyau(n,environnement,argc,argv)?TRUE:FALSE;
}

SVM_Kernel svm_process_kernel_get_current(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Processus::ProcessusSP processus = Outils::processus(environnement,process,__func__);
	auto noyau = processus->_noyau_courant;
	if(not noyau)
		return nullptr;
	return environnement->ajout(noyau);
}

void svm_memory_synchronisation_enable(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	noyau->_memoire->active_synchronisation(*pointeur);
}

void svm_memory_synchronisation_disable(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	noyau->_memoire->desactive_synchronisation(*pointeur);
}

SVM_Lock svm_lock_new(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Synchronisation::AccesSP acces = std::make_shared<SVM_Synchronisation::Acces>(true);
	auto verrou = environnement->ajout(acces);
	environnement->globale(verrou);
	return verrou;
}

SVM_LockGuard_Read svm_lock_readguard_new(const void *svm,  SVM_Lock lock, const SVM_Boolean soft)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto verrou = Outils::valeur_typee<SVM_Synchronisation::Acces>(environnement,lock,__func__,"lock");
	SVM_Processus::ProcessusSP p;
	bool attente = Outils::pause_processus(environnement,soft,p,__func__);
	SHARED_PTR(SVM_Synchronisation::LecteurUP) lecteur = std::make_shared<SVM_Synchronisation::LecteurUP>(verrou->lecteur());
	Outils::reprise_processus(environnement,attente,p,__func__);
	return environnement->ajout(lecteur);
}

SVM_LockGuard_Write svm_lock_writeguard_new(const void *svm,  SVM_Lock lock, const SVM_Boolean soft)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto verrou = Outils::valeur_typee<SVM_Synchronisation::Acces>(environnement,lock,__func__,"lock");
	SVM_Processus::ProcessusSP p;
	bool attente = Outils::pause_processus(environnement,soft,p,__func__);
	SHARED_PTR(SVM_Synchronisation::EcrivainUP) ecrivain = std::make_shared<SVM_Synchronisation::EcrivainUP>(verrou->ecrivain());
	Outils::reprise_processus(environnement,attente,p,__func__);
	return environnement->ajout(ecrivain);
}

SVM_Process svm_process_new_interne(SVM_Interface::EnvironnementSP& environnement, const std::string& fonction, const std::string nom, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Kernel kernel)
{
	SVM_Valeur::PointEntreeExtensionSP sequenceur;
	if(sequencer)
	{
		auto s = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,sequencer,fonction,"sequencer","plugin entry point");
		sequenceur = std::make_shared<SVM_Valeur::PointEntreeExtension>(*s);
	}
	auto noyau = Outils::valeur_typee<SVM_Noyau::Noyau>(environnement,kernel,fonction,"kernel");
	SVM_Noyau::NoyauSP noyau_courant;
	if(environnement->_variable_noyau)
	{
		noyau_courant = Outils::noyau(environnement,environnement->_variable_noyau,__func__,"current kernel");
	}
	if(not static_cast<bool>(noyau_courant) and not environnement->_systeme) { throw MauvaisAppelFonction(fonction,"called outside a system instruction"); }
	SVM_Processus::ProcessusSP processus = SVM_Processus::Processus::creation_processus(nom,environnement->_machine->_systeme,environnement->_machine->_base,sequenceur,auto_terminated==TRUE,noyau);
	return environnement->ajout_avatar(std::make_shared<VariableProcessus>(processus,false));
}

SVM_Process svm_process_new(const void *svm, const SVM_Value_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::Chaine>(environnement,name,__func__,"name","a string");
	return svm_process_new_interne(environnement,__func__,*nom,sequencer,auto_terminated,kernel);
}

SVM_Process svm_process_new__string(const void *svm, const SVM_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::chaine(environnement,name,__func__,"name");
	return svm_process_new_interne(environnement,__func__,nom,sequencer,auto_terminated,kernel);
}

SVM_Process svm_process_new__raw(const void *svm, const char *name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not name) { throw MauvaisAppelFonction(__func__,"name is void"); }
	std::string nom(name);
	return svm_process_new_interne(environnement,__func__,nom,sequencer,auto_terminated,kernel);
}

SVM_Process svm_process_new_code_interne(SVM_Interface::EnvironnementSP& environnement, const std::string& fonction, const std::string nom, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Code code, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control)
{
	SVM_Valeur::PointEntreeExtensionSP sequenceur;
	if(sequencer)
	{
		auto s = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,sequencer,fonction,"sequencer","plugin entry point");
		sequenceur = std::make_shared<SVM_Valeur::PointEntreeExtension>(*s);
	}
	auto source = Outils::valeur_typee<SVM_Valeur::Code>(environnement,code,fonction,"code");
	SVM_Noyau::NoyauSP noyau_courant;
	if(environnement->_variable_noyau)
	{
		noyau_courant = Outils::noyau(environnement,environnement->_variable_noyau,__func__,"current kernel");
	}
	SVM_Noyau::ControleAccesSP controle_acces;
	if(access_control)
	{
		controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,fonction,"access_control");
	}
	bool mode_protege = false;
	if(static_cast<bool>(noyau_courant))
	{
		controle_acces = SVM_Noyau::ControleAcces::integre(noyau_courant->_controle_acces,controle_acces);
		mode_protege = noyau_courant->mode_protege();
	}
	else
	{
		if(not environnement->_systeme) { throw MauvaisAppelFonction(fonction,"called outside a system instruction"); }
	}
	SVM_Memoire::MemoireSP memoire;// = SVM_Memoire::Memoire::creation_memoire(debugueur,controle_acces);
	SVM_Processus::ProcessusSP processus = SVM_Processus::Processus::creation_processus(nom,environnement->_machine->_systeme,environnement->_machine->_base,sequenceur,auto_terminated==TRUE,source,memoire,transmit_interruptions==TRUE,last_return_is_shutdown==TRUE,mode_protege or (protected_mode==TRUE),controle_acces);
	return environnement->ajout_avatar(std::make_shared<VariableProcessus>(processus,false));
}

SVM_Process svm_process_new_code(const void *svm, const SVM_Value_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Code code, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::Chaine>(environnement,name,__func__,"name","a string");
	return svm_process_new_code_interne(environnement,__func__,*nom,sequencer,auto_terminated,code,transmit_interruptions,last_return_is_shutdown,protected_mode,access_control);
}

SVM_Process svm_process_new_code__string(const void *svm, const SVM_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Code code, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::chaine(environnement,name,__func__,"name");
	return svm_process_new_code_interne(environnement,__func__,nom,sequencer,auto_terminated,code,transmit_interruptions,last_return_is_shutdown,protected_mode,access_control);
}

SVM_Process svm_process_new_code__raw(const void *svm, const char *name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Code code, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not name) { throw MauvaisAppelFonction(__func__,"name is void"); }
	std::string nom(name);
	return svm_process_new_code_interne(environnement,__func__,nom,sequencer,auto_terminated,code,transmit_interruptions,last_return_is_shutdown,protected_mode,access_control);
}

SVM_Process svm_process_new_symbol_interne(SVM_Interface::EnvironnementSP& environnement, const std::string& fonction, const std::string nom, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Value_Symbol symbol, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control)
{
	SVM_Valeur::PointEntreeExtensionSP sequenceur;
	if(sequencer)
	{
		auto s = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,sequencer,fonction,"sequencer","plugin entry point");
		sequenceur = std::make_shared<SVM_Valeur::PointEntreeExtension>(*s);
	}
	auto symbole = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,fonction,"symbol","a symbol");
	SVM_Noyau::NoyauSP noyau_courant;
	if(environnement->_variable_noyau)
	{
		noyau_courant = Outils::noyau(environnement,environnement->_variable_noyau,__func__,"current kernel");
	}
	SVM_Noyau::ControleAccesSP controle_acces;
	if(access_control)
	{
		controle_acces = Outils::valeur_typee<SVM_Noyau::ControleAcces>(environnement,access_control,fonction,"access_control");
	}
	bool mode_protege = false;
	if(static_cast<bool>(noyau_courant))
	{
		controle_acces = SVM_Noyau::ControleAcces::integre(noyau_courant->_controle_acces,controle_acces);
		mode_protege = noyau_courant->mode_protege();
	}
	else
	{
		if(not environnement->_systeme) { throw MauvaisAppelFonction(fonction,"called outside a system instruction"); }
	}
	SVM_Memoire::MemoireSP memoire;// = SVM_Memoire::Memoire::creation_memoire(debugueur,controle_acces);
	SVM_Processus::ProcessusSP processus = SVM_Processus::Processus::creation_processus(nom,environnement->_machine->_systeme,environnement->_machine->_base,sequenceur,auto_terminated==TRUE,*symbole,memoire,transmit_interruptions==TRUE,last_return_is_shutdown==TRUE,mode_protege or(protected_mode==TRUE),controle_acces);
	return environnement->ajout_avatar(std::make_shared<VariableProcessus>(processus,false));
}

SVM_Process svm_process_new_symbol(const void *svm, const SVM_Value_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Value_Symbol symbol, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::valeur<SVM_Memoire::Chaine>(environnement,name,__func__,"name","a string");
	return svm_process_new_symbol_interne(environnement,__func__,*nom,sequencer,auto_terminated,symbol,transmit_interruptions,last_return_is_shutdown,protected_mode,access_control);
}

SVM_Process svm_process_new_symbol__string(const void *svm, const SVM_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Value_Symbol symbol, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto nom = Outils::chaine(environnement,name,__func__,"name");
	return svm_process_new_symbol_interne(environnement,__func__,nom,sequencer,auto_terminated,symbol,transmit_interruptions,last_return_is_shutdown,protected_mode,access_control);
}

SVM_Process svm_process_new_symbol__raw(const void *svm, const char *name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Value_Symbol symbol, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not name) { throw MauvaisAppelFonction(__func__,"name is void"); }
	std::string nom(name);
	return svm_process_new_symbol_interne(environnement,__func__,nom,sequencer,auto_terminated,symbol,transmit_interruptions,last_return_is_shutdown,protected_mode,access_control);
}

void svm_process_run(const void *svm, const SVM_Process process, const SVM_Value_Integer delay)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_variable_processus) { throw MauvaisAppelFonction(__func__,"a process can not be triggered from another process"); }
	auto contexte = Outils::adresse(environnement,__func__);
	auto p = Outils::processus_libre(environnement,process,__func__,"process");
	auto delai = Outils::valeur<SVM_Memoire::Entier>(environnement,delay,__func__,"delay","an integer");
	if(delai<0) { throw MauvaisAppelFonction(__func__,"delay can not be negative"); }
	p->execution_demandee(contexte,*delai);
}

void svm_process_run__raw(const void *svm, const SVM_Process process, const unsigned long int delay)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(environnement->_variable_processus) { throw MauvaisAppelFonction(__func__,"a process can not be triggered from another process"); }
	auto contexte = Outils::adresse(environnement,__func__);
	auto p = Outils::processus_libre(environnement,process,__func__,"process");
	p->execution_demandee(contexte,delay);
}

void svm_process_suspend(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto contexte = Outils::adresse(environnement,__func__);
	auto p = Outils::processus_libre(environnement,process,__func__);
	p->suspension_demandee(contexte);
}

void svm_process_terminate(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto contexte = Outils::adresse(environnement,__func__);
	auto p = Outils::processus_libre(environnement,process,__func__);
	p->terminaison_demandee(contexte);
}

void svm_process_pause(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_attente) { throw MauvaisAppelFonction(__func__,"called outside a waiting instruction"); }
	auto p = Outils::processus(environnement,process,__func__);
	if(not p->entree_mode_attente()) { throw MauvaisAppelFonction(__func__,"unable to change process state"); }
}

void svm_process_resume(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_attente) { throw MauvaisAppelFonction(__func__,"called outside a waiting instruction"); }
	auto p = Outils::processus(environnement,process,__func__);
	if(not p->sortie_mode_attente()) { throw MauvaisAppelFonction(__func__,"unable to change process state"); }
}

void svm_process_interruptionnotification_enable(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_attente) { throw MauvaisAppelFonction(__func__,"called outside a waiting instruction"); }
	auto p = Outils::processus(environnement,process,__func__);
	p->accepte_notification_interruption();
}

void svm_process_interruptionnotification_disable(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_attente) { throw MauvaisAppelFonction(__func__,"called outside a waiting instruction"); }
	auto p = Outils::processus(environnement,process,__func__);
	p->ignore_notification_interruption();
}

void svm_process_interruptionnotification_wait(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus_courant = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
	if(not environnement->_attente) { throw MauvaisAppelFonction(__func__,"called outside a waiting instruction"); }
	if(not processus_courant->entree_mode_attente()) { throw MauvaisAppelFonction(__func__,"unable to change current process state"); }
	processus_courant->attend_notification_interruption();
	if(not processus_courant->sortie_mode_attente()) { throw MauvaisAppelFonction(__func__,"unable to change current process state"); }
}

void svm_process_interruptionnotification_send(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = Outils::processus_libre(environnement,process,__func__);
	p->notification_interruption();
}

SVM_Boolean svm_scheduler_process_attach(const void *svm, const SVM_Scheduler scheduler, const SVM_Process process, const SVM_Value_Integer parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto ordonnanceur = Outils::valeur_typee<SVM_Ordonnanceur::Ordonnanceur>(environnement,scheduler,__func__,"scheduler");
	auto processus = Outils::processus(environnement,process,__func__,"process");
	auto parametre = Outils::valeur<SVM_Memoire::Entier>(environnement,parameter,__func__,"parameter","an integer");
	if((*parametre)<0) { throw MauvaisAppelFonction(__func__,"parameter can not be negative"); }
	if(environnement->_variable_processus)
	{
		auto processus_courant = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
		if(not processus_courant) { throw MauvaisAppelFonction(__func__,"called outside a process"); }
		ordonnanceur->attache(Outils::adresse(environnement,__func__),processus,*parametre);
		return Outils::booleen(processus_courant->attente_reponse(Outils::adresse(ordonnanceur))==SVM_Systeme::Evenement::Reponse::OUI);
	}
	ordonnanceur->attache(Outils::adresse(environnement,__func__),processus,*parametre);
	return Outils::booleen(environnement->_machine->_systeme->attente_reponse(Outils::adresse(ordonnanceur))==SVM_Systeme::Evenement::Reponse::OUI);
}

SVM_Boolean svm_scheduler_process_attach__raw(const void *svm, const SVM_Scheduler scheduler, const SVM_Process process, const unsigned long int parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto ordonnanceur = Outils::valeur_typee<SVM_Ordonnanceur::Ordonnanceur>(environnement,scheduler,__func__,"scheduler");
	auto processus = Outils::processus(environnement,process,__func__,"process");
	if(environnement->_variable_processus)
	{
		auto processus_courant = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
		if(not processus_courant) { throw MauvaisAppelFonction(__func__,"called outside a process"); }
		ordonnanceur->attache(Outils::adresse(environnement,__func__),processus,parameter);
		return Outils::booleen(processus_courant->attente_reponse(Outils::adresse(ordonnanceur))==SVM_Systeme::Evenement::Reponse::OUI);
	}
	ordonnanceur->attache(Outils::adresse(environnement,__func__),processus,parameter);
	return Outils::booleen(environnement->_machine->_systeme->attente_reponse(Outils::adresse(ordonnanceur))==SVM_Systeme::Evenement::Reponse::OUI);
}

SVM_Boolean svm_scheduler_process_detach(const void *svm, const SVM_Scheduler scheduler, const SVM_Process process, const SVM_Value_Integer parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto ordonnanceur = Outils::valeur_typee<SVM_Ordonnanceur::Ordonnanceur>(environnement,scheduler,__func__,"scheduler");
	auto processus = Outils::processus(environnement,process,__func__,"process");
	auto parametre = Outils::valeur<SVM_Memoire::Entier>(environnement,parameter,__func__,"parameter","an integer");
	if((*parametre)<0) { throw MauvaisAppelFonction(__func__,"parameter can not be negative"); }
	if(environnement->_variable_processus)
	{
		auto processus_courant = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
		if(not processus_courant) { throw MauvaisAppelFonction(__func__,"called outside a process"); }
		ordonnanceur->detache(Outils::adresse(environnement,__func__),processus,*parametre);
		return Outils::booleen(processus_courant->attente_reponse(Outils::adresse(ordonnanceur))==SVM_Systeme::Evenement::Reponse::OUI);
	}
	ordonnanceur->detache(Outils::adresse(environnement,__func__),processus,*parametre);
	return Outils::booleen(environnement->_machine->_systeme->attente_reponse(Outils::adresse(ordonnanceur))==SVM_Systeme::Evenement::Reponse::OUI);
}

SVM_Boolean svm_scheduler_process_detach__raw(const void *svm, const SVM_Scheduler scheduler, const SVM_Process process, const unsigned long int parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto ordonnanceur = Outils::valeur_typee<SVM_Ordonnanceur::Ordonnanceur>(environnement,scheduler,__func__,"scheduler");
	auto processus = Outils::processus(environnement,process,__func__,"process");
	if(environnement->_variable_processus)
	{
		auto processus_courant = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
		if(not processus_courant) { throw MauvaisAppelFonction(__func__,"called outside a process"); }
		ordonnanceur->detache(Outils::adresse(environnement,__func__),processus,parameter);
		return Outils::booleen(processus_courant->attente_reponse(Outils::adresse(ordonnanceur))==SVM_Systeme::Evenement::Reponse::OUI);
	}
	ordonnanceur->detache(Outils::adresse(environnement,__func__),processus,parameter);
	return Outils::booleen(environnement->_machine->_systeme->attente_reponse(Outils::adresse(ordonnanceur))==SVM_Systeme::Evenement::Reponse::OUI);
}

SVM_String svm_process_get_name(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	return ::svm_string_new(svm,processus->_nom.c_str(),processus->_nom.size());
}

SVM_Process_State svm_process_get_state(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	switch(processus->etat_global())
	{
		case SVM_Processus::Etat::Execution::EXECUTION: return RUNNING; break;
		case SVM_Processus::Etat::Execution::SUSPENDU: return SUSPENDED; break;
		case SVM_Processus::Etat::Execution::ATTENTE: return WAITING; break;
		case SVM_Processus::Etat::Execution::BLOQUE: return LOCKED; break;
		case SVM_Processus::Etat::Execution::INVESTIGATION: return DEBUG; break;
		case SVM_Processus::Etat::Execution::ZOMBIE: return ZOMBIE; break;
		case SVM_Processus::Etat::Execution::INTERROMPU: return INTERRUPTED; break;
		default:
		case SVM_Processus::Etat::Execution::ERREUR: return ERROR; break;
	}
}

SVM_String svm_process_get_coredump(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	std::ostringstream oss;
	oss << *processus;
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_String svm_process_print(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	std::ostringstream oss;
	processus->status(oss);
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_Boolean svm_process_has_scheduler(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	auto ordonnanceur = Outils::ordonnanceur(processus);
	return Outils::booleen(static_cast<bool>(ordonnanceur));
}

SVM_Scheduler svm_process_get_scheduler(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	auto ordonnanceur = Outils::ordonnanceur(processus);
	if(not ordonnanceur) { throw MauvaisAppelFonction(__func__,"process is not attached to a scheduler"); }
	return environnement->ajout(ordonnanceur);
}

SVM_Value_PluginEntryPoint svm_process_get_sequencer(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	auto nom = processus->_sequenceur->nom();
	if(not nom) { return nullptr; }
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::PointEntreeExtension>(*nom));
}

void* svm_process_sequencer_get_internal(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	return processus->_sequenceur->interne();
}

SVM_String svm_process_sequencer_print(const void *svm, const SVM_Process process)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto processus = Outils::processus_libre(environnement,process,__func__,"process");
	std::ostringstream oss;
	oss << processus->_sequenceur;
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_Scheduler svm_scheduler_get(const void *svm, const SVM_Value_PluginEntryPoint name)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Valeur::PointEntreeExtensionSP nom;
	if(name)
	{
		auto n= Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,name,__func__,"name","plugin entry point");
		nom = std::make_shared<SVM_Valeur::PointEntreeExtension>(*n);
	}
	auto ordonnanceur = environnement->_machine->_systeme->ordonnanceur(nom);
	if(not ordonnanceur) { throw MauvaisAppelFonction(__func__,"unknown scheduler"); }
	return environnement->ajout(ordonnanceur);
}

void* svm_scheduler_get_internal(const void *svm, const SVM_Scheduler scheduler)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto ordonnanceur = Outils::ordonnanceur(environnement,scheduler,__func__,"scheduler");
	return ordonnanceur->interne();
}

SVM_String svm_scheduler_print(const void *svm, const SVM_Scheduler scheduler)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto ordonnanceur = Outils::ordonnanceur(environnement,scheduler,__func__,"scheduler");
	std::ostringstream oss;
	oss << *ordonnanceur;
	return svm_string_new__raw(svm,oss.str().c_str());
}

void svm_scheduler_notify(const void *svm, const SVM_Scheduler scheduler, const SVM_Value_Integer parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto ordonnanceur = Outils::ordonnanceur(environnement,scheduler,__func__,"scheduler");
	SVM_Synchronisation::AdresseSP source = Outils::adresse(environnement,__func__);
	auto parametre = Outils::valeur<SVM_Memoire::Entier>(environnement,parameter,__func__,"parameter","an integer");
	if((*parametre)<0) { throw MauvaisAppelFonction(__func__,"parameter can not be negative"); }
	ordonnanceur->notifie(source,*parametre);
}

void svm_scheduler_notify__raw(const void *svm, const SVM_Scheduler scheduler, const unsigned long int parameter)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto ordonnanceur = Outils::ordonnanceur(environnement,scheduler,__func__,"scheduler");
	SVM_Synchronisation::AdresseSP source = Outils::adresse(environnement,__func__);
	ordonnanceur->notifie(source,parameter);
}

SVM_Event_Queue svm_event_queue_new(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return environnement->ajout(std::make_shared<SVM_Interface::QueueEvenements>());
}

SVM_Event_Queue_Address svm_event_address_new(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return environnement->ajout(std::make_shared<StructureAdresse>());
}

SVM_Event_Queue_Address svm_event_address_new_struct(const void *svm, const SVM_Structure structure)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto valeur_structure = Outils::valeur_typee<SVM_Interface::Structure>(environnement,structure,__func__,"structure");
	return environnement->ajout(std::make_shared<StructureAdresse>(valeur_structure));
}

SVM_Boolean svm_event_address_has_struct(const void *svm, SVM_Event_Queue_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto adresse = Outils::valeur_typee<StructureAdresse>(environnement,address,__func__,"address");
	return Outils::booleen(static_cast<bool>(adresse->_structure));
}

SVM_Structure svm_event_address_get_struct(const void *svm, SVM_Event_Queue_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto adresse = Outils::valeur_typee<StructureAdresse>(environnement,address,__func__,"address");
	if(not adresse->_structure)
	{
		throw MauvaisAppelFonction(__func__,"event queue address not linked to a structure");
	}
	return environnement->ajout(adresse->_structure);
}

void svm_event_queue_join(const void *svm,  SVM_Event_Queue event_queue, const SVM_Event_Queue_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto queue = Outils::valeur_typee<SVM_Interface::QueueEvenements>(environnement,event_queue,__func__,"event_queue");
	auto adresse = Outils::valeur_typee<StructureAdresse>(environnement,address,__func__,"address");
	queue->creation_boite(adresse);
}

void svm_event_queue_leave(const void *svm,  SVM_Event_Queue event_queue, const SVM_Event_Queue_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto queue = Outils::valeur_typee<SVM_Interface::QueueEvenements>(environnement,event_queue,__func__,"event_queue");
	auto adresse = Outils::valeur_typee<StructureAdresse>(environnement,address,__func__,"address");
	queue->destruction_boite(adresse);
}

void svm_event_queue_push(const void *svm,  SVM_Event_Queue event_queue, const SVM_Event_Queue_Address destination, const SVM_Event_Queue_Address origin, const SVM_Structure event)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto queue = Outils::valeur_typee<SVM_Interface::QueueEvenements>(environnement,event_queue,__func__,"event_queue");
	auto dest = Outils::valeur_typee<StructureAdresse>(environnement,destination,__func__,"destination");
	auto src = Outils::valeur_typee<StructureAdresse>(environnement,origin,__func__,"origin");
	auto evenement = Outils::valeur_typee<SVM_Interface::Structure>(environnement,event,__func__,"event");
	queue->poste(dest,src,evenement);
}

void svm_event_queue_broadcast(const void *svm, SVM_Event_Queue event_queue, const SVM_Event_Queue_Address origin, const SVM_Structure event)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto queue = Outils::valeur_typee<SVM_Interface::QueueEvenements>(environnement,event_queue,__func__,"event_queue");
	auto src = Outils::valeur_typee<StructureAdresse>(environnement,origin,__func__,"origin");
	auto adresses = queue->liste_adresses();
	for(const auto& a: adresses)
	{
		auto evenement = Outils::valeur_typee<SVM_Interface::Structure>(environnement,event,__func__,"event");
		queue->poste(a,src,evenement);
	}
}

SVM_Boolean svm_event_queue_pull(const void *svm,  SVM_Event_Queue event_queue, const SVM_Event_Queue_Address destination,  SVM_Event_Queue_Address *origin,  SVM_Structure *event, const SVM_Boolean soft)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto queue = Outils::valeur_typee<SVM_Interface::QueueEvenements>(environnement,event_queue,__func__,"event_queue");
	auto dest = Outils::valeur_typee<StructureAdresse>(environnement,destination,__func__,"destination");
	SVM_Synchronisation::AdresseSP src;
	SVM_Interface::StructureSP evenement;
	SVM_Processus::ProcessusSP p;
	auto processus_courant = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
	bool attente = Outils::pause_processus(environnement,soft,p,__func__);
	bool r;
	if(not processus_courant)
	{
		r = queue->recupere(dest,src,evenement);
	}
	else
	{
		r = queue->recupere_notification(dest,src,evenement,processus_courant->_notification_recue);
	}
	Outils::reprise_processus(environnement,attente,p,__func__);
	StructureAdresseSP vraie_src = std::dynamic_pointer_cast<StructureAdresse>(src);
	*origin = environnement->ajout(vraie_src);
	*event = environnement->ajout(evenement);
	return Outils::booleen(r);
}

SVM_Boolean svm_event_queue_check(const void *svm,  SVM_Event_Queue event_queue, const SVM_Event_Queue_Address destination,  SVM_Event_Queue_Address *origin,  SVM_Structure *event, const SVM_Value_Integer timeout, const SVM_Boolean soft)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto queue = Outils::valeur_typee<SVM_Interface::QueueEvenements>(environnement,event_queue,__func__,"event_queue");
	auto dest = Outils::valeur_typee<StructureAdresse>(environnement,destination,__func__,"destination");
	auto delai = Outils::valeur<SVM_Memoire::Entier>(environnement,timeout,__func__,"timeout","an integer");
	if((*delai)<0) { throw MauvaisAppelFonction(__func__,"timeout can not be negative"); }
	SVM_Synchronisation::AdresseSP src;
	SVM_Interface::StructureSP evenement;
	auto processus_courant = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
	SVM_Processus::ProcessusSP p;
	bool attente = Outils::pause_processus(environnement,soft,p,__func__);
	bool r;
	if(not processus_courant)
	{
		r = queue->recupere_echeance(dest,src,evenement,static_cast<size_t>(*delai));
	}
	else
	{
		r = queue->recupere_echeance_notification(dest,src,evenement,processus_courant->_notification_recue,static_cast<size_t>(*delai));
	}
	Outils::reprise_processus(environnement,attente,p,__func__);
	StructureAdresseSP vraie_src = std::dynamic_pointer_cast<StructureAdresse>(src);
	*origin = environnement->ajout(vraie_src);
	*event = environnement->ajout(evenement);
	return Outils::booleen(r);
}

SVM_Boolean svm_event_queue_check__raw(const void *svm,  SVM_Event_Queue event_queue, const SVM_Event_Queue_Address destination,  SVM_Event_Queue_Address *origin,  SVM_Structure *event, const unsigned long int timeout, const SVM_Boolean soft)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto queue = Outils::valeur_typee<SVM_Interface::QueueEvenements>(environnement,event_queue,__func__,"event_queue");
	auto dest = Outils::valeur_typee<StructureAdresse>(environnement,destination,__func__,"destination");
	SVM_Synchronisation::AdresseSP src;
	SVM_Interface::StructureSP evenement;
	SVM_Processus::ProcessusSP p;
	bool attente = Outils::pause_processus(environnement,soft,p,__func__);
	auto r = queue->recupere_echeance(dest,src,evenement,static_cast<size_t>(timeout));
	Outils::reprise_processus(environnement,attente,p,__func__);
	StructureAdresseSP vraie_src = std::dynamic_pointer_cast<StructureAdresse>(src);
	*origin = environnement->ajout(vraie_src);
	*event = environnement->ajout(evenement);
	return Outils::booleen(r);
}

SVM_Value_Symbol svm_processor_get_nextinstruction(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Symbole>(noyau->_processeur->prochaine_instruction_sans_increment()));
}

SVM_Value_Symbol svm_processor_get_currentinstruction(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Symbole>(noyau->_processeur->instruction_courante()));
}

void svm_processor_jump_local(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_processeur->saut(SVM_Valeur::AdresseInstruction(address));
}

void svm_processor_jump_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto symbole = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	const SVM_Valeur::AdresseInstruction& ps = *symbole;
	noyau->_processeur->saut(ps);
}

void svm_processor_call_local(const void *svm, const SVM_Kernel kernel, const SVM_Address function, const SVM_Value_Pointer parameters)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,parameters,__func__,"parameters","a pointer");
	noyau->_processeur->appel(SVM_Valeur::AdresseInstruction(function),*pointeur);
}

void svm_processor_call_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol function, const SVM_Value_Pointer parameters)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto fonction = Outils::valeur<SVM_Memoire::Symbole>(environnement,function,__func__,"function","a symbol");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,parameters,__func__,"parameters","a pointer");
	noyau->_processeur->appel(*fonction,*pointeur);
}

void svm_processor_call_identical(const void *svm, const SVM_Kernel kernel, const SVM_Boolean transfer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_processeur->appel_identique(transfer==TRUE);
}

void svm_processor_return(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_processeur->retour(*(noyau->_memoire),noyau->retour_est_arret());
}

SVM_Size svm_processor_returnstack_get_size(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return noyau->_processeur->taille_pile();
}

void svm_processor_returnstack_swap_level(const void *svm, const SVM_Kernel kernel, const SVM_Index level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_processeur->echange(level);
}

SVM_Index svm_processor_returnstack_find_flag(const void *svm, const SVM_Kernel kernel, const SVM_Value_String flag, const SVM_Index start)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto drapeau = Outils::valeur<SVM_Memoire::Chaine>(environnement,flag,__func__,"flag","a string");
	return noyau->_processeur->cherche_pile(*drapeau,start);
}

SVM_Index svm_processor_returnstack_find_flag__string(const void *svm, const SVM_Kernel kernel, const SVM_String flag, const SVM_Index start)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto drapeau = Outils::chaine(environnement,flag,__func__,"flag");
	return noyau->_processeur->cherche_pile(drapeau,start);
}

SVM_Index svm_processor_returnstack_find_flag__raw(const void *svm, const SVM_Kernel kernel, const char *flag, const SVM_Index start)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(not flag) { throw MauvaisAppelFonction(__func__,"flag is void"); }
	return noyau->_processeur->cherche_pile(flag,start);
}

void svm_processor_returnstack_move_level(const void *svm, const SVM_Kernel kernel, const SVM_Index start, const SVM_Index end, const SVM_Index target)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(end<start) { throw MauvaisAppelFonction(__func__,"end inferior to start"); }
	return noyau->_processeur->deplace_pile(start,end,target);
}

void svm_processor_current_shutdown(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_variable_noyau) { throw MauvaisAppelFonction(__func__,"called outside an instruction"); }
	throw SVM_Valeur::Arret();
}

void svm_machine_shutdown(const void *svm, const SVM_Value_Integer return_code)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_variable_noyau) { throw MauvaisAppelFonction(__func__,"called outside an instruction"); }
	if(not environnement->_systeme) { throw MauvaisAppelFonctionSecurite(__func__,"called outside a system instruction"); }
	auto entier = Outils::valeur<SVM_Memoire::Entier>(environnement,return_code,__func__,"return_code","an integer");
	if((*entier)<0) { throw MauvaisAppelFonction(__func__,"return_code can not be negative"); }
	throw SVM_Valeur::Arret(*entier);
}

void svm_machine_shutdown__raw(const void *svm, const unsigned long int return_code)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_variable_noyau) { throw MauvaisAppelFonction(__func__,"called outside an instruction"); }
	if(not environnement->_systeme) { throw MauvaisAppelFonctionSecurite(__func__,"called outside a system instruction"); }
	throw SVM_Valeur::Arret(return_code);
}

void svm_machine_trace(const void *svm, const SVM_Value_String trace)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto message = Outils::valeur<SVM_Memoire::Chaine>(environnement,trace,__func__,"trace","a string");
	SVM::Machine::Trace(environnement->_machine->_historique,environnement->nom()) << (*message);
}

void svm_machine_trace__string(const void *svm, const SVM_String trace)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto message = Outils::chaine(environnement,trace,__func__,"trace");
	SVM::Machine::Trace(environnement->_machine->_historique,environnement->nom()) << message;
}

void svm_machine_trace__raw(const void *svm, const char *trace)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not trace) { throw MauvaisAppelFonction(__func__,"trace is void"); }
	SVM::Machine::Trace(environnement->_machine->_historique,environnement->nom()) << trace;
}

SVM_Value_Interruption* svm_machine_list_interruption(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto i = SVM::Machine::Element::Valeur::Interruption::internes();
	auto e = environnement->_machine->_base->_gestionnaire_extensions->externes();
	SVM_Interface::TableauVariablesSP tableau = std::make_shared<SVM_Interface::TableauVariables>("interruptions",i.size()+e.size()+1);
	environnement->ajout_avatar(tableau);
	size_t indice = 0;
	for(const auto& ii: i)
	{
		tableau->_variables[indice++] = Outils::ajout(environnement,std::make_shared<SVM_Memoire::Interruption>(ii));
	}
	for(const auto& ii: e)
	{
		tableau->_variables[indice++] = Outils::ajout(environnement,std::make_shared<SVM_Memoire::Interruption>(ii));
	}
	tableau->_variables[indice] = nullptr;
	return tableau->_variables;
}

SVM_String svm_machine_config(const void *svm, const char *key)
{
	auto environnement = Outils::environnement(svm,__func__);
	try
	{
		auto v = SVM::Global::Installation::valeur(key);
		return ::svm_string_new(svm,v.c_str(),v.size());
	}
	catch(SVM::Global::ClefInvalide& e)
	{
		std::ostringstream oss;
		oss << "Configuration " << key << " key does not exist";
		throw MauvaisAppelFonction(__func__,oss.str());
	}
}

SVM_Boolean svm_machine_mode_is_debug(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto d = environnement->_machine->_base->_debugueur;
	return Outils::booleen(static_cast<bool>(d));
}

SVM_Boolean svm_machine_mode_is_performance(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto p = environnement->_machine->_base->_performance;
	return Outils::booleen(static_cast<bool>(p));
}

void svm_processor_current_sleep(const void *svm, const SVM_Value_Integer seconds, const SVM_Value_Integer milliseconds, const SVM_Boolean soft)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto secondes = Outils::valeur<SVM_Memoire::Entier>(environnement,seconds,__func__,"seconds","an integer");
	auto millisecondes = Outils::valeur<SVM_Memoire::Entier>(environnement,milliseconds,__func__,"milliseconds","an integer");
	if((*secondes)<0) { throw MauvaisAppelFonction(__func__,"seconds can not be negative"); }
	if((*millisecondes)<0) { throw MauvaisAppelFonction(__func__,"milliseconds can not be negative"); }
	SVM_Processus::ProcessusSP p;
	bool attente = Outils::pause_processus(environnement,soft,p,__func__);
	if((*secondes)>0) std::this_thread::sleep_for(std::chrono::seconds(*secondes));
	if((*millisecondes)>0) std::this_thread::sleep_for(std::chrono::milliseconds(*millisecondes));
	Outils::reprise_processus(environnement,attente,p,__func__);
}

void svm_processor_current_sleep__raw(const void *svm, const unsigned long int seconds, const unsigned long int milliseconds, const SVM_Boolean soft)
{
	auto environnement = Outils::environnement(svm,__func__);
	SVM_Processus::ProcessusSP p;
	bool attente = Outils::pause_processus(environnement,soft,p,__func__);
	if((seconds)>0) std::this_thread::sleep_for(std::chrono::seconds(seconds));
	if((milliseconds)>0) std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	Outils::reprise_processus(environnement,attente,p,__func__);
}

void svm_processor_interruptionhandler_set_local(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption, const SVM_Address address, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto interrupt = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	if(level==GLOBAL)
		noyau->_processeur->ajoute_interruption_gestionnaire_global(*interrupt,SVM_Valeur::AdresseInstruction(address),noyau->gestionnaire_interruptions_materielles());
	else
		noyau->_processeur->ajoute_interruption_gestionnaire_local(*interrupt,SVM_Valeur::AdresseInstruction(address),level==CASCADE);
}

void svm_processor_interruptionhandler_set_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption, const SVM_Value_Symbol symbol, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto interrupt = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	auto symbole = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	if(level==GLOBAL)
		noyau->_processeur->ajoute_interruption_gestionnaire_global(*interrupt,*symbole,noyau->gestionnaire_interruptions_materielles());
	else
		noyau->_processeur->ajoute_interruption_gestionnaire_local(*interrupt,*symbole,level==CASCADE);
}

void svm_processor_interruptionhandler_reset(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto interrupt = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	if(level==GLOBAL)
		noyau->_processeur->enleve_interruption_gestionnaire_global(*interrupt,noyau->gestionnaire_interruptions_materielles());
	else
		noyau->_processeur->enleve_interruption_gestionnaire_local(*interrupt,level==CASCADE);
}


SVM_Boolean svm_processor_interruptionhandler_has(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto interrupt = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	if(level==CASCADE) { throw MauvaisAppelFonction(__func__,"level can not be CASCADE"); }
	try
	{
		if(level==GLOBAL)
			noyau->_processeur->interruption_gestionnaire_global(*interrupt);
		else
			noyau->_processeur->interruption_gestionnaire_local(*interrupt);
		return TRUE;
	}
	catch(const SVM_Processeur::GestionnaireNonDefini& g)
	{
		return FALSE;
	}
}

SVM_Value_Symbol svm_processor_interruptionhandler_get(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto interrupt = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	if(level==CASCADE) { throw MauvaisAppelFonction(__func__,"level can not be CASCADE"); }
	if(level==GLOBAL)
	{
		auto gestionnaire = noyau->_processeur->interruption_gestionnaire_global(*interrupt);
		return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Symbole>(gestionnaire));
	}
	else
	{
		auto gestionnaire = noyau->_processeur->interruption_gestionnaire_local(*interrupt);
		return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Symbole>(gestionnaire));
	}
}

void svm_processor_add_interruption(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto interrupt = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	noyau->_processeur->interruption(*interrupt);
}

void svm_processor_clear_interruption(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_processeur->nettoie_interruptions();
}

SVM_Value_Interruption* svm_processor_list_interruption(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto i = noyau->_processeur->liste_interruptions();
	SVM_Interface::TableauVariablesSP tableau = std::make_shared<SVM_Interface::TableauVariables>("interruptions",i.size()+1);
	environnement->ajout_avatar(tableau);
	size_t indice = 0;
	for(const auto& ii: i)
	{
		tableau->_variables[indice++] = Outils::ajout(environnement,std::make_shared<SVM_Memoire::Interruption>(ii));
	}
	tableau->_variables[indice] = nullptr;
	return tableau->_variables;
}

void svm_processor_hold_interruption(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_processeur->utilise_interruptions(false);
}

void svm_processor_release_interruption(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_processeur->utilise_interruptions(true);
}

void svm_processor_current_raise_error(const void *svm, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto interrupt = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	throw static_cast<SVM_Valeur::Interruption>(*interrupt);
}

void svm_processor_current_raise_error_internal__raw(const void *svm, const SVM_Interruption_Internal interruption, const char *message)
{
	auto environnement = Outils::environnement(svm,__func__);
	throw SVM_Valeur::Interruption(Outils::interruption_interne(environnement,interruption,__func__,"interruption"),std::string(message));
}

void svm_processor_current_raise_error_external__raw(const void *svm, const SVM_Value_PluginEntryPoint interruption, const char *message)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,interruption,__func__,"interruption","a plugin entry name");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->interruption(*pep);
	if(not patron) { throw MauvaisAppelFonction(__func__,"name is not a plugin interruption"); }
	throw SVM_Valeur::Interruption(*pep,std::string(message));
}

SVM_Boolean svm_processor_is_hardwareinterrupted(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return Outils::booleen(noyau->_processeur->interruptions_materielles_detectees()>0);
}

SVM_Value_Pointer svm_processor_get_currentpointer(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Pointeur>(noyau->_processeur->memoire_courante()));
}

void svm_processor_set_currentpointer(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	noyau->_processeur->memoire_courante(*pointeur);
}

SVM_Boolean svm_processor_has_flag(const void *svm, const SVM_Kernel kernel, const SVM_Value_String flag)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto drapeau = Outils::valeur<SVM_Memoire::Chaine>(environnement,flag,__func__,"flag","a string");
	return Outils::booleen(noyau->_processeur->teste_drapeau(*drapeau));
}

SVM_Boolean svm_processor_has_flag__string(const void *svm, const SVM_Kernel kernel, const SVM_String flag)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto drapeau = Outils::chaine(environnement,flag,__func__,"flag");
	return Outils::booleen(noyau->_processeur->teste_drapeau(drapeau));
}

SVM_Boolean svm_processor_has_flag__raw(const void *svm, const SVM_Kernel kernel, const char *flag)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(not flag) { throw MauvaisAppelFonction(__func__,"flag is void"); }
	return Outils::booleen(noyau->_processeur->teste_drapeau(std::string(flag)));
}

SVM_Value_String* svm_processor_list_flag(const void *svm, const SVM_Kernel kernel)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return Outils::tableau_chaines(environnement,noyau->_processeur->liste_drapeaux());
}

void svm_processor_set_flag(const void *svm, const SVM_Kernel kernel, const SVM_Value_String flag, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto drapeau = Outils::valeur<SVM_Memoire::Chaine>(environnement,flag,__func__,"flag","a string");
	if(level==GLOBAL) { throw MauvaisAppelFonction(__func__,"level can not be GLOBAL"); }
	noyau->_processeur->ajoute_drapeau(*drapeau,level==CASCADE);
}

void svm_processor_set_flag__string(const void *svm, const SVM_Kernel kernel, const SVM_String flag, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto drapeau = Outils::chaine(environnement,flag,__func__,"flag");
	if(level==GLOBAL) { throw MauvaisAppelFonction(__func__,"level can not be GLOBAL"); }
	noyau->_processeur->ajoute_drapeau(drapeau,level==CASCADE);
}

void svm_processor_set_flag__raw(const void *svm, const SVM_Kernel kernel, const char *flag, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(not flag) { throw MauvaisAppelFonction(__func__,"flag is void"); }
	if(level==GLOBAL) { throw MauvaisAppelFonction(__func__,"level can not be GLOBAL"); }
	noyau->_processeur->ajoute_drapeau(std::string(flag),level==CASCADE);
}

void svm_processor_reset_flag(const void *svm, const SVM_Kernel kernel, const SVM_Value_String flag, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto drapeau = Outils::valeur<SVM_Memoire::Chaine>(environnement,flag,__func__,"flag","a string");
	if(level==GLOBAL) { throw MauvaisAppelFonction(__func__,"level can not be GLOBAL"); }
	noyau->_processeur->enleve_drapeau(*drapeau,level==CASCADE);
}

void svm_processor_reset_flag__string(const void *svm, const SVM_Kernel kernel, const SVM_String flag, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto drapeau = Outils::chaine(environnement,flag,__func__,"flag");
	if(level==GLOBAL) { throw MauvaisAppelFonction(__func__,"level can not be GLOBAL"); }
	noyau->_processeur->enleve_drapeau(drapeau,level==CASCADE);
}

void svm_processor_reset_flag__raw(const void *svm, const SVM_Kernel kernel, const char *flag, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(not flag) { throw MauvaisAppelFonction(__func__,"flag is void"); }
	if(level==GLOBAL) { throw MauvaisAppelFonction(__func__,"level can not be GLOBAL"); }
	noyau->_processeur->enleve_drapeau(std::string(flag),level==CASCADE);
}

void svm_processor_instructionoverride_set_interne(SVM_Interface::EnvironnementSP& environnement, SVM_Noyau::NoyauSP& noyau, const SVM_Valeur::AdresseInstruction& adresse, const SVM_Valeur::PointEntreeExtension& nom, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Object_Level niveau, const std::string& fonction)
{
	std::vector<SVM_Programme::ParametreSP> parametres;
	for(SVM_Size i=0 ; i<argc ; ++i)
	{
		parametres.push_back(std::make_shared<SVM_Programme::ParametreCalcule>(Outils::valeur_typee<SVM_Valeur::Parametre>(environnement,argv[i],"argv","a parameter")));
	}
	auto n = std::make_shared<SVM_Programme::PointEntreeExtension>(nom);
	auto instruction = std::make_shared<SVM::Machine::Element::Processeur::Instruction::Extension>(*environnement->_position,n,parametres);
	SVM_Valeur::Decorateur decorateur(SVM_Valeur::CodeSP(),environnement->_machine,true);
	instruction->decoration(decorateur);
	if(niveau==GLOBAL)
	{
		if(not noyau->_processeur->ajoute_remplacementinstruction_global(adresse,instruction))
		{
			throw MauvaisAppelFonction(fonction,"address already overridden");
		}
	}
	else
	{
		if(not noyau->_processeur->ajoute_remplacementinstruction_local(adresse,instruction,niveau==CASCADE))
		{
			throw MauvaisAppelFonction(fonction,"address already overridden");
		}
	}
}

void svm_processor_instructionoverride_set_local(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Value_PluginEntryPoint name, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	SVM_Valeur::AdresseInstruction a(noyau->_processeur->code_courant(),address);
	auto nom = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,name,__func__,"name","a plugin entry name");
	::svm_processor_instructionoverride_set_interne(environnement,noyau,a,*nom,argc,argv,level,__func__);
}

void svm_processor_instructionoverride_set_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol, const SVM_Value_PluginEntryPoint name, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto valeur = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	auto nom = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,name,__func__,"name","a plugin entry name");
	::svm_processor_instructionoverride_set_interne(environnement,noyau,*valeur,*nom,argc,argv,level,__func__);
}

SVM_Boolean svm_processor_instructionoverride_has_local(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	SVM_Valeur::AdresseInstruction a(noyau->_processeur->code_courant(),address);
	if(level==CASCADE) { throw MauvaisAppelFonction(__func__,"level can not be CASCADE"); }
	if(level==GLOBAL)
	{
		return Outils::booleen(noyau->_processeur->teste_remplacementinstruction_global(a));
	}
	else
	{
		return Outils::booleen(noyau->_processeur->teste_remplacementinstruction_local(a));
	}
}

SVM_Boolean svm_processor_instructionoverride_has_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto valeur = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	if(level==CASCADE) { throw MauvaisAppelFonction(__func__,"level can not be CASCADE"); }
	if(level==GLOBAL)
	{
		return Outils::booleen(noyau->_processeur->teste_remplacementinstruction_global(*valeur));
	}
	else
	{
		return Outils::booleen(noyau->_processeur->teste_remplacementinstruction_local(*valeur));
	}
}

void svm_processor_instructionoverride_reset_local(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	SVM_Valeur::AdresseInstruction a(noyau->_processeur->code_courant(),address);
	if(level==GLOBAL)
		noyau->_processeur->enleve_remplacementinstruction_global(a);
	else
		noyau->_processeur->enleve_remplacementinstruction_local(a,level==CASCADE);
}

void svm_processor_instructionoverride_reset_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol, const SVM_Object_Level level)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto valeur = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	if(level==GLOBAL)
		noyau->_processeur->enleve_remplacementinstruction_global(*valeur);
	else
		noyau->_processeur->enleve_remplacementinstruction_local(*valeur,level==CASCADE);
}

SVM_Memory_Zone svm_memory_zone_new(const void *svm)
{
	auto environnement = Outils::environnement(svm,__func__);
	return environnement->ajout(std::make_shared<SVM_Memoire::DescriptionBloc>());
}

void svm_memory_zone_append(const void *svm,  SVM_Memory_Zone zone, const SVM_Type type, const SVM_Value_Integer times)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	auto entier = Outils::valeur<SVM_Memoire::Entier>(environnement,times,__func__,"times","an integer");
	if((*entier)<0) { throw MauvaisAppelFonction(__func__,"times can not be negative"); }
	z->ajout(*t,*entier);
}

void svm_memory_zone_append__raw(const void *svm,  SVM_Memory_Zone zone, const SVM_Type type, const SVM_Size times)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	z->ajout(*t,times);
}

void svm_memory_zone_append_internal(const void *svm,  SVM_Memory_Zone zone, const SVM_Type_Internal type, const SVM_Value_Integer times)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto entier = Outils::valeur<SVM_Memoire::Entier>(environnement,times,__func__,"times","an integer");
	if((*entier)<0) { throw MauvaisAppelFonction(__func__,"times can not be negative"); }
	z->ajout(SVM_Memoire::Type(Outils::type_interne(environnement,type,__func__,"type")),*entier);
}

void svm_memory_zone_append_internal__raw(const void *svm,  SVM_Memory_Zone zone, const SVM_Type_Internal type, const SVM_Size times)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	z->ajout(SVM_Memoire::Type(Outils::type_interne(environnement,type,__func__,"type")),times);
}

void svm_memory_zone_append_external(const void *svm,  SVM_Memory_Zone zone, const SVM_Value_PluginEntryPoint type, const SVM_Value_Integer times)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type","a plugin entry name");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->type(*pep);
	if(not patron) { throw MauvaisAppelFonction(__func__,"type is not a plugin type"); }
	auto entier = Outils::valeur<SVM_Memoire::Entier>(environnement,times,__func__,"times","an integer");
	if((*entier)<0) { throw MauvaisAppelFonction(__func__,"times can not be negative"); }
	z->ajout(SVM_Memoire::Type(*pep,patron),*entier);
}

void svm_memory_zone_append_external__raw(const void *svm,  SVM_Memory_Zone zone, const SVM_Value_PluginEntryPoint type, const SVM_Size times)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type","a plugin entry name");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->type(*pep);
	if(not patron) { throw MauvaisAppelFonction(__func__,"type is not a plugin type"); }
	z->ajout(SVM_Memoire::Type(*pep,patron),times);
}

void svm_memory_zone_append_memory_zone(const void *svm, SVM_Memory_Zone zone, const SVM_Memory_Zone append)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto s = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,append,__func__,"append");
	z->ajout(*s);
}

void svm_memory_zone_insert_memory_zone(const void *svm, SVM_Memory_Zone zone, const SVM_Index index, const SVM_Memory_Zone insert)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto s = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,insert,__func__,"insert");
	if((index<0) or (index>z->taille())) { throw MauvaisAppelFonction(__func__,"invalid index"); }
	z->insere(index,*s);
}

void svm_memory_zone_remove_memory_zone(const void *svm, SVM_Memory_Zone zone, const SVM_Index index, const SVM_Size size)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	if((index<0) or (index>z->taille())) { throw MauvaisAppelFonction(__func__,"invalid index"); }
	if(((index+size)<0) or ((index+size)>z->taille())) { throw MauvaisAppelFonction(__func__,"invalid size"); }
	z->enleve(index,size);
}

void svm_memory_zone_replace_memory_zone(const void *svm, SVM_Memory_Zone zone, const SVM_Index index, const SVM_Memory_Zone replace)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto s = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,replace,__func__,"replace");
	if((index<0) or (index>z->taille())) { throw MauvaisAppelFonction(__func__,"invalid index"); }
	if(((index+s->taille())<0) or ((index+s->taille())>z->taille())) { throw MauvaisAppelFonction(__func__,"invalid replace size"); }
	z->remplace(index,*s);
}

SVM_Index svm_memory_zone_find_memory_zone(const void *svm, SVM_Memory_Zone zone, const SVM_Index index, const SVM_Memory_Zone find)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto s = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,find,__func__,"find");
	if((index<0) or (index>z->taille())) { throw MauvaisAppelFonction(__func__,"invalid index"); }
	return z->cherche(index,*s);
}

SVM_Size svm_memory_zone_get_size(const void *svm, const SVM_Memory_Zone zone)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	return z->taille();
}

SVM_Type svm_memory_zone_get_type(const void *svm, const SVM_Memory_Zone zone, const SVM_Index index)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	if((index<0) or (index>=z->taille())) { throw MauvaisAppelFonction(__func__,"invalid index"); }
	return environnement->ajout(std::make_shared<SVM_Memoire::Type>(z->element(index)));
}

SVM_Memory_Zone svm_memory_zone_copy(const void *svm, const SVM_Memory_Zone zone)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	return environnement->ajout(std::make_shared<SVM_Memoire::DescriptionBloc>(*z));
}

SVM_String svm_memory_zone_print(const void *svm, const SVM_Memory_Zone zone)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	std::ostringstream oss;
	oss << *z;
	return ::svm_string_new(svm,oss.str().c_str(),oss.str().size());
}

SVM_Value_Pointer svm_memory_allocate(const void *svm, const SVM_Kernel kernel, const SVM_Memory_Zone zone)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto p = noyau->_memoire->allocation(*z);
	noyau->_processeur->memoire_allouee({p});
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Pointeur>(p));
}

SVM_Value_Pointer svm_memory_allocate_address(const void *svm, const SVM_Kernel kernel, const SVM_Memory_Zone zone, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto p = noyau->_memoire->allocation(*z,SVM_Valeur::AdresseMemoire(address));
	if(p.first)
	{
		noyau->_processeur->memoire_allouee({p.second});
		return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Pointeur>(p.second));
	}
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::ValeurNulle>(SVM_Memoire::Type(SVM_Memoire::Type::TypeInterne::POINTEUR)));
}

void svm_memory_free(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	noyau->_memoire->liberation({*pointeur});
}

void svm_memory_scope_set_local(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	noyau->_processeur->memoire_allouee({*pointeur});
}

void svm_memory_scope_set_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	noyau->_processeur->retire_memoire_allouee({*pointeur});
}

void svm_memory_scope_set_local_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::valeur<SVM_Memoire::Chaine>(environnement,alias,__func__,"alias","a string");
	noyau->_processeur->alias_definis({*chaine});
}

void svm_memory_scope_set_local_alias__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::chaine(environnement,alias,__func__,"alias");
	noyau->_processeur->alias_definis({chaine});
}

void svm_memory_scope_set_local_alias__raw(const void *svm, const SVM_Kernel kernel, const char *alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(not alias) { throw MauvaisAppelFonction(__func__,"alias is void"); }
	std::string chaine(alias);
	noyau->_processeur->alias_definis({chaine});
}

void svm_memory_scope_set_global_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::valeur<SVM_Memoire::Chaine>(environnement,alias,__func__,"alias","a string");
	noyau->_processeur->retire_alias_definis({*chaine});
}

void svm_memory_scope_set_global_alias__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::chaine(environnement,alias,__func__,"alias");
	noyau->_processeur->retire_alias_definis({chaine});
}

void svm_memory_scope_set_global_alias__raw(const void *svm, const SVM_Kernel kernel, const char *alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(not alias) { throw MauvaisAppelFonction(__func__,"alias is void"); }
	std::string chaine(alias);
	noyau->_processeur->retire_alias_definis({chaine});
}

SVM_Boolean svm_memory_address_is_defined(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return Outils::booleen(noyau->_memoire->adresse_definie(SVM_Valeur::AdresseMemoire(address)));
}

SVM_Boolean svm_memory_address_is_initialised(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return Outils::booleen(noyau->_memoire->adresse_initialisee(SVM_Valeur::AdresseMemoire(address)));
}

SVM_Value svm_memory_read_address(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return Outils::ajout(environnement,noyau->_memoire->lecture(SVM_Valeur::AdresseMemoire(address),[] (const SVM_Memoire::Type& type) -> bool { return true; },""));
}

SVM_Value svm_memory_extract_address(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return Outils::ajout(environnement,noyau->_memoire->lecture<false>(SVM_Valeur::AdresseMemoire(address),[] (const SVM_Memoire::Type& type) -> bool { return true; },""));
}

SVM_Value svm_memory_read_address_type(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Type type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	std::ostringstream oss;
	oss << (*t);
	return Outils::ajout(environnement,noyau->_memoire->lecture(SVM_Valeur::AdresseMemoire(address),[t] (const SVM_Memoire::Type& tt) -> bool { return (*t)==tt; },oss.str()));
}

SVM_Value svm_memory_read_address_type_internal(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Type_Internal type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto tt = SVM_Memoire::Type(Outils::type_interne(environnement,type,__func__,"type"));
	std::ostringstream oss;
	oss << tt;
	return Outils::ajout(environnement,noyau->_memoire->lecture(SVM_Valeur::AdresseMemoire(address),[tt] (const SVM_Memoire::Type& t) -> bool { return t==tt; },oss.str()));
}

SVM_Value svm_memory_read_address_type_external(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Value_PluginEntryPoint type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type","a plugin entry name");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->type(*pep);
	if(not patron) { throw MauvaisAppelFonction(__func__,"type is not a plugin type"); }
	auto tt = SVM_Memoire::Type(*pep,patron);
	std::ostringstream oss;
	oss << tt;
	return Outils::ajout(environnement,noyau->_memoire->lecture(SVM_Valeur::AdresseMemoire(address),[tt] (const SVM_Memoire::Type& t) -> bool { return t==tt; },oss.str()));
}

SVM_Type svm_memory_address_get_type(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	return environnement->ajout(std::make_shared<SVM_Memoire::Type>(noyau->_memoire->type(SVM_Valeur::AdresseMemoire(address))));
}

void svm_memory_address_set_type(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Type type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	noyau->_memoire->change_type(SVM_Valeur::AdresseMemoire(address),*t);
}

void svm_memory_write_address(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	noyau->_memoire->ecriture(SVM_Valeur::AdresseMemoire(address),valeur);
}

SVM_Boolean svm_memory_address_is_writable(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto valeur = Outils::valeur_valeur(environnement,value,__func__,"value");
	return Outils::booleen(noyau->_memoire->inscriptible(SVM_Valeur::AdresseMemoire(address),valeur));
}

void svm_memory_address_shift(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Value_Integer shift)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto entier = Outils::valeur<SVM_Memoire::Entier>(environnement,shift,__func__,"shift","an integer");
	noyau->_memoire->decalage(SVM_Valeur::AdresseMemoire(address),*entier);
}

void svm_memory_address_shift__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, signed long int shift)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_memoire->decalage(SVM_Valeur::AdresseMemoire(address),shift);
}

SVM_Value* svm_memory_read_array_internal(SVM_Interface::EnvironnementSP& environnement, const std::vector<SVM_Memoire::ValeurSP>& valeurs)
{
	SVM_Interface::TableauVariablesSP tableau = std::make_shared<SVM_Interface::TableauVariables>("values",valeurs.size()+1);
	environnement->ajout_avatar(tableau);
	size_t indice=0;
	for(const auto& v:valeurs)
	{
		tableau->_variables[indice++] = Outils::ajout(environnement,v);
	}
	tableau->_variables[indice] = nullptr;
	return tableau->_variables;
}

SVM_Value* svm_memory_read_pointer(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *pointeur;
	auto valeurs = noyau->_memoire->lecture(p,[] (const SVM_Memoire::Type& type) -> bool { return true; },"");
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_read_pointer__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	SVM_Valeur::Pointeur p = SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(address),size);
	auto valeurs = noyau->_memoire->lecture(p,[] (const SVM_Memoire::Type& type) -> bool { return true; },"");
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_extract_pointer(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *pointeur;
	auto valeurs = noyau->_memoire->lecture<false>(p,[] (const SVM_Memoire::Type& type) -> bool { return true; },"");
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_extract_pointer__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	SVM_Valeur::Pointeur p = SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(address),size);
	auto valeurs = noyau->_memoire->lecture<false>(p,[] (const SVM_Memoire::Type& type) -> bool { return true; },"");
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_read_pointer_type(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Type type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *pointeur;
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	std::ostringstream oss;
	oss << (*t);
	auto valeurs = noyau->_memoire->lecture(p,[t] (const SVM_Memoire::Type& tt) -> bool { return (*t)==tt; },oss.str());
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_read_pointer_type__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size, const SVM_Type type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	SVM_Valeur::Pointeur p = SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(address),size);
	auto t = Outils::valeur_typee<SVM_Memoire::Type>(environnement,type,__func__,"type");
	std::ostringstream oss;
	oss << (*t);
	auto valeurs = noyau->_memoire->lecture(p,[t] (const SVM_Memoire::Type& tt) -> bool { return (*t)==tt; },oss.str());
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_read_pointer_type_internal(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Type_Internal type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *pointeur;
	auto tt = SVM_Memoire::Type(Outils::type_interne(environnement,type,__func__,"type"));
	std::ostringstream oss;
	oss << tt;
	auto valeurs = noyau->_memoire->lecture(p,[tt] (const SVM_Memoire::Type& t) -> bool { return t==tt; },oss.str());
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_read_pointer_type_internal__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size, const SVM_Type_Internal type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	SVM_Valeur::Pointeur p = SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(address),size);
	auto tt = SVM_Memoire::Type(Outils::type_interne(environnement,type,__func__,"type"));
	std::ostringstream oss;
	oss << tt;
	auto valeurs = noyau->_memoire->lecture(p,[tt] (const SVM_Memoire::Type& t) -> bool { return t==tt; },oss.str());
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_read_pointer_type_external(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Value_PluginEntryPoint type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *pointeur;
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type","a plugin entry name");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->type(*pep);
	if(not patron) { throw MauvaisAppelFonction(__func__,"type is not a plugin type"); }
	auto tt = SVM_Memoire::Type(*pep,patron);
	std::ostringstream oss;
	oss << tt;
	auto valeurs = noyau->_memoire->lecture(p,[tt] (const SVM_Memoire::Type& t) -> bool { return t==tt; },oss.str());
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_read_pointer_type_external__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size, const SVM_Value_PluginEntryPoint type)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	SVM_Valeur::Pointeur p = SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(address),size);
	auto pep = Outils::valeur<SVM_Memoire::PointEntreeExtension>(environnement,type,__func__,"type","a plugin entry name");
	auto patron = environnement->_machine->_base->_gestionnaire_extensions->type(*pep);
	if(not patron) { throw MauvaisAppelFonction(__func__,"type is not a plugin type"); }
	auto tt = SVM_Memoire::Type(*pep,patron);
	std::ostringstream oss;
	oss << tt;
	auto valeurs = noyau->_memoire->lecture(p,[tt] (const SVM_Memoire::Type& t) -> bool { return t==tt; },oss.str());
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_read_pointer_zone(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Memory_Zone zone)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *pointeur;
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto valeurs = noyau->_memoire->lecture(p,*z);
	return svm_memory_read_array_internal(environnement,valeurs);
}

SVM_Value* svm_memory_read_pointer_zone__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size, const SVM_Memory_Zone zone)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	SVM_Valeur::Pointeur p = SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(address),size);
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	auto valeurs = noyau->_memoire->lecture(p,*z);
	return svm_memory_read_array_internal(environnement,valeurs);
}

void svm_memory_write_pointer(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Value* values)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *pointeur;
	std::vector<SVM_Memoire::ValeurSP> valeurs;
	for(SVM_Size i=0 ; i<p.taille() ; ++i)
	{
		auto v = Outils::valeur_valeur(environnement,values[i],__func__,"values");
		valeurs.push_back(v);
	}
	noyau->_memoire->ecriture(p,valeurs);
}

void svm_memory_write_pointer__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size, const SVM_Value* values)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	SVM_Valeur::Pointeur p = SVM_Valeur::Pointeur(SVM_Valeur::AdresseMemoire(address),size);
	std::vector<SVM_Memoire::ValeurSP> valeurs;
	for(SVM_Size i=0 ; i<p.taille() ; ++i)
	{
		auto v = Outils::valeur_valeur(environnement,values[i],__func__,"values");
		valeurs.push_back(v);
	}
	noyau->_memoire->ecriture(p,valeurs);
}

SVM_Boolean svm_memory_has_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::valeur<SVM_Memoire::Chaine>(environnement,alias,__func__,"alias","a string");
	return Outils::booleen(noyau->_memoire->existence_alias(*chaine));
}

SVM_Boolean svm_memory_has_alias__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::chaine(environnement,alias,__func__,"alias");
	return Outils::booleen(noyau->_memoire->existence_alias(chaine));
}

SVM_Boolean svm_memory_has_alias__raw(const void *svm, const SVM_Kernel kernel, const char *alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(not alias) { throw MauvaisAppelFonction(__func__,"alias is void"); }
	return Outils::booleen(noyau->_memoire->existence_alias(std::string(alias)));
}

SVM_Value_Pointer svm_memory_alias_get_pointer(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::valeur<SVM_Memoire::Chaine>(environnement,alias,__func__,"alias","a string");
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Pointeur>(noyau->_memoire->resolution_alias(*chaine)));
}

SVM_Value_Pointer svm_memory_alias_get_pointer__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::chaine(environnement,alias,__func__,"alias");
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Pointeur>(noyau->_memoire->resolution_alias(chaine)));
}

SVM_Value_Pointer svm_memory_alias_get_pointer__raw(const void *svm, const SVM_Kernel kernel, const char *alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(not alias) { throw MauvaisAppelFonction(__func__,"alias is void"); }
	return Outils::ajout(environnement,std::make_shared<SVM_Memoire::Pointeur>(noyau->_memoire->resolution_alias(std::string(alias))));
}

void svm_memory_add_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::valeur<SVM_Memoire::Chaine>(environnement,alias,__func__,"alias","a string");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	noyau->_memoire->definition_alias(*chaine,*pointeur);
}

void svm_memory_add_alias__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::chaine(environnement,alias,__func__,"alias");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	noyau->_memoire->definition_alias(chaine,*pointeur);
}

void svm_memory_add_alias__raw(const void *svm, const SVM_Kernel kernel, const char *alias, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(not alias) { throw MauvaisAppelFonction(__func__,"alias is void"); }
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	noyau->_memoire->definition_alias(std::string(alias),*pointeur);
}

void svm_memory_remove_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::valeur<SVM_Memoire::Chaine>(environnement,alias,__func__,"alias","a string");
	noyau->_memoire->suppression_alias({*chaine});
}

void svm_memory_remove_alias__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto chaine = Outils::chaine(environnement,alias,__func__,"alias");
	noyau->_memoire->suppression_alias({chaine});
}

void svm_memory_remove_alias__raw(const void *svm, const SVM_Kernel kernel, const char *alias)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	if(not alias) { throw MauvaisAppelFonction(__func__,"alias is void"); }
	noyau->_memoire->suppression_alias({std::string(alias)});
}

SVM_Value_Pointer* svm_memory_pointer_list_accessible(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	auto accessible = noyau->_memoire->accessible(*pointeur);
	std::vector<SVM_Valeur::Pointeur> accessible_retravaille;
	accessible_retravaille.push_back(*pointeur);
	for(auto& a:accessible)
	{
		if(a.inclus(*pointeur))
		{
			auto e = a.evide(*pointeur);
			if(not e.first.vide())
			{
				accessible_retravaille.push_back(e.first);
			}
			if(not e.second.vide())
			{
				accessible_retravaille.push_back(e.second);
			}
		}
		else
		{
			accessible_retravaille.push_back(a);
		}
	}
	SVM_Interface::TableauVariablesSP tableau = std::make_shared<SVM_Interface::TableauVariables>("pointers",accessible_retravaille.size()+1);
	auto it = tableau->_variables;
	for(const auto& a : accessible_retravaille)
	{
		*it = Outils::ajout(environnement,std::make_shared<SVM_Memoire::Pointeur>(a));
		++it;
	}
	*it=nullptr;
	environnement->ajout_avatar(tableau);
	return tableau->_variables;
}

SVM_Value_String* svm_memory_pointer_list_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	auto accessible = noyau->_memoire->alias_inclus({*pointeur});
	return Outils::tableau_chaines(environnement,accessible);
}

SVM_Value_String* svm_memory_pointer_expand_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	auto accessible = noyau->_memoire->alias_inclus<false>({*pointeur});
	return Outils::tableau_chaines(environnement,accessible);
}

SVM_Boolean svm_memory_pointer_has_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	auto accessible = noyau->_memoire->alias_inclus<false>({*pointeur});
	return Outils::booleen(accessible.empty()?FALSE:TRUE);
}

SVM_Memory_Zone svm_memory_pointer_get_zone(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *pointeur;
	auto zone = std::make_shared<SVM_Memoire::DescriptionBloc>();
	auto adresse = p.adresse();
	auto taille = p.taille();
	for(size_t i=0 ; i<taille ; ++i)
	{
		zone->ajout(noyau->_memoire->type<SVM_Memoire::LectureAdresseNonDefinie>(adresse+i),1);
	}
	return environnement->ajout(zone);
}

void svm_memory_pointer_set_zone(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Memory_Zone zone)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto pointeur = Outils::valeur<SVM_Memoire::Pointeur>(environnement,pointer,__func__,"pointer","a pointer");
	SVM_Valeur::Pointeur p = *pointeur;
	auto z = Outils::valeur_typee<SVM_Memoire::DescriptionBloc>(environnement,zone,__func__,"zone");
	if(z->taille()!=p.taille())
	{
		throw MauvaisAppelFonction(__func__,"pointer size and zone size mismatch");
	}
	noyau->_memoire->change_type(p,*z);
}

void svm_memory_copy(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer origin, const SVM_Kernel remote, const SVM_Value_Pointer target)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto origine = Outils::valeur<SVM_Memoire::Pointeur>(environnement,origin,__func__,"origin","a pointer");
	auto distant = Outils::noyau(environnement,remote,__func__,"remote");
	auto destination = Outils::valeur<SVM_Memoire::Pointeur>(environnement,target,__func__,"target","a pointer");
	noyau->_memoire->copie(*origine,distant->_memoire,*destination);
}

void svm_memory_move(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer origin, const SVM_Kernel remote, const SVM_Value_Pointer target)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto origine = Outils::valeur<SVM_Memoire::Pointeur>(environnement,origin,__func__,"origin","a pointer");
	auto distant = Outils::noyau(environnement,remote,__func__,"remote");
	auto destination = Outils::valeur<SVM_Memoire::Pointeur>(environnement,target,__func__,"target","a pointer");
	noyau->_memoire->deplacement(*origine,distant->_memoire,*destination);
}

void svm_memory_share(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer origin, const SVM_Kernel remote, const SVM_Value_Pointer target)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto origine = Outils::valeur<SVM_Memoire::Pointeur>(environnement,origin,__func__,"origin","a pointer");
	auto distant = Outils::noyau(environnement,remote,__func__,"remote");
	auto destination = Outils::valeur<SVM_Memoire::Pointeur>(environnement,target,__func__,"target","a pointer");
	noyau->_memoire->partage(*origine,distant->_memoire,*destination);
	if(noyau!=distant)
	{
		noyau->_memoire->active_synchronisation(*origine);
	}
}

void svm_memory_translate(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer zone, const SVM_Value_Pointer origin, const SVM_Value_Pointer target)
{
	auto environnement = Outils::environnement(svm,__func__);
	auto noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto z = Outils::valeur<SVM_Memoire::Pointeur>(environnement,zone,__func__,"zone","a pointer");
	auto origine = Outils::valeur<SVM_Memoire::Pointeur>(environnement,origin,__func__,"origin","a pointer");
	auto destination = Outils::valeur<SVM_Memoire::Pointeur>(environnement,target,__func__,"target","a pointer");
	noyau->_memoire->traduit(*z,*origine,*destination);
}

void svm_debug_break(const void *svm, const SVM_Kernel kernel, const SVM_Value_String message)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto chaine = Outils::valeur<SVM_Memoire::Chaine>(environnement,message,__func__,"message","a string");
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	std::ostringstream oss;
	oss << environnement->nom() << ":\n" << SVM::Global::Chaine::echappe_html(*chaine);
	bool interne = false;
	if(environnement->_variable_noyau)
	{
		SVM_Noyau::NoyauSP noyau_courant = Outils::noyau(environnement,environnement->_variable_noyau,__func__,"current kernel");
		interne = noyau==noyau_courant;
	}
	if(interne)
	{
		noyau->_processeur->_piege->point_arret("Plugin",false,oss.str());
	}
	else
	{
		noyau->_processeur->_piege->point_arret_externe("Plugin",oss.str());
	}
}

void svm_debug_break__string(const void *svm, const SVM_Kernel kernel, const SVM_String message)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto chaine = Outils::chaine(environnement,message,__func__,"message");
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	std::ostringstream oss;
	oss << environnement->nom() << ":\n" << SVM::Global::Chaine::echappe_html(std::string(message.string,message.size));
	bool interne = false;
	if(environnement->_variable_noyau)
	{
		SVM_Noyau::NoyauSP noyau_courant = Outils::noyau(environnement,environnement->_variable_noyau,__func__,"current kernel");
		interne = noyau==noyau_courant;
	}
	if(interne)
	{
		noyau->_processeur->_piege->point_arret("Plugin",false,oss.str());
	}
	else
	{
		noyau->_processeur->_piege->point_arret_externe("Plugin",oss.str());
	}
}

void svm_debug_break__raw(const void *svm, const SVM_Kernel kernel, const char *message)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	if(not message) { throw MauvaisAppelFonction(__func__,"message is void"); }
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	std::ostringstream oss;
	oss << environnement->nom() << ":\n" << SVM::Global::Chaine::echappe_html(message);
	bool interne = false;
	if(environnement->_variable_noyau)
	{
		SVM_Noyau::NoyauSP noyau_courant = Outils::noyau(environnement,environnement->_variable_noyau,__func__,"current kernel");
		interne = noyau==noyau_courant;
	}
	if(interne)
	{
		noyau->_processeur->_piege->point_arret("Plugin",false,oss.str());
	}
	else
	{
		noyau->_processeur->_piege->point_arret_externe("Plugin",oss.str());
	}
}

void svm_debug_notify(const void *svm, const SVM_Kernel kernel, const SVM_Value_String message)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto chaine = Outils::valeur<SVM_Memoire::Chaine>(environnement,message,__func__,"message","a string");
	if(not kernel)
	{
		environnement->_machine->_base->_debugueur->_points_arret->ajout_point_arret(SVM_Debugueur::PointArret("Notification",*chaine));
		return;
	}
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto processus = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
	std::ostringstream oss;
	oss << "At ";
	noyau->_processeur->instruction_courante().html(oss);
	oss << "\n" << environnement->nom() << ":\n" << SVM::Global::Chaine::echappe_html(*chaine);
	environnement->_machine->_base->_debugueur->_points_arret->ajout_point_arret(SVM_Debugueur::PointArret("Notification",oss.str(),noyau->_piege->identifiant(),processus->nom_debug(),noyau->nom_debug()));
}

void svm_debug_notify__string(const void *svm, const SVM_Kernel kernel, const SVM_String message)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto chaine = Outils::chaine(environnement,message,__func__,"message");
	if(not kernel)
	{
		environnement->_machine->_base->_debugueur->_points_arret->ajout_point_arret(SVM_Debugueur::PointArret("Notification",chaine));
		return;
	}
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto processus = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
	std::ostringstream oss;
	oss << "At ";
	noyau->_processeur->instruction_courante().html(oss);
	oss << "\n" << environnement->nom() << ":\n" << SVM::Global::Chaine::echappe_html(chaine);
	environnement->_machine->_base->_debugueur->_points_arret->ajout_point_arret(SVM_Debugueur::PointArret("Notification",oss.str(),noyau->_piege->identifiant(),processus->nom_debug(),noyau->nom_debug()));
}

void svm_debug_notify__raw(const void *svm, const SVM_Kernel kernel, const char *message)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	if(not message) { throw MauvaisAppelFonction(__func__,"message is void"); }
	if(not kernel)
	{
		environnement->_machine->_base->_debugueur->_points_arret->ajout_point_arret(SVM_Debugueur::PointArret("Notification",message));
		return;
	}
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto processus = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
	std::ostringstream oss;
	oss << "At ";
	noyau->_processeur->instruction_courante().html(oss);
	oss << "\n" << environnement->nom() << ":\n" << SVM::Global::Chaine::echappe_html(message);
	environnement->_machine->_base->_debugueur->_points_arret->ajout_point_arret(SVM_Debugueur::PointArret("Notification",oss.str(),noyau->_piege->identifiant(),processus->nom_debug(),noyau->nom_debug()));
}

void svm_debug_synchronise(const void *svm, const SVM_Value value)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto valeur = Outils::valeur_typee<SVM_Memoire::ValeurInterface>(environnement,value,__func__,"value");
	if(not valeur->_valeur) return;
	SVM_Memoire::PiegeMemoireSP piege;
	SVM_Noyau::NoyauSP noyau_courant = environnement->valeur<SVM_Noyau::Noyau>(environnement->_variable_noyau);
	if(static_cast<bool>(noyau_courant))
	{
		piege = noyau_courant->_memoire->_piege;
	}
	valeur->_valeur->notification_debugueur_changement_memoire(piege);
}

void svm_debug_breakpoint_add_break(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto valeur = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	SVM_Valeur::AdresseInstruction adresse = *valeur;
	noyau->_processeur->_piege->ajoute_point_arret(adresse);
}

void svm_debug_breakpoint_remove_break(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto valeur = Outils::valeur<SVM_Memoire::Symbole>(environnement,symbol,__func__,"symbol","a symbol");
	SVM_Valeur::AdresseInstruction adresse = *valeur;
	noyau->_processeur->_piege->enleve_point_arret(adresse);
}

void svm_debug_breakpoint_add_interruption(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto valeur = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	SVM_Valeur::Interruption i = *valeur;
	noyau->_processeur->_piege->ajoute_interruption(i);
}

void svm_debug_breakpoint_remove_interruption(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	auto valeur = Outils::valeur<SVM_Memoire::Interruption>(environnement,interruption,__func__,"interruption","an interruption");
	SVM_Valeur::Interruption i = *valeur;
	noyau->_processeur->_piege->enleve_interruption(i);
}

void svm_debug_breakpoint_add_memoryread(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_memoire->_piege->point_arret_lecture(SVM_Valeur::AdresseMemoire(address),true);
}

void svm_debug_breakpoint_remove_memoryread(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_memoire->_piege->point_arret_lecture(SVM_Valeur::AdresseMemoire(address),false);
}

void svm_debug_breakpoint_add_memorywrite(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_memoire->_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(address),true);
}

void svm_debug_breakpoint_remove_memorywrite(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_memoire->_piege->point_arret_ecriture(SVM_Valeur::AdresseMemoire(address),false);
}

void svm_debug_breakpoint_add_memoryaccess(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_memoire->_piege->point_arret_acces(SVM_Valeur::AdresseMemoire(address),true);
}

void svm_debug_breakpoint_remove_memoryaccess(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_memoire->_piege->point_arret_acces(SVM_Valeur::AdresseMemoire(address),false);
}

void svm_debug_breakpoint_add_memoryfree(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_memoire->_piege->point_arret_suppression(SVM_Valeur::AdresseMemoire(address),true);
}

void svm_debug_breakpoint_remove_memoryfree(const void *svm, const SVM_Kernel kernel, const SVM_Address address)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	SVM_Noyau::NoyauSP noyau = Outils::noyau(environnement,kernel,__func__,"kernel");
	noyau->_memoire->_piege->point_arret_suppression(SVM_Valeur::AdresseMemoire(address),false);
}

SVM_Debug_Form svm_debug_form_new(const void *svm, const char *title)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return nullptr;
	if(not title) { throw MauvaisAppelFonction(__func__,"title is void"); }
	return environnement->ajout(std::make_shared<SVM_Interface::Formulaire>(title));
}

void svm_debug_form_append_checkbox(const void *svm, SVM_Debug_Form form, const char *label, const SVM_Value_Boolean default_value)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto formulaire = Outils::valeur_typee<SVM_Interface::Formulaire>(environnement,form,__func__,"form");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	auto defaut = Outils::valeur<SVM_Memoire::Booleen>(environnement,default_value,__func__,"default_value","a boolean"); 
	formulaire->_champs.push_back(std::make_shared<SVM_Interface::CocheFormulaire>(label,*defaut));
}

void svm_debug_form_append_checkbox__raw(const void *svm, SVM_Debug_Form form, const char *label, const int default_value)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto formulaire = Outils::valeur_typee<SVM_Interface::Formulaire>(environnement,form,__func__,"form");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	formulaire->_champs.push_back(std::make_shared<SVM_Interface::CocheFormulaire>(label,default_value!=0));
}

void svm_debug_form_append_selection(const void *svm, SVM_Debug_Form form, const char *label, const SVM_Size size, const SVM_Value *values)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto formulaire = Outils::valeur_typee<SVM_Interface::Formulaire>(environnement,form,__func__,"form");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	std::list<std::string> valeurs;
	for(SVM_Size i=0 ; i<size ; ++i)
	{
		auto valeur = Outils::valeur_valeur(environnement,values[i],__func__,"values");
		std::ostringstream oss;
		oss << (*valeur);
		valeurs.push_back(oss.str());
	}
	formulaire->_champs.push_back(std::make_shared<SVM_Interface::SelectionFormulaire>(label,valeurs));
}

void svm_debug_form_append_integer(const void *svm, SVM_Debug_Form form, const char *label, const SVM_Value_Integer default_value, const long long int min_value, const long long int max_value)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto formulaire = Outils::valeur_typee<SVM_Interface::Formulaire>(environnement,form,__func__,"form");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	auto defaut = Outils::valeur<SVM_Memoire::Entier>(environnement,default_value,__func__,"default_value","an integer");
	formulaire->_champs.push_back(std::make_shared<SVM_Interface::EntierFormulaire>(label,*defaut,min_value,max_value));
}

void svm_debug_form_append_integer__raw(const void *svm, SVM_Debug_Form form, const char *label, const long long int default_value, const long long int min_value, const long long int max_value)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto formulaire = Outils::valeur_typee<SVM_Interface::Formulaire>(environnement,form,__func__,"form");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	formulaire->_champs.push_back(std::make_shared<SVM_Interface::EntierFormulaire>(label,default_value,min_value,max_value));
}

void svm_debug_form_append_string(const void *svm, SVM_Debug_Form form, const char *label, const SVM_Value_String default_value, const SVM_Size min_size, const SVM_Size max_size)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto formulaire = Outils::valeur_typee<SVM_Interface::Formulaire>(environnement,form,__func__,"form");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	auto defaut = Outils::valeur<SVM_Memoire::Chaine>(environnement,default_value,__func__,"default_value","a string");
	formulaire->_champs.push_back(std::make_shared<SVM_Interface::ChaineFormulaire>(label,*defaut,min_size,max_size));
}

void svm_debug_form_append_string__string(const void *svm, SVM_Debug_Form form, const char *label, const SVM_String default_value, const SVM_Size min_size, const SVM_Size max_size)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto formulaire = Outils::valeur_typee<SVM_Interface::Formulaire>(environnement,form,__func__,"form");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	if(not default_value.string) { throw MauvaisAppelFonction(__func__,"default_value is void"); }
	formulaire->_champs.push_back(std::make_shared<SVM_Interface::ChaineFormulaire>(label,std::string(default_value.string,default_value.size),min_size,max_size));
}

void svm_debug_form_append_string__raw(const void *svm, SVM_Debug_Form form, const char *label, const char *default_value, const SVM_Size min_size, const SVM_Size max_size)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto formulaire = Outils::valeur_typee<SVM_Interface::Formulaire>(environnement,form,__func__,"form");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	if(not default_value) { throw MauvaisAppelFonction(__func__,"default_value is void"); }
	formulaire->_champs.push_back(std::make_shared<SVM_Interface::ChaineFormulaire>(label,default_value,min_size,max_size));
}

void svm_debug_form_append_text(const void *svm, SVM_Debug_Form form, const char *label, const SVM_Size width, const SVM_Size height)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return;
	auto formulaire = Outils::valeur_typee<SVM_Interface::Formulaire>(environnement,form,__func__,"form");
	if(not label) { throw MauvaisAppelFonction(__func__,"label is void"); }
	formulaire->_champs.push_back(std::make_shared<SVM_Interface::TexteFormulaire>(label,width,height));
}

SVM_Value* svm_debug_form_request(const void *svm, const SVM_Debug_Form form)
{
	auto environnement = Outils::environnement(svm,__func__);
	if(not environnement->_machine->_base->_debugueur) return nullptr;
	auto noyau = Outils::noyau(environnement,environnement->_variable_noyau,__func__,"current kernel");
	auto processus = Outils::processus(environnement,environnement->_variable_processus,__func__,"current process");
	auto debug = environnement->_machine->_base->_debugueur;
	auto formulaire = Outils::valeur_typee<SVM_Interface::Formulaire>(environnement,form,__func__,"form");
	auto json = formulaire->json();
	json->ajoute("processor",SVM_Debugueur::JSON::entier(noyau->_processeur->_piege->identifiant()));
	noyau->mode_debugueur(true);
	noyau->_piege->notification_changement_objet();
	processus->mode_debugueur(true);
	std::ostringstream oss;
	oss << "At ";
	noyau->_processeur->instruction_courante().html(oss);
	oss << "\n" << environnement->nom() << ":\n";
	SVM_Debugueur::PointArret pa("Form",oss.str(),noyau->_piege->identifiant(),processus->nom_debug(),noyau->nom_debug(),json);
	debug->_points_arret->ajout_point_arret(pa);
	SVM_Debugueur::JSON_Interne::TableauSP champs;
	std::vector<ValeurFormulaireSP> valeurs;
	for( ; ; )
	{
		auto reponse = debug->attente_commande(noyau->_processeur->_piege);
		auto nom = SVM_Debugueur::JSON::chaine((*reponse)["command"]);
		if(not (SVM_Debugueur::JSON::est(nom) and (static_cast<std::string>(*nom)=="submit")))
			continue;
		champs = SVM_Debugueur::JSON::tableau((*reponse)["values"]);
		if(not SVM_Debugueur::JSON::est(champs))
			continue;
		if(champs->taille()!=formulaire->_champs.size())
			continue;
		bool fin = true;
		auto itc=formulaire->_champs.begin();
		for(auto itv = champs->begin() ; itv!=champs->end() ; ++itv,++itc)
		{
			auto v = (*itc)->valeur(*itv);
			if(not v)
			{
				fin = false;
				break;
			}
			valeurs.push_back(v);
		}
		if(fin) break;
	}
	debug->_points_arret->nettoie_formulaire(pa._identifiant,champs);
	noyau->mode_debugueur(false);
	noyau->_piege->notification_changement_objet();
	processus->mode_debugueur(false);
	SVM_Interface::TableauVariablesSP tableau = std::make_shared<SVM_Interface::TableauVariables>("values",formulaire->_champs.size()+1);
	environnement->ajout_avatar(tableau);
	size_t indice = 0;
	for(auto& v: valeurs)
	{
		tableau->_variables[indice++] = v->convertit(environnement);
	}
	tableau->_variables[formulaire->_champs.size()] = nullptr;
	return tableau->_variables;
}

}
