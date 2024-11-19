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

#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/machine/machine.h>
#include <src/machine/elements/processus/processus.h>
#include <src/machine/elements/systeme/systeme.h>
#include <src/machine/machine/historique.h>
#include <src/machine/machine/machine.h>
#include <src/machine/debugueur/debugueur.h>

using namespace SVM::Machine::Element::Noyau;

NoyauSP Noyau::creation_noyau(SVM_Debugueur::DebugueurSP& debugueur, const bool transmission_interruption_demandee, const bool retour_est_arret, const bool mode_protege, const ControleAccesSP& controle_acces, const SVM_Valeur::AdresseInstruction& code, const SVM_Memoire::MemoireSP& memoire)
{
	auto noyau = std::make_shared<Noyau>(debugueur,transmission_interruption_demandee,retour_est_arret,mode_protege,controle_acces,code,memoire);
	noyau->_processeur->_noyau = noyau;
	noyau->_memoire->_noyau = noyau;
	noyau->_piege = SVM_Debugueur::Debugueur::ajoute_piege<PiegeNoyau>(debugueur,noyau);
	static std::atomic<unsigned long long> identifiant(0);
	noyau->_identifiant = ++identifiant;
	return noyau;
}

Noyau::Noyau(SVM_Debugueur::DebugueurSP& debugueur, const bool transmission_interruption_demandee, const bool retour_est_arret, const bool mode_protege, const ControleAccesSP& controle_acces, const SVM_Valeur::AdresseInstruction& code, const SVM_Memoire::MemoireSP& memoire)
:_etat(transmission_interruption_demandee,mode_protege,retour_est_arret), _memoire(memoire), _controle_acces(mode_protege?controle_acces:SVM_Noyau::ControleAccesSP())
{
	_processeur = SVM_Processeur::Processeur::creation_processeur(debugueur,code,_controle_acces);
	if(not static_cast<bool>(memoire))
	{
		_memoire = SVM_Memoire::Memoire::creation_memoire(debugueur,_controle_acces);
	}
	_nom = code.nom_noyau();
}

const Etat& Noyau::execution()
{
	std::lock_guard<std::mutex> protege(_execution);
	bool changement;
	bool fin = not _etat.execution(_processus.lock(),changement);
	if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
	if(fin)
	{
		return _etat;
	}
	if(_etat._mode_protege)
	{
		_processeur->execution<SVM_Processeur::Processeur::Mode::PROTEGE>(this->shared_from_this());
	}
	else
	{
		_processeur->execution<SVM_Processeur::Processeur::Mode::NORMAL>(this->shared_from_this());
	}
	return _etat;
}

bool Noyau::pause()
{
	auto processus = _processus.lock();
	if(not processus)
		return true;
	if(not processus->en_execution())
		return true;
	bool changement;
	auto p = not _etat.en_execution(changement);
	if(changement and static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
	return p;
}

SVM::Machine::MachineSP Noyau::machine()
{
	return _processus.lock()->_systeme.lock()->_machine.lock(); 
}

SVM_Debugueur::DebugueurSP& Noyau::debugueur()
{
	return _processus.lock()->_base.lock()->_debugueur; 
}

SVM::Machine::Interruption::InterruptionSP& Noyau::gestionnaire_interruptions_materielles()
{
	return _processus.lock()->_base.lock()->_gestionnaire_interruptions_materielles; 
}

SVM_Interface::VariablesDefiniesSP Noyau::variables_extension(const std::string& nom_extension) const
{
	auto processus = _processus.lock();
	if(not processus)
		return std::make_shared<SVM_Interface::VariablesDefinies>();
	auto systeme = processus->_systeme.lock();
	if(not systeme)
		return std::make_shared<SVM_Interface::VariablesDefinies>();
	return systeme->variables_extension(nom_extension);
}

void Noyau::trace() const
{
	auto processus = _processus.lock();
	if(not processus) return;
	auto systeme = processus->_systeme.lock();
	if(not systeme) return;
	auto machine = systeme->_machine.lock();
	if(not machine) return;
	std::ostringstream oss;
	processus->nom_flux(oss);
	SVM::Machine::Trace trace(machine->_historique,oss.str());
	trace << "Kernel interrupted: ";
	_etat.interrompu()->erreur(trace);
	trace << "" << std::endl << std::endl << "Core dump:" << std::endl << (*this);
}

PiegeNoyau::PiegeNoyau(const NoyauWP& noyau)
:Piege("kernel"), _noyau(noyau)
{
}

SVM_Debugueur::JSON_Interne::ObjetSP PiegeNoyau::mise_a_jour()
{
	auto n = _noyau.lock();
	if(not n)
	{
		throw SVM_Debugueur::PasDeMiseAJour();
	}
	std::ostringstream oss;
	oss << n->_etat;
	auto o = SVM_Debugueur::JSON::objet({ { "name" , SVM_Debugueur::JSON::chaine(n->nom_debug()) } , { "state" , SVM_Debugueur::JSON::chaine(n->_etat.etat()) } , { "full_state" , SVM_Debugueur::JSON::chaine(oss.str()) } , { "processor" , SVM_Debugueur::JSON::entier(n->_processeur->_piege->identifiant()) } , { "memory" , SVM_Debugueur::JSON::entier(n->_memoire->_piege->identifiant()) } });
	auto p = n->_processus.lock();
	if(not p)
	{
		o->ajoute("process",SVM_Debugueur::JSON::chaine("orphan"));
	}
	else
	{
		o->ajoute("process",SVM_Debugueur::JSON::chaine(p->nom_debug()));
	}
	return o;
}

void PiegeNoyau::cascade()
{
	auto n = _noyau.lock();
	if(not n) return;
	auto s = n->_sequenceur.lock();
	if(not s) return;
	if(static_cast<bool>(s->piege()))
	{
		s->piege()->notification_changement_objet();
	}
}
