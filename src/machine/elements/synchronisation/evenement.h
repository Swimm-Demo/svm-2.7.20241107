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
#include <src/machine/elements/valeur/exception.h>

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Synchronisation
{
	template<typename Evenement>
	struct Livraison;
	template<typename Evenement>
	struct Boite;

	DECL_SHARED_PTR(Adresse);
	struct Adresse
	{
		Adresse(const Adresse&) = delete;
		virtual ~Adresse() {}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Adresse& a)
		{
			f << "<" << (&a) << ":";
			a.format(f);
			f << ">";
			return f;
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const AdresseSP& a)
		{
			f << "<" << a.get() << ":";
			if(static_cast<bool>(a))
			{
				a->format(f);
			}
			f << ">";
			return f;
		}
		virtual void format(std::ostream& p) const = 0;
		virtual void format(SVM::Global::Print& p) const = 0;
		virtual std::string uml() const = 0;
		protected:
		Adresse() {};
	};

	template<typename Type>
	struct Objet : public Adresse
	{
		template<typename Evenement>
		friend struct Livraison;
		Objet(const WEAK_PTR(Type)& o)
		:_objet(o) {}
		virtual void format(std::ostream& p) const override
		{
			auto o = _objet.lock();
			if(not o)
			{
				p << "?";
			}
			else
			{
				o->nom_flux(p);
			}
		}
		virtual void format(SVM::Global::Print& p) const override
		{
			auto o = _objet.lock();
			if(not o)
			{
				p << "?";
			}
			else
			{
				o->nom_flux(p);
			}
		}
		virtual std::string uml() const override
		{
			std::ostringstream oss;
			auto o = _objet.lock();
			if(not o)
			{
				oss << "unknown";
			}
			else
			{
				o->nom_flux(oss);
			}
			oss << "_" << this;
			return oss.str();
		}
		private:
			WEAK_PTR(Type) _objet;
	};

	template<typename Evenement>
	struct Message
	{
		friend struct Livraison<Evenement>;
		friend struct Boite<Evenement>;
		Message(const AdresseSP& source, const SHARED_PTR(Evenement)& evenement)
		:_source(source), _evenement(evenement) {}
		Message(const Message&) = delete;
		private:
			AdresseSP _source;
			SHARED_PTR(Evenement) _evenement;
	};

	DECL_SHARED_PTR(BoiteGenerique);
	struct BoiteGenerique : public std::enable_shared_from_this<BoiteGenerique>
	{
		virtual ~BoiteGenerique() {}
		virtual void debloque() = 0;
	};

	DECL_SHARED_PTR(Notification);
	struct Notification
	{
		Notification()
		:_notification(false) {}
		void arme(const BoiteGeneriqueSP& boite)
		{
			_boite = boite;
		}
		void desarme()
		{
			_boite.reset();
		}
		void initialise()
		{
			_notification = false;
		}
		bool verifie()
		{
			return _notification.load();
		}
		void debloque()
		{
			_notification = true;
			if(static_cast<bool>(_boite))
			{
				_boite->debloque();
			}
		}
		std::atomic_bool _notification;
		BoiteGeneriqueSP _boite;
	};

	struct ArmementNotification
	{
		ArmementNotification(NotificationSP& notification, const BoiteGeneriqueSP& boite)
		:_notification(notification)
		{
			_notification->arme(boite);
		}
		~ArmementNotification()
		{
			_notification->desarme();
		}
		NotificationSP _notification;
	};

	template<typename Evenement>
	struct Boite : public BoiteGenerique
	{
		friend struct Livraison<Evenement>;
		Boite()
		:_fin(false) {};
		Boite(const Boite&) = delete;
		~Boite() { SVM_TRACE("DESTRUCTION boite");}
		void poste(const SHARED_PTR(Message<Evenement>)& message)
		{
			{
				std::unique_lock<std::mutex> verrou(_protection);
				_messages_en_attente.push_back(message);
			}
			_attente.notify_one();
		}
		virtual void debloque() override
		{
			_attente.notify_one();
		}
		SHARED_PTR(Message<Evenement>) recupere(const bool bloquant)
		{
			std::unique_lock<std::mutex> verrou(_protection);
			while(_messages_en_attente.empty())
			{
				if(not bloquant)
				{
					return SHARED_PTR(Message<Evenement>)();
				}
				_attente.wait(verrou);
				if(_fin.load(std::memory_order_relaxed))
				{
					SVM_TRACE("Destruction boite detectee");
					return SHARED_PTR(Message<Evenement>)();
				}
			}
			auto message = _messages_en_attente.front();
			_messages_en_attente.pop_front();
			return message;
		}
		SHARED_PTR(Message<Evenement>) recupere_echeance(const size_t echeance)
		{
			std::unique_lock<std::mutex> verrou(_protection);
			while(_messages_en_attente.empty())
			{
				if(_attente.wait_for(verrou,std::chrono::milliseconds(echeance))==std::cv_status::timeout)
				{
					return SHARED_PTR(Message<Evenement>)();
				}
				if(_fin.load(std::memory_order_relaxed))
				{
					SVM_TRACE("Destruction boite detectee");
					return SHARED_PTR(Message<Evenement>)();
				}
			}
			auto message = _messages_en_attente.front();
			_messages_en_attente.pop_front();
			return message;
		}
		SHARED_PTR(Message<Evenement>) recupere_notification(const bool bloquant, NotificationSP& notification)
		{
			std::unique_lock<std::mutex> verrou(_protection);
			ArmementNotification armement(notification,this->shared_from_this());
			while(_messages_en_attente.empty())
			{
				if(not bloquant)
				{
					return SHARED_PTR(Message<Evenement>)();
				}
				notification->initialise();
				_attente.wait(verrou);
				if(notification->verifie())
				{
					return SHARED_PTR(Message<Evenement>)();
				}
				if(_fin.load(std::memory_order_relaxed))
				{
					SVM_TRACE("Destruction boite detectee");
					return SHARED_PTR(Message<Evenement>)();
				}
			}
			auto message = _messages_en_attente.front();
			_messages_en_attente.pop_front();
			return message;
		}
		SHARED_PTR(Message<Evenement>) recupere_echeance_notification(const size_t echeance, NotificationSP& notification)
		{
			std::unique_lock<std::mutex> verrou(_protection);
			ArmementNotification armement(notification,this->shared_from_this());
			while(_messages_en_attente.empty())
			{
				notification->initialise();
				if(_attente.wait_for(verrou,std::chrono::milliseconds(echeance))==std::cv_status::timeout)
				{
					return SHARED_PTR(Message<Evenement>)();
				}
				if(notification->verifie())
				{
					return SHARED_PTR(Message<Evenement>)();
				}
				if(_fin.load(std::memory_order_relaxed))
				{
					SVM_TRACE("Destruction boite detectee");
					return SHARED_PTR(Message<Evenement>)();
				}
			}
			auto message = _messages_en_attente.front();
			_messages_en_attente.pop_front();
			return message;
		}
		SHARED_PTR(Message<Evenement>) recupere_specifique(std::function<bool(const AdresseSP& source, const SHARED_PTR(Evenement)&)>& validateur)
		{
			std::unique_lock<std::mutex> verrou(_protection);
			SHARED_PTR(Message<Evenement>) message;
			std::list<SHARED_PTR(Message<Evenement>)> messages_en_attente;
			for( ; ; )
			{
				while(_messages_en_attente.empty())
				{
					_attente.wait(verrou);
					if(_fin.load(std::memory_order_relaxed))
					{
						SVM_TRACE("Destruction boite detectee");
						for(const auto& m: messages_en_attente)
						{
							_messages_en_attente.push_front(m);
						}
						return SHARED_PTR(Message<Evenement>)();
					}
				}
				message = _messages_en_attente.front();
				_messages_en_attente.pop_front();
				if(validateur(message->_source,message->_evenement))
				{
					break;
				}
				messages_en_attente.push_front(message);
			}
			for(const auto& m: messages_en_attente)
			{
				_messages_en_attente.push_front(m);
			}
			return message;
		}
		void nettoie()
		{
			std::unique_lock<std::mutex> verrou(_protection);
			_messages_en_attente.clear();
		}
		private:
			std::list<SHARED_PTR(Message<Evenement>)> _messages_en_attente;
			mutable std::mutex _protection;
			mutable std::condition_variable _attente;
			std::atomic_bool _fin;
	};

	struct AdresseInvalide : public SVM_Valeur::ExceptionExecution
	{
		AdresseInvalide()
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			DETAILS_EXCEPTION("Invalid event queue address.");
		}
		virtual ~AdresseInvalide() {}
	};

	template<typename Evenement>
	struct Livraison
	{
		Livraison() = default;
		Livraison(const Livraison&) = delete;
		virtual ~Livraison()
		{
			SVM_TRACE("DESTRUCTION livraison");
			decltype(_boites) boites;
			{
				std::lock_guard<std::mutex> verrou(_protection);
				_boites.swap(boites);
			}
			for(auto& b:boites)
			{
				b.second->_fin.store(true,std::memory_order_relaxed);
				b.second->_attente.notify_all();
			}
		}
		bool creation_boite(const AdresseSP& adresse)
		{
			SVM_TRACE("AJOUT " << adresse);
			std::lock_guard<std::mutex> verrou(_protection);
			return _boites.insert(std::make_pair(adresse,std::make_shared<Boite<Evenement> >())).second;
		}
		void destruction_boite(const AdresseSP& adresse)
		{
			SVM_TRACE("RETIRE " << adresse);
			SHARED_PTR(Boite<Evenement>) boite;
			{
				std::lock_guard<std::mutex> verrou(_protection);
				auto it = _boites.find(adresse);
				if(it!=_boites.end())
				{
					boite = it->second;
					_boites.erase(it);
				}
				else
				{
					return;
				}
			}
			boite->_fin.store(true,std::memory_order_relaxed);
			boite->_attente.notify_all();
		}
		void poste(const AdresseSP& destination, const AdresseSP& source, const SHARED_PTR(Evenement)& evenement)
		{
			piege_poste(source,destination,evenement);
			SVM_TRACE("ENVOI " << source << " => " << destination << " : " << evenement);
			auto message = std::make_shared<Message<Evenement> >(source,evenement);
			auto boite_destination = boite(destination);
			boite_destination->poste(message);
			std::this_thread::yield();
		}
		bool recupere(const AdresseSP& destination, AdresseSP& source, SHARED_PTR(Evenement)& evenement, const bool attente=true)
		{
			SVM_TRACE("RECEPTION " << (attente?"ATTENTE":"") << " " << destination << " <= ? : ?" );
			auto boite_destination = boite(destination);
			auto message = boite_destination->recupere(attente);
			if(not message) return false;
			source = message->_source;
			evenement = message->_evenement;
			SVM_TRACE("RECU " << destination << " <= " << source << " : " << evenement);
			piege_reception(source,destination,evenement);
			return true;
		}
		bool recupere_echeance(const AdresseSP& destination, AdresseSP& source, SHARED_PTR(Evenement)& evenement, const size_t echeance)
		{
			SVM_TRACE("RECEPTION " << echeance << " " << destination << " <= ? : ?" );
			auto boite_destination = boite(destination);
			auto message = boite_destination->recupere_echeance(echeance);
			if(not message) return false;
			source = message->_source;
			evenement = message->_evenement;
			SVM_TRACE("RECU " << destination << " <= " << source << " : " << evenement);
			piege_reception(source,destination,evenement);
			return true;
		}
		bool recupere_specifique(const AdresseSP& destination, AdresseSP& source, SHARED_PTR(Evenement)& evenement, std::function<bool(const AdresseSP& source, const SHARED_PTR(Evenement)&)>&& validateur)
		{
			SVM_TRACE("RECEPTION VALIDATION" << " " << destination << " <= ? : ?" );
			auto boite_destination = boite(destination);
			auto message = boite_destination->recupere_specifique(validateur);
			if(not message) return false;
			source = message->_source;
			evenement = message->_evenement;
			SVM_TRACE("RECU " << destination << " <= " << source << " : " << evenement);
			piege_reception(source,destination,evenement);
			return true;
		}
		bool recupere_notification(const AdresseSP& destination, AdresseSP& source, SHARED_PTR(Evenement)& evenement, NotificationSP& notification, const bool attente=true)
		{
			SVM_TRACE("RECEPTION " << (attente?"ATTENTE":"") << " " << destination << " <= ? : ?" );
			auto boite_destination = boite(destination);
			auto message = boite_destination->recupere_notification(attente,notification);
			if(not message) return false;
			source = message->_source;
			evenement = message->_evenement;
			SVM_TRACE("RECU " << destination << " <= " << source << " : " << evenement);
			piege_reception(source,destination,evenement);
			return true;
		}
		bool recupere_echeance_notification(const AdresseSP& destination, AdresseSP& source, SHARED_PTR(Evenement)& evenement, NotificationSP& notification, const size_t echeance)
		{
			SVM_TRACE("RECEPTION " << echeance << " " << destination << " <= ? : ?" );
			auto boite_destination = boite(destination);
			auto message = boite_destination->recupere_notification(echeance,notification);
			if(not message) return false;
			source = message->_source;
			evenement = message->_evenement;
			SVM_TRACE("RECU " << destination << " <= " << source << " : " << evenement);
			piege_reception(source,destination,evenement);
			return true;
		}
		void nettoie(const AdresseSP& destination)
		{
			SVM_TRACE("NETTOYAGE " << destination);
			auto boite_destination = boite(destination);
			boite_destination->nettoie();
		}
		virtual void piege_poste(const AdresseSP& source, const AdresseSP& destination, const SHARED_PTR(Evenement)& evenement) { }
		virtual void piege_reception(const AdresseSP& source, const AdresseSP& destination, const SHARED_PTR(Evenement)& evenement) { }
		template<typename Type>
		static AdresseSP adresse(const SHARED_PTR(Type)& objet)
		{
			return std::make_shared<Objet<Type> >(objet);
		}

		template<typename Type>
		static SHARED_PTR(Type) objet(const AdresseSP& adresse)
		{
			auto objet = std::dynamic_pointer_cast<Objet<Type> >(adresse);
			if(not objet)
				return SHARED_PTR(Type)();
			return objet->_objet.lock();
		}
		std::vector<AdresseSP> liste_adresses() const
		{
			std::vector<AdresseSP> adresses;
			for(const auto& b: _boites)
			{
				adresses.push_back(b.first);
			}
			return adresses;
		}
		private:
			SHARED_PTR(Boite<Evenement>) boite(const AdresseSP& adresse) const
			{
				std::lock_guard<std::mutex> verrou(_protection);
				auto it = _boites.find(adresse);
				if(it == _boites.end())
					throw AdresseInvalide();
				return it->second;
			}
			std::map<AdresseSP,SHARED_PTR(Boite<Evenement>)> _boites;
			mutable std::mutex _protection;
	};
}
}
}
}
