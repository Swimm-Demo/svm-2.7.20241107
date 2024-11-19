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
#include <src/machine/interface/variable.h>
#include <src/machine/elements/systeme/evenement.h>

namespace SVM_Synchronisation = SVM::Machine::Element::Synchronisation;

namespace SVM
{
namespace Machine
{
	DECL_SHARED_PTR(Machine);
namespace Element
{
namespace Memoire
{
	DECL_SHARED_PTR(PiegeMemoire);
}
namespace Processus
{
	DECL_SHARED_PTR(Processus);
}
}
namespace Extension
{
	DECL_SHARED_PTR(PatronObjetExtension);
}
namespace Interface
{
	DECL_SHARED_PTR(Environnement);
	DECL_WEAK_PTR(Environnement);
	struct Environnement
	{
		Environnement(const SVM::Machine::Extension::PatronObjetExtensionSP& patron, const SVM::Machine::MachineSP& machine, const VariablesDefiniesSP& extension, const bool attente = true, const bool systeme = true, const bool nettoyage = false)
		:_appellant(std::make_shared<VariablesDefinies>()), _extension(extension), _machine(machine), _patron(patron), _variable_noyau(nullptr), _variable_processus(nullptr), _variable_ordonnanceur(nullptr), _attente(attente), _systeme(systeme), _nettoyage(nettoyage)
		, _notification_debugueur(false)
		{}
		~Environnement()
		{
			notification_debugueur_changement_memoire();
		};
		template<typename T>
		const void* ajout(const SHARED_PTR(T)& valeur)
		{
			return _appellant->ajout(valeur);
		}
		template<typename T>
		const void* ajout_avatar(const SHARED_PTR(T)& valeur)
		{
			return _appellant->ajout_avatar(valeur);
		}
		template<typename T>
		void remplace(const void *v, const SHARED_PTR(T)& valeur )
		{
			_appellant->remplace<T>(v,valeur);
			_extension->remplace<T>(v,valeur);
		}
		template<typename T>
		SHARED_PTR(T) valeur(const void *v)
		{
			auto vv = _appellant->valeur<T>(v);
			if(not vv)
			{
				vv = _extension->valeur<T>(v);
			}
			return vv;
		}
		bool existe(const void *variable) const
		{
			return _appellant->teste(variable) or _extension->teste(variable);
		}
		bool ephemere(const void *variable) const
		{
			return _appellant->teste(variable);
		}
		bool durable(const void *variable) const
		{
			return _extension->teste(variable);
		}
		void partagee(const void *variable)
		{
			_appellant->partagee(variable);
			_extension->partagee(variable);
		}
		bool est_partagee(const void *variable)
		{
			return _appellant->est_partagee(variable) or _extension->est_partagee(variable);
		}
		void reguliere(const void *variable)
		{
			_appellant->reguliere(variable);
			_extension->reguliere(variable);
		}
		void globale(const void *variable)
		{
			_appellant->globale(variable,_extension);
		}
		void locale(const void *variable)
		{
			_appellant->locale(variable,_extension);
		}
		void libere(const void *variable)
		{
			_appellant->libere(variable);
			_extension->libere(variable);
		}
		void exporte(const void *variable, VariablesDefiniesSP& variables)
		{
			_appellant->exporte(variable,variables);
			_extension->exporte(variable,variables);
		}
		void notification_debugueur_changement_memoire(const SVM::Machine::Element::Memoire::PiegeMemoireSP& piege = SVM::Machine::Element::Memoire::PiegeMemoireSP());
		std::string nom();
		public:
			VariablesDefiniesSP _appellant;
			VariablesDefiniesSP _extension;
			SVM::Machine::MachineSP _machine;
			SVM::Machine::Extension::PatronObjetExtensionSP _patron;
			const void *_variable_noyau;
			const void *_variable_processus;
			const void *_variable_ordonnanceur;
			SVM::Machine::Element::Valeur::PositionInstructionSP _position;
			const bool _attente;
			const bool _systeme;
			const bool _nettoyage;
			bool _notification_debugueur;
	};

	DECL_SHARED_PTR(VariableProcessus);
	struct VariableProcessus
	{
		explicit VariableProcessus(const SVM::Machine::Element::Processus::ProcessusSP& processus, const bool pur=true)
			:_processus(processus),_pur(pur) {}
		~VariableProcessus();
		template<typename Flux>
			friend Flux& operator<<(Flux& f, const VariableProcessus& v)
			{
				f << (v._processus);
				return f;
			}
		const void *avatar() const
		{
			return _processus.get();
		}
		SVM::Machine::Element::Processus::ProcessusSP _processus;
		bool _pur;
	};
}
}
}
