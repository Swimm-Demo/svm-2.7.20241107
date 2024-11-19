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

#include <string.h>
#include <stdlib.h>
#include <src/global/global.h>

#define FICHIER_STDIN "-"

using namespace SVM::Global;

std::string Chaine::echappe(const std::string& chaine)
{
	std::ostringstream resultat;
	for(auto it=chaine.cbegin() ; it!=chaine.cend() ; ++it)
	{
		switch(*it)
		{
			case '\n':
				resultat << "\\n";
				break;
			case '\r':
				resultat << "\\r";
				break;
			case '\t':
				resultat << "\\t";
				break;
			case '\\':
			case '"':
				resultat << "\\" << (*it);
				break;
			default:
				if((((*it)>=' ') and ((*it)<='~')) or (((*it)>='\xA1') and ((*it)<='\xFF')))
				{
					resultat << (*it);
				}
				else
				{
					unsigned char caractere = *it;
					char buffer[10];
					::snprintf(buffer,3,"%02X",static_cast<unsigned int>(caractere));
					resultat << "\\0" << buffer ;
				}
				break;
		}
	}
	return resultat.str();
}

std::string Chaine::desechappe(const std::string& chaine)
{
	std::ostringstream resultat;
	bool echappe = false;
	for(auto it = chaine.cbegin() ; it!=chaine.cend() ; ++it)
	{
		if(echappe)
		{
			switch(*it)
			{

				case 'n':
					resultat << '\n';
					break;
				case 'r':
					resultat << '\r';
					break;
				case 't':
					resultat << '\t';
					break;
				case '\\':
				case '"':
					resultat << (*it);
					break;
				case '0':
					{
						std::stringstream caractere;
						++it;
						if(it==chaine.cend())
						{
							resultat << "\\0" << caractere.str();
							return resultat.str();
						}
						caractere << (*it);
						++it;
						if(it==chaine.cend())
						{
							resultat << "\\0" << caractere.str();
							return resultat.str();
						}
						caractere << (*it);
						long int entier;
						if(not (caractere >> std::hex >> entier))
						{
							resultat << "\\0" << caractere.str();
							break;
						}
						unsigned char caracterediscret = entier;
						resultat << caracterediscret;
					}
					break;
				default:
					resultat << "\\";
					resultat << (*it);
					break;
			}
			echappe=false;
		}
		else
		{
			if((*it)=='\\')
				echappe=true;
			else
				resultat << (*it);
		}
	}
	return resultat.str();
}

char *Chaine::duplique(const char *chaine, const size_t taille)
{
	char *retour = static_cast<char*>(::malloc(sizeof(char)*taille));
	::memcpy(retour,chaine,taille);
	return retour;
}

std::string Chaine::echappe_html(const std::string& chaine)
{
	std::string s;
	for(const auto& c: chaine)
	{
		switch(c)
		{
			case '<':
				s += "&lt;";
				break;
			case '>':
				s += "&gt;";
				break;
			default:
				s += c;
				break;
		}
	}
	return s;
}

bool Fichier::teste_fichier(std::string fichier)
{
	if(fichier=="-")
		return true;
	if(::access(fichier.c_str(),R_OK))
		return false;
	struct stat buf;
	if(::stat(fichier.c_str(),&buf))
		return false;
	if(S_ISDIR(buf.st_mode))
		return false;
	return true;
}

FILE *Fichier::ouvrir(std::string fichier)
{
	ASSERT(not fichier.empty(),"fichier vide en entree");
	if(fichier==FICHIER_STDIN)
	{
		return stdin;
	}

	if(not teste_fichier(fichier)) // changer ce test pour eviter les timings attacks
		throw /* exception fichier invalide */;

	FILE *fic = ::fopen(fichier.c_str(),"r");
	return fic;
}
