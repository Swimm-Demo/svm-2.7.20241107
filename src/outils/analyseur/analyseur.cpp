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

#include <src/outils/analyseur/analyseur.h>
#define YY_BUF_SIZE 16*1024

using namespace SVM::Outils;

struct yy_buffer_state;

void outilssvmpluginanalyseurlex_init(void **);
void outilssvmpluginanalyseurlex_destroy(void *);
yy_buffer_state* outilssvmpluginanalyseur_scan_buffer(char *, size_t, void*);
yy_buffer_state* outilssvmpluginanalyseur_create_buffer(FILE*, int, void *scanner); // pour parser un fichier
void outilssvmpluginanalyseur_switch_to_buffer(yy_buffer_state*, void *scanner); // pour parser un fichier
void outilssvmpluginanalyseur_delete_buffer(yy_buffer_state *tampon, void *scanner);
int outilssvmpluginanalyseurparse(void *scanner, SVM::Outils::ExtensionSP& extension, SVM::Outils::ErreurSP& erreur);

struct Memoire_outilssvmpluginanalyseur
{
	Memoire_outilssvmpluginanalyseur()
	:source(nullptr), tampon(nullptr), scanner(nullptr),fichier(nullptr) {}
	~Memoire_outilssvmpluginanalyseur()
	{
		if(source) delete [] source;
		if(tampon)
			::outilssvmpluginanalyseur_delete_buffer(tampon,scanner);
		if(scanner)
			::outilssvmpluginanalyseurlex_destroy(scanner);
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

SVM::Outils::ExtensionSP Analyseur::analyse_chaine(const std::string& source)
{
	SVM::Outils::ExtensionSP extension = std::make_shared<SVM::Outils::Extension>();
	SVM::Outils::ErreurSP erreur;
	Memoire_outilssvmpluginanalyseur memoire;
	::outilssvmpluginanalyseurlex_init(&memoire.scanner);
	memoire.source = new char[source.size()+2];
	::memcpy(memoire.source,source.c_str(),source.size());
	memoire.source[source.size()] = memoire.source[source.size()+1] = '\0';
	memoire.tampon = ::outilssvmpluginanalyseur_scan_buffer(memoire.source,source.size()+2,memoire.scanner);
	if(::outilssvmpluginanalyseurparse(memoire.scanner,extension,erreur))
	{
		throw *erreur;
	}
	return extension;
}

SVM::Outils::ExtensionSP Analyseur::analyse_fichier(const std::string& source)
{
	SVM::Outils::ExtensionSP extension = std::make_shared<SVM::Outils::Extension>();
	SVM::Outils::ErreurSP erreur;
	Memoire_outilssvmpluginanalyseur memoire;
	::outilssvmpluginanalyseurlex_init(&memoire.scanner);
	if(not SVM::Global::Fichier::teste_fichier(source))
	{
		std::ostringstream oss;
		oss << "File " << source << " can not be opened or read"; 
		throw SVM::Outils::Erreur(oss.str());
	}
	memoire.fichier = SVM::Global::Fichier::ouvrir(source);
	memoire.tampon = ::outilssvmpluginanalyseur_create_buffer(memoire.fichier,YY_BUF_SIZE,memoire.scanner);
	::outilssvmpluginanalyseur_switch_to_buffer(memoire.tampon,memoire.scanner);
	if(::outilssvmpluginanalyseurparse(memoire.scanner,extension,erreur))
	{
		throw *erreur;
	}
	return extension;
}
