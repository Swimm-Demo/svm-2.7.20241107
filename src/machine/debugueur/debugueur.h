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

#include <src/machine/machine/demarrage.h>
#include <src/machine/machine/historique.h>
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/debugueur/client.h>
#include <src/machine/debugueur/json.h>
#include <src/machine/debugueur/evenement.h>
#include <src/machine/debugueur/piege.h>

namespace SVM
{
namespace Machine
{
namespace Debugueur
{
DECL_SHARED_PTR(Debugueur);
DECL_WEAK_PTR(Debugueur);

DECL_SHARED_PTR(PiegePointsArret);

struct PointArret
{
	PointArret(const std::string& type, const std::string& message, const size_t noyau, const std::string& nom_processus, const std::string& nom_noyau, const JSON_Interne::ObjetSP& formulaire = JSON_Interne::ObjetSP())
	:_identifiant(PointArret::nouvel_identifiant()), _type(type), _message(message), _presence_noyau(true), _noyau(noyau), _nom_processus(nom_processus), _nom_noyau(nom_noyau), _formulaire(formulaire) {}
	PointArret(const std::string& type, const std::string& message)
	:_identifiant(PointArret::nouvel_identifiant()), _type(type), _message(message), _presence_noyau(false), _noyau(0) {}
	virtual ~PointArret() {}
	static size_t nouvel_identifiant()
	{
		static size_t id = 0;
		return ++id;
	}
	JSON_Interne::ObjetSP debugueur() const
	{
		auto o = JSON::objet({ { "identifier" , JSON::entier(_identifiant) } , { "type" , JSON::chaine(_type) } , { "message", JSON::chaine(_message) } });
		if(_presence_noyau)
		{ 
			o->ajoute("kernel",JSON::entier(_noyau));
			o->ajoute("process_name",JSON::chaine(_nom_processus));
			o->ajoute("kernel_name",JSON::chaine(_nom_noyau));
		}
		if(static_cast<bool>(_formulaire))
		{
			o->ajoute("form",_formulaire);
		}
		return o;
	}
	size_t _identifiant;
	std::string _type;
	std::string _message;
	bool _presence_noyau;
	size_t _noyau;
	std::string _nom_processus;
	std::string _nom_noyau;
	JSON_Interne::ObjetSP _formulaire;
};

struct PiegePointsArret : public Piege
{
	PiegePointsArret()
	:Piege("breakpoints") {}
	virtual ~PiegePointsArret() {}
	virtual JSON_Interne::ObjetSP mise_a_jour() override;
	virtual void evenement(const JSON_Interne::ObjetSP& evenement) override;
	void ajout_point_arret(const PointArret& pointarret);
	void nettoie_formulaire(const size_t identifiant, const JSON_Interne::TableauSP& valeurs);
	mutable std::mutex _protection;
	std::vector<PointArret> _points_arret;
};

struct Debugueur : public std::enable_shared_from_this<Debugueur>
{
	friend struct Piege;
	static DebugueurSP creation(const SVM::Machine::DebugSP& debug);
	static void arret(DebugueurSP& debugueur);
	Debugueur(const std::string& titre, const std::string& ip, const std::string& port, const std::string& style, const size_t nombre_clients, const size_t niveau_securite, const SVM::Machine::HistoriqueSP& historique);
	virtual ~Debugueur()
	{
		SVM_TRACE("~Debugueur");
	}
	template<typename Flux>
	void nom_flux(Flux& f) const
	{
		f << "DEBUG"; 
	}
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const Debugueur& debugueur)
	{
		f << debugueur._titre << " IP=" << debugueur._ip << " PORT=" << debugueur._port << " STYLE=" << debugueur._style << " CLIENTS=" << debugueur._nombre_clients << " SECURITE=" << debugueur._niveau_securite;
		return f;
	}
	static size_t nouvel_identifiant_piege()
	{
		static size_t id = 0;
		return id++;
	}
	void creation_effecteurs();
	template<typename P, typename... Args>
	static std::shared_ptr<P> ajoute_piege(DebugueurSP& debugueur, const Args&... args)
	{
		if(not debugueur) return std::shared_ptr<P>();
		auto p = std::make_shared<P>(args...);
		p->identifiant() = Debugueur::nouvel_identifiant_piege();
		p->_debugueur = debugueur;
		p->_adresse = std::make_shared<SVM::Machine::Element::Synchronisation::Objet<Piege>>(p);
		{
			std::lock_guard<std::mutex> verrou(debugueur->_protection);
			debugueur->_pieges.insert(std::make_pair(p->identifiant(),p));
		}
		debugueur->_livraison->creation_boite(p->_adresse);
		p->notification_changement_objet();
		debugueur->notification_changement_index(p->type());
		return p;
	}
	void enleve_piege(const size_t& identifiant, const SVM_Synchronisation::AdresseSP& adresse)
	{
		std::lock_guard<std::mutex> verrou(_protection);
		auto it=_pieges.find(identifiant);
		if(it==_pieges.end()) return;
		_livraison->destruction_boite(adresse);
		_pieges.erase(it);
	}
	template<typename P>
	std::shared_ptr<P> recupere_piege(const size_t& identifiant)
	{
		std::lock_guard<std::mutex> verrou(_protection);
		auto it=_pieges.find(identifiant);
		if(it==_pieges.end()) return std::shared_ptr<P>();
		auto p = it->second.lock();
		return std::dynamic_pointer_cast<P>(p);
	}
	void notification_changement_objet(const PiegeSP& piege);
	void notification_evenement_objet(const PiegeSP& piege, const Evenement::Categorie categorie, const JSON_Interne::ObjetSP& evenement);
	void notification_suppression_objet(const size_t& identifiant);
	void notification_changement_index(const std::string& type);
	void notification_fin();
	JSON_Interne::ObjetSP attente_commande(const PiegeSP& piege);
	private:
	std::string _titre;
	std::string _ip;
	std::string _port;
	std::string _style;
	std::atomic_size_t _nombre_clients;
	size_t _niveau_securite;
	SVM::Machine::HistoriqueSP _historique;
	std::vector<SVM::Machine::Debugueur::ClientSP> _clients;
	SVM::Machine::Debugueur::LivraisonSP _livraison;
	SVM::Machine::Element::Synchronisation::AdresseSP _adresse;
	std::thread _lecteur;
	std::thread _ecrivain;
	int _fd_fin;
	int _fd_declenchement_fin;
	int _fd_serveur;
	mutable std::mutex _protection;
	std::map<size_t, PiegeWP> _pieges;
	std::map<std::string, std::string> _index;
	std::string _styles_html;
	bool _affiche_avertissement = true;
	public:
	PiegePointsArretSP _points_arret;
	private:
	
	void envoie_tout(const ClientSP& client) const;

	void traite_requete_http(const ClientSP& client, const std::string& requete);
	void traite_requete_http_ouverture(const ClientSP& client);
	void traite_requete_http_session(const ClientSP& client, const std::string& ressource);

	void traite_requete_ws(const ClientSP& client, const std::string& requete);
	Chiffre::Challenges challenges() const
	{
		Chiffre::Challenges challenges;
		for(size_t i=0 ; i<_niveau_securite ; ++i)
		{
			challenges.push_back(std::make_pair(Securite::genere_aleatoire(10), Securite::genere_aleatoire(10)));
		}
		return challenges;
	}
	static JSON_Interne::ObjetSP convertit_challenges(const Chiffre::Challenges& challenges)
	{
		auto tableau = JSON::tableau();
		for(const auto& c: challenges)
		{
			tableau->ajoute(JSON::tableau({ JSON::chaine(Securite::encode_hex(c.first)), JSON::chaine(Securite::encode_hex(c.second)) }));
		}
		return JSON::objet({ { "command" , JSON::chaine("cipher") } , { "challenges" , tableau } } );
	}

	std::vector<SVM::Machine::Debugueur::ClientSP> clients() const
	{
		std::lock_guard<std::mutex> verrou(_protection);
		return std::vector<SVM::Machine::Debugueur::ClientSP>(_clients.begin(),_clients.end());
	}
};

struct PasDeDebuggueur : public SVM::Machine::Element::Valeur::ExceptionDemarrage
{
	PasDeDebuggueur()
	{
		DETAILS_EXCEPTION("Unable to start debugger.");
	}
	virtual ~PasDeDebuggueur() {}
};
}
}
}
