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

#include <src/machine/elements/processeur/processeur.h>
#include <src/machine/machine/machine.h>
#include <src/machine/elements/processus/processus.h>
#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/valeur/adresse_instruction.h>
#include <src/machine/elements/valeur/code.h>
#include <src/machine/elements/valeur/instruction.h>
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/elements/memoire/booleen.h>

namespace SVM_Noyau = SVM::Machine::Element::Noyau;
namespace SVM_Valeur = SVM::Machine::Element::Valeur;

using namespace SVM::Machine::Element::Processeur;

ProcesseurSP Processeur::creation_processeur(SVM_Debugueur::DebugueurSP& debugueur, const SVM_Valeur::AdresseInstruction& code, const SVM_Noyau::ControleAccesSP& controle_acces)
{
	auto processeur = std::make_shared<Processeur>(code,controle_acces);
	processeur->_piege = SVM_Debugueur::Debugueur::ajoute_piege<PiegeProcesseur>(debugueur,processeur);
	return processeur;
}

template<enum Processeur::Mode mode>
void Processeur::execution(const SVM_Noyau::NoyauSP& noyau)
{
	auto m = noyau->machine();
	auto performance = m->_base->_performance;
	SVM_Valeur::ArretSP arret;
	for( ; ; )
	{
		try
		{
			if(noyau->pause())
				return;
			//std::cerr << *(noyau->_processeur) << std::endl;
			//std::cerr << *(noyau->_memoire) << std::endl;
			{
				_gestionnaire_interruptions_globales.interruptions_materielles(_interruptions_en_attente);
				if(static_cast<bool>(_piege)) { _piege->interruptions_en_attente(_interruptions_en_attente,true); }
				if(_interruptions_activees.load(std::memory_order_relaxed) and not _interruptions_en_attente.empty())
				{
					SVM_TRACE("Interruption en attente a traiter");
					while(not _interruptions_en_attente.empty())
					{
						auto i = _interruptions_en_attente.front();
						SVM_Valeur::AdresseInstructionSP adresse_interruption = this->gestionnaire_interruption(i);
						if(not adresse_interruption)
						{
							throw i;
						}
						this->appel(*adresse_interruption,memoire_courante());
						_interruptions_en_attente.pop_front();
						if(static_cast<bool>(_piege)) { _piege->interruptions_en_attente(_interruptions_en_attente); }
					}
					SVM_TRACE("Interruption en attente traitees");
				}
			}
			SVM_Valeur::AdresseInstruction adresse = this->prochaine_instruction();
			SVM_Valeur::InstructionSP instruction = adresse.instruction();
			bool remplacement=false;
			auto instruction_remplacement = this->remplacementinstructions(adresse);
			if(static_cast<bool>(instruction_remplacement))
			{
				instruction = instruction_remplacement;
				remplacement=true;
			}
			if(not instruction)
			{
				throw SVM_Valeur::Arret();
			}
			if(static_cast<bool>(_piege)) { _piege->arret(); }
			if(instruction->_attente)
			{
				noyau->etat_attente();
			}
			const auto debut = std::chrono::steady_clock::now();
			const size_t taille = taille_pile();
			try
			{
				if(mode == Mode::PROTEGE)
				{
					if(instruction->_systeme)
					{
						throw SVM_Noyau::InstructionSystemeExecuteeEnModeProtege(instruction);
					}
					if(static_cast<bool>(_utilisation))
					{
						_utilisation->incremente<SVM_Noyau::TropDInstructionsExecutees>();
						if(static_cast<bool>(_piege)) { _piege->quota(_utilisation); }
					}
				}
				instruction->execution(noyau);
			}
			catch(const SVM_Valeur::ExceptionExecution& e)
			{
				SVM_Valeur::Interruption i(e);
				i.position(instruction->_position);
				if(mode == Mode::PROTEGE)
				{
					if(i == SVM_Valeur::Interruption::InterruptionInterne::SECURITE)
					{
						throw i;
					}
				}
				_interruptions_en_attente.push_back(i);
				if(static_cast<bool>(_piege)) { _piege->interruptions_en_attente(_interruptions_en_attente); }
			}
			catch(SVM_Valeur::Interruption& i)
			{
				i.position(instruction->_position);
				_interruptions_en_attente.push_back(i);
				if(static_cast<bool>(_piege)) { _piege->interruptions_en_attente(_interruptions_en_attente); }
			}
			catch(const SVM_Valeur::Arret& a)
			{
				arret = std::make_shared<SVM_Valeur::Arret>(a);
			}
			catch(const std::exception& e)
			{
				SVM_Valeur::Interruption i(SVM_Valeur::Interruption::InterruptionInterne::GENERIQUE,e.what());
				i.position(instruction->_position);
				_interruptions_en_attente.push_back(i);
				if(static_cast<bool>(_piege)) { _piege->interruptions_en_attente(_interruptions_en_attente); }
			}
			catch(...)
			{
				SVM_Valeur::Interruption i(SVM_Valeur::Interruption::InterruptionInterne::GENERIQUE,"Unexpected error");
				i.position(instruction->_position);
				_interruptions_en_attente.push_back(i);
				if(static_cast<bool>(_piege)) { _piege->interruptions_en_attente(_interruptions_en_attente); }
			}
			const auto fin = std::chrono::steady_clock::now();
			if(static_cast<bool>(performance))
			{
				performance->evenement(adresse,remplacement,debut,fin,noyau,taille);
			}
			if(instruction->_attente)
			{
				noyau->etat_execution();
				auto processus = noyau->_processus.lock();
				if(static_cast<bool>(processus))
				{
					if(processus->etat_global()==SVM_Processus::Etat::Execution::ATTENTE)
					{
						processus->sortie_mode_attente();
					}
					processus->ignore_notification_interruption();
				}
			}
			if(static_cast<bool>(arret))
			{
				throw (*arret);
			}
		}
		catch(const SVM_Valeur::Interruption& i)
		{
			if(noyau->_etat._transmission_interruption_demandee)
			{
				noyau->etat_interrompu(std::make_shared<SVM_Valeur::Interruption>(i));
				noyau->trace();
			}
			else
			{
				noyau->etat_zombie();
			}
			break;
		}
		catch(const SVM_Valeur::Arret& a)
		{
			noyau->etat_zombie(a);
			break;
		}
	}
}

template void Processeur::execution<Processeur::Mode::NORMAL>(const SVM_Noyau::NoyauSP& noyau);
template void Processeur::execution<Processeur::Mode::PROTEGE>(const SVM_Noyau::NoyauSP& noyau);

PiegeProcesseur::PiegeProcesseur(const ProcesseurWP& processeur)
:SVM_Debugueur::Piege("processor"), _processeur(processeur), _arret_global(false), _arret_local(0)
{
}

SVM_Debugueur::JSON_Interne::ObjetSP PiegeProcesseur::mise_a_jour()
{
	auto processeur = _processeur.lock();
	if(not processeur)
	{
		throw SVM_Debugueur::PasDeMiseAJour();
	}
	auto p = SVM_Debugueur::JSON::objet();
	p->ajoute("state",processeur->_etat.debugueur());
	auto pile = SVM_Debugueur::JSON::tableau();
	for(const auto& e: processeur->_etats_sauvegardes)
	{
		pile->ajoute(e.debugueur());
	}
	p->ajoute("return_stack",pile);
	p->ajoute("global_interruptions",processeur->_gestionnaire_interruptions_globales.debugueur());
	p->ajoute("interruptions_enabled",SVM_Debugueur::JSON::booleen(processeur->_interruptions_activees.load(std::memory_order_relaxed)));
	auto ia = SVM_Debugueur::JSON::tableau();
	for(const auto& i: processeur->_interruptions_en_attente)
	{
		ia->ajoute(i.debugueur());
	}
	p->ajoute("waiting_interruptions",ia);
	if(static_cast<bool>(processeur->_utilisation))
	{
		p->ajoute("quota",processeur->_utilisation->debugueur());
	}
	p->ajoute("global_instruction_overrides",Etat::debugueur(processeur->_remplacement_instructions_global));
	auto pa = SVM_Debugueur::JSON::tableau();
	for(const auto& a:_points_arret)
	{
		pa->ajoute(a.debugueur());
	}
	p->ajoute("breakpoints",pa);
	return p;
}

void PiegeProcesseur::evenement(const SVM_Debugueur::JSON_Interne::ObjetSP& evenement)
{
	auto debug = debugueur();
	auto ajout = SVM_Debugueur::JSON::objet((*evenement)["add_breakpoint"]);
	if(SVM_Debugueur::JSON::est(ajout))
	{
		auto code = SVM_Debugueur::JSON::entier((*ajout)["code"]);
		auto local = SVM_Debugueur::JSON::entier((*ajout)["local"]);
		if(SVM_Debugueur::JSON::est(code) and SVM_Debugueur::JSON::est(local))
		{
			auto piege_code = debug->recupere_piege<SVM::Machine::Element::Valeur::PiegeCode>(*code);
			auto vrai_code = piege_code->_code.lock();
			if(static_cast<bool>(vrai_code->instruction(*local)))
			{
				SVM_Valeur::AdresseInstruction adresse(vrai_code,*local);
				ajoute_point_arret(adresse);
			}
		}
	}
	auto retrait = SVM_Debugueur::JSON::objet((*evenement)["remove_breakpoint"]);
	if(SVM_Debugueur::JSON::est(retrait))
	{
		auto code = SVM_Debugueur::JSON::entier((*retrait)["code"]);
		auto local = SVM_Debugueur::JSON::entier((*retrait)["local"]);
		if(SVM_Debugueur::JSON::est(code) and SVM_Debugueur::JSON::est(local))
		{
			auto piege_code = debug->recupere_piege<SVM::Machine::Element::Valeur::PiegeCode>(*code);
			auto vrai_code = piege_code->_code.lock();
			if(static_cast<bool>(vrai_code->instruction(*local)))
			{
				SVM_Valeur::AdresseInstruction adresse(vrai_code,*local);
				enleve_point_arret(adresse);
			}
		}
	}
	auto pause = SVM_Debugueur::JSON::booleen((*evenement)["pause"]);
	if(SVM_Debugueur::JSON::est(pause))
	{
		std::lock_guard<std::mutex> verrou(_protection);
		_arret_global = true;
		SVM_TRACE("Pause demandee");
	}
	auto explication = SVM_Debugueur::JSON::booleen((*evenement)["explain"]);
	if(SVM_Debugueur::JSON::est(explication))
	{
		auto processeur = _processeur.lock();
		if(static_cast<bool>(processeur))
		{
			auto noyau = processeur->_noyau.lock();
			auto adresse_courante = processeur->instruction_courante();
			auto instruction = adresse_courante.instruction();
			auto texte = instruction->texte_html();
			std::list<std::string> explications;
			instruction->explication(noyau,explications);
			auto je = SVM_Debugueur::JSON::tableau();
			for(const auto& i: explications)
			{
				je->ajoute(SVM_Debugueur::JSON::chaine(i));
			}
			auto e = SVM_Debugueur::JSON::objet({ { "explain" , SVM_Debugueur::JSON::objet({ { "text", SVM_Debugueur::JSON::chaine(texte) } , { "parameters", je } }) } } );
			notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::RESULTAT,e);
		}
	}
}

void PiegeProcesseur::instruction_courante(const SVM_Valeur::AdresseInstruction& courante, const SVM_Valeur::AdresseInstruction& prochaine, const bool point_arret)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("current_instruction",courante.debugueur());
	e->ajoute("next_instruction",prochaine.debugueur());
	notification_evenement_objet(point_arret?SVM_Debugueur::Evenement::Categorie::RESULTAT:SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::prochaine_instruction(const SVM_Valeur::AdresseInstruction& a)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("next_instruction",a.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::etat(const Etat& courant)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("state",courant.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::memoire_courante(const SVM_Valeur::Pointeur& courante)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("current_memory",courante.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::memoire_allouee(const std::set<SVM_Valeur::Pointeur>& allouee)
{
	auto e = SVM_Debugueur::JSON::objet();
	auto ma = SVM_Debugueur::JSON::tableau();
	for(const auto& m: allouee)
	{
		ma->ajoute(m.debugueur());
	}
	e->ajoute("allocated_memory",ma);
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::alias_definis(const std::set<std::string>& alias)
{
	auto e = SVM_Debugueur::JSON::objet();
	auto ad = SVM_Debugueur::JSON::tableau();
	for(const auto& a: alias)
	{
		ad->ajoute(SVM_Debugueur::JSON::chaine(a));
	}
	e->ajoute("defined_aliases",ad);
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::interruptions_locales(const Interruptions& locales)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("local_interruptions",locales.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::interruptions_locales_cascadees(const Interruptions& locales)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("cascaded_local_interruptions",locales.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::interruptions_globales(const Interruptions& globales)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("global_interruptions",globales.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::remplacementinstructions_local(const Etat::RemplacementInstructions& locales)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("instruction_overrides",Etat::debugueur(locales));
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::remplacementinstructions_local_cascadees(const Etat::RemplacementInstructions& locales)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("cascaded_instruction_overrides",Etat::debugueur(locales));
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::remplacementinstructions_global(const Etat::RemplacementInstructions& globales)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("global_instruction_overrides",Etat::debugueur(globales));
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}


void PiegeProcesseur::drapeaux(const std::set<std::string>& drapeaux)
{
	auto e = SVM_Debugueur::JSON::objet();
	auto dd = SVM_Debugueur::JSON::tableau();
	for(const auto& d: drapeaux)
	{
		dd->ajoute(SVM_Debugueur::JSON::chaine(d));
	}
	e->ajoute("flags",dd);
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::drapeaux_cascades(const std::set<std::string>& drapeaux)
{
	auto e = SVM_Debugueur::JSON::objet();
	auto dd = SVM_Debugueur::JSON::tableau();
	for(const auto& d: drapeaux)
	{
		dd->ajoute(SVM_Debugueur::JSON::chaine(d));
	}
	e->ajoute("cascaded_flags",dd);
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::interruptions_en_attente(const std::list<SVM_Valeur::Interruption>& interruptions, const bool point_arret)
{
	auto e = SVM_Debugueur::JSON::objet();
	auto ia = SVM_Debugueur::JSON::tableau();
	for(const auto& i: interruptions)
	{
		ia->ajoute(i.debugueur());
	}
	e->ajoute("waiting_interruptions",ia);
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
	if(not point_arret) return;
	auto processeur = _processeur.lock();
	if(not processeur) return;
	bool arret = false;
	std::ostringstream oss;
	oss << "With:";
	for(const auto& i : processeur->_interruptions_en_attente)
	{
		if(_interruptions.find(i)!=_interruptions.end())
		{
			arret = true;
			oss << " " << i;
		}
	}
	if(arret)
	{
		this->point_arret("Interruption",false,oss.str());
	}
}

void PiegeProcesseur::empile(const Etat& etat)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("push_state",etat.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::depile(const Etat& etat)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("pop_state",etat.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegeProcesseur::quota(const SVM_Noyau::LimiteUtilisationSP& utilisation)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("quota",utilisation->debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

bool PiegeProcesseur::arret_ephemere() const
{
	auto processeur = _processeur.lock();
	if(not processeur) return false;
	std::lock_guard<std::mutex> verrou(_protection);
	return _arret_global or (processeur->_etats_sauvegardes.size()<_arret_local) or (_points_arret.find(processeur->instruction_courante())!=_points_arret.end());
}

void PiegeProcesseur::arret()
{
	{
		std::lock_guard<std::mutex> verrou(_protection);
		if(not _points_arret_externes.empty())
		{
			auto debug = debugueur();
			auto processeur = _processeur.lock();
			if(static_cast<bool>(processeur))
			{
				std::ostringstream oss;
				oss << "At ";
				processeur->instruction_courante().html(oss);
				oss << "\n";
				auto noyau = processeur->_noyau.lock();
				if(static_cast<bool>(noyau))
				{
					auto processus = noyau->_processus.lock();
					if(static_cast<bool>(processus))
					{
						
						for(const auto& pa: _points_arret_externes)
						{
							debug->_points_arret->ajout_point_arret(SVM_Debugueur::PointArret(pa._type,oss.str()+pa._message,noyau->_piege->identifiant(),processus->nom_debug(),noyau->nom_debug()));
						}
					}
				}
			}
			_points_arret_externes.clear();
			_arret_global = true;
		}
	}
	if(arret_ephemere())
	{
		{
			std::lock_guard<std::mutex> verrou(_protection);
			_arret_global = false;
			_arret_local = 0;
		}
		point_arret("",true);
	}
}

void PiegeProcesseur::point_arret(const std::string& type, const bool ephemere, const std::string& message)
{
	auto processeur = _processeur.lock();
	if(not processeur) return;
	auto noyau = processeur->_noyau.lock();
	if(not noyau) return;
	auto processus = noyau->_processus.lock();
	if(not processus) return;
	noyau->mode_debugueur(true);
	noyau->_piege->notification_changement_objet();
	processus->mode_debugueur(true);
	auto debug = debugueur();
	auto p = this->shared_from_this();
	std::ostringstream oss;
	oss << "At ";
	processeur->instruction_courante().html(oss);
	oss << "\n";
	SVM_TRACE("Avant envoi point arret");
	debug->notification_evenement_objet(p,SVM_Debugueur::Evenement::Categorie::POINT_ARRET,SVM_Debugueur::JSON::objet({ { "breakpoint" , SVM_Debugueur::JSON::objet({ { "type" , SVM_Debugueur::JSON::chaine(type) } , { "ephemeral" , SVM_Debugueur::JSON::booleen(ephemere) } , { "message" , SVM_Debugueur::JSON::chaine(oss.str()+message) } }) } }));
	if(not ephemere)
	{
		SVM_TRACE("Avant ajout point arret a liste");
		debug->_points_arret->ajout_point_arret(SVM_Debugueur::PointArret(type,oss.str()+message,noyau->_piege->identifiant(),processus->nom_debug(),noyau->nom_debug()));
		SVM_TRACE("Apres ajout point arret a liste");
	}
	SVM_TRACE("Avant attente commande");
	for( ; ; )
	{
		auto commande = debug->attente_commande(p);
		SVM_TRACE("Apres attente commande");
		auto nom = SVM_Debugueur::JSON::chaine((*commande)["command"]);
		if(not (SVM_Debugueur::JSON::est(nom) and (static_cast<std::string>(*nom)=="run")))
			continue;
		auto type = SVM_Debugueur::JSON::chaine((*commande)["type"]);
		if(SVM_Debugueur::JSON::est(type))
		{
			if(static_cast<std::string>(*type)=="instruction")
			{
				std::lock_guard<std::mutex> verrou(_protection);
				_arret_global = true;
				_arret_local = 0;
				SVM_TRACE("Execution instruction demandee");
				break;
			}
			else if(static_cast<std::string>(*type)=="function")
			{
				std::lock_guard<std::mutex> verrou(_protection);
				_arret_global = false;
				_arret_local=processeur->_etats_sauvegardes.size()+1;
				SVM_TRACE("Execution fonction demandee");
				break;
			}
			else if(static_cast<std::string>(*type)=="return")
			{
				auto niveau = SVM_Debugueur::JSON::entier((*commande)["level"]);
				if(SVM_Debugueur::JSON::est(niveau))
				{
					std::lock_guard<std::mutex> verrou(_protection);
					_arret_global = false;
					_arret_local= *niveau;
					SVM_TRACE("Execution retour demandee");
					break;
				}
			}
		}
		else
		{
			std::lock_guard<std::mutex> verrou(_protection);
			_arret_global = false;
			_arret_local = 0;
			SVM_TRACE("Execution demandee");
			break;
		}
	}
	noyau->mode_debugueur(false);
	noyau->_piege->notification_changement_objet();
	processus->mode_debugueur(false);
}

void PiegeProcesseur::ajoute_interruption(const SVM_Valeur::Interruption& interruption)
{
	std::lock_guard<std::mutex> verrou(_protection);
	_interruptions.insert(interruption);
}

void PiegeProcesseur::enleve_interruption(const SVM_Valeur::Interruption& interruption)
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto it = _interruptions.find(interruption);
	if(it!=_interruptions.end())
	{
		_interruptions.erase(it);
	}
}

void PiegeProcesseur::point_arret_externe(const std::string& type, const std::string& message)
{
	std::lock_guard<std::mutex> verrou(_protection);
	_points_arret_externes.push_back(PointArret(type,message));
}

void PiegeProcesseur::ajoute_point_arret(const SVM_Valeur::AdresseInstruction& adresse)
{
	std::lock_guard<std::mutex> verrou(_protection);
	_points_arret.insert(adresse);
	SVM_TRACE("Ajout point arret " << adresse);
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("add_breakpoint",adresse.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::RESULTAT,e);
}

void PiegeProcesseur::enleve_point_arret(const SVM_Valeur::AdresseInstruction& adresse)
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto it = _points_arret.find(adresse);
	if(it!=_points_arret.end())
	{
		_points_arret.erase(it);
		SVM_TRACE("Retrait point arret " << adresse);
	}
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("remove_breakpoint",adresse.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::RESULTAT,e);
}

void PiegeProcesseur::utilise_interruptions(const bool activation)
{
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("interruptions_enabled",SVM_Debugueur::JSON::booleen(activation));
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}
