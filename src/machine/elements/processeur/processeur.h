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
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/elements/valeur/arret.h>
#include <src/machine/elements/processeur/etat.h>
#include <src/machine/elements/processeur/interruptions.h>
#include <src/machine/debugueur/piege.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Memoire = SVM::Machine::Element::Memoire;
namespace SVM_Debugueur = SVM::Machine::Debugueur;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Noyau
{
	DECL_SHARED_PTR(Noyau);
	DECL_WEAK_PTR(Noyau);
}
namespace Valeur
{
	DECL_SHARED_PTR(Instruction);
}
namespace Processeur
{
	DECL_SHARED_PTR(Processeur);
	DECL_WEAK_PTR(Processeur);

	struct PointArret
	{
		PointArret(const std::string& type, const std::string& message)
		:_type(type), _message(message) {}

		std::string _type;
		std::string _message;
	};

	DECL_SHARED_PTR(PiegeProcesseur);
	struct PiegeProcesseur : public SVM_Debugueur::Piege
	{
		PiegeProcesseur(const ProcesseurWP& processeur);
		virtual ~PiegeProcesseur() {};
		virtual SVM_Debugueur::JSON_Interne::ObjetSP mise_a_jour() override;
		virtual void evenement(const SVM_Debugueur::JSON_Interne::ObjetSP& evenement) override;
		void point_arret(const std::string& type, const bool ephemere, const std::string& message = std::string());
		void point_arret_externe(const std::string& type, const std::string& message);
		void arret();
		void ajoute_interruption(const SVM_Valeur::Interruption& interruption);
		void enleve_interruption(const SVM_Valeur::Interruption& interruption);
		void ajoute_point_arret(const SVM_Valeur::AdresseInstruction& adresse);
		void enleve_point_arret(const SVM_Valeur::AdresseInstruction& adresse);
		bool arret_ephemere() const;
		void etat(const Etat& courant);
		void instruction_courante(const SVM_Valeur::AdresseInstruction& courante, const SVM_Valeur::AdresseInstruction& prochaine, const bool point_arret = false);
		void prochaine_instruction(const SVM_Valeur::AdresseInstruction& a);
		void memoire_courante(const SVM_Valeur::Pointeur& courante);
		void memoire_allouee(const std::set<SVM_Valeur::Pointeur>& allouee);
		void alias_definis(const std::set<std::string>& alias);
		void interruptions_locales(const Interruptions& locales);
		void interruptions_locales_cascadees(const Interruptions& locales);
		void interruptions_globales(const Interruptions& globales);
		void drapeaux(const std::set<std::string>& drapeaux);
		void drapeaux_cascades(const std::set<std::string>& drapeaux);
		void remplacementinstructions_local(const Etat::RemplacementInstructions& locales);
		void remplacementinstructions_local_cascadees(const Etat::RemplacementInstructions& locales);
		void remplacementinstructions_global(const Etat::RemplacementInstructions& globales);
		void interruptions_en_attente(const std::list<SVM_Valeur::Interruption>& interruptions, const bool point_arret = false);
		void empile(const Etat& e);
		void depile(const Etat& e);
		void quota(const SVM_Noyau::LimiteUtilisationSP& utilisation);
		void utilise_interruptions(const bool activation);

		ProcesseurWP _processeur;
		bool _arret_global;
		size_t _arret_local; // execution continue tant que la pile a une taille superieure ou egale a cette valeur (aka 0 n'arrete jamais l'execution)
		std::set<SVM_Valeur::AdresseInstruction> _points_arret;
		std::set<SVM_Valeur::Interruption> _interruptions;
		std::vector<PointArret> _points_arret_externes;
		mutable std::mutex _protection;
	};

	struct TropDeRetourDeFonction : public SVM_Valeur::ExceptionExecution
	{
		TropDeRetourDeFonction()
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR)
		{
			DETAILS_EXCEPTION("Too many function return.");
		}
		virtual ~TropDeRetourDeFonction() {}
	};

	struct PileRetourPasAssezGrande : public SVM_Valeur::ExceptionExecution
	{
		PileRetourPasAssezGrande(const size_t taille)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR)
		{
			DETAILS_EXCEPTION("Return stack does not contain at least " << taille << " states.");
		}
		virtual ~PileRetourPasAssezGrande() {}
	};

	struct PileRetourDepassement : public SVM_Valeur::ExceptionExecution
	{
		PileRetourDepassement(const size_t indice)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::PROCESSEUR)
		{
			DETAILS_EXCEPTION("Return stack move to " << indice << " invalid.");
		}
		virtual ~PileRetourDepassement() {}
	};

	struct Processeur
	{
		friend struct PiegeProcesseur;
		static ProcesseurSP creation_processeur(SVM_Debugueur::DebugueurSP& debugueur, const SVM_Valeur::AdresseInstruction& code, const SVM_Noyau::ControleAccesSP& controle_acces);
		Processeur(const SVM_Valeur::AdresseInstruction& code, const SVM_Noyau::ControleAccesSP& controle_acces)
		:_etat(code), _interruptions_activees(true)
		{
			if(static_cast<bool>(controle_acces)) { _utilisation = controle_acces->_instructions_executees; }
		}
		SVM_Valeur::AdresseInstruction prochaine_instruction()
		{
			auto e = _etat.prochaine_instruction();
			if(static_cast<bool>(_piege)) { _piege->instruction_courante(_etat._instruction_courante,_etat._prochaine_instruction); }
			return e;
		}
		SVM_Valeur::AdresseInstruction prochaine_instruction_sans_increment() { return _etat.prochaine_instruction_sans_increment(); }
		SVM_Valeur::AdresseInstruction instruction_courante() { return _etat.instruction_courante(); }
		SVM_Valeur::CodeSP code_courant() const { return _etat.code_courant(); }
		void saut(const SVM_Valeur::AdresseInstruction& cible)
		{
			_etat.saut(cible);
			if(static_cast<bool>(_piege)) { _piege->prochaine_instruction(_etat._prochaine_instruction); }
		}
		void appel(const SVM_Valeur::AdresseInstruction& cible, const SVM_Valeur::Pointeur& parametres)
		{
			_etats_sauvegardes.push_front(_etat);
			_etat.appel(cible,parametres);
			if(static_cast<bool>(_piege)) { _piege->etat(_etat); _piege->empile(_etats_sauvegardes.front()); }
		}
		void appel_identique(const bool transfert)
		{
			_etats_sauvegardes.push_front(_etat);
			if(transfert)
			{
				_etat.ignore_memoire();
			}
			else
			{
				_etats_sauvegardes.front().ignore_memoire();
			}
			if(static_cast<bool>(_piege)) { _piege->etat(_etat); _piege->empile(_etats_sauvegardes.front()); }
		}
		void retour(SVM_Memoire::Memoire& memoire, const bool equivalent_arret)
		{
			if(_etats_sauvegardes.empty())
			{
				if(equivalent_arret)
				{
					throw SVM_Valeur::Arret();
				}
				throw TropDeRetourDeFonction();
			}
			_etat.nettoyage(memoire);
			_etat = _etats_sauvegardes.front();
			_etats_sauvegardes.pop_front();
			if(static_cast<bool>(_piege)) { _piege->depile(_etat); }
		}
		SVM_Valeur::Pointeur memoire_courante() const { return _etat.memoire_courante(); }
		void memoire_courante(const SVM_Valeur::Pointeur& pointeur)
		{
			_etat.memoire_courante(pointeur);
			if(static_cast<bool>(_piege)) { _piege->memoire_courante(_etat._memoire_courante); }
		}
		void memoire_allouee(const std::set<SVM_Valeur::Pointeur>& bloc)
		{
			_etat.memoire_allouee(bloc);
			if(static_cast<bool>(_piege)) { _piege->memoire_allouee(_etat._memoire_allouee); }
		}
		void alias_definis(const std::set<std::string>& alias)
		{
			_etat.alias_definis(alias);
			if(static_cast<bool>(_piege)) { _piege->alias_definis(_etat._alias_definis); }
		}
		void retire_memoire_allouee(const std::set<SVM_Valeur::Pointeur>& bloc)
		{
			_etat.retire_memoire_allouee(bloc);
			if(static_cast<bool>(_piege)) { _piege->memoire_allouee(_etat._memoire_allouee); }
		}
		void retire_alias_definis(const std::set<std::string>& alias)
		{
			_etat.retire_alias_definis(alias);
			if(static_cast<bool>(_piege)) { _piege->alias_definis(_etat._alias_definis); }
		}
		void ajoute_interruption_gestionnaire_local(const SVM_Valeur::Interruption& interruption, const SVM_Valeur::AdresseInstruction& gestionnaire, const bool cascade)
		{
			_etat.ajoute_interruption_gestionnaire_local(interruption,gestionnaire.globale(_etat.code_courant()),cascade);
			if(static_cast<bool>(_piege)) { if(cascade) _piege->interruptions_locales_cascadees(_etat._gestionnaire_interruptions_locales); else _piege->interruptions_locales(_etat._gestionnaire_interruptions_locales_ephemeres); }
		}
		bool enleve_interruption_gestionnaire_local(const SVM_Valeur::Interruption& interruption, const bool cascade)
		{
			auto r = _etat.enleve_interruption_gestionnaire_local(interruption,cascade);
			if(static_cast<bool>(_piege)) { if(cascade) _piege->interruptions_locales_cascadees(_etat._gestionnaire_interruptions_locales); else _piege->interruptions_locales(_etat._gestionnaire_interruptions_locales_ephemeres); }
			return r;
		}
		void ajoute_interruption_gestionnaire_global(const SVM_Valeur::Interruption& interruption, const SVM_Valeur::AdresseInstruction& gestionnaire, const SVM::Machine::Interruption::InterruptionSP& interruptions_materielles)
		{
			_gestionnaire_interruptions_globales.ajoute_interruption(interruption,gestionnaire.globale(_etat.code_courant()),interruptions_materielles);
			if(static_cast<bool>(_piege)) { _piege->interruptions_globales(_gestionnaire_interruptions_globales); }
		}
		bool enleve_interruption_gestionnaire_global(const SVM_Valeur::Interruption& interruption, const SVM::Machine::Interruption::InterruptionSP& interruptions_materielles)
		{
			auto r = _gestionnaire_interruptions_globales.enleve_interruption(interruption,interruptions_materielles);
			if(static_cast<bool>(_piege)) { _piege->interruptions_globales(_gestionnaire_interruptions_globales); }
			return r;
		}
		const SVM_Valeur::AdresseInstruction& interruption_gestionnaire_local(const SVM_Valeur::Interruption& interruption) { return _etat.interruption_gestionnaire_local(interruption); }
		const SVM_Valeur::AdresseInstruction& interruption_gestionnaire_global(const SVM_Valeur::Interruption& interruption) { return _gestionnaire_interruptions_globales.interruption(interruption); }
		SVM_Valeur::AdresseInstructionSP gestionnaire_interruption(const SVM_Valeur::Interruption& interruption)
		{
			if(not interruption._materielle)
			{
				SVM_Valeur::AdresseInstructionSP gestionnaire = _etat.gestionnaire_interruption(interruption);
				if(static_cast<bool>(gestionnaire))
				{
					return gestionnaire;
				}
			}
			return _gestionnaire_interruptions_globales.gestionnaire_interruption(interruption);
		}
		void interruption(const SVM_Valeur::Interruption& i)
		{
			_interruptions_en_attente.push_back(i);
			if(static_cast<bool>(_piege)) { _piege->interruptions_en_attente(_interruptions_en_attente); }
		}
		void nettoie_interruptions()
		{
			_interruptions_en_attente.clear();
			if(static_cast<bool>(_piege)) { _piege->interruptions_en_attente(_interruptions_en_attente); }
		}
		size_t interruptions_materielles_detectees() const { return _gestionnaire_interruptions_globales.interruptions_materielles_detectees(); }
		bool teste_drapeau(const std::string& drapeau) { return _etat.teste_drapeau(drapeau); }
		void ajoute_drapeau(const std::string& drapeau, const bool cascade)
		{
			_etat.ajoute_drapeau(drapeau,cascade);
			if(static_cast<bool>(_piege)) { if(cascade) _piege->drapeaux_cascades(_etat._drapeaux); else _piege->drapeaux(_etat._drapeaux_ephemeres); }
		}
		void enleve_drapeau(const std::string& drapeau, const bool cascade)
		{
			_etat.enleve_drapeau(drapeau,cascade);
			if(static_cast<bool>(_piege)) { if(cascade) _piege->drapeaux_cascades(_etat._drapeaux); else _piege->drapeaux(_etat._drapeaux_ephemeres); }
		}
		std::set<std::string> liste_drapeaux() { return _etat.liste_drapeaux(); }
		std::string nom_noyau() const { return _etat.nom_noyau(); }
		size_t taille_pile() const { return _etats_sauvegardes.size(); }
		void echange(const size_t niveau)
		{
			if(_etats_sauvegardes.size()<niveau)
			{
				throw PileRetourPasAssezGrande(niveau);
			}
			auto it = _etats_sauvegardes.begin();
			for(size_t i=0 ; i<niveau ; ++i,++it) {}
			std::swap(_etat,*it);
			if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		}
		size_t cherche_pile(const std::string& drapeau, const size_t depart)
		{
			if(_etats_sauvegardes.size()==depart)
			{
				return depart;
			}
			if(_etats_sauvegardes.size()<depart)
			{
				throw PileRetourPasAssezGrande(depart);
			}
			auto it = _etats_sauvegardes.begin();
			for(size_t i=0 ; i<depart ; ++i,++it) {}
			size_t i = depart;
			for( ; i<_etats_sauvegardes.size() ; ++i,++it)
			{
				if(it->teste_drapeau(drapeau)) break;
			}
			return i;
		}
		void deplace_pile(const size_t debut, const size_t fin, const size_t cible)
		{
			if(debut>_etats_sauvegardes.size()) { throw PileRetourPasAssezGrande(debut); }
			if(fin>_etats_sauvegardes.size()) { throw PileRetourPasAssezGrande(fin); }
			if(cible>(_etats_sauvegardes.size()-(fin-debut))) { throw PileRetourDepassement(cible); }
			auto itdebut = std::next(_etats_sauvegardes.begin(),debut);
			auto itfin = std::next(_etats_sauvegardes.begin(),fin);
			decltype(_etats_sauvegardes) etats(itdebut,itfin);
			_etats_sauvegardes.erase(itdebut,itfin);
			auto itcible = std::next(_etats_sauvegardes.begin(),cible);
			_etats_sauvegardes.insert(itcible,etats.begin(),etats.end());
			if(static_cast<bool>(_piege)) { _piege->notification_changement_objet(); }
		}
		std::list<SVM_Valeur::Interruption> liste_interruptions() const
		{
			return _interruptions_en_attente;
		}
		void utilise_interruptions(const bool activation)
		{
			_interruptions_activees.store(activation);
			if(static_cast<bool>(_piege)) { _piege->utilise_interruptions(_interruptions_activees.load(std::memory_order_relaxed)); }
		}
		bool ajoute_remplacementinstruction_local(const SVM_Valeur::AdresseInstruction& adresse, const SVM_Valeur::InstructionSP& instruction, const bool cascade)
		{
			bool b=_etat.ajoute_remplacementinstruction_local(adresse,instruction,cascade);
			if(static_cast<bool>(_piege)) { if(cascade) _piege->remplacementinstructions_local_cascadees(_etat._remplacement_instructions); else _piege->remplacementinstructions_local(_etat._remplacement_instructions_ephemeres); }
			return b;
		}
		bool teste_remplacementinstruction_local(const SVM_Valeur::AdresseInstruction& adresse) const
		{
			return _etat.teste_remplacementinstruction_local(adresse);
		}
		void enleve_remplacementinstruction_local(const SVM_Valeur::AdresseInstruction& adresse, const bool cascade)
		{
			_etat.enleve_remplacementinstruction_local(adresse,cascade);
			if(static_cast<bool>(_piege)) { if(cascade) _piege->remplacementinstructions_local_cascadees(_etat._remplacement_instructions); else _piege->remplacementinstructions_local(_etat._remplacement_instructions_ephemeres); }
		}
		bool ajoute_remplacementinstruction_global(const SVM_Valeur::AdresseInstruction& adresse, const SVM_Valeur::InstructionSP& instruction)
		{
			bool b=_remplacement_instructions_global.insert(std::make_pair(adresse,instruction)).second;
			if(static_cast<bool>(_piege)) { _piege->remplacementinstructions_global(_remplacement_instructions_global); }
			return b;
		}
		bool teste_remplacementinstruction_global(const SVM_Valeur::AdresseInstruction& adresse) const
		{
			return _remplacement_instructions_global.find(adresse)!=_remplacement_instructions_global.end();
		}
		void enleve_remplacementinstruction_global(const SVM_Valeur::AdresseInstruction& adresse)
		{
			auto it = _remplacement_instructions_global.find(adresse);
			if(it==_remplacement_instructions_global.end()) return;
			_remplacement_instructions_global.erase(it);
			if(static_cast<bool>(_piege)) { _piege->remplacementinstructions_global(_remplacement_instructions_global); }
		}
		SVM_Valeur::InstructionSP remplacementinstructions(const SVM_Valeur::AdresseInstruction& adresse) const
		{
			auto i = _etat.remplacementinstructions(adresse);
			if(static_cast<bool>(i)) return i;
			auto it = _remplacement_instructions_global.find(adresse);
			if(it==_remplacement_instructions_global.end()) return SVM_Valeur::InstructionSP();
			return it->second;
		}
		enum class Mode
		{
			NORMAL,
			PROTEGE
		};
		template<enum Mode>
		void execution(const SVM::Machine::Element::Noyau::NoyauSP& noyau);
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Processeur& p)
		{
			f	<< "Processor:" << std::endl
				<< p._etat
				<< " Saved states:" << std::endl;
			size_t indice = 0;
			for(const auto& e:p._etats_sauvegardes)
			{
				f << "  Index " << (indice++) << e;	
			}
			f	<< " Global interruption handlers:" << std::endl
				<< p._gestionnaire_interruptions_globales
				<< " Waiting interruptions";
			if(not p._interruptions_activees.load(std::memory_order_relaxed))
			{
				f	<< " (on hold)";
			}
			f	<< ":" << std::endl;
			if(not p._interruptions_en_attente.empty())
			{
				f	<< " ";
				for(const auto& i:p._interruptions_en_attente)
				{
					f	<< " " << i;
				}
				f	<< std::endl;
			}
			f	<< " Global instruction overrides:" << std::endl;
			for(const auto& i:p._remplacement_instructions_global)
			{
				f	<< "  " << i.first << ": " << (*i.second) << std::endl;
			}
			f	<< std::endl;
			Etat::liste_code_etats(f,p._etat,p._etats_sauvegardes);
			return f;
		}
		private:
			Etat _etat;
			std::list<Etat> _etats_sauvegardes;
			std::list<SVM_Valeur::Interruption> _interruptions_en_attente;
			std::atomic_bool _interruptions_activees;
			Interruptions _gestionnaire_interruptions_globales;
			SVM_Noyau::LimiteUtilisationSP _utilisation;
			Etat::RemplacementInstructions _remplacement_instructions_global;
		public:
			SVM_Noyau::NoyauWP _noyau;
			PiegeProcesseurSP _piege;
	};
}
}
}
}
