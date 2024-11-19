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

#include <src/machine/debugueur/analyseur/analyseur.h>
#define YY_BUF_SIZE 16*1024
using namespace SVM::Machine::Debugueur;

struct yy_buffer_state;

void machinedebugueuranalyseurlex_init(void **);
void machinedebugueuranalyseurlex_destroy(void *);
yy_buffer_state* machinedebugueuranalyseur_scan_bytes(char const*, unsigned long, void*);
yy_buffer_state* machinedebugueuranalyseur_scan_buffer(char *, size_t, void*);
yy_buffer_state* machinedebugueuranalyseur_create_buffer(FILE*, int, void *scanner); // pour parser un fichier
void machinedebugueuranalyseur_switch_to_buffer(yy_buffer_state*, void *scanner); // pour parser un fichier
void machinedebugueuranalyseur_delete_buffer(yy_buffer_state *tampon, void *scanner);
int machinedebugueuranalyseurparse(void *scanner, SVM::Machine::Debugueur::JSON_Interne::ValeurSP& valeur);

struct Memoire_machinedebugueuranalyseur
{
	Memoire_machinedebugueuranalyseur()
	:source(nullptr), tampon(nullptr), scanner(nullptr),fichier(nullptr) {}
	~Memoire_machinedebugueuranalyseur()
	{
		if(source) delete [] source;
		if(tampon)
			::machinedebugueuranalyseur_delete_buffer(tampon,scanner);
		if(scanner)
			::machinedebugueuranalyseurlex_destroy(scanner);
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

SVM::Machine::Debugueur::JSON_Interne::ValeurSP Analyseur::analyse(const std::string& source)
{
	SVM::Machine::Debugueur::JSON_Interne::ValeurSP valeur;
	Memoire_machinedebugueuranalyseur memoire;
	::machinedebugueuranalyseurlex_init(&memoire.scanner);
	memoire.source = new char[source.size()+2];
	::memcpy(memoire.source,source.c_str(),source.size());
	memoire.source[source.size()] = memoire.source[source.size()+1] = '\0';
	memoire.tampon = ::machinedebugueuranalyseur_scan_buffer(memoire.source,source.size()+2,memoire.scanner);
	if(::machinedebugueuranalyseurparse(memoire.scanner,valeur))
	{
		return SVM::Machine::Debugueur::JSON_Interne::ValeurSP();
	}
	return valeur;
}

