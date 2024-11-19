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

#include <src/machine/elements/valeur/interruption.h>
#include <src/global/global.h>
#include <src/machine/elements/valeur/point_entree_extension.h>
#include <src/machine/elements/valeur/position_instruction.h>
#include <src/machine/debugueur/json.h>

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Interface
{
	struct Outils;
}
namespace Element
{
namespace Programme
{
	struct InterruptionExterne;
}
namespace Processeur
{
	struct Interruptions;
}
namespace Valeur
{
	namespace SVM_Programme = SVM::Machine::Element::Programme;

	DECL_SHARED_PTR(Interruption);

	struct Interruption 
	{
		friend struct SVM::Machine::Interface::Outils;
		friend struct SVM_Programme::InterruptionExterne;
		friend struct SVM::Machine::Element::Processeur::Interruptions;
		enum class InterruptionInterne
		{
			ECHEC, // ABRT, QUIT
			PROCESSEUR, // ILL
			MEMOIRE,
			TERMINAISON, // TERM, INT, HUP
			NUMERIQUE, // FPE
			PERIPHERIQUE, // PIPE, BUS
			CLONE, // CHLD
			PREMIER, // USR1
			SECOND, // USR2
			TERMINAL, // TTIN, TTOU, WINCH
			GENERIQUE, // autres
			SECURITE,
			INTERRUPTION_EXTENSION
		};
		Interruption(const InterruptionInterne interne, const std::string& details, const bool materielle = false)
			:_materielle(materielle),_interruption_interne(interne),_details(details)
		{
			ASSERT((interne!=InterruptionInterne::INTERRUPTION_EXTENSION),"Une interruption interne ne peut pas etre de type INTERRUPTION_EXTENSION");
		}
		Interruption(const SVM_Valeur::PointEntreeExtension& externe, const std::string& details, const bool materielle = false)
			:_materielle(materielle),_interruption_interne(InterruptionInterne::INTERRUPTION_EXTENSION),_point_entree_extension(externe),_details(details)
		{
		}
		Interruption(const Interruption& interruption)
			:_materielle(interruption._materielle),_interruption_interne(interruption._interruption_interne),_point_entree_extension(interruption._point_entree_extension), _details(interruption._details), _position(interruption._position)
		{
		}
		Interruption& operator=(const Interruption& interruption)
		{
			const_cast<bool&>(_materielle) = interruption._materielle;
			_interruption_interne = interruption._interruption_interne;
			_point_entree_extension = interruption._point_entree_extension;
			_details = interruption._details;
			_position = interruption._position;
			return *this;
		}
		static std::string texte(const InterruptionInterne interruption_interne)
		{
			switch(interruption_interne)
			{
				case InterruptionInterne::ECHEC: return "FAILURE"; break;
				case InterruptionInterne::PROCESSEUR: return "PROCESSOR"; break;
				case InterruptionInterne::MEMOIRE: return "MEMORY"; break;
				case InterruptionInterne::TERMINAISON: return "TERMINATE"; break;
				case InterruptionInterne::NUMERIQUE: return "NUMERIC"; break;
				case InterruptionInterne::PREMIER: return "FIRST"; break;
				case InterruptionInterne::SECOND: return "SECOND"; break;
				case InterruptionInterne::SECURITE: return "SECURITY"; break;
				case InterruptionInterne::PERIPHERIQUE: return "DEVICE"; break;
				case InterruptionInterne::CLONE: return "CLONE"; break;
				case InterruptionInterne::TERMINAL: return "TERMINAL"; break;
				case InterruptionInterne::GENERIQUE: return "GENERIC"; break;
				case InterruptionInterne::INTERRUPTION_EXTENSION: return "custom"; break;
			}
			return "unknown";
		}
		static std::set<Interruption> internes()
		{
			std::set<Interruption> i;
			i.insert(Interruption(InterruptionInterne::ECHEC,""));
			i.insert(Interruption(InterruptionInterne::PROCESSEUR,""));
			i.insert(Interruption(InterruptionInterne::MEMOIRE,""));
			i.insert(Interruption(InterruptionInterne::TERMINAISON,""));
			i.insert(Interruption(InterruptionInterne::NUMERIQUE,""));
			i.insert(Interruption(InterruptionInterne::PERIPHERIQUE,""));
			i.insert(Interruption(InterruptionInterne::CLONE,""));
			i.insert(Interruption(InterruptionInterne::PREMIER,""));
			i.insert(Interruption(InterruptionInterne::SECOND,""));
			i.insert(Interruption(InterruptionInterne::TERMINAL,""));
			i.insert(Interruption(InterruptionInterne::GENERIQUE,""));
			i.insert(Interruption(InterruptionInterne::SECURITE,""));
			return i;
		}
		bool operator==(const InterruptionInterne& i) const
		{
			return _interruption_interne==i;
		}
		bool operator==(const Interruption& i) const
		{
			if(_interruption_interne!=i._interruption_interne)
			{
				return false;
			}
			if(_interruption_interne!=InterruptionInterne::INTERRUPTION_EXTENSION)
				return true;
			return _point_entree_extension==i._point_entree_extension;
		}
		bool operator!=(const Interruption& i) const
		{
			return not ((*this)==i);
		}
		bool operator<(const Interruption& i) const
		{
			if(_interruption_interne!=i._interruption_interne)
			{
				return _interruption_interne<i._interruption_interne;
			}
			return _point_entree_extension<i._point_entree_extension;
		}
		bool operator>(const Interruption& i) const
		{
			if(_interruption_interne!=i._interruption_interne)
			{
				return _interruption_interne>i._interruption_interne;
			}
			return _point_entree_extension>i._point_entree_extension;
		}
		void position(const SVM_Valeur::PositionInstruction& position)
		{
			if(not _position)
			{
				_position = std::make_shared<SVM_Valeur::PositionInstruction>(position);
			}
		}
		template<typename Flux>
			friend Flux& operator<<(Flux& f, const Interruption& interruption)
			{
				if(interruption._interruption_interne!=InterruptionInterne::INTERRUPTION_EXTENSION)
				{
					f << texte(interruption._interruption_interne);
				}
				else
				{
					f << "!" << interruption._point_entree_extension;
				}
				return f;
			}
		void html(std::ostream& os) const
		{
			if(_interruption_interne!=InterruptionInterne::INTERRUPTION_EXTENSION)
			{
				os << "<span class=\"keyword\">" << texte(_interruption_interne) << "</span>";
			}
			else
			{
				os << "!";
				_point_entree_extension.html(os);
			}
		}
		SVM::Machine::Debugueur::JSON_Interne::ChaineSP debugueur() const
		{
			std::ostringstream oss;
			oss << (*this);
			return SVM::Machine::Debugueur::JSON::chaine(oss.str());
		}
		template<typename Flux>
			void erreur(Flux& f) const
			{
				if(static_cast<bool>(_position))
				{
					f << (*_position) << " ";
				}
				f << "Interruption " << (*this) << " not handled: " << _details;
			}
		template<typename Flux>
			void rapport(Flux& f) const
			{
				f	<< "Interruption " << (*this) << ":" << std::endl
					<< _details << std::endl;
				if(static_cast<bool>(_position))
				{
					f << (*_position) << std::endl;
				}
			}
		template<typename Flux>
			void erreur_demarrage(Flux& f) const
			{
				if(static_cast<bool>(_position))
				{
					f << (*_position) << " ";
				}
				f << _details;
			}
		const bool _materielle;
		private:
		InterruptionInterne _interruption_interne;
		SVM_Valeur::PointEntreeExtension _point_entree_extension;
		std::string _details; // pour un humain, peut etre multi lignes
		SVM_Valeur::PositionInstructionSP _position; // pour un humain aussi
	};
}
}
}
}
