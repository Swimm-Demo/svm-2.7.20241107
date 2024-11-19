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

#include <src/machine/elements/systeme/systeme.h>
#include <src/machine/elements/base/base.h>
#include <src/machine/elements/ordonnanceur/ordonnanceur_defaut.h>
#include <src/machine/elements/ordonnanceur/ordonnanceur_extension.h>
#include <src/machine/machine/machine.h>
using namespace SVM::Machine::Element::Systeme;

Systeme::Systeme(const SVM_Machine::MachineSP& machine)
:_livraison(std::make_shared<Livraison>()), _machine(machine)
{ }

Systeme::~Systeme()
{
	SVM_TRACE("Arret du systeme");
	_livraison->destruction_boite(_adresse);
}

SystemeSP Systeme::creation_systeme(const SVM_Machine::MachineSP& machine)
{
	auto systeme = std::make_shared<Systeme>(machine);
	systeme->_adresse = Livraison::adresse(systeme->shared_from_this()); 
	systeme->_livraison->creation_boite(systeme->_adresse);
	systeme->_livraison->_piege = SVM::Machine::Debugueur::Debugueur::ajoute_piege<PiegeEvenements>(machine->_base->_debugueur);
	machine->_base->_gestionnaire_interruptions_materielles->_systeme = systeme;
	machine->_base->_gestionnaire_extensions->initialisation_generale(machine);
	auto ord_defaut = SVM_Ordonnanceur::OrdonnanceurDefaut::creation_ordonnanceur(systeme->shared_from_this());
	systeme->_ordonnanceurs.insert(std::make_pair(SVM_Valeur::PointEntreeExtensionSP(),ord_defaut));
	for(auto& o : machine->_base->_gestionnaire_extensions->liste_ordonannanceurs())
	{
		auto nom = std::make_shared<SVM_Valeur::PointEntreeExtension>(o);
		auto ord_extension = SVM_Ordonnanceur::OrdonnanceurExtension::creation_ordonnanceur(nom,systeme->shared_from_this());
		systeme->_ordonnanceurs.insert(std::make_pair(nom,ord_extension));
	}
	for(auto& o: systeme->_ordonnanceurs)
	{
		o.second->execution();
	}
	return systeme;
}

int Systeme::execution(const SVM::Machine::MachineSP& machine, std::vector<SVM_Machine::Processus>& liste_processus)
{
	size_t retour_naturel = 0;
	bool au_moins_un = false;
	bool alerte_au_moins_un_vivant = false;
	SHARED_PTR(size_t) retour;
	{
		machine->_base->_gestionnaire_extensions->demarrage_general(machine);
		if(liste_processus.empty())
			return 0;
		for(auto& p: liste_processus)
		{
			try
			{
				auto memoire = SVM_Memoire::Memoire::creation_memoire(machine->_base->_debugueur,SVM_Noyau::ControleAccesSP());
				p.initialise(memoire);
				SVM_Processus::ProcessusSP processus = SVM_Processus::Processus::creation_processus(p._nom,this->shared_from_this(),machine->_base,p._sequenceur,p._terminaison_automatique,p._code,memoire);
				auto ordonnanceur = _ordonnanceurs.find(p._ordonnanceur);
				ordonnanceur->second->attache(_adresse,processus,0);
				if(attente_reponse(processus))
				{
					ordonnanceur->second->pret(processus);
					au_moins_un = true;
				}
				else
				{
					auto machine = _machine.lock();
					if(static_cast<bool>(machine))
					{
						SVM_Machine::Trace trace(machine->_historique,"SYSTEM");
						trace << "Error: ";
						processus->nom_flux(trace);
						trace << " not attached to ";
						ordonnanceur->second->nom_flux(trace);
					}
					processus->terminaison_demandee(_adresse);
					processus->attente_terminaison();
					retire_processus(processus);
				}
			}
			catch(SVM_Valeur::ExceptionExecution& e)
			{
				auto machine = _machine.lock();
				if(static_cast<bool>(machine))
				{
					SVM_Machine::Trace(machine->_historique,"SYSTEM") << "Error during process " << p._nom << " start: " << e;
				}
			}
			catch(SVM_Valeur::Interruption& i)
			{
				auto machine = _machine.lock();
				if(static_cast<bool>(machine))
				{
					SVM_Machine::Trace trace(machine->_historique,"SYSTEM");
					trace << "Error during process " << p._nom << " start: ";
					i.erreur(trace);
				}
			}
		}
		liste_processus.clear();
	}
	for( ; au_moins_un ; )
	{
		SVM_Synchronisation::AdresseSP source;
		EvenementSP evenement;
		if(not _livraison->recupere(_adresse,source,evenement))
		{
			SVM_TRACE("SYSTEM message non recupere");
			continue;
		}
		SVM_TRACE("SYSTEM traitement message " << evenement << " pour " << source);
		if(evenement->_type == Evenement::Type::ETAT)
		{
			SVM_Processus::ProcessusSP processus = Livraison::objet<SVM_Processus::Processus>(source);
			if(static_cast<bool>(processus) and SVM_Processus::Etat::termine(evenement->_etat) and processus->termine())
			{
				SVM_TRACE("SYSTEM traite terminaison processus " << processus->_nom);
				auto code_retour = traitement_processus_mort(processus,evenement->_etat);
				if(static_cast<bool>(code_retour))
				{
					if(not retour) retour = code_retour;
					for(auto& p : this->liste_processus())
					{
						p->terminaison_demandee(_adresse);
					}
				}
				if(evenement->_etat!=SVM_Processus::Etat::Execution::ZOMBIE) retour_naturel=125;
				auto ordonnanceur = processus->_ordonnanceur.lock();
				if(static_cast<bool>(ordonnanceur))
				{
					ordonnanceur->detache(_adresse,processus,0);
				}
				else
				{
					SVM_TRACE("Retrait du processus du systeme");
					if(retire_processus(processus))
					{
						break;
					}
				}
			}
			encore_un_vivant(alerte_au_moins_un_vivant);
		}
		else if(evenement->_type == Evenement::Type::REPONSE)
		{
			SVM_Ordonnanceur::OrdonnanceurSP ordonnanceur = Livraison::objet<SVM_Ordonnanceur::Ordonnanceur>(source);
			if(evenement->_reponse == Evenement::Reponse::NON)
			{
				auto machine = _machine.lock();
				if(static_cast<bool>(machine) and static_cast<bool>(evenement->_processus) and static_cast<bool>(ordonnanceur))
				{
					SVM_Machine::Trace trace(machine->_historique,"SYSTEM");
					trace << "Error: ";
					evenement->_processus->nom_flux(trace);
					trace << " not detached from ";
					ordonnanceur->nom_flux(trace);
				}
			}
			SVM_TRACE("Retrait du processus du systeme");
			if(static_cast<bool>(evenement->_processus) and retire_processus(evenement->_processus))
			{
				break;
			}
			encore_un_vivant(alerte_au_moins_un_vivant);
		}
		else if(evenement->_type == Evenement::Type::TERMINAISON)
		{
			for(auto& p : this->liste_processus())
			{
				p->terminaison_demandee(_adresse);
			}
		}
	}
	SVM_TRACE("Processus restants : " << _processus.size());
	SVM_TRACE("Arret des ordonnanceurs");
	size_t compteur=0;
	for(auto& o : _ordonnanceurs)
	{
		++compteur;
		SVM_TRACE( compteur << " ordonnanceurs a attendre");
		o.second->terminaison_demandee(_adresse);
	}
	while(compteur>0)
	{
		SVM_Synchronisation::AdresseSP source;
		SVM_Systeme::EvenementSP evenement;
		SVM_TRACE("Attente d'un ordonnanceur");
		if(not _livraison->recupere(_adresse,source,evenement))
			continue;
		if((evenement->_type==SVM_Systeme::Evenement::Type::REPONSE) and (evenement->_reponse==SVM_Systeme::Evenement::Reponse::OUI))
		{
			SVM_TRACE("Un ordonnanceur de moins");
			--compteur;
		}
	}
	_ordonnanceurs.clear();
	machine->_base->_gestionnaire_extensions->finalisation_generale(machine);
	machine->_base->_gestionnaire_extensions->nettoie_variables();
	SVM_TRACE("Processus restants : " << _processus.size());
	if(not retour)
		return retour_naturel;
	return *retour;
}

void Systeme::acquittement_terminaison(const SVM_Synchronisation::AdresseSP& source)
{
	_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::REPONSE,SVM_Systeme::Evenement::Reponse::OUI));
}

void Systeme::ajoute_processus(const SVM_Processus::ProcessusSP& processus)
{
	std::lock_guard<std::mutex> verrou(_protection);
	_processus.push_back(processus);
}

bool Systeme::retire_processus(const SVM_Processus::ProcessusSP& processus)
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto it = std::find(_processus.begin(),_processus.end(),processus);
	if(it!=_processus.end())
	{
		_processus.erase(it);
	}
	return _processus.empty();
}

std::vector<SVM_Processus::ProcessusSP> Systeme::liste_processus() const
{
	std::lock_guard<std::mutex> verrou(_protection);
	return std::vector<SVM_Processus::ProcessusSP>(_processus.begin(),_processus.end());
}

SHARED_PTR(size_t) Systeme::traitement_processus_mort(const SVM_Processus::ProcessusSP& processus, const SVM_Processus::Etat::Execution etat_declare) 
{
	switch(etat_declare)
	{
		case SVM_Processus::Etat::Execution::ZOMBIE:
			{
				SVM_TRACE("Traitement processus zombie par systeme");
				return processus->code_retour();
			}
			break;
		case SVM_Processus::Etat::Execution::INTERROMPU:
			{
				SVM_TRACE("Traitement processus interrompu par systeme");
				SVM_Valeur::InterruptionSP i = processus->interrompu();
				ASSERT(static_cast<bool>(i),"Processus interrompu sans interruption");
				auto machine = _machine.lock();
				if(static_cast<bool>(machine))
				{
					SVM_Machine::Trace trace(machine->_historique,"SYSTEM");
					trace << "Process " << processus->_nom << " interrupted: ";
					i->erreur(trace);
				}
			}
			break;
		case SVM_Processus::Etat::Execution::ERREUR:
				{
				SVM_TRACE("Traitement processus en erreur par systeme");
				auto e = processus->erreur();
				ASSERT(static_cast<bool>(e),"Processus en erreur sans erreur");
				auto machine = _machine.lock();
				if(static_cast<bool>(machine))
				{
					SVM_Machine::Trace(machine->_historique,"SYSTEM") << "Process " << processus->_nom << " aborted: " << (*e);
				}
				}
			break;
		default:
			break;
	}
	return SHARED_PTR(size_t)();
}

SVM_Interface::VariablesDefiniesSP Systeme::variables_extension(const std::string& nom_extension) const
{
	auto machine = _machine.lock();
	if(not machine)
	{
		return std::make_shared<SVM_Interface::VariablesDefinies>();
	}
	return machine->_base->_gestionnaire_extensions->variables_extension(nom_extension);
}

SVM_Ordonnanceur::OrdonnanceurSP Systeme::ordonnanceur(const SVM_Valeur::PointEntreeExtensionSP& nom) const
{
	auto it = _ordonnanceurs.find(nom);
	if(it==_ordonnanceurs.end())
	{
		return SVM_Ordonnanceur::OrdonnanceurSP();
	}
	return it->second;
}

void Systeme::terminaison_demandee()
{
	_livraison->poste(_adresse,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::TERMINAISON));
}

void Systeme::interruption_materielle()
{
	std::lock_guard<std::mutex> verrou(_protection);
	for(auto& p: _processus)
	{
		p->notification_interruption();
	}
}

void Systeme::force_deverrouillage(const SVM_Synchronisation::AdresseSP& adresse)
{
	std::lock_guard<std::mutex> verrou(_protection);
	for(auto& p: _processus)
	{
		p->force_deverrouillage(adresse);
	}
}

bool Systeme::attente_reponse(const SVM_Processus::ProcessusSP& processus) const
{
	for( ; ; )
	{
		SVM_Synchronisation::AdresseSP source;
		SVM_Systeme::EvenementSP evenement;
		if(not _livraison->recupere_specifique(_adresse,source,evenement,[&processus](const SVM_Synchronisation::AdresseSP& s, const SVM_Systeme::EvenementSP& e) -> bool { return (e->_type==Evenement::Type::REPONSE) and (e->_processus==processus);}))
			continue;
		return evenement->_reponse==SVM_Systeme::Evenement::Reponse::OUI;
	}
}

SVM_Systeme::Evenement::Reponse Systeme::attente_reponse(const SVM_Synchronisation::AdresseSP& origine)
{
	SVM_Systeme::Evenement::Reponse reponse=SVM_Systeme::Evenement::Reponse::NON;
	for( ; ; )
	{
		SVM_Synchronisation::AdresseSP source;
		SVM_Systeme::EvenementSP evenement;
		if(not _livraison->recupere_specifique(_adresse,source,evenement,[&origine](const SVM_Synchronisation::AdresseSP& s, const SVM_Systeme::EvenementSP& e) -> bool { return (e->_type==SVM_Systeme::Evenement::Type::REPONSE) and (s==origine);})) continue;
		reponse = evenement->_reponse;
		break;
	}
	return reponse;
}

void Systeme::encore_un_vivant(bool& alerte)
{
	bool encore_un_vivant = false;
	auto lp = this->liste_processus();
	for(auto& p : lp)
	{
		encore_un_vivant |= p->vivant();
	}
	if(not encore_un_vivant and not alerte)
	{
		SVM_TRACE("Aucun processus vivant");
		for(auto& p : this->liste_processus())
		{
			p->terminaison_demandee(_adresse);
		}
		alerte = true;
	}
}

SVM_Systeme::Evenement::Reponse Systeme::attente_verrouillage(const SVM_Synchronisation::AdresseSP& verrouillage)
{
	SVM_Systeme::Evenement::Reponse reponse=SVM_Systeme::Evenement::Reponse::NON;
	for( ; ; )
	{
		SVM_Synchronisation::AdresseSP source;
		SVM_Systeme::EvenementSP evenement;
		if(not _livraison->recupere_specifique(_adresse,source,evenement,[] (const SVM_Synchronisation::AdresseSP& s, const SVM_Systeme::EvenementSP& e) -> bool { return e->_type==SVM_Systeme::Evenement::Type::REPONSE; })) continue;
		reponse = evenement->_reponse;
		break;
	}
	return reponse;
}

