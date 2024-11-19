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

namespace SVM
{
namespace Machine
{
namespace Debugueur
{
namespace JSON_Interne
{
	DECL_SHARED_PTR(Valeur);
	struct Valeur
	{
		virtual ~Valeur() {};
		virtual void format(std::ostream& os) const = 0;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Valeur& v)
		{
			v.format(f);
			return f;
		}
	};

	DECL_SHARED_PTR(Entier);
	struct Entier : public Valeur
	{
		typedef long long int Type;
		explicit Entier(const Type v)
		:_v(v) {}
		virtual ~Entier() {}
		virtual void format(std::ostream& os) const override
		{
			os << _v;
		}
		operator Type () const { return _v; }
		private:
			Type _v;
	};

	DECL_SHARED_PTR(Chaine);
	struct Chaine : public Valeur
	{
		typedef std::string Type;
		explicit Chaine(const Type& v)
		:_v(v) {}
		virtual ~Chaine() {}
		virtual void format(std::ostream& os) const override
		{
			os << '"' << SVM::Global::Chaine::echappe(_v) << '"';
		}
		operator const Type& () const { return _v; }
		private:
			Type _v;
	};

	DECL_SHARED_PTR(Booleen);
	struct Booleen : public Valeur
	{
		typedef bool Type;
		explicit Booleen(const Type v)
		:_v(v) {}
		virtual ~Booleen() {}
		virtual void format(std::ostream& os) const override
		{
			os << (_v?"true":"false");
		}
		operator Type () const { return _v; }
		private:
			Type _v;
	};

	DECL_SHARED_PTR(Objet);
	struct Objet : public Valeur
	{
		typedef std::map<std::string,ValeurSP> Membres;
		explicit Objet(const Membres& membres = {})
		:_membres(membres) {}
		virtual ~Objet() {}
		ValeurSP& operator[](const std::string& m)
		{
			return _membres[m];
		}
		void ajoute(const std::string& m, const ValeurSP& v)
		{
			_membres.insert(std::make_pair(m,v));
		}
		virtual void format(std::ostream& os) const override
		{
			os << "{" ;
			bool debut = true;
			for(const auto& m:_membres)
			{
				if(debut)
				{
					debut = false;
				}
				else
				{
					os << ",";
				}
				os << " \"" << m.first << "\": ";
				m.second->format(os);
			}
			os << " }";
		}
		private:
			Membres _membres;
	};

	DECL_SHARED_PTR(Tableau);
	struct Tableau : public Valeur
	{
		typedef std::vector<ValeurSP> Membres;
		explicit Tableau(const Membres& membres = {})
		:_membres(membres) {}
		virtual ~Tableau() {}
		void ajoute(const ValeurSP& v)
		{
			_membres.push_back(v);
		}
		virtual void format(std::ostream& os) const override
		{
			os << "[" ;
			bool debut = true;
			for(const auto& m:_membres)
			{
				if(debut)
				{
					debut = false;
				}
				else
				{
					os << ",";
				}
				os << " ";
				m->format(os);
			}
			os << " ]";
		}
		Membres::const_iterator begin() const { return _membres.cbegin(); }
		Membres::const_iterator end() const { return _membres.cend(); }
		size_t taille() const { return _membres.size(); }
		private:
			Membres _membres;
	};
}
	struct JSON
	{
		static std::string serialise(const JSON_Interne::ValeurSP& source)
		{
			if(not source) return std::string();
			std::ostringstream oss;
			oss << (*source);
			return oss.str();
		}
		static JSON_Interne::ValeurSP deserialise(const std::string& source);
		static JSON_Interne::EntierSP entier(const JSON_Interne::Entier::Type v) { return std::make_shared<JSON_Interne::Entier>(v); }
		static JSON_Interne::ChaineSP chaine(const JSON_Interne::Chaine::Type& v) { return std::make_shared<JSON_Interne::Chaine>(v); }
		static JSON_Interne::BooleenSP booleen(const JSON_Interne::Booleen::Type v) { return std::make_shared<JSON_Interne::Booleen>(v); }
		static JSON_Interne::ObjetSP objet(const JSON_Interne::Objet::Membres& v) { return std::make_shared<JSON_Interne::Objet>(v); }
		static JSON_Interne::ObjetSP objet() { return std::make_shared<JSON_Interne::Objet>(); }
		static JSON_Interne::TableauSP tableau(const JSON_Interne::Tableau::Membres& v) { return std::make_shared<JSON_Interne::Tableau>(v); }
		static JSON_Interne::TableauSP tableau() { return std::make_shared<JSON_Interne::Tableau>(); }

		static JSON_Interne::EntierSP entier(const JSON_Interne::ValeurSP& v) { return std::dynamic_pointer_cast<JSON_Interne::Entier>(v); }
		static JSON_Interne::ChaineSP chaine(const JSON_Interne::ValeurSP& v) { return std::dynamic_pointer_cast<JSON_Interne::Chaine>(v); }
		static JSON_Interne::BooleenSP booleen(const JSON_Interne::ValeurSP& v) { return std::dynamic_pointer_cast<JSON_Interne::Booleen>(v); }
		static JSON_Interne::ObjetSP objet(const JSON_Interne::ValeurSP& v) { return std::dynamic_pointer_cast<JSON_Interne::Objet>(v); }
		static JSON_Interne::TableauSP tableau(const JSON_Interne::ValeurSP& v) { return std::dynamic_pointer_cast<JSON_Interne::Tableau>(v); }

		template<typename VSP>
		static bool est(const VSP& v) { return static_cast<bool>(v); }
	};
}
}
}
