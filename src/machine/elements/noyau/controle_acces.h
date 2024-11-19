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
#include <src/machine/elements/processeur/interruptions.h>
#include <src/machine/debugueur/json.h>

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Processeur
{
	struct Processeur;
}
namespace Memoire
{
	struct Memoire;
}
namespace Noyau
{
	struct Noyau;
	DECL_SHARED_PTR(Utilisation);
	DECL_SHARED_PTR(LimiteUtilisation);
	DECL_SHARED_PTR(ControleAcces);
	struct Utilisation
	{
		friend struct LimiteUtilisation;
		explicit operator size_t () const { return _utilisation.load(); }
		void operator++() { ++_utilisation; }
		void operator+=(const size_t utilisation) { _utilisation += utilisation; }
		void operator-=(const size_t utilisation) { _utilisation -= utilisation; }
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Utilisation& u)
		{
			f << "usage=" << u._utilisation;
			return f;
		}
		private:
			Utilisation()
			:_utilisation(0) {}
			Utilisation(const Utilisation&) = delete;
			std::atomic_size_t _utilisation;
	};

	struct LimiteUtilisation
	{
		friend struct ControleAcces;
		explicit LimiteUtilisation(const size_t limite)
		:_limite(limite) {}
		static LimiteUtilisationSP integre(const LimiteUtilisationSP& ancien, const LimiteUtilisationSP& nouveau)
		{
			if(not nouveau) return ancien; // pas de nouvelle limitation
			if(not ancien)
			{
				UtilisationSP utilisation(new Utilisation()); // on commence a compter pour la premiere limitation
				LimiteUtilisationSP limitation(new LimiteUtilisation(nouveau->_limite,utilisation));
				return limitation;
			}
			LimiteUtilisationSP limitation(new LimiteUtilisation((nouveau->_limite<ancien->_limite)?nouveau->_limite:ancien->_limite,ancien->_utilisation));
			return limitation;
		}
		size_t restant() const
		{
			if(not _utilisation) return _limite;
			size_t u = static_cast<size_t>(*_utilisation);
			if(u>_limite) return 0;
			return _limite-u;
		}
		template<typename E>
		void incremente()
		{
			++(*_utilisation);
			if(_utilisation->_utilisation>_limite)
			{
				throw E(_limite);
			}
		}
		void change(const unsigned long int restant)
		{
			if(not _utilisation)
			{
				_utilisation.reset(new Utilisation());
			}
			if((restant<=_limite) and (_limite>(_utilisation->_utilisation+restant)))
				_limite = _utilisation->_utilisation+restant;
		}
		template<typename E>
		void augmente(const size_t ecart)
		{
			(*_utilisation) += ecart;
			if(_utilisation->_utilisation>_limite)
			{
				throw E(_limite);
			}
		}

		template<typename E>
		void diminue(const size_t ecart)
		{
			(*_utilisation) -= ecart;
		}

		template<typename Flux>
		friend Flux& operator<<(Flux& f, const LimiteUtilisation& l)
		{
			if(static_cast<bool>(l._utilisation))
			{
				f << *l._utilisation << " ";
			}
			f << "limit=" << l._limite;
			return f;
		}

		SVM::Machine::Debugueur::JSON_Interne::ObjetSP debugueur() const
		{
			auto l = SVM::Machine::Debugueur::JSON::objet({ { "limit" , SVM::Machine::Debugueur::JSON::entier(_limite) } });
			if(static_cast<bool>(_utilisation))
			{
				l->ajoute("current",SVM::Machine::Debugueur::JSON::entier(_utilisation->_utilisation));
			}
			return l;
		}

		private:
			LimiteUtilisation(const size_t limite, const UtilisationSP& utilisation)
			:_limite(limite), _utilisation(utilisation) {}
			size_t _limite;
			UtilisationSP _utilisation;
	};

	struct InstructionSystemeExecuteeEnModeProtege : public SVM_Valeur::ExceptionExecution
	{
		InstructionSystemeExecuteeEnModeProtege(const SVM_Valeur::InstructionSP& i)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::SECURITE)
		{
			DETAILS_EXCEPTION("System instruction " << (*i) << " execution attempt in protected mode.");
		}
		virtual ~InstructionSystemeExecuteeEnModeProtege() {}
	};

	struct InstructionInterditeExecuteeEnModeProtege : public SVM_Valeur::ExceptionExecution
	{
		InstructionInterditeExecuteeEnModeProtege(const SVM_Valeur::InstructionSP& i)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::SECURITE)
		{
			DETAILS_EXCEPTION("Forbidden instruction " << (*i) << " execution attempt in protected mode.");
		}
		virtual ~InstructionInterditeExecuteeEnModeProtege() {}
	};

	struct TropDInstructionsExecutees : public SVM_Valeur::ExceptionExecution
	{
		TropDInstructionsExecutees(const size_t limite)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::SECURITE)
		{
			DETAILS_EXCEPTION("Extra instruction execution attempt. Execution limit was " << limite << ".");
		}
		virtual ~TropDInstructionsExecutees() {}
	};

	struct TropDeMemoireUtilisee : public SVM_Valeur::ExceptionExecution
	{
		TropDeMemoireUtilisee(const size_t limite)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::SECURITE)
		{
			DETAILS_EXCEPTION("Extra memory allocation attempt. Allocation limit was " << limite << ".");
		}
		virtual ~TropDeMemoireUtilisee() {}
	};

	struct ControleAcces
	{
		friend struct SVM::Machine::Element::Processeur::Processeur;
		friend struct SVM::Machine::Element::Memoire::Memoire;
		ControleAcces() {}
		ControleAcces(const SHARED_PTR(size_t)& nombre_instructions_max, const SHARED_PTR(size_t)& memoire_allouee_max)
		{
			if(static_cast<bool>(nombre_instructions_max)) _instructions_executees = std::make_shared<LimiteUtilisation>(*nombre_instructions_max);
			if(static_cast<bool>(memoire_allouee_max)) _memoire_allouee = std::make_shared<LimiteUtilisation>(*memoire_allouee_max);
		}
		static ControleAccesSP integre(const ControleAccesSP& ancien, const ControleAccesSP& nouveau)
		{
			if(not nouveau) return ancien;
			if(not nouveau->_instructions_executees and not nouveau->_memoire_allouee) return ControleAccesSP(); // nouveau controle d'acces d'operette
			LimiteUtilisationSP ancien_instructions_executees;
			LimiteUtilisationSP ancien_memoire_allouee;
			if(static_cast<bool>(ancien))
			{
				ancien_instructions_executees = ancien->_instructions_executees;
				ancien_memoire_allouee = ancien->_memoire_allouee;
			}
			ControleAccesSP ca = std::make_shared<ControleAcces>();
			ca->_instructions_executees = LimiteUtilisation::integre(ancien_instructions_executees,nouveau->_instructions_executees);
			ca->_memoire_allouee = LimiteUtilisation::integre(ancien_memoire_allouee,nouveau->_memoire_allouee);
			return ca;
		}
		void change_instructions(const unsigned long int instructions)
		{
			if(not _instructions_executees)
			{
				_instructions_executees = std::make_shared<LimiteUtilisation>(instructions);
				return;
			}
			_instructions_executees->change(instructions);
		}
		void change_memoire(const unsigned long int memoire)
		{
			if(not _memoire_allouee)
			{
				_memoire_allouee = std::make_shared<LimiteUtilisation>(memoire);
				return;
			}
			_memoire_allouee->change(memoire);
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const ControleAcces& c)
		{
			f << "Access control:";
			if(static_cast<bool>(c._instructions_executees))
			{
				f << " [ instructions: " << *(c._instructions_executees) << " ]";
			}
			if(static_cast<bool>(c._memoire_allouee))
			{
				f << " [ memory: " << *(c._memoire_allouee) << " ]";
			}
			return f;
		}
		LimiteUtilisationSP _instructions_executees;
		LimiteUtilisationSP _memoire_allouee;
	};
}
}
}
}
