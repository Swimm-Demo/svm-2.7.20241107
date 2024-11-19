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
#include <src/machine/elements/memoire/valeur.h>
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/elements/valeur/position_instruction.h>
#include <src/machine/elements/valeur/decorateur.h>
// ici, juste le concept d'instruction
// les differentes instructions seront definies ailleurs

namespace SVM_Memoire = SVM::Machine::Element::Memoire;
namespace SVM_Extension = SVM::Machine::Extension;

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
namespace Noyau
{
	DECL_SHARED_PTR(Noyau);
}
namespace Processeur
{
	struct Processeur;
namespace Instruction
{
	struct Extension;
	struct Bibliotheque;
	struct AffectationInstruction;
}
}
namespace Valeur
{

namespace SVM_Noyau = SVM::Machine::Element::Noyau;

DECL_SHARED_PTR(Instruction);

struct InstructionSansRetour : public SVM_Valeur::ExceptionExecution
{
	InstructionSansRetour(const Instruction* i);
	virtual ~InstructionSansRetour() {}
};

struct Instruction
{
	friend struct Processeur::Processeur;
	friend struct Processeur::Instruction::Extension;
	friend struct Processeur::Instruction::Bibliotheque;
	friend struct Processeur::Instruction::AffectationInstruction;
	friend struct Interface::Outils;
	explicit Instruction(const PositionInstruction& position, const bool attente=false, const bool systeme=false)
	:_position(position), _attente(attente), _systeme(systeme) {};
	Instruction(const Instruction& instruction) = delete;
	Instruction(Instruction&& instruction) = delete;
	virtual ~Instruction() {};
	virtual void decoration(Decorateur& decorateur) = 0;
	virtual void execution(const SVM_Noyau::NoyauSP& noyau) const
	{
		execution_avec_resultat(noyau);
	}
	virtual SVM_Memoire::ValeurSP execution_avec_resultat(const SVM_Noyau::NoyauSP& noyau) const
	{
		throw InstructionSansRetour(this);
	}
	virtual void format(std::ostream& os) const = 0;
	virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const = 0;
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const Instruction& instruction)
	{
		instruction.format(f);
		return f;
	}
	template<typename Flux>
	void extra(Flux& f) const
	{
		f << " " << _position;
		if(_attente)
		{
			f << " WAITING";
		}
		if(_systeme)
		{
			f << " SYSTEM";
		}
	}
	std::string texte_html() const
	{
		std::ostringstream oss;
		oss << "<pre class=\"code\">";
		html(oss);
		oss << "</pre>";
		return oss.str();
	}
	virtual void html(std::ostream& os) const = 0;
	std::string extra_html(const size_t adresse) const
	{
		std::ostringstream oss;
		oss << "<pre class=\"extra\">Local address: &lt;" << adresse << "&gt;   Source location:";
		extra(oss);
		oss << "</pre>";
		return oss.str();
	}
	const PositionInstruction& position() const
	{
		return _position;
	}
	bool systeme() const
	{
		return _systeme;
	}
	bool attente() const
	{
		return _attente;
	}
	protected:
	void position(Decorateur& decorateur)
	{
		decorateur._position = std::make_shared<PositionInstruction>(_position);
	}
	void sans_position(Decorateur& decorateur)
	{
		decorateur._position.reset();
	}
	private:
		PositionInstruction _position;
		bool _attente;
		bool _systeme;
};

}
}
}
}
