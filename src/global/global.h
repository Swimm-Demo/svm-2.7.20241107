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
#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <dlfcn.h>
#include <fcntl.h>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <netdb.h>
#include <random>
#include <regex>
#include <set>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <time.h>
#include <typeinfo>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#include <config.h>

#define ASSERT(exp, msg) assert(((void)msg, exp))

#define SHARED_PTR(Type) std::shared_ptr< Type >

#define DECL_SHARED_PTR(Type) \
struct Type;\
typedef SHARED_PTR(Type) Type##SP;\
typedef SHARED_PTR(const Type) Type##CSP


#define WEAK_PTR(Type) std::weak_ptr< Type >

#define DECL_WEAK_PTR(Type) \
struct Type;\
typedef WEAK_PTR(Type) Type##WP;\
typedef WEAK_PTR(const Type) Type##CWP

#define UNIQUE_PTR(Type) std::unique_ptr< Type >

#define DECL_UNIQUE_PTR(Type) \
struct Type;\
typedef UNIQUE_PTR(Type) Type##UP;\
typedef UNIQUE_PTR(const Type) Type##CUP

template<typename T>
std::ostream& operator<<(std::ostream& flux, SHARED_PTR(T) t)
{
	if(static_cast<bool>(t)) { flux << (*t); };
	return flux;
}

template<typename T>
std::ostream& operator<<(std::ostream& flux, WEAK_PTR(T) t)
{
	SHARED_PTR(T) tsp=t.lock();
	if(static_cast<bool>(tsp)) { flux << (*tsp); };
	return flux;
}

namespace SVM
{
namespace Global
{
	template<typename Type>
		struct SPinferieurSP
		{
			bool operator() (const SHARED_PTR(const Type)& gauche, const SHARED_PTR(const Type)& droite) const
			{
				return (*gauche)<(*droite);
			};
		};
	template<typename Type>
		struct SPinferieurornulSP
		{
			bool operator() (const SHARED_PTR(const Type)& gauche, const SHARED_PTR(const Type)& droite) const
			{
				if(gauche and not droite)
					return true;
				if(not gauche)
					return false;
				return (*gauche)<(*droite);
			};
		};
	template<typename Type>
		struct SPegalSP
		{
			bool operator() (const SHARED_PTR(const Type) gauche, const SHARED_PTR(const Type) droite) const
			{
				return (*gauche)==(*droite);
			};
		};
	struct Chaine
	{
		static std::string echappe(const std::string& chaine);
		static std::string desechappe(const std::string& chaine);
		static char *duplique(const char *chaine, const size_t taille);
		static std::string echappe_html(const std::string& chaine);
	};
	struct Fichier
	{
		static bool teste_fichier(std::string fichier);
		static FILE *ouvrir(std::string fichier);
	};
	struct Print
	{
		Print() {}
		~Print()
		{
			std::lock_guard<std::mutex> v(Print::print_mutex());
			std::cerr << _oss.str() << std::endl;
		}
		template<typename T>
			Print& operator<<(const T& t)
			{
				_oss << t;
				return *this;
			}
		template<typename T>
			Print& operator<<(const T&& t)
			{
				_oss << t;
				return *this;
			}
		template<typename T>
			Print& operator<<(T&& t)
			{
				_oss << t;
				return *this;
			}
		template<typename T>
			Print& operator<<(const SHARED_PTR(T)& t)
			{
				if(static_cast<bool>(t))
				{
					_oss << (*t);
				}
				return *this;
			}
		template<typename Flux>
		Print& operator<<(Flux& manip( Flux& f ))
		{
			_oss << manip;
			return *this;
		}
		std::ostringstream _oss;
		static std::mutex& print_mutex()
		{
			static std::mutex pm;
			return pm;
		}
	};

namespace Interne
{
	struct MemoireTCP
	{
		MemoireTCP()
		{
			res0 = nullptr;
		}

		~MemoireTCP()
		{
			if(res0)
				::freeaddrinfo(res0);
		}
		struct addrinfo *res0;
	};
}
	struct TCP
	{
		TCP(const bool serveur, const std::string& ip, const std::string& port, const int id)
			:_ip_distante(ip), _port_distant(port), _id(id), _serveur(serveur)
		{
			if(_serveur)
			{
				_ip_locale.swap(_ip_distante);
				_port_local.swap(_port_distant);
			}
		}
		std::string _ip_locale;
		std::string _port_local;
		std::string _ip_distante;
		std::string _port_distant;
		int _id;
		bool _serveur;

		static bool resolution_ip_port(const struct sockaddr* sock, const socklen_t longueur, std::string& ip, std::string& port)
		{
			char host[4096+1];
			char serv[4096+1];
			bool resultat=::getnameinfo(sock,longueur,host,4096,serv,4096,NI_NUMERICHOST)==0;
			if(resultat)
			{
				ip = host;
				port = serv;
			}
			return resultat;
		}

		static void log_erreur(struct addrinfo *res, std::ostringstream& os, const std::string& fonction, int erreur)
		{
			std::string ip;
			std::string port;
			os << std::endl << fonction << " ";
			if(TCP::resolution_ip_port(res->ai_addr,res->ai_addrlen,ip,port))
			{
				os << "(" << ip << " : " << port << ")";
			}
			os << ": " << ::strerror(erreur);
		}

		template<bool appelle_bind, bool appelle_listen, bool appelle_connect>
			static int ouvrir(const int type, const std::string& ip, const std::string& port, std::ostringstream& details)
			{
				int fd;
				struct addrinfo hints, *res;
				int error;
				Interne::MemoireTCP memoire;
				details << "Unable to open TCP socket.";

				::memset(&hints, 0, sizeof(hints));
				hints.ai_family = PF_UNSPEC;
				hints.ai_socktype = type;
				error = ::getaddrinfo(ip.c_str(), port.c_str(), &hints, &memoire.res0);
				if (error)
				{
					details << std::endl << "getaddrinfo: " << ::gai_strerror(error);
					return -1;
				}
				fd = -1;
				for (res = memoire.res0 ; res ; res = res->ai_next)
				{
					fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
					if (fd < 0)
					{
						TCP::log_erreur(res,details,"socket",errno);
						continue;
					}

					if(appelle_bind)
					{
						if (::bind(fd, res->ai_addr, res->ai_addrlen) < 0)
						{
							TCP::log_erreur(res,details,"bind",errno);
							::close(fd);
							fd = -1;
							continue;
						}
					}

					if(appelle_listen)
					{
						if(::listen(fd,20)<0)
						{
							TCP::log_erreur(res,details,"listen",errno);
							::close(fd);
							fd = -1;
							continue;
						}
					}

					if(appelle_connect)
					{
						if (::connect(fd, res->ai_addr, res->ai_addrlen) < 0)
						{
							TCP::log_erreur(res,details,"connect",errno);
							::close(fd);
							fd = -1;
							continue;
						}
					}
					break;
				}
				if (fd < 0)
				{
					details << std::endl;
				}
				return fd;
			}
		static void fermer(const int id, const int sens = SHUT_RDWR)
		{
			::shutdown(id,sens);
		}
	};

#ifdef NDEBUG
#	define SVM_TRACE(message)
#else
#	define SVM_TRACE(message) SVM::Global::Print() << "SVM trace : " << __FILE__ << ":" << __LINE__ << "> " << message 
#endif

}
}
