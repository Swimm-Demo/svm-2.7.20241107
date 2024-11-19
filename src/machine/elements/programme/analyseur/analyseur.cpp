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

#include <src/machine/elements/programme/analyseur/analyseur.h>
#include <src/machine/machine/machine.h>
#include <src/machine/debugueur/debugueur.h>
#define YY_BUF_SIZE 16*1024

using namespace SVM::Machine::Element::Programme;

struct yy_buffer_state;

void machineelementsprogrammeanalyseurlex_init(void **);
void machineelementsprogrammeanalyseurlex_destroy(void *);
yy_buffer_state* machineelementsprogrammeanalyseur_scan_buffer(char *, size_t, void*);
yy_buffer_state* machineelementsprogrammeanalyseur_create_buffer(FILE*, int, void *scanner); // pour parser un fichier
void machineelementsprogrammeanalyseur_switch_to_buffer(yy_buffer_state*, void *scanner); // pour parser un fichier
void machineelementsprogrammeanalyseur_delete_buffer(yy_buffer_state *tampon, void *scanner);
int machineelementsprogrammeanalyseurparse(void *scanner, const std::string& nom, SVM_Valeur::CodeSP& code, SVM_Valeur::InterruptionSP& erreur, const SVM_Valeur::PositionInstructionSP& position);

struct Memoire_machineelementsprogrammeanalyseur
{
	Memoire_machineelementsprogrammeanalyseur()
	:source(nullptr), tampon(nullptr), scanner(nullptr),fichier(nullptr) {}
	~Memoire_machineelementsprogrammeanalyseur()
	{
		if(source) delete [] source;
		if(tampon)
			::machineelementsprogrammeanalyseur_delete_buffer(tampon,scanner);
		if(scanner)
			::machineelementsprogrammeanalyseurlex_destroy(scanner);
		if(fichier)
		{
			if(fichier!=stdin)
			{
				::fclose(fichier);
			}
		}
	}
	char *source;
	yy_buffer_state *tampon;
	void *scanner;
	FILE *fichier;
};

SVM_Valeur::CodeSP Analyseur::analyse_chaine(const std::string& nom, const std::string& source, const SVM::Machine::MachineSP& machine, const SVM_Valeur::PositionInstructionSP& position)
{
	auto code = std::make_shared<SVM_Valeur::Code>(nom,source,"");
	SVM_Valeur::InterruptionSP interruption;
	Memoire_machineelementsprogrammeanalyseur memoire;
	::machineelementsprogrammeanalyseurlex_init(&memoire.scanner);
	memoire.source = new char[source.size()+2];
	::memcpy(memoire.source,source.c_str(),source.size());
	memoire.source[source.size()] = memoire.source[source.size()+1] = '\0';
	memoire.tampon = ::machineelementsprogrammeanalyseur_scan_buffer(memoire.source,source.size()+2,memoire.scanner);
	if(::machineelementsprogrammeanalyseurparse(memoire.scanner,nom,code,interruption,position))
	{
		throw *interruption;
	}
	code->decoration(machine);
	code->_piege = SVM::Machine::Debugueur::Debugueur::ajoute_piege<SVM_Valeur::PiegeCode>(machine->_base->_debugueur,code);
	return code;
}

SVM_Valeur::CodeSP Analyseur::analyse_fichier(const std::string& nom, const std::string& source, const SVM::Machine::MachineSP& machine, const SVM_Valeur::PositionInstructionSP& position)
{
	auto code = std::make_shared<SVM_Valeur::Code>(nom,"",source);
	SVM_Valeur::InterruptionSP interruption;
	Memoire_machineelementsprogrammeanalyseur memoire;
	::machineelementsprogrammeanalyseurlex_init(&memoire.scanner);
	if(not SVM::Global::Fichier::teste_fichier(source))
	{
		std::ostringstream oss;
		oss << "File " << source << " can not be opened or read"; 
		interruption=std::make_shared<SVM_Valeur::Interruption>(SVM_Valeur::Interruption::InterruptionInterne::ECHEC,oss.str());
		throw *interruption;
	}
	memoire.fichier = SVM::Global::Fichier::ouvrir(source);
	memoire.tampon = ::machineelementsprogrammeanalyseur_create_buffer(memoire.fichier,YY_BUF_SIZE,memoire.scanner);
	::machineelementsprogrammeanalyseur_switch_to_buffer(memoire.tampon,memoire.scanner);
	if(::machineelementsprogrammeanalyseurparse(memoire.scanner,nom,code,interruption,position))
	{
		throw *interruption;
	}
	code->decoration(machine);
	code->_piege = SVM::Machine::Debugueur::Debugueur::ajoute_piege<SVM_Valeur::PiegeCode>(machine->_base->_debugueur,code);
	return code;
}
