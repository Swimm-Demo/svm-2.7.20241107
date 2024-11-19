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
#include <src/machine/elements/sequenceur/sequenceur.h>

namespace SVM
{
namespace Machine
{
namespace Element
{
namespace Sequenceur
{
	struct SequenceurDefaut : public Sequenceur
	{
		SequenceurDefaut(const SVM_Processus::ProcessusSP& processus)
		:Sequenceur(SVM_Valeur::PointEntreeExtensionSP(),processus)
		{
		}
		virtual ~SequenceurDefaut() {}
		virtual SVM_Noyau::NoyauSP noyau_courant() override
		{
			if(not _noyau)
				return SVM_Noyau::NoyauSP();
			if(not _noyau->termine())
			{
				return _noyau;
			}
			return SVM_Noyau::NoyauSP();
		}
		virtual bool attache_noyau(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& env_parent, const size_t argc, SVM_Parameter *argv) override;
		virtual bool detache_noyau(const SVM_Noyau::NoyauSP& noyau, SVM_Interface::EnvironnementSP& env_parent, const size_t argc, SVM_Parameter *argv) override;
		virtual void affiche(std::ostream& os) const override
		{
			if(static_cast<bool>(_noyau))
			{
				os << "  Kernel ";
				_noyau->trace_etat(os);
			}
		}
		virtual void* interne() override { return nullptr; }
		private:
			SVM_Noyau::NoyauSP _noyau;
	};
}
}
}
}
