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
#include <src/machine/elements/valeur/adresse_instruction.h>
#include <src/machine/elements/valeur/pointeur.h>
#include <src/machine/elements/processeur/interruptions.h>
#include <src/machine/elements/memoire/memoire.h>
#include <src/machine/debugueur/json.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Memoire = SVM::Machine::Element::Memoire;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Processeur
{
	struct Processeur;
	struct Etat
	{
		friend struct Processeur;
		typedef std::map<SVM_Valeur::AdresseInstruction,SVM_Valeur::InstructionSP> RemplacementInstructions;
		Etat(const SVM_Valeur::AdresseInstruction& code)
		:_prochaine_instruction(code),_instruction_courante(code), _memoire_courante(SVM_Valeur::AdresseMemoire(0),0) { }
		SVM_Valeur::AdresseInstruction prochaine_instruction() { _instruction_courante = _prochaine_instruction; return _prochaine_instruction++; }
		SVM_Valeur::AdresseInstruction prochaine_instruction_sans_increment() { return _prochaine_instruction; }
		SVM_Valeur::AdresseInstruction instruction_courante() { return _instruction_courante; }
		SVM_Valeur::CodeSP code_courant() const { return _instruction_courante._code; }
		void saut(const SVM_Valeur::AdresseInstruction& cible) { _prochaine_instruction = cible; }
		void appel(const SVM_Valeur::AdresseInstruction& cible, const SVM_Valeur::Pointeur& parametres)
		{
			_prochaine_instruction = cible;
			_memoire_courante = parametres;
			_memoire_allouee.clear();
			_alias_definis.clear();
			_drapeaux_ephemeres.clear();
			_gestionnaire_interruptions_locales_ephemeres.vide();
			_remplacement_instructions_ephemeres.clear();
		}
		SVM_Valeur::Pointeur memoire_courante() const { return _memoire_courante; }
		void memoire_courante(const SVM_Valeur::Pointeur& pointeur) { _memoire_courante = pointeur; }
		void nettoyage(SVM_Memoire::Memoire& memoire)
		{
			memoire.liberation(_memoire_allouee);
			memoire.suppression_alias(_alias_definis);
		}
		void memoire_allouee(const std::set<SVM_Valeur::Pointeur>& bloc)
		{
			_memoire_allouee.insert(bloc.begin(),bloc.end());
			rationalisation_memoire_allouee();
		}
		void alias_definis(const std::set<std::string>& alias)
		{
			_alias_definis.insert(alias.begin(),alias.end());
		}
		void retire_memoire_allouee(const std::set<SVM_Valeur::Pointeur>& bloc)
		{
			for(const auto& p: bloc)
			{
				decltype(_memoire_allouee) temp;
				for(const auto& ma: _memoire_allouee)
				{
					auto e = ma.evide(p);
					if(not e.first.vide())
						temp.insert(e.first);
					if(not e.second.vide())
						temp.insert(e.second);
				}
				_memoire_allouee.swap(temp);
			}
			rationalisation_memoire_allouee();
		}
		void retire_alias_definis(const std::set<std::string>& alias)
		{
			for(const auto& a: alias)
			{
				auto it=_alias_definis.find(a);
				if(it!=_alias_definis.end())
				{
					_alias_definis.erase(it);
				}
			}
		}
		void ignore_memoire()
		{
			_memoire_allouee.clear();
			_alias_definis.clear();
		}
		void ajoute_interruption_gestionnaire_local(const SVM_Valeur::Interruption& interruption, const SVM_Valeur::AdresseInstruction& gestionnaire, const bool cascade)
		{
			if(cascade)
			{
				_gestionnaire_interruptions_locales.ajoute_interruption(interruption,gestionnaire);
			}
			else
			{
				_gestionnaire_interruptions_locales_ephemeres.ajoute_interruption(interruption,gestionnaire);
			}
		}
		bool enleve_interruption_gestionnaire_local(const SVM_Valeur::Interruption& interruption, const bool cascade)
		{
			if(cascade)
			{
				return _gestionnaire_interruptions_locales.enleve_interruption(interruption);
			}
			else
			{
				return _gestionnaire_interruptions_locales_ephemeres.enleve_interruption(interruption);
			}
		}
		const SVM_Valeur::AdresseInstruction& interruption_gestionnaire_local(const SVM_Valeur::Interruption& interruption)
		{
			auto i = _gestionnaire_interruptions_locales_ephemeres.gestionnaire_interruption(interruption);
			if(not i)
			{
				return _gestionnaire_interruptions_locales.interruption(interruption);
			}
			else
			{
				return *i;
			}
		}
		SVM_Valeur::AdresseInstructionSP gestionnaire_interruption(const SVM_Valeur::Interruption& interruption)
		{
			auto i = _gestionnaire_interruptions_locales_ephemeres.gestionnaire_interruption(interruption);
			if(static_cast<bool>(i)) return i;
			return _gestionnaire_interruptions_locales.gestionnaire_interruption(interruption);
		}
		bool teste_drapeau(const std::string& drapeau)
		{
			bool e = _drapeaux_ephemeres.find(drapeau)!=_drapeaux_ephemeres.end();
			if(e) return true;
			return _drapeaux.find(drapeau)!=_drapeaux.end();
		}
		void ajoute_drapeau(const std::string& drapeau, const bool cascade)
		{
			if(cascade)
			{
				_drapeaux.insert(drapeau);
			}
			else
			{
				_drapeaux_ephemeres.insert(drapeau);
			}
		}
		void enleve_drapeau(const std::string& drapeau, const bool cascade)
		{
			if(cascade)
			{
				auto it = _drapeaux.find(drapeau);
				if(it!=_drapeaux.end())
				{
					_drapeaux.erase(it);
				}
			}
			else
			{
				auto it = _drapeaux_ephemeres.find(drapeau);
				if(it!=_drapeaux_ephemeres.end())
				{
					_drapeaux_ephemeres.erase(it);
				}
			}
		}
		bool ajoute_remplacementinstruction_local(const SVM_Valeur::AdresseInstruction& adresse, const SVM_Valeur::InstructionSP& instruction, const bool cascade)
		{
			if(cascade)
			{
				return _remplacement_instructions.insert(std::make_pair(adresse,instruction)).second;
			}
			else
			{
				return _remplacement_instructions_ephemeres.insert(std::make_pair(adresse,instruction)).second;
			}
		}
		bool teste_remplacementinstruction_local(const SVM_Valeur::AdresseInstruction& adresse) const
		{
			bool e = _remplacement_instructions_ephemeres.find(adresse)!=_remplacement_instructions_ephemeres.end();
			if(e) return true;
			return _remplacement_instructions.find(adresse)!=_remplacement_instructions.end();
		}
		void enleve_remplacementinstruction_local(const SVM_Valeur::AdresseInstruction& adresse, const bool cascade)
		{
			if(cascade)
			{
				auto it = _remplacement_instructions.find(adresse);
				if(it!=_remplacement_instructions.end())
				{
					_remplacement_instructions.erase(it);
				}
			}
			else
			{
				auto it = _remplacement_instructions_ephemeres.find(adresse);
				if(it!=_remplacement_instructions_ephemeres.end())
				{
					_remplacement_instructions_ephemeres.erase(it);
				}
			}
		}
		SVM_Valeur::InstructionSP remplacementinstructions(const SVM_Valeur::AdresseInstruction& adresse) const
		{
			auto it = _remplacement_instructions_ephemeres.find(adresse);
			if(it!=_remplacement_instructions_ephemeres.end())
				return it->second;
			it = _remplacement_instructions.find(adresse);
			if(it!=_remplacement_instructions.end())
				return it->second;
			return SVM_Valeur::InstructionSP();
		}
		std::set<std::string> liste_drapeaux()
		{
			std::set<std::string> drapeaux = _drapeaux_ephemeres;
			drapeaux.insert(_drapeaux.begin(),_drapeaux.end());
			return drapeaux;
		}
		std::string nom_noyau() const
		{
			return _instruction_courante.nom_noyau();
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Etat& e)
		{
			f	<< " State:" << std::endl
				<< "   Next instruction: " << e._prochaine_instruction << " (Current one: " << e._instruction_courante << ")" << std::endl
				<< "   Current memory  : " << e._memoire_courante << std::endl
				<< "   Allocated memory:";
			for(auto& ma:e._memoire_allouee)
			{
				f << " " << ma;
			}
			f	<< std::endl
				<< "   Aliases         :";
			for(auto& ma:e._alias_definis)
			{
				f << " " << ma;
			}
			f	<< std::endl
				<< "   Flags           :" ;
			for(auto& d:e._drapeaux_ephemeres)
			{
				f << " " << d;
			}
			f	<< std::endl
				<< "   Cascaded flags  :" ;
			for(auto& d:e._drapeaux)
			{
				f << " " << d;
			}
			f	<< std::endl
				<< "   Local interruption handlers:" << std::endl
				<< e._gestionnaire_interruptions_locales_ephemeres
				<< "   Cascaded local interruption handlers:" << std::endl
				<< e._gestionnaire_interruptions_locales
				<< "   Local instruction overrides:" << std::endl;
			for(auto& i:e._remplacement_instructions_ephemeres)
			{
				f	<< "    " << i.first << ": " << (*i.second) << std::endl;
			}
			f	<< "   Cascaded local instruction overrides:" << std::endl;
			for(auto& i:e._remplacement_instructions)
			{
				f	<< "    " << i.first << ": " << (*i.second) << std::endl;
			}

			return f;
		}
		template<typename Flux>
		static void liste_code_etats(Flux& f, const Etat& courant, const std::list<Etat>& sauvegardes)
		{
			std::set<SVM_Valeur::AdresseInstruction> adresses_courantes = { courant._instruction_courante };
			courant.liste_code(f);
			f << std::endl;
			for(const auto& e: sauvegardes)
			{
				if(adresses_courantes.insert(e._instruction_courante).second)
				{
					e.liste_code(f);
				}
				else
				{
					f << "Code at " << e._instruction_courante << ": already generated" << std::endl;
				}
				f << std::endl;
			}
		}
		template<typename Flux>
		void liste_code(Flux& flux) const
		{
			_instruction_courante.liste_code(flux);
		}
		static SVM_Debugueur::JSON_Interne::TableauSP debugueur(const Etat::RemplacementInstructions& r)
		{
			auto io = SVM_Debugueur::JSON::tableau();
			for(const auto& i:r)
			{
				std::ostringstream oss;
				oss << (*i.second);
				auto ii = SVM_Debugueur::JSON::objet({ { "address", i.first.debugueur() } , { "instruction", SVM_Debugueur::JSON::chaine(oss.str()) } });
				io->ajoute(ii);
			}
			return io;
		}
		SVM::Machine::Debugueur::JSON_Interne::ObjetSP debugueur() const
		{
			auto d = SVM::Machine::Debugueur::JSON::objet();
			d->ajoute("next_instruction",_prochaine_instruction.debugueur());
			d->ajoute("current_instruction",_instruction_courante.debugueur());
			d->ajoute("current_memory",_memoire_courante.debugueur());
			auto ma = SVM::Machine::Debugueur::JSON::tableau();
			for(const auto& m: _memoire_allouee)
			{
				ma->ajoute(m.debugueur());
			}
			d->ajoute("allocated_memory",ma);
			auto ad = SVM::Machine::Debugueur::JSON::tableau();
			for(const auto& a: _alias_definis)
			{
				ad->ajoute(SVM::Machine::Debugueur::JSON::chaine(a));
			}
			d->ajoute("defined_aliases",ad);
			d->ajoute("local_interruptions",_gestionnaire_interruptions_locales_ephemeres.debugueur());
			d->ajoute("cascaded_local_interruptions",_gestionnaire_interruptions_locales.debugueur());
			auto dd = SVM::Machine::Debugueur::JSON::tableau();
			for(const auto& i: _drapeaux_ephemeres)
			{
				dd->ajoute(SVM::Machine::Debugueur::JSON::chaine(i));
			}
			d->ajoute("flags",dd);
			auto cdd = SVM::Machine::Debugueur::JSON::tableau();
			for(const auto& i: _drapeaux)
			{
				cdd->ajoute(SVM::Machine::Debugueur::JSON::chaine(i));
			}
			d->ajoute("cascaded_flags",cdd);
			d->ajoute("instruction_overrides",Etat::debugueur(_remplacement_instructions_ephemeres));
			d->ajoute("cascaded_instruction_overrides",Etat::debugueur(_remplacement_instructions));
			return d;
		}
		private:
		void rationalisation_memoire_allouee()
		{
			decltype(_memoire_allouee) temp;
			SVM_Valeur::Pointeur c(SVM_Valeur::AdresseMemoire(0),0);
			for(auto& p:_memoire_allouee)
			{
				if(c.inclus(p)) continue;
				if(not c.colle(p))
				{
					if(c.taille()>0)
					{
						temp.insert(temp.end(),c);
					}
					c=p;
				}
			}
			if(c.taille()>0)
			{
				temp.insert(temp.end(),c);
			}
			_memoire_allouee.swap(temp);
		}
		SVM_Valeur::AdresseInstruction _prochaine_instruction;
		SVM_Valeur::AdresseInstruction _instruction_courante; // pour debug seulement (et retention)
		SVM_Valeur::Pointeur _memoire_courante;
		std::set<SVM_Valeur::Pointeur> _memoire_allouee;
		std::set<std::string> _alias_definis;
		Interruptions _gestionnaire_interruptions_locales_ephemeres;
		Interruptions _gestionnaire_interruptions_locales;
		std::set<std::string> _drapeaux_ephemeres;
		std::set<std::string> _drapeaux;
		RemplacementInstructions _remplacement_instructions_ephemeres;
		RemplacementInstructions _remplacement_instructions;
	};
}
}
}
}
