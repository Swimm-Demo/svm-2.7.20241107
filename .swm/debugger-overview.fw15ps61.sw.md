---
title: Debugger Overview
---
<SwmSnippet path="/src/machine/debugueur/debugueur.cpp" line="28">

---

## Initialization

The Debugger is initialized with various parameters such as title, IP address, port, style, number of clients, security level, and history. This is done in the constructor <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="28:0:2" line-data="Debugueur::Debugueur(const std::string&amp; titre, const std::string&amp; ip, const std::string&amp; port, const std::string&amp; style, const size_t nombre_clients, const size_t niveau_securite, const SVM::Machine::HistoriqueSP&amp; historique)">`Debugueur::Debugueur`</SwmToken>.

```c++
Debugueur::Debugueur(const std::string& titre, const std::string& ip, const std::string& port, const std::string& style, const size_t nombre_clients, const size_t niveau_securite, const SVM::Machine::HistoriqueSP& historique)
:_titre(titre), _ip(ip), _port(port), _style(style), _nombre_clients(nombre_clients), _niveau_securite(niveau_securite), _historique(historique), _livraison(std::make_shared<SVM::Machine::Debugueur::Livraison>()), _fd_fin(-1), _fd_declenchement_fin(-1), _fd_serveur(-1)
{
	if(_nombre_clients<2) _nombre_clients=2;
	if(_niveau_securite>255) _niveau_securite=255;
	_styles_html = SVM::Debug::Styles::Styles::html(_style);
}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/debugueur/debugueur.cpp" line="36">

---

## Creation

The <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="36:4:4" line-data="DebugueurSP Debugueur::creation(const SVM::Machine::DebugSP&amp; debug)">`creation`</SwmToken> method initializes the Debugger and sets up the necessary components for handling connections and commands.

```c++
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
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/debugueur/debugueur.cpp" line="360">

---

## Handling HTTP Requests

The <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="360:4:4" line-data="void Debugueur::traite_requete_http(const ClientSP&amp; client, const std::string&amp; requete)">`traite_requete_http`</SwmToken> method handles incoming HTTP requests and processes them accordingly.

```c++
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
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/debugueur/debugueur.cpp" line="422">

---

## Handling <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="368:6:6" line-data="		SVM_TRACE(&quot;Ouverture WebSocket demandee &quot; &lt;&lt; client.get());">`WebSocket`</SwmToken> Requests

The <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="422:4:4" line-data="void Debugueur::traite_requete_ws(const ClientSP&amp; client, const std::string&amp; requete)">`traite_requete_ws`</SwmToken> method handles incoming <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="368:6:6" line-data="		SVM_TRACE(&quot;Ouverture WebSocket demandee &quot; &lt;&lt; client.get());">`WebSocket`</SwmToken> requests and processes them accordingly.

```c++
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
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/debugueur/debugueur.cpp" line="303">

---

## Notification of Object Changes

The <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="303:4:4" line-data="void Debugueur::notification_changement_objet(const PiegeSP&amp; piege) ">`notification_changement_objet`</SwmToken> method notifies the Debugger of changes in objects.

```c++
void Debugueur::notification_changement_objet(const PiegeSP& piege) 
{
	auto e = std::make_shared<Evenement>(Evenement::Categorie::STATUS_OBJET,piege->_description,piege->identifiant());
	_livraison->poste(_adresse,piege->_adresse,e);
}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/debugueur/debugueur.cpp" line="309">

---

## Notification of Object Events

The <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="309:4:4" line-data="void Debugueur::notification_evenement_objet(const PiegeSP&amp; piege, const Evenement::Categorie categorie, const JSON_Interne::ObjetSP&amp; evenement)">`notification_evenement_objet`</SwmToken> method notifies the Debugger of events related to objects.

```c++
void Debugueur::notification_evenement_objet(const PiegeSP& piege, const Evenement::Categorie categorie, const JSON_Interne::ObjetSP& evenement)
{
	auto o = JSON::objet({ { "command" , JSON::chaine("event") } , { "identifier", JSON::entier(piege->identifiant()) } , { "event" , evenement } });
	auto e = std::make_shared<Evenement>(categorie,JSON::serialise(o),piege->identifiant());
	_livraison->poste(_adresse,piege->_adresse,e);
}
```

---

</SwmSnippet>

<SwmSnippet path="/src/machine/debugueur/debugueur.cpp" line="343">

---

## Notification of End of Debugging Session

The <SwmToken path="src/machine/debugueur/debugueur.cpp" pos="343:4:4" line-data="void Debugueur::notification_fin() ">`notification_fin`</SwmToken> method notifies the Debugger of the end of a debugging session.

```c++
void Debugueur::notification_fin() 
{
	auto o = JSON::objet({ { "command" , JSON::chaine("end") } });
	auto e = std::make_shared<Evenement>(Evenement::Categorie::RESULTAT,JSON::serialise(o));
	_livraison->poste(_adresse,_adresse,e);
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBc3ZtLTIuNy4yMDI0MTEwNyUzQSUzQVN3aW1tLURlbW8=" repo-name="svm-2.7.20241107"><sup>Powered by [Swimm](/)</sup></SwmMeta>
