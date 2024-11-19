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
#include <src/machine/elements/valeur/instruction.h>
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/debugueur/piege.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Debugueur = SVM::Machine::Debugueur;

namespace SVM
{
namespace Machine
{
	DECL_SHARED_PTR(Machine);
namespace Debugueur
{
	DECL_SHARED_PTR(Debugueur);
}
namespace Element
{
namespace Valeur
{
	DECL_SHARED_PTR(Code);
	DECL_WEAK_PTR(Code);

	DECL_SHARED_PTR(PiegeCode);
	struct PiegeCode : public SVM_Debugueur::Piege
	{
		PiegeCode(const CodeWP& code)
		:Piege("code"), _code(code) {}
		virtual ~PiegeCode() {};
		virtual SVM_Debugueur::JSON_Interne::ObjetSP mise_a_jour() override;
		CodeWP _code;
	};

	struct AncreDejaDefinie : public ExceptionExecution
	{
		AncreDejaDefinie(const std::string& code, const std::string& ancre)
		:ExceptionExecution(Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Anchor " << ancre << " already defined in code " << code << ".");
		}
		virtual ~AncreDejaDefinie() {}
	};

	struct EtiquetteNonDefinie : public ExceptionExecution
	{
		EtiquetteNonDefinie(const std::string& code, const std::string& etiquette)
		:ExceptionExecution(Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Label " << etiquette << " is not defined in code " << code << ".");
		}
		virtual ~EtiquetteNonDefinie() {}
	};

	struct SymboleNonDefini : public ExceptionExecution
	{
		SymboleNonDefini(const std::string& code, const std::string& symbole)
		:ExceptionExecution(Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Symbol " << symbole << " is not defined in code " << code << ".");
		}
		virtual ~SymboleNonDefini() {}
	};

	struct AnalyseIncorrecte : public ExceptionExecution
	{
		AnalyseIncorrecte(const std::string& nom)
		:ExceptionExecution(Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Unable to parse code " << nom << ".");
		}
	};

	struct Code : public std::enable_shared_from_this<Code>
	{
		enum class Decorations { AUCUNE, EXTRA, CIBLES, TOUT };
		friend struct PiegeCode;
		Code(const std::string& nom, const std::string& source, const std::string& fichier)
		:_nom(nom), _source(source), _fichier(fichier)
		{
			static std::atomic<unsigned long long> identifiant(0);
			_identifiant = ++identifiant;
		}
		Code(const Code& code) = delete;
		virtual ~Code() {}
		void ajoute_instruction(const InstructionSP& instruction)
		{
			_instructions.push_back(instruction);
		}
		void ajoute_etiquette(const std::string& etiquette)
		{
			auto it = _ancres_etiquettes.insert(std::make_pair(etiquette,_instructions.size()));
			if(not it.second)
			{
				throw AncreDejaDefinie(_nom, etiquette);
			}
		}
		void ajoute_symbole(const std::string& symbole)
		{
			if(_ancres_etiquettes.find(symbole)!=_ancres_etiquettes.end())
			{
				throw AncreDejaDefinie(_nom, symbole);
			}
			auto it = _ancres_symboles.insert(std::make_pair(symbole,_instructions.size()));
			if(not it.second)
			{
				throw AncreDejaDefinie(_nom, symbole);
			}
		}
		SVM_Valeur::InstructionSP instruction(const size_t adresse) const
		{
			if(adresse>=_instructions.size())
			{
				return SVM_Valeur::InstructionSP();
			}
			return _instructions.at(adresse);
		}
		size_t resolution_etiquette(const std::string& etiquette) const
		{
			auto it = _ancres_etiquettes.find(etiquette);
			if(it!=_ancres_etiquettes.end())
			{
				return it->second;
			}
			it = _ancres_symboles.find(etiquette);
			if(it!=_ancres_symboles.end())
			{
				return it->second;
			}
			throw EtiquetteNonDefinie(_nom,etiquette);
		}
		size_t resolution_symbole(const std::string& symbole) const
		{
			auto it = _ancres_symboles.find(symbole);
			if(it==_ancres_symboles.end())
			{
				throw SymboleNonDefini(_nom,symbole);
			}
			return it->second;
		}
		void decoration(const SVM::Machine::MachineSP& machine)
		{
			Decorateur decorateur(this->shared_from_this(),machine);
			try
			{
				decoration(decorateur);
			}
			catch(ExceptionExecution& e)
			{
				Interruption i(e);
				if(static_cast<bool>(decorateur._position))
				{
					i.position(*decorateur._position);
				}
				throw i;
			}
		}
		void decoration(SVM_Valeur::Decorateur& decorateur)
		{
			for(auto& i:_instructions)
			{
				i->decoration(decorateur);
			}
		}
		std::set<std::string> etiquettes() const
		{
			std::set<std::string> re;
			for(const auto& e: _ancres_etiquettes)
			{
				re.insert(re.end(),e.first);
			}
			return re;
		}
		std::set<std::string> symboles() const
		{
			std::set<std::string> rs;
			for(const auto& s: _ancres_symboles)
			{
				rs.insert(rs.end(),s.first);
			}
			return rs;
		}
		size_t taille() const
		{
			return _instructions.size();
		}
		std::string nom() const
		{
			std::ostringstream oss;
			oss << _nom << ":" << _identifiant;
			return oss.str();
		}
		const std::string _nom;
		const std::string _source;
		const std::string _fichier;
		template<typename Flux>
		void trace(Flux& f, const SHARED_PTR(size_t)& adresse_executee = SHARED_PTR(size_t)(), const Decorations decorations = Decorations::TOUT, const std::map<size_t,size_t>& occurences = {}) const
		{
			auto etiquettes_inverses = Code::inverse(_ancres_etiquettes);
			auto symboles_inverses = Code::inverse(_ancres_symboles);
			if(decorations!=Decorations::AUCUNE)
			{
				if(not adresse_executee)
				{
					f << "Code of <" << nom() << ">:" << std::endl;
				}
				else
				{
					f << "Code at <" << nom() << "/" << (*adresse_executee) << ">:" << std::endl;
				}
			}
			std::string entete;
			if(not occurences.empty())
			{
				entete = std::string(12,' ')+'|';
				f << " Occurences | Instructions" << std::endl
				  << "------------+-----------------------------------------" << std::endl;
			}
			size_t adresse = 0;
			for(auto& i: _instructions)
			{
				bool ici = static_cast<bool>(adresse_executee) and (*adresse_executee==adresse);
				auto e = etiquettes_inverses.equal_range(adresse);
				for(auto& ee = e.first ; ee!=e.second ; ++ee)
				{
					f << entete << "  :label " << ee->second << std::endl;
				}
				auto s = symboles_inverses.equal_range(adresse);
				for(auto& ss = s.first ; ss!=s.second ; ++ss)
				{
					f << entete << "  :symbol " << ss->second << std::endl;
				}
				if(ici)
				{
					f << entete << "====== HERE ======" << std::endl;
				}
				if(not occurences.empty())
				{
					auto it = occurences.find(adresse);
					if(it==occurences.end())
					{
						f	<< entete;
					}
					else
					{
						f	<< std::setw(11) << it->second << " |";
					}
				}
				f << "     " << (*i);
				if((decorations==Decorations::EXTRA) or (decorations==Decorations::TOUT))
				{
					f << "\t# <" << adresse << ">";
					i->extra(f);
				}
				f << std::endl;
				if(ici)
				{
					f << entete << "==================" << std::endl;
				}
				++adresse;
			}
			auto e = etiquettes_inverses.equal_range(adresse);
			for(auto& ee = e.first ; ee!=e.second ; ++ee)
			{
				f << entete << "  :label " << ee->second << std::endl;
			}
			auto s = symboles_inverses.equal_range(adresse);
			for(auto& ss = s.first ; ss!=s.second ; ++ss)
			{
				f << entete << "  :symbol " << ss->second << std::endl;
			}
			if((decorations==Decorations::CIBLES) or (decorations==Decorations::TOUT))
			{
				f << entete << " Labels:" << std::endl;
				for(auto& e:_ancres_etiquettes)
				{
					f << entete << "  " << e.first << " -> <" << e.second << ">" << std::endl;
				}
				f << entete << " Symbols:" << std::endl;
				for(auto& e:_ancres_symboles)
				{
					f << entete << "  " << e.first << " -> <" << e.second << ">" << std::endl;
				}
			}
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Code& code)
		{
			code.trace(f);
			return f;
		}
		private:
		std::vector<SVM_Valeur::InstructionSP> _instructions;
		std::map<std::string, size_t> _ancres_etiquettes;
		std::map<std::string, size_t> _ancres_symboles;
		static std::multimap<size_t,std::string> inverse(const std::map<std::string, size_t>& ancres)
		{
			std::multimap<size_t,std::string> i;
			for(const auto& a: ancres)
			{
				i.insert(std::make_pair(a.second,a.first));
			}
			return i;
		}
		public:
		PiegeCodeSP _piege;
		unsigned long long int _identifiant;
	};
}
}
}
}

