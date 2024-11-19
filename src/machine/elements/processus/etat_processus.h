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
#include <src/machine/elements/synchronisation/evenement.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Synchronisation = SVM::Machine::Element::Synchronisation;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Processus
{
	struct Etat
	{
		enum class Execution
		{
			SUSPENDU,
			EXECUTION,
			BLOQUE,
			ATTENTE,
			INVESTIGATION,
			ZOMBIE,
			INTERROMPU,
			ERREUR
		};
		explicit Etat(const bool terminaison_automatique)
		:_execution(Execution::SUSPENDU),_terminaison_automatique(terminaison_automatique) {}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Etat& e)
		{
			f << Etat::texte(e._execution);
			if(static_cast<bool>(e._code_retour))
			{
				f << ", return_code " << *e._code_retour;
			}
			if(static_cast<bool>(e._interruption))
			{
				f << ", interrupted ";
				e._interruption->erreur(f);
			}
			if(static_cast<bool>(e._erreur))
			{
				f << ", error " << *e._erreur;
			}
			if(e._terminaison_automatique)
			{
				f << ", autoterminated ";
			}
			return f;
		}
		std::string etat() const
		{
			return Etat::texte(_execution);
		}
		bool operator==(const Execution e) const
		{
			return _execution==e;
		}
		static std::string texte(const Execution& e)
		{
			switch(e)
			{
				case Execution::SUSPENDU: return "S";
				case Execution::EXECUTION: return "R";
				case Execution::BLOQUE: return "L";
				case Execution::ATTENTE: return "W";
				case Execution::INVESTIGATION: return "D";
				case Execution::ZOMBIE: return "Z";
				case Execution::INTERROMPU: return "I";
				case Execution::ERREUR: return "E";
			}
			return "?";
		}
		bool executable()
		{
			return (_execution == Execution::SUSPENDU);
		}
		void execution() { _execution = Execution::EXECUTION; }
		void suspension() { _execution = Execution::SUSPENDU; }
		void attente() { _execution = Execution::ATTENTE; }
		void bloque() { _execution = Execution::BLOQUE; }
		void debugueur(const bool mode)
		{
			if(mode)
			{
				_execution = Execution::INVESTIGATION;
			}
			else
			{
				_execution = Execution::EXECUTION;
			}
		}
		void zombie(const SHARED_PTR(size_t)& code_retour = SHARED_PTR(size_t)())
		{
			_code_retour = code_retour;
			_execution = Execution::ZOMBIE;
		}
		void interrompu(const SVM_Valeur::InterruptionSP& interruption)
		{
			_interruption = interruption;
			_execution = Execution::INTERROMPU;
		}
		void erreur(const std::string& message)
		{
			_erreur = std::make_shared<std::string>(message);
			_execution = Execution::ERREUR;
		}
		Execution etat_global() const { return _execution; }
		bool en_execution() const
		{
			return _execution==Execution::EXECUTION;
		}
		static bool termine(const SVM::Machine::Element::Processus::Etat::Execution e)
		{
			switch(e)
			{
				case Execution::ZOMBIE:
				case Execution::INTERROMPU:
				case Execution::ERREUR:
					return true;
				default:
					return false;
			}
		}
		bool termine() const
		{
			return SVM::Machine::Element::Processus::Etat::termine(_execution);
		}
		bool vivant() const
		{
			return not _terminaison_automatique and not SVM::Machine::Element::Processus::Etat::termine(_execution);
		}
		SHARED_PTR(size_t) code_retour() const
		{
			if(_execution!=Execution::ZOMBIE)
			{
				return SHARED_PTR(size_t)();
			}
			return _code_retour;
		}
		SHARED_PTR(std::string) erreur() const
		{
			if(_execution!=Execution::ERREUR)
			{
				return SHARED_PTR(std::string)();
			}
			return _erreur;
		}
		SVM_Valeur::InterruptionSP interrompu() const
		{
			if(_execution!=Execution::INTERROMPU)
			{
				return SVM_Valeur::InterruptionSP();
			}
			return _interruption;
		}
		bool terminaison_automatique() const { return _terminaison_automatique; }
		private:
		Execution _execution;
		SHARED_PTR(size_t) _code_retour;
		SHARED_PTR(std::string) _erreur;
		SVM_Valeur::InterruptionSP _interruption;
		const bool _terminaison_automatique;
	};
}
}
}
}
