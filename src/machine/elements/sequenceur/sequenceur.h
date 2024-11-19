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
#include <src/machine/elements/noyau/noyau.h>
#include <src/machine/elements/valeur/parametre.h>
#include <src/machine/elements/valeur/exception.h>
#include <src/machine/elements/valeur/point_entree_extension.h>
#include <src/machine/interface/svm.h>
#include <src/machine/debugueur/piege.h>

namespace SVM_Noyau = SVM::Machine::Element::Noyau;
namespace SVM_Valeur = SVM::Machine::Element::Valeur;
namespace SVM_Debugueur = SVM::Machine::Debugueur;

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Processus
{
	struct Processus;
	struct PiegeProcessus;
}
namespace Noyau
{
	struct Noyau;
}
namespace Sequenceur
{
	struct OperationNonSupportee : public SVM_Valeur::ExceptionExecution
	{
		OperationNonSupportee(const SVM_Valeur::PointEntreeExtensionSP& nom, const std::string& op)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			if(static_cast<bool>(nom))
			{
				DETAILS_EXCEPTION("Sequencer " << *nom << " has no " << op << " operation.");
			}
			else
			{
				DETAILS_EXCEPTION("Default sequencer has no " << op << " operation.");
			}
		}
		virtual ~OperationNonSupportee() {}
	};

DECL_SHARED_PTR(Sequenceur);
DECL_WEAK_PTR(Sequenceur);

DECL_SHARED_PTR(PiegeSequenceur);
struct PiegeSequenceur : public SVM_Debugueur::Piege
{
	PiegeSequenceur(const SequenceurWP& sequenceur)
	:SVM_Debugueur::Piege("sequencer"), _sequenceur(sequenceur) {}
	virtual ~PiegeSequenceur() {}

	virtual SVM_Debugueur::JSON_Interne::ObjetSP mise_a_jour() override;

	SequenceurWP _sequenceur;
};

struct Sequenceur : public std::enable_shared_from_this<Sequenceur>
{
	friend struct SVM::Machine::Element::Processus::Processus;
	friend struct SVM::Machine::Element::Processus::PiegeProcessus;
	friend struct SVM::Machine::Element::Noyau::PiegeNoyau;
	static SequenceurSP creation_sequenceur(SVM_Debugueur::DebugueurSP& debugueur, const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM_Processus::ProcessusSP& processus);
	Sequenceur(const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM_Processus::ProcessusSP& processus)
	:_nom(nom), _processus(processus) {}
	virtual ~Sequenceur() {}
	void initialise(const SVM::Machine::Element::Processus::ProcessusSP processus) { _processus = processus; }

	virtual SVM_Noyau::NoyauSP noyau_courant() = 0;
	virtual bool attache_noyau(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& env_parent, const size_t argc, SVM_Parameter *argv) = 0;
	virtual bool detache_noyau(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& env_parent, const size_t argc, SVM_Parameter *argv) = 0;
	virtual void affiche(std::ostream& os) const = 0;
	const SVM_Valeur::PointEntreeExtensionSP& nom() const { return _nom; }
	virtual void* interne() = 0;
	template<typename Flux>
	friend Flux& operator<<(Flux& f, const Sequenceur& s)
	{
		s.nom_flux(f);
		f << ":" << std::endl;
		s.affiche(f);
		return f;
	}
	template<typename Flux>
	void nom_flux(Flux& f) const
	{
		f << "SEQUENCER";
		if(static_cast<bool>(_nom))
		{
			f << " " << (*_nom);
		}
	}
	protected:
	SVM_Valeur::PointEntreeExtensionSP _nom;
	SVM::Machine::Element::Processus::ProcessusWP _processus;
	PiegeSequenceurSP _piege;
	private:
	PiegeSequenceurSP& piege() { return _piege; }
};

	struct NoyauDejaAttache : public SVM_Valeur::ExceptionExecution
	{
		NoyauDejaAttache(const SVM_Valeur::PointEntreeExtensionSP& nom)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			if(static_cast<bool>(nom))
			{
				DETAILS_EXCEPTION("Cannot attach already attached kernel to " << (*nom) << " sequencer.");
			}
			else
			{
				DETAILS_EXCEPTION("Cannot attach already attached kernel to default sequencer.");
			}
		}
		virtual ~NoyauDejaAttache() {}
	};

	struct NoyauDejaDetache : public SVM_Valeur::ExceptionExecution
	{
		NoyauDejaDetache(const SVM_Valeur::PointEntreeExtensionSP& nom)
		:SVM_Valeur::ExceptionExecution(SVM_Valeur::Interruption::InterruptionInterne::ECHEC)
		{
			if(static_cast<bool>(nom))
			{
				DETAILS_EXCEPTION("Cannot detach already detached kernel to " << (*nom) << " sequencer.");
			}
			else
			{
				DETAILS_EXCEPTION("Cannot detach already detached kernel to default sequencer.");
			}
		}
		virtual ~NoyauDejaDetache() {}
	};

}
}
}
}

