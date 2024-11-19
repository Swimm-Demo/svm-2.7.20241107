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

#include <src/global/systeme.h>
#include <src/machine/debugueur/debugueur.h>
#include <src/machine/debugueur/securite.h>
#include <src/machine/debugueur/protocoles.h>
#include <debug/styles/styles.h>

using namespace SVM::Machine::Debugueur;

Debugueur::Debugueur(const std::string& titre, const std::string& ip, const std::string& port, const std::string& style, const size_t nombre_clients, const size_t niveau_securite, const SVM::Machine::HistoriqueSP& historique)
:_titre(titre), _ip(ip), _port(port), _style(style), _nombre_clients(nombre_clients), _niveau_securite(niveau_securite), _historique(historique), _livraison(std::make_shared<SVM::Machine::Debugueur::Livraison>()), _fd_fin(-1), _fd_declenchement_fin(-1), _fd_serveur(-1)
{
	if(_nombre_clients<2) _nombre_clients=2;
	if(_niveau_securite>255) _niveau_securite=255;
	_styles_html = SVM::Debug::Styles::Styles::html(_style);
}

DebugueurSP Debugueur::creation(const SVM::Machine::DebugSP& debug)
{
	if(not debug)
	{
		return DebugueurSP();
	}
	DebugueurSP debugueur = std::make_shared<Debugueur>(debug->_titre, debug->_ip, debug->_port, debug->_style, debug->_clients, debug->_securite, debug->_historique);
	debugueur->_adresse = SVM::Machine::Debugueur::Livraison::adresse(debugueur->shared_from_this());
	debugueur->_livraison->creation_boite(debugueur->_adresse);
	SVM_TRACE("Debugueur " << (*debugueur));
	debugueur->_points_arret = Debugueur::ajoute_piege<PiegePointsArret>(debugueur);
	std::ostringstream os;
	debugueur->_fd_serveur = TCP::ouvrir<true,true,false>(os,SOCK_STREAM,debugueur->_ip,debugueur->_port);
	if(debugueur->_fd_serveur<0)
	{
		SVM::Machine::Trace(debug->_historique,"debug") << os.str() << "Abort Simple Virtual Machine boot!";
		throw PasDeDebuggueur();
	}
	debugueur->creation_effecteurs();
	return debugueur;
}

void Debugueur::arret(DebugueurSP& debugueur)
{
	if(not debugueur) return;
	SVM_TRACE("Arret Debugueur debut");
	debugueur->_points_arret.reset();
	if(debugueur->_fd_serveur>=0)
	{
		SVM_TRACE("Arret Debugueur relache");
		::shutdown(debugueur->_fd_fin,SHUT_RDWR);
		debugueur->_lecteur.join();
		auto evenement = std::make_shared<SVM::Machine::Debugueur::Evenement>();
		debugueur->_livraison->poste(debugueur->_adresse,debugueur->_adresse,evenement);
		debugueur->_ecrivain.join();
		debugueur->_livraison->destruction_boite(debugueur->_adresse);
		::close(debugueur->_fd_fin);
		::close(debugueur->_fd_serveur);
	}
	SVM_TRACE("Arret Debugueur fin");
}

void Debugueur::creation_effecteurs()
{
	std::ostringstream os;
	_fd_fin = TCP::ouvrir<false,false,true>(os,SOCK_STREAM,"localhost",_port);
	if(_fd_fin<0)
	{
		SVM::Machine::Trace(_historique,"debug") << os.str() << "Abort Simple Virtual Machine boot!";
		throw PasDeDebuggueur();
	}
	struct sockaddr sock_client;
	socklen_t longueur_client = sizeof(sock_client);
	SVM_TRACE("accept - avant");
	_fd_declenchement_fin = ::accept(_fd_serveur,&sock_client,&longueur_client);
	linger linger = { 1, 0 };
	::setsockopt(_fd_declenchement_fin,SOL_SOCKET,SO_LINGER,&linger,sizeof(linger));
	SVM_TRACE("accept - apres");
	SVM_TRACE("ouverture declenchement fin : " << _fd_declenchement_fin);
	_lecteur = std::thread([this] ()
		{
			for( ; ; )
			{
				fd_set lecture;
				fd_set ecriture;
				fd_set exception;
				FD_ZERO(&lecture);
				FD_ZERO(&ecriture);
				FD_ZERO(&exception);
				FD_SET(this->_fd_serveur,&lecture);
				FD_SET(this->_fd_declenchement_fin,&lecture);
				int max = this->_fd_serveur;
				if(this->_fd_declenchement_fin>max)
				{
					max=this->_fd_declenchement_fin;
				}
				for(const auto& c: this->clients())
				{
					SVM_TRACE("ajoute client pour select " << c->_fd);
					FD_SET(c->_fd,&lecture);
					if(c->_fd>max)
					{
						max=c->_fd;
					}
				}

				++max;
				SVM_TRACE("select - avant");
				int resultat = ::select(max,&lecture,&ecriture,&exception,NULL);
				SVM_TRACE("select - apres");
				SVM_TRACE("resultat=" << resultat);
				if(resultat<0)
				{
					continue;
				}
				if(FD_ISSET(this->_fd_declenchement_fin,&lecture))
				{
					char tampon[2] = "\0";
					SVM_TRACE("recv fin - avant");
					::recv(this->_fd_fin,tampon,1,0);
					SVM_TRACE("recv fin - apres");
					SVM_TRACE("fin demandee : " << this->_fd_fin << " >>> " << std::string(tampon));
					return;
				}
				if(FD_ISSET(this->_fd_serveur,&lecture))
				{
					struct sockaddr sock_client;
					socklen_t longueur_client = sizeof(sock_client);
					SVM_TRACE("accept - avant");
					int fd_client = ::accept(this->_fd_serveur,&sock_client,&longueur_client);
					::linger linger = { 1, 0 };
					::setsockopt(fd_client,SOL_SOCKET,SO_LINGER,&linger,sizeof(linger));
					SVM_TRACE("accept - apres");
					std::string ip_client;
					std::string port_client;
					TCP::resolution_ip_port(&sock_client,longueur_client,ip_client,port_client);
					if(this->_nombre_clients==0)
					{
						::shutdown(fd_client,SHUT_RDWR);
						SVM::Machine::Trace(_historique,"debug") << "Number of allowed clients reached. Aborting connection with client.";
					}
					--this->_nombre_clients;
					auto client = std::make_shared<Client>(fd_client,ip_client,port_client);
					SVM_TRACE("ouverture : " << client->_fd);
					std::lock_guard<std::mutex> verrou(_protection);
					this->_clients.push_back(client);
					continue;
				}
				decltype(this->_clients) clients;
				for(const auto& c: this->clients())
				{
					if(not FD_ISSET(c->_fd,&lecture))
					{
						clients.push_back(c);
						continue;
					}
					char tampon[1025];
					SVM_TRACE("recv - avant");
					int longueur = ::recv(c->_fd,tampon,1024,0);
					SVM_TRACE("recv - apres");
					if(longueur<0)
					{
						clients.push_back(c);
						continue;
					}
					if(longueur==0)
					{
						SVM_TRACE("fermeture : " << c->_fd);
						++this->_nombre_clients;
						continue;
					}
					clients.push_back(c);
					c->_tampon << std::string(tampon,longueur);
					SVM_TRACE("tampon : " << c->_fd << " >>> " << c->_tampon.str());
					switch(c->_protocole)
					{
						case Client::Protocole::HTTP:
							{
								SVM_TRACE("Client - protocole: HTTP");
								size_t taille;
								bool evacue;
								for( ; ; )
								{
									bool trouve = HTTP::requete_http_complete(c->_tampon.str(),longueur==0,taille,evacue);
									if(evacue)
									{
										c->_tampon.str(c->_tampon.str().substr(taille));
										continue;
									}
									if(trouve)
									{
										std::string requete = c->_tampon.str().substr(0,taille);
										c->_tampon.str(c->_tampon.str().substr(taille));
										this->traite_requete_http(c,requete);
									}
									break;
								}
							}
							break;
						case Client::Protocole::WS:
							{
								SVM_TRACE("Client - protocole: WS");
								uint64_t taille;
								bool evacue;
								for( ; ; )
								{
									bool trouve = WS::requete_ws_complete(c->_tampon.str(),longueur==0,taille,evacue);
									if(evacue)
									{
										c->_tampon.str(c->_tampon.str().substr(taille));
										continue;
									}
									if(trouve)
									{
										std::string requete = c->_tampon.str().substr(0,taille);
										c->_tampon.str(c->_tampon.str().substr(taille));
										this->traite_requete_ws(c,requete);
									}
									break;
								}
							}
							break;
					}
				}
				{
					std::lock_guard<std::mutex> verrou(_protection);
					this->_clients.swap(clients);
				}
			}
		});
	_ecrivain = std::thread([this] ()
		{
			for( ; ; )
			{
				SVM::Machine::Debugueur::EvenementSP evenement;
				SVM::Machine::Element::Synchronisation::AdresseSP source;
				bool resultat = this->_livraison->recupere(this->_adresse,source,evenement);
				if(not resultat) continue;
				if(evenement->_categorie==Evenement::Categorie::FIN) return;
				for(const auto& c: this->clients())
				{
					if(c->_protocole!=Client::Protocole::WS) continue;
					if(c->_rafraichit_toujours)
					{
						if(evenement->_identifiant>0)
						{
							if(c->_souscription_objet.find(evenement->_identifiant)==c->_souscription_objet.end())
							{
								continue;
							}
						}
						if(not evenement->_type.empty())
						{
							if(c->_souscription_index.find(evenement->_type)==c->_souscription_index.end())
							{
								continue;
							}
						}
						c->envoie_ws_chiffre(evenement->_texte);
					}
					else
					{
						switch(evenement->_categorie)
						{
							case Evenement::Categorie::POINT_ARRET:
							{
								envoie_tout(c);
								c->envoie_ws_chiffre(evenement->_texte);
								break;
							}
							case Evenement::Categorie::RESULTAT:
							{
								c->envoie_ws_chiffre(evenement->_texte);
							}
							default:
								break;
						}
					}
				}
			}
		});
#ifdef LINUX_EXTENSIONS
	SVM::Global::Systeme::nom_thread(_lecteur.native_handle(),"D in");
	SVM::Global::Systeme::nom_thread(_ecrivain.native_handle(),"D out");
#endif
}

void Debugueur::notification_changement_objet(const PiegeSP& piege) 
{
	auto e = std::make_shared<Evenement>(Evenement::Categorie::STATUS_OBJET,piege->_description,piege->identifiant());
	_livraison->poste(_adresse,piege->_adresse,e);
}

void Debugueur::notification_evenement_objet(const PiegeSP& piege, const Evenement::Categorie categorie, const JSON_Interne::ObjetSP& evenement)
{
	auto o = JSON::objet({ { "command" , JSON::chaine("event") } , { "identifier", JSON::entier(piege->identifiant()) } , { "event" , evenement } });
	auto e = std::make_shared<Evenement>(categorie,JSON::serialise(o),piege->identifiant());
	_livraison->poste(_adresse,piege->_adresse,e);
}

void Debugueur::notification_suppression_objet(const size_t& identifiant) 
{
	auto o = JSON::objet({ { "command" , JSON::chaine("delete") } , { "identifier", JSON::entier(identifiant) } });
	auto e = std::make_shared<Evenement>(Evenement::Categorie::STATUS_OBJET,JSON::serialise(o));
	_livraison->poste(_adresse,_adresse,e);
}

void Debugueur::notification_changement_index(const std::string& type)
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto t = JSON::tableau();
	for(const auto& p: _pieges)
	{
		auto pp = p.second.lock();
		if(not pp) continue;
		if(pp->type()==type)
		{
			t->ajoute(JSON::entier(pp->identifiant()));
		}
	}
	auto o = JSON::objet({ { "command" , JSON::chaine("index") } , { "type" , JSON::chaine(type) } , { "identifiers" , t } });
	auto texte = JSON::serialise(o);
	_index[type] = texte;
	auto e = std::make_shared<Evenement>(Evenement::Categorie::STATUS_INDEX,texte,type);
	_livraison->poste(_adresse,_adresse,e);
}

void Debugueur::notification_fin() 
{
	auto o = JSON::objet({ { "command" , JSON::chaine("end") } });
	auto e = std::make_shared<Evenement>(Evenement::Categorie::RESULTAT,JSON::serialise(o));
	_livraison->poste(_adresse,_adresse,e);
}

JSON_Interne::ObjetSP Debugueur::attente_commande(const PiegeSP& piege)
{
	SVM::Machine::Debugueur::EvenementSP evenement;
	SVM::Machine::Element::Synchronisation::AdresseSP source;
	_livraison->nettoie(piege->_adresse);
	bool resultat = _livraison->recupere(piege->_adresse,source,evenement);
	if(not resultat) return JSON_Interne::ObjetSP();
	return evenement->_objet;
}

void Debugueur::traite_requete_http(const ClientSP& client, const std::string& requete)
{
	HTTP http = HTTP::analyse_requete_http(requete);
	SVM_TRACE(http);
	if(not http) return;
	auto itentete = http._entetes.find("Upgrade");
	if((itentete!=http._entetes.end()) and (itentete->second=="websocket"))
	{
		SVM_TRACE("Ouverture WebSocket demandee " << client.get());
		itentete = http._entetes.find("Sec-WebSocket-Key");
		if(itentete==http._entetes.end())
		{
			client->envoie(HTTP::reponse_400(""));
			return;
		}
		SVM_TRACE("Ouverture WebSocket valide");
		std::string acceptation = Securite::encode_base64(Securite::hashe_sha1(itentete->second+"258EAFA5-E914-47DA-95CA-C5AB0DC85B11"));
		client->envoie(HTTP::reponse_101("",{ { "Upgrade" , "websocket" }, { "Connection" , "Upgrade" }, { "Sec-WebSocket-Accept" , acceptation } }));
		client->_protocole = Client::Protocole::WS;
		WS ws(true,WS::TEXTE);
		client->_mot_de_passe = Securite::encode_hex(Securite::genere_aleatoire(30));
		std::ostringstream oss;
		if(_affiche_avertissement)
		{
			oss << std::endl
			<< "Disclaimer: The debugger can disclose application code and data." << std::endl
			<< "            Transferring this login to the debugger user interface" << std::endl
			<< "            through a secured channel is under your responsibility."
			<< std::endl
			<< std::endl;
			_affiche_avertissement = false;
		}
		SVM::Machine::Trace(_historique,"debug") << oss.str() << "Connection from " << client->_ip << ":" << client->_port << ". Login: " << client->_mot_de_passe;
		client->_mot_de_passe = Securite::hashe_sha1(client->_mot_de_passe);
		client->_challenge = Securite::genere_aleatoire(10);
		auto valeur = JSON::objet({ { "command" , JSON::chaine("login") } , { "challenge" , JSON::chaine(Securite::encode_hex(client->_challenge)) } });
		ws._message = JSON::serialise(valeur);
		SVM_TRACE("Envoi: " << ws);
		client->envoie(WS::encode_ws(ws));
		client->_etat = Client::Etat::AUTHENTIFICATION;
		SVM_TRACE("Ouverture WebSocket faite");
		return;
	}
	if(http._ressource.empty() or (http._ressource[0]!='/') or (std::count(http._ressource.begin(),http._ressource.end(),'.')>1))
	{
		client->envoie(HTTP::reponse_404(""));
		return;
	}
	if(http._ressource=="/") http._ressource="/index.html";
	std::string contenu = HTTP::lecture_fichier(http._ressource);
	if(contenu.empty())
	{
		client->envoie(HTTP::reponse_404(""));
		return;
	}
	HTTP::remplace_variable_html(contenu,"$STYLE$",_styles_html);
	std::ostringstream oss_port;
	oss_port << _port;
	HTTP::remplace_variable_html(contenu,"$PORT$",oss_port.str());
	client->envoie(HTTP::reponse_200(contenu,HTTP::type_contenu(http._ressource)));
}

void Debugueur::traite_requete_ws(const ClientSP& client, const std::string& requete)
{
	WS ws = WS::decode_ws(requete);
	SVM_TRACE(ws);
	if(not ws)
	{
		SVM_TRACE("message non WS");
		return;
	}
	switch(ws._opcode)
	{
		case WS::FIN:
			{
				client->envoie(WS::encode_ws(WS(true,WS::FIN)));
				client->_protocole = Client::Protocole::HTTP;
				return;
			}
			break;
		case WS::PING:
			{
				WS trame(true,WS::PONG);
				trame._message = ws._message;
				client->envoie(WS::encode_ws(trame));
				return;
			}
			break;
		case WS::PONG:
			{
				return;
			}
			break;
		case WS::TEXTE:
		case WS::BINAIRE:
			{
				client->_message = ws._message;
			}
			break;
		case WS::CONTINUATION:
			{
				client->_message += ws._message;
			}
			break;
		default:
			{
				return;
			}
			break;
	}
	if(not ws._fin) return;
	std::string texte_message;
	if(not client->_chiffre)
	{
		texte_message = ws._message;
	}
	else
	{
		texte_message = client->_chiffre->dechiffre(Securite::decode_hex(ws._message));
		if(client->_chiffrement==0)
		{
			auto c = challenges();
			client->envoie_ws_chiffre(JSON::serialise(Debugueur::convertit_challenges(c)));
			client->nouveau_chiffre(c);
			client->relance_chiffrement();
		}
		else
		{
			--client->_chiffrement;
		}
	}
	SVM_TRACE("Recu clair : " << texte_message);
	auto message_brut = JSON::deserialise(texte_message);
	if(not message_brut)
	{
		SVM_TRACE("Message non JSON : " << texte_message);
		return;
	}
	auto message = JSON::objet(message_brut);
	if(not message) return;
	if(client->_etat==Client::Etat::AUTHENTIFICATION)
	{
		std::string reponse_attendue;
		std::string reponse_recue;
		auto valeur = JSON::chaine((*message)["command"]);
		if(not JSON::est(valeur)) goto authentification_echouee;
		if(std::string(*valeur)!="login") goto authentification_echouee;
		valeur = JSON::chaine((*message)["response"]);
		if(not JSON::est(valeur)) goto authentification_echouee;
		reponse_attendue = Securite::encode_hex(Securite::hashe_sha1(Securite::encode_hex(client->_mot_de_passe)+Securite::encode_hex(client->_challenge)));
		reponse_recue = (*valeur);
		SVM_TRACE("Reponse attendue: " << reponse_attendue);
		SVM_TRACE("Reponse recue   : " << reponse_recue);
		if(reponse_attendue==reponse_recue)
		{
			client->_etat = Client::Etat::SESSION;
			SVM_TRACE("Client authentifie");
			auto c = challenges();
			client->nouveau_chiffre(c);
			client->envoie_ws(JSON::serialise(Debugueur::convertit_challenges(c)));
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("title") } , { "title" , JSON::chaine(_titre) } })));
		}
		else
		{
authentification_echouee:
			WS ws(true,WS::FIN);
			SVM_TRACE("Envoi >> " << ws);
			client->envoie(WS::encode_ws(ws));
			SVM::Machine::Trace(_historique,"debug") << "Authentication failed. Closing connection with client.";
		}
		return;
	}
	//SVM_TRACE("Poste message >>> " << client->_message);
	//auto evenement = std::make_shared<SVM::Machine::Debugueur::Evenement>(client->_message);
	//_livraison->poste(_adresse,_adresse,evenement);
	auto valeur = JSON::chaine((*message)["command"]);
	if(not JSON::est(valeur)) return;
	std::string commande = (*valeur);
	if(commande == "index")
	{
		valeur = JSON::chaine((*message)["type"]);
		if(not JSON::est(valeur))
		{
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("type field missing") } })));
			return;
		}
		std::lock_guard<std::mutex> verrou(_protection);
		auto it = _index.find(*valeur);
		if(it==_index.end())
		{
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("type not found") } })));
			return;
		}
		client->envoie_ws_chiffre(it->second);
		client->_souscription_index.insert(*valeur);
	}
	else if(commande == "object")
	{
		auto identifiant = JSON::entier((*message)["identifier"]);
		if(not JSON::est(identifiant))
		{
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("identifier field missing") } })));
			return;
		}
		std::lock_guard<std::mutex> verrou(_protection);
		auto it=_pieges.find(*identifiant);
		if(it==_pieges.end())
		{
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("object not found") } })));
			return;
		}
		auto p = it->second.lock();
		if(not p)
		{
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("invalid object") } })));
			return;
		}
		client->envoie_ws_chiffre(p->description());
		client->_souscription_objet.insert(*identifiant);
	}
	else if(commande == "objects")
	{
		auto tableau_identifiants = JSON::tableau((*message)["identifiers"]);
		if(not JSON::est(tableau_identifiants))
		{
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("identifiers field missing") } })));
			return;
		}
		std::lock_guard<std::mutex> verrou(_protection);
		for(const auto& v: *tableau_identifiants)
		{
			auto i = JSON::entier(v);
			if(not JSON::est(i))
			{
				client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("invalid identifier") } })));
				continue;
			}
			auto it=_pieges.find(*i);
			if(it==_pieges.end())
			{
				client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("object not found") } })));
				continue;
			}
			auto p = it->second.lock();
			if(not p)
			{
				client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("invalid object") } })));
				return;
			}
			client->envoie_ws_chiffre(p->description());
			client->_souscription_objet.insert(*i);
		}
	}
	else if(commande == "event")
	{
		auto identifiant = JSON::entier((*message)["identifier"]);
		if(not JSON::est(identifiant))
		{
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("identifier field missing") } })));
			return;
		}
		PiegeSP p;
		{
			std::lock_guard<std::mutex> verrou(_protection);
			auto it=_pieges.find(*identifiant);
			if(it==_pieges.end())
			{
				client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("object not found") } })));
				return;
			}
			p = it->second.lock();
		}
		if(not p)
		{
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("invalid object") } })));
			return;
		}
		p->evenement(message);
	}
	else if(commande == "client")
	{
		auto toujours = JSON::chaine((*message)["always_refresh"]);
		if(JSON::est(toujours))
		{
			std::string t = *toujours;
			if(t=="toggle")
			{
				client->_rafraichit_toujours.store(not client->_rafraichit_toujours.load(),std::memory_order_relaxed);
				if(client->_rafraichit_toujours)
				{
					envoie_tout(client);
				}
			}
			else if(t!="get")
			{
				return;
			}
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("client") } , { "always_refresh" , JSON::booleen(client->_rafraichit_toujours) } })));
		}
	}
	else
	{
		auto identifiant = JSON::entier((*message)["identifier"]);
		if(not JSON::est(identifiant))
		{
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("identifier field missing") } })));
			return;
		}
		PiegeSP p;
		{
			std::lock_guard<std::mutex> verrou(_protection);
			auto it=_pieges.find(*identifiant);
			if(it==_pieges.end())
			{
				client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("object not found") } })));
				return;
			}
			p = it->second.lock();
		}
		if(not p)
		{
			client->envoie_ws_chiffre(JSON::serialise(JSON::objet({ { "command" , JSON::chaine("error") } , { "text" , JSON::chaine("invalid object") } })));
			return;
		}
		auto e = std::make_shared<Evenement>(Evenement::Categorie::COMMANDE,message);
		SVM_TRACE("Envoi commande au piege");
		_livraison->poste(p->_adresse,_adresse,e);
	}
}

void Debugueur::envoie_tout(const ClientSP& client) const
{
	std::lock_guard<std::mutex> verrou(_protection);
	for(const auto& o: client->_souscription_objet)
	{
		auto it = _pieges.find(o);
		if(it==_pieges.end()) continue;
		auto p = it->second.lock();
		if(not p) continue;
		client->envoie_ws_chiffre(p->description());
	}
	for(const auto& i: client->_souscription_index)
	{
		auto t = _index.find(i);
		client->envoie_ws_chiffre(t->second);
	}
}

JSON_Interne::ObjetSP PiegePointsArret::mise_a_jour()
{
	std::lock_guard<std::mutex> verrou(_protection);
	auto o = JSON::objet();
	auto t = JSON::tableau();
	for(const auto& p: _points_arret)
	{
		t->ajoute(p.debugueur());
	}
	o->ajoute("breakpoints",t);
	return o;
}

void PiegePointsArret::evenement(const JSON_Interne::ObjetSP& evenement)
{
	auto pa = JSON::entier((*evenement)["remove_breakpoint"]);
	if(not JSON::est(pa)) return;
	size_t i = *pa;
	{
		std::lock_guard<std::mutex> verrou(_protection);
		auto it = std::find_if(_points_arret.begin(),_points_arret.end(),[i] (const PointArret& p) { return p._identifiant == i;});
		if(it!=_points_arret.end())
		{
			_points_arret.erase(it);
		}
	}
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("remove_breakpoint",pa);
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegePointsArret::ajout_point_arret(const PointArret& pointarret)
{
	{
		std::lock_guard<std::mutex> verrou(_protection);
		_points_arret.push_back(pointarret);
	}
	auto e = SVM_Debugueur::JSON::objet();
	e->ajoute("add_breakpoint",pointarret.debugueur());
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}

void PiegePointsArret::nettoie_formulaire(const size_t identifiant, const JSON_Interne::TableauSP& valeurs)
{
	auto e = SVM_Debugueur::JSON::objet();
	{
		std::lock_guard<std::mutex> verrou(_protection);
		auto it = std::find_if(_points_arret.begin(),_points_arret.end(),[identifiant] (const PointArret& p) { return p._identifiant == identifiant;});
		if(it==_points_arret.end())
			return;
		auto itv = valeurs->begin();
		auto champs = SVM_Debugueur::JSON::tableau((*(it->_formulaire))["fields"]);
		if(not champs) return;
		for(const auto& c: *champs)
		{
			auto cc = SVM_Debugueur::JSON::objet(c);
			if(not cc) return;
			(*cc)["value"] = *itv;
			++itv;
		}
		it->_formulaire->ajoute("submitted",SVM_Debugueur::JSON::booleen(true));
		e->ajoute("clear_form",SVM_Debugueur::JSON::objet({ { "identifier" , SVM_Debugueur::JSON::entier(identifiant) } , { "form", it->_formulaire} }));
	}
	notification_evenement_objet(SVM_Debugueur::Evenement::Categorie::CHANGEMENT_OBJET,e);
}
