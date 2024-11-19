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

#include <src/machine/machine/machine.h>

using namespace SVM::Machine;

namespace SVM_Ordonnanceur = SVM::Machine::Element::Ordonnanceur;

Machine::Machine(const SHARED_PTR(std::string)& description, const HistoriqueSP& historique, const std::vector<DeclarationExtension>& extensions, const std::vector<Processus>& processus, const DebugSP& debugueur, const PerfSP& performance)
:_description(description), _historique(historique), _extensions(extensions), _processus(processus), _debugueur(debugueur), _performance(performance) 
{
}

int Machine::execution()
{
	SVM::Machine::Interruption::Interruption::bloque_signaux();
	SVM_Debugueur::DebugueurSP debugueur;
	debugueur = SVM_Debugueur::Debugueur::creation(_debugueur);
	_debugueur.reset();
	SVM::Machine::Performance::PerformanceSP performance;
	performance = SVM::Machine::Performance::Performance::creation(_performance,_historique);
	_performance.reset();
	SVM_Extension::GestionnaireSP gestionnaire_extensions;
	try
	{
		gestionnaire_extensions = SVM_Extension::Gestionnaire::gestionnaire(_extensions,debugueur);
	}
	catch(...)
	{
		SVM_Debugueur::Debugueur::arret(debugueur);
		throw;
	}
	_base = std::make_shared<SVM_Base::Base>(gestionnaire_extensions,debugueur,performance);
	_piege = SVM_Debugueur::Debugueur::ajoute_piege<PiegeMachine>(_base->_debugueur);
	try
	{
		for(auto& p: _processus)
		{
			try
			{
				p.compile(this->shared_from_this());
			}
			catch(SVM_Valeur::Interruption& i)
			{
				throw ProcessusCodeInvalide(p._nom,i);
			}
			if(static_cast<bool>(p._sequenceur))
			{
				if(not gestionnaire_extensions->sequenceur(*p._sequenceur))
				{
					throw ProcessusObjetInvalide(p._nom,"sequencer",p._sequenceur);
				}
			}
			if(static_cast<bool>(p._ordonnanceur))
			{
				if(not gestionnaire_extensions->ordonnanceur(*p._ordonnanceur))
				{
					throw ProcessusObjetInvalide(p._nom,"scheduler",p._ordonnanceur);
				}
			}
		}
		_systeme = SVM_Systeme::Systeme::creation_systeme(this->shared_from_this());
		try
		{
			if(static_cast<bool>(_piege)) { _piege->demarree(); }
			auto r = _systeme->execution(this->shared_from_this(),_processus);
			if(static_cast<bool>(_piege)) { _piege->terminee(); }
			_piege.reset(); 
			SVM::Machine::Performance::Performance::rapport(performance);
			SVM_Debugueur::Debugueur::arret(debugueur);
			return r;
		}
		catch(...)
		{
			if(static_cast<bool>(_piege)) { _piege->erreur(); }
			throw;
		}
	}
	catch(...)
	{
		_piege.reset();
		SVM::Machine::Performance::Performance::rapport(performance);
		SVM_Debugueur::Debugueur::arret(debugueur);
		throw;
	}
	return 128;
}
