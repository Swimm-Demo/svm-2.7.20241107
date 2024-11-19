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

#include <src/machine/extensions/gestionnaire/gestionnaire.h>
#include <src/machine/extensions/analyseur/analyseur.h>
#include <src/machine/interface/svm.h>
#include <src/machine/interface/environnement.h>
#include <src/machine/machine/historique.h>
#include <src/machine/machine/machine.h>
#include <src/machine/elements/memoire/entier.h>
#include <src/machine/elements/memoire/chaine.h>
#include <src/machine/elements/memoire/booleen.h>
using namespace SVM::Machine::Extension;

struct SoPointeur
{
	SoPointeur(void *so)
	:_so(so) {}
	~SoPointeur()
	{
		if(_so!=nullptr)
		{
			::dlclose(_so);
		}
	}
	void commit() { _so = nullptr; }
	void *_so;
};

GestionnaireSP Gestionnaire::gestionnaire(const std::vector<SVM::Machine::DeclarationExtension>& extensions, SVM::Machine::Debugueur::DebugueurSP& debugueur)
{
	GestionnaireSP gestionnaire = std::make_shared<Gestionnaire>();
	std::vector<DescriptionSP> descriptions;
	for(auto& extension: extensions)
	{
		SoPointeur so(::dlopen(extension._nom.c_str(),RTLD_LAZY|RTLD_LOCAL));
		if(not so._so)
		{
			throw ExtensionFichierInvalide(extension._nom,::dlerror());
		}
		void (*configuration)(void *) = reinterpret_cast<void (*)(void *)>(::dlsym(so._so,"plugin_configure"));
		if(not configuration)
		{
			throw ExtensionSansConfiguration(extension._nom);
		}
		ExtensionConfiguration extension_configuration;
		(*configuration)(&extension_configuration);
		if(extension_configuration._configuration.empty() or extension_configuration._signature_api.empty() or extension_configuration._version_svm.empty())
		{
			throw ExtensionConfigurationSansEnregistrement(extension._nom);
		}
		std::string signature_api_machine(::svm_api_get_signature());
		auto signature_api_machine_set = Gestionnaire::tokens(signature_api_machine,'_');
		auto signature_api_extension_set = Gestionnaire::tokens(extension_configuration._signature_api,'_');
		std::set<std::string> differences;
		std::set_difference(signature_api_extension_set.begin(),signature_api_extension_set.end(),signature_api_machine_set.begin(),signature_api_machine_set.end(),std::inserter(differences,differences.end()));
		if(not differences.empty())
		{
			throw ExtensionAPINonCompatible(extension._nom,extension_configuration._version_svm);
		}
		DescriptionSP description = SVM_Extension::Analyseur::analyse(extension._nom,extension_configuration._configuration);
		auto it = gestionnaire->_extensions.insert(description->_nom);
		if(not it.second)
		{
			throw ExtensionCollision(extension._nom,description->_nom);
		}
		descriptions.push_back(description);
		MapOptions options;
		for(const auto& o:extension._options)
		{
			if(not options.insert(std::make_pair(o._nom,Gestionnaire::conversion(description->_nom,o._valeur))).second)
			{
				throw OptionEnDouble(description->_nom,o._nom);
			}
		}
		ListeArguments arguments;
		for(const auto& a:extension._arguments)
		{
			arguments.push_back(Gestionnaire::conversion(description->_nom,a));
		}
		if(arguments.size()<description->_indice_argument)
		{
			throw PasAssezArguments(description->_nom,arguments.size(),description->_indice_argument);
		}
		for(auto& d:description->_declare)
		{
			ReferenceObjet r = *d;
			auto p = d->patron(extension._nom,so._so);
			switch(r._type)
			{
				case ReferenceObjet::Type::OPTION:
					{
						auto o = std::dynamic_pointer_cast<PatronOptionExtension>(p);
						auto it = options.find(o->_clef);
						if(it!=options.end())
						{
							if(not (SVM_Memoire::Type(o->_type)==static_cast<SVM_Memoire::Type>(*it->second)))
							{
								if(o->_type==SVM_Memoire::Type::TypeInterne::BOOLEEN)
								{

									throw OptionAvecValeur(description->_nom,o->_clef);
								}
								else
								{
									throw OptionTypeIncorrect(description->_nom,o->_clef,SVM_Memoire::Type(o->_type));
								}
							}
							o->_valeur = it->second;
							options.erase(it);
						}
					}
					break;
				case ReferenceObjet::Type::ARGUMENT:
					{
						auto a = std::dynamic_pointer_cast<PatronArgumentExtension>(p);
						if(not (SVM_Memoire::Type(a->_type)==static_cast<SVM_Memoire::Type>(*arguments[a->_indice])))
						{
							throw ArgumentTypeIncorrect(description->_nom,a->_indice,SVM_Memoire::Type(a->_type));
						}
						a->_valeur = arguments[a->_indice];
					}
					break;
				case ReferenceObjet::Type::ARGUMENTS:
					{
						auto a = std::dynamic_pointer_cast<PatronArgumentsExtension>(p);
						a->_valeurs = { arguments.begin()+description->_indice_argument, arguments.end() };
					}
					break;
				default:
					break;
			}
			gestionnaire->_objets.insert(std::make_pair(r,p));
		}
		if(not options.empty())
		{
			std::ostringstream oss;
			for(const auto& o: options)
			{
				oss << " -" << o.first;
			}
			throw OptionsNonReconnues(description->_nom,oss.str());
		}
		gestionnaire->_bibliotheques.push_back(Bibliotheque(description->_nom,so._so));
	//	for(const auto& o:options)
	//	{
	//		std::cout << description->_nom << ": -" << o.first << " " << o.second->serialise() << std::endl;
	//	}
	//	for(const auto& a:arguments)
	//	{
	//		std::cout << description->_nom << ": " << a->serialise() << std::endl;
	//	}
		so.commit();
	}
	for(auto& d: descriptions)
	{
		for(auto& u:d->_utilise)
		{
			auto dependance_requise = u->dependance(d->_nom);
			auto dependance_trouvee = gestionnaire->_objets.find(static_cast<ReferenceObjet>(*u));
			if(dependance_trouvee==gestionnaire->_objets.end())
			{
				throw ExtensionDependanceNonTrouvee(d->_nom,dependance_requise);
			}
			if(not dependance_trouvee->second->compatible(dependance_requise))
			{
				throw ExtensionDependanceNonCompatible(d->_nom,dependance_trouvee->second,dependance_requise);
			}
		}
		// Les types d'extensions presents dans les parametres et retour de fonction ne sont volontairement pas testes !
		// Il n'est pas obligatoire d'avoir l'extension qui les definit de presente. Si le type en question est requis par
		// l'extension, elle DOIT le mentionner dans sa clause USES.
	}
	for(auto& b:gestionnaire->_bibliotheques)
	{
		b._initialisation_generale = reinterpret_cast<Bibliotheque::Fonction>(::dlsym(b._bibliotheque,"plugin_initialisation"));
		b._demarrage_general = reinterpret_cast<Bibliotheque::Fonction>(::dlsym(b._bibliotheque,"plugin_startup"));
		b._finalisation_generale = reinterpret_cast<Bibliotheque::Fonction>(::dlsym(b._bibliotheque,"plugin_finalisation"));
		//b._initialisation_processus = reinterpret_cast<Bibliotheque::Fonction>(::dlsym(b._bibliotheque,"plugin_initialisation_process"));
		//b._finalisation_processus = reinterpret_cast<Bibliotheque::Fonction>(::dlsym(b._bibliotheque,"plugin_finalisation_process"));
		//b._initialisation_noyau = reinterpret_cast<Bibliotheque::Fonction>(::dlsym(b._bibliotheque,"plugin_initialisation_kernel"));
		//b._finalisation_noyau = reinterpret_cast<Bibliotheque::Fonction>(::dlsym(b._bibliotheque,"plugin_finalisation_kernel"));
	}
	gestionnaire->_initialisation = true;
	for(const auto& e: gestionnaire->_extensions)
	{
		gestionnaire->_variables_extensions.insert(std::make_pair(e,std::make_shared<SVM_Interface::VariablesDefinies>(std::make_shared<SVM_Synchronisation::Acces>())));
	}
	gestionnaire->_piege = SVM::Machine::Debugueur::Debugueur::ajoute_piege<PiegeGestionnaireExtensions>(debugueur,gestionnaire);
	if(static_cast<bool>(gestionnaire->_piege)) { gestionnaire->_piege->notification_changement_objet(); }
	return gestionnaire;
}

Gestionnaire::~Gestionnaire()
{
	for(auto& b:_bibliotheques)
	{
		SVM_TRACE("Ferme bibliotheque " << b._bibliotheque);
		::dlclose(b._bibliotheque);
	}
}

PatronTypeExtensionSP Gestionnaire::type(const SVM_Valeur::PointEntreeExtension& type) const
{
	auto o = _objets.find(ReferenceObjet(ReferenceObjet::Type::TYPE,type));
	if(o == _objets.end())
	{
		return PatronTypeExtensionSP();
	}
	return std::dynamic_pointer_cast<PatronTypeExtension>(o->second);
}

PatronInterruptionExtensionSP Gestionnaire::interruption(const SVM_Valeur::PointEntreeExtension& interruption) const
{
	auto o = _objets.find(ReferenceObjet(ReferenceObjet::Type::INTERRUPTION,interruption));
	if(o == _objets.end())
	{
		return PatronInterruptionExtensionSP();
	}
	return std::dynamic_pointer_cast<PatronInterruptionExtension>(o->second);
}

PatronInstructionExtensionSP Gestionnaire::instruction(const SVM_Valeur::PointEntreeExtension& instruction) const
{
	auto o = _objets.find(ReferenceObjet(ReferenceObjet::Type::INSTRUCTION,instruction));
	if(o == _objets.end())
	{
		return PatronInstructionExtensionSP();
	}
	return std::dynamic_pointer_cast<PatronInstructionExtension>(o->second);
}

PatronStructureExtensionSP Gestionnaire::structure(const SVM_Valeur::PointEntreeExtension& structure) const
{
	auto o = _objets.find(ReferenceObjet(ReferenceObjet::Type::STRUCTURE,structure));
	if(o == _objets.end())
	{
		return PatronStructureExtensionSP();
	}
	return std::dynamic_pointer_cast<PatronStructureExtension>(o->second);
}

PatronFonctionExtensionSP Gestionnaire::fonction(const SVM_Valeur::PointEntreeExtension& fonction) const
{
	auto o = _objets.find(ReferenceObjet(ReferenceObjet::Type::FONCTION,fonction));
	if(o == _objets.end())
	{
		return PatronFonctionExtensionSP();
	}
	return std::dynamic_pointer_cast<PatronFonctionExtension>(o->second);
}

PatronSequenceurExtensionSP Gestionnaire::sequenceur(const SVM_Valeur::PointEntreeExtension& sequenceur) const
{
	auto o = _objets.find(ReferenceObjet(ReferenceObjet::Type::SEQUENCEUR,sequenceur));
	if(o == _objets.end())
	{
		return PatronSequenceurExtensionSP();
	}
	return std::dynamic_pointer_cast<PatronSequenceurExtension>(o->second);
}

PatronOrdonnanceurExtensionSP Gestionnaire::ordonnanceur(const SVM_Valeur::PointEntreeExtension& ordonnanceur) const
{
	auto o = _objets.find(ReferenceObjet(ReferenceObjet::Type::ORDONNANCEUR,ordonnanceur));
	if(o == _objets.end())
	{
		return PatronOrdonnanceurExtensionSP();
	}
	return std::dynamic_pointer_cast<PatronOrdonnanceurExtension>(o->second);
}

PatronOptionExtensionSP Gestionnaire::option(const SVM_Valeur::PointEntreeExtension& option) const
{
	auto o = _objets.find(ReferenceObjet(ReferenceObjet::Type::OPTION,option));
	if(o == _objets.end())
	{
		return PatronOptionExtensionSP();
	}
	return std::dynamic_pointer_cast<PatronOptionExtension>(o->second);
}

PatronArgumentExtensionSP Gestionnaire::argument(const SVM_Valeur::PointEntreeExtension& argument) const
{
	auto o = _objets.find(ReferenceObjet(ReferenceObjet::Type::ARGUMENT,argument));
	if(o == _objets.end())
	{
		return PatronArgumentExtensionSP();
	}
	return std::dynamic_pointer_cast<PatronArgumentExtension>(o->second);
}

PatronArgumentsExtensionSP Gestionnaire::arguments(const SVM_Valeur::PointEntreeExtension& arguments) const
{
	auto o = _objets.find(ReferenceObjet(ReferenceObjet::Type::ARGUMENTS,arguments));
	if(o == _objets.end())
	{
		return PatronArgumentsExtensionSP();
	}
	return std::dynamic_pointer_cast<PatronArgumentsExtension>(o->second);
}

std::list<SVM_Valeur::PointEntreeExtension> Gestionnaire::liste_ordonannanceurs() const
{
	std::list<SVM_Valeur::PointEntreeExtension> liste;
	for(auto& o : _objets)
	{
		if(o.first._type==ReferenceObjet::Type::ORDONNANCEUR)
		{
			liste.push_back(o.first._nom);
		}
	}
	return liste;
}

SVM_Interface::VariablesDefiniesSP Gestionnaire::variables_extension(const std::string& nom_extension) const
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto it = _variables_extensions.find(nom_extension);
	if(it==_variables_extensions.end())
		return std::make_shared<SVM_Interface::VariablesDefinies>();
	return it->second;
}

template<typename E>
void gestionnaire_extensions_erreur(const SVM::Machine::MachineSP& machine, const PatronObjetExtensionSP& patron, const E& e)
{
	if(not machine->_historique) return;
	std::ostringstream oss;
	oss << (*patron);
	SVM::Machine::Trace trace(machine->_historique,oss.str());
	trace << "Error in " << (*patron) << ": " << e;
}

void Gestionnaire::initialisation_generale(const SVM::Machine::MachineSP& machine)
{
	for(auto& b:_bibliotheques)
	{
		if(b._initialisation_generale)
		{
			SVM::Machine::Interface::EnvironnementSP environnement = std::make_shared<SVM::Machine::Interface::Environnement>(std::make_shared<PatronFonctionGeneriqueExtension>(b._nom,"plugin_initialisation"),machine,variables_extension(b._nom),true,true,_nettoyage);
			try
			{
				b._initialisation_generale(&environnement);
			}
			catch(const SVM_Valeur::Interruption& i)
			{
				std::ostringstream oss;
				i.erreur(oss);
				gestionnaire_extensions_erreur(machine,environnement->_patron,oss.str());
			}
			catch(const SVM_Valeur::ExceptionExecution& e)
			{
				gestionnaire_extensions_erreur(machine,environnement->_patron,e);
			}
			catch(const std::exception& e)
			{
				gestionnaire_extensions_erreur(machine,environnement->_patron,e.what());
			}
			catch(...)
			{
				gestionnaire_extensions_erreur(machine,environnement->_patron,"Unexpected error");
			}
		}
	}
}

void Gestionnaire::demarrage_general(const SVM::Machine::MachineSP& machine)
{
	for(auto& b:_bibliotheques)
	{
		if(b._demarrage_general)
		{
			SVM::Machine::Interface::EnvironnementSP environnement = std::make_shared<SVM::Machine::Interface::Environnement>(std::make_shared<PatronFonctionGeneriqueExtension>(b._nom,"plugin_startup"),machine,variables_extension(b._nom),true,true,_nettoyage);
			try
			{
				b._demarrage_general(&environnement);
			}
			catch(const SVM_Valeur::Interruption& i)
			{
				std::ostringstream oss;
				i.erreur(oss);
				gestionnaire_extensions_erreur(machine,environnement->_patron,oss.str());
			}
			catch(const SVM_Valeur::ExceptionExecution& e)
			{
				gestionnaire_extensions_erreur(machine,environnement->_patron,e);
			}
			catch(const std::exception& e)
			{
				gestionnaire_extensions_erreur(machine,environnement->_patron,e.what());
			}
			catch(...)
			{
				gestionnaire_extensions_erreur(machine,environnement->_patron,"Unexpected error");
			}
		}
	}
}

void Gestionnaire::finalisation_generale(const SVM::Machine::MachineSP& machine)
{
	if(not _initialisation) return;
	for(auto it = _bibliotheques.rbegin() ; it!=_bibliotheques.rend() ; ++it)
	{
		if(it->_finalisation_generale)
		{
			SVM::Machine::Interface::EnvironnementSP environnement = std::make_shared<SVM::Machine::Interface::Environnement>(std::make_shared<PatronFonctionGeneriqueExtension>(it->_nom,"plugin_finalisation"),machine,variables_extension(it->_nom),true,true,_nettoyage);
			try
			{
				it->_finalisation_generale(&environnement);
			}
			catch(const SVM_Valeur::Interruption& i)
			{
				std::ostringstream oss;
				i.erreur(oss);
				gestionnaire_extensions_erreur(machine,environnement->_patron,oss.str());
			}
			catch(const SVM_Valeur::ExceptionExecution& e)
			{
				gestionnaire_extensions_erreur(machine,environnement->_patron,e);
			}
			catch(const std::exception& e)
			{
				gestionnaire_extensions_erreur(machine,environnement->_patron,e.what());
			}
			catch(...)
			{
				gestionnaire_extensions_erreur(machine,environnement->_patron,"Unexpected error");
			}
		}
	}
}

//void Gestionnaire::initialisation_processus(const SVM::Machine::Element::Processus::ProcessusSP& processus, const SVM::Machine::Element::Noyau::NoyauSP& noyau)
//{
//	for(auto& b:_bibliotheques)
//	{
//		if(b._initialisation_processus)
//		{
//			SVM::Machine::Interface::EnvironnementSP environnement = std::make_shared<SVM::Machine::Interface::Environnement>(std::make_shared<PatronFonctionGeneriqueExtension>(b._nom,"plugin_initialisation_process"),this->shared_from_this(),variables_extension(b._nom),true,true,_nettoyage);
//			environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM::Machine::Interface::VariableProcessus>(processus));
//			environnement->_variable_noyau = environnement->ajout(noyau);
//			try
//			{
//				b._initialisation_processus(&environnement);
//			}
//			catch(const SVM_Valeur::Interruption& i)
//			{
//				gestionnaire_extensions_erreur(this,environnement->_patron,i);
//			}
//			catch(const SVM_Valeur::ExceptionExecution& e)
//			{
//				gestionnaire_extensions_erreur(this,environnement->_patron,e);
//			}
//			catch(const std::exception& e)
//			{
//				gestionnaire_extensions_erreur(machine,environnement->_patron,e.what());
//			}
//			catch(...)
//			{
//				gestionnaire_extensions_erreur(this,environnement->_patron,"Unexpected error");
//			}
//		}
//	}
//}
//
//void Gestionnaire::finalisation_processus(const SVM::Machine::Element::Processus::ProcessusSP& processus)
//{
//	for(auto it = _bibliotheques.rbegin() ; it!=_bibliotheques.rend() ; ++it)
//	{
//		if(_initialisation)
//		{
//			if(it->_finalisation_processus)
//			{
//				SVM::Machine::Interface::EnvironnementSP environnement = std::make_shared<SVM::Machine::Interface::Environnement>(std::make_shared<PatronFonctionGeneriqueExtension>(it->_nom,"plugin_finalisation_process"),this->shared_from_this(),variables_extension(it->_nom),true,true,_nettoyage);
//				environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM::Machine::Interface::VariableProcessus>(processus));
//				try
//				{
//					it->_finalisation_processus(&environnement);
//				}
//				catch(const SVM_Valeur::Interruption& i)
//				{
//					gestionnaire_extensions_erreur(this,environnement->_patron,i);
//				}
//				catch(const SVM_Valeur::ExceptionExecution& e)
//				{
//					gestionnaire_extensions_erreur(this,environnement->_patron,e);
//				}
//				catch(const std::exception& e)
//				{
//					gestionnaire_extensions_erreur(machine,environnement->_patron,e.what());
//				}
//				catch(...)
//				{
//					gestionnaire_extensions_erreur(this,environnement->_patron,"Unexpected error");
//				}
//			}
//		}
//	}
//}
//
//void Gestionnaire::initialisation_noyau(const SVM::Machine::Element::Noyau::NoyauSP& noyau)
//{
//	for(auto& b:_bibliotheques)
//	{
//		if(b._initialisation_noyau)
//		{
//			SVM::Machine::Interface::EnvironnementSP environnement = std::make_shared<SVM::Machine::Interface::Environnement>(std::make_shared<PatronFonctionGeneriqueExtension>(b._nom,"plugin_initialisation_kernel"),this->shared_from_this(),variables_extension(b._nom),true,true,_nettoyage);
//			environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM::Machine::Interface::VariableProcessus>(noyau->_processus.lock()));
//			environnement->_variable_noyau = environnement->ajout(noyau);
//			try
//			{
//				b._initialisation_noyau(&environnement);
//			}
//			catch(const SVM_Valeur::Interruption& i)
//			{
//				gestionnaire_extensions_erreur(this,environnement->_patron,i);
//			}
//			catch(const SVM_Valeur::ExceptionExecution& e)
//			{
//				gestionnaire_extensions_erreur(this,environnement->_patron,e);
//			}
//			catch(const std::exception& e)
//			{
//				gestionnaire_extensions_erreur(machine,environnement->_patron,e.what());
//			}
//			catch(...)
//			{
//				gestionnaire_extensions_erreur(this,environnement->_patron,"Unexpected error");
//			}
//		}
//	}
//}
//void Gestionnaire::finalisation_noyau(const SVM::Machine::Element::Noyau::NoyauSP& noyau)
//{
//	for(auto it = _bibliotheques.rbegin() ; it!=_bibliotheques.rend() ; ++it)
//	{
//		if(_initialisation)
//		{
//			if(it->_finalisation_noyau)
//			{
//				SVM::Machine::Interface::EnvironnementSP environnement = std::make_shared<SVM::Machine::Interface::Environnement>(std::make_shared<PatronFonctionGeneriqueExtension>(it->_nom,"plugin_finalisation_kernel"),this->shared_from_this(),variables_extension(it->_nom),true,true,_nettoyage);
//				environnement->_variable_processus = environnement->ajout_avatar(std::make_shared<SVM::Machine::Interface::VariableProcessus>(noyau->_processus.lock()));
//				environnement->_variable_noyau = environnement->ajout(noyau);
//				try
//				{
//					it->_finalisation_noyau(&environnement);
//				}
//				catch(const SVM_Valeur::Interruption& i)
//				{
//					gestionnaire_extensions_erreur(this,environnement->_patron,i);
//				}
//				catch(const SVM_Valeur::ExceptionExecution& e)
//				{
//					gestionnaire_extensions_erreur(this,environnement->_patron,e);
//				}
//				catch(const std::exception& e)
//				{
//					gestionnaire_extensions_erreur(machine,environnement->_patron,e.what());
//				}
//				catch(...)
//				{
//					gestionnaire_extensions_erreur(this,environnement->_patron,"Unexpected error");
//				}
//			}
//		}
//	}
//}

void Gestionnaire::nettoie_variables()
{
	decltype(_variables_extensions) temp;
	{
		std::lock_guard<std::mutex> verrou(_protection);
		_variables_extensions.swap(temp);
		_nettoyage = true;
	}
	temp.clear();
}

std::set<SVM::Machine::Element::Valeur::Interruption> Gestionnaire::externes() const
{
	std::set<SVM::Machine::Element::Valeur::Interruption> i;
	for(const auto& o: _objets)
	{
		o.second->interruption(i);
	}
	return i;
}

SVM::Machine::Element::Memoire::ValeurSP Gestionnaire::conversion(const std::string& nom, const SVM::Machine::ParametreExtensionSP& p)
{
	if(not p)
	{
		auto v = std::make_shared<SVM::Machine::Element::Memoire::Booleen>(true);
		v->constante();
		return v;
	}
	auto e = std::dynamic_pointer_cast<SVM::Machine::ParametreExtensionEntier>(p);
	if(static_cast<bool>(e))
	{
		auto v = std::make_shared<SVM::Machine::Element::Memoire::Entier>(e->_entier);
		v->constante();
		return v;
	}
	auto s = std::dynamic_pointer_cast<SVM::Machine::ParametreExtensionChaine>(p);
	if(static_cast<bool>(s))
	{
		auto v = std::make_shared<SVM::Machine::Element::Memoire::Chaine>(s->_chaine);
		v->constante();
		return v;
	}
	throw TypeParametreInvalide(nom);
}

SVM_Debugueur::JSON_Interne::ObjetSP PiegeGestionnaireExtensions::mise_a_jour()
{
	auto gestionnaire = _gestionnaire.lock();
	if(not gestionnaire)
	{
		throw SVM_Debugueur::PasDeMiseAJour();
	}
	auto elements = SVM_Debugueur::JSON::tableau();
	for(const auto& o:gestionnaire->_objets)
	{
		elements->ajoute(SVM_Debugueur::JSON::objet({ { "type", SVM_Debugueur::JSON::chaine(ReferenceObjet::texte(o.first._type)) }, { "definition", SVM_Debugueur::JSON::chaine(o.second->debugueur()) } }));
	}
	return SVM_Debugueur::JSON::objet({ { "objects" , elements} });
}
