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

#include <src/machine/debugueur/chiffre.h>

using namespace SVM::Machine::Debugueur;

Chiffre::Chiffre(const std::string& clef_primaire, const std::vector<std::pair<std::string, std::string> >& challenges)
{
	//SVM_TRACE("Clef principale : " << Securite::encode_hex(clef_primaire));
	for(const auto& c: challenges)
	{
		//SVM_TRACE((Securite::encode_hex(clef_primaire)+Securite::encode_hex(c.first)) << " / " << (Securite::encode_hex(clef_primaire)+Securite::encode_hex(c.second)));
		_clefs_secondaires.push_back(Securite::hashe_sha1(Securite::encode_hex(clef_primaire)+Securite::encode_hex(c.first))+Securite::hashe_sha1(Securite::encode_hex(clef_primaire)+Securite::encode_hex(c.second)));
	}
}

#define ALEATOIRE(max) (::rand()%max)
//#define ALEATOIRE(max) 0

std::string Chiffre::chiffre(const std::string& source)
{
	std::string resultat = source;
	for(const auto& c: _clefs_secondaires)
	{
		//SVM_TRACE("Clef secondaire : " << Securite::encode_hex(c));
		std::string tour;
		bool remplissage = false;
		size_t taille_remplissage = 0;
		uint64_t increment = 0;
		for(unsigned char j = 0 ; j<8 ; ++j)
		{
			increment <<= 8;
			increment += static_cast<unsigned char>(static_cast<unsigned char>(c[16+j]));
			//SVM_TRACE((16+j) << " / " << static_cast<int>(static_cast<unsigned char>(static_cast<unsigned char>(c[16+j]))));
		}
		//SVM_TRACE(increment);
		unsigned char masque[8];
		for(unsigned char i=0 ; i<8 ; ++i)
		{
			masque[i] = static_cast<unsigned char>(c[24+i]);
		}
		for(size_t i = 0 ; ; i+=32)
		{
			unsigned char matrice[32];
			bool fin = false;
			for(size_t j=0 ; j<32 ; ++j)
			{
				if((i+j)<resultat.size())
				{
					matrice[j] = resultat[i+j];
				}
				else
				{
					matrice[j] = ALEATOIRE(256);
					if(not remplissage)
					{
						taille_remplissage = 32-j;
						//SVM_TRACE("Taille remplissage = " << taille_remplissage);
						remplissage = true;
						if(j<(32-5)) fin=true;
					}
					else
					{
						if(j==0) fin=true;
					}
					if(fin and j>=(32-5))
					{
						unsigned char bit = (taille_remplissage >> (31-j)) bitand 0x1;
						unsigned char parite = 0;
						unsigned char element = matrice[j];
						while(element)
						{
							parite ^= (element&0x1);
							element >>= 1;
						}
						if(bit xor parite)
						{
							matrice[j] ^= 1 << ALEATOIRE(8);
						}
						//SVM_TRACE("Taille tronque = " << taille_remplissage << ", bit " << (31-j) << " = " << (char)('0'+bit) << ", parite = " << (char)('0'+parite) << ", changement = " << (bit xor parite) << ", matrice = " << Securite::encode_hex(std::string(1,matrice[j])));
					}
				}
			}
			//SVM_TRACE("Bloc " << Securite::encode_hex(std::string(reinterpret_cast<const char*>(matrice),32)));
			// premiere passe : rotations
			for(unsigned char j = 0 ; j<16 ; ++j)
			{
				unsigned char cc = static_cast<unsigned char>(c[j]) bitand 0xF;
				uint16_t ligne = (matrice[2*j] << 8) + (matrice[2*j+1]);
				uint16_t ligne_decalee = Chiffre::decalage_circulaire(cc,ligne);
				matrice[2*j] = ligne_decalee>>8;
				matrice[2*j+1] = ligne_decalee bitand 0xFF;
			}
			//SVM_TRACE("Bloc passe 1/2 " << Securite::encode_hex(std::string(reinterpret_cast<const char*>(matrice),32)));
			Chiffre::transpose(matrice,16);
			for(unsigned char j = 0 ; j<16 ; ++j)
			{
				unsigned char cc = (static_cast<unsigned char>(c[j])>>4) bitand 0xF;
				uint16_t ligne = (matrice[2*j] << 8) + (matrice[2*j+1]);
				uint16_t ligne_decalee = Chiffre::decalage_circulaire(cc,ligne);
				matrice[2*j] = ligne_decalee>>8;
				matrice[2*j+1] = ligne_decalee bitand 0xFF;
			}
			Chiffre::transpose(matrice,16);
			//SVM_TRACE("Bloc passe 1 " << Securite::encode_hex(std::string(reinterpret_cast<const char*>(matrice),32)));
			// seconde passe : addition
			for(unsigned char j = 0 ; j<32 ; j+=8)
			{
				uint64_t valeur = 0;
				for(unsigned char k = 0 ; k<8 ; ++k)
				{
					valeur <<= 8;
					valeur += matrice[j+k];
				}
				valeur += increment;
				for(unsigned char k = 0 ; k<8 ; ++k)
				{
					matrice[j+k] = valeur bitand 0xFF;
					valeur >>= 8;
				}
			}
			//SVM_TRACE("Bloc passe 2 " << Securite::encode_hex(std::string(reinterpret_cast<const char*>(matrice),32)));
			// troisieme passe : masquage
			Chiffre::masquage(matrice,masque,c);
			//SVM_TRACE("Bloc passe 3 " << Securite::encode_hex(std::string(reinterpret_cast<const char*>(matrice),32)));
			// ajout
			tour += std::string(reinterpret_cast<const char*>(matrice),32);
			if(fin) break;
		}
		resultat.swap(tour);
	}
	return resultat;
}

std::string Chiffre::dechiffre(const std::string& source)
{
	std::string resultat = source;
	for(auto it = _clefs_secondaires.rbegin() ; it!=_clefs_secondaires.rend(); ++it)
	{
		std::string tour;
		uint64_t increment = 0;
		for(unsigned char j = 0 ; j<8 ; ++j)
		{
			increment <<= 8;
			increment += static_cast<unsigned char>((*it)[16+j]);
		}
		unsigned char masque[8];
		for(unsigned char i=0 ; i<8 ; ++i)
		{
			masque[i] = static_cast<unsigned char>((*it)[24+i]);
		}
		for(size_t i = 0 ; i<resultat.size() ; i+=32)
		{
			unsigned char matrice[32];
			for(size_t j=0 ; j<32 ; ++j)
			{
				if(i+j>resultat.size())
					return std::string();
				matrice[j] = resultat[i+j];
			}
			// premiere passe : demasquage
			Chiffre::masquage(matrice,masque,*it);
			// seconde passe : soustraction
			//SVM_TRACE("Bloc avant sous " << Securite::encode_hex(std::string(reinterpret_cast<const char*>(matrice),32)));
			for(unsigned char j = 0 ; j<32 ; j+=8)
			{
				uint64_t valeur = 0;
				for(unsigned char k = 0 ; k<8 ; ++k)
				{
					valeur <<= 8;
					valeur += matrice[j+7-k];
				}
				valeur -= increment;
				for(unsigned char k = 0 ; k<8 ; ++k)
				{
					matrice[j+7-k] = valeur bitand 0xFF;
					valeur >>= 8;
				}
			}
			//SVM_TRACE("Bloc apres sous " << Securite::encode_hex(std::string(reinterpret_cast<const char*>(matrice),32)));
			// troisieme passe : derotations
			Chiffre::transpose(matrice,16);
			for(unsigned char j = 0 ; j<16 ; ++j)
			{
				unsigned char cc = (static_cast<unsigned char>((*it)[j])>>4) bitand 0xF;
				uint16_t ligne = (matrice[2*j] << 8) + (matrice[2*j+1]);
				uint16_t ligne_decalee = Chiffre::decalage_circulaire(16-cc,ligne);
				matrice[2*j] = ligne_decalee>>8;
				matrice[2*j+1] = ligne_decalee bitand 0xFF;
			}
			//SVM_TRACE("Bloc " << Securite::encode_hex(std::string(reinterpret_cast<const char*>(matrice),32)));
			Chiffre::transpose(matrice,16);
			for(unsigned char j = 0 ; j<16 ; ++j)
			{
				unsigned char cc = static_cast<unsigned char>((*it)[j]) bitand 0xF;
				uint16_t ligne = (matrice[2*j] << 8) + (matrice[2*j+1]);
				uint16_t ligne_decalee = Chiffre::decalage_circulaire(16-cc,ligne);
				matrice[2*j] = ligne_decalee>>8;
				matrice[2*j+1] = ligne_decalee bitand 0xFF;
			}
			//SVM_TRACE("Bloc " << Securite::encode_hex(std::string(reinterpret_cast<const char*>(matrice),32)));
			tour += std::string(reinterpret_cast<const char*>(matrice),32);
		}
		size_t taille = 0;
		for(size_t i=0 ; i<5 ; ++i)
		{
			size_t parite = 0;
			unsigned char element = static_cast<unsigned char>(tour[tour.size()-5+i]);
			//SVM_TRACE("matrice = " << Securite::encode_hex(std::string(1,element)));
			while(element)
			{
				parite ^= (element&0x1);
				element >>= 1;
			}
			taille <<= 1;
			taille += parite;
		}
		if(taille<=5)
		{
			taille += 32;
		}
		if(taille>tour.size())
			return std::string();
		//SVM_TRACE("Taille tronque = " << taille);
		resultat = tour.substr(0,tour.size()-taille);
	}
	return resultat;
}

void Chiffre::transpose(unsigned char *matrice, size_t taille)
{
	for(unsigned char l=0 ; l<taille ; ++l)
	{
		for(unsigned char c=l+1 ; c<taille ; ++c)
		{
			size_t indice_octet_normal = l*(taille/8) + c/8;
			size_t indice_bit_normal = 7-c%8;
			unsigned char bit_normal = (matrice[indice_octet_normal] >> indice_bit_normal) bitand 0x1;
			size_t indice_octet_miroir = c*(taille/8) + l/8;
			size_t indice_bit_miroir = 7-l%8;
			unsigned char bit_miroir = (matrice[indice_octet_miroir] >> indice_bit_miroir) bitand 0x1;
			////SVM_TRACE("Transpose l=" << static_cast<int>(l) << ", c=" << static_cast<int>(c) << " => normal=(" << indice_octet_normal << "/" << indice_bit_normal << "), miroir=(" << indice_octet_miroir << "/" << indice_bit_miroir << ")");
			if(bit_normal)
			{
				matrice[indice_octet_miroir] |= 1 << indice_bit_miroir;
			}
			else
			{
				matrice[indice_octet_miroir] &= ~ (1 << indice_bit_miroir);
			}
			if(bit_miroir)
			{
				matrice[indice_octet_normal] |= 1 << indice_bit_normal;
			}
			else
			{
				matrice[indice_octet_normal] &= ~ (1 << indice_bit_normal);
			}
		}
	}
}

void Chiffre::masquage(unsigned char matrice[32], unsigned char masque[8], const std::string& clef)
{
	for(unsigned char j = 0 ; j<32 ; j+=8)
	{
		for(unsigned char k = 0 ; k<8 ; ++k)
		{
			unsigned char cc = ((clef[32] >> k bitand 0x1) << 2) + ((clef[33] >> k bitand 0x1) << 1) + ((clef[34] >> k bitand 0x1));
			masque[k] = Chiffre::decalage_circulaire(cc,masque[k]);
		}
		Chiffre::transpose(masque,8);
		for(unsigned char k = 0 ; k<8 ; ++k)
		{
			unsigned char cc = ((clef[35] >> k bitand 0x1) << 2) + ((clef[36] >> k bitand 0x1) << 1) + ((clef[37] >> k bitand 0x1));
			masque[k] = Chiffre::decalage_circulaire(cc,masque[k]);
		}
		Chiffre::transpose(masque,8);
		unsigned char a = clef[38];
		unsigned char b = clef[39];
		for(unsigned char k = 0 ; k<8 ; k+=2)
		{
			masque[k] ^= a;
			masque[k+1] ^= b;
			uint16_t valeur = (a << 8) + (b bitand 0xFF);
			valeur = Chiffre::decalage_circulaire(4,valeur);
			a = valeur >> 8;
			b = valeur bitand 0xFF;
		}
		//SVM_TRACE("Masque: " << Securite::encode_hex(std::string(reinterpret_cast<const char*>(masque),8))); 
		for(unsigned char k = 0 ; k<8 ; ++k)
		{
			matrice[j+k] ^= masque[k];
		}
	}
}
