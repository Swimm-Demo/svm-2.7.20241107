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

#include <src/demarrage/lanceur/analyseur/analyseur.h>
#define YY_BUF_SIZE 16*1024
using namespace SVM::Demarrage::Lanceur;

struct yy_buffer_state;

void demarragelanceuranalyseurlex_init(void **);
void demarragelanceuranalyseurlex_destroy(void *);
yy_buffer_state* demarragelanceuranalyseur_scan_bytes(char const*, unsigned long, void*);
yy_buffer_state* demarragelanceuranalyseur_create_buffer(FILE*, int, void *scanner); // pour parser un fichier
void demarragelanceuranalyseur_switch_to_buffer(yy_buffer_state*, void *scanner); // pour parser un fichier
void demarragelanceuranalyseur_delete_buffer(yy_buffer_state *tampon, void *scanner);
int demarragelanceuranalyseurparse(void *scanner, SVM::Demarrage::Lanceur::LanceurSP& lanceur);

struct Memoire_demarragelanceuranalyseur
{
	Memoire_demarragelanceuranalyseur()
	:tampon(nullptr), scanner(nullptr),fichier(nullptr) {}
	~Memoire_demarragelanceuranalyseur()
	{
		if(tampon)
			::demarragelanceuranalyseur_delete_buffer(tampon,scanner);
		if(scanner)
			::demarragelanceuranalyseurlex_destroy(scanner);
		if(fichier)
		{
			if(fichier!=stdin)
			{
				::fclose(fichier);
			}
		}
	}
	yy_buffer_state *tampon;
	void *scanner;
	FILE *fichier;
};

SVM::Demarrage::Lanceur::LanceurSP Analyseur::analyse(const std::string& source)
{
	SVM::Demarrage::Lanceur::LanceurSP lanceur(new SVM::Demarrage::Lanceur::Lanceur(source));
	Memoire_demarragelanceuranalyseur memoire;
	::demarragelanceuranalyseurlex_init(&memoire.scanner);
	if(not SVM::Global::Fichier::teste_fichier(source))
	{
		throw Erreur() << "File " << source << " can not be opened or read";
	}
	memoire.fichier = SVM::Global::Fichier::ouvrir(source);
	memoire.tampon = ::demarragelanceuranalyseur_create_buffer(memoire.fichier,YY_BUF_SIZE,memoire.scanner);
	::demarragelanceuranalyseur_switch_to_buffer(memoire.tampon,memoire.scanner);
	if(::demarragelanceuranalyseurparse(memoire.scanner,lanceur))
	{
		throw lanceur->erreur();
	}
	lanceur->decoration();
	return lanceur;
}

