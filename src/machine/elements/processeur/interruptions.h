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

#pragma once
#include <src/global/global.h>
#include <src/machine/elements/valeur/interruption.h>
#include <src/machine/elements/valeur/adresse_instruction.h>
#include <src/machine/interruption/interruption.h>
#include <src/machine/debugueur/json.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Processeur
{
	struct GestionnaireNonDefini : public SVM_Valeur::ExceptionExecution
	{
		GestionnaireNonDefini(const SVM_Valeur::Interruption& i)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR)
		{
			DETAILS_EXCEPTION("Interruption handler not defined for " << i << ".");
		}
		virtual ~GestionnaireNonDefini() {}
	};

	struct Interruptions
	{
		Interruptions() {}
		void vide() { _gestionnaires.clear(); }
		void ajoute_interruption(const SVM_Valeur::Interruption& interruption, const SVM_Valeur::AdresseInstruction& gestionnaire, const SVM::Machine::Interruption::InterruptionSP& interruptions_materielles = SVM::Machine::Interruption::InterruptionSP())
		{
			auto it=_gestionnaires.insert(std::make_pair(interruption,gestionnaire));
			if(not it.second)
			{
				it.first->second=gestionnaire;
				return;
			}
			if(interruption._interruption_interne==SVM_Valeur::Interruption::InterruptionInterne::INTERRUPTION_EXTENSION) return;
			if(not interruptions_materielles) return;
			auto compteur = std::make_shared<SVM::Machine::Interruption::CompteurInterruption>();
			_interruptions_materielles.insert(std::make_pair(interruption._interruption_interne,compteur));
			interruptions_materielles->ajoute(interruption._interruption_interne,compteur);
		}
		bool enleve_interruption(const SVM_Valeur::Interruption& interruption, const SVM::Machine::Interruption::InterruptionSP& interruptions_materielles = SVM::Machine::Interruption::InterruptionSP())
		{
			auto it = _gestionnaires.find(interruption);
			if(it != _gestionnaires.end())
			{
				_gestionnaires.erase(it);
				if(interruption._interruption_interne==SVM_Valeur::Interruption::InterruptionInterne::INTERRUPTION_EXTENSION) return true;
				if(not interruptions_materielles) return true;
				auto compteur = _interruptions_materielles.find(interruption._interruption_interne);
				interruptions_materielles->enleve(interruption._interruption_interne,compteur->second);
				_interruptions_materielles.erase(compteur);
				return true;
			}
			return false;
		}
		const SVM_Valeur::AdresseInstruction& interruption(const SVM_Valeur::Interruption& interruption) const
		{
			auto it = _gestionnaires.find(interruption);
			if(it == _gestionnaires.end())
			{
				throw GestionnaireNonDefini(interruption);
			}
			return it->second;
		}
		SVM_Valeur::AdresseInstructionSP gestionnaire_interruption(const SVM_Valeur::Interruption& interruption) const
		{
			auto it = _gestionnaires.find(interruption);
			if(it != _gestionnaires.end())
			{
				return std::make_shared<SVM_Valeur::AdresseInstruction>(it->second);
			}
			return SVM_Valeur::AdresseInstructionSP();
		}
		void interruptions_materielles(std::list<SVM_Valeur::Interruption>& interruptions_en_attente)
		{
			for(auto& i:_interruptions_materielles)
			{
				while(i.second->lecture())
				{
					interruptions_en_attente.push_back(SVM_Valeur::Interruption(i.first,"hardware interruption received",true));
				}
			}
		}
		size_t interruptions_materielles_detectees() const
		{
			size_t detections = 0;
			for(auto& i:_interruptions_materielles)
			{
				detections += i.second->compteur();
			}
			return detections;
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Interruptions& i)
		{
			for(auto& gi:i._gestionnaires)
			{
				f << "    " << gi.first << " => " << gi.second << std::endl;
			}
			return f;
		}
		SVM::Machine::Debugueur::JSON_Interne::TableauSP debugueur() const
		{
			auto t = SVM::Machine::Debugueur::JSON::tableau();
			for(const auto& i: _gestionnaires)
			{
				t->ajoute(SVM::Machine::Debugueur::JSON::objet({ { "interruption" , i.first.debugueur() } , { "handler" , i.second.debugueur() } }));
			}
			return t;
		}
		private:
			std::map<SVM_Valeur::Interruption,SVM_Valeur::AdresseInstruction> _gestionnaires;
			std::map<SVM_Valeur::Interruption::InterruptionInterne,SVM::Machine::Interruption::CompteurInterruptionSP> _interruptions_materielles;
	};
}
}
}
}
