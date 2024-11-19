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

#define TAILLE_ENTETE 150
#define TAILLE_MARQUEUR_ENTETE 3

namespace SVM
{
namespace Machine
{
	DECL_SHARED_PTR(Historique);
	struct Trace;
	struct Historique
	{
		Historique()
		:_actif(false), _erreur(false) {}
		virtual ~Historique() {};
		virtual void trace(const std::string& trace) {};
		operator bool () const
		{
			return _actif;
		}
		protected:
			Historique(const bool erreur)
			:_actif(true), _erreur(erreur) {}
			bool _actif;
			bool _erreur;
	};

	struct HistoriqueTerminal : public Historique
	{
		explicit HistoriqueTerminal(const bool erreur)
		:Historique(erreur) {}
		virtual void trace(const std::string& trace) override
		{
			std::cerr << trace;
		}
	};

	struct HistoriqueFichier : public Historique
	{
		HistoriqueFichier(const std::string& fichier, const bool erreur)
		:Historique(erreur),_fichier(fichier,std::ios_base::app)
		{
			if(_fichier.fail())
			{
				if(_erreur)
				{
					SVM::Global::Print() << "Unable to write traces to " << fichier << ", switching to silent mode";
				}
				_actif = false;
			}
		}
		virtual void trace(const std::string& trace) override
		{
			_fichier << trace;
		}
		private:
			std::ofstream _fichier;
	};

	struct HistoriqueServeur : public Historique
	{
		HistoriqueServeur(const std::string& ip, const std::string& port, const bool erreur)
		:Historique(erreur),_ip(ip),_port(port), _id(-1)
		{
			std::ostringstream oss;
			_id = SVM::Global::TCP::ouvrir<false,false,true>(SOCK_STREAM,_ip,_port,oss);
			if((_id<0) and _erreur)
			{
				SVM::Global::Print() << "Unable to write traces to " << _ip << ":" << _port << ", switching to silent mode: " << oss.str();
			}
		}
		virtual ~HistoriqueServeur()
		{
			if(_id>=0)
			{
				SVM::Global::TCP::fermer(_id,SHUT_RDWR);
			}
		}
		virtual void trace(const std::string& trace) override
		{
			if(_id<0)
			{
				std::ostringstream oss;
				_id = SVM::Global::TCP::ouvrir<false,false,true>(SOCK_STREAM,_ip,_port,oss);
				if(_id<0)
				{
					if(_erreur)
					{
						SVM::Global::Print() << "Unable to write traces to " << _ip << ":" << _port << ", switching to silent mode: " << oss.str();
					}
					return;
				}
			}
			auto r = ::send(_id,trace.c_str(),trace.size(),MSG_NOSIGNAL);
			if(r<0)
			{
				if(_erreur)
				{
					SVM::Global::Print() << "Unable to write traces to " << _ip << ":" << _port << ", switching to silent mode: Connection lost.";
				}
				SVM::Global::TCP::fermer(_id,SHUT_RDWR);
				_id = -1;
			}
		}
		private:
			std::string _ip;
			std::string _port;
			int _id;
	};

	struct Trace
	{
		Trace(HistoriqueSP& historique, const std::string& nom, const bool brut=false)
		:_historique(historique),_nom(nom),_brut(brut)
		{
			if(_brut) return;
			time_t t;
			::time(&t);
			char date[1024];
			::strftime(date,1023,"%Y-%m-%d %H:%M:%S %Z",::gmtime(&t));
			_oss << std::string(TAILLE_MARQUEUR_ENTETE,'#') << " Simple Virtual Machine " << ::getpid() << " : ";
			if(not _nom.empty())
			{
				_oss << _nom << " ";
			}
			_oss << "| " << date << " ";
			size_t s = _oss.str().size();
			if(s>(TAILLE_ENTETE-TAILLE_MARQUEUR_ENTETE))
			{
				s=TAILLE_MARQUEUR_ENTETE;
			}
			else
			{
				s=TAILLE_ENTETE-s;
			}
			_oss << std::string(s,'#') << std::endl;
		}
		~Trace()
		{
			if(not (*_historique)) return;
			std::lock_guard<std::mutex> verrou(Trace::trace_mutex());
			_oss << std::endl;
			if(not _brut)
			{
				_oss << std::endl;
			}
			_historique->trace(_oss.str());
		}
		template<typename T>
		std::ostringstream& operator<<(const T& t)
		{
			_oss << t;
			return _oss;
		}
		template<typename T>
		std::ostringstream& operator<<(const T&& t)
		{
			_oss << t;
			return _oss;
		}
		template<typename T>
		std::ostringstream& operator<<(const SHARED_PTR(T)& t)
		{
			if(static_cast<bool>(t))
			{
				_oss << *t;
			}
			return _oss;
		}
		static std::mutex& trace_mutex()
		{
			static std::mutex pm;
			return pm;
		}
		private:
		std::ostringstream _oss;
		HistoriqueSP _historique;
		std::string _nom;
		bool _brut;
	};

}
}
