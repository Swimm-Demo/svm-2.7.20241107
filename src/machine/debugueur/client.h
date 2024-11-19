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
#include <src/machine/debugueur/protocoles.h>

namespace SVM
{
namespace Machine
{
namespace Debugueur
{
struct Debugueur;

DECL_SHARED_PTR(Client);

struct Client
{
	friend struct Debugueur;
	enum class Protocole {HTTP, WS};
	enum class Etat {OUVERTURE,AUTHENTIFICATION,SESSION};
	Client(const int fd, const std::string& ip, const std::string& port)
	:_fd(fd), _ip(ip),_port(port), _protocole(Protocole::HTTP), _etat(Etat::OUVERTURE), _rafraichit_toujours(true)
	{
		relance_chiffrement();
	}
	~Client()
	{
		::shutdown(_fd,SHUT_RDWR);
		::close(_fd);
	}
	void envoie(const std::string& message)
	{
		::send(_fd,message.c_str(),message.size(),0);
	}
	void envoie_ws(const std::string& message)
	{
		WS ws(true,WS::TEXTE);
		ws._message=message;
		SVM_TRACE("Envoi: " << ws);
		envoie(WS::encode_ws(ws));
	}
	void envoie_ws_chiffre(const std::string& message)
	{
		SVM_TRACE("Envoi chiffre: " << message);
		if(not _chiffre)
		{
			envoie_ws(message);
			return;
		}
		WS ws(true,WS::TEXTE);
		ws._message=Securite::encode_hex(_chiffre->chiffre(message));
		envoie(WS::encode_ws(ws));
	}
	void nouveau_chiffre(const Chiffre::Challenges& challenges)
	{
		_chiffre = std::make_shared<Chiffre>(_mot_de_passe,challenges);
	}
	void relance_chiffrement()
	{
		_chiffrement = 50 + ::rand() % 50;
	}
	private:
		int _fd;
		std::string _ip;
		std::string _port;
		Protocole _protocole;
		std::stringstream _tampon;
		std::string _mot_de_passe;
		std::string _challenge;
		Etat _etat;
		std::string _message;
		size_t _chiffrement;
		ChiffreSP _chiffre;
		std::set<size_t> _souscription_objet;
		std::set<std::string> _souscription_index;
		std::atomic_bool _rafraichit_toujours;
		// donnees session
};

}
}
}
