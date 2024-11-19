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
#include <src/machine/machine/demarrage.h>
#include <src/machine/elements/memoire/valeur.h>
#include <src/machine/elements/valeur/interruption.h>
#include <src/machine/extensions/reference_objet.h>
#include <src/machine/extensions/patron_objet_extension.h>
#include <src/machine/interface/variable.h>
#include <src/machine/debugueur/debugueur.h>

namespace SVM_Debugueur = SVM::Machine::Debugueur;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Systeme
{
	struct Systeme;
}
}
namespace Extension
{

struct ExtensionConfiguration
{
	std::string _configuration;
	std::string _signature_api;
	std::string _version_svm;
};

	DECL_SHARED_PTR(Description);

	struct Bibliotheque
	{
		typedef void (*Fonction)(const void *svm);
		Bibliotheque(const std::string& nom, void *bibliotheque)
		:_nom(nom), _bibliotheque(bibliotheque), _initialisation_generale(nullptr), _demarrage_general(nullptr), _finalisation_generale(nullptr)//, _initialisation_processus(nullptr), _finalisation_processus(nullptr), _initialisation_noyau(nullptr), _finalisation_noyau(nullptr)
		{}

		std::string _nom;
		void *_bibliotheque;
		Fonction _initialisation_generale;
		Fonction _demarrage_general;
		Fonction _finalisation_generale;
		// Bonne idee, sauf que pas realisable (instabilites sur _finalisation_processus, et impraticable sur noyau)
		//Fonction _initialisation_processus;
		//Fonction _finalisation_processus;
		//Fonction _initialisation_noyau;
		//Fonction _finalisation_noyau;
	};

	DECL_SHARED_PTR(Gestionnaire);
	DECL_WEAK_PTR(Gestionnaire);

	DECL_SHARED_PTR(PiegeGestionnaireExtensions);
	struct PiegeGestionnaireExtensions : public SVM_Debugueur::Piege
	{
		PiegeGestionnaireExtensions(const GestionnaireSP& gestionnaire)
		:Piege("plugins"), _gestionnaire(gestionnaire) {}
		virtual ~PiegeGestionnaireExtensions() {}
		virtual SVM_Debugueur::JSON_Interne::ObjetSP mise_a_jour() override;

		GestionnaireWP _gestionnaire;
	};

	struct Gestionnaire : public std::enable_shared_from_this<Gestionnaire>
	{
		friend struct PiegeGestionnaireExtensions;
		friend struct SVM::Machine::Element::Systeme::Systeme;
		using MapOptions = std::map<std::string,SVM::Machine::Element::Memoire::ValeurSP>;
		using ListeArguments = std::vector<SVM::Machine::Element::Memoire::ValeurSP>;
		static GestionnaireSP gestionnaire(const std::vector<SVM::Machine::DeclarationExtension>& extensions, SVM_Debugueur::DebugueurSP& debugueur);
		Gestionnaire()
		:_nettoyage(false),_initialisation(false) {}
		~Gestionnaire();
		static std::set<std::string> tokens(const std::string& chaine, const char separateur)
		{
			std::set<std::string> elements;
			std::stringstream ss;
			ss.str(chaine);
			std::string item;
			while (std::getline(ss, item, separateur))
			{
				elements.insert(item);
			}
			return elements;
		}
		PatronTypeExtensionSP type(const SVM_Valeur::PointEntreeExtension& type) const;
		PatronInterruptionExtensionSP interruption(const SVM_Valeur::PointEntreeExtension& interruption) const;
		PatronInstructionExtensionSP instruction(const SVM_Valeur::PointEntreeExtension& instruction) const;
		PatronStructureExtensionSP structure(const SVM_Valeur::PointEntreeExtension& structure) const;
		PatronFonctionExtensionSP fonction(const SVM_Valeur::PointEntreeExtension& fonction) const;
		PatronSequenceurExtensionSP sequenceur(const SVM_Valeur::PointEntreeExtension& sequenceur) const;
		PatronOrdonnanceurExtensionSP ordonnanceur(const SVM_Valeur::PointEntreeExtension& ordonnanceur) const;
		PatronOptionExtensionSP option(const SVM_Valeur::PointEntreeExtension& option) const;
		PatronArgumentExtensionSP argument(const SVM_Valeur::PointEntreeExtension& argument) const;
		PatronArgumentsExtensionSP arguments(const SVM_Valeur::PointEntreeExtension& arguments) const;
		std::list<SVM_Valeur::PointEntreeExtension> liste_ordonannanceurs() const;
		const std::set<std::string>& extensions() const { return _extensions; }
		SVM_Interface::VariablesDefiniesSP variables_extension(const std::string& nom_extension) const;
		void initialisation_generale(const SVM::Machine::MachineSP& machine);
		void demarrage_general(const SVM::Machine::MachineSP& machine);
		void finalisation_generale(const SVM::Machine::MachineSP& machine);
		//void initialisation_processus(const SVM::Machine::Element::Processus::ProcessusSP& processus, const SVM::Machine::Element::Noyau::NoyauSP& noyau);
		//void finalisation_processus(const SVM::Machine::Element::Processus::ProcessusSP& processus);
		//void initialisation_noyau(const SVM::Machine::Element::Noyau::NoyauSP& noyau);
		//void finalisation_noyau(const SVM::Machine::Element::Noyau::NoyauSP& noyau);
		void nettoie_variables();
		std::set<SVM::Machine::Element::Valeur::Interruption> externes() const;
		template<typename Flux>
		friend Flux& operator<<(Flux& f, const Gestionnaire& g)
		{
			for(auto& o:g._objets)
			{
				f << *o.second << std::endl;
			}
			return f;
		}
		private:
		static SVM::Machine::Element::Memoire::ValeurSP conversion(const std::string& nom, const SVM::Machine::ParametreExtensionSP& p);
		std::map<ReferenceObjet,PatronObjetExtensionSP> _objets;
		std::vector<Bibliotheque> _bibliotheques;
		std::set<std::string> _extensions;
		mutable std::mutex _protection;
		std::map<std::string,SVM_Interface::VariablesDefiniesSP> _variables_extensions;
		public:
		bool _nettoyage;
		bool _initialisation;
		PiegeGestionnaireExtensionsSP _piege;
	};

	struct ExtensionFichierInvalide : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionFichierInvalide(const std::string& nom, const std::string& raison)
		{
			DETAILS_EXCEPTION("File " << nom << " is not a valid plugin file (" << raison << ").");
		}
		virtual ~ExtensionFichierInvalide() {}
	};

	struct ExtensionSansConfiguration : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionSansConfiguration(const std::string& nom)
		{
			DETAILS_EXCEPTION("Plugin file " << nom << " does not contain the C configuration function void plugin_configure(void *plugin).");
		}
		virtual ~ExtensionSansConfiguration() {}
	};

	struct ExtensionConfigurationSansEnregistrement : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionConfigurationSansEnregistrement(const std::string& nom)
		{
			DETAILS_EXCEPTION("Plugin file " << nom << " C configuration function did not register the plugin. Please call svm_plugin_configure API function from your plugin_configure plugin function.");
		}
		virtual ~ExtensionConfigurationSansEnregistrement() {}
	};

	struct ExtensionAPINonCompatible : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionAPINonCompatible(const std::string& nom, const std::string& svm_version)
		{
			DETAILS_EXCEPTION("Plugin file " << nom << " API version is not compatible. Please use SVM version " << svm_version << " or rebuild the plugin.");
		}
		virtual ~ExtensionAPINonCompatible() {}
	};

	struct ExtensionCollision : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionCollision(const std::string& fichier, const std::string& nom)
		{
			DETAILS_EXCEPTION("Plugin file " << fichier << " contains a second declaration of plugin " << nom << ".");
		}
		virtual ~ExtensionCollision() {}
	};

	struct ExtensionDependanceNonTrouvee : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionDependanceNonTrouvee(const std::string& nom, const PatronObjetExtensionSP& objet)
		{
			DETAILS_EXCEPTION("Plugin " << nom << " has an unsatisfied dependency on " << *objet << ".");
		}
		virtual ~ExtensionDependanceNonTrouvee() {}
	};

	struct ExtensionDependanceNonCompatible : public SVM_Valeur::ExceptionDemarrage
	{
		ExtensionDependanceNonCompatible(const std::string& nom, const PatronObjetExtensionSP& dependance, const PatronObjetExtensionSP& objet)
		{
			DETAILS_EXCEPTION("Plugin " << nom << " has an incompatible dependency on " << *objet << ". Dependency object is " << *dependance << ".");
		}
		virtual ~ExtensionDependanceNonCompatible() {}
	};

	struct TypeParametreInvalide : public SVM_Valeur::ExceptionDemarrage
	{
		TypeParametreInvalide(const std::string& nom)
		{
			DETAILS_EXCEPTION("Plugin " << nom << " has a parameter with an invalid type.");
		}
		virtual ~TypeParametreInvalide() {}
	};

	struct OptionEnDouble : public SVM_Valeur::ExceptionDemarrage
	{
		OptionEnDouble(const std::string& nom, const std::string& option)
		{
			DETAILS_EXCEPTION("Plugin " << nom << " used with the option -" << option << " specified twice.");
		}
		virtual ~OptionEnDouble() {}
	};

	struct OptionTypeIncorrect : public SVM_Valeur::ExceptionDemarrage
	{
		OptionTypeIncorrect(const std::string& nom, const std::string& option, const SVM_Memoire::Type& type)
		{
			DETAILS_EXCEPTION("Plugin " << nom << " used with the option -" << option << " having a value with an invalid type. " << type << " is requested.");
		}
		virtual ~OptionTypeIncorrect() {}
	};

	struct OptionAvecValeur : public SVM_Valeur::ExceptionDemarrage
	{
		OptionAvecValeur(const std::string& nom, const std::string& option)
		{
			DETAILS_EXCEPTION("Plugin " << nom << " used with the option -" << option << " having a value. No value is requested.");
		}
		virtual ~OptionAvecValeur() {}
	};

	struct PasAssezArguments : public SVM_Valeur::ExceptionDemarrage
	{
		PasAssezArguments(const std::string& nom, const size_t fournis, const size_t& attendus)
		{
			DETAILS_EXCEPTION("Plugin " << nom << " used with only " << fournis << " argument" << (fournis==1?"":"s") << ". At least " << attendus << " " << (attendus==1?"is":"are") << " requested.");
		}
		virtual ~PasAssezArguments() {}
	};

	struct ArgumentTypeIncorrect : public SVM_Valeur::ExceptionDemarrage
	{
		ArgumentTypeIncorrect(const std::string& nom, const size_t& indice, const SVM_Memoire::Type& type)
		{
			DETAILS_EXCEPTION("Plugin " << nom << " used with the argument at index " << indice << " having an invalid type. " << type << " is requested.");
		}
		virtual ~ArgumentTypeIncorrect() {}
	};

	struct OptionsNonReconnues : public SVM_Valeur::ExceptionDemarrage
	{
		OptionsNonReconnues(const std::string& nom, const std::string& options)
		{
			DETAILS_EXCEPTION("Plugin " << nom << " used with unknown option" << (options.size()==3?"":"s") << options << ".");
		}
		virtual ~OptionsNonReconnues() {}
	};
}
}
}
