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

#include <src/machine/elements/processus/processus.h>
#include <src/machine/elements/processeur/instructions/debogage.h>

using namespace SVM::Machine::Element::Processeur::Instruction;

void DebogageArretProcesseur::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	if(static_cast<bool>(_message))
	{
		_message->decoration(decorateur);
	}
	sans_position(decorateur);
}

void DebogageArretProcesseur::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	auto debugueur = noyau->debugueur();
	if(not debugueur) return;
	if(noyau->_processeur->_piege->arret_ephemere()) return;
	std::ostringstream oss;
	if(static_cast<bool>(_message))
	{
		auto m = _message->evaluation_chaine(noyau);
		oss << m;
	}
	noyau->_processeur->_piege->point_arret("Breakpoint",false,oss.str());
}

void DebogageArretProcesseur::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	if(not _message) return;
	std::ostringstream mes;
	_message->explique(noyau,mes);
	explications.push_back(mes.str());
}

void DebogageArretMemoire::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_adresse->decoration(decorateur);
	sans_position(decorateur);
}

void DebogageArretMemoire::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	auto debugueur = noyau->debugueur();
	if(not debugueur) return;
	auto adresse = _adresse->evaluation(noyau);
	switch(_type)
	{
		case Type::LECTURE:
			noyau->_memoire->_piege->point_arret_lecture(adresse,_ajout);
			break;
		case Type::ECRITURE:
			noyau->_memoire->_piege->point_arret_ecriture(adresse,_ajout);
			break;
		case Type::ACCES:
			noyau->_memoire->_piege->point_arret_acces(adresse,_ajout);
			break;
		case Type::SUPPRESSION:
			noyau->_memoire->_piege->point_arret_suppression(adresse,_ajout);
			break;
	}
}

void DebogageArretMemoire::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream adr;
	_adresse->explique(noyau,adr);
	explications.push_back(adr.str());
}

void DebogageExplicationAdresse::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_adresse->decoration(decorateur);
	sans_position(decorateur);
}

void DebogageExplicationAdresse::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	auto debugueur = noyau->debugueur();
	if(not debugueur) return;
	std::ostringstream oss;
	_adresse->explique(noyau,oss);
	if(noyau->_processeur->_piege->arret_ephemere())
	{
		auto processus = noyau->_processus.lock();
		SVM_TRACE("Avant ajout point arret a liste");
		debugueur->_points_arret->ajout_point_arret(SVM_Debugueur::PointArret("Explain",oss.str(),noyau->_piege->identifiant(),processus->nom_debug(),noyau->nom_debug()));
	}
	else
	{
		noyau->_processeur->_piege->point_arret("Explain",false,oss.str());
	}
}

void DebogageExplicationAdresse::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream adr;
	_adresse->explique(noyau,adr);
	explications.push_back(adr.str());
}

void DebogageExplicationValeur::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_valeur->decoration(decorateur);
	sans_position(decorateur);
}

void DebogageExplicationValeur::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	auto debugueur = noyau->debugueur();
	if(not debugueur) return;
	std::ostringstream oss;
	_valeur->explique(noyau,oss);
	if(noyau->_processeur->_piege->arret_ephemere())
	{
		auto processus = noyau->_processus.lock();
		SVM_TRACE("Avant ajout point arret a liste");
		debugueur->_points_arret->ajout_point_arret(SVM_Debugueur::PointArret("Explain",oss.str(),noyau->_piege->identifiant(),processus->nom_debug(),noyau->nom_debug()));
	}
	else
	{
		noyau->_processeur->_piege->point_arret("Explain",false,oss.str());
	}
}

void DebogageExplicationValeur::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream val;
	_valeur->explique(noyau,val);
	explications.push_back(val.str());
}

void DebogageArretInterruption::decoration(SVM_Valeur::Decorateur& decorateur)
{
	position(decorateur);
	_interruption->decoration(decorateur);
	sans_position(decorateur);
}

void DebogageArretInterruption::execution(const SVM_Noyau::NoyauSP& noyau) const
{
	auto debugueur = noyau->debugueur();
	if(not debugueur) return;
	SVM_Valeur::Interruption interruption = _interruption->evaluation_interruption(noyau);
	if(_ajout)
	{
		noyau->_processeur->_piege->ajoute_interruption(interruption);
	}
	else
	{
		noyau->_processeur->_piege->enleve_interruption(interruption);
	}
}

void DebogageArretInterruption::explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
{
	std::ostringstream irq;
	_interruption->explique(noyau,irq);
	explications.push_back(irq.str());
}

