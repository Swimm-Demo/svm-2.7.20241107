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
#include <src/machine/debugueur/chiffre.h>

namespace SVM
{
namespace Machine
{
namespace Debugueur
{

struct MemoireDebugueur
{
	MemoireDebugueur()
	{
		res0 = nullptr;
	}

	~MemoireDebugueur()
	{
		if(res0)
			::freeaddrinfo(res0);
	}
	struct addrinfo *res0;
};

#define TAILLE_DONNEES_FLUX 4096

struct TCP
{
	static std::string strerror_local(int e)
	{
		return ::strerror(e);
	}

	static std::string gai_strerror_local(int e)
	{
		return ::gai_strerror(e);
	}

	static bool resolution_ip_port(const struct sockaddr* sock, const socklen_t longueur, std::string& ip, std::string& port)
	{
		char host[TAILLE_DONNEES_FLUX+1];
		char serv[TAILLE_DONNEES_FLUX+1];
		bool resultat=::getnameinfo(sock,longueur,host,TAILLE_DONNEES_FLUX,serv,TAILLE_DONNEES_FLUX,NI_NUMERICHOST)==0;
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
		os << ": " << TCP::strerror_local(erreur);
	}

	template<bool appelle_bind, bool appelle_listen, bool appelle_connect>
		static int ouvrir(std::ostringstream& os, const int type, const std::string& ip, const std::string& port)
		{
			int fd;
			struct addrinfo hints, *res;
			int error;
			MemoireDebugueur memoire;
			os << "Unable to open TCP socket.";

			::memset(&hints, 0, sizeof(hints));
			hints.ai_family = PF_UNSPEC;
			hints.ai_socktype = type;
			error = ::getaddrinfo(ip.c_str(), port.c_str(), &hints, &memoire.res0);
			if (error)
			{
				os << std::endl << "getaddrinfo: " << TCP::gai_strerror_local(error) << std::endl;
				return -1;
			}
			fd = -1;
			for (res = memoire.res0 ; res ; res = res->ai_next)
			{
				fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
				if (fd < 0)
				{
					TCP::log_erreur(res,os,"socket",errno);
					continue;
				}

				if(appelle_bind)
				{
					if (::bind(fd, res->ai_addr, res->ai_addrlen) < 0)
					{
						TCP::log_erreur(res,os,"bind",errno);
						::close(fd);
						fd = -1;
						continue;
					}
				}

				if(appelle_listen)
				{
					if(::listen(fd,20)<0)
					{
						TCP::log_erreur(res,os,"listen",errno);
						::close(fd);
						fd = -1;
						continue;
					}
				}

				if(appelle_connect)
				{
					if (::connect(fd, res->ai_addr, res->ai_addrlen) < 0)
					{
						TCP::log_erreur(res,os,"connect",errno);
						::close(fd);
						fd = -1;
						continue;
					}
				}
				break;
			}
			if (fd < 0)
			{
				os << std::endl;
				return -1;
			}
			return fd;
		}
};

struct HTTP
{
	HTTP()
	:_valide(false) {}
	static bool requete_http_complete(const std::string& tampon, const bool fin, size_t& taille, bool& evacue)
	{
		taille=0;
		evacue=false;
		size_t debut = std::string::npos;
		size_t indice = tampon.find("GET"); if(indice<debut) debut=indice;
		indice = tampon.find("HEAD"); if(indice<debut) debut=indice;
		indice = tampon.find("POST"); if(indice<debut) debut=indice;
		indice = tampon.find("PUT"); if(indice<debut) debut=indice;
		indice = tampon.find("DELETE"); if(indice<debut) debut=indice;
		indice = tampon.find("CONNECT"); if(indice<debut) debut=indice;
		indice = tampon.find("OPTIONS"); if(indice<debut) debut=indice;
		indice = tampon.find("TRACE"); if(indice<debut) debut=indice;
		indice = tampon.find("PATCH"); if(indice<debut) debut=indice;
		if(debut==std::string::npos) return false;
		if(debut>0)
		{
			taille = debut;
			evacue = true;
			return false;
		}
		size_t t = 4;
		indice = tampon.find("\r\n\r\n");
		if(indice==std::string::npos) 
		{
			indice = tampon.find("\n\n");
			t=2;
		}
		if(indice!=std::string::npos)
		{
			size_t indice_taille = tampon.substr(0,indice).find("Content-Length: ");
			if(indice_taille==std::string::npos)
			{
				taille = indice+t;
				if(fin and (tampon.size()<taille))
				{
					taille = tampon.size();
					evacue = true;
					return false;
				}
				return tampon.size()>=taille;
			}
			size_t tt = tampon.find("\n",indice_taille+16);
			long int ttaille = ::atoi(tampon.substr(indice_taille+16,tt-(indice_taille-16)).c_str());
			if(ttaille>0)
			{
				taille = indice+t+ttaille;
				if(fin and (tampon.size()<taille))
				{
					taille = tampon.size();
					evacue = true;
					return false;
				}
				return tampon.size()>=taille;
			}
		}
		return false;
	}
	static std::vector<std::string> decoupage(const std::string& chaine, const std::string& separateur)
	{
		std::vector<std::string> v;
		std::string c = chaine;
		size_t p;
		while((p=c.find(separateur))!=std::string::npos)
		{
			v.push_back(c.substr(0,p));
			c = c.substr(p+separateur.size());
		}
		v.push_back(c);
		return v;
	}
	static void remplace_variable_html(std::string& contenu, const std::string& variable, const std::string& valeur)
	{
		std::ostringstream oss;
		size_t p;
		while((p=contenu.find(variable))!=std::string::npos)
		{
			oss << contenu.substr(0,p) << valeur;
			contenu = contenu.substr(p+variable.size());
		}
		oss << contenu;
		contenu = oss.str();
	}
	static HTTP analyse_requete_http(const std::string& requete)
	{
		HTTP resultat;
		auto lignes = HTTP::decoupage(requete,"\r\n");
		if(lignes.size()<2) return resultat;
		auto premiere_ligne = HTTP::decoupage(lignes[0]," ");
		if(premiere_ligne.size()<3) return resultat;
		resultat._methode = premiere_ligne[0];
		if(not ((resultat._methode=="GET") or (resultat._methode=="POST"))) return resultat;
		std::string ressource = premiere_ligne[1];
		auto uri = HTTP::decoupage(ressource,"?");
		resultat._ressource = uri[0];
		if(uri.size()>1)
		{
			auto parametres = HTTP::decoupage(uri[1],"&");
			for(const auto& p: parametres)
			{
				auto pp = HTTP::decoupage(p,"=");
				if(pp.size()!=2) return resultat;
				resultat._parametres[pp[0]]=pp[1];
			}
		}
		size_t e;
		for(e=1 ; e<lignes.size() ; ++e)
		{
			if(lignes[e]=="") break;
			auto ee = HTTP::decoupage(lignes[e],": ");
			if(ee.size()!=2) return resultat;
			if(ee[0]=="Cookie")
			{
				auto cookies = HTTP::decoupage(ee[1],"; ");
				for(const auto& c:cookies)
				{
					auto cc = HTTP::decoupage(c,"=");
					if(cc.size()!=2) return resultat;
					resultat._cookies[cc[0]]=cc[1];
				}
			}
			else
			{
				resultat._entetes[ee[0]]=ee[1];
			}
		}
		if(e==lignes.size()) return resultat;
		for( ++e ; e<lignes.size() ; ++e)
		{
			resultat._contenu += lignes[e];
			if(e!=(lignes.size()-1))
			{
				resultat._contenu += "\r\n";
			}
		}
		resultat._valide = true;
		return resultat;
	}
	static std::string lecture_fichier(const std::string& ressource)
	{
		SVM_TRACE("ressource " << ressource);
		std::string nom(std::string(DEBUGGER_SITE)+ressource);
		std::stringstream ss;
		char tampon[1025];
		int fichier = ::open(nom.c_str(),O_RDONLY);
		if(fichier<0)
		{
			return std::string();
		}
		for( ; ; )
		{
			int l = ::read(fichier,tampon,1024);
			if(l==0) break;
			ss << std::string(tampon,l);
		}
		::close(fichier);
		SVM_TRACE("lecture fichier " << ressource << ": " << ss.str().size() << " octets");
		return ss.str();
	}
	static std::string type_contenu(const std::string& ressource)
	{
		auto elements = HTTP::decoupage(ressource,".");
		static std::map<std::string,std::string> correspondances = { { "html" , "text/html" } , { "css" , "text/css" } , { "js" , "text/javascript" } , { "ico" , "image/vnd.microsoft.icon" } };
		auto it = correspondances.find(elements.back());
		if(it==correspondances.end())
		{
			return "text/plain";
		}
		return it->second;
	}
	static std::string reponse_101(const std::string& contenu, const std::map<std::string, std::string>& entetes, const std::string& type = "text/plain")
	{
		std::ostringstream oss;
		oss << "HTTP/1.1 101 Switching Protocols\r\n";
		for(const auto& e: entetes)
		{
			oss << e.first << ": " << e.second << "\r\n";
		}
		oss << "Content-Type: " << type << "\r\nContent-Length: " << contenu.size() << "\r\n\r\n" << contenu;
		return oss.str();
	}
	static std::string reponse_200(const std::string& contenu, const std::string& type)
	{
		std::ostringstream oss;
		oss << "HTTP/1.1 200 OK\r\nContent-Type: " << type << "\r\nContent-Length: " << contenu.size() << "\r\n\r\n" << contenu;
		return oss.str();
	}
	static std::string reponse_400(const std::string& contenu, const std::string& type = "text/plain")
	{
		std::ostringstream oss;
		oss << "HTTP/1.1 400 Bad Request\r\nContent-Type: " << type << "\r\nContent-Length: " << contenu.size() << "\r\n\r\n" << contenu;
		return oss.str();
	}
	static std::string reponse_401(const std::string& contenu, const std::string& type = "text/plain")
	{
		std::ostringstream oss;
		oss << "HTTP/1.1 401 Not Authorized\r\nContent-Type: " << type << "\r\nContent-Length: " << contenu.size() << "\r\n\r\n" << contenu;
		return oss.str();
	}
	static std::string reponse_404(const std::string& contenu, const std::string& type = "text/plain")
	{
		std::ostringstream oss;
		oss << "HTTP/1.1 404 Not Found\r\nContent-Type: " << type << "\r\nContent-Length: " << contenu.size() << "\r\n\r\n" << contenu;
		return oss.str();
	}
	operator bool () const { return _valide; }
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const HTTP& h)
	{
		f	<< "HTTP " << (h._valide?"":"in") << "valide" << std::endl
			<< "Methode: " << h._methode << std::endl
			<< "Ressource: " << h._ressource << std::endl;
		for(const auto& p: h._parametres)
		{
			f << "  Parametre: " << p.first << " => " << p.second << std::endl;
		}
		for(const auto& e: h._entetes)
		{
			f << "Entete: " << e.first << " => " << e.second << std::endl;
		}
		for(const auto& c: h._cookies)
		{
			f << "  Cookie: " << c.first << " => " << c.second << std::endl;
		}
		f	<< "Contenu: " << h._contenu << std::endl;
		return f;
	}
	bool _valide;
	std::string _methode;
	std::string _ressource;
	std::map<std::string, std::string> _parametres;
	std::map<std::string, std::string> _entetes;
	std::map<std::string, std::string> _cookies;
	std::string _contenu;
};

struct WS
{
	typedef enum {
		CONTINUATION = 0,
		TEXTE = 1,
		BINAIRE = 2,
		FIN = 8,
		PING = 9,
		PONG = 10
		} OpCode;
	WS()
	:_valide(false), _fin(false), _reserve1(false), _reserve2(false), _reserve3(false), _opcode(0) {}
	WS(const bool fin, const char op_code)
	:_valide(true), _fin(fin), _reserve1(false), _reserve2(false), _reserve3(false), _opcode(op_code) {}
	static bool requete_ws_complete(const std::string& tampon, const bool fin, uint64_t& taille, bool& evacue)
	{
		evacue = false;
		if(tampon.size()<2) return false;
		taille=static_cast<uint64_t>(static_cast<unsigned char>(tampon[1])) bitand 0x7F;
		if(taille==126)
		{
			if(tampon.size()<4) return false;
			taille = 4 + (static_cast<uint64_t>(static_cast<unsigned char>(tampon[2])) << 8) + (static_cast<uint64_t>(static_cast<unsigned char>(tampon[3])));
		}
		else if(taille==127)
		{
			if(tampon.size()<10) return false;
			taille = 10 + (static_cast<uint64_t>(static_cast<unsigned char>(tampon[2])) << 56) + (static_cast<uint64_t>(static_cast<unsigned char>(tampon[3])) << 48) + (static_cast<uint64_t>(static_cast<unsigned char>(tampon[4])) << 40) + (static_cast<uint64_t>(static_cast<unsigned char>(tampon[5])) << 32) + (static_cast<uint64_t>(static_cast<unsigned char>(tampon[6])) << 24) + (static_cast<uint64_t>(static_cast<unsigned char>(tampon[7])) << 16) + (static_cast<uint64_t>(static_cast<unsigned char>(tampon[8])) << 8) + static_cast<uint64_t>(static_cast<unsigned char>(tampon[9]));
		}
		else
		{
			taille += 2;
		}
		bool masque = (static_cast<size_t>(static_cast<unsigned char>(tampon[1])) & 0x80) >0;
		if(masque)
		{
			taille += 4;
		}
		if(fin and tampon.size()<taille)
		{
			evacue = true;
			taille = tampon.size();
			return false;
		}
		return tampon.size()>=taille;
	}
	static WS decode_ws(const std::string& requete)
	{
		if(requete.size()<2) return WS();
		WS ws;
		ws._fin = (static_cast<unsigned char>(requete[0]) & 0x80)>0;
		ws._reserve1 = (static_cast<unsigned char>(requete[0]) & 0x40) > 0;
		ws._reserve2 = (static_cast<unsigned char>(requete[0]) & 0x20) > 0;
		ws._reserve3 = (static_cast<unsigned char>(requete[0]) & 0x10) > 0;
		ws._opcode = (static_cast<unsigned char>(requete[0]) & 0x0F);
		bool masque = (static_cast<unsigned char>(requete[1]) & 0x80) > 0;
		uint64_t taille = (static_cast<unsigned char>(requete[1]) & 0x7F);
		size_t entetes = 2;
		if(taille==126)
		{
			if(requete.size()<4)
			{
				return WS();
			}
			entetes = 4;
			taille = (static_cast<uint64_t>(static_cast<unsigned char>(requete[2])) << 8) + static_cast<uint64_t>(static_cast<unsigned char>(requete[3]));
		
		}
		else if(taille==127)
		{
			if(requete.size()<10)
			{
				return WS();
			}
			entetes = 10;
			taille = (static_cast<uint64_t>(static_cast<unsigned char>(requete[2])) << 56) + (static_cast<uint64_t>(static_cast<unsigned char>(requete[3])) << 48) + (static_cast<uint64_t>(static_cast<unsigned char>(requete[4])) << 40) + (static_cast<uint64_t>(static_cast<unsigned char>(requete[5])) << 32) + (static_cast<uint64_t>(static_cast<unsigned char>(requete[6])) << 24) + (static_cast<uint64_t>(static_cast<unsigned char>(requete[7])) << 16) + (static_cast<uint64_t>(static_cast<unsigned char>(requete[8])) << 8) + static_cast<uint64_t>(static_cast<unsigned char>(requete[9]));
		}
		if(masque)
		{
			if(requete.size()<entetes+4)
			{
				return WS();
			}
			ws._masque = requete.substr(entetes,4);	
			entetes += 4;
		}
		ws._message = requete.substr(entetes);
		if(ws._message.size()!=taille)
		{
			return WS();
		}
		if(masque)
		{
			for(size_t i=0 ; i<ws._message.length() ; ++i)
			{
				ws._message[i] ^= ws._masque[i%4];
			}
		}
		ws._valide = true;
		return ws;
	}
	static std::string encode_ws(const WS& ws)
	{
		std::string tampon;
		unsigned char premier = (ws._fin ? (1 << 7) : 0) bitor (ws._reserve1 ? (1 << 6) : 0) bitor (ws._reserve2 ? (1 << 5) : 0)  bitor (ws._reserve3 ? (1 << 4) : 0) bitor (ws._opcode & 0x0F);
		tampon += premier;
		unsigned char second = (ws._masque.empty() ? 0 : (1 << 7));
		uint64_t taille = ws._message.size();
		if(taille<=125)
		{
			second |= (taille & 0x7F);
			tampon += second;
		}
		else if(taille<=65536)
		{
			second |= 126;
			tampon += second;
			tampon += (unsigned char)(taille>>8);
			tampon += (unsigned char)(taille & 0xFF);
		}
		else
		{
			second |= 127;
			tampon += second;
			tampon += (unsigned char)(taille>>56);
			tampon += (unsigned char)(taille>>48 & 0xFF);
			tampon += (unsigned char)(taille>>40 & 0xFF);
			tampon += (unsigned char)(taille>>32 & 0xFF);
			tampon += (unsigned char)(taille>>24 & 0xFF);
			tampon += (unsigned char)(taille>>16 & 0xFF);
			tampon += (unsigned char)(taille>>8 & 0xFF);
			tampon += (unsigned char)(taille & 0xFF);
		}
		std::string message = ws._message;
		if(not ws._masque.empty())
		{
			if(ws._masque.size()!=4)
			{
				return std::string();
			}
			tampon += ws._masque;
			std::string clef(ws._masque);
			for(size_t i=0 ; i<message.length() ; ++i)
			{
				message[i] ^= clef[i%4];
			}
		}
		tampon += message;
		//SVM_TRACE("WS encodee : " << Securite::encode_hex(tampon));
		return tampon;
	}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const WS& w)
	{
		f	<< "WS " << (w._valide?"":"in") << "valide" << std::endl
			<< "Fin: " << w._fin << std::endl
			<< "Reserve1: " << w._reserve1 << std::endl
			<< "Reserve2: " << w._reserve2 << std::endl
			<< "Reserve3: " << w._reserve3 << std::endl
			<< "OpCode: " << Securite::encode_hex(std::string(1,w._opcode)) << " (";
		switch(w._opcode)
		{
			case CONTINUATION:
				f << "CONTINUATION";
				break;
			case TEXTE:
				f << "TEXTE";
				break;
			case BINAIRE:
				f << "BINAIRE";
				break;
			case FIN:
				f << "FIN";
				break;
			case PING:
				f << "PING";
				break;
			case PONG:
				f << "PONG";
				break;
			default:
				f << "invalid";
				break;
		}
		f	<< ")" << std::endl
			<< "Taille: " << w._message.size() << std::endl
			<< "Masque: " << Securite::encode_hex(w._masque) << std::endl
			<< "Contenu: " << ((w._opcode==TEXTE)?w._message:Securite::encode_hex(w._message)) << std::endl;
		return f;
	}
	operator bool () const { return _valide; }
	bool _valide;
	bool _fin;
	bool _reserve1;
	bool _reserve2;
	bool _reserve3;
	unsigned char _opcode;
	std::string _masque;
	std::string _message;
};

}
}
}
