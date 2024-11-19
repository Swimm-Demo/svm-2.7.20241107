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
#include <src/machine/elements/processus/processus.h>
#include <src/machine/extensions/gestionnaire/gestionnaire.h>
#include <src/machine/elements/sequenceur/sequenceur_defaut.h>
#include <src/machine/elements/sequenceur/sequenceur_extension.h>
#include <src/machine/elements/systeme/systeme.h>
#include <src/machine/elements/ordonnanceur/ordonnanceur.h>
namespace SVM_Sequenceur = SVM::Machine::Element::Sequenceur;
namespace SVM_Systeme = SVM::Machine::Element::Systeme;

using namespace SVM::Machine::Element::Processus;

Processus::Processus(const std::string& nom, const SVM_Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const bool terminaison_automatique)
:_nom(nom), _etat(terminaison_automatique), _systeme(systeme), _base(base), _livraison(systeme->_livraison), _nombre_verrouillage(0), _notification(0), _accepte_notification_interruption(false)
{
	_notification_recue = std::make_shared<SVM_Synchronisation::Notification>();
}

ProcessusSP Processus::creation_processus(const std::string& nom, const SVM_Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique, SVM_Valeur::CodeSP& code, SVM_Memoire::MemoireSP& memoire, const bool transmet_interruption, const bool retour_est_arret, const bool mode_protege, const SVM_Noyau::ControleAccesSP& controle_acces)
{
	ProcessusSP processus = creation_processus(nom,systeme,base,sequenceur,terminaison_automatique,SVM_Valeur::AdresseInstruction(code,0),memoire,transmet_interruption,retour_est_arret,mode_protege,controle_acces);
	code.reset();
	return processus;
}

ProcessusSP Processus::creation_processus(const std::string& nom, const SVM_Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique, const SVM_Valeur::AdresseInstruction& code, SVM_Memoire::MemoireSP& memoire, const bool transmet_interruption, const bool retour_est_arret, const bool mode_protege, const SVM_Noyau::ControleAccesSP& controle_acces)
{
	SVM_Noyau::NoyauSP noyau = SVM_Noyau::Noyau::creation_noyau(base->_debugueur,transmet_interruption,retour_est_arret,mode_protege,controle_acces,code,memoire);
	memoire.reset();
	return creation_processus(nom,systeme,base,sequenceur,terminaison_automatique,noyau);
}

ProcessusSP Processus::creation_processus(const std::string& nom, const SVM::Machine::Element::Systeme::SystemeSP& systeme, const SVM_Base::BaseSP& base, const SVM_Valeur::PointEntreeExtensionSP& sequenceur, const bool terminaison_automatique, SVM_Noyau::NoyauSP& noyau)
{
	auto processus = std::make_shared<Processus>(nom,systeme,base,terminaison_automatique);
	processus->_sequenceur = SVM_Sequenceur::Sequenceur::creation_sequenceur(base->_debugueur,sequenceur,processus);
	processus->_adresse=SVM_Systeme::Livraison::adresse(processus->shared_from_this());
	processus->_livraison->creation_boite(processus->_adresse);
	//base->_gestionnaire_extensions->initialisation_processus(processus,noyau);
	SVM_Interface::EnvironnementSP environnement;
	processus->_sequenceur->attache_noyau(noyau,environnement,0,nullptr);
	systeme->ajoute_processus(processus);
	processus->execution();
	processus->_piege = SVM_Debugueur::Debugueur::ajoute_piege<PiegeProcessus>(base->_debugueur,processus);
	static std::atomic<unsigned long long> identifiant(0);
	processus->_identifiant = ++identifiant;
	return processus;
}

Processus::~Processus()
{
	if(_travailleur.get_id()==std::thread::id())
	{
		SVM_TRACE("Lancement du processus " << _nom << " echoue");
	}
	else
	{
		if(not _etat.termine())
		{
			_livraison->poste(_adresse,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::TERMINAISON));
		}
		if(_travailleur.joinable()) _travailleur.join();
	}
	_livraison->destruction_boite(_adresse);
	SVM_TRACE("Arret du processus " << _nom);
}

void Processus::execution()
{
	try
	{
		_travailleur = std::thread([this] ()
		{
		SVM_TRACE("PROCESS " << _nom << " depart travailleur");
		std::lock_guard<std::mutex> protege(_execution);
		if(not _etat.executable())
			return;
		SVM_Valeur::InterruptionSP interruption_transmise;
		for( ; ; )
		{
			
			try
			{
				_noyau_courant = _sequenceur->noyau_courant();
				if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
			}
			catch(SVM_Valeur::Interruption& i)
			{
				std::ostringstream oss;
				oss << "Unable to get current kernel: ";
				i.erreur(oss);
				_etat.erreur(oss.str());
				if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
				break;
			}
			catch(SVM_Valeur::ExceptionExecution& e)
			{
				std::ostringstream oss;
				oss << "Unable to get current kernel: " << e;
				_etat.erreur(oss.str());
				if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
				break;
			}
			if(not _noyau_courant)
			{
				if(static_cast<bool>(interruption_transmise))
				{
					_etat.interrompu(interruption_transmise);
					if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
				}
				else
				{
					_etat.zombie();
					if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
				}
				break;
			}
			if(_noyaux.find(_noyau_courant)==_noyaux.end())
			{
				_etat.erreur("Execution attempt of kernel not attached to this process.");
				if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
				break;
			}
			if(static_cast<bool>(interruption_transmise))
			{
				_noyau_courant->interruption(*interruption_transmise);
				interruption_transmise.reset();
			}
			
			const SVM_Noyau::Etat& etat_noyau = _noyau_courant->execution();

			if(_etat.termine())
			{
				break;
			}

			if(etat_noyau._transmission_interruption_demandee)
			{
				interruption_transmise = etat_noyau.interrompu();
			}
			{
				auto erreur_noyau = etat_noyau.erreur();
				if(static_cast<bool>(erreur_noyau))
				{
					std::ostringstream oss;
					oss << "Execution failed in kernel: " << (*erreur_noyau) ;
					_etat.erreur(oss.str());
					if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
					break;
				}
			}
			{
				auto code_retour = etat_noyau.code_retour();
				if(static_cast<bool>(code_retour))
				{
					_etat.zombie(code_retour);
					if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
					break;
				}
			}
			if(etat_noyau.termine())
			{
				try
				{
					SVM_Interface::EnvironnementSP environnement;
					_sequenceur->detache_noyau(_noyau_courant,environnement,0,nullptr);
				}
				catch(SVM_Valeur::Interruption& i)
				{
					std::ostringstream oss;
					oss << "Unable to detach dead kernel: " << i;
					_etat.erreur(oss.str());
					if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
				}
				catch(SVM_Valeur::ExceptionExecution& e)
				{
					std::ostringstream oss;
					oss << "Unable to detach dead kernel: " << e;
					_etat.erreur(oss.str());
					if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
				}
			}
		}
		auto ordonnanceur = _ordonnanceur.lock();
		if(static_cast<bool>(ordonnanceur))
		{
			ordonnanceur->notifie_etat(_adresse,_etat.etat_global());
		}
		auto systeme = _systeme.lock();
		SVM_TRACE("PROCESS " << _nom << " notification systeme etat " << Etat::texte(_etat.etat_global()));
		systeme->notifie_etat(_adresse,_etat.etat_global());
		systeme->force_deverrouillage(_adresse);
		_verrouille_par.reset();
		for(auto& i: _verrous_en_cours)
		{
			_livraison->poste(i,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::DEVERROUILLAGE));
		}
		_verrous_en_cours.clear();
		for(auto& i: _verrouillages_en_attente)
		{
			_livraison->poste(i,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::REPONSE,SVM_Systeme::Evenement::Reponse::NON));
		}
		_verrouillages_en_attente.clear();
		if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		//_base->_gestionnaire_extensions->finalisation_processus(this->shared_from_this());
		SVM_TRACE("PROCESS " << _nom << " arret travailleur");
		return;
		});
	}
	catch(std::system_error& e)
	{
		throw ProcessusNonLance(_nom,e.what());
	}
#ifdef LINUX_EXTENSIONS
	std::ostringstream oss;
	nom_flux_court(oss);
	SVM::Global::Systeme::nom_thread(_travailleur.native_handle(),oss.str());
#endif
}

void Processus::interactions()
{
	SVM_TRACE("PROCESS " << _nom << " INTERACTIONS");
	for(;;)
	{
		SVM_TRACE("PROCESS " << _nom << " TENTATIVE");
		if((_etat.etat_global()==Etat::Execution::EXECUTION) and (_notification.load(std::memory_order_relaxed)==0))
		{
			SVM_TRACE("PROCESS " << _nom << " EXECUTION");
			return;
		}
		SVM_TRACE("PROCESS " << _nom << " ATTENTE " << (_notification) << " ETAT COURANT: " << Etat::texte(_etat.etat_global()));
		SVM_Synchronisation::AdresseSP source;
		SVM_Systeme::EvenementSP evenement;
		if(not _livraison->recupere(_adresse,source,evenement))
			continue;
		--_notification;
		if(_notification<0)
		{
			SVM_TRACE("Nombre de notifications " << _notification << " invalide pour " << *_adresse );
		}
		auto ordonnanceur = _ordonnanceur.lock();
		switch(evenement->_type)
		{
			case SVM_Systeme::Evenement::Type::EXECUTION:
				{
					if(not ordonnanceur)
					{
						_etat.erreur("Cannot run a process without a scheduler");
						if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
						SVM_TRACE("PROCESS " << _nom << " ERREUR");
						return;
					}
					switch(_etat.etat_global())
					{
						case Etat::Execution::SUSPENDU:
							{
								_etat.execution();
								ordonnanceur->notifie_etat(_adresse,_etat.etat_global());
								if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
							}
							break;
						case Etat::Execution::ATTENTE:
						case Etat::Execution::BLOQUE:
						case Etat::Execution::INVESTIGATION:
							{
								if(evenement->_entier>0)
								{
									std::this_thread::sleep_for(std::chrono::milliseconds(evenement->_entier));
								}
								ordonnanceur->notifie_etat(_adresse,_etat.etat_global());
							}
							break;
						default:
							break;
					}
				}
				break;
			case SVM_Systeme::Evenement::Type::SUSPENSION:
				{
					if(not ordonnanceur)
					{
						_etat.erreur("Cannot suspend a process without a scheduler");
						if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
						SVM_TRACE("PROCESS " << _nom << " ERREUR");
						return;
					}
					switch(_etat.etat_global())
					{
						case Etat::Execution::EXECUTION:
							{
								_etat.suspension();
								ordonnanceur->notifie_etat(_adresse,_etat.etat_global());
								if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
							}
							break;
						default:
							break;
					}
				}
				break;
			case SVM_Systeme::Evenement::Type::TERMINAISON:
				{
					switch(_etat.etat_global())
					{
						case Etat::Execution::EXECUTION:
						case Etat::Execution::SUSPENDU:
						case Etat::Execution::ATTENTE:
						case Etat::Execution::BLOQUE:
						case Etat::Execution::INVESTIGATION:
							{
								_etat.zombie();
								for(auto &i: _verrouillages_en_attente)
								{
									_livraison->poste(i,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::REPONSE,SVM_Systeme::Evenement::Reponse::NON));
								}
								_verrouillages_en_attente.clear();
								if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
								SVM_TRACE("PROCESS " << _nom << " ZOMBIE");
								return;
							}
						default:
							break;
					}
				}
				break;
			case SVM_Systeme::Evenement::Type::VERROUILLAGE:
				{
					switch(_etat.etat_global())
					{
						case Etat::Execution::EXECUTION:
						case Etat::Execution::SUSPENDU:
							{
								_etat.bloque();
								if(static_cast<bool>(ordonnanceur))
								{
									ordonnanceur->notifie_etat(_adresse,_etat.etat_global());
								}
								auto systeme = _systeme.lock();
								systeme->notifie_etat(_adresse,_etat.etat_global());
								_verrouille_par = source;
								_nombre_verrouillage = 1;
								_livraison->poste(source,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::REPONSE,SVM_Systeme::Evenement::Reponse::OUI));
								if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
							}
							break;
						case Etat::Execution::BLOQUE:
							{
								if(source==_verrouille_par)
								{
									++_nombre_verrouillage;
									_livraison->poste(source,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::REPONSE,SVM_Systeme::Evenement::Reponse::DEJA_VERROUILLE));
								}
								else
								{
									if(evenement->_reponse==SVM_Systeme::Evenement::Reponse::OUI)
									{
										_livraison->poste(source,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::REPONSE,SVM_Systeme::Evenement::Reponse::AUTRE_VERROUILLE));
									}
									else
									{
										_verrouillages_en_attente.push_back(source);
										if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
									}
								}
							}
							break;
						case Etat::Execution::INVESTIGATION:
						case Etat::Execution::ATTENTE:
						default:
							break;
					}
				}
				break;
			case SVM_Systeme::Evenement::Type::DEVERROUILLAGE:
				{
					switch(_etat.etat_global())
					{
						case Etat::Execution::BLOQUE:
							if(source==_verrouille_par)
							{
								--_nombre_verrouillage;
								if(_nombre_verrouillage==0)
								{
									if(_verrouillages_en_attente.empty())
									{
										_etat.suspension();
										if(static_cast<bool>(ordonnanceur))
										{
											ordonnanceur->notifie_etat(_adresse,_etat.etat_global());
										}
										_verrouille_par.reset();
										if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
									}
									else
									{
										auto adresse = _verrouillages_en_attente.front();
										_verrouillages_en_attente.pop_front();
										_livraison->poste(adresse,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::REPONSE,SVM_Systeme::Evenement::Reponse::OUI));
										_verrouille_par = adresse;
										if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
									}
								}
							}
							break;
						case Etat::Execution::EXECUTION:
						case Etat::Execution::SUSPENDU:
						case Etat::Execution::ATTENTE:
						case Etat::Execution::INVESTIGATION:
						default:
							break;
					}
				}
				break;
			default:
				break;
		}
	}
}

bool Processus::entree_mode_attente()
{
	_etat.attente();
	auto ordonnanceur = _ordonnanceur.lock();
	if(not ordonnanceur)
		return false;
	ordonnanceur->notifie_etat(_adresse,_etat.etat_global());
	if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
	return true;
}

bool Processus::sortie_mode_attente()
{
	_etat.suspension();
	auto ordonnanceur = _ordonnanceur.lock();
	if(not ordonnanceur)
		return false;
	ordonnanceur->notifie_etat(_adresse,_etat.etat_global());
	if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
	return true;
}

SVM_Systeme::Evenement::Reponse Processus::attente_verrouillage(const SVM_Synchronisation::AdresseSP& verrouillage)
{
	if(not entree_mode_attente())
		return SVM_Systeme::Evenement::Reponse::ERREUR;
	{
		std::lock_guard<std::mutex> verrou(_verrouillage);
		_verrouillage_en_cours = verrouillage;
	}
	SVM_Systeme::Evenement::Reponse reponse=SVM_Systeme::Evenement::Reponse::NON;
	for( ; ; )
	{
		SVM_Synchronisation::AdresseSP source;
		SVM_Systeme::EvenementSP evenement;
		if(not _livraison->recupere_specifique(_adresse,source,evenement,[](const SVM_Synchronisation::AdresseSP& s, const SVM_Systeme::EvenementSP& e) -> bool { return (e->_type==SVM_Systeme::Evenement::Type::REPONSE) or (e->_type==SVM_Systeme::Evenement::Type::VERROUILLAGE); })) continue;
		if(evenement->_type==SVM_Systeme::Evenement::Type::REPONSE)
		{
			reponse = evenement->_reponse;
			break;
		}
		else if(evenement->_type==SVM_Systeme::Evenement::Type::VERROUILLAGE)
		{
			if(verrouillage_mortel(source))
			{
				_livraison->poste(source,_adresse,std::make_shared<SVM_Systeme::Evenement>(SVM_Systeme::Evenement::Type::REPONSE,SVM_Systeme::Evenement::Reponse::VERROU_MORTEL));
			}
			else
			{
				std::lock_guard<std::mutex> verrou(_verrouillage);
				_verrouillages_en_attente.push_back(source);
			}
		}
	}
	sortie_mode_attente();
	if(reponse==SVM_Systeme::Evenement::Reponse::OUI)
	{
		std::lock_guard<std::mutex> verrou(_verrouillage);
		_verrous_en_cours.insert(verrouillage);
	}
	{
		std::lock_guard<std::mutex> verrou(_verrouillage);
		_verrouillage_en_cours.reset();
	}
	return reponse;
}

std::pair<SVM_Systeme::Evenement::Reponse,SVM_Synchronisation::AdresseSP> Processus::attente_verrouillage()
{
	if(not entree_mode_attente())
		return std::make_pair(SVM_Systeme::Evenement::Reponse::ERREUR,SVM_Synchronisation::AdresseSP());
	auto reponse=std::make_pair(SVM_Systeme::Evenement::Reponse::NON,SVM_Synchronisation::AdresseSP());
	for( ; ; )
	{
		SVM_Synchronisation::AdresseSP source;
		SVM_Systeme::EvenementSP evenement;
		if(not _livraison->recupere_specifique(_adresse,source,evenement,[](const SVM_Synchronisation::AdresseSP& s, const SVM_Systeme::EvenementSP& e) -> bool { return (e->_type==SVM_Systeme::Evenement::Type::REPONSE) or (e->_type==SVM_Systeme::Evenement::Type::VERROUILLAGE); })) continue;
		if(evenement->_type==SVM_Systeme::Evenement::Type::REPONSE)
		{
			reponse = std::make_pair(evenement->_reponse,source);
			break;
		}
		else if(evenement->_type==SVM_Systeme::Evenement::Type::VERROUILLAGE)
		{
			std::lock_guard<std::mutex> verrou(_verrouillage);
			_verrouillages_en_attente.push_back(source);
		}
	}
	sortie_mode_attente();
	if(reponse.first==SVM_Systeme::Evenement::Reponse::OUI)
	{
		std::lock_guard<std::mutex> verrou(_verrouillage);
		_verrous_en_cours.insert(reponse.second);
	}
	{
		std::lock_guard<std::mutex> verrou(_verrouillage);
		_verrouillage_en_cours.reset();
	}
	return reponse;
}

SVM_Systeme::Evenement::Reponse Processus::attente_reponse(const SVM_Synchronisation::AdresseSP& origine)
{
	if(not entree_mode_attente())
		return SVM_Systeme::Evenement::Reponse::ERREUR;
	SVM_Systeme::Evenement::Reponse reponse=SVM_Systeme::Evenement::Reponse::NON;
	for( ; ; )
	{
		SVM_Synchronisation::AdresseSP source;
		SVM_Systeme::EvenementSP evenement;
		if(not _livraison->recupere_specifique(_adresse,source,evenement,[&origine](const SVM_Synchronisation::AdresseSP& s, const SVM_Systeme::EvenementSP& e) -> bool { return (e->_type==SVM_Systeme::Evenement::Type::REPONSE) and (s==origine);})) continue;
		reponse = evenement->_reponse;
		break;
	}
	sortie_mode_attente();
	return reponse;
}

void Processus::accepte_notification_interruption()
{
	if(_accepte_notification_interruption) return;
#ifdef LINUX_EXTENSIONS
	sigset_t sigset;
	SVM::Global::Systeme::sigset(&sigset,REACTIVE_SIGNAL);
	::pthread_sigmask(SIG_UNBLOCK,&sigset,nullptr);
#endif
	_accepte_notification_interruption=true;
}

void Processus::ignore_notification_interruption()
{
	if(not _accepte_notification_interruption) return;
	_accepte_notification_interruption=false;
#ifdef LINUX_EXTENSIONS
	sigset_t sigset;
	SVM::Global::Systeme::sigset(&sigset,REACTIVE_SIGNAL);
	::pthread_sigmask(SIG_BLOCK,&sigset,nullptr);
#endif
}

void Processus::attend_notification_interruption()
{
#ifdef LINUX_EXTENSIONS
	sigset_t sigset;
	SVM::Global::Systeme::sigset(&sigset,REACTIVE_SIGNAL);
	::pthread_sigmask(SIG_UNBLOCK,&sigset,nullptr);
	int s = 0;
	bool sauvegarde = _accepte_notification_interruption;
	_accepte_notification_interruption=true;
	::sigwait(&sigset,&s);
	if(sauvegarde) return;
	_accepte_notification_interruption=false;
	::pthread_sigmask(SIG_BLOCK,&sigset,nullptr);
#endif
}

void Processus::notification_interruption()
{
	SVM_TRACE("PROCESS " << _nom << " notification interruption ?");
#ifdef LINUX_EXTENSIONS
	if(_accepte_notification_interruption.load(std::memory_order_relaxed))
	{
		SVM_TRACE("PROCESS " << _nom << " notification interruption");
		_notification_recue->debloque();
		::pthread_kill(_travailleur.native_handle(),REACTIVE_SIGNAL);
	}
#endif
}

void Processus::force_deverrouillage(const SVM_Synchronisation::AdresseSP& adresse)
{
	std::lock_guard<std::mutex> verrou(_verrouillage);
	auto it=_verrous_en_cours.find(adresse);
	if(it!=_verrous_en_cours.end())
	{
		_verrous_en_cours.erase(it);
	}
}

void Processus::mode_debugueur(const bool mode)
{
	auto ordonnanceur = _ordonnanceur.lock();
	if(not ordonnanceur) return;
	_etat.debugueur(mode);
	ordonnanceur->notifie_etat(_adresse,_etat.etat_global());
	if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
}

SVM_Debugueur::JSON_Interne::ObjetSP PiegeProcessus::mise_a_jour()
{
	auto p = _processus.lock();
	if(not p)
	{
		throw SVM_Debugueur::PasDeMiseAJour();
	}
	std::ostringstream oss;
	std::lock_guard<std::mutex> verrou(p->_verrouillage);
	oss << p->_etat;
	auto o = SVM_Debugueur::JSON::objet({ { "name", SVM_Debugueur::JSON::chaine(p->nom_debug()) } , { "state", SVM_Debugueur::JSON::chaine(p->_etat.etat()) } , { "full_state", SVM_Debugueur::JSON::chaine(oss.str()) } });
	if(static_cast<bool>(p->_verrouille_par))
	{
		oss.str("");
		p->_verrouille_par->format(oss);
		o->ajoute("locked_by", SVM_Debugueur::JSON::chaine(oss.str()));
	}
	if(not p->_verrouillages_en_attente.empty())
	{
		auto t = SVM_Debugueur::JSON::tableau();
		for(const auto& v:p->_verrouillages_en_attente)
		{
			oss.str("");
			v->format(oss);
			t->ajoute(SVM_Debugueur::JSON::chaine(oss.str()));
		}
		o->ajoute("waiting_lock", t);
	}
	if(not p->_verrous_en_cours.empty())
	{
		auto t = SVM_Debugueur::JSON::tableau();
		for(const auto& v:p->_verrous_en_cours)
		{
			oss.str("");
			v->format(oss);
			t->ajoute(SVM_Debugueur::JSON::chaine(oss.str()));
		}
		o->ajoute("locking", t);
	}
	if(static_cast<bool>(p->_sequenceur->piege()))
	{
		o->ajoute("sequencer", SVM_Debugueur::JSON::entier(p->_sequenceur->piege()->identifiant()));
	}
	if(static_cast<bool>(p->_noyau_courant))
	{
		if(static_cast<bool>(p->_noyau_courant->_piege))
		{
			o->ajoute("current_kernel",p->_noyau_courant->_piege->objet());
		}
	}
	{
		auto t = SVM_Debugueur::JSON::tableau();
		for(const auto& n: p->_noyaux)
		{
			if(static_cast<bool>(n->_piege))
			{
				t->ajoute(n->_piege->objet());
			}
		}
		o->ajoute("kernels",t);
	}
	return o;
}
