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
#include <src/machine/interruption/interruption.h>
#include <src/machine/machine/machine.h>

using namespace SVM::Machine::Interruption;

void gestionnairesignalvide(int signal)
{
}

void Interruption::bloque_signaux()
{
	sigset_t signaux_bloques;
	::sigfillset(&signaux_bloques);
	::sigdelset(&signaux_bloques,SIGSTOP);
	::sigdelset(&signaux_bloques,SIGTSTP);
	::sigdelset(&signaux_bloques,SIGCONT);
	::sigdelset(&signaux_bloques,SIGSEGV);
	::sigprocmask(SIG_BLOCK,&signaux_bloques,nullptr);
}

Interruption::~Interruption()
{
	SVM_TRACE("destruction interruptions");
	::kill(::getpid(),SIGTERM);
	_effecteur.join();
}

InterruptionSP Interruption::creation_interruptions()
{
	InterruptionSP interruption = std::make_shared<Interruption>();
	struct sigaction sigaction;
	sigset_t sigset;
	::sigemptyset(&sigset);
	sigaction.sa_handler=::gestionnairesignalvide;
	sigaction.sa_flags=0;
	sigaction.sa_mask=sigset;
	::sigaction(REACTIVE_SIGNAL,&sigaction,nullptr);
	try
	{
		InterruptionWP avatar = interruption;
		interruption->_effecteur = std::thread([avatar] ()
			{
				SVM_TRACE("creation HwInt");
				sigset_t signaux_attendus;
				::sigfillset(&signaux_attendus);
				for( ; ; )
				{
					int signal_recu = -1;
					SVM_TRACE("Attente signal");
					::sigwait(&signaux_attendus,&signal_recu);
					if(signal_recu<0)
						continue;
					SVM_TRACE("Signal recu: " << signal_recu);
					auto interruptions = avatar.lock();
					if(not interruptions)
					{
						SVM_TRACE("plus d'interruptions");
						break;
					}
					interruptions->interruptions_materielles(interruptions->_systeme.lock(),signal_recu);
				}
				SVM_TRACE("destruction HwInt");
			});
#ifdef LINUX_EXTENSIONS
		SVM::Global::Systeme::nom_thread(interruption->_effecteur.native_handle(),"HwInt");
#endif
	}
	catch(std::system_error& e)
	{
		throw InterruptionNonLance(e.what());
	}
	return interruption;
}

void Interruption::interruptions_materielles(const SVM::Machine::Element::Systeme::SystemeSP& systeme, const int signal_recu)
{
	std::lock_guard<std::mutex> verrou(_protection);
	SVM_Valeur::Interruption::InterruptionInterne interne;
	bool quitter = false;
	switch(signal_recu)
	{
		case SIGQUIT:
			quitter = true;
		case SIGABRT:
			interne = SVM_Valeur::Interruption::InterruptionInterne::ECHEC;
			break;
		case SIGALRM:
		case SIGTERM:
		case SIGINT:
		case SIGHUP:
			interne = SVM_Valeur::Interruption::InterruptionInterne::TERMINAISON;
			break;
		case SIGILL:
			interne = SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR;
			break;
		case SIGFPE:
			interne = SVM_Valeur::Interruption::InterruptionInterne::NUMERIQUE;
			break;
		case SIGPIPE:
		case SIGBUS:
			interne = SVM_Valeur::Interruption::InterruptionInterne::PERIPHERIQUE;
			break;
		case SIGCHLD:
			interne = SVM_Valeur::Interruption::InterruptionInterne::CLONE;
			break;
		case SIGUSR1:
			interne = SVM_Valeur::Interruption::InterruptionInterne::PREMIER;
			break;
		case SIGUSR2:
			interne = SVM_Valeur::Interruption::InterruptionInterne::SECOND;
			break;
		case SIGTTIN:
		case SIGTTOU:
		case SIGWINCH:
			interne = SVM_Valeur::Interruption::InterruptionInterne::TERMINAL;
			break;
		case SIGSEGV:
			::kill(::getpid(),SIGSEGV);
			break;
		default:
			interne = SVM_Valeur::Interruption::InterruptionInterne::GENERIQUE;
			break;
	};
	bool interruption_materielle = false;
	auto it = _interruptions.find(interne);
	if(it!=_interruptions.end())
	{
		interruption_materielle = not it->second.empty();
	}
	if(not interruption_materielle)
	{
		switch(interne)
		{
			case SVM_Valeur::Interruption::InterruptionInterne::CLONE:
				::waitpid(-1,nullptr,WNOHANG);
				break;
			case SVM_Valeur::Interruption::InterruptionInterne::TERMINAISON:
				{
					if(static_cast<bool>(systeme))
					{
						systeme->terminaison_demandee();
					}
				}
				break;
			case SVM_Valeur::Interruption::InterruptionInterne::ECHEC:
			case SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR:
			case SVM_Valeur::Interruption::InterruptionInterne::NUMERIQUE:
			case SVM_Valeur::Interruption::InterruptionInterne::PERIPHERIQUE:
				if(quitter)
				{
					sigset_t masque;
					SVM::Global::Systeme::sigset(&masque,SIGQUIT);
					::sigprocmask(SIG_UNBLOCK,&masque,nullptr);
					::kill(::getpid(),SIGQUIT);
				}
				std::abort();
				break;
			default:
				break;
		}
	}
	else
	{
		for(auto& c: it->second)
		{
			c->ecriture();
		}
		systeme->interruption_materielle();
	}
}	

void Interruption::ajoute(const SVM_Valeur::Interruption::InterruptionInterne interruption, const CompteurInterruptionSP& compteur)
{
	ASSERT(interruption!=SVM_Valeur::Interruption::InterruptionInterne::INTERRUPTION_EXTENSION,"interruption materielle externe");
	std::lock_guard<std::mutex> verrou(_protection);
	auto it = _interruptions.find(interruption);
	if(it==_interruptions.end())
	{
		it = _interruptions.insert(std::make_pair(interruption,std::set<CompteurInterruptionSP>())).first;
	}
	it->second.insert(compteur);
}

void Interruption::enleve(const SVM_Valeur::Interruption::InterruptionInterne interruption, const CompteurInterruptionSP& compteur)
{
	ASSERT(interruption!=SVM_Valeur::Interruption::InterruptionInterne::INTERRUPTION_EXTENSION,"interruption materielle externe");
	std::lock_guard<std::mutex> verrou(_protection);
	auto it = _interruptions.find(interruption);
	if(it==_interruptions.end())
		return;
	auto itit = it->second.find(compteur);
	if(itit == it->second.end())
		return;
	it->second.erase(itit);
}
