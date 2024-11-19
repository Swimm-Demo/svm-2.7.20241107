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
#include <src/machine/elements/memoire/type.h>
#include <src/machine/elements/programme/valeur_programme.h>
#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/valeur/arbre_substitution.h>

namespace SVM_Memoire = SVM::Machine::Element::Memoire;
namespace SVM_Noyau = SVM::Machine::Element::Noyau;

namespace SVM
{
namespace Machine
{
namespace Extension
{
	DECL_SHARED_PTR(PatronTypeExtension);
}
namespace Element
{
namespace Programme
{
	DECL_SHARED_PTR(Type);
	
	struct Type
	{
		virtual ~Type() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) = 0;
		virtual SVM_Memoire::Type evaluation(const SVM_Noyau::NoyauSP& noyau) const = 0; 
		virtual SVM_Memoire::Type explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const = 0;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const = 0;
		virtual void format(std::ostream& os) const = 0;
		virtual void html(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Type& type)
		{
			type.format(f);
			return f;
		}
	};

	struct TypeInterne : public Type
	{
		TypeInterne(const SVM_Memoire::Type::TypeInterne& type)
		:_type(type) {}
		virtual ~TypeInterne() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override
		{
		}
		virtual SVM_Memoire::Type evaluation(const SVM_Noyau::NoyauSP& noyau) const override
		{
			return _type;
		}
		virtual SVM_Memoire::Type explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override
		{
			arbre->resultat(_type);
			return _type;
		}
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override { }
		virtual void format(std::ostream& os) const override
		{
			os << _type;
		}
		virtual void html(std::ostream& os) const override
		{
			os << "<span class=\"type\">" << _type << "</span>";
		}
		private:
			SVM_Memoire::Type _type;
	};

	struct TypeExtension : public Type
	{
		TypeExtension(const ValeurSP& pep)
		:_pep(pep) {}
		virtual ~TypeExtension() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override;
		virtual SVM_Memoire::Type evaluation(const SVM_Noyau::NoyauSP& noyau) const override;
		virtual SVM_Memoire::Type explique_calcul(const SVM_Noyau::NoyauSP& noyau, SVM_Valeur::ArbreSubstitutionSP& arbre) const override;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override
		{
			std::ostringstream pep;
			_pep->explique(noyau,pep);
			explications.push_back(pep.str());
		}
		virtual void format(std::ostream& os) const override
		{
			os << *_pep;
		}
		virtual void html(std::ostream& os) const override
		{
			_pep->html(os);
		}
		private:
			ValeurSP _pep;
			SVM_Extension::PatronTypeExtensionSP _patron;
	};

	DECL_SHARED_PTR(DescriptionBloc);
	struct DescriptionBloc
	{
		virtual ~DescriptionBloc() {}	
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) = 0;
		virtual std::vector<SVM_Memoire::Type> evaluation(const SVM_Noyau::NoyauSP& noyau) const = 0;
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const = 0;
		virtual void format(std::ostream& os) const = 0;
		virtual void html(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const DescriptionBloc& bloc)
		{
			bloc.format(f);
			return f;
		}
		
	};

	struct RepetitionNegative : public SVM_Valeur::ExceptionExecution
	{
		RepetitionNegative(const long int repetition)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Repetition is a negative number " << repetition << ".");
		}
	};

	struct DescriptionBlocAtome : public DescriptionBloc
	{
		DescriptionBlocAtome(const TypeSP& type, const ValeurSP& repetition)
		:_type(type), _repetition(repetition) {}
		virtual ~DescriptionBlocAtome() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override
		{
			_type->decoration(decorateur);
			if(static_cast<bool>(_repetition))
			{
				_repetition->decoration(decorateur);
			}
		}
		virtual std::vector<SVM_Memoire::Type> evaluation(const SVM_Noyau::NoyauSP& noyau) const override
		{
			SVM_Memoire::Type type = _type->evaluation(noyau);
			if(not _repetition)
			{
				return {type};
			}
			long int repetition = _repetition->evaluation_entier(noyau);
			if(repetition<0)
			{
				throw RepetitionNegative(repetition);
			}
			return std::vector<SVM_Memoire::Type>(repetition,type);
		}
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override
		{
			_type->explication(noyau,explications);
			if(not _repetition) return;
			std::ostringstream rep;
			_repetition->explique(noyau,rep);
			explications.push_back(rep.str());
		}
		virtual void format(std::ostream& os) const override
		{
			os << *_type;
			if(static_cast<bool>(_repetition))
			{
				os << "*" << *_repetition;
			}
		}
		virtual void html(std::ostream& os) const override
		{
			_type->html(os);
			if(static_cast<bool>(_repetition))
			{
				os << "*";
				_repetition->html(os);
			}
		}
		private:
		TypeSP _type;
		ValeurSP _repetition;
	};

	struct DescriptionBlocCompose : public DescriptionBloc
	{
		DescriptionBlocCompose(const std::vector<DescriptionBlocSP>& sous_blocs, const ValeurSP& repetition)
		:_sous_blocs(sous_blocs), _repetition(repetition) {}
		virtual ~DescriptionBlocCompose() {}
		virtual void decoration(SVM_Valeur::Decorateur& decorateur) override
		{
			for(auto& sb : _sous_blocs)
			{
				sb->decoration(decorateur);
			}
			if(static_cast<bool>(_repetition))
			{
				_repetition->decoration(decorateur);
			}
		}
		virtual std::vector<SVM_Memoire::Type> evaluation(const SVM_Noyau::NoyauSP& noyau) const override
		{
			std::vector<SVM_Memoire::Type> sous_bloc;
			for(auto& sb : _sous_blocs)
			{
				auto ssb = sb->evaluation(noyau);
				sous_bloc.insert(sous_bloc.end(),ssb.begin(),ssb.end());
			}
			if(not _repetition)
			{
				return sous_bloc;
			}
			long int repetition = _repetition->evaluation_entier(noyau);
			if(repetition<0)
			{
				throw RepetitionNegative(repetition);
			}
			std::vector<SVM_Memoire::Type> sb;
			for(long int i=0 ; i<repetition ; ++i)
			{
				sb.insert(sb.end(),sous_bloc.begin(),sous_bloc.end());
			}
			return sb;
		}
		virtual void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const override
		{
			for(auto& sb : _sous_blocs)
			{
				sb->explication(noyau,explications);
			}
			if(not _repetition) return;
			std::ostringstream rep;
			_repetition->explique(noyau,rep);
			explications.push_back(rep.str());
		}
		virtual void format(std::ostream& os) const override
		{
			os << "(";
			bool virgule=false;
			for(auto& sb:_sous_blocs)
			{
				if(virgule)
					os << " ,";
				virgule=true;
				os << " " << *sb;
			}
			os << " )";
			if(static_cast<bool>(_repetition))
			{
				os << "*" << *_repetition;
			}
		}
		virtual void html(std::ostream& os) const override
		{
			os << "(";
			bool virgule=false;
			for(auto& sb:_sous_blocs)
			{
				if(virgule)
					os << " ,";
				virgule=true;
				os << " ";
				sb->html(os);
			}
			os << " )";
			if(static_cast<bool>(_repetition))
			{
				os << "*";
				_repetition->html(os);
			}
		}
		private:
			std::vector<DescriptionBlocSP> _sous_blocs;
			ValeurSP _repetition;
	};

	struct AliasDuplique : public SVM_Valeur::ExceptionExecution
	{
		AliasDuplique(const std::string& alias)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Alias " << alias << " duplicated in bloc description.");
		}
	};

	DECL_SHARED_PTR(DescriptionBlocAlias);
	struct DescriptionBlocAlias
	{
		DescriptionBlocAlias(const DescriptionBlocSP& bloc, const std::string& alias = "")
		:_bloc(bloc), _alias(alias) {}
		void decoration(SVM_Valeur::Decorateur& decorateur)
		{
			_bloc->decoration(decorateur);
		}
		void evaluation(const SVM_Noyau::NoyauSP& noyau, SVM_Memoire::DescriptionBloc& description_bloc) const
		{
			auto sb = _bloc->evaluation(noyau);
			if(not _alias.empty())
			{
				if(not description_bloc._alias.insert(std::make_pair(_alias,SVM_Memoire::SousBloc(description_bloc._elements.size(),sb.size()))).second)
				{
					throw AliasDuplique(_alias);
				}
			}
			description_bloc._elements.insert(description_bloc._elements.end(),sb.begin(),sb.end());
		}
		void explication(const SVM_Noyau::NoyauSP& noyau, std::list<std::string>& explications) const
		{
			_bloc->explication(noyau,explications);
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const DescriptionBlocAlias& description_bloc)
		{
			f << *description_bloc._bloc;
			if(not description_bloc._alias.empty())
			{
				f << "/" << description_bloc._alias;
			}
			return f;
		}
		void html(std::ostream& os) const
		{
			_bloc->html(os);
			if(not _alias.empty())
			{
				os << "<span class=\"operator\">/</span>" << _alias;
			}
		}
		private:
			DescriptionBlocSP _bloc;
			std::string _alias;
	};

}
}
}
}
