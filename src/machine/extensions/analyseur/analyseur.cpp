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

#include <src/machine/extensions/analyseur/analyseur.h>

using namespace SVM::Machine::Extension;

struct yy_buffer_state;

void machineextensionsanalyseurlex_init(void **);
void machineextensionsanalyseurlex_destroy(void *);
yy_buffer_state* machineextensionsanalyseur_scan_buffer(char *, size_t, void*);
yy_buffer_state* machineextensionsanalyseur_create_buffer(FILE*, void*, void *scanner); // pour parser un fichier
void machineextensionsanalyseur_delete_buffer(yy_buffer_state *tampon, void *scanner);
int machineextensionsanalyseurparse(void *scanner, const std::string& nom, SVM_Extension::DescriptionSP& description);

struct Memoire_machineextensionsanalyseur
{
	Memoire_machineextensionsanalyseur()
	:source(nullptr), tampon(nullptr), scanner(nullptr) {}
	~Memoire_machineextensionsanalyseur()
	{
		if(source) delete [] source;
		::machineextensionsanalyseur_delete_buffer(tampon,scanner);
		::machineextensionsanalyseurlex_destroy(scanner);
	}
	char *source;
	yy_buffer_state *tampon;
	void *scanner;
};

struct ConfigurationInvalide : public SVM_Valeur::ExceptionDemarrage
{
	ConfigurationInvalide(const size_t ligne, const std::string& message)
	{
		DETAILS_EXCEPTION("Line " << ligne << ": " << message);
	}
	virtual ~ConfigurationInvalide() {}
};

SVM_Extensions::DescriptionSP Analyseur::analyse(const std::string& nom, const std::string& source)
{
	SVM_Extensions::DescriptionSP description(new SVM_Extensions::Description(nom,source));
	Memoire_machineextensionsanalyseur memoire;
	::machineextensionsanalyseurlex_init(&memoire.scanner);
	std::string s(source.c_str(), source.size());
	memoire.source = new char[source.size()+2];
	::memcpy(memoire.source,source.c_str(),source.size());
	memoire.source[source.size()] = memoire.source[source.size()+1] = '\0';
	memoire.tampon = ::machineextensionsanalyseur_scan_buffer(memoire.source,source.size()+2,memoire.scanner);
	if(::machineextensionsanalyseurparse(memoire.scanner,nom,description))
	{
		throw ConfigurationInvalide(description->_ligne,description->_message);
	}
	description->verification();
	return description;
}

