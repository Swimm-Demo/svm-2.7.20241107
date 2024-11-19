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

namespace SVM_Valeur = SVM::Machine::Element::Valeur;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Valeur
{
	struct Exception
	{
		Exception() {}
		virtual ~Exception() {};
	};

#define DETAILS_EXCEPTION(details) do { std::ostringstream oss ; oss << details ; _details = oss.str(); } while(false)

	struct ExceptionDemarrage : public Exception
	{
		virtual ~ExceptionDemarrage() {};
		template<typename Flux>
			friend Flux& operator<<(Flux& f, const ExceptionDemarrage& exception)
			{
				f << "Boot error: " << exception._details;
				return f;
			}
		std::string _details;
	};

	struct ExceptionExecution : public Exception
	{
		ExceptionExecution(const SVM_Valeur::Interruption::InterruptionInterne interruption)
		:_interruption(interruption) {}
		virtual ~ExceptionExecution() {};
		operator SVM_Valeur::Interruption () const
		{
			return SVM_Valeur::Interruption(_interruption,_details);
		}
		template<typename Flux>
			friend Flux& operator<<(Flux& f, const ExceptionExecution& exception)
			{
				f << "Runtime error (" << SVM_Valeur::Interruption::texte(exception._interruption) << "): " << exception._details;
				return f;
			}
		protected:
		SVM_Valeur::Interruption::InterruptionInterne _interruption;
		std::string _details;
	};
}
}
}
}

