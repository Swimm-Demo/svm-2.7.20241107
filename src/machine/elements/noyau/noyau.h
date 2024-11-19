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
#include <src/machine/elements/noyau/etat_noyau.h>
#include <src/machine/elements/processeur/processeur.h>
#include <src/machine/elements/memoire/memoire.h>
#include <src/machine/elements/valeur/adresse_instruction.h>
#include <src/machine/elements/valeur/arret.h>
#include <src/machine/elements/valeur/interruption.h>
#include <src/machine/interface/environnement.h>
#include <src/machine/elements/noyau/controle_acces.h>
#include <src/machine/debugueur/piege.h>

namespace SVM_Processeur = SVM::Machine::Element::Processeur;
namespace SVM_Memoire = SVM::Machine::Element::Memoire;
namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Extension = SVM::Machine::Extension;
namespace SVM_Interface = SVM::Machine::Interface;
namespace SVM_Debugueur = SVM::Machine::Debugueur;

namespace SVM
{
namespace Machine
{
namespace Extension
{
	DECL_SHARED_PTR(Gestionnaire);
}
namespace Debugueur
{
	DECL_SHARED_PTR(Debugueur);
}
namespace Element
{
namespace Processus
{
	DECL_SHARED_PTR(Processus);
	DECL_WEAK_PTR(Processus);
}
namespace Programme
{
	struct LectureMemoire;
	struct PointeurAlias;
	struct PointeurCourant;
	struct SymboleLocal;
	struct CibleDynamique;
}
namespace Processeur
{
	struct Processeur;
namespace Instruction
{
	struct AffectationValeur;
	struct AffectationBloc;
	struct AffectationInstruction;
	struct Appel;
	struct ConditionAdresseDefinie;
	struct ConditionAdresseInitialisee;
	struct ConditionDrapeau;
	struct Decalage;
	struct Drapeau;
	struct Interruption;
	struct Local;
	struct Memoire;
	struct Retour;
	struct Saut;
}
}
namespace Sequenceur
{
	DECL_WEAK_PTR(Sequenceur);
	struct SequenceurDefaut;
	struct SequenceurExtension;
}
namespace Noyau
{
namespace SVM_Instruction = SVM::Machine::Element::Processeur::Instruction;
namespace SVM_Programme = SVM::Machine::Element::Programme;
namespace SVM_Processus = SVM::Machine::Element::Processus;
namespace SVM_Processeur = SVM::Machine::Element::Processeur;

	DECL_SHARED_PTR(Noyau);
	DECL_WEAK_PTR(Noyau);

	DECL_SHARED_PTR(PiegeNoyau);
	struct PiegeNoyau : public SVM_Debugueur::Piege
	{
		PiegeNoyau(const NoyauWP& noyau);
		virtual ~PiegeNoyau() {};
		virtual SVM_Debugueur::JSON_Interne::ObjetSP mise_a_jour() override;
		virtual void cascade() override;

		NoyauWP _noyau;
	};

	struct Noyau : public std::enable_shared_from_this<Noyau>
	{
		friend struct Sequenceur::SequenceurDefaut;
		friend struct Sequenceur::SequenceurExtension;
		friend struct SVM_Processeur::Processeur;
		friend struct SVM_Instruction::AffectationValeur;
		friend struct SVM_Instruction::AffectationBloc;
		friend struct SVM_Instruction::AffectationInstruction;
		friend struct SVM_Instruction::Appel;
		friend struct SVM_Instruction::ConditionAdresseDefinie;
		friend struct SVM_Instruction::ConditionAdresseInitialisee;
		friend struct SVM_Instruction::ConditionDrapeau;
		friend struct SVM_Instruction::Decalage;
		friend struct SVM_Instruction::Drapeau;
		friend struct SVM_Instruction::Interruption;
		friend struct SVM_Instruction::Local;
		friend struct SVM_Instruction::Memoire;
		friend struct SVM_Instruction::Retour;
		friend struct SVM_Instruction::Saut;
		friend struct SVM_Programme::LectureMemoire;
		friend struct SVM_Programme::PointeurAlias;
		friend struct SVM_Programme::PointeurCourant;
		friend struct SVM_Programme::SymboleLocal;
		friend struct SVM_Programme::CibleDynamique;
		friend struct PiegeNoyau;
		static NoyauSP creation_noyau(SVM_Debugueur::DebugueurSP& debugueur, const bool transmission_interruption_demandee, const bool retour_est_arret, const bool mode_protege, const ControleAccesSP& controle_acces, const SVM_Valeur::AdresseInstruction& code, const SVM_Memoire::MemoireSP& memoire = SVM_Memoire::MemoireSP());
		Noyau(SVM_Debugueur::DebugueurSP& debugueur, const bool transmission_interruption_demandee, const bool retour_est_arret, const bool mode_protege, const ControleAccesSP& controle_acces, const SVM_Valeur::AdresseInstruction& code, const SVM_Memoire::MemoireSP& memoire = SVM_Memoire::MemoireSP());
		bool transmet_interruptions() const { return _etat._transmission_interruption_demandee; } // pour API
		bool mode_protege() const { return _etat._mode_protege; } // pour API
		ControleAccesSP controle_acces() const { return _controle_acces; } // pour API
		void integre(const ControleAccesSP& controle_acces)
		{
			_controle_acces = ControleAcces::integre(_controle_acces,controle_acces);
		}

		bool retour_est_arret() const { return _etat._retour_est_arret; } // pour API
		Etat::Execution etat_global() const { return _etat._execution; } // pour API
		void suspension_demandee() // pour API
		{
			_etat.suspension_demandee();
			if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		}
		void terminaison_demandee() // pour API
		{
			_etat.terminaison_demandee();
			if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		}
		bool investigation() { return _etat.investigation(); }
		bool pause();
		bool en_execution(bool& changement) { return _etat.en_execution(changement); }
		bool termine() { return _etat.termine(); }
		const Etat& execution();
		void etat_execution()
		{
			_etat.execution();
			if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		}
		void etat_attente()
		{
			_etat.attente();
			if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		}
		void etat_zombie(const SVM_Valeur::Arret& arret = SVM_Valeur::Arret())
		{
			_etat.zombie(arret);
			if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		}
		void etat_interrompu(const SVM_Valeur::InterruptionSP& interruption)
		{
			_etat.interrompu(interruption);
			if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		}
		void mode_debugueur(const bool mode) { _etat.mode_debugueur(mode); }
		void interruption(const SVM_Valeur::Interruption& i) { _processeur->interruption(i); }
		SVM_Valeur::InterruptionSP interrompu() const { return _etat.interrompu(); }
		SVM::Machine::MachineSP machine();
		SVM_Debugueur::DebugueurSP& debugueur();
		SVM::Machine::Interruption::InterruptionSP& gestionnaire_interruptions_materielles();
		SVM_Interface::VariablesDefiniesSP variables_extension(const std::string& nom_extension) const;
		void echange_memoire(NoyauSP& noyau)
		{
			std::lock_guard<std::mutex> verrou(SVM_Memoire::Memoire::protection());
			_memoire.swap(noyau->_memoire);
			_memoire->_noyau = this->shared_from_this();
			noyau->_memoire->_noyau = noyau;
			if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
			if(static_cast<bool>(noyau->_piege)) { noyau->_piege->notification_changement_objet(); }
		}
		void trace() const;
		template<typename Flux>
		void trace_etat(Flux& f)
		{
			f << _etat;
		}
		std::string nom_debug() const
		{
			std::ostringstream oss;
			oss << "Kernel " << _nom << ":" << _identifiant;
			return oss.str();
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Noyau& n)
		{
			f	<< "Kernel:" << std::endl
				<< "State: " << n._etat << std::endl << std::endl;
			if(static_cast<bool>(n._controle_acces))
			{
				f << *(n._controle_acces) << std::endl;
			}
			f	<< *n._processeur << std::endl
				<< *n._memoire << std::endl;
			return f;
		}
		private:
			Etat _etat;
			mutable std::mutex _execution;
		public:
			SVM_Processeur::ProcesseurSP _processeur;
			SVM_Memoire::MemoireSP _memoire;
			ControleAccesSP _controle_acces;
			SVM::Machine::Element::Sequenceur::SequenceurWP _sequenceur;
			SVM_Processus::ProcessusWP _processus;
			PiegeNoyauSP _piege;
			unsigned long long int _identifiant = 0;
			std::string _nom;
	};
}
}
}
}
