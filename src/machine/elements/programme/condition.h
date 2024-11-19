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
#include <src/machine/elements/valeur/expression_condition.h>
#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/valeur/decorateur.h>
namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Noyau = SVM::Machine::Element::Noyau;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Programme
{
	DECL_SHARED_PTR(Condition);
	struct Condition
	{
		Condition() = default;
		virtual ~Condition() {};
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) = 0;
		virtual bool evaluation(const SVM_Noyau::NoyauSP& ce) const = 0;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const = 0;
		virtual void format(std::ostream& os) const = 0;
		virtual void html(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Condition& c)
		{
			c.format(f);
			return f;
		}
	};

	struct PasDeCondition : public Condition
	{
		PasDeCondition() = default;
		virtual ~PasDeCondition() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override {}
		virtual bool evaluation(const SVM_Noyau::NoyauSP& ce) const override { return true; }
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override { }
		virtual void format(std::ostream& os) const override {}
		virtual void html(std::ostream& os) const override {}
	};

	struct ConditionNormale : public Condition
	{
		ConditionNormale(const SVM_Valeur::ExpressionConditionSP& expression)
		:_expression(expression) {}
		virtual ~ConditionNormale() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override { _expression->decoration(decorateur); }
		virtual bool evaluation(const SVM_Noyau::NoyauSP& ce) const override { return _expression->evaluation(ce); }
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override { _expression->explication(noyau,explications); }
		virtual void format(std::ostream& os) const override
		{
			os << " :when " << *_expression;
		}
		virtual void html(std::ostream& os) const override
		{
			os << " <span class=\"instruction\">:when</span> ";
			_expression->html(os);
		}
		private:
			SVM_Valeur::ExpressionConditionSP _expression;
	};

	struct ConditionInverse : public Condition
	{
		ConditionInverse(const SVM_Valeur::ExpressionConditionSP& expression)
		:_expression(expression) {}
		virtual ~ConditionInverse() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override { _expression->decoration(decorateur); }
		virtual bool evaluation(const SVM_Noyau::NoyauSP& ce) const override { return not _expression->evaluation(ce); }
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override { _expression->explication(noyau,explications); }
		virtual void format(std::ostream& os) const override
		{
			os << " :unless " << *_expression;
		}
		virtual void html(std::ostream& os) const override
		{
			os << " <span class=\"instruction\">:unless</span> ";
			_expression->html(os);
		}
		private:
			SVM_Valeur::ExpressionConditionSP _expression;
	};
}
}
}
}

