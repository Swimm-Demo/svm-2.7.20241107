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
#include <src/machine/elements/valeur/arret.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Processus
{
	DECL_SHARED_PTR(Processus);
}
namespace Noyau
{
	DECL_SHARED_PTR(Noyau);

	struct Etat
	{
		friend struct Noyau;
		enum class Execution
		{
			EXECUTION,
			ATTENTE,
			SUSPENDU,
			INVESTIGATION,
			ZOMBIE,
			INTERROMPU,
			ERREUR
		};
		Etat(const bool transmission_interruption_demandee, const bool mode_protege, const bool retour_est_arret)
		:_execution(Execution::SUSPENDU),_suspension_demandee(false),_terminaison_demandee(false),_transmission_interruption_demandee(transmission_interruption_demandee),_mode_protege(mode_protege), _retour_est_arret(retour_est_arret)
		{}
		void suspension_demandee() { _suspension_demandee.store(true,std::memory_order_relaxed); }
		void terminaison_demandee() { _terminaison_demandee.store(true,std::memory_order_relaxed); }
		void mise_a_jour(bool& changement)
		{
			changement = false;
			if(_terminaison_demandee)
			{
				zombie();
				_suspension_demandee = false;
				_terminaison_demandee = false;
				changement = true;
			}
			if(_suspension_demandee)
			{
				_execution = Execution::SUSPENDU;
				_suspension_demandee = false;
				changement = true;
			}
		}
		void attente() { _execution = Execution::ATTENTE; }
		void execution() { _execution = Execution::EXECUTION; }
		void mode_debugueur(const bool mode)
		{
			if(mode)
			{
				_sauvegarde = _execution;
				_execution = Execution::INVESTIGATION;
			}
			else
			{
				_execution = _sauvegarde;
			}
		}
		bool execution(const Processus::ProcessusSP& processus, bool& changement)
		{
			if(not processus)
			{
				erreur("Impossible to run kernel outside a process");
				return false;
			}
			if(termine(changement))
			{
				return false;
			}
			_execution = Execution::EXECUTION;
			return true;
		}
		bool en_execution(bool& changement) 
		{
			mise_a_jour(changement);
			return _execution==Execution::EXECUTION or _execution==Execution::ATTENTE;
		}
		bool termine(bool& changement)
		{
			mise_a_jour(changement);
			return _execution==Execution::ZOMBIE or _execution==Execution::INTERROMPU or _execution==Execution::ERREUR;
		}
		bool termine() const
		{
			return _execution==Execution::ZOMBIE or _execution==Execution::INTERROMPU or _execution==Execution::ERREUR;
		}

		bool operator==(const Execution e) const
		{
			return _execution==e;
		}
		void zombie(const SVM_Valeur::Arret& arret = SVM_Valeur::Arret())
		{
			_code_retour = arret;
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
		bool investigation() const { return _execution==Execution::INVESTIGATION; }
		std::string etat() const
		{
			std::string e;
			switch(_execution)
			{
				case Execution::EXECUTION: e = "R"; break;
				case Execution::ATTENTE: e = "W"; break;
				case Execution::SUSPENDU: e = "S"; break;
				case Execution::INVESTIGATION: e = "D"; break;
				case Execution::ZOMBIE: e = "Z"; break;
				case Execution::INTERROMPU: e = "I"; break;
				case Execution::ERREUR: e = "E"; break;
			}
			if(_suspension_demandee)
			{
				e += "s";
			}
			if(_terminaison_demandee)
			{
				e += "t";
			}
			return e;
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Etat& e)
		{
			f << e.etat();
			if(e._transmission_interruption_demandee)
			{
				f << ", transmit_interruption";
			}
			if(e._retour_est_arret)
			{
				f << ", last_return_is_shutdown";
			}
			if(e._mode_protege)
			{
				f << ", protected_mode";
			}
			if(static_cast<bool>(e._code_retour))
			{
				f << ", return_code " << *e._code_retour;
			}
			if(static_cast<bool>(e._interruption))
			{
				f << ", interrupted " ;
				e._interruption->erreur(f);
			}
			if(static_cast<bool>(e._erreur))
			{
				f << ", error " << *e._erreur;
			}
			return f;
		}
		private:
			Execution _execution;
			Execution _sauvegarde;
			std::atomic_bool _suspension_demandee;
			std::atomic_bool _terminaison_demandee;
			SHARED_PTR(size_t) _code_retour;
			SVM_Valeur::InterruptionSP _interruption;
			SHARED_PTR(std::string) _erreur;
		public:
			const bool _transmission_interruption_demandee;
			const bool _mode_protege; // en fonction de la valeur, appelle fonction specifique CPU pour controles
			const bool _retour_est_arret;
	};
}
}
}
}

