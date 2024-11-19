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

#include <src/global/systeme.h>
#include <src/machine/elements/ordonnanceur/ordonnanceur.h>
#include <src/machine/elements/systeme/systeme.h>
#include <src/machine/machine/machine.h>

using namespace SVM::Machine::Element::Ordonnanceur;

Ordonnanceur::Ordonnanceur(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme)
	:_nom(nom), _systeme(systeme), _livraison(systeme->_livraison)
	{
		ASSERT(systeme,"ordonnanceur sans systeme");
	}

Ordonnanceur::~Ordonnanceur()
{
	if(_systeme.expired())
	{
		_livraison->poste(_adresse,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::TERMINAISON));
	}
	_effecteur.join();
	_livraison->destruction_boite(_adresse);
	SVM_TRACE("Arret de l'ordonnanceur " << _nom);
}

void Ordonnanceur::initialisation()
{
	_adresse = SVM_Systeme::Livraison::adresse(this->shared_from_this()); 
	_livraison->creation_boite(_adresse);
}

void Ordonnanceur::execution()
{
	try
	{
		_effecteur = std::thread([this] ()
		{
			thread_local size_t echeance = 0;
			for( ; ; )
			{
				SVM_Synchronisation::AdresseSP source;
				SVM_Systeme::EvenementSP evenement;
				if(echeance==0)
				{
					if(not _livraison->recupere(_adresse,source,evenement))
						continue;
				}
				else
				{
					if(not _livraison->recupere_echeance(_adresse,source,evenement,echeance))
					{
						try
						{
							echeance = notification(_adresse,TIMER,echeance);
							if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
						}
						catch(const SVM_Valeur::Interruption& i)
						{
							erreur(i);
						}
						catch(const SVM_Valeur::ExceptionExecution& e)
						{
							erreur(e);
						}
						catch(const std::exception& e)
						{
							erreur(e.what());
						}
						catch(...)
						{
							erreur("Unexpected error");
						}
						continue;
					}
				}
				switch(evenement->_type)
				{
					case SVM_Systeme::Evenement::Type::TERMINAISON:
						{
							auto systeme = _systeme.lock();
							systeme->acquittement_terminaison(_adresse);
						}
						return;
					case SVM_Systeme::Evenement::Type::ETAT:
						try
						{
							auto processus = SVM_Systeme::Livraison::objet<SVM_Processus::Processus>(source);
							if(not processus)
							{
								erreur("no process attached to state");
								continue;
							}
							echeance = ordonnancement(processus,evenement->_etat);
							if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
						}
						catch(const SVM_Valeur::Interruption& i)
						{
							erreur(i);
						}
						catch(const SVM_Valeur::ExceptionExecution& e)
						{
							erreur(e);
						}
						catch(const std::exception& e)
						{
							erreur(e.what());
						}
						catch(...)
						{
							erreur("Unexpected error");
						}
						break;
					case SVM_Systeme::Evenement::Type::ATTACHE:
						{
							bool reussite = false;
							try
							{
								if(not evenement->_processus->_ordonnanceur.expired())
								{
									throw ProcessusDejaAttache(_nom,evenement->_processus->_nom);
								}
								evenement->_processus->_ordonnanceur = this->shared_from_this();
								reussite = attache_processus(evenement->_processus,evenement->_entier);
								if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
							}
							catch(const SVM_Valeur::Interruption& i)
							{
								erreur(i);
							}
							catch(const SVM_Valeur::ExceptionExecution& e)
							{
								erreur(e);
							}
							catch(const std::exception& e)
							{
								erreur(e.what());
							}
							catch(...)
							{
								erreur("Unexpected error");
							}
							if(not reussite)
							{
								evenement->_processus->_ordonnanceur.reset();
							}
							_livraison->poste(source,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::REPONSE,reussite?SVM_Systeme::Evenement::Reponse::OUI:SVM_Systeme::Evenement::Reponse::NON,evenement->_processus));
						}
						break;
					case SVM_Systeme::Evenement::Type::DETACHE:
						{
							bool reussite = false;
							try
							{
								if(evenement->_processus->_ordonnanceur.expired())
								{
									throw ProcessusDejaDetache(_nom,evenement->_processus->_nom);
								}
								if(evenement->_processus->_ordonnanceur.lock()!=this->shared_from_this())
								{
									throw ProcessusAttacheParAutreOrdonnanceur(_nom,evenement->_processus->_nom);
								}
								reussite = detache_processus(evenement->_processus,evenement->_entier);
								if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
							}
							catch(const SVM_Valeur::Interruption& i)
							{
								erreur(i);
							}
							catch(const SVM_Valeur::ExceptionExecution& e)
							{
								erreur(e);
							}
							catch(const std::exception& e)
							{
								erreur(e.what());
							}
							catch(...)
							{
								erreur("Unexpected error");
							}
							if(reussite)
							{
								evenement->_processus->_ordonnanceur.reset();
							}
							_livraison->poste(source,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::REPONSE,reussite?SVM_Systeme::Evenement::Reponse::OUI:SVM_Systeme::Evenement::Reponse::NON,evenement->_processus));
						}
						break;
					case SVM_Systeme::Evenement::Type::NOTIFICATION:
						try
						{
							echeance = notification(_adresse,NOTIFICATION,evenement->_entier);
							if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
						}
						catch(const SVM_Valeur::Interruption& i)
						{
							erreur(i);
						}
						catch(const SVM_Valeur::ExceptionExecution& e)
						{
							erreur(e);
						}
						catch(const std::exception& e)
						{
							erreur(e.what());
						}
						catch(...)
						{
							erreur("Unexpected error");
						}
						break;
					default:
						break;
				}
			}
		}
		);
	}
	catch(std::system_error& e)
	{
		throw OrdonnanceurNonLance(_nom,e.what());
	}
#ifdef LINUX_EXTENSIONS
	std::ostringstream oss;
	nom_flux_court(oss);
	SVM::Global::Systeme::nom_thread(_effecteur.native_handle(),oss.str());
#endif
}

void Ordonnanceur::attache(const SVM_Synchronisation::AdresseSP& source, const SVM_Processus::ProcessusSP& processus, const size_t parametre)
{
	_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::ATTACHE,processus,parametre));
}

void Ordonnanceur::detache(const SVM_Synchronisation::AdresseSP& source, const SVM_Processus::ProcessusSP& processus, const size_t parametre)
{
	_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::DETACHE,processus,parametre));
}

void Ordonnanceur::notifie(const SVM_Synchronisation::AdresseSP& source, const size_t parametre)
{
	_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::NOTIFICATION,parametre));
}

void Ordonnanceur::terminaison_demandee(const SVM_Synchronisation::AdresseSP& source)
{
	_livraison->poste(_adresse,source,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::TERMINAISON));
}

SVM_Systeme::Evenement::Reponse Ordonnanceur::attente_verrouillage(const SVM_Synchronisation::AdresseSP& verrouillage)
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

SVM_Machine::HistoriqueSP Ordonnanceur::historique() const
{
	auto systeme = _systeme.lock();
	if(not systeme) return SVM_Machine::HistoriqueSP();
	auto machine = systeme->_machine.lock();
	if(not machine) return SVM_Machine::HistoriqueSP();
	return machine->_historique;
}

SVM_Debugueur::JSON_Interne::ObjetSP PiegeOrdonnanceur::mise_a_jour()
{
	auto o = _ordonnanceur.lock();
	if(not o)
	{
		throw SVM_Debugueur::PasDeMiseAJour();
	}
	std::string nom;
	std::string description;
	std::ostringstream oss;
	o->nom_flux(oss);
	nom = oss.str();
	oss.str("");
	oss << (*o);
	description = oss.str();
	return SVM_Debugueur::JSON::objet({ { "name" , SVM_Debugueur::JSON::chaine(nom) }, { "state" , SVM_Debugueur::JSON::chaine(description) } });
}
