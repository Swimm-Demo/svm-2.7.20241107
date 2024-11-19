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
#include <src/machine/elements/ordonnanceur/ordonnanceur.h>
#include <src/machine/elements/systeme/evenement.h>
#include <src/machine/machine/demarrage.h>

namespace SVM_Machine = SVM::Machine;
namespace SVM_Interface = SVM::Machine::Interface;
namespace SVM_Ordonnanceur = SVM::Machine::Element::Ordonnanceur;
namespace SVM_Synchronisation = SVM::Machine::Element::Synchronisation;

namespace SVM
{
namespace Machine
{
	DECL_WEAK_PTR(Machine);
	DECL_SHARED_PTR(Machine);
namespace Interruption
{
	DECL_SHARED_PTR(Interruption);
}
namespace Element
{
namespace Ordonnanceur
{
	struct OrdonnanceurDefaut;
	struct OrdonnanceurExtension;
}
namespace Processus
{
	struct Processus;
}
namespace Systeme
{
	DECL_SHARED_PTR(Systeme);
	DECL_WEAK_PTR(Systeme);

	struct Systeme : public std::enable_shared_from_this<Systeme>
	{
		friend struct Ordonnanceur::OrdonnanceurDefaut;
		friend struct Ordonnanceur::OrdonnanceurExtension;
		friend struct Processus::Processus;
		static SystemeSP creation_systeme(const SVM_Machine::MachineSP& machine);
		Systeme(const SVM_Machine::MachineSP& machine);
		virtual ~Systeme();
		int execution(const SVM::Machine::MachineSP& machine, std::vector<SVM_Machine::Processus>& liste_processus);
		void terminaison_demandee();
		void acquittement_terminaison(const SVM_Synchronisation::AdresseSP& source);
		void ajoute_processus(const SVM_Processus::ProcessusSP& processus);
		bool retire_processus(const SVM_Processus::ProcessusSP& processus);
		std::vector<SVM_Processus::ProcessusSP> liste_processus() const;
		const void* ajout_environnement(const SVM_Interface::EnvironnementSP& environnement);
		bool enleve_environnement(const void *clef);
		void notifie_etat(const SVM_Synchronisation::AdresseSP source, const SVM_Processus::Etat::Execution etat)
		{
			_livraison->poste(_adresse,source,std::make_shared<Evenement>(Evenement::Type::ETAT,etat));
		}
		SHARED_PTR(size_t) traitement_processus_mort(const SVM_Processus::ProcessusSP& processus,const SVM_Processus::Etat::Execution etat_declare);
		SVM_Ordonnanceur::OrdonnanceurSP ordonnanceur(const SVM_Valeur::PointEntreeExtensionSP& nom) const;
		SVM_Interface::VariablesDefiniesSP variables_extension(const std::string& nom_extension) const;
		void interruption_materielle();
		void force_deverrouillage(const SVM_Synchronisation::AdresseSP& adresse);
		bool attente_reponse(const SVM_Processus::ProcessusSP& processus) const;
		void encore_un_vivant(bool& alerte);
		SVM_Systeme::Evenement::Reponse attente_verrouillage(const SVM_Synchronisation::AdresseSP& verrouillage);
		SVM_Systeme::Evenement::Reponse attente_reponse(const SVM_Synchronisation::AdresseSP& origine);
		template<typename Flux>
		void nom_flux(Flux& f) const
		{
			f << "SYSTEM";
		}
		template<typename Flux>
		friend Flux& operator<<(Flux& p, const Systeme& o)
		{
			o.nom_flux(p);
			return p;
		}
		private:
		std::map<SVM_Valeur::PointEntreeExtensionSP,SVM_Ordonnanceur::OrdonnanceurSP,SVM::Global::SPinferieurornulSP<SVM_Valeur::PointEntreeExtension> > _ordonnanceurs;
		std::vector<SVM_Processus::ProcessusSP> _processus;
		mutable std::mutex _protection;
		public:
		const LivraisonSP _livraison;
		SVM_Synchronisation::AdresseSP _adresse;
		const SVM_Machine::MachineWP _machine;
	};
}
}
}
}
