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

#include <src/machine/elements/sequenceur/sequenceur.h>
#include <src/machine/elements/sequenceur/sequenceur_defaut.h>
#include <src/machine/elements/sequenceur/sequenceur_extension.h>
#include <src/machine/debugueur/debugueur.h>

using namespace SVM::Machine::Element::Sequenceur;

SequenceurSP Sequenceur::creation_sequenceur(SVM_Debugueur::DebugueurSP& debugueur, const SVM_Valeur::PointEntreeExtensionSP& nom, const SVM_Processus::ProcessusSP& processus)
{
	SequenceurSP s;
	if(not nom)
	{
		s = std::make_shared<SequenceurDefaut>(processus);
	}
	else
	{
		s = std::make_shared<SequenceurExtension>(nom,processus);
	}
	s->piege() = SVM_Debugueur::Debugueur::ajoute_piege<PiegeSequenceur>(debugueur,s);
	return s;
}

SVM_Debugueur::JSON_Interne::ObjetSP PiegeSequenceur::mise_a_jour()
{
	auto s = _sequenceur.lock();
	if(not s)
	{
		throw SVM_Debugueur::PasDeMiseAJour();
	}
	std::string nom;
	std::string description;
	std::ostringstream oss;
	s->nom_flux(oss);
	nom = oss.str();
	oss.str("");
	oss << (*s);
	description = oss.str();
	return SVM_Debugueur::JSON::objet({ { "name" , SVM_Debugueur::JSON::chaine(nom) }, { "state" , SVM_Debugueur::JSON::chaine(description) } });
}
